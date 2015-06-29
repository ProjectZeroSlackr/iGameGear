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

/*****
	This Input Code is based on the iBoy's input code.
	Actually, it's almost the same code with some mods to fit on iGameGear.
*****/

#include <termios.h> // tcgetattr, etc
#include <unistd.h>  // same
#include "ipl.h"
#include "input.h"

int push_map[5] = {
        INPUT_BUTTON1,                                  //x01 = IPOD_CENTER
        INPUT_BUTTON2,                                  //x02 = IPOD_RIGHT
        INPUT_BUTTON2,                                  //x04 = IPOD_LEFT
        INPUT_BUTTON2,                                  //x08 = IPOD_DOWN
        INPUT_BUTTON2,                                  //x10 = IPOD_UP
};

int touch_map[8] = {
        INPUT_UP,
        INPUT_BUTTON2,
        INPUT_RIGHT,
        INPUT_BUTTON3, /* used as start */
        INPUT_DOWN,
        0,
        INPUT_LEFT,
        INPUT_BUTTON1
};

int push_combo[32];
int spec_combo[32];

static struct termios stored_settings;
void set_keypress(void) {
	int i, j, bit, h;
	struct termios tattr;
	tcgetattr(STDIN_FILENO, &stored_settings);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);

	// build a map for all possible combinations
	for(i=0; i<32; i++) {
			h = 1;
			push_combo[i] = 0;
			for(bit=0;bit<5;bit++,h<<=1)
				if(i&h) push_combo[i] |= push_map[bit];

		for(j=0; j<32; j++){
			if(spec_combo[j&i] != 0) {
				h = 1;
				for(bit=0;bit<5;bit++,h<<=1){
					if(j&h)
						push_combo[i] &= ~push_map[bit];
				}
				push_combo[i] |= spec_combo[i&j];
			}
		}
	}
}

void reset_keypress(void) {
	tcsetattr(STDIN_FILENO, TCSANOW, &stored_settings);
}

static void pad_press(int k) {
	if(k & INPUT_BUTTON3) input.system |= (IS_GG) ? INPUT_START : INPUT_PAUSE;
	input.pad[0] |= k;
}

static void pad_release(int k) {
	if(k & INPUT_BUTTON3) input.system &= (IS_GG) ? ~INPUT_START : ~INPUT_PAUSE;
	input.pad[0] &= ~k;
}

static void sense_kb(int *press, int *touch) {
	int in, st;

	*touch = 0xff;

	if(!(inl(0x6000d030)&0x20)) {
		*press = IPOD_HOLD;
		return;
	}

	in = inl(0x7000C140);
	st = ((in & 0xff000000)>>24);
	*press = (in & 0x00001F00) >> 8;

	if(*press == 0x18) *press = IPOD_HOLD;
	if(st == 0xc0) *touch = (in & 0x007F0000 ) >> 16;
}

int kb_gethold()
{
	int press, touch;
	sense_kb(&press, &touch);
	return press==IPOD_HOLD;
}

void kb_poll()
{
	int touch, button;

	pad_release(0xFFFFFFFF); // release ALL
	pad_release(INPUT_START);

	sense_kb(&button, &touch);

	if(button) pad_press(push_combo[button]);
		/*if(button!=IPOD_HOLD) pad_press(push_combo[button]);
		else enter_menu();
	}*/

	if(touch!=0xff) { 	// touched
		touch+=6;
		touch/=12;
		if(touch>7) touch=0;
		pad_press(touch_map[touch]);
	}
}
