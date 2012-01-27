/* packet-qcom.c
 * Routines for Interlink protocol packet disassembly
 * By Claes Sjöfors <claes.sjofors@proview.se>
 * Copyright 2010 Claes Sjöfors
 *
 * $Id$
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <glib.h>
#include <epan/packet.h>
#include <string.h>
#include "packet-qcom.h"
#include "pwr_def.h"

/*
 * No public information available.
 */
#define QCOM_PORT 50
#define PROTO_TAG_QCOM    "QCOM"

static int proto_qcom = -1;

static dissector_handle_t data_handle = 0;
static dissector_handle_t qcom_handle;

static const value_string packettypenames[] = {
  { 0, "TEXT"},
  { 1, "SOMETHING_ELSE" },
  { 0, 0}};

static gint hf_qcom = -1;
static gint hf_qcom_header = -1;
static gint hf_qcom_header_nodeid = -1;
static gint hf_qcom_header_birth = -1;
static gint hf_qcom_header_flags = -1;
static gint hf_qcom_header_lacksequence = -1;
static gint hf_qcom_header_lacktimestamp = -1;
static gint hf_qcom_header_racksequence = -1;
static gint hf_qcom_header_racktimestamp = -1;
static gint hf_qcom_info = -1;
static gint hf_qcom_info_senderaix = -1;
static gint hf_qcom_info_sendernid = -1;
static gint hf_qcom_info_pid = -1;
static gint hf_qcom_info_receiverqix = -1;
static gint hf_qcom_info_receivernid = -1;
static gint hf_qcom_info_replyqix = -1;
static gint hf_qcom_info_replynid = -1;
static gint hf_qcom_info_btype = -1;
static gint hf_qcom_info_stype = -1;
static gint hf_qcom_info_rid = -1;
static gint hf_qcom_info_size = -1;
static gint hf_qcom_info_tmo = -1;
static gint hf_qcom_info_status = -1;
static gint hf_qcom_info_flags = -1;
static gint hf_mh_header = -1;
static gint hf_mh_header_xdr = -1;
static gint hf_mh_header_platform = -1;
static gint hf_mh_header_version = -1;
static gint hf_mh_header_source = -1;
static gint hf_mh_header_birthtime = -1;
static gint hf_mh_header_type = -1;
static gint hf_mh_header_qidqix = -1;
static gint hf_mh_header_qidnid = -1;
static gint hf_mh_header_nix = -1;
static gint hf_mh_header_outunitoix = -1;
static gint hf_mh_header_outunitvid = -1;
static gint hf_mh_header_aidoix = -1;
static gint hf_mh_header_aidvid = -1;
static gint hf_mh_msginfo = -1;
static gint hf_mh_msginfo_idnix = -1;
static gint hf_mh_msginfo_idbirthtime = -1;
static gint hf_mh_msginfo_ididx = -1;
static gint hf_mh_msginfo_outunitoix = -1;
static gint hf_mh_msginfo_outunitvid = -1;
static gint hf_mh_msginfo_eventflags = -1;
static gint hf_mh_msginfo_eventtime = -1;
static gint hf_mh_msginfo_eventnamev3 = -1;
static gint hf_mh_msginfo_eventtype = -1;
static gint hf_mh_msginfo_eventprio = -1;
static gint hf_mh_message = -1;
static gint hf_mh_message_eventtext = -1;
static gint hf_mh_message_status = -1;
static gint hf_mh_message_eventsoundoix = -1;
static gint hf_mh_message_eventsoundvid = -1;
static gint hf_mh_message_eventsoundbody = -1;
static gint hf_mh_message_eventsoundoffset = -1;
static gint hf_mh_message_eventsoundsize = -1;
static gint hf_mh_message_eventsoundflags = -1;
static gint hf_mh_message_eventmoretext = -1;
static gint hf_mh_message_objectoix = -1;
static gint hf_mh_message_objectvid = -1;
static gint hf_mh_message_objectbody = -1;
static gint hf_mh_message_objectoffset = -1;
static gint hf_mh_message_objectsize = -1;
static gint hf_mh_message_objectflags = -1;
static gint hf_mh_message_supobjectoix = -1;
static gint hf_mh_message_supobjectvid = -1;
static gint hf_mh_message_supobjectbody = -1;
static gint hf_mh_message_supobjectoffset = -1;
static gint hf_mh_message_supobjectsize = -1;
static gint hf_mh_message_supobjectflags = -1;
static gint hf_mh_message_eventname = -1;
static gint hf_mh_message_receiver = -1;
static gint hf_mh_block = -1;
static gint hf_mh_block_targetidnix = -1;
static gint hf_mh_block_targetidbirthtime = -1;
static gint hf_mh_block_targetididx = -1;
static gint hf_mh_block_detecttime = -1;
static gint hf_mh_block_outunitoix = -1;
static gint hf_mh_block_outunitvid = -1;
static gint hf_mh_block_status = -1;
static gint hf_mh_block_objectoix = -1;
static gint hf_mh_block_objectvid = -1;
static gint hf_mh_block_objectbody = -1;
static gint hf_mh_block_objectoffset = -1;
static gint hf_mh_block_objectsize = -1;
static gint hf_mh_block_objectflags = -1;
static gint hf_mh_block_supobjectoix = -1;
static gint hf_mh_block_supobjectvid = -1;
static gint hf_mh_block_supobjectbody = -1;
static gint hf_mh_block_supobjectoffset = -1;
static gint hf_mh_block_supobjectsize = -1;
static gint hf_mh_block_supobjectflags = -1;
static gint hf_mh_block_eventname = -1;
static gint hf_mh_ack = -1;
static gint hf_mh_ack_targetidnix = -1;
static gint hf_mh_ack_targetidbirthtime = -1;
static gint hf_mh_ack_targetididx = -1;
static gint hf_mh_ack_detecttime = -1;
static gint hf_mh_ack_outunitoix = -1;
static gint hf_mh_ack_outunitvid = -1;
static gint hf_mh_ack_objectoix = -1;
static gint hf_mh_ack_objectvid = -1;
static gint hf_mh_ack_objectbody = -1;
static gint hf_mh_ack_objectoffset = -1;
static gint hf_mh_ack_objectsize = -1;
static gint hf_mh_ack_objectflags = -1;
static gint hf_mh_ack_supobjectoix = -1;
static gint hf_mh_ack_supobjectvid = -1;
static gint hf_mh_ack_supobjectbody = -1;
static gint hf_mh_ack_supobjectoffset = -1;
static gint hf_mh_ack_supobjectsize = -1;
static gint hf_mh_ack_supobjectflags = -1;
static gint hf_mh_ack_eventname = -1;
static gint hf_mh_return = -1;
static gint hf_mh_return_eventtext = -1;
static gint hf_mh_return_targetidnix = -1;
static gint hf_mh_return_targetidbirthtime = -1;
static gint hf_mh_return_targetididx = -1;
static gint hf_mh_return_detecttime = -1;
static gint hf_mh_return_objectoix = -1;
static gint hf_mh_return_objectvid = -1;
static gint hf_mh_return_objectbody = -1;
static gint hf_mh_return_objectoffset = -1;
static gint hf_mh_return_objectsize = -1;
static gint hf_mh_return_objectflags = -1;
static gint hf_mh_return_supobjectoix = -1;
static gint hf_mh_return_supobjectvid = -1;
static gint hf_mh_return_supobjectbody = -1;
static gint hf_mh_return_supobjectoffset = -1;
static gint hf_mh_return_supobjectsize = -1;
static gint hf_mh_return_supobjectflags = -1;
static gint hf_mh_return_eventname = -1;
static gint hf_net_message = -1;
static gint hf_net_message_xdr = -1;
static gint hf_net_message_msn = -1;
static gint hf_net_message_nid = -1;
static gint hf_net_oidtoobject = -1;
static gint hf_net_oidtoobject_oidoix = -1;
static gint hf_net_oidtoobject_oidvid = -1;
static gint hf_net_oidtoobject_trans = -1;
static gint hf_net_oidtoobject_lcount = -1;
static gint hf_net_oidtoobject_rcount = -1;
static gint hf_net_objectr = -1;
static gint hf_net_objectr_status = -1;
static gint hf_net_objectr_oidoix = -1;
static gint hf_net_objectr_oidvid = -1;
static gint hf_net_objectr_count = -1;
static gint hf_net_gobject_oidoix = -1;
static gint hf_net_gobject_oidvid = -1;
static gint hf_net_gobject_cid = -1;
static gint hf_net_gobject_familynameorig = -1;
static gint hf_net_gobject_familynamenorm = -1;
static gint hf_net_gobject_familynamekey = -1;
static gint hf_net_gobject_familypoidoix = -1;
static gint hf_net_gobject_familypoidvid = -1;
static gint hf_net_gobject_sibflink = -1;
static gint hf_net_gobject_sibblink = -1;
static gint hf_net_gobject_soidoix = -1;
static gint hf_net_gobject_soidvid = -1;
static gint hf_net_gobject_size = -1;
static gint hf_net_gobject_flags = -1;
static gint hf_net_getobjectinfo = -1;
static gint hf_net_getobjectinfo_arefoix = -1;
static gint hf_net_getobjectinfo_arefvid = -1;
static gint hf_net_getobjectinfo_arefbody = -1;
static gint hf_net_getobjectinfo_arefoffset = -1;
static gint hf_net_getobjectinfo_arefsize = -1;
static gint hf_net_getobjectinfo_arefflags = -1;
static gint hf_net_getobjectinfor = -1;
static gint hf_net_getobjectinfor_sts = -1;
static gint hf_net_getobjectinfor_arefoix = -1;
static gint hf_net_getobjectinfor_arefvid = -1;
static gint hf_net_getobjectinfor_arefbody = -1;
static gint hf_net_getobjectinfor_arefoffset = -1;
static gint hf_net_getobjectinfor_arefsize = -1;
static gint hf_net_getobjectinfor_arefflags = -1;
static gint hf_net_getobjectinfor_size = -1;

