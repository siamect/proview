/* $ANTLR 2.7.1: "wb_wblparser.g" -> "wb_wblparser.cpp"$ */
#include "wb_wblparser.hpp"
#include "antlr/NoViableAltException.hpp"
#include "antlr/SemanticException.hpp"
#line 1 "wb_wblparser.g"

#line 8 "wb_wblparser.cpp"
wb_wblparser::wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
	setTokenNames(_tokenNames);
}

wb_wblparser::wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
	setTokenNames(_tokenNames);
}

wb_wblparser::wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
	setTokenNames(_tokenNames);
}

wb_wblparser::wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
	setTokenNames(_tokenNames);
}

wb_wblparser::wb_wblparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
	setTokenNames(_tokenNames);
}

void wb_wblparser::unit() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode unit_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		{
		int _cnt3=0;
		for (;;) {
			switch ( LA(1)) {
			case VOLUME:
			{
				volume();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				break;
			}
			case SOBJECT:
			{
				sobject();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				break;
			}
			case OBJECT:
			{
				object();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				break;
			}
			default:
			{
				if ( _cnt3>=1 ) { goto _loop3; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			}
			_cnt3++;
		}
		_loop3:;
		}
		unit_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_0);
	}
	returnAST = unit_AST;
}

void wb_wblparser::volume() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode volume_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ref_wblnode tmp1_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp1_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp1_AST));
		match(VOLUME);
		cid();
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		ref_wblnode tmp2_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp2_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp2_AST));
		match(OID);
		{
		for (;;) {
			if ((LA(1)==OBJECT)) {
				object();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop6;
			}
			
		}
		_loop6:;
		}
		ref_wblnode tmp3_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp3_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp3_AST));
		match(ENDVOLUME);
		volume_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_1);
	}
	returnAST = volume_AST;
}

void wb_wblparser::sobject() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode sobject_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ref_wblnode tmp4_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp4_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp4_AST));
		match(SOBJECT);
		{
		for (;;) {
			if ((LA(1)==OBJECT)) {
				object();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop9;
			}
			
		}
		_loop9:;
		}
		ref_wblnode tmp5_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp5_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp5_AST));
		match(ENDSOBJECT);
		sobject_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_1);
	}
	returnAST = sobject_AST;
}

void wb_wblparser::object() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode object_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ref_wblnode tmp6_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp6_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
		match(OBJECT);
		cid();
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		{
		switch ( LA(1)) {
		case INT:
		case VALUE:
		{
			oix();
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			break;
		}
		case OBJECT:
		case ENDOBJECT:
		case BODY:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		for (;;) {
			if ((LA(1)==BODY)) {
				body();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop13;
			}
			
		}
		_loop13:;
		}
		{
		for (;;) {
			if ((LA(1)==OBJECT)) {
				object();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop15;
			}
			
		}
		_loop15:;
		}
		ref_wblnode tmp7_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp7_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
		match(ENDOBJECT);
		object_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_2);
	}
	returnAST = object_AST;
}

void wb_wblparser::cid() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode cid_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case VALUE:
		{
			ref_wblnode tmp8_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp8_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp8_AST));
			match(VALUE);
			cid_AST = ref_wblnode(currentAST.root);
			break;
		}
		case STRING_LITERAL:
		{
			ref_wblnode tmp9_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp9_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp9_AST));
			match(STRING_LITERAL);
			cid_AST = ref_wblnode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_3);
	}
	returnAST = cid_AST;
}

void wb_wblparser::oix() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode oix_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case VALUE:
		{
			ref_wblnode tmp10_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp10_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp10_AST));
			match(VALUE);
			oix_AST = ref_wblnode(currentAST.root);
			break;
		}
		case INT:
		{
			ref_wblnode tmp11_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp11_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp11_AST));
			match(INT);
			oix_AST = ref_wblnode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_4);
	}
	returnAST = oix_AST;
}

void wb_wblparser::body() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode body_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ref_wblnode tmp12_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp12_AST = astFactory.create(LT(1));
		astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp12_AST));
		match(BODY);
		{
		for (;;) {
			if ((LA(1)==ATTRIBUTE||LA(1)==BUFFER)) {
				attribute();
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			else {
				goto _loop18;
			}
			
		}
		_loop18:;
		}
		ref_wblnode tmp13_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp13_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp13_AST));
		match(ENDBODY);
		body_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_4);
	}
	returnAST = body_AST;
}

