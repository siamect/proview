#ifndef INC_wb_wblparserTokenTypes_hpp_
#define INC_wb_wblparserTokenTypes_hpp_

/* $ANTLR 2.7.4: "wb_wblparser.g" -> "wb_wblparserTokenTypes.hpp"$ */

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
		ASC_TIME = 24,
		EQ = 25,
		OREQ = 26,
		WS = 27,
		COMMENT = 28,
		INDEX = 29,
		VALUE = 30,
		CHAR_LITERAL = 31,
		STRING_LITERAL = 32,
		DIGITS = 33,
		ESC = 34,
		SWEC = 35,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_wb_wblparserTokenTypes_hpp_*/
