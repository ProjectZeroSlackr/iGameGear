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

#include "ipl.h"
#include "hotdog.h"

t_igg_video igg_video;

/* video */
static void update(hd_engine *eng, int x, int y, int w, int h) {
	HD_LCD_Update(eng->screen.framebuffer, 0, 0, igg_video.width, igg_video.height);
}

static void set_displaymode(int display_mode) {
	igg_video.display_mode = display_mode;
	switch(igg_video.display_mode) {
		case DISPLAY_STRETCH:
			// FIXME: not working correctly for gamegear
			igg_video.canvas->x = 0;
			igg_video.canvas->y = 0;
			igg_video.canvas->w = igg_video.width;
			igg_video.canvas->h = igg_video.height;
			break;
		case DISPLAY_CENTER:
			igg_video.canvas->x = -(SMS_SCREEN_WIDTH - (BMP_X_OFFSET + igg_video.width)) >> 1;
			igg_video.canvas->y = -(SMS_SCREEN_HEIGHT - igg_video.height) >> 1;
			igg_video.canvas->w = SMS_SCREEN_WIDTH;
			igg_video.canvas->h = SMS_SCREEN_HEIGHT;
			break;
		case DISPLAY_SCALE:
			// FIXME: not working correctly for gamegear
			igg_video.canvas->w = SMS_SCREEN_WIDTH*igg_video.height/SMS_SCREEN_HEIGHT;
			igg_video.canvas->h = igg_video.height;
			igg_video.canvas->y = 0;
			igg_video.canvas->x = (igg_video.canvas->w - igg_video.width) >> 1;
			break;
	}
}

int igg_video_init(int frameskip, int display_mode) {
	HD_LCD_Init();
	HD_LCD_GetInfo(&igg_video.hwver, &igg_video.width, &igg_video.height, &igg_video.lcdtype);
	
	igg_video.frame_skip = frameskip;
	igg_video.bpp = 16; // hardcoded bpp
	
	igg_video.screen = NULL;
	igg_video.screen = malloc(igg_video.width * igg_video.height * sizeof(uint32));
	if(!igg_video.screen) {
		printf("ERROR: cannot allocate memory for the screen.\n");
		return 0;
	}
	
	igg_video.engine = HD_Initialize(igg_video.width, igg_video.height, igg_video.bpp, igg_video.screen, update);
	
	igg_video.canvas = HD_Canvas_Create(SMS_SCREEN_WIDTH, SMS_SCREEN_HEIGHT);
	//igg_video.canvas->speed = HD_SPEED_NOALPHA;
	
	set_displaymode(display_mode);
	
	HD_Register(igg_video.engine, igg_video.canvas);
	
	igg_video.frames_rendered = 0;
	
	return 1;
}

void igg_video_update() {
	int skip_current_frame = 0;
	
	if(igg_video.frame_skip > 1) skip_current_frame = ((igg_video.frames_rendered % igg_video.frame_skip) == 0) ? 0 : 1;
	
	sms_frame(skip_current_frame);
	if(!skip_current_frame) igg_video.canvas->dirty = 1;
	
	HD_Render(igg_video.engine);
	
	igg_video.frames_rendered++;
}

void igg_video_close() {
	HD_Destroy(igg_video.canvas);
	HD_LCD_Quit();
	if(igg_video.screen) HD_FreeSurface(igg_video.screen);
	printf("[INFO] Frames rendered = %lu.\n", igg_video.frames_rendered);
}
