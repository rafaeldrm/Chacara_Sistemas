@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

set "BASE=%~dp0"
for %%i in ("%BASE%..\..") do set "LIB=%%~fi"
for %%i in ("%BASE%..") do set "DIRETORIO_RAIZ=%%~fi"
for %%i in ("%BASE%..\..\..") do set "PASTA_PROJETO=%%~fi"

set "DIRETORIO_PROJETOS=%PASTA_PROJETO%"
set "DIRETORIO_ESTADOS=%BASE%Estados"
set "DIRETORIO_FSM_SIMPLES=%DIRETORIO_ESTADOS%\FSM_SIMPLES"
set "DIRETORIO_FSM_SUBCATEGORIA=%DIRETORIO_ESTADOS%\FSM_SUBCATEGORIA"
set "DIRETORIO_SISTEMA_ESTADOS=%LIB%\lib\Estados_Sistemas"
set "NOMENCLATURA_BAT=%DIRETORIO_RAIZ%\Nomenclatura\NOMENCLATURA.bat"
set "DIRETORIO_BIBLIOTECAS=%BASE%Bibliotecas"
set "DIRETORIO_TEMP=%BASE%temp"
set "DIRETORIO_TEMPLATES=%BASE%Templates"
set "ARQUIVO_SESSAO=%BASE%temp\sessao.tmp"

:: Garante que a pasta temp exista antes de criar o arquivo
if not exist "%DIRETORIO_TEMP%" mkdir "%DIRETORIO_TEMP%"

:: =========================================================================
::    SALVAR_TEMPORARIO_SESSAO
:: =========================================================================
echo DIRETORIO_PROJETOS=%DIRETORIO_PROJETOS%> "%ARQUIVO_SESSAO%"
echo DIRETORIO_SISTEMA_ESTADOS=%DIRETORIO_SISTEMA_ESTADOS%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_FSM_SIMPLES=%DIRETORIO_FSM_SIMPLES%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_FSM_SUBCATEGORIA=%DIRETORIO_FSM_SUBCATEGORIA%>> "%ARQUIVO_SESSAO%"
echo NOMENCLATURA_BAT=%NOMENCLATURA_BAT%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_BIBLIOTECAS=%DIRETORIO_BIBLIOTECAS%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_TEMP=%DIRETORIO_TEMP%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_ESTADOS=%DIRETORIO_ESTADOS%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_TEMPLATES=%DIRETORIO_TEMPLATES%>> "%ARQUIVO_SESSAO%"
echo ARQUIVO_SESSAO=%ARQUIVO_SESSAO%>> "%ARQUIVO_SESSAO%"
echo Endereços salvos com sucesso!!!
timeout /t 1 >nul

::==================================================================
:MENU
::==================================================================
cls
echo ============================================================================
echo         CONSTRUTOR / REFATORADOR DE ESTADOS COMPLETO (FSM)
echo ============================================================================
echo.
echo 1 - Criar Estrutura Base
echo 2 - Criar nova FSM
echo 3 - Editar FSM
echo 4 - Registrar Arquivos
echo.
echo 0 - Sair
echo.

set "OPCAO="
set /p "OPCAO=Digite a opcao: "

:: Remove espaços extras que o usuário possa ter digitado sem querer
set "OPCAO=%OPCAO: =%"

if "%OPCAO%"=="0" goto SAIR
if "%OPCAO%"=="1" ( call "%BASE%GERADOR_BASE.bat" & goto MENU )
if "%OPCAO%"=="2" ( call "%BASE%CRIAR_FSM.bat" & goto MENU )
if "%OPCAO%"=="3" ( call "%BASE%EDITAR_FSM.bat" & goto MENU )
if "%OPCAO%"=="4" ( call "%BASE%CADASTRAR_ARQUIVOS.bat" & goto MENU )

echo.
echo [ERRO] Opcao "%OPCAO%" invalida.
pause
goto MENU

::==================================================================
:SAIR
::==================================================================
echo.
echo ENCERRANDO SESSÃO...
if exist "%ARQUIVO_SESSAO%" del "%ARQUIVO_SESSAO%"
timeout /t 2 >nul
exit /b
