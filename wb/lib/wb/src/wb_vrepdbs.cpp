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

#if 1
wb_vrepdbs::wb_vrepdbs(const char *fileName)
{
    pwr_tStatus sts;
    
    dbs_sEnv *ep =dbs_Map(&sts, &m_dbsenv, fileName);
    if (!ep)
        ep = 0;
    
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, pwr_tOid oid)
{    
    dbs_sObject *op = dbs_OidToObject(sts, &m_dbsenv, oid);
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
#if 0
    dbs_sName n;   

    n.poix = parent->oix();
    strcpy(n.normname, name.normName(cdh_mName_object));
    
    dbs_sName *nrp = dbs_Bfind(&sts, &m_dbsenv, m_dbs.name_bt, &n, dbs_CompName);
    if (!nrp)
        return 0;

    dbs_Object *o = dbs_Address(&sts, &m_dbsenv, nrp->ref);
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
    dbs_sObject *op = dbs_Ancestor(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::parent(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Parent(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::after(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_After(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::before(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Before(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::first(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_First(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::child(pwr_tStatus *sts, wb_orep *o, char *name)
{
    dbs_sObject *op = dbs_Child(sts, &m_dbsenv, ((wb_orepdbs *)o)->o(), name);
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::last(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Last(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::next(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Next(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_orep *wb_vrepdbs::previous(pwr_tStatus *sts, wb_orep *o)
{
    dbs_sObject *op = dbs_Previous(sts, &m_dbsenv, ((wb_orepdbs *)o)->o());
    if (op == 0)
        return 0;

    return new (this) wb_orepdbs(op);
}


wb_srep *wb_vrepdbs::newSession()
{
    return (wb_srep*)0;
}
#endif
