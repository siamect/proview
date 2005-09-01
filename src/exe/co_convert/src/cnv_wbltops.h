/* 
 * Proview   $Id: cnv_wbltops.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_wbltops_h
#define cnv_wbltops_h

#include "pwr.h"
#include "cnv_readwbl.h"
#include "cnv_wblto.h"
#include "cnv_tops.h"

using namespace std;

class CnvReadWbl;

class CnvWblToPs : public CnvWblTo {
 public:
  CnvWblToPs( CnvCtx *cnv_ctx) : 
    ctx(cnv_ctx), conf_pass(false) {}
  virtual ~CnvWblToPs();

  CnvCtx  	*ctx;
  CnvToPs	tops;
  bool		conf_pass;

  int init( char *first);
  int close();
  int class_exec();
  int class_close();
  int body_exec();
  int attribute_exec();
  int typedef_exec();
  int bit_exec();
  Cnv_eWblToType type() { return Cnv_eWblToType_Ps;}
  int index_open() { return 0;}
  void set_confpass( bool conf) {
    tops.set_confpass( conf);
    conf_pass = conf;
  }
  bool confpass() { return true;}
};

#endif

