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

enum CanvasConfiguration
{
    BlackOnWhite,
    WhiteOnBlack
};

enum Mode
{
    Paint,
    Erase
};

class Painter {
public:
    Painter(
        unsigned char* image_buffer, 
        uint16_t image_buffer_size, 
        uint16_t width, uint16_t height, 
        CanvasConfiguration canvas_configuration = CanvasConfiguration::BlackOnWhite);
    void clear();
    void paint_all();
    void draw_pixel(Point point, Mode mode = Mode::Paint);
    void draw_character(Point point, unsigned char ascii_char, sFONT* font, Mode mode = Mode::Paint);
    void draw_string(Point point, const char* text, sFONT* font, Mode mode = Mode::Paint);
    void draw_line(Point point_1, Point point_2, Mode mode = Mode::Paint);
    void draw_rectangle(Point point_1, Point point_2, bool fill = false, Mode mode = Mode::Paint);
    unsigned char* get_image_buffer();

private:
    Point transform(Point point);
    void draw_horizontal_line(Point point_1, Point point_2, Mode mode = Mode::Paint);
    void draw_vertical_line(Point point_1, Point point_2, Mode mode = Mode::Paint);
    unsigned char* image_buffer;
    uint16_t image_buffer_size;
    uint16_t width;
    uint16_t height;
    CanvasConfiguration canvas_configuration;
};

#endif // PAINTER_H

