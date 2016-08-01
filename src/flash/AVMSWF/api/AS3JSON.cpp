#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3JSON.h"
#include "ShellCore.h"

enum{
		JS_NONE = -2,
		JS_UNKNOWN = -1,
		JS_COMMA = 0,
		JS_LEFT_BRACE = 1,
		JS_RIGHT_BRACE = 2,
		JS_LEFT_BRACKET = 3,
		JS_RIGHT_BRACKET = 4,
		JS_COLON = 6,
		JS_TRUE = 7,
		JS_FALSE = 8,
		JS_NULL = 9,
		JS_STRING = 10,
		JS_NUMBER = 11,
		JS_NAN = 12		
	};
enum
{
	VALUE_FALSE = -1,
	VALUE_TRUE = -2,
	VALUE_NAN = -3,
	VALUE_NULL = -4
};

namespace avmshell
{
	static int FindChar(const char* pChars, char c, int start = 0, int end = 0x7fffffff)
	{
		for ( ; start <= end && pChars[start]; start++ )
		{
			if( pChars[start] != c )
				continue;
			else
				return start;
		}	
		return -1;
	}
	class JSONDecoder
	{
	private:
		bool m_strict;
		//JSONTokenizer* tokenizer;
		JSONClass*m_pClass;
		AvmCore* m_core;
		Toplevel*m_top;
		Atom m_value;
		const char* m_pStr;
		int m_loc;
		char m_ch;
		int m_tokenType;
		char m_tokenChar;
		Atom m_tokenValue;
	public:
		JSONDecoder(const char* s,bool strict,JSONClass*pClass,AvmCore*core,Toplevel*top)
		{	
			m_core = core;
			m_top = top;
			m_pClass = pClass;
			m_strict = strict;
			m_pStr = s;
			m_loc = 0;
			nextChar();
			//tokenizer = new JSONTokenizer( s, strict );
			
			nextToken();
			m_value = parseValue();
			//if(!m_core->isObject(atom)
				
			
			// Make sure the input stream is empty
			if ( m_strict && nextToken() != JS_NONE )
			{
				m_pClass->ThrowError( "Unexpected characters left in input stream" );
			}
		}
		Atom getValue()
		{
			return m_value;
		}
	private:
		int nextToken()
		{
			//return JS_NONE;
			//var token:JSONToken = new JSONToken();
			//m_tokenType = JS_LEFT_BRACE;
			//m_tokenValue = "";
			// skip any whitespace / comments since the last 
			// token was read
			m_tokenType = JS_NONE;
			//m_tokenValue = 0;
			skipIgnored();
			
			char ch = m_ch;
			// examine the new character and see what we have...
			switch ( ch )
			{	
				case '{':
					m_tokenType = JS_LEFT_BRACE;
					m_tokenChar = '{';
					nextChar();
					break;
					
				case '}':
					m_tokenType = JS_RIGHT_BRACE;
					m_tokenChar = '}';
					nextChar();
					break;
					
				case '[':
					m_tokenType = JS_LEFT_BRACKET;
					m_tokenChar = '[';
					nextChar();
					break;
					
				case ']':
					m_tokenType = JS_RIGHT_BRACKET;
					m_tokenChar = ']';
					nextChar();
					break;
				
				case ',':
					m_tokenType = JS_COMMA;
					m_tokenChar = ',';
					nextChar();
					break;
					
				case ':':
					m_tokenType = JS_COLON;
					m_tokenChar = ':';
					nextChar();
					break;
					
				case 't': // attempt to read true
					//var possibleTrue:String = "t" + nextChar() + nextChar() + nextChar();
					{
						char possibleTrue[5]={'t'};
						possibleTrue[1]=nextChar();
						possibleTrue[2]=nextChar();
						possibleTrue[3]=nextChar();
						possibleTrue[4]=0;
						
						if ( strcmp(possibleTrue,"true")==0 )
						{
							m_tokenType = JS_TRUE;
							m_tokenValue = trueAtom;
							nextChar();
						}
						else
						{
							//parseError( "Expecting 'true' but found " + possibleTrue );
							m_pClass->ThrowError("Expecting 'true' but found %s", possibleTrue);
						}
					}
					break;
					
				case 'f': // attempt to read false
					{
						char possibleFalse[6] ={'f'};// "f" + nextChar() + nextChar() + nextChar() + nextChar();
						possibleFalse[1]=nextChar();
						possibleFalse[2]=nextChar();
						possibleFalse[3]=nextChar();
						possibleFalse[4]=nextChar();
						possibleFalse[5]=0;

						
						if ( strcmp(possibleFalse,"false")==0 )
						{
							m_tokenType = JS_FALSE;
							m_tokenValue = falseAtom;
							nextChar();
						}
						else
						{
							m_pClass->ThrowError( "Expecting 'false' but found %s", possibleFalse );
						}
					}
					break;
					
				case 'n': // attempt to read null
					{
						char possibleNull[5] ={'n'};// "n" + nextChar() + nextChar() + nextChar();
						possibleNull[1]=nextChar();
						possibleNull[2]=nextChar();
						possibleNull[3]=nextChar();
						possibleNull[4]=0;
						if ( strcmp(possibleNull,"null")==0 )
						{
							m_tokenType = JS_NULL;
							m_tokenValue = nullObjectAtom;
							nextChar();
						}
						else
						{
							m_pClass->ThrowError( "Expecting 'null' but found %s",possibleNull );
						}
					}
					break;
					
				case 'N': // attempt to read NaN
					{
						char possibleNaN[4]={'N'};//:String = "N" + nextChar() + nextChar();
						possibleNaN[1]=nextChar();
						possibleNaN[2]=nextChar();
						possibleNaN[3]=0;
						if ( strcmp(possibleNaN,"NaN")==0 )
						{
							m_tokenType = JS_NAN;
							//union { float f; uint32_t d; }; d = 0x7FFFFFFF;
							m_tokenValue = m_core->doubleToAtom(MathUtils::kNaN);
							nextChar();
						}
						else
						{
							m_pClass->ThrowError( "Expecting 'NaN' but found %s",possibleNaN );
						}
					}
					break;
					
				case '"': // the start of a string
					m_tokenValue = readString();
					break;
					
				default: 
					// see if we can read a number
					if ( isDigit( ch ) || ch == '-' )
					{
						m_tokenValue = readNumber();
					}
					else if ( ch == 0 )
					{
						// check for reading past the end of the string
						return JS_NONE;
					}
					else
					{						
						// not sure what was in the input string - it's not
						// anything we expected
						m_pClass->ThrowError("Unexpected %c encountered",ch);
						//parseError( "Unexpected " + ch + " encountered" );
					}
			}
			
			return m_tokenType;
		}
		Atom parseValue()
		{
			// Catch errors when the input stream ends abruptly
			/*if ( m_tokenType == JS_NULL )
			{
				m_pClass->ThrowError( "Unexpected end of input" );
			}*/
			//if(strstr(m_pStr,"taskContent")!=NULL)
			//	int v=0;
					
			switch ( m_tokenType )
			{
				case JS_NONE:
					m_pClass->ThrowError( "Unexpected end of input" );
					break;
				case JS_LEFT_BRACE:
					return parseObject();
					
				case JS_LEFT_BRACKET:
					return parseArray();
					
				case JS_STRING:
				case JS_NUMBER:
				case JS_TRUE:
				case JS_FALSE:
				case JS_NULL:
					return m_tokenValue;
					
				case JS_NAN:
					if ( !m_strict )
					{
						return m_tokenValue;
					}
					else
					{
						m_pClass->ThrowError( "Unexpected Value!");
					}

				default:
					m_pClass->ThrowError( "Unexpected Value");
					
			}
			
            return nullObjectAtom;
			//return NULL;
		}	

