/* 
 * Proview   $Id$
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* rt_io_m_usb_joystick.c -- I/O methods for USB joysticks. */

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "pwr.h"
#include "co_time.h"
#include "co_cdh.h"

#include <linux/input.h>
#include <linux/joystick.h>
#include <errno.h>

typedef struct {
  int fd;
  int axis_map[ABS_MAX + 1];
  int button_map[KEY_MAX - BTN_MISC + 1];
} io_sLocalUSB_Joystick;

static char *axis_names[ABS_MAX + 1] = {
  "X", "Y", "Z", "Rx", "Ry", "Rz", "Throttle", "Rudder", 
  "Wheel", "Gas", "Brake", "?", "?", "?", "?", "?",
  "Hat0X", "Hat0Y", "Hat1X", "Hat1Y", "Hat2X", "Hat2Y", "Hat3X", "Hat3Y",
  "?", "?", "?", "?", "?", "?", "?", 0,
};

static char *button_names[KEY_MAX - BTN_MISC + 1] = {
  "Btn0", "Btn1", "Btn2", "Btn3", "Btn4", "Btn5", "Btn6", "Btn7", 
  "Btn8", "Btn9", "?", "?", "?", "?", "?", "?",
  "LeftBtn", "RightBtn", "MiddleBtn", "SideBtn", "ExtraBtn", "ForwardBtn", "BackBtn", "TaskBtn",
  "?", "?", "?", "?", "?", "?", "?", "?",
  "Trigger", "ThumbBtn", "ThumbBtn2", "TopBtn", "TopBtn2", "PinkieBtn", "BaseBtn", "BaseBtn2",
  "BaseBtn3", "BaseBtn4", "BaseBtn5", "BaseBtn6", "BtnDead", "BtnA", "BtnB", "BtnC", 
  "BtnX", "BtnY", "BtnZ", "BtnTL", "BtnTR", "BtnTL2", "BtnTR2", "BtnSelect", 
  "BtnStart", "BtnMode", "BtnThumbL", "BtnThumbR", "?", "?", "?", "?", 
  "?", "?", "?", "?", "?", "?", "?", "?", 
  "?", "?", "?", "?", "?", "WheelBtn", "Gear up", 0,
};

#define NAME_LENGTH 128

static void usage()
{
  printf( "\n\
Show joystick axes and button configuration\n\n\
Argument:\n\
device, eg /dev/input/js0\n\n\
> rt_joystick /dev/input/js0\n\n");
}

int main (int argc, char **argv)
{
  if ( argc < 2) {
    usage();
    exit(0);
  }

  if ( strcmp( argv[1], "-h") == 0) {
    usage();
    exit(0);
  }

  int i;
  int fd;
  unsigned char axes = 2;
  unsigned char buttons = 2;
  uint8_t axmap[ABS_MAX + 1];
  uint16_t btnmap[KEY_MAX - BTN_MISC + 1];
  char name[NAME_LENGTH] = "Unknown";

  fd = open( argv[1], O_RDONLY);
  if ( fd == -1) {
    printf( "USB_Joystick, unable to attach device, sts %d, '%s'\n", errno, argv[1]);
    exit(0);
  }

  ioctl( fd, JSIOCGNAME(NAME_LENGTH), name);
  ioctl( fd, JSIOCGAXES, &axes);
  ioctl( fd, JSIOCGBUTTONS, &buttons);
  ioctl( fd, JSIOCGAXMAP, axmap);
  ioctl( fd, JSIOCGBTNMAP, btnmap);

  printf( "\nAxes and button configuration\n\n");
  printf( "Joystick: %s\n", name);

  printf( "\nAxes:\n\n");
  for (i = 0; i < axes; i++)
    printf("%d	%s\n", i+1, axis_names[axmap[i]]);
  
	  
  printf( "\nButtons:\n\n");
  for (i = 0; i < buttons; i++)
    printf("%d	%s\n", i+1, button_names[btnmap[i] - BTN_MISC]);
  printf( "\n");

  return 0;
}
