/*

  u8g_dev_linux_fb.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, 2013 olikraus@gmail.com, David Lechner <david@lechnology.com>
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

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <fcntl.h>
#include "u8g.h"

struct fb_t
{
  uint32_t size;
  struct fb_fix_screeninfo fix;
  struct fb_var_screeninfo var;
} fb;


uint8_t u8g_dev_linux_fb_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

u8g_pb_t u8g_dev_linux_fb_pb;
u8g_dev_t u8g_dev_linux_fb = { u8g_dev_linux_fb_fn, &u8g_dev_linux_fb_pb, NULL };

uint8_t map_fb()
{
  int fd;

  /* TODO: automatically find fb or allow to pass the file name as a parameter */
  fd = open("/dev/fb0", O_RDWR);
  if (fd == -1)
    return 0;
  if (ioctl(fd, FBIOGET_VSCREENINFO, &fb.var) == -1)
    return 0;
  if (ioctl(fd, FBIOGET_FSCREENINFO, &fb.fix) == -1)
    return 0;
  /* only supports 1bpp for now */
  if (fb.var.bits_per_pixel != 1)
    return 0;
  fb.size = fb.fix.line_length * fb.var.yres_virtual;
  u8g_dev_linux_fb_pb.width = fb.fix.line_length * 8;
  u8g_dev_linux_fb_pb.buf = mmap(NULL, fb.size, PROT_READ | PROT_WRITE,
                                 MAP_SHARED, fd, 0);
  if (u8g_dev_linux_fb_pb.buf == MAP_FAILED)
    return 0;
  u8g_page_Init(&u8g_dev_linux_fb_pb.p, fb.var.yres_virtual, fb.var.yres_virtual);
  return 1;
}

uint8_t u8g_dev_linux_fb_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  if (msg != U8G_DEV_MSG_INIT && u8g_dev_linux_fb_pb.buf == NULL)
    return 0;

  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      return map_fb(); 
    case U8G_DEV_MSG_STOP:
      munmap(u8g_dev_linux_fb_pb.buf, fb.size);
      u8g_dev_linux_fb_pb.buf = NULL;
      return 1;
    case U8G_DEV_MSG_GET_WIDTH:
      *((u8g_uint_t *)arg) = fb.var.xres_virtual;
      return 1;
  }
  return u8g_dev_pb8h1f_base_fn(u8g, dev, msg, arg);
}
