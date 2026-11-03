@echo off
::setlocal EnableDelayedExpansion

:: ============================================================================
:: FSM_CONTEXTO.BAT
::
:: Carrega completamente uma FSM para memória.
::
:: Responsabilidade
::
::     Ler o Escopo através do LEITOR_ESCOPO.
::     Normalizar todos os nomes.
::     Disponibilizar toda a FSM pronta para consumo.
::
:: NÃO altera arquivos.
:: NÃO gera código.
:: NÃO cria IDs.
:: NÃO modifica Escopo.md.
::
:: ============================================================================


:: ============================================================================
:: VALIDAÇÕES
:: ============================================================================

if not defined DIRETORIO_BIBLIOTECAS (
    echo.
    echo [ERRO] DIRETORIO_BIBLIOTECAS nao definido.
    exit /b 1
)

if not defined NOMENCLATURA_BAT (
    echo.
    echo [ERRO] NOMENCLATURA_BAT nao definido.
    exit /b 1
)

if not defined ARQUIVO_ESCOPO (
    echo.
    echo [ERRO] ARQUIVO_ESCOPO nao definido.
    exit /b 1
)

:: ============================================================================
:: LIMPEZA
:: ============================================================================

for /f "delims==" %%V in ('set ESTADO 2^>nul') do set "%%V="
for /f "delims==" %%V in ('set ANTERIOR_ESTADO 2^>nul') do set "%%V="

set "SISTEMA="
set "NAMESPACE_PRINCIPAL="
set "NAMESPACE_SUBCATEGORIA="
set "NAMESPACE_FUNCAO="
set "ENUM="
set "VARIAVEL="

set "TOTAL_ESTADOS=0"
set "TOTAL_ANTERIOR_ESTADOS=0"

set "EXISTE_ANTERIOR=0"

set "TOTAL_IDS=0"
set "TOTAL_SEM_IDS=0"
set "POSSUI_IDS=0"


:: ============================================================================
:: DADOS GERAIS
:: ============================================================================

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" SISTEMA
set "SISTEMA=%VALOR%"
echo Sistema........... %SISTEMA%

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" NAMESPACE_PRINCIPAL
set "NAMESPACE_PRINCIPAL=%VALOR%"
echo Namespace Principal......... %NAMESPACE_PRINCIPAL%

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" NAMESPACE_SUBCATEGORIA
set "NAMESPACE_SUBCATEGORIA=%VALOR%"
echo Namespace Subcategoria......... %NAMESPACE_SUBCATEGORIA%

:: NAMESPACE FUNÇÃO
call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" NAMESPACE_PRINCIPAL
call "%NOMENCLATURA_BAT%" "%VALOR%" PASCAL
set "NAMESPACE_FUNCAO=%TEXTO_TRATADO%"
echo Namespace Função........ %NAMESPACE_FUNCAO%

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ENUM
call "%NOMENCLATURA_BAT%" "%VALOR%" UPPER
set "ENUM=%TEXTO_TRATADO%"
echo Enum.............. %ENUM%

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" VARIAVEL
call "%NOMENCLATURA_BAT%" "%VALOR%" CAMEL
set "VARIAVEL=%TEXTO_TRATADO%"
echo Variavel.......... %VARIAVEL%

:: ============================================================================
:: ESTADOS ATUAIS
:: ============================================================================

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ESTADOS

echo.
echo ===== DEBUG LEITOR =====
echo TOTAL_ITENS=[%TOTAL_ITENS%]

set ITEM

set "TOTAL_ESTADOS=%TOTAL_ITENS%"

for /L %%i in (1,1,!TOTAL_ESTADOS!) do (

echo.
echo ===== LOOP ESTADO : %%i =====

call echo ITEM%%i = %%ITEM%%i%%

call set "LINHA=%%ITEM%%i%%"

echo LINHA=[!LINHA!]



    rem ---------------------------------------------------------
    rem Separação futura de ID
    rem ---------------------------------------------------------

    set "ESTADO_ID%%i="

    rem Quando existir:
    rem 0005|Estado Normal
    rem aqui será separado automaticamente.

    set "ESTADO=!LINHA!"

    rem ---------------------------------------------------------
    rem Texto Original
    rem ---------------------------------------------------------

    set "ESTADO%%i=!ESTADO!"

    rem ---------------------------------------------------------
    rem ENUM
    rem ---------------------------------------------------------

    call "%NOMENCLATURA_BAT%" "!ESTADO!" UPPER
    set "ESTADO_UPPER%%i=!TEXTO_TRATADO!"


    rem ---------------------------------------------------------
    rem FUNÇÃO
    rem ---------------------------------------------------------

    call "%NOMENCLATURA_BAT%" "!ESTADO!" PASCAL
    set "ESTADO_PASCAL%%i=!TEXTO_TRATADO!"

)

