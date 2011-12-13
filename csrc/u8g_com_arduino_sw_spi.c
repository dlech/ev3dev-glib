/*
  
  u8g_arduino_sw_spi.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  

*/

#include "u8g.h"

#if defined(ARDUINO)

#if ARDUINO < 100 
#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif


#define PIN_SCK 13
#define PIN_MISO  12
#define PIN_MOSI 11
#define PIN_CS 10
#define PIN_A0 9
#define PIN_RST 8


void u8g_arduino_sw_spi_shift_out(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
  uint8_t i = 8;
  do
  {
    if ( val & 128 )
      digitalWrite(dataPin, HIGH);
    else
      digitalWrite(dataPin, LOW);
    val <<= 1;
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);		
    i--;
  } while( i != 0 );
}

uint8_t u8g_com_arduino_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      pinMode(PIN_SCK, OUTPUT);
      digitalWrite(PIN_SCK, LOW);
      pinMode(PIN_MOSI, OUTPUT);
      digitalWrite(PIN_MOSI, LOW);
      pinMode(PIN_A0, OUTPUT); 
      pinMode(PIN_CS, OUTPUT);			/* this is the user chip select */
      digitalWrite(PIN_CS, HIGH);
#ifdef PIN_RST
      pinMode(PIN_RST, OUTPUT);
      digitalWrite(PIN_RST, HIGH);
#endif    
      break;
    
    case U8G_COM_MSG_STOP:
      break;

#ifdef PIN_RST
    case U8G_COM_MSG_RESET:
      digitalWrite(PIN_RST, arg_val);
      break;
#endif    
    
    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        digitalWrite(PIN_CS, HIGH);
      }
      else
      {
        /* enable */
        digitalWrite(PIN_SCK, LOW);
        digitalWrite(PIN_CS, LOW);  
      }
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_arduino_sw_spi_shift_out(PIN_MOSI, PIN_SCK, arg_val);
      break;
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_arduino_sw_spi_shift_out(PIN_MOSI, PIN_SCK, *ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_arduino_sw_spi_shift_out(PIN_MOSI, PIN_SCK, u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      digitalWrite(PIN_A0, arg_val);
      break;
  }
  return 1;
}

#else /* ARDUINO */

uint8_t u8g_com_arduino_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

