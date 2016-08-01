#include "FontBuffer.h"

//FontBuffer gFlashFonts[MAX_FONTS];


int InsertFont(FontBuffer*fonts, unsigned short charindex, int delpos )
{
	int pos = -1;
	int i = 0;
	for( i = 0; i < MAX_FONTS; i++ )  //查找插入位置
	{
		if( fonts[i].usecount == 0 )
		{
			return i;
		}
		if( charindex < fonts[i].index )
		{
			pos = i;
			break;
		}
	}
	//int delpos = DeleteOneFont();
	if( pos > delpos )  //移动位置
	{
		for( i = delpos; i < pos; i++ )
		{
			fonts[i] = fonts[ i + 1 ];
		}
	}
	if( pos < delpos )
	{
		for( i = delpos; i > pos; i-- )
		{
			fonts[i] = fonts[ i - 1 ];
		}
	}
	return pos;
}

bool InitFontBuffer(FontBuffer*fonts)
{
	for( int i = 0; i < MAX_FONTS; i++ )
	{
		fonts[i].bitmap = new FT_Bitmap();
	}
	return true;
}

bool ReleaseFontBuffer(FontBuffer*fonts)
{
	for( int i = 0; i < MAX_FONTS; i++ )
	{
		delete fonts[i].bitmap;
		fonts[i].bitmap = NULL;
		fonts[ i ].index = 0;
		fonts[ i ].usecount = 0;
		fonts[ i ].left = 0;
		fonts[ i ].top = 0;
		fonts[ i ].width = 0;
		fonts[ i ].fontStyle = 0;
	}
	return true;
}

int DeleteOneFont(FontBuffer*fonts)			//删除最少使用字体
{
	int i = 0;
	int delpos = 0;
	for( i = 0; i < MAX_FONTS; i++ )
	{
		if( fonts[i].usecount == 0 )
		{
			delpos = i;
			break;
		}
		if( fonts[i].usecount < fonts[delpos].usecount )
		{
			delpos = i;
		}
	}
	fonts[ delpos ].index = 0;
	fonts[ delpos ].usecount = 0;
	fonts[ delpos ].left = 0;
	fonts[ delpos ].top = 0;
	fonts[ delpos ].width = 0;
	fonts[ delpos ].fontStyle = 0;
	return delpos;
}

int FindFontIndex( FontBuffer*fonts, unsigned short charindex )
{
	int left, right, middle;
	left = 0, right = MAX_FONTS - 1;

	while (left <= right)
	{
		middle = (left + right) / 2;
		//LOGE( "middle = %d", middle );
		if( fonts[middle].index == 0 )
		{
			right = middle - 1;
		}
		else if (fonts[middle].index > charindex)
		{
			right = middle - 1;
		}
		else if (fonts[middle].index < charindex)
		{
			left = middle + 1;
		}
		else
		{
			return middle;
		}
	}
	return -1;
}
