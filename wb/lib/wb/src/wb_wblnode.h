#ifndef wb_wblnode_h
#define wb_wblnode_h

#include <iostream.h>
#include "antlr/CommonAST.hpp"
#include "pwr.h"
#include "wb_name.h"
#include "wb_wblfile.h"


ANTLR_USING_NAMESPACE(std)
ANTLR_USING_NAMESPACE(antlr)

class wb_wblnode;
class wb_vrepwbl;

#define wbl_cixToCid( Vid, Cix) (0 + (Vid << 16) +  (Cix << 3))
#define wbl_tixToTid( Vid, Tyg, Tix) (0 + (Vid << 16) + (1 << 15) + (Tyg << 11) +  Tix)
#define wbl_cixToOix( Cix, Bix, Aix) (0 + (1 << 31) + (Cix << 18) + (Bix << 15) + Aix)
#define wbl_tixToOix( Tyg, Tix) (0 + (1 << 31) + (1 << 30) + (Tyg << 26) + (Tix << 15))

typedef enum {
  wbl_eNodeType_No,
  wbl_eNodeType_Type,
  wbl_eNodeType_TypeDef,
  wbl_eNodeType_ClassDef,
  wbl_eNodeType_ObjBodyDef,
  wbl_eNodeType_Attribute,
  wbl_eNodeType_BodyNode,
  wbl_eNodeType_AttrNode,
  wbl_eNodeType_Template,
  wbl_eNodeType_Volume,
  wbl_eNodeType_Buffer
} wbl_eNodeType;

typedef antlr::ASTRefCount<wb_wblnode> ref_wblnode;

typedef struct {
  char	*sym;
  int	value;
} wbl_sSym;

class wb_wblnode : public CommonAST {
public:
    wb_wblnode() : 
      node_type(wbl_eNodeType_No), 
      rbody_size(0), dbody_size(0), rbody(0), dbody(0), m_cid(0),
      m_tid(0), m_oid(pwr_cNOid), m_vrep(0), o_fth(0), o_bws(0), o_fws(0), o_fch(0), 
      line_number(0), file(0), is_built(0), c_cid(0), c_cix(0), c_template(0), 
      b_bix(0), a_tid(0),  a_size(0), a_offset(0), a_elements(0), a_flags(0)
    {
      strcpy( name, "");
      strcpy( cname, "");
    }
    wb_wblnode(antlr::RefToken t) : 
      node_type(wbl_eNodeType_No), 
      rbody_size(0), dbody_size(0), rbody(0), dbody(0), m_cid(0),
      m_tid(0), m_oid(pwr_cNOid), m_vrep(0), o_fth(0), o_bws(0), o_fws(0), o_fch(0), 
      line_number(0), file(0), is_built(0), c_cid(0), c_cix(0), c_template(0), 
      b_bix(0), a_tid(0),  a_size(0), a_offset(0), a_elements(0), a_flags(0)
    {
      CommonAST::setType(t->getType());
      CommonAST::setText(t->getText());
    }
    ~wb_wblnode() 
    {
      if ( rbody_size) free( rbody);
      if ( dbody_size) free( dbody);
    }

    void initialize(int t, const std::string& txt)
    {
      CommonAST::setType(t);
      CommonAST::setText(txt);
    }

    void initialize( ref_wblnode t)
    {
      CommonAST::setType(t->getType());
      CommonAST::setText(t->getText());
    }
    void initialize( RefAST t )
    {
      CommonAST::initialize(t);
    }
    void initialize( antlr::RefToken t )
    {
      CommonAST::initialize(t);
      line_number = t->getLine();
    }

    void setText(const std::string& txt)
    {
      CommonAST::setText(txt);
    }

    void setType(int type)
    {
      CommonAST::setType(type);
    }

    void addChild( ref_wblnode c )
    {
      BaseAST::addChild( RefAST(c));
    }

    static antlr::RefAST factory( void )
    {
      antlr::RefAST ret = RefAST(new wb_wblnode);
      return ret;
    }

    ref_wblnode wb_wblnode::getFirstChild()
    {
      return ref_wblnode(BaseAST::getFirstChild());
    }

