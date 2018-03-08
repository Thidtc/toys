#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "surface.h"

void *BmpSurface::get_buffer() const {
    return buffer_;
}

void BmpSurface::set_pixel(size_t x, size_t y, int color) {
    assert(is_coord_valid(x, y));
    buffer_[(x + y * width_) * 3 + 0] = (color) & 0xFF; // Blue
    buffer_[(x + y * width_) * 3 + 1] = (color >> 8) & 0xFF; // Green
    buffer_[(x + y * width_) * 3 + 2] = (color >> 16) & 0xFF; // Red
}

void BmpSurface::store(const std::string &filename) const {
    store_bmp(filename, width_, height_, buffer_);
}

void BmpSurface::store_bmp(const std::string &filename, size_t width,
    size_t height, unsigned char *buffer) const{
    #pragma pack(push)
    #pragma pack(2)
    typedef struct {  
        unsigned short    bfType;  
        unsigned int   bfSize;  
        unsigned short    bfReserved1;  
        unsigned short    bfReserved2;  
        unsigned int   bfOffBits;  
    } BITMAPFILEHEADER;  
    
    typedef struct {  
        unsigned int      biSize;  
        int       biWidth;  
        int       biHeight;  
        unsigned short       biPlanes;  
        unsigned short       biBitCount;  
        unsigned int      biCompression;  
        unsigned int      biSizeImage;  
        int       biXPelsPerMeter;  
        int       biYPelsPerMeter;  
        unsigned int      biClrUsed;  
        unsigned int      biClrImportant;  
    } BITMAPINFOHEADER;
    #pragma pack(pop)

    const size_t size = width * height * 3;

    // Create Bitmap File Header  
    BITMAPFILEHEADER fileHeader;  
  
    fileHeader.bfType = 0x4D42;  
    fileHeader.bfReserved1 = 0;  
    fileHeader.bfReserved2 = 0;  
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;  
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
  
    // Create Bitmap Info Header  
    BITMAPINFOHEADER bitmapHeader = { 0 };  
  
    bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);  
    bitmapHeader.biHeight = height;  
    bitmapHeader.biWidth = width;  
    bitmapHeader.biPlanes = 1;  
    bitmapHeader.biBitCount = 24;  
    bitmapHeader.biSizeImage = size;  
    bitmapHeader.biCompression = 0; //BI_RGB  
  
  
    // Write to file  
    FILE *output = fopen(filename.c_str(), "wb");
    if (output == NULL) {
        std::cerr << "Failed to open file" + filename;
    } else {
        fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
        fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, output);
        fwrite(buffer, size, 1, output);
        fclose(output);
    }
}