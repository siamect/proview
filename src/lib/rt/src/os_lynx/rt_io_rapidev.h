/* 
 * Proview   $Id: rt_io_rapidev.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_rapidev.h -- Declaration of RAPI devices */

/*
modification history
--------------------
950108,LT  	Ported to vxWorks from INTERAY demo source code.
*/

#ifndef rt_io_rapidev_h
#define rt_io_rapidev_h

#ifdef __cplusplus
extern	"C"	{
#endif

/*
* RAPI class codes 
*/
#define RAPI_BIT           1
#define RAPI_PORT          2
#define RAPI_ADC           3
#define RAPI_DAC           4
#define RAPI_SERIAL        5
#define RAPI_CAN           6
#define RAPI_COUNT         7
#define RAPI_LVAR          8
#define RAPI_GVAR          9
#define RAPI_MUXMOD        10
#define RAPI_IPMUX         10
#define RAPI_MOT           11
#define RAPI_BBUS          12
#define RAPI_MOOG          13
#define RAPI_SUI           14
#define RAPI_PROFI         15

/*
* RAPI type codes 
*/
#define BIT_IN             1
#define BIT_OUT            2
#define BIT_COS            3
#define BIT_COUNT          4
#define BIT_PULSE          5
#define BIT_PWM            6
#define BIT_DAC            7
#define BIT_EVCAP          8
#define BIT_EVCOMP         9

#define PORT8_IN           1
#define PORT16_IN          2
#define PORT32_IN          3
#define PORT8_OUT          4
#define PORT16_OUT         5
#define PORT32_OUT         6

#define LVAR8_IN           1
#define LVAR16_IN          2
#define LVAR32_IN          3
#define LVAR8_OUT          4
#define LVAR16_OUT         5
#define LVAR32_OUT         6

#define GVAR8_IN           1
#define GVAR16_IN          2
#define GVAR32_IN          3
#define GVAR8_OUT          4
#define GVAR16_OUT         5
#define GVAR32_OUT         6

#define CAN_REGS           1
#define CAN_FLTR           2 
#define CAN_TBUF           3
#define CAN_RBUF           4 

#define SER_TRM            1
#define SER_REC            2
#define SER_RAWREC         3

#define CNT_TIM16          1
#define CNT_TIM32          2
#define CNT_CNT16          3
#define CNT_CNT32          4

#define STEP_LOOP          1
#define STEP_MODE          2
#define STEP_ACC           3
#define STEP_SPD           4
#define STEP_SETPOS        5
#define STEP_CURPOS        6

#define ADC8               1
#define ADC10              2
#define ADC12              3
#define ADC14              4
#define ADC16              5
#define ADCRAW             9

#define DAC8               1
#define DAC10              2
#define DAC12              3
#define DAC14              4
#define DAC16              5

#define IPMUX_INP8        1
#define IPMUX_INP16       2
#define IPMUX_OUTP8       3
#define IPMUX_OUTP16      4
#define IPMUX_REG8        5
#define IPMUX_REG16       6
#define IPMUX_REG32       7
#define IPMUX_SETP8       8
#define IPMUX_SETP16      9
#define IPMUX_SETP32      10
#define IPMUX_BOARD       11

#define MOT_GCTRL  		  1
#define MOT_GSTAT  		  2
#define MOT_MCTRL  		  3
#define MOT_MSTAT  		  4
#define MOT_ACC    		  5
#define MOT_SPEED  		  6
#define MOT_PARM   		  7
#define MOT_SPOS   		  8
#define MOT_APOS   		  9
#define MOT_IREG   		 10
#define MOT_OREG   		 11

#define BBUS_CMD          1
#define BBUS_STAT         2
#define BBUS_NODE         3
#define BBUS_MODE         4

#define MOOG_RMC          1

#define SUI_COMD          1
#define SUI_STAT          2
#define SUI_MSGBUF        3
#define SUI_WINDOW        4
#define SUI_KEY           5
#define SUI_SERIAL        6
#define SUI_IIC           7

#ifdef __cplusplus
}
#endif

#endif
