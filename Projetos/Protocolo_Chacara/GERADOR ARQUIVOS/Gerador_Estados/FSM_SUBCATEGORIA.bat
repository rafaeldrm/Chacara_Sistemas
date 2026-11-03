@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ============================================================================
:: FSM_SUBCATEGORIA.bat
:: Cria FSM com Subcategoria (PAI + FILHO)
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
echo   CRIANDO FSM COM SUBCATEGORIA - Sistema: !SISTEMA_ATUAL!
echo ===================================================
echo.

:: ------------------------------------------------------------------------
:: 1. Lista os PAIs já existentes
:: ------------------------------------------------------------------------
set "PASTA_PAIS=!PASTA_SISTEMA!\FSM_SUBCATEGORIA"
if not exist "!PASTA_PAIS!" mkdir "!PASTA_PAIS!"

echo PAIs existentes:
echo.

set "CONTADOR=0"
for %%F in ("!PASTA_PAIS!\*.md") do (
    set /a CONTADOR+=1
    set "NOME_ARQ=%%~nF"

    rem Pega só a parte do PAI (antes do hífen)
 ::   for /f "tokens=1 delims=-" %%P in ("!NOME_ARQ!") do (
 ::       set "PAI_!CONTADOR!=%%P"
 ::       echo   !CONTADOR! - %%P
    set "PAI_!CONTADOR!=!NOME_ARQ!"
    echo   !CONTADOR! - !NOME_ARQ! 

::    )
)

echo.
echo   0 - Criar um NOVO PAI
echo.

set "ESCOLHA="
set /p "ESCOLHA=Digite o numero do PAI (ou 0 para criar novo): "

if "!ESCOLHA!"=="0" (
    set "MODO=NOVO_PAI"
    goto PEDIR_NOVO_PAI
)

if not defined PAI_!ESCOLHA! (
    echo [ERRO] Opcao invalida.
    pause
    exit /b 1
)

set "NAMESPACE_PRINCIPAL=!PAI_%ESCOLHA%!"
set "MODO=FILHO_EXISTENTE"
echo.
echo PAI selecionado: !NAMESPACE_PRINCIPAL!-!NAMESPACE_SUBCATEGORIA!
goto PEDIR_FILHO


:PEDIR_NOVO_PAI
echo.
set "NOME_PAI="
set /p "NOME_PAI=Digite o nome do NOVO PAI: "
if "!NOME_PAI!"=="" (
    echo [ERRO] Nome do PAI nao pode ser vazio.
    pause
    exit /b 1
)
call "%DIRETORIO_BIBLIOTECAS%\FORMATA_CONTEXTO.bat" NAMESPACE_PRINCIPAL "!NOME_PAI!" TEXTO_TRATADO
set "NAMESPACE_PRINCIPAL=!TEXTO_TRATADO!"
echo Namespace PAI = !NAMESPACE_PRINCIPAL!
goto PEDIR_FILHO


:PEDIR_FILHO
echo.
set "NOME_FILHO="
set /p "NOME_FILHO=Digite o nome da SUBCATEGORIA (filho): "
if "!NOME_FILHO!"=="" (
    echo [ERRO] Nome do filho nao pode ser vazio.
    pause
    exit /b 1
)
call "%DIRETORIO_BIBLIOTECAS%\FORMATA_CONTEXTO.bat" NAMESPACE_SUBCATEGORIA "!NOME_FILHO!" TEXTO_TRATADO
set "NAMESPACE_SUBCATEGORIA=!TEXTO_TRATADO!"
echo Namespace Filho = !NAMESPACE_SUBCATEGORIA!
echo.

:: ------------------------------------------------------------------------
:: 2. Cria o arquivo de Escopo
:: ------------------------------------------------------------------------
set "ARQ_DESTINO=!PASTA_PAIS!\!NAMESPACE_PRINCIPAL!-!NAMESPACE_SUBCATEGORIA!.md"
set "ARQUIVO_ESCOPO=!ARQ_DESTINO!"

if exist "!ARQ_DESTINO!" (
    echo [AVISO] Ja existe Escopo para este PAI+FILHO.
    choice /c SN /m "Deseja sobrescrever"
    if errorlevel 2 exit /b 0
)

