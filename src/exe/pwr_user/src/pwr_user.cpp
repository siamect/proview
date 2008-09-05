

#include <iostream.h>
#include <fstream.h>
#include <string.h>

#include "pwr.h"
#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_dcli_input.h"
#include "co_api_user.h"
#include "co_user.h"

static GeUser *gu;
static dcli_sChannel 	user_chn;
static dcli_sRecall 	*user_recall_buf;
static unsigned int user_priv = 0;

static void user_message( char severity, char *msg);
static int	user_help_func(	void		*client_data,
				void		*client_flag);
static int	user_load_func(	void		*client_data,
				void		*client_flag);
static int	user_save_func(	void		*client_data,
				void		*client_flag);
static int	user_list_func(	void		*client_data,
				void		*client_flag);
static int	user___list_func( void		*client_data,
				void		*client_flag);
static int	user_quit_func(	void		*client_data,
				void		*client_flag);
static int	user_add_func(	void		*client_data,
				void		*client_flag);
static int	user_remove_func( void		*client_data,
				void		*client_flag);
static int	user_modify_func( void		*client_data,
				void		*client_flag);
static int	user_get_func(	void		*client_data,
				void		*client_flag);
static int	user_login_func( void		*client_data,
				void		*client_flag);


dcli_tCmdTable	user_command_table[] = {
		{
			"help",
			&user_help_func,
			{ "dcli_arg1", "dcli_arg2", ""}
		},
		{
			"modify",
			&user_modify_func,
			{ "dcli_arg1", "dcli_arg2", "/user", "/group", 
			  "/password", "/privilege" , "/description", "/fullname",
			  "/email", "/phone", "/sms",
			  "/rtread", "/rtwrite", "/rtevents", "/system", 
			  "/maintenance", "/process", "/instrument", 
			  "/operator1", "/operator2", "/operator3", 
			  "/operator4", "/operator5", "/operator6", 
			  "/operator7", "/operator8", "/operator9", 
			  /* "/oper10",*/ "/devread", "/devplc", 
			  "/devconfig", "/devclass", "/sevread", "/sevadmin",
			  "/nortread", "/nortwrite", "/nortevents", 
			  "/nosystem", 
			  "/nomaintenance", "/noprocess", "/noinstrument", 
			  "/nooperator1", "/nooperator2", "/nooperator3", 
			  "/nooperator4", "/nooperator5", "/nooperator6", 
			  "/nooperator7", "/nooperator8", "/nooperator9", 
			  /* "/nooper10",*/ "/nodevread", "/nodevplc", 
			  "/nodevconfig", "/nodevclass", "/nosevread", "/nosevadmin",
			  "/nouserinherit", "/userinherit",
			  ""}
		},
		{
			"add",
			&user_add_func,
			{ "dcli_arg1", "dcli_arg2", "/user", "/group", 
			  "/password", "/privilege" , "/fullname", "/description",
			  "/email", "/phone", "/sms",
			  "/rtread", "/rtwrite", "/rtevents", "/system", 
			  "/maintenance", "/process", "/instrument", 
			  "/operator1", "/operator2", "/operator3", 
			  "/operator4", "/operator5", "/operator6", 
			  "/operator7", "/operator8", "/operator9", 
			  "/oper10", "/devread", "/devplc", 
			  "/devconfig", "/devclass", "/sevread", "/sevadmin",
			  "/nouserinherit", "/userinherit",
			  ""}
		},
		{
			"remove",
			&user_remove_func,
			{ "dcli_arg1", "dcli_arg2", "dcli_arg3", "/group",
			 "/user", ""}
		},
		{
			"get",
			&user_get_func,
			{ "dcli_arg1", "/user", "/group",
				"/password", ""}
		},
		{
			"list",
			&user_list_func,
			{ "dcli_arg1", "/user", "/group", "/brief",
			  ""}
		},
		{
			"show",
			&user_list_func,
			{ "dcli_arg1", "/user", "/group", "/brief",
			  ""}
		},
		{
			"__list",
			&user___list_func,
			{ "dcli_arg1", "/user", "/group", "/brief",
			  ""}
		},
		{
			"save",
			&user_save_func,
			{ ""}
		},
		{
			"load",
			&user_load_func,
			{ ""}
		},
		{
			"quit",
			&user_quit_func,
			{ ""}
		},
		{
			"login",
			&user_login_func,
			{ "dcli_arg1", "dcli_arg2", "/administrator",
			  ""}
		},
		{"",}};

