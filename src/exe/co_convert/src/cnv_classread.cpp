#include <stdio.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

extern "C" {
#include "pwr.h"
#include "co_dcli.h"
#include "co_cdh.h"
}
#include "cnv_classread.h"

#define CNV__UNKNOWN_LINETYPE 2

int ClassRead::read( char *filename)
{
  int sts;
  int return_sts = 1;
  char line[400];
  char	line_part[4][80];
  int nr;
  int object_level = 0;
  char *s;
  char line_cnt = 0;

  doc_init();

  // Get source directory
  strcpy( source_dir, filename);
  if ( (s = strrchr( source_dir, '/')))
    *(s+1) = 0;
  else if ( (s = strrchr( source_dir, '>')))
    *(s+1) = 0;
  else if ( (s = strrchr( source_dir, ']')))
    *(s+1) = 0;
  else if ( (s = strrchr( source_dir, ':')))
    *(s+1) = 0;


  fp = fopen( filename, "r");
  if ( !fp)
    return 0;

  state = 0;
  doc_fresh = 0;

  while( 1)
  {
    sts = read_line( line, sizeof(line), fp);
    if ( !sts)
      linetype = cread_eLine_EOF;
    else
    {
      line_cnt++;
      remove_spaces( line, line);
      if ( strcmp( line, "") == 0)
        continue;

      if ( line[0] == '!' && 
           strncmp( line, "!/**", 4) != 0 &&
           !(state & cread_mState_Doc))
        continue;

      nr = dcli_parse( line, " 	=", "", (char *)line_part,
                	sizeof( line_part) / sizeof( line_part[0]), 
			sizeof( line_part[0]), 0);

      if ( strcmp( low( line_part[0]), "sobject") == 0)
        linetype = cread_eLine_SObject;
      else if ( strcmp( low( line_part[0]), "endsobject") == 0)
        linetype = cread_eLine_EndSObject;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 1 &&
                strcmp( low( line_part[1]), "template") == 0)
        linetype = cread_eLine_Template;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$classdef") == 0)
        linetype = cread_eLine_ClassDef;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "pwr_eclass_classdef") == 0)
        linetype = cread_eLine_ClassDef;
      else if ( strcmp( low( line_part[0]), "body") == 0 &&
		nr > 1 &&
                strcmp( low( line_part[1]), "sysbody") == 0)
        linetype = cread_eLine_SysBody;
      else if ( strcmp( low( line_part[0]), "dbody") == 0 &&
		nr > 1 &&
                strcmp( low( line_part[1]), "sysbody") == 0)
        linetype = cread_eLine_SysBody;
      else if ( strcmp( low( line_part[0]), "body") == 0 &&
		nr > 1 &&
                strcmp( low( line_part[1]), "rtbody") == 0)
        linetype = cread_eLine_RtBody;
      else if ( strcmp( low( line_part[0]), "body") == 0 &&
		nr > 1 &&
                strcmp( low( line_part[1]), "devbody") == 0)
        linetype = cread_eLine_DevBody;
      else if ( strcmp( low( line_part[0]), "endbody") == 0)
        linetype = cread_eLine_EndBody;
      else if ( strcmp( low( line_part[0]), "enddbody") == 0)
        linetype = cread_eLine_EndBody;
      else if ( strcmp( low( line_part[0]), "endobject") == 0)
        linetype = cread_eLine_EndObject;
      else if ( strcmp( low( line_part[0]), "enddobject") == 0)
        linetype = cread_eLine_EndObject;
      else if ( strcmp( low( line_part[0]), "attr") == 0)
        linetype = cread_eLine_Attr;
      else if ( strcmp( low( line_part[0]), "dattr") == 0)
        linetype = cread_eLine_DAttr;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "pwr_eclass_param") == 0)
        linetype = cread_eLine_Attribute;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$objbodydef") == 0)
        linetype = cread_eLine_ObjBodyDef;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "pwr_eclass_objbodydef") == 0)
        linetype = cread_eLine_ObjBodyDef;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$input") == 0)
        linetype = cread_eLine_Input;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$output") == 0)
        linetype = cread_eLine_Output;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$intern") == 0)
        linetype = cread_eLine_Intern;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$attribute") == 0)
        linetype = cread_eLine_Attribute;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$param") == 0)
        linetype = cread_eLine_Attribute;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$objxref") == 0)
        linetype = cread_eLine_ObjXRef;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$graphplcnode") == 0)
        linetype = cread_eLine_GraphPlcNode;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$graphplcconnection") == 0)
        linetype = cread_eLine_GraphPlcCon;
      else if ( strcmp( low( line_part[0]), "object") == 0 &&
		nr > 2 &&
                strcmp( low( line_part[2]), "$buffer") == 0)
        linetype = cread_eLine_Buffer;
      else if ( strcmp( low( line_part[0]), "object") == 0)
        linetype = cread_eLine_Object;
      else if ( strcmp( low( line_part[0]), "!/**") == 0)
        linetype = cread_eLine_Doc;
      else if ( strcmp( low( line_part[0]), "!*/") == 0)
        linetype = cread_eLine_DocEnd;
      else if ( strcmp( low( line_part[0]), "!") == 0)
        linetype = cread_eLine_Comment;
      else if ( strcmp( low( line_part[0]), "volume") == 0)
        linetype = cread_eLine_Volume;
      else if ( strcmp( low( line_part[0]), "endvolume") == 0)
        linetype = cread_eLine_EndVolume;
      else if ( strcmp( low( line_part[0]), "range") == 0)
        linetype = cread_eLine_Range;
      else      
        linetype = cread_eLine_Unknown;

      sts = 1;
      switch( linetype)
      {
        case cread_eLine_Doc:
          state |= cread_mState_Doc;
          sts = object_close();
          doc_init();
          break;
        case cread_eLine_DocEnd:
          doc_close();
          state &= ~cread_mState_Doc;
          break;
        case cread_eLine_Comment:
          doc_add( line);
          break;
        case cread_eLine_SObject:
          state |= cread_mState_SObject;
          strcpy( sobject_name, line_part[1]);
          volume_init();
          break;
        case cread_eLine_ClassDef:
          state |= cread_mState_ClassDef;
          object_state = cread_mState_ClassDef;
          class_init();
          if ( line_part[1][0] == '$')
            strcpy( class_name, &line_part[1][1]);
          else
            strcpy( class_name, line_part[1]);
          if ( nr > 3)
            strcpy( class_id, line_part[3]);
          break;
        case cread_eLine_SysBody:
          state |= cread_mState_SysBody;
          break;
        case cread_eLine_RtBody:
          state |= cread_mState_RtBody;
          break;
        case cread_eLine_DevBody:
          state |= cread_mState_DevBody;
          break;
        case cread_eLine_EndBody:
          sts = object_close();
          state &= ~cread_mState_SysBody;
          state &= ~cread_mState_RtBody;
          state &= ~cread_mState_DevBody;
          break;
        case cread_eLine_ObjBodyDef:
          sts = object_close();
          state |= cread_mState_ObjBodyDef;
          object_state = cread_mState_ObjBodyDef;
          body_init();
          strcpy( ObjBodyDef, line_part[1]);
          strcpy( body_name, line_part[1]);
          break;
        case cread_eLine_Template:
          state |= cread_mState_Template;
          object_state = cread_mState_Template;
          template_init();
          break;
        case cread_eLine_DParam:
          sts = object_close();
          state |= cread_mState_DParam;
          object_state = cread_mState_DParam;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Attribute");
          attribute_init();
          break;
        case cread_eLine_Input:
          sts = object_close();
          state |= cread_mState_Input;
          object_state = cread_mState_Input;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Input");
          attribute_init();
          break;
        case cread_eLine_Output:
          sts = object_close();
          state |= cread_mState_Output;
          object_state = cread_mState_Output;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Output");
          attribute_init();
          break;
        case cread_eLine_Intern:
          sts = object_close();
          state |= cread_mState_Intern;
          object_state = cread_mState_Intern;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Intern");
          attribute_init();
          break;
        case cread_eLine_Attribute:
          sts = object_close();
          state |= cread_mState_Attribute;
          object_state = cread_mState_Attribute;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Attribute");
          attribute_init();
          break;
        case cread_eLine_ObjXRef:
          sts = object_close();
          state |= cread_mState_ObjXRef;
          object_state = cread_mState_ObjXRef;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "ObjXRef");
          attribute_init();
          strcpy( attr_typeref, "Objid");
          break;
        case cread_eLine_EndSObject:
          state &= ~cread_mState_SObject;
          break;
        case cread_eLine_EndObject:
          if ( state & cread_mState_Input)
            state &= ~cread_mState_Input;
          else if ( state & cread_mState_Output)
            state &= ~cread_mState_Output;
          else if ( state & cread_mState_Intern)
            state &= ~cread_mState_Intern;
          else if ( state & cread_mState_Attribute)
            state &= ~cread_mState_Attribute;
          else if ( state & cread_mState_DParam)
            state &= ~cread_mState_DParam;
          else if ( state & cread_mState_ObjXRef)
            state &= ~cread_mState_ObjXRef;
          else if ( state & cread_mState_Buffer)
            state &= ~cread_mState_Buffer;
          else if ( state & cread_mState_ObjBodyDef)
          {
            state &= ~cread_mState_ObjBodyDef;
            if ( generate_html)
              html_body_close();
            if ( generate_struct)
              struct_body_close();
          }
          else if ( state & cread_mState_GraphPlcNode)
          {
            state &= ~cread_mState_GraphPlcNode;
            if ( generate_html)
              html_body_close();
          }
          else if ( state & cread_mState_GraphPlcCon)
          {
            state &= ~cread_mState_GraphPlcCon;
            if ( generate_html)
              html_body_close();
          }
          else if ( state & cread_mState_Template)
          {
            state &= ~cread_mState_Template;
            if ( generate_html)
              html_body_close();
          }
          else if ( state & cread_mState_ClassDef) {
            state &= ~cread_mState_ClassDef;
            if ( generate_html && html_class_open)
              html_class_close();
	    if ( generate_struct && struct_class_open)
              struct_class_close();
          }
          else if ( state & cread_mState_Object)
          {
            object_level--;
            if ( object_level <= 0)
              state &= ~cread_mState_Object;
          }
          else
            printf( "Error: mismatch in Object-Endobject nesting, at line %d\n",
			line_cnt);
          break;
        case cread_eLine_Attr:
        {
	  char attr_name[80];
          char attr_value[100];
          int oreq = 0;

          if ( strcmp( line_part[2], "|") == 0)
          {
            strcpy( line_part[2], line_part[3]);
            oreq = 1;
          }

          strcpy( attr_name, line_part[1]);
          strcpy( attr_value, line_part[2]);
          switch ( object_state)
          {
            case cread_mState_Input:
            case cread_mState_Output:
            case cread_mState_Intern:
            case cread_mState_Attribute:
            case cread_mState_ObjXRef:
            case cread_mState_Buffer:
              attribute_attr( attr_name, attr_value);
              break;
            case cread_mState_GraphPlcNode:
              graphplcnode_attr( attr_name, attr_value);
              break;
            case cread_mState_GraphPlcCon:
              graphplccon_attr( attr_name, attr_value);
              break;
            case cread_mState_ClassDef:
              class_attr( attr_name, attr_value);
              break;
            case cread_mState_ObjBodyDef:
              body_attr( attr_name, attr_value);
              break;
            case cread_mState_Template:
              template_attr( attr_name, attr_value);
              break;
            default:
              ;
          }
          break;
        }
        case cread_eLine_DAttr:
        {
	  char attr_name[80];
          char attr_value[100];
          int oreq = 0;

          if ( strcmp( line_part[3], "|") == 0)
          {
            strcpy( line_part[3], line_part[4]);
            oreq = 1;
          }

          strcpy( attr_name, line_part[1]);
          strcpy( attr_value, line_part[3]);
          switch ( object_state)
          {
            case cread_mState_DParam:
            case cread_mState_Input:
            case cread_mState_Output:
            case cread_mState_Intern:
            case cread_mState_Attribute:
            case cread_mState_ObjXRef:
            case cread_mState_Buffer:
              attribute_attr( attr_name, attr_value);
              break;
            case cread_mState_GraphPlcNode:
              graphplcnode_attr( attr_name, attr_value);
              break;
            case cread_mState_GraphPlcCon:
              graphplccon_attr( attr_name, attr_value);
              break;
            case cread_mState_ClassDef:
              class_attr( attr_name, attr_value);
              break;
            case cread_mState_ObjBodyDef:
              body_attr( attr_name, attr_value);
              break;
            case cread_mState_Template:
              template_attr( attr_name, attr_value);
              break;
            default:
              ;
          }
          break;
        }
        case cread_eLine_Buffer:
        {
          sts = object_close();
          state |= cread_mState_Buffer;
          object_state = cread_mState_Buffer;
          strcpy( attr_name, line_part[1]);
          strcpy( attr_type, "Buffer");
          attribute_init();
          break;
        }
        case cread_eLine_GraphPlcNode:
        {
          sts = object_close();
          state |= cread_mState_GraphPlcNode;
          object_state = cread_mState_GraphPlcNode;
          graphplcnode_init();
          strcpy( graphplcnode_name, line_part[1]);
          break;
        }
        case cread_eLine_GraphPlcCon:
        {
          sts = object_close();
          state |= cread_mState_GraphPlcCon;
          object_state = cread_mState_GraphPlcCon;
          graphplccon_init();
          strcpy( graphplccon_name, line_part[1]);
          break;
        }
        case cread_eLine_Object:
        {
          sts = object_close();
          state |= cread_mState_Object;
          object_state = cread_mState_Object;
          object_level++;
          break;
        }
        case cread_eLine_Volume:
        {
          state |= cread_mState_Volume;
          break;
        }
        case cread_eLine_EndVolume:
        {
          if ( state & cread_mState_Volume)
            state &= ~cread_mState_Volume;
          break;
        }
        case cread_eLine_Range:
          break;
        case cread_eLine_EOF:
          break;

        default:
          printf( "Error, unknown linetype, at line %d\n", line_cnt);
          return sts = CNV__UNKNOWN_LINETYPE;
      }
      if ( EVEN(sts))
      {
        printf( ", at line %d\n", line_cnt);
        return_sts = sts;
      }
    }

    if ( linetype ==  cread_eLine_EOF)
      break;
  }

  if ( generate_html && html_class_open)
    html_class_close();
  if ( generate_xtthelp && xtthelp_index_open)
    xtthelp_class_close();
  if ( generate_struct)
    struct_class_close();

  fclose(fp);
  return return_sts;
}


