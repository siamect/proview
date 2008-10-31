/* 
 * Proview   $Id: cnv_classread.h,v 1.12 2008-10-31 12:51:30 claes Exp $
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

#ifndef wb_classread_h
#define wb_classread_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"


using namespace std;

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

class CnvFile {
 public:
    ofstream 	f;
};
  
class ClassRead {
  public:
    ClassRead() : first_class(1), verbose(0), generate_html(0), 
      html_class_open(0), html_index_open(0), 
      generate_xtthelp(0), xtthelp_index_open(0), xtthelp_in_topic(0),
      generate_src(0), generate_struct(0), struct_class_open(0),
      common_structfile_only(0), hpp(0), setup_group_cnt(0)
      { strcpy( setup_filename, "");};
    ~ClassRead() {};
    
    FILE		*fp;
    int			first_class;
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
    int			bit_value;
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

    int read( char *filename);
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
    int doc_add( char *line);
    int doc_close();
    void volume_init();
    int object_close();
    char *low( char *in);
    int copy_tmp_file( const char *tmpfilename, ofstream& fp_to);
    char *flags_to_string( int value);

    // html
    int		generate_html;
    // ofstream 	fp_html_class;
    CnvFile     *html_clf;
    ofstream 	fp_html_index;
    ofstream 	fp_js_all;
    ofstream 	fp_tmp;
    ofstream	fp_html_group[MAX_GROUPS];
    ofstream	fp_js_group[MAX_GROUPS];
    char	html_first[80];
    char	html_tmp_name[80];
    int		html_class_open;
    int		html_index_open;
    bool	js_all_first;
    bool	js_group_first[80];

    int html_init( char *first);
    int html_close();
    int html_class();
    int html_class_close();
    int html_body();
    int html_body_close();
    int html_attribute();
    int html_graphplcnode();
    int html_graphplccon();
    int html_template();
    int html_typedef();
    int html_typedef_close();
    int html_bit();

    // xtthelp
    int		generate_xtthelp;
    ofstream 	fp_xtthelp_index;
    int		xtthelp_index_open;
    int         xtthelp_in_topic;

    int xtthelp_init();
    int xtthelp_close();
    int xtthelp_class();
    int xtthelp_class_close();
    int xtthelp_body();
    int xtthelp_attribute();
    int xtthelp_bit();

    // src
    int		generate_src;
    char	src_aref[80];
    char	src_aref_text[200];
    ofstream 	fp_src_html;

    int src_read( char *filename);
    int src_html_init( char *filename);
    int src_html_close();
    int src_html_line( char *line);
    int src_html_aref();
    void src_filename_to_html( char *fname, char *filename);

    // struct
    int		generate_struct;
    int         struct_class_open;
    int		common_structfile_only;
    ofstream 	fp_struct;
    // ofstream 	fp_cstruc;
    CnvFile     *cstruc;
    int		struct_cclass_written;
    int		struct_cclass_endwritten;
    int		struct_filler_cnt;
    char	struct_volid[80];
    unsigned int	struct_vid_0;
    unsigned int	struct_vid_1;
    int		attr_count;
    int		hpp;

    int struct_init();
    int struct_close();
    int struct_class();
    int struct_class_close();
    int struct_body();
    int struct_body_close();
    int struct_attribute();
    int struct_typedef();
    int struct_typedef_close();
    int struct_bit();
    int struct_volname_to_id();
    void struct_cix_to_classid( unsigned int cix, pwr_tClassId *cid);
    int struct_cixstr_to_classid( char *cix_str, pwr_tClassId *cid);
    void struct_get_filename( char *struct_file);
    int struct_check_typename( char *type_volume, char *type_name);

    // Setup
    char setup_filename[120];
    int setup_group_cnt;
    char setup_groups[MAX_GROUPS][40];
    char setup_groups_startpage[MAX_GROUPS][80];
    int setup();
};

#if defined __cplusplus
}
#endif
#endif