/* These are the ids of the subtrees that we may be creating */
static gint ett_qcom = -1;
static gint ett_qcom_header = -1;
static gint ett_qcom_header_nodeid = -1;
static gint ett_qcom_header_birth = -1;
static gint ett_qcom_header_flags = -1;
static gint ett_qcom_header_lacksequence = -1;
static gint ett_qcom_header_lacktimestamp = -1;
static gint ett_qcom_header_racksequence = -1;
static gint ett_qcom_header_racktimestamp = -1;
static gint ett_qcom_info = -1;
static gint ett_qcom_info_senderaix = -1;
static gint ett_qcom_info_sendernid = -1;
static gint ett_qcom_info_pid = -1;
static gint ett_qcom_info_receiverqix = -1;
static gint ett_qcom_info_receivernid = -1;
static gint ett_qcom_info_replyqix = -1;
static gint ett_qcom_info_replynid = -1;
static gint ett_qcom_info_btype = -1;
static gint ett_qcom_info_stype = -1;
static gint ett_qcom_info_rid = -1;
static gint ett_qcom_info_size = -1;
static gint ett_qcom_info_tmo = -1;
static gint ett_qcom_info_status = -1;
static gint ett_qcom_info_flags = -1;
static gint ett_mh_header = -1;
static gint ett_mh_header_xdr = -1;
static gint ett_mh_header_platform = -1;
static gint ett_mh_header_version = -1;
static gint ett_mh_header_source = -1;
static gint ett_mh_header_birthtime = -1;
static gint ett_mh_header_type = -1;
static gint ett_mh_header_qidqix = -1;
static gint ett_mh_header_qidnid = -1;
static gint ett_mh_header_nix = -1;
static gint ett_mh_header_outunitoix = -1;
static gint ett_mh_header_outunitvid = -1;
static gint ett_mh_header_aidoix = -1;
static gint ett_mh_header_aidvid = -1;
static gint ett_mh_msginfo = -1;
static gint ett_mh_msginfo_idnix = -1;
static gint ett_mh_msginfo_idbirthtime = -1;
static gint ett_mh_msginfo_ididx = -1;
static gint ett_mh_msginfo_outunitoix = -1;
static gint ett_mh_msginfo_outunitvid = -1;
static gint ett_mh_msginfo_eventflags = -1;
static gint ett_mh_msginfo_eventtime = -1;
static gint ett_mh_msginfo_eventnamev3 = -1;
static gint ett_mh_msginfo_eventtype = -1;
static gint ett_mh_msginfo_eventprio = -1;
static gint ett_mh_message = -1;
static gint ett_mh_message_eventtext = -1;
static gint ett_mh_message_status = -1;
static gint ett_mh_message_eventsoundoix = -1;
static gint ett_mh_message_eventsoundvid = -1;
static gint ett_mh_message_eventsoundbody = -1;
static gint ett_mh_message_eventsoundoffset = -1;
static gint ett_mh_message_eventsoundsize = -1;
static gint ett_mh_message_eventsoundflags = -1;
static gint ett_mh_message_eventmoretext = -1;
static gint ett_mh_message_objectoix = -1;
static gint ett_mh_message_objectvid = -1;
static gint ett_mh_message_objectbody = -1;
static gint ett_mh_message_objectoffset = -1;
static gint ett_mh_message_objectsize = -1;
static gint ett_mh_message_objectflags = -1;
static gint ett_mh_message_supobjectoix = -1;
static gint ett_mh_message_supobjectvid = -1;
static gint ett_mh_message_supobjectbody = -1;
static gint ett_mh_message_supobjectoffset = -1;
static gint ett_mh_message_supobjectsize = -1;
static gint ett_mh_message_supobjectflags = -1;
static gint ett_mh_message_eventname = -1;
static gint ett_mh_message_receiver = -1;
static gint ett_mh_block = -1;
static gint ett_mh_block_targetidnix = -1;
static gint ett_mh_block_targetidbirthtime = -1;
static gint ett_mh_block_targetididx = -1;
static gint ett_mh_block_detecttime = -1;
static gint ett_mh_block_outunitoix = -1;
static gint ett_mh_block_outunitvid = -1;
static gint ett_mh_block_status = -1;
static gint ett_mh_block_objectoix = -1;
static gint ett_mh_block_objectvid = -1;
static gint ett_mh_block_objectbody = -1;
static gint ett_mh_block_objectoffset = -1;
static gint ett_mh_block_objectsize = -1;
static gint ett_mh_block_objectflags = -1;
static gint ett_mh_block_supobjectoix = -1;
static gint ett_mh_block_supobjectvid = -1;
static gint ett_mh_block_supobjectbody = -1;
static gint ett_mh_block_supobjectoffset = -1;
static gint ett_mh_block_supobjectsize = -1;
static gint ett_mh_block_supobjectflags = -1;
static gint ett_mh_block_eventname = -1;
static gint ett_mh_ack = -1;
static gint ett_mh_ack_targetidnix = -1;
static gint ett_mh_ack_targetidbirthtime = -1;
static gint ett_mh_ack_targetididx = -1;
static gint ett_mh_ack_detecttime = -1;
static gint ett_mh_ack_outunitoix = -1;
static gint ett_mh_ack_outunitvid = -1;
static gint ett_mh_ack_objectoix = -1;
static gint ett_mh_ack_objectvid = -1;
static gint ett_mh_ack_objectbody = -1;
static gint ett_mh_ack_objectoffset = -1;
static gint ett_mh_ack_objectsize = -1;
static gint ett_mh_ack_objectflags = -1;
static gint ett_mh_ack_supobjectoix = -1;
static gint ett_mh_ack_supobjectvid = -1;
static gint ett_mh_ack_supobjectbody = -1;
static gint ett_mh_ack_supobjectoffset = -1;
static gint ett_mh_ack_supobjectsize = -1;
static gint ett_mh_ack_supobjectflags = -1;
static gint ett_mh_ack_eventname = -1;
static gint ett_mh_return = -1;
static gint ett_mh_return_eventtext = -1;
static gint ett_mh_return_targetidnix = -1;
static gint ett_mh_return_targetidbirthtime = -1;
static gint ett_mh_return_targetididx = -1;
static gint ett_mh_return_detecttime = -1;
static gint ett_mh_return_objectoix = -1;
static gint ett_mh_return_objectvid = -1;
static gint ett_mh_return_objectbody = -1;
static gint ett_mh_return_objectoffset = -1;
static gint ett_mh_return_objectsize = -1;
static gint ett_mh_return_objectflags = -1;
static gint ett_mh_return_supobjectoix = -1;
static gint ett_mh_return_supobjectvid = -1;
static gint ett_mh_return_supobjectbody = -1;
static gint ett_mh_return_supobjectoffset = -1;
static gint ett_mh_return_supobjectsize = -1;
static gint ett_mh_return_supobjectflags = -1;
static gint ett_mh_return_eventname = -1;
static gint ett_net_message = -1;
static gint ett_net_message_xdr = -1;
static gint ett_net_message_msn = -1;
static gint ett_net_message_nid = -1;
static gint ett_net_oidtoobject = -1;
static gint ett_net_oidtoobject_oidoix = -1;
static gint ett_net_oidtoobject_oidvid = -1;
static gint ett_net_oidtoobject_trans = -1;
static gint ett_net_oidtoobject_lcount = -1;
static gint ett_net_oidtoobject_rcount = -1;
static gint ett_net_objectr = -1;
static gint ett_net_objectr_status = -1;
static gint ett_net_objectr_oidoix = -1;
static gint ett_net_objectr_oidvid = -1;
static gint ett_net_objectr_count = -1;
static gint ett_net_gobject = -1;
static gint ett_net_gobject_oidoix = -1;
static gint ett_net_gobject_oidvid = -1;
static gint ett_net_gobject_cid = -1;
static gint ett_net_gobject_familynameorig = -1;
static gint ett_net_gobject_familynamenorm = -1;
static gint ett_net_gobject_familynamekey = -1;
static gint ett_net_gobject_familypoidoix = -1;
static gint ett_net_gobject_familypoidvid = -1;
static gint ett_net_gobject_sibflink = -1;
static gint ett_net_gobject_sibblink = -1;
static gint ett_net_gobject_soidoix = -1;
static gint ett_net_gobject_soidvid = -1;
static gint ett_net_gobject_size = -1;
static gint ett_net_gobject_flags = -1;
static gint ett_net_getobjectinfo = -1;
static gint ett_net_getobjectinfo_arefoix = -1;
static gint ett_net_getobjectinfo_arefvid = -1;
static gint ett_net_getobjectinfo_arefbody = -1;
static gint ett_net_getobjectinfo_arefoffset = -1;
static gint ett_net_getobjectinfo_arefsize = -1;
static gint ett_net_getobjectinfo_arefflags = -1;
static gint ett_net_getobjectinfor = -1;
static gint ett_net_getobjectinfor_sts = -1;
static gint ett_net_getobjectinfor_arefoix = -1;
static gint ett_net_getobjectinfor_arefvid = -1;
static gint ett_net_getobjectinfor_arefbody = -1;
static gint ett_net_getobjectinfor_arefoffset = -1;
static gint ett_net_getobjectinfor_arefsize = -1;
static gint ett_net_getobjectinfor_arefflags = -1;
static gint ett_net_getobjectinfor_size = -1;