void ClassRead::attribute_init()
{
  strcpy( attr_flags, "");
  strcpy( attr_typeref, "");
  strcpy( attr_pgmname, "");
  strcpy( attr_elements, "");
  attr_pointer = 0;
  attr_array = 0;
  attr_rtvirtual = 0;
  attr_elem = 0;
}

int ClassRead::attribute_attr( char *name, char *value)
{
  char 	*s;
  int	nr;

  if ( strcmp( low( name), "typeref") == 0)
  {
    if ((s = strrchr( value, '-')))
    {
      s++;
      if ( *s == '$')
        s++;
      strcpy( attr_typeref, s);
    }
    else
      strcpy( attr_typeref, value);
  }
  else if ( strcmp( low( name), "flags") == 0)
  {
    if ( strcmp( attr_flags, "") != 0)
      strcat( attr_flags, " | ");
    if ( strncmp( value, "PWR_MASK_", 9) == 0)
      strcat( attr_flags, &value[9]);
    else
      strcat( attr_flags, value);

    if ( strcmp( value, "PWR_MASK_POINTER") == 0)
      attr_pointer = 1;
    else if ( strcmp( value, "PWR_MASK_ARRAY") == 0)
      attr_array = 1;
    else if ( strcmp( value, "PWR_MASK_RTVIRTUAL") == 0)
      attr_rtvirtual = 1;
  }
  else if ( strcmp( low( name), "elements") == 0)
  {
    strcpy( attr_elements, value);
    nr = sscanf( attr_elements, "%d", &attr_elem);
    if ( nr == 0)
      attr_elem = 0;    
  }
  else if ( strcmp( low( name), "class") == 0)
  {
    strcpy( attr_typeref, value);
  }
  else if ( strcmp( low( name), "pgmname") == 0)
  {
    strcpy( attr_pgmname, value);
  }
  return 1;
}

