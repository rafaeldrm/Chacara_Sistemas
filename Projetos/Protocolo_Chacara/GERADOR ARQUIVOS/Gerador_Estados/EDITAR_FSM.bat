@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: =========================================================================
:: EDITA_FSM.bat - Arquitetura por Blocos Isolados (Versão Corrigida)
:: =========================================================================

if not exist "%ARQUIVO_SESSAO%" (

    echo.
    echo [ERRO] Sessao nao encontrada.
    echo Execute primeiro o FSMa.bat
    echo.

    pause
    exit /b

)

:: Lê o arquivo de sessão e carrega as variáveis para o escopo local
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"



echo =============================================================
echo  ESCOLHA SEU SISTEMA A EDITAR UMA MÁQUINA DE ESTADOS EXISTENTE
echo =============================================================
echo.
echo.

for %%F in ("%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_*.h") do (
    set "ARQ_NOME=%%~nF"
    set "SISTEMA_NOME=!ARQ_NOME:Estados_Sistema_=!"
    set /a CONTADOR+=1
    set "SISTEMA_!CONTADOR!=!SISTEMA_NOME!"
    echo  !CONTADOR! - !SISTEMA_NOME!
)

echo.
echo.
set "ESCOLHA_SISTEMA="
set /p ESCOLHA_SISTEMA=Digite o número do sistema desejado: 
echo.


if not defined SISTEMA_%ESCOLHA_SISTEMA% (
    echo [ERRO] Opção inválida!
    pause
    exit /b
)

set "SISTEMA_ATUAL=!SISTEMA_%ESCOLHA_SISTEMA%!"
set "PASTA_SISTEMA=%DIRETORIO_ESTADOS%\!SISTEMA_ATUAL!"

cls
set "CONTADOR_TXT=0"

echo =================================================
echo LISTA DE FSM's DISPONÍVEIS PARA EDIÇÃO
echo =================================================

for %%T in ("!PASTA_SISTEMA!\*.md") do (
    set /a CONTADOR_TXT+=1
    set "TXT_NAME=%%~nT"
    set "ED_!CONTADOR_TXT!=!TXT_NAME!"
    echo !CONTADOR_TXT! - !TXT_NAME!
)

if !CONTADOR_TXT! EQU 0 (
    echo [AVISO] Nenhuma FSM configurada para o sistema !SISTEMA_ATUAL!.
    echo Use a opção 2 para criar uma nova FSM primeiro.
    pause
    exit /b
)
set "ESCOLHA_FSM="
set /p ESCOLHA_FSM=Digite o número do arquivo FSM que deseja editar:

if not defined ED_%ESCOLHA_FSM% (
    echo [ERRO] Opção inválida!
    pause
    exit /b
)

set "NAMESPACE=!ED_%ESCOLHA_FSM%!"
set "ARQUIVO_ESCOPO=!PASTA_SISTEMA!\!NAMESPACE!.md"
echo ARQUIVO_ESCOPO = %ARQUIVO_ESCOPO%

call "%DIRETORIO_BIBLIOTECAS%\GERADOR_ID.bat"

call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" PREPARAR_HISTORICO_EDICAO

:: Carrega sessão inicial
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"
set "ARQ_H=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_%SISTEMA%.h"
set "ARQ_CPP=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_%SISTEMA%.cpp"
set "ARQ_ALVO=%PASTA_SISTEMA%\%NAMESPACE%.md"

call code --wait "!ARQ_ALVO!" >nul

call "%NOMENCLATURA_BAT%" "%NAMESPACE%" UPPER
set "TAG_NAMESPACE=%TEXTO_TRATADO%"

echo ====================================================
echo  INICIANDO PIPELINE DE ATUALIZAÇÃO DA FSM: %TAG_NAMESPACE%
echo ====================================================

:: Limpeza preventiva de lixos temporários de execuções anteriores antes de começar
set "TMP_H=%BASE%temp\_fsm_h.tmp"
set "TMP_CPP=%BASE%temp\_fsm_cpp.tmp"
if exist "%TMP_H%" del "%TMP_H%"
if exist "%TMP_CPP%" del "%TMP_CPP%"
:: -------------------------------------------------------------------------
:: DISPACHO CRONOLÓGICO DOS BLOCOS (Execução Cadenciada)
:: -------------------------------------------------------------------------
call :BLOCO_1_LENDO_ESCOPO
echo Bloco 1 finalizado
if errorlevel 1 exit /b

call :BLOCO_2_GERAR_TEMPORARIOS
echo Bloco 2 finalizado
if errorlevel 1 exit /b

