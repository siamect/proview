#ifndef co_msg2cmsg_h
#define co_msg2cmsg_h

void lex_FacName(const char *Facnam);
void lex_FacNum(int Facnum);
void lex_FacPrefix(const char *Prefix);
void lex_MsgName(const char *Name);
void lex_MsgText(const char *Text);
void lex_MsgSeverity(msg_eSeverity Severity);
void lex_LexError(int Lineno, char *Str);

#endif
