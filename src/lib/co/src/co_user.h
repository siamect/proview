#ifndef co_user_h
#define co_user_h

/* ge_user.h -- User authorization

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <iostream.h>
#include <fstream.h>
#include <string.h>

#if defined __cplusplus
extern "C" {
#endif

#define user_cFilename 	"pwra_db:pwr_user.dat"
#define user_cVersion 	"V1.0.0"
#define FSPACE 		" "

typedef enum {
	user_eData_GeUser			= 1,
	user_eData_System			= 2,
	user_eData_User				= 3,
	user_eData_End				= 99,
	user_eData_GeUserVersion		= 100,
	user_eData_SystemName			= 200,
	user_eData_SystemLevel			= 201,
	user_eData_SystemAttributes		= 202,
	user_eData_UserName			= 300,
	user_eData_UserPassword			= 301,
	user_eData_UserPrivilege		= 302
	} user_eData;


typedef enum {
	user_mSystemAttr_UserInherit = 1
	} user_mSystemAttr;

class SystemName {
  public:
	SystemName( char *system_name) : segments(0)
	{
	  strcpy( pathname, system_name);
	};
	char	pathname[80];
	char	segname[10][40];
	int	segments;

	int 	parse();
	SystemName *parent();
	char	*name() { return pathname; };
	char	*segment( int idx) { return segname[idx]; };
};


class UserList;
class GeUser;

class SystemList {

  friend class GeUser;
  friend class UserList;

  public:
	SystemList( char *sl_name, int sl_level, 
		unsigned int sl_attributes) :
		level(sl_level), attributes( sl_attributes),
		next(0), childlist(0), userlist(0) 
	{
          strcpy( name, sl_name);
	};


  private:
	char			name[40];
	int			level;
	unsigned long		attributes;
	SystemList		*next;
	SystemList		*childlist;
	UserList		*userlist;

  public:
        int 		load( ifstream& fp);
        void		save( ofstream& fp);
        void 		print();
        void 		print_all();
	void		*find_user( char *name);
        int		add_user( char *user, char *password, unsigned int priv);
        int		add_system( SystemName *name, unsigned int attributes);
        int		load_system( ifstream& fp);
        int 		load_user( ifstream& fp);
	SystemList	*find_system( SystemName *name);
        int		remove_user( char *user);
	int		remove_system( SystemList *sys);
        void		modify( unsigned int attributes);
        void		get_data( unsigned int *attributes);
	SystemList	*first_system() { return childlist;}
	SystemList	*next_system() { return next;}	
	UserList	*first_user() { return userlist;}
	char		*get_name() { return name;}
	unsigned long	get_attributes() { return attributes;}
	~SystemList();
};

class UserList {

  friend class SystemList;
  friend class GeUser;

  public:
	UserList( char *ul_name, char *ul_password, unsigned int ul_priv) :
		priv(ul_priv), next(NULL)
	{
	  strcpy( name, ul_name);
	  strcpy( password, ul_password);
	};


  private:
	char			name[40];
	char			password[40];
	unsigned long		priv;
	UserList		*next;

	char		*crypt( char *str);
	unsigned long	crypt( unsigned long i);
	char		*decrypt( char *str);
	unsigned long	decrypt( unsigned long i);

  public:
        int 		load( ifstream& fp);
        void 		save( ofstream& fp);
        void 		print();
        void 		print_all();
	void		modify( char *password, unsigned int priv);
	int		check_password( char *password);
        void	 	get_data( char *password, unsigned int *priv);
	UserList	*next_user() { return next;}
	char		*get_name() { return name;}
	unsigned long	get_priv() { return priv;}		
};

class GeUser {

    friend class SystemList;
    friend class UserList;

  public:
    GeUser() : root(NULL) { strcpy( version, ""); strcpy( fname, "");}
    ~GeUser();

  private:
    SystemList		*root;
    SystemList		*last_system;
    char		version[20];
    char		fname[256];

    bool 		get_system_name_child( SystemList *s, SystemList *system, char *name);
    SystemList 		*get_system_child( SystemList *system, UserList *user);
    

  public:
    int 		load( char *filename);
    int 		save() { return save( fname);}
    int 		save( char *filename);
    int 		load_system( ifstream& fp);
    void 		print();
    void 		print_all();
    SystemList		*find_system( SystemName *name);
    int			add_system( char *name, unsigned int attributes);
    int			add_user( char *system, char *user, char *password, 
				unsigned int priv);
    int			remove_user( char *system, char *user);
    int			modify_user( char *system, char *user, char *password,
			 unsigned int priv);
    int			get_user_data( char *system, char *user, char *password,
			 unsigned int *priv);
    int			modify_system( char *system, unsigned int attributes);
    int			get_system_data( char *system, unsigned int *attributes);
    int			remove_system( char *name);
    int 		get_user( char *system, char *user, char *password,
				unsigned int *priv);
    int 		get_user_priv( char *system, char *user,
				unsigned int *priv);
    char 		*get_status( int sts);
    SystemList		*root_system() { return root;}
    SystemList		*get_system( UserList *user);
    bool		get_system_name( SystemList *system, char *name);
    static void		priv_to_string( unsigned int priv, char *str, int size);
    static void		rt_priv_to_string( unsigned int priv, char *str, int size);
    static void		dev_priv_to_string( unsigned int priv, char *str, int size);

};

#if defined __cplusplus
}
#endif
#endif








