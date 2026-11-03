//-------------------------- DEPENDÊNCIAS ---------------------------------- //

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------ //

namespace MUSICA
{
  enum PASTA : uint8_t
  {
    SISTEMA = 1, // pasta 01
    EVENTOS_ECO_HIDRO = 2,
    AMBIENTAL = 3,
    JAZZ = 4
  };

  // ------------------------ SISTEMA ---------------------------------------------- //

  namespace SISTEMA
  {
    enum BOOT : uint8_t
    {
      ECO_HIDRO = 1,          // Sonic Theme Remix
      DRIMEL_SYSTEM,          // A Dream Within A Dream, Edgar Allan Poe (Mall Goth Massacre)
      SOLARIUS24,             // Hadouken Theme Remix
      GATE_DRIMEL,            // Need For Speed II Menu Theme Remix
      RASPBERRY_DRIMEL_SYSTEM // Top Gear Theme Techno Mix
    };
  };

  // -------------------------  EVENTOS ECO HIDRO -------------------------------- //

  namespace ECO_HIDRO
  {
    enum EVENTOS : uint8_t // PASTA 02
    {
      CAIXA_ENTRADA_VAZIO_CLORO = 1,     // Mizuhara's Piano Theme - Rent A Girlfriend
      CAIXA_ENTRADA_CHEIO_TRANSFERENCIA, // Camel By Camel (remix Curse by Curse)
      BOMBA_CASA,                        // Westminster_Chime,
      BOMBA_RESERVATORIO                 // Encounter Metal Gear Theme Trance Remix
    };
  };

  //-------------------------- AMBIENTAL ------------------------------------------//

  namespace AMBIENTAL
  {
    enum FAIXA : uint8_t // PASTA 03
    {
      SANDY_BEACH = 1, // 001.mp3
      SAD_NIGHT,
      COZY_SUNDAY,
      RAINY_DAY,
      MEMORIES
    };
  };


}

// criarei depois Jazz, rock, por, classi e outras

//-------------------------- CHAMADAS EXTERNAS ------------------------------//

template <typename T>
constexpr uint8_t faixa(T v)
{
  return static_cast<uint8_t>(v);
}