#pragma once

#include <cmath>
#include <cstdint>
#include <utility>

#include "OLED_SSD1306.hpp"
#include "PixelColor.hpp"

namespace gfx {
//
// Line drawing
//
template <typename T>
void writeLine(OLED_SSD1306<T>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color) {
    auto steep = std::abs(y_end - y_start) > std::abs(x_end - x_start);

    if (steep) {
        std::swap(x_start, y_start);
        std::swap(x_end, y_end);
    }

    if (x_start > x_end) {
        std::swap(x_start, x_end);
        std::swap(y_start, y_end);
    }

    const int16_t dx = x_end - x_start;
    const int16_t dy = std::abs(y_end - y_start);

    int16_t err = dx / 2;
    int16_t y_step;

    if (y_start < y_end) {
        y_step = 1;
    } else {
        y_step = -1;
    }

    for (; x_start <= x_end; x_start++) {
        if (steep) {
            oled.drawPixel(y_start, x_start, color);
        } else {
            oled.drawPixel(x_start, y_start, color);
        }

        err -= dy;

        if (err < 0) {
            y_start += y_step;
            err += dx;
        }
    }
}

template <typename T>
void drawFastVLine(OLED_SSD1306<T>& oled, int16_t x_start, int16_t y_start, int16_t h, PixelColor color) {
    writeLine(oled, x_start, y_start, x_start, y_start + h - 1, color);
}

template <typename T>
void drawFastHLine(OLED_SSD1306<T>& oled, int16_t x_start, int16_t y_start, int16_t w, PixelColor color) {
    writeLine(oled, x_start, y_start, x_start + w - 1, y_start, color);
}

template <typename T>
void drawLine(OLED_SSD1306<T>& oled, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, PixelColor color) {
    if (x_start == x_end) {
        if (y_start > y_end) {
            std::swap(y_start, y_end);
        }
        drawFastVLine(oled, x_start, y_start, y_end - y_start + 1, color);
    } else if (y_start == y_end) {
        if (x_start > x_end) {
            std::swap(x_start, x_end);
        }
        drawFastVLine(oled, x_start, y_start, x_end - x_start + 1, color);
    } else {
        writeLine(oled, x_start, y_start, x_end, y_end, color);
    }
}
}  // namespace gfx