		Atom parseArray()
		{
			// create an array internally that we're going to attempt
			// to parse from the tokenizer
			//var a:Array = new Array();
			ArrayObject* a = m_top->arrayClass->newArray(0);
			
			// grab the next token from the tokenizer to move
			// past the opening [
			nextToken();
			
			// check to see if we have an empty array
			if ( m_tokenType == JS_RIGHT_BRACKET )
			{
				// we're done reading the array, so return it
				return a->atom();
			}
			// in non-strict mode an empty array is also a comma
			// followed by a right bracket
			else if ( !m_strict && m_tokenType == JS_COMMA )
			{
				// move past the comma
				nextToken();
				
				// check to see if we're reached the end of the array
				if ( m_tokenType == JS_RIGHT_BRACKET )
				{
					return a->atom();	
				}
				else
				{
					m_pClass->ThrowError( "Leading commas are not supported.  Expecting ']' but found ");
				}
			}
			
			// deal with elements of the array, and use an "infinite"
			// loop because we could have any amount of elements
			while ( true )
			{
				// read in the value and add it to the array
				Atom values[1]={parseValue()};
				a->push(values,1);
				//a.push( parseValue() );
			
				// after the value there should be a ] or a ,
				nextToken();
				
				if ( m_tokenType == JS_RIGHT_BRACKET )
				{
					// we're done reading the array, so return it
					return a->atom();
				}
				else if ( m_tokenType == JS_COMMA )
				{
					// move past the comma and read another value
					nextToken();
					
					// Allow arrays to have a comma after the last element
					// if the decoder is not in strict mode
					if ( !m_strict )
					{
						// Reached ",]" as the end of the array, so return it
						if ( m_tokenType == JS_RIGHT_BRACKET )
						{
							return a->atom();
						}
					}
				}
				else
				{
					m_pClass->ThrowError( "Expecting ] or , but found ");
				}
			}
            return nullObjectAtom;
		}

