/* packet-redcom.c
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
#include "packet-redcom.h"
#include "redcom_def.h"

/*
 * No public information available.
 */
#define REDCOM_PORT 50
#define PROTO_TAG_REDCOM    "REDCOM"

static int proto_redcom = -1;

static dissector_handle_t data_handle = 0;
static dissector_handle_t redcom_handle;

static const value_string packettypenames[] = {
  { 0, "TEXT"},
  { 1, "SOMETHING_ELSE" },
  { 0, 0}};

static gint hf_redcom = -1;
static gint hf_redcom_header = -1;
static gint hf_redcom_header_nodeid = -1;
static gint hf_redcom_header_birth = -1;
static gint hf_redcom_header_flags = -1;
static gint hf_redcom_header_lacksequence = -1;
static gint hf_redcom_header_lacktimestamp = -1;
static gint hf_redcom_header_racksequence = -1;
static gint hf_redcom_header_racktimestamp = -1;
static gint hf_redcom_header_prio = -1;
static gint hf_redcom_header_state = -1;
static gint hf_redcom_info = -1;
static gint hf_redcom_info_senderaix = -1;
static gint hf_redcom_info_sendernid = -1;
static gint hf_redcom_info_pid = -1;
static gint hf_redcom_info_receiverqix = -1;
static gint hf_redcom_info_receivernid = -1;
static gint hf_redcom_info_replyqix = -1;
static gint hf_redcom_info_replynid = -1;
static gint hf_redcom_info_btype = -1;
static gint hf_redcom_info_stype = -1;
static gint hf_redcom_info_rid = -1;
static gint hf_redcom_info_size = -1;
static gint hf_redcom_info_tmo = -1;
static gint hf_redcom_info_status = -1;
static gint hf_redcom_info_flags = -1;
static gint hf_redu_msgheader = -1;
static gint hf_redu_msgheader_type = -1;
static gint hf_redu_msgheader_size = -1;
static gint hf_redu_msgheader_version = -1;
static gint hf_redu_tablemsgheader = -1;
static gint hf_redu_tablemsgheader_type = -1;
static gint hf_redu_tablemsgheader_size = -1;
static gint hf_redu_tablemsgheader_attributes = -1;
static gint hf_redu_tablemsgheader_version = -1;
static gint hf_redu_tablereqmsgheader = -1;
static gint hf_redu_tablereqmsgheader_type = -1;
static gint hf_redu_tableversionreqmsgheader = -1;
static gint hf_redu_tableversionreqmsgheader_type = -1;
static gint hf_redu_tableversionmsgheader = -1;
static gint hf_redu_tableversionmsgheader_type = -1;

/* These are the ids of the subtrees that we may be creating */
static gint ett_redcom = -1;
static gint ett_redcom_header = -1;
static gint ett_redcom_header_nodeid = -1;
static gint ett_redcom_header_birth = -1;
static gint ett_redcom_header_flags = -1;
static gint ett_redcom_header_lacksequence = -1;
static gint ett_redcom_header_lacktimestamp = -1;
static gint ett_redcom_header_racksequence = -1;
static gint ett_redcom_header_racktimestamp = -1;
static gint ett_redcom_header_prio = -1;
static gint ett_redcom_header_state = -1;
static gint ett_redcom_info = -1;
static gint ett_redcom_info_senderaix = -1;
static gint ett_redcom_info_sendernid = -1;
static gint ett_redcom_info_pid = -1;
static gint ett_redcom_info_receiverqix = -1;
static gint ett_redcom_info_receivernid = -1;
static gint ett_redcom_info_replyqix = -1;
static gint ett_redcom_info_replynid = -1;
static gint ett_redcom_info_btype = -1;
static gint ett_redcom_info_stype = -1;
static gint ett_redcom_info_rid = -1;
static gint ett_redcom_info_size = -1;
static gint ett_redcom_info_tmo = -1;
static gint ett_redcom_info_status = -1;
static gint ett_redcom_info_flags = -1;
static gint ett_redu_msgheader = -1;
static gint ett_redu_msgheader_type = -1;
static gint ett_redu_msgheader_size = -1;
static gint ett_redu_msgheader_version = -1;
static gint ett_redu_tablemsgheader = -1;
static gint ett_redu_tablemsgheader_type = -1;
static gint ett_redu_tablemsgheader_size = -1;
static gint ett_redu_tablemsgheader_attributes = -1;
static gint ett_redu_tablemsgheader_version = -1;
static gint ett_redu_tablereqmsgheader = -1;
static gint ett_redu_tablereqmsgheader_type = -1;
static gint ett_redu_tableversionreqmsgheader = -1;
static gint ett_redu_tableversionreqmsgheader_type = -1;
static gint ett_redu_tableversionmsgheader = -1;
static gint ett_redu_tableversionmsgheader_type = -1;


