/* xtt_audio.cpp -- soundcard bell routines.
   PROVIEW/R
*/
#include <iostream>
#include <math.h>
#include <rt_errh.h>
#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API

#include "rt_gdh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "rt_xnav_msg.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xtt_audio.h"

#define OSS_BUFFER_SIZE 65536
#define ALSA_BUFFER_SIZE 65536
#define AUDIO_BUFFER_SIZE 65536
#define AUDIO_SAMPLE_RATE 44100
//using namespace std;

typedef struct {
  char  groupID[4];
  char  unk[4];
  char  riffType[4];
  char	chunkID[4];
  long	chunkSize;
  short wFormatTag;
  unsigned short wChannels;
  unsigned long dwSamplesPerSec;
  unsigned long dwAvgBytesPerSec;
  unsigned short wBlockAlign;
  unsigned short wBitsPerSample;

  char dataId[4];
  long dataSize;
} wav_sFormat;


int XttAudio::number_of=0;
int XttAudio::audio_ok=0;
int XttAudio::OSS_audio_ok=0;
int XttAudio::ALSA_audio_ok=0;
int XttAudio::OSS_handle=0;
int XttAudio::srate=0; 
Widget XttAudio::parent_wid = 0;
snd_pcm_t *XttAudio::ALSA_handle=NULL;
snd_pcm_uframes_t XttAudio::frames=AUDIO_BUFFER_SIZE/4;
snd_pcm_hw_params_t *XttAudio::hw_params=NULL;
static long unsigned int period_size = 65536;

XttAudio::XttAudio( Widget w, char *OSS_device, char * ALSA_device) :
  write_buffer(0), timerid(0), queue_cnt(0)
{
  /*constructor*/

  number_of++;

  if(number_of<2 && audio_ok==0)
  {
    if(Init_ALSA(ALSA_device,AUDIO_SAMPLE_RATE)>0)
    {
      ALSA_audio_ok=1;
      errh_Info("ALSA audio init of device %s ok",ALSA_device);
    }
    if(Init_OSS(OSS_device,AUDIO_SAMPLE_RATE)>0)
    {
      OSS_audio_ok=1;
      errh_Info("OSS audio init of device %s ok",OSS_device);
    }
    if(ALSA_audio_ok || OSS_audio_ok)
      audio_ok=1;
    else
      errh_Info("No audio device found, using only Bell");
  }
  parent_wid = w;

  // Calculate frequences for tones from ground tone C2
  for ( int i = 0; i < 100; i++)
    freq[i] = 65.4066 * exp( log(2)/12 * i);
}

XttAudio::~XttAudio()
{
  /*destructor*/

  if ( timerid)
    XtRemoveTimeOut( timerid);
    
  number_of--;

  if(audio_ok && (number_of<1))
  {
    audio_ok=0;
    errh_Info("Last xtt_audio instance destructing, closing open devices\n");
    if(OSS_audio_ok)
      close(OSS_handle);
    if(ALSA_audio_ok)
      snd_pcm_close(ALSA_handle);
  }
}

int XttAudio::init(char *OSS_device, char * ALSA_device)
{

  if(number_of<2 && audio_ok==0)
  {
    if(Init_ALSA(ALSA_device,AUDIO_SAMPLE_RATE)>0)
    {
      ALSA_audio_ok=1;
      errh_Info("ALSA audio init of device %s ok",ALSA_device);
    }
    if(Init_OSS(OSS_device,AUDIO_SAMPLE_RATE)>0)
    {
      OSS_audio_ok=1;
      errh_Info("OSS audio init of device %s ok",OSS_device);
    }
    if(ALSA_audio_ok || OSS_audio_ok)
      audio_ok=1;
    else
      errh_Info("No audio device found, using only Bell");
  }
  return audio_ok;	
}

