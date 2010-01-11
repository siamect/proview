/* 
 * Proview   $Id: rt_sevhistmon.cpp,v 1.6 2008-11-24 15:21:33 claes Exp $
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


#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_error.h"
#include "rt_gdh.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"
#include "rt_errh.h"
#include "rt_sev_net.h"
#include "rt_sevhistmon.h"
#include "rt_sev_msg.h"
#include "rt_pwr_msg.h"
#include <iostream>

#define sevclient_cQix 121
using namespace std;
int rt_sevhistmon::init()
{
  pwr_tStatus sts;
  qcom_sQid qid;
  qcom_sQid     qini;
  qcom_sNode    node;
  pwr_tNid    nid;
  pwr_tOid    conf_oid;
  pwr_tOName oname;

  errh_Init("pwr_sevhistmon", errh_eNAnix);
  // errh_SetStatus( PWR__SRVSTARTUP);

  sts = gdh_Init( "rt_sevhistmon");
  if ( EVEN(sts)) throw co_error(sts);

  // Get the config object
  sts = gdh_GetClassList( pwr_cClass_SevHistMonitor, &conf_oid);
  if ( EVEN(sts)) {
    errh_CErrLog( PWR__SRVNOTCONF, 0);
    exit(0);
  }

  m_sts = gdh_ObjidToName( conf_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
  if ( EVEN(m_sts)) throw co_error(m_sts);

  m_sts = gdh_RefObjectInfo( oname, (void **)&m_confp, &m_conf_refid, sizeof(*m_confp));
  if ( EVEN(m_sts)) throw co_error(m_sts);

  m_confp->Status = m_server_status = PWR__SRVSTARTUP;
  m_scantime = m_confp->ScanTime;
  if ( m_scantime < 0.02)
    m_scantime = 0.02;

  // Create a queue to server
  qcom_sQattr attr;

  qid.qix = sev_eProcSevClient;
  qid.nid = 0;
  attr.type = qcom_eQtype_private;
  attr.quota = 100;

  if (!qcom_CreateQ(&sts, &qid, &attr, "SevClient")) {
    if ( sts == QCOM__QALLREXIST) {
      if ( !qcom_AttachQ(&sts, &qid)) {
        if ( !qcom_DeleteQ(&sts, &qid))
          throw co_error(sts);
        if ( !qcom_CreateQ(&sts, &qid, &attr, "SevClient"))
          throw co_error(sts);
      }
    }
    else
      throw co_error( sts);
  }

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &qid, &qini))
    throw co_error(sts);

  // Get all qcom nodes
  qcom_MyNode( &m_sts, &node);
  if ( EVEN(m_sts)) throw co_error(m_sts);

  sev_node myn;
  myn.nid = node.nid;
  strncpy( myn.name, node.name, sizeof(myn.name));
  m_nodes.push_back( myn);

  for (nid = qcom_cNNid; qcom_NextNode(&sts, &node, nid); nid = node.nid) {
    sev_node n;

    n.nid = node.nid;
    strncpy( n.name, node.name, sizeof(n.name));
    m_nodes.push_back( n);
  }

  while (EVEN(gdh_NethandlerRunning()))
    sleep(1);

  return init_objects();
}

int rt_sevhistmon::init_objects()
{
  pwr_tStatus sts;
  pwr_tOid hs_oid;
  pwr_tAttrRef h_aref;
  pwr_tAttrRef uaref;
  pwr_tTid a_tid;
  unsigned int a_size, a_offset, a_dim;
  pwr_tAName hname;
  pwr_tOName oname;

  // Get all SevHist and SevHistThread objects
  int thread_cnt = 0;
  for ( sts = gdh_GetClassList( pwr_cClass_SevHistThread, &hs_oid);
      ODD(sts);
      sts = gdh_GetNextObject( hs_oid, &hs_oid)) {
    sev_sevhistthread hs;

    m_sts = gdh_ObjidToName( hs_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    m_sts = gdh_RefObjectInfo( oname, (void **)&hs.threadp, &hs.refid, sizeof(*hs.threadp));
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs.oid = hs_oid;
    hs.scantime = hs.threadp->ScanTime;
    strncpy( hs.nodename, hs.threadp->ServerNode, sizeof(hs.nodename));
    hs.size = 0;

    bool found = false;
    for ( unsigned int i = 0; i < m_nodes.size(); i++) {
      if ( cdh_NoCaseStrcmp( hs.nodename, m_nodes[i].name) == 0) {
        hs.nid = m_nodes[i].nid;
        found = true;
        break;
      }
    }
    m_confp->ThreadObjects[thread_cnt++] = hs_oid;
    if ( !found) {
      pwr_tOName oname;

      m_sts = gdh_ObjidToName( hs_oid, oname, sizeof(oname), cdh_mName_volumeStrict);
      if ( EVEN(m_sts)) throw co_error(m_sts);

      errh_Error( "Unknown nodename, %s", oname);
      hs.threadp->Status = SEV__UNKNOWNNODE;
      hs.configerror = 1;
    }
    else
      hs.threadp->Status = SEV__INIT;
    m_hs.push_back( hs);
  }

  for ( int i = thread_cnt; i < int(sizeof(m_confp->ThreadObjects)/sizeof(m_confp->ThreadObjects[0])); i++)
    m_confp->ThreadObjects[i] = pwr_cNOid;

  for ( sts = gdh_GetClassListAttrRef( pwr_cClass_SevHist, &h_aref);
      ODD(sts);
      sts = gdh_GetNextAttrRef( pwr_cClass_SevHist, &h_aref, &h_aref)) {
    sev_sevhist h;
    pwr_sClass_SevHist *h_p;
    int hs_idx;

    m_sts = gdh_AttrRefToPointer( &h_aref, (void **)&h_p);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    h.aref = h_p->Attribute;

    m_sts = gdh_AttrrefToName( &h_aref, hname, sizeof(hname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs_idx = -1;
    for ( int i = 0; i < (int) m_hs.size(); i++) {
      if ( cdh_ObjidIsEqual( h_p->ThreadObject, m_hs[i].oid)) {
        hs_idx = i;
        break;
      }
    }
    if ( hs_idx == -1) {
      errh_Error( "Invalid HistServerObject %s", hname);
      continue;
    }

    m_sts = gdh_GetAttributeCharAttrref( &h_p->Attribute, &a_tid, &a_size, &a_offset, &a_dim);
    if ( EVEN(m_sts)) {
      errh_Error( "Invalid SevHist Attribute %s", hname);
      continue;
    }

    h.sevid.nid = m_nodes[0].nid;
    h.sevid.rix = m_next_rix++;
    h.storagetime = h_p->StorageTime;
    h.deadband = h_p->DeadBand;
    h.options = h_p->Options;
    strncpy( h.description, h_p->Description, sizeof(h.description));

    // Get unit from attribute object
    sts = gdh_ArefANameToAref( &h_p->Attribute, "Unit", &uaref);
    if ( ODD(sts)) {
      sts = gdh_GetObjectInfoAttrref( &uaref, &h.unit, sizeof(h.unit));
      if ( EVEN(sts))
        strcpy( h.unit, "");
    }
    else
      strcpy( h.unit, "");

    h.type = (pwr_eType) a_tid;
    h.size = a_size;
    h.scantime = m_hs[hs_idx].scantime;

    if( !correct_histtype(h.type) ) {
      errh_Error( "Invalid SevHist Attribute type %s", hname);
      continue;
    }

    m_sts = gdh_AttrrefToName( &h_p->Attribute, h.aname, sizeof(h.aname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    m_sts = gdh_RefObjectInfo( h.aname, &h.datap, &h.refid, h.size);
    if ( EVEN(m_sts)) {
      errh_Error( "Unable to link to Attribute %s", h.aname);
      continue;
    }
    m_hs[hs_idx].size += h.size;
    m_hs[hs_idx].sevhistlist.push_back(h);
    m_hs[hs_idx].threadp->NoOfItems++;
  }

  init_sevhistobjects();

  set_status();

  return 1;
}

int rt_sevhistmon::init_sevhistobjects()
{
  pwr_tStatus sts;
  pwr_tAttrRef h_aref;
  pwr_tTid a_tid;
  unsigned int a_size, a_offset, a_dim;
  pwr_tAName hname;

  for ( sts = gdh_GetClassListAttrRef( pwr_cClass_SevHistObject, &h_aref);
      ODD(sts);
      sts = gdh_GetNextAttrRef( pwr_cClass_SevHistObject, &h_aref, &h_aref)) {
    sev_sevhistobject h;
    pwr_sClass_SevHistObject *h_p;
    int hs_idx;

    m_sts = gdh_AttrRefToPointer( &h_aref, (void **)&h_p);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    h.aref = h_p->Object;

    m_sts = gdh_AttrrefToName( &h_aref, hname, sizeof(hname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);

    hs_idx = -1;
    for ( int i = 0; i < (int) m_hs.size(); i++) {
      if ( cdh_ObjidIsEqual( h_p->ThreadObject, m_hs[i].oid)) {
        hs_idx = i;
        break;
      }
    }
    if ( hs_idx == -1) {
      errh_Error( "Invalid HistServerObject %s", hname);
      continue;
    }

    m_sts = gdh_GetAttributeCharAttrref( &h_p->Object, &a_tid, &a_size, &a_offset, &a_dim);
    if ( EVEN(m_sts)) {
      errh_Error( "Invalid SevHistObject Attribute %s", hname);
      continue;
    }

    h.storagetime = h_p->StorageTime;
    h.deadband = h_p->DeadBand;
    h.options = h_p->Options;
    strncpy( h.description, h_p->Description, sizeof(h.description));

    h.scantime = m_hs[hs_idx].scantime;

    h.sevid.nid = m_nodes[0].nid;
    h.sevid.rix = m_next_rix++;

    //Time to fetch all attributes for the object and put them into a list
    m_sts = gdh_AttrrefToName( &h_p->Object, hname, sizeof(hname), cdh_mName_volumeStrict);
    if ( EVEN(m_sts)) throw co_error(m_sts);
    strcpy(h.aname, hname);
    get_sevhistobjectattributes(hname, &h.sevhistobjectattrlist, hs_idx, true);
    h.datasize = 0;
    for( size_t k = 0; k < h.sevhistobjectattrlist.size(); k++ )
       h.datasize += h.sevhistobjectattrlist[k].size;

    //Check if this object have a chance to be stored in one table
    if( h.datasize >= 65536 ) {
      errh_Warning( "SevHistObject Attribute %s is to big(maxsize 65536), ignored", hname);
      continue;
    }
    if( h.sevhistobjectattrlist.size() > 200 ) {
      errh_Warning( "SevHistObject Attribute %s has to many attributes(max 200), ignored", hname);
      continue;
    }

    m_hs[hs_idx].size += h.datasize;
    m_hs[hs_idx].threadp->NoOfItems += h.sevhistobjectattrlist.size();


    m_hs[hs_idx].sevhistobjectlist.push_back(h);

    //for( size_t k = 0; k < m_hs[hs_idx].sevhistobjectlist.size(); k++ ) {
    //  std::cout << m_hs[hs_idx].sevhistobjectlist[k].aname << endl;
    //  for( size_t l = 0; l < m_hs[hs_idx].sevhistobjectlist[k].sevhistobjectattrlist.size(); l++ ) {
    //    std::cout << m_hs[hs_idx].sevhistobjectlist[k].sevhistobjectattrlist[l].aname << endl;
    //  }
    //}

  }
  return 1;
}

void rt_sevhistmon::insert_sevhistobjectattr(pwr_sAttrRef    *aref,
                                             pwr_tAName      objectname, // Name of object,
                                             int hs_idx,
                                             vector<sev_sevhistobjectattr>    *listP)
{
  pwr_tTid a_tid;
  unsigned int a_size, a_offset, a_dim;
  sev_sevhistobjectattr oattr;

  m_sts = gdh_GetAttributeCharAttrref( aref, &a_tid, &a_size, &a_offset, &a_dim);
  if ( EVEN(m_sts)) {
    errh_Error( "Invalid SevHist Attribute %s", objectname);
    return;
  }
  

  oattr.type = (pwr_eType) a_tid;
  oattr.size = a_size;
  oattr.unit[0] = '\0';

  if( !correct_histtype(oattr.type) ) {
    errh_Error( "Invalid SevHist Attribute type %s, ignored\n", objectname);
    return;
  }
  //We ignore description fields
  string str = objectname;
  if( str.find(".Description") != string::npos ) {
    errh_Info( "Ignoring SevHist Attribute %s\n", objectname);
    return;
  }

  strncpy(oattr.aname, objectname, sizeof(pwr_tAName));

  m_sts = gdh_RefObjectInfo( oattr.aname, &oattr.datap, &oattr.refid, oattr.size);
  if ( EVEN(m_sts)) {
    errh_Error( "Unable to link to Attribute %s", oattr.aname);
    return;
  }
  listP->push_back(oattr);
}

int rt_sevhistmon::get_sevhistobjectattributes (
                                               pwr_tAName      objectname, // Name of object
                                               vector<sev_sevhistobjectattr>    *listP,   // Pointer to list
                                               int hs_idx,
                                               pwr_tBoolean    first
                                               )
{

  pwr_tAName    attrname;
  pwr_sAttrRef  attrref;
  pwr_tStatus   sts;
  pwr_tTid      tid;
  unsigned int i;
  int     rows, j, elements;
  char    idx[20];
  pwr_tUInt32   size, offs, elem;
  pwr_tBoolean  tgtdynamic = 0;
  pwr_tObjid    volobject;
  pwr_tClassId    volclass;

  sts = gdh_NameToAttrref(pwr_cNObjid, objectname, &attrref);
  if ( EVEN(sts)) return sts;

  if (first) {
    volobject.vid = attrref.Objid.vid;
    volobject.oix = pwr_cNObjectIx;
    if (ODD(gdh_GetObjectClass(volobject, &volclass)))
      tgtdynamic = volclass == pwr_eClass_DynamicVolume;

    if (tgtdynamic) {
      if (attrref.Flags.b.Indirect)
        return 2;

      attrref.Offset = 0;
      attrref.Flags.b.Indirect = 0;
      attrref.Flags.b.Object = 1;

      sts = gdh_GetObjectSize(attrref.Objid, (pwr_tUInt32 *)&attrref.Size);
      if ( EVEN(sts)) return sts;

      // Insert list element
      insert_sevhistobjectattr(&attrref, objectname, hs_idx, listP);
      return sts;
    }
  }

  sts = gdh_GetAttrRefTid (&attrref, &tid);
  if ( EVEN(sts)) return sts;

  if (cdh_tidIsCid(tid)) {

    sts = gdh_GetAttributeCharacteristics( objectname, NULL, &size, &offs, &elem);
    if ( EVEN(sts)) return sts;

    if (elem > 1) {
      for (i = 0; i < elem; i++) {
        strcpy(attrname, objectname);
        sprintf(idx, "[%d]", i);
        strcat(attrname, idx);

        // call again ...
        sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
      }

    }
    else {
      gdh_sAttrDef  *bd;
      sts = gdh_GetObjectBodyDef( tid, &bd, &rows, attrref.Objid);
      if ( EVEN(sts)) return sts;

      for ( int i = 0; (int)i < rows; i++) {

        if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
             bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
          continue;

        if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY)
          elements = bd[i].attr->Param.Info.Elements;
        else
          elements = 1;

        if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS) {
          if ( elements == 1) {
            strcpy(attrname, objectname);
            strcat(attrname, ".");
            strcat(attrname, bd[i].attrName);

            // call again ...
            sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
          }
          else {
            for ( j = 0; j < elements; j++) {
              strcpy(attrname, objectname);
              strcat(attrname, ".");
              strcat(attrname, bd[i].attrName);
              sprintf( idx, "[%d]", j);
              strcat(attrname, idx);

              // call again ...
              sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
            }
          }
        }
        else {
          if (elements > 1) {
            for ( j = 0; j < elements; j++) {
              strcpy(attrname, objectname);
              strcat(attrname, ".");
              strcat(attrname, bd[i].attrName);
              sprintf( idx, "[%d]", j);
              strcat(attrname, idx);

              // call again ...
              sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
            }
          }
          else {
            strcpy(attrname, objectname);
            strcat(attrname, ".");
            strcat(attrname, bd[i].attrName);

            // call again ...
            sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
          }
        }
      }
      free( (char *)bd);
    }
  }
  else {

    sts = gdh_GetAttributeCharacteristics( objectname, &tid, &size, &offs, &elem);
    if ( EVEN(sts)) return sts;

    if (elem > 1) {
      for (i = 0; i < elem; i++) {
        strcpy(attrname, objectname);
        sprintf(idx, "[%d]", i);
        strcat(attrname, idx);

        // call again ...
        sts = get_sevhistobjectattributes (attrname, listP, hs_idx, 0);
      }
    }
    else {
      // Insert list element
      insert_sevhistobjectattr(&attrref, objectname, hs_idx, listP);
    }
  }

  return sts;
}

bool rt_sevhistmon::correct_histtype(const pwr_eType type)
{
  switch ( type) {
    case pwr_eType_Boolean:
    case pwr_eType_Int64:
    case pwr_eType_Int32:
    case pwr_eType_Int16:
    case pwr_eType_Int8:
    case pwr_eType_UInt64:
    case pwr_eType_UInt32:
    case pwr_eType_UInt16:
    case pwr_eType_UInt8:
    case pwr_eType_Float32:
    case pwr_eType_Float64:
    case pwr_eType_String:
    case pwr_eType_Time:
      return true;
      break;
    default:
      return false;
  }
}

int rt_sevhistmon::close_objects()
{
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++)
      gdh_UnrefObjectInfo( m_hs[i].sevhistlist[j].refid);
    gdh_UnrefObjectInfo( m_hs[i].refid);
  }
  m_hs.clear();

  return 1;
}


int rt_sevhistmon::close()
{
  close_objects();
  m_confp->Status = PWR__SRVTERM;
  gdh_UnrefObjectInfo( m_conf_refid);
  return 1;
}

int rt_sevhistmon::send_data()
{
  qcom_sQid     tgt;
  qcom_sPut put;
  pwr_tStatus sts, lsts;
  int msize;
  sev_sMsgHistDataStore *msg;
  sev_sHistData *dp;
  int   stime;
  pwr_tTime current_time;
  pwr_tStatus   conf_sts = SEV__SUCCESS;

  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;

    stime = int(m_hs[i].scantime / m_scantime + 0.5);
    if ( !stime || m_loopcnt % stime != 0)
      continue;

    m_hs[i].threadp->ScanCount++;

    msize = m_hs[i].sevhistlist.size() * (sizeof(*dp) - sizeof(dp->data)) + m_hs[i].size; 
    msize += sizeof(*msg) - sizeof(msg->Data);

    unsigned int histobjectsize = (m_hs[i].sevhistobjectlist.size() * (sizeof(*dp) - sizeof(dp->data)));
    msize += histobjectsize;

    msg = (sev_sMsgHistDataStore *) qcom_Alloc(&lsts, msize);

    put.size = msize;
    put.data = msg;

    msg->Type = sev_eMsgType_HistDataStore;
    time_GetTime( &current_time);
    msg->Time = net_TimeToNetTime( &current_time);

    dp = (sev_sHistData *) &msg->Data;
    for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++) {
      dp->sevid = m_hs[i].sevhistlist[j].sevid;
      dp->type = m_hs[i].sevhistlist[j].type;
      dp->size = m_hs[i].sevhistlist[j].size;
      memcpy( &dp->data, m_hs[i].sevhistlist[j].datap, dp->size);
      dp = (sev_sHistData *)((char *)dp + sizeof( *dp) - sizeof(dp->data) +  dp->size);
    }

    void *dpp;
    for ( unsigned int j = 0; j < m_hs[i].sevhistobjectlist.size(); j++) {
      dp->sevid = m_hs[i].sevhistobjectlist[j].sevid;
      dp->size = m_hs[i].sevhistobjectlist[j].datasize;
      dpp = &(dp->data);
      for ( unsigned int k = 0; k < m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist.size(); k++) {
        //dp->type = m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].type;
        //printf("sevhistobj[%d].attrlist[%d].aname: %s size:%d\n", j, k, m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].aname, m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].size);
        //if( m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].type == pwr_eType_String ) {
        //  printf("text:%s\n", (char*)m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].datap);
        //}
        memcpy( dpp, m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].datap, m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].size);
        dpp = (sev_sHistData *)((char *)dpp + m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[k].size);
      }
      dp = (sev_sHistData *)((char *)dp + sizeof( *dp) - sizeof(dp->data) +  dp->size);
    }

    tgt.nid = m_hs[i].nid;
    tgt.qix = sev_eProcSevServer;

    put.reply.nid = m_nodes[0].nid;
    put.reply.qix = sev_eProcSevClient;
    put.type.b = (qcom_eBtype) sev_cMsgClass;
    put.type.s = (qcom_eStype) sev_eMsgType_HistDataStore;
    put.msg_id = m_msg_id++;

    if ( !qcom_Put( &sts, &tgt, &put)) {
      m_hs[i].threadp->ErrorCount++;
      m_hs[i].threadp->Status = sts;
      conf_sts = sts;
      qcom_Free( &sts, put.data);
      continue;
    }
    m_hs[i].threadp->SendCount++;
    m_hs[i].threadp->Status = sts;
  }

  set_status();
  return 1;
}

void rt_sevhistmon::set_status()
{
  pwr_tStatus sts = m_server_status;

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    if ( m_nodes[i].connected && m_nodes[i].status != 0 &&
         errh_Severity( m_nodes[i].status) > errh_Severity( sts))
      sts = m_nodes[i].status;
  }

  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( EVEN( m_hs[i].threadp->Status) &&
         errh_Severity( m_hs [i].threadp->Status) > errh_Severity( sts)) {
      sts = m_hs[i].threadp->Status;
    }
  }
  m_confp->Status = sts;
}

int rt_sevhistmon::retry_connect()
{
  pwr_tStatus sts;  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {

    if ( m_nodes[i].is_server && !m_nodes[i].connected) {
      m_nodes[i].ctime += m_scantime;
      if ( m_nodes[i].ctime > 60) {
        m_nodes[i].ctime = 0;

        send_connect( m_nodes[i].nid, &sts);
      }
    }
  }
  return 1;
}

int rt_sevhistmon::connect()
{
  pwr_tStatus sts;  

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {

    // Check if this node should be connected
    bool found = false;
    for ( unsigned int j = 0; j < m_hs.size(); j++) {
      if ( m_hs[j].configerror)
        continue;
      if ( cdh_NoCaseStrcmp( m_nodes[i].name, m_hs[j].nodename) == 0) {
        found = true;
        break;
      }
    }
    if ( !found)
      continue;

    m_nodes[i].is_server = 1;

    send_connect( m_nodes[i].nid, &sts);
  }
  return 1;
}

bool rt_sevhistmon::send_connect( pwr_tNid nid, pwr_tStatus *sts)
{
  sev_sMsgAny   *msg;
  qcom_sQid     tgt;
  qcom_sPut put;
  pwr_tStatus   lsts;  

  tgt.nid = nid;
  tgt.qix = sev_eProcSevServer;

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevClient;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_NodeUp;
  put.msg_id = m_msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc(&lsts, put.size);
  put.data = msg;

  msg->Type = sev_eMsgType_NodeUp;

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
  }

  return ODD(*sts);
}

bool rt_sevhistmon::send_server_status_request( pwr_tStatus *sts)
{
  int   stime;
  float   scantime = 30;
  pwr_tStatus send_sts;

  *sts = SEV__SUCCESS;

  stime = int(scantime / m_scantime + 0.5);
  if ( !stime || m_loopcnt % stime != 0)
    return false;

  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    if ( m_nodes[i].connected) {
      send_server_status_request( m_nodes[i].nid, &send_sts);
      if ( EVEN(send_sts))
        *sts = send_sts;
    }
  }
  return true;
}

bool rt_sevhistmon::send_server_status_request( pwr_tNid nid, pwr_tStatus *sts)
{
  sev_sMsgAny   *msg;
  qcom_sQid     tgt;
  qcom_sPut put;
  pwr_tStatus   lsts;  

  tgt.nid = nid;
  tgt.qix = sev_eProcSevServer;

  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevClient;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_ServerStatusRequest;
  put.msg_id = m_msg_id++;
  put.size = sizeof(*msg);
  msg = (sev_sMsgAny *) qcom_Alloc(&lsts, put.size);
  put.data = msg;

  msg->Type = sev_eMsgType_NodeUp;

  if ( !qcom_Put( sts, &tgt, &put)) {
    qcom_Free( &lsts, put.data);
  }

  return ODD(*sts);
}

void rt_sevhistmon::receive_server_status( sev_sMsgServerStatus *msg, pwr_tNid nid)
{
  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    if ( nid == m_nodes[i].nid) {
      m_nodes[i].status = msg->Status;
      break;
    }
  }
}

int rt_sevhistmon::send_itemlist( pwr_tNid nid)
{
  int item_cnt = 0;
  int attr_cnt = 0;
  int objectitem_cnt = 0;
  qcom_sQid     tgt;
  qcom_sPut put;
  pwr_tStatus sts, lsts;
  int   size = 0;
  pwr_tAName  aname;
  char    *s;

  // Identify node
  bool found = false;
  for ( unsigned int i = 0; i < m_nodes.size(); i++) {
    if ( nid == m_nodes[i].nid) {
      found = true;
      m_nodes[i].connected = 1;
      break;
    }
  }

  int histobjectsize = 0;

  // Count items for this node
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;
    if ( nid == m_hs[i].nid)
    {
        item_cnt += m_hs[i].sevhistlist.size();
        attr_cnt += m_hs[i].sevhistlist.size();
        objectitem_cnt += m_hs[i].sevhistobjectlist.size();
        for( size_t j = 0; j < m_hs[i].sevhistobjectlist.size(); j++ ) {
          attr_cnt += m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist.size();
          histobjectsize += (m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist.size() * sizeof(sev_sHistAttr));
        }
    }
  }

  if ( item_cnt == 0 && objectitem_cnt == 0 )
    return 1;

  if( item_cnt != 0 ) {
    size = sizeof(sev_sMsgHistItems) + (item_cnt - 1) * sizeof(sev_sHistItem);
    size += objectitem_cnt * (sizeof(sev_sHistItem) - sizeof(sev_sHistAttr)) + histobjectsize;
  }
  else {
    size += (objectitem_cnt - 1) * (sizeof(sev_sHistItem) - sizeof(sev_sHistAttr)) + histobjectsize;  
  }


  put.size = size;
  put.data = qcom_Alloc(&lsts, put.size);

  ((sev_sMsgHistItems *)put.data)->Type = sev_eMsgType_HistItems;
  ((sev_sMsgHistItems *)put.data)->Status = SEV__SUCCESS;

  ((sev_sMsgHistItems *)put.data)->NumItems = item_cnt + objectitem_cnt;
  ((sev_sMsgHistItems *)put.data)->NumAttributes = attr_cnt;

  int k = 0;
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;
    if ( nid == m_hs[i].nid) {
      for ( unsigned int j = 0; j < m_hs[i].sevhistlist.size(); j++) {
        ((sev_sMsgHistItems *)put.data)->Items[k].attrnum = 1;
        ((sev_sMsgHistItems *)put.data)->Items[k].oid = m_hs[i].sevhistlist[j].aref.Objid;
        strcpy( aname, m_hs[i].sevhistlist[j].aname); 
        s = strchr( aname, '.');
        if ( !s)
          continue;
        *s = 0;
        strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].oname, aname);
        strcpy( ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].aname, s + 1);
        ((sev_sMsgHistItems *)put.data)->Items[k].storagetime = 
        net_DeltaTimeToNetTime( &m_hs[i].sevhistlist[j].storagetime);
        ((sev_sMsgHistItems *)put.data)->Items[k].deadband = m_hs[i].sevhistlist[j].deadband;
        ((sev_sMsgHistItems *)put.data)->Items[k].options = m_hs[i].sevhistlist[j].options;
        ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].type = m_hs[i].sevhistlist[j].type;
        ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].size = m_hs[i].sevhistlist[j].size;
        ((sev_sMsgHistItems *)put.data)->Items[k].sevid = m_hs[i].sevhistlist[j].sevid;
        strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].description,
                 m_hs[i].sevhistlist[j].description, 
                 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].description));
        strncpy( ((sev_sMsgHistItems *)put.data)->Items[k].attr[0].unit,
                 m_hs[i].sevhistlist[j].unit, 
                 sizeof(((sev_sMsgHistItems *)put.data)->Items[0].attr[0].unit));
        ((sev_sMsgHistItems *)put.data)->Items[k].scantime = m_hs[i].sevhistlist[j].scantime;
        k++;
      }
    }
  }

  //Add the objectitems at the end of the message
  sev_sHistItem *buffP = &((sev_sMsgHistItems *)put.data)->Items[k];
  for ( unsigned int i = 0; i < m_hs.size(); i++) {
    if ( m_hs[i].configerror)
      continue;
    if ( nid == m_hs[i].nid) {
      int numberOfAttributes;
      for ( unsigned int j = 0; j < m_hs[i].sevhistobjectlist.size(); j++) {
        buffP->oid = m_hs[i].sevhistobjectlist[j].aref.Objid;
        strcpy( aname, m_hs[i].sevhistobjectlist[j].aname); 
        strcpy( buffP->oname, aname);
        buffP->storagetime = net_DeltaTimeToNetTime( &m_hs[i].sevhistobjectlist[j].storagetime);
        buffP->deadband    = m_hs[i].sevhistobjectlist[j].deadband;
        buffP->options     = m_hs[i].sevhistobjectlist[j].options;
        buffP->sevid       = m_hs[i].sevhistobjectlist[j].sevid;
        strncpy( buffP->description, m_hs[i].sevhistobjectlist[j].description, sizeof(pwr_tString80) );
        buffP->scantime    = m_hs[i].sevhistobjectlist[j].scantime;
        buffP->attrnum     = m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist.size();
        for( size_t jj = 0; jj < m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist.size(); jj++ ) {
          strcpy( aname, m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[jj].aname);
          //Point out attribute name
          s = strchr( aname, '.');
          if ( !s)
            continue;
          *s = 0;
          strcpy( buffP->attr[jj].aname, s+1);
          buffP->attr[jj].type = m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[jj].type;
          buffP->attr[jj].size = m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[jj].size;
          strncpy( buffP->attr[jj].unit, 
                   m_hs[i].sevhistobjectlist[j].sevhistobjectattrlist[jj].unit, sizeof(pwr_tString16));
        }
        numberOfAttributes = buffP->attrnum;
        //buffP points after the last attribute written
        buffP = (sev_sHistItem *)&buffP->attr[numberOfAttributes];
      }
    }
  }

  tgt.nid = nid;
  tgt.qix = sev_eProcSevServer;


  put.reply.nid = m_nodes[0].nid;
  put.reply.qix = sev_eProcSevClient;
  put.type.b = (qcom_eBtype) sev_cMsgClass;
  put.type.s = (qcom_eStype) sev_eMsgType_HistItems;
  put.msg_id = m_msg_id++;

  if ( !qcom_Put( &sts, &tgt, &put)) {
    printf("rt_sevhistmon: ItemList send ERROR sts:%d\n", sts);
    qcom_Free( &sts, put.data);
    return 0;
  }

  // If all nodes are connected, set running status
  if ( !m_allconnected) {
    bool all_conn = true;
    for ( unsigned int i = 0; i < m_nodes.size(); i++) {
      if ( m_nodes[i].is_server && !m_nodes[i].connected) {
        all_conn = false;
        break;
      }
    }
    if ( all_conn) {
      m_allconnected = 1;
      m_server_status = PWR__SRUN;
      set_status();
    }
  }
  printf("rt_sevhistmon: ItemList sent\n");
  return 1;
}

int rt_sevhistmon::mainloop()
{
  qcom_sQid qid;
  int tmo = int(1000 * m_scantime);
  qcom_sGet get;
  void *mp;
  pwr_tStatus sts;

  qid.nid = 0;
  qid.qix = sev_eProcSevClient;

  for (;;) {
    memset( &get, 0, sizeof(get));
    mp = qcom_Get(&sts, &qid, &get, tmo);
    if ( sts == QCOM__TMO || !mp) {
      m_loopcnt++;
      send_data();
      send_server_status_request( &sts);
      continue;
    }

    switch (get.type.b) {
      case sev_cMsgClass:
        switch ( get.type.s) {
          case sev_eMsgType_NodeUp:
            printf("rt_sevhistmon: Node up recevied\n");
            send_itemlist( get.sender.nid);
            break;
          case sev_eMsgType_HistItemsRequest:
            printf("rt_sevhistmon: HistitemsRequest recevied\n");
            send_itemlist( get.sender.nid);
            break;
          case sev_eMsgType_ServerStatus:
            receive_server_status( (sev_sMsgServerStatus *) mp, get.sender.nid);
            break;
          default: ;
        }
        break;
      case qcom_eBtype_event: {
          ini_mEvent  new_event;
          qcom_sEvent *ep = (qcom_sEvent*) get.data;

          new_event.m  = ep->mask;
          if (new_event.b.oldPlcStop && !m_swap) {
            m_swap = 1;
            // errh_SetStatus( PWR__SRVRESTART);
            m_confp->Status = PWR__SRVRESTART;
            close();
          }
          else if (new_event.b.swapDone && m_swap) {
            m_swap = 0;
            try {
              init_objects();
              connect();
            }
            catch ( co_error e) {
              errh_Error( "SevHistMonitor terminating, %m", e.sts());
              exit(0);
            }
            // errh_SetStatus( PWR__SRUN);
            m_confp->Status = PWR__SRUN;
            errh_Info("Warm restart completed");
          }
          else if (new_event.b.terminate) {
            m_confp->Status = PWR__SRVTERM;
            exit(0);
          }
          break;
        }
      default: ;
    }

    qcom_Free( &sts, mp);

    if ( !m_allconnected)
      retry_connect();
  }

}

int main()
{
  rt_sevhistmon client;

  try {
    client.init();
    client.connect();
  }
  catch ( co_error e) {
    errh_Error( "SevHistMonitor terminating, %m", e.sts());
    exit(0);
  }

  client.mainloop();

  client.close();
}