void
proto_register_redcom(void)
{
  /* A header field is something you can search/filter on.
   * 
   * We create a structure to register our fields. It consists of an
   * array of hf_register_info structures, each of which are of the format
   * {&(field id), {name, abbrev, type, display, strings, bitmask, blurb, HFILL}}.
   */
  static hf_register_info hf[] = {
    { &hf_redcom,
      { "Data", "redcom.data", FT_NONE, BASE_NONE, NULL, 0x0,
	"REDCOM Data", HFILL }},

    /* Redcom Header */
    { &hf_redcom_header,
      { "Header", "redcom.header", FT_NONE, BASE_NONE, NULL, 0x0,"REDCOM Header", HFILL }},
    { &hf_redcom_header_nodeid,
      { "NodeId", "redcom.header.nodeid", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},
    { &hf_redcom_header_birth,
      { "Birth", "redcom.header.birth", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_flags,
      { "Flags", "redcom.header.flags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_lacksequence,
      { "LackSequence", "redcom.header.lacksequence", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_lacktimestamp,
      { "LackTimestamp", "redcom.header.lacktimestamp", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_racksequence,
      { "RackSequence", "redcom.header.racksequence", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_racktimestamp,
      { "RackTimestamp", "redcom.header.racktimestamp", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_prio,
      { "Prio", "redcom.header.prio", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_header_state,
      { "State", "redcom.header.state", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},

    /* Redcom Info */
    { &hf_redcom_info,
      { "Info", "redcom.info", FT_NONE, BASE_NONE, NULL, 0x0, "REDCOM Info", HFILL }},
    { &hf_redcom_info_senderaix,
      { "SenderAix", "redcom.info.senderaix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_sendernid,
      { "SenderNid", "redcom.info.sendernid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_pid,
      { "Pid", "redcom.info.pid", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_receiverqix,
      { "ReceiverQix", "redcom.info.receiverqix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_receivernid,
      { "ReceiverNid", "redcom.info.receivernid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_replyqix,
      { "ReplyQix", "redcom.info.replyqix", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_replynid,
      { "ReplyNid", "redcom.info.replynid", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_btype,
      { "BType", "redcom.info.btype", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_stype,
      { "SType", "redcom.info.stype", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_rid,
      { "Rid", "redcom.info.rid", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_size,
      { "Size", "redcom.info.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_tmo,
      { "Tmo", "redcom.info.tmo", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_status,
      { "Status", "redcom.info.status", FT_INT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redcom_info_flags,
      { "Flags", "redcom.info.flags", FT_UINT32, BASE_HEX, NULL, 0x0, NULL, HFILL }},


    /* Redu MsgHeader */
    { &hf_redu_msgheader,
      { "MsgHeader", "redcom.msgheader", FT_NONE, BASE_NONE, NULL, 0x0,"Redcom Msg Header", HFILL }},
    { &hf_redu_msgheader_type,
      { "Type", "redcom.msgheader.type", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},
    { &hf_redu_msgheader_size,
      { "Size", "redcom.msgheader.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redu_msgheader_version,
      { "Version", "redcom.msgheader.version", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},

    /* Redu TableMsgHeader */
    { &hf_redu_tablemsgheader,
      { "MsgTableHeader", "redcom.tablemsgheader", FT_NONE, BASE_NONE, NULL, 0x0,"Redcom TableMsg Header", HFILL }},
    { &hf_redu_tablemsgheader_type,
      { "Type", "redcom.tablemsgheader.type", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},
    { &hf_redu_tablemsgheader_size,
      { "Size", "redcom.redu.tablemsgheader.size", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redu_tablemsgheader_attributes,
      { "Attributes", "redcom.redu.tablemsgheader.attributes", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL }},
    { &hf_redu_tablemsgheader_version,
      { "Version", "redcom.redu.tablemsgheader.version", FT_ABSOLUTE_TIME, ABSOLUTE_TIME_LOCAL, NULL, 0x0, NULL, HFILL }},

    /* Redu TableReqMsgHeader */
    { &hf_redu_tablereqmsgheader,
      { "MsgTableReqHeader", "redcom.tablereqmsgheader", FT_NONE, BASE_NONE, NULL, 0x0,"Redcom TableReqeustMsg Header", HFILL }},
    { &hf_redu_tablereqmsgheader_type,
      { "Type", "redcom.tablereqmsgheader.type", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},

    /* Redu TableVersionReqMsgHeader */
    { &hf_redu_tableversionreqmsgheader,
      { "MsgTableVersionReqHeader", "redcom.tableversionreqmsgheader", FT_NONE, BASE_NONE, NULL, 0x0,"Redcom TableVersionReqeustMsg Header", HFILL }},
    { &hf_redu_tableversionreqmsgheader_type,
      { "Type", "redcom.tableversionreqmsgheader.type", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},

    /* Redu TableVersionMsgHeader */
    { &hf_redu_tableversionmsgheader,
      { "MsgTableVersionHeader", "redcom.tableversionmsgheader", FT_NONE, BASE_NONE, NULL, 0x0,"Redcom TableVersionMsg Header", HFILL }},
    { &hf_redu_tableversionmsgheader_type,
      { "Type", "redcom.tableversionmsgheader.type", FT_UINT32, BASE_HEX, NULL, 0x0,	NULL, HFILL }},

  };

  static gint *ett[] = {
    &ett_redcom,
    &ett_redcom_header,
    &ett_redcom_header_nodeid,
    &ett_redcom_header_birth,
    &ett_redcom_header_flags,
    &ett_redcom_header_lacksequence,
    &ett_redcom_header_lacktimestamp,
    &ett_redcom_header_racksequence,
    &ett_redcom_header_racktimestamp,
    &ett_redcom_header_prio,
    &ett_redcom_header_state,
    &ett_redcom_info,
    &ett_redcom_info_senderaix,
    &ett_redcom_info_sendernid,
    &ett_redcom_info_pid,
    &ett_redcom_info_receiverqix,
    &ett_redcom_info_receivernid,
    &ett_redcom_info_replyqix,
    &ett_redcom_info_replynid,
    &ett_redcom_info_btype,
    &ett_redcom_info_stype,
    &ett_redcom_info_rid,
    &ett_redcom_info_size,
    &ett_redcom_info_tmo,
    &ett_redcom_info_status,
    &ett_redcom_info_flags,
    &ett_redu_msgheader,
    &ett_redu_msgheader_type,
    &ett_redu_msgheader_size,
    &ett_redu_msgheader_version,
    &ett_redu_tablemsgheader,
    &ett_redu_tablemsgheader_type,
    &ett_redu_tablemsgheader_size,
    &ett_redu_tablemsgheader_attributes,
    &ett_redu_tablemsgheader_version,
    &ett_redu_tablereqmsgheader,
    &ett_redu_tablereqmsgheader_type,
    &ett_redu_tableversionreqmsgheader,
    &ett_redu_tableversionreqmsgheader_type,
    &ett_redu_tableversionmsgheader,
    &ett_redu_tableversionmsgheader_type,
    
  };

  proto_register_field_array(  proto_redcom, hf, array_length(hf));
  proto_register_subtree_array( ett, array_length(ett));
  register_dissector( "redcom", dissect_redcom, proto_redcom);
  
  proto_redcom = proto_register_protocol( "Redcom Proview","Redcom","redcom");
}

void
dissect_redcom(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
  proto_item *redcom_item = NULL;
  proto_item *redcom_sub_item = NULL;
  proto_tree *redcom_tree = NULL;
  proto_tree *redcom_header_tree = NULL;
  proto_tree *redcom_info_tree = NULL;
  proto_tree *redcom_redu_cyclic_tree = NULL;
  proto_tree *redcom_redu_table_tree = NULL;
  proto_tree *redcom_redu_tablerequest_tree = NULL;
  proto_tree *redcom_redu_tableversionrequest_tree = NULL;
  proto_tree *redcom_redu_tableversion_tree = NULL;
  int is_user;
  int is_reducyclic;
  int is_redutable;
  int is_redutablerequest;
  int is_redutableversionrequest;
  int is_redutableversion;
  guint32 msg_flags;
  gint32 redu_type;
  gchar *protostrp;

  guint32 header_size = 36;
  guint32 info_size = 56;
  guint32 reducyclic_size = 16;
  guint32 redutable_size = 20;
  gint32 prio;
  gchar info[300] = "";
  guint32 local_endian = ENC_LITTLE_ENDIAN;
  static gchar protostr[][20] = {
    "REDCOM",
    "REDCOM-SYS",
    "REDCOM-Redu",
  };

  protostrp = protostr[0];
  strcpy( info, "Unknown basetype");
  
  msg_flags = tvb_get_ntohl( tvb, 8);


  is_user = 0;
  is_reducyclic = 0;
  is_redutable = 0;
  is_redutablerequest = 0;
  is_redutableversionrequest = 0;
  is_redutableversion = 0;
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
  case eEvent_set_active: {
    strcpy( info, "Set Active");
    break;
  }
  case eEvent_set_passive: {
    strcpy( info, "Set Passive");
    break;
    }
  case eEvent_set_off: {
    strcpy( info, "Set Off");
    break;
  }
  }

  is_reducyclic = 0;
  is_redutable = 0;
  is_redutablerequest = 0;
  is_redutableversionrequest = 0;
  is_redutableversion = 0;
  if ( is_user) {
    prio = tvb_get_ntohl( tvb, 28);

    if ( msg_flags & mSeg_middle && !(msg_flags & mSeg_first))
      sprintf( info, "P%02d Middle segment", prio);
    else if ( msg_flags & mSeg_last && !(msg_flags & mSeg_first))
      sprintf( info, "P%02d Last segment", prio);
    else {
      redu_type = tvb_get_letohl( tvb, header_size + info_size);

      switch ( redu_type) {
      case redu_eMsgType_Table:
	sprintf( info, "P%02d Table", prio);
	is_redutable = 1;
	break;
      case redu_eMsgType_Cyclic:
	sprintf( info, "P%02d Cyclic", prio);
	is_reducyclic = 1;
	break;
      case redu_eMsgType_TableRequest:
	sprintf( info, "P%02d TableRequest", prio);
	is_redutablerequest = 1;
	break;
      case redu_eMsgType_TableVersionRequest:
	sprintf( info, "P%02d TableVersionRequest", prio);
	is_redutableversionrequest = 1;
	break;
      case redu_eMsgType_TableVersion:
	sprintf( info, "P%02d TableVersion", prio);
	is_redutableversion = 1;
	break;
      default:
	strcpy( info, "Unknown redu_eMsgType");
      }
    }
    if ( msg_flags & mSeg_first && !(msg_flags & mSeg_last))
      strcat( info, " First segment");
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

    redcom_item = proto_tree_add_item(tree, proto_redcom, tvb, 0, -1, FALSE);
    redcom_tree = proto_item_add_subtree(redcom_item, ett_redcom);
    // redcom_header_tree = proto_item_add_subtree(redcom_item, ett_redcom);

    /* Header map */
    redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redcom_header, 
					 tvb, offset, header_size, FALSE );

    redcom_header_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);

    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_nodeid, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_birth, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_flags, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_lacksequence, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_lacktimestamp, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_racksequence, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_racktimestamp, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_prio, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    
    redcom_sub_item = proto_tree_add_item( redcom_header_tree, hf_redcom_header_state, 
					 tvb, offset, 4, ENC_BIG_ENDIAN );
    offset += 4;    


    if ( is_user) {
      /* Info map */
      redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redcom_info, 
					   tvb, offset, info_size, FALSE );

      redcom_info_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_senderaix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_sendernid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_pid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_receiverqix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_receivernid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_replyqix, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_replynid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_btype, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_stype, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_rid, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_size, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_tmo, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_status, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    
      redcom_sub_item = proto_tree_add_item( redcom_info_tree, hf_redcom_info_flags, 
					   tvb, offset, 4, ENC_BIG_ENDIAN );
      offset += 4;    

      if ( is_reducyclic) {

	/* Redu cyclic */
	redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redu_msgheader, 
					       tvb, offset, reducyclic_size, FALSE );

	redcom_redu_cyclic_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
	redcom_sub_item = proto_tree_add_item( redcom_redu_cyclic_tree, hf_redu_msgheader_type, 
					     tvb, offset, 4, local_endian );
	offset += 4;
	redcom_sub_item = proto_tree_add_item( redcom_redu_cyclic_tree, hf_redu_msgheader_size, 
					     tvb, offset, 4, local_endian );
	offset += 4;    
	redcom_sub_item = proto_tree_add_item( redcom_redu_cyclic_tree, hf_redu_msgheader_version, 
					     tvb, offset, 8, local_endian );
	offset += 8;    
      }
      else if ( is_redutable) {
	/* Redu table */
	redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redu_tablemsgheader, 
					       tvb, offset, redutable_size, FALSE );

	redcom_redu_table_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
	redcom_sub_item = proto_tree_add_item( redcom_redu_table_tree, hf_redu_tablemsgheader_type, 
					     tvb, offset, 4, local_endian );
	offset += 4;
	redcom_sub_item = proto_tree_add_item( redcom_redu_table_tree, hf_redu_tablemsgheader_size, 
					     tvb, offset, 4, local_endian );
	offset += 4;    
	redcom_sub_item = proto_tree_add_item( redcom_redu_table_tree, hf_redu_tablemsgheader_attributes, 
					     tvb, offset, 4, local_endian );
	offset += 4;    
	redcom_sub_item = proto_tree_add_item( redcom_redu_table_tree, hf_redu_tablemsgheader_version, 
					     tvb, offset, 8, local_endian );
	offset += 8;    
      }
      else if ( is_redutablerequest) {
	/* redu table request message map */
	redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redu_tablereqmsgheader, 
					       tvb, offset, redutable_size, FALSE );

	redcom_redu_tablerequest_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
	redcom_sub_item = proto_tree_add_item( redcom_redu_tablerequest_tree, hf_redu_tablereqmsgheader_type, 
					     tvb, offset, 4, local_endian );
	offset += 4;
      }
      else if ( is_redutableversionrequest) {
	/* redu table request message map */
	redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redu_tableversionreqmsgheader, 
					       tvb, offset, redutable_size, FALSE );

	redcom_redu_tableversionrequest_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
	redcom_sub_item = proto_tree_add_item( redcom_redu_tableversionrequest_tree, hf_redu_tableversionreqmsgheader_type, 
					     tvb, offset, 4, local_endian );
	offset += 4;
      }
      else if ( is_redutableversion) {
	/* redu table request message map */
	redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redu_tableversionmsgheader, 
					       tvb, offset, redutable_size, FALSE );

	redcom_redu_tableversion_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);
      
	redcom_sub_item = proto_tree_add_item( redcom_redu_tableversion_tree, hf_redu_tableversionmsgheader_type, 
					     tvb, offset, 4, local_endian );
	offset += 4;
      }
    }
#if 0
    guint32 offset = 0;
    guint32 nodeid = 0;

    redcom_item = proto_tree_add_item(tree, proto_redcom, tvb, 0, -1, FALSE);
    redcom_tree = proto_item_add_subtree(redcom_item, ett_redcom);
    redcom_header_tree = proto_item_add_subtree(redcom_item, ett_redcom);

    redcom_sub_item = proto_tree_add_item( redcom_tree, hf_redcom_header, 
					 tvb, offset, -1, FALSE );

    redcom_header_tree = proto_item_add_subtree(redcom_sub_item, ett_redcom);

    // We use tvb_memcpy to get our length value out (Host order)

    tvb_memcpy(tvb, (guint32 *)&nodeid, offset, 4);
    proto_tree_add_uint(redcom_header_tree, hf_redcom_nodeid, tvb, offset, 4, nodeid);

    // We increment the offset to get past the 4 bytes indicating length
    offset+=4;

    // Here we submit the type parameter to the tree. 
    proto_tree_add_item(redcom_header_tree, hf_redcom_type, tvb, offset, 1, FALSE);
    type = tvb_get_guint8( tvb, offset ); // Get our type byte

    offset+=1;
    //If the type is TEXT, we add the text to the tree. 

    if( type == 0 ) {
      proto_tree_add_item( redcom_tree, hf_redcom_text, tvb, offset, length-1, FALSE );
    }
#endif
  }
  
#if 0
  col_set_str(pinfo->cinfo, COL_PROTOCOL, "Redcom");
  /* Clear out stuff in the info column */
  col_clear(pinfo->cinfo, COL_INFO);
#endif
}


void
proto_register_handoff_redcom(void)
{
  static gboolean initialized = FALSE;
  int port;

  if ( !initialized) {
    port = redu_cPort;

    printf( "REDCOM Port %d\n", port);
    
    data_handle = find_dissector("data");
    redcom_handle = create_dissector_handle(dissect_redcom, proto_redcom);
    dissector_add_uint( "udp.port", port, redcom_handle);
  }
}