int ClassRead::attribute_close()
{
  int sts;

  if ( generate_html)
    sts = html_attribute();
  if ( generate_xtthelp)
    sts = xtthelp_attribute();
  if ( generate_struct)
    sts = struct_attribute();

  doc_fresh = 0;
  return sts;
}

void ClassRead::class_init()
{
  strcpy( class_name, "");
  strcpy( class_editor, "");
  strcpy( class_method, "");
  strcpy( class_popeditor, "");
  strcpy( class_flags, "");
  class_devonly = 0;
}

int ClassRead::class_attr( char *name, char *value)
{
  if ( strcmp( low( name), "editor") == 0)
  {
    if ( strncmp( value, "pwr_eEditor_", 12) == 0)
      strcpy( class_editor, &value[12]);
    else
      strcpy( class_editor, value);

  }
  else if ( strcmp( low( name), "method") == 0)
  {
    if ( strncmp( value, "pwr_eMethod_", 12) == 0)
      strcpy( class_method, &value[12]);
    else
      strcpy( class_method, value);

  }
  else if ( strcmp( low( name), "popeditor") == 0)
  {
    strcpy( class_method, value);
  }
  else if ( strcmp( low( name), "flags") == 0)
  {
    if ( strcmp( class_flags, "") != 0)
      strcat( class_flags, " | ");
    if ( strncmp( value, "pwr_mClassDef_", 14) == 0)
      strcat( body_flags, &value[14]);
    else
      strcat( body_flags, value);

    if ( strcmp( value, "pwr_mClassDef_DevOnly") == 0)
      class_devonly = 1;
  }
  return 1;
}