		Atom parseObject()
		{
			// create the object internally that we're going to
			// attempt to parse from the tokenizer
			//var o:Object = new Object();
			ScriptObject* o = m_top->objectClass->newInstance();
						
			// store the string part of an object member so
			// that we can assign it a value in the object
			Stringp key;
			
			// grab the next token from the tokenizer
			nextToken();
			
			// check to see if we have an empty object
			if ( m_tokenType == JS_RIGHT_BRACE )
			{
				// we're done reading the object, so return it
				return o->atom();
			}
			// in non-strict mode an empty object is also a comma
			// followed by a right bracket
			else if ( !m_strict && m_tokenType == JS_COMMA )
			{
				// move past the comma
				nextToken();
				
				// check to see if we're reached the end of the object
				if ( m_tokenType == JS_RIGHT_BRACE )
				{
					return o->atom();
				}
				else
				{
					m_pClass->ThrowError( "Leading commas are not supported.  Expecting '}' but found ");
				}
			}
			
			// deal with members of the object, and use an "infinite"
			// loop because we could have any amount of members
			while ( true )
			{
				if ( m_tokenType == JS_STRING )
				{
					// the string value we read is the key for the object
					key = m_core->atomToString(m_tokenValue);
						//String( token.value );
					//key->setInterned();
					// move past the string to see what's next
					nextToken();
					
					// after the string there should be a :
					if ( m_tokenType == JS_COLON )
					{	
						// move past the : and read/assign a value for the key
						nextToken();
						//o[key] = parseValue();
						//m_core->internString(
						o->setStringProperty(m_core->internString(key),parseValue());
						
						// move past the value to see what's next
						nextToken();
						
						// after the value there's either a } or a ,
						if ( m_tokenType == JS_RIGHT_BRACE )
						{
							// we're done reading the object, so return it
							return o->atom();	
						}
						else if ( m_tokenType == JS_COMMA )
						{
							// skip past the comma and read another member
							nextToken();
							
							// Allow objects to have a comma after the last member
							// if the decoder is not in strict mode
							if ( !m_strict )
							{
								// Reached ",}" as the end of the object, so return it
								if ( m_tokenType == JS_RIGHT_BRACE )
								{
									return o->atom();
								}
							}
						}
						else
						{
							m_pClass->ThrowError( "Expecting } or , but found ");
						}
					}
					else
					{
						m_pClass->ThrowError( "Expecting : but found " );
					}
				}
				else
				{	
					m_pClass->ThrowError( "Expecting string but found ");
				}
			}
            return nullObjectAtom;
		}

