@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: FSM_SIMPLES.bat  (versão reforçada)
:: Cria uma nova FSM Simples e injeta de forma cirúrgica e segura
:: ============================================================================

:: Carrega sessão
if not exist "%ARQUIVO_SESSAO%" (
    echo.
    echo [ERRO] Sessao nao encontrada. Execute o FSM.bat primeiro.
    pause
    exit /b 1
)
for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"

cls
echo ===================================================
echo      CRIANDO NOVA FSM SIMPLES PARA: !SISTEMA_ATUAL!
echo ===================================================
echo.

:: ------------------------------------------------------------------------
:: 1. Coleta do nome do módulo
:: ------------------------------------------------------------------------
set "NOME_MODULO="
set /p "NOME_MODULO=Nome do modulo FSM (Ex: BombaCasa): "
if "!NOME_MODULO!"=="" (
    echo [ERRO] Nome do modulo nao pode ser vazio.
    pause
    exit /b 1
)

call "%DIRETORIO_BIBLIOTECAS%\FORMATA_CONTEXTO.bat" NAMESPACE_PRINCIPAL "!NOME_MODULO!" TEXTO_TRATADO
set "NAMESPACE_PRINCIPAL=!TEXTO_TRATADO!"
echo Namespace principal = !NAMESPACE_PRINCIPAL!
echo.

:: ------------------------------------------------------------------------
:: 2. Cria / prepara o arquivo de Escopo
:: ------------------------------------------------------------------------
set "ARQ_TEMPLATE=%DIRETORIO_TEMPLATES%\Escopo.md"
set "ARQ_DESTINO=!PASTA_SISTEMA!\FSM_SIMPLES\!NAMESPACE_PRINCIPAL!.md"
set "ARQUIVO_ESCOPO=!ARQ_DESTINO!"

if not exist "!PASTA_SISTEMA!\FSM_SIMPLES" mkdir "!PASTA_SISTEMA!\FSM_SIMPLES"

if exist "!ARQ_DESTINO!" (
    echo [AVISO] Ja existe um Escopo para !NAMESPACE_PRINCIPAL!.
    echo Se continuar, o arquivo atual sera sobrescrito pelo template.
    choice /c SN /m "Deseja sobrescrever"
    if errorlevel 2 exit /b 0
)

copy /y "%ARQ_TEMPLATE%" "!ARQ_DESTINO!" >nul
if errorlevel 1 (
    echo [ERRO] Falha ao copiar Escopo.md
    pause
    exit /b 1
)

:: Preenche SISTEMA e NAMESPACE no Escopo
call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA SISTEMA "!SISTEMA_ATUAL!"
call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA NAMESPACE_PRINCIPAL "!NAMESPACE_PRINCIPAL!"

echo.
echo [INFO] Abrindo o Escopo no VS Code.
echo        Edite ENUM, VARIAVEL e ESTADOS, salve e feche o arquivo para continuar...
echo.
call code --wait "!ARQ_DESTINO!"

:: ------------------------------------------------------------------------
:: 3. Carrega o contexto completo da FSM
:: ------------------------------------------------------------------------
echo.
echo ==========================================
echo CARREGANDO CONTEXTO DA FSM...
echo ==========================================
echo.

call "%DIRETORIO_BIBLIOTECAS%\FSM_CONTEXTO.bat"
if errorlevel 1 (
    echo [ERRO] Falha ao carregar o contexto da FSM.
    pause
    exit /b 1
)

if "!TOTAL_ESTADOS!"=="0" (
    echo [ERRO] Nenhum estado encontrado no Escopo.
    pause
    exit /b 1
)

set "PRIMEIRO_ESTADO=!ESTADO_UPPER1!"

echo Sistema............... !SISTEMA!
echo Namespace Principal... !NAMESPACE_PRINCIPAL!
echo Enum.................. !ENUM!
echo Variavel.............. !VARIAVEL!
echo Total de Estados...... !TOTAL_ESTADOS!
echo Primeiro Estado....... !PRIMEIRO_ESTADO!
echo.

for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
    call echo   %%i - %%ESTADO_UPPER%%i%%
)
echo.

:: ------------------------------------------------------------------------
:: 4. Define caminhos
:: ------------------------------------------------------------------------
set "ARQ_H=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_!SISTEMA!.h"
set "ARQ_CPP=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_!SISTEMA!.cpp"
set "TEMPLATE_ESTADOS=%DIRETORIO_TEMPLATES%\Estrutura_Estados.md"

