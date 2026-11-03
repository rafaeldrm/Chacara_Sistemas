@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: =========================================================================
:: CONSTRUTOR_ESTADOS_SISTEMA.bat - O Maestro (Ultra Auto - Revisado)
:: =========================================================================
:: % para variável antiga do bloco de execução
:: ! para variável "atualizada" do bloco de execução
:: ~n, ~x, ~dp, etc, são modificadores de parâmetro do FOR

set "BASE=%~dp0"
for %%i in ("%BASE%..\..") do set "LIB=%%~fi"
for %%i in ("%BASE%..") do set "DIRETORIO_RAIZ=%%~fi"
for %%i in ("%BASE%..\..\..") do set "PASTA_PROJETO=%%~fi"
set "DIRETORIO_PROJETOS=%PASTA_PROJETO%"
set "DIRETORIO_ESTADOS=%BASE%Estados"
set "DIRETORIO_SISTEMA_ESTADOS=%LIB%\lib\Estados_Sistemas"
set "NOMENCLATURA_BAT=%DIRETORIO_RAIZ%\Nomenclatura\NOMENCLATURA.bat"
set "ARQUIVO_SESSAO=%BASE%temp\sessao.tmp"
set "DIRETORIO_BIBLIOTECAS=%BASE%Bibliotecas"
set "DIRETORIO_TEMP=%BASE%temp"
set "DIRETORIO_TEMPLATES=%BASE%Templates"

:: Garante que as pastas de suporte essenciais existam
if not exist "%DIRETORIO_ESTADOS%" mkdir "%DIRETORIO_ESTADOS%"
if not exist "%DIRETORIO_SISTEMA_ESTADOS%" mkdir "%DIRETORIO_SISTEMA_ESTADOS%"

:MENU
cls
echo ===================================================
echo         CONSTRUTOR ESTADOS COMPLETO (FSM)
echo ===================================================
echo.
echo  1 - Criar Estrutura Base (Arquivos .h e .cpp vazios)
echo  2 - Criar nova FSM (Novo Estado/Namespace)
echo  3 - Editar/Atualizar FSM Existente
echo  4 - Registrar Arquivos que usam o FSM Existente.
echo  0 - Sair
echo.
echo ===================================================
echo.
set "OPCAO="
set /p OPCAO=Digite o numero da opcao e aperte ENTER : 
echo.

::encerra o .bat ==================================================================
if "%OPCAO%"=="0" (
    if exist "%ARQUIVO_SESSAO%" del "%ARQUIVO_SESSAO%"
    exit /b
)
::opções escolhidas ================================================================
if "%OPCAO%"=="1" goto OPCAO_NOVA_BASE
if "%OPCAO%"=="2" goto OPCAO_SISTEMA_EXISTENTE
if "%OPCAO%"=="3" goto OPCAO_SISTEMA_EXISTENTE
if "%OPCAO%"=="4" goto OPCAO_SISTEMA_EXISTENTE

echo [ERRO] Opção inválida.
pause
goto MENU

::==============================================================================
::  OPÇÃO 1 = PARA CRIAR ARQUIVOS .H e .CPP
::==============================================================================

:OPCAO_NOVA_BASE
set "NOME_SISTEMA="
set /p NOME_SISTEMA=Digite o nome do NOVO SISTEMA alvo (Ex: Reservatorio) : 
echo.
if "%NOME_SISTEMA%"==" " goto OPCAO_NOVA_BASE

call "%NOMENCLATURA_BAT%" "%NOME_SISTEMA%" PASCAL
set "SISTEMA_ATUAL=!TEXTO_TRATADO!"
set "NAMESPACE_ATUAL=NENHUM"
set "PASTA_SISTEMA=%DIRETORIO_ESTADOS%\!SISTEMA_ATUAL!"

:: Gera a pasta local do Sistema caso não exista
if not exist "!PASTA_SISTEMA!" mkdir "!PASTA_SISTEMA!"

goto SALVAR_SESSAO_E_EXECUCAO

::==========================================================================
::             OPÇÕES 2, 3 e 4.
::==========================================================================

:OPCAO_SISTEMA_EXISTENTE
cls
set "CONTADOR=0"