		inline void skipIgnored()
		{
			int originalLoc=0;
			
			// keep trying to skip whitespace and comments as long
			// as we keep advancing past the original location 
			do
			{
				originalLoc = m_loc;
				skipWhite();
				skipComments();
			}
			while ( originalLoc != m_loc );
		}

	
		Atom readNumber()
		{
			// the string to accumulate the number characters
			// into that we'll convert to a number at the end
			//var input:String = "";
			//int loc = m_loc-1;
			//Stringp input=m_core->newStringLatin1("");
			XString8 input;
			
			// check for a negative number
			if ( m_ch == '-' )
			{
				input += '-';
				nextChar();
			}
			
			// the number must start with a digit
			if ( !isDigit( m_ch ) )
			{
				m_pClass->ThrowError( "Expecting a digit" );
			}
			
			// 0 can only be the first digit if it
			// is followed by a decimal point
			if ( m_ch == '0' )
			{
				input += m_ch;
				nextChar();
				
				// make sure no other digits come after 0
				if ( isDigit( m_ch ) )
				{
					m_pClass->ThrowError( "A digit cannot immediately follow 0" );
				}
				// unless we have 0x which starts a hex number, but this
				// doesn't match JSON spec so check for not strict mode.
				else if ( !m_strict && m_ch == 'x' )
				{
					// include the x in the input
					input += m_ch;
					nextChar();
					
					// need at least one hex digit after 0x to
					// be valid
					if ( isHexDigit( m_ch ) )
					{
						input += m_ch;
						nextChar();
					}
					else
					{
						m_pClass->ThrowError( "Number in hex format require at least one hex digit after \"0x\"" );	
					}
					
					// consume all of the hex values
					while ( isHexDigit( m_ch ) )
					{
						input += m_ch;
						nextChar();
					}
				}
			}
			else
			{
				// read numbers while we can
				while ( isDigit( m_ch ) )
				{
					input += m_ch;
					nextChar();
				}
			}
			
			// check for a decimal value
			if ( m_ch == '.' )
			{
				input += '.';
				nextChar();
				
				// after the decimal there has to be a digit
				if ( !isDigit( m_ch ) )
				{
					m_pClass->ThrowError( "Expecting a digit" );
				}
				
				// read more numbers to get the decimal value
				while ( isDigit( m_ch ) )
				{
					input += m_ch;
					nextChar();
				}
			}
			
			// check for scientific notation
			if ( m_ch == 'e' || m_ch == 'E' )
			{
				input += 'e';
				nextChar();
				// check for sign
				if ( m_ch == '+' || m_ch == '-' )
				{
					input += m_ch;
					nextChar();
				}
				
				// require at least one number for the exponent
				// in this case
				if ( !isDigit( m_ch ) )
				{
					m_pClass->ThrowError( "Scientific notation number needs exponent value" );
				}
							
				// read in the exponent
				while ( isDigit( m_ch ) )
				{
					input += m_ch;
					nextChar();
				}
			}
			
			// convert the string to a number value
			//var num:Number = Number( input );
			Stringp ns=m_core->newStringUTF8(input);
			double num = ns->toNumber();
			
			if ( !MathUtils::isInfinite( num ) && !MathUtils::isNaN( num ) )
			{
				// the token for the number that we've read
				//var token:JSONToken = new JSONToken();
				m_tokenType = JS_NUMBER;
				//token.value = num;
				//return token;
				return m_core->doubleToAtom(num);
			}
			else
			{
				//StUTF8String str(input);
				m_pClass->ThrowError( "Number %s is not valid!",input);
			}
			m_tokenType=JS_NONE;
            return 0;
		}

