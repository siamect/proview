
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "pwr.h"
#include "co_dcli.h"
#include "wb_error.h"
#include "wb_wbl_parser.h"
#include "wb_wblnode.h"

using namespace std;

#define WS " 	"
#define WSEOL "$ 	"

typedef enum {
  eState_StartDelim,
  eState_StartDelimFound,
  eState_InToken,
  eState_EndDelimFound
} eState;


#if 0
int main()
{
  wb_wbl_parser p;

  try {
    // p.parse( "/home/claes/test/pwrb_c_and.wb_load");
    p.parse( "/home/claes/test/directory.wb_load");
  }
  catch ( wb_error_str e) {
    p.print_error( e);
  }
  p.print();
}
#endif

wb_wbl_parser::wb_wbl_parser() :
  m_line_cnt(0), m_state(0), m_tree(0), m_current(0), m_object_level(0), m_buffer_level(0)
{
}

wb_wbl_parser::~wb_wbl_parser()
{
  if ( m_tree)
    delete m_tree;
}

void wb_wbl_parser::print_error( wb_error_str& e)
{
  cout << "** Error " << m_fname << " line " << m_line_cnt <<  ", " << e.what() << endl;  
}

void wb_wbl_parser::print()
{
  printf( "Parsing file %s\n", m_fname);
  
  print_node( m_tree, 0);
}

void wb_wbl_parser::print_node( wbl_ast_node *n, int level)
{
  wbl_ast_node *nc;

  for ( int i = 0; i < level; i++)
    printf( " ");
  printf( "%d %s\n", n->token, n->text);
  for ( nc = n->fch; nc; nc = nc->fws) {
    print_node( nc, level+1);
  }
}

//
// Return 1 if token is found, 0 if no more token is found.
//
int wb_wbl_parser::next_token( ifstream& is, char *line, const char *start_delim, 
			       const char *end_delim, wbl_eToken type, char **start, 
			       unsigned int *len, int *allocated)
{
  char *s, *token;
  const char *d;
  int state = eState_StartDelim;
  bool in_string = false;

  for ( s = line; *s; s++) {    
    if ( in_string) {
      if ( *s == '\"' && *(s-1) != '\\')
	in_string = false;
      else
	continue;
    }

    switch ( state) {
    case eState_StartDelim:
      for ( d = start_delim; *d; d++) {
	if ( *s == *d) {
	  state = eState_StartDelimFound;
	  break;
	}
      }
      if ( state != eState_StartDelimFound)
	throw wb_error_str("Unexpected end of token");
      break;
    case eState_StartDelimFound: {
      bool found = false;
      for ( d = start_delim; *d; d++) {
	if ( *s == *d) {
	  found = true;
	  break;
	}
      }
      if ( !found) {
	state = eState_InToken;
	token = s;
	
	if ( *s == '\"')
	  in_string = true;
      }
      break;
    }
    case eState_InToken:
      for ( d = end_delim; *d; d++) {
	if ( *s == *d) {
	  if ( type == wbl_eToken_Date && *s == ' ' && (s - token) < 14) {
	    // Space in date is Ok
	  }
	  else {
	    state = eState_EndDelimFound;
	    break;
	  }
	}
      }
      break;
    }
    if ( state == eState_EndDelimFound)
      break;
  }
    
  if ( *s == 0 && state == eState_InToken) {
    for ( d = end_delim; *d; d++) {
      if ( *d == '$') {
	state = eState_EndDelimFound;
	break;
      }
    }
  }
  if ( state == eState_StartDelim ||
       state == eState_StartDelimFound)
    return 0;

  if ( state != eState_EndDelimFound)
    throw wb_error_str( "Syntax error");

  *allocated = 0;

  if ( in_string) {
    // Find end of string on next lines
    bool found = false;
    char c, cold;

    // Get size of string
    streampos fend, fstart = is.tellg();
    cold = 0;
    while ( is.get(c)) {
      if ( c == '\"' && cold != '\\') {
	fend = is.tellg();
	found = true;
	break;
      }
      cold = c;
    }
    if ( !found)
      throw wb_error_str( "Unterminated string");

    int size = s - token + (fend - fstart) + 1;
    char *tp = (char *)malloc( size + 1);
    if ( !tp) throw wb_error_str("Out of memory");

    strncpy( tp, token, s - token);
    tp[s - token] = '\n';
    is.seekg( fstart);
    m_line_cnt++;
    cold = 0;
    int i = s - token + 1;
    while ( is.get(c)) {
      tp[i] = c;
      i++;
      if ( c == '\"' && cold != '\\')
	break;
      if ( c == '\n')
	m_line_cnt++;
      cold = c;
    }
    tp[size] = 0;
    
    // Read to end of line
    while ( is.get(c)) {
      if ( c == '\n')
	break;
    }	  
    // m_line_cnt++;

    *allocated = 1;
    *start = tp;
    *len = size;
  }
  else {
    *start = token;
    *len = s - token;  
  }
  return 1;
}

