@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: ============================================================
::  NOMENCLATURA.bat
::  Standalone : NOMENCLATURA.bat
::  Pascal_Case  : call NOMENCLATURA.bat "texto"
::  UPPER        : call NOMENCLATURA.bat "texto" UPPER
::  CamelCase    : call NOMENCLATURA.bat "texto" CAMEL
::  snakeCase    : call NOMENCLATURA.bat "texto" SNAKE
::  Resultado  : %TEXTO_TRATADO%
:: ============================================================

set "DIRETORIO_SIGLAS=%DIRETORIO_RAIZ%\Nomenclatura"

if not exist "%DIRETORIO_SIGLAS%\SIGLAS.lst" (
    echo.
    echo [ERRO] SIGLAS.lst nao encontrado.
    exit /b 1
)


echo.
echo ============================
echo NOMENCLATURA
echo ARG1=[%~1]
echo ARG2=[%~2]
echo ============================

if not "%~1"=="" (

    set "LETRA=%~1"
    set "SAIDA=PASCAL"

    if /i "%~2"=="UPPER" set "SAIDA=UPPER"
    if /i "%~2"=="CAMEL" set "SAIDA=CAMEL"
    if /i "%~2"=="SNAKE" set "SAIDA=SNAKE"

    goto CONTINUAR

)

set "MODO_INTERATIVO=1"

::==================================================================
::      MODO INTERATIVO
::==================================================================

:MODO_INTERATIVO
set /p LETRA=Digite o texto para normalizar:

echo.
echo Escolha o formato de saida:
echo [1] PascalCase (Padrao)
echo [2] UPPER_CASE
echo [3] camelCase
echo [4] snake_case
echo.

set "SAIDA=PASCAL"
set /p opcao=Digite o numero da opcao desejada (ou Enter para Pascal):

if "%opcao%"=="2" set "SAIDA=UPPER"
if "%opcao%"=="3" set "SAIDA=CAMEL"
if "%opcao%"=="4" set "SAIDA=SNAKE"

:CONTINUAR

:: -------- PASSO 0: Remove acentos --------
set "LETRA=!LETRA:á=a!"
set "LETRA=!LETRA:à=a!"
set "LETRA=!LETRA:â=a!"
set "LETRA=!LETRA:ã=a!"
set "LETRA=!LETRA:Á=A!"
set "LETRA=!LETRA:À=A!"
set "LETRA=!LETRA:Â=A!"
set "LETRA=!LETRA:Ã=A!"
set "LETRA=!LETRA:é=e!"
set "LETRA=!LETRA:è=e!"
set "LETRA=!LETRA:ê=e!"
set "LETRA=!LETRA:ë=e!"
set "LETRA=!LETRA:É=E!"
set "LETRA=!LETRA:È=E!"
set "LETRA=!LETRA:Ê=E!"
set "LETRA=!LETRA:Ë=E!"
set "LETRA=!LETRA:í=i!"
set "LETRA=!LETRA:ì=i!"
set "LETRA=!LETRA:î=i!"
set "LETRA=!LETRA:ï=i!"
set "LETRA=!LETRA:Í=I!"
set "LETRA=!LETRA:Ì=I!"
set "LETRA=!LETRA:Î=I!"
set "LETRA=!LETRA:Ï=I!"
set "LETRA=!LETRA:ó=o!"
set "LETRA=!LETRA:ò=o!"
set "LETRA=!LETRA:ô=o!"
set "LETRA=!LETRA:õ=o!"
set "LETRA=!LETRA:ö=o!"
set "LETRA=!LETRA:Ó=O!"
set "LETRA=!LETRA:Ò=O!"
set "LETRA=!LETRA:Ô=O!"
set "LETRA=!LETRA:Õ=O!"
set "LETRA=!LETRA:Ö=O!"
set "LETRA=!LETRA:ú=u!"
set "LETRA=!LETRA:ù=u!"
set "LETRA=!LETRA:û=u!"
set "LETRA=!LETRA:ü=u!"
set "LETRA=!LETRA:Ú=U!"
set "LETRA=!LETRA:Ù=U!"
set "LETRA=!LETRA:Û=U!"
set "LETRA=!LETRA:Ü=U!"
set "LETRA=!LETRA:ç=c!"
set "LETRA=!LETRA:Ç=C!"

:: -------- PASSO 1: Checa se o input inteiro é uma sigla composta --------
set "LETRA_CHECK=!LETRA: =_!"          :: espaços → underscore para comparar
for /f "usebackq delims=" %%S in ("%DIRETORIO_SIGLAS%\SIGLAS.lst") do (
    if /i "!LETRA_CHECK!"=="%%S" (
        set "TEXTO_TRATADO=%%S"
        goto FIM
    )
)

:: -------- PASSO 2: Unifica separadores --------
set "LETRA=!LETRA:_= !"
set "LETRA=!LETRA:-= !"

:: -------- PASSO 3: Monta saida conforme modo --------
set "TEXTO_TRATADO="

if "!SAIDA!"=="PASCAL" goto MODO_PASCAL
if "!SAIDA!"=="UPPER"  goto MODO_UPPER
if "!SAIDA!"=="SNAKE"  goto MODO_SNAKE
if "!SAIDA!"=="CAMEL"  goto MODO_CAMEL

goto MODO_PASCAL

:: ---- PASCAL_SNAKE ----

:MODO_PASCAL