		Atom readString()
		{
			// Rather than examine the string character-by-character, it's
			// faster to use indexOf to try to and find the closing quote character
			// and then replace escape sequences after the fact.
			
			// Start at the current input stream position
			int quoteIndex = m_loc;
			do
			{
				// Find the next quote in the input stream
				quoteIndex = ::strchr(m_pStr+quoteIndex,'"')-m_pStr;
					//jsonString.indexOf( "\"", quoteIndex );
				
				if ( quoteIndex >= 0 )
				{
					// We found the next double quote character in the string, but we need
					// to make sure it is not part of an escape sequence.
					
					// Keep looping backwards while the previous character is a backslash
					int backspaceCount = 0;
					int backspaceIndex = quoteIndex - 1;
					while ( m_pStr[backspaceIndex]=='\\')//jsonString.charAt( backspaceIndex ) == "\\" )
					{
						backspaceCount++;
						backspaceIndex--;
					}
					
					// If we have an even number of backslashes, that means this is the ending quote 
					if ( backspaceCount % 2 == 0 )
					{
						break;
					}
					
					// At this point, the quote was determined to be part of an escape sequence
					// so we need to move past the quote index to look for the next one
					quoteIndex++;
				}
				else // There are no more quotes in the string and we haven't found the end yet
				{
					m_pClass->ThrowError( "Unterminated string literal" );
				}
			} while ( true );
			
			// Unescape the string
			// the token for the string we'll try to read
			//var token:JSONToken = new JSONToken();
			m_tokenType = JS_STRING;
			//Stringp str = unescapeString(m_pStr+m_loc,quoteIndex - m_loc);
				//m_core->newStringLatin1(m_pStr+m_loc,quoteIndex - m_loc);
			int len = quoteIndex - m_loc;
			Stringp str = m_core->kEmptyString;
			if (len)
			{
				int index = FindChar( m_pStr, '\\', m_loc, quoteIndex );
				if(index>=0)
				{
					str = unescapeString(m_pStr+m_loc, len);
				}else{
					str = String::createUTF8( m_core, (const utf8_t*)(m_pStr + m_loc), len );
				}
			}
			// Attach resulting string to the token to return it
			//token.value = unescapeString( jsonString.substr( loc, quoteIndex - loc ) );
			
			// Move past the closing quote in the input string.  This updates the next
			// character in the input stream to be the character one after the closing quote
			m_loc = quoteIndex + 1;
			nextChar();
			
			return str->atom();
		}

