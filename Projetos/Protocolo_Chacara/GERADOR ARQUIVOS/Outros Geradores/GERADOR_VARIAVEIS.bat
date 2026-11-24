@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion


:: =========================================
:: GERADOR DE VARIÁVEIS ARQUIVO TEMPORARIO
:: =========================================
set "BASE=%~dp0"
set "TMP_VARIAVEL=%BASE%tmp_variaveis.txt"
set "TMP_VARIAVEL_OUT=%BASE%tmp_variaveis_out.txt"

:: =========================================
:: SELECIONAR PASTA
:: =========================================
set count=0
echo =================================
echo   SELECIONE UMA PASTA
echo =================================

for /d %%i in (*) do (
    set /a count+=1
    set "folder!count!=%%i"
    echo !count! - %%i
)

echo.
set /p selPasta=Numero da pasta: 
if not defined folder%selPasta% (
    echo [ERRO] Pasta invalida
    pause
    exit /b
)

cd "!folder%selPasta%!"

:: =========================================
:: LISTAR ARQUIVOS
:: =========================================
set count=0
echo ================================
echo   ARQUIVOS
echo ================================

for %%f in (*.cpp) do (
    set /a count+=1
    set "file!count!=%%f"
    echo !count! - %%f
)

echo.
set /p selCPP=Arquivo .cpp: 

set "ARQ_CPP=!file%selCPP%!"

:: Captura o caminho absoluto do arquivo para não perder a referência no cd /d
for %%i in ("!ARQ_CPP!") do set "ARQ_CPP_FULL=%%~fi"

if not exist "%ARQ_CPP%" (
    echo [ERRO] Arquivo .cpp nao encontrado
    pause
    exit /b
)

findstr /C:"// [BAT:VARIAVEIS]" "%ARQ_CPP%" >nul || (
    echo [ERRO] Marcador nao existe no .cpp
    pause
    exit /b
)

:: ============================================================
:: GERADOR_VARIAVEIS.bat
:: ============================================================

:LOOP

cls

echo.
echo ┌──────────────────────────────────────┐
echo │        GERADOR DE VARIÁVEIS          │
echo ├──────────────────────────────────────┤
echo │                                      │
echo │  [MODIFICADORES]                     │
echo │                                      │
echo │   [s]   → static                     │
echo │   [c]   → const                      │
echo │   [cx]  → constexpr                  │
echo │   [f]   → static constexpr [FIXA]    │
echo │                                      │
echo │  [TIPOS]                             │
echo │                                      │
echo │   [8]   → uint8_t                    │
echo │   [16]  → uint16_t                   │
echo │   [32]  → uint32_t                   │
echo │   [b]   → bool                       │
echo │   [ch]  → char                       │
echo │                                      │
echo ├──────────────────────────────────────┤
echo │  Exemplo:                            │
echo │                                      │
echo │  s c 8 contador 0                    │
echo │  → static const uint8_t contador     │
echo │    = 0;                              │
echo │                                      │
echo │  0 = SAIR                            │
echo └──────────────────────────────────────┘
echo.

set "INPUT="
set /p INPUT=Variavel: 

if "!INPUT!"=="0" goto PROCESSAR

if "!INPUT!"=="" goto LOOP

:: =========================================
:: RESET
:: =========================================
set "PREFIXO="
set "TIPO="
set "NOME="
set "VALOR="

:: =========================================
:: CONTAR TOKENS
:: =========================================
set /a TOTAL=0

for %%T in (!INPUT!) do (
    set /a TOTAL+=1
)

:: =========================================
:: ANALISADOR DE TOKENS
:: =========================================
set /a INDEX=0