int XttAudio::beep( pwr_tAttrRef *arp)
{
  int size;
  short *buffer;
  pwr_tTid tid;
  pwr_tStatus sts;
  pwr_tAttrRef prioaref;
  pwr_tInt32 prio;

  if ( write_buffer) {

    // Busy, add to que...
    if ( queue_cnt == AUDIO_QUESIZE) {
      // Queue full, skip beep
      return XNAV__QUEUEFULL;
    }
    // Check if already inserted
    for ( int i = 0; i < queue_cnt; i++) {
      if ( cdh_ArefIsEqual( arp, &queue[i]))
	return XNAV__SUCCESS;
    }
    // Insert in priority order
    // Get prio

    sts = gdh_ArefANameToAref( arp, "Prio", &prioaref);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetObjectInfoAttrref( &prioaref, (void *) &prio, sizeof(prio));
    if ( EVEN(sts)) return sts;

    for ( int i = queue_cnt - 1; i >= -1; i--) {
      if ( i == -1) {
	queue[0] = *arp;
	queue_prio[0] = prio;
	queue_cnt++;
	break;
      }
      if ( queue_prio[i] >= prio) {
	// Insert
	queue[i+1] = *arp;
	queue_prio[i+1] = prio;
	queue_cnt++;
	break;
      }
      else {
	// Shift
	queue[i+1] = queue[i];
	queue_prio[i+1] = queue_prio[i];
      }
    }
    
    return XNAV__QUEUED;
  }

  sts = 0;
  if ( arp && cdh_ObjidIsNotNull( arp->Objid)) {
    while ( 1) {
      sts = gdh_GetAttrRefTid( arp, &tid);
      if ( EVEN(sts)) break;

      if ( !(tid == pwr_cClass_Sound || tid == pwr_cClass_SoundSequence)) {
	sts = 0;
	break;
      }

      switch ( tid) {
      case pwr_cClass_Sound: {
	pwr_sClass_Sound sound;

	sts = gdh_GetObjectInfoAttrref( arp, &sound, sizeof(sound));
	if ( EVEN(sts)) break;

	if ( strncmp( sound.Source, "Sine", 4) == 0 ||
	     strncmp( sound.Source, "Square", 6) == 0) {
	  size = 2 * int(sound.Length * srate);
	  size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	  buffer = (short *) calloc( sizeof(short), size);
	  if(!buffer) return XNAV__NOMEMORY;

	  for ( int i = 0; i < int(sizeof(sound.ToneTable)/sizeof(sound.ToneTable[0])); i++) {
	    if ( sound.VolumeTable[i] > 0) {
	      if ( strncmp( sound.Source, "Sine", 4) == 0) {
		MakeSine( buffer, size, 0, 0, sound.Length, sound.BaseTone + sound.ToneTable[i], 
			  sound.Volume/100 * sound.VolumeTable[i], sound.Volume/100 * sound.VolumeTable[i], 
			  sound.Attack, sound.Decay, sound.Sustain/100, sound.Release, sound.Tremolo/100);
	      }
	      else if ( strncmp( sound.Source, "Square", 6) == 0) {
		MakeSquare( buffer, size, 0, 0, sound.Length, sound.BaseTone + sound.ToneTable[i], 
			    sound.Volume/100 * sound.VolumeTable[i], sound.Volume/100 * sound.VolumeTable[i], 
			    sound.Attack, sound.Decay, sound.Sustain/100, sound.Release, sound.Tremolo/100);
	      }
	    }
	  }
	}
	else if ( strstr( sound.Source, ".wav") != 0) {
	  // Wave file
	  FILE *fp;
	  wav_sFormat chunk;
	  pwr_tFileName fname;

	  dcli_translate_filename( fname, sound.Source);
	  fp = fopen( fname, "r");
	  if ( !fp) {
	    printf("Unable to open file %s\n", fname);
	    return XNAV__NOFILE;
	  }

	  sts = fread( &chunk, sizeof(chunk), 1, fp);
	  if ( sts == 0)
	    return XNAV__FILECORRUPT;

	  if ( !(chunk.wFormatTag == 1 &&
		 (chunk.wBitsPerSample == 16 || chunk.wBitsPerSample == 8) &&
		 (chunk.wChannels == 2 || chunk.wChannels == 1))) {
	    printf( "Unsupported wavefile format %s\n", fname);
	    return XNAV__WAVEFORMAT;
	  }

	  if ( chunk.dataSize == 0 || chunk.dataSize > 10000000)
	    return XNAV__FILECORRUPT;
	  if ( chunk.wBitsPerSample == 8) {
	    unsigned char *buf8 = (unsigned char *) malloc( chunk.dataSize);
	    if ( !buf8) return XNAV__NOMEMORY;

	    sts = fread( buf8, chunk.dataSize, 1, fp);
	    fclose(fp);
	    
	    if ( chunk.wChannels == 1) {
	      size = chunk.dataSize * 2;
	      size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	      buffer = (short *) calloc( sizeof(short), size);
	      if ( !buffer) return XNAV__NOMEMORY;

	      for ( int i = 0; i < chunk.dataSize; i++) {
		buffer[2*i] = buffer[2*i+1] = 255 * ((short) buf8[i] - 127);
	      }
	    }
	    else {
	      // wChannels == 2
	      size = chunk.dataSize;
	      size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	      buffer = (short *) calloc( sizeof(short), size);
	      if ( !buffer) return XNAV__NOMEMORY;

	      for ( int i = 0; i < chunk.dataSize; i++) {
		buffer[i] = 255 * ((short)buf8[i] - 127);
	      }
	    }
	    free( buf8);
	  }
	  else {
	    // wBitsPerSample == 16

	    if ( chunk.wChannels == 1) {
	      short *buf16 = (short *) malloc( chunk.dataSize*2);
	      if ( !buf16) return XNAV__NOMEMORY;

	      sts = fread( buf16, chunk.dataSize, 1, fp);
	      fclose(fp);

	      size = chunk.dataSize * 2;
	      size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	      buffer = (short *) calloc( sizeof(short), size);
	      if ( !buffer) return XNAV__NOMEMORY;

	      for ( int i = 0; i < chunk.dataSize; i++) {
		buffer[2*i] = buffer[2*i+1] = buf16[i];
	      }
	      free( buf16);
	    }
	    else {
	      // wChannels == 2
	      size = chunk.dataSize / 2;
	      size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	      buffer = (short *) calloc( sizeof(short), size);
	      if ( !buffer) return XNAV__NOMEMORY;

	      sts = fread( buffer, chunk.dataSize, 1, fp);
	      fclose( fp);
	    }
	  }
	}
	break;
      }
      case pwr_cClass_SoundSequence: {
	pwr_sClass_SoundSequence seq;
	pwr_sClass_Sound sound[3];
	int sound_cnt;
	int j;
	
	memset( sound, 0, sizeof(sound));
	sts = gdh_GetObjectInfoAttrref( arp, &seq, sizeof(seq));
	if ( EVEN(sts)) break;

	for ( j = 0; j < 3; j++) {
	  if ( cdh_ObjidIsNotNull( seq.SoundObject[j].Objid)) {
	    sts = gdh_GetObjectInfoAttrref( &seq.SoundObject[j], &sound[j], sizeof(sound[j]));
	    if ( EVEN(sts))
	      break;
	  }
	  else
	    break;
	}
	sound_cnt = j;

	size = 2 * int(seq.Length * srate);
	size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
	buffer = (short *) calloc( sizeof(short), size);
	if(!buffer) return XNAV__NOMEMORY;

	for ( int i = 0; i < int(sizeof(seq.SequenceTable)/sizeof(seq.SequenceTable[0])); i++) {
	  if ( !seq.SequenceTable[i].Used ||
	       (seq.SequenceTable[i].VolumeRight == 0 && seq.SequenceTable[i].VolumeLeft == 0))
	    continue;

	  if ( (int)seq.SequenceTable[i].SoundIdx >= sound_cnt)
	    continue;

	  for ( int j = 0; j < int(sizeof(sound[0].ToneTable)/sizeof(sound[0].ToneTable[0])); j++) {
	    if ( sound[seq.SequenceTable[i].SoundIdx].VolumeTable[j] > 0) {
	      if ( strncmp( sound[seq.SequenceTable[i].SoundIdx].Source, "Sine", 4) == 0) {
		MakeSine( buffer, size, 0, seq.SequenceTable[i].StartTime, 
			  seq.SequenceTable[i].EndTime, 
			  seq.SequenceTable[i].Tone + sound[seq.SequenceTable[i].SoundIdx].ToneTable[j], 
			  seq.SequenceTable[i].VolumeRight/100 * sound[seq.SequenceTable[i].SoundIdx].VolumeTable[j],
			  seq.SequenceTable[i].VolumeLeft/100 * sound[seq.SequenceTable[i].SoundIdx].VolumeTable[j],
			  sound[seq.SequenceTable[i].SoundIdx].Attack, 
			  sound[seq.SequenceTable[i].SoundIdx].Decay, 
			  sound[seq.SequenceTable[i].SoundIdx].Sustain/100, 
			  sound[seq.SequenceTable[i].SoundIdx].Release,
			  sound[seq.SequenceTable[i].SoundIdx].Tremolo/100);
	      }
	      else if ( strncmp( sound[seq.SequenceTable[i].SoundIdx].Source, "Square", 6) == 0) {
		MakeSquare( buffer, size, 0, seq.SequenceTable[i].StartTime, 
			    seq.SequenceTable[i].EndTime,
			    seq.SequenceTable[i].Tone + sound[seq.SequenceTable[i].SoundIdx].ToneTable[j], 
			    seq.SequenceTable[i].VolumeRight,
			    seq.SequenceTable[i].VolumeLeft,
			    sound[seq.SequenceTable[i].SoundIdx].Attack, 
			    sound[seq.SequenceTable[i].SoundIdx].Decay, 
			    sound[seq.SequenceTable[i].SoundIdx].Sustain/100, 
			    sound[seq.SequenceTable[i].SoundIdx].Release,
			    sound[seq.SequenceTable[i].SoundIdx].Tremolo/100);
	      }
	    }
	  }

	}

	if ( seq.Echo > 0) {
	  double val;
	  int offs = 2 * (int) (0.25 * srate);
	  for ( int i = offs; i < size; i += 2) {
	    val = seq.Echo / 100 * buffer[i - offs] + buffer[i];
	    if ( val > 32767)
	      buffer[i] = 32767;
	    else
	      buffer[i] = (unsigned short)val;
	    val = seq.Echo / 100 * buffer[i+1 - offs] + buffer[i+1];
	    if ( val > 32767)
	      buffer[i+1] = 32767;
	    else
	      buffer[i+1] = (unsigned short)val;
	  }
	}

	break;
      }
      }
      break;
    }
  }
  if ( EVEN(sts)) {
    // Standard beep
    size = 2 * int( 0.3 * srate);
    size = ((size - 1) / (AUDIO_BUFFER_SIZE/2) + 1) * AUDIO_BUFFER_SIZE/2;
    buffer = (short *) calloc( sizeof(short), size);
    if(!buffer) return XNAV__NOMEMORY;

    MakeSine( buffer, size, 0, 0, 0.3, 38, 100.0, 100.0, 0, 0, 0, 0, 0);
  }

  write_buffer = buffer;
  write_buffer_size = size;
  write_buffer_idx = 0;
  audio_write( this);

  return XNAV__SUCCESS;
}