set "TMP_H=%DIRETORIO_TEMP%\_bloco_!NAMESPACE_PRINCIPAL!_h.tmp"
set "TMP_CPP=%DIRETORIO_TEMP%\_bloco_!NAMESPACE_PRINCIPAL!_cpp.tmp"

if not exist "%ARQ_H%" (
    echo [ERRO] Arquivo nao encontrado: %ARQ_H%
    pause
    exit /b 1
)
if not exist "%ARQ_CPP%" (
    echo [ERRO] Arquivo nao encontrado: %ARQ_CPP%
    pause
    exit /b 1
)
if not exist "%TEMPLATE_ESTADOS%" (
    echo [ERRO] Template Estrutura_Estados.md nao encontrado.
    pause
    exit /b 1
)

:: Limpa temporários antigos
if exist "%TMP_H%" del "%TMP_H%"
if exist "%TMP_CPP%" del "%TMP_CPP%"

:: ------------------------------------------------------------------------
:: 5. Gera os blocos completos da FSM (a partir do template)
:: ------------------------------------------------------------------------
echo Gerando blocos da FSM...

call :GERAR_BLOCOS
if errorlevel 1 (
    echo [ERRO] Falha ao gerar os blocos.
    pause
    exit /b 1
)

:: ------------------------------------------------------------------------
:: 6. Injeta os blocos ANTES das tags de inserção
:: ------------------------------------------------------------------------
echo Injetando no arquivo .h...
call :INJETAR_ANTES_DA_TAG "%ARQ_H%" "// [INSERE::BAT::ESTADOS_H_!SISTEMA!]" "%TMP_H%"
if errorlevel 1 (
    echo [ERRO] Falha na injecao do .h
    pause
    exit /b 1
)

echo Injetando no arquivo .cpp...
call :INJETAR_ANTES_DA_TAG "%ARQ_CPP%" "// [INSERE::BAT::ESTADOS_CPP_!SISTEMA!]" "%TMP_CPP%"
if errorlevel 1 (
    echo [ERRO] Falha na injecao do .cpp
    pause
    exit /b 1
)

:: ------------------------------------------------------------------------
:: 7. Injeta a variável no struct e a inicialização no static
:: ------------------------------------------------------------------------
echo Atualizando struct e inicializacao...
call :INJETAR_STRUCT_E_STATIC
if errorlevel 1 (
    echo [ERRO] Falha ao atualizar struct/static
    pause
    exit /b 1
)

:: Limpeza
if exist "%TMP_H%" del "%TMP_H%"
if exist "%TMP_CPP%" del "%TMP_CPP%"

echo.
echo ====================================================
echo [SUCESSO] FSM '!NAMESPACE_PRINCIPAL!' criada e injetada com sucesso!
echo ====================================================
echo.
pause
exit /b 0


:: ========================================================================
::  FUNÇÕES AUXILIARES
:: ========================================================================

:GERAR_BLOCOS
if exist "%TMP_H%" del "%TMP_H%"
if exist "%TMP_CPP%" del "%TMP_CPP%"

set "ALVO="

