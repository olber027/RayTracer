#pragma once

#include <array>
#include <string>

namespace color_core
{

    class Color
    {
    private:
        std::array<double, 4> values;

    public:

        explicit Color(double r, double g, double b) : Color(r, g, b, 1.0) { }
        explicit Color(double r, double g, double b, double alpha) : values{r, g, b, alpha} { }
        explicit Color(int r, int g, int b) : Color(r, g, b, 1.0) { }
        explicit Color(int r, int g, int b, double alpha) : values{static_cast<double>(r), static_cast<double>(g),
                                                                   static_cast<double>(b), alpha} { }

        Color() = default;
        ~Color() = default;
        Color(const Color& other) = default;
        Color(Color&& other) noexcept = default;
        Color& operator=(const Color& other) = default;
        Color& operator=(Color&& other) noexcept = default;

        template<typename T>
        Color operator*(T scalar) const {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return Color(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3]);
        }

        template<typename T>
        friend Color operator*(T scalar, const Color& color) {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            return color * scalar;
        }

        template<typename T>
        Color& operator*=(T scalar) {
            static_assert(std::is_arithmetic_v<T>, "scalar must be an arithmetic type");
            values[0] *= scalar;
            values[1] *= scalar;
            values[2] *= scalar;
            return (*this);
        }

        Color operator+(const Color& rhs) {
            return Color(values[0] + rhs.R(), values[1] + rhs.G(), values[2] + rhs.B(), std::min(values[3] + rhs.Alpha(), 1.0));
        }

        void operator+=(const Color& rhs) {
            values[0] += rhs.R();
            values[1] += rhs.G();
            values[2] += rhs.B();
        }

        [[nodiscard]] std::string to_string() const
        {
            return std::to_string(R()) + " " + std::to_string(G()) + " " + std::to_string(B());
        }

        [[nodiscard]] std::string to_string_with_alpha() const
        {
            return std::to_string(R()) + " " + std::to_string(G()) + " " + std::to_string(B()) + " " + std::to_string(Alpha());
        }

        /*!
         * Will blend the two colors given. A \p t value of 0.5 will be an equal mix of the two colors, a \p t value of
         * 1.0 will be purely the second color, and a \p t value of 0.0 will be purely the first color.
         * @param first First color to be blended
         * @param second Second color to be blended
         * @param t value determining how much of each color there will be
         * @return The blended color
         */
        [[nodiscard]] static Color blend(const Color& first, const Color& second, double t) {
            if(t > 1.0 || t < 0.0)
            {
                throw std::invalid_argument("Color::blend - t must be in the range of [0.0, 1.0]. Given value was: " + std::to_string(t));
            }
            return Color(
                first.R() + (t * (second.R() - first.R())),
                first.G() + (t * (second.G() - first.G())),
                first.B() + (t * (second.B() - first.B()))
            );
        }

        [[nodiscard]] int R() const { return static_cast<int>(values[0]); }

        [[nodiscard]] int G() const { return static_cast<int>(values[1]); }

        [[nodiscard]] int B() const { return static_cast<int>(values[2]); }

        [[nodiscard]] double Alpha() const { return static_cast<int>(values[3]); }
    };
}