copy /y "%DIRETORIO_TEMPLATES%\Escopo.md" "!ARQ_DESTINO!" >nul
if errorlevel 1 (
    echo [ERRO] Falha ao copiar Escopo.md
    pause
    exit /b 1
)

call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA SISTEMA "!SISTEMA_ATUAL!"
call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA NAMESPACE_PRINCIPAL "!NAMESPACE_PRINCIPAL!"
call "%DIRETORIO_BIBLIOTECAS%\GERENCIADOR_ESCOPO.bat" ATUALIZA NAMESPACE_SUBCATEGORIA "!NAMESPACE_SUBCATEGORIA!"

echo.
echo [INFO] Abrindo o Escopo no VS Code.
echo        Edite ENUM, VARIAVEL e ESTADOS, salve e feche para continuar...
echo.
call code --wait "!ARQ_DESTINO!"

:: ------------------------------------------------------------------------
:: 3. Carrega o contexto
:: ------------------------------------------------------------------------
echo.
echo ==========================================
echo CARREGANDO CONTEXTO...
echo ==========================================
echo.

call "%DIRETORIO_BIBLIOTECAS%\FSM_CONTEXTO.bat"
if errorlevel 1 (
    echo [ERRO] Falha ao carregar o contexto.
    pause
    exit /b 1
)

if "!TOTAL_ESTADOS!"=="0" (
    echo [ERRO] Nenhum estado encontrado.
    pause
    exit /b 1
)

set "PRIMEIRO_ESTADO=!ESTADO_UPPER1!"

echo Sistema............... !SISTEMA!
echo Namespace PAI......... !NAMESPACE_PRINCIPAL!
echo Namespace Filho....... !NAMESPACE_SUBCATEGORIA!
echo Enum.................. !ENUM!
echo Variavel.............. !VARIAVEL!
echo Total de Estados...... !TOTAL_ESTADOS!
echo.

:: ------------------------------------------------------------------------
:: 4. Define caminhos
:: ------------------------------------------------------------------------
set "ARQ_H=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_!SISTEMA!.h"
set "ARQ_CPP=%DIRETORIO_SISTEMA_ESTADOS%\Estados_Sistema_!SISTEMA!.cpp"
set "TEMPLATE_CASCA=%DIRETORIO_TEMPLATES%\Estrutura_FSM_Subcategoria.md"
set "TEMPLATE_ESTADOS=%DIRETORIO_TEMPLATES%\Estrutura_Estados.md"

set "TMP_CASCA_H=%DIRETORIO_TEMP%\_casca_!NAMESPACE_PRINCIPAL!_h.tmp"
set "TMP_CASCA_CPP=%DIRETORIO_TEMP%\_casca_!NAMESPACE_PRINCIPAL!_cpp.tmp"
set "TMP_FILHO_H=%DIRETORIO_TEMP%\_filho_!NAMESPACE_SUBCATEGORIA!_h.tmp"
set "TMP_FILHO_CPP=%DIRETORIO_TEMP%\_filho_!NAMESPACE_SUBCATEGORIA!_cpp.tmp"

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

:: ------------------------------------------------------------------------
:: 5. Verifica se o PAI já existe
:: ------------------------------------------------------------------------
findstr /C:"// [INICIO::BAT::!NAMESPACE_PRINCIPAL!]" "%ARQ_H%" >nul
if errorlevel 1 (
    set "PAI_EXISTE=0"
    echo [INFO] PAI ainda nao existe. Sera criada a casca.
) else (
    set "PAI_EXISTE=1"
    echo [INFO] PAI ja existe. Apenas o filho sera injetado.
)

:: ------------------------------------------------------------------------
:: 6. Gera o bloco do FILHO (mesmo da FSM Simples)
:: ------------------------------------------------------------------------
echo.
echo Gerando bloco do FILHO...
call :GERAR_BLOCO_FILHO
if errorlevel 1 (
    echo [ERRO] Falha ao gerar bloco do filho.
    pause
    exit /b 1
)

:: ------------------------------------------------------------------------
:: 7. Se o PAI nao existe, gera a casca e injeta tudo
:: ------------------------------------------------------------------------
if "!PAI_EXISTE!"=="0" (
    echo Gerando casca do PAI...
    call :GERAR_CASCA_PAI
    if errorlevel 1 (
        echo [ERRO] Falha ao gerar casca do PAI.
        pause
        exit /b 1
    )

    echo Injetando casca + primeiro filho...
    call :INJETAR_CASCA_COM_FILHO
) else (
    echo Injetando filho no PAI existente...
    call :INJETAR_FILHO_NO_PAI
)