:: ALINHAMENTO CRÍTICO: Procura pelo padrão exato do Gerador Base (Estados_Sistema_*.h)
:: %% indica ao Batch que aquilo é a variável do laço FOR.
:: No arquivo BAT precisa dobrar o %, para %%, diferente do uso direto no cmd que é apenas %
:: a letra F poderia ser qualquer outra letra, nunca uma palavra porque no laço FOR é correto apenas 1 caractere
:: ~ = Ativa o modo de modificação de caminhos.
:: n = Extrai apenas o nome do arquivo (Name), descartando a pasta e a extensão.
:: /a = cálculo aritmético (contador e incremento)

for %%F in ("%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_*.h") do (
    set "ARQ_NOME=%%~nF"
    set "SISTEMA_NOME=!ARQ_NOME:Estados_Sistema_=!"
    set /a CONTADOR+=1
    set "SISTEMA_!CONTADOR!=!SISTEMA_NOME!"
    echo  !CONTADOR! - !SISTEMA_NOME!
)

echo ===================================================
echo               SELECIONE O SISTEMA ALVO
echo ===================================================
echo.
echo.
set "ESCOLHA_SISTEMA="
set /p ESCOLHA_SISTEMA=Digite o número do sistema desejado: 
echo.

if not defined SISTEMA_%ESCOLHA_SISTEMA% (
    echo [ERRO] Opção inválida!
    pause
    goto OPCAO_SISTEMA_EXISTENTE
)
:: % e o ! juntos para fazer o Bat ler uma variável de dentro da outra.
:: Primeiro, ele lê os % (Variáveis Comuns): Ele olha para %ESCOLHA_SISTEMA%. 
:: quando escolhe o Sistema, ele substitui esse trecho na hora.  a linha fica internamente
:: set "SISTEMA_ATUAL=!SISTEMA_X!"Segundo, ele lê os ! (Expansão Retardada): Agora ele busca o valor da variável chamada SISTEMA_X
:: e o entrega para a SISTEMA_ATUAL.

set "SISTEMA_ATUAL=!SISTEMA_%ESCOLHA_SISTEMA%!"
set "PASTA_SISTEMA=%DIRETORIO_ESTADOS%\!SISTEMA_ATUAL!"
if not exist "!PASTA_SISTEMA!" mkdir "!PASTA_SISTEMA!"

:: Roteamento do fluxo interno
if "%OPCAO%"=="2" goto FLUXO_CRIAR_FSM
if "%OPCAO%"=="3" goto FLUXO_EDITAR_FSM
if "%OPCAO%"=="4" goto OPCAO_CADASTRAMENTO

:FLUXO_CRIAR_FSM
cls
echo ===================================================
echo     CRIANDO NOVA FSM PARA: !SISTEMA_ATUAL!
echo ===================================================
echo.
set "NOME_MODULO="
set /p NOME_MODULO=Nome do modulo FSM (Ex: BombaCasa) : 
echo.

call "%NOMENCLATURA_BAT%" "%NOME_MODULO%" UPPER
set "NAMESPACE_ATUAL=!TEXTO_TRATADO!"


:: ==========================================================================
:: GERAÇÃO E ATUALIZAÇÃO DO ESCOPO DA FSM (100% NATIVO)
:: ==========================================================================
set "ARQ_TEMPLATE=%DIRETORIO_TEMPLATES%\Escopo.md"
set "ARQ_DESTINO=!PASTA_SISTEMA!\!NAMESPACE_ATUAL!.md"

if exist "!ARQ_DESTINO!" (

    echo.
    echo [INFO] Esta FSM ja existe.
    echo.
    echo Abrindo o arquivo existente...
    echo.

) else (

     echo.
    echo [INFO] Criando nova FSM...
    echo.

    copy "%ARQ_TEMPLATE%" "!ARQ_DESTINO!" >nul

    set "ARQUIVO_ESCOPO=!ARQ_DESTINO!"

    call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA SISTEMA "!SISTEMA_ATUAL!"
    call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA NAMESPACE "!NAMESPACE_ATUAL!"


    if errorlevel 1 (
        echo.
        echo [ERRO] Falha ao copiar Escopo.md
        pause
        goto MENU
    )

)

echo [INFO] Abrindo o escopo no VS Code. Edite, salve e feche o arquivo para continuar...
call code --wait "!ARQ_DESTINO!" >nul
goto SALVAR_SESSAO_E_EXECUCAO


:FLUXO_EDITAR_FSM
cls
echo ===================================================
echo         ARQUIVOS FSM DE: !SISTEMA_ATUAL!
echo ===================================================
echo.
set "CONTADOR_TXT=0"