static char *qcom_oid_to_string( guint32 oix, guint32 vid)
{
  static char		ls[80];

  sprintf(ls, "%u.%u.%u.%u:%u", vid >> 24, 0xFF & (vid >> 16),
	  0xFF & (vid >> 8), 0xFF & vid, oix);
  return ls;  
}

static char *qcom_aref_to_string( guint32 oix, guint32 vid, guint32 offset, guint32 size)
{
  static char		ls[200];

  sprintf(ls, "%u.%u.%u.%u:%u[%u:%u]", vid >> 24, 0xFF & (vid >> 16),
	  0xFF & (vid >> 8), 0xFF & vid, oix, offset, size);
  return ls;  
}


void
proto_register_qcom(void)
{
  /* A header field is something you can search/filter on.
   * 
   * We create a structure to register our fields. It consists of an
   * array of hf_register_info structures, each of which are of the format
   * {&(field id), {name, abbrev, type, display, strings, bitmask, blurb, HFILL}}.
   */
  static hf_register_info hf[] = {
    { &hf_qcom,
      { "Data", "qcom.data", FT_NONE, BASE_NONE, NULL, 0x0,
	"QCOM Data", HFILL }},

    /* Qcom Header */
    { &hf_qcom_header,
      { "Header", "qcom.header", FT_NONE, BASE_NONE, NULL, 0x0,"QCOM Header", HFILL }},
    { &hf_qcom_header_nodeid,
      { "NodeId", "qcom.header.nodeid", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},
    { &hf_qcom_header_birth,
      { "Birth", "qcom.header.birth", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_header_flags,
      { "Flags", "qcom.header.flags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_header_lacksequence,
      { "LackSequence", "qcom.header.lacksequence", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_header_lacktimestamp,
      { "LackTimestamp", "qcom.header.lacktimestamp", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_header_racksequence,
      { "RackSequence", "qcom.header.racksequence", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_header_racktimestamp,
      { "RackTimestamp", "qcom.header.racktimestamp", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

    /* Qcom Info */
    { &hf_qcom_info,
      { "Info", "qcom.info", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM Info", HFILL }},
    { &hf_qcom_info_senderaix,
      { "SenderAix", "qcom.info.senderaix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_sendernid,
      { "SenderNid", "qcom.info.sendernid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_pid,
      { "Pid", "qcom.info.pid", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_receiverqix,
      { "ReceiverQix", "qcom.info.receiverqix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_receivernid,
      { "ReceiverNid", "qcom.info.receivernid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_replyqix,
      { "ReplyQix", "qcom.info.replyqix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_replynid,
      { "ReplyNid", "qcom.info.replynid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_btype,
      { "BType", "qcom.info.btype", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_stype,
      { "SType", "qcom.info.stype", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_rid,
      { "Rid", "qcom.info.rid", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_size,
      { "Size", "qcom.info.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_tmo,
      { "Tmo", "qcom.info.tmo", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_status,
      { "Status", "qcom.info.status", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_qcom_info_flags,
      { "Flags", "qcom.info.flags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},

    /* Mh header */
    { &hf_mh_header,
      { "MhHeader", "qcom.mh.header", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhHeader", HFILL }},
    { &hf_mh_header_xdr,
      { "Xdr", "qcom.mh.header.xdr", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_platform,
      { "Platform", "qcom.mh.header.platform", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_version,
      { "Version", "qcom.mh.header.version", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_source,
      { "Source", "qcom.mh.header.source", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_birthtime,
      { "BirthTime", "qcom.mh.header.birthtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_type,
      { "Type", "qcom.mh.header.type", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_qidqix,
      { "QidQix", "qcom.mh.header.qidqix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_qidnid,
      { "QidNid", "qcom.mh.header.qidnid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_nix,
      { "Nix", "qcom.mh.header.nix", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_outunitoix,
      { "OutunitOix", "qcom.mh.header.outunitoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_outunitvid,
      { "OutunitVid", "qcom.mh.header.outunitvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_aidoix,
      { "AidOix", "qcom.mh.header.aidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_header_aidvid,
      { "AidVid", "qcom.mh.header.aidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},

    /* Mh msginfo */
    { &hf_mh_msginfo,
      { "MhMsgInfo", "qcom.mh.msginfo", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhMsgInfo", HFILL }},
    { &hf_mh_msginfo_idnix,
      { "IdNix", "qcom.mh.msginfo.idnix", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_idbirthtime,
      { "IdBirthTime", "qcom.mh.msginfo.idbirthtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_ididx,
      { "IdIdx", "qcom.mh.msginfo.ididx", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_outunitoix,
      { "OutunitOix", "qcom.mh.msginfo.outunitoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_outunitvid,
      { "OutunitVid", "qcom.mh.msginfo.outunitvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_eventflags,
      { "EventFlags", "qcom.mh.msginfo.eventflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_eventtime,
      { "EventTime", "qcom.mh.msginfo.eventtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_eventnamev3,
      { "EventNameV3", "qcom.mh.msginfo.eventnamev3", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_eventtype,
      { "EventType", "qcom.mh.msginfo.eventtype", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_msginfo_eventprio,
      { "EventPrio", "qcom.mh.msginfo.eventprio", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

    /* Mh Message */
    { &hf_mh_message,
      { "MhMessage", "qcom.mh.message", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhMessage", HFILL }},
    { &hf_mh_message_eventtext,
      { "EventText", "qcom.mh.message.eventtext", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_status,
      { "Status", "qcom.mh.message.status", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundoix,
      { "EventSoundOix", "qcom.mh.message.eventsoundoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundvid,
      { "EventSoundVid", "qcom.mh.message.eventsoundvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundbody,
      { "EventSoundBody", "qcom.mh.message.eventsoundbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundoffset,
      { "EventSoundOffset", "qcom.mh.message.eventsoundoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundsize,
      { "EventSoundSize", "qcom.mh.message.eventsoundsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventsoundflags,
      { "EventSoundFlags", "qcom.mh.message.eventsoundflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventmoretext,
      { "EventMoreText", "qcom.mh.message.eventmoretext", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectoix,
      { "ObjectOix", "qcom.mh.message.objectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectvid,
      { "ObjectVid", "qcom.mh.message.objectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectbody,
      { "ObjectBody", "qcom.mh.message.objectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectoffset,
      { "ObjectOffset", "qcom.mh.message.objectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectsize,
      { "ObjectSize", "qcom.mh.message.objectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_objectflags,
      { "ObjectFlags", "qcom.mh.message.objectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectoix,
      { "SupObjectOix", "qcom.mh.message.supobjectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectvid,
      { "SupObjectVid", "qcom.mh.message.supobjectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectbody,
      { "SupObjectBody", "qcom.mh.message.supobjectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectoffset,
      { "SupObjectOffset", "qcom.mh.message.supobjectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectsize,
      { "SupObjectSize", "qcom.mh.message.supobjectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_supobjectflags,
      { "SupObjectFlags", "qcom.mh.message.supobjectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_eventname,
      { "EventName", "qcom.mh.message.eventname", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_message_receiver,
      { "Receiver", "qcom.mh.message.receiver", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},

    /* Mh Block */
    { &hf_mh_block,
      { "MhBlock", "qcom.mh.block", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhBlock", HFILL }},
    { &hf_mh_block_targetidnix,
      { "TargetIdNix", "qcom.mh.block.targetidnix", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_targetidbirthtime,
      { "TargetIdBirthTime", "qcom.mh.block.targetidbirthtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_targetididx,
      { "TargetIdIdx", "qcom.mh.block.targetididx", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_detecttime,
      { "DetectTime", "qcom.mh.block.detecttime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_outunitoix,
      { "OutunitOix", "qcom.mh.block.outunitoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_outunitvid,
      { "OutunitVid", "qcom.mh.block.outunitvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_status,
      { "Status", "qcom.mh.block.status", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectoix,
      { "ObjectOix", "qcom.mh.block.objectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectvid,
      { "ObjectVid", "qcom.mh.block.objectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectbody,
      { "ObjectBody", "qcom.mh.block.objectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectoffset,
      { "ObjectOffset", "qcom.mh.block.objectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectsize,
      { "ObjectSize", "qcom.mh.block.objectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_objectflags,
      { "ObjectFlags", "qcom.mh.block.objectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectoix,
      { "SupObjectOix", "qcom.mh.block.supobjectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectvid,
      { "SupObjectVid", "qcom.mh.block.supobjectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectbody,
      { "SupObjectBody", "qcom.mh.block.supobjectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectoffset,
      { "SupObjectOffset", "qcom.mh.block.supobjectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectsize,
      { "SupObjectSize", "qcom.mh.block.supobjectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_supobjectflags,
      { "SupObjectFlags", "qcom.mh.block.supobjectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_block_eventname,
      { "EventName", "qcom.mh.block.eventname", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},

    /* Mh Ack */
    { &hf_mh_ack,
      { "MhAck", "qcom.mh.ack", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhAck", HFILL }},
    { &hf_mh_ack_targetidnix,
      { "TargetIdNix", "qcom.mh.ack.targetidnix", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_targetidbirthtime,
      { "TargetIdBirthTime", "qcom.mh.ack.targetidbirthtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_targetididx,
      { "TargetIdIdx", "qcom.mh.ack.targetididx", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_detecttime,
      { "DetectTime", "qcom.mh.ack.detecttime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_outunitoix,
      { "OutunitOix", "qcom.mh.ack.outunitoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_outunitvid,
      { "OutunitVid", "qcom.mh.ack.outunitvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectoix,
      { "ObjectOix", "qcom.mh.ack.objectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectvid,
      { "ObjectVid", "qcom.mh.ack.objectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectbody,
      { "ObjectBody", "qcom.mh.ack.objectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectoffset,
      { "ObjectOffset", "qcom.mh.ack.objectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectsize,
      { "ObjectSize", "qcom.mh.ack.objectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_objectflags,
      { "ObjectFlags", "qcom.mh.ack.objectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectoix,
      { "SupObjectOix", "qcom.mh.ack.supobjectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectvid,
      { "SupObjectVid", "qcom.mh.ack.supobjectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectbody,
      { "SupObjectBody", "qcom.mh.ack.supobjectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectoffset,
      { "SupObjectOffset", "qcom.mh.ack.supobjectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectsize,
      { "SupObjectSize", "qcom.mh.ack.supobjectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_supobjectflags,
      { "SupObjectFlags", "qcom.mh.ack.supobjectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_ack_eventname,
      { "EventName", "qcom.mh.ack.eventname", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},

    /* Mh Return */
    { &hf_mh_return,
      { "MhReturn", "qcom.mh.return", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM MhReturn", HFILL }},
    { &hf_mh_return_eventtext,
      { "EventText", "qcom.mh.return.eventtext", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_targetidnix,
      { "TargetIdNix", "qcom.mh.return.targetidnix", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_targetidbirthtime,
      { "TargetIdBirthTime", "qcom.mh.return.targetidbirthtime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_targetididx,
      { "TargetIdIdx", "qcom.mh.return.targetididx", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_detecttime,
      { "DetectTime", "qcom.mh.return.detecttime", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectoix,
      { "ObjectOix", "qcom.mh.return.objectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectvid,
      { "ObjectVid", "qcom.mh.return.objectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectbody,
      { "ObjectBody", "qcom.mh.return.objectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectoffset,
      { "ObjectOffset", "qcom.mh.return.objectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectsize,
      { "ObjectSize", "qcom.mh.return.objectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_objectflags,
      { "ObjectFlags", "qcom.mh.return.objectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectoix,
      { "SupObjectOix", "qcom.mh.return.supobjectoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectvid,
      { "SupObjectVid", "qcom.mh.return.supobjectvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectbody,
      { "SupObjectBody", "qcom.mh.return.supobjectbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectoffset,
      { "SupObjectOffset", "qcom.mh.return.supobjectoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectsize,
      { "SupObjectSize", "qcom.mh.return.supobjectsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_supobjectflags,
      { "SupObjectFlags", "qcom.mh.return.supobjectflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_mh_return_eventname,
      { "EventName", "qcom.mh.return.eventname", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},

    /* Net Message */
    { &hf_net_message,
      { "NetMessage", "qcom.net.message", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM NetMessage", HFILL }},
    { &hf_net_message_xdr,
      { "Xdr", "qcom.net.message.xdr", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_message_msn,
      { "Msn", "qcom.net.message.msn", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_message_nid,
      { "Nid", "qcom.net.message.nid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},

    /* Net OidToObject */
    { &hf_net_oidtoobject,
      { "NetOidToObject", "qcom.net.oidtoobject", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM NetOidToObject", HFILL }},
    { &hf_net_oidtoobject_oidoix,
      { "OidOix", "qcom.net.oidtoobject.oidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_oidtoobject_oidvid,
      { "OidVid", "qcom.net.oidtoobject.oidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_oidtoobject_trans,
      { "Trans", "qcom.net.oidtoobject.oidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_oidtoobject_lcount,
      { "LCount", "qcom.net.oidtoobject.lcount", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_oidtoobject_rcount,
      { "RCount", "qcom.net.oidtoobject.rcount", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

    /* Net ObjectR */
    { &hf_net_objectr,
      { "NetObjectR", "qcom.net.objectr", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM NetObjectR", HFILL }},
    { &hf_net_objectr_status,
      { "Status", "qcom.net.objectr.status", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_objectr_oidoix,
      { "OidOix", "qcom.net.objectr.oidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_objectr_oidvid,
      { "OidVid", "qcom.net.objectr.oidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_objectr_count,
      { "Count", "qcom.net.objectr.count", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},


    /* Net Gobject */
    { &hf_net_gobject_oidoix,
      { "OidOix", "qcom.net.gobject.oidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_oidvid,
      { "OidVid", "qcom.net.gobject.oidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_cid,
      { "Cid", "qcom.net.gobject.cid", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_familynameorig,
      { "FamilyNameOrig", "qcom.net.gobject.familynameorig", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_familynamenorm,
      { "FamilyNameNorm", "qcom.net.gobject.familynamenorm", FT_STRING, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_familynamekey,
      { "FamilyNameKey", "qcom.net.gobject.familynamekey", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_familypoidoix,
      { "PoidOix", "qcom.net.gobject.familypoidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_familypoidvid,
      { "PoidVid", "qcom.net.gobject.familypoidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_sibflink,
      { "SibFlink", "qcom.net.gobject.sibflink", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_sibblink,
      { "SibBlink", "qcom.net.gobject.sibblink", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_soidoix,
      { "SoidOix", "qcom.net.gobject.soidoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_soidvid,
      { "SoidVid", "qcom.net.gobject.soidvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_size,
      { "Size", "qcom.net.gobject.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_gobject_flags,
      { "Flags", "qcom.net.gobject.flags", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

    /* Net GetObjectInfo */
    { &hf_net_getobjectinfo,
      { "NetGetObjectInfo", "qcom.net.getobjectinfo", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM NetGetObjectInfo", HFILL }},
    { &hf_net_getobjectinfo_arefoix,
      { "ArefOix", "qcom.net.getobjectinfo.arefoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfo_arefvid,
      { "ArefVid", "qcom.net.getobjectinfo.arefvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfo_arefbody,
      { "ArefBody", "qcom.net.getobjectinfo.arefbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfo_arefoffset,
      { "ArefOffset", "qcom.net.getobjectinfo.arefoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfo_arefsize,
      { "ArefSize", "qcom.net.getobjectinfo.arefsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfo_arefflags,
      { "ArefFlags", "qcom.net.getobjectinfo.arefflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},

    /* Net GetObjectInfoR */
    { &hf_net_getobjectinfor,
      { "NetGetObjectInfoR", "qcom.net.getobjectinfor", FT_NONE, BASE_NONE, NULL, 0x0, "QCOM NetGetObjectInfoR", HFILL }},
    { &hf_net_getobjectinfor_arefoix,
      { "Sts", "qcom.net.getobjectinfo.stsr", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefoix,
      { "ArefOix", "qcom.net.getobjectinfor.arefoix", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefvid,
      { "ArefVid", "qcom.net.getobjectinfor.arefvid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefbody,
      { "ArefBody", "qcom.net.getobjectinfor.arefbody", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefoffset,
      { "ArefOffset", "qcom.net.getobjectinfor.arefoffset", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefsize,
      { "ArefSize", "qcom.net.getobjectinfor.arefsize", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_arefflags,
      { "ArefFlags", "qcom.net.getobjectinfor.arefflags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_net_getobjectinfor_size,
      { "Size", "qcom.net.getobjectinfor.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

  };

  static gint *ett[] = {
    &ett_qcom,
    &ett_qcom_header,
    &ett_qcom_header_nodeid,
    &ett_qcom_header_birth,
    &ett_qcom_header_flags,
    &ett_qcom_header_lacksequence,
    &ett_qcom_header_lacktimestamp,
    &ett_qcom_header_racksequence,
    &ett_qcom_header_racktimestamp,
    &ett_qcom_info,
    &ett_qcom_info_senderaix,
    &ett_qcom_info_sendernid,
    &ett_qcom_info_pid,
    &ett_qcom_info_receiverqix,
    &ett_qcom_info_receivernid,
    &ett_qcom_info_replyqix,
    &ett_qcom_info_replynid,
    &ett_qcom_info_btype,
    &ett_qcom_info_stype,
    &ett_qcom_info_rid,
    &ett_qcom_info_size,
    &ett_qcom_info_tmo,
    &ett_qcom_info_status,
    &ett_qcom_info_flags,
    &ett_mh_header,
    &ett_mh_header_xdr,
    &ett_mh_header_platform,
    &ett_mh_header_version,
    &ett_mh_header_source,
    &ett_mh_header_birthtime,
    &ett_mh_header_type,
    &ett_mh_header_qidqix,
    &ett_mh_header_qidnid,
    &ett_mh_header_nix,
    &ett_mh_header_outunitoix,
    &ett_mh_header_outunitvid,
    &ett_mh_header_aidoix,
    &ett_mh_header_aidvid,
    &ett_mh_msginfo,
    &ett_mh_msginfo_idnix,
    &ett_mh_msginfo_idbirthtime,
    &ett_mh_msginfo_ididx,
    &ett_mh_msginfo_outunitoix,
    &ett_mh_msginfo_outunitvid,
    &ett_mh_msginfo_eventflags,
    &ett_mh_msginfo_eventtime,
    &ett_mh_msginfo_eventnamev3,
    &ett_mh_msginfo_eventtype,
    &ett_mh_msginfo_eventprio,
    &ett_mh_message,
    &ett_mh_message_eventtext,
    &ett_mh_message_status,
    &ett_mh_message_eventsoundoix,
    &ett_mh_message_eventsoundvid,
    &ett_mh_message_eventsoundbody,
    &ett_mh_message_eventsoundoffset,
    &ett_mh_message_eventsoundsize,
    &ett_mh_message_eventsoundflags,
    &ett_mh_message_eventmoretext,
    &ett_mh_message_objectoix,
    &ett_mh_message_objectvid,
    &ett_mh_message_objectbody,
    &ett_mh_message_objectoffset,
    &ett_mh_message_objectsize,
    &ett_mh_message_objectflags,
    &ett_mh_message_supobjectoix,
    &ett_mh_message_supobjectvid,
    &ett_mh_message_supobjectbody,
    &ett_mh_message_supobjectoffset,
    &ett_mh_message_supobjectsize,
    &ett_mh_message_supobjectflags,
    &ett_mh_message_eventname,
    &ett_mh_message_receiver,
    &ett_mh_block,
    &ett_mh_block_targetidnix,
    &ett_mh_block_targetidbirthtime,
    &ett_mh_block_targetididx,
    &ett_mh_block_detecttime,
    &ett_mh_block_outunitoix,
    &ett_mh_block_outunitvid,
    &ett_mh_block_status,
    &ett_mh_block_objectoix,
    &ett_mh_block_objectvid,
    &ett_mh_block_objectbody,
    &ett_mh_block_objectoffset,
    &ett_mh_block_objectsize,
    &ett_mh_block_objectflags,
    &ett_mh_block_supobjectoix,
    &ett_mh_block_supobjectvid,
    &ett_mh_block_supobjectbody,
    &ett_mh_block_supobjectoffset,
    &ett_mh_block_supobjectsize,
    &ett_mh_block_supobjectflags,
    &ett_mh_block_eventname,
    &ett_mh_ack,
    &ett_mh_ack_targetidnix,
    &ett_mh_ack_targetidbirthtime,
    &ett_mh_ack_targetididx,
    &ett_mh_ack_detecttime,
    &ett_mh_ack_outunitoix,
    &ett_mh_ack_outunitvid,
    &ett_mh_ack_objectoix,
    &ett_mh_ack_objectvid,
    &ett_mh_ack_objectbody,
    &ett_mh_ack_objectoffset,
    &ett_mh_ack_objectsize,
    &ett_mh_ack_objectflags,
    &ett_mh_ack_supobjectoix,
    &ett_mh_ack_supobjectvid,
    &ett_mh_ack_supobjectbody,
    &ett_mh_ack_supobjectoffset,
    &ett_mh_ack_supobjectsize,
    &ett_mh_ack_supobjectflags,
    &ett_mh_ack_eventname,
    &ett_mh_return,
    &ett_mh_return_eventtext,
    &ett_mh_return_targetidnix,
    &ett_mh_return_targetidbirthtime,
    &ett_mh_return_targetididx,
    &ett_mh_return_detecttime,
    &ett_mh_return_objectoix,
    &ett_mh_return_objectvid,
    &ett_mh_return_objectbody,
    &ett_mh_return_objectoffset,
    &ett_mh_return_objectsize,
    &ett_mh_return_objectflags,
    &ett_mh_return_supobjectoix,
    &ett_mh_return_supobjectvid,
    &ett_mh_return_supobjectbody,
    &ett_mh_return_supobjectoffset,
    &ett_mh_return_supobjectsize,
    &ett_mh_return_supobjectflags,
    &ett_mh_return_eventname,
    &ett_net_message,
    &ett_net_message_xdr,
    &ett_net_message_msn,
    &ett_net_message_nid,
    &ett_net_oidtoobject,
    &ett_net_oidtoobject_oidoix,
    &ett_net_oidtoobject_oidvid,
    &ett_net_oidtoobject_trans,
    &ett_net_oidtoobject_lcount,
    &ett_net_oidtoobject_rcount,
    &ett_net_objectr,
    &ett_net_objectr_status,
    &ett_net_objectr_oidoix,
    &ett_net_objectr_oidvid,
    &ett_net_objectr_count,
    &ett_net_gobject,
    &ett_net_gobject_oidoix,
    &ett_net_gobject_oidvid,
    &ett_net_gobject_cid,
    &ett_net_gobject_familynameorig,
    &ett_net_gobject_familynamenorm,
    &ett_net_gobject_familynamekey,
    &ett_net_gobject_familypoidoix,
    &ett_net_gobject_familypoidvid,
    &ett_net_gobject_sibflink,
    &ett_net_gobject_sibblink,
    &ett_net_gobject_soidoix,
    &ett_net_gobject_soidvid,
    &ett_net_gobject_size,
    &ett_net_gobject_flags,
    &ett_net_getobjectinfo,
    &ett_net_getobjectinfo_arefoix,
    &ett_net_getobjectinfo_arefvid,
    &ett_net_getobjectinfo_arefbody,
    &ett_net_getobjectinfo_arefoffset,
    &ett_net_getobjectinfo_arefsize,
    &ett_net_getobjectinfo_arefflags,
    &ett_net_getobjectinfor,
    &ett_net_getobjectinfor_sts,
    &ett_net_getobjectinfor_arefoix,
    &ett_net_getobjectinfor_arefvid,
    &ett_net_getobjectinfor_arefbody,
    &ett_net_getobjectinfor_arefoffset,
    &ett_net_getobjectinfor_arefsize,
    &ett_net_getobjectinfor_arefflags,
    &ett_net_getobjectinfor_size,
    
  };

  proto_register_field_array(  proto_qcom, hf, array_length(hf));
  proto_register_subtree_array( ett, array_length(ett));
  register_dissector( "qcom", dissect_qcom, proto_qcom);

  proto_qcom = proto_register_protocol( "QCom Proview","QCom","qcom");
}

void
dissect_qcom(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
  proto_item *qcom_item = NULL;
  proto_item *qcom_sub_item = NULL;
  proto_tree *qcom_tree = NULL;
  proto_tree *qcom_header_tree = NULL;
  proto_tree *qcom_info_tree = NULL;
  proto_tree *qcom_net_message_tree = NULL;
  proto_tree *qcom_net_oidtoobject_tree = NULL;
  proto_tree *qcom_net_objectr_tree = NULL;
  proto_tree *qcom_net_getobjectinfo_tree = NULL;
  proto_tree *qcom_net_getobjectinfor_tree = NULL;
  proto_tree *qcom_mh_header_tree = NULL;
  proto_tree *qcom_mh_msginfo_tree = NULL;
  proto_tree *qcom_mh_message_tree = NULL;
  proto_tree *qcom_mh_block_tree = NULL;
  proto_tree *qcom_mh_ack_tree = NULL;
  proto_tree *qcom_mh_return_tree = NULL;
  guint32 offs;
  guint32 oid_oix;
  guint32 oid_vid;
  guint32 aref_oix;
  guint32 aref_vid;
  guint32 aref_offset;
  guint32 aref_size;
  int is_user;
  int is_net;
  int is_netoidtoobject;
  int is_netobjectr;
  int is_netgetobjectinfo;
  int is_netgetobjectinfor;
  int is_mh;
  int is_mhmsg;
  int is_mhmessage;
  int is_mhblock;
  int is_mhack;
  int is_mhreturn;
  int is_sev;
  guint32 receiver_qix;
  guint32 reply_qix;
  guint32 btype;
  guint32 stype;
  guint32 msg_flags;
  guint32 msg_size;
  guint32 mh_type = 0;
  guint32 mh_id = 0;
  guint32 sev_type = 0;
  guint32 mh_eventtype = 0;
  gchar *protostrp;
  guint32 mh_endian = 0;
  guint32 net_endian = 0;
  int i;

  guint32 header_size = 28;
  guint32 info_size = 56;
  guint32 qcommsg_size = header_size + info_size + 12;
  guint32 net_message_size = 12;
  guint32 net_oidtoobject_size = 20;
  guint32 net_objectr_size = 16;
  guint32 net_getobjectinfo_size = 24;
  guint32 net_getobjectinfor_size = 32;
  guint32 net_gobject_count = 0;
  guint32 mh_header_size = 76;
  guint32 mh_msginfo_size = 140;
  guint32 mh_message_size = 888;
  guint32 mh_block_size = 484;
  guint32 mh_ack_size = 516;
  guint32 mh_return_size = 584;
  gchar info[300] = "";

  static gchar protostr[][20] = {
    "QCOM",
    "QCOM-SYS",
    "QCOM-EV",
    "QCOM-Net",
    "QCOM-Mh",
    "QCOM-Sev"
  };

  protostrp = protostr[0];
  strcpy( info, "Unknown basetype");
  
  msg_flags = tvb_get_ntohl( tvb, 8);


  is_user = 0;
  switch ( msg_flags >> 24) {
  case eEvent_connect: {
    strcpy( info, "Connect");
    break;
  }
  case eEvent_down: {
    strcpy( info, "Down");
    break;
  }
  case eEvent_user: {
    strcpy( info, "User");
    is_user = 1;
    break;
  }
  case eEvent_ack: {
    strcpy( info, "Ack");
    break;
  }
  }

  is_net = 0;
  is_netoidtoobject = 0;
  is_netobjectr = 0;
  is_netgetobjectinfo = 0;
  is_netgetobjectinfor = 0;
  is_mh = 0;
  is_mhmsg = 0;
  is_mhmessage = 0;
  is_mhblock = 0;
  is_mhack = 0;
  is_mhreturn = 0;
  is_sev = 0;
  if ( is_user) {
    receiver_qix = tvb_get_ntohl( tvb, header_size + 12);
    reply_qix = tvb_get_ntohl( tvb, header_size + 20);
    btype = tvb_get_ntohl( tvb, header_size + 28);
    stype = tvb_get_ntohl( tvb, header_size + 32);
    msg_size = tvb_get_ntohl( tvb, header_size + 40);

    if ( receiver_qix == 122 || reply_qix == 122)
      /* Fix for 4.8.2 and earlier */
      btype = sev_cMsgClass;

    switch ( btype) {
    case qcom_eBtype_system: { 
      /* System */
      protostrp = protostr[1];  /* QCOM SYS */

      break;
    }
    case qcom_eBtype_qcom: { 
      /* Qcom */
      protostrp = protostr[0]; /* QCOM */

      switch ( stype) {
      case qcom_eStype_linkConnect:
	strcpy( info, "LinkConnect");
	break;
      case qcom_eStype_linkDisconnect:
	strcpy( info, "LinkDisonnect");
	break;
      case qcom_eStype_linkActive:
	strcpy( info, "LinkActive");
	break;
      case qcom_eStype_linkStalled:
	strcpy( info, "LinkStalled");
	break;
      case qcom_eStype_applConnect:
	strcpy( info, "ApplConnect");
	break;
      case qcom_eStype_applDisconnect:
	strcpy( info, "ApplDisconnect");
	break;
      default:
	strcpy( info, "Unknown subtype");
      }

      break;
    }
    case qcom_eBtype_event: {
      /* Event */
      protostrp = protostr[2]; /* QCOM EV */

      switch ( stype) {
      case qcom_cIloopBack:
	strcpy( info, "LoopBack");
	break;
      case qcom_cInetEvent:
	strcpy( info, "NetEvent");
	break;
      case qcom_cIapplEvent:
	strcpy( info, "ApplEvent");
	break;
      case qcom_cImhAllHandlers:
	strcpy( info, "MhAllHandlers");
	break;
      case qcom_cImhAllOutunits:
	strcpy( info, "MhAllOutunits");
	break;
      case qcom_cIhdServer:
	strcpy( info, "HdServer");
	break;
      case qcom_cIhdClient:
	strcpy( info, "HdClient");
	break;
      case qcom_cInacp:
	strcpy( info, "Nacp");
	break;
      case qcom_cIini:
	strcpy( info, "Ini");
	break;
      default:
	strcpy( info, "Unknown subtype");
      }
      break;
    }
    case net_cMsgClass: {
      /* Neth */
      protostrp = protostr[3]; /* NETH */

      is_net = 1;
      net_endian = ENC_LITTLE_ENDIAN;

      switch ( stype) {
      case net_eMsg_error:
	strcpy( info, "Error");
	break;
      case net_eMsg_id:
	strcpy( info, "Id");
	break;
      case net_eMsg_idAck:
	strcpy( info, "IdAck");
	break;
      case net_eMsg_idAck2:
	strcpy( info, "IdAck2");
	break;
      case net_eMsg_volumes:
	strcpy( info, "Volumes");
	break;
      case net_eMsg_volumesR:
	strcpy( info, "VolumesR");
	break;
      case net_eMsg_subAdd:
	strcpy( info, "SubAdd");
	break;
      case net_eMsg_subRemove:
	strcpy( info, "SubRemove");
	break;
      case net_eMsg_subData:
	strcpy( info, "SubData");
	break;
      case net_eMsg_sanAdd:
	strcpy( info, "SanAdd");
	break;
      case net_eMsg_sanRemove:
	strcpy( info, "SanRemove");
	break;
      case net_eMsg_sanUpdate:
	strcpy( info, "SanUpdate");
	break;
      case net_eMsg_nameToObject:
	strcpy( info, "NameToObject");
	break;
      case net_eMsg_oidToObject:
	is_netoidtoobject = 1;

	/* Get oid */
	offs = header_size + info_size + net_message_size;
	if ( net_endian == ENC_BIG_ENDIAN) {
	  oid_oix = tvb_get_ntohl( tvb, offs);
	  offs += 4;
	  oid_vid = tvb_get_ntohl( tvb, offs);
	}
	else {
	  oid_oix = tvb_get_letohl( tvb, offs);
	  offs += 4;
	  oid_vid = tvb_get_letohl( tvb, offs);
	}
	sprintf( info, "OidToObject     oid  %s", qcom_oid_to_string( oid_oix, oid_vid));
	break;
      case net_eMsg_objectR: {
	gchar name[200];
	guint32 poid_oix;
	guint32 prev_oid_oix;
	guint32 prev_poid_oix = 0;
	strcpy( info, "ObjectR");
	is_netobjectr = 1;

	/* Get gobject count */
	offs = header_size + info_size + net_message_size + 12;
	if ( net_endian == ENC_BIG_ENDIAN)
	  net_gobject_count = tvb_get_ntohl( tvb, offs);
	else
	  net_gobject_count = tvb_get_letohl( tvb, offs);

	/* Get the name */
	offs += 4;
	strcpy( name, "");
	for ( i = 0; i < (int)net_gobject_count; i++) {
	  if ( i == 1 && prev_poid_oix == 0)
	    strcat( name, ":");
	  else if ( i > 1)
	    strcat( name, "-");	    

	  if ( net_endian == ENC_BIG_ENDIAN)
	    oid_oix = tvb_get_ntohl( tvb, offs);
	  else
	    oid_oix = tvb_get_letohl( tvb, offs);
	  offs += 12;
	  strncat( name, tvb_get_string( tvb, offs, 32), sizeof(name));
	  offs += 68;
	  if ( net_endian == ENC_BIG_ENDIAN)
	    poid_oix = tvb_get_ntohl( tvb, offs);
	  else
	    poid_oix = tvb_get_letohl( tvb, offs);
	  offs += 32;

	  if  ( i != 0 && poid_oix != prev_poid_oix)
	    break;

	  prev_oid_oix = oid_oix;
	  prev_poid_oix = poid_oix;	    
	}

	if ( strcmp( name, "") != 0)
	  sprintf( info, "ObjectR         name %s", name);

	if ( net_gobject_count > 10)
	  net_gobject_count = 10;

	break;
      }
      case net_eMsg_getObjectInfo:
	is_netgetobjectinfo = 1;

	/* Get aref */
	offs = header_size + info_size + net_message_size;
	if ( net_endian == ENC_BIG_ENDIAN) {
	  aref_oix = tvb_get_ntohl( tvb, offs);
	  offs += 4;
	  aref_vid = tvb_get_ntohl( tvb, offs);
	  offs += 8;
	  aref_offset = tvb_get_ntohl( tvb, offs);
	  offs += 4;
	  aref_size = tvb_get_ntohl( tvb, offs);	    
	}
	else {
	  aref_oix = tvb_get_letohl( tvb, offs);
	  offs += 4;
	  aref_vid = tvb_get_letohl( tvb, offs);
	  offs += 8;
	  aref_offset = tvb_get_letohl( tvb, offs);
	  offs += 4;
	  aref_size = tvb_get_letohl( tvb, offs);
	}
	sprintf( info, "GetObjectInfo   aref %s", qcom_aref_to_string( aref_oix, aref_vid,
								      aref_offset, aref_size));
	break;
      case net_eMsg_getObjectInfoR:
	is_netgetobjectinfor = 1;

	/* Get aref */
	offs = header_size + info_size + net_message_size + 4;
	if ( net_endian == ENC_BIG_ENDIAN) {
	  aref_oix = tvb_get_ntohl( tvb, offs);
	  offs += 4;
	  aref_vid = tvb_get_ntohl( tvb, offs);
	  offs += 8;
	  aref_offset = tvb_get_ntohl( tvb, offs);
	  offs += 4;
	  aref_size = tvb_get_ntohl( tvb, offs);	    
	}
	else {
	  aref_oix = tvb_get_letohl( tvb, offs);
	  offs += 4;
	  aref_vid = tvb_get_letohl( tvb, offs);
	  offs += 8;
	  aref_offset = tvb_get_letohl( tvb, offs);
	  offs += 4;
	  aref_size = tvb_get_letohl( tvb, offs);
	}
	sprintf( info, "GetObjectInfoR  aref %s", qcom_aref_to_string( aref_oix, aref_vid,
								       aref_offset, aref_size));
	break;
      case net_eMsg_setObjectInfo:
	strcpy( info, "SetObjectInfo");
	break;
      case net_eMsg_setObjectInfoR:
	strcpy( info, "SetObjectInfoR");
	break;
      case net_eMsg_flush:
	strcpy( info, "Flush");
	break;
      case net_eMsg_createObject:
	strcpy( info, "CreateObject");
	break;
      case net_eMsg_deleteObject:
	strcpy( info, "DeleteObject");
	break;
	strcpy( info, "MoveObject");
      case net_eMsg_moveObject:
	break;
      case net_eMsg_renameObject:
	strcpy( info, "RenameObject");
	break;
      case net_eMsg_nodeUp:
	strcpy( info, "NodeUp");
	break;
      case net_eMsg_nodeDown:
	strcpy( info, "NodeDown");
	break;
      case net_eMsg_getCclass:
	strcpy( info, "GetCclass");
	break;
      case net_eMsg_getCclassR:
	strcpy( info, "GetCclassR");
	break;
      case net_eMsg_getGclass:
	strcpy( info, "GetGclass");
	break;
      case net_eMsg_getGclassR:
	strcpy( info, "GetGclassR");
	break;
      case net_eMsg_serverConnect:
	strcpy( info, "ServerConnect");
	break;
      case net_eMsg_fileList:
	strcpy( info, "FileList");
	break;
      case net_eMsg_fileListR:
	strcpy( info, "FileListR");
	break;
      case net_eMsg_volumes7:
	strcpy( info, "Volumes7");
	break;
      default:
	strcpy( info, "Unknown net_eMsgType");
      }
      break;
    }
    case mh_cMsgClass: {
      /* Mh */
      protostrp = protostr[4];  /* MH */

      if ( msg_size >= 28) {
	is_mh = 1;
	mh_type = tvb_get_ntohl( tvb, qcommsg_size + 24);
	mh_endian = ENC_BIG_ENDIAN;
	if ( mh_type > 255) {
	  mh_type = tvb_get_letohl( tvb, qcommsg_size + 24);
	  mh_endian = ENC_LITTLE_ENDIAN;
	}

	switch ( mh_type) {
	case mh_eMsg_ApplConnect:
	  strcpy( info, "ApplConnect");
	  break;
	case mh_eMsg_ApplDisconnect:
	  strcpy( info, "ApplDisconnect");
	  break;
	case mh_eMsg_ApplGetMsgInfo:
	  strcpy( info, "ApplGetMsgInfo");
	  break;
	case mh_eMsg_ApplMessage:
	  strcpy( info, "ApplMessage");
	  break;
	case mh_eMsg_ApplReply:
	  strcpy( info, "ApplReply");
	  break;
	case mh_eMsg_ApplReturn:
	  strcpy( info, "ApplReturn");
	  break;
	case mh_eMsg_Event: {
	  is_mhmsg = 1;
	  
	  /* Get event type */
	  if ( mh_endian == ENC_BIG_ENDIAN)
	    mh_eventtype = tvb_get_ntohl( tvb, qcommsg_size + mh_header_size + 132);
	  else
	    mh_eventtype = tvb_get_letohl( tvb, qcommsg_size + mh_header_size + 132);

	  switch ( mh_eventtype) {
	  case mh_eEvent_Ack:
	    is_mhack = 1;

	    if ( mh_endian == ENC_BIG_ENDIAN) 
	      mh_id = tvb_get_ntohl( tvb, qcommsg_size + mh_header_size + mh_msginfo_size + 12);
	    else
	      mh_id = tvb_get_letohl( tvb, qcommsg_size + mh_header_size + mh_msginfo_size + 12);

	    snprintf( info, sizeof(info), "Event Ack       id   %-5d", mh_id);
	    break;
	  case mh_eEvent_Block:
	  case mh_eEvent_CancelBlock:
	  case mh_eEvent_Reblock:
	  case mh_eEvent_Unblock:
	    is_mhblock = 1;

	    if ( mh_eventtype == mh_eEvent_CancelBlock)
	      strcpy( info, "Event CancelBlo ");
	    else if ( mh_eventtype == mh_eEvent_Reblock)
	      strcpy( info, "Event CancelBlo ");
	    else if ( mh_eventtype == mh_eEvent_Unblock)
	      strcpy( info, "Event Unblock   ");
	    else
	      strcpy( info, "Event Block     ");

	    /* Get oid */
	    offs = qcommsg_size + mh_header_size + mh_msginfo_size + 36;
	    if ( mh_endian == ENC_BIG_ENDIAN) {
	      oid_oix = tvb_get_ntohl( tvb, offs);
	      offs += 4;
	      oid_vid = tvb_get_ntohl( tvb, offs);
	    }
	    else {
	      oid_oix = tvb_get_letohl( tvb, offs);
	      offs += 4;
	      oid_vid = tvb_get_letohl( tvb, offs);
	    }
	    snprintf( &info[16], sizeof(info)-16, "oid  %s", qcom_oid_to_string( oid_oix, oid_vid));
	    break;
	  case mh_eEvent_Missing:
	    strcpy( info, "Event Missing");
	    break;
	  case mh_eEvent_Cancel:
	  case mh_eEvent_Return:
	    is_mhreturn = 1;

	    if ( mh_eventtype == mh_eEvent_Cancel)
	      strcpy( info, "Event Cancel    ");
	    else
	      strcpy( info, "Event Return    ");

	    if ( mh_endian == ENC_BIG_ENDIAN) 
	      mh_id = tvb_get_ntohl( tvb, qcommsg_size + mh_header_size + mh_msginfo_size + 92);
	    else
	      mh_id = tvb_get_letohl( tvb, qcommsg_size + mh_header_size + mh_msginfo_size + 92);

	    snprintf( &info[16], sizeof(info)-16, "id   %-5d", mh_id);
	    break;
	  case mh_eEvent_Info:
	  case mh_eEvent_Alarm:
	    is_mhmessage = 1;

	    if ( mh_eventtype == mh_eEvent_Info)
	      strcpy( info, "Event Info      ");
	    else
	      strcpy( info, "Event Alarm     ");

	    if ( mh_endian == ENC_BIG_ENDIAN) 
	      mh_id = tvb_get_ntohl( tvb, qcommsg_size + mh_header_size + 12);
	    else
	      mh_id = tvb_get_letohl( tvb, qcommsg_size + mh_header_size + 12);

	    snprintf( &info[16], sizeof(info)-16, "id   %-5d  \"%s\"", mh_id, 
		      tvb_get_string( tvb, qcommsg_size + mh_header_size + mh_msginfo_size, 80));
	    break;
	  default: 
	    strcpy( info, "Unknown Mh EventType");
	  }
	  break;
	}
	case mh_eMsg_HandlerDisconnect:
	  strcpy( info, "HandlerDisconnect");
	  break;
	case mh_eMsg_HandlerHello:
	  strcpy( info, "HandlerHello");
	  break;
	case mh_eMsg_HandlerSync:
	  strcpy( info, "HandlerSync");
	  break;
	case mh_eMsg_OutunitAck:
	  strcpy( info, "OutunitAck");
	  break;
	case mh_eMsg_OutunitBlock:
	  strcpy( info, "OutunitBlock");
	  break;
	case mh_eMsg_OutunitDisconnect:
	  strcpy( info, "OutunitDisconnect");
	  break;
	case mh_eMsg_OutunitHello:
	  strcpy( info, "OutunitHello");
	  break;
	case mh_eMsg_OutunitInfo:
	  strcpy( info, "OutunitInfo");
	  break;
	case mh_eMsg_OutunitSync:
	  strcpy( info, "OutunitSync");
	  break;
	case mh_eMsg_ProcDown:
	  strcpy( info, "ProcDown");
	  break;
	case mh_eMsg_StopScanSup:
	  strcpy( info, "StopScanSup");
	  break;
	case mh_eMsg_StartScanSup:
	  strcpy( info, "StartScanSup");
	  break;
	case mh_eMsg_Sync:
	  strcpy( info, "Sync");
	  break;
	default:
	  strcpy( info, "Unknown mh_eMsg");
	}
      }
      else {
	strcpy( info, "Unknown sub type");
      }
      break;
    }
    case sev_cMsgClass: {
      /* Sev */
      protostrp = protostr[5];  /* QCOM-Sev */

      if ( msg_size >= 4) {
	is_sev = 1;
	sev_type = tvb_get_ntohl( tvb, header_size + info_size);

	// printf( "sev_type: %d %d\n", header_size + info_size, sev_type);
	  
	switch ( sev_type) {
	case sev_eMsgType_NodeUp:
	  strcpy( info, "NodeUp");
	  break;
	case sev_eMsgType_HistItemsRequest:
	  strcpy( info, "HistItemRequest");
	  break;
	case sev_eMsgType_HistItems:
	  strcpy( info, "HistItems");
	  break;
	case sev_eMsgType_HistDataStore:
	  strcpy( info, "HistDataStore");
	  break;
	case sev_eMsgType_HistDataGetRequest:
	  strcpy( info, "HistDataGetRequest");
	  break;
	case sev_eMsgType_HistDataGet:
	  strcpy( info, "HistDataGet");
	  break;
	case sev_eMsgType_HistItemDelete:
	  strcpy( info, "HistItemDelete");
	  break;
	case sev_eMsgType_HistItemStatus:
	  strcpy( info, "HistItemStatus");
	  break;
	case sev_eMsgType_ServerStatusRequest:
	  strcpy( info, "ServerStatusRequest");
	  break;
	case sev_eMsgType_ServerStatus:
	  strcpy( info, "ServerStatus");
	  break;
	case sev_eMsgType_HistObjectDataGetRequest:
	  strcpy( info, "HistObjectDataGetRequest");
	  break;
	case sev_eMsgType_HistObjectDataGet:
	  strcpy( info, "HistObjectDataGet");
	  break;
	default:
	  strcpy( info, "Unknown sev_eMsgType");
	}
      }
      break;
    }

    default:
      protostrp = protostr[0]; /* QCOM */
      strcpy( info, "Unknown basetype");
    }
  }

  if (check_col(pinfo->cinfo, COL_PROTOCOL)) {
    col_set_str(pinfo->cinfo, COL_PROTOCOL, protostrp);
  }

  /* Clear out stuff in the info column */
  if(check_col(pinfo->cinfo,COL_INFO)){    
    if ( strcmp( info, "") != 0)
      col_add_fstr(pinfo->cinfo, COL_INFO, "%s", info);
    else
      col_clear(pinfo->cinfo,COL_INFO);
  }

  // Here we check to see if the INFO column is present. If it is we output 
  // which ports the packet came from and went to. Also, we indicate the type 
  // of packet.

#if 0  
  // This is not a good way of dissecting packets. The tvb length should
  // be sanity checked so we aren't going past the actual size.

  type = tvb_get_guint8( tvb, 4 ); // Get the type byte

  if (check_col(pinfo->cinfo, COL_INFO)) {
    col_add_fstr(pinfo->cinfo, COL_INFO, "%d > %d Info Type:[%s]",
		 pinfo->srcporct, pinfo->destport, 
		 val_to_str(type, packettypenames, "Unknown Type:0x%02x"));
  }
#endif

  if (tree) { /* we are being asked for details */
    guint32 offset = 0;

    qcom_item = proto_tree_add_item(tree, proto_qcom, tvb, 0, -1, FALSE);
    qcom_tree = proto_item_add_subtree(qcom_item, ett_qcom);
    // qcom_header_tree = proto_item_add_subtree(qcom_item, ett_qcom);

    /* Header map */
    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_qcom_header, 
					 tvb, offset, header_size, FALSE );

    qcom_header_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);

    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_nodeid, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_birth, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_flags, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_lacksequence, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_lacktimestamp, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_racksequence, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    qcom_sub_item = proto_tree_add_item( qcom_header_tree, hf_qcom_header_racktimestamp, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    


    if ( is_user) {
      /* Info map */
      qcom_sub_item = proto_tree_add_item( qcom_tree, hf_qcom_info, 
					   tvb, offset, info_size, FALSE );

      qcom_info_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_senderaix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_sendernid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_pid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_receiverqix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_receivernid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_replyqix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_replynid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_btype, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_stype, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_rid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_size, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_tmo, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_status, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      qcom_sub_item = proto_tree_add_item( qcom_info_tree, hf_qcom_info_flags, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    

      if ( is_net) {

	/* Net message map */
	qcom_sub_item = proto_tree_add_item( qcom_tree, hf_net_message, 
					     tvb, offset, net_message_size, FALSE );

	qcom_net_message_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	qcom_sub_item = proto_tree_add_item( qcom_net_message_tree, hf_net_message_xdr, 
					     tvb, offset, 4, net_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_net_message_tree, hf_net_message_msn, 
					     tvb, offset, 4, net_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_net_message_tree, hf_net_message_nid, 
					     tvb, offset, 4, net_endian );
	offset += 4;    

	if ( is_netoidtoobject) {

	  /* Net oidtoobject map */
	  qcom_sub_item = proto_tree_add_item( qcom_tree, hf_net_oidtoobject, 
					       tvb, offset, net_oidtoobject_size, FALSE );

	  qcom_net_oidtoobject_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	  qcom_sub_item = proto_tree_add_item( qcom_net_oidtoobject_tree, hf_net_oidtoobject_oidoix, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_oidtoobject_tree, hf_net_oidtoobject_oidvid, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_oidtoobject_tree, hf_net_oidtoobject_trans, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_oidtoobject_tree, hf_net_oidtoobject_lcount, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_oidtoobject_tree, hf_net_oidtoobject_rcount, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	}
	else if ( is_netobjectr) {

	  /* Net objectr map */
	  qcom_sub_item = proto_tree_add_item( qcom_tree, hf_net_objectr, 
					       tvb, offset, net_objectr_size, FALSE );

	  qcom_net_objectr_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	  qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_objectr_status, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_objectr_oidoix, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_objectr_oidvid, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_objectr_count, 
					       tvb, offset, 4, net_endian );
	  offset += 4;

	  /* Gobject */
	  for ( i = 0; i < (int) net_gobject_count; i++) {
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_oidoix, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_oidvid, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_cid, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_familynameorig, 
						 tvb, offset, 32, net_endian );
	    offset += 32;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_familynamenorm, 
						 tvb, offset, 32, net_endian );
	    offset += 32;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_familynamekey, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_familypoidoix, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_familypoidvid, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_sibflink, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_sibblink, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_soidoix, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_soidvid, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_size, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_net_objectr_tree, hf_net_gobject_flags, 
						 tvb, offset, 4, net_endian );
	    offset += 4;    
	  }
	}
	else if ( is_netgetobjectinfo) {

	  /* Net getobjectinfo map */
	  qcom_sub_item = proto_tree_add_item( qcom_tree, hf_net_getobjectinfo, 
					       tvb, offset, net_getobjectinfo_size, FALSE );

	  qcom_net_getobjectinfo_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefoix, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefvid, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefbody, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefoffset, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefsize, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfo_arefflags, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	}
	else if ( is_netgetobjectinfor) {

	  /* Net getobjectinfo map */
	  qcom_sub_item = proto_tree_add_item( qcom_tree, hf_net_getobjectinfor, 
					       tvb, offset, net_getobjectinfor_size, FALSE );

	  qcom_net_getobjectinfor_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_sts, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefoix, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefvid, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefbody, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefoffset, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefsize, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_arefflags, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_net_getobjectinfo_tree, hf_net_getobjectinfor_size, 
					       tvb, offset, 4, net_endian );
	  offset += 4;    
	}
      }
      else if ( is_mh) {
	/* Mh header map */
	offset += 12;

	qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_header, 
					     tvb, offset, mh_header_size, FALSE );

	qcom_mh_header_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_xdr, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_platform, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_version, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_source, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_birthtime, 
					     tvb, offset, 8, mh_endian );
	offset += 8;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_type, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_qidqix, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_qidnid, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_nix, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_outunitoix, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_outunitvid, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_aidoix, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	qcom_sub_item = proto_tree_add_item( qcom_mh_header_tree, hf_mh_header_aidvid, 
					     tvb, offset, 4, mh_endian );
	offset += 4;    
	offset += 20;

	if ( is_mhmsg) {
	  /* Mh msginfo map */
	  qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_msginfo, 
					       tvb, offset, mh_msginfo_size, FALSE );

	  qcom_mh_msginfo_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_idnix, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_idbirthtime, 
					       tvb, offset, 8, mh_endian );
	  offset += 8;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_ididx, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  offset += 8;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_outunitoix, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_outunitvid, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  offset += 8;
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_eventflags, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_eventtime, 
					       tvb, offset, 8, mh_endian );
	  offset += 8;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_eventnamev3, 
					       tvb, offset, 80, mh_endian );
	  offset += 80;
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_eventtype, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;    
	  qcom_sub_item = proto_tree_add_item( qcom_mh_msginfo_tree, hf_mh_msginfo_eventprio, 
					       tvb, offset, 4, mh_endian );
	  offset += 4;

	  if ( is_mhmessage) {
	    /* Mh message map */
	    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_message, 
						 tvb, offset, mh_message_size, FALSE );

	    qcom_mh_message_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventtext, 
						 tvb, offset, 80, mh_endian );
	    offset += 80;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_status, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    offset += 36; /* SupInfo TODO... */
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventsoundflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventmoretext, 
						 tvb, offset, 256, mh_endian );
	    offset += 256;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_objectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_supobjectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_eventname, 
						 tvb, offset, 400, mh_endian );
	    offset += 400;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_message_tree, hf_mh_message_receiver, 
						 tvb, offset, 40, mh_endian );
	    offset += 40;    
	  }
	  else if ( is_mhblock) {
	    /* Mh block map */
	    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_block, 
						 tvb, offset, mh_block_size, FALSE );

	    qcom_mh_block_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_targetidnix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_targetidbirthtime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_targetididx, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_detecttime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_outunitoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_outunitvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_status, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_objectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_supobjectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_block_tree, hf_mh_block_eventname, 
						 tvb, offset, 400, mh_endian );
	    offset += 400;    
	  }
	  else if ( is_mhack) {
	    /* Mh ack map */
	    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_ack, 
						 tvb, offset, mh_ack_size, FALSE );

	    qcom_mh_ack_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_targetidnix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_targetidbirthtime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_targetididx, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_detecttime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_outunitoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_outunitvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    offset += 36; /* SupInfo TODO */
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_objectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_supobjectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_ack_tree, hf_mh_ack_eventname, 
						 tvb, offset, 400, mh_endian );
	    offset += 400;    
	  }
	  else if ( is_mhreturn) {
	    /* Mh return map */
	    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_mh_return, 
						 tvb, offset, mh_return_size, FALSE );

	    qcom_mh_return_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);
      
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_eventtext, 
						 tvb, offset, 80, mh_endian );
	    offset += 80;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_targetidnix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_targetidbirthtime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_targetididx, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_detecttime, 
						 tvb, offset, 8, mh_endian );
	    offset += 8;    
	    offset += 36; /* SupInfo TODO */
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_objectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectoix, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectvid, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectbody, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectoffset, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectsize, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_supobjectflags, 
						 tvb, offset, 4, mh_endian );
	    offset += 4;    
	    qcom_sub_item = proto_tree_add_item( qcom_mh_return_tree, hf_mh_return_eventname, 
						 tvb, offset, 400, mh_endian );
	    offset += 400;    
	  }
	}
      }
    }
#if 0
    guint32 offset = 0;
    guint32 nodeid = 0;

    qcom_item = proto_tree_add_item(tree, proto_qcom, tvb, 0, -1, FALSE);
    qcom_tree = proto_item_add_subtree(qcom_item, ett_qcom);
    qcom_header_tree = proto_item_add_subtree(qcom_item, ett_qcom);

    qcom_sub_item = proto_tree_add_item( qcom_tree, hf_qcom_header, 
					 tvb, offset, -1, FALSE );

    qcom_header_tree = proto_item_add_subtree(qcom_sub_item, ett_qcom);

    // We use tvb_memcpy to get our length value out (Host order)

    tvb_memcpy(tvb, (guint32 *)&nodeid, offset, 4);
    proto_tree_add_uint(qcom_header_tree, hf_qcom_nodeid, tvb, offset, 4, nodeid);

    // We increment the offset to get past the 4 bytes indicating length
    offset+=4;

    // Here we submit the type parameter to the tree. 
    proto_tree_add_item(qcom_header_tree, hf_qcom_type, tvb, offset, 1, FALSE);
    type = tvb_get_guint8( tvb, offset ); // Get our type byte

    offset+=1;
    //If the type is TEXT, we add the text to the tree. 

    if( type == 0 ) {
      proto_tree_add_item( qcom_tree, hf_qcom_text, tvb, offset, length-1, FALSE );
    }
#endif
  }
  
#if 0
  col_set_str(pinfo->cinfo, COL_PROTOCOL, "QCom");
  /* Clear out stuff in the info column */
  col_clear(pinfo->cinfo, COL_INFO);
#endif
}


void
proto_register_handoff_qcom(void)
{
  static gboolean initialized = FALSE;
  char busid[] = "PWR_BUS_ID";
  char *s;
  int port;

  if ( !initialized) {
    s = getenv( busid);
    if ( s) {
      port = atoi( s) + 55000;
      // port = 999 + 55000;
    }
    else
      /* Default port 999 */
      port = 999 + 55000;

    printf( "QCOM Port %d\n", port);
    
    data_handle = find_dissector("data");
    qcom_handle = create_dissector_handle(dissect_qcom, proto_qcom);
    dissector_add_uint( "udp.port", port, qcom_handle);
  }
}
