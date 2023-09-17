/*
 *  tga.c
 *
 *	Loading and saving of TGA files (limited implementation).
 *  Created by Fredrik Sandin on 2012-08-20.
 *  Update by Sandeep Patil on 2020-09-26
 *
 */

#include "tga.h"
#include <string.h>

void copy_pixel(PIXEL_RGB24 *dst, const PIXEL_RGB24 *src) {

    *dst = *src;

}

/*!
	Read TGA image from file.

	\param file_name File name of TGA image to load (input).
	\param width Width of TGA image if file is successfully read (output).
	\param height Height of TGA image if file is successfully read (output).
	\param pixels Array of pixels read. Allocated with malloc(), user should free this memory (output).

	\return Return codes are defined in TGA_STATUS.
*/
int tga_read(const char *file_name, int *width, int *height, PIXEL_RGB24 **pixelBuf) {

    uint16_t p16;
    int numPixels;
    int i, status = TGA_OK;

    TGA_HEADER head;
    PIXEL_RGB24 *image;

    (*pixelBuf) = 0;

    FILE *in = fopen(file_name, "rb");
    if (!in) return TGA_FILE_OPEN_FAILED;


    /*
        Comment

        The goto keyword is useful to handle errors
        and to escape from multiple nested loops.
        In general it should be avoided, motivation:
        http://en.wikipedia.org/wiki/Spaghetti_code
        Exceptions are commonly used to handle errors
        in professionally written code. Exceptions
        is part of the C++ standard and can be emulated
        in C with macros and the functions setjmp() and
        longjmp() defined in setjmp.h. Introduction:
        http://en.wikipedia.org/wiki/Exception_handling
    */

    if (fread((void *) &head, sizeof(TGA_HEADER), 1, in) != 1) {
        status = TGA_FILE_READ_FAILED;
        goto error;
    }

    (*width) = head.width;
    (*height) = head.height;
    numPixels = (*width) * (*height);

    /* Skip ID string (name of creator, time of creation etc) */
    fseek(in, head.idLen, SEEK_CUR);

    /* Skip palette, presently not supported */
    fseek(in, head.colorMapLength * (head.colorMapDepth / 8), SEEK_CUR);

    /* Check if image format is supported */
    if (head.xOffset != 0 ||
        head.yOffset != 0 ) // ||
       // (head.imageDescriptor & TGA_FLIP_MASK) == 0)
        {
        status = TGA_UNSUPPORTED_FORMAT;
        goto error;
    }

    /* Allocate memory for pixels */
    (*pixelBuf) = (PIXEL_RGB24 *) malloc(sizeof(PIXEL_RGB24) * numPixels);
    if ((*pixelBuf) == 0) {
        status = TGA_MALLOC_FAILED;
        goto error;
    }
    image = (*pixelBuf);

    /* Parse each supported format as separate cases */
    switch (head.imageType) {

        case TGA_UNCOMPRESSED_RGB:

            switch (head.pixelDepth) {

                /* Read 24-bit RGB pixels.               */
                /*****************************************/
                case 24:

                    if (fread(image, sizeof(PIXEL_RGB24) * numPixels, 1, in) != 1)
                        status = TGA_FILE_READ_FAILED;
                    break;

                    /* Read 16-bit RGB pixels.               */
                    /*****************************************/
                case 16:
                    for (i = 0; i < numPixels; i++) {
                        if (fread(&p16, 2, 1, in) != 1) {
                            status = TGA_FILE_READ_FAILED;
                            goto error;
                        }
                        image[i].R = ((p16 >> 10) & 0x1F) << 3;
                        image[i].G = ((p16 >> 5) & 0x1F) << 3;
                        image[i].B = ((p16) & 0x1F) << 3;
                    }
                    break;

                    /*****************************************/
                    /* DEFINE MORE RGB(A) PIXEL FORMATS HERE */
                    /*****************************************/

                default:
                    status = TGA_UNSUPPORTED_FORMAT;
            }
            break;

        default:
            status = TGA_UNSUPPORTED_FORMAT;
    }

    if (status == TGA_OK)
        goto exit;

    error:
    if ((*pixelBuf) != 0) {
        free(*pixelBuf); /* Free allocated memory */
    }
    (*width) = (*height) = 0;
    (*pixelBuf) = 0;

    exit:
    fclose(in); /* Close input file */
    return status;
}

/*!
	Write image buffer to TGA file.

	\param file_name File name of TGA image to load (input).
	\param width Width of TGA image (input).
	\param height Height of TGA image (input).
	\param pixels Array of pixels (input).

	\return Return codes are defined in TGA_STATUS.
*/
int tga_write(const char *file_name, int width, int height, PIXEL_RGB24 *pixelBuf, int bits) {

    int i, size = width * height;

    FILE *out = fopen(file_name, "wb");
    if (!out) return TGA_FILE_OPEN_FAILED;

    TGA_HEADER head;
    memset(&head, 0, sizeof(TGA_HEADER)); /* Set all fields to zero */
    head.width = width;
    head.height = height;
    head.pixelDepth = bits;
    head.imageType = TGA_UNCOMPRESSED_RGB;

    /* Write header to file */
    if (fwrite((void *) &head, sizeof(TGA_HEADER), 1, out) != 1)
        goto write_error;

    /* Handle each pixel format as separate cases */
    switch (bits) {

        case 24: /* 24 bits per pixel */
            if (fwrite((void *) pixelBuf, sizeof(PIXEL_RGB24) * size, 1, out) != 1)
                goto write_error;
            break;

        case 16: /* 16 bits per pixel */
            for (i = 0; i < width * height; i++) {
                uint16_t p16 =
                        (((pixelBuf[i].R >> 3) & 0x1F) << 10) |
                        (((pixelBuf[i].G >> 3) & 0x1F) << 5) |
                        (((pixelBuf[i].B >> 3) & 0x1F));
                if (fwrite(&p16, 2, 1, out) != 1)
                    goto write_error;
            }
            break;

        default:
            goto format_error;
    }

    fclose(out);
    return TGA_OK;

/*
	Error handlers.
*/

    write_error:
    fclose(out);
    return TGA_FILE_WRITE_FAILED;

    format_error:
    fclose(out);
    return TGA_UNSUPPORTED_FORMAT;
}
