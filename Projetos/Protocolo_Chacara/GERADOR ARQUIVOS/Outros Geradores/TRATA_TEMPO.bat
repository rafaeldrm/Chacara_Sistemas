@echo off
:: ================================================================
::  TRATA_TEMPO.bat - Conversor de sufixos h, m, s para Macros C++
:: ================================================================
chcp 65001 >nul
setlocal enabledelayedexpansion

set "ARQ_IN=tmp_variaveis.txt"
set "ARQ_OUT=tmp_variaveis_convertido.txt"

if not exist "%ARQ_IN%" (
    echo [AVISO] Arquivo %ARQ_IN% não encontrado.
    exit /b
)

echo [PROCESSANDO...] Convertendo sufixos de tempo...

:: Limpa o arquivo de saída se já existir
if exist "%ARQ_OUT%" del "%ARQ_OUT%"

(for /f "usebackq delims=" %%L in ("%ARQ_IN%") do (
    set "linha=%%L"
    
    :: Filtro rápido: Só processa se tiver um número seguido de d, h, m ou s
    echo !linha! | findstr /R "[0-9]d [0-9]h [0-9]m [0-9]s" >nul
    if !errorlevel! == 0 (
        
        :: Separa a linha no sinal de igual
        for /f "tokens=1,2 delims==" %%A in ("!linha!") do (
            set "parte_esq=%%A"
            set "parte_dir=%%B"
            
            :: Remove o ponto e vírgula e espaços extras do valor
            set "valor_bruto=!parte_dir:;=!"
            set "valor_bruto=!valor_bruto: =!"
            
            set "final_str="
            set "temp_val=!valor_bruto!"

            :: --- Processa Dias (d) ---
            if "!temp_val:d=!" neq "!temp_val!" (
                for /f "tokens=1* delims=d" %%d in ("!temp_val!") do (
                    set "final_str=DIAS(%%d)"
                    set "temp_val=%%e"
                )
            )

            :: --- Processa Horas (h) ---
            if "!temp_val:h=!" neq "!temp_val!" (
                for /f "tokens=1* delims=h" %%h in ("!temp_val!") do (
                    set "final_str=HORAS(%%h)"
                    set "temp_val=%%i"
                )
            )

            :: --- Processa Minutos (m) ---
            if "!temp_val:m=!" neq "!temp_val!" (
                for /f "tokens=1* delims=m" %%m in ("!temp_val!") do (
                    if defined final_str (set "final_str=!final_str! + MINUTOS(%%m)") else (set "final_str=MINUTOS(%%m)")
                    set "temp_val=%%n"
                )
            )

            :: --- Processa Segundos (s) ---
            if "!temp_val:s=!" neq "!temp_val!" (
                for /f "tokens=1* delims=s" %%s in ("!temp_val!") do (
                    if defined final_str (set "final_str=!final_str! + SEGUNDOS(%%s)") else (set "final_str=SEGUNDOS(%%s)")
                )
            )

            :: Se conseguiu converter algo, reconstrói a linha, senão mantém original
            if defined final_str (
                :: Remove espaços sobrando no final da parte esquerda (trim)
                set "prefixo=!parte_esq!"
                for /l %%p in (1,1,10) do if "!prefixo:~-1!"==" " set "prefixo=!prefixo:~0,-1!"
                echo !prefixo! = !final_str!;
            ) else (
                echo !linha!
            )
        )
    ) else (
        echo !linha!
    )
)) > "%ARQ_OUT%"

echo [SUCESSO] Arquivo gerado: %ARQ_OUT%