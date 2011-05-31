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

/* misc_plc.c  Plc function object code. */

#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_miscellaneousclasses.h"
#include "rt_gdh.h"
#include "rt_plc.h"
#include "co_time.h"

#define BASE_SPEED 10
#define MAX_ANGLE 60

static float my_random() {
  static unsigned int seed = 100;
  
  seed = rand_r(&seed);

  return (float)seed/RAND_MAX * 2 - 1;
}

/*_*
  Misc_PingPongFo

  @aref misc_pingpong
  @aref misc_pingpongfo Misc_PingPongFo
*/
void Misc_PingPongFo_init( pwr_sClass_Misc_PingPongFo *o)
{
  pwr_tDlid dlid;
  pwr_tStatus sts;
  pwr_sClass_Misc_PingPong *co;

  sts = gdh_DLRefObjectInfoAttrref( &o->PlcConnect, (void **)&o->PlcConnectP, &dlid);
  if ( EVEN(sts))
    o->PlcConnectP = 0;

  co = (pwr_sClass_Misc_PingPong *) o->PlcConnectP;
  co->LeftKnobPtr = (pwr_tFloat32 *)gdh_TranslateRtdbPointer( (unsigned long)co->LeftKnob.ActualValue);
  co->RightKnobPtr = (pwr_tFloat32 *)gdh_TranslateRtdbPointer( (unsigned long)co->RightKnob.ActualValue);
  co->XCoordinate = co->Height/2;
  co->YCoordinate = -co->BallRadius;
  co->Speed = 15;
  co->Direction = -20;
}