echo =================================================
echo LISTA DE FSM's DISPONÍVEIS PARA EDIÇÃO
echo =================================================

for %%T in ("!PASTA_SISTEMA!\*.md") do (
    set /a CONTADOR_TXT+=1
    set "TXT_NAME=%%~nT"
    set "ED_!CONTADOR_TXT!=!TXT_NAME!"
    echo !CONTADOR_TXT! - !TXT_NAME!
)

if !CONTADOR_TXT! EQU 0 (
    echo [AVISO] Nenhuma FSM configurada para o sistema !SISTEMA_ATUAL!.
    echo Use a opção 2 para criar uma nova FSM primeiro.
    pause
    goto MENU
)

echo.
set "ESCOLHA_TXT="
set /p ESCOLHA_TXT=Digite o número do arquivo FSM que deseja editar: 
echo.

if not defined ED_%ESCOLHA_TXT% (
    echo [ERRO] Opção inválida!
    pause
    goto FLUXO_EDITAR_FSM
)

set "NAMESPACE_ATUAL=!ED_%ESCOLHA_TXT%!"
set "ARQUIVO_ESCOPO=!PASTA_SISTEMA!\!NAMESPACE_ATUAL!.md"
echo ABRINDO O GERENCIADOR ESCOPO PREPARAR 
call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" VERIFICAR_SE_E_PRIMEIRA_EDICAO
echo [INFO] Abrindo para edição no VS Code...
call code --wait "!PASTA_SISTEMA!\!NAMESPACE_ATUAL!.md" >nul
goto SALVAR_SESSAO_E_EXECUCAO


:SALVAR_SESSAO_E_EXECUCAO
:: =========================================================================
:: GRAVAÇÃO DA SESSÃO TEMPORÁRIA
:: =========================================================================
echo SISTEMA=!SISTEMA_ATUAL!> "%ARQUIVO_SESSAO%"
echo NAMESPACE=!NAMESPACE_ATUAL!>> "%ARQUIVO_SESSAO%"
echo BASE=%BASE%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_SISTEMA_ESTADOS=%DIRETORIO_SISTEMA_ESTADOS%>> "%ARQUIVO_SESSAO%"
echo PASTA_SISTEMA=!PASTA_SISTEMA!>> "%ARQUIVO_SESSAO%"
echo NOMENCLATURA_BAT=%NOMENCLATURA_BAT%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_BIBLIOTECAS=%DIRETORIO_BIBLIOTECAS%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_TEMP=%DIRETORIO_TEMP%>> "%ARQUIVO_SESSAO%"
echo DIRETORIO_TEMPLATES=%DIRETORIO_TEMPLATES%>> "%ARQUIVO_SESSAO%"


if "%OPCAO%"=="1" goto EXECUTA_BASE
if "%OPCAO%"=="2" goto EXECUTA_CRIA_FSM
if "%OPCAO%"=="3" goto EXECUTA_EDITA_FSM

:EXECUTA_BASE
echo [PROCESSANDO] Gerando arquivos de esqueleto inicial (.h / .cpp)...
call "%BASE%GERADOR_BASE.bat"
goto MENU

:EXECUTA_CRIA_FSM
echo [PROCESSANDO] Compilando dados do escopo e gerando blocos C++...
call "%BASE%CRIAR_FSM.bat"
goto MENU

:EXECUTA_EDITA_FSM
echo [PROCESSANDO] Atualizando blocos da FSM nos arquivos do sistema...
call "%BASE%EDITA_FSM.bat"
goto MENU

::============================================================================
::              OPÇÃO 4
::============================================================================

:OPCAO_CADASTRAMENTO

cls
echo ===================================================
echo         SELECIONE A FSM
echo ===================================================
echo.

set "CONTADOR_TXT=0"

for %%T in ("!PASTA_SISTEMA!\*.md") do (
    findstr /I /C:"SISTEMA=!SISTEMA_ATUAL!" "%%T" >nul

    if !errorlevel! EQU 0 (
        set /a CONTADOR_TXT+=1
        set "TXT_NAME=%%~nT"
        set "ED_!CONTADOR_TXT!=!TXT_NAME!"
        echo !CONTADOR_TXT! - !TXT_NAME!.md
    )
)

if !CONTADOR_TXT! EQU 0 (
    echo.
    echo Nenhuma FSM encontrada.
    pause
    goto MENU
)

echo.
set /p ESCOLHA_TXT=Digite o numero da FSM:
echo.

