@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: Lê o arquivo de sessão e carrega as variáveis para o escopo local
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"

:: =========================================================================
:: GERADOR_BASE.bat - O Construtor da Fundação
:: =========================================================================

::
cls
echo ============================================================================
echo         GERADOR BASE ESTRUTURAL DO SISTEMA DE FSM
echo ============================================================================
echo.
set "NOME_SISTEMA="
set /p NOME_SISTEMA=Digite o nome do NOVO SISTEMA alvo (Ex: Reservatorio) : 
echo.

call "%DIRETORIO_BIBLIOTECAS%\FORMATA_CONTEXTO.bat" ARQUIVO "%NOME_SISTEMA%" TEXTO_TRATADO
set "SISTEMA_ATUAL=!TEXTO_TRATADO!"
echo Nome do Sistema = %SISTEMA_ATUAL%
set "PASTA_SISTEMA=%DIRETORIO_ESTADOS%\!SISTEMA_ATUAL!"

:: Gera a pasta local do Sistema caso não exista
if not exist "!PASTA_SISTEMA!" mkdir "!PASTA_SISTEMA!"

:: Cria a estrutura interna para armazenamento das FSMs
if not exist "!PASTA_SISTEMA!\FSM_SIMPLES" mkdir "!PASTA_SISTEMA!\FSM_SIMPLES"
if not exist "!PASTA_SISTEMA!\FSM_SUBCATEGORIA" mkdir "!PASTA_SISTEMA!\FSM_SUBCATEGORIA"

if not exist "%ARQUIVO_SESSAO%" (
    echo [ERRO CRÍTICO] Sessão não encontrada! Execute o CONSTRUTOR_ESTADOS_SISTEMA.bat primeiro.
    pause
    exit /b
)

:: Definição dos alvos físicos com base no SISTEMA recuperado (Padronizado no Plural conforme Estrutura_Arquivos.md)
echo SISTEMA=%SISTEMA_ATUAL%>> "%ARQUIVO_SESSAO%"
echo PASTA_SISTEMA=%PASTA_SISTEMA%>> "%ARQUIVO_SESSAO%"
set "TEMPLATE_ARQ=%DIRETORIO_TEMPLATES%\Estrutura_Arquivos.md"
set "ARQ_H=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_%SISTEMA_ATUAL%.h"
set "ARQ_CPP=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_%SISTEMA_ATUAL%.cpp"

cls
echo =========================================================================
echo               CONSTRUTOR DE ESTRUTURA BASE (FUNDAÇÃO) DO %SISTEMA_ATUAL%
echo =========================================================================
echo.
echo.
:: 2. Validações Iniciais de Segurança
if not exist "%TEMPLATE_ARQ%" (
    echo [ERRO] O arquivo de modelo "%TEMPLATE_ARQ%" não foi encontrado.
    echo Certifique-se de que o arquivo 'Estrutura_Arquivos.txt' existe nesta pasta.
    pause
    exit /b
)

:: Se os arquivos já existirem, avisa o usuário por segurança.
if exist "%ARQ_H%" (
    echo [AVISO] O arquivo "%ARQ_H%" já existe.
    echo A criação da base foi cancelada para evitar sobrescrever dados existentes.
    echo Se deseja reiniciar o arquivo do zero, delete-o manualmente.
    echo.
    pause
    exit /b
)

echo Gerando arquivos estruturais...

:: Limpa arquivos temporários antigos de geração se houver resquícios
if exist "%ARQ_H%.tmp" del "%ARQ_H%.tmp"
if exist "%ARQ_CPP%.tmp" del "%ARQ_CPP%.tmp"

set "ALVO="

:: 3. Processamento e Leitura do Template (Estrutura_Arquivos.md)
for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%TEMPLATE_ARQ%"') do (
    set "LINHA=%%B"
    
    :: Limpeza temporária para testar as tags estruturais do template
    set "TESTE=!LINHA!"
    if defined TESTE (
        for /f "tokens=*" %%X in ("!TESTE!") do set "TESTE=%%X"
    )

    :: Validação das tags de escopo do arquivo template
    if "!TESTE!"=="[FILE:Estados_Sistema_{{SISTEMA}}.h]" (
        set "ALVO=H"
    ) else if "!TESTE!"=="[FILE:Estados_Sistema_{{SISTEMA}}.cpp]" (
        set "ALVO=CPP"
    ) else if "!TESTE!"=="[END::FILE]" (
        set "ALVO="
    ) else if "!TESTE!"=="[END_FILE]" (
        set "ALVO="
    ) else (
        if defined ALVO (
            :: Substituições cirúrgicas mantendo os espaçamentos originais do código
            if defined LINHA (
                :: Substitui a tag global do sistema
                set "LINHA=!LINHA:{{SISTEMA}}=%SISTEMA_ATUAL%!"
            )
            
            :: Grava no arquivo respectivo
            if "!ALVO!"=="H" (
                if defined LINHA (echo(!LINHA!>> "%ARQ_H%.tmp") else (echo(>> "%ARQ_H%.tmp")
            )
            if "!ALVO!"=="CPP" (
                if defined LINHA (echo(!LINHA!>> "%ARQ_CPP%.tmp") else (echo(>> "%ARQ_CPP%.tmp")
            )
        )
    )
)

:: 4. Validação Física Final e Finalização dos Arquivos
set "SUCESSO=1"

if exist "%ARQ_H%.tmp" (
    move /y "%ARQ_H%.tmp" "%ARQ_H%" >nul
) else (
    set "SUCESSO=0"
)

if exist "%ARQ_CPP%.tmp" (
    move /y "%ARQ_CPP%.tmp" "%ARQ_CPP%" >nul
) else (
    set "SUCESSO=0"
)

echo.
if "%SUCESSO%"=="1" (
    echo =========================================================================
    echo [SUCESSO] Estrutura Base gerada perfeitamente na lib do Arduino!
    echo.
    echo  Criado: Estados_Sistema_%SISTEMA_ATUAL%.h
    echo  Criado: Estados_Sistema_%SISTEMA_ATUAL%.cpp
    echo =========================================================================
) else (
    echo [ERRO] Falha ao processar o template de arquivos. Verifique as tags.
)
echo.
echo ENCERRANDO...
timeout /t 2 >nul
exit /b