		Stringp unescapeString(const char* input,int len)
		{
			// Issue #104 - If the string contains any unescaped control characters, this
			// is an error in strict mode
			if(len<=0)
				return m_core->kEmptyString;
			int i;
			if ( m_strict )
			{
				for(i=0;i<len;i++)
				{
					if(input[i]>=0&&input[i]<=0x1F)
						m_pClass->ThrowError( "String contains unescaped control character (0x00-0x1F)" );
				}
			}
			
			//Stringp result=m_core->newStringLatin1("");
			XString8 result("");
			int backslashIndex = 0;
			int nextSubstringStartPosition = 0;
			//var len:int = input.length;
			do
			{
				// Find the next backslash in the input
				//backslashIndex = strchr(input+nextSubstringStartPosition,'\\')-input;
				backslashIndex = FindChar(input, '\\', nextSubstringStartPosition, len);
				if(backslashIndex>=len) backslashIndex=-1;
					//input.indexOf( '\\', nextSubstringStartPosition );
				
				if ( backslashIndex >= 0 )
				{
					//result=result->concatStrings(result,m_core->newStringLatin1(input+nextSubstringStartPosition,backslashIndex - nextSubstringStartPosition));
					if(backslashIndex > nextSubstringStartPosition)
						result.AddString(input+nextSubstringStartPosition,backslashIndex - nextSubstringStartPosition);
						//input.substr( nextSubstringStartPosition, backslashIndex - nextSubstringStartPosition );
					
					// Move past the backslash and next character (all escape sequences are
					// two characters, except for \u, which will advance this further)
					nextSubstringStartPosition = backslashIndex + 2;
					
					// Check the next character so we know what to escape
					int afterBackslashIndex = backslashIndex + 1;
					char escapedChar = input[afterBackslashIndex];//.charAt( afterBackslashIndex );
					switch ( escapedChar )
					{	
						// Try to list the most common expected cases first to improve performance
						
						case '"': result += '"'; break; // quotation mark
						case '\\': result += '\\'; break; // reverse solidus	
						case 'n': result += '\n'; break; // newline
						case 'r': result += '\r'; break; // carriage return
						case 't': result += '\t'; break; // horizontal tab	
						
						// Convert a unicode escape sequence to it's character value
						case 'u':
							{
							// Save the characters as a string we'll convert to an int
							//Stringp hexValue = m_core->newStringLatin1("");
							const char* pNum = input+nextSubstringStartPosition;
							int lc = 0;
							
							// Make sure there are enough characters in the string leftover
							if ( nextSubstringStartPosition + 4 > len )
							{
								m_pClass->ThrowError( "Unexpected end of input.  Expecting 4 hex digits after \\u." );
							}
							
							// Try to find 4 hex characters
							for ( i = nextSubstringStartPosition; i < nextSubstringStartPosition + 4; i++ )
							{
								// get the next character and determine
								// if it's a valid hex digit or not
								char possibleHexChar = input[i];//.charAt( i );
								if ( !isHexDigit( possibleHexChar ) )
								{
									m_pClass->ThrowError( "Excepted a hex digit, but found: %c", possibleHexChar );
								}
								
								// Valid hex digit, add it to the value
								//hexValue += possibleHexChar;
								lc++;
							}
							
							// Convert hexValue to an integer, and use that
							// integer value to create a character to add
							// to our string.
							if(lc>0)
							{
								Stringp hexValue = m_core->newStringLatin1(pNum,lc);
								result+=(char)hexValue->toNumber();
							}
							//result += String.fromCharCode( parseInt( hexValue, 16 ) );
							// Move past the 4 hex digits that we just read
							nextSubstringStartPosition += 4;
							}
							break;
						
						case 'f': result += '\f'; break; // form feed
						case '/': result += '/'; break; // solidus
						case 'b': result += '\b'; break; // bell
						default: result += '\\' + escapedChar; // Couldn't unescape the sequence, so just pass it through
							
					}
				}
				else
				{
					// No more backslashes to replace, append the rest of the string
					//result = result->concatStrings(result,m_core->newStringLatin1(input+nextSubstringStartPosition,len-nextSubstringStartPosition));
					result.AddString(input+nextSubstringStartPosition,len-nextSubstringStartPosition);
					//result += input.substr( nextSubstringStartPosition );
					break;
				}
				
			} while ( nextSubstringStartPosition < len );
			
			return m_core->newStringUTF8(result);
		}

		void skipComments()
		{
			if ( m_ch == '/' )
			{
				// Advance past the first / to find out what type of comment
				nextChar();
				switch ( m_ch )
				{
					case '/': // single-line comment, read through end of line
						
						// Loop over the characters until we find
						// a newline or until there's no more characters left
						do
						{
							nextChar();
						}
						while ( m_ch != '\n' && m_ch != 0 );
						
						// move past the \n
						nextChar();
						
						break;
					
					case '*': // multi-line comment, read until closing */

						// move past the opening *
						nextChar();
						
						// try to find a trailing */
						while ( true )
						{
							if ( m_ch == '*' )
							{
								// check to see if we have a closing /
								nextChar();
								if ( m_ch == '/')
								{
									// move past the end of the closing */
									nextChar();
									break;
								}
							}
							else
							{
								// move along, looking if the next character is a *
								nextChar();
							}
							
							// when we're here we've read past the end of 
							// the string without finding a closing */, so error
							if ( m_ch == 0 )
							{
								m_pClass->ThrowError( "Multi-line comment not closed" );
							}
						}

						break;
					
					// Can't match a comment after a /, so it's a parsing error
					default:
					{
						m_pClass->ThrowError( "Unexpected %c encountered (expecting '/' or '*' )",m_ch );
					}
				}
			}
			
		}
		
		inline bool isHexDigit( char ch )
		{
			return ( isDigit( ch ) || ( ch >= 'A' && ch <= 'F' ) || ( ch >= 'a' && ch <= 'f' ) );
		}

		inline bool isDigit(char ch)
		{
			return ( ch >= '0' && ch <= '9' );
		}

		inline void skipWhite()
		{	
			// As long as there are spaces in the input 
			// stream, advance the current location pointer
			// past them
			while ( isWhiteSpace( m_ch ) )
			{
				nextChar();
			}
			
		}

