#ifndef INC_wb_wblparserTokenTypes_hpp_
#define INC_wb_wblparserTokenTypes_hpp_

/* $ANTLR 2.7.1: "wb_wblparser.g" -> "wb_wblparserTokenTypes.hpp"$ */
struct wb_wblparserTokenTypes {
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
		EQ = 23,
		OREQ = 24,
		WS = 25,
		COMMENT = 26,
		INDEX = 27,
		VALUE = 28,
		CHAR_LITERAL = 29,
		STRING_LITERAL = 30,
		DIGITS = 31,
		ESC = 32,
		SWEC = 33,
		NULL_TREE_LOOKAHEAD = 3
	};
};
#endif /*INC_wb_wblparserTokenTypes_hpp_*/
