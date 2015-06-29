
#ifndef _RENDER_H_
#define _RENDER_H_

/* Pack RGB data into a 16-bit RGB 5:6:5 format */
//#define MAKE_PIXEL(r,g,b)   (((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | ((b >> 3) & 0x001F))

/* Pack RGB data into a 32-bit ARGB8888 format */
/* Alpha is fixed to 0xFF (opaque)             */
//#define MAKE_PIXEL(r,g,b) ( (uint32)((0xFF << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF)) )
#define MAKE_PIXEL(r,g,b) (0xff000000 | ((r) << 16) | ((g) << 8) | (b))

/* Used for blanking a line in whole or in part */
#define BACKDROP_COLOR      (0x10 | (vdp.reg[7] & 0x0F))

/* Global data - used by 'vdp.c' */
extern uint8 vram_dirty[0x200];
extern uint8 is_vram_dirty;

/* Function prototypes */
void render_init(void);
void render_reset(void);
void render_bg_gg(int line);
void render_bg_sms(int line);
void render_obj(int line);
void render_line(int line);
void update_cache(void);
void palette_sync(int index);
void remap_8_to_16(int line);

#endif /* _RENDER_H_ */
