#pragma once
#include <vector>

#include "Color.h"

namespace output
{
    using namespace color_core;
    class Image
    {
    private:
        std::vector<std::vector<Color>> pixels;
        int color_range;

    public:

        Image() : Image(0, 0, 255) { }
        Image(unsigned int width, unsigned int height, int colorRange) : pixels(height, std::vector<Color>(width, Color())), color_range(colorRange) { }

        ~Image() = default;

        Image(const Image& other) = default;

        Image(Image&& other) noexcept = default;

        Image& operator=(const Image& other) = default;

        Image& operator=(Image&& other) noexcept = default;

        [[nodiscard]] Color at(size_t x, size_t y) const {
            return pixels.at(y).at(x);
        }

        [[nodiscard]] Color& at(size_t x, size_t y) {
            return pixels.at(y).at(x);
        }

        [[nodiscard]] unsigned int width() const { return pixels.at(0).size(); }
        [[nodiscard]] unsigned int height() const { return pixels.size(); }

        [[nodiscard]] int getColorRange() const { return color_range; }
        void setColorRange(int newRange) { color_range = newRange; }
    };
}