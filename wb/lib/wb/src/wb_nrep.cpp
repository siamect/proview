
#include "wb_nrep.h"
#include "wb_ldh_msg.h"

char wb_nrep::normname_tab[] = \
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!$!!!!!!!!-.!0123456789:!!!!!"\
			  "!ABCDEFGHIJKLMNOPQRSTUVWXYZ[!]!_"\
			  "!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "AAAAÄÅÆCEEEEIIII!NOOOOÖ×OUUUUY!!"\
			  "^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!";
char wb_nrep::idname_tab[] = \
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!$!!!()!!!-.!0123456789:!!!!!"\
			  "!ABCDEFGHIJKLMNOPQRSTUVWXYZ[!]!_"\
			  "!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "AAAAÄÅÆCEEEEIIII!NOOOOÖ×OUUUUY!!"\
			  "^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!";
char wb_nrep::objname_tab[] = \
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!$!!!!!!!!!!!0123456789!!!!!!"\
			  "!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_"\
			  "!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "AAAAÄÅÆCEEEEIIII!NOOOOÖ×OUUUUY!!"\
			  "^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!";
char wb_nrep::pathname_tab[] = \
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!$!!!!!!!!-!!0123456789:!!!!!"\
			  "!ABCDEFGHIJKLMNOPQRSTUVWXYZ!!!!_"\
			  "!^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"\
			  "AAAAÄÅÆCEEEEIIII!NOOOOÖ×OUUUUY!!"\
			  "^^^^^^^^^^^^^^^^!^^^^^^^^^^^^^!!";
char wb_nrep::unatname_tab[] = \
			  "================================"\
			  "================================"\
			  "================================"\
			  "================================"\
			  "================================"\
			  "================================"\
			  "AAAAAAACEEEEIIII=NOOOOOOOUUUUY=="\
			  "aaaaaaaceeeeiiii=nooooooouuuuy==";

#define nameDiff( s1, s2) ((long)(s1) - (long)(s2))

wb_nrep::wb_nrep() : m_nRef(0), num_seg(0), num_attr(0), vol_len(0), 
  seg(0), attr(0)
{
  strcpy( oname,"");
}

wb_nrep::wb_nrep( const char *n) throw (wb_error) : num_seg(0), num_attr(0), vol_len(0),
				   vol_offs(0), b_size(0), b_offset(0),
				   seg(0), attr(0)
{ 
  const char *s;
  int seg_cnt = 1;
  int attr_cnt = 0;
  int name_len = 0;

  if (*n != '_') {
    // Count number of segments and attributes

    for ( s = n; *s; s++) {
      if ( *s == '-') seg_cnt++;
      else if ( *s == '.') attr_cnt++;
      name_len++;
    }
    if ( name_len >= (int) sizeof( oname)) {
      throw wb_error(LDH__BADNAME);
      return;
    }
  }
  else {
    name_type = wname_eType_Name;
    int enable_cnt = 0;
    seg_cnt = 2;
    attr_cnt = 1;
    for ( s = n; *s; s++) {
      if ( *s == ')')
	enable_cnt = 1;
      else if ( *s == '[')
	enable_cnt = 0;
      else if ( *s == ']')
	enable_cnt = 1;
      if ( enable_cnt && *s == '.') attr_cnt++;
      name_len++;
    }
    if ( name_len >= (int) sizeof( oname)) {
      throw wb_error(LDH__BADNAME);
      return;
    }
  }
  if ( seg_cnt)
    seg = (wb_namesegments *) calloc( seg_cnt, sizeof( wb_namesegments));
  if ( attr_cnt) {
    attr = (wb_nameattributes *) calloc( attr_cnt, sizeof( wb_nameattributes));
    for ( int i = 0; i < attr_cnt; i++)
      attr[i].index = -1;
  }
  strcpy( oname, n);
  parse();
}

