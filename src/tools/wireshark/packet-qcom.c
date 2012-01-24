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
    { &hf_qcom_header,
      { "Header", "qcom.header", FT_NONE, BASE_NONE, NULL, 0x0,
	"QCOM Header", HFILL }},
    { &hf_qcom_header_nodeid,
      { "NodeId", "qcom.header.nodeid", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_birth,
      { "Birth", "qcom.header.birth", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_flags,
      { "Flags", "qcom.header.flags", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_lacksequence,
      { "LackSequence", "qcom.header.lacksequence", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_lacktimestamp,
      { "LackTimestamp", "qcom.header.lacktimestamp", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_racksequence,
      { "RackSequence", "qcom.header.racksequence", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_header_racktimestamp,
      { "RackTimestamp", "qcom.header.racktimestamp", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info,
      { "Info", "qcom.info", FT_NONE, BASE_NONE, NULL, 0x0,
	"QCOM Info", HFILL }},
    { &hf_qcom_info_senderaix,
      { "SenderAix", "qcom.info.senderaix", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_sendernid,
      { "SenderNid", "qcom.info.sendernid", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_pid,
      { "Pid", "qcom.info.pid", FT_UINT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_receiverqix,
      { "ReceiverQix", "qcom.info.receiverqix", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_receivernid,
      { "ReceiverNid", "qcom.info.receivernid", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_replyqix,
      { "ReplyQix", "qcom.info.replyqix", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_replynid,
      { "ReplyNid", "qcom.info.replynid", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_btype,
      { "BType", "qcom.info.btype", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_stype,
      { "SType", "qcom.info.stype", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_rid,
      { "Rid", "qcom.info.rid", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_size,
      { "Size", "qcom.info.size", FT_UINT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_tmo,
      { "Tmo", "qcom.info.tmo", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_status,
      { "Status", "qcom.info.status", FT_INT32, BASE_DEC, NULL, 0x0,
	NULL, HFILL }},
    { &hf_qcom_info_flags,
      { "Flags", "qcom.info.flags", FT_UINT32, BASE_HEX, NULL, 0x0,
	NULL, HFILL }}
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
    &ett_qcom_info_flags
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
  int is_user;
  int is_neth;
  int is_mh;
  guint32 receiver_qix;
  guint32 reply_qix;
  guint32 btype;
  guint32 stype;
  guint32 msg_flags;
  guint32 msg_size;
  guint32 mh_type;
  static gchar protostr[] = "QCOM";
  static gchar infostr[] = "";
  gchar *protostrp;
  gchar *infostrp;

  guint32 header_size = 28;
  guint32 info_size = 56;
  guint32 qcommsg_size = 96;                                          

  protostrp = protostr;
  infostrp = infostr;
  
  msg_flags = tvb_get_ntohl( tvb, 8);

  is_user = 0;
  switch ( msg_flags >> 24) {
  case 1: {
    static gchar infostr[] = "Connect";
    infostrp = infostr;
    break;
  }
  case 2: {
    static gchar infostr[] = "Down";
    infostrp = infostr;
    break;
  }
  case 3: {
    static gchar infostr[] = "User";
    infostrp = infostr;
    is_user = 1;
    break;
  }
  case 4: {
    static gchar infostr[] = "Ack";
    infostrp = infostr;
    break;
  }
  }

  is_neth = 0;
  is_mh = 0;
  if ( is_user) {
    receiver_qix = tvb_get_ntohl( tvb, header_size + 12);
    reply_qix = tvb_get_ntohl( tvb, header_size + 20);
    btype = tvb_get_ntohl( tvb, header_size + 28);
    stype = tvb_get_ntohl( tvb, header_size + 32);
    msg_size = tvb_get_ntohl( tvb, header_size + 40);
    if ( receiver_qix == 110 || reply_qix == 110)
      is_neth = 1;
    else 
      is_neth = 0;

    /* Proto and info string have to be statically allocated */
    switch ( btype) {
    case 1: { 
      /* System */
      static gchar protostr[] = "QCOM SYS";
      protostrp = protostr;

      break;
    }
    case 2: { 
      /* Qcom */
      static gchar protostr[] = "QCOM";
      protostrp = protostr;

      switch ( stype) {
      case 1: {
	static gchar infostr[] = "Link connect";
	infostrp = infostr;
	break;
      }
      case 2: {
	static gchar infostr[] = "Link disconnect";
	infostrp = infostr;
	break;
      }
      case 3: {
	static gchar infostr[] = "Link active";
	infostrp = infostr;
	break;
      }
      case 4: {
	static gchar infostr[] = "Link stalled";
	infostrp = infostr;
	break;
      }
      case 5: {
	static gchar infostr[] = "Appl connect";
	infostrp = infostr;
	break;
      }
      case 6: { 
	static gchar infostr[] = "Appl disconnect";
	infostrp = infostr;
	break;
      }
      default: {
	static gchar infostr[] = "Unknown stype";
	infostrp = infostr;
      }
      }

      break;
    }
    case 3: {
      /* Event */
      static gchar protostr[] = "QCOM EV";
      protostrp = protostr;

      switch ( stype) {
      case 1<<31 | 0: { 
	static gchar infostr[] = "loopBack";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 100: { 
	static gchar infostr[] = "netEvent";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 101: { 
	static gchar infostr[] = "applEvent";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 102: { 
	static gchar infostr[] = "mhAllHandlers";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 103: { 
	static gchar infostr[] = "mhAllOutunits";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 104: { 
	static gchar infostr[] = "hdServer";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 105: { 
	static gchar infostr[] = "hdClient";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 106: { 
	static gchar infostr[] = "nacp";
	infostrp = infostr;
	break;
      }
      case 1<<31 | 107: { 
	static gchar infostr[] = "ini";
	infostrp = infostr;
	break;
      }
      default: { 
	static gchar infostr[] = "Unknown stype";
	infostrp = infostr;
      }
      }
      break;
    }
    case 200: {
      /* Neth */
      static gchar protostr[] = "QCOM NET";
      protostrp = protostr;

      switch ( stype) {
      case 0: { 
	static gchar infostr[] = "error";
	infostrp = infostr;
	break;
      }
      case 1: { 
	static gchar infostr[] = "id";
	infostrp = infostr;
	break;
      }
      case 2: { 
	static gchar infostr[] = "idAck";
	infostrp = infostr;
	break;
      }
      case 3: { 
	static gchar infostr[] = "idAck2";
	infostrp = infostr;
	break;
      }
      case 4: { 
	static gchar infostr[] = "volumes";
	infostrp = infostr;
	break;
      }
      case 5: { 
	static gchar infostr[] = "volumesR";
	infostrp = infostr;
	break;
      }
      case 6: { 
	static gchar infostr[] = "subAdd";
	infostrp = infostr;
	break;
      }
      case 7: { 
	static gchar infostr[] = "subRemove";
	infostrp = infostr;
	break;
      }
      case 8: { 
	static gchar infostr[] = "subData";
	infostrp = infostr;
	break;
      }
      case 9: { 
	static gchar infostr[] = "sanAdd";
	infostrp = infostr;
	break;
      }
      case 10: { 
	static gchar infostr[] = "sanRemove";
	infostrp = infostr;
	break;
      }
      case 11: { 
	static gchar infostr[] = "sanUpdate";
	infostrp = infostr;
	break;
      }
      case 12: { 
	static gchar infostr[] = "nameToObjid";
	infostrp = infostr;
	break;
      }
      case 13: { 
	static gchar infostr[] = "oidToObject";
	infostrp = infostr;
	break;
      }
      case 14: { 
	static gchar infostr[] = "objectR";
	infostrp = infostr;
	break;
      }
      case 15: { 
	static gchar infostr[] = "getObjectInfo";
	infostrp = infostr;
	break;
      }
      case 16: { 
	static gchar infostr[] = "getObjectInfoR";
	infostrp = infostr;
	break;
      }
      case 17: { 
	static gchar infostr[] = "setObjectInfo";
	infostrp = infostr;
	break;
      }
      case 18: { 
	static gchar infostr[] = "setObjectInfoR";
	infostrp = infostr;
	break;
      }
      case 19: { 
	static gchar infostr[] = "flush";
	infostrp = infostr;
	break;
      }
      case 20: { 
	static gchar infostr[] = "createObject";
	infostrp = infostr;
	break;
      }
      case 21: { 
	static gchar infostr[] = "deleteObject";
	infostrp = infostr;
	break;
      }
      case 22: { 
	static gchar infostr[] = "moveObject";
	infostrp = infostr;
	break;
      }
      case 23: { 
	static gchar infostr[] = "renameObject";
	infostrp = infostr;
	break;
      }
      case 24: { 
	static gchar infostr[] = "nodeUp";
	infostrp = infostr;
	break;
      }
      case 25: { 
	static gchar infostr[] = "nodeDown";
	infostrp = infostr;
	break;
      }
      case 26: { 
	static gchar infostr[] = "getCclass";
	infostrp = infostr;
	break;
      }
      case 27: { 
	static gchar infostr[] = "getCclassR";
	infostrp = infostr;
	break;
      }
      case 28: { 
	static gchar infostr[] = "getGclass";
	infostrp = infostr;
	break;
      }
      case 29: { 
	static gchar infostr[] = "getGclassR";
	infostrp = infostr;
	break;
      }
      case 30: { 
	static gchar infostr[] = "serverConnect";
	infostrp = infostr;
	break;
      }
      case 31: { 
	static gchar infostr[] = "fileList";
	infostrp = infostr;
	break;
      }
      case 32: { 
	static gchar infostr[] = "fileListR";
	infostrp = infostr;
	break;
      }
      case 36: { 
	static gchar infostr[] = "volumes7";
	infostrp = infostr;
	break;
      }
      default: { 
	static gchar infostr[] = "Unknown stype";
	infostrp = infostr;
      }
      }
      break;
    }
    case 201: {
      /* Mh */
      static gchar protostr[] = "QCOM MH";
      protostrp = protostr;

      if ( msg_size >= 28) {
	mh_type = tvb_get_ntohl( tvb, qcommsg_size + 24);
#if 0
	int i;
	for ( i = 0; i < 25; i++) {
	  qcommsg_size = 50 + 4 * i;
	  mh_type = tvb_get_ntohl( tvb, qcommsg_size);
	  printf( "size: %d %d\n", qcommsg_size, mh_type);
	}
#endif

	switch ( mh_type) {
	case 1: { 
	  static gchar infostr[] = "ApplConnect";
	  infostrp = infostr;
	  break;
	}
	case 2: { 
	  static gchar infostr[] = "ApplDisconnect";
	  infostrp = infostr;
	  break;
	}
	case 3: { 
	  static gchar infostr[] = "ApplGetMsgInfo";
	  infostrp = infostr;
	  break;
	}
	case 4: { 
	  static gchar infostr[] = "ApplMessage";
	  infostrp = infostr;
	  break;
	}
	case 5: { 
	  static gchar infostr[] = "ApplReply";
	  infostrp = infostr;
	  break;
	}
	case 6: { 
	  static gchar infostr[] = "ApplReturn";
	  infostrp = infostr;
	  break;
	}
	case 8: { 
	  static gchar infostr[] = "Event";
	  infostrp = infostr;
	  break;
	}
	case 10: { 
	  static gchar infostr[] = "HandlerDisconnect";
	  infostrp = infostr;
	  break;
	}
	case 11: { 
	  static gchar infostr[] = "HandlerHello";
	  infostrp = infostr;
	  break;
	}
	case 12: { 
	  static gchar infostr[] = "HandlerSync";
	  infostrp = infostr;
	  break;
	}
	case 15: { 
	  static gchar infostr[] = "OutunitAck";
	  infostrp = infostr;
	  break;
	}
	case 16: { 
	  static gchar infostr[] = "OutunitBlock";
	  infostrp = infostr;
	  break;
	}
	case 18: { 
	  static gchar infostr[] = "OutunitDisconnect";
	  infostrp = infostr;
	  break;
	}
	case 19: { 
	  static gchar infostr[] = "OutunitHello";
	  infostrp = infostr;
	  break;
	}
	case 20: { 
	  static gchar infostr[] = "OutunitInfo";
	  infostrp = infostr;
	  break;
	}
	case 21: { 
	  static gchar infostr[] = "OutunitSync";
	  infostrp = infostr;
	  break;
	}
	case 24: { 
	  static gchar infostr[] = "ProcDown";
	  infostrp = infostr;
	  break;
	}
	case 25: { 
	  static gchar infostr[] = "StopScanSup";
	  infostrp = infostr;
	  break;
	}
	case 26: { 
	  static gchar infostr[] = "StartScanSup";
	  infostrp = infostr;
	  break;
	}
	case 27: { 
	  static gchar infostr[] = "Sync";
	  infostrp = infostr;
	  break;
	}
	default: { 
	  static gchar infostr[] = "Unknown mh type";
	  infostrp = infostr;
	}
	}
      }
      else {
	  static gchar infostr[] = "Unknown stype";
	  infostrp = infostr;
      }
      break;
    }
    default: {
      static gchar protostr[] = "QCOM";
      static gchar infostr[] = "Unknown btype";
      protostrp = protostr;
      infostrp = infostr;
    }
    }
  }

  if (check_col(pinfo->cinfo, COL_PROTOCOL)) {
    col_set_str(pinfo->cinfo, COL_PROTOCOL, protostrp);
  }

  /* Clear out stuff in the info column */
  if(check_col(pinfo->cinfo,COL_INFO)){
    col_set_str(pinfo->cinfo, COL_INFO, infostrp);

    // col_clear(pinfo->cinfo,COL_INFO);
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
