/* ds_rtt.h */

#define DTT_CTX_EDIT		1000
#define DTT_CTX_MENU		1001

#define DTT_DISPLAY_MODE_FORMAT	0
#define DTT_DISPLAY_MODE_NUMBER	1

#define DTT_MENUITEM_EDIT	1
#define DTT_MENUITEM_MENU	2
#define DTT_MENUITEM_OBJECTS	4
#define DTT_MENUITEM_EXIT	5
#define DTT_MENUITEM_COMMAND	6
#define DTT_MENUITEM_PIFUNC	7
#define DTT_MENUITEM_SYSPICTURE	8
#define DTT_MENUITEM_OBJPICTURE	9
#define DTT_MENUITEM_VMSCOMMAND	10
#define DTT_MENUITEM_KEYS	11
#define DTT_MENUITEM_PIPERM 	12
#define DTT_MENUITEM_PIFUNCPERM 13
#define DTT_MENUITEM_VMSCOMMANDHOLD 14
#define DTT_MENUITEM_VMSCOMMANDCONF 15
#define DTT_MENUITEM_VMSCOMMANDNOWAIT 16
#define DTT_MENUITEM_FILESPEC 	17
#define DTT_MENUITEM_COMMANDHOLD 18

typedef struct {
	int		number;
	char		text[80];
	char		type[80];
	char		parameter[120];
	char		dualparameter[120];
	char		priv[80];
	char		outflags[80];
	char		characters;
	char		decimals;
	float		maxlimit;
	float		minlimit;
	char		database[10];
	char		declaration[10];
	int		x;
	int		y;
	} dtt_t_upd_item, *dtt_item;

typedef struct {
	unsigned long	ctx_type;
	void		*parent_ctx;
	char		title[80];
	int		chartable[2][80][22];
	unsigned long	direction;
	unsigned long	charset;
	int		cursor_x;
	int		cursor_y;
	int		select_active;
	int		select_x;
	int		select_y;
	dtt_item	upd_items;
	int		index;
	int		display_mode;
	} dtt_t_edit_ctx, *edit_ctx;

typedef struct {
	char		key[80];
	int		idx;
	edit_ctx	ctx;
	} dtt_t_store_ctx;

typedef struct {
	int		key;
	menu_ctx	ctx;
	} dtt_t_store_menuctx;



int	dtt_initialize();

int	dtt_edit_new( 	menu_ctx	parent_ctx, 
			unsigned long	arg1,
			unsigned long	arg2,
			unsigned long	index,
			unsigned long	arg4);

int	dtt_menu_new( 	menu_ctx	parent_ctx,
			rtt_t_menu	**menu_p,
			char		*title,
			unsigned long	userdata,
			unsigned long	flag);
int	dtt_start( char		*programname);