wb_nrep::wb_nrep( const wb_nrep& n)
{
  memcpy( this, &n, sizeof(*this));
  if ( num_seg) {
    seg = (wb_namesegments *) calloc( num_seg, sizeof(wb_namesegments));
    memcpy( seg, n.seg, num_seg * sizeof( wb_namesegments));
  }
  if ( num_attr) {
    attr = (wb_nameattributes *) calloc( num_attr, sizeof( wb_nameattributes));
    memcpy( attr, n.attr, num_attr * sizeof( wb_nameattributes));
  }
}

wb_nrep::~wb_nrep()
{
  free( seg);
  free( attr);
}

void wb_nrep::unref()
{
  if ( --m_nRef == 0)
    delete this;
}

wb_nrep *wb_nrep::ref()
{
  m_nRef++;
  return this;
}

void wb_nrep::parse () throw (wb_error)
{
  int state = 0;
  char *s, *snn;

  snn = norm_name;

  if (oname[0] == '_') {  // This is an identity name.
    for ( s = oname; *s; s++, snn++) {
      if (idname_tab[*(unsigned char *)s] == '!')
	throw wb_error(LDH__BADNAME);
      else if (idname_tab[*(unsigned char *)s] == '^')
	*snn = *s - 32;
      else
	*snn = idname_tab[*(unsigned char *)s];

      if ( s == oname)
        continue;
      if ( s == oname + 1) {
        switch ( *s) {
	  case 'V':
            name_type = wname_eType_Vid;
            continue;
	  case 'O':
            name_type = wname_eType_Oid;
            continue;
	  case 'A':
	    name_type = wname_eType_Aid;
            continue;
	  default:
            throw wb_error(LDH__BADNAME);
	}
      }

      /* States
          0: before ':'
	  1: before '('
	  2: before ')'
	  3: attribute before '['
	  4: attribute index before ']'
	  5: attribute after  ']'
	  6: offset.size before '.'
	  7: offset.size before ']'
	  8: offset.size after  ']'  */

      switch (state) {

      case 0:	// before ':'
	if ( *s == ':') {
	  if ( s == oname)
	    throw wb_error(LDH__BADNAME);
	  vol_offs = 2;
	  vol_len = nameDiff( s, oname) - vol_offs;
	  seg[0].offs = nameDiff( s + 1, oname);
	  state = 1;
	} else if (*s == '(' || *s == ')' || *s == '[' || *s == ']') {
	  throw wb_error(LDH__BADNAME);
	}
	break;
      case 1:	// before '('
	if ( *s == '(') {
          if ( nameDiff( s, oname) == seg[0].offs)
	    throw wb_error(LDH__BADNAME);
	  seg[1].offs = nameDiff( s + 1, oname);
	  seg[0].len = nameDiff( s, oname) - seg[0].offs;
	  num_seg++;
	  state = 2;
	} else if (*s == ')' || *s == '[' || *s == ']') {
	  throw wb_error(LDH__BADNAME);
	}
	break;
      case 2:	// before ')'
	if (*s == ')') {
          if ( nameDiff( s, oname) == seg[1].offs)
	    throw wb_error(LDH__BADNAME);
	  attr[0].offs = nameDiff( s + 1, oname);
	  seg[1].len = nameDiff( s, oname) - seg[1].offs;
	  num_seg++;
	  state = 3;
	} else if (*s == '[' || *s == ']') {
	  throw wb_error(LDH__BADNAME);
	}  
	break;
      case 3:	// after ')'
	if (*s == '[') {
	  if ( nameDiff( s, oname) == seg[1].offs + seg[1].len + 1) {
	    // offset.size
	    state = 7;	      
	  }
        } else if ( *s == ']' || *s == '.') {
          throw wb_error(LDH__BADNAME);
	} else {
	  attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
	  attr[num_attr].index = -1;
	  state = 4;
	}
	break;
      case 4:	// attribute before '['
	if (*s == '.') {
          if ( nameDiff( s, oname) == attr[num_attr].offs)
	    throw wb_error(LDH__BADNAME);
	  attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
          num_attr++;
	  attr[num_attr].offs = nameDiff( s + 1, oname);
	  state = 4;
	} else if (*s == '[') {
	  attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
	  attr[num_attr].index = 0;
	  state = 5;
	}
	break;
      case 5:	// attribute index before ']'
	if (*s == ']') {
	  num_attr++;
	  state = 6;
	} else if ((*s < '0') || (*s > '9')) {
	  throw wb_error(LDH__BADNAME);
	} else {
	  attr[num_attr].index = attr[num_attr].index * 10 + (*s - '0');
	  if (attr[num_attr].index > 65535) throw wb_error(LDH__BADNAME);
	}
	break;
      case 6:	// attribute after ']', '.' or nothing
	if ( *s == '.') {
	  attr[num_attr].offs = nameDiff( s + 1, oname);
	  state = 4;
        }
        else
	  throw wb_error(LDH__BADNAME);
	break;
      case 7:	// offset.size before '.'
	if (*s == '.') {
	  b_size = 0;
	  state = 8;
	} else if (*s == ']') {
	  throw wb_error(LDH__BADNAME);
	} else {
	  if ((*s < '0') || (*s > '9')) {
	    throw wb_error(LDH__BADNAME);
	  } else {
	    b_offset = b_offset * 10 + (*s - '0');
	    if (b_offset > 65535) throw wb_error(LDH__BADNAME);
	  }
	}
	break;
      case 8:	// offset.size before ']'
	if (*s == ']') {
	  state = 9;
	} else if ((*s < '0') || (*s > '9')) {
	  throw wb_error(LDH__BADNAME);
	} else {
	  b_size = b_size * 10 + (*s - '0');
	  if (b_size > 65535) throw wb_error(LDH__BADNAME);
	}
	break;
      case 9:	// offset.size after ']', must be nothing there!
	throw wb_error(LDH__BADNAME);
	break;
      }
    }

    switch ( state) {
      case 0:
        if ( s == oname) 
	  throw wb_error(LDH__BADNAME);  // No segment name
        vol_len = nameDiff( s, oname);
        break;
      case 1:
        if ( nameDiff( s, oname) == seg[num_seg].offs)
	  throw wb_error(LDH__BADNAME);
        if ( nameDiff( s, oname)  - (vol_len + 1) == 0)
          break;
	seg[num_seg].len = nameDiff( s, oname) - seg[num_seg].offs;
        if ( seg[num_seg].len)
	  num_seg++;
        break;
      case 4:
        if ( nameDiff( s, oname) == attr[num_attr].offs)
          throw wb_error(LDH__BADNAME);
	attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
        if ( attr[num_attr].len)
	  num_attr++;
	break;
      case 2:
      case 3:
      case 5:
      case 7:
      case 8:
        throw wb_error(LDH__BADNAME);
      default:
        ;
    }

    *snn = 0;
  }
  else {

    for ( s = oname; *s; s++, snn++) {
      if (normname_tab[*(unsigned char *)s] == '!')
	throw wb_error(LDH__BADNAME);
      else if (normname_tab[*(unsigned char *)s] == '^')
	*snn = *s - 32;
      else
	*snn = normname_tab[*(unsigned char *)s];

      /* States
	  0: before ':'
	  1: before '.'
	  2: before '['
	  3: before ']'
	  4: after  ']'  */

      switch (state) {
      case 0:	// before ':'
	if ( *s == ':') {
          if ( s == oname) 
	    throw wb_error(LDH__BADNAME);  // No volume name
	  vol_len = nameDiff( s, oname);
	  if ( *(s+1))
            seg[0].offs = nameDiff(s+1, oname);
	  state = 1;
	} else if ( *s == '-') {
          if ( s == oname) 
	    throw wb_error(LDH__BADNAME);  // No segment name
          seg[0].offs = 0;
          seg[0].len = nameDiff( s, oname);
	  num_seg++;
	  seg[1].offs = nameDiff( s+1, oname);
	  state = 1;
	} else if ( *s == '.') {
          if ( s != oname) {
	    seg[0].offs = 0;
            seg[0].offs = nameDiff( s, oname);
	    num_seg++;
	  }
	  attr[0].offs = nameDiff( s+1, oname);
	  state = 2;
	}
        else if ( *s == '[' || *s == ']')
          throw wb_error(LDH__BADNAME);
	break;
      case 1:	// before '.'
	if ( *s == '-') {
          if ( nameDiff( s, oname) == seg[num_seg].offs)
	    throw wb_error(LDH__BADNAME);
	  seg[num_seg+1].offs = nameDiff( s+1, oname);
	  seg[num_seg].len = seg[num_seg+1].offs - seg[num_seg].offs - 1;
	  num_seg++;
	} else if ( *s == '.') {
          if ( nameDiff( s, oname) == seg[num_seg].offs)
	    throw wb_error(LDH__BADNAME);
	  attr[0].offs = nameDiff( s+1, oname);
	  seg[num_seg].len = attr[0].offs - seg[num_seg].offs - 1;
	  num_seg++;
	  state = 2;
	}
	break;
      case 2:	// before '['
	if ( *s == '.') {
          if ( nameDiff( s, oname) == attr[num_attr].offs)
            throw wb_error(LDH__BADNAME);
	  attr[num_attr+1].offs = nameDiff( s+1, oname);
	  attr[num_attr].len = attr[num_attr+1].offs - attr[num_attr].offs - 1;
	  num_attr++;
	}
	else if ( *s == '[') {
          if ( nameDiff( s, oname) == attr[num_attr].offs)
            throw wb_error(LDH__BADNAME);
	  attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
	  attr[num_attr].index = 0;
	  state = 3;
	}
	break;
      case 3:	/* within brackets */
	if (*s == ']') {
	  num_attr++;
	  state = 4;
	} else if ((*s < '0') || (*s > '9')) {
	  throw wb_error(LDH__BADNAME);
	} else {
	  attr[num_attr].index = attr[num_attr].index * 10 + (*s - '0');
	  if (attr[num_attr].index > 65535) throw wb_error(LDH__BADNAME);
	}
	break;
      case 4:	// after brackets
        if ( *s == '.') {
	  attr[num_attr].offs = nameDiff( s+1, oname);
	  state = 2;
        }
        else
	  throw wb_error(LDH__BADNAME);
	break;
      }
    }

    switch ( state) {
      case 0:
        if ( s == oname) 
	  throw wb_error(LDH__BADNAME);  // No segment name
        seg[0].offs = 0;
        seg[0].len = nameDiff( s, oname);
	num_seg++;
        break;
      case 1:
        if ( nameDiff( s, oname) == seg[num_seg].offs)
	  throw wb_error(LDH__BADNAME);
        if ( nameDiff( s, oname)  - (vol_len + 1) == 0)
          break;
	seg[num_seg].len = nameDiff( s, oname) - seg[num_seg].offs;
        if ( seg[num_seg].len)
	  num_seg++;
        break;
      case 2:
        if ( nameDiff( s, oname) == attr[num_attr].offs)
          throw wb_error(LDH__BADNAME);
	attr[num_attr].len = nameDiff( s, oname) - attr[num_attr].offs;
        if ( attr[num_attr].len)
	  num_attr++;
	break;
      case 3:
        throw wb_error(LDH__BADNAME);
      case 4:
        break;
    }
    *snn = 0;
  }
}


