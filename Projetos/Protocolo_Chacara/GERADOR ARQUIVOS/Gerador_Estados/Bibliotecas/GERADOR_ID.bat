@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: Carrega sessão atual

if not exist "%ARQUIVO_SESSAO%" exit /b 

for /f "usebackq delims=" %%A in ("%ARQUIVO_SESSAO%") do set "%%A"

set "ARQ_ALVO=%PASTA_SISTEMA%\%NAMESPACE%.md"
set "TMP_IDS=%DIRETORIO_TEMP%\ids_gerenciados.tmp"

if exist "%TMP_IDS%" del "%TMP_IDS%" 

echo [INFO] Higienizando e Gerenciando IDs dos Estados...

:: 1. Descobre o maior ID atual para saber de onde continuar a contagem
set "MAIOR_ID=0"
set "DENTRO_ESTADOS=0"

for /f "usebackq delims=" %%L in ("%ARQ_ALVO%") do (
    set "LN=%%L"
    if defined LN for /f "tokens=*" %%X in ("!LN!") do set "LN=%%X"
    
    if /i "!LN!"=="[INICIO_ESTADOS]" (set "DENTRO_ESTADOS=1")
    if /i "!LN!"=="[FIM_ESTADOS]" (set "DENTRO_ESTADOS=0")
    
    if "!DENTRO_ESTADOS!"=="1" (
        if not "!LN!"=="" if /i not "!LN!"=="[INICIO_ESTADOS]" (
            :: Verifica se a linha tem o formato ID=ESTADO
            echo !LN! | findstr /r "^[0-9]*=" >nul && (
                for /f "tokens=1 delims==" %%I in ("!LN!") do (
                    if %%I GTR !MAIOR_ID! set "MAIOR_ID=%%I"
                )
            )
        )
    )
)

:: 2. Reescreve o arquivo gerando IDs para quem não tem e mantendo quem já tem
set "DENTRO_ESTADOS=0"
for /f "tokens=1,* delims=:" %%A in ('findstr /n "^" "%ARQ_ALVO%"') do (
    set "LN=%%B"
    set "TEST=%%B"
    if defined TEST for /f "tokens=*" %%X in ("!TEST!") do set "TEST=%%X"
    
    if /i "!TEST!"=="[INICIO_ESTADOS]" (
        echo [INICIO_ESTADOS]>> "%TMP_IDS%"
        set "DENTRO_ESTADOS=1"
    ) else if /i "!TEST!"=="[FIM_ESTADOS]" (
        echo [FIM_ESTADOS]>> "%TMP_IDS%"
        set "DENTRO_ESTADOS=0"
    ) else (
        if "!DENTRO_ESTADOS!"=="1" (
            if not "!TEST!"=="" (
                :: Ignora linhas de comentários humanos de rascunho
                echo !TEST! | findstr /v /c:"apague esse" | findstr /v /c:"//" >nul && (
                    :: Se já possui ID, mantém intacto
                    echo !TEST! | findstr /r "^[0-9]*=" >nul && (
                        echo !TEST!>> "%TMP_IDS%"
                    ) || (
                        :: Se não possui ID, calcula o próximo e incrementa
                        set /a MAIOR_ID+=1
                        echo !MAIOR_ID!=!TEST!>> "%TMP_IDS%"
                    )
                )
            )
        ) else (
            if "%%B"== "" (echo(>> "%TMP_IDS%") else (echo(!LN!>> "%TMP_IDS%")
        )
    )
)
move /y "%TMP_IDS%" "%ARQ_ALVO%" >nul
exit /b 0
