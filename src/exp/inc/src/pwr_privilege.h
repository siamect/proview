/* 
 * Proview   $Id: pwr_privilege.h,v 1.6 2008-09-05 08:56:40 claes Exp $
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

#ifndef pwr_privilege_h
#define pwr_privilege_h

/* pwr_privilege.h -- pwr privileges
*/

/* Privilege mask */

typedef enum {
  pwr_mPrv_RtRead		= 1 << 0,
  pwr_mPrv_RtWrite		= 1 << 1,
  pwr_mPrv_System		= 1 << 2,
  pwr_mPrv_Maintenance		= 1 << 3,
  pwr_mPrv_Process		= 1 << 4,
  pwr_mPrv_Instrument		= 1 << 5,
  pwr_mPrv_Operator1		= 1 << 6,
  pwr_mPrv_Operator2		= 1 << 7,
  pwr_mPrv_Operator3		= 1 << 8,
  pwr_mPrv_Operator4		= 1 << 9,
  pwr_mPrv_Operator5		= 1 << 10,
  pwr_mPrv_Operator6		= 1 << 11,
  pwr_mPrv_Operator7		= 1 << 12,
  pwr_mPrv_Operator8		= 1 << 13,
  pwr_mPrv_Operator9		= 1 << 14,
  pwr_mPrv_Operator10		= 1 << 15,
  pwr_mPrv_DevRead		= 1 << 21,
  pwr_mPrv_DevPlc		= 1 << 22,
  pwr_mPrv_DevConfig		= 1 << 23,
  pwr_mPrv_DevClass		= 1 << 24,
  pwr_mPrv_RtEvents		= 1 << 25,
  pwr_mPrv_Administrator       	= 1 << 26,
  pwr_mPrv_SevRead     		= 1 << 27,
  pwr_mPrv_SevAdmin     	= 1 << 28
} pwr_mPrv;

typedef enum {
  pwr_mAccess_RtRead		= pwr_mPrv_RtRead,
  pwr_mAccess_RtWrite		= pwr_mPrv_RtWrite,
  pwr_mAccess_System		= pwr_mPrv_System,
  pwr_mAccess_Maintenance	= pwr_mPrv_Maintenance,
  pwr_mAccess_Process		= pwr_mPrv_Process,
  pwr_mAccess_Instrument       	= pwr_mPrv_Instrument,
  pwr_mAccess_Operator1		= pwr_mPrv_Operator1,
  pwr_mAccess_Operator2		= pwr_mPrv_Operator2,
  pwr_mAccess_Operator3		= pwr_mPrv_Operator3,
  pwr_mAccess_Operator4		= pwr_mPrv_Operator4,
  pwr_mAccess_Operator5		= pwr_mPrv_Operator5,
  pwr_mAccess_Operator6		= pwr_mPrv_Operator6,
  pwr_mAccess_Operator7		= pwr_mPrv_Operator7,
  pwr_mAccess_Operator8		= pwr_mPrv_Operator8,
  pwr_mAccess_Operator9		= pwr_mPrv_Operator9,
  pwr_mAccess_Operator10	= pwr_mPrv_Operator10,
  pwr_mAccess_RtEvents		= pwr_mPrv_RtEvents,
  pwr_mAccess_AllRt		= pwr_mPrv_System |
				  pwr_mPrv_Maintenance |
				  pwr_mPrv_Process |
				  pwr_mPrv_Instrument |
				  pwr_mPrv_RtRead |
				  pwr_mPrv_RtWrite |
				  pwr_mPrv_RtEvents |
				  pwr_mPrv_Operator1 |
				  pwr_mPrv_Operator2 |
				  pwr_mPrv_Operator3 |
				  pwr_mPrv_Operator4 |
				  pwr_mPrv_Operator5 |
				  pwr_mPrv_Operator6 |
				  pwr_mPrv_Operator7 |
				  pwr_mPrv_Operator8 |
				  pwr_mPrv_Operator9 |
				  pwr_mPrv_Operator10,
  pwr_mAccess_AllOperators	= pwr_mPrv_Operator1 |
				  pwr_mPrv_Operator2 |
				  pwr_mPrv_Operator3 |
				  pwr_mPrv_Operator4 |
				  pwr_mPrv_Operator5 |
				  pwr_mPrv_Operator6 |
				  pwr_mPrv_Operator7 |
				  pwr_mPrv_Operator8 |
				  pwr_mPrv_Operator9 |
				  pwr_mPrv_Operator10,
  pwr_mAccess_AllSev		= pwr_mPrv_SevRead |
  				  pwr_mPrv_SevAdmin,
  pwr_mAccess_AllPwr		= ~(1 << 31),
  pwr_mAccess_Default     	= 1 << 31
} pwr_mAccess;

#endif





