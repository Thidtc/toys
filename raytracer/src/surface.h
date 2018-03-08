#ifndef SURFACE_H_
#define SURFACE_H_

#include <string>
#include <cstddef>
#include <cassert>
#include <cfloat>

// Surface is the buffer for the target rendering picture
class Surface {
public:
    Surface(const std::string &filename, size_t width, size_t height) :
        filename_(filename),
        width_(width),
        height_(height) {}
    virtual ~Surface() {}
    virtual void *get_buffer() const = 0;
    virtual void set_pixel(size_t x, size_t y, int color) {};
    virtual void store(const std::string &filename) const = 0;
    virtual void resize(size_t width, size_t height) {};
    virtual void clear() {};
    inline std::string get_filename() const { return filename_; }
    inline size_t get_width() const { return width_; }
    inline size_t get_height() const { return height_; }
    inline void store() { store(filename_); }
protected:
    inline bool is_coord_valid(size_t x, size_t y) {
        return x < width_ && y < height_;
    }
protected:
    std::string filename_;
    size_t width_;
    size_t height_;
};

class BmpSurface : public Surface {
public:
    typedef struct {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    } Pixel;
public:
    BmpSurface(const std::string &filename, size_t width, size_t height) :
        Surface(filename, width, height) {
        buffer_ = new unsigned char[width * height * 3];
    } 
    ~BmpSurface() {
        delete[] buffer_;
    }
    void *get_buffer() const;
    void set_pixel(size_t x, size_t y, int color);
    void store(const std::string &filename) const;
private:
    void store_bmp(const std::string &filename, size_t width,
        size_t height, unsigned char *buffer) const;

private:
    unsigned char *buffer_;
};

#endif // SURFACE_H_