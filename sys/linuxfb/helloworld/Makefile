CFLAGS = -g -Wall -I../../../csrc/. -DUSE_DUMMY_DELAY

SRC = ../../../csrc/u8g_circle.c       \
      ../../../csrc/u8g_clip.c         \
      ../../../csrc/u8g_com_api.c      \
      ../../../csrc/u8g_delay.c        \
      ../../../csrc/u8g_font.c         \
      ../../../csrc/u8g_ll_api.c       \
      ../../../csrc/u8g_page.c         \
      ../../../csrc/u8g_pb.c           \
      ../../../csrc/u8g_pb8h1f.c       \
      ../../../csrc/u8g_rect.c         \
      ../../../csrc/u8g_state.c        \
      ../../../fntsrc/u8g_font_10x20.c \
      ../dev/u8g_dev_linux_fb.c        \
      main.c

OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o helloworld

clean:	
	-rm $(OBJ) helloworld

