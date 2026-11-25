@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: CRIAR_FSM.bat
:: ============================================================================
:: Maestro da criação de uma nova FSM
:: ============================================================================

:: Lê o arquivo de sessão e carrega as variáveis para o escopo local
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"

if not exist "%ARQUIVO_SESSAO%" (

    echo.
    echo [ERRO] Sessao nao encontrada.
    echo Execute primeiro o FSM.bat
    echo.

    pause
    exit /b

)
:ESCOLHER_SISTEMA

echo =============================================================
echo  ESCOLHA SEU SISTEMA = PROJETO = A CRIAR UMA NOVA MÁQUINA DE ESTADOS
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
    goto ESCOLHER_SISTEMA
)

set "SISTEMA_ATUAL=!SISTEMA_%ESCOLHA_SISTEMA%!"
set "PASTA_SISTEMA=%DIRETORIO_ESTADOS%\!SISTEMA_ATUAL!"
if not exist "!PASTA_SISTEMA!" mkdir "!PASTA_SISTEMA!"

cls

echo ===================================================
echo        TIPO DE FSM
echo ===================================================
echo.
echo 1 - FSM simples
echo 2 - FSM com subcategoria
echo.

set "ESCOLHA_TIPO_FSM="
set /p ESCOLHA_TIPO_FSM=Digite o tipo de FSM desejado: 
echo.

if "%ESCOLHA_TIPO_FSM%"=="1" (
    call "%BASE%FSM_SIMPLES.bat"
    exit /b
)

if "%ESCOLHA_TIPO_FSM%"=="2" (
    call "%BASE%FSM_SUBCATEGORIA.bat"
    exit /b
)

echo [ERRO] Opcao invalida!
pause
exit /b