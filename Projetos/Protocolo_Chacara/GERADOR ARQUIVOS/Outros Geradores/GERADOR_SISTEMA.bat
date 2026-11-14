:: tabelas de caracter UNICODE (Windows+R :: charmap "enter")

@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

echo.
echo ###############################################
echo ##        GERADOR DE MODULOS ARDUINO         ##
echo ###############################################
echo.

::======================================================
::    DEFININDO CONCEITOS DAS CHAMADAS DOS ARQUIVOS .bat
::======================================================

set "DIR_RAIZ=%~dp0"

set "DIR_NOMENCLATURA=%DIR_RAIZ%Nomenclatura"

::set "DIR_CRIAR=%DIR_RAIZ%Criar"
::set "DIR_GERAR=%DIR_RAIZ%Gerar"
::set "DIR_MODIFICAR=%DIR_RAIZ%Modificar"


:: =======================================================
:: SELEÇÃO DO PROJETO
:: =======================================================

echo ┌─────────────────────────────┐
echo │     SELECIONE O PROJETO     │
echo └─────────────────────────────┘

:: volta duas pastas para trás
cd ..\..

set contadorProjetos=0

for /d %%i in (*) do (
    set /a contadorProjetos+=1
    set "pastaProjeto!contadorProjetos!=%%i"
    echo !contadorProjetos! - %%i
)

echo.
set /p projetoSelecionado=DIGITE O NÚMERO DO PROJETO e aperte ENTER : 

if not defined pastaProjeto%projetoSelecionado% (
    echo.
    echo [ERRO] Projeto inválido.
    pause
    exit /b
)

set "PROJETO=!pastaProjeto%projetoSelecionado%!"

echo.
echo Projeto selecionado:
echo.
echo ====================================================
echo %PROJETO%
echo ====================================================
echo.

:: entra no projeto
cd "%PROJETO%"

:: variáveis globais do projeto
set "CAMINHO_PROJETO=%cd%"
set "CAMINHO_LIB=%CAMINHO_PROJETO%\lib"

:: entra na pasta lib
cd "%CAMINHO_LIB%"

:: =======================================================
:: LISTA DE MÓDULOS
:: =======================================================

echo.
echo ┌───────────────────────────────────┐
echo │        MÓDULOS EXISTENTES         │
echo ├───────────────────────────────────┤
echo │                                   │

set contadorPastas=0

for /d %%i in (*) do (

    set /a contadorPastas+=1

    :: guarda o nome do módulo
    set "Modulo!contadorPastas!=%%i"

    :: exibição formatada
    set "linha= !contadorPastas! - %%i"
    set "espacos=                                                            "
    set "texto=!linha!!espacos!"

    echo ^│!texto:~0,35!^│
)

echo ├───────────────────────────────────┤
echo │                                   │

:: opção criar novo módulo
set /a contadorPastas+=1

set "linha= !contadorPastas! - [CRIAR NOVO MODULO]"
set "espacos=                                                            "
set "texto=!linha!!espacos!"

echo ^│!texto:~0,35!^│

echo │                                   │
echo └───────────────────────────────────┘
echo.

:: =======================================================
:: ESCOLHA DO MÓDULO
:: =======================================================

set /p escolha=ESCOLHA UMA OPÇÃO : 

:: criar novo módulo
if "%escolha%"=="%contadorPastas%" goto NOVO_MODULO

:: selecionar módulo existente
if defined Modulo%escolha% (
    set "MODULO=!Modulo%escolha%!"
    goto MODULO_SELECIONADO
)

echo.
echo [ERRO] Opção inválida.
pause
exit /b


:: =======================================================
:: CRIAR NOVO MÓDULO
:: =======================================================

:NOVO_MODULO

echo.
set /p INPUT_PASTA=Nome do novo módulo : 

call "%DIR_NOMENCLATURA%\NOMENCLATURA.bat" "%INPUT_PASTA%"

set "MODULO=%NORM_OUT%"

if not exist "%MODULO%" (
    mkdir "%MODULO%"
    echo.
    echo [OK] Módulo criado.
) else (
    echo.
    echo [AVISO] Módulo já existia.
)

goto MODULO_SELECIONADO


:: =======================================================
:: RESULTADO
:: =======================================================

:MODULO_SELECIONADO

set "CAMINHO_MODULO=%CAMINHO_LIB%\%MODULO%"

echo.
echo ====================================================
echo PROJETO        : %PROJETO%
echo CAM_PROJETO    : %CAMINHO_PROJETO%
echo MODULO         : %MODULO%
echo CAM_MODULO     : %CAMINHO_MODULO%
echo ====================================================
echo.
