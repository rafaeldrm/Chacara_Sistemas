@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: =========================================
:: BASE SEGURA
:: =========================================
set "BASE=%~dp0"
set "TMP_H=%BASE%tmp_h.txt"
set "TMP_CPP=%BASE%tmp_cpp.txt"
set "TMP_OUT=%BASE%tmp_out.txt"

:: =========================================
:: IR PARA LIB
:: =========================================
cd..
cd lib\

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

for %%f in (*.h *.cpp) do (
    set /a count+=1
    set "file!count!=%%f"
    echo !count! - %%f
)

echo.
set /p selH=Arquivo .h: 
set /p selCPP=Arquivo .cpp: 

set "ARQ_H=!file%selH%!"
set "ARQ_CPP=!file%selCPP%!"

if not exist "%ARQ_H%" (
    echo [ERRO] Arquivo .h nao encontrado
    pause
    exit /b
)

if not exist "%ARQ_CPP%" (
    echo [ERRO] Arquivo .cpp nao encontrado
    pause
    exit /b
)

findstr /C:"// [BAT_NAMESPACE/CLASS]" "%ARQ_H%" >nul || (
    echo [ERRO] Marcador nao existe no .h
    pause
    exit /b
)

findstr /C:"// [BAT_NAMESPACE/CLASS]" "%ARQ_CPP%" >nul || (
    echo [ERRO] Marcador nao existe no .cpp
    pause
    exit /b
)

:: =========================================
:: DADOS
:: =========================================
set /p NOME_NAMESPACE=Namespace:
call "%BASE%NORMALIZAR.bat" "%NOME_NAMESPACE%" UPPER
set "NAMESPACE=%NORM_OUT%"

set /p NOME_ENUM_CLASS=Enum:
call "%BASE%NORMALIZAR.bat" "%NOME_ENUM_CLASS%" UPPER
set "ENUM_CLASS=%NORM_OUT%"

set /p VARIAVEL=Variavel:
call "%BASE%NORMALIZAR.bat" "%VARIAVEL%"
set "VARIAVEL=%NORM_OUT%"

:: =========================================
:: CAPTURA ESTADOS
:: =========================================
set count=0

:LOOP
set /p ITEM=Estado (0 sai): 
if "%ITEM%"=="0" goto FIM

if "%ITEM%"=="" goto LOOP

set /a count+=1
call "%BASE%NORMALIZAR.bat" "%ITEM%" UPPER
set "ESTADO!count!=%NORM_OUT%"

goto LOOP
:FIM

if %count%==0 (
    echo [ERRO] Nenhum estado informado
    pause
    exit /b
)

:: =========================================
:: GERAR .H
:: =========================================
:: limpa arquivo
> "%TMP_H%" echo.

