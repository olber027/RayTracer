#pragma once
#include <vector>

#include "Color.h"
#include "json.h"

namespace output
{
    using namespace color_core;
    class Image
    {
    private:
        std::vector<std::vector<Color>> m_pixels;
        int m_colorRange;

    public:

        Image() : Image(0, 0, 255) { }
        Image(unsigned int width, unsigned int height, int colorRange) : m_pixels(height, std::vector<Color>(width, Color())), m_colorRange(colorRange) { }
        Image(nlohmann::json json_file) {
            int width, height, colorRange;
            try {
                width = json_file.at("width").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'width' key");
            }
            try {
                height = json_file.at("height").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'height' key");
            }
            try {
                colorRange = json_file.at("color_range").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'color_range' key");
            }
            m_pixels     = std::vector<std::vector<Color>>(height, std::vector<Color>(width, Color()));
            m_colorRange = colorRange;
        }

        ~Image() = default;

        Image(const Image& other) = default;

        Image(Image&& other) noexcept = default;

        Image& operator=(const Image& other) = default;

        Image& operator=(Image&& other) noexcept = default;

        [[nodiscard]] Color at(size_t x, size_t y) const {
            return m_pixels.at(y).at(x);
        }

        [[nodiscard]] Color& at(size_t x, size_t y) {
            return m_pixels.at(y).at(x);
        }

        [[nodiscard]] int width() const { return static_cast<int>(m_pixels.at(0).size()); }
        [[nodiscard]] int height() const { return static_cast<int>(m_pixels.size()); }
        [[nodiscard]] double aspectRatio() const { return static_cast<double>(width()) / static_cast<double>(height()); }

        [[nodiscard]] int getColorRange() const { return m_colorRange; }
        void setColorRange(int newRange) { m_colorRange = newRange; }

        void from_json(const nlohmann::json& json_file)
        {
            *this = Image(json_file);
        }
    };
}