char *wb_nrep::object( char *res)
{
  return objectName( oname, res);
}

char *wb_nrep::normObject( char *res)
{
  return objectName( norm_name, res);
}

char *wb_nrep::segment( int idx, char *res)
{
  return segmentName( oname, idx, res);
}

char *wb_nrep::normSegment( int idx, char *res)
{
  return segmentName( norm_name, idx, res);
}

char *wb_nrep::path( char *res)
{
  return pathName( oname, res);
}

char *wb_nrep::normPath( char *res)
{
  return pathName( norm_name, res);
}

char *wb_nrep::volume( char *res)
{
  return volumeName( oname, res);
}

char *wb_nrep::normVolume( char *res)
{
  return volumeName( norm_name, res);
}

char *wb_nrep::attribute( int idx, char *res)
{
  return attributeName( oname, idx, res);
}

char *wb_nrep::normAttribute( int idx, char *res)
{
  return attributeName( norm_name, idx, res);
}

bool wb_nrep::objectIsEqual( const char *n)
{
  const char *s1 = norm_name + seg[num_seg-1].offs;
  const char *s2 = n;
  char c;

  for ( int i = 0; i < seg[num_seg-1].len; i++) {
    if (normname_tab[*(unsigned char *)s2] == '^')
      c = *s2 - 32;
    else
      c = normname_tab[*(unsigned char *)s2];
    if ( *s1 != c)
      return false;
    s1++;
    s2++;
  }
  if ( *s2)
    return false;
  return true;
}