int ClassRead::class_close()
{
  if ( first_class)
  {
    if ( generate_html)
      html_init( class_name);
    if ( generate_xtthelp)
      xtthelp_init();
    if ( generate_struct)
      struct_init();

    first_class = 0;
  }

  if ( generate_html)
    html_class();
  if ( generate_xtthelp)
    xtthelp_class();
  if ( generate_struct)
    struct_class();

  doc_fresh = 0;
  return 1;
}

void ClassRead::body_init()
{
  strcpy( body_name, "");
  strcpy( body_structname, "");
  strcpy( body_flags, "");
  body_rtvirtual = 0;
}

int ClassRead::body_attr( char *name, char *value)
{
  if ( strcmp( low( name), "structname") == 0)
  {
    strcpy( body_structname, value);
  }
  else if ( strcmp( low( name), "flags") == 0)
  {
    if ( strcmp( body_flags, "") != 0)
      strcat( body_flags, " | ");
    if ( strncmp( value, "pwr_mObjBodyDef_", 16) == 0)
      strcat( body_flags, &value[9]);
    else
      strcat( body_flags, value);

    if ( strcmp( value, "pwr_mObjBodyDef_RtVirtual") == 0)
      body_rtvirtual = 1;
  }
  return 1;
}

int ClassRead::body_close()
{
  int sts;

  if ( generate_html)
    html_body();
  if ( generate_xtthelp)
    xtthelp_body();
  if ( generate_struct)
  {
    sts = struct_body();
    if ( EVEN(sts)) return sts;
  }
  doc_fresh = 0;
  return 1;
}

