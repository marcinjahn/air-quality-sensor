#ifndef PAINTER_H
#define PAINTER_H

#include "fonts.h"

struct Point {
    Point(uint16_t x, uint16_t y)
    {
        this->x = x;
        this->y = y;
    }

    uint16_t x;
    uint16_t y;
};

class Painter {
public:
    Painter(unsigned char* image, int width, int height, bool invert_colors = false);
    void clear(bool colored = true);
    void draw_pixel(Point point, bool colored = true);
    void draw_character(Point point, char ascii_char, sFONT* font, bool colored = true);
    void draw_string(Point point, const char* text, sFONT* font, bool colored = true);
    void draw_line(Point point_1, Point point_2, bool colored = true);
    void draw_rectangle(Point point_1, Point point_2, bool fill = false, bool colored = true);

private:
    Point transform(Point point);
    unsigned char* image;
    int width;
    int height;
    bool inverted_colors;
};

#endif // PAINTER_H

