#ifndef COMMON_STRUCTS_HPP
#define COMMON_STRUCTS_HPP

struct DisplayMode
{
    int width = 0;
    int height = 0;
    int bpp = 0;
    int refreshRate = 0;
    DisplayMode(int w = 0, int h = 0, int b = 32, int r = 0):
        width(w), height(h), bpp(b), refreshRate(r) {}
};

#endif // COMMON_STRUCTS_HPP