void Misc_PingPongFo_exec( plc_sThread *tp, pwr_sClass_Misc_PingPongFo *o)
{
  pwr_sClass_Misc_PingPong *co = (pwr_sClass_Misc_PingPong *) o->PlcConnectP;
  if ( !co || fabs(co->Width < FLT_EPSILON))
    return;

  co->XCoordinate += co->Speed * tp->PlcThread->ScanTime * cos(co->Direction/180*M_PI);
  co->YCoordinate -= co->Speed * tp->PlcThread->ScanTime * sin(co->Direction/180*M_PI);

  if ( co->UseLeftKnob)
    co->LeftPos = *co->LeftKnobPtr;
  else
    co->LeftPos = co->LeftSlider;
  
  if ( co->UseRightKnob)
    co->RightPos = *co->RightKnobPtr;
  else
    co->RightPos = co->RightSlider;

  switch ( co->Mode) {
  case pwr_eMisc_PingPongModeEnum_Idle: {
    if ( co->OldMode != co->Mode) {
      co->State = pwr_eMisc_PingPongStateEnum_Playing;
      time_GetTime( &co->StateShiftTime);
      co->LeftScore = 0;
      co->RightScore = 0;
      co->XCoordinate = co->Height/2;
      co->YCoordinate = -co->BallRadius;
      co->Speed = 15;
      co->Direction = -20;
    }
    if ( co->XCoordinate - co->BallRadius < 0) {
      co->XCoordinate = co->BallRadius - (co->XCoordinate - co->BallRadius);
      if ( co->Direction > 0)
	co->Direction = -co->Direction + 180 + my_random() * 10;
      else
	co->Direction = -co->Direction - 180 + my_random() * 10;
      if ( fabs(co->Direction) > MAX_ANGLE)
	co->Direction = my_random() * 10;
	
    }
    if ( co->XCoordinate + co->BallRadius > co->Width) {
      co->XCoordinate = co->Width - co->BallRadius - 
	(co->XCoordinate + co->BallRadius - co->Width);
      if ( co->Direction > 0)
	co->Direction = -co->Direction + 180 + my_random() * 10;
      else
	co->Direction = -co->Direction - 180 + my_random() * 10;
      if ( fabs(co->Direction) < 135)
	co->Direction = 180 - my_random() * 10;
    }
    if ( co->YCoordinate - co->BallRadius < 0) {
      co->YCoordinate = co->BallRadius - (co->YCoordinate - co->BallRadius); 
      co->Direction = -co->Direction;
      
    }
    if ( co->YCoordinate + co->BallRadius > co->Height) {
      co->YCoordinate = co->Height - co->BallRadius - 
	(co->YCoordinate + co->BallRadius - co->Height);
      co->Direction = -co->Direction;
    }
    break;
  }
  case pwr_eMisc_PingPongModeEnum_OnePlayer: {
    if ( co->OldMode != co->Mode) {
      co->State = pwr_eMisc_PingPongStateEnum_ServeLeft;
      time_GetTime( &co->StateShiftTime);
      co->LeftScore = 0;
      co->RightScore = 0;
      co->XCoordinate = co->BallRadius;
      co->YCoordinate = co->Height/2;
      co->Speed = 0;
    }
    switch ( co->State) {
    case pwr_eMisc_PingPongStateEnum_Playing: {
      pwr_tTime current;
      pwr_tDeltaTime diff;
      
      time_GetTime( &current);
      time_Adiff( &diff, &current, &co->StateShiftTime);
      co->LevelFactor = 1.0 + time_DToFloat( 0, &diff) * co->LevelIncrement / 20;

      if ( co->XCoordinate - co->BallRadius < 0) {
	co->XCoordinate = co->BallRadius - (co->XCoordinate - co->BallRadius);
	if ( co->Direction > 0)
	  co->Direction = -co->Direction + 180 + my_random() * 20;
	else
	  co->Direction = -co->Direction - 180 + my_random() * 20;
	if ( fabs(co->Direction) > MAX_ANGLE)
	  co->Direction = my_random() * 10;
	co->Speed = BASE_SPEED * co->LeftLevel * co->LevelFactor;
      }
      if ( co->XCoordinate + co->BallRadius > co->Width) {
	if ( co->YCoordinate >= co->RightPos - co->RacketWidth/2 &&
	     co->YCoordinate <= co->RightPos + co->RacketWidth/2) {
	  // Hit on racket
	  co->XCoordinate = co->Width - co->BallRadius - 
	    (co->XCoordinate + co->BallRadius - co->Width);
	  if ( co->Direction > 0)
	    co->Direction = -co->Direction + 180 + my_random() * 10;
	  else
	    co->Direction = -co->Direction - 180 + my_random() * 10;
	  if ( fabs(co->Direction) < 135)
	    co->Direction = my_random() * 10;
	  co->Speed = BASE_SPEED * co->RightLevel * co->LevelFactor;
	  co->BallCount++;
	}
	else {
	  // Failure
	  co->State = pwr_eMisc_PingPongStateEnum_ServeLeft;
	  time_GetTime( &co->StateShiftTime);
	}
      }
      if ( co->YCoordinate - co->BallRadius < 0) {
	co->YCoordinate = co->BallRadius - (co->YCoordinate - co->BallRadius); 
	co->Direction = -co->Direction;
      
      }
      if ( co->YCoordinate + co->BallRadius > co->Height) {
	co->YCoordinate = co->Height - co->BallRadius - 
	  (co->YCoordinate + co->BallRadius - co->Height);
	co->Direction = -co->Direction;
      }
      break;
    }
    case pwr_eMisc_PingPongStateEnum_ServeLeft: {
      pwr_tTime current;
      pwr_tDeltaTime diff;
      
      if ( co->XCoordinate + co->BallRadius > co->Width + 5) {
	co->XCoordinate = co->BallRadius;
	co->YCoordinate = co->Height/2;
	co->Speed = 0;
      }
      time_GetTime( &current);
      time_Adiff( &diff, &current, &co->StateShiftTime);
      if ( time_DToFloat( 0, &diff) > 2) {
	co->Speed = BASE_SPEED * co->LeftLevel;
	co->Direction = my_random() * 20;
	co->State = pwr_eMisc_PingPongStateEnum_Playing;
	co->LastBallCount = co->BallCount;
	co->BallCount = 0;
      }
      break;
    }
    }
    break;
  }
  case pwr_eMisc_PingPongModeEnum_TwoPlayers: {
    if ( co->OldMode != co->Mode || co->NewMatch) {
      co->NewMatch = 0;
      co->State = pwr_eMisc_PingPongStateEnum_ServeRight;
      time_GetTime( &co->StateShiftTime);
      co->LeftScore = 0;
      co->RightScore = 0;
      co->XCoordinate = co->Width - co->BallRadius;
      co->YCoordinate = co->Height/2;
      co->Speed = 0;
    }
    switch ( co->State) {
    case pwr_eMisc_PingPongStateEnum_Playing: {
      pwr_tTime current;
      pwr_tDeltaTime diff;
      
      time_GetTime( &current);
      time_Adiff( &diff, &current, &co->StateShiftTime);
      co->LevelFactor = 1.0 + time_DToFloat( 0, &diff) * co->LevelIncrement / 20;

      if ( co->XCoordinate - co->BallRadius < 0) {
	if ( co->YCoordinate >= co->LeftPos - co->RacketWidth/2 &&
	     co->YCoordinate <= co->LeftPos + co->RacketWidth/2) {
	  // Hit on racket
	  co->XCoordinate = co->BallRadius - (co->XCoordinate - co->BallRadius);
	  if ( co->Direction > 0)
	    co->Direction = -co->Direction + 180 + my_random() * 20;
	  else
	    co->Direction = -co->Direction - 180 + my_random() * 20;
	  if ( fabs(co->Direction) > MAX_ANGLE)
	    co->Direction = my_random() * 10;
	  co->Speed = BASE_SPEED * co->LeftLevel * co->LevelFactor;
	}
	else {
	  // Failure
	  co->RightScore++;
	  co->State = pwr_eMisc_PingPongStateEnum_ServeRight;
	  time_GetTime( &co->StateShiftTime);
	}	
      }
      if ( co->XCoordinate + co->BallRadius > co->Width) {
	if ( co->YCoordinate >= co->RightPos - co->RacketWidth/2 &&
	     co->YCoordinate <= co->RightPos + co->RacketWidth/2) {
	  // Hit on racket
	  co->XCoordinate = co->Width - co->BallRadius - 
	    (co->XCoordinate + co->BallRadius - co->Width);
	  if ( co->Direction > 0)
	    co->Direction = -co->Direction + 180 + my_random() * 10;
	  else
	    co->Direction = -co->Direction - 180 + my_random() * 10;
	  if ( fabs(co->Direction) < 135)
	    co->Direction = my_random() * 10;
	  co->Speed = BASE_SPEED * co->RightLevel * co->LevelFactor;
	}
	else {
	  // Failure
	  co->LeftScore++;
	  co->State = pwr_eMisc_PingPongStateEnum_ServeLeft;
	  time_GetTime( &co->StateShiftTime);
	}
      }
      if ( co->YCoordinate - co->BallRadius < 0) {
	co->YCoordinate = co->BallRadius - (co->YCoordinate - co->BallRadius); 
	co->Direction = -co->Direction;
      
      }
      if ( co->YCoordinate + co->BallRadius > co->Height) {
	co->YCoordinate = co->Height - co->BallRadius - 
	  (co->YCoordinate + co->BallRadius - co->Height);
	co->Direction = -co->Direction;
      }
      break;
    }
    case pwr_eMisc_PingPongStateEnum_ServeLeft: {
      pwr_tTime current;
      pwr_tDeltaTime diff;
      
      if ( co->XCoordinate + co->BallRadius > co->Width + 5) {
	co->XCoordinate = co->BallRadius;
	co->YCoordinate = co->Height/2;
	co->Speed = 0;
      }
      time_GetTime( &current);
      time_Adiff( &diff, &current, &co->StateShiftTime);
      if ( time_DToFloat( 0, &diff) > 2) {
	co->Speed = BASE_SPEED * co->LeftLevel;
	co->Direction = my_random() * 20;
	co->State = pwr_eMisc_PingPongStateEnum_Playing;
      }
      break;
    }
    case pwr_eMisc_PingPongStateEnum_ServeRight: {
      pwr_tTime current;
      pwr_tDeltaTime diff;
      
      if ( co->XCoordinate + co->BallRadius < - 5) {
	co->XCoordinate = co->Width - co->BallRadius;
	co->YCoordinate = co->Height/2;
	co->Speed = 0;
      }
      time_GetTime( &current);
      time_Adiff( &diff, &current, &co->StateShiftTime);
      if ( time_DToFloat( 0, &diff) > 2) {
	co->Speed = BASE_SPEED * co->LeftLevel;
	co->Direction = 180 + my_random() * 20;
	co->State = pwr_eMisc_PingPongStateEnum_Playing;
      }
      break;
    }
    }
    break;
  }
  }    
  float height;
  if ( fabs(co->Direction) < 90) {
    if ( co->XCoordinate < co->Width * 3/4)
      height = (1 - co->XCoordinate / co->Width * 4 / 3); 
    else
      height = ((co->XCoordinate - co->Width * 3/4) / co->Width * 4); 
  }
  else {
    if ( co->XCoordinate < co->Width / 4)
      height = (1 - co->XCoordinate / co->Width * 4); 
    else
      height = ((co->XCoordinate - co->Width / 4) / co->Width * 4/3); 
  }
  co->XShadow = co->XCoordinate + height * 4;
  co->YShadow = co->YCoordinate + height * 1;

  co->OldMode = co->Mode;
}