void ClassRead::graphplcnode_init()
{
  doc_cnt = 0;
}

int ClassRead::graphplcnode_attr( char *name, char *value)
{
  strcpy( doc_text[doc_cnt++], name);
  strcpy( doc_text[doc_cnt++], value);

  // Description of methods
  if ( strcmp( low( name), "graphmethod") == 0)
  {
    if ( strcmp( value, "0") == 0)
      strcat( doc_text[doc_cnt-1], " (standard, individual attributes)");
    else if ( strcmp( value, "1") == 0)
      strcat( doc_text[doc_cnt-1], " (standard, common attributes)");
    else if ( strcmp( value, "2") == 0)
      strcat( doc_text[doc_cnt-1], " (standard, two textfield)");
    else if ( strcmp( value, "3") == 0)
      strcat( doc_text[doc_cnt-1], " (text)");
    else if ( strcmp( value, "4") == 0)
      strcat( doc_text[doc_cnt-1], " (special)");
    else if ( strcmp( value, "5") == 0)
      strcat( doc_text[doc_cnt-1], " (grafcet order)");
    else if ( strcmp( value, "6") == 0)
      strcat( doc_text[doc_cnt-1], " (document)");
    else if ( strcmp( value, "7") == 0)
      strcat( doc_text[doc_cnt-1], " (Get,Set)");
  }
  else if ( strcmp( low( name), "compmethod") == 0)
    ;
  else if ( strcmp( low( name), "tracemethod") == 0)
    ;
  else if ( strcmp( low( name), "executeordermethod") == 0)
    ;
  return 1;
}

