%{
#include <stdio.h>
#include <stdlib.h>

#include "pwr_msg.h"
#include "msg2cmsg.h"

int lineno = 0;

#undef ECHO
#define ECHO
%}


ws	[ \t]+
nl	\n
comment !.*

%x facnam faccom facnum prefix prefixstr prefixstr_nl msgnam msgtxt msgsev

%%

{ws}		;
{comment}	;
{nl}		lineno++;


\.facility		{BEGIN(facnam);}

<facnam>{comment}	;
<facnam>[A-Z0-9$_]+ 	{BEGIN(faccom); lex_FacName((char *)yytext);};
<facnam>{nl}		{printf("No facility name given!\n"); exit(1);}

<faccom>[ \t]*,[ \t]*	{BEGIN(facnum);}
<faccom>{nl}		{printf("No facility number given!\n"); exit(1);}

<facnum>[0-9]+		{BEGIN(prefix); lex_FacNum(atoi((char *)yytext));}
<facnum>{nl}		{printf("No facility number given!\n"); exit(1);}

<prefix>{comment}	{BEGIN(msgnam); printf("Default prefix\n");}
<prefix>{nl}		{BEGIN(msgnam); printf("Default prefix\n");}
<prefix>\/prefix[ \t]*=[ \t]* {BEGIN(prefixstr);}

<prefixstr>{comment}	;
<prefixstr>[A-Z0-9$_]+ 	{BEGIN(prefixstr_nl); lex_FacPrefix((char *)yytext);}
<prefixstr>{nl}		{printf("No prefix string given!\n"); exit(1);}

<prefixstr_nl>\n	{BEGIN(msgnam);}

<msgnam>{comment}	;
<msgnam>[A-Za-z0-9$_]+ 	{BEGIN(msgtxt); lex_MsgName((char *)yytext);}
<msgnam>\.facility	{BEGIN(facnam);}
<msgnam>\.end		{BEGIN(0);}

<msgtxt>\<.*\>		{BEGIN(msgsev); lex_MsgText((char *)yytext);}

<msgsev>{comment}	{printf("Comment: no severity given"); exit(1);}
<msgsev>\n		{printf("New line: no severity given"); exit(1);}
<msgsev>\/w.*\n	{BEGIN(msgnam);	lineno++; lex_MsgSeverity(msg_eSeverity_Warning);}
<msgsev>\/s.*\n	{BEGIN(msgnam);	lineno++; lex_MsgSeverity(msg_eSeverity_Success);}
<msgsev>\/e.*\n	{BEGIN(msgnam);	lineno++; lex_MsgSeverity(msg_eSeverity_Error);}		
<msgsev>\/i.*\n	{BEGIN(msgnam);	lineno++; lex_MsgSeverity(msg_eSeverity_Info);}		
<msgsev>\/f.*\n	{BEGIN(msgnam);	lineno++; lex_MsgSeverity(msg_eSeverity_Fatal);}		

%%