for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%TEMPLATE_ESTADOS%"') do (
    set "L=%%B"
    set "C=!L!"s
    if defined C for /f "tokens=*" %%X in ("!C!") do set "C=%%X"

    if "!C!"=="[FILE:Estados_Sistema_{{SISTEMA}}.h]" (
        set "ALVO=H"
    ) else if "!C!"=="[FILE:Estados_Sistema_{{SISTEMA}}.cpp]" (
        set "ALVO=CPP"
    ) else if "!C!"=="[END_FILE]" (
        set "ALVO="
    ) else (
        if defined ALVO (
            if defined L (
                rem ===== Substituições seguras =====
                for %%S in ("!SISTEMA!") do set "L=!L:{{SISTEMA}}=%%~S!"
                for %%N in ("!NAMESPACE_PRINCIPAL!") do set "L=!L:{{NAMESPACE}}=%%~N!"
                for %%E in ("!ENUM!") do set "L=!L:{{ENUM}}=%%~E!"
                for %%V in ("!VARIAVEL!") do set "L=!L:{{VARIAVEL}}=%%~V!"
            )

            if "!ALVO!"=="H" (
                if defined L (
                    echo !L! | findstr /c:"{{ESTADOS}}" >nul
                    if not errorlevel 1 (
                        for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                            >>"%TMP_H%" echo        !ESTADO_UPPER%%i!,
                        )
                    ) else (
                        >>"%TMP_H%" echo(!L!
                    )
                ) else (
                    >>"%TMP_H%" echo(
                )
            )

            if "!ALVO!"=="CPP" (
                if defined L (
                    echo !L! | findstr /c:"{{CASES}}" >nul
                    if not errorlevel 1 (
                        for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                            set "EST=!ESTADO_UPPER%%i!"
                            set "ESQUERDA=            case !ENUM!::!EST!:"

                            call :STRLEN "!ESQUERDA!" TAM

                            set "ALVO_ALINHA=60"
                            set /a FALTA=ALVO_ALINHA - TAM
                            if !FALTA! LSS 1 set "FALTA=1"

                            set "ESPACOS="
                            for /L %%s in (1,1,!FALTA!) do set "ESPACOS=!ESPACOS! "

                            >>"%TMP_CPP%" echo !ESQUERDA!!ESPACOS!return F^("!EST!"^);
                        )
                    ) else (
                        >>"%TMP_CPP%" echo(!L!
                    )
                ) else (
                    >>"%TMP_CPP%" echo(
                )
            )
        )
    )
)

if not exist "%TMP_H%" (
    echo [ERRO] Bloco .h nao foi gerado.
    exit /b 1
)
if not exist "%TMP_CPP%" (
    echo [ERRO] Bloco .cpp nao foi gerado.
    exit /b 1
)
exit /b 0


:: ============================================================
:: Subrotina para calcular tamanho de string
:: Uso: call :STRLEN "texto" VARIAVEL_DESTINO
:: ============================================================
:STRLEN
set "STR=%~1"
set "LEN=0"
:STRLEN_LOOP
if defined STR (
    set "STR=!STR:~1!"
    set /a LEN+=1
    goto STRLEN_LOOP
)
set "%~2=!LEN!"
exit /b

:INJETAR_ANTES_DA_TAG
:: %1 = arquivo destino
:: %2 = tag de inserção
:: %3 = arquivo com o bloco a inserir
set "ARQ_DEST=%~1"
set "TAG=%~2"
set "BLOCO=%~3"
set "ARQ_NEW=%ARQ_DEST%.new"

if exist "%ARQ_NEW%" del "%ARQ_NEW%"

set "INSERIDO=0"

(
    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_DEST%"') do (
        set "LN=%%B"
        set "TEST=%%B"
        if defined TEST for /f "tokens=*" %%X in ("!TEST!") do set "TEST=%%X"

        if "!TEST!"=="!TAG!" (
            if exist "%BLOCO%" type "%BLOCO%"
            echo(
            echo(!LN!
            set "INSERIDO=1"
        ) else (
            if "%%B"=="" (
                echo(
            ) else (
                echo(!LN!
            )
        )
    )
) > "%ARQ_NEW%"

if "!INSERIDO!"=="0" (
    echo [ERRO] Tag nao encontrada: !TAG!
    del "%ARQ_NEW%" 2>nul
    exit /b 1
)

move /y "%ARQ_NEW%" "%ARQ_DEST%" >nul
exit /b 0


:INJETAR_STRUCT_E_STATIC
set "ARQ_NEW=%ARQ_CPP%.new"
if exist "%ARQ_NEW%" del "%ARQ_NEW%"

(
    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_CPP%"') do (
        set "LN=%%B"
        set "TEST=%%B"
        if defined TEST for /f "tokens=*" %%X in ("!TEST!") do set "TEST=%%X"

        set "PROCESSADO=0"

        if "!TEST!"=="// [STRUCT::BAT]" (
            echo        !NAMESPACE_PRINCIPAL!::!ENUM! !VARIAVEL!; // [VAR::!NAMESPACE_PRINCIPAL!]
            echo(!LN!
            set "PROCESSADO=1"
        )

        if "!TEST!"=="// [STATIC::BAT]" (
            echo        !NAMESPACE_PRINCIPAL!::!ENUM!::!PRIMEIRO_ESTADO!, // [INIT::!NAMESPACE_PRINCIPAL!]
            echo(!LN!
            set "PROCESSADO=1"
        )

        if "!PROCESSADO!"=="0" (
            if "%%B"=="" (
                echo(
            ) else (
                echo(!LN!
            )
        )
    )
) > "%ARQ_NEW%"

move /y "%ARQ_NEW%" "%ARQ_CPP%" >nul
exit /b 0