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

#ifndef __INPUT_H__
#define __INPUT_H__

#define IPOD_LEFT	0x04
#define IPOD_RIGHT	0x02
#define IPOD_UP		0x10
#define IPOD_DOWN	0x08
#define IPOD_CENTER	0x01
#define IPOD_HOLD	0x80

void set_keypress(void);
void reset_keypress(void);
int kb_gethold();
void kb_poll();

typedef struct {
  int up,down,left,right;
  int b1,b2;
  int start;
} t_paddle;

typedef struct {
  int state_slot;
  t_paddle pad[2];
} t_igg_controls;

#endif // ndef __INPUT_H__