void XttAudio::audio_write( XttAudio *audio)
{
  int rc; 

  audio->timerid = 0;

  if ( audio->OSS_audio_ok) {
    int size = audio->write_buffer_size - audio->write_buffer_idx;
    if ( size > OSS_BUFFER_SIZE/2)
      size = OSS_BUFFER_SIZE/2;
    rc = write( audio->OSS_handle, &audio->write_buffer[audio->write_buffer_idx], 
		size*2);
    if (rc == -EPIPE) {
      fprintf(stderr, "OSS audio underrun occurred\n");
    }
    else if (rc < 0) {
      printf("OSS audio error from write\n");
      close( audio->OSS_handle);
      free( audio->write_buffer);
      audio->write_buffer = 0;
      return;
    }
    else if (rc != size*2) {
      fprintf(stderr,"OSS audio short write, write %d frames\n", rc);
    }
    audio->write_buffer_idx += size;
    if ( audio->write_buffer_idx >= audio->write_buffer_size) {
      free( audio->write_buffer);
      audio->write_buffer = 0;
    }    
    else {
      int time = 1000 * size/2 / srate + 10;
      audio->timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(parent_wid), time,
	(XtTimerCallbackProc)XttAudio::audio_write, audio);
    }
  }

  else if(ALSA_audio_ok) {
    int size = audio->write_buffer_size - audio->write_buffer_idx;
    if ( size > ALSA_BUFFER_SIZE/2)
      size = ALSA_BUFFER_SIZE/2;

    printf( "Write idx: %d totsize: %d size: %d\n", audio->write_buffer_idx, audio->write_buffer_size, size);

    snd_pcm_prepare( audio->ALSA_handle);
    rc = snd_pcm_writei( audio->ALSA_handle, &audio->write_buffer[audio->write_buffer_idx], 
			 size/2);
    if (rc == -EPIPE) {
      // EPIPE means underrun
      fprintf(stderr, "ALSA audio underrun occurred\n");
      int time = 20;
      audio->timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(parent_wid), time,
	(XtTimerCallbackProc)XttAudio::audio_write, audio);
      return;
      // snd_pcm_prepare( audio->ALSA_handle);
    }
    else if (rc < 0) {
      printf("ALSA audio error from writei:%s\n",snd_strerror(rc));
      snd_pcm_close( audio->ALSA_handle);
      free( audio->write_buffer);
      audio->write_buffer = 0;
      return;
    }
    else if (rc != size/2) {
     fprintf(stderr,"ALSA audio short write, write %d frames\n", rc);
    }
    audio->write_buffer_idx += size;
    if ( audio->write_buffer_idx < audio->write_buffer_size) {
      // Submit next write
      int time = 1000 * size/2 / srate;
      audio->timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext(parent_wid), time,
	(XtTimerCallbackProc)XttAudio::audio_write, audio);
    }    
    else {
      // Free buffer
      free( audio->write_buffer);
      audio->write_buffer = 0;

      // Process next on queue
      if ( audio->queue_cnt) {
	pwr_tAttrRef aref = audio->queue[0];
	for ( int i = 0; i < audio->queue_cnt - 1; i++)
	  audio->queue[i] = audio->queue[i+1];
	audio->queue_cnt--;

	audio->beep( &aref);
      }
    }
  }
}

