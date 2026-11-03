@echo off
:: ================================================================
::  IF_ELSE.bat  -  Construtor de blocos if / else com Parser
:: ================================================================

:: Chars especiais - ANTES do setlocal
set "OP_NEQ=!="
set "OP_EQQ==="
set "OP_AND=&&"
set "OP_OR=||"

chcp 65001 >nul
setlocal enabledelayedexpansion

:: CONFIGURAÇÃO: Altere para o arquivo que deseja ler as variáveis
set "FONTE=meu_modulo.cpp"

echo.
echo ================================================================
echo           CONSTRUTOR IF / ELSE (INTELLISENSE)
echo ================================================================

:: 1. PROXIMO NUMERO
if not exist tmp mkdir tmp
set "ULTIMO=0"
for %%F in (tmp\*.txt) do (
    for /f "tokens=1 delims=_" %%N in ("%%~nF") do (
        if %%N gtr !ULTIMO! set "ULTIMO=%%N"
    )
)
set /a "PROXIMO=!ULTIMO! + 1"
set "ARQ=tmp\!PROXIMO!_ifelse.txt"

:: 2. PARSER DE VARIÁVEIS DO ARQUIVO FONTE
echo  Lendo variáveis de: %FONTE%
set "COUNT_VAR=0"
if exist "%FONTE%" (
    :: Procura por tipos comuns de Arduino/C++
    for /f "tokens=1,2 delims=;= " %%A in ('findstr /R "uint8_t uint16_t uint32_t bool char int float" "%FONTE%"') do (
        set "TIPO_OK=0"
        for %%T in (uint8_t uint16_t uint32_t bool char int float) do (
            if "%%A"=="%%T" set "TIPO_OK=1"
        )
        
        if "!TIPO_OK!"=="1" (
            set "VAR_NOME=%%B"
            :: Evita duplicados na listagem
            set "EXISTE=0"
            for /l %%i in (1,1,!COUNT_VAR!) do (
                if "!VAR_LIST[%%i]!"=="!VAR_NOME!" set "EXISTE=1"
            )
            if "!EXISTE!"=="0" (
                set /a COUNT_VAR+=1
                set "VAR_LIST[!COUNT_VAR!]=!VAR_NOME!"
            )
        )
    )
)

:: 3. EXIBE VARIÁVEIS NUMERADAS
echo  ---- Variáveis Detectadas ----
if !COUNT_VAR! gtr 0 (
    for /l %%v in (1,1,!COUNT_VAR!) do (
        echo    [%%v] !VAR_LIST[%%v]!
    )
) else (
    echo    (Nenhuma variável encontrada em %FONTE%)
)
echo.

:: 4. OPERADORES
echo  Operadores: MAIOR_QUE_IGUAL (>=), IGUAL (==), DIFERENTE (!=), E (&&), OU (||)
echo  Exemplo: 1 MAIOR_QUE_IGUAL 100 E 2 IGUAL true
echo.

:: 5. CAPTURA E TRADUZ
set /p "INPUT_COND=Condicao: "
call :TRADUZ "!INPUT_COND!"

echo.
echo  Preview:  if (!COND_TRAD!) {
echo.

echo if (!COND_TRAD!) {> "!ARQ!"

:: 6. CORPO DO IF
echo  ---- Corpo do IF ---- [0] para encerrar
call :CORPO "!ARQ!"

:: 7. MENU ELSE
:MENU_ELSE
echo.
echo  [1] else if   [2] else   [0] fechar bloco
set /p "OPT=Opcao: "

if "!OPT!"=="1" (
    set /p "INPUT_EI=Condicao do else if: "
    call :TRADUZ "!INPUT_EI!"
    echo } else if (!COND_TRAD!) {>> "!ARQ!"
    echo  ---- Corpo do ELSE IF ----
    call :CORPO "!ARQ!"
    goto MENU_ELSE
)

if "!OPT!"=="2" (
    echo } else {>> "!ARQ!"
    echo  ---- Corpo do ELSE ----
    call :CORPO "!ARQ!"
    goto FIM_BLOCO
)

:FIM_BLOCO
echo }>> "!ARQ!"

:: 8. RESULTADO
echo.
echo ================================================================
echo  Gerado com sucesso em: !ARQ!
type "!ARQ!"
echo.
pause
exit /b 0

:: ================================================================
:TRADUZ
set "COND_TRAD="
set "_RAW=%~1"
for %%T in (!_RAW!) do (
    set "_T=%%T"
    set "_R=%%T"
    
    :: Se for um número, tenta trocar pelo nome da variável correspondente
    for /l %%n in (1,1,!COUNT_VAR!) do (
        if "%%T"=="%%n" set "_R=!VAR_LIST[%%n]!"
    )

    if /i "!_T!"=="MAIOR_QUE_IGUAL"  set "_R=>="
    if /i "!_T!"=="MENOR_QUE_IGUAL"  set "_R=<="
    if /i "!_T!"=="MAIOR_QUE"        set "_R=>"
    if /i "!_T!"=="MENOR_QUE"        set "_R=<"
    if /i "!_T!"=="IGUAL"            set "_R=!OP_EQQ!"
    if /i "!_T!"=="DIFERENTE"        set "_R=!OP_NEQ!"
    if /i "!_T!"=="E"                set "_R=!OP_AND!"
    if /i "!_T!"=="OU"               set "_R=!OP_OR!"

    if "!COND_TRAD!"=="" (set "COND_TRAD=!_R!") else (set "COND_TRAD=!COND_TRAD! !_R!")
)
exit /b

:: ================================================================
:CORPO
:_CORPO_LOOP
set "_L="
set /p "_L=    > "
if "!_L!"=="0" exit /b
if "!_L!"=="" goto _CORPO_LOOP

set "_PREF=!_L:~0,2!"
set "_LAST=!_L:~-1!"

if "!_PREF!"=="//" (
    echo     !_L!>> "%~1"
) else if "!_LAST!"==";" (
    echo     !_L!>> "%~1"
) else (
    :: Adiciona ponto e virgula apenas se não tiver
    echo     !_L!;>> "%~1"
)
goto _CORPO_LOOP