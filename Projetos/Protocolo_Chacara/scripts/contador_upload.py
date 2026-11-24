Import("env")
import re

if "upload" not in COMMAND_LINE_TARGETS:
    print("[BUILD] Ignorado (não é upload)")

else:
    arquivo = "src/main.cpp"

    with open(arquivo, "r", encoding="utf-8") as f:
        conteudo = f.read()

    match = re.search(r"(HODOMETRO_BOOTLOADER\s*=\s*)(\d+)", conteudo)

    if match:
        numero_atual = int(match.group(2))
        novo_numero = numero_atual + 1

        novo_conteudo = re.sub(
            r"(HODOMETRO_BOOTLOADER\s*=\s*)\d+",
            f"\\g<1>{novo_numero}",
            conteudo
        )

        with open(arquivo, "w", encoding="utf-8") as f:
            f.write(novo_conteudo)

        print(f"[UPLOAD] Incrementado: {numero_atual} -> {novo_numero}")
    else:
        print("[UPLOAD] HODOMETRO_BOOTLOADER não encontrado!")