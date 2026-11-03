@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================
:: ATUALIZA_ARQUIVOS.bat
:: Renderizador de Templates FSM
:: ============================================================

set "BASE=%~dp0"
set "ARQUIVO_SESSAO=%BASE%sessao.tmp"

if not exist "%ARQUIVO_SESSAO%" (
    echo [ERRO] Sessao nao encontrada.
    pause
    exit /b
)

for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do (
    set "%%A"
)

set "ARQ_FSM=%PASTA_SISTEMA%\%NAMESPACE%.txt"

echo.
echo ============================================================
echo Atualizando arquivos vinculados a FSM: %NAMESPACE%
echo ============================================================
echo.

:: ============================================================
:: CARREGA ENUM
:: ============================================================

set "TAG_ENUM="

for /f "usebackq tokens=1,* delims==" %%A in ("%ARQ_FSM%") do (

    if /i "%%A"=="ENUM" (

        for /f "tokens=*" %%X in ("%%B") do (

            call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "%%X" UPPER

            set "TAG_ENUM=!NORM_OUT!"
        )
    )
)

:: ============================================================
:: CARREGA IDS
:: ============================================================

echo [INFO] Carregando estados...

set "DENTRO_ESTADOS=0"

for /f "usebackq delims=" %%L in ("%ARQ_FSM%") do (

    set "LN=%%L"

    if defined LN (
        for /f "tokens=*" %%X in ("!LN!") do set "LN=%%X"
    )

    if /i "!LN!"=="[ESTADOS]" (
        set "DENTRO_ESTADOS=1"
    )

    if /i "!LN!"=="[END_ESTADOS]" (
        set "DENTRO_ESTADOS=0"
    )

    if "!DENTRO_ESTADOS!"=="1" (

        echo !LN! | findstr /r "^[0-9][0-9]*=" >nul

        if not errorlevel 1 (

            for /f "tokens=1,2 delims==" %%I in ("!LN!") do (

                set "ID=%%I"

                call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "%%J" UPPER

                set "ESTADO_ID_%%I=!NORM_OUT!"

                echo   ID %%I = !NORM_OUT!
            )
        )
    )
)

echo.

:: ============================================================
:: PROCESSA ARQUIVOS
:: ============================================================

set "DENTRO_ARQUIVOS=0"

for /f "usebackq delims=" %%A in ("%ARQ_FSM%") do (

    set "LN=%%A"

    if defined LN (
        for /f "tokens=*" %%X in ("!LN!") do set "LN=%%X"
    )

    if /i "!LN!"=="[ARQUIVOS]" (
        set "DENTRO_ARQUIVOS=1"
    )

    if /i "!LN!"=="[END_ARQUIVOS]" (
        set "DENTRO_ARQUIVOS=0"
    )

    if "!DENTRO_ARQUIVOS!"=="1" (

        if not "!LN!"=="" if /i not "!LN!"=="[ARQUIVOS]" (

            for /f "tokens=1,2 delims=|" %%F in ("!LN!") do (

                set "PROJETO=%%F"
                set "ARQUIVO_REL=%%G"

                set "ARQUIVO_CPP=%DIRETORIO_PROJETOS%\!PROJETO!\lib\!ARQUIVO_REL!"

                if exist "!ARQUIVO_CPP!" (

                    echo [PROCESSANDO]
                    echo !ARQUIVO_CPP!

                    call :PROCESSAR_ARQUIVO "!ARQUIVO_CPP!"
                )
                else (

                    echo [AVISO] Nao encontrado:
                    echo !ARQUIVO_CPP!
                )
            )
        )
    )
)

echo.
echo ============================================================
echo Atualizacao concluida
echo ============================================================
exit /b

:: ############################################################
:: PROCESSAR ARQUIVO
:: ############################################################

:PROCESSAR_ARQUIVO

set "ALVO=%~1"
set "TMP=%ALVO%.tmp"

if exist "%TMP%" del "%TMP%"

for /f "usebackq delims=" %%L in ("%ALVO%") do (

    set "LINHA=%%L"

    call :PROCESSAR_LINHA

    echo(!LINHA_SAIDA!>> "%TMP%"
)

move /y "%TMP%" "%ALVO%" >nul

exit /b

:: ############################################################
:: PROCESSAR LINHA
:: ############################################################

:PROCESSAR_LINHA

set "LINHA_SAIDA=!LINHA!"

echo(!LINHA! | findstr /c:"[BAT$" >nul

if errorlevel 1 exit /b

:: ------------------------------------------------------------
:: Extrai TAG
:: ------------------------------------------------------------

for /f "tokens=2 delims=[" %%A in ("!LINHA!") do (
    set "TAG=%%A"
)

:: Exemplo:
:: BAT$CASA_TORRE$FUNCAO$1]

for /f "tokens=1,2,3,4 delims=$]" %%A in ("!TAG!") do (

    set "MARCA=%%A"
    set "TAG_NAMESPACE=%%B"
    set "TAG_TIPO=%%C"
    set "TAG_ID=%%D"
)

set "ESTADO=!ESTADO_ID_%TAG_ID%!"

if "!ESTADO!"=="" (
    exit /b
)

:: ------------------------------------------------------------
:: SWITCH
:: ------------------------------------------------------------

if /i "!TAG_TIPO!"=="SWITCH" (

    set "LINHA_SAIDA=        case !TAG_NAMESPACE!::!TAG_ENUM!::!ESTADO!://[BAT$!TAG_NAMESPACE!$SWITCH$!TAG_ID!]"

    exit /b
)

:: ------------------------------------------------------------
:: DEFINIR
:: ------------------------------------------------------------

if /i "!TAG_TIPO!"=="DEFINIR" (

    set "LINHA_SAIDA=        !TAG_NAMESPACE!::definir(!TAG_NAMESPACE!::!TAG_ENUM!::!ESTADO!);//[BAT$!TAG_NAMESPACE!$DEFINIR$!TAG_ID!]"

    exit /b
)

:: ------------------------------------------------------------
:: VOID
:: ------------------------------------------------------------

if /i "!TAG_TIPO!"=="VOID" (

    call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "!TAG_NAMESPACE!" PASCAL
    set "NS_PASCAL=!NORM_OUT!"

    call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "!ESTADO!" PASCAL
    set "ESTADO_PASCAL=!NORM_OUT!"

    set "LINHA_SAIDA=        void Controle_!NS_PASCAL!_!ESTADO_PASCAL!();//[BAT$!TAG_NAMESPACE!$VOID$!TAG_ID!]"

    exit /b
)

exit /b
:: ------------------------------------------------------------
:: FUNCAO
:: ------------------------------------------------------------

if /i "!TAG_TIPO!"=="FUNCAO" (

    call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "!TAG_NAMESPACE!" PASCAL
    set "NS_PASCAL=!NORM_OUT!"

    call "%DIRETORIO_NOMENCLATURA%\NOMENCLATURA.bat" "!ESTADO!" PASCAL
    set "ESTADO_PASCAL=!NORM_OUT!"

    set "LINHA_SAIDA=        Controle_!NS_PASCAL!_!ESTADO_PASCAL!();//[BAT$!TAG_NAMESPACE!$FUNCAO$!TAG_ID!]"

    exit /b
)

exit /b