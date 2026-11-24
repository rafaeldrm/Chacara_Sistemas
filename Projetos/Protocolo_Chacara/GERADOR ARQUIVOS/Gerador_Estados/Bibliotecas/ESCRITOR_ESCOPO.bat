@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: ESCRITOR_ESCOPO.BAT
::
:: Biblioteca oficial para escrita do Escopo.md
::
:: RESPONSABILIDADE
::
::     Atualizar completamente um bloco do arquivo.
::
:: NÃO faz:
::
::     • leitura
::     • comparação
::     • sincronização
::     • geração de IDs
::     • validação semântica
::
:: Apenas substitui o conteúdo existente entre:
::
::     [INICIO_xxx]
::     [FIM_xxx]
::
:: ============================================================================
::
:: Entrada
::
::     set "ARQUIVO_ESCOPO=C:\Projeto\CASA_TORRE.md"
::
:: Valores simples:
::
::     call ESCRITOR_ESCOPO.bat SISTEMA Eco_Hidro
::     call ESCRITOR_ESCOPO.bat NAMESPACE CASA_TORRE
::     call ESCRITOR_ESCOPO.bat ENUM ETAPAS
::     call ESCRITOR_ESCOPO.bat VARIAVEL estadoCasa
::
:: Lista:
::
::     ITEM1
::     ITEM2
::     ITEM3
::     TOTAL_ITENS
::
::     call ESCRITOR_ESCOPO.bat ESTADOS
::
:: Também aceita:
::
::     ANTERIOR_NAMESPACE
::     ANTERIOR_ENUM
::     ANTERIOR_VARIAVEL
::     ANTERIOR_ESTADOS
::
:: ============================================================================


:: ============================================================================
:: VERIFICAÇÕES
:: ============================================================================

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

if "%~1"=="" (
    echo.
    echo [ERRO] Bloco nao informado.
    exit /b 1
)


:: ============================================================================
:: PARÂMETROS
:: ============================================================================

set "BLOCO=%~1"
set "NOVO_VALOR=%~2"

set "PREFIXO="

if /I "%BLOCO:~0,10%"=="ANTERIOR_" (
    set "PREFIXO=ANTERIOR_"
    set "BLOCO=%BLOCO:~10%"
)


:: ============================================================================
:: TAGS
:: ============================================================================

set "TAG_INICIO=[INICIO_%PREFIXO%%BLOCO%]"
set "TAG_FIM=[FIM_%PREFIXO%%BLOCO%]"

set "ARQUIVO_TEMP=%ARQUIVO_ESCOPO%.tmp"

set "DENTRO_BLOCO=0"


:: ============================================================================
:: REESCREVE O ARQUIVO
:: ============================================================================

(

for /f "usebackq delims=" %%L in ("%ARQUIVO_ESCOPO%") do (

    set "LINHA=%%L"

    if /I "!LINHA!"=="!TAG_INICIO!" (

        echo(!LINHA!

        set "DENTRO_BLOCO=1"

        if /I "%BLOCO%"=="ESTADOS" (

            for /L %%i in (1,1,!TOTAL_ITENS!) do (

                echo(!ITEM%%i!

            )

        ) else (

            if defined NOVO_VALOR (

                echo(!NOVO_VALOR!

            )

        )

    ) else if /I "!LINHA!"=="!TAG_FIM!" (

        set "DENTRO_BLOCO=0"

        echo(!LINHA!

    ) else (

        if "!DENTRO_BLOCO!"=="0" (

            echo(!LINHA!

        )

    )

)

)> "%ARQUIVO_TEMP%"


move /Y "%ARQUIVO_TEMP%" "%ARQUIVO_ESCOPO%" >nul

exit /b 0