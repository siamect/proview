/* 
 * Proview   $Id: ssabox_ssabutil.h,v 1.1 2006-01-12 05:54:27 claes Exp $
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

/************************************************************************
*
* Filename:		rs_ssabutil.h
*
*			Datum	Pgm.		Anm
* Reviderad		920724	CS		Skapad.
*
* Beskrivning:
*
**************************************************************************/

int	ssabutil_lopnr_check( pwr_tInt32	lopnr);

pwr_tInt32 ssabutil_chksum_lop(	pwr_tInt32 lopnummer);

pwr_tInt32 ssabutil_chksum_kupong(	pwr_tInt32 kupong_nummer);

pwr_tInt32 ssabutil_chksum_calculate(	pwr_tInt32 value,
					pwr_tInt16 *weights,
					pwr_tInt16 num_figures);

int ssab_curve( 	float x, 
			float *y, 
			float *table,
			int size);

int ssab_curve_2d( 	float x, 
			float y, 
			float *z, 
			float *table,
			int x_size, 
			int y_size);

#define SSAB_CURVE_2D( x, y, z, table) ssab_curve_2d( x, y, z, (float *)&table,\
		sizeof(table[0])/sizeof(table[0][0])-1,\
		sizeof(table)/sizeof(table[0])-1)

#define SSAB_CURVE( x, y, table) ssab_curve( x, y, (float *)&table,\
		sizeof(table)/sizeof(table[0]))







