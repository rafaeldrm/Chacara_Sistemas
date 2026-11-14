@echo off
chcp 65001 >nul


:: ============================================================================
:: FORMATA_CONTEXTO.BAT
::
:: Biblioteca para Tratamento de Textos e Padronização para Variáveis, Funções, Arquivos e etc.
::
:: VERSÃO 1.0
::
:: Comandos:
::
::      ARQUIVO
::      PASTA
::      FUNCAO
::      VARIAVEL
::      VARIAVEL_FIXA
::      ENUM
::      ESTADO
::
:: EXEMPLO DE COMANDO NO BAT QUE CHAMA FORMATA_CONTEXTO.bat
::
::    call "%DIRETORIO_BIBLIOTECAS%\FORMATA_CONTEXTO.bat" ARQUIVO "%NOME_VARIAVEL%" TEXTO_FORMATADO
::    set "SISTEMA_ATUAL=!TEXTO_FORMATADOs!"
::
::
::
::   call "Chama o FORMATA_CONTEXTO.bat obedecendo o endereço completo"
::    ARQUIVO = é o nome do comando a ser usado no FORMATA_CONTEXTO.bat, o comando é que define o eestilo da formatação no caso ARQUIVO sempre deixará o texto em PASCAL
::    "%NOME_VARIAVEL%" = variavel criada dentro do .bat que chama o FORMATA_CONTEXTO, é o texto a ser tratado e convertido para um estilo PASCAL, CAMEL, UPPER....
::    TEXTO_FORMATADO = é a variável que é retornada para o bat que chamou o FORMATA_CONTEXTO.bat, é o texto formatado e configurado. 
::
:: ============================================================================

:: ============================================================================
:: VERIFICAÇÕES
:: ============================================================================

if "%~1"=="" (
    echo [ERRO] Contexto nao informado.
    exit /b 1
)

if "%~2"=="" (
    echo [ERRO] Texto nao informado.
    exit /b 1
)

if "%~3"=="" (
    echo [ERRO] Variavel destino nao informada.
    exit /b 1
)

::==============================================================================
:: PADRONIZAÇÕES
::==============================================================================


set "FORMATO="

if /I "%~1"=="ARQUIVO" set "FORMATO=PASCAL"
if /I "%~1"=="PASTA" set "FORMATO=PASCAL"
if /I "%~1"=="FUNCAO" set "FORMATO=PASCAL"

if /I "%~1"=="VARIAVEL" set "FORMATO=CAMEL"

if /I "%~1"=="ENUM" set "FORMATO=UPPER"
if /I "%~1"=="ESTADO" set "FORMATO=UPPER"
if /I "%~1"=="VARIAVEL_FIXA" set "FORMATO=UPPER"
if /I "%~1"=="NAMESPACE_PRINCIPAL" set "FORMATO=UPPER"
if /I "%~1"=="NAMESPACE_SUBCATEGORIA" set "FORMATO=UPPER"

if not defined FORMATO (
    echo [ERRO] Contexto "%~1" invalido.
    exit /b 1
)

call "%NOMENCLATURA_BAT%" "%~2" %FORMATO%
set "%~3=%TEXTO_TRATADO%"
exit /b