@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
:: GERADOR_INCLUDE.bat
:: ============================================================

:: =========================================
:: BASE
:: =========================================
set "BASE=%~dp0"

:: =========================================
:: IR PARA LIB
:: =========================================
cd..
cd lib\

:: =========================================
:: LISTAR TODOS .CPP E .H
:: =========================================
echo.
echo =========================================
echo        ARQUIVOS DESTINO
echo =========================================
echo.

set count=0

for /r %%f in (*.cpp *.h) do (

    set /a count+=1

    set "DESTINO!count!=%%f"

    echo !count! - %%f
)

echo.
set /p SEL_DESTINO=Escolha o arquivo destino: 

if not defined DESTINO%SEL_DESTINO% (
    echo.
    echo [ERRO] Arquivo invalido
    pause
    exit /b
)

set "ARQ_DESTINO=!DESTINO%SEL_DESTINO%!"

:: =========================================
:: VERIFICAR LINKER
:: =========================================
findstr /C:"// [BAT_INCLUDE]" "%ARQ_DESTINO%" >nul

if errorlevel 1 (
    echo.
    echo [ERRO] Linker // [BAT_INCLUDE] nao encontrado
    pause
    exit /b
)

:: =========================================
:: LISTAR TODOS HEADERS
:: =========================================
echo.
echo =========================================
echo         HEADERS DISPONIVEIS
echo =========================================
echo.

set count=0

for /r %%f in (*.h) do (

    set /a count+=1

    set "HEADER!count!=%%~nxf"

    echo !count! - %%~nxf
)

echo.
set /p INPUT=Digite os numeros separados por espaco: 

if "%INPUT%"=="" exit /b

:: =========================================
:: GERAR TEMPORARIO
:: =========================================
set "TMP=%TEMP%\bat_include_tmp.txt"

break > "%TMP%"

echo.
echo =========================================
echo              PREVIEW
echo =========================================
echo.

for %%N in (%INPUT%) do (

    if defined HEADER%%N (

        set "ARQ=!HEADER%%N!"

        echo #include "!ARQ!"
        echo #include "!ARQ!">> "%TMP%"

    ) else (

        echo [AVISO] Numero %%N invalido

    )
)

:: =========================================
:: CONFIRMACAO
:: =========================================
echo.
set /p CONFIRM=Confirmar inclusoes? [s/n]: 

if /i not "%CONFIRM%"=="s" (

    del "%TMP%"

    echo.
    echo Cancelado

    pause
    exit /b
)

:: =========================================
:: INJETAR
:: =========================================
set "OUT=%TEMP%\bat_include_out.tmp"

break > "%OUT%"

set "ACHOU=0"

for /f "usebackq delims=" %%L in ("%ARQ_DESTINO%") do (

    set "LINHA=%%L"

    echo(!LINHA! | findstr /C:"// [BAT_INCLUDE]" >nul
    if !errorlevel! == 0 (
        type "%TMP%" >> "%OUT%"
        set "ACHOU=1"
    ) else (
        >> "%OUT%" echo(!LINHA!
    )
)

if !ACHOU!==0 (

    echo.
    echo [ERRO] Linker nao encontrado durante injecao

    del "%TMP%"
    del "%OUT%"

    pause
    exit /b
)

move /y "%OUT%" "%ARQ_DESTINO%" >nul

:: =========================================
:: LIMPEZA
:: =========================================
del "%TMP%"

echo.
echo =========================================
echo [OK] Includes injetados com sucesso!
echo =========================================
echo.

pause