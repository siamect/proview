/* 
 * Proview   $Id: xtt_audio.h,v 1.1 2005-11-14 16:17:13 claes Exp $
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

#ifndef xtt_audio_h
#define xtt_audio_h

/* xtt_audio.h -- soundcard bell routines.
*/

#include <iostream.h>
#include <alsa/asoundlib.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>

#include "pwr.h"

#define ALSA_PCM_NEW_HW_PARAMS_API
#ifndef XTTAUDIO_H
#define XTTAUDIO_H
#endif

#define XTT_AUDIO_AALARM 0x01
#define XTT_AUDIO_BALARM 0x02
#define XTT_AUDIO_INFO   0x03

#define AUDIO_QUESIZE 20


class XttAudio
{
  public:
    static int audio_ok;
    XttAudio( Widget w = 0, char *OSS_device="/dev/dsp", char *ALSA_device="plughw:0,0");
    ~XttAudio();
    int init(char *OSS_device="/dev/dsp", char *ALSA_device="plughw:0,0");
    int beep( pwr_tAttrRef *arp);
  private:
    static int number_of;
    static int OSS_audio_ok;
    static int ALSA_audio_ok;
    static int OSS_handle;
    static snd_pcm_t *ALSA_handle;
    static snd_pcm_uframes_t frames;
    static snd_pcm_hw_params_t *hw_params;
    static int srate;
    static Widget parent_wid;
    double freq[100];
    short *write_buffer;
    int write_buffer_size;
    int write_buffer_idx;
    XtIntervalId  timerid;
    pwr_tAttrRef queue[AUDIO_QUESIZE];
    int queue_cnt;

    int Init_ALSA(char *, unsigned int);
    int Init_OSS(char *, int);
    double XttAudio::envelope( double time, double endtime, double attack, 
			       double decay, double sustain, double release);
    void MakeSine(short *buffer, int buffersize, double time, double starttime, double endtime,
		  int tone, double volume_ch1, double volume_ch2, double attack, double decay, 
		  double sustain, double release);
    void MakeSquare(short *buffer, int buffersize, double time, double starttime, double endtime,
		    int tone, double volume_ch1, double volume_ch2, double attack, double decay, 
		    double sustain, double release);
    static void audio_write( XttAudio *);
};

#endif
