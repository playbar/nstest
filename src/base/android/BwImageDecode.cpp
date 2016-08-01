#include "BwImageDecode.h"


unsigned char* BwImageDecode::LoadFile(char *filename, long &fSize)
{
    FILE *fp = 0;
    unsigned char * fData = 0;

    fp = fopen (filename, "rb");
    if (fp != 0)
    {
    	fseek(fp , 0 , SEEK_END);
    	fSize = ftell(fp);
    	rewind(fp);

    	fData = (unsigned char *)malloc(fSize);
        if (fread(fData, 1, fSize, fp) != fSize)
        {
        	free(fData);
        	fData = 0;
        }
        fclose(fp);
    }

    return fData;
}
int BwImageDecode::checkFormatMem(unsigned char* fData)
{
	//png
	if (!png_sig_cmp((png_byte*)fData, (png_size_t)0, 4))
	{
		return EN_IMG_TYIE_PNG;
	}
    // gif
	if (memcmp(GIF_STAMP,   fData, GIF_STAMP_LEN) == 0 ||
			memcmp(GIF87_STAMP, fData, GIF_STAMP_LEN) == 0 ||
			memcmp(GIF89_STAMP, fData, GIF_STAMP_LEN) == 0)
	{
		return EN_IMG_TYIE_GIF;
	}

    // jpg
	char gHeader[] = { 0xFF, 0xD8, 0xFF };
	if (memcmp(fData, gHeader, 3) == 0)
	{
		return EN_IMG_TYIE_JPG;
	}
    // bmp
	char kBmpMagic[] = { 'B', 'M' };
	if (memcmp(fData, kBmpMagic, 2) == 0)
	{
		return EN_IMG_TYIE_BMP;
	}
    return EN_IMG_TYPE_NONE;
}
unsigned char* BwImageDecode::DecodeFile(char *filename, int& width, int& height, int index)
{
	long fSize = 0;
	unsigned char* fData = LoadFile(filename,fSize);
	unsigned char* ret = 0;

	ret = DecodeMemory(fData, fSize, width, height, index);

	free(fData);

	return ret;
}
unsigned char* BwImageDecode::DecodeMemory(unsigned char* fData, long fSize, int& width, int& height, int index)
{
	unsigned char* ret = 0;
	int format = checkFormatMem(fData);
	switch (format)
	{
	case EN_IMG_TYIE_PNG: // Png
		ret = DecodeMemoryPNG(fData, fSize, width, height);
		break;
	case EN_IMG_TYIE_GIF: // Gif
		ret = DecodeMemoryGIF(fData, fSize, width, height, index);
		break;
	case EN_IMG_TYIE_JPG: // Jpg
		ret = DecodeMemoryJPG(fData, fSize, width, height);
		break;
	case EN_IMG_TYIE_BMP: // Bmp
		break;
	default:
		break;
	}
	return ret;
}
typedef struct
{
	unsigned char* data;
	int size;
	int offset;
}ImageSource;
static void memReadFuncPng(png_structp png_ptr, png_bytep data, png_size_t length)
{
	ImageSource* isource = (ImageSource*) png_get_io_ptr(png_ptr);
	if (isource->offset + length <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

unsigned char* BwImageDecode::DecodeMemoryPNG(unsigned char* fData, long fSize, int& width, int& height)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int bit_depth, color_type;
    png_bytep *row_pointers = NULL;
    unsigned char* image_data;
    int rowbytes;

    /* Create a png read struct */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        return NULL;
    }

   /* Create a png info struct */
    info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct (&png_ptr, NULL, NULL);
        return NULL;
    }

    /* Initialize the setjmp for returning properly after a libpng error occured */
    if (setjmp (png_jmpbuf (png_ptr)))
    {
        png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

        if (row_pointers)
            free (row_pointers);

        return NULL;
    }

    ImageSource imgsource;
    imgsource.data = fData;
    imgsource.size = fSize;
    imgsource.offset = 0;
    png_set_read_fn(png_ptr, &imgsource, memReadFuncPng);

    /* Read png info */
    png_read_info (png_ptr, info_ptr);

    /* Get some usefull information from header */
    bit_depth = png_get_bit_depth (png_ptr, info_ptr);
    color_type = png_get_color_type (png_ptr, info_ptr);

    /* Convert index color images to RGB images */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb (png_ptr);

    /* Convert RGB images to RGBA images */
    if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    /* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_gray_1_2_4_to_8 (png_ptr);

    if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha (png_ptr);

    if (bit_depth == 16)
        png_set_strip_16 (png_ptr);
    else if (bit_depth < 8)
        png_set_packing (png_ptr);

    /* Update info structure to apply transformations */
    png_read_update_info (png_ptr, info_ptr);

    /* Retrieve updated information */
    png_get_IHDR (png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type, NULL, NULL, NULL);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if ((image_data =(unsigned char *) malloc(height * rowbytes)) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }

    /* Setup a pointer array.  Each one points at the begening of a row. */
    if ((row_pointers =(png_bytepp) malloc(height * sizeof(png_bytep))) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        return NULL;
    }
    for (int i = 0; i < height; i++)
        row_pointers[height - 1 - i] = image_data + i*rowbytes;

    /* Read pixel data using row pointers */
    png_read_image (png_ptr, row_pointers);

    /* Finish decompression and release memory */
    png_read_end (png_ptr, NULL);
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

    /* We don't need row pointers anymore */
    free (row_pointers);

    return image_data;
}

