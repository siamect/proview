/* wb_session.h -- work bench database session

   PROVIEW/R
   Copyright (C) 2002 by Stiftelsen Proview.

   .  */

#ifndef wb_session_h
#define wb_session_h

#include "pwr.h"
//#include "wb_vrep.h"
#include "wb_object.h"
#include "wb_attribute.h"
#include "wb_ldh.h"
//#include "wb_ldhi.h"
//#include "wb_adef.h"
//#include "wb_bdef.h"
#include "wb_cdef.h"
#include "wb_name.h"
#include "wb_destination.h"
#include "wb_oset.h"
#include "wb_srep.h"
#include "wb_volume.h"

class wb_object;
class wb_bdef;
class wb_cdef;
class wb_destination;
class wb_volume;


class wb_session : public wb_volume
{
protected:
    wb_srep    *m_srep;
    
public:

    
    wb_session();
    wb_session(wb_volume &v);
    wb_session(wb_session &s);
    ~wb_session();
    
    // Calls redirected to srep.
    
    ldh_eAccess access() { return m_srep->access( &m_sts);}
    bool access(ldh_eAccess access) { return m_srep->access( &m_sts, access);}
    ldh_eUtility utility() { return m_srep->utility( &m_sts);}
    bool utility(ldh_eUtility utility) { return m_srep->utility( &m_sts, utility);}

    //

    bool isReadonly();
    bool isEmpty();
    bool commit();
    bool abort();

    // Calls that need write privileges

    wb_object createObject(wb_cdef cdef, wb_destination d, wb_name name);
    wb_object copyObject(wb_object o, wb_destination d, wb_name name);
    bool copyOset(wb_oset *oset, wb_destination d);

    bool moveObject(wb_object o, wb_destination d);
    bool renameObject(wb_object o, wb_name name);
    bool deleteObject(wb_object o);
    bool deleteFamily(wb_object o);

    bool writeAttribute() {return false;} // Fix
    bool writeBody() {return false;} // Fix

    //

};


inline bool wb_session::isEmpty()
{
    return m_srep->isEmpty(&m_sts);
}

inline bool wb_session::isReadonly()
{
    return m_srep->isReadonly(&m_sts);
}

inline bool wb_session::commit()
{
    return m_srep->commit(&m_sts);
}

inline bool wb_session::abort()
{
    return m_srep->abort(&m_sts);
}



#endif









