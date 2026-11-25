@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: =========================================
:: VARIAVEIS DE SUPORTE
:: =========================================
set "BASE=%~dp0"
for %%i in ("%BASE%..") do set "DIR_RAIZ=%%~fi"
set "DIR_ESTADOS=%BASE%Estados"
set "DIR_NOMENCLATURA=%DIR_RAIZ%\Nomenclatura"

if not exist "%DIR_ESTADOS%" mkdir "%DIR_ESTADOS%"

:MENU
cls
echo %DIR_NOMENCLATURA%
echo.
echo =========================================
echo           GERADOR DE ESTADOS
echo =========================================
echo.
echo  1 - Criar novo estado
echo  2 - Editar estado existente
echo  3 - Atualizar .h e .cpp
echo  0 - Sair
echo.
echo =========================================
echo.

set /p OPCAO=Digite o numero da opcao e aperte ENTER : 
echo.

if "%OPCAO%"=="1" goto CRIAR
if "%OPCAO%"=="2" goto EDITAR
if "%OPCAO%"=="3" goto ATUALIZAR
if "%OPCAO%"=="0" exit /b

echo [ERRO] Opcao invalida.
pause
goto MENU

:CRIAR
cls
echo.
set /p ARQUIVO_TXT=Nome do arquivo estrutural :
echo.
call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "%ARQUIVO_TXT%" UPPER

setlocal enabledelayedexpansion
set "ARQUIVO_TXT=%NORM_OUT%"
set "DESTINO=%DIR_ESTADOS%\%ARQUIVO_TXT%.txt"

if exist "%DESTINO%" (
    echo.
    echo [ERRO] Arquivo ja existe.
    pause
    goto MENU
)

copy "%BASE%escopo.txt" "%DESTINO%" >nul
call code -r "%DESTINO%"

echo.
echo Edite o arquivo e salve.
pause
call :VALIDAR "%DESTINO%"
goto MENU

:EDITAR
cls
echo =========================================
echo  SELECIONE O ARQUIVO PARA EDITAR
echo =========================================
echo.
set "COUNT=0"
for %%F in ("%DIR_ESTADOS%\*.txt") do (
    set /a COUNT+=1
    set "ARQ_!COUNT!=%%F"
    echo  !COUNT! - %%~nF
)
if !COUNT! EQU 0 ( pause & goto MENU )
echo.
set /p OP_EDIT=Digite o numero do arquivo: 
if not defined ARQ_%OP_EDIT% ( pause & goto MENU )
call code -r "!ARQ_%OP_EDIT%!"
pause
call :VALIDAR "!ARQ_%OP_EDIT%!"
goto MENU

:ATUALIZAR
cls
echo =========================================
echo  ATUALIZAR CONFIGURACOES EXISTENTES
echo =========================================
echo.
set "COUNT=0"
for %%F in ("%DIR_ESTADOS%\*.txt") do (
    set /a COUNT+=1
    set "ARQ_!COUNT!=%%F"
    echo  !COUNT! - %%~nF
)
if !COUNT! EQU 0 ( pause & goto MENU )
echo.
set /p OP_AT=Digite o numero do arquivo para gerar os fontes: 
if not defined ARQ_%OP_AT% ( pause & goto MENU )
call :VALIDAR "!ARQ_%OP_AT%!"
goto MENU

::==========================================================================================
:: VALIDAÇÃO E GERAÇÃO
::==========================================================================================
:VALIDAR
set "ARQ=%~1"
if "%~1"=="" exit /b

set "SISTEMA=" & set "NAMESPACE=" & set "ENUM=" & set "VARIAVEL="

for /f "usebackq tokens=1,* delims==" %%A in ("%~1") do (
    set "CHAVE=%%A"
    set "VALOR=%%B"
    for /f "tokens=*" %%X in ("!CHAVE!") do set "CHAVE=%%X"
    for /f "tokens=*" %%X in ("!VALOR!") do set "VALOR=%%X"
    
    if /i "!CHAVE!"=="SISTEMA"   set "SISTEMA=!VALOR!"
    if /i "!CHAVE!"=="NAMESPACE" set "NAMESPACE=!VALOR!"
    if /i "!CHAVE!"=="ENUM"      set "ENUM=!VALOR!"
    if /i "!CHAVE!"=="VARIAVEL"  set "VARIAVEL=!VALOR!"
)

if "!SISTEMA!"=="" ( echo [ERRO] SISTEMA vazio. & pause & exit /b )
if "!NAMESPACE!"=="" ( echo [ERRO] NAMESPACE vazio. & pause & exit /b )
if "!ENUM!"=="" ( echo [ERRO] ENUM vazio. & pause & exit /b )
if "!VARIAVEL!"=="" ( echo [ERRO] VARIAVEL vazio. & pause & exit /b )

set "DENTRO_ESTADOS=0"
set "TOTAL_ESTADOS=0"