call :BLOCO_3_PURGAR_ARQUIVOS
echo Bloco 3 finalizado
if errorlevel 1 exit /b

call :BLOCO_4_INJETAR_H
echo Bloco 4 finalizado
if errorlevel 1 exit /b

call :BLOCO_5_INJETAR_CPP
echo Bloco 5 finalizado
if errorlevel 1 exit /b

call :BLOCO_7_LIMPEZA_FINAL
echo Bloco 7 finalizado
if errorlevel 1 exit /b

echo.
echo ====================================================
echo [SUCESSO] Pipeline finalizado! FSM atualizada.
echo ====================================================
echo.
exit /b


:: =========================================================================
:: BLOCO 1: PARSE DO TXT DE ESCOPO
:: =========================================================================
:BLOCO_1_LENDO_ESCOPO
echo [1/5] Executando Bloco: Leitura do arquivo de escopo...

set "ARQUIVO_ESCOPO=%ARQ_ALVO%"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" NAMESPACE
set "NAMESPACE_LIDO=%VALOR%"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ENUM
set "ENUM_LIDO=%VALOR%"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" VARIAVEL
set "VARIAVEL_LIDO=%VALOR%"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ESTADOS
set "TOTAL_ESTADOS=%VALOR%"

:: =========================================================================
:: BLOCO 2: GERAR BLOCOS ISOLADOS NOS TEMPORÁRIOS
:: =========================================================================
:BLOCO_2_GERAR_TEMPORARIOS
echo [2/5] Executando Bloco: Gerando arquivos estruturais limpos (.tmp)...
set "TEMPLATE_ESTADOS=%BASE%Estrutura_Estados.md"
set "TMP_H=%BASE%temp\_fsm_h.tmp"
set "TMP_CPP=%BASE%temp\_fsm_cpp.tmp"