int XttAudio::Init_ALSA(char *device, unsigned int samplerate)
{
  int err;

  if ((err = snd_pcm_open (&ALSA_handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    return -1;
  else 
  {
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0)
    {
      fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_hw_params_any (ALSA_handle, hw_params)) < 0)
    {
      fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_hw_params_set_access (ALSA_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
      fprintf (stderr, "cannot set access type (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_hw_params_set_format (ALSA_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
      fprintf (stderr, "cannot set sample format (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
#if defined SND_LIB_MAJOR && SND_LIB_MAJOR < 1
    // Alsa 0.9...
    if ((err = snd_pcm_hw_params_set_rate_near (ALSA_handle, hw_params, samplerate, 0)) < 0)
#else
    if ((err = snd_pcm_hw_params_set_rate_near (ALSA_handle, hw_params, &samplerate, 0)) < 0)
#endif
    {
      fprintf (stderr, "cannot set sample rate (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_hw_params_set_channels (ALSA_handle, hw_params, 2)) < 0)
    {
      fprintf (stderr, "cannot set channel count (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
#if defined SND_LIB_MAJOR && SND_LIB_MAJOR < 1
    // Alsa 0.9...
    if((err =  snd_pcm_hw_params_set_period_size_near(ALSA_handle, hw_params, period_size, 0)) <0)
#else
    if((err =  snd_pcm_hw_params_set_period_size_near(ALSA_handle, hw_params, &period_size, 0)) <0)
#endif
    {
      fprintf(stderr, "could not set period size (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_hw_params (ALSA_handle, hw_params)) < 0)
    {
      fprintf (stderr, "cannot set parameters (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    if ((err = snd_pcm_prepare (ALSA_handle)) < 0)
    {
      fprintf (stderr, "cannot prepare audio interface for use (%s)\n",snd_strerror (err));
      snd_pcm_close(ALSA_handle);
      return -1;
    }
    srate = samplerate;
    return 1;
  }
}

int XttAudio::Init_OSS(char *device, int samplerate)
{
  int err;
  int format=AFMT_S16_LE;
  int samplesize=16;
  int channels=2;
  
  if ((OSS_handle = open (device, O_RDWR, 0)) < 0)
    return -1;
  else 
  {
    if((err=ioctl(OSS_handle, SNDCTL_DSP_SAMPLESIZE,&samplesize)) <0)
    {
      printf("err:%i, format:%i \n",err,samplesize);
      close(OSS_handle);
      return -1;
    }
    if((err=ioctl(OSS_handle, SNDCTL_DSP_STEREO, &channels)) <0)
    {
      printf("err:%i, format:%i \n",err,channels);
      close(OSS_handle);
      return -1;      
    }
    if((err=ioctl(OSS_handle, SNDCTL_DSP_SPEED, &samplerate)) <0)
    {
      printf("err:%i, format:%i \n",err,samplerate);
      close(OSS_handle);
      return -1;
    }
    if((err=ioctl(OSS_handle, SNDCTL_DSP_SETFMT, &format)) <0)
    {
      printf("err:%i, format:%i \n",err,format);
      close(OSS_handle);
      return -1;            
    }
  }
  return 1;
  
}

double XttAudio::envelope( double time, double endtime, double attack, double decay, double sustain, 
			   double release)
{
#if 0
  // Version with linear decay, replaced by exponential decay
  if ( time < endtime) {
    if ( time >= attack + decay)
      return sustain;
    else if ( time >= attack)
      return 1.0 - (1.0 - sustain) * (time - attack) / decay;
    return time / attack;
  }
  if ( time >= endtime + release)
    return 0;
  return (1.0 - (time - endtime) / release) * sustain;
#endif
  if ( time < endtime) {
    if ( time <= attack)
      return time / attack;
    if ( sustain == 1.0)
      return 1.0;
    return sustain + (1.0 - sustain) * exp( -(time - attack) / decay);
  }
  if ( time >= endtime + release)
    return 0;
  if ( sustain == 1.0)
    return ( 1.0 - (time - endtime) / release);
  return (1.0 - (time - endtime) / release) * (sustain + (1.0 - sustain) * exp( -(endtime - attack) / decay));
}

void XttAudio::MakeSine(short *buffer, int buffersize, double time, double starttime, double endtime,
			int tone, double volume_ch1, double volume_ch2, double attack, double decay,  
			double sustain, double release, double tremolo)
{
  int i;
  double val;
  double valtremolo;
  double sum;
  double ampl;
  double t;

  t = starttime;
  for( i = 2 * int( starttime * srate); i < buffersize; i += 2, t += 1.0/srate) {
    if ( t < starttime)
      continue;

    ampl = envelope( t - starttime, endtime - starttime, attack, decay, sustain, release);
    if ( ampl == 0)
      continue;
    
    val = ampl * sin( freq[tone] * t * 2 * M_PI);

    if ( tremolo != 0)
      valtremolo = 1 - tremolo * (sin( t * 6 * 2 * M_PI) + 1) / 2;
    else
      valtremolo = 1;

    sum = 327.67 * volume_ch1 * val * valtremolo + buffer[i];
    if ( sum > 32767)
      buffer[i] = 32767;
    else
      buffer[i] = (unsigned short) sum;

    sum = 327.67 * volume_ch2 * val * valtremolo + buffer[i+1];
    if ( sum > 32767)
      buffer[i+1] = 32767;
    else
      buffer[i+1] = (unsigned short) sum;
  }
}

void XttAudio::MakeSquare(short *buffer, int buffersize, double time, double starttime, double endtime,
			  int tone, double volume_ch1, double volume_ch2, double attack, double decay,  
			  double sustain, double release, double tremolo)
{
  int i;
  double val;
  double valtremolo;
  double sum;
  double ampl;
  double t;

  t = starttime;
  for( i = 2 * int( starttime * srate); i < buffersize; i += 2, t += 1.0/srate) {
    if ( t < starttime)
      continue;

    ampl = envelope( t - starttime, endtime - starttime, attack, decay, sustain, release);
    if ( ampl == 0)
      continue;
    
    val = ampl * ( int(freq[tone] * t * 2) % 2 * 2 - 1);

    if ( tremolo != 0)
      valtremolo = 1 - tremolo * (sin( t * 6 * 2 * M_PI) + 1) / 2;
    else
      valtremolo = 1;

    sum = 327.67 * volume_ch1 * val * valtremolo + buffer[i];
    if ( sum > 32767)
      buffer[i] = 32767;
    else
      buffer[i] = (unsigned short) sum;

    sum = 327.67 * volume_ch2 * val * valtremolo + buffer[i+1];
    if ( sum > 32767)
      buffer[i+1] = 32767;
    else
      buffer[i+1] = (unsigned short) sum;
  }
}


