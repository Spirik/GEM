#ifndef __STM32_H_
#define __STM32_H_


#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>

  #include <stm32u5xx_hal.h>

  #include "dostrf.h"

  #define delay HAL_Delay

  typedef unsigned char byte;
#endif


#endif
