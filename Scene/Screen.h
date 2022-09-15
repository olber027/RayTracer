//
// Created by olber on 9/1/2022.
//

#pragma once

#include "Point_X.h"
#include "Vector_X.h"
#include "json.h"

namespace scene {
    using namespace linear_algebra_core;

    class Screen
    {
    private:
        Point_3  m_topLeftCorner;
        Vector_3 m_width;
        Vector_3 m_height;

    public:
        Screen() = default;
        explicit Screen(const Point_3& top_left) : m_topLeftCorner(top_left),
                                                   m_width(Vector_3(1.0, 0.0, 0.0)),
                                                   m_height(Vector_3(0.0, -1.0, 0.0)) { }

        explicit Screen(const Point_3& top_left, const Vector_3& width, const Vector_3& height)
            : m_topLeftCorner(top_left), m_width(width), m_height(height) { }

        explicit Screen(const nlohmann::json& screen_config)
        {
            std::vector<double> top_left{0, 0, 0};
            std::vector<double> width{0, 0, 0};
            std::vector<double> height{0, 0, 0};
            auto vector_to_string = [] (auto val) { return "[" + std::to_string(val[0]) + ", " + std::to_string(val[1]) + ", " + std::to_string(val[2]) + "]"; };

            try {
                top_left = screen_config.at("top_left").get<std::vector<double>>();
                if(top_left.size() != 3)
                {
                    throw std::invalid_argument("'top_left' field must be specified in the form: [X, Y, Z]");
                }
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'top_left' key. Using " + vector_to_string(top_left));
            }

            try {
                width = screen_config.at("width").get<std::vector<double>>();
                if(width.size() != 3)
                {
                    throw std::invalid_argument("'width' field must be specified in the form: [X, Y, Z]");
                }
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'width' key. Using " + vector_to_string(width));
            }

            try {
                height = screen_config.at("height").get<std::vector<double>>();
                if(height.size() != 3)
                {
                    throw std::invalid_argument("'height' field must be specified in the form: [X, Y, Z]");
                }
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'height' key. Using " + vector_to_string(height));
            }

            m_topLeftCorner = Point_3(top_left);
            m_width  = Vector_3(width);
            m_height = Vector_3(height);
        }

        ~Screen() = default;
        Screen(const Screen& other) = default;
        Screen(Screen&& other) noexcept = default;
        Screen& operator=(const Screen& other) = default;
        Screen& operator=(Screen&& other) noexcept = default;

        Screen& setWidth(const Vector_3& new_width)
        {
            m_width = new_width;
            return (*this);
        }

        Screen& setHeight(const Vector_3& new_height)
        {
            m_height = new_height;
            return (*this);
        }

        [[nodiscard]] Point_3 getPointAt(double i, double j) const
        {
            if(i < 0 || i > 1 || j < 0 || j > 1) {
                throw std::invalid_argument("i and j must be in the range [0.0, 1.0]. got i=" + std::to_string(i) + " and j=" + std::to_string(j));
            }
            return m_topLeftCorner + (m_width * i) + (m_height * j);
        }

        Screen& translate(const Vector_3& translation_vector)
        {
            m_topLeftCorner += translation_vector;
            return (*this);
        }

        Screen& translateTo(const Point_3& new_position)
        {
            m_topLeftCorner = new_position;
            return (*this);
        }

        Vector_3 getWidth() const { return m_width; }
        Vector_3 getHeight() const { return m_height; }
        Point_3 getCorner() const { return m_topLeftCorner; }
    };
}