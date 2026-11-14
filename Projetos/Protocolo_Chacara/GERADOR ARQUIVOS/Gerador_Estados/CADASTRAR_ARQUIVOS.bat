@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

if not exist "%ARQUIVO_SESSAO%" (

    echo.
    echo [ERRO] Sessao nao encontrada.
    echo Execute primeiro o FSM.bat
    echo.

    pause
    exit /b

)

:: Lê o arquivo de sessão e carrega as variáveis para o escopo local
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"


::====================================================================
::         LISTAGEM DAS PASTAS DO BANCO DE DADOS DE FSM's
::====================================================================

cls
echo ===================================================
echo         SELECIONE A PASTA DA FSM a CADASTRAR
echo ===================================================
echo.

set "PASTA_CONTADOR=0"

for /d %%D in ("!DIRETORIO_ESTADOS!\*") do (

    set /a PASTA_CONTADOR+=1

    set "PASTA_!PASTA_CONTADOR!=%%~fD"
    set "NOME_!PASTA_CONTADOR!=%%~nxD"

    echo !PASTA_CONTADOR! - %%~nxD

)

echo.
set /p ESCOLHA_PASTA=Digite o numero da pasta :
echo.


set "PASTA_ESCOLHIDA=!PASTA_%ESCOLHA_PASTA%!"
set "NOME_ESCOLHIDO=!NOME_%ESCOLHA_PASTA%!"

::====================================================================
::  LISTAGEM DOS ESCOPOS.MD EXISTENTES
::====================================================================

cls
echo ===================================================
echo         SELECIONE A FSM a CADASTRAR os ARQUIVOS
echo ===================================================
echo.

set "CONTADOR_TXT=0"

for /R  %%T in ("!PASTA_ESCOLHIDA!\*.md") do (
    set /a CONTADOR_TXT+=1
    set "TXT_NAME=%%~nT"
    set "ED_!CONTADOR_TXT!=!TXT_NAME!"
    echo !CONTADOR_TXT! - !TXT_NAME!
)

if !CONTADOR_TXT! EQU 0 (
    echo.
    echo Nenhuma FSM encontrada.
    pause
)

echo.
set /p ESCOLHA_MD=Digite o numero da FSM:
echo.

if not defined ED_%ESCOLHA_MD% (
    echo Opcao invalida.
    pause
    goto OPCAO_CADASTRAMENTO
)

set "NAMESPACE_ATUAL=!ED_%ESCOLHA_MD%!"

echo pasta selecionada : !PASTA_ESCOLHIDA!
echo arquivo escolhido : !NAMESPACE_ATUAL!

call code "!PASTA_ESCOLHIDA!\!NAMESPACE_ATUAL!.md" >nul

::======================================================
:: LISTA PASTAS DOS PROJETOS EXISTENTES
::======================================================

cls
echo ===================================================
echo         LISTAGEM DAS PASTAS DOS PROJETOS 
echo ===================================================
echo.

set "PASTA_CONTADOR=0"

for /d %%D in ("!DIRETORIO_PROJETOS!\*") do (

    set /a PASTA_CONTADOR+=1

    set "PASTA_!PASTA_CONTADOR!=%%~fD"
    set "NOME_!PASTA_CONTADOR!=%%~nxD"

    echo !PASTA_CONTADOR! - %%~nxD

)

echo.

set /p ESCOLHA_PROJETO=Digite os numeros das pastas :
echo.

set "PROJETO_PASTA_ESCOLHIDO=!PASTA_%ESCOLHA_PROJETO%!"
set "PROJETO_NOME_ESCOLHIDO=!NOME_%ESCOLHA_PROJETO%!"

::================================================================
::       LISTAGEM PASTAS DO PROJETO
::================================================================

cls
echo ===================================================
echo         LISTAGEM DAS PASTAS DENTRO DA LIB
echo ===================================================
echo.

echo Pasta : !PROJETO_PASTA_ESCOLHIDO!
echo Lib : !PROJETO_PASTA_ESCOLHIDO!\lib\
echo.
set "PASTA_CONTADOR=0"

for /d %%D in ("!PROJETO_PASTA_ESCOLHIDO!\lib\*") do (

    set /a PASTA_CONTADOR+=1

    set "LIBPASTA_!PASTA_CONTADOR!=%%~fD"
    set "LIBNOME_!PASTA_CONTADOR!=%%~nxD"

    echo !PASTA_CONTADOR! - %%~nxD

)

echo.

