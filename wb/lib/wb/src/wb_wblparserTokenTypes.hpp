#ifndef INC_wb_wblparserTokenTypes_hpp_
#define INC_wb_wblparserTokenTypes_hpp_

/* $ANTLR 2.7.3 (20040413-1): "wb_wblparser.g" -> "wb_wblparserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API wb_wblparserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		OBJECT = 4,
		ENDOBJECT = 5,
		VOLUME = 6,
		ENDVOLUME = 7,
		SOBJECT = 8,
		ENDSOBJECT = 9,
		BODY = 10,
		ENDBODY = 11,
		ATTRIBUTE = 12,
		BUFFER = 13,
		ENDBUFFER = 14,
		DOBJECT = 15,
		ENDDOBJECT = 16,
		DBUFFER = 17,
		ENDDBUFFER = 18,
		DATTRIBUTE = 19,
		NUM_FLOAT = 20,
		INT = 21,
		OID = 22,
		DOCBLOCK = 23,
		EQ = 24,
		OREQ = 25,
		WS = 26,
		COMMENT = 27,
		INDEX = 28,
		VALUE = 29,
		CHAR_LITERAL = 30,
		STRING_LITERAL = 31,
		DIGITS = 32,
		ESC = 33,
		SWEC = 34,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_wb_wblparserTokenTypes_hpp_*/
