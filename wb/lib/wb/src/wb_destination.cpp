#include "wb_destination.h"

wb_destination::wb_destination(wb_object &parent, wb_object &before, wb_object &after)
{
    //m_parent = parent;
}

wb_destination::wb_destination(wb_object &o, ldh_eDest dest)
{
    m_dest = dest;
    
    switch (m_dest) {
    case ldh_eDest_Before:
        //m_parent = o.parent();
        //m_after = o;
        //m_before = o.before();
        break;
    case ldh_eDest_After:
        //m_parent = object.parent;
        //m_after = object.after;
        //m_before = object;
        break;
    case ldh_eDest_IntoFirst:
        //m_parent = object;
        //m_before = object;
        //m_after = object.first;
        break;
    case ldh_eDest_IntoLast:
        //m_parent = object;
        //m_after = object;
        //m_before = object.last;
    default:
        break;
    }
    
}

/*
wb_destination::adopt(wb_object o)
{
    wb_object parent;
    wb_object before;
    wb_object after;
    
    switch (m_dest) {
    case first:
        o.before(m_o);
        if (m_o == m_o.first()) {
            
        
    
    if (m_before == m_parent) {
        m_parent.first(o);
    } else {
        m_before.after(o);
    }
    
    if (m_after == m_parent) {
        m_parent.last(o);
    } else {
        m_after.before(o);
    }
    
    o.parent(parent);
    o.before(before);
    o.after(after);
    
    
    
    
}
*/