set /p ESCOLHA_PASTAS=Digite os numeros das pastas  ex: (1 3 5 8 10):
echo.

set "PASTAS_ESCOLHIDAS=!PASTA_%ESCOLHA_PASTAS%!"
set "NOMES_ESCOLHIDOS=!NOME_%ESCOLHA_PASTAS%!"

::======================================================
:: LISTA ARQUIVOS
::======================================================

cls
echo ===================================================
echo         ARQUIVOS ENCONTRADOS
echo ===================================================
echo.

set "ARQ=0"

for %%N in (%ESCOLHA_PASTAS%) do (

    for %%F in ("!LIBPASTA_%%N!\*.cpp") do (

        set /a ARQ+=1
        set "ARQUIVO_!ARQ!=%%~fF"
        echo !ARQ! - %%~nxF

    )

    for %%F in ("!LIBPASTA_%%N!\*.h") do (

        set /a ARQ+=1
        set "ARQUIVO_!ARQ!=%%~fF"
        echo !ARQ! - %%~nxF

    )

)

if !ARQ! EQU 0 (

    echo.
    echo Nenhum arquivo encontrado.
    pause
    goto MENU

)

echo.
set /p ESCOLHA_ARQUIVOS=Digite os numeros dos arquivos (Ex: 1 3 5):
echo.

set "PASTAS_ARQUIVOS_ESCOLHIDAS=!PASTA_%ESCOLHA_ARQUIVOS%!"
set "NOMES_ARQUIVOS_ESCOLHIDOS=!NOME_%ESCOLHA_ARQUIVOS%!"

::======================================================
:: CONFIRMAÇÃO
::======================================================

cls
echo ===================================================
echo         CONFIRMACAO
echo ===================================================
echo.

for %%N in (!ESCOLHA_ARQUIVOS!) do (

    set "ARQ_ESCOLHIDO=!ARQUIVO_%%N!"

    set "RELATIVO=!ARQ_ESCOLHIDO:%PROJETO_PASTA_ESCOLHIDO%\lib\=!"

    echo [X] !PROJETO_NOME_ESCOLHIDO! ^| !RELATIVO!

)
echo.
set /p CONFIRMA=Confirmar cadastro? (S/N):

if /I not "!CONFIRMA!"=="S" (
    goto MENU
)

::======================================================
:: SALVAR NO .MD DA FSM
::======================================================

set "ARQUIVO_FSM=!PASTA_ESCOLHIDA!\!NAMESPACE_ATUAL!.md"
set "TEMP_FILE=%TEMP%\fsm_arquivos.tmp"

if exist "!TEMP_FILE!" del "!TEMP_FILE!"

set "DENTRO=0"

:: findstr /n "^" garante que linhas vazias e espaços sejam preservados
for /f "usebackq delims=" %%L in (`findstr /n "^" "!ARQUIVO_FSM!"`) do (
    set "LINHA=%%L"
    
    :: Remove o número da linha e o primeiro caractere ":"
    set "LINHA=!LINHA:*:=!"

    if /I "!LINHA!"=="[INICIO_ARQUIVOS]" (
        set "DENTRO=1"
    ) else if /I "!LINHA!"=="[FIM_ARQUIVOS]" (
        set "DENTRO=0"
    ) else (
        if "!DENTRO!"=="0" (
            :: Grava a linha exatamente como estava (vazia ou com espaços)
            echo(!LINHA!>>"!TEMP_FILE!"
        )
    )
)

echo ARQUIVO_FSM = !ARQUIVO_FSM!
echo TEMP_FILE   = !TEMP_FILE!
pause

move /y "!TEMP_FILE!" "!ARQUIVO_FSM!" >nul

echo.>>"!ARQUIVO_FSM!"
echo [INICIO_ARQUIVOS]>>"!ARQUIVO_FSM!"

for %%N in (!ESCOLHA_ARQUIVOS!) do (
    set "ARQ_ESCOLHIDO=!ARQ_%%N!"
    findstr /x /c:"!ARQ_ESCOLHIDO!" "!ARQUIVO_FSM!" >nul
    if errorlevel 1 (
        set "RELATIVO=!ARQ_ESCOLHIDO:!PROJETO_NOME_ESCOLHIDO!\lib\"
        echo !NOME_ESCOLHIDO!^|!RELATIVO!>> "!ARQUIVO_FSM!"
    )
)

echo [FIM_ARQUIVOS]>>"!ARQUIVO_FSM!"

echo.
echo ====================================
echo CADASTRO CONCLUIDO
echo ====================================
echo.
goto MENU