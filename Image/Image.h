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
        std::vector<std::vector<Color>> m_pixels{};
        int m_colorRange;

    public:

        Image() : Image(0, 0, 255) { }
        ~Image() = default;
        Image(const Image& other) = default;
        Image(Image&& other) noexcept = default;
        Image& operator=(const Image& other) = default;
        Image& operator=(Image&& other) noexcept = default;

        /*!
         * Construct a blank image using the given inputs
         * @param width width of the image
         * @param height height of the image
         * @param colorRange maximum value for the colors in the image
         */
        Image(unsigned int width, unsigned int height, int colorRange) : m_pixels(height, std::vector<Color>(width, Color())), m_colorRange(colorRange) { }

        /*!
         * Construct a blank image using parameters from the \p json_file
         * @param json_file the file containing the parameters for the image
         */
        explicit Image(nlohmann::json json_file) {
            int width = 0;
            int height = 0;
            int colorRange = 255;
            try {
                width = json_file.at("width").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'width' key. Using " + std::to_string(width));
            }
            try {
                height = json_file.at("height").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'height' key. Using " + std::to_string(height));
            }
            try {
                colorRange = json_file.at("color_range").get<int>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'color_range' key. Using " + std::to_string(colorRange));
            }
            m_pixels     = std::vector<std::vector<Color>>(height, std::vector<Color>(width, Color()));
            m_colorRange = colorRange;
        }

        /*!
         * Get the Color at the given \p x and \p y coordinates. 0,0 is the top left corner,
         * width(), height() is the bottom right corner
         * @param x X coordinate
         * @param y Y coordinate
         * @return The Color at the given coordinates
         */
        [[nodiscard]] inline Color at(size_t x, size_t y) const {
            return m_pixels.at(y).at(x);
        }

        /*!
         * Get the Color at the given \p x and \p y coordinates. 0,0 is the top left corner,
         * width(), height() is the bottom right corner
         * @param x X coordinate
         * @param y Y coordinate
         * @return The Color at the given coordinates
         */
        [[nodiscard]] inline Color& at(size_t x, size_t y) {
            return m_pixels.at(y).at(x);
        }

        /*!
         * @return the width of the image
         */
        [[nodiscard]] inline size_t width()  const { return height() > 0 ? m_pixels.at(0).size() : 0; }

        /*!
         * @return the height of the image
         */
        [[nodiscard]] inline size_t height() const { return m_pixels.size(); }

        /*!
         * get the aspect ratio of the image. Result will be 0 if the height() is 0.
         * @return the aspect ratio of the image
         */
        [[nodiscard]] inline double aspectRatio() const { return height() > 0 ? static_cast<double>(width()) / static_cast<double>(height()) : 0; }

        /*!
         * @return the maximum value for the colors in the image
         */
        [[nodiscard]] inline int getColorRange() const { return m_colorRange; }

        /*!
         * Set the color range to the \p newRange
         * @param newRange the new color range to use
         */
        inline void setColorRange(int newRange) { m_colorRange = newRange; }

        /*!
         * Replace the current image with a blank image with parameters in the \p json_file
         * @param json_file the file containing the parameters for the image
         */
        inline void from_json(const nlohmann::json& json_file)
        {
            *this = Image(json_file);
        }
    };
}