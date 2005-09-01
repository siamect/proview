/* 
 * Proview   $Id: cnv_wblto.h,v 1.2 2005-09-01 14:57:47 claes Exp $
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

#ifndef cnv_wblto_h
#define cnv_wblto_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_readwbl.h"

using namespace std;

class CnvReadWbl;

typedef enum {
  Cnv_eWblToType_Html,
  Cnv_eWblToType_H,
  Cnv_eWblToType_Xtthelp,
  Cnv_eWblToType_Ps
} Cnv_eWblToType;
  
class CnvWblTo {
 public:
  // CnvWblTo() {}
  virtual ~CnvWblTo() {}

  virtual int init( char *);
  virtual int close() { return 1;}
  virtual int class_exec() { return 1;}
  virtual int class_close() { return 1;}
  virtual int body_exec() { return 1;}
  virtual int body_close() { return 1;}
  virtual int attribute_exec() { return 1;}
  virtual int typedef_exec() { return 1;}
  virtual int typedef_close() { return 1;}
  virtual int bit_exec() { return 1;}
  virtual int graphplcnode() { return 1;}
  virtual int graphplccon() { return 1;}
  virtual int template_exec() { return 1;}
  virtual Cnv_eWblToType type() { return Cnv_eWblToType_H;} 
  virtual int class_open() { return 0;}
  virtual int index_open() { return 0;}
  virtual bool confpass() { return false;}
  virtual void set_confpass( bool conf) {};
};

#if defined __cplusplus
}
#endif
#endif