static int	user_help_func(	void		*client_data,
				void		*client_flag)
{
  char	arg1_str[80];
	
  if ( EVEN( dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str))))
  {
    cout <<
"pwr_user help" << endl << endl <<
"load			Reload user database." << endl <<
"save			Save user database." << endl <<
"list			List user database." << endl <<
"show			Synonym to list." << endl <<
"quit			Exit program." << endl <<
"add			Add user or system group." << endl <<
"remove			Remove user or system group." << endl <<
"modify			Modify user." << endl <<
"get			Get user data." << endl <<
"login			Login as administrator." << endl << endl <<
"print   \"help 'command'\" to get help for a specific command." << endl << endl;
  }
  else if ( strncmp( arg1_str, "load", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"load			Reload user database." << endl << endl;
  }
  else if ( strncmp( arg1_str, "save", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"save			Save user database." << endl << endl;
  }
  else if ( strncmp( arg1_str, "list", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"list			List user database." << endl << endl;
  }
  else if ( strncmp( arg1_str, "quit", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"quit			Exit program." << endl << endl;
  }
  else if ( strncmp( arg1_str, "add", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"add group 'systemgroup' /userinherit		Add system group." << endl <<
"add user 'user' /group= /password= 		Add user." << endl <<
"	/fullname= /description= /email= /phone= /sms=" << endl <<
"	/rtread /rtwrite /rtevents /system /maintenance" << endl <<
"	/process /instrument /operator1 /operator2" << endl <<
"	/operator3 /operator4 /operator5 /operator6" << endl <<
"	/operator7 /operator8 /operator9 /oper10" << endl <<
"	/devread /devplc /devconfig /devclass /sevread /sevadmin" << endl <<
"	[/privilge=] " << endl << endl;
  }
  else if ( strncmp( arg1_str, "remove", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"remove group 'systemgroup' 		Remove system group." << endl <<
"remove user 'user' /group= 		Remove user." << endl << endl;
  }
  else if ( strncmp( arg1_str, "modify", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"modify user 'user' /group= /password= 		Modify user." << endl <<
"	/fullname= /description= /email= /phone= /sms=" << endl <<
"	/rtread /rtwrite /rtevents /system /maintenance" << endl <<
"	/process /instrument /operator1 /operator2" << endl <<
"	/operator3 /operator4 /operator5 /operator6" << endl <<
"	/operator7 /operator8 /operator9 /oper10" << endl <<
"	/devread /devplc /devconfig /devclass /sevread /sevadmin" << endl <<
"	[/privilge=] " << endl << endl;
  }
  else if ( strncmp( arg1_str, "get", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"get 'user' /group= /password= 	Get user data." << endl << endl;
  }
  else if ( strncmp( arg1_str, "login", strlen( arg1_str)) == 0)
  {
    cout <<
"pwr_user help" << endl << endl <<
"login	'username' 'password'	    Login as administrator." << endl << endl;
  }
  else
  {
    cout << "No help on this subject" << endl << endl;
  }
  return 1;
}

static int	user_load_func(	void		*client_data,
				void		*client_flag)
{
  char filename[120];
  int sts;

  delete gu;
  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  gu->load( filename);
  return 1;
}

static int	user_save_func(	void		*client_data,
				void		*client_flag)
{
  char filename[120];
  int sts;

  if ( !(user_priv & pwr_mPrv_Administrator)) {
    user_message( 'E', "Not authorized for this operation");
    return 1;
  }

  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  gu->save( filename);
  return 1;
}

static int	user_list_func(	void		*client_data,
				void		*client_flag)
{
  int brief = ODD( dcli_get_qualifier( "/brief", 0, 0));

  gu->print( brief);
  return 1;
}

static int	user___list_func( void		*client_data,
				void		*client_flag)
{
  if ( !(user_priv & pwr_mPrv_Administrator)) {
    user_message( 'E', "Not authorized for this operation");
    return 1;
  }

  int brief = ODD( dcli_get_qualifier( "/brief", 0, 0));

  gu->print_all( brief);
  return 1;
}

static int	user_quit_func(	void		*client_data,
				void		*client_flag)
{
  dcli_input_end( &user_chn, user_recall_buf);
  exit(0);
  return 1;
}

static int	user_add_func(	void		*client_data,
				void		*client_flag)
{
  char	arg1_str[80];
  int	arg1_sts;
  int	sts;
	
  if ( !(user_priv & pwr_mPrv_Administrator)) {
    user_message( 'E', "Not authorized for this operation");
    return 1;
  }

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "user", strlen( arg1_str)) == 0)
  {
    // Command is "add user"
    char	user_str[80];
    char	system_str[80];
    char	password_str[80];
    char	privilege_str[80];
    char	description_str[80];
    char	fullname_str[80];
    char	email_str[80];
    char	phone_str[40];
    char	sms_str[40];
    char 	*description_p = description_str;
    char 	*fullname_p = fullname_str;
    char 	*email_p = email_str;
    char 	*phone_p = phone_str;
    char 	*sms_p = sms_str;
    unsigned int privilege;
    int		nr;

    if ( EVEN( dcli_get_qualifier( "/user", user_str, sizeof(user_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", user_str, sizeof(user_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      user_message('E',"Enter group");
      return 1;
    }
    if ( EVEN( dcli_get_qualifier( "/password", password_str, sizeof(password_str))))
    {
      user_message('E',"Enter password");
      return 1;
    }
    if ( EVEN( dcli_get_qualifier( "/description", description_str, sizeof(description_str))))
      description_p = 0;
    if ( EVEN( dcli_get_qualifier( "/fullname", fullname_str, sizeof(fullname_str))))
      fullname_p = 0;
    if ( EVEN( dcli_get_qualifier( "/email", email_str, sizeof(email_str))))
      email_p = 0;
    if ( EVEN( dcli_get_qualifier( "/phone", fullname_str, sizeof(phone_str))))
      phone_p = 0;
    if ( EVEN( dcli_get_qualifier( "/sms", sms_str, sizeof(sms_str))))
      sms_p = 0;

    privilege = 0;
    if ( ODD( dcli_get_qualifier( "/rtread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtRead;
    if ( ODD( dcli_get_qualifier( "/rtwrite", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtWrite;
    if ( ODD( dcli_get_qualifier( "/rtevents", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtEvents;
    if ( ODD( dcli_get_qualifier( "/system", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_System;
    if ( ODD( dcli_get_qualifier( "/maintenance", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Maintenance;
    if ( ODD( dcli_get_qualifier( "/process", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Process;
    if ( ODD( dcli_get_qualifier( "/instrument", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Instrument;
    if ( ODD( dcli_get_qualifier( "/operator1", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator1;
    if ( ODD( dcli_get_qualifier( "/operator2", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator2;
    if ( ODD( dcli_get_qualifier( "/operator3", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator3;
    if ( ODD( dcli_get_qualifier( "/operator4", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator4;
    if ( ODD( dcli_get_qualifier( "/operator5", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator5;
    if ( ODD( dcli_get_qualifier( "/operator6", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator6;
    if ( ODD( dcli_get_qualifier( "/operator7", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator7;
    if ( ODD( dcli_get_qualifier( "/operator8", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator8;
    if ( ODD( dcli_get_qualifier( "/operator9", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator9;
    if ( ODD( dcli_get_qualifier( "/oper10", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator10;
    if ( ODD( dcli_get_qualifier( "/devread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevRead;
    if ( ODD( dcli_get_qualifier( "/devplc", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevPlc;
    if ( ODD( dcli_get_qualifier( "/devconfig", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevConfig;
    if ( ODD( dcli_get_qualifier( "/devclass", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevClass;
    if ( ODD( dcli_get_qualifier( "/sevread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_SevRead;
    if ( ODD( dcli_get_qualifier( "/sevadmin", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_SevAdmin;
    if ( privilege == 0)
    {
      if ( EVEN( dcli_get_qualifier( "/privilege", privilege_str, sizeof(privilege_str))))
      {
        user_message('E',"Enter privilege");
        return 1;
      }
      nr = sscanf( privilege_str, "%u", &privilege);
      if ( nr != 1)
      {
        user_message('E', "Syntax error in privilege");
        return 1;
      }
    }
    cdh_ToLower( user_str, user_str);
    cdh_ToLower( system_str, system_str);
    cdh_ToLower( password_str, password_str);
    sts = gu->add_user( system_str, user_str, UserList::pwcrypt(password_str), privilege, fullname_p, description_p,
			email_p, phone_p, sms_p);
    cout << gu->get_status(sts) << endl;
  }
  else if ( strncmp( arg1_str, "group", strlen( arg1_str)) == 0)
  {
    // Command is "add group"
    char	system_str[80];
    char	dummy_str[80];
    unsigned long	attributes;
    char	description_str[80];
    char 	*description_p = description_str;

    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", system_str, sizeof(system_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    attributes = user_mSystemAttr_UserInherit;
    if ( ODD( dcli_get_qualifier( "/nouserinherit", dummy_str, sizeof(dummy_str))))
      attributes &= ~user_mSystemAttr_UserInherit;

    if ( EVEN( dcli_get_qualifier( "/description", description_str, sizeof(description_str))))
      description_p = 0;

    cdh_ToLower( system_str, system_str);
    sts = gu->add_system( system_str, attributes, description_p);
    cout << gu->get_status(sts) << endl;
  }
  else
    user_message('E',"Syntax error");
  return 1;
}

static int	user_remove_func( void		*client_data,
				void		*client_flag)
{
  char	arg1_str[80];
  int	arg1_sts;
  int	sts;
	
  if ( !(user_priv & pwr_mPrv_Administrator)) {
    user_message( 'E', "Not authorized for this operation");
    return 1;
  }

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "user", strlen( arg1_str)) == 0)
  {
    // Command is "remove user"
    char	user_str[80];
    char	system_str[80];

    if ( EVEN( dcli_get_qualifier( "/user", user_str, sizeof(user_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", user_str, sizeof(user_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      user_message('E',"Enter group");
      return 1;
    }
    sts = gu->remove_user( system_str, user_str);
    cout << gu->get_status(sts) << endl;
  }
  else if ( strncmp( arg1_str, "group", strlen( arg1_str)) == 0)
  {
    // Command is "remove group"
    char	system_str[80];

    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", system_str, sizeof(system_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    sts = gu->remove_system( system_str);
    cout << gu->get_status(sts) << endl;
  }
  else
    user_message('E',"Syntax error");
  return 1;
}

static int	user_modify_func( void		*client_data,
				void		*client_flag)
{
  char	arg1_str[80];
  int	arg1_sts;
  int	sts;
	
  if ( !(user_priv & pwr_mPrv_Administrator)) {
    user_message( 'E', "Not authorized for this operation");
    return 1;
  }

  arg1_sts = dcli_get_qualifier( "dcli_arg1", arg1_str, sizeof(arg1_str));

  if ( strncmp( arg1_str, "user", strlen( arg1_str)) == 0)
  {
    // Command is "modify user"
    char	user_str[80];
    char	system_str[80];
    char	password_str[80];
    char	p_str[80];
    char	privilege_str[80];
    char	description_str[80];
    char	fullname_str[80];
    char	email_str[80];
    char	phone_str[40];
    char	sms_str[40];
    char 	*description_p = description_str;
    char 	*fullname_p = fullname_str;
    char 	*email_p = email_str;
    char 	*phone_p = phone_str;
    char 	*sms_p = sms_str;
    unsigned int privilege;
    int		nr;
    pwr_tOix 	id;

    if ( EVEN( dcli_get_qualifier( "/user", user_str, sizeof(user_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", user_str, sizeof(user_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      user_message('E',"Enter group");
      return 1;
    }

    cdh_ToLower( user_str, user_str);
    cdh_ToLower( system_str, system_str);

    sts = gu->get_user_data( system_str, user_str, password_str, &privilege, &id, fullname_str, description_str,
			     email_str, phone_str, sms_str);
    if ( EVEN(sts))
    {
      cout << gu->get_status(sts) << endl;
      return 1;
    }
    if ( ODD( dcli_get_qualifier( "/password", p_str, sizeof(p_str)))) {
      strcpy( password_str, p_str);
      cdh_ToLower( password_str, password_str);
      strcpy( password_str, UserList::pwcrypt(password_str));
    }
    if ( EVEN( dcli_get_qualifier( "/description", description_str, sizeof(description_str))))
      description_p = 0;
    if ( EVEN( dcli_get_qualifier( "/fullname", fullname_str, sizeof(fullname_str))))
      fullname_p = 0;
    if ( EVEN( dcli_get_qualifier( "/email", email_str, sizeof(email_str))))
      email_p = 0;
    if ( EVEN( dcli_get_qualifier( "/phone", phone_str, sizeof(phone_str))))
      phone_p = 0;
    if ( EVEN( dcli_get_qualifier( "/sms", sms_str, sizeof(sms_str))))
      sms_p = 0;

    if ( ODD( dcli_get_qualifier( "/rtread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtRead;
    if ( ODD( dcli_get_qualifier( "/rtwrite", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtWrite;
    if ( ODD( dcli_get_qualifier( "/rtevents", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_RtEvents;
    if ( ODD( dcli_get_qualifier( "/system", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_System;
    if ( ODD( dcli_get_qualifier( "/maintenance", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Maintenance;
    if ( ODD( dcli_get_qualifier( "/process", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Process;
    if ( ODD( dcli_get_qualifier( "/instrument", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Instrument;
    if ( ODD( dcli_get_qualifier( "/operator1", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator1;
    if ( ODD( dcli_get_qualifier( "/operator2", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator2;
    if ( ODD( dcli_get_qualifier( "/operator3", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator3;
    if ( ODD( dcli_get_qualifier( "/operator4", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator4;
    if ( ODD( dcli_get_qualifier( "/operator5", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator5;
    if ( ODD( dcli_get_qualifier( "/operator6", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator6;
    if ( ODD( dcli_get_qualifier( "/operator7", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator7;
    if ( ODD( dcli_get_qualifier( "/operator8", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator8;
    if ( ODD( dcli_get_qualifier( "/operator9", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator9;
    if ( ODD( dcli_get_qualifier( "/oper10", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_Operator10;
    if ( ODD( dcli_get_qualifier( "/devread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevRead;
    if ( ODD( dcli_get_qualifier( "/devplc", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevPlc;
    if ( ODD( dcli_get_qualifier( "/devconfig", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevConfig;
    if ( ODD( dcli_get_qualifier( "/devclass", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_DevClass;
    if ( ODD( dcli_get_qualifier( "/sevread", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_SevRead;
    if ( ODD( dcli_get_qualifier( "/sevadmin", privilege_str, sizeof(privilege_str))))
      privilege |= pwr_mPrv_SevAdmin;

    if ( ODD( dcli_get_qualifier( "/nortread", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_RtRead;
    if ( ODD( dcli_get_qualifier( "/nortwrite", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_RtWrite;
    if ( ODD( dcli_get_qualifier( "/nortevents", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_RtEvents;
    if ( ODD( dcli_get_qualifier( "/nosystem", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_System;
    if ( ODD( dcli_get_qualifier( "/nomaintenance", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Maintenance;
    if ( ODD( dcli_get_qualifier( "/noprocess", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Process;
    if ( ODD( dcli_get_qualifier( "/noinstrument", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Instrument;
    if ( ODD( dcli_get_qualifier( "/nooperator1", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator1;
    if ( ODD( dcli_get_qualifier( "/nooperator2", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator2;
    if ( ODD( dcli_get_qualifier( "/nooperator3", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator3;
    if ( ODD( dcli_get_qualifier( "/nooperator4", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator4;
    if ( ODD( dcli_get_qualifier( "/nooperator5", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator5;
    if ( ODD( dcli_get_qualifier( "/nooperator6", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator6;
    if ( ODD( dcli_get_qualifier( "/nooperator7", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator7;
    if ( ODD( dcli_get_qualifier( "/nooperator8", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator8;
    if ( ODD( dcli_get_qualifier( "/nooperator9", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator9;
    if ( ODD( dcli_get_qualifier( "/nooper10", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_Operator10;
    if ( ODD( dcli_get_qualifier( "/nodevread", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_DevRead;
    if ( ODD( dcli_get_qualifier( "/nodevplc", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_DevPlc;
    if ( ODD( dcli_get_qualifier( "/nodevconfig", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_DevConfig;
    if ( ODD( dcli_get_qualifier( "/nodevclass", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_DevClass;
    if ( ODD( dcli_get_qualifier( "/nosevread", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_SevRead;
    if ( ODD( dcli_get_qualifier( "/nosevadmin", privilege_str, sizeof(privilege_str))))
      privilege &= ~pwr_mPrv_SevAdmin;

    if ( privilege == 0)
    {
      if ( EVEN( dcli_get_qualifier( "/privilege", privilege_str, sizeof(privilege_str))))
      {
        user_message('E',"Enter privilege");
        return 1;
      }
      nr = sscanf( privilege_str, "%u", &privilege);
      if ( nr != 1)
      {
        user_message('E', "Syntax error in privilege");
        return 1;
      }
    }

    sts = gu->modify_user( system_str, user_str, password_str, privilege, fullname_p, description_p,
			   email_p, phone_p, sms_p);
    cout << gu->get_status(sts) << endl;
  }
  else if ( strncmp( arg1_str, "group", strlen( arg1_str)) == 0)
  {
    // Command is "modify group"
    char	system_str[80];
    char	dummy_str[80];
    unsigned int attributes;
    char	description_str[80];
    char 	*description_p = description_str;
    pwr_tOix	id;
    
    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg2", system_str, sizeof(system_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    cdh_ToLower( system_str, system_str);

    sts = gu->get_system_data( system_str, &attributes, &id, description_str);
    if ( EVEN(sts))
    {
      cout << gu->get_status(sts) << endl;
      return 1;
    }
    if ( ODD( dcli_get_qualifier( "/nouserinherit", dummy_str, sizeof(dummy_str))))
      attributes &= ~user_mSystemAttr_UserInherit;
    if ( ODD( dcli_get_qualifier( "/userinherit", dummy_str, sizeof(dummy_str))))
      attributes |= user_mSystemAttr_UserInherit;
    if ( EVEN( dcli_get_qualifier( "/description", description_str, sizeof(description_str))))
      description_p = 0;
    sts = gu->modify_system( system_str, attributes, description_p);
    cout << gu->get_status(sts) << endl;
  }
  else
    user_message('E',"Syntax error");
  return 1;
}

static int	user_get_func(	void		*client_data,
				void		*client_flag)
{
    // Command is "get"
    char	user_str[80];
    char	system_str[80];
    char	password_str[80];
    unsigned int privilege;
    int		sts;

    if ( EVEN( dcli_get_qualifier( "/user", user_str, sizeof(user_str))))
    {
      if ( EVEN( dcli_get_qualifier( "dcli_arg1", user_str, sizeof(user_str))))
      {
        user_message('E',"Syntax error");
        return 1;
      }
    }
    if ( EVEN( dcli_get_qualifier( "/group", system_str, sizeof(system_str))))
    {
      user_message('E',"Enter group");
      return 1;
    }
    if ( EVEN( dcli_get_qualifier( "/password", password_str, sizeof(password_str))))
    {
      user_message('E',"Enter password");
      return 1;
    }

    cdh_ToLower( user_str, user_str);
    cdh_ToLower( system_str, system_str);
    cdh_ToLower( password_str, password_str);

    sts = gu->get_user( system_str, user_str, UserList::pwcrypt(password_str), &privilege);
    if ( EVEN( sts))
      cout << "Error: " << gu->get_status(sts) << endl;
    else {
      char str[200];

      GeUser::priv_to_string( privilege, str, sizeof(str));
      cout << "Success: " << "Privileges " << str << endl;      
    }
    return 1;
}

static int	user_login_func(	void		*client_data,
					void		*client_flag)
{
  char username_str[80];
  char password_str[80];
  unsigned int priv;
  pwr_tStatus sts;
  char msg[200];

  sts = user_CheckSystemGroup( "administrator");
  if ( EVEN(sts)) {
    // Username and password are not required
    user_priv = pwr_mPrv_Administrator;
    user_message('I', "Administrator logged in");
    return 1;
  }
  else {
    // Check user and password in systemgroup "aministrator"
    if ( EVEN( dcli_get_qualifier( "dcli_arg1", username_str, sizeof(username_str)))) {
      user_message('E',"Username and password required");
      return 1;
    }
    
    if ( EVEN( dcli_get_qualifier( "dcli_arg1", password_str, sizeof(password_str)))) {
      user_message('E',"Password required");
      return 1;
    }

    cdh_ToLower( username_str, username_str);
    cdh_ToLower( password_str, password_str);
    strcpy( password_str, UserList::pwcrypt(password_str));
    sts = user_CheckUser( "administrator", username_str, password_str, &priv);
    if ( EVEN(sts))
      user_message('E',"Login failure");
    else {
      user_priv = pwr_mPrv_Administrator;
      sprintf( msg, "User %s logged in", username_str);
      user_message('I', msg);
    }
    return 1;
  }
  return 1;
}

static void user_message( char severity, char *msg)
{
  cout << "%CLI-" << severity << "-MSG, " << msg << endl;
}

int main()
{
  char line[256];
  char filename[120];
  int sts;

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  gu->load( filename);

  sts = dcli_input_init( &user_chn, &user_recall_buf);

  sts = user_CheckSystemGroup( "administrator");
  if ( EVEN(sts)) {
    // Username and password are not required
    user_priv = pwr_mPrv_Administrator;
  }
  else {
    printf("\n Administrator has disabled public write access.\n");
    printf(" Login is required.\n\n");
  }

  for (;;)
  {
    sts = dcli_get_input_command( &user_chn, "pwr> ", line, 
		sizeof(line), user_recall_buf);
//    cout << "pwr_user> ";
//    cin.getline( line, sizeof(line));
    if ( strcmp( line, "") == 0)
      continue;

    // cdh_ToLower( line, line);
    sts = dcli_cli( (dcli_tCmdTable *)&user_command_table, 
		line, 0, 0);
    if ( EVEN(sts))
      cout << "%USER-E-MSG, Syntax error" << endl;
  }
}

