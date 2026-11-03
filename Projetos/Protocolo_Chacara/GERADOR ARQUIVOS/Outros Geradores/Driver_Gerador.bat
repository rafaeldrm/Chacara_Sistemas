@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ###############################################
echo ##        GERADOR DO SISTEMA                 ##
echo ###############################################
echo.

set "PASTA_LOCAL=Sistema_%PASTA%"
set "ORIGEM=Estrutura_Sistema.txt"
set "MAPA_PINOS=%~dp0..\..\SISTEMA RESERVATÓRIO\lib\Plataforma\Mapa_Pinos_Arduino.h"

if not exist "%MAPA_PINOS%" (
    echo [ERRO] Mapa de pinos não encontrado em:
    echo %MAPA_PINOS%
    pause
    exit /b
)

:: 1. Normaliza o nome do Driver para UPPER (ex: voltagem -> VOLTAGEM)
call NORMALIZAR.bat "%DRIVER%" UPPER
set "DRIVER_UPPER=%NORM_OUT%"

:: 2. Menu de seleção do Namespace/Pino
echo.
echo ===============================================
echo    SELECIONE O PINO (Mapa_Pinos_Arduino.h)
echo ===============================================

set countE=0
for /f "tokens=3" %%E in ('findstr /C:"enum class" "%MAPA_PINOS%"') do (
    set /a countE+=1
    set "enum!countE!=%%E"
    echo [!countE!] %%E
)

echo.
set /p selE=Escolha o Grupo (ex: 3): 
if not defined enum%selE% (echo [ERRO] Opcao invalida & pause & exit /b)
set "GRUPO_ALVO=!enum%selE%!"

echo.
echo Opções em !GRUPO_ALVO!:
set countP=0
set lendo=0
for /f "usebackq tokens=*" %%L in ("%MAPA_PINOS%") do (
    set "LINHA_MAPA=%%L"
    echo !LINHA_MAPA! | findstr /C:"enum class !GRUPO_ALVO!" >nul && set lendo=1
    if !lendo! == 1 (
        echo !LINHA_MAPA! | findstr /C:"};" >nul && set lendo=0
        if !lendo! == 1 (
            for /f "tokens=1 delims==" %%M in ("!LINHA_MAPA!") do (
                set "ITEM=%%M"
                set "ITEM=!ITEM: =!"
                set "ITEM=!ITEM:,=!"
                echo !ITEM! | findstr /V "enum {" >nul && (
                    set /a countP+=1
                    set "pino!countP!=!ITEM!"
                    echo [!countP!] !ITEM!
                )
            )
        )
    )
)

echo.
set /p selP=Escolha o PINO (ex: 1): 
if not defined pino%selP% (echo [ERRO] Opcao invalida & pause & exit /b)
set "PINO_ESCOLHIDO=ARDUINO_CONEXAO::!GRUPO_ALVO!::!pino%selP%!"

:: --- GERA O .H ---
set "TAG_BUSCA=[FILE:Driver_DRIVER.h]"
set "ARQ_SAIDA=%PASTA_LOCAL%\Driver_%DRIVER%.h"
call :EXTRAIR_BLOCO

:: --- GERA O .CPP ---
set "TAG_BUSCA=[FILE:Driver_DRIVER.cpp]"
set "ARQ_SAIDA=%PASTA_LOCAL%\Driver_%DRIVER%.cpp"
call :EXTRAIR_BLOCO

:: --- GERA A LEITURA .H ---
set "TAG_BUSCA=[FILE:Leitura_LEITURA.h]"
set "ARQ_SAIDA=%PASTA_LOCAL%\Leitura_%FUNCIONALIDADE%.h"
call :EXTRAIR_BLOCO

:: --- GERA A LEITURA .CPP ---
set "TAG_BUSCA=[FILE:Leitura_LEITURA.cpp]"
set "ARQ_SAIDA=%PASTA_LOCAL%\Leitura_%FUNCIONALIDADE%.cpp"
call :EXTRAIR_BLOCO

echo [OK] Arquivos Driver_%DRIVER% criados em %PASTA_LOCAL%
goto :EOF

:: =================================================
:: FUNÇÃO DE EXTRAÇÃO E SUBSTITUIÇÃO
:: =================================================
:EXTRAIR_BLOCO
set "LENDO=0"
if exist "%ARQ_SAIDA%" del "%ARQ_SAIDA%"

for /f "usebackq delims=" %%L in ("%ORIGEM%") do (
    set "LINHA=%%L"
    
    :: Remove espaços no início para comparação da tag
    for /f "tokens=* delims= " %%S in ("!LINHA!") do set "LINHA_TRIM=%%S"
    
    if "!LINHA_TRIM!"=="%TAG_BUSCA%" (
        set "LENDO=1"
    ) else if "!LINHA_TRIM!"=="[END_FILE]" (
        if "!LENDO!"=="1" (
            set "LENDO=0"
        )
    ) else if "!LENDO!"=="1" (
        :: Faz a substituição do placeholder pelo nome real do Driver
        set "PROCESSADA=!LINHA:PINO_{{DRIVER}}=PINO_%DRIVER_UPPER%!"
        set "PROCESSADA=!PROCESSADA:{{DRIVER}}=%DRIVER%!"
        set "PROCESSADA=!PROCESSADA:{{LEITURA}}=%FUNCIONALIDADE%!"
        set "PROCESSADA=!PROCESSADA:{{NAMESPACE_ARDUINO_PINO}}=%PINO_ESCOLHIDO%!"
        set "PROCESSADA=!PROCESSADA:{{DRIVER_UPPER}}=%DRIVER_UPPER%!"
        
        echo.!PROCESSADA!>>"%ARQ_SAIDA%"
    )
)
exit /b
