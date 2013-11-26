#include "u8g.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  u8g_uint_t w,h, fw, fh, fd;
  u8g_t u8g;

  u8g_Init(&u8g, &u8g_dev_linux_fb);
  w = u8g_GetWidth(&u8g);
  h = u8g_GetHeight(&u8g);

  u8g_SetFont(&u8g, u8g_font_10x20);
  fw = u8g_GetFontBBXWidth(&u8g);
  fh = u8g_GetFontBBXHeight(&u8g);
  fd = u8g_GetFontDescent(&u8g);

  u8g_BeginDraw(&u8g);
  u8g_SetColorIndex(&u8g, 1);
  u8g_DrawStr(&u8g, 10, 10+fh, "ABCdefg");
  u8g_DrawStr(&u8g, 10+6*fw, 10+2*fh, "h");
  u8g_DrawStr(&u8g, 10+6*fw, 10+3*fh, "i");
  u8g_DrawStr(&u8g, 10+6*fw, 10+4*fh, "j");
  u8g_DrawFrame(&u8g, 9, 11-fd, fw*7+2, fh+1);
  u8g_DrawFrame(&u8g, 0, 0, w, h);
  u8g_DrawHLine( &u8g, 15, 25, 50 );
  u8g_SetColorIndex(&u8g, 0);
  u8g_DrawHLine( &u8g, 15, 26, 50 );
  u8g_EndDraw(&u8g);

  u8g_Stop(&u8g);
  return 0;
}

