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

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "ipl.h"

int igg_video_init(int frameskip, int display_mode);
void igg_video_update();
void igg_video_close();

extern t_igg_video igg_video;

#endif // ndef __VIDEO_H__
