
#include <stdio.h>

#include "rt_rtt_menu.h"
#include "rt_rtt_functions.h"
#include "rt_rtt_edit.h"


RTT_HELP_START
RTT_HELP_SUBJ("PWR_RTT")
RTT_HELP_INFO("  Välj ut önskat ämne med piltangenterna och tryck på RETURN")
RTT_HELP_TEXT( "\
	Pwr_rtt visar info om databasen.\n\n\
'OBJECT HIERARCHY'	Visar anläggings och node hierarkin.\n\
'EXIT'			Avsluta.")
RTT_HELP_END

RTT_DB_START
RTT_DB_CONTINUE
RTT_DB_END

RTT_MAINMENU_START( "PWR_RTT")
RTT_MENUITEM_EXIT( "EXIT")
{"",0,0,0,{0,0},0,0,0,0}};

int main(int argc, char *argv[])
{
	int 	sts;					
	char		buffername[80];
	char		command[120];
	int		i;

	if ( argc < 2 )
	{
	  printf("Program:  ");
	  scanf("%s", buffername);
	}
	else
	  strcpy( buffername, argv[1]);

	/* Convert name to upper case */
#if defined(OS_VMS)
	rtt_toupper( buffername, buffername);
#endif

	if ( argc > 2 )
	{
	  strcpy( command, argv[2]);
	  for ( i = 3; i < argc; i++)
  	  {
	    strcat( command, " ");
	    strcat( command, argv[i]);
	  } 
	  rtt_toupper( command, command);
	  sts = rtt_commandmode_single( command);
	}

	sts = dtt_initialize();				

	sts = dtt_start( buffername);
	rtt_error_msg(sts);				

	return 1;
}