if not defined ED_%ESCOLHA_TXT% (
    echo Opcao invalida.
    pause
    goto OPCAO_CADASTRAMENTO
)

set "NAMESPACE_ATUAL=!ED_%ESCOLHA_TXT%!"

::======================================================
:: ESCOLHA DO PROJETO
::======================================================

cls
echo ===================================================
echo         PROJETOS DISPONIVEIS
echo ===================================================
echo.

set "CONTADOR=0"

for /d %%F in ("%DIRETORIO_PROJETOS%\*") do (

    set /a CONTADOR+=1

    set "PASTA_!CONTADOR!=%%F"
    set "NOME_!CONTADOR!=%%~nxF"

    echo !CONTADOR! - %%~nxF

)

echo.
set /p NUMERO_PROJETO=Digite o numero do projeto:
echo.

set "PASTA_ESCOLHIDA=!PASTA_%NUMERO_PROJETO%!"
set "NOME_ESCOLHIDO=!NOME_%NUMERO_PROJETO%!"

if "!PASTA_ESCOLHIDA!"=="" (
    echo Opcao invalida.
    pause
    goto OPCAO_CADASTRAMENTO
)

set "PASTA_LIB=!PASTA_ESCOLHIDA!\lib"

if not exist "!PASTA_LIB!" (
    echo Pasta lib nao encontrada.
    pause
    goto MENU
)

::======================================================
:: LISTA PASTAS DA LIB
::======================================================

cls
echo ===================================================
echo         PASTAS DA LIB
echo ===================================================
echo.

set "PASTA_CONTADOR=0"

for /d %%D in ("!PASTA_LIB!\*") do (

    set /a PASTA_CONTADOR+=1

    set "LIBPASTA_!PASTA_CONTADOR!=%%~fD"
    set "LIBNOME_!PASTA_CONTADOR!=%%~nxD"

    echo !PASTA_CONTADOR! - %%~nxD

)

echo.
set /p ESCOLHA_PASTAS=Digite os numeros das pastas (Ex: 1 3 5):
echo.

::======================================================
:: LISTA ARQUIVOS
::======================================================

cls
echo ===================================================
echo         ARQUIVOS ENCONTRADOS
echo ===================================================
echo.

set "FILE_CONTADOR=0"

for %%N in (!ESCOLHA_PASTAS!) do (

    set "PASTA_ATUAL=!LIBPASTA_%%N!"

    echo.
    echo Varrendo:
    echo !PASTA_ATUAL!
    echo.

    for /f "delims=" %%F in ('dir /s /b "!PASTA_ATUAL!\*.*" 2^>nul') do (

        set /a FILE_CONTADOR+=1

        set "ARQ_!FILE_CONTADOR!=%%~fF"

        echo !FILE_CONTADOR! - %%~nxF

    )
)

if !FILE_CONTADOR! EQU 0 (

    echo.
    echo Nenhum arquivo encontrado.
    pause
    goto MENU

)

echo.
set /p ESCOLHA_ARQUIVOS=Digite os numeros dos arquivos (Ex: 1 3 5):
echo.

::======================================================
:: CONFIRMAÇÃO
::======================================================

cls
echo ===================================================
echo         CONFIRMACAO
echo ===================================================
echo.

for %%N in (!ESCOLHA_ARQUIVOS!) do (

    set "ARQ_ESCOLHIDO=!ARQ_%%N!"

    echo [X] !ARQ_ESCOLHIDO!

)

echo.
set /p CONFIRMA=Confirmar cadastro? (S/N):

if /I not "!CONFIRMA!"=="S" (
    goto MENU
)

::======================================================
:: SALVAR NO .MD DA FSM
::======================================================

set "ARQUIVO_FSM=!DIRETORIO_ESTADOS!\!SISTEMA_ATUAL!\!NAMESPACE_ATUAL!.md"
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

move /y "!TEMP_FILE!" "!ARQUIVO_FSM!" >nul

echo.>>"!ARQUIVO_FSM!"
echo [INICIO_ARQUIVOS]>>"!ARQUIVO_FSM!"

for %%N in (!ESCOLHA_ARQUIVOS!) do (
    set "ARQ_ESCOLHIDO=!ARQ_%%N!"
    findstr /x /c:"!ARQ_ESCOLHIDO!" "!ARQUIVO_FSM!" >nul
    if errorlevel 1 (
        set "RELATIVO=!ARQ_ESCOLHIDO:%PASTA_ESCOLHIDA%\lib\=!"
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