#ifndef wb_object_h
#define wb_object_h

#include "pwr.h"
#include "wb_status.h"
#include "wb_orep.h"
#include "wb_ldh.h"
#include "wb_destination.h"
//#include "wb_location.h"
#include "wb_attribute.h"

class wb_bdef;
class wb_orep;
class wb_attribute;
class wb_destination;

class wb_object : public wb_status
{
public:

  wb_orep *m_orep;

  wb_object();
  wb_object(wb_orep*);
  wb_object(pwr_tStatus, wb_orep*);
  wb_object(const wb_object&);

  ~wb_object();

  wb_object& operator=(const wb_object&);

  operator bool() const;
  operator wb_orep*() const;
  bool operator==(wb_object&);
    
  void uniqueName(const char *) {}  // Fix

  pwr_tStatus checkXref(const char *name);

  void user(void *p) {} // Fix  // set user data
  void *user() { return 0;} // Fix   // get user data
    
  ldh_sRefInfo *refinfo(ldh_sRefInfo *rp) { memset(rp,0,sizeof(*rp)); return rp;} // Fix
    

  pwr_tOid oid();  //< Object identifier of this object
  pwr_tVid vid();  //< Volume identifier of this object
  pwr_tOix oix();  //< Object index of this object
  pwr_tCid cid();  //< Class identifier of class that this object is an instance of

  pwr_tOid poid(); //< Object identifier of parent of this object
  pwr_tOid foid(); //< Object identifier of first child of this object
  pwr_tOid loid(); //< Object identifier of last child of this object
  pwr_tOid boid(); //< Object identifier of object before this object 
  pwr_tOid aoid(); //< Object identifier of object after this object
    
  pwr_tOix poix(); //< Object index of parent of this object
  pwr_tOix foix(); //< Object index of first child of this object
  pwr_tOix loix(); //< Object index of last child of this object
  pwr_tOix boix(); //< Object index of object before this object 
  pwr_tOix aoix(); //< Object index of object after this object
    
  const char *name();
  wb_name longName();

  pwr_tTime ohTime();  //< time when this object was last changed
  pwr_tTime rbTime();  //< time when run time body of this object was last changed
  pwr_tTime dbTime();  //< time when development body of this object was last changed

  pwr_mClassDef flags();

  size_t rbSize();  //< size of run time body
  size_t dbSize();  //< size of development body
    
  wb_bdef bdef(const char *bname);
  wb_bdef bdef(pwr_eBix bix);

  wb_attribute attribute();
  wb_attribute attribute(const char *aname);
  wb_attribute attribute(const char *bname, const char*aname);
    
  /* Navigational operations */

  wb_object ancestor();
  wb_object parent();
  wb_object after();     //< next sibling
  wb_object before();    //< previous sibling
  wb_object first();     //< first child
  wb_object child(wb_name &name);
  wb_object last();      //< last child

  wb_object next();      //< next in list of objects of same class in one volume
  wb_object previous();  //< previous in list of objects of same class in one volume

  wb_destination destination(ldh_eDest dest);

  bool docBlock( char **block, int *size);
  bool docBlock( char *block);

private:
  bool check();
  bool check(string str);
    
};

#endif