:: ============================================================================
:: ESTADOS ANTERIORES
:: ============================================================================

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_ESTADOS

set "TOTAL_ANTERIOR_ESTADOS=%TOTAL_ITENS%"

if %TOTAL_ANTERIOR_ESTADOS% GTR 0 set EXISTE_ANTERIOR=1

for /L %%i in (1,1,!TOTAL_ANTERIOR_ESTADOS!) do (

    call set "LINHA=%%ITEM%%i%%"

    set "ANTERIOR_ESTADO_ID%%i="

    set "ESTADO=!LINHA!"

    set "ANTERIOR_ESTADO%%i=!ESTADO!"

    call "%NOMENCLATURA_BAT%" "!ESTADO!" UPPER
    set "ANTERIOR_ESTADO_UPPER%%i=!TEXTO_TRATADO!"
    echo TEXTO_TRATADO UPPER=[!TEXTO_TRATADO!]


    call "%NOMENCLATURA_BAT%" "!ESTADO!" PASCAL
    set "ANTERIOR_ESTADO_PASCAL%%i=!TEXTO_TRATADO!"
    echo TEXTO_TRATADO PASCAL=[!TEXTO_TRATADO!]


)

:: ============================================================================
:: EXPORTAÇÃO
:: ============================================================================

(
    echo set "SISTEMA=%SISTEMA%"
    echo set "NAMESPACE_PRINCIPAL=%NAMESPACE_PRINCIPAL%"
    echo set "NAMESPACE_SUBCATEGORIA=%NAMESPACE_SUBCATEGORIA%"
    echo set "NAMESPACE_FUNCAO=%NAMESPACE_FUNCAO%"
    echo set "ENUM=%ENUM%"
    echo set "VARIAVEL=%VARIAVEL%"

    echo set "TOTAL_ESTADOS=%TOTAL_ESTADOS%"
    echo set "TOTAL_ANTERIOR_ESTADOS=%TOTAL_ANTERIOR_ESTADOS%"

    echo set "EXISTE_ANTERIOR=%EXISTE_ANTERIOR%"
    echo set "TOTAL_IDS=%TOTAL_IDS%"
    echo set "TOTAL_SEM_IDS=%TOTAL_SEM_IDS%"
    echo set "POSSUI_IDS=%POSSUI_IDS%"

    for /L %%i in (1,1,%TOTAL_ESTADOS%) do (

        call echo set "ESTADO%%i=%%ESTADO%%i%%"
        call echo set "ESTADO_UPPER%%i=%%ESTADO_UPPER%%i%%"
        call echo set "ESTADO_PASCAL%%i=%%ESTADO_PASCAL%%i%%"
        call echo set "ESTADO_ID%%i=%%ESTADO_ID%%i%%"

    )

    for /L %%i in (1,1,%TOTAL_ANTERIOR_ESTADOS%) do (

        call echo set "ANTERIOR_ESTADO%%i=%%ANTERIOR_ESTADO%%i%%"
        call echo set "ANTERIOR_ESTADO_UPPER%%i=%%ANTERIOR_ESTADO_UPPER%%i%%"
        call echo set "ANTERIOR_ESTADO_PASCAL%%i=%%ANTERIOR_ESTADO_PASCAL%%i%%"
        call echo set "ANTERIOR_ESTADO_ID%%i=%%ANTERIOR_ESTADO_ID%%i%%"

    )

) > "%TEMP%\FSM_EXPORT.tmp"

endlocal

call "%TEMP%\FSM_EXPORT.tmp"
del "%TEMP%\FSM_EXPORT.tmp"

exit /b