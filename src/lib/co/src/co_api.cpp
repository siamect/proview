
extern "C" {
#include "pwr.h"
#include "co_api.h"
#include "co_dcli.h"
}

#include "co_user.h"
#include "co_lng.h"

//
// c-api to co_user
//

int user_CheckUser( char *systemgroup, char *user, char *password, 
	unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user( systemgroup, user, password, priv);
  delete gu;
  return sts;
}

int user_CheckSystemGroup( char *systemgroup)
{
  GeUser *gu;
  int sts;
  unsigned int attributes;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_system_data( systemgroup, &attributes);
  delete gu;
  return sts;
}

int user_GetUserPriv( char *systemgroup, char *user, unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user_priv( systemgroup, user, priv);
  delete gu;
  return sts;
}

void user_PrivToString( unsigned int priv, char *str, int size)
{
  GeUser::priv_to_string( priv, str, size);
}

void user_RtPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::rt_priv_to_string( priv, str, size);
}

void user_DevPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::dev_priv_to_string( priv, str, size);
}

//
//  c api to co_lng
//

void lng_get_uid( char *in, char *out)
{
  Lng::get_uid( in, out);
}

char *lng_translate( char *str)
{
  return Lng::translate( str);
}

