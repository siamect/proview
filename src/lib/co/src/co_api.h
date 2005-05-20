#ifndef co_api_h
#define co_api_h

#if defined __cplusplus
extern "C" {
#endif

#if !defined co_nav_crr_h
typedef enum {
	navc_eItemType_Crossref,
	navc_eItemType_Header,
	navc_eItemType_Text
	} navc_eItemType;
#endif

#if !defined co_msgwindow_h
typedef enum {
  msgw_ePop_No,
  msgw_ePop_Yes,
  msgw_ePop_Default
} msgw_ePop;
#endif

#if !defined co_nav_help_h
typedef enum {
	navh_eHelpFile_,
	navh_eHelpFile_Base,
	navh_eHelpFile_Project,
	navh_eHelpFile_Other
	} navh_eHelpFile;
#endif

int user_CheckUser( char *systemgroup, char *user, char *password, 
	unsigned int *priv);
int user_CheckSystemGroup( char *systemgroup);
int user_GetUserPriv( char *systemgroup, char *user, unsigned int *priv);
void user_PrivToString( unsigned int priv, char *str, int size);
void user_RtPrivToString( unsigned int priv, char *str, int size);
void user_DevPrivToString( unsigned int priv, char *str, int size);

void lng_get_uid( char *in, char *out);
char *lng_translate( char *str);
void lng_set( char *str);
char *lng_get_language_str();

int crr_signal( void *parent_ctx, char *signalname,
		void (*insert_cb)( void *, void *, navc_eItemType, char *, char *, int),
		int (*name_to_objid_cb)( void *, char *, pwr_tObjid *),
		int (*get_volume_cb)( void *, pwr_tVolumeId *));
int crr_object( void *parent_ctx, char *objectname,
		void (*insert_cb)( void *, void *, navc_eItemType, char *, char *, int),
		int (*name_to_objid_cb)( void *, char *, pwr_tObjid *),
		int (*get_volume_cb)( void *, pwr_tVolumeId *));

void msgw_message( int severity, char *text, msgw_ePop pop);
void msgw_message_sts( pwr_tStatus sts, char *text1, char *text2);
void msgw_message_object( pwr_tStatus sts, char *text1, char *text2, pwr_tOid oid);
void msgw_message_plcobject( pwr_tStatus sts, char *text1, char *text2, pwr_tOid oid);
void msgw_set_nodraw();
void msgw_reset_nodraw();
int xhelp_help( char *key, char *help_bookmark, navh_eHelpFile file_type,
		char *file_name, int strict);
int crr_signal( void *parent_ctx, char *signalname,
		void (*insert_cb)( void *, void *, navc_eItemType, char *, char *, int),
		int (*name_to_objid_cb)( void *, char *, pwr_tObjid *),
		int (*get_volume_cb)( void *, pwr_tVolumeId *));
int crr_object( void *parent_ctx, char *objectname,
		void (*insert_cb)( void *, void *, navc_eItemType, char *, char *, int),
		int (*name_to_objid_cb)( void *, char *, pwr_tObjid *),
		int (*get_volume_cb)( void *, pwr_tVolumeId *));
#if defined __cplusplus
}
#endif
#endif








