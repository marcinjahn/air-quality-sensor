#include <stdlib.h>
#include "utilities/graphics/painter.hpp"
#include "output/console.hpp"
#include "utilities/math.hpp"

Painter::Painter(
    unsigned char *image_buffer, 
    uint16_t image_buffer_size, 
    uint16_t width, 
    uint16_t height, 
    CanvasConfiguration canvas_configuration)
{
    this->image_buffer = image_buffer;
    this->image_buffer_size = image_buffer_size;
    this->width = width;
    this->height = height;
    this->canvas_configuration = canvas_configuration;
}

void Painter::clear()
{
    for (uint16_t x = 0; x < this->width; x++)
    {
        for (uint16_t y = 0; y < this->height; y++)
        {
            draw_pixel(Point(x, y), Mode::Erase);
        }
    }
}

void Painter::paint_all()
{
    for (uint16_t x = 0; x < this->width; x++)
    {
        for (uint16_t y = 0; y < this->height; y++)
        {
            draw_pixel(Point(x, y), Mode::Paint);
        }
    }
}

void Painter::draw_pixel(Point point, Mode mode)
{
    if (point.x >= this->width || point.y >= this->height)
    {
        return;
    }

    Point real_point = transform(point);

    uint16_t index = (real_point.y * this->height + real_point.x) / 8;

    if (index >= this->image_buffer_size)
    {
        return;
    }

    uint8_t bit_to_change = 128 >> (real_point.x % 8);

    image_buffer[index] = 
        (this->canvas_configuration == CanvasConfiguration::BlackOnWhite && mode == Mode::Paint) || 
        (this->canvas_configuration == CanvasConfiguration::WhiteOnBlack && mode == Mode::Erase) ? 
            image_buffer[index] | bit_to_change : 
            image_buffer[index] & ~bit_to_change;
}

Point Painter::transform(Point point)
{
    return Point(point.y, this->width - point.x - 1);
}

void Painter::draw_character(Point point, unsigned char ascii_character, sFONT *font, Mode mode)
{
    uint16_t i, j, char_offset;

    if (font->numbers_only)
    {
        if (ascii_character - '0' > 9)
        {
            // invalid input results in 0 being printed
            ascii_character = '0';
        }

        char_offset = (ascii_character - '0') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    }
    else
    {
        char_offset = (ascii_character - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    }
    

    const unsigned char *ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++)
    {
        for (i = 0; i < font->Width; i++)
        {
            if (pgm_read_byte(ptr) & (128 >> (i % 8)))
            {
                draw_pixel(Point(point.x + i, point.y + j), mode);
            }
            if (i % 8 == 7)
            {
                ptr++;
            }
        }

        if (font->Width % 8 != 0)
        {
            ptr++;
        }
    }
}

void Painter::draw_string(Point point, const char *text, sFONT *font, Mode mode)
{
    const char *char_pointer = text;
    int current_x = point.x;

    while (*char_pointer != 0)
    {
        draw_character(Point(current_x, point.y), *char_pointer, font, mode);
        current_x += font->Width;
        char_pointer++;
    }
}

void swap(Point* point_1, Point* point_2)
{
    Point temp = *point_1;
    *point_1 = *point_2;
    *point_2 = temp;
}

bool is_horizontal(Point point_1, Point point_2)
{
    return point_1.y == point_2.y;
}

bool is_vertical(Point point_1, Point point_2)
{
    return point_1.x == point_2.x;
}

void Painter::draw_horizontal_line(Point point_1, Point point_2, Mode mode)
{
    for(uint16_t i = Math::min(point_1.x, point_2.x); i <= Math::max(point_1.x, point_2.x); i++)
    {
        draw_pixel(Point(i, point_1.y), mode);
    }
}

void Painter::draw_vertical_line(Point point_1, Point point_2, Mode mode)
{
    for(uint16_t i = Math::min(point_1.y, point_2.y); i <= Math::max(point_1.y, point_2.y); i++)
    {
        draw_pixel(Point(point_1.x, i), mode);
    }
}

void Painter::draw_line(Point point_1, Point point_2, Mode mode)
{
    if (point_1.x > point_2.x || (point_1.x == point_2.x && point_1.y > point_2.y))
    {
        swap(&point_1, &point_2);
    }

    if (is_horizontal(point_1, point_2))
    {
        return draw_horizontal_line(point_1, point_2, mode);
    }
    else if (is_vertical(point_1, point_2))
    {
        return draw_vertical_line(point_1, point_2, mode);
    }

    // Bresenham Algorithm
    int dx = abs((int)point_2.x - (int)point_1.x);
    int sx = point_1.x < point_2.x ? 1 : -1;
    int dy = -abs((int)point_2.y - (int)point_1.y);
    int sy = point_1.y < point_2.y ? 1 : -1;
    int err = dx + dy;

    while ((point_1.x != point_2.x) && (point_1.y != point_2.y))
    {
        draw_pixel(point_1, mode);
        if (2 * err >= dy)
        {
            err += dy;
            point_1.x += sx;
        }
        if (2 * err <= dx)
        {
            err += dx;
            point_1.y += sy;
        }
    }
}

void Painter::draw_rectangle(Point point_1, Point point_2, bool fill, Mode mode)
{
    uint16_t min_x = Math::min(point_1.x, point_2.x);
    uint16_t max_x = Math::max(point_1.x, point_2.x);
    uint16_t min_y = Math::min(point_1.y, point_2.y);
    uint16_t max_y = Math::max(point_1.y, point_2.y);

    if (!fill)
    {
        draw_line(Point(min_x, min_y), Point(max_x, min_y), mode);
        draw_line(Point(min_x, min_y), Point(min_x, max_y), mode);
        draw_line(Point(min_x, max_y), Point(max_x, max_y), mode);
        draw_line(Point(max_x, min_y), Point(max_x, max_y), mode);
    }
    else
    {
        for (uint16_t i = min_x; i <= max_x; i++)
        {
            draw_line(Point(i, min_y), Point(i, max_y + 1), mode);
        }
    }
}

unsigned char* Painter::get_image_buffer()
{
    return this->image_buffer;
}
