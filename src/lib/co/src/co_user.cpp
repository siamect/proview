/* rt_ge.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#define EVEN(sts) ((sts % 2) == 0)

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#include "co_user.h"

extern "C" {
#include "pwr.h"
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_user_msg.h"
}

static int check_file( char *filename);

GeUser::~GeUser()
{
  SystemList *sl, *next;

  for ( sl = root; sl; )
  {
    next = sl->next;
    delete sl;
    sl = next;
  }
}

int GeUser::load( char *filename)
{
  ifstream	fp;
  int		type;
  int		end_found = 0;
  char		dummy[40];

  if ( !check_file( filename))
    return USER__FILEOPEN;

  strcpy( fname, filename);

  fp.open( filename);
#ifndef OS_VMS
  if ( !fp)
    return USER__FILEOPEN;
#endif

  for (;;)
  {
    fp >> type;

    switch( type) {
      case user_eData_GeUser: break;
      case user_eData_GeUserVersion:
        fp.get();
        fp.getline( version, sizeof(version));
        break;
      case user_eData_System:
        load_system( fp); break;
      case user_eData_End: end_found = 1; break;
      default:
        cout << "GeUser:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
   }
   if ( end_found)
     break;
  }

  fp.close();
  return 1;
}

int GeUser::save( char *filename)
{
  ofstream	fp;
  SystemList	*sl;

  fp.open( filename);
#ifndef OS_VMS
  if ( !fp)
    return USER__FILEOPEN;
#endif

  fp << int(user_eData_GeUser) << endl;

  fp <<	int(user_eData_GeUserVersion) << FSPACE << user_cVersion << endl;
  for ( sl = root; sl != NULL; sl = sl->next)
    sl->save( fp);

  fp << int(user_eData_End) << endl;

  fp.close();
  return 1;
}

int GeUser::load_system( ifstream& fp)
{
  // Insert
  SystemList *system_list = new SystemList("", 0, 0);
  SystemList *sl = root;
  if ( !sl)
    root = system_list;
  else {
    while( sl->next)
      sl = sl->next;
    sl->next = system_list;
  }
  system_list->load( fp);
  return 1;
}

int GeUser::add_system( char *name, unsigned int attributes)
{
  int sts;

  SystemName *sn = new SystemName( name);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  if ( sl)
    return USER__SYSTEMALREXIST;

  SystemName *parent = sn->parent();
  if ( !parent)
  {

    SystemList *system_list = new SystemList(name, 0, 0);
    system_list->next = root;
    root = system_list;
  }
  else
  {
    parent->parse();
    SystemList *sl = find_system( parent);
    if ( !sl)
    {
      delete sn;
      delete parent;
      return USER__NOSUCHSYSTEM;
    }
    sl->add_system( sn, attributes);
    delete parent;
  }
  delete sn;
  return USER__SUCCESS;
}

int GeUser::remove_system( char *name)
{
  int sts;
  SystemList *sl_p, *prev;

  SystemName *sn = new SystemName( name);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  if ( !sl) {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }
  if ( sl->childlist || sl->userlist) {
    delete sn;
    return USER__SYSTEMNOTEMPTY;
  }

  SystemName *parent = sn->parent();
  if ( !parent)
  {
    for ( sl_p = root; sl_p; sl_p = sl_p->next)
    {
      if ( sl_p == sl)
      {
        if ( sl == root)
          root = sl->next;
        else
          prev->next = sl->next;
        delete sl;
        delete sn;
        return USER__SUCCESS;
      }
      prev = sl_p;
    }
  }
  else
  {
    parent->parse();
    SystemList *parent_system = find_system( parent);
    if ( !parent_system) return USER__NOSUCHSYSTEM;

    sts =  parent_system->remove_system( sl);
    delete sn;
    delete parent;
    return sts;
  }
  return USER__NOSUCHSYSTEM;
}

int GeUser::modify_system( char *name, unsigned int attributes)
{
  int sts;

  SystemName *sn = new SystemName( name);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  if ( !sl)
  {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }

  sl->modify( attributes);
  return USER__SUCCESS;
}

int GeUser::get_system_data( char *name, unsigned int *attributes)
{
  int sts;

  SystemName *sn = new SystemName( name);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  if ( !sl)
  {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }

  sl->get_data( attributes);
  return USER__SUCCESS;
}

int GeUser::remove_user( char *system, char *user)
{
  int sts;

  SystemName *sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  if ( !sl)
  {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }
  sts = sl->remove_user( user);
  delete sn;
  return sts;
}

int GeUser::modify_user( char *system, char *user, char *password, unsigned int priv)
{
  int sts;
  UserList *ul;

  SystemName *sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  delete sn;
  if ( !sl)
    return USER__NOSUCHSYSTEM;

  ul = (UserList *) sl->find_user( user);
  if ( !ul)
    return USER__NOSUCHUSER;
  ul->modify( password, priv);

  return USER__SUCCESS;
}

int GeUser::get_user_data( char *system, char *user, char *password, 
		unsigned int *priv)
{
  int sts;
  UserList *ul;

  SystemName *sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }

  SystemList *sl = find_system( sn);
  delete sn;
  if ( !sl)
    return USER__NOSUCHSYSTEM;

  ul = (UserList *) sl->find_user( user);
  if ( !ul)
    return USER__NOSUCHUSER;
  ul->get_data( password, priv);

  return USER__SUCCESS;
}

SystemList *GeUser::find_system( SystemName *name)
{
  SystemList *sl;

  for ( sl = root; sl; sl = sl->next)
  {
    if ( strcmp( sl->name, name->segment(0)) == 0)
    {
      if ( name->segments == sl->level + 1)
        return sl;
      return sl->find_system( name);
    }
  }
  return NULL;
}

int GeUser::add_user( char *system, char *user, char *password, unsigned int priv)
{
  SystemList *sl;
  int sts;

  // Find system
  SystemName *sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return sts;
  }
  sl = find_system( sn);
  if ( !sl) return USER__NOSUCHSYSTEM;

  sl->add_user( user, password, priv);
  return USER__SUCCESS;
}

int GeUser::get_user( char *gu_system, char *gu_user, char *gu_password, unsigned int *priv)
{
  UserList *ul;
  SystemList *sl;
  SystemName *sn, *parent;
  int sts;
  char system[80];
  char password[80];
  char user[80];

  cdh_ToLower( system, gu_system);
  cdh_ToLower( user, gu_user);
  cdh_ToLower( password, gu_password);

  // Find system
  sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }

  while( sn)
  {
    sl = find_system( sn);
    if ( sl)
    {
      ul = (UserList *) sl->find_user( user);
      if ( ul)
      {
        sts = ul->check_password( password);
        if ( EVEN(sts))
          return sts;
        *priv = ul->priv;
        return USER__SUCCESS;
      }
      if ( !(sl->attributes & user_mSystemAttr_UserInherit))
        break;
    }
    parent = sn->parent();
    if ( parent)
      parent->parse();
    delete sn;
    sn = parent;
  }
  return USER__NOSUCHUSER;
}

int GeUser::get_user_priv( char *gu_system, char *gu_user, unsigned int *priv)
{
  UserList *ul;
  SystemList *sl;
  SystemName *sn, *parent;
  int sts;
  char system[80];
  char user[80];

  cdh_ToLower( system, gu_system);
  cdh_ToLower( user, gu_user);


  // Find system
  sn = new SystemName( system);
  sts = sn->parse();
  if ( EVEN(sts))
  {
    delete sn;
    return USER__NOSUCHSYSTEM;
  }

  while( sn)
  {
    sl = find_system( sn);
    if ( sl)
    {
      ul = (UserList *) sl->find_user( user);
      if ( ul)
      {
        *priv = ul->priv;
        return USER__SUCCESS;
      }
      if ( !(sl->attributes & user_mSystemAttr_UserInherit))
        break;
    }
    parent = sn->parent();
    if ( parent)
      parent->parse();
    delete sn;
    sn = parent;
  }
  return USER__NOSUCHUSER;
}

void GeUser::print()
{
  SystemList *sl;

  cout << endl << "Proview user database " << user_cVersion << endl << endl;
  for ( sl = root; sl != NULL; sl = sl->next)
    sl->print();
  cout << endl;
}

void GeUser::print_all()
{
  SystemList *sl;

  cout << endl << "Proview user database " << user_cVersion << endl << endl;
  for ( sl = root; sl != NULL; sl = sl->next)
    sl->print_all();
  cout << endl;
}

SystemList::~SystemList()
{
  SystemList *sl, *next_sl;
  UserList *ul, *next_ul;

  for ( ul = (UserList *) userlist; ul; )
  {
    next_ul = ul->next;
    delete ul;
    ul = next_ul;
  }
  for ( sl = childlist; sl; )
  {
    next_sl = sl->next;
    delete sl;
    sl = next_sl;
  }
}

int SystemList::load( ifstream& fp)
{
  int		type;
  int		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;

    switch( type) {
      case user_eData_SystemName:
        fp.get();
        fp.getline( name, sizeof(name));
        break;
      case user_eData_SystemLevel: fp >> level; break;
      case user_eData_SystemAttributes: fp >> attributes; break;
      case user_eData_User:
        load_user( fp); break;
      case user_eData_System:
        load_system( fp); break;
      case user_eData_End: end_found = 1; break;
      default:
        cout << "System:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
   }
   if ( end_found)
     break;
  }
  return 1;
}

int SystemList::load_user( ifstream& fp)
{
  // Insert
  UserList *user_list = new UserList("", "", 0);
  UserList *ul = userlist;
  if ( !ul)
    userlist = user_list;
  else {
    while( ul->next)
      ul = ul->next;
    ul->next = user_list;
  }
  user_list->load( fp);
  return 1;
}

int SystemList::load_system( ifstream& fp)
{
  // Insert
  SystemList *system_list = new SystemList("", 0, 0);
  SystemList *sl = childlist;
  if ( !sl)
    childlist = system_list;
  else {
    while ( sl->next)
      sl = sl->next;
    sl->next = system_list;
  }
  system_list->load( fp);
  return 1;
}

void SystemList::save( ofstream& fp) 
{ 
  UserList *ul;
  SystemList *sl;

  fp << int(user_eData_System) << endl;
  fp << int(user_eData_SystemName) << FSPACE << name << endl;
  fp << int(user_eData_SystemLevel) << FSPACE << level << endl;
  fp << int(user_eData_SystemAttributes) << FSPACE << attributes << endl;
  for ( ul = (UserList *) userlist; ul != NULL; ul = ul->next)
    ul->save( fp);
  for ( sl = childlist; sl; sl = sl->next)
    sl->save( fp);
  fp << int(user_eData_End) << endl;
}

void SystemList::print()
{
  UserList *ul;
  SystemList *sl;
  int i, j;

  for ( i = 0; i < level; i++)
    cout << ". ";
  cout << name;
  cout << " ";
  for ( j = 0; j < int(22 - 2*i - strlen(name)); j++)
    cout << " ";
  if ( attributes & user_mSystemAttr_UserInherit)
    cout << "UserInherit";
  cout << endl;
  for ( ul = (UserList *)userlist; ul; ul = ul->next)
    ul->print();
  for ( sl = childlist; sl; sl = sl->next)
    sl->print();
}

void SystemList::print_all()
{
  UserList *ul;
  SystemList *sl;
  int i, j;

  for ( i = 0; i < level; i++)
    cout << ". ";
  cout << name;
  cout << " ";
  for ( j = 0; j < int(22 - 2*i - strlen(name)); j++)
    cout << " ";
  if ( attributes & user_mSystemAttr_UserInherit)
    cout << "UserInherit";
  cout << endl;
  for ( ul = (UserList *)userlist; ul; ul = ul->next)
    ul->print_all();
  for ( sl = childlist; sl; sl = sl->next)
    sl->print_all();
}

SystemList *SystemList::find_system( SystemName *name)
{
  SystemList *sl;

  for ( sl = childlist; sl; sl = sl->next)
  {
    if ( strcmp( sl->name, name->segment( sl->level)) == 0)
    {
      if ( name->segments == sl->level + 1)
        return sl;
      return sl->find_system( name);
    }
  }
  return NULL;
}

void *SystemList::find_user( char *name)
{
  UserList *ul;

  for ( ul = (UserList *) userlist; ul != NULL; ul = ul->next)
  {
    if ( strcmp( ul->name, name) == 0)
      return ul;
  }
  return NULL;
}

int SystemList::add_user( char *user, char *password, unsigned int priv)
{
  UserList *ul;

  // Check if user exist
  ul = (UserList *) find_user( user);
  if ( ul) return USER__USERALREXIST;

  ul = new UserList( user, password, priv);
  UserList *u = userlist;
  if ( !u)
    userlist = ul;
  else {
    while( u->next)
      u = u->next;
    u->next = ul;
  }
  return USER__SUCCESS;
}

int SystemList::add_system( SystemName *name, unsigned int attributes)
{
  SystemList *sl;

  // Check if system exist
  sl = find_system( name);
  if ( sl) return USER__SYSTEMALREXIST;

  sl = new SystemList( name->segment(level+1), level + 1, attributes);
  SystemList *s = childlist;
  if ( !s)
    childlist = sl;
  else {
    while( s->next)
      s = s->next;
    s->next = sl;
  }
  return USER__SUCCESS;
}

int SystemList::remove_system( SystemList *sys)
{
  SystemList *sl, *prev;

  for ( sl = childlist; sl; sl = sl->next)
  {
    if ( sl == sys)
    {
      if ( sl == childlist)
        childlist = sl->next;
      else
        prev->next = sl->next;
      delete sl;
      return USER__SUCCESS;
    }
    prev = sl;
  }
  return USER__NOSUCHSYSTEM;
}

int SystemList::remove_user( char *user)
{
  UserList *ul, *ul_p, *prev;

  ul = (UserList *) find_user( user);
  if ( !ul) return USER__NOSUCHUSER;

  for ( ul_p = (UserList *) userlist; ul_p; ul_p = ul_p->next)
  {
    if ( ul == ul_p)
    {
      if ( ul == (UserList *) userlist)
        userlist = ul->next;
      else
        prev->next = ul->next;
      delete ul;
      return USER__SUCCESS;
    }
    prev = ul_p;
  }
  return USER__NOSUCHUSER;
}

void SystemList::modify( unsigned int attributes)
{
  this->attributes = attributes;
}

void SystemList::get_data( unsigned int *attributes)
{
  *attributes = this->attributes;
}

int UserList::load( ifstream& fp)
{
  int		type;
  int		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;

    switch( type) {
      case user_eData_User: break;
      case user_eData_UserName:
        fp.get();
        fp.getline( name, sizeof(name));
        break;
      case user_eData_UserPassword:
        fp.get();
        fp.getline( password, sizeof(password));
        strcpy( password, decrypt( password));
        break;
      case user_eData_UserPrivilege: fp >> priv; priv = decrypt( priv); break;
      case user_eData_End: end_found = 1; break;
      default:
        cout << "User:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
   }
   if ( end_found)
     break;
  }
  return 1;
}

void UserList::save( ofstream& fp) 
{ 
  fp << int(user_eData_User) << endl;
  fp << int(user_eData_UserName) << FSPACE << name << endl;
  fp << int(user_eData_UserPassword) << FSPACE << crypt( password) << endl;
  fp << int(user_eData_UserPrivilege) << FSPACE << crypt( priv) << endl;
  fp << int(user_eData_End) << endl;
}

void UserList::print_all()
{
  int i;
  char priv_str[300];

  for ( i = 0; i < 6; i++)
    cout << ". ";
  cout << name << " ";
  for ( i = 0; i < int(10 - strlen(name)); i++)
    cout << " ";
  cout << password << " ";
  for ( i = 0; i < int(10 - strlen(password)); i++)
    cout << " ";
  GeUser::priv_to_string( priv, priv_str, sizeof(priv_str));
  cout << priv_str;
  cout  << "(" << priv << ")" << endl;
}

void UserList::print()
{
  int i;
  char priv_str[300];

  for ( i = 0; i < 6; i++)
    cout << ". ";
  cout << name << " ";
  for ( i = 0; i < int(10 - strlen(name)); i++)
    cout << " ";
  GeUser::priv_to_string( priv, priv_str, sizeof(priv_str));
  cout << priv_str;
  cout  << "(" << priv << ")" << endl;
}

char *UserList::crypt( char *str)
{
  static char cstr[200];
  char *s, *t;

  for ( s = str, t = cstr; *s; s++, t++)
  {
    if ( *s >= 48 && *s <= 122)
      *t = 122 - ( *s - 48);
    else
      *t = *s;
  }
  *t = *s;
  return cstr;
}

unsigned long UserList::crypt( unsigned long i)
{
  return ~(i + 123456);
}

char *UserList::decrypt( char *str)
{
  static char cstr[200];
  char *s, *t;

  for ( s = str, t = cstr; *s; s++, t++)
  {
    if ( *s >= 48 && *s <= 122)
      *t = 122 - ( *s - 48);
    else
      *t = *s;
  }
  *t = *s;
  return cstr;
}

unsigned long UserList::decrypt( unsigned long i)
{
  return ~i - 123456;
}

void UserList::modify( char *password, unsigned int priv)
{
  strcpy( this->password, password);
  this->priv = priv;
}

void UserList::get_data( char *password, unsigned int *priv)
{
  strcpy( password,  this->password);
  *priv = this->priv;
}

int UserList::check_password( char *password)
{
  if ( strcmp( this->password, password) == 0)
    return USER__SUCCESS;
  return USER__NOTAUTHORIZED;
}

int SystemName::parse()
{
  char *seg_start, *t;

  t = pathname;
  seg_start = pathname;
  for ( t = pathname;; t++)
  {
    if ( *t == '.' || *t == 0)
    {
      strncpy( segname[segments], seg_start, t - seg_start);
      segname[segments][t - seg_start] = 0;
      if ( strcmp( segname[segments], "") == 0)
        return USER__INVSYSNAME;
      segments++;
      if ( segments >= int(sizeof(segname)/sizeof(segname[0])))
        break;
      seg_start = t + 1;
      if ( *t == 0)
        break;
    }
  }
  return 1;
}

SystemName *SystemName::parent()
{
  char parent_name[80];

  if ( segments == 1)
    return 0;

  strcpy( parent_name, segname[0]);
  for ( int i = 1; i < segments - 1; i++)
  {
    strcat( parent_name, ".");
    strcat( parent_name, segname[i]);
  }

  return new SystemName( parent_name);
}

char *GeUser::get_status( int sts)
{
  static char str[80];

  switch ( sts)
  {
    case USER__SUCCESS :
      strcpy( str, "Success");
      break;
    case USER__INVSYSNAME :
      strcpy( str, "Invalid system name");
      break;
    case USER__NOSUCHSYSTEM :
      strcpy( str, "No such system");
      break;
    case USER__NOSUCHUSER :
      strcpy( str, "No such user");
      break;
    case USER__NOTAUTHORIZED :
      strcpy( str, "User not autorized");
      break;
    case USER__USERALREXIST :
      strcpy( str, "User already exist");
      break;
    case USER__SYSTEMALREXIST :
      strcpy( str, "System already exist");
      break;
    case USER__SYSTEMNOTEMPTY :
      strcpy( str, "System is not empty");
      break;
    default :
      strcpy( str, "Undefined message");
  }
  return str;
}

// Fix because of bug in the cc-compiler on VMS
static int check_file( char *filename)
{
  FILE *fp;

  fp = fopen( filename, "r");
  if ( !fp)
    return 0;

  fclose( fp);
  return 1;
}

void GeUser::priv_to_string( unsigned int priv, char *str, int size)
{
  char buff[300];

  buff[0] = 0;
  if ( priv & pwr_mPrv_RtRead) strcat(buff, "RtRead ");
  if ( priv & pwr_mPrv_RtWrite) strcat(buff, "RtWrite ");
  if ( priv & pwr_mPrv_System) strcat(buff, "System ");
  if ( priv & pwr_mPrv_Maintenance) strcat(buff, "Maintenance ");
  if ( priv & pwr_mPrv_Process) strcat(buff, "Process ");
  if ( priv & pwr_mPrv_Instrument) strcat(buff, "Instrument ");
  if ( priv & pwr_mPrv_Operator1) strcat(buff, "Operator1 ");
  if ( priv & pwr_mPrv_Operator2) strcat(buff, "Operator2 ");
  if ( priv & pwr_mPrv_Operator3) strcat(buff, "Operator3 ");
  if ( priv & pwr_mPrv_Operator4) strcat(buff, "Operator4 ");
  if ( priv & pwr_mPrv_Operator5) strcat(buff, "Operator5 ");
  if ( priv & pwr_mPrv_Operator6) strcat(buff, "Operator6 ");
  if ( priv & pwr_mPrv_Operator7) strcat(buff, "Operator7 ");
  if ( priv & pwr_mPrv_Operator8) strcat(buff, "Operator8 ");
  if ( priv & pwr_mPrv_Operator9) strcat(buff, "Operator9 ");
  if ( priv & pwr_mPrv_Operator10) strcat(buff, "Operator10 ");
  if ( priv & pwr_mPrv_DevRead) strcat(buff, "DevRead ");
  if ( priv & pwr_mPrv_DevPlc) strcat(buff, "DevPlc ");
  if ( priv & pwr_mPrv_DevConfig) strcat(buff, "DevConfig ");
  if ( priv & pwr_mPrv_DevClass) strcat(buff, "DevClass ");
  strncpy( str, buff, size);
  str[size-1] = 0;
}

void GeUser::rt_priv_to_string( unsigned int priv, char *str, int size)
{
  char buff[300];

  buff[0] = 0;
  if ( priv & pwr_mPrv_RtRead) strcat(buff, "RtRead ");
  if ( priv & pwr_mPrv_RtWrite) strcat(buff, "RtWrite ");
  if ( priv & pwr_mPrv_System) strcat(buff, "System ");
  if ( priv & pwr_mPrv_Maintenance) strcat(buff, "Maintenance ");
  if ( priv & pwr_mPrv_Process) strcat(buff, "Process ");
  if ( priv & pwr_mPrv_Instrument) strcat(buff, "Instrument ");
  if ( priv & pwr_mPrv_Operator1) strcat(buff, "Operator1 ");
  if ( priv & pwr_mPrv_Operator2) strcat(buff, "Operator2 ");
  if ( priv & pwr_mPrv_Operator3) strcat(buff, "Operator3 ");
  if ( priv & pwr_mPrv_Operator4) strcat(buff, "Operator4 ");
  if ( priv & pwr_mPrv_Operator5) strcat(buff, "Operator5 ");
  if ( priv & pwr_mPrv_Operator6) strcat(buff, "Operator6 ");
  if ( priv & pwr_mPrv_Operator7) strcat(buff, "Operator7 ");
  if ( priv & pwr_mPrv_Operator8) strcat(buff, "Operator8 ");
  if ( priv & pwr_mPrv_Operator9) strcat(buff, "Operator9 ");
  if ( priv & pwr_mPrv_Operator10) strcat(buff, "Operator10 ");
  strncpy( str, buff, size);
  str[size-1] = 0;
}

void GeUser::dev_priv_to_string( unsigned int priv, char *str, int size)
{
  char buff[300];

  buff[0] = 0;
  if ( priv & pwr_mPrv_DevRead) strcat(buff, "DevRead ");
  if ( priv & pwr_mPrv_DevPlc) strcat(buff, "DevPlc ");
  if ( priv & pwr_mPrv_DevConfig) strcat(buff, "DevConfig ");
  if ( priv & pwr_mPrv_DevClass) strcat(buff, "DevClass ");
  strncpy( str, buff, size);
  str[size-1] = 0;
}
 
SystemList *GeUser::get_system( UserList *user)
{
  SystemList *sl = root;
  while( sl) {
    UserList *ul = sl->userlist;
    while ( ul) {
      if ( ul == user)
	return sl;
      ul = ul->next;
    }
    SystemList *found_sl = get_system_child( sl, user);
    if ( found_sl)
      return found_sl;
    sl = sl->next;
  }

  return 0;
}

SystemList *GeUser::get_system_child( SystemList *system, UserList *user)
{
  SystemList *sl = system->childlist;
  while( sl) {
    UserList *ul = sl->userlist;
    while ( ul) {
      if ( ul == user)
	return sl;
      ul = ul->next;
    }
    SystemList *found_sl = get_system_child( sl, user);
    if ( found_sl)
      return found_sl;
    sl = sl->next;
  }

  return 0;
}



bool GeUser::get_system_name( SystemList *system, char *name)
{
  SystemList *sl = root;
  while( sl) {
    if ( sl == system) {
      strcpy( name, sl->name);
      return true;
    }
    if ( get_system_name_child( sl, system, name)) {
      char tmp[256];
      strcpy( tmp, name);
      strcpy( name, sl->name);
      strcat( name, ".");
      strcat( name, tmp);
      return true;
    }
    sl = sl->next;
  }
  return false;
}

bool GeUser::get_system_name_child( SystemList *s, SystemList *system, char *name)
{
  SystemList *sl = s->childlist;
  while( sl) {
    if ( sl == system) {
      strcpy( name, sl->name);
      return true;
    }
    if ( get_system_name_child( sl, system, name)) {
      char tmp[256];
      strcpy( tmp, name);
      strcpy( name, sl->name);
      strcat( name, ".");
      strcat( name, tmp);
      return true;
    }
    sl = sl->next;
  }
  return false;
}

