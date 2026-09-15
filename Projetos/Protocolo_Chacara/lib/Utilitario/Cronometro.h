//-------------------------- DEPENDÊNCIAS ----------------------------------//
#pragma once
#include <Arduino.h>

//-------------------------- ACESSO AOS DADOS -------------------------------//

namespace CRONOMETRO
{

    struct Temporizador
    {
    private:
        uint32_t _inicio = 0;
        bool _ativo = false;

    public:
        //========================================================================
        //                           INICIAR / REINICIAR
        //========================================================================
        void Cronometro_Iniciar()
        {
            _inicio = millis();
            _ativo = true;
        }

        //========================================================================
        //                          CANCELAR
        //========================================================================
        void Cronometro_Cancelar()
        {
            _ativo = false;
        }

        //========================================================================
        //                          VERIFICAR ESTADO
        //========================================================================
        bool Cronometro_Verificar() const
        {
            return _ativo;
        }

        //========================================================================
        //              TEMPO DECORRIDO (Dispara uma só vez e para)
        //========================================================================
        bool Cronometro_Dispara_Uma_Vez(uint32_t tempo)
        {
            if (!_ativo) return false;

            // Captura o millis atual uma única vez para consistência na função
            uint32_t atual = millis(); 

            if ((atual - _inicio) >= tempo)
            {
                _ativo = false;
                return true;
            }
            return false;
        }

        //========================================================================
        //             CICLO (Termina, reinicia o ciclo e continua ativo)
        //========================================================================
        bool Cronometro_Ciclo_Continuo(uint32_t tempo)
        {
            if (!_ativo) return false;

            uint32_t atual = millis();

            if ((atual - _inicio) >= tempo)
            {
                // Ajuste incremental para evitar acúmulo de pequenos atrasos (drift)
                _inicio += tempo; 
                return true;
            }
            return false;
        }
    };

}