set "ALVO="
for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%TEMPLATE_ESTADOS%"') do (
    set "L=%%B" & set "C=!L!"
    if defined C for /f "tokens=*" %%X in ("!C!") do set "C=%%X"
    
    if "!C!"=="[FILE:Estados_Sistema_{{SISTEMA}}.h]" ( set "ALVO=H" ) ^
    else if "!C!"=="[FILE:Estados_Sistema_{{SISTEMA}}.cpp]" ( set "ALVO=CPP" ) ^
    else if "!C!"=="[END_FILE]" ( set "ALVO=" ) ^
    else if defined ALVO (
        if defined L (
            set "L=!L:{{SISTEMA}}=%SISTEMA%!"
            set "L=!L:{{NAMESPACE}}=%NAMESPACE_LIDO%!"
            set "L=!L:{{ENUM}}=%ENUM_LIDO%!"
            set "L=!L:{{VARIAVEL}}=%VARIAVEL_LIDO%!"
        )
        if "!ALVO!"=="H" (
            if defined L (
                echo !L! | findstr /c:"{{ESTADOS}}" >nul && (
                    for /L %%i in (1,1,!TOTAL_ESTADOS!) do echo        !ESTADO%%i!,>> "%TMP_H%"
                ) || ( echo(!L!>> "%TMP_H%" )
            ) else ( echo(>> "%TMP_H%" )
        )
        if "!ALVO!"=="CPP" (
            if defined L (
                echo !L! | findstr /c:"{{CASES}}" >nul && (
                    for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                        set "TEXTO_CASE=            case !ENUM_LIDO!::!ESTADO%%i!:"
                        set "ESPACOS=                                                                                      "
                        set "CASE_COMPLETA=!TEXTO_CASE!!ESPACOS!"
                        set "CASE_ALINHADA=!CASE_COMPLETA:~0,70!"
                        echo !CASE_ALINHADA!return F^("!ESTADO%%i!"^);>> "%TMP_CPP%"
                    )
                ) || ( echo(!L!>> "%TMP_CPP%" )
            ) else ( echo(>> "%TMP_CPP%" )
        )
    )
)
exit /b 0


:: =========================================================================
:: BLOCO 3: PURGA COMPLETA E ISOLADA DOS BLOCOS ANTIGOS
:: =========================================================================
:BLOCO_3_PURGAR_ARQUIVOS
echo [3/5] Executando Bloco: Expurgando arquivos reais e cravando Linker...

:: Purga do .h
set "START_H=0" & set "END_H=0"
for /f "delims=:" %%A in ('findstr /n /c:"// [INICIO::BAT::%TAG_NAMESPACE%]" "%ARQ_H%"') do if !START_H! equ 0 set "START_H=%%A"
for /f "delims=:" %%A in ('findstr /n /c:"// [FIM::BAT::%TAG_NAMESPACE%]" "%ARQ_H%"') do set "END_H=%%A"

if !START_H! GTR 0 (
    if exist "%ARQ_H%.purged" del "%ARQ_H%.purged"
    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_H%"') do (
        set "NUM=%%A" & set "LN=%%B"
        if !NUM! LSS !START_H! ( if "%%B"== "" ( echo(>> "%ARQ_H%.purged" ) else ( echo(!LN!>> "%ARQ_H%.purged" ) ) ^
        else if !NUM! EQU !START_H! ( echo // [BAT::ATUALIZA]>> "%ARQ_H%.purged" ) ^
        else if !NUM! GTR !END_H! ( if "%%B"== "" ( echo(>> "%ARQ_H%.purged" ) else ( echo(!LN!>> "%ARQ_H%.purged" ) )
    )
    move /y "%ARQ_H%.purged" "%ARQ_H%" >nul
)

:: Purga do .cpp (Mantendo regras estáveis de Struct/Static In-Place)
set "START_CPP=0" & set "END_CPP=0"
for /f "delims=:" %%A in ('findstr /n /c:"// [INICIO::BAT::%TAG_NAMESPACE%]" "%ARQ_CPP%"') do if !START_CPP! equ 0 set "START_CPP=%%A"
for /f "delims=:" %%A in ('findstr /n /c:"// [FIM::BAT::%TAG_NAMESPACE%]" "%ARQ_CPP%"') do set "END_CPP=%%A"

if !START_CPP! GTR 0 (
    if exist "%ARQ_CPP%.purged" del "%ARQ_CPP%.purged"
    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_CPP%"') do (
        set "NUM=%%A" & set "LN=%%B" & set "TEST=%%B"
        if defined TEST for /f "tokens=*" %%X in ("!TEST!") do set "TEST=%%X"
        set "PROCESSED=0"
        
        if !NUM! LSS !START_CPP! (
            if defined TEST (
                echo !TEST! | findstr /c:"// [VAR::%TAG_NAMESPACE%]" >nul && (
                    echo      %NAMESPACE_LIDO%::%ENUM_LIDO% %VARIAVEL_LIDO%; // [VAR::%NAMESPACE_LIDO%]>> "%ARQ_CPP%.purged"
                    set "PROCESSED=1"
                )
                echo !TEST! | findstr /c:"// [INIT::%TAG_NAMESPACE%]" >nul && (
                    echo !TEST! | findstr /c:"// [STATIC::VIRGULA]" >nul && (
                        echo      %NAMESPACE_LIDO%::%ENUM_LIDO%::%PRIMEIRO_ESTADO%// [STATIC::VIRGULA] // [INIT::%NAMESPACE_LIDO%]>> "%ARQ_CPP%.purged"
                    ) || (
                        echo      %NAMESPACE_LIDO%::%ENUM_LIDO%::%PRIMEIRO_ESTADO%, // [INIT::%NAMESPACE_LIDO%]>> "%ARQ_CPP%.purged"
                    )
                    set "PROCESSED=1"
                )
            )
            if "!PROCESSED!"=="0" ( if "%%B"== "" ( echo(>> "%ARQ_CPP%.purged" ) else ( echo(!LN!>> "%ARQ_CPP%.purged" ) )
        ) else if !NUM! EQU !START_CPP! (
            echo // [BAT::ATUALIZA]>> "%ARQ_CPP%.purged"
        ) else if !NUM! GTR !END_CPP! (
            if "%%B"== "" ( echo(>> "%ARQ_CPP%.purged" ) else ( echo(!LN!>> "%ARQ_CPP%.purged" )
        )
    )
    move /y "%ARQ_CPP%.purged" "%ARQ_CPP%" >nul
)
exit /b 0


:: =========================================================================
:: BLOCO 4: INJEÇÃO ISOLADA DO ARQUIVO .h (COM FILTRO DE LINHAS VAZIAS)
:: =========================================================================
:BLOCO_4_INJETAR_H
echo [4/5] Executando Bloco: Injetando nova FSM no arquivo .h...
set "TMP_H=%BASE%temp\_fsm_h.tmp"

if exist "%ARQ_H%.new" del "%ARQ_H%.new"

set "ULTIMA_LINHA_VAZIA=0"

for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_H%"') do (
    set "LN=%%B" & set "TEST=%%B"
    
    if defined TEST (
        :: Se a linha tem conteúdo, limpa o sinalizador de linha vazia
        set "ULTIMA_LINHA_VAZIA=0"
        
        echo !TEST! | findstr /c:"[BAT::ATUALIZA]" >nul && (
            if exist "%TMP_H%" type "%TMP_H%">> "%ARQ_H%.new"
        ) || (
            echo(!LN!>> "%ARQ_H%.new"
        )
    ) else (
        :: Se a linha está vazia, só escreve se a anterior NÃO for vazia (evita acúmulo)
        if "!ULTIMA_LINHA_VAZIA!"=="0" (
            echo(>> "%ARQ_H%.new"
            set "ULTIMA_LINHA_VAZIA=1"
        )
    )
)
move /y "%ARQ_H%.new" "%ARQ_H%" >nul
exit /b 0


:: =========================================================================
:: BLOCO 5: INJEÇÃO ISOLADA DO ARQUIVO .cpp (COM FILTRO DE LINHAS VAZIAS)
:: =========================================================================
:BLOCO_5_INJETAR_CPP
echo [5/5] Executando Bloco: Injetando nova FSM no arquivo .cpp...
set "TMP_CPP=%BASE%temp\_fsm_cpp.tmp"

if exist "%ARQ_CPP%.new" del "%ARQ_CPP%.new"

set "ULTIMA_LINHA_VAZIA=0"

for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_CPP%"') do (
    set "LN=%%B" & set "TEST=%%B"
    
    if defined TEST (
        set "ULTIMA_LINHA_VAZIA=0"
        
        echo !TEST! | findstr /c:"[BAT::ATUALIZA]" >nul && (
            if exist "%TMP_CPP%" type "%TMP_CPP%">> "%ARQ_CPP%.new"
        ) || (
            echo(!LN!>> "%ARQ_CPP%.new"
        )
    ) else (
        if "!ULTIMA_LINHA_VAZIA!"=="0" (
            echo(>> "%ARQ_CPP%.new"
            set "ULTIMA_LINHA_VAZIA=1"
        )
    )
)
move /y "%ARQ_CPP%.new" "%ARQ_CPP%" >nul
exit /b 0

:: =========================================================================
:: BLOCO 7: HIGIENIZAÇÃO E LIMPEZA DE EXCESSO DE LINHAS VAZIAS (ARQUIVO TODO)
:: =========================================================================
:BLOCO_7_LIMPEZA_FINAL
REM echo [6/6] Executando Bloco: Higienizando arquivos e removendo linhas vazias em excesso...

:: Trata o arquivo .h
REM if exist "%ARQ_H%" (
REM     if exist "%ARQ_H%.clean" del "%ARQ_H%.clean"
REM     set "ULTIMA_FOI_VAZIA=0"
    
REM     for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_H%"') do (
REM         set "LN=%%B"
REM         set "TEST=%%B"
        
REM         if defined TEST (
            :: Remove espaços e Tabs para testar se a linha é puramente fantasma
REM             set "TEST=!TEST: =!"
REM             set "TEST=!TEST:	=!"
REM         )
        
REM         if "!TEST!"=="" (
REM             :: A linha está verdadeiramente vazia ou só tinha espaços/tabs
REM             if "!ULTIMA_FOI_VAZIA!"=="0" (
REM                 echo(>> "%ARQ_H%.clean"
REM                 set "ULTIMA_FOI_VAZIA=1"
REM             )
REM         ) else (
            :: A linha tem código real (ex: chaves indentadas, códigos, etc)
REM            echo(!LN!>> "%ARQ_H%.clean"
REM            set "ULTIMA_FOI_VAZIA=0"
REM        )
REM    )
REM    move /y "%ARQ_H%.clean" "%ARQ_H%" >nul
REM )

:: Trata o arquivo .cpp
REM if exist "%ARQ_CPP%" (
REM    if exist "%ARQ_CPP%.clean" del "%ARQ_CPP%.clean"
REM    set "ULTIMA_FOI_VAZIA=0"
    
REM    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_CPP%"') do (
REM        set "LN=%%B"
REM        set "TEST=%%B"
        
REM        if defined TEST (
REM            set "TEST=!TEST: =!"
REM            set "TEST=!TEST:	=!"
REM        )
        
REM       if "!TEST!"=="" (
REM            if "!ULTIMA_FOI_VAZIA!"=="0" (
REM                echo(>> "%ARQ_CPP%.clean"
REM                set "ULTIMA_FOI_VAZIA=1"
REM            )
REM        ) else (
REM            echo(!LN!>> "%ARQ_CPP%.clean"
REM            set "ULTIMA_FOI_VAZIA=0"
REM        )
REM    )
REM    move /y "%ARQ_CPP%.clean" "%ARQ_CPP%" >nul
REM )

call "%BASE%ATUALIZA_ARQUIVOS.bat"
exit /b 0