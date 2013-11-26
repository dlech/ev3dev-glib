#include "u8g.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  u8g_uint_t w,h;
  u8g_t u8g;
  
  u8g_Init(&u8g, &u8g_dev_linux_fb);
  u8g_FirstPage(&u8g);
  do
  {
    u8g_SetColorIndex(&u8g, 1);
    u8g_SetFont(&u8g, u8g_font_10x20);
    w = u8g_GetFontBBXWidth(&u8g);
    h = u8g_GetFontBBXHeight(&u8g);
    u8g_DrawStr(&u8g, 0, h, "ABCdef");
    u8g_DrawStr(&u8g, 0, 2*h, "g");
    u8g_DrawStr(&u8g, 10, 3*h, "g");
    u8g_DrawStr(&u8g, 20, 4*h, "g");
    u8g_DrawFrame(&u8g, 2, 2, 90, 30);
    u8g_DrawFrame(&u8g, 0, 0, 130, 70);
    u8g_DrawHLine( &u8g, 0, 16, 50 );   
    
    u8g_SetColorIndex(&u8g, 0);
    
    u8g_DrawHLine( &u8g, 0, 17, 50 );   
    
  } while( u8g_NextPage(&u8g) );
  return 0;
}