echo namespace %NAMESPACE% >> "%TMP_H%"
echo { >> "%TMP_H%"
echo enum class %ENUM_CLASS% : uint8_t >> "%TMP_H%"
echo { >> "%TMP_H%"

for /l %%i in (1,1,%count%) do (
    set "VAL=!ESTADO%%i!"
    set "LINE=    !VAL!"
    if %%i NEQ %count% set "LINE=!LINE!,"
    echo(!LINE!>>"%TMP_H%"
)

echo }; >> "%TMP_H%"
echo. >> "%TMP_H%"
echo    void definir(%ENUM_CLASS% novo); >> "%TMP_H%"
echo    %ENUM_CLASS% atual(); >> "%TMP_H%"
echo    const __FlashStringHelper* texto(%ENUM_CLASS% s); >> "%TMP_H%"
echo }; >> "%TMP_H%"
:: =========================================
:: GERAR .CPP
:: =========================================
:: limpa arquivo
> "%TMP_CPP%" echo.

echo namespace %NAMESPACE% >> "%TMP_CPP%"
echo { >>"%TMP_CPP%"

echo void definir(%ENUM_CLASS% novo) >> "%TMP_CPP%"
echo { >> "%TMP_CPP%"
echo     if (estado.%VARIAVEL% == novo^) return; >> "%TMP_CPP%"
echo. >> "%TMP_CPP%"
echo     estado.%VARIAVEL% = novo; >> "%TMP_CPP%"
echo     debugVariavel("%VARIAVEL% : ", texto(novo^), NENHUM, VERDE, AMARELO^); >> "%TMP_CPP%"
echo } >> "%TMP_CPP%"
echo. >> "%TMP_CPP%"

echo %ENUM_CLASS% atual() >> "%TMP_CPP%"
echo { >> "%TMP_CPP%"
echo     return estado.%VARIAVEL%; >> "%TMP_CPP%"
echo } >> "%TMP_CPP%"
echo. >> "%TMP_CPP%"

echo const __FlashStringHelper* texto(%ENUM_CLASS% s) >> "%TMP_CPP%"
echo { >> "%TMP_CPP%"
echo     switch(s) >> "%TMP_CPP%"
echo     { >> "%TMP_CPP%"

for /l %%i in (1,1,%count%) do (
    set "VAL=!ESTADO%%i!"
    echo     case %ENUM_CLASS%::!VAL!: return F("!VAL!"^); >> "%TMP_CPP%"
)

echo     default: return F("DESCONHECIDO"); >> "%TMP_CPP%"
echo     } >> "%TMP_CPP%"
echo } >> "%TMP_CPP%"
echo. >> "%TMP_CPP%"

echo // ==== STRUCT SUGESTAO ==== >> "%TMP_CPP%"
echo // %NAMESPACE%::%ENUM_CLASS% %VARIAVEL%; >> "%TMP_CPP%"
echo // %NAMESPACE%::%ENUM_CLASS%::%ESTADO1%; >> "%TMP_CPP%"

echo } >> "%TMP_CPP%"

echo GERANDO ARQUIVOS TEMPORÁRIOS...
:: =========================================
:: INJETAR
:: =========================================
echo ================================
echo INJETANDO NO .H
echo ================================

set "OUT=%ARQ_H%.tmp"
break > "%OUT%"

set "ACHOU=0"


for /f "usebackq delims=" %%a in ("%ARQ_H%") do (
    
    set "linha=%%a"

    echo(!linha! | findstr /C:"// [BAT_NAMESPACE/CLASS]" >nul
   if !errorlevel! == 0 (
    type "%TMP_H%" >> "%OUT%"
    set "ACHOU=1"
) else (
    >>"%OUT%" echo(!linha!
)
    )
)

if !ACHOU!==0 (
    echo [ERRO] Marcador nao encontrado no .h
    pause
) else (
    move /y "%OUT%" "%ARQ_H%" >nul
    echo [OK] .h atualizado
)

echo ================================
echo INJETANDO NO .CPP
echo ================================

set "OUT=%ARQ_CPP%.tmp"
break > "%OUT%"

set "ACHOU=0"

for /f "usebackq delims=" %%a in ("%ARQ_CPP%") do (

    set "linha=%%a"
    
      echo(!linha! | findstr /C:"// [BAT_NAMESPACE/CLASS]" >nul
       if !errorlevel! == 0 (
    type "%TMP_CPP%" >> "%OUT%"
    set "ACHOU=1"
) else (
    >>"%OUT%" echo(!linha!
      )
    )
 )

if !ACHOU!==0 (
    echo [ERRO] Marcador nao encontrado no .cpp
    pause
) else (
    move /y "%OUT%" "%ARQ_CPP%" >nul
    echo [OK] .cpp atualizado
)

echo ================================
echo LIMPEZA
echo ================================

echo LIMPANDO ARQUIVOS TEMPORARIOS...

if exist "%TMP_H%" del "%TMP_H%"
if exist "%TMP_CPP%" del "%TMP_CPP%"

echo [OK] Limpeza concluida!