    ref_wblnode wb_wblnode::getNextSibling()
    {
       return ref_wblnode(BaseAST::getNextSibling());
    }


    // Heterogeneous part
    void info( int level) {
      for ( int i = 0; i < level; i++)
        cout << "  ";
      cout << getType() << " " << getText() << endl;
      ref_wblnode t = getFirstChild();
      if ( t)
        t->info( level + 1);

      t = getNextSibling();
      if ( t)
        t->info( level);
    }

    bool isType() { return (node_type == wbl_eNodeType_Type);}
    bool isTypeDef() { return (node_type == wbl_eNodeType_TypeDef);}
    bool isClassDef() { return (node_type == wbl_eNodeType_ClassDef);}
    bool isObjBodyDef() { return (node_type == wbl_eNodeType_ObjBodyDef);}
    bool isAttribute() { return (node_type == wbl_eNodeType_Attribute);}
    bool isBodyNode() { return (node_type == wbl_eNodeType_BodyNode);}
    bool isAttrNode() { return (node_type == wbl_eNodeType_AttrNode);}
    bool isTemplate() { return (node_type == wbl_eNodeType_Template);}
    bool isVolume() { return (node_type == wbl_eNodeType_Volume);}
    bool isBuffer() { return (node_type == wbl_eNodeType_Buffer);}

    void setFile( wb_wblfile *f);
    char *getFileName() { if ( file) return file->file_name; else return "";}
    pwr_tTime getFileTime() 
    {
      if ( file) return file->time; 
      else { pwr_tTime t = {0,0}; return t;} 
    }
    void registerNode( wb_vrepwbl *vol);
    void build( bool recursive);
    void buildObjBodyDef( ref_wblnode classdef);
    void buildAttribute( ref_wblnode classdef, ref_wblnode objbodydef, 
			 int *bindex, int *boffset);
    void buildBuffer( ref_wblnode classdef, ref_wblnode objbodydef, 
			 int *bindex, int *boffset);
    void buildTemplate( ref_wblnode classdef);
    void buildBody( ref_wblnode object);
    void buildAttr( ref_wblnode object, int bix);
    void link( wb_vrepwbl *vol, ref_wblnode father_node);
    void info_link( int level);
    ref_wblnode find( wb_name *oname, int level);
    ref_wblnode get_o_lch();
    int classNameToCid( char *cname, pwr_tCid *cid);
    int stringToOix( const char *buf, pwr_tOix *oix) const;
    pwr_tCid Cid() { return c_cid;}
    int attrStringToValue( int type_id, char *value_str, 
			   void *buffer_ptr, int buff_size, int attr_size);
    void iterObject( void *udata, 
		     pwr_tStatus (*bc)(void *, pwr_tOid, pwr_tCid, pwr_tOid, pwr_tOid,
				       pwr_tOid, pwr_tOid, pwr_tOid, char *,
				       pwr_tTime, int, int));
    void iterBody( void *udata, 
		   pwr_tStatus (*bc)(void *, pwr_tOid, void *, void *));

    static int lookup( int *type, const char *keyword, wbl_sSym *table);
    static int convconst( int *val, char *str);

    wbl_eNodeType node_type;
    int rbody_size;
    int dbody_size;
    void *rbody;
    void *dbody;
    pwr_tCid m_cid;
    pwr_tTid m_tid;
    pwr_tOid m_oid;
    char name[200];
    char cname[32];
    wb_vrepwbl *m_vrep;
    wb_wblnode *o_fth;
    wb_wblnode *o_bws;
    wb_wblnode *o_fws;
    wb_wblnode *o_fch;
    int line_number;
    wb_wblfile *file;
    int is_built;

    // Classdef stuff
    pwr_tCid c_cid;
    int c_cix;
    ref_wblnode c_template;

    // Type and Typedef stuff
    pwr_tTid ty_tid;
    int ty_elements;
    int ty_size;

    // ObjBodyDef stuff
    int b_bix;
    int b_size;

    // Attribute stuff
    pwr_tTid a_tid;
    int a_size;
    int a_offset;
    int a_elements;
    int a_flags;
    int a_index;

    // Template object stuff
};


#endif








