#include "utilities/graphics/painter.hpp"

Painter::Painter(unsigned char* image, int width, int height, bool inverted_colors = false) {
    this->image = image;
    this->width = width;
    this->height = height;
    this->inverted_colors = inverted_colors;
}

void Painter::clear(bool colored) {
    for (uint16_t x = 0; x < this->width; x++) {
        for (uint16_t y = 0; y < this->height; y++) {
            draw_pixel(Point(x, y), colored);
        }
    }
}

void Painter::draw_pixel(Point point, bool colored = true) {
    if (point.x < 0 || point.x >= this->width ||
        point.y < 0 || point.y >= this->height) 
    {
        return;
    }

    Point real_point = transform(point);

    uint16_t index = (real_point.y * this->width + real_point.x ) / 8;
    uint8_t bit_to_change = 128 >> (real_point.x % 8);

    image[index] = (!this->inverted_colors && colored) || (this->inverted_colors && !colored) ? 
        image[index] | bit_to_change :
        image[index] & ~bit_to_change;
}

Point Painter::transform(Point point)
{
    return Point(point.y, this->width - point.x - 1);
}

void Painter::draw_character(Point point, char ascii_character, sFONT* font, bool colored = true) {
    uint16_t i, j;
    uint8_t char_offset = (ascii_character - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++) {
        for (i = 0; i < font->Width; i++) {
            if (pgm_read_byte(ptr) & (128 >> (i % 8))) {
                draw_pixel(Point(point.x + i, point.y + j), colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

void Painter::draw_string(Point point, const char* text, sFONT* font, bool colored = true) {
    const char* char_pointer = text;
    int current_x = point.x;
    
    while (*char_pointer != 0) {
        draw_character(Point(current_x, point.y), *char_pointer, font, colored);
        current_x += font->Width;
        char_pointer++;
    }
}

void Painter::draw_line(Point point_1, Point point_2, bool colored = true) {
    /* Bresenham algorithm */
    int dx = point_2.x - point_1.x >= 0 ? point_1.x - point_2.x : point_2.x - point_1.x;
    int sx = point_1.x < point_2.x ? 1 : -1;
    int dy = point_2.y - point_1.y <= 0 ? point_2.y - point_1.y : point_1.y - point_2.y;
    int sy = point_1.y < point_2.y ? 1 : -1;
    int err = dx + dy;

    while((point_1.x != point_2.x) && (point_1.y != point_2.y)) {
        draw_pixel(point_1 , colored);
        if (2 * err >= dy) {     
            err += dy;
            point_1.x += sx;
        }
        if (2 * err <= dx) {
            err += dx; 
            point_1.y += sy;
        }
    }
}

void Painter::draw_rectangle(Point point_1, Point point_2, bool fill = false, bool colored = true) {
    int min_x, min_y, max_x, max_y;
    min_x = point_2.x > point_1.x ? point_1.x : point_2.x;
    max_x = point_2.x > point_1.x ? point_2.x : point_1.x;
    min_y = point_2.y > point_1.y ? point_1.y : point_2.y;
    max_y = point_2.y > point_1.y ? point_2.y : point_1.y;
    
    if (!fill)
    {
        draw_line(Point(min_x, min_y), Point(max_x + 1, min_y));
        draw_line(Point(min_x, max_y), Point(max_x + 1, max_y));
        draw_line(Point(min_x, min_y), Point(min_x, max_y + 1));
        draw_line(Point(max_x, min_y), Point(max_x, max_y + 1));
    }
    else
    {
        for (uint16_t i = min_x; i <= max_x; i++) 
        {
            draw_line(Point(i, min_y), Point(i, max_y + 1), colored);
        }
    }

}





















