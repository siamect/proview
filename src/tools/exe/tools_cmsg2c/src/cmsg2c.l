%{
#include <stdio.h>
#include <stdlib.h>

#include "pwr_msg.h"
#include "cmsg2c.h"

int lineno = 0;

#undef ECHO
#define ECHO

%}


ws	[ \t]+
nl	\n
comment !.*

%x facnam facnum

%%

{ws}		;
{comment}	;
{nl}		lineno++;


msg_sFacility		{BEGIN(facnam);}

<facnam>[A-Z0-9$_]+facility {BEGIN(facnum); lex_FacName((char *)yytext);}

<facnum>[0-9]+		{BEGIN(0); lex_FacNum(atoi((char *)yytext));}


%%

