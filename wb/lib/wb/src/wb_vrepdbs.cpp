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

wb_vrepdbs::wb_vrepdbs()
{
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, pwr_tOid oid)
{
    //wb_orepdbs *o = new wb_orepdbs();
    return 0;
}

wb_orep *wb_vrepdbs::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
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


wb_orep *wb_vrepdbs::ancestor(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::parent(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::after(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::before(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::first(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::child(pwr_tStatus *sts, wb_orep *o, char *name) const
{
    return 0;
}


wb_orep *wb_vrepdbs::last(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::next(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_orep *wb_vrepdbs::previous(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}


wb_srep *wb_vrepdbs::newSession()
{
    return (wb_srep*)0;
}
