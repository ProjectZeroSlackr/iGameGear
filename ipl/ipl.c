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

#include <stdlib.h>
#include "ipl.h"
#include "shared.h"
#include "saves.h"
#include "input.h"
#include "video.h"

static const char *rom_filename;
#ifdef ENABLESOUND
static t_sdl_sound sdl_sound;
#endif


/* FIXME: sound */

#ifdef ENABLESOUND
static void sdlsms_sound_callback(void *userdata, Uint8 *stream, int len)
{
  if(sdl_sound.current_emulated_samples < len) {
    memset(stream, 0, len);
  }
  else {
    memcpy(stream, sdl_sound.buffer, len);
    /* loop to compensate desync */
    do {
      sdl_sound.current_emulated_samples -= len;
    } while(sdl_sound.current_emulated_samples > 2 * len);
    memcpy(sdl_sound.buffer,
           sdl_sound.current_pos - sdl_sound.current_emulated_samples,
           sdl_sound.current_emulated_samples);
    sdl_sound.current_pos = sdl_sound.buffer + sdl_sound.current_emulated_samples;
  }
}

static int sdlsms_sound_init()
{
  int n;
  SDL_AudioSpec as_desired, as_obtained;
  
  if(SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("ERROR: %s.\n", SDL_GetError());
    return 0;
  }

  as_desired.freq = SOUND_FREQUENCY;
  as_desired.format = AUDIO_S16LSB;
  as_desired.channels = 2;
  as_desired.samples = SOUND_SAMPLES_SIZE;
  as_desired.callback = sdlsms_sound_callback;

  if(SDL_OpenAudio(&as_desired, &as_obtained) == -1) {
    printf("ERROR: can't open audio: %s.\n", SDL_GetError());
    return 0;
  }

  if(as_desired.samples != as_obtained.samples) {
    printf("ERROR: soundcard driver does not accept specified samples size.\n");
    return 0;
  }

  sdl_sound.current_emulated_samples = 0;
  n = SOUND_SAMPLES_SIZE * 2 * sizeof(short) * 11;
  sdl_sound.buffer = (char*)malloc(n);
  if(!sdl_sound.buffer) {
    printf("ERROR: can't allocate memory for sound.\n");
    return 0;
  }
  memset(sdl_sound.buffer, 0, n);
  sdl_sound.current_pos = sdl_sound.buffer;
  return 1;
}

static void sdlsms_sound_update()
{
  int i;
  short* p;

  SDL_LockAudio();
  p = (short*)sdl_sound.current_pos;
  for(i = 0; i < snd.bufsize; ++i) {
      *p = snd.buffer[0][i];
      ++p;
      *p = snd.buffer[1][i];
      ++p;
  }
  sdl_sound.current_pos = (char*)p;
  sdl_sound.current_emulated_samples += snd.bufsize * 2 * sizeof(short);
  SDL_UnlockAudio();
}

static void sdlsms_sound_close()
{
  SDL_PauseAudio(1);
  SDL_CloseAudio();
  free(sdl_sound.buffer);
}
#endif

/* globals */

int igg_init(const t_config* pcfg)
{
  rom_filename = pcfg->game_name;
  if(load_rom(rom_filename) == 0) {
    printf("ERROR: can't load `%s'.\n", pcfg->game_name);
    return 0;
  }
  else printf("Loaded `%s'.\n", pcfg->game_name);

  printf("Initializing HOTDOG VIDEO SUBSYSTEM... ");
  if(!igg_video_init(pcfg->frameskip, pcfg->display_mode))
    return 0;
  printf("Ok.\n");

  set_keypress();

#ifdef ENABLESOUND
  // FIXME: sound
  if(!pcfg->nosound) {
    printf("Initializing SDL SOUND SUBSYSTEM... ");
    if(!sdlsms_sound_init())
      return 0;
    printf("Ok.\n");
  }
#endif

  /* set up the virtual console emulation */
  printf("Initializing virtual console emulation... ");
  sms.use_fm = pcfg->fm;
  sms.country = pcfg->japan ? TYPE_DOMESTIC : TYPE_OVERSEAS;
  memset(&bitmap, 0, sizeof(t_bitmap));
  bitmap.width  = SMS_SCREEN_WIDTH;
  bitmap.height = SMS_SCREEN_HEIGHT;
  bitmap.depth  = igg_video.bpp;
  bitmap.pitch  = SMS_SCREEN_WIDTH*sizeof(uint32);
  bitmap.data   = (unsigned char*)HD_SRF_PIXELS(igg_video.canvas->canvas);
  system_init(pcfg->nosound ? 0 : SOUND_FREQUENCY);
  if(pcfg->usesram) load_sram(pcfg->game_name);
  printf("Ok.\n");

  return 1;
}

void igg_emulate()
{
  int quit = 0;
  int scroll_pos = 0;
  printf("Starting emulation...\n");

#ifdef ENABLESOUND
  // FIXME: sound
  if(snd.enabled) SDL_PauseAudio(0);
#endif

  while(!quit) {
	kb_poll();
	if(kb_gethold()) quit = 1;
    igg_video_update();
#ifdef ENABLESOUND
    // FIXME: sound.
    if(snd.enabled) sdlsms_sound_update();
#endif
  }
}

void igg_shutdown() 
{
  /* shutdown the virtual console emulation */
  printf("Shutting down virtual console emulation...\n");
  save_sram(rom_filename);
  system_reset();
  system_shutdown();

#ifdef ENABLESOUND
  if(snd.enabled)
    sdlsms_sound_close();
#endif

  reset_keypress();
  igg_video_close();
}