int ClassRead::graphplcnode_close()
{
  if ( generate_html)
    html_graphplcnode();

  doc_fresh = 0;
  return 1;
}
void ClassRead::graphplccon_init()
{
  doc_cnt = 0;
}

int ClassRead::graphplccon_attr( char *name, char *value)
{
  strcpy( doc_text[doc_cnt++], name);
  strcpy( doc_text[doc_cnt++], value);

  // Description
  if ( strcmp( low( name), "curvature") == 0)
    ;
  else if ( strcmp( low( name), "corners") == 0)
    ;
  else if ( strcmp( low( name), "color") == 0)
    ;
  else if ( strcmp( low( name), "attributes") == 0)
    ;
  return 1;
}

int ClassRead::graphplccon_close()
{
  if ( generate_html)
    html_graphplccon();

  doc_fresh = 0;
  return 1;
}
void ClassRead::template_init()
{
  doc_cnt = 0;
}

int ClassRead::template_attr( char *name, char *value)
{
  strcpy( doc_text[doc_cnt++], name);
  strcpy( doc_text[doc_cnt++], value);
  return 1;
}

int ClassRead::template_close()
{
  if ( generate_html)
    html_template();
  doc_fresh = 0;
  return 1;
}

void ClassRead::volume_init()
{
  char *s;

  strcpy( volume_name, sobject_name);
  if ( (s = strchr( volume_name, ':')))
    *s = 0;

}

void ClassRead::doc_init()
{
  doc_cnt = 0;
  strcpy( doc_author, "");
  strcpy( doc_version, "");
  strcpy( doc_code, "");
  strcpy( doc_summary, "");
  memset( doc_clink_ref, 0, sizeof(doc_clink_ref));
  memset( doc_clink_text, 0, sizeof(doc_clink_text));
  memset( doc_link_ref, 0, sizeof(doc_link_ref));
  memset( doc_link_text, 0, sizeof(doc_link_text));
  doc_clink_cnt = 0;
  doc_link_cnt = 0;
}

