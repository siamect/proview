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

/* rt_io_m_hilscher_cifx_module.c -- I/O methods for class Hilscher_cifX_Module. */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "co_cdh.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_msg.h"

#if defined PWRE_CONF_CIFX

static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  return IO__SUCCESS;
}

#else
static pwr_tStatus IoCardInit( io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp) { return IO__RELEASEBUILD;}
static pwr_tStatus IoCardClose( io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp) { return IO__RELEASEBUILD;}
#endif

/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(Hilscher_cifX_Module) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_NullMethod
};

