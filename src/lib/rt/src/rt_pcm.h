/* 
 * Proview   $Id: rt_pcm.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_pcm_h
#define rt_pcm_h

#define FILENAMESIZE 128

#define	C_PCM_NOP	0
#define	C_PCM_NOP_CNT	2

#define	C_PCM_MSG	1
#define	C_PCM_MSG_CNT	4

#define	C_PCM_LOAD	2
#define	C_PCM_LOAD_CNT	4

#define	C_PCM_DONE	3
#define	C_PCM_DONE_CNT	2

#define	C_PCM_LOADDBG	4
#define	C_PCM_LOADDBG_CNT 4

#define	C_PCM_VERSION	1

#define pcm_cProcNewsReceiver 5194
#define pcm_cClassNews 205
#define pcm_cNews_HotSwitchInit 1
#define pcm_cNews_HotSwitchDone 2

#define pcm_cNewsVersion 1

typedef enum {
  pcm_eNews__			= 0,
  pcm_eNews_HotSwitchInit	= pcm_cNews_HotSwitchInit,
  pcm_eNews_HotSwitchDone	= pcm_cNews_HotSwitchDone,
  pcm_eNews_
} pcm_eNews;

typedef struct pcm_s_News pcm_sNews;

struct pcm_s_News {
  pwr_tVersion	  Version;	/* version of this message */
  pwr_tNodeIndex  NodeIndex;	/* index of source node */
  pwr_tTime	  Time;
  pcm_eNews	  News;
};

#endif
/* End rt_pcm.h */
