@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: OBSERVAÇÃO
::
:: Esta biblioteca devolve exatamente o conteúdo encontrado no Escopo.
::
:: Não aplica nomenclaturas.
:: Não interpreta IDs.
:: Não modifica valores.
::
:: Qualquer tratamento deve ser realizado pelo FSM_CONTEXTO.
:: ============================================================================
::
::     set "ARQUIVO_ESCOPO=C:\Projeto\FSM.md"
::
::     call LEITOR_ESCOPO.bat SISTEMA
::     call LEITOR_ESCOPO.bat NAMESPACE_PPRINCIPAL
::     call LEITOR_ESCOPOP.bat NAMESPACE_SUBCATEGORIA
::     call LEITOR_ESCOPO.bat ENUM
::     call LEITOR_ESCOPO.bat VARIAVEL
::     call LEITOR_ESCOPO.bat ESTADOS
::     call LEITOR_ESCOPO.bat ANTERIOR_ESTADOS
::
:: Saída:
::
::     EXISTE
::     VALOR
::     TOTAL_ITENS
::     ITEM1
::     ITEM2
::     ITEM3...
::
:: ============================================================================


:: ============================================================================
:: VALIDAÇÕES
:: ============================================================================

if "%~1"=="" (
    echo.
    echo [ERRO] Bloco nao informado.
    exit /b 1
)

if not defined ARQUIVO_ESCOPO (
    echo.
    echo [ERRO] Variavel ARQUIVO_ESCOPO nao definida.
    exit /b 1
)

if not exist "%ARQUIVO_ESCOPO%" (
    echo.
    echo [ERRO] Arquivo nao encontrado:
    echo %ARQUIVO_ESCOPO%
    exit /b 1
)

:: ============================================================================
:: PARÂMETROS
:: ============================================================================

set "BLOCO=%~1"
set "PREFIXO="

if /I "%BLOCO:~0,10%"=="ANTERIOR_" (
    set "PREFIXO=ANTERIOR_"
    set "BLOCO=%BLOCO:~10%"
)

set "TAG_INICIO=[INICIO_%PREFIXO%%BLOCO%]"
set "TAG_FIM=[FIM_%PREFIXO%%BLOCO%]"

:: ============================================================================
:: INICIALIZAÇÃO
:: ============================================================================

set "EXISTE=0"
set "VALOR="
set "TOTAL_ITENS=0"

set "LENDO=0"

for /f "delims==" %%V in ('set ITEM 2^>nul') do (
    set "%%V="
)

:: ============================================================================
:: LEITURA
:: ============================================================================

for /f "usebackq delims=" %%L in ("%ARQUIVO_ESCOPO%") do (

    set "LINHA=%%L"

    if /I "!LINHA!"=="!TAG_INICIO!" (

        set "LENDO=1"

    ) else if /I "!LINHA!"=="!TAG_FIM!" (

        set "LENDO=0"

    ) else if "!LENDO!"=="1" (

        if not "!LINHA!"=="" (

          set /a TOTAL_ITENS+=1
          set "ITEM!TOTAL_ITENS!=!LINHA!"
          echo [INFO] Item !TOTAL_ITENS! encontrado: !LINHA!

)

    )

)

:: ============================================================================
:: PREPARA RETORNO
:: ============================================================================

if !TOTAL_ITENS! GTR 0 (

    set "EXISTE=1"
    set "VALOR=!ITEM1!"

)

:: ============================================================================
:: EXPORTAÇÃO
:: ============================================================================

set "RET_EXISTE=!EXISTE!"
set "RET_VALOR=!VALOR!"
set "RET_TOTAL=!TOTAL_ITENS!"

(
    echo set "EXISTE=%RET_EXISTE%"
    echo set "VALOR=%RET_VALOR%"
    echo set "TOTAL_ITENS=%RET_TOTAL%"

    for /L %%i in (1,1,!TOTAL_ITENS!) do (
        echo set "ITEM%%i=!ITEM%%i!"
    )

) > "%TEMP%\LEITOR_EXPORT.bat"


endlocal

call "%TEMP%\LEITOR_EXPORT.bat"

del "%TEMP%\LEITOR_EXPORT.bat"

exit /b 0