void wb_wblparser::attribute() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode attribute_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case ATTRIBUTE:
		{
			ref_wblnode tmp14_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp14_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp14_AST));
			match(ATTRIBUTE);
			{
			switch ( LA(1)) {
			case EQ:
			{
				ref_wblnode tmp15_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp15_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp15_AST));
				match(EQ);
				break;
			}
			case OREQ:
			{
				ref_wblnode tmp16_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp16_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp16_AST));
				match(OREQ);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			value();
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			attribute_AST = ref_wblnode(currentAST.root);
			break;
		}
		case BUFFER:
		{
			ref_wblnode tmp17_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp17_AST = astFactory.create(LT(1));
			astFactory.makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp17_AST));
			match(BUFFER);
			{
			switch ( LA(1)) {
			case INDEX:
			{
				ref_wblnode tmp18_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp18_AST = astFactory.create(LT(1));
				astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp18_AST));
				match(INDEX);
				break;
			}
			case ATTRIBUTE:
			case BUFFER:
			case ENDBUFFER:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			for (;;) {
				if ((LA(1)==ATTRIBUTE||LA(1)==BUFFER)) {
					attribute();
					astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
				else {
					goto _loop23;
				}
				
			}
			_loop23:;
			}
			ref_wblnode tmp19_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp19_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp19_AST));
			match(ENDBUFFER);
			attribute_AST = ref_wblnode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_5);
	}
	returnAST = attribute_AST;
}

void wb_wblparser::value() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode value_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		switch ( LA(1)) {
		case VALUE:
		{
			ref_wblnode tmp20_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp20_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp20_AST));
			match(VALUE);
			value_AST = ref_wblnode(currentAST.root);
			break;
		}
		case STRING_LITERAL:
		{
			ref_wblnode tmp21_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp21_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp21_AST));
			match(STRING_LITERAL);
			value_AST = ref_wblnode(currentAST.root);
			break;
		}
		case NUM_FLOAT:
		{
			ref_wblnode tmp22_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp22_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp22_AST));
			match(NUM_FLOAT);
			value_AST = ref_wblnode(currentAST.root);
			break;
		}
		case INT:
		{
			ref_wblnode tmp23_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp23_AST = astFactory.create(LT(1));
			astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp23_AST));
			match(INT);
			value_AST = ref_wblnode(currentAST.root);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_5);
	}
	returnAST = value_AST;
}

void wb_wblparser::oname() {
	returnAST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ref_wblnode oname_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		ref_wblnode tmp24_AST = static_cast<ref_wblnode>(ANTLR_USE_NAMESPACE(antlr)nullAST);
		tmp24_AST = astFactory.create(LT(1));
		astFactory.addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp24_AST));
		match(VALUE);
		oname_AST = ref_wblnode(currentAST.root);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_0);
	}
	returnAST = oname_AST;
}

ref_wblnode wb_wblparser::getAST()
{
	return returnAST;
}

const char* wb_wblparser::_tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"OBJECT",
	"ENDOBJECT",
	"VOLUME",
	"ENDVOLUME",
	"SOBJECT",
	"ENDSOBJECT",
	"BODY",
	"ENDBODY",
	"ATTRIBUTE",
	"BUFFER",
	"ENDBUFFER",
	"DOBJECT",
	"ENDDOBJECT",
	"DBUFFER",
	"ENDDBUFFER",
	"DATTRIBUTE",
	"NUM_FLOAT",
	"INT",
	"OID",
	"EQ",
	"OREQ",
	"WS",
	"COMMENT",
	"an index",
	"an identifer",
	"CHAR_LITERAL",
	"STRING_LITERAL",
	"DIGITS",
	"ESC",
	"SWEC",
	0
};

const unsigned long wb_wblparser::_tokenSet_0_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long wb_wblparser::_tokenSet_1_data_[] = { 338UL, 0UL, 0UL, 0UL };
// EOF OBJECT VOLUME SOBJECT 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long wb_wblparser::_tokenSet_2_data_[] = { 1010UL, 0UL, 0UL, 0UL };
// EOF OBJECT ENDOBJECT VOLUME ENDVOLUME SOBJECT ENDSOBJECT 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long wb_wblparser::_tokenSet_3_data_[] = { 274727984UL, 0UL, 0UL, 0UL };
// OBJECT ENDOBJECT BODY INT OID VALUE 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long wb_wblparser::_tokenSet_4_data_[] = { 1072UL, 0UL, 0UL, 0UL };
// OBJECT ENDOBJECT BODY 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long wb_wblparser::_tokenSet_5_data_[] = { 30720UL, 0UL, 0UL, 0UL };
// ENDBODY ATTRIBUTE BUFFER ENDBUFFER 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wblparser::_tokenSet_5(_tokenSet_5_data_,4);


