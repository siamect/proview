/* $ANTLR 2.7.1: "wb_wbllexer.g" -> "wb_wbllexer.cpp"$ */
#include "wb_wbllexer.hpp"
#include "antlr/CharBuffer.hpp"
#include "antlr/TokenStreamException.hpp"
#include "antlr/TokenStreamIOException.hpp"
#include "antlr/TokenStreamRecognitionException.hpp"
#include "antlr/CharStreamException.hpp"
#include "antlr/CharStreamIOException.hpp"
#include "antlr/NoViableAltForCharException.hpp"

#line 1 "wb_wbllexer.g"

#line 14 "wb_wbllexer.cpp"
wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(std)istream& in)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in))
{
	setCaseSensitive(true);
	initLiterals();
}

wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(ib)
{
	setCaseSensitive(true);
	initLiterals();
}

wb_wbllexer::wb_wbllexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(state)
{
	setCaseSensitive(true);
	initLiterals();
}

void wb_wbllexer::initLiterals()
{
}
bool wb_wbllexer::getCaseSensitiveLiterals() const
{
	return true;
}

ANTLR_USE_NAMESPACE(antlr)RefToken wb_wbllexer::nextToken()
{
	ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
	for (;;) {
		ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
		int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
		resetText();
		try {   // for char stream error handling
			try {   // for lexical error handling
				switch ( LA(1)) {
				case static_cast<unsigned char>('='):
				{
					mEQ(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('|'):
				{
					mOREQ(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('\t'):
				case static_cast<unsigned char>('\n'):
				case static_cast<unsigned char>('\r'):
				case static_cast<unsigned char>(' '):
				{
					mWS(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('!'):
				{
					mCOMMENT(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('['):
				{
					mINDEX(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('$'):
				case static_cast<unsigned char>('-'):
				case static_cast<unsigned char>('.'):
				case static_cast<unsigned char>('0'):
				case static_cast<unsigned char>('1'):
				case static_cast<unsigned char>('2'):
				case static_cast<unsigned char>('3'):
				case static_cast<unsigned char>('4'):
				case static_cast<unsigned char>('5'):
				case static_cast<unsigned char>('6'):
				case static_cast<unsigned char>('7'):
				case static_cast<unsigned char>('8'):
				case static_cast<unsigned char>('9'):
				case static_cast<unsigned char>('A'):
				case static_cast<unsigned char>('B'):
				case static_cast<unsigned char>('C'):
				case static_cast<unsigned char>('D'):
				case static_cast<unsigned char>('E'):
				case static_cast<unsigned char>('F'):
				case static_cast<unsigned char>('G'):
				case static_cast<unsigned char>('H'):
				case static_cast<unsigned char>('I'):
				case static_cast<unsigned char>('J'):
				case static_cast<unsigned char>('K'):
				case static_cast<unsigned char>('L'):
				case static_cast<unsigned char>('M'):
				case static_cast<unsigned char>('N'):
				case static_cast<unsigned char>('O'):
				case static_cast<unsigned char>('P'):
				case static_cast<unsigned char>('Q'):
				case static_cast<unsigned char>('R'):
				case static_cast<unsigned char>('S'):
				case static_cast<unsigned char>('T'):
				case static_cast<unsigned char>('U'):
				case static_cast<unsigned char>('V'):
				case static_cast<unsigned char>('W'):
				case static_cast<unsigned char>('X'):
				case static_cast<unsigned char>('Y'):
				case static_cast<unsigned char>('Z'):
				case static_cast<unsigned char>('_'):
				case static_cast<unsigned char>('a'):
				case static_cast<unsigned char>('b'):
				case static_cast<unsigned char>('c'):
				case static_cast<unsigned char>('d'):
				case static_cast<unsigned char>('e'):
				case static_cast<unsigned char>('f'):
				case static_cast<unsigned char>('g'):
				case static_cast<unsigned char>('h'):
				case static_cast<unsigned char>('i'):
				case static_cast<unsigned char>('j'):
				case static_cast<unsigned char>('k'):
				case static_cast<unsigned char>('l'):
				case static_cast<unsigned char>('m'):
				case static_cast<unsigned char>('n'):
				case static_cast<unsigned char>('o'):
				case static_cast<unsigned char>('p'):
				case static_cast<unsigned char>('q'):
				case static_cast<unsigned char>('r'):
				case static_cast<unsigned char>('s'):
				case static_cast<unsigned char>('t'):
				case static_cast<unsigned char>('u'):
				case static_cast<unsigned char>('v'):
				case static_cast<unsigned char>('w'):
				case static_cast<unsigned char>('x'):
				case static_cast<unsigned char>('y'):
				case static_cast<unsigned char>('z'):
				case static_cast<unsigned char>('\304'):
				case static_cast<unsigned char>('\305'):
				case static_cast<unsigned char>('\326'):
				case static_cast<unsigned char>('\344'):
				case static_cast<unsigned char>('\345'):
				case static_cast<unsigned char>('\366'):
				{
					mVALUE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('\''):
				{
					mCHAR_LITERAL(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('"'):
				{
					mSTRING_LITERAL(true);
					theRetToken=_returnToken;
					break;
				}
				default:
				{
					if (LA(1)==EOF_CHAR) {uponEOF(); _returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);}
				else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				}
				if ( !_returnToken ) goto tryAgain; // found SKIP token
				_ttype = _returnToken->getType();
				_ttype = testLiteralsTable(_ttype);
				_returnToken->setType(_ttype);
				return _returnToken;
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
				throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamIOException& csie) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamIOException(csie.io);
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamException& cse) {
		   throw ANTLR_USE_NAMESPACE(antlr)TokenStreamException(cse.getMessage());
		}
tryAgain:;
	}
}

void wb_wbllexer::mEQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = EQ;
	int _saveIndex;
	
	match(static_cast<unsigned char>('='));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mOREQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = OREQ;
	int _saveIndex;
	
	match("|=");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mWS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WS;
	int _saveIndex;
	
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>(' '):
	{
		match(static_cast<unsigned char>(' '));
		break;
	}
	case static_cast<unsigned char>('\t'):
	{
		match(static_cast<unsigned char>('\t'));
		break;
	}
	case static_cast<unsigned char>('\n'):
	{
		match(static_cast<unsigned char>('\n'));
		if ( inputState->guessing==0 ) {
#line 48 "wb_wbllexer.g"
			newline();
#line 253 "wb_wbllexer.cpp"
		}
		break;
	}
	case static_cast<unsigned char>('\r'):
	{
		match(static_cast<unsigned char>('\r'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	if ( inputState->guessing==0 ) {
#line 50 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP;
#line 271 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCOMMENT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = COMMENT;
	int _saveIndex;
	
	match("!");
	{
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			matchNot(static_cast<unsigned char>('\n'));
		}
		else {
			goto _loop7;
		}
		
	}
	_loop7:;
	}
	match(static_cast<unsigned char>('\n'));
	if ( inputState->guessing==0 ) {
#line 57 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP; newline();
#line 303 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mINDEX(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = INDEX;
	int _saveIndex;
	
	_saveIndex=text.length();
	match(static_cast<unsigned char>('['));
	text.erase(_saveIndex);
	{
	int _cnt10=0;
	for (;;) {
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
		}
		else {
			if ( _cnt10>=1 ) { goto _loop10; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt10++;
	}
	_loop10:;
	}
	_saveIndex=text.length();
	match(static_cast<unsigned char>(']'));
	text.erase(_saveIndex);
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mVALUE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = VALUE;
	int _saveIndex;
	ANTLR_USE_NAMESPACE(antlr)RefToken oname;
	ANTLR_USE_NAMESPACE(antlr)RefToken soname;
	ANTLR_USE_NAMESPACE(antlr)RefToken doname;
	ANTLR_USE_NAMESPACE(antlr)RefToken vname;
	ANTLR_USE_NAMESPACE(antlr)RefToken buname;
	ANTLR_USE_NAMESPACE(antlr)RefToken boname;
	ANTLR_USE_NAMESPACE(antlr)RefToken aname;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('-'):
	{
		match(static_cast<unsigned char>('-'));
		mDIGITS(false);
		match(static_cast<unsigned char>('.'));
		{
		for (;;) {
			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
			}
			else {
				goto _loop66;
			}
			
		}
		_loop66:;
		}
		{
		if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
			{
			switch ( LA(1)) {
			case static_cast<unsigned char>('e'):
			{
				match(static_cast<unsigned char>('e'));
				break;
			}
			case static_cast<unsigned char>('E'):
			{
				match(static_cast<unsigned char>('E'));
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
			}
			}
			}
			{
			switch ( LA(1)) {
			case static_cast<unsigned char>('+'):
			{
				match(static_cast<unsigned char>('+'));
				break;
			}
			case static_cast<unsigned char>('-'):
			{
				match(static_cast<unsigned char>('-'));
				break;
			}
			case static_cast<unsigned char>('0'):
			case static_cast<unsigned char>('1'):
			case static_cast<unsigned char>('2'):
			case static_cast<unsigned char>('3'):
			case static_cast<unsigned char>('4'):
			case static_cast<unsigned char>('5'):
			case static_cast<unsigned char>('6'):
			case static_cast<unsigned char>('7'):
			case static_cast<unsigned char>('8'):
			case static_cast<unsigned char>('9'):
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
			}
			}
			}
			{
			int _cnt71=0;
			for (;;) {
				if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
					matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
				}
				else {
					if ( _cnt71>=1 ) { goto _loop71; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				
				_cnt71++;
			}
			_loop71:;
			}
		}
		else {
		}
		
		}
		if ( inputState->guessing==0 ) {
#line 108 "wb_wbllexer.g"
			_ttype = NUM_FLOAT;
#line 449 "wb_wbllexer.cpp"
		}
		break;
	}
	case static_cast<unsigned char>('.'):
	{
		match(static_cast<unsigned char>('.'));
		{
		int _cnt73=0;
		for (;;) {
			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
			}
			else {
				if ( _cnt73>=1 ) { goto _loop73; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
			}
			
			_cnt73++;
		}
		_loop73:;
		}
		{
		if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
			{
			switch ( LA(1)) {
			case static_cast<unsigned char>('e'):
			{
				match(static_cast<unsigned char>('e'));
				break;
			}
			case static_cast<unsigned char>('E'):
			{
				match(static_cast<unsigned char>('E'));
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
			}
			}
			}
			{
			switch ( LA(1)) {
			case static_cast<unsigned char>('+'):
			{
				match(static_cast<unsigned char>('+'));
				break;
			}
			case static_cast<unsigned char>('-'):
			{
				match(static_cast<unsigned char>('-'));
				break;
			}
			case static_cast<unsigned char>('0'):
			case static_cast<unsigned char>('1'):
			case static_cast<unsigned char>('2'):
			case static_cast<unsigned char>('3'):
			case static_cast<unsigned char>('4'):
			case static_cast<unsigned char>('5'):
			case static_cast<unsigned char>('6'):
			case static_cast<unsigned char>('7'):
			case static_cast<unsigned char>('8'):
			case static_cast<unsigned char>('9'):
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
			}
			}
			}
			{
			int _cnt78=0;
			for (;;) {
				if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
					matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
				}
				else {
					if ( _cnt78>=1 ) { goto _loop78; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				
				_cnt78++;
			}
			_loop78:;
			}
		}
		else {
		}
		
		}
		if ( inputState->guessing==0 ) {
#line 110 "wb_wbllexer.g"
			_ttype = NUM_FLOAT;
#line 543 "wb_wbllexer.cpp"
		}
		break;
	}
	default:
		bool synPredMatched13 = false;
		if (((LA(1)==static_cast<unsigned char>('O')) && (LA(2)==static_cast<unsigned char>('b')))) {
			int _m13 = mark();
			synPredMatched13 = true;
			inputState->guessing++;
			try {
				{
				match("Object");
				mWS(false);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched13 = false;
			}
			rewind(_m13);
			inputState->guessing--;
		}
		if ( synPredMatched13 ) {
			_saveIndex=text.length();
			match("Object");
			text.erase(_saveIndex);
			_saveIndex = text.length();
			mWS(false);
			text.erase(_saveIndex);
			{
			int _cnt_oname=0;
			for (;;) {
				switch ( LA(1)) {
				case static_cast<unsigned char>('a'):
				case static_cast<unsigned char>('b'):
				case static_cast<unsigned char>('c'):
				case static_cast<unsigned char>('d'):
				case static_cast<unsigned char>('e'):
				case static_cast<unsigned char>('f'):
				case static_cast<unsigned char>('g'):
				case static_cast<unsigned char>('h'):
				case static_cast<unsigned char>('i'):
				case static_cast<unsigned char>('j'):
				case static_cast<unsigned char>('k'):
				case static_cast<unsigned char>('l'):
				case static_cast<unsigned char>('m'):
				case static_cast<unsigned char>('n'):
				case static_cast<unsigned char>('o'):
				case static_cast<unsigned char>('p'):
				case static_cast<unsigned char>('q'):
				case static_cast<unsigned char>('r'):
				case static_cast<unsigned char>('s'):
				case static_cast<unsigned char>('t'):
				case static_cast<unsigned char>('u'):
				case static_cast<unsigned char>('v'):
				case static_cast<unsigned char>('w'):
				case static_cast<unsigned char>('x'):
				case static_cast<unsigned char>('y'):
				case static_cast<unsigned char>('z'):
				{
					matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
					break;
				}
				case static_cast<unsigned char>('A'):
				case static_cast<unsigned char>('B'):
				case static_cast<unsigned char>('C'):
				case static_cast<unsigned char>('D'):
				case static_cast<unsigned char>('E'):
				case static_cast<unsigned char>('F'):
				case static_cast<unsigned char>('G'):
				case static_cast<unsigned char>('H'):
				case static_cast<unsigned char>('I'):
				case static_cast<unsigned char>('J'):
				case static_cast<unsigned char>('K'):
				case static_cast<unsigned char>('L'):
				case static_cast<unsigned char>('M'):
				case static_cast<unsigned char>('N'):
				case static_cast<unsigned char>('O'):
				case static_cast<unsigned char>('P'):
				case static_cast<unsigned char>('Q'):
				case static_cast<unsigned char>('R'):
				case static_cast<unsigned char>('S'):
				case static_cast<unsigned char>('T'):
				case static_cast<unsigned char>('U'):
				case static_cast<unsigned char>('V'):
				case static_cast<unsigned char>('W'):
				case static_cast<unsigned char>('X'):
				case static_cast<unsigned char>('Y'):
				case static_cast<unsigned char>('Z'):
				{
					matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
					break;
				}
				case static_cast<unsigned char>('0'):
				case static_cast<unsigned char>('1'):
				case static_cast<unsigned char>('2'):
				case static_cast<unsigned char>('3'):
				case static_cast<unsigned char>('4'):
				case static_cast<unsigned char>('5'):
				case static_cast<unsigned char>('6'):
				case static_cast<unsigned char>('7'):
				case static_cast<unsigned char>('8'):
				case static_cast<unsigned char>('9'):
				{
					matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
					break;
				}
				case static_cast<unsigned char>('$'):
				{
					match(static_cast<unsigned char>('$'));
					break;
				}
				case static_cast<unsigned char>('_'):
				{
					match(static_cast<unsigned char>('_'));
					break;
				}
				case static_cast<unsigned char>('\304'):
				case static_cast<unsigned char>('\305'):
				case static_cast<unsigned char>('\326'):
				case static_cast<unsigned char>('\344'):
				case static_cast<unsigned char>('\345'):
				case static_cast<unsigned char>('\366'):
				{
					mSWEC(false);
					break;
				}
				default:
				{
					if ( _cnt_oname>=1 ) { goto oname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				}
				_cnt_oname++;
			}
			oname:;
			}
			if ( inputState->guessing==0 ) {
#line 78 "wb_wbllexer.g"
				_ttype = OBJECT;
#line 682 "wb_wbllexer.cpp"
			}
		}
		else {
			bool synPredMatched17 = false;
			if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
				int _m17 = mark();
				synPredMatched17 = true;
				inputState->guessing++;
				try {
					{
					match("EndObject");
					mWS(false);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched17 = false;
				}
				rewind(_m17);
				inputState->guessing--;
			}
			if ( synPredMatched17 ) {
				match("EndObject");
				_saveIndex = text.length();
				mWS(false);
				text.erase(_saveIndex);
				if ( inputState->guessing==0 ) {
#line 80 "wb_wbllexer.g"
					_ttype = ENDOBJECT;
#line 711 "wb_wbllexer.cpp"
				}
			}
			else {
				bool synPredMatched19 = false;
				if (((LA(1)==static_cast<unsigned char>('S')) && (LA(2)==static_cast<unsigned char>('O')))) {
					int _m19 = mark();
					synPredMatched19 = true;
					inputState->guessing++;
					try {
						{
						match("SObject");
						mWS(false);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched19 = false;
					}
					rewind(_m19);
					inputState->guessing--;
				}
				if ( synPredMatched19 ) {
					_saveIndex=text.length();
					match("SObject");
					text.erase(_saveIndex);
					_saveIndex = text.length();
					mWS(false);
					text.erase(_saveIndex);
					{
					int _cnt_soname=0;
					for (;;) {
						switch ( LA(1)) {
						case static_cast<unsigned char>('a'):
						case static_cast<unsigned char>('b'):
						case static_cast<unsigned char>('c'):
						case static_cast<unsigned char>('d'):
						case static_cast<unsigned char>('e'):
						case static_cast<unsigned char>('f'):
						case static_cast<unsigned char>('g'):
						case static_cast<unsigned char>('h'):
						case static_cast<unsigned char>('i'):
						case static_cast<unsigned char>('j'):
						case static_cast<unsigned char>('k'):
						case static_cast<unsigned char>('l'):
						case static_cast<unsigned char>('m'):
						case static_cast<unsigned char>('n'):
						case static_cast<unsigned char>('o'):
						case static_cast<unsigned char>('p'):
						case static_cast<unsigned char>('q'):
						case static_cast<unsigned char>('r'):
						case static_cast<unsigned char>('s'):
						case static_cast<unsigned char>('t'):
						case static_cast<unsigned char>('u'):
						case static_cast<unsigned char>('v'):
						case static_cast<unsigned char>('w'):
						case static_cast<unsigned char>('x'):
						case static_cast<unsigned char>('y'):
						case static_cast<unsigned char>('z'):
						{
							matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
							break;
						}
						case static_cast<unsigned char>('A'):
						case static_cast<unsigned char>('B'):
						case static_cast<unsigned char>('C'):
						case static_cast<unsigned char>('D'):
						case static_cast<unsigned char>('E'):
						case static_cast<unsigned char>('F'):
						case static_cast<unsigned char>('G'):
						case static_cast<unsigned char>('H'):
						case static_cast<unsigned char>('I'):
						case static_cast<unsigned char>('J'):
						case static_cast<unsigned char>('K'):
						case static_cast<unsigned char>('L'):
						case static_cast<unsigned char>('M'):
						case static_cast<unsigned char>('N'):
						case static_cast<unsigned char>('O'):
						case static_cast<unsigned char>('P'):
						case static_cast<unsigned char>('Q'):
						case static_cast<unsigned char>('R'):
						case static_cast<unsigned char>('S'):
						case static_cast<unsigned char>('T'):
						case static_cast<unsigned char>('U'):
						case static_cast<unsigned char>('V'):
						case static_cast<unsigned char>('W'):
						case static_cast<unsigned char>('X'):
						case static_cast<unsigned char>('Y'):
						case static_cast<unsigned char>('Z'):
						{
							matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
							break;
						}
						case static_cast<unsigned char>('0'):
						case static_cast<unsigned char>('1'):
						case static_cast<unsigned char>('2'):
						case static_cast<unsigned char>('3'):
						case static_cast<unsigned char>('4'):
						case static_cast<unsigned char>('5'):
						case static_cast<unsigned char>('6'):
						case static_cast<unsigned char>('7'):
						case static_cast<unsigned char>('8'):
						case static_cast<unsigned char>('9'):
						{
							matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
							break;
						}
						case static_cast<unsigned char>('$'):
						{
							match(static_cast<unsigned char>('$'));
							break;
						}
						case static_cast<unsigned char>('_'):
						{
							match(static_cast<unsigned char>('_'));
							break;
						}
						case static_cast<unsigned char>(':'):
						{
							match(static_cast<unsigned char>(':'));
							break;
						}
						case static_cast<unsigned char>('\304'):
						case static_cast<unsigned char>('\305'):
						case static_cast<unsigned char>('\326'):
						case static_cast<unsigned char>('\344'):
						case static_cast<unsigned char>('\345'):
						case static_cast<unsigned char>('\366'):
						{
							mSWEC(false);
							break;
						}
						default:
						{
							if ( _cnt_soname>=1 ) { goto soname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
						}
						}
						_cnt_soname++;
					}
					soname:;
					}
					if ( inputState->guessing==0 ) {
#line 82 "wb_wbllexer.g"
						_ttype = SOBJECT;
#line 854 "wb_wbllexer.cpp"
					}
				}
				else {
					bool synPredMatched23 = false;
					if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
						int _m23 = mark();
						synPredMatched23 = true;
						inputState->guessing++;
						try {
							{
							match("EndSObject");
							mWS(false);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched23 = false;
						}
						rewind(_m23);
						inputState->guessing--;
					}
					if ( synPredMatched23 ) {
						match("EndSObject");
						_saveIndex = text.length();
						mWS(false);
						text.erase(_saveIndex);
						if ( inputState->guessing==0 ) {
#line 84 "wb_wbllexer.g"
							_ttype = ENDSOBJECT;
#line 883 "wb_wbllexer.cpp"
						}
					}
					else {
						bool synPredMatched25 = false;
						if (((LA(1)==static_cast<unsigned char>('D')) && (LA(2)==static_cast<unsigned char>('O')))) {
							int _m25 = mark();
							synPredMatched25 = true;
							inputState->guessing++;
							try {
								{
								match("DObject");
								mWS(false);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched25 = false;
							}
							rewind(_m25);
							inputState->guessing--;
						}
						if ( synPredMatched25 ) {
							_saveIndex=text.length();
							match("DObject");
							text.erase(_saveIndex);
							_saveIndex = text.length();
							mWS(false);
							text.erase(_saveIndex);
							{
							int _cnt_doname=0;
							for (;;) {
								switch ( LA(1)) {
								case static_cast<unsigned char>('a'):
								case static_cast<unsigned char>('b'):
								case static_cast<unsigned char>('c'):
								case static_cast<unsigned char>('d'):
								case static_cast<unsigned char>('e'):
								case static_cast<unsigned char>('f'):
								case static_cast<unsigned char>('g'):
								case static_cast<unsigned char>('h'):
								case static_cast<unsigned char>('i'):
								case static_cast<unsigned char>('j'):
								case static_cast<unsigned char>('k'):
								case static_cast<unsigned char>('l'):
								case static_cast<unsigned char>('m'):
								case static_cast<unsigned char>('n'):
								case static_cast<unsigned char>('o'):
								case static_cast<unsigned char>('p'):
								case static_cast<unsigned char>('q'):
								case static_cast<unsigned char>('r'):
								case static_cast<unsigned char>('s'):
								case static_cast<unsigned char>('t'):
								case static_cast<unsigned char>('u'):
								case static_cast<unsigned char>('v'):
								case static_cast<unsigned char>('w'):
								case static_cast<unsigned char>('x'):
								case static_cast<unsigned char>('y'):
								case static_cast<unsigned char>('z'):
								{
									matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
									break;
								}
								case static_cast<unsigned char>('A'):
								case static_cast<unsigned char>('B'):
								case static_cast<unsigned char>('C'):
								case static_cast<unsigned char>('D'):
								case static_cast<unsigned char>('E'):
								case static_cast<unsigned char>('F'):
								case static_cast<unsigned char>('G'):
								case static_cast<unsigned char>('H'):
								case static_cast<unsigned char>('I'):
								case static_cast<unsigned char>('J'):
								case static_cast<unsigned char>('K'):
								case static_cast<unsigned char>('L'):
								case static_cast<unsigned char>('M'):
								case static_cast<unsigned char>('N'):
								case static_cast<unsigned char>('O'):
								case static_cast<unsigned char>('P'):
								case static_cast<unsigned char>('Q'):
								case static_cast<unsigned char>('R'):
								case static_cast<unsigned char>('S'):
								case static_cast<unsigned char>('T'):
								case static_cast<unsigned char>('U'):
								case static_cast<unsigned char>('V'):
								case static_cast<unsigned char>('W'):
								case static_cast<unsigned char>('X'):
								case static_cast<unsigned char>('Y'):
								case static_cast<unsigned char>('Z'):
								{
									matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
									break;
								}
								case static_cast<unsigned char>('0'):
								case static_cast<unsigned char>('1'):
								case static_cast<unsigned char>('2'):
								case static_cast<unsigned char>('3'):
								case static_cast<unsigned char>('4'):
								case static_cast<unsigned char>('5'):
								case static_cast<unsigned char>('6'):
								case static_cast<unsigned char>('7'):
								case static_cast<unsigned char>('8'):
								case static_cast<unsigned char>('9'):
								{
									matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
									break;
								}
								case static_cast<unsigned char>('$'):
								{
									match(static_cast<unsigned char>('$'));
									break;
								}
								case static_cast<unsigned char>('_'):
								{
									match(static_cast<unsigned char>('_'));
									break;
								}
								case static_cast<unsigned char>('\304'):
								case static_cast<unsigned char>('\305'):
								case static_cast<unsigned char>('\326'):
								case static_cast<unsigned char>('\344'):
								case static_cast<unsigned char>('\345'):
								case static_cast<unsigned char>('\366'):
								{
									mSWEC(false);
									break;
								}
								default:
								{
									if ( _cnt_doname>=1 ) { goto doname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
								}
								}
								_cnt_doname++;
							}
							doname:;
							}
							if ( inputState->guessing==0 ) {
#line 86 "wb_wbllexer.g"
								_ttype = DOBJECT;
#line 1021 "wb_wbllexer.cpp"
							}
						}
						else {
							bool synPredMatched29 = false;
							if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
								int _m29 = mark();
								synPredMatched29 = true;
								inputState->guessing++;
								try {
									{
									match("EndDObject");
									mWS(false);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched29 = false;
								}
								rewind(_m29);
								inputState->guessing--;
							}
							if ( synPredMatched29 ) {
								match("EndDObject");
								_saveIndex = text.length();
								mWS(false);
								text.erase(_saveIndex);
								if ( inputState->guessing==0 ) {
#line 88 "wb_wbllexer.g"
									_ttype = ENDDOBJECT;
#line 1050 "wb_wbllexer.cpp"
								}
							}
							else {
								bool synPredMatched31 = false;
								if (((LA(1)==static_cast<unsigned char>('V')) && (LA(2)==static_cast<unsigned char>('o')))) {
									int _m31 = mark();
									synPredMatched31 = true;
									inputState->guessing++;
									try {
										{
										match("Volume");
										mWS(false);
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched31 = false;
									}
									rewind(_m31);
									inputState->guessing--;
								}
								if ( synPredMatched31 ) {
									_saveIndex=text.length();
									match("Volume");
									text.erase(_saveIndex);
									_saveIndex = text.length();
									mWS(false);
									text.erase(_saveIndex);
									{
									int _cnt_vname=0;
									for (;;) {
										switch ( LA(1)) {
										case static_cast<unsigned char>('a'):
										case static_cast<unsigned char>('b'):
										case static_cast<unsigned char>('c'):
										case static_cast<unsigned char>('d'):
										case static_cast<unsigned char>('e'):
										case static_cast<unsigned char>('f'):
										case static_cast<unsigned char>('g'):
										case static_cast<unsigned char>('h'):
										case static_cast<unsigned char>('i'):
										case static_cast<unsigned char>('j'):
										case static_cast<unsigned char>('k'):
										case static_cast<unsigned char>('l'):
										case static_cast<unsigned char>('m'):
										case static_cast<unsigned char>('n'):
										case static_cast<unsigned char>('o'):
										case static_cast<unsigned char>('p'):
										case static_cast<unsigned char>('q'):
										case static_cast<unsigned char>('r'):
										case static_cast<unsigned char>('s'):
										case static_cast<unsigned char>('t'):
										case static_cast<unsigned char>('u'):
										case static_cast<unsigned char>('v'):
										case static_cast<unsigned char>('w'):
										case static_cast<unsigned char>('x'):
										case static_cast<unsigned char>('y'):
										case static_cast<unsigned char>('z'):
										{
											matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
											break;
										}
										case static_cast<unsigned char>('A'):
										case static_cast<unsigned char>('B'):
										case static_cast<unsigned char>('C'):
										case static_cast<unsigned char>('D'):
										case static_cast<unsigned char>('E'):
										case static_cast<unsigned char>('F'):
										case static_cast<unsigned char>('G'):
										case static_cast<unsigned char>('H'):
										case static_cast<unsigned char>('I'):
										case static_cast<unsigned char>('J'):
										case static_cast<unsigned char>('K'):
										case static_cast<unsigned char>('L'):
										case static_cast<unsigned char>('M'):
										case static_cast<unsigned char>('N'):
										case static_cast<unsigned char>('O'):
										case static_cast<unsigned char>('P'):
										case static_cast<unsigned char>('Q'):
										case static_cast<unsigned char>('R'):
										case static_cast<unsigned char>('S'):
										case static_cast<unsigned char>('T'):
										case static_cast<unsigned char>('U'):
										case static_cast<unsigned char>('V'):
										case static_cast<unsigned char>('W'):
										case static_cast<unsigned char>('X'):
										case static_cast<unsigned char>('Y'):
										case static_cast<unsigned char>('Z'):
										{
											matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
											break;
										}
										case static_cast<unsigned char>('0'):
										case static_cast<unsigned char>('1'):
										case static_cast<unsigned char>('2'):
										case static_cast<unsigned char>('3'):
										case static_cast<unsigned char>('4'):
										case static_cast<unsigned char>('5'):
										case static_cast<unsigned char>('6'):
										case static_cast<unsigned char>('7'):
										case static_cast<unsigned char>('8'):
										case static_cast<unsigned char>('9'):
										{
											matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
											break;
										}
										case static_cast<unsigned char>('$'):
										{
											match(static_cast<unsigned char>('$'));
											break;
										}
										case static_cast<unsigned char>('_'):
										{
											match(static_cast<unsigned char>('_'));
											break;
										}
										case static_cast<unsigned char>('\304'):
										case static_cast<unsigned char>('\305'):
										case static_cast<unsigned char>('\326'):
										case static_cast<unsigned char>('\344'):
										case static_cast<unsigned char>('\345'):
										case static_cast<unsigned char>('\366'):
										{
											mSWEC(false);
											break;
										}
										default:
										{
											if ( _cnt_vname>=1 ) { goto vname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
										}
										}
										_cnt_vname++;
									}
									vname:;
									}
									{
									if ((LA(1)==static_cast<unsigned char>(':'))) {
										match(static_cast<unsigned char>(':'));
									}
									else {
									}
									
									}
									if ( inputState->guessing==0 ) {
#line 90 "wb_wbllexer.g"
										_ttype = VOLUME;
#line 1196 "wb_wbllexer.cpp"
									}
								}
								else {
									bool synPredMatched36 = false;
									if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
										int _m36 = mark();
										synPredMatched36 = true;
										inputState->guessing++;
										try {
											{
											match("EndVolume");
											mWS(false);
											}
										}
										catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
											synPredMatched36 = false;
										}
										rewind(_m36);
										inputState->guessing--;
									}
									if ( synPredMatched36 ) {
										match("EndVolume");
										_saveIndex = text.length();
										mWS(false);
										text.erase(_saveIndex);
										if ( inputState->guessing==0 ) {
#line 92 "wb_wbllexer.g"
											_ttype = ENDVOLUME;
#line 1225 "wb_wbllexer.cpp"
										}
									}
									else {
										bool synPredMatched38 = false;
										if (((LA(1)==static_cast<unsigned char>('B')) && (LA(2)==static_cast<unsigned char>('u')))) {
											int _m38 = mark();
											synPredMatched38 = true;
											inputState->guessing++;
											try {
												{
												match("Buffer");
												mWS(false);
												}
											}
											catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
												synPredMatched38 = false;
											}
											rewind(_m38);
											inputState->guessing--;
										}
										if ( synPredMatched38 ) {
											_saveIndex=text.length();
											match("Buffer");
											text.erase(_saveIndex);
											_saveIndex = text.length();
											mWS(false);
											text.erase(_saveIndex);
											{
											int _cnt_buname=0;
											for (;;) {
												switch ( LA(1)) {
												case static_cast<unsigned char>('a'):
												case static_cast<unsigned char>('b'):
												case static_cast<unsigned char>('c'):
												case static_cast<unsigned char>('d'):
												case static_cast<unsigned char>('e'):
												case static_cast<unsigned char>('f'):
												case static_cast<unsigned char>('g'):
												case static_cast<unsigned char>('h'):
												case static_cast<unsigned char>('i'):
												case static_cast<unsigned char>('j'):
												case static_cast<unsigned char>('k'):
												case static_cast<unsigned char>('l'):
												case static_cast<unsigned char>('m'):
												case static_cast<unsigned char>('n'):
												case static_cast<unsigned char>('o'):
												case static_cast<unsigned char>('p'):
												case static_cast<unsigned char>('q'):
												case static_cast<unsigned char>('r'):
												case static_cast<unsigned char>('s'):
												case static_cast<unsigned char>('t'):
												case static_cast<unsigned char>('u'):
												case static_cast<unsigned char>('v'):
												case static_cast<unsigned char>('w'):
												case static_cast<unsigned char>('x'):
												case static_cast<unsigned char>('y'):
												case static_cast<unsigned char>('z'):
												{
													matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
													break;
												}
												case static_cast<unsigned char>('A'):
												case static_cast<unsigned char>('B'):
												case static_cast<unsigned char>('C'):
												case static_cast<unsigned char>('D'):
												case static_cast<unsigned char>('E'):
												case static_cast<unsigned char>('F'):
												case static_cast<unsigned char>('G'):
												case static_cast<unsigned char>('H'):
												case static_cast<unsigned char>('I'):
												case static_cast<unsigned char>('J'):
												case static_cast<unsigned char>('K'):
												case static_cast<unsigned char>('L'):
												case static_cast<unsigned char>('M'):
												case static_cast<unsigned char>('N'):
												case static_cast<unsigned char>('O'):
												case static_cast<unsigned char>('P'):
												case static_cast<unsigned char>('Q'):
												case static_cast<unsigned char>('R'):
												case static_cast<unsigned char>('S'):
												case static_cast<unsigned char>('T'):
												case static_cast<unsigned char>('U'):
												case static_cast<unsigned char>('V'):
												case static_cast<unsigned char>('W'):
												case static_cast<unsigned char>('X'):
												case static_cast<unsigned char>('Y'):
												case static_cast<unsigned char>('Z'):
												{
													matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
													break;
												}
												case static_cast<unsigned char>('0'):
												case static_cast<unsigned char>('1'):
												case static_cast<unsigned char>('2'):
												case static_cast<unsigned char>('3'):
												case static_cast<unsigned char>('4'):
												case static_cast<unsigned char>('5'):
												case static_cast<unsigned char>('6'):
												case static_cast<unsigned char>('7'):
												case static_cast<unsigned char>('8'):
												case static_cast<unsigned char>('9'):
												{
													matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
													break;
												}
												case static_cast<unsigned char>('$'):
												{
													match(static_cast<unsigned char>('$'));
													break;
												}
												case static_cast<unsigned char>('_'):
												{
													match(static_cast<unsigned char>('_'));
													break;
												}
												case static_cast<unsigned char>('\304'):
												case static_cast<unsigned char>('\305'):
												case static_cast<unsigned char>('\326'):
												case static_cast<unsigned char>('\344'):
												case static_cast<unsigned char>('\345'):
												case static_cast<unsigned char>('\366'):
												{
													mSWEC(false);
													break;
												}
												default:
												{
													if ( _cnt_buname>=1 ) { goto buname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
												}
												}
												_cnt_buname++;
											}
											buname:;
											}
											if ( inputState->guessing==0 ) {
#line 94 "wb_wbllexer.g"
												_ttype = BUFFER;
#line 1363 "wb_wbllexer.cpp"
											}
										}
										else {
											bool synPredMatched42 = false;
											if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
												int _m42 = mark();
												synPredMatched42 = true;
												inputState->guessing++;
												try {
													{
													match("EndBuffer");
													mWS(false);
													}
												}
												catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
													synPredMatched42 = false;
												}
												rewind(_m42);
												inputState->guessing--;
											}
											if ( synPredMatched42 ) {
												match("EndBuffer");
												_saveIndex = text.length();
												mWS(false);
												text.erase(_saveIndex);
												if ( inputState->guessing==0 ) {
#line 96 "wb_wbllexer.g"
													_ttype = ENDBUFFER;
#line 1392 "wb_wbllexer.cpp"
												}
											}
											else {
												bool synPredMatched44 = false;
												if (((LA(1)==static_cast<unsigned char>('B')) && (LA(2)==static_cast<unsigned char>('o')))) {
													int _m44 = mark();
													synPredMatched44 = true;
													inputState->guessing++;
													try {
														{
														match("Body");
														mWS(false);
														}
													}
													catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
														synPredMatched44 = false;
													}
													rewind(_m44);
													inputState->guessing--;
												}
												if ( synPredMatched44 ) {
													_saveIndex=text.length();
													match("Body");
													text.erase(_saveIndex);
													_saveIndex = text.length();
													mWS(false);
													text.erase(_saveIndex);
													{
													int _cnt_boname=0;
													for (;;) {
														switch ( LA(1)) {
														case static_cast<unsigned char>('a'):
														case static_cast<unsigned char>('b'):
														case static_cast<unsigned char>('c'):
														case static_cast<unsigned char>('d'):
														case static_cast<unsigned char>('e'):
														case static_cast<unsigned char>('f'):
														case static_cast<unsigned char>('g'):
														case static_cast<unsigned char>('h'):
														case static_cast<unsigned char>('i'):
														case static_cast<unsigned char>('j'):
														case static_cast<unsigned char>('k'):
														case static_cast<unsigned char>('l'):
														case static_cast<unsigned char>('m'):
														case static_cast<unsigned char>('n'):
														case static_cast<unsigned char>('o'):
														case static_cast<unsigned char>('p'):
														case static_cast<unsigned char>('q'):
														case static_cast<unsigned char>('r'):
														case static_cast<unsigned char>('s'):
														case static_cast<unsigned char>('t'):
														case static_cast<unsigned char>('u'):
														case static_cast<unsigned char>('v'):
														case static_cast<unsigned char>('w'):
														case static_cast<unsigned char>('x'):
														case static_cast<unsigned char>('y'):
														case static_cast<unsigned char>('z'):
														{
															matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
															break;
														}
														case static_cast<unsigned char>('A'):
														case static_cast<unsigned char>('B'):
														case static_cast<unsigned char>('C'):
														case static_cast<unsigned char>('D'):
														case static_cast<unsigned char>('E'):
														case static_cast<unsigned char>('F'):
														case static_cast<unsigned char>('G'):
														case static_cast<unsigned char>('H'):
														case static_cast<unsigned char>('I'):
														case static_cast<unsigned char>('J'):
														case static_cast<unsigned char>('K'):
														case static_cast<unsigned char>('L'):
														case static_cast<unsigned char>('M'):
														case static_cast<unsigned char>('N'):
														case static_cast<unsigned char>('O'):
														case static_cast<unsigned char>('P'):
														case static_cast<unsigned char>('Q'):
														case static_cast<unsigned char>('R'):
														case static_cast<unsigned char>('S'):
														case static_cast<unsigned char>('T'):
														case static_cast<unsigned char>('U'):
														case static_cast<unsigned char>('V'):
														case static_cast<unsigned char>('W'):
														case static_cast<unsigned char>('X'):
														case static_cast<unsigned char>('Y'):
														case static_cast<unsigned char>('Z'):
														{
															matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
															break;
														}
														case static_cast<unsigned char>('0'):
														case static_cast<unsigned char>('1'):
														case static_cast<unsigned char>('2'):
														case static_cast<unsigned char>('3'):
														case static_cast<unsigned char>('4'):
														case static_cast<unsigned char>('5'):
														case static_cast<unsigned char>('6'):
														case static_cast<unsigned char>('7'):
														case static_cast<unsigned char>('8'):
														case static_cast<unsigned char>('9'):
														{
															matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
															break;
														}
														case static_cast<unsigned char>('$'):
														{
															match(static_cast<unsigned char>('$'));
															break;
														}
														case static_cast<unsigned char>('_'):
														{
															match(static_cast<unsigned char>('_'));
															break;
														}
														case static_cast<unsigned char>('\304'):
														case static_cast<unsigned char>('\305'):
														case static_cast<unsigned char>('\326'):
														case static_cast<unsigned char>('\344'):
														case static_cast<unsigned char>('\345'):
														case static_cast<unsigned char>('\366'):
														{
															mSWEC(false);
															break;
														}
														default:
														{
															if ( _cnt_boname>=1 ) { goto boname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
														}
														}
														_cnt_boname++;
													}
													boname:;
													}
													if ( inputState->guessing==0 ) {
#line 98 "wb_wbllexer.g"
														_ttype = BODY;
#line 1530 "wb_wbllexer.cpp"
													}
												}
												else {
													bool synPredMatched48 = false;
													if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
														int _m48 = mark();
														synPredMatched48 = true;
														inputState->guessing++;
														try {
															{
															match("EndBody");
															mWS(false);
															}
														}
														catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
															synPredMatched48 = false;
														}
														rewind(_m48);
														inputState->guessing--;
													}
													if ( synPredMatched48 ) {
														match("EndBody");
														_saveIndex = text.length();
														mWS(false);
														text.erase(_saveIndex);
														if ( inputState->guessing==0 ) {
#line 100 "wb_wbllexer.g"
															_ttype = ENDBODY;
#line 1559 "wb_wbllexer.cpp"
														}
													}
													else {
														bool synPredMatched50 = false;
														if (((LA(1)==static_cast<unsigned char>('A')) && (LA(2)==static_cast<unsigned char>('t')))) {
															int _m50 = mark();
															synPredMatched50 = true;
															inputState->guessing++;
															try {
																{
																match("Attr");
																mWS(false);
																}
															}
															catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																synPredMatched50 = false;
															}
															rewind(_m50);
															inputState->guessing--;
														}
														if ( synPredMatched50 ) {
															_saveIndex=text.length();
															match("Attr");
															text.erase(_saveIndex);
															_saveIndex = text.length();
															mWS(false);
															text.erase(_saveIndex);
															{
															int _cnt_aname=0;
															for (;;) {
																switch ( LA(1)) {
																case static_cast<unsigned char>('a'):
																case static_cast<unsigned char>('b'):
																case static_cast<unsigned char>('c'):
																case static_cast<unsigned char>('d'):
																case static_cast<unsigned char>('e'):
																case static_cast<unsigned char>('f'):
																case static_cast<unsigned char>('g'):
																case static_cast<unsigned char>('h'):
																case static_cast<unsigned char>('i'):
																case static_cast<unsigned char>('j'):
																case static_cast<unsigned char>('k'):
																case static_cast<unsigned char>('l'):
																case static_cast<unsigned char>('m'):
																case static_cast<unsigned char>('n'):
																case static_cast<unsigned char>('o'):
																case static_cast<unsigned char>('p'):
																case static_cast<unsigned char>('q'):
																case static_cast<unsigned char>('r'):
																case static_cast<unsigned char>('s'):
																case static_cast<unsigned char>('t'):
																case static_cast<unsigned char>('u'):
																case static_cast<unsigned char>('v'):
																case static_cast<unsigned char>('w'):
																case static_cast<unsigned char>('x'):
																case static_cast<unsigned char>('y'):
																case static_cast<unsigned char>('z'):
																{
																	matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																	break;
																}
																case static_cast<unsigned char>('A'):
																case static_cast<unsigned char>('B'):
																case static_cast<unsigned char>('C'):
																case static_cast<unsigned char>('D'):
																case static_cast<unsigned char>('E'):
																case static_cast<unsigned char>('F'):
																case static_cast<unsigned char>('G'):
																case static_cast<unsigned char>('H'):
																case static_cast<unsigned char>('I'):
																case static_cast<unsigned char>('J'):
																case static_cast<unsigned char>('K'):
																case static_cast<unsigned char>('L'):
																case static_cast<unsigned char>('M'):
																case static_cast<unsigned char>('N'):
																case static_cast<unsigned char>('O'):
																case static_cast<unsigned char>('P'):
																case static_cast<unsigned char>('Q'):
																case static_cast<unsigned char>('R'):
																case static_cast<unsigned char>('S'):
																case static_cast<unsigned char>('T'):
																case static_cast<unsigned char>('U'):
																case static_cast<unsigned char>('V'):
																case static_cast<unsigned char>('W'):
																case static_cast<unsigned char>('X'):
																case static_cast<unsigned char>('Y'):
																case static_cast<unsigned char>('Z'):
																{
																	matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																	break;
																}
																case static_cast<unsigned char>('0'):
																case static_cast<unsigned char>('1'):
																case static_cast<unsigned char>('2'):
																case static_cast<unsigned char>('3'):
																case static_cast<unsigned char>('4'):
																case static_cast<unsigned char>('5'):
																case static_cast<unsigned char>('6'):
																case static_cast<unsigned char>('7'):
																case static_cast<unsigned char>('8'):
																case static_cast<unsigned char>('9'):
																{
																	matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																	break;
																}
																case static_cast<unsigned char>('$'):
																{
																	match(static_cast<unsigned char>('$'));
																	break;
																}
																case static_cast<unsigned char>('_'):
																{
																	match(static_cast<unsigned char>('_'));
																	break;
																}
																case static_cast<unsigned char>('\304'):
																case static_cast<unsigned char>('\305'):
																case static_cast<unsigned char>('\326'):
																case static_cast<unsigned char>('\344'):
																case static_cast<unsigned char>('\345'):
																case static_cast<unsigned char>('\366'):
																{
																	mSWEC(false);
																	break;
																}
																case static_cast<unsigned char>('['):
																{
																	match(static_cast<unsigned char>('['));
																	break;
																}
																case static_cast<unsigned char>(']'):
																{
																	match(static_cast<unsigned char>(']'));
																	break;
																}
																default:
																{
																	if ( _cnt_aname>=1 ) { goto aname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																}
																}
																_cnt_aname++;
															}
															aname:;
															}
															if ( inputState->guessing==0 ) {
#line 102 "wb_wbllexer.g"
																_ttype = ATTRIBUTE;
#line 1707 "wb_wbllexer.cpp"
															}
														}
														else {
															bool synPredMatched54 = false;
															if ((((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (_tokenSet_1.member(LA(2))))) {
																int _m54 = mark();
																synPredMatched54 = true;
																inputState->guessing++;
																try {
																	{
																	mDIGITS(false);
																	match(static_cast<unsigned char>('.'));
																	mDIGITS(false);
																	match(static_cast<unsigned char>('.'));
																	}
																}
																catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																	synPredMatched54 = false;
																}
																rewind(_m54);
																inputState->guessing--;
															}
															if ( synPredMatched54 ) {
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
																mDIGITS(false);
																{
																if ((LA(1)==static_cast<unsigned char>(':'))) {
																	match(static_cast<unsigned char>(':'));
																	mDIGITS(false);
																}
																else {
																}
																
																}
																if ( inputState->guessing==0 ) {
#line 104 "wb_wbllexer.g"
																	_ttype = OID;
#line 1750 "wb_wbllexer.cpp"
																}
															}
															else {
																bool synPredMatched57 = false;
																if ((((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (_tokenSet_1.member(LA(2))))) {
																	int _m57 = mark();
																	synPredMatched57 = true;
																	inputState->guessing++;
																	try {
																		{
																		mDIGITS(false);
																		match(static_cast<unsigned char>('.'));
																		}
																	}
																	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																		synPredMatched57 = false;
																	}
																	rewind(_m57);
																	inputState->guessing--;
																}
																if ( synPredMatched57 ) {
																	mDIGITS(false);
																	match(static_cast<unsigned char>('.'));
																	{
																	for (;;) {
																		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																		}
																		else {
																			goto _loop59;
																		}
																		
																	}
																	_loop59:;
																	}
																	{
																	if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('e'):
																		{
																			match(static_cast<unsigned char>('e'));
																			break;
																		}
																		case static_cast<unsigned char>('E'):
																		{
																			match(static_cast<unsigned char>('E'));
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('+'):
																		{
																			match(static_cast<unsigned char>('+'));
																			break;
																		}
																		case static_cast<unsigned char>('-'):
																		{
																			match(static_cast<unsigned char>('-'));
																			break;
																		}
																		case static_cast<unsigned char>('0'):
																		case static_cast<unsigned char>('1'):
																		case static_cast<unsigned char>('2'):
																		case static_cast<unsigned char>('3'):
																		case static_cast<unsigned char>('4'):
																		case static_cast<unsigned char>('5'):
																		case static_cast<unsigned char>('6'):
																		case static_cast<unsigned char>('7'):
																		case static_cast<unsigned char>('8'):
																		case static_cast<unsigned char>('9'):
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		int _cnt64=0;
																		for (;;) {
																			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																			}
																			else {
																				if ( _cnt64>=1 ) { goto _loop64; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																			}
																			
																			_cnt64++;
																		}
																		_loop64:;
																		}
																	}
																	else {
																	}
																	
																	}
																	if ( inputState->guessing==0 ) {
#line 106 "wb_wbllexer.g"
																		_ttype = NUM_FLOAT;
#line 1859 "wb_wbllexer.cpp"
																	}
																}
																else if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (true)) {
																	mDIGITS(false);
																	if ( inputState->guessing==0 ) {
#line 112 "wb_wbllexer.g"
																		_ttype = INT;
#line 1867 "wb_wbllexer.cpp"
																	}
																}
																else if ((_tokenSet_2.member(LA(1))) && (true)) {
																	{
																	switch ( LA(1)) {
																	case static_cast<unsigned char>('a'):
																	case static_cast<unsigned char>('b'):
																	case static_cast<unsigned char>('c'):
																	case static_cast<unsigned char>('d'):
																	case static_cast<unsigned char>('e'):
																	case static_cast<unsigned char>('f'):
																	case static_cast<unsigned char>('g'):
																	case static_cast<unsigned char>('h'):
																	case static_cast<unsigned char>('i'):
																	case static_cast<unsigned char>('j'):
																	case static_cast<unsigned char>('k'):
																	case static_cast<unsigned char>('l'):
																	case static_cast<unsigned char>('m'):
																	case static_cast<unsigned char>('n'):
																	case static_cast<unsigned char>('o'):
																	case static_cast<unsigned char>('p'):
																	case static_cast<unsigned char>('q'):
																	case static_cast<unsigned char>('r'):
																	case static_cast<unsigned char>('s'):
																	case static_cast<unsigned char>('t'):
																	case static_cast<unsigned char>('u'):
																	case static_cast<unsigned char>('v'):
																	case static_cast<unsigned char>('w'):
																	case static_cast<unsigned char>('x'):
																	case static_cast<unsigned char>('y'):
																	case static_cast<unsigned char>('z'):
																	{
																		matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																		break;
																	}
																	case static_cast<unsigned char>('A'):
																	case static_cast<unsigned char>('B'):
																	case static_cast<unsigned char>('C'):
																	case static_cast<unsigned char>('D'):
																	case static_cast<unsigned char>('E'):
																	case static_cast<unsigned char>('F'):
																	case static_cast<unsigned char>('G'):
																	case static_cast<unsigned char>('H'):
																	case static_cast<unsigned char>('I'):
																	case static_cast<unsigned char>('J'):
																	case static_cast<unsigned char>('K'):
																	case static_cast<unsigned char>('L'):
																	case static_cast<unsigned char>('M'):
																	case static_cast<unsigned char>('N'):
																	case static_cast<unsigned char>('O'):
																	case static_cast<unsigned char>('P'):
																	case static_cast<unsigned char>('Q'):
																	case static_cast<unsigned char>('R'):
																	case static_cast<unsigned char>('S'):
																	case static_cast<unsigned char>('T'):
																	case static_cast<unsigned char>('U'):
																	case static_cast<unsigned char>('V'):
																	case static_cast<unsigned char>('W'):
																	case static_cast<unsigned char>('X'):
																	case static_cast<unsigned char>('Y'):
																	case static_cast<unsigned char>('Z'):
																	{
																		matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																		break;
																	}
																	case static_cast<unsigned char>('$'):
																	{
																		match(static_cast<unsigned char>('$'));
																		break;
																	}
																	case static_cast<unsigned char>('_'):
																	{
																		match(static_cast<unsigned char>('_'));
																		break;
																	}
																	case static_cast<unsigned char>('\304'):
																	case static_cast<unsigned char>('\305'):
																	case static_cast<unsigned char>('\326'):
																	case static_cast<unsigned char>('\344'):
																	case static_cast<unsigned char>('\345'):
																	case static_cast<unsigned char>('\366'):
																	{
																		mSWEC(false);
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																	}
																	}
																	}
																	{
																	for (;;) {
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('a'):
																		case static_cast<unsigned char>('b'):
																		case static_cast<unsigned char>('c'):
																		case static_cast<unsigned char>('d'):
																		case static_cast<unsigned char>('e'):
																		case static_cast<unsigned char>('f'):
																		case static_cast<unsigned char>('g'):
																		case static_cast<unsigned char>('h'):
																		case static_cast<unsigned char>('i'):
																		case static_cast<unsigned char>('j'):
																		case static_cast<unsigned char>('k'):
																		case static_cast<unsigned char>('l'):
																		case static_cast<unsigned char>('m'):
																		case static_cast<unsigned char>('n'):
																		case static_cast<unsigned char>('o'):
																		case static_cast<unsigned char>('p'):
																		case static_cast<unsigned char>('q'):
																		case static_cast<unsigned char>('r'):
																		case static_cast<unsigned char>('s'):
																		case static_cast<unsigned char>('t'):
																		case static_cast<unsigned char>('u'):
																		case static_cast<unsigned char>('v'):
																		case static_cast<unsigned char>('w'):
																		case static_cast<unsigned char>('x'):
																		case static_cast<unsigned char>('y'):
																		case static_cast<unsigned char>('z'):
																		{
																			matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																			break;
																		}
																		case static_cast<unsigned char>('A'):
																		case static_cast<unsigned char>('B'):
																		case static_cast<unsigned char>('C'):
																		case static_cast<unsigned char>('D'):
																		case static_cast<unsigned char>('E'):
																		case static_cast<unsigned char>('F'):
																		case static_cast<unsigned char>('G'):
																		case static_cast<unsigned char>('H'):
																		case static_cast<unsigned char>('I'):
																		case static_cast<unsigned char>('J'):
																		case static_cast<unsigned char>('K'):
																		case static_cast<unsigned char>('L'):
																		case static_cast<unsigned char>('M'):
																		case static_cast<unsigned char>('N'):
																		case static_cast<unsigned char>('O'):
																		case static_cast<unsigned char>('P'):
																		case static_cast<unsigned char>('Q'):
																		case static_cast<unsigned char>('R'):
																		case static_cast<unsigned char>('S'):
																		case static_cast<unsigned char>('T'):
																		case static_cast<unsigned char>('U'):
																		case static_cast<unsigned char>('V'):
																		case static_cast<unsigned char>('W'):
																		case static_cast<unsigned char>('X'):
																		case static_cast<unsigned char>('Y'):
																		case static_cast<unsigned char>('Z'):
																		{
																			matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																			break;
																		}
																		case static_cast<unsigned char>('0'):
																		case static_cast<unsigned char>('1'):
																		case static_cast<unsigned char>('2'):
																		case static_cast<unsigned char>('3'):
																		case static_cast<unsigned char>('4'):
																		case static_cast<unsigned char>('5'):
																		case static_cast<unsigned char>('6'):
																		case static_cast<unsigned char>('7'):
																		case static_cast<unsigned char>('8'):
																		case static_cast<unsigned char>('9'):
																		{
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																			break;
																		}
																		case static_cast<unsigned char>('$'):
																		{
																			match(static_cast<unsigned char>('$'));
																			break;
																		}
																		case static_cast<unsigned char>('_'):
																		{
																			match(static_cast<unsigned char>('_'));
																			break;
																		}
																		case static_cast<unsigned char>('\304'):
																		case static_cast<unsigned char>('\305'):
																		case static_cast<unsigned char>('\326'):
																		case static_cast<unsigned char>('\344'):
																		case static_cast<unsigned char>('\345'):
																		case static_cast<unsigned char>('\366'):
																		{
																			mSWEC(false);
																			break;
																		}
																		default:
																		{
																			goto _loop81;
																		}
																		}
																	}
																	_loop81:;
																	}
																}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}}}}}}}}}}}}}}}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mSWEC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = SWEC;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('\345'):
	{
		match(static_cast<unsigned char>('\345'));
		break;
	}
	case static_cast<unsigned char>('\344'):
	{
		match(static_cast<unsigned char>('\344'));
		break;
	}
	case static_cast<unsigned char>('\366'):
	{
		match(static_cast<unsigned char>('\366'));
		break;
	}
	case static_cast<unsigned char>('\305'):
	{
		match(static_cast<unsigned char>('\305'));
		break;
	}
	case static_cast<unsigned char>('\304'):
	{
		match(static_cast<unsigned char>('\304'));
		break;
	}
	case static_cast<unsigned char>('\326'):
	{
		match(static_cast<unsigned char>('\326'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mDIGITS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = DIGITS;
	int _saveIndex;
	
	{
	int _cnt89=0;
	for (;;) {
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
		}
		else {
			if ( _cnt89>=1 ) { goto _loop89; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt89++;
	}
	_loop89:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCHAR_LITERAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = CHAR_LITERAL;
	int _saveIndex;
	
	match(static_cast<unsigned char>('\''));
	{
	if ((LA(1)==static_cast<unsigned char>('\\'))) {
		mESC(false);
	}
	else if ((_tokenSet_3.member(LA(1)))) {
		matchNot(static_cast<unsigned char>('\''));
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	
	}
	match(static_cast<unsigned char>('\''));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mESC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = ESC;
	int _saveIndex;
	
	match(static_cast<unsigned char>('\\'));
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('n'):
	{
		match(static_cast<unsigned char>('n'));
		break;
	}
	case static_cast<unsigned char>('r'):
	{
		match(static_cast<unsigned char>('r'));
		break;
	}
	case static_cast<unsigned char>('t'):
	{
		match(static_cast<unsigned char>('t'));
		break;
	}
	case static_cast<unsigned char>('b'):
	{
		match(static_cast<unsigned char>('b'));
		break;
	}
	case static_cast<unsigned char>('f'):
	{
		match(static_cast<unsigned char>('f'));
		break;
	}
	case static_cast<unsigned char>('"'):
	{
		match(static_cast<unsigned char>('"'));
		break;
	}
	case static_cast<unsigned char>('\''):
	{
		match(static_cast<unsigned char>('\''));
		break;
	}
	case static_cast<unsigned char>('\\'):
	{
		match(static_cast<unsigned char>('\\'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mSTRING_LITERAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = STRING_LITERAL;
	int _saveIndex;
	
	match(static_cast<unsigned char>('"'));
	{
	for (;;) {
		if ((LA(1)==static_cast<unsigned char>('\\'))) {
			mESC(false);
		}
		else if ((_tokenSet_4.member(LA(1)))) {
			matchNot(static_cast<unsigned char>('"'));
		}
		else {
			goto _loop86;
		}
		
	}
	_loop86:;
	}
	match(static_cast<unsigned char>('"'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}


const unsigned long wb_wbllexer::_tokenSet_0_data_[] = { 4294966264UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long wb_wbllexer::_tokenSet_1_data_[] = { 0UL, 67059712UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_1(_tokenSet_1_data_,10);
const unsigned long wb_wbllexer::_tokenSet_2_data_[] = { 0UL, 16UL, 2281701374UL, 134217726UL, 0UL, 0UL, 4194352UL, 4194352UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_2(_tokenSet_2_data_,16);
const unsigned long wb_wbllexer::_tokenSet_3_data_[] = { 4294967288UL, 4294967167UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_3(_tokenSet_3_data_,16);
const unsigned long wb_wbllexer::_tokenSet_4_data_[] = { 4294967288UL, 4294967291UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_4(_tokenSet_4_data_,16);

