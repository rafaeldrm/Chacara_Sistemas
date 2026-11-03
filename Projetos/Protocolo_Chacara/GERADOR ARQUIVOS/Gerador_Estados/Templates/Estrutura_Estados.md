[FILE:Estados_Sistema_{{SISTEMA}}.h]
 
// [INICIO::BAT::{{NAMESPACE}}]
 
//====================================================================================
//                   {{NAMESPACE}}
//====================================================================================

namespace {{NAMESPACE}}
{
enum class {{ENUM}} : uint8_t
 {
    {{ESTADOS}}
 };

    void definir({{ENUM}} novo);
    {{ENUM}} atual();
    const __FlashStringHelper* texto({{ENUM}} s);
    
}

// [FIM::BAT::{{NAMESPACE}}]
 
[END_FILE]

/////////////////////////////////////////////////////////////////////////////////////////////
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/////////////////////////////////////////////////////////////////////////////////////////////

[FILE:Estados_Sistema_{{SISTEMA}}.cpp]
 
// [INICIO::BAT::{{NAMESPACE}}]

//====================================================================================
//                   {{NAMESPACE}}
//====================================================================================

namespace {{NAMESPACE}}
{
    void definir({{ENUM}} novo) 
    {
        if (estado.{{VARIAVEL}} == novo) return;
        estado.{{VARIAVEL}} = novo;
        debugVariavel("{{ENUM}} {{VARIAVEL}} : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    {{ENUM}} atual()
    {
         return estado.{{VARIAVEL}}; 
    }

    const __FlashStringHelper* texto({{ENUM}} a)
    {
        switch(a)
        {
{{CASES}}

            default:                return F("DESCONHECIDO");
        }
    }
}
 
// [FIM::BAT::{{NAMESPACE}}]
 
[END_FILE]