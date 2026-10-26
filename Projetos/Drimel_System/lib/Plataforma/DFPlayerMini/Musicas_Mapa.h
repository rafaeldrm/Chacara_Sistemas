//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

enum Pasta : uint8_t {
  AMBIENTAL = 1, //pasta 01
  JAZZ      =  2 //pasta 02
};

enum Sistema : uint8_t { // arquivos na RAIZ do SD musicas DF Player Minni, fora de qualquer pasta.
  SONIC_THEME_REMIX   = 1,  //001.mp3
  NEED_FOR_SPEED_II_MENU_THEME,
  STREET_FIGHTER_RYU,
  MORTAL_KOMBAT_REMIX,
  SUPER_MARIO_WORLD,
  TOP_GEAR_REMIX,
  CALL_OF_DUTY_BO1_115,
  METAL_GEAR_THEME,
  CALL_OF_DUTY_BO1_DANMED,
  HADOUKEN_REMIX,
  WESTMINSTER_CHIME,
  HADOUKEN_EPIC_TRANCEMIX,
  METAL_GEAR_ENCOUNTER,
  NEED_FOR_SPEED_II_EPIC_THEME
};

//-------------------------- AMBIENTAL ------------------------------------------//

enum AMBIENTAL : uint8_t {
  SANDY_BEACH = 1, //001.mp3
  SAD_NIGHT, //002.mp3
  COZY_SUNDAY, //003.mp3
  RAINY_DAY, //004.mp3
  MEMORIES, //005.mp3
};

//criarei depois Jazz, rock, por, classi e outras