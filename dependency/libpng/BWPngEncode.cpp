#include "StdAfxGGBase.h"
#include "BWPngEncode.h"

#include "png.h"

XPngEncode::XPngEncode()
{

}

XPngEncode::~XPngEncode()
{
	Release();
}

void XPngEncode::Release()
{
}

static void WriteData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	XPngEncode*pEncode = (XPngEncode*)png_get_io_ptr(png_ptr);
	pEncode->m_pData->Append(data,length);
}

static void FlushData(png_structp png_ptr)
{

}

#define entire

bool XPngEncode::Encode(XDib&dib,XU8Array&data)
{
	m_pData = &data;
	png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;
    //png_bytep *row_pointers = NULL;
    //unsigned char* image_data;
    //int rowbytes;

    /* Create a png read struct */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        return false;
    }

   /* Create a png info struct */
    info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr)
    {
        //png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
        return false;
    }

    /* Initialize the setjmp for returning properly after a libpng error occured */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
      /* If we get here, we had a problem writing the file */    
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return false;
    } 

   /* One of the following I/O initialization functions is REQUIRED */

   png_set_write_fn(png_ptr, (void *)this, WriteData,
      FlushData);
   /* where user_io_ptr is a structure you want available to the callbacks */
   png_uint_32 width = dib.Width();
   png_uint_32 height = dib.Height();
   int bit_depth = 8;//dib.GetBits();

#ifdef hilevel
   /* This is the easy way.  Use it if you already have all the
    * image info living in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
   png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);

#else
   /* This is the hard way */

   /* Set the image information here.  Width and height are up to 2^31,
    * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
    */
   png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   /* Set the palette if there is one.  REQUIRED for indexed-color images */
   palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
             * png_sizeof(png_color));
   /* ... Set palette colors ... */
   png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
   /* You must not free palette here, because png_set_PLTE only makes a link to
    * the palette that you malloced.  Wait until you are about to destroy
    * the png structure.
    */

   /* Optional significant bit (sBIT) chunk */
   png_color_8 sig_bit;
   /* If we are dealing with a grayscale image then */
   sig_bit.gray = 8;//true_bit_depth;
   /* Otherwise, if we are dealing with a color image then */
   sig_bit.red = 8;//true_red_bit_depth;
   sig_bit.green = 8;//true_green_bit_depth;
   sig_bit.blue = 8;//true_blue_bit_depth;
   /* If the image has an alpha channel then */
   sig_bit.alpha = 8;//true_alpha_bit_depth;
   png_set_sBIT(png_ptr, info_ptr, &sig_bit);


   /* Optional gamma chunk is strongly suggested if you have any guess
    * as to the correct gamma of the image.
    */
   //png_set_gAMA(png_ptr, info_ptr, gamma);

   /* Optionally write comments into the image */
   png_text text_ptr[3];
   text_ptr[0].key = "Title";
   text_ptr[0].text = "Mona Lisa";
   text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[1].key = "Author";
   text_ptr[1].text = "Baiwan";
   text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[2].key = "Description";
   text_ptr[2].text = "BW Png Image";
   text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;

   png_set_text(png_ptr, info_ptr, text_ptr, 3);

   /* Other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs */

   /* Note that if sRGB is present the gAMA and cHRM chunks must be ignored
    * on read and, if your application chooses to write them, they must
    * be written in accordance with the sRGB profile
    */

   /* Write the file header information.  REQUIRED */
   png_write_info(png_ptr, info_ptr);

   /* If you want, you can write the info in two steps, in case you need to
    * write your private chunk ahead of PLTE:
    *
    *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
    *   write_my_chunk();
    *   png_write_info(png_ptr, info_ptr);
    *
    * However, given the level of known- and unknown-chunk support in 1.2.0
    * and up, this should no longer be necessary.
    */

   /* Once we write out the header, the compression type on the text
    * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
    * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
    * at the end.
    */

   /* Set up the transformations you want.  Note that these are
    * all optional.  Only call them if you want them.
    */

   /* Invert monochrome pixels */
   //png_set_invert_mono(png_ptr);

   /* Shift the pixels up to a legal bit depth and fill in
    * as appropriate to correctly scale the image.
    */
   //png_set_shift(png_ptr, &sig_bit);

   /* Pack pixels into bytes */
   //png_set_packing(png_ptr);

   /* Swap location of alpha bytes from ARGB to RGBA */
   //png_set_swap_alpha(png_ptr);

   /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
    * RGB (4 channels -> 3 channels). The second parameter is not used.
    */
   //png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

   /* Flip BGR pixels to RGB */
   //If Android, Modify here...
