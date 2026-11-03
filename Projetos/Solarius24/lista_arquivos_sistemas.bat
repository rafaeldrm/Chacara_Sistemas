@echo off
FOR /R  %%d IN (*) DO (
     echo "%%d"
)

REM Gera lista detalhada, também ignorando essas pastas e o .bat
dir /S /B | findstr /V ".pio" | findstr /V ".vscode" | findstr /V "test" | findstr /V "%~nx0" > lista_sistema.txt
echo Arquivos gerados: lista_sistema.txt

dir /s /b lib\*.* > lista_sistema.txt
dir /s /b src\*.* >> lista_sistema.txt

rem remove o caminho inicial
 set ROOT=E:\ARDUINO PROJETO CHµCARA AUTOMA€ÇO\SISTEMA SOLAR 24V\

powershell -command "(Get-Content lista_sistema.txt) | %% { $_ -replace '^.*?(\\lib\\|\\src\\)', '$1' } | Set-Content lista_sistema.txt"

echo Lista pronta!
timeout /t 5 >null


