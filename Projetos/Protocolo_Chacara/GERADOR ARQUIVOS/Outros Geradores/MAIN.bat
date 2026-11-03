@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion
echo.
echo ###############################################
echo ##        GERADOR DE MODULOS ARDUINO         ##
echo ###############################################
echo.

:: ================================================
:: PERGUNTA 1 - Nome da Pasta
:: ================================================

set /p INPUT_PASTA=Nome da pasta do modulo: 
call NORMALIZAR.bat "%INPUT_PASTA%"

:: Atribui o valor de NORM_OUT para PASTA
set "PASTA=%NORM_OUT%"

:: Cria a pasta usando o valor recebido
if not exist "%PASTA%" (
    mkdir "%PASTA%"
) else (
    echo [AVISO] Pasta ja existe!
)

echo Pasta salva como: %PASTA%
echo.

:: ================================================
:: PERGUNTA 2 - Nome do Arquivo
:: ================================================

set /p INPUT_ARQUIVO=Nome do arquivo (.h e .cpp): 
call NORMALIZAR.bat "%INPUT_ARQUIVO%"

:: Atribui o valor de NORM_OUT para NAME
set "NAME=%NORM_OUT%"

::===============================================================
echo Criando arquivos do modulo %NAME%...

:: ===== .H =====
 echo //-------------------------- DEPENDÊNCIAS ----------------------------------// >> %NAME%.h
 echo. >> %NAME%.h
 echo #pragma once >> %NAME%.h
 echo #include ^<Arduino.h^> >> %NAME%.h
 echo. >> %NAME%.h
 echo. //-------------------------- FUNÇÕES PÚBLICAS -------------------------------// >> %NAME%.h
 echo. >> %NAME%.h
 echo. void %NAME%_Inicializa();>> %NAME%.h
 echo. >> %NAME%.h
 echo. void %NAME%_Atualiza(); >> %NAME%.h
 echo. >> %NAME%.h

:: ===== .CPP =====
echo //====================================================================================  >> %NAME%.cpp
echo //                   DEPENDÊNCIAS  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo #include "%NAME%.h" >> %NAME%.cpp
echo #include "DEBUG_MODE.h" >> %NAME%.cpp
echo. >> %NAME%.cpp
echo #include "Tempo.h" >> %NAME%.cpp
echo. >> %NAME%.cpp
echo // [BAT_INCLUDE] >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   VARIÁVEIS INTERNAS  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo // [BAT_VARIAVEIS] >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   SETUP  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. void %NAME%_Inicializa() >> %NAME%.cpp
echo { >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo } >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   LOOP  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. void %NAME%_Atualiza() >> %NAME%.cpp
echo { >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo } >> %NAME%.cpp
echo. >> %NAME%.cpp
echo.
echo Modulo %NAME% criado com sucesso!

move "%NAME%.h" "%PASTA%\"
move "%NAME%.cpp" "%PASTA%\"

robocopy "%PASTA%" "%~dp0..\lib\%PASTA%" /MOVE /E

echo Arquivos movidos com sucesso!