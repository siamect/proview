#ifndef rt_rtt_global_h
#define rt_rtt_global_h

/*************************************************************************
*  	Globala variabler i rtt
**************************************************************************/

extern	char			rtt_version[10];
extern	rtt_t_helptext		*rtt_command_helptext;
extern	rtt_t_helptext		*rtt_script_helptext;
extern	rtt_t_helptext		rtt_appl_helptext[];
extern	rtt_t_comtbl		rtt_command_table[];
extern	unsigned long		rtt_chn[4];
extern	rtt_t_recall 		*rtt_recallbuff;
extern	rtt_t_recall		*rtt_value_recallbuff;
extern	menu_ctx		rtt_collectionmenuctx;
extern	rtt_t_menu_upd		*rtt_collectionmenulist;
extern	rtt_t_menu_alarm	*rtt_menu_eventlist;
extern	rtt_t_menu_alarm	*rtt_menu_alarmlist;
extern	menu_ctx		rtt_alarm_ctx;
extern	menu_ctx		rtt_event_ctx;
extern	int			rtt_commandmode;
extern	int			rtt_quiet;
extern	int			rtt_mode_dump;
extern	int			rtt_mode_address;
extern	int			rtt_rtdb_offset;
extern	rtt_t_db 		rtt_appl_db[];
extern	rtt_t_db 		rtt_rttsys_db[];
extern	char			rtt_time[80];
extern	unsigned long		rtt_priv;
extern  char			rtt_ConfigureObject[80];
extern	pwr_tObjid		rtt_UserObject;
extern  char			rtt_DefaultVMSNode[80];
extern  unsigned char		rtt_AlarmAutoLoad;
extern  unsigned char		rtt_AlarmMessage;
extern  unsigned char		rtt_AlarmBeep;
extern  unsigned char		rtt_AlarmReturn;
extern  unsigned char		rtt_AlarmAck;
extern	rtt_t_loggtable		rtt_loggtable[ RTT_LOGG_MAXENTRY];
extern	int 			rtt_gdh_started;
extern	char			rtt_AlarmLastMessage[80];
extern	char			rtt_AlarmText1[80];
extern	char			rtt_AlarmText2[80];
extern	char			rtt_AlarmText3[80];
extern	char			rtt_AlarmText4[80];
extern	char			rtt_AlarmText5[80];
extern	int			rtt_fastkey;
extern 	rtt_t_menu		rtt_mainmenu[];
extern	char			rtt_platform[20];
extern	char			rtt_os[20];
extern	char			rtt_hw[20];
extern	int			rtt_verify;
extern	int			rtt_file_on;
extern	FILE			*rtt_outfile;
extern	int			rtt_print_message;
extern	int			rtt_print_command;
extern	int			rtt_command_level;
extern	char			rtt_node[80];
extern	char			rtt_sys[80];
extern	char			rtt_mainmenu_title[80];
extern	char			rtt_title_prefix[80];
extern	char			rtt_ident[80];
extern	int			rtt_description_on;
extern	char			rtt_default_directory[80];
extern	int			rtt_scantime;
extern	char			rtt_symbolfilename[80];
extern	int			rtt_signal_test_mode;
extern	char			rtt_arg[10][80];
extern	int			rtt_args;
extern  int                     rtt_flash;
extern	int			rtt_message_off;
extern	int			rtt_noredraw;
#ifdef RTT_MENU_MODULE
/* Datasättning endast i modulen rs_rtt_menu */

char				rtt_version[10] = "V4.0.0";
menu_ctx			rtt_collectionmenuctx = 0;
rtt_t_menu_upd			*rtt_collectionmenulist = 0;
menu_ctx			rtt_alarm_ctx = 0;
menu_ctx			rtt_event_ctx = 0;
int				rtt_rtdb_offset = 0;
int				rtt_mode_dump = 0;
int				rtt_mode_address = 0;
int				rtt_commandmode = 0;
int				rtt_quiet = 0;
char				rtt_ConfigureObject[80] = "";
pwr_tObjid			rtt_UserObject = {0,0};
char				rtt_DefaultVMSNode[80] = "";
unsigned char			rtt_AlarmAutoLoad = 0;
unsigned char			rtt_AlarmMessage = 0;
unsigned char			rtt_AlarmBeep = 0;
unsigned char			rtt_AlarmReturn = 0;
unsigned char			rtt_AlarmAck = 0;
rtt_t_loggtable			rtt_loggtable[ RTT_LOGG_MAXENTRY];
int 				rtt_gdh_started = 0;
char				rtt_AlarmLastMessage[80] = "";
char				rtt_AlarmText1[80] = "";
char				rtt_AlarmText2[80] = "";
char				rtt_AlarmText3[80] = "";
char				rtt_AlarmText4[80] = "";
char				rtt_AlarmText5[80] = "";
int				rtt_fastkey = 0;
unsigned long			rtt_chn[4];
rtt_t_recall 			*rtt_recallbuff;
rtt_t_recall			*rtt_value_recallbuff;
unsigned long			rtt_priv;
char				rtt_time[80];
char				rtt_hw[20];
char				rtt_platform[20];
#if defined OS_VMS
char				rtt_os[20] = "vms";
#elif defined OS_ELN
char				rtt_os[20] = "eln";
#elif defined OS_LYNX
char				rtt_os[20] = "lynx";
#elif defined OS_LINUX
char				rtt_os[20] = "linux";
#endif
int				rtt_verify = 0;
int				rtt_file_on = 0;
FILE				*rtt_outfile;
int				rtt_print_message = 0;
int				rtt_print_command = 0;
int				rtt_command_level = 0;
char				rtt_node[80] = "";
char				rtt_sys[80] = "";
char				rtt_mainmenu_title[80] = "";
char				rtt_title_prefix[80] = "";
char				rtt_ident[80] = "";
int				rtt_description_on = 1;
char				rtt_default_directory[80] = "";
int				rtt_scantime = 1000;
char				rtt_symbolfilename[80] = "";
int				rtt_signal_test_mode = 0;
char				rtt_arg[10][80] = {"","","","","","","","","",""};
int				rtt_args = 0;
int                             rtt_flash = 0;
int				rtt_message_off = 0;
int				rtt_noredraw = 0;

#endif
#endif
