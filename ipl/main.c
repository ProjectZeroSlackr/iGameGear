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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ipl.h"
#include "saves.h"
#include "shared.h"


static t_config cfg;

static t_named_display displays[] = {
  {"center",  DISPLAY_CENTER },
  {"scale",   DISPLAY_SCALE  },
  {"stretch", DISPLAY_STRETCH}
};


static int parse_args(int argc, char **argv) {
	int i;
	
	/* default virtual console emulation settings */
	sms.use_fm = 0;
	sms.country = TYPE_OVERSEAS;
	
	cfg.display_mode = DISPLAY_CENTER;
	cfg.frameskip = 1;
	cfg.usesram = 1;
	
	strcpy(cfg.game_name, argv[1]);
	
	for(i = 2; i < argc; ++i) {
		if(strcasecmp(argv[i], "--japan") == 0) {
			sms.country = TYPE_DOMESTIC;
#ifdef ENABLESOUND
		} else if(strcasecmp(argv[i], "--fm") == 0) {
			sms.use_fm = 1;
#endif
		} else if(strcasecmp(argv[i], "--nosram") == 0) {
			cfg.usesram = 0;
		} else if(strcasecmp(argv[i], "--fskip") == 0) {
			if(++i < argc) cfg.frameskip = atoi(argv[i])+1;
		} else if(strcasecmp(argv[i], "--fullspeed") == 0) {
			cfg.fullspeed = 1;
#ifdef ENABLESOUND
		} else if(strcasecmp(argv[i], "--nosound") == 0) {
			cfg.nosound = 1;
#endif
		} else if(strcasecmp(argv[i], "--display") == 0) {
			i++;        
			if(i < argc) {
				int j = 0;
				for(; j < sizeof(displays) / sizeof(displays[0]); ++j) {
					if(strcasecmp(displays[j].name, argv[i]) == 0) {
						cfg.display_mode = displays[j].type;
						break;
					}
				}  
			}  
		} else {
			printf("WARNING: unknown option '%s'.\n", argv[i]);
		}
	}
	
#ifndef ENABLESOUND
	// just in case :)
	cfg.nosound = 1;
#endif
	
	return 1;
}


int main(int argc, char **argv) {
	printf("%s (Build date %s)\n", IGG_TITLE, __DATE__);
	printf("(C) Charles Mac Donald in 1998, 1999, 2000\n");
	printf("iPodLinux/Hotdog Port by Luiz \"venox\" Ribeiro (luizribeiro@gmail.com)\n");
	printf("Based on SDL Version by Gregory Montoir (cyx@frenchkiss.net)\n");
	printf("\n");
	
	if(argc < 2) {
		int i;
		printf("Usage: %s <filename> [--options]\n", argv[0]);
		printf("Options:\n");
#ifdef ENABLESOUND
		printf(" --fm           \t enable YM2413 sound.\n");
#endif
		printf(" --japan        \t set the machine type as DOMESTIC instead of OVERSEAS.\n");
		printf(" --nosram       \t do not load SRAM contents before starting.\n");
		printf(" --fskip <n>    \t specify the number of frames to skip.\n");
// FIXME: fullspeed isn't working as there is no timing
//		printf(" --fullspeed    \t do not limit to 60 frames per second.\n");
#ifdef ENABLESOUND
		printf(" --nosound      \t disable sound.\n");
#endif
		printf(" --display <mode>\t sets the display mode: ");
		for(i = 0; i < sizeof(displays) / sizeof(displays[0]) - 1; ++i)
			printf("%s,", displays[i].name);
		printf("%s.", displays[i].name);
		printf("\n");
		return 1;
	}
	
	memset(&cfg, 0, sizeof(cfg));
	if(!parse_args(argc, argv))
		return 0;
	
	if(igg_init(&cfg)) {
		igg_emulate();
		igg_shutdown();
	}
	
	return 0;
}