bool wb_nrep::volumeIsEqual(const char *n)
{
  const char *s1 = norm_name;
  const char *s2 = n;
  char c;

  for ( int i = 0; i < vol_len; i++) {
    if (normname_tab[*(unsigned char *)s2] == '^')
      c = *s2 - 32;
    else
      c = normname_tab[*(unsigned char *)s2];
    if ( *s1 != c)
      return false;
    s1++;
    s2++;
  }
  if ( *s2)
    return false;
  return true;
}

bool wb_nrep::segmentIsEqual(const char *n, int idx)
{
  const char *s1 = norm_name + seg[idx].offs;
  const char *s2 = n;
  char c;

  if ( idx < 0 || idx >= num_seg)
    return false;

  for ( int i = 0; i < seg[idx].len; i++) {
    if (normname_tab[*(unsigned char *)s2] == '^')
      c = *s2 - 32;
    else
      c = normname_tab[*(unsigned char *)s2];
    if ( *s1 != c)
      return false;
    s1++;
    s2++;
  }
  if ( *s2)
    return false;
  return true;
}

bool wb_nrep::attributeIsEqual(const char *n, int idx)
{
  const char *s1 = norm_name + attr[idx].offs;
  const char *s2 = n;
  char c;

  if ( idx < 0 || idx >= num_attr)
    return false;

  for ( int i = 0; i < attr[idx].len; i++) {
    if (normname_tab[*(unsigned char *)s2] == '^')
      c = *s2 - 32;
    else
      c = normname_tab[*(unsigned char *)s2];
    if ( *s1 != c)
      return false;
    s1++;
    s2++;
  }
  if ( *s2)
    return false;
  return true;
}

