/*

  u8g_dev_uc1701_dogs102.c

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

#define WIDTH 102
#define HEIGHT 64
#define PAGE_HEIGHT 8


u8g_pgm_uint8_t u8g_dev_dogs102_init_seq[] = {
  0x040,		/* set display start line to 0 */
  0x0a1,		/* ADC set to reverse */
  0x0c0,		/* common output mode */
  0x0a6,		/* display normal, bit val 0: LCD pixel off. */
  0x0a2,		/* LCD bias 1/9 */
  0x02f,		/* all power  control circuits on */
  0x027,		/* regulator, booster and follower */
  0x081,		/* set contrast */
  0x00e,		/* contrast value, EA default: 0x010, previous value for S102: 0x0e */
  0x0fa,		/* Set Temp compensation */ 
  0x090,		/* 0.11 deg/c WP Off WC Off*/
  0x0a4,		/* normal display  */
  0x0af		/* display on */
};


uint8_t u8g_dev_dogs102_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      /*
      if ( u8g_InitCom(u8g, dev) == 0 )
        return 0;
      */
      u8g_InitCom(u8g, dev);
      //u8g_DisableCom(u8g, dev);
      u8g_SetChipSelect(u8g, dev, 0);
      u8g_SetAddress(u8g, dev, 0);           /* command mode */
      //u8g_EnableCom(u8g, dev);
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_WriteSequenceP(u8g, dev, sizeof(u8g_dev_dogs102_init_seq), u8g_dev_dogs102_init_seq);      
      u8g_Delay(300);
      u8g_WriteByte(u8g, dev, 0x0a5);		/* display all points, ST7565, UC1610 */
      u8g_Delay(300);
      u8g_WriteByte(u8g, dev, 0x0a4);		/* normal display  */
      u8g_Delay(300);      
      u8g_SetChipSelect(u8g, dev, 0);
      //u8g_DisableCom(u8g, dev);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        //u8g_EnableCom(u8g, dev);
        u8g_SetChipSelect(u8g, dev, 1);
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (ST7565R) */
        u8g_WriteByte(u8g, dev, 0x010 );		/* set upper 4 bit of the col adr to 0 */
        u8g_WriteByte(u8g, dev, 0x000 );		/* set lower 4 bit of the col adr to 0 */      
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
      
        u8g_SetChipSelect(u8g, dev, 0);
        //u8g_DisableCom(u8g, dev);
      }
      break;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_uc1701_dogs102_sw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_dogs102_fn, u8g_com_arduino_sw_spi_fn);
U8G_PB_DEV(u8g_dev_uc1701_dogs102_hw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_dogs102_fn, u8g_com_arduino_hw_spi_fn);