:: ========================================================================
:: FUNÇÕES
:: ========================================================================

:GERAR_BLOCO_FILHO
:: Gera o bloco do filho usando o mesmo template da FSM Simples
:: Mas aninha o namespace (NAMESPACE_PRINCIPAL::NAMESPACE_SUBCATEGORIA)
if exist "%TMP_FILHO_H%" del "%TMP_FILHO_H%"
if exist "%TMP_FILHO_CPP%" del "%TMP_FILHO_CPP%"

set "ALVO="

for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%TEMPLATE_ESTADOS%"') do (
    set "L=%%B"
    set "C=!L!"
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
                rem Substitui as tags normais
                for %%S in ("!SISTEMA!") do set "L=!L:{{SISTEMA}}=%%~S!"
                for %%N in ("!NAMESPACE_SUBCATEGORIA!") do set "L=!L:{{NAMESPACE}}=%%~N!"
                for %%E in ("!ENUM!") do set "L=!L:{{ENUM}}=%%~E!"
                for %%V in ("!VARIAVEL!") do set "L=!L:{{VARIAVEL}}=%%~V!"

       ::         rem AQUI está a diferença: o namespace do filho é aninhado
        ::        set "L=!L:{{NAMESPACE}}=!NAMESPACE_SUBCATEGORIA!"
            )

            if "!ALVO!"=="H" (
                if defined L (
                    echo !L! | findstr /c:"{{ESTADOS}}" >nul
                    if not errorlevel 1 (
                        for /L %%i in (1,1,!TOTAL_ESTADOS!) do (
                            >>"%TMP_FILHO_H%" echo        !ESTADO_UPPER%%i!,
                        )
                    ) else (
                        >>"%TMP_FILHO_H%" echo(!L!
                    )
                ) else (
                    >>"%TMP_FILHO_H%" echo(
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
                            >>"%TMP_FILHO_CPP%" echo !ESQUERDA!!ESPACOS!return F^("!EST!"^);
                        )
                    ) else (
                        >>"%TMP_FILHO_CPP%" echo(!L!
                    )
                ) else (
                    >>"%TMP_FILHO_CPP%" echo(
                )
            )
        )
    )
)

if not exist "%TMP_FILHO_H%" exit /b 1
if not exist "%TMP_FILHO_CPP%" exit /b 1
exit /b 0


:GERAR_CASCA_PAI
if exist "%TMP_CASCA_H%" del "%TMP_CASCA_H%"
if exist "%TMP_CASCA_CPP%" del "%TMP_CASCA_CPP%"