int wb_wbl_parser::read_docblock( ifstream& is, char *line,
				  char **start, unsigned int *len)
{
  bool found = false;
  char c, c1, c2;
  streampos fend, fstart = is.tellg();

  // Get size of docblock
  c1 = c2 = 0;
  while ( is.get(c)) {
    if ( c == '/' && c1 == '*' && c2 == '!') {
      fend = is.tellg();
      found = true;
      break;
    }
    c2 = c1;
    c1 = c;
  }
  if ( !found)
    throw wb_error_str( "Unterminated documentation block");

  int line_len = strlen(line);
  int size = line_len + (fend - fstart);
  char *tp = (char *)malloc( size + 2);
  strncpy( tp, line, line_len);
  tp[line_len] = '\n';

  is.seekg( fstart);
  m_line_cnt++;
  c1 = c2 = 0;
  int i = line_len + 1;
  while ( is.get(c)) {
    tp[i] = c;
    i++;
    if ( c == '/' && c1 == '*' && c2 == '!') {
      m_state &= ~wbl_mState_InDocBlock;
      break;
    }
    if ( c == '\n')
      m_line_cnt++;
    c2 = c1;
    c1 = c;
  }
  tp[size + 1] = 0;
  // printf( "Docblock: %s\n", tp);
    
  // Read to end of line
  while ( is.get(c)) {
    if ( c == '\n')
      break;
  }
  m_line_cnt++;
  
  *start = tp;
  *len = size;

  return 1;
}

void wb_wbl_parser::ast_node_insert_lch( wbl_ast_node *fth, wbl_ast_node *n)
{
  if ( !fth->fch) {
    fth->fch = n;
    fth->lch = n;
    n->fth = fth;
  }
  else {
    fth->lch->fws = n;
    fth->lch = n;
    n->fth = fth;
  }
}

wbl_ast_node *wb_wbl_parser::new_ast_node( wbl_eToken token, char *text, unsigned int len,
					   unsigned int line, int allocated)
{
  wbl_ast_node *n = (wbl_ast_node *)new wb_wblnode();
  if ( !n) throw wb_error_str("Out of memory");

  if ( allocated)
    n->text = text;
  else {
    n->text = (char *)malloc( len+1);
    if ( !n->text) throw wb_error_str("Out of memory");

    n->token = token;
    strncpy( n->text, text, len);
    n->text[len] = 0;
  }
  n->size = len;
  n->token = token;
  n->line_number = line;
  return n;
}

void wb_wbl_parser::print_line()
{
  printf( "-- Processing line: %d\r", m_line_cnt); 
  fflush(stdout);
}

int wb_wbl_parser::check_operator( wbl_ast_node *n)
{
  if ( strcmp( n->text, "=") == 0) {
    n->token = wbl_eToken_Operator_eq;
    return 1;
  }
  else if ( strcmp( n->text, "|=") == 0) {
    n->token = wbl_eToken_Operator_oreq;
    return 1;
  }
  return 0;
}

