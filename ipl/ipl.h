/*
    iGameGear is based on SMSSDL by Gregory Montoir.
    iPodLinux/Hotdog port by Luiz "venox" Ribeiro
    Copyright (c) 2002, 2003 Gregory Montoir

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __SDLSMS_H__
#define __SDLSMS_H__

#include "hotdog.h"
#include "shared.h"

#define IGG_CORE_VER  VERSION
#define IGG_RELEASE   "1"
#define IGG_VERSION   "0.1.0"
#define IGG_TITLE     "iGameGear v" IGG_VERSION "R" IGG_RELEASE " (Core " IGG_CORE_VER ")"

#define SMS_SCREEN_WIDTH  256
#define SMS_SCREEN_HEIGHT 192
#define GG_SCREEN_WIDTH   160
#define GG_SCREEN_HEIGHT  144

#define FILTER_MARGIN_HEIGHT 3

#define SOUND_FREQUENCY    22050
#define SOUND_SAMPLES_SIZE  2048

#define MACHINE_FPS 60

typedef enum {
  DISPLAY_CENTER = 0,
  DISPLAY_SCALE  = 1,
  DISPLAY_STRETCH = 2
} t_display;

typedef struct {
  const char* name;
  t_display type;
} t_named_display;

typedef struct {
  int fm;
  int japan;
  int usesram;
  int frameskip;
  int display_mode;
  int fullspeed;
  int nosound;
  int joystick;
  char game_name[0x100];
} t_config;

typedef struct {
  uint32 *screen;

  int width, height, bpp;
  int hwver, lcdtype;

  hd_engine *engine;
  hd_object *canvas;

  uint32 frames_rendered;
  int display_mode;
  int frame_skip;
  int bitmap_offset;
} t_igg_video;

#ifdef ENABLESOUND
typedef struct {
  char* current_pos;
  char* buffer;
  int current_emulated_samples;
} t_sdl_sound;
#endif

extern int igg_init(const t_config*);
extern void igg_emulate();
extern void igg_shutdown();

#endif
