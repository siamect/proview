#include "wb_session.h"
#include "wb_error.h"
#include "wb_ldh_msg.h"

wb_session::wb_session(wb_volume &v) : wb_volume(v)
{
  m_srep = new wb_srep(v);
  m_srep->ref();
}

wb_session::wb_session(wb_session &s): wb_volume(s.m_vrep), m_srep(s.m_srep)
{
  if ( m_srep)
    m_srep->ref();
}

wb_session::~wb_session()
{
  m_srep->unref();
}

wb_object wb_session::createObject(wb_cdef cdef, wb_destination d, wb_name name)
{
    wb_orep *orep = 0;
    
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    orep = m_vrep->createObject(&m_sts, cdef, d, name);
    return wb_object(m_sts, orep);
}

wb_object wb_session::copyObject(wb_object o, wb_destination d, wb_name name)
{
    wb_orep *orep = 0;
    
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    orep = m_vrep->copyObject(&m_sts, (wb_orep*)o, d, name);
    return wb_object(m_sts, orep);
}

bool wb_session::copyOset(wb_oset *o, wb_destination d)
{
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    return m_vrep->copyOset(&m_sts, o, d);
}

bool wb_session::moveObject(wb_object o, wb_destination d)
{   
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    return m_vrep->moveObject(&m_sts, (wb_orep*)o, d);
}


bool wb_session::renameObject(wb_object o, wb_name name)
{
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    return m_vrep->renameObject(&m_sts, (wb_orep*)o, name);
}

bool wb_session::deleteObject(wb_object o)
{
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    return m_vrep->deleteObject(&m_sts, (wb_orep*)o);
    
/*
    if (!o.isLocal())
        return LDH__OTHERVOLUME;
    
    if (o.hasChild())
        return LDH__HAS_CHILD;

    o.triggAnteUnadopt();
    o.unaAdopt();
    o.triggPostUnadopt();
*/
}

bool wb_session::deleteFamily(wb_object o)
{
    if (isReadonly())
        throw wb_error_str("ReadOnlySession");

    return m_vrep->deleteFamily(&m_sts, (wb_orep*)o);
}