set "ALVO="
for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%TEMPLATE_CASCA%"') do (
    set "L=%%B"
    set "C=!L!"
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
                for %%S in ("!SISTEMA!") do set "L=!L:{{SISTEMA}}=%%~S!"
                for %%N in ("!NAMESPACE_PRINCIPAL!") do set "L=!L:{{NAMESPACE}}=%%~N!"
            )

            if "!ALVO!"=="H" (
                if defined L (>>"%TMP_CASCA_H%" echo(!L!) else (>>"%TMP_CASCA_H%" echo()
            )
            if "!ALVO!"=="CPP" (
                if defined L (>>"%TMP_CASCA_CPP%" echo(!L!) else (>>"%TMP_CASCA_CPP%" echo()
            )
        )
    )
)

if not exist "%TMP_CASCA_H%" exit /b 1
if not exist "%TMP_CASCA_CPP%" exit /b 1
exit /b 0


:INJETAR_CASCA_COM_FILHO
:: Injeta a casca do PAI + o primeiro filho já dentro dela
:: Estratégia: gera um bloco único (casca com o filho já inserido) e injeta antes da tag INSERE

:: Monta o bloco final do .h
set "TMP_FINAL_H=%DIRETORIO_TEMP%\_final_h.tmp"
if exist "%TMP_FINAL_H%" del "%TMP_FINAL_H%"

(
    for /f "usebackq delims=" %%A in ("%TMP_CASCA_H%") do (
        set "LINHA=%%A"
        if not "!LINHA://[PROXIMA::BAT::SUBCATEGORIA]=!"=="!LINHA!" (
            rem Encontrou o ponto de injeção → coloca o filho antes
            type "%TMP_FILHO_H%"
            echo(
            echo !LINHA!
        ) else (
            echo(!LINHA!
        )
    )
) > "%TMP_FINAL_H%"

:: Monta o bloco final do .cpp
set "TMP_FINAL_CPP=%DIRETORIO_TEMP%\_final_cpp.tmp"
if exist "%TMP_FINAL_CPP%" del "%TMP_FINAL_CPP%"

(
    for /f "usebackq delims=" %%A in ("%TMP_CASCA_CPP%") do (
        set "LINHA=%%A"
        if not "!LINHA://[PROXIMA::BAT::SUBCATEGORIA]=!"=="!LINHA!" (
            type "%TMP_FILHO_CPP%"
            echo(
            echo !LINHA!
        ) else (
            echo(!LINHA!
        )
    )
) > "%TMP_FINAL_CPP%"

:: Agora injeta esses blocos finais antes da tag INSERE
call :INJETAR_ANTES_DA_TAG "%ARQ_H%" "// [INSERE::BAT::ESTADOS_H_!SISTEMA!]" "%TMP_FINAL_H%"
call :INJETAR_ANTES_DA_TAG "%ARQ_CPP%" "// [INSERE::BAT::ESTADOS_CPP_!SISTEMA!]" "%TMP_FINAL_CPP%"
exit /b 0


:INJETAR_FILHO_NO_PAI
:: Procura o PAI e injeta o filho acima da tag PROXIMA
call :INJETAR_DENTRO_DO_PAI "%ARQ_H%" "%TMP_FILHO_H%"
call :INJETAR_DENTRO_DO_PAI "%ARQ_CPP%" "%TMP_FILHO_CPP%"
exit /b 0


:INJETAR_DENTRO_DO_PAI
:: %1 = arquivo (.h ou .cpp)
:: %2 = bloco do filho
set "ARQ=%~1"
set "BLOCO=%~2"
set "ARQ_NEW=%ARQ%.new"
if exist "%ARQ_NEW%" del "%ARQ_NEW%"

set "DENTRO_PAI=0"
set "INJETOU=0"

(
    for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ%"') do (
        set "LN=%%B"
        set "TEST=%%B"
        if defined TEST for /f "tokens=*" %%X in ("!TEST!") do set "TEST=%%X"

        rem Detecta início do PAI
        if "!TEST!"=="// [INICIO::BAT::!NAMESPACE_PRINCIPAL!]" (
            set "DENTRO_PAI=1"
        )

        rem Detecta fim do PAI
        if "!TEST!"=="// [FIM::BAT::!NAMESPACE_PRINCIPAL!]" (
            set "DENTRO_PAI=0"
        )

        rem Se estamos dentro do PAI e encontramos a tag PROXIMA → injeta
        if "!DENTRO_PAI!"=="1" if "!INJETOU!"=="0" (
            echo !TEST! | findstr /C:"[PROXIMA::BAT::SUBCATEGORIA]" >nul
            if not errorlevel 1 (
                type "%BLOCO%"
                echo(
                set "INJETOU=1"
            )
        )

        if "%%B"=="" (
            echo(
        ) else (
            echo(!LN!
        )
    )
) > "%ARQ_NEW%"

if "!INJETOU!"=="0" (
    echo [ERRO] Nao encontrou a tag PROXIMA dentro do PAI !NAMESPACE_PRINCIPAL!
    del "%ARQ_NEW%" 2>nul
    exit /b 1
)

move /y "%ARQ_NEW%" "%ARQ%" >nul
exit /b 0


:INJETAR_ANTES_DA_TAG
:: %1 = arquivo destino
:: %2 = tag
:: %3 = bloco
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
            if "%%B"=="" (echo() else (echo(!LN!)
        )
    )
) > "%ARQ_NEW%"

if "!INSERIDO!"=="0" (
    echo [ERRO] Tag nao encontrada: !TAG!
    del "%ARQ_NEW%" 2>nul
    exit /b 1
)
move /y "%ARQ_NEW%" "%ARQ_DEST%" >nul


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