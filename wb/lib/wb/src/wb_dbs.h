#ifndef wb_dbs_h
#define wb_dbs_h

#include "pwr.h"
#include "co_pdr.h"
#include "co_dbs.h"
#include "co_tree.h"
#include "wb_status.h"
#include "wb_ldh.h"
#include "wb_import.h"


class wb_vrep;

class wb_dbs : public wb_status, public wb_import
{
public:

  typedef union {
    struct {
      pwr_tBit    devOnly       : 1;
      pwr_tBit    exist         : 1;
    } b;

    pwr_tBitMask m;

#define mOentry_devOnly		     1
#define mOentry_exist		     2
  } mOentry;

  struct sOentry;
  struct sNentry;
  struct sCentry;

  struct sOentry {
    tree_sNode       node;
    dbs_sObject      o;
    dbs_sBody        rbody;
    dbs_sBody        dbody;    
    mOentry          flags;
    dbs_tRef         ref;
    dbs_tRef         oidref;

    sOentry         *poep;
    sOentry         *boep;
    sOentry         *aoep;
    sOentry         *foep;
    sOentry         *loep;
    
    sOentry         *o_ll;
  };

  /* Name entry, used in name table.  */

  struct sNentry {
    tree_sNode   node;
    dbs_sName    n;
    dbs_tRef     ref;
    sOentry      *oep;
  };

  struct sCentry {
    tree_sNode node;
    dbs_sClass c;
    dbs_tRef   ref;
    sOentry    *o_lh;  // Header of object list
    sOentry    *o_lt;  // Tail of object list
  };

  FILE         *m_fp;                 /**< file pointer */
  char         m_name[100];           /**< name of load file */
  //    wb_object    *m_o;		/* Volume object header.  */
  wb_vrep      *m_v;
  dbs_sVolume  m_volume;
  pwr_tOid     m_oid;
  dbs_sSect    m_sect[dbs_eSect_];    /**< section header */

  unsigned int m_warnings;
  unsigned int m_errors;
    
  unsigned int m_nObjects;
  unsigned int m_nTreeObjects;
  unsigned int m_nClassObjects;
  unsigned int m_nNameObjects;
  unsigned int m_nRbodyObjects;
  unsigned int m_nDbodyObjects;
    
  sOentry      *m_oep;   /* object entry of volume object */

  tree_sTable  *m_oid_th;
  tree_sTable  *m_name_th;
  tree_sTable  *m_class_th;
    
  wb_dbs();
  wb_dbs(wb_vrep *);

  ~wb_dbs();

  //wb_dbs& operator=(const wb_object&);

  operator bool() const;
    
  bool operator==(wb_dbs&);
    
//  void         buildFile();
  void         buildSectName();
  void         buildSectOid();
  void         buildSectClass();
  void         checkObject(sOentry *oep);
  pwr_tStatus  closeFile(pwr_tBoolean doDelete);
  pwr_tBoolean createFile();
  // ldhi_sObjHead *getAliasServer(sLCB *lcbp, ldhi_sObjHead *o, pwr_tOid *soid);
  // pwr_tStatus    getMountServer(sLCB *lcbp, ldhi_sObjHead *o, pwr_tOid *soid);

  pwr_tStatus  installObject(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid, pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid,
                             pwr_tOid loid, pwr_tObjName name, pwr_tObjName normname, pwr_tTime ohTime,
                             pwr_tTime rbTime, pwr_tTime dbTime, size_t rbSize, size_t dbSize);
  pwr_tStatus  installRbody(pwr_tOid oid, void *body);
  pwr_tStatus  installDbody(pwr_tOid oid, void *body);
  void         classInsert(sOentry *oep);
  pwr_tStatus  openFile();
  pwr_tStatus  writeSectFile();
  pwr_tStatus  writeSectDirectory();
  pwr_tStatus  writeSectVolume();
  pwr_tStatus  writeSectVolref();
  pwr_tStatus  writeSectOid();
  pwr_tStatus  writeSectObject();
  pwr_tStatus  writeSectRbody();
  pwr_tStatus  writeSectName();
  pwr_tStatus  writeSectClass();
  pwr_tStatus  writeSectDbody();


  virtual bool importVolume(wb_export &e);
    
  virtual bool importHead(pwr_tOid oid, pwr_tCid cid, pwr_tOid poid,
                          pwr_tOid boid, pwr_tOid aoid, pwr_tOid foid, pwr_tOid loid,
                          pwr_tObjName name, pwr_tObjName normname,
                          pwr_tTime ohTime, pwr_tTime rbTime, pwr_tTime dbTime,
                          size_t rbSize, size_t dbSize);

  virtual bool importRbody(pwr_tOid oid, size_t size, void *body);
    
  virtual bool importDbody(pwr_tOid oid, size_t size, void *body);
    
  virtual bool importMeta(dbs_sEnv *ep);

private:
    
};

#endif