for %%T in (!INPUT!) do (

    set /a INDEX+=1

    set "TOKEN=%%T"
    set "PROCESSADO=0"

    if "!INDEX!"=="!TOTAL!" (

        set "VALOR=!TOKEN!"
        set "PROCESSADO=1"

    )

    if "!PROCESSADO!"=="0" (

        if /i "!TOKEN!"=="s" (
            set "PREFIXO=!PREFIXO!static "
            set "PROCESSADO=1"
        )

        if /i "!TOKEN!"=="c" (
            set "PREFIXO=!PREFIXO!const "
            set "PROCESSADO=1"
        )

        if /i "!TOKEN!"=="cx" (
            set "PREFIXO=!PREFIXO!constexpr "
            set "PROCESSADO=1"
        )

        if /i "!TOKEN!"=="f" (
            set "PREFIXO=!PREFIXO!static constexpr "
            set "PROCESSADO=1"
        )
    )

    if "!PROCESSADO!"=="0" (

        if "!TOKEN!"=="8" (
            set "TIPO=uint8_t"
            set "PROCESSADO=1"
        )

        if "!TOKEN!"=="16" (
            set "TIPO=uint16_t"
            set "PROCESSADO=1"
        )

        if "!TOKEN!"=="32" (
            set "TIPO=uint32_t"
            set "PROCESSADO=1"
        )

        if /i "!TOKEN!"=="b" (
            set "TIPO=bool"
            set "PROCESSADO=1"
        )

        if /i "!TOKEN!"=="ch" (
            set "TIPO=char"
            set "PROCESSADO=1"
        )

    )

    if "!PROCESSADO!"=="0" (

        if "!NOME!"=="" (
            set "NOME=!TOKEN!"
            set "PROCESSADO=1"
        ) else (
            set "NOME=!NOME! !TOKEN!"
        )

    )

    if "!PROCESSADO!"=="0" (
        if "!VALOR!"=="!TOKEN!" (
            set "VALOR=!TOKEN!"
        ) else (
            set "VALOR=!VALOR! !TOKEN!"
        )
    )
)

:: =========================================
:: DEBUG
:: =========================================

echo.
echo ============================
echo PREFIXO = !PREFIXO!
echo TIPO    = !TIPO!
echo NOME    = !NOME!
echo VALOR   = !VALOR!
echo ============================
echo.

:: =========================================
:: NORMALIZAR NOME
:: =========================================

echo !PREFIXO! | findstr /i "constexpr" >nul

if not errorlevel 1 (

    call "%BASE%NORMALIZAR.bat" "!NOME!" UPPER
    set "NOME_FINAL=!NORM_OUT!"

) else (

    call "%BASE%NORMALIZAR.bat" "!NOME!" CAMEL
    set "NOME_FINAL=!NORM_OUT!"

)

:: =========================================
:: RESULTADO FINAL
:: =========================================

echo.
echo ============================
echo RESULTADO
echo ============================
echo.

echo !PREFIXO!!TIPO! !NOME_FINAL! = !VALOR!; >> "%TMP_VARIAVEL%"
echo.

goto LOOP

:: =========================================
:: PROCESSAMENTO E INJEÇÃO FINAL
:: =========================================
:PROCESSAR
cls
echo [AGUARDE] Processando tempos e injetando no código...

:: Volta para a base para garantir que o TRATA_TEMPO encontre os arquivos
cd /d "%BASE%"

if not exist "tmp_variaveis.txt" (
    echo [ERRO] Nenhuma variável foi gerada para processar.
    pause
    exit /b
)

:: Chama o script de conversão de h, m, s (HORAS, MINUTOS, SEGUNDOS)
call "%BASE%TRATA_TEMPO.bat"

:: Agora injetamos o resultado convertido no arquivo .cpp
set "ARQ_CONVERTIDO=%BASE%tmp_variaveis_convertido.txt"
set "OUT_TMP=%ARQ_CPP_FULL%.tmp"

if not exist "%ARQ_CONVERTIDO%" set "ARQ_CONVERTIDO=%BASE%tmp_variaveis.txt"

set "ACHOU=0"

:: O findstr /n garante que até as linhas vazias tragam o número da linha, impedindo o loop de ignorá-las
for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_CPP_FULL%"') do (
    set "linha=%%B"
    
    set "TESTE=!linha!"
    if defined TESTE for /f "tokens=*" %%X in ("!TESTE!") do set "TESTE=%%X"

    if "!TESTE!"=="// [BAT:VARIAVEIS]" (
        type "%ARQ_CONVERTIDO%" >> "%OUT_TMP%"
        echo(!linha!>> "%OUT_TMP%"
        set "ACHOU=1"
    ) else (
        if "%%B"== "" (echo(>> "%OUT_TMP%") else (echo(!linha!>> "%OUT_TMP%")
    )
)

if !ACHOU!==0 (
    echo [ERRO] Marcador nao encontrado no .cpp
    pause
) else (
    move /y "%OUT_TMP%" "%ARQ_CPP_FULL%" >nul
    echo [OK] .cpp atualizado
)

:: Substitui o original e limpa a sujeira
del "%BASE%tmp_variaveis.txt"
if exist "%BASE%tmp_variaveis_convertido.txt" del "%BASE%tmp_variaveis_convertido.txt"

echo.
echo ===================================================
echo [SUCESSO] Variáveis injetadas em: %ARQ_CPP%
echo ===================================================
echo.
pause
exit /b