void wb_wbl_parser::parse( const char *filename)
{
  char line[400];
  char *token;
  unsigned int len;
  wbl_ast_node *t1, *t2, *t3, *t4;
  int sts;
  int alloc;
  static unsigned int last = 0;

  dcli_translate_filename( m_fname, filename);
  ifstream is( m_fname);
  if ( !is) throw wb_error_str( "No such file");

  while ( is.getline( line, sizeof(line))) {
    m_line_cnt++;

    if ( (m_line_cnt % 1000) == 0 && m_line_cnt != last) {
      print_line();
      last = m_line_cnt;
    }


    dcli_trim( line, line);

    if ( strcmp( line, "") == 0)
      continue;

    if ( strncmp( line, "!/**", 4) == 0) {
      if ( m_state & wbl_mState_InDocBlock)
	throw wb_error_str( "Already in documentation block");
      m_state |= wbl_mState_InDocBlock;

      read_docblock( is, line, &token, &len);
      t1 = new_ast_node( wbl_eToken_DocBlock, token, len, m_line_cnt, 1);
      ast_node_insert_lch( m_current, t1);
    }
    else if ( line[0] == '!') {
      // Comment
      continue;
    }
    else if ( strncmp( line, "Volume", 6) == 0) {
      if ( m_state & wbl_mState_InVolume)
	throw wb_error_str( "Volume already defined");
      if ( m_tree)
	throw wb_error_str( "Volume is not first");
      m_state |= wbl_mState_InVolume;

      sts = next_token( is, &line[6], WS, WS, wbl_eToken_Volume, &token, &len, &alloc);
      if ( !sts) throw wb_error_str("Volume name is missing");
      t1 = new_ast_node( wbl_eToken_Volume, token, len, m_line_cnt, 0);
      m_tree = t1;
      m_current = t1;

      // Volume class
      sts = next_token( is, token + len, WS, WS, wbl_eToken_Name, &token, &len, &alloc);
      if ( !sts) throw wb_error_str("Volume class is missing");
      t2 = new_ast_node( wbl_eToken_Name, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t2);

      // Volumeid
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Index, &token, &len, &alloc);
      if ( !sts) throw wb_error_str("Volume id is missing");
      t3 = new_ast_node( wbl_eToken_Index, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t3);
    }
    else if ( strncmp( line, "EndVolume", 9) == 0) {
      if ( !(m_state & wbl_mState_InVolume))
	throw wb_error_str( "Volume - EndVolume mismatch");
      if ( m_state & wbl_mState_InBody)
	throw wb_error_str( "Body not terminated");
      if ( m_state & wbl_mState_InObject)
	throw wb_error_str( "Body not terminated");
      m_state &= ~wbl_mState_InVolume;
    }
    else if ( strncmp( line, "SObject", 7) == 0) {
      if ( m_state & wbl_mState_InSObject)
	throw wb_error_str( "SObject already defined");
      m_state |= wbl_mState_InSObject;

      // SObject name
      next_token( is, &line[7], WS, WSEOL, wbl_eToken_Name, &token, &len, &alloc);
      t1 = new_ast_node( wbl_eToken_SObject, token, len, m_line_cnt, 0);
      if ( !m_tree) {
	m_tree = t1;
	m_current = t1;
      }
      else {
	// Insert as last sibling to m_tree
	wbl_ast_node *n;
	for ( n = m_tree; n->fws; n = n->fws) ;
	n->fws = t1;
	m_current = t1;
      }
    }
    else if ( strncmp( line, "EndSObject", 10) == 0) {
      if ( !(m_state & wbl_mState_InSObject))
	throw wb_error_str( "SObject - EndSObject mismatch");
      if ( m_state & wbl_mState_InBody)
	throw wb_error_str( "Body not terminated");
      if ( m_state & wbl_mState_InObject)
	throw wb_error_str( "Object not terminated");
      if ( m_state & wbl_mState_InBuffer)
	throw wb_error_str( "Buffer not terminated");
      m_state &= ~wbl_mState_InSObject;
    }
    else if ( strncmp( line, "Object", 6) == 0) {
      if ( !(m_state & wbl_mState_InVolume || m_state & wbl_mState_InSObject))
	throw wb_error_str( "Object defined outside volume");
      m_object_level++;
      m_state |= wbl_mState_InObject;

      // Object name
      sts = next_token( is, &line[6], WS, WS, wbl_eToken_Name, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Object syntax error");
      t1 = new_ast_node( wbl_eToken_Object, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t1);
      m_current = t1;

      // Object class
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Name, &token, &len, &alloc);
      if ( sts) {
	t2 = new_ast_node( wbl_eToken_Name, token, len, m_line_cnt, 0);
	ast_node_insert_lch( m_current, t2);
      }

      // Object identity
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Index, &token, &len, &alloc);
      if ( sts) {
	t3 = new_ast_node( wbl_eToken_Index, token, len, m_line_cnt, 0);
	ast_node_insert_lch( m_current, t3);
      }

      // Object time
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Date, &token, &len, &alloc);
      if ( sts) {
	t4 = new_ast_node( wbl_eToken_Date, token, len, m_line_cnt, 0);
	ast_node_insert_lch( m_current, t4);
      }
    }
    else if ( strncmp( line, "EndObject", 9) == 0) {
      if ( !(m_state & wbl_mState_InObject))
	throw wb_error_str( "Object - EndObject mismatch");
      if ( m_state & wbl_mState_InBody)
	throw wb_error_str( "Body not terminated");
      if ( m_state & wbl_mState_InBuffer)
	throw wb_error_str( "Buffer not terminated");
      if ( m_object_level <= 0)
	throw wb_error_str( "Object - EndObject mismatch");
      m_object_level--;
      if ( m_object_level == 0)
	m_state &= ~wbl_mState_InObject;
      m_current = m_current->fth;
    }
    else if ( strncmp( line, "Body", 4) == 0) {
      if ( m_state & wbl_mState_InBody)
	throw wb_error_str( "Body already defined");
      if ( !(m_state & wbl_mState_InVolume || m_state & wbl_mState_InObject))
	throw wb_error_str( "Body defined outside object");
      m_state |= wbl_mState_InBody;

      // Body name
      sts = next_token( is, &line[4], WS, WSEOL, wbl_eToken_Name, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Body syntax error");
      t1 = new_ast_node( wbl_eToken_Body, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t1);
      m_current = t1;

      // Body time
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Date, &token, &len, &alloc);
      if ( sts) {
	t2 = new_ast_node( wbl_eToken_Date, token, len, m_line_cnt, 0);
	ast_node_insert_lch( m_current, t2);
      }
    }
    else if ( strncmp( line, "EndBody", 7) == 0) {
      if ( !(m_state & wbl_mState_InBody))
	throw wb_error_str( "Body - EndBody mismatch");
      m_state &= ~wbl_mState_InBody;
      m_current = m_current->fth;
    }
    else if ( strncmp( line, "Buffer", 6) == 0) {
      if ( !(m_state & wbl_mState_InBody))
	throw wb_error_str( "Attr defined outside body");
      if ( !(m_state & wbl_mState_InVolume || m_state & wbl_mState_InObject))
	throw wb_error_str( "Buffer defined outside object");
      m_buffer_level++;
      m_state |= wbl_mState_InBuffer;

      // Buffer name
      sts = next_token( is, &line[6], WS, WSEOL, wbl_eToken_Name, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Buffer syntax error");
      t1 = new_ast_node( wbl_eToken_Buffer, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t1);
      m_current = t1;

      // Buffer time
      sts = next_token( is, token + len, WS, WSEOL, wbl_eToken_Date, &token, &len, &alloc);
      if ( sts) {
	t2 = new_ast_node( wbl_eToken_Date, token, len, m_line_cnt, 0);
	ast_node_insert_lch( m_current, t2);
      }
    }
    else if ( strncmp( line, "EndBuffer", 9) == 0) {
      if ( !(m_state & wbl_mState_InBuffer))
	throw wb_error_str( "Buffer - EndBuffer mismatch");
      m_buffer_level--;
      if ( m_buffer_level == 0)
	m_state &= ~wbl_mState_InBuffer;
      m_current = m_current->fth;
    }
    else if ( strncmp( line, "Attr", 4) == 0) {
      if ( !(m_state & wbl_mState_InBody))
	throw wb_error_str( "Attr defined outside body");

      // Attr name
      sts = next_token( is, &line[4], WS, WS, wbl_eToken_Name, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Attr syntax error");
      t1 = new_ast_node( wbl_eToken_Attr, token, len, m_line_cnt, 0);
      ast_node_insert_lch( m_current, t1);
      m_current = t1;

      // Attr operator
      sts = next_token( is, token + len, WS, WS, wbl_eToken_Operator, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Attr syntax error");
      t2 = new_ast_node( wbl_eToken_Operator, token, len, m_line_cnt, 0);
      if ( !check_operator( t2))
	throw wb_error_str( "Undefined operator");
	
      ast_node_insert_lch( m_current, t2);

      // Attr value
      sts = next_token( is, token + len, WS "=", WSEOL, wbl_eToken_Value, &token, &len, &alloc);
      if ( !sts)
	throw wb_error_str( "Attr syntax error");
      t2 = new_ast_node( wbl_eToken_Value, token, len, m_line_cnt, alloc);
      ast_node_insert_lch( m_current, t2);

      m_current = m_current->fth;
    }
    else {
      // Syntax error
      throw wb_error_str( "Undefined token");
    }
  }
  print_line();
}
