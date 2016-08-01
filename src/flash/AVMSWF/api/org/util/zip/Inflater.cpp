#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ByteArray.h"
#include "AS3Endian.h"
#include "Inflater.h"


#include "gzipapis.h"
#include "genericzlib.h"

namespace avmshell{

#ifdef __USEEXT_INFLATE__
static const XU16 LENS[] ={3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
static const int MAXDCODES = 30;
static const int MAXLCODES = 286;
static const int FIXLCODES = 288;
static const int MAXCODES = MAXLCODES + MAXDCODES;
static const int MAXBITS = 15;        
        
static const int LEXT[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
static const int DISTS[] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
static const int DEXT[] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
#endif
//SETARRAY(AR,IDX,V) AR->setUIntProperty

InflaterClass::InflaterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* InflaterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InflaterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InflaterObject::InflaterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
void InflaterObject::AS3_setInput(ByteArrayObject*pInput)
{
	inbuf=pInput;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	inbuf->AS3_endian_set(top->getEndianClass()->getSlotLITTLE_ENDIAN());
}

#ifdef __USEEXT_INFLATE__
void InflaterObject::throwError(const char*buf,int id)
{
	//Toplevel*top=this->toplevel();
	//top->errorClass()->throwError(id,core()->newConstantStringLatin1(buf));
	toplevel()->throwError(kShellCompressedDataError);
}

void InflaterObject::stored(ByteArrayObject* param1)
{
    uint32_t _loc_2 = 0;
    bitbuf = 0;
    bitcnt = 0;
	Toplevel*top=this->toplevel();
	uint8_t*buf=inbuf->GetByteArray().GetBuffer();
	if (incnt + 4 > inbuf->GetLength())
    {
        throwError("available inflate data did not terminate", 2);
    }
    _loc_2 = buf[incnt++];
    _loc_2 = _loc_2 | (buf[incnt++] << 8);
    if (buf[incnt++] != (~_loc_2 & 255) || buf[incnt++] != (~_loc_2 >> 8 & 255))
    {
        throwError("stored block length did not match one\'s complement", -2);
    }
	if (incnt + _loc_2 > inbuf->GetLength())
    {
        throwError("available inflate data did not terminate", 2);
    }
    //while (_loc_2--)
    //{   
    //    param1[param1.length] = buf[incnt++];
    //}
	param1->WriteBytes(buf,_loc_2);
	incnt+=_loc_2;
    return;
}// end function


int InflaterObject::construct(ArrayObject *count,ArrayObject*symbol,int*param2, int param3)
        {
			

			Toplevel*top=toplevel();
			//ArrayObject* _loc_4 = top->arrayClass->newArray(0);
			int _loc_4[FIXLCODES+1];
			memset(_loc_4,0,sizeof(_loc_4));
			int _loc_5= 0;
            int _loc_6 = 0;
            int _loc_7 = 0;
            //_loc_4 = [];
            //_loc_5 = 0;
            while (_loc_5 <= MAXBITS)
            {
				//count->setUintProperty(_loc_5,zeroIntAtom);
                //param1.count[_loc_5] = 0;
				//count[_loc_5]=0;
				SetValue(count,_loc_5,0);
                _loc_5++;
            }
            _loc_6 = 0;
            while (_loc_6 < param3)
            {                
                //var _loc_8:* = param1.count;
                //var _loc_9:* = param2[_loc_6];
                //var _loc_10:* = param1.count[param2[_loc_6]] + 1;
                //_loc_8[_loc_9] = _loc_10;
				int id=param2[_loc_6];
				int value=GetValue(count,id)+1;
				SetValue(count,id,value);

                _loc_6++;
				//int _loc_9 = param2[_loc_6];//param2->getUintProperty(_loc_6);
				//int _loc_10 = count[_loc_9]+1;//count->getUintProperty(_loc_9)+1;
				//count[_loc_9]=_loc_10;//->setUintProperty(_loc_9,_loc_10);
            }
            //if (count[0] == param3)
			if(GetValue(count,0)==param3)
            {
                return 0;
            }
            _loc_7 = 1;
            _loc_5 = 1;
            while (_loc_5 <= MAXBITS)
            {
                
                _loc_7 = _loc_7 << 1;
                _loc_7 = _loc_7 - GetValue(count,_loc_5);//count[_loc_5];
                if (_loc_7 < 0)
                {
                    return _loc_7;
                }
                _loc_5++;
            }
            _loc_4[1] = 0;
            _loc_5 = 1;
            while (_loc_5 < MAXBITS)
            {
                
                _loc_4[(_loc_5 + 1)] = _loc_4[_loc_5] + GetValue(count,_loc_5);//count[_loc_5];
                _loc_5++;
            }
            _loc_6 = 0;
            while (_loc_6 < param3)
            {
                
                if (param2[_loc_6] != 0)
                {
                    int* _loc_9 = _loc_4;
                    int _loc_10 = param2[_loc_6];
                    //_loc_9[_loc_10] = _loc_4[_loc_10] + 1;
                    //symbol[++_loc_4[_loc_10]] = _loc_6;
					SetValue(symbol,_loc_4[_loc_10],_loc_6);
					_loc_4[_loc_10]++;
                }
                _loc_6++;
            }
            return _loc_7;
        }// end function
#endif
uint32_t InflaterObject::AS3_inflate(ByteArrayObject*param1)
{
	/*if(inbuf==NULL) return -1;

	unsigned long gzlen = inbuf->GetByteArray().GetLength();
	if (!gzlen) // empty buffer should give us a empty result
		return -1;

	//if(gzlen==8)
	//	gzlen = m_byteArray.GetLength();

	uint8_t *gzdata = inbuf->GetByteArray().GetBuffer();//mmfx_new_array( uint8_t, gzlen );
	//VMPI_memcpy(gzdata, m_byteArray.GetBuffer(), gzlen);

	
	// Clear the buffer
	//m_byteArray.Seek(0);
	//m_byteArray.SetLength(0);
	param1->SetLength(0);

	// The following block is to force destruction
	// of zstream before potential exception throw.
	//int nTotalIn=0;
	int error = Z_OK;
	{
		// Decompress the data
		PlatformZlibStream zstream;
		zstream.SetNextIn(gzdata);
		zstream.SetAvailIn(gzlen);

		const int kBufferSize = 8192;
		uint8_t *buffer = mmfx_new_array( uint8_t, kBufferSize );

		do {
			zstream.SetNextOut(buffer);
			zstream.SetAvailOut(kBufferSize);
			error = zstream.InflateWithStatus();
			param1->GetByteArray().Write(buffer, kBufferSize - zstream.AvailOut());
		} while (error == Z_OK);

		//nTotalIn=zstream.AvailIn();
		//if (error != Z_OK && error != Z_STREAM_END) {
		//	m_byteArray.SetLength(gzlen);//.Write(gzdata,gzlen);
			//toplevel()->throwError(kShellCompressedDataError);
		//}

		//XFile file;
		//file.Open("D:\\dc.dat",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
		//file.Write(buffer,8192);
		//file.Close();
	
		mmfx_delete_array( buffer );
		//mmfx_delete_array( gzdata );
	}

	// position byte array at the beginning
	param1->GetByteArray().Seek(0);*/

	//if (error != Z_OK && error != Z_STREAM_END) {
	//		m_byteArray.SetLength(gzlen);//.Write(gzdata,gzlen);
	//		toplevel()->throwError(kShellCompressedDataError);
	//	}
#ifdef __USEEXT_INFLATE__
	int _loc_2 = 0;
    int _loc_3 = 0;
    int _loc_4 = 0;
    int _loc_5 = 0;
    bitcnt = 0;    
    bitbuf = _loc_5;
    incnt = _loc_5;
    _loc_2 = 0;
	Toplevel*top=this->toplevel();
    do
    {
        
        _loc_3 = bits(1);
        _loc_4 = bits(2);
        if (_loc_4 == 0)
        {
            stored(param1);
        }
        else
        {
            if (_loc_4 == 3)
            {
				throwError("invalid block type (type == 3)",-1);
            }
            //lencode = {count:[], symbol:[]};
            //distcode = {count:[], symbol:[]};
			lencodeCount=top->arrayClass->newArray(0);
			lencodeSymbol=top->arrayClass->newArray(0);
			distcodeCount=top->arrayClass->newArray(0);
			distcodeSymbol=top->arrayClass->newArray(0);
            if (_loc_4 == 1)
            {
                constructFixedTables();
            }
            else if (_loc_4 == 2)
            {
                _loc_2 = constructDynamicTables();
            }
            if (_loc_2 != 0)
            {
                return _loc_2;
            }
            _loc_2 = codes(param1);
        }
        if (_loc_2 != 0)
        {
            break;
        }
    }while (!_loc_3);
    return _loc_2;
#else
	int len = inbuf->GetByteArray().GetLength();
	if (len <= 0) // 0 or (uint)len > max int
		return -1;

	int result_len = 0;
	CFileAlikeBuffer bin(len, (char *)inbuf->GetByteArray().GetBuffer(), len);
	CFileAlikeBuffer bout(result_len);
	
	int ret=gzip_decompress(&bin, &bout);
	
	if(ret<0) return ret;

	param1->SetLength(0);
	param1->GetByteArray().Write(bout.GetBuffer(), bout.Length());
	param1->Seek(0);
	return ret;
#endif
}
#ifdef __USEEXT_INFLATE__
int InflaterObject::decode(ArrayObject*count,ArrayObject*symbol)
        {
			int _loc_2=0;
			int _loc_3=0;
			int _loc_4=0;
			int _loc_5=1;
			int _loc_6=0;
            //var _loc_2:int = 0;
            //var _loc_3:int = 0;
            //var _loc_4:int = 0;
            //var _loc_5:int = 0;
            //var _loc_6:int = 0;
            //_loc_2 = 0;
            //_loc_3 = 0;
            //_loc_4 = 0;
           // _loc_5 = 1;
            while (_loc_5 <= MAXBITS)
            {                
                _loc_2 = _loc_2 | bits(1);
                _loc_6 = GetValue(count,_loc_5);//param1.count[_loc_5];
                if (_loc_2 < _loc_3 + _loc_6)
                {
                    return GetValue(symbol,_loc_4 + (_loc_2 - _loc_3));//param1.symbol[_loc_4 + (_loc_2 - _loc_3)];
                }
                _loc_4 = _loc_4 + _loc_6;
                _loc_3 = _loc_3 + _loc_6;
                _loc_3 = _loc_3 << 1;
                _loc_2 = _loc_2 << 1;
                _loc_5++;
            }
            return -9;
        }// end function

int InflaterObject::codes(ByteArrayObject*param1)
        {
			int _loc_2=0;
			int _loc_3=0;
			int _loc_4=0;
            //var _loc_2:* = undefined;
            //var _loc_3:* = undefined;
            //var _loc_4:* = undefined;
            do
            {
                
                _loc_2 = decode(lencodeCount,lencodeSymbol);
                if (_loc_2 < 0)
                {
                    return _loc_2;
                }
                if (_loc_2 < 256)
                {
                    //param1[param1.length] = _loc_2;
					param1->AS3_writeByte(_loc_2);
                    continue;
                }
                if (_loc_2 > 256)
                {
                    _loc_2 = _loc_2 - 257;
                    if (_loc_2 >= 29)
                    {
                        //throw new Error("invalid literal/length or distance code in fixed or dynamic block", -9);
						throwError("invalid literal/length or distance code in fixed or dynamic block", -9);
                    }
                    _loc_3 = LENS[_loc_2] + bits(LEXT[_loc_2]);
                    _loc_2 = decode(distcodeCount,distcodeSymbol);
                    if (_loc_2 < 0)
                    {
                        return _loc_2;
                    }
                    _loc_4 = DISTS[_loc_2] + bits(DEXT[_loc_2]);
					if (_loc_4 > param1->GetLength())//param1.length)
                    {
                        //throw new Error("distance is too far back in fixed or dynamic block", -10);
						throwError("distance is too far back in fixed or dynamic block", -10);
                    }
                    while (_loc_3--)
                    {
						int l=param1->GetLength();
						PushByte(param1,GetByteValue(param1,l-_loc_4));
                        //param1[param1.length] = param1[param1.length - _loc_4];
						
                    }
                }
            }while (_loc_2 != 256);
            return 0;
        }// end function

int InflaterObject::constructDynamicTables()
        {
			//ArrayClass*pClass=toplevel()->arrayClass;
            //var _loc_1:Array = null;
            //var _loc_2:Array = null;
            //var _loc_3:int = 0;
            //var _loc_4:int = 0;
            //var _loc_5:int = 0;
            //var _loc_6:int = 0;
            //var _loc_7:int = 0;
            //var _loc_8:int = 0;
            //var _loc_9:int = 0;
            //_loc_1 = [];
			//ArrayObject* _loc_1=pClass->newArray(0);
			int _loc_1[512];//[19]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			memset(_loc_1,0,512*4);
			int _loc_2[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
            int _loc_3 = bits(5) + 257;
            int _loc_4 = bits(5) + 1;
            int _loc_5 = bits(4) + 4;
			int _loc_6=0;
			int _loc_7=0;
			int _loc_8=0;
			int _loc_9=0;
            if (_loc_3 > MAXLCODES || _loc_4 > MAXDCODES)
            {
                //throw new Error("dynamic block code description: too many length or distance codes", -3);
				throwError("dynamic block code description: too many length or distance codes", -3);
            }
            _loc_6 = 0;
            while (_loc_6 < _loc_5)
            {
                _loc_1[_loc_2[_loc_6]] = bits(3);
				//SetValue(_loc_1,_loc_2[_loc_6],bits(3));
                _loc_6++;
            }
            while (_loc_6 < 19)
            {                
                _loc_1[_loc_2[_loc_6]] = 0;
                _loc_6++;
            }
            _loc_7 = construct(lencodeCount,lencodeSymbol,_loc_1, 19);


			/*{
				int iCount=lencodeSymbol->get_length();
				XString16 ot("Symbol:");
				for(int i=0;i<iCount;i++)
				{
					XString16 s;
					s.FromInt(lencodeSymbol->getUintProperty(i)>>3);
					ot+=s;
					ot+=XString16(",");
				}
				XSWFPlayer::m_pInstance->Trace(ot);
			}*/

            if (_loc_7 != 0)
            {
                //throw new Error("dynamic block code description: code lengths codes incomplete", -4);
				throwError("dynamic block code description: code lengths codes incomplete", -4);
            }
            _loc_6 = 0;
            while (_loc_6 < _loc_3 + _loc_4)
            {
                
                _loc_8 = decode(lencodeCount,lencodeSymbol);
                if (_loc_8 < 16)
                {
                    _loc_1[_loc_6] = _loc_8;
					_loc_6++;
                    continue;
                }
                _loc_9 = 0;
                if (_loc_8 == 16)
                {
                    if (_loc_6 == 0)
                    {
                        throwError("dynamic block code description: repeat lengths with no first length", -5);
                    }
                    _loc_9 = _loc_1[(_loc_6 - 1)];
                    _loc_8 = 3 + bits(2);
                }
                else if (_loc_8 == 17)
                {
                    _loc_8 = 3 + bits(3);
                }
                else
                {
                    _loc_8 = 11 + bits(7);
                }
                if (_loc_6 + _loc_8 > _loc_3 + _loc_4)
                {
                    throwError("dynamic block code description: repeat more than specified lengths", -6);
                }
                while (_loc_8--)
                {
                    
                    _loc_1[_loc_6] = _loc_9;
					_loc_6++;
                }
            }
			//if(_loc_6==298)
			//	int v=0;
            _loc_7 = construct(lencodeCount,lencodeSymbol, _loc_1, _loc_3);
            if (_loc_7 != 0 && _loc_3 - GetValue(lencodeCount,0) != 1)
            {
                throwError("dynamic block code description: invalid literal/length code lengths", -7);
            }
            _loc_7 = construct(distcodeCount,distcodeSymbol, _loc_1+_loc_3, _loc_4);
            if (_loc_7 != 0 && _loc_4 - GetValue(distcodeCount,0) != 1)
            {
                throwError("dynamic block code description: invalid distance code lengths", -8);
            }
            return _loc_7;
        }// end function

int InflaterObject::bits(int param1)
    {
		AvmCore*c=core();
		Toplevel*top=toplevel();
        int _loc_2 = bitbuf;
		uint8_t*buf=inbuf->GetByteArray().GetBuffer();
        while (bitcnt < param1)
        {            
			if (incnt == inbuf->GetLength())//.length)
            {
				top->errorClass()->throwError(2,c->newConstantStringLatin1("available inflate data did not terminate"));
            }
            _loc_2 = _loc_2 | buf[incnt++] << bitcnt;
            bitcnt = bitcnt + 8;
        }
        bitbuf = _loc_2 >> param1;
        bitcnt = bitcnt - param1;
        return _loc_2 & (1 << param1) - 1;
    }// end function


void InflaterObject::constructFixedTables()
{
    // var _loc_1:Array = null;
	int _loc_1[FIXLCODES];
    //var _loc_2:int = 0;
    //_loc_1 = [];
    int _loc_2 = 0;
    while (_loc_2 < 144)
    {        
        _loc_1[_loc_2] = 8;
        _loc_2++;
    }
    while (_loc_2 < 256)
    {
        
        _loc_1[_loc_2] = 9;
        _loc_2++;
    }
    while (_loc_2 < 280)
    {
        
        _loc_1[_loc_2] = 7;
        _loc_2++;
    }
    while (_loc_2 < FIXLCODES)
    {
        
        _loc_1[_loc_2] = 8;
        _loc_2++;
    }
    construct(lencodeCount,lencodeSymbol, _loc_1, FIXLCODES);
    _loc_2 = 0;
    while (_loc_2 < MAXDCODES)
    {
        
        _loc_1[_loc_2] = 5;
        _loc_2++;
    }
    construct(distcodeCount,distcodeSymbol, _loc_1, MAXDCODES);
    return;
}// end function
#endif
}