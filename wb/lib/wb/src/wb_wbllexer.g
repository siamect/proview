/*
 * Make sure to run antlr.Tool on the lexer.g file first!
 */
options {
	mangleLiteralPrefix = "t_";
	language="Cpp";
}

class wb_wbllexer extends Lexer;
options {
	k=2;
	exportVocab=wb_wblvocab;
	charVocabulary = '\3'..'\377';
	defaultErrorHandler=false;
}

tokens {
	OBJECT;
	ENDOBJECT;
	VOLUME;
	ENDVOLUME;
	SOBJECT;
	ENDSOBJECT;
	BODY;
       	ENDBODY;
	ATTRIBUTE;
	BUFFER;	
	ENDBUFFER;
	DOBJECT;
	ENDDOBJECT; 
	DBUFFER;
	ENDDBUFFER;
	DATTRIBUTE;
	NUM_FLOAT;
	INT;
	OID;
	DOCBLOCK;
}

EQ	:	'='
	;

OREQ
	:	"|="
	;

WS	:	(' '
	|	'\t'
	|	'\n'	{newline();}
	|	'\r')+
		{ _ttype = antlr::Token::SKIP; }
	;

COMMENT
	: 	("!" ( ' ' | '\t') (~'\n')* '\n'
	|	"!" '\n')
		{ _ttype = antlr::Token::SKIP; newline(); }
	;

DOCBLOCK
	:	"!/**"
		(	/*	'\r' '\n' can be matched in one alternative or by matching
				'\r' in one iteration and '\n' in another.  I am trying to
				handle any flavor of newline that comes in, but the language
				that allows both "\r\n" and "\r" and "\n" to all be valid
				newline is ambiguous.  Consequently, the resulting grammar
				must be ambiguous.  I'm shutting this warning off.
			 */
			options {
				generateAmbigWarnings=false;
			}
		:
			{ LA(2)!='*' }? '!'
		|	'\r' '\n'		{newline();}
		|	'\r'			{newline();}
		|	'\n'			{newline();}
		|	~('!'|'\n'|'\r')
		)*
		"!*/"
	;

INDEX
options {
	paraphrase="an index";
}
	:	'['! ('0'..'9')+ ']'!
	;


VALUE
options {
  testLiterals = false;
  paraphrase = "an identifer";
}
/*	:
	 ('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|':')+
	;
*/
	:	("Object" WS) => "Object"! WS! oname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)+
		{$setType(OBJECT);}
	|	("EndObject" WS) => "EndObject" WS!
		{$setType(ENDOBJECT);}
	|	("SObject" WS) => "SObject"! WS! soname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|':'|'-'|SWEC)+
		{$setType(SOBJECT);}
	|	("EndSObject" WS) => "EndSObject" WS!
		{$setType(ENDSOBJECT);}
	|	("DObject" WS) => "DObject"! WS! doname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)+
		{$setType(DOBJECT);}
	|	("EndDObject" WS) => "EndDObject" WS!
		{$setType(ENDDOBJECT);}
	|	("Volume" WS) => "Volume"! WS! vname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)+ (':')?!
		{$setType(VOLUME);}
	|	("EndVolume" WS) => "EndVolume" WS!
		{$setType(ENDVOLUME);}
	|	("Buffer" WS) => "Buffer"! WS! buname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)+
		{$setType(BUFFER);}
	|	("EndBuffer" WS) => "EndBuffer" WS!
		{$setType(ENDBUFFER);}
	|	("Body" WS) => "Body"! WS! boname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)+
		{$setType(BODY);}
	|	("EndBody" WS) => "EndBody" WS!
		{$setType(ENDBODY);}
	|	("Attr" WS) => "Attr"! WS! aname:('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC|'['|']')+ ('.'('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC|'['|']')+)*
		{$setType(ATTRIBUTE);}
	|	(DIGITS '.' DIGITS '.' ) => DIGITS '.' DIGITS '.' DIGITS '.' DIGITS (':' DIGITS)?
		{$setType(OID);}
	|	(DIGITS '.') => DIGITS '.' ('0'..'9')* (('e'|'E') ('+'|'-')? ('0'..'9')+)?
		{$setType(NUM_FLOAT);}
	|	('-' DIGITS '.') => '-' DIGITS '.' ('0'..'9')* (('e'|'E') ('+'|'-')? ('0'..'9')+)?
		{$setType(NUM_FLOAT);}
	|	'.' ('0'..'9')+ (('e'|'E') ('+'|'-')? ('0'..'9')+)?
		{$setType(NUM_FLOAT);}
	|	DIGITS
		{$setType(INT);}
	|	'-' DIGITS
		{$setType(INT);}
	|	('a'..'z'|'A'..'Z'|'$'|'_'|SWEC) ('a'..'z'|'A'..'Z'|'0'..'9'|'$'|'_'|SWEC)*
	;

CHAR_LITERAL
	:	'\'' (ESC|~'\'') '\''
	;

STRING_LITERAL
	:	'"' (ESC|'\n' {newline();} |~'"')* '"'
	;

protected
DIGITS
	:
	('0'..'9')+
	;

protected
ESC	:	'\\'
		(	'n'
		|	'r'
		|	't'
		|	'b'
		|	'f'
		|	'"'
		|	'0'
		|	'\''
		|	'\\'
		)
	;

protected
SWEC	:	'\345' | '\344' | '\366' | '\305' | '\304' | '\326';











