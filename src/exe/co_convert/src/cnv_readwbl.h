/* 
 * Proview   $Id: cnv_readwbl.h,v 1.6 2008-06-24 06:53:19 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef cnv_readwbl_h
#define cnv_readwbl_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "cnv_file.h"

using namespace std;

class CnvCtx;
class CnvWblTo;

#define MAX_GROUPS 100

typedef enum {
	cread_eLine_No,
	cread_eLine_EOF,
	cread_eLine_SObject,
	cread_eLine_EndSObject,
	cread_eLine_ClassDef,
	cread_eLine_ObjBodyDef,
	cread_eLine_SysBody,
	cread_eLine_RtBody,
	cread_eLine_DevBody,
	cread_eLine_EndBody,
	cread_eLine_Input,
	cread_eLine_Output,
	cread_eLine_Intern,
	cread_eLine_Attribute,
	cread_eLine_ObjXRef,
	cread_eLine_Attr,
	cread_eLine_GraphPlcNode,
	cread_eLine_GraphPlcCon,
	cread_eLine_Buffer,
	cread_eLine_Object,
	cread_eLine_EndObject,
	cread_eLine_Doc,
	cread_eLine_DocEnd,
	cread_eLine_Template,
	cread_eLine_Comment,
	cread_eLine_Volume,
	cread_eLine_EndVolume,
	cread_eLine_DParam,
	cread_eLine_DAttr,
	cread_eLine_Buff,
	cread_eLine_EndBuff,
	cread_eLine_Range,
	cread_eLine_TypeDef,
	cread_eLine_Type,
	cread_eLine_Bit,
	cread_eLine_Value,
	cread_eLine_Unknown
	} cread_eLine;

typedef enum {
	cread_mState_SObject 	= 1 << 0,
	cread_mState_ClassDef	= 1 << 1,
	cread_mState_ObjBodyDef	= 1 << 2,
	cread_mState_SysBody	= 1 << 3,
	cread_mState_RtBody	= 1 << 4,
	cread_mState_DevBody	= 1 << 5,
	cread_mState_Input	= 1 << 6,
	cread_mState_Output	= 1 << 7,
	cread_mState_Intern	= 1 << 8,
	cread_mState_Attribute	= 1 << 9,
	cread_mState_ObjXRef	= 1 << 10,
	cread_mState_GraphPlcNode = 1 << 11,
	cread_mState_GraphPlcCon = 1 << 12,
	cread_mState_Buffer	= 1 << 13,
	cread_mState_Object	= 1 << 14,
	cread_mState_Doc	= 1 << 15,
	cread_mState_Template	= 1 << 16,
	cread_mState_Volume	= 1 << 17,
	cread_mState_DParam	= 1 << 18,
	cread_mState_TypeDef	= 1 << 19,
	cread_mState_Type	= 1 << 20,
	cread_mState_StringAttr	= 1 << 21,
	cread_mState_Bit	= 1 << 22,
	cread_mState_Value	= 1 << 23
	} cread_mState;

#define cread_cTmpFile1 "classread1.tmp"
#define cread_cTmpFile2 "classread2.tmp"
#define cread_cTmpFile3 "classread3.tmp"
  
class CnvReadWbl {
  public:
    CnvReadWbl( CnvCtx *cnv_ctx, CnvWblTo *cnv_wblto) : 
      ctx(cnv_ctx), wblto(cnv_wblto)
      {}
    ~CnvReadWbl() {}
    
    CnvCtx		*ctx;
    CnvWblTo		*wblto;
    FILE		*fp;
    cread_eLine		linetype;
    int			verbose;
    int			state;
    int			object_state;
    char		source_dir[200];
    char		current_file[200];
    char		attr_name[80];
    char		attr_flags[200];
    char		attr_type[80];
    char		attr_typeref[80];
    char		attr_typeref_volume[80];
    int			attr_pointer;
    int			attr_array;
    int			attr_rtvirtual;
    int			attr_isclass;
    char		attr_elements[80];
    int			attr_elem;
    char		attr_pgmname[80];
    char		volume_name[80];
    char		class_id[80];
    char		class_name[80];
    char		class_editor[80];
    char		class_method[80];
    char		class_popeditor[80];
    char		class_flags[200];
    int			class_devonly;
    char		class_version[80];
    char		body_name[80];
    char		body_structname[80];
    char		body_flags[200];
    int			body_rtvirtual;
    char		graphplcnode_name[80];
    char		graphplccon_name[20];
    char		typedef_name[80];
    char		typedef_typeref[80];
    int			typedef_elements;
    char		typedef_pgmname[32];
    char		bit_name[80];
    char		bit_type[80];
    unsigned int       	bit_value;
    char		bit_text[80];
    char		bit_pgmname[80];
    char		doc_author[200];
    char		doc_version[80];
    char		doc_groups[10][40];
    int			doc_group_cnt;
    char		doc_code[200];
    char                doc_link_ref[20][80];
    char                doc_link_text[20][200];
    int                 doc_link_cnt;
    char                doc_clink_ref[20][80];
    char                doc_clink_text[20][200];
    int                 doc_clink_cnt;
    char		doc_summary[400];
    char		doc_text[1000][200];
    int			doc_cnt;
    int			doc_fresh;
    char		sobject_name[80];
    char		dir[120];

    char		ClassDef[80];
    char		ObjBodyDef[80];
    char		GraphPlcNode[80];
    char		GraphPlcCon[80];
    char		Attribute[80];

    int read_wbl( char *filename);
    int read_line( char	*line, int maxsize, FILE *file);
    int	remove_spaces( char *in, char *out);
    void attribute_init();
    int attribute_attr( char *name, char *value);
    int attribute_close();
    void class_init();
    int class_attr( char *name, char *value);
    int class_close();
    void typedef_init();
    int typedef_attr( char *name, char *value);
    int typedef_close();
    void bit_init();
    int bit_attr( char *name, char *value);
    int bit_close();
    void body_init();
    int body_attr( char *name, char *value);
    int body_close();
    void graphplcnode_init();
    int graphplcnode_attr( char *name, char *value);
    int graphplcnode_close();
    void graphplccon_init();
    int graphplccon_attr( char *name, char *value);
    int graphplccon_close();
    void template_init();
    int template_attr( char *name, char *value);
    int template_close();
    void doc_init();
    void doc_init_keep();
    int doc_add( char *line);
    int doc_close();
    void volume_init();
    int object_close();
    char *low( char *in);
    int copy_tmp_file( char *tmpfilename, ofstream& fp_to);
    char *flags_to_string( int value);
    int read_lng( char *cname, char *aname);
};

#if defined __cplusplus
}
#endif
#endif