		inline char nextChar()
		{
			m_ch = m_pStr[m_loc];
			m_loc++;
			return m_ch;
		}

		inline bool isWhiteSpace( char ch )
		{
			// Check for the whitespace defined in the spec
			if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' )
			{
				return true;
			}
			// If we're not in strict mode, we also accept non-breaking space
			else if ( !m_strict && ch == 160 )
			{
				return true;
			}
			
			return false;
		}

	};



	/*class JSONEncoder
	{
	private:
		AvmCore* m_core;
		Toplevel*m_top;
		Stringp m_str;
	public:
		JSONEncoder(ScriptObject*obj,AvmCore*core,Toplevel*top)
		{
			m_core = core;
			m_top = top;
			m_str=convertToString(obj->atom());
		}

		Stringp getString()
		{
			return m_str;
		}

	private:
		Stringp escapeString(Stringp str)
		{
			// create a string to store the string's jsonstring value
			///var s:String = "";
			// current character in the string we're processing
			//Stringp s = m_core->newStringLatin1("");
			XString8 s("");
			//var ch:String;
			// store the length in a local variable to reduce lookups
			//var len:Number = str.length;
			char ch;
			StUTF8String output(str);
			int len = output.length();
			const char* pStr = output.c_str();
			int i;
			
			// loop over all of the characters in the string
			for ( i = 0; i < len; i++ ) {
			
				// examine the character to determine if we have to escape it
				ch = pStr[i];
				switch ( ch ) {
				
					case '"':	// quotation mark
						s += "\\\"";
						break;
						
					//case '/':	// solidus
					//	s += "\\/";
					//	break;
						
					case '\\':	// reverse solidus
						s += "\\\\";
						break;
						
					case '\b':	// bell
						s += "\\b";
						break;
						
					case '\f':	// form feed
						s += "\\f";
						break;
						
					case '\n':	// newline
						s += "\\n";
						break;
						
					case '\r':	// carriage return
						s += "\\r";
						break;
						
					case '\t':	// horizontal tab
						s += "\\t";
						break;
						
					default:	// everything else
						
						// check for a control character and escape as unicode
						if ( ch < ' ' ) {
							// get the hex digit(s) of the character (either 1 or 2 digits)
							//var hexCode:String = ch.charCodeAt( 0 ).toString( 16 );
							//XString8 hexCode("");
							char v=ch>>4;
							if(v>0)
							{
								s+="\\u00";
								if(v>=10)
									s+=(char)('a'+(v-10));
								else
									s+=(char)('0'+v);
								//hexCode+="
								//hexCode+=char(ch>>4);
							}
							else s+="\\u000";
							v=ch&0xf;
							if(v>=10)
								s+=(char)('a'+(v-10));
							else
								s+=(char)('0'+v);
							
							// ensure that there are 4 digits by adjusting
							// the # of zeros accordingly.
							//var zeroPad:String = hexCode.length == 2 ? "00" : "000";
							
							// create the unicode escape sequence with 4 hex digits
							//s += "\\u" + zeroPad + hexCode;
						} else {
						
							// no need to do any special encoding, just pass-through
							s += ch;
							
						}
				}	// end switch
				
			}	// end for loop
			
			return m_core->newStringUTF8(s);
			//return "\"" + s + "\"";
		}

		Stringp convertToString( Atom value )
		{
			switch(atomKind(value))
			{
			// determine what value is and convert it based on it's type
			case kStringType:
			///if ( value is String ) {
				
				// escape the string so it's formatted correctly
				 return escapeString( m_core->atomToString(value) );
				 break;
				
			case kDoubleType:
				{
					double value=m_core->atomToDouble(value);
					if(MathUtils::isInfinite(value))
						return m_core->knull;
					else
						return m_core->string(value);
				}
				break;
			case kBooleanType:
				if(m_core->boolean(value))
					return m_core->ktrue;
				else
					return m_core->kstring;
				break;
			default:
				{
					ScriptObject*obj=m_core->atomToScriptObject(value);
					if(((ShellCore*)m_core)->IsTypeClass(obj,m_top->arrayClass))
					{

					}
					else
					{

					}
				}
			}
            return m_core->knull;
		}
	};*/
	JSONParseErrorClass::JSONParseErrorClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};

	ScriptObject* JSONParseErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) JSONParseErrorObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}

	void JSONParseErrorClass::ThrowError(Stringp str)
	{
		Atom args[2] = { nullObjectAtom, str->atom() };
		
		core()->throwAtom(this->construct(1, args));
	}

	JSONParseErrorObject::JSONParseErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity):ErrorObject(_vtable,_delegate)
	{
	}


	JSONClass::JSONClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		m_pErrorClass = NULL;
		//file.Open("D:/JSON.txt",XFile::XBINARY|XFile::XCREATE|XFile::XWRITE);
		//XU8 utf8[]={0xEF,0xBB,0xBF};
		//file.Write(utf8,3);
		//Add your construct code here...
	};

	inline void JSONClass::ThrowError(char* str)
	{
		if(m_pErrorClass == NULL)
		{
			ShellToplevel* t = (ShellToplevel*)toplevel();
			m_pErrorClass=(JSONParseErrorClass*)t->getBuiltinExtensionClass(abcclass_JSONParseError,true);
		}

		m_pErrorClass->ThrowError(core()->newStringLatin1(str));
	}

	inline void JSONClass::ThrowError(char* str,char c)
	{
		if(m_pErrorClass == NULL)
		{
			ShellToplevel* t = (ShellToplevel*)toplevel();
			m_pErrorClass=(JSONParseErrorClass*)t->getBuiltinExtensionClass(abcclass_JSONParseError,true);
		}
		//Stringp out = core()->newStringLatin1(str);
		AvmCore* cc=core();
		//StringBuffer buffer(cc);
		//buffer.format(str,c);
		Stringp out = cc->newStringUTF8(str);
		m_pErrorClass->ThrowError(out);
	}

	inline void JSONClass::ThrowError(char* str,const char* c)
	{
		if(m_pErrorClass == NULL)
		{
			ShellToplevel* t = (ShellToplevel*)toplevel();
			m_pErrorClass=(JSONParseErrorClass*)t->getBuiltinExtensionClass(abcclass_JSONParseError,true);
		}
		//Stringp out = core()->newStringLatin1(str);
		AvmCore* cc=core();
		//StringBuffer buffer(cc);
		//buffer.format(str,c);
		//Stringp out = cc->newStringUTF8(buffer.c_str());
		//m_pErrorClass->ThrowError(out);
		Stringp out = cc->newStringUTF8(str);
		m_pErrorClass->ThrowError(out);
	}

	ScriptObject* JSONClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) JSONObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	JSONObject::JSONObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////

	Atom JSONClass::AS3_parse(Stringp text, FunctionObject* reviver)
	{
		//Add your construct code here...
		//this["test"] = "test";
		//ScriptObject *pObj = 
		//ScriptObject *pObj = createInstance( ivtable(),prototype );
		/*ScriptObject *pTmp = core()->newObject(ivtable(), prototype );
		pTmp->traits()->set_needsHashtable( true );
		Stringp pstr = String::createLatin1( core(), "test");
		Stringp pname = String::createLatin1( core(), "name" );
		pname->setInterned();
		pTmp->setAtomProperty( pname->toAtom(), pstr->toAtom() );
		//setStringProperty( pname, pstr->toAtom() );
		return this;*/
		StUTF8String str(text);
		/*if(strstr(str.c_str(),"breakTopMissionId")!=NULL)
		{
			XFile file;
			if(file.Open("D:\\js.txt",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE))
			{
				file.Write(str.c_str(),str.length());
				file.Close();
			}
			//int v=0;
		}*/		
		//file.Write(str.c_str(),str.length());
		//file.Write("\r\n",2);
		JSONDecoder jd( str.c_str(), reviver!=NULL,this,core(),toplevel() );
		return jd.getValue();
		
		
		//return NULL;
	}
	Stringp JSONClass::AS3_stringify(ScriptObject* value, void* replacer, void* space)
	{
		//Add your construct code here...
		//JSONEncoder je( value,core(),toplevel() );
		//return je.getString();
		return NULL;
	}

}