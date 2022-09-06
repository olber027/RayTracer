#pragma once

#include "Point_X.h"
#include "Vector_X.h"
#include "Matrix_MxN.h"
#include "Ray.h"

namespace scene
{
    using namespace linear_algebra_core;
    class Camera
    {
    private:
        Point_3  m_position;
        Vector_3 m_zAxis;
        Vector_3 m_yAxis;
    public:

        [[nodiscard]] Vector_3 X() const { return m_zAxis.cross(m_yAxis).normalize(); }
        [[nodiscard]] Vector_3 Y() const { return m_yAxis; }
        [[nodiscard]] Vector_3 Z() const { return m_zAxis; }

        Camera& move(const Vector_3& translation_vector) {
            m_position += translation_vector;
            return (*this);
        }

        Camera& moveTo(const Point_3& new_position) {
            m_position = new_position;
            return (*this);
        }

    };
}