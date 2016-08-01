#ifndef __BW_IMAGE_DECODE__
#define __BW_IMAGE_DECODE__

#include "png.h"
#include "gif_lib.h"
#include "jpeglib.h"

enum EnImgType
{
	EN_IMG_TYPE_NONE = 0,
	EN_IMG_TYIE_PNG,
	EN_IMG_TYIE_GIF,
	EN_IMG_TYIE_JPG,
	EN_IMG_TYIE_BMP
};

class BwImageDecode
{
public:
	BwImageDecode(){};
	~BwImageDecode(){};

	// 从文件中进行解码
	unsigned char* DecodeFile(char *filename, int& width, int& height, int index = 0);
	// 从内存中解码，对于GIF，根据index获得其中的某个图片
	unsigned char* DecodeMemory(unsigned char* fData, long fSize, int& width, int& height, int index = 0);
//	unsigned char* DecodeStream(void *stream, int& width, int& height, int index = 0);
	// 对于GIF，先对整个图形解码，然后在获得其中的某个图片
	void* DecodeMemoryGIFImage(unsigned char* fData, long fSize, int& width, int& height);

	// 将文件载入内存
	unsigned char* LoadFile(char *filename, long &fSize);
	// 对内存中的PNG图片进行解码
	unsigned char* DecodeMemoryPNG(unsigned char* fData, long fSize, int& width, int& height);
	// 对内存中的JPG图片进行解码
	unsigned char* DecodeMemoryJPG(unsigned char* fData, long fSize, int& width, int& height);
	// 对内存中的GIF图片进行解码
	unsigned char* DecodeMemoryGIF(unsigned char* fData, long fSize, int& width, int& height, int index = 0);
	// 获得GIF中的某个图片
	unsigned char* getGifImage(void* gifFile, int index = 0);

	// 根据内存中的数据，检查图片的种类
	int checkFormatMem(unsigned char* fData);

	// 获得GIF多个图片之间的时间间隔
	char *getGIFDuration(void* gifFile);

private:
//	int m_ImageCount;
//	int m_ImageIndex;
//	int m_ImageWidth;
//	int m_ImageHeight;
//	void *m_GifFile;
//	void *m_ImageData;


	int savedimage_duration(const SavedImage* image);
};













#endif // end define __BW_IMAGE_DECODE__
