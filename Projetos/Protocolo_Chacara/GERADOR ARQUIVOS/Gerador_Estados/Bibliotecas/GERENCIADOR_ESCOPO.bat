@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: GERENCIADOR_ESCOPO.BAT
::
:: Biblioteca para manipulação do arquivo de Escopo.
::
:: VERSÃO 1.0
::
:: Comandos:
::
::      ATUALIZA
::      PREPARAR (Verificar se Existe Histórico. Caso não exista, copia ATUAL -> ANTERIOR)
::      FINALIZAR (Copia sempre ATUAL -> ANTERIOR)
::      ATUALIZA VALORES COM OS ANTERIORES
::
:: Exemplo:
::
::      set "ARQUIVO_ESCOPO=C:\Projeto\CASA\BOMBA_CASA.md"
::
::      call GERENCIADOR_ESCOPO.bat ATUALIZA SISTEMA Reservatorio
::      call GERENCIADOR_ESCOPO.bat ATUALIZA NAMESPACE BOMBA_CASA
::
:: ============================================================================

:: ============================================================================
:: VERIFICAÇÕES
:: ============================================================================

if exist "%ARQUIVO_SESSAO%" (
    for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"
)


if "%~1"=="" (

    echo.
    echo [ERRO] Operacao nao informada.
    echo.
    echo Uso:
    echo.
    echo     GERENCIADOR_ESCOPO.bat ATUALIZA VARIAVEL nome da variavel
    echo     GERENCIADOR_ESCOPO.bat PREPARAR_HISTORICO_EDICAO
    echo     GERENCIADOR_ESCOPO.bat FINALIZAR
    echo.
    exit /b 1

)


:: ============================================================================
:: PARÂMETROS
:: ============================================================================

set "OPERACAO=%~1"
set "BLOCO=%~2"
set "CONTEUDO=%~3"

echo.
echo ================= DEBUG GERENCIADOR =================
echo OPERACAO=[%OPERACAO%]
echo BLOCO=[%BLOCO%]
echo CONTEUDO=[%CONTEUDO%]
echo ARQUIVO_ESCOPO=[%ARQUIVO_ESCOPO%]
echo =====================================================

set "TAG_INICIO=[INICIO_%BLOCO%]"
set "TAG_FIM=[FIM_%BLOCO%]"

if /I "%OPERACAO%"=="ATUALIZA" goto ATUALIZA

if /I "%OPERACAO%"=="PREPARAR_HISTORICO_EDICAO" goto PREPARAR_HISTORICO_EDICAO

if /I "%OPERACAO%"=="FINALIZAR" goto FINALIZAR

echo.
echo [ERRO] Operacao desconhecida.
exit /b

:: ============================================================================
:ATUALIZA
:: ============================================================================

set "TMP=%ARQUIVO_ESCOPO%.tmp"

set "IGNORAR=0"

