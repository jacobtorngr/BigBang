#ifndef __TGA_H__
#define __TGA_H__

/*
 *  tga.h
 *
 *	Loading and saving of TGA files (minimal implementation).
 *  Created by Fredrik Sandin on 2012-08-20.
 *  Update by Sandeep Patil on 2020-09-26
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/*!
	Definition of TGA-header specific constants.
*/
enum TGA_TYPE {

    /* Uncompressed formats */
    TGA_UNCOMPRESSED_PALETTE = 1,
    TGA_UNCOMPRESSED_RGB = 2,
    TGA_UNCOMPRESSED_MONOCHROME = 3,

    /* Run-length encoded formats */
    TGA_RLE_PALETTE = 9,
    TGA_RLE_RGB = 10,
    TGA_RLE_MONOCHROME = 11,

    /* Reversed ordering */
    TGA_FLIP_MASK = 0x30
};

/*!
	Definition of return and status codes.
*/
enum TGA_STATUS {
    TGA_OK = 0,
    TGA_UNSUPPORTED_FORMAT = -99,
    TGA_FILE_OPEN_FAILED,
    TGA_FILE_READ_FAILED,
    TGA_FILE_WRITE_FAILED,
    TGA_MALLOC_FAILED
};

#pragma pack(1)

/*!
	Definition of TGA header structure. For details, see:
	http://en.wikipedia.org/wiki/Truevision_TGA
*/
typedef struct {
    uint8_t  idLen;
    uint8_t  colorMapType;
    uint8_t  imageType;
    uint16_t colorMapStart;
    uint16_t colorMapLength;
    uint8_t  colorMapDepth;
    uint16_t xOffset;
    uint16_t yOffset;
    uint16_t width;
    uint16_t height;
    uint8_t  pixelDepth;
    uint8_t  imageDescriptor;
} TGA_HEADER;

/*!
	24-bit BGR pixel (TGA ordering of colors).
*/
typedef struct {
    uint8_t B; /* Blue Colour components of pixel */
    uint8_t G; /* Green Colour components of pixel */
    uint8_t R; /* Red Colour components of pixel */
} PIXEL_RGB24;

#pragma pack()

/* Function definitions, to be implemented in tga.c */
void copy_pixel(PIXEL_RGB24 *dst, const PIXEL_RGB24 *src);

int tga_read(const char *file_name, int *width, int *height, PIXEL_RGB24 **pixelBuf);

int tga_write(const char *file_name, int width, int height, PIXEL_RGB24 *pixelBuf, int bits);

#endif /* __TGA_H__ */