char *wb_nrep::objectName(const char *n, char *res)
{
  static char result[80];

  if ( !num_seg) {
    if ( res) {
      strcpy( res, "");
      return res;
    }
    else {
      strcpy( result, "");
      return result;
    }
  }
  else {
    if ( res) {
      strncpy( res, n + seg[num_seg-1].offs, seg[num_seg-1].len); 
      res[seg[num_seg-1].len] = 0;
      return res;
    }
    else {
      strncpy( result, n + seg[num_seg-1].offs, seg[num_seg-1].len); 
      result[seg[num_seg-1].len] = 0;
      return result;
    }
  }
}

char *wb_nrep::pathName(const char *n, char *res)
{
  static char result[80];

  if ( num_seg <= 1) {
    if ( res) {
      strcpy( res, "");
      return res;
    }
    else {
      strcpy( result, "");
      return result;
    }
  }
  else {
    if ( res) {
      strncpy( res, n + seg[0].offs, seg[num_seg-2].offs - seg[0].offs +
	       seg[num_seg-2].len); 
      res[seg[num_seg-2].offs - seg[0].offs +seg[num_seg-2].len] = 0;
      return res;
    }
    else {
      strncpy( result, n + seg[0].offs, seg[num_seg-2].offs - seg[0].offs +
	       seg[num_seg-2].len); 
      result[seg[num_seg-2].offs - seg[0].offs +seg[num_seg-2].len] = 0;
      return result;
    }
  }
}

char *wb_nrep::segmentName(const char *n, int idx, char *res)
{
  static char result[80];

  if ( idx >= num_seg || idx < 0) {
    if ( res) {
      strcpy( res, "");
      return res;
    }
    else {
      strcpy( result, "");
      return result;
    }
  }
  else {
    if ( res) {
      strncpy( res, n + seg[idx].offs, seg[idx].len); 
      res[seg[idx].len] = 0;
      return res;
    }
    else {
      strncpy( result, n + seg[idx].offs, seg[idx].len); 
      result[seg[idx].len] = 0;
      return result;
    }
  }
}

