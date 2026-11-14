@Echo off
chcp 65001 >nul
echo Acentuação: á é í ó ú ç ã õ
setlocal enabledelayedexpansion

echo #####################################################################################
echo ##                                                                                 ##
echo ##                       GERADOR DE ARQUIVOS .H e .CPP automatizado                ##
echo ##                                                                                 ##
echo #####################################################################################
echo #####################################################################################
echo ##                                                                                 ##
echo ##                       Lista de Pastas do Projeto Existente                      ##
echo ##                                                                                 ##
echo #####################################################################################
cd lib
dir
cd ..
echo ######################################################################################
echo.
set /p NOME_PASTA=Digite Somente o nome da PASTA EXISTENTE ou NOVA PASTA :

mkdir "%NOME_PASTA%"

echo Pasta Criada: %NOME_PASTA%
:: ==========================================================
set /p NAME=DIGITE O NOME DOS ARQUIVOS .h e .cpp: 
echo.
echo.
echo É GERADO AUTOMATICAMENTE O LOOP E O SETUP, se precisar definir mais funções digite a quantidade abaixo
echo.
set /p QTD_FUNC=Quantas funcoes personalizadas voce deseja criar? :
echo.
:: --- LOOP PARA CAPTURAR NOME DA FUNÇÃO E O TIPO ---
for /l %%i in (1,1,%QTD_FUNC%) do (
    set /p FUNC=Digite o nome da função %%i :
    set /p TYPE=Digite o tipo de retorno da %FUNC% ex: void, bool. : 
    set FUNC_LIST[%%i]=!FUNC!
    set TYPE_LIST[%%i]=!TYPE!
)
::===============================================================
echo Criando arquivos do modulo %NAME%...

:: ===== .H =====
 echo //-------------------------- DEPENDÊNCIAS ----------------------------------// >> %NAME%.h
 echo. >> %NAME%.h
 echo #pragma once >> %NAME%.h
 echo #include ^<Arduino.h^> >> %NAME%.h
 echo #include "DEBUG_MODE.h" >> %NAME%.h
 echo. >> %NAME%.h
 echo. //-------------------------- FUNÇÕES PÚBLICAS -------------------------------// >> %NAME%.h
 echo. >> %NAME%.h
 echo. void %NAME%_Inicializa();>> %NAME%.h
 echo. >> %NAME%.h
 echo. void %NAME%_Atualiza(); >> %NAME%.h
 echo. >> %NAME%.h
 echo. //-------------------------- ACESSO AOS DADOS -------------------------------// >> %NAME%.h

for /l %%i in (1,1,%QTD_FUNC%) do (
    echo. >> %NAME%.h
    echo !TYPE_LIST[%%i]! %NAME%_!FUNC_LIST[%%i]!^(^)^; >> %NAME%.h
)

:: ===== .CPP =====
echo //====================================================================================  >> %NAME%.cpp
echo //                   DEPENDÊNCIAS  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo #include "%NAME%.h" >> %NAME%.cpp
echo #include "DEBUG_MODE.h" >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   VARIÁVEIS INTERNAS  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   SETUP  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. void %NAME%_Inicializa() >> %NAME%.cpp
echo ^{ >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo ^} >> %NAME%.cpp
echo. >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo //                   LOOP  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. void %NAME%_Atualiza() >> %NAME%.cpp
echo ^{ >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo. >> %NAME%.cpp
echo ^} >> %NAME%.cpp
echo. >> %NAME%.cpp
for /l %%i in (1,1,%QTD_FUNC%) do (
echo //====================================================================================  >> %NAME%.cpp
echo //                   !FUNC_LIST[%%i]!  >> %NAME%.cpp
echo //====================================================================================  >> %NAME%.cpp
    echo. >> %NAME%.cpp
    echo !TYPE_LIST[%%i]! %NAME%_!FUNC_LIST[%%i]!^(^)^ >> %NAME%.cpp
    echo ^{ >> %NAME%.cpp
    echo. >> %NAME%.cpp
    echo. >> %NAME%.cpp
    echo. >> %NAME%.cpp
    echo ^} >> %NAME%.cpp
    echo. >> %NAME%.cpp
)
echo.
echo Modulo %NAME% criado com sucesso!

move "%NAME%.h" "%NOME_PASTA%\"
move "%NAME%.cpp" "%NOME_PASTA%\"

robocopy "%PASTA%" "%~dp0..\lib\%PASTA%" /MOVE /E

echo Arquivos movidos com sucesso!
timeout /t 2 >null