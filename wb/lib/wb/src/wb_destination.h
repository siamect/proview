#ifndef wb_destination_h
#define wb_destination_h

#include "pwr.h"
#include "wb_ldh.h"

class wb_orep;
class wb_object;

class wb_destination
{
public:

    wb_orep *m_parent;
    wb_orep *m_before;
    wb_orep *m_after;
    ldh_eDest m_dest;
    
    /*
      Used as intention:

      wb_Position(0, 0, after)        => the position just before the object with oid = 'after'
      wb_Position(0, before, 0)       => the position just after  the object with oid = 'before'
      wb_Position(parent, 0, parent)  => the position as last  child of the object with oid = 'parent'
      wb_Position(parent, parent, 0)  => the position as first child of the object with oid = 'parent'

    */

    wb_destination(wb_object &parent, wb_object &before, wb_object &after);
    wb_destination(wb_object &object, ldh_eDest dest);

    bool canAdopt(pwr_tCid cid) {return true;} // Fix
    bool canAdopt(pwr_tOid oid) {return true;} // Fix
    
};

#endif



