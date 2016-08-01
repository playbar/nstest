#include "StdAfxGGBase.h"
#include "BWJpegEncode.h"
#undef FAR
//#ifdef INT32
#ifdef WIN32
#define XMD_H
#endif
//#endif
#include "jpeglib.h"

XJpegEncode::XJpegEncode()
{

}

XJpegEncode::~XJpegEncode()
{
	Release();
}

void XJpegEncode::Release()
{
}

/*static void WriteData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	XPngEncode*pEncode = (XPngEncode*)png_get_io_ptr(png_ptr);
	pEncode->m_pData->Append(data,length);
}*/


bool XJpegEncode::Encode(XDib&dib,XU8Array&data)
{
	m_pData = &data;

   struct jpeg_compress_struct jcs;

// 声明错误处理器，并赋值给jcs.err域
   struct jpeg_error_mgr jem;
   jcs.err = jpeg_std_error(&jem);

   jpeg_create_compress(&jcs);

   jpeg_stdio_dest(

   jpeg_stdio_buffer_dest(&jcs,this,WriteData);

   /* That's it */
   return true;
   //return (OK);
}