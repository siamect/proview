#ifndef wb_vrepwbl_h
#define wb_vrepwbl_h

#include <string>

#include "wb_vrep.h"
#include "wb_orepdb.h"
#include "antlr/AST.hpp"
#include "wb_wbllexer.hpp"
#include "wb_wblparser.hpp"
#include "wb_wblnode.h"
#include "wb_wblfile.h"
#include "wb_erep.h"

#define WBL_FILEMAX 500
#define wbl_eBix_SysBody 1
#define wbl_eBix_RtBody  1
#define wbl_eBix_DevBody 2

class wb_orepwbl;

class wb_vrepwbl : public wb_vrep
{
    pwr_tVid m_vid;
    pwr_tCid m_cid;
    map<int, wb_srep*> m_srep;
    //wb_session m_wsession;

    wb_erep *m_erep;
    wb_merep *m_merep;
    unsigned int m_nSession;
    unsigned int m_nRef;

    map<string, ref_wblnode> m_type_list;
    map<pwr_tTid, ref_wblnode> m_tid_list;
    map<string, ref_wblnode> m_class_list;
    map<pwr_tCid, ref_wblnode> m_cid_list;
    map<pwr_tOix, ref_wblnode> m_oix_list;

    typedef map<string, ref_wblnode>::iterator iterator_type_list;
    typedef map<pwr_tTid, ref_wblnode>::iterator iterator_tid_list;
    typedef map<string, ref_wblnode>::iterator iterator_class_list;
    typedef map<pwr_tCid, ref_wblnode>::iterator iterator_cid_list;
    typedef map<pwr_tOix, ref_wblnode>::iterator iterator_oix_list;

public:
    wb_vrepwbl( wb_erep *erep) : 
      m_erep(erep), m_merep(erep->merep()), root_object(0), error_cnt(0), 
      file_cnt(0), next_oix(0), volume_node(0) {}
    wb_vrepwbl( wb_erep *erep, pwr_tVid vid) : 
      m_vid(vid), m_erep(erep), m_merep(erep->merep()), root_object(0), 
      error_cnt(0), file_cnt(0), next_oix(0), volume_node(0) {}
    ~wb_vrepwbl();

    pwr_tVid vid() const { return m_vid;}
    pwr_tCid cid() const { return m_cid;}

    wb_vrep *next() const;

    virtual bool createSnapshot(char *fileName);

    virtual void iterObject(wb_dbs *);
    virtual void iterRbody(wb_dbs *);
    virtual void iterDbody(wb_dbs *);
#if 0
    void iterObject( void *udata,
		     pwr_tStatus (*bc)(void *,pwr_tOid, pwr_tCid, pwr_tOid, pwr_tOid,
				       pwr_tOid, pwr_tOid, pwr_tOid, char *,
				       pwr_tTime, int, int));
    void iterBody( void *udata,
		   pwr_tStatus (*bc)(void *,pwr_tOid, void *, void *));
#endif
    int load( char *fname);
    int load_files( char *file_spec);
    void info();

    wb_wblfile *file[WBL_FILEMAX];
    wb_wblnode *root_object;

    void error( char *msg, char *file, int line_number);
    ref_wblnode find( char *name);
    void registerClass( char *name, pwr_tCid cid, ref_wblnode node);
    void registerType( char *name, pwr_tTid tid, ref_wblnode node);
    void registerVolume( char *name, pwr_tCid cid, pwr_tVid vid, ref_wblnode node);
    bool registerObject( pwr_tOix oix, ref_wblnode node);
    int nextOix() { return ++next_oix; }
    int classNameToCid( char *name, pwr_tCid *cid);
    int getTemplateBody( pwr_tCid cid, int bix, int *size, void **body);
    int getTypeInfo( pwr_tTid tid, pwr_eType *type, int *size,
		     int *elements);
    int getTypeInfo( char *type, pwr_tTid *tid, pwr_eType *type, int *size,
		     int *elements);
    int getClassInfo( pwr_tCid cid, int *rsize, int *dsize);
    int getAttrInfo( char *attr, int bix, pwr_tCid cid, int *size,
		     int *offset, pwr_tTid *tid, int *elements);
    int getAttrInfoRec( wb_name *attr, int bix, pwr_tCid cid, int *size,
			int *offset, pwr_tTid *tid, int *elements, int level);
    ref_wblnode findObject( pwr_tOix oix);
    ref_wblnode findClass( char *name);
    ref_wblnode findType( char *name);
    ref_wblnode findClass( pwr_tCid cid);
    ref_wblnode findType( pwr_tTid tid);
    int nameToOid( char *name, pwr_tOid *oid);

    int error_cnt;
    int file_cnt;
    int next_oix;
    char volume_class[32];
    char volume_name[32];
    wb_wblnode *volume_node;
    

    virtual void unref();
    virtual wb_vrep *ref();

    wb_erep *erep() const {return m_erep;};
    wb_merep *merep() const { return m_merep;}

    wb_orep *object(pwr_tStatus *sts);
    wb_orep *object(pwr_tStatus *sts, pwr_tOid oid) {return 0;};
    wb_orep *object(pwr_tStatus *sts, wb_orep *parent, wb_name name) 
      {return 0;};

    wb_orep *createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name) {return 0;};

    wb_orep *copyObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d, wb_name name) {return 0;};
    bool copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination d) 
      {return false;};

    bool moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d) 
      {return false;};

    bool deleteObject(pwr_tStatus *sts, wb_orep *orep) {return false;};
    bool deleteFamily(pwr_tStatus *sts, wb_orep *orep) {return false;};
    bool deleteOset(pwr_tStatus *sts, wb_oset *oset) {return false;};

    bool renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name name)
      {return false;};


    bool commit(pwr_tStatus *sts) {return false;};
    bool abort(pwr_tStatus *sts) {return false;};

    bool writeAttribute() {return false;};

    bool readAttribute(wb_orep *o, pwr_tOix bix, unsigned int offset, unsigned int size) {return false;};

    bool readBody() {return false;};

    bool writeBody() {return false;};


    wb_orep *ancestor(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *parent(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *after(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *before(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *first(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *child(pwr_tStatus *sts, wb_orep *o, char *name) const;

    wb_orep *last(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *next(pwr_tStatus *sts, wb_orep *o) const;

    wb_orep *previous(pwr_tStatus *sts, wb_orep *o) const;

    wb_srep *newSession() {return 0;};

    bool isLocal(wb_orep *o) const {return false;};

};

#endif