int ClassRead::doc_add( char *line)
{
  char	line_part[4][80];
  int nr;
  int i;

  nr = dcli_parse( line, " 	=", "", (char *)line_part,
                	sizeof( line_part) / sizeof( line_part[0]), 
			sizeof( line_part[0]), 0);

  if ( strcmp( low(line_part[1]), "@author") == 0)
  {
    for ( i = 2; i < nr; i++)
    {
      if ( i != 2)
        strcat( doc_author, " ");
      strcat( doc_author, line_part[i]);
    }
  }
  else if ( strcmp( low(line_part[1]), "@version") == 0)
  {
    for ( i = 2; i < nr; i++)
    {
      if ( i != 2)
        strcat( doc_version, " ");
      strcat( doc_version, line_part[i]);
    }
  }
  else if ( strcmp( low(line_part[1]), "@link") == 0)
  {
    if ( doc_link_cnt >= (int) (sizeof(doc_link_ref)/sizeof(doc_link_ref[0]))) {
      printf("Error: max number of links exceeded\n");
      return 1;
    }
    for ( i = 2; i < nr; i++)
    {
      if ( i == nr - 1)
        strcpy( doc_link_ref[doc_link_cnt], line_part[i]);
      else {
        if ( i != 2)
          strcat( doc_link_text[doc_link_cnt], " ");
        strcat( doc_link_text[doc_link_cnt], line_part[i]);
      }
    }
    doc_link_cnt++;
  }
  else if ( strcmp( low(line_part[1]), "@classlink") == 0)
  {
    if ( doc_clink_cnt >= (int) (sizeof(doc_clink_ref)/sizeof(doc_clink_ref[0]))) {
      printf("Error: max number of classlinks exceeded\n");
      return 1;
    }
    for ( i = 2; i < nr; i++)
    {
      if ( i == nr - 1)
        strcpy( doc_clink_ref[doc_clink_cnt], line_part[i]);
      else {
        if ( i != 2)
          strcat( doc_clink_text[doc_clink_cnt], " ");
        strcat( doc_clink_text[doc_clink_cnt], line_part[i]);
      }
    }
    doc_clink_cnt++;
  }
  else if ( strcmp( low(line_part[1]), "@code") == 0)
  {
    if ( nr > 2)
      strcpy( doc_code, line_part[2]);
  }
  else if ( strcmp( low(line_part[1]), "@summary") == 0)
  {
    char low_line[400];
    char *s;

    cdh_ToLower( low_line, line);
    s = strstr( low_line, "@summary");
    if ( !s)
      return 0;

    strcpy( doc_summary, &line[s - low_line + 9]);
  }
  else
  {
    if ( doc_cnt > int(sizeof(doc_text)/sizeof(doc_text[0]) - 1))
      return 0;
    strcpy( doc_text[doc_cnt], &line[1]);
    doc_cnt++;
  }
  return 1;
}

int ClassRead::doc_close()
{
  doc_fresh = 1;
  return 1;
}

int ClassRead::object_close()
{
  int sts;

  switch ( object_state)
  {
    case 0:
      break;
    case cread_mState_Input:
    case cread_mState_Output:
    case cread_mState_Intern:
    case cread_mState_Attribute:
    case cread_mState_ObjXRef:
    case cread_mState_Buffer:
    case cread_mState_DParam:
      sts = attribute_close();
      if ( EVEN(sts)) return sts;
      break;
    case cread_mState_ObjBodyDef:
      sts = body_close();
      if ( EVEN(sts)) return sts;
      break;
    case cread_mState_GraphPlcNode:
      graphplcnode_close();
      break;
    case cread_mState_GraphPlcCon:
      graphplccon_close();
      break;
    case cread_mState_Template:
      template_close();
      break;
    case cread_mState_ClassDef:
      class_close();
      break;
    case cread_mState_Object:
      break;
    default:
      ;
  }
  object_state = 0;
  return 1;
}

char *ClassRead::low( char *in)
{
  static char str[400];

  cdh_ToLower( str, in);
  return str;
}

int ClassRead::remove_spaces(
			char	*in,
			char	*out)
{
  char    *s;

  for ( s = in; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++);

  strcpy( out, s);
        
  s = out;
  if ( strlen(s) != 0)
  {
    for ( s += strlen(s) - 1; 
          !((s == out) || ((*s != ' ') && (*s != 9))); s--) ;
    s++;
    *s = 0;
  }

  return 1;
}

int ClassRead::read_line(
			char	*line,
			int	maxsize,
			FILE	*file)
{ 
  char	*s;

  if (fgets( line, maxsize, file) == NULL)
    return 0;
  line[maxsize-1] = 0;
  s = strchr( line, 10);
  if ( s != 0)
    *s = 0;
  s = strchr( line, 13);
  if ( s != 0)
    *s = 0;

  return 1;
}

int ClassRead::copy_tmp_file( char *tmpfilename, ofstream& fp_to)
{
  FILE *fp;
  char c;
  char cmd[80];

  fp = fopen( tmpfilename, "r");
  if ( !fp)
    return 0;
  while( (c = fgetc( fp)) != EOF)
    fp_to.put(c);
  fclose(fp);

#if defined OS_VMS
  sprintf( cmd, "delete/noconf/nolog %s.*", tmpfilename);
#else
  sprintf( cmd, "rm %s", tmpfilename);
#endif
  system( cmd);

  return 1;
}