#ifndef _ARGB_PIXEL_
   png_set_bgr(png_ptr);
#endif
   /* Swap bytes of 16-bit files to most significant byte first */
  // png_set_swap(png_ptr);

   /* Swap bits of 1, 2, 4 bit packed pixel formats */
  // png_set_packswap(png_ptr);

   /* Turn on interlace handling if you are not using png_write_image() */
   int number_passes;
   //if (interlacing)
      number_passes = png_set_interlace_handling(png_ptr);
  // else
    //  number_passes = 1;

   /* The easiest way to write the image (you may have a different memory
    * layout, however, so choose what fits your needs best).  You need to
    * use the first method if you aren't handling interlacing yourself.
    */
   png_uint_32 k;//, height, width;
   png_bytep image=(png_bytep)dib.GetAddress();//[height][width*bytes_per_pixel];
   png_bytepp row_pointers=new png_bytep[height];

   if (height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
     png_error (png_ptr, "Image is too tall to process in memory");

   png_bytep base=image;
   for (k = 0; k < height; k++)
   {
     row_pointers[k] = base;//image + k*width*bytes_per_pixel;
	 base+= dib.GetLinePitch();
   }

   /* One of the following output methods is REQUIRED */

#ifdef entire /* Write out the entire image data in one call */
   png_write_image(png_ptr, row_pointers);

   /* The other way to write the image - deal with interlacing */

#else no_entire /* Write out the image data by one or more scanlines */

   /* The number of passes is either 1 for non-interlaced images,
    * or 7 for interlaced images.
    */
   int pass;
   for (pass = 0; pass < number_passes; pass++)
   {
      /* Write a few rows at a time. */
      png_write_rows(png_ptr, &row_pointers[first_row], number_of_rows);

      /* If you are only writing one row at a time, this works */
      for (y = 0; y < height; y++)
         png_write_rows(png_ptr, &row_pointers[y], 1);
   }
#endif no_entire /* Use only one output method */

   /* You can write optional chunks like tEXt, zTXt, and tIME at the end
    * as well.  Shouldn't be necessary in 1.2.0 and up as all the public
    * chunks are supported and you can use png_set_unknown_chunks() to
    * register unknown chunks into the info structure to be written out.
    */

   /* It is REQUIRED to call this to finish writing the rest of the file */
   png_write_end(png_ptr, info_ptr);
#endif hilevel

   /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
    * as recommended in versions 1.0.5m and earlier of this example; if
    * libpng mallocs info_ptr->palette, libpng will free it).  If you
    * allocated it with malloc() instead of png_malloc(), use free() instead
    * of png_free().
    */
   png_free(png_ptr, palette);
   palette = NULL;

   /* Similarly, if you png_malloced any data that you passed in with
    * png_set_something(), such as a hist or trans array, free it here,
    * when you can be sure that libpng is through with it.
    */
  // png_free(png_ptr, trans);
  // trans = NULL;
   /* Whenever you use png_free() it is a good idea to set the pointer to
    * NULL in case your application inadvertently tries to png_free() it
    * again.  When png_free() sees a NULL it returns without action, thus
    * avoiding the double-free security problem.
    */

   /* Clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   delete row_pointers;
   /* Close the file */
   //fclose(fp);

   /* That's it */
   return true;
   //return (OK);
}