#include "wb_vrepdbs.h"
#include "wb_orepdbs.h"

void wb_vrepdbs::unref()
{
    if (--m_nRef == 0)
        delete this;
}

wb_vrep *wb_vrepdbs::ref()
{
    m_nRef++;
    return this;
}

wb_vrepdbs::wb_vrepdbs(wb_erep *erep, const char *fileName) : m_erep(erep)
{
    
    strcpy(m_fileName, fileName);
    m_isDbsenvLoaded = false;
}

dbs_sEnv *
wb_vrepdbs::dbsenv()
{
    dbs_sEnv *ep;
    pwr_tStatus sts;

    if (!m_isDbsenvLoaded) {
        ep = dbs_Map(&sts, &m_dbsenv, m_fileName);
        if (!ep) {
            throw wb_error(sts);
        }
        m_isDbsenvLoaded = true;
        dbs_GetVolumeName(&sts, &m_dbsenv, m_name);
    } else {
        ep = &m_dbsenv;
    }
    
    
    return ep;
}

bool
wb_vrepdbs::load()
{
    return dbsenv() != 0;
}


wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, pwr_tOid oid)
{    
    dbs_sObject *op = dbs_OidToObject(sts, dbsenv(), oid);
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}

wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, char *name)
{
    return 0;
}

wb_orep *
wb_vrepdbs::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
#if 0
    dbs_sName n;   

    n.poix = parent->oix();
    strcpy(n.normname, name.normName(cdh_mName_object));
    
    dbs_sName *nrp = dbs_Bfind(&sts, dbsenv(), m_dbs.name_bt, &n, dbs_CompName);
    if (!nrp)
        return 0;

    dbs_Object *o = dbs_Address(&sts, dbsenv(), nrp->ref);
    if (!o)
        return 0;
    
    return new (this) orepdbs(o);
#endif
    return 0;
}


wb_orep *wb_vrepdbs::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name)
{
    return 0;
}


wb_orep *wb_vrepdbs::copyObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d, wb_name name)
{
    return 0;
}

bool wb_vrepdbs::copyOset(pwr_tStatus *sts, wb_oset *oset, wb_destination d)
{
    return true;
}


bool wb_vrepdbs::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination d)
{
    return true;
}


bool wb_vrepdbs::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
    return true;
}

bool wb_vrepdbs::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
    return true;
}

bool wb_vrepdbs::deleteOset(pwr_tStatus *sts, wb_oset *oset)
{
    return true;
}


bool wb_vrepdbs::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name name)
{
    return true;
}

bool wb_vrepdbs::commit(pwr_tStatus *sts)
{
    return true;
}

bool wb_vrepdbs::abort(pwr_tStatus *sts)
{
    return true;
}


bool wb_vrepdbs::writeAttribute()
{
    return true;
}


bool wb_vrepdbs::readAttribute(wb_orep *o, pwr_tOix bix, unsigned int offset, unsigned int size)
{
    return true;
}


bool wb_vrepdbs::readBody()
{
    return true;
}


bool wb_vrepdbs::writeBody()
{
    return true;
}


wb_orep *wb_vrepdbs::ancestor(pwr_tStatus *sts, wb_orep *o)
{    
    dbs_sObject *op = dbs_Ancestor(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::parent(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Parent(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::after(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_After(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::before(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Before(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::first(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_First(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::child(pwr_tStatus *sts, wb_orep *o, char *name)
{
    dbs_sObject *op = dbs_Child(sts, dbsenv(), ((wb_orepdbs *)o)->o(), name);
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::last(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Last(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::next(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Next(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::previous(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Previous(sts, dbsenv(), ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_srep *wb_vrepdbs::newSession()
{
    return (wb_srep*)0;
}


wb_orep *
wb_vrepdbs::object(pwr_tStatus *)
{
    return 0;
}

bool
wb_vrepdbs::isLocal(wb_orep *) const
{
    return false;
}

pwr_tCid
wb_vrepdbs::cid() const
{
    return 0;
}

pwr_tVid
wb_vrepdbs::vid() const
{
    return 0;
}

wb_erep *
wb_vrepdbs::erep() const
{
    return 0;
}

wb_vrep *
wb_vrepdbs::next () const
{
    return 0;
}

wb_orep *
object (pwr_tStatus *)
{
    return 0;
}


wb_merep *wb_vrepdbs::merep() const
{
    return 0;
}

bool wb_vrepdbs::createSnapshot(char *)
{
    return false;
}

void wb_vrepdbs::iterObject(wb_dbs *)
{
}

void wb_vrepdbs::iterRbody(wb_dbs *)
{
}

void wb_vrepdbs::iterDbody(wb_dbs *)
{
}

wb_orepdbs *
wb_vrepdbs::new_wb_orepdbs(size_t size)
{
    wb_orepdbs *o = (wb_orepdbs *) calloc(1, size);
    o->m_vrep = this;
    return o;
}

void
wb_vrepdbs::delete_wb_orepdbs(void *p)
{
    free(p);
}

void
wb_vrepdbs::objectName(wb_orep *o, char *str) const
{
#if 0
    *str = 0;
        
    // Count ancestors
    int cnt = 0;
    wb_wblnode *n = ((wb_orepwbl *)o)->wblNode();
    while ( n) {
      cnt++;
      n = n->o_fth;
    }

    wb_wblnode **vect = (wb_wblnode **) calloc( cnt, sizeof(vect));

    n = ((wb_orepwbl *)o)->wblNode();
    for ( int i = 0; i < cnt; i++) {
      vect[i] = n;
      n = n->o_fth;
    }

    for ( int i = cnt - 1; i >= 0; i--) {
      strcat( str, vect[i]->name);
      if ( i == cnt - 1)
        strcat( str, ":");
      else if ( i != 0)
        strcat( str, "-");
    }
    free( vect);
#endif
}