for /f "usebackq delims=" %%L in ("%ARQ%") do (
    set "LINHA=%%L"
    for /f "tokens=*" %%X in ("!LINHA!") do set "LINHA=%%X"
    
    if /i "!LINHA!"=="[ESTADOS]" (
        set "DENTRO_ESTADOS=1"
    ) else if /i "!LINHA!"=="[END_ESTADOS]" (
        set "DENTRO_ESTADOS=0"
    ) else (
        if "!DENTRO_ESTADOS!"=="1" (
            if not "!LINHA!"=="" (
                :: Evita ler linhas de comentários que sobram no escopo
                echo !LINHA! | findstr /v /c:"apague esse" >nul && (
                    set /a TOTAL_ESTADOS+=1
                    set "ESTADO!TOTAL_ESTADOS!=!LINHA!"
                )
            )
        )
    )
)

if !TOTAL_ESTADOS! EQU 0 ( echo [ERRO] Nenhum estado encontrado. & pause & exit /b )

:: Normalizações Tratadas Individuais
call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "!SISTEMA!"
setlocal enabledelayedexpansion
set "SISTEMA=!NORM_OUT!"

call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "!NAMESPACE!" UPPER
setlocal enabledelayedexpansion
set "NAMESPACE=!NORM_OUT!"

call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "!ENUM!" UPPER
setlocal enabledelayedexpansion
set "ENUM=!NORM_OUT!"

call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "!VARIAVEL!" CAMEL
setlocal enabledelayedexpansion
set "VARIAVEL=!NORM_OUT!"

for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
    call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "!ESTADO%%i!" UPPER
    setlocal enabledelayedexpansion
    set "ESTADO%%i=!NORM_OUT!"
)

:: --- PROCESSA TEMPLATE ---
set "TEMPLATE=%BASE%Estrutura_Estados.txt"
set "ARQ_H=%BASE%Estado_Sistema_%SISTEMA%.h"
set "ARQ_CPP=%BASE%Estado_Sistema_%SISTEMA%.cpp"

if exist "%ARQ_H%" del "%ARQ_H%"
if exist "%ARQ_CPP%" del "%ARQ_CPP%"

if not exist "%TEMPLATE%" (
    echo [ERRO CRITICO] O arquivo %TEMPLATE% nao foi encontrado!
    pause
    exit /b
)

set "ALVO="

:: Força a leitura exata mantendo linhas em branco e caracteres especiais do template
for /f "usebackq delims=" %%L in ("%TEMPLATE%") do (
    set "TXT_LINE=%%L"
    
    :: Limpeza temporária apenas para testar as tags de controle
    set "CLEAN=!TXT_LINE!"
    for /f "tokens=*" %%X in ("!CLEAN!") do set "CLEAN=%%X"

    if "!CLEAN!"=="[FILE:Estados_Sistema_{{SISTEMA}}.h]" (
        set "ALVO=H"
    ) else if "!CLEAN!"=="[FILE:Estados_Sistema_{{SISTEMA}}.cpp]" (
        set "ALVO=CPP"
    ) else if "!CLEAN!"=="[END_FILE]" (
        set "ALVO="
    ) else (
        if defined ALVO (
            :: Executa as substituições com segurança na linha original de texto
            set "TXT_LINE=!TXT_LINE:{{SISTEMA}}=%SISTEMA%!"
            set "TXT_LINE=!TXT_LINE:{{NAMESPACE}}=%NAMESPACE%!"
            set "TXT_LINE=!TXT_LINE:{{ENUM}}=%ENUM%!"
            set "TXT_LINE=!TXT_LINE:{{VARIAVEL}}=%VARIAVEL%!"

            if "!ALVO!"=="H" (
                echo !TXT_LINE! | findstr /c:"{{ESTADOS}}" >nul && (
                    for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                        >>"%ARQ_H%" echo     !ESTADO%%i!,
                    )
                ) || (
                    >>"%ARQ_H%" echo(!TXT_LINE!
                )
            )

            if "!ALVO!"=="CPP" (
                echo !TXT_LINE! | findstr /c:"{{CASES}}" >nul && (
                    for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                        >>"%ARQ_CPP%" echo             case %ENUM%::!ESTADO%%i!: return F^("!ESTADO%%i!"^);
                    )
                ) || (
                    >>"%ARQ_CPP%" echo(!TXT_LINE!
                )
            )
        )
    )
)

:: =========================================================================
:: VALIDAÇÃO FÍSICA FINAL
:: =========================================================================
echo.
set "SUCESSO=1"
if not exist "%ARQ_H%" (
    echo [ALERTA] O arquivo .h nao foi gerado com sucesso.
    set "SUCESSO=0"
)
if not exist "%ARQ_CPP%" (
    echo [ALERTA] O arquivo .cpp nao foi gerado com sucesso.
    set "SUCESSO=0"
)

if "%SUCESSO%"=="1" (
    echo ====================================================
    echo [SUCESSO] Arquivos estruturados gerados na raiz!
    echo Target H  : Estado_Sistema_%SISTEMA%.h
    echo Target CPP: Estado_Sistema_%SISTEMA%.cpp
    echo ====================================================
)
pause
exit /b