struct bw_jpg_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

METHODDEF(void) bw_jpg_error_exit( j_common_ptr cinfo )
{
	LOGWHERE();
	bw_jpg_error_mgr *myerr = (bw_jpg_error_mgr*)cinfo->err;
	(cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer, 1 );
}

unsigned char* BwImageDecode::DecodeMemoryJPG(unsigned char* fData, long fSize, int& width, int& height)
{
    unsigned char* image_data;
	struct jpeg_decompress_struct cinfo;
	struct bw_jpg_error_mgr jerr;
	unsigned char* buffer;
	int row_stride;
	int out_color_components = 4;

	cinfo.err = jpeg_std_error(&jerr.pub );
	jerr.pub.error_exit = bw_jpg_error_exit;
	if( setjmp( jerr.setjmp_buffer ) )
	{
		LOGWHERE();
		jpeg_destroy_decompress( &cinfo );
		return NULL;
	}
	jpeg_create_decompress(&cinfo);

	//从内存读取
	jpeg_stdio_buffer_src(&cinfo, fData, fSize);
	jpeg_read_header(&cinfo, TRUE);
//	cinfo.out_color_components = 3;
//	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (unsigned char*)malloc(row_stride);
	image_data = (unsigned char*)malloc(cinfo.output_width * cinfo.output_height * out_color_components);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char * p = image_data + (cinfo.output_width * out_color_components * cinfo.output_scanline);
		jpeg_read_scanlines(&cinfo, &buffer, 1);
		for(int i=0; i < width; i++)
		{
			p[4*i  ] = buffer[3*i];
			p[4*i+1] = buffer[3*i+1];
			p[4*i+2] = buffer[3*i+2];
			p[4*i+3] = 0xff;
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_abort_decompress( &cinfo );
	jpeg_destroy_decompress(&cinfo);
	free(buffer);
	return image_data;
}

static int memReadFuncGif(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)(GifFile->UserData);
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}

