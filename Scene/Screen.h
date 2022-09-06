//
// Created by olber on 9/1/2022.
//

#pragma once

#include "Point_X.h"
#include "Vector_X.h"

namespace scene {
    using namespace linear_algebra_core;

    class Screen
    {
    private:
        Point_3  m_topLeftCorner;
        Vector_3 m_width;
        Vector_3 m_height;

    public:
        explicit Screen(const Point_3& top_left) : m_topLeftCorner(top_left),
                                                   m_width(Vector_3(1.0, 0.0, 0.0)),
                                                   m_height(Vector_3(0.0, -1.0, 0.0)) { }

        explicit Screen(const Point_3& top_left, const Vector_3& width, const Vector_3& height)
            : m_topLeftCorner(top_left), m_width(width), m_height(height) { }

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
    };
}