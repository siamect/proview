/*
 * Proview   Open Source Process Control.
 * Copyright (C) 2005-2017 SSAB EMEA AB.
 *
 * This file is part of Proview.
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
 * along with Proview. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking Proview statically or dynamically with other modules is
 * making a combined work based on Proview. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * Proview give you permission to, from the build function in the
 * Proview Configurator, combine Proview with modules generated by the
 * Proview PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every
 * copy of the combined work is accompanied by a complete copy of
 * the source code of Proview (the version used to produce the
 * combined work), being distributed under the terms of the GNU
 * General Public License plus this exception.
 **/

/* wb_c_pb_dp_slave_qt.c -- work bench methods of the Pb_DP_Slave class. */

#include "co_dcli.h"

#include "cow_xhelp_qt.h"

#include "wb_pwrs.h"
#include "wb_wsx.h"

#include "rt_pb_msg.h"

#include "cow_pn_gsdml_attr_qt.h"

#include "wb_c_pndevice.h"

char* id_to_string(pwr_tOid oid)
{
  unsigned char vid[4];
  static char str[40];

  memcpy(&vid, &oid.vid, sizeof(vid));
  sprintf(str, "%3.3u_%3.3u_%3.3u_%3.3u_%8.8x", vid[3], vid[2], vid[1], vid[0],
      oid.oix);
  return str;
}

static pwr_tStatus Configure(ldh_sMenuCall* ip)
{
  pwr_tStatus sts;
  device_sCtx* ctx;
  pwr_tFileName datafile;

  sprintf(
      datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string(ip->Pointed.Objid));

  sts = pndevice_create_ctx(ip->PointedSession, ip->Pointed, ip->wnav, &ctx);
  if (EVEN(sts)) {
    return sts;
  }

  ctx->attr = new GsdmlAttrQt(CoXHelpQt::get_widget(), ctx, 0, ctx->gsdml,
      ctx->edit_mode, datafile, &sts);
  if (sts == PB__CONFIGABORTED) {
    ((GsdmlAttrQt*)ctx->attr)->toplevel->close();
    return 1;
  }
  ctx->attr->close_cb = pndevice_close_cb;
  ctx->attr->save_cb = pndevice_save_cb;
  ctx->attr->help_cb = pndevice_help_cb;

  if (ODD(sts)) {
    sts = pndevice_init(ctx);
  }
  if (EVEN(sts)) {
    ctx->attr->wow->DisplayError("Configuration load error",
        "Configuration load error\nCheck configuration data");
  }

  return 1;
}

static pwr_tStatus ConfigureFilter(ldh_sMenuCall* ip)
{
  return 1;
}

//
// Copy Device.
//
static pwr_tStatus CopyDevice(ldh_sMenuCall* ip)
{
  char* gsdml;
  int size;
  int sts;
  pwr_tOid oid;
  pwr_tAttrRef aref[2];
  pwr_tFileName datafile_src, datafile_dest;
  pwr_tFileName found_file;
  pwr_tCmd cmd;

  // Check if data file exist
  sprintf(datafile_src, "$pwrp_load/pwr_pn_%s.xml",
      id_to_string(ip->Pointed.Objid));
  dcli_translate_filename(datafile_src, datafile_src);

  sts = dcli_search_file(datafile_src, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file(datafile_src, found_file, DCLI_DIR_SEARCH_END);
  if (EVEN(sts)) {
    return sts;
  }

  sts = ldh_GetObjectPar(ip->PointedSession, ip->Pointed.Objid, "RtBody",
      "GSDMLfile", &gsdml, &size);
  if (EVEN(sts)) {
    return sts;
  }
  if (strcmp(gsdml, "") == 0) {
    free(gsdml);
    return 0;
  }
  free(gsdml);

  memset(aref, 0, sizeof(aref));
  aref[0] = ip->Pointed;
  sts = ldh_CopyObjectTrees(
      ip->PointedSession, aref, ip->Pointed.Objid, ldh_eDest_After, 0, 1, 0, 0);
  if (EVEN(sts)) {
    return sts;
  }

  sts = ldh_GetNextSibling(ip->PointedSession, ip->Pointed.Objid, &oid);
  if (EVEN(sts)) {
    return sts;
  }

  sprintf(datafile_dest, "$pwrp_load/pwr_pn_%s.xml", id_to_string(oid));
  dcli_translate_filename(datafile_dest, datafile_dest);

  snprintf(cmd, sizeof(cmd), "cp %s %s", datafile_src, datafile_dest);
  system(cmd);

  return 1;
}

static pwr_tStatus CopyDeviceFilter(ldh_sMenuCall* ip)
{
  char* gsdml;
  int size;
  int sts;
  pwr_tFileName datafile;
  pwr_tFileName found_file;

  // Check if data file exist
  sprintf(
      datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string(ip->Pointed.Objid));
  dcli_translate_filename(datafile, datafile);

  sts = dcli_search_file(datafile, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file(datafile, found_file, DCLI_DIR_SEARCH_END);
  if (EVEN(sts)) {
    return 0;
  }

  sts = ldh_GetObjectPar(ip->PointedSession, ip->Pointed.Objid, "RtBody",
      "GSDMLfile", &gsdml, &size);
  if (EVEN(sts)) {
    return sts;
  }
  if (strcmp(gsdml, "") == 0) {
    free(gsdml);
    return 0;
  }
  free(gsdml);
  return 1;
}

//
//  Syntax check.
//
static pwr_tStatus SyntaxCheck(
    ldh_tSesContext Session, pwr_tAttrRef Object, /* current object */
    int* ErrorCount, /* accumulated error count */
    int* WarningCount /* accumulated waring count */
    )
{
  return wsx_CheckIoDevice(
      Session, Object, ErrorCount, WarningCount, wsx_mCardOption_None);
}

//
//  Get value.
//
static pwr_tStatus GetIoDeviceData(
    pwr_tAttrRef Object, const char* Attr, char* Buf, int BufSize)
{
  pwr_tFileName datafile;
  pwr_tStatus sts;

  sprintf(datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string(Object.Objid));
  dcli_translate_filename(datafile, datafile);

  GsdmlDeviceData* data = new GsdmlDeviceData();
  sts = data->read(datafile);
  if (EVEN(sts)) {
    return sts;
  }

  sts = data->get_value(Attr, Buf, BufSize);

  delete data;

  return sts;
}

static pwr_tStatus SetIoDeviceData(
    pwr_tAttrRef Object, const char* Attr, const char* Value)
{
  pwr_tFileName datafile;
  pwr_tStatus sts;

  sprintf(datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string(Object.Objid));
  dcli_translate_filename(datafile, datafile);

  GsdmlDeviceData* data = new GsdmlDeviceData();
  sts = data->read(datafile);
  if (EVEN(sts)) {
    return sts;
  }

  sts = data->modify_value(Attr, Value);
  if (ODD(sts)) {
    data->print(datafile);
  }

  delete data;

  return sts;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/
pwr_dExport pwr_BindMethods(PnDevice)
    = { pwr_BindMethod(Configure), pwr_BindMethod(ConfigureFilter),
        pwr_BindMethod(CopyDevice), pwr_BindMethod(CopyDeviceFilter),
        pwr_BindMethod(SyntaxCheck), pwr_BindMethod(GetIoDeviceData),
        pwr_BindMethod(SetIoDeviceData), pwr_NullMethod };