char *wb_nrep::attributeName(const char *n, int idx, char *res)
{
  static char result[80];

  if ( idx >= num_attr || idx < 0) {
    if ( res) {
      strcpy( res, "");
      return res;
    }
    else {
      strcpy( result, "");
      return result;
    }
  }
  else {
    if ( res) {
      strncpy( res, n + attr[idx].offs, attr[idx].len); 
      res[attr[idx].len] = 0;
      return res;
    }
    else {
      strncpy( result, n + attr[idx].offs, attr[idx].len); 
      result[attr[idx].len] = 0;
      return result;
    }
  }
}

char *wb_nrep::volumeName(const char *n, char *res)
{
  static char result[80];
  if ( vol_len == 0) {
    if ( res) {
      strcpy( res, "");
      return res;
    }
    else {
      strcpy( result, "");
      return result;
    }
  }
  else {
    if ( res) {
      strncpy( res, n + vol_offs, vol_len);
      res[vol_len] = 0;
      return res;
    }
    else {
      strncpy( result, n + vol_offs, vol_len);
      result[vol_len] = 0;
      return result;
    }
  }
  return result;
}

char *wb_nrep::name( int ntype, char *res)
{
  return nameName( oname, ntype, res);
}

char *wb_nrep::normName( int ntype, char *res)
{
  return nameName( norm_name, ntype, res);
}

char *wb_nrep::nameName(const char *n, int ntype, char *res)
{
  static char result[200];
  int colon_added = 0;

  if ( !res)
    res = result;
  if ( ntype & cdh_mName_volume && ntype & cdh_mName_object)
    ntype |= cdh_mName_path;
  if ( ntype & cdh_mName_path && ntype & cdh_mName_attribute)
    ntype |= cdh_mName_object;
  if ( ntype & cdh_mName_volume && ntype & cdh_mName_attribute)
    ntype = ntype | cdh_mName_path | cdh_mName_object;
  strcpy( res, "");

  if ( ntype & cdh_mName_idString) {
    if ( ntype & cdh_mName_volume) printf( "wname: volume\n");
    if ( ntype & cdh_mName_object) printf( "wname: object\n");
    if ( ntype & cdh_mName_attribute) printf( "wname: attribute\n");
    if ( !(ntype & cdh_mName_attribute)) {
      if ( ntype & cdh_mName_volume && !(ntype & cdh_mName_object))
	strcat( res, "_V");
      else if ( !(ntype & cdh_mName_volume))
	strcat( res, "_X");
      else
	strcat( res, "_O");
    }
    else
      strcat( res, "_A");
  }
  if ( ntype & cdh_mName_volume)
    volumeName( n, res + strlen(res));
  if ( ntype & cdh_mName_path) {
    if ( ntype & cdh_mName_volume && hasVolume()) {
      strcat( res, ":");
      colon_added = 1;
    }
    pathName( n, res + strlen(res));
  }
  if ( ntype & cdh_mName_object) {
    if ( ntype & cdh_mName_path && hasPath())
      strcat( res, "-");
    else if ( ntype & cdh_mName_volume && !hasPath() && hasVolume() && !colon_added)
      strcat( res, ":");
    objectName( n, res + strlen(res));
  }
  if ( ntype & cdh_mName_attribute && hasAttribute()) {
    strcat( res + strlen(res), ".");
    strcat( res + strlen(res), n + attr[0].offs);
  }
  return res;
}

char *wb_nrep::unatName(const char *name)
{
  static char result[80];
  const char *s;
  char *su;

  su = result;

  for ( s = name; *s; s++, su++) {
    if (unatname_tab[*(unsigned char *)s] == '=')
      *su = *s;
    else
     *su = unatname_tab[ *(unsigned char *)s];
  }
  *su = 0;
  return result;
}

bool wb_nrep::checkObjectName( const char *name)
{
  const char *s;

  for ( s = name; *s; s++) {
    if ( objname_tab[*(unsigned char *)s] == '!')
      return false;
  }
  return true;
}