unsigned char* BwImageDecode::DecodeMemoryGIF(unsigned char* fData, long fSize, int& width, int& height, int index)
{
	GifFileType *GifFile;

	if ((GifFile = DGifOpen(fData, memReadFuncGif)) == NULL)
	{
	   return 0;
	}

	if (DGifSlurp(GifFile) == 0)
	{
	   return 0;
	}

	unsigned char *image_data = 0, *src = 0, *dst = 0;
	ColorMapObject *ColorMap;
	SavedImage *saveImg;
	GifRowType *ScreenBuffer;
    GifColorType *ColorMapEntry;
    int loc = 0;

    if (index >= GifFile->ImageCount)
    {
    	return 0;
    }
//    index = index % GifFile->ImageCount;

	ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
    saveImg = &(GifFile->SavedImages[index]);
    ScreenBuffer = &(saveImg->RasterBits);

    if ((image_data = (unsigned char *) malloc(GifFile->SWidth * 4 * GifFile->SHeight)) == NULL)
    {
    	DGifCloseFile(GifFile);
    	return 0;
    }
    src = saveImg->RasterBits;
    dst = image_data;

	for (int i = 0; i < GifFile->SHeight; i++)
    {
		loc = GifFile->SWidth * i;
        for (int j = 0; j < GifFile->SWidth; j++)
        {
            ColorMapEntry = &(ColorMap->Colors[*(src + loc + j)]);
            *dst++ = ColorMapEntry->Red;
            *dst++ = ColorMapEntry->Green;
            *dst++ = ColorMapEntry->Blue;
            *dst++ = 255;
        }
    }

	width = GifFile->SWidth;
	height = GifFile->SHeight;

//	{
//		m_ImageCount = GifFile->ImageCount;
//		m_ImageWidth = GifFile->SWidth;
//		m_ImageHeight = GifFile->SHeight;
//		m_GifFile = GifFile;
//		m_ImageData = image_data;
//	}

	DGifCloseFile(GifFile);
    return image_data;
}
void* BwImageDecode::DecodeMemoryGIFImage(unsigned char* fData, long fSize, int& width, int& height)
{
	GifFileType *GifFile;

	if ((GifFile = DGifOpen(fData, memReadFuncGif)) == NULL)
	{
	   return 0;
	}

	if (DGifSlurp(GifFile) == 0)
	{
	   return 0;
	}

	width = GifFile->SWidth;
	height = GifFile->SHeight;
	return GifFile;
}
unsigned char* BwImageDecode::getGifImage(void* gifFile, int index)
{
	if (gifFile == 0) return 0;

	GifFileType *GifFile = (GifFileType *)gifFile;

	unsigned char *image_data = 0, *src = 0, *dst = 0;
	ColorMapObject *ColorMap;
	SavedImage *saveImg;
	GifRowType *ScreenBuffer;
    GifColorType *ColorMapEntry;
    int loc = 0;

    if (index >= GifFile->ImageCount)
    {
    	return 0;
    }
    index = index % GifFile->ImageCount;

	ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
    saveImg = &(GifFile->SavedImages[index]);
    ScreenBuffer = &(saveImg->RasterBits);

    if ((image_data = (unsigned char *) malloc(GifFile->SWidth * 4 * GifFile->SHeight)) == NULL)
    {
    	DGifCloseFile(GifFile);
    	return 0;
    }
    src = saveImg->RasterBits;
    dst = image_data;

	for (int i = 0; i < GifFile->SHeight; i++)
    {
		loc = GifFile->SWidth * i;
        for (int j = 0; j < GifFile->SWidth; j++)
        {
            ColorMapEntry = &(ColorMap->Colors[*(src + loc + j)]);
            *dst++ = ColorMapEntry->Red;
            *dst++ = ColorMapEntry->Green;
            *dst++ = ColorMapEntry->Blue;
            *dst++ = 255;
        }
    }

//	LOGE("getGifImage  --  %d",image_data);
    return image_data;
}
char *BwImageDecode::getGIFDuration(void* gifFile)
{
	if (gifFile == 0) return 0;

	GifFileType *GifFile = (GifFileType *)gifFile;

	char * duration = (char*)malloc(GifFile->ImageCount);//new char[GifFile->ImageCount];

    for (int i = 0; i < GifFile->ImageCount; i++)
    {
    	duration[i] = savedimage_duration(&GifFile->SavedImages[i]);
    }
    return duration;
}
int BwImageDecode::savedimage_duration(const SavedImage* image)
{
    for (int j = 0; j < image->ExtensionBlockCount; j++)
    {
        if (image->ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
        {
            int size = image->ExtensionBlocks[j].ByteCount;
            const uint8_t* b = (const uint8_t*)image->ExtensionBlocks[j].Bytes;
            return ((b[2] << 8) | b[1]) * 10;
        }
    }
    return 0;
}