(
    for /f "usebackq delims=" %%L in ("%ARQUIVO_ESCOPO%") do (

        set "LINHA=%%L"

        REM ==========================================================
        REM Encontrou o INICIO do bloco?
        REM ==========================================================

        if /I "!LINHA!"=="!TAG_INICIO!" (

            echo(!LINHA!
            echo(!CONTEUDO!

            set "IGNORAR=1"

        ) else (

            REM ======================================================
            REM Estamos ignorando o conteúdo antigo?
            REM ======================================================

            if "!IGNORAR!"=="1" (

                if /I "!LINHA!"=="!TAG_FIM!" (

                    echo(!LINHA!
                    set "IGNORAR=0"

                )

            ) else (

                echo(!LINHA!

            )

        )

    )

)>"!TMP!"


move /Y "!TMP!" "%ARQUIVO_ESCOPO%" >nul

exit /b 0


:: ============================================================================
:PREPARAR_HISTORICO_EDICAO
:: ============================================================================

echo.
echo ============================================
echo VERIFICANDO SE É A PRIMEIRA EDIÇÃO
echo ============================================
echo.
echo INICIANDO CÓPIAS
set "PRIMEIRA_EDICAO=0"

:: Verifica cada bloco anterior com segurança
call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_SISTEMA
if not defined VALOR set "PRIMEIRA_EDICAO=1"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_NAMESPACE_PRINCIPAL
if not defined VALOR set "PRIMEIRA_EDICAO=1"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_NAMESPACE_SUBCATEGORIA
if not defined VALOR set "PRIMEIRA_EDICAO=1"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_ENUM
if not defined VALOR set "PRIMEIRA_EDICAO=1"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_VARIAVEL
if not defined VALOR set "PRIMEIRA_EDICAO=1"

call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" ANTERIOR_ESTADOS
if %TOTAL_ITENS% EQU 0 set "PRIMEIRA_EDICAO=1"

if %PRIMEIRA_EDICAO% EQU 1 (
    echo [INFO] Primeira edição detectada. Copiando dados atuais para histórico...
    call :COPIAR_TODOS_ATUAIS_PARA_ANTERIORES
) else (
    echo [INFO] Histórico detectado
     call :RENOVA_HISTORICO
)

endlocal & set "PRIMEIRA_EDICAO=%PRIMEIRA_EDICAO%"
exit /b 0

::=============================================================================
:RENOVA_HISTORICO
::=============================================================================

echo [INFO] Limpando histórico anterior...

call :LIMPAR_BLOCO SISTEMA
call :LIMPAR_BLOCO NAMESPACE_PRINCIPAL
call :LIMPAR_BLOCO NAMESPACE_SUBCATEGORIA
call :LIMPAR_BLOCO ENUM
call :LIMPAR_BLOCO VARIAVEL
call :LIMPAR_BLOCO ESTADOS

call :COPIAR_TODOS_ATUAIS_PARA_ANTERIORES

exit /b

:: ============================================================================
:LIMPAR_BLOCO
:: ============================================================================

set "BLOCO_ALVO=%~1"

set "TAG_INICIO=[INICIO_ANTERIOR_%BLOCO_ALVO%]"
set "TAG_FIM=[FIM_ANTERIOR_%BLOCO_ALVO%]"

set "ARQ_TEMP=%ARQUIVO_ESCOPO%.tmp"
set "DENTRO_BLOCO=0"

(
    for /f "usebackq delims=" %%A in (`findstr /n "^" "%ARQUIVO_ESCOPO%"`) do (

        set "LINHA=%%A"
        set "LINHA=!LINHA:*:=!"

        if /i "!LINHA!"=="!TAG_INICIO!" (

            echo !LINHA!
            set "DENTRO_BLOCO=1"

        ) else if /i "!LINHA!"=="!TAG_FIM!" (

            echo !LINHA!
            set "DENTRO_BLOCO=0"

        ) else (

            if "!DENTRO_BLOCO!"=="0" (
                echo !LINHA!
            )

        )

    )

) > "!ARQ_TEMP!"

move /y "!ARQ_TEMP!" "%ARQUIVO_ESCOPO%" >nul

exit /b


:: ============================================================================
:COPIAR_TODOS_ATUAIS_PARA_ANTERIORES
:: ============================================================================

call :COPIAR_BLOCO SISTEMA
call :COPIAR_BLOCO NAMESPACE_PRINCIPAL
call :COPIAR_BLOCO NAMESPACE_SUBCATEGORIA
call :COPIAR_BLOCO ENUM
call :COPIAR_BLOCO VARIAVEL
call :COPIAR_BLOCO ESTADOS

echo [OK] Histórico inicial salvo corretamente.
exit /b

:: ============================================================================
:COPIAR_BLOCO
:: ============================================================================
set "BLOCO_ALVO=%~1"
echo BLOCO_ALVO = %BLOCO_ALVO%

:: Lê o valor ATUAL
echo lendo gerenciador escopo para copiar bloco
call "%DIRETORIO_BIBLIOTECAS%\LEITOR_ESCOPO.bat" "%BLOCO_ALVO%"
set "CONTEUDO_LINHA=!VALOR!"
set "TOTAL_LINHAS=!TOTAL_ITENS!"

:: Define as tags corretas
set "TAG_INICIO_ATUAL=[INICIO_%BLOCO_ALVO%]"
set "TAG_FIM_ATUAL=[FIM_%BLOCO_ALVO%]"
set "TAG_INICIO_ANT=[INICIO_ANTERIOR_%BLOCO_ALVO%]"
set "TAG_FIM_ANT=[FIM_ANTERIOR_%BLOCO_ALVO%]"

:: Arquivo temporário seguro
set "ARQ_TEMP=%ARQUIVO_ESCOPO%.tmp"
set "DENTRO_BLOCO=0"
(
    for /f "usebackq delims=" %%A in (`findstr /n "^" "%ARQUIVO_ESCOPO%"`) do (
        set "LINHA=%%A"
        :: Remove o número da linha gerado pelo findstr
        set "LINHA=!LINHA:*:=!"

        if /i "!LINHA!"=="!TAG_INICIO_ANT!" (
            echo !LINHA!
            set "DENTRO_BLOCO=1"

            :: Grava o conteúdo ATUAL dentro do bloco ANTERIOR
            if /i "%BLOCO_ALVO%"=="ESTADOS" (
                for /L %%i in (1,1,%TOTAL_LINHAS%) do (
                    echo !ITEM%%i!
                )
            ) else (
                :: Força o tratamento como texto com aspas
                if defined CONTEUDO_LINHA (
                    echo !CONTEUDO_LINHA!
                )
            )

        ) else if /i "!LINHA!"=="!TAG_FIM_ANT!" (
            echo !LINHA!
            set "DENTRO_BLOCO=0"

        ) else if "!DENTRO_BLOCO!"=="0" (
            echo !LINHA!
        )
    )
) > "!ARQ_TEMP!"

move /y "!ARQ_TEMP!" "%ARQUIVO_ESCOPO%" >nul
if errorlevel 1 echo [ERRO] Falha ao atualizar bloco !BLOCO_ALVO!

exit /b