for %%W in (!LETRA!) do (
    set "PALAVRA=%%W"
    set "FINAL="

    rem ==================================================
    rem 1. PRIORIDADE MAXIMA → LISTA DE SIGLAS
    rem ==================================================
    for /f "usebackq delims=" %%S in ("%DIRETORIO_SIGLAS%\SIGLAS.lst") do (
        if /i "%%W"=="%%S" (
            set "FINAL=%%S"
        )
    )

    rem ==================================================
    rem 2. SE NAO ESTA NA LISTA → VERIFICA NUMERO
    rem ==================================================
    if not defined FINAL (
        echo %%W | findstr /r "[0-9]" >nul && (
            call :LETRA_UP "!PALAVRA!" FINAL
        )
    )

    rem ==================================================
    rem 3. PADRAO → PASCAL
    rem ==================================================
    if not defined FINAL (
        call :CAPITALIZAR
        set "FINAL=!LIMITE!"
    )

    rem ==================================================
    rem MONTA RESULTADO
    rem ==================================================
    if "!TEXTO_TRATADO!"=="" (
        set "TEXTO_TRATADO=!FINAL!"
    ) else (
        set "TEXTO_TRATADO=!TEXTO_TRATADO!_!FINAL!"
    )
)
goto FIM

:: ---- UPPER_SNAKE ----
:MODO_UPPER
for %%W in (!LETRA!) do (
    set "PALAVRA=%%W"
    call :LETRA_UP "!PALAVRA!" PALAVRA_UP
    if "!TEXTO_TRATADO!"=="" (
        set "TEXTO_TRATADO=!PALAVRA_UP!"
    ) else (
        set "TEXTO_TRATADO=!TEXTO_TRATADO!_!PALAVRA_UP!"
    )
)
goto FIM

:: ---- SNAKE_CASE ----
:MODO_SNAKE
for %%W in (!LETRA!) do (
    set "PALAVRA=%%W"
    call :LETRA_LOW "!PALAVRA!" PALAVRA_L
    if "!TEXTO_TRATADO!"=="" (
        set "TEXTO_TRATADO=!PALAVRA_L!"
    ) else (
        set "TEXTO_TRATADO=!TEXTO_TRATADO!_!PALAVRA_L!"
    )
)
goto FIM

:: ---- CAMEL_CASE ----
:MODO_CAMEL
set "CONTADOR=0"
for %%W in (!LETRA!) do (
    set /a CONTADOR+=1
    set "PALAVRA=%%W"
    
    if !CONTADOR! EQU 1 (
        :: Primeira palavra sempre minúscula
        call :LETRA_LOW "!PALAVRA!" FINAL
    ) else (
        :: Demais palavras seguem sua lógica inteligente (sigla ou capitalizada)
        set "FINAL="
        for /f "usebackq delims=" %%S in ("%DIRETORIO_SIGLAS%\SIGLAS.lst") do (
            if /i "%%W"=="%%S" set "FINAL=%%S"
        )
        if not defined FINAL (
            call :CAPITALIZAR
            set "FINAL=!LIMITE!"
        )
    )
    set "TEXTO_TRATADO=!TEXTO_TRATADO!!FINAL!"
)
goto FIM

:: ================================================================================
:FIM

if defined MODO_INTERATIVO (
    set "RET_INTERATIVO=1"
)

endlocal & (
    set "TEXTO_TRATADO=%TEXTO_TRATADO%"
    set "RET_INTERATIVO=%RET_INTERATIVO%"
)

if defined RET_INTERATIVO (
    echo Saida: %TEXTO_TRATADO%
)

exit /b 0
:: ============================================================
:CAPITALIZAR
set "PRIMEIRA_LETRA=!PALAVRA:~0,1!"
set "RESTO_PALAVRA=!PALAVRA:~1!"
call :CHAR_UP "!PRIMEIRA_LETRA!" PRIMEIRA_LETRA_U
call :LETRA_LOW "!RESTO_PALAVRA!"  RESTO_PALAVRA_L
set "LIMITE=!PRIMEIRA_LETRA_U!!RESTO_PALAVRA_L!"
exit /b

:: ============================================================
:CHAR_UP
set "_C=%~1"
for %%P in (A-a B-b C-c D-d E-e F-f G-g H-h I-i J-j K-k L-l M-m N-n O-o P-p Q-q R-r S-s T-t U-u V-v W-w X-x Y-y Z-z) do (
    for /f "tokens=1,2 delims=-" %%X in ("%%P") do (
        if "!_C!"=="%%Y" set "_C=%%X"
    )
)
set "%~2=!_C!"
exit /b

:: ============================================================
:LETRA_LOW
set "_IN=%~1"
set "_OUT="

:LETRA_LOW_LOOP
if "!_IN!"=="" ( set "%~2=!_OUT!" & exit /b )
set "_CHAR=!_IN:~0,1!"
set "_IN=!_IN:~1!"
for %%P in (A-a B-b C-c D-d E-e F-f G-g H-h I-i J-j K-k L-l M-m N-n O-o P-p Q-q R-r S-s T-t U-u V-v W-w X-x Y-y Z-z) do (
    for /f "tokens=1,2 delims=-" %%X in ("%%P") do (
        if "!_CHAR!"=="%%X" set "_CHAR=%%Y"
    )
)
set "_OUT=!_OUT!!_CHAR!"
goto LETRA_LOW_LOOP

:: ============================================================
:LETRA_UP
set "_IN=%~1"
set "_OUT="

:LETRA_UP_LOOP
if "!_IN!"=="" ( set "%~2=!_OUT!" & exit /b )
set "_CHAR=!_IN:~0,1!"
set "_IN=!_IN:~1!"
for %%P in (A-a B-b C-c D-d E-e F-f G-g H-h I-i J-j K-k L-l M-m N-n O-o P-p Q-q R-r S-s T-t U-u V-v W-w X-x Y-y Z-z) do (
    for /f "tokens=1,2 delims=-" %%X in ("%%P") do (
        if "!_CHAR!"=="%%Y" set "_CHAR=%%X"
    )
)
set "_OUT=!_OUT!!_CHAR!"
goto LETRA_UP_LOOP
