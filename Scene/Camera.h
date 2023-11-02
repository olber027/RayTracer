#pragma once

#include "Point_X.h"
#include "Vector_X.h"
#include "Matrix_MxN.h"
#include "json.h"

namespace scene
{
    using namespace linear_algebra_core;

    template<IsFloatingPoint value_type>
    class Camera
    {
    private:
        using Point_3 = Point_X<3, value_type>;
        using Vector_3 = Vector_X<3, value_type>;

        Point_3  m_position;
        Vector_3 m_zAxis;
        Vector_3 m_yAxis;
    public:
        Camera() = default;
        ~Camera() = default;
        Camera(const Camera& other) = default;
        Camera(Camera&& other) noexcept = default;
        Camera& operator=(const Camera& other) = default;
        Camera& operator=(Camera&& other) noexcept = default;

        /*!
         * Construct a Camera object from the given \p camera_config
         * @param camera_config json config for the camera
         */
        explicit Camera(const nlohmann::json& camera_config)
        {
            fromJson(camera_config);
        }

        /*!
         * @return the unit vector defining the x-axis for the camera
         */
        [[nodiscard]] Vector_3 X() const { return m_zAxis.cross(m_yAxis).normalize(); }
        /*!
         * @return the unit vector defining the y-axis for the camera
         */
        [[nodiscard]] Vector_3 Y() const { return m_yAxis; }
        /*!
         * @return the unit vector defining the z-axis for the camera
         */
        [[nodiscard]] Vector_3 Z() const { return m_zAxis; }
        /*!
         * @return the position of the camera
         */
        [[nodiscard]] Point_3 getPosition() const { return m_position; }

        /*!
         * Translate the camera by the given \p translation_vector
         * @param translation_vector vector to translate the camera by
         * @return a reference to this camera
         */
        Camera& translate(const Vector_3& translation_vector) {
            m_position += translation_vector;
            return (*this);
        }

        /*!
         * Translate the camera to the \p new_position
         * @param new_position position to translate the camera to
         * @return a reference to this camera
         */
        Camera& translateTo(const Point_3& new_position) {
            m_position = new_position;
            return (*this);
        }

        void fromJson(const nlohmann::json& camera_json)
        {
            std::vector<value_type> position{0, 0, 0};
            std::vector<value_type> zAxis{0, 0, 1.0};
            std::vector<value_type> yAxis{0, 1.0, 0};
            auto vector_to_string = [] (auto val) { return "[" + std::to_string(val[0]) + ", " + std::to_string(val[1]) + ", " + std::to_string(val[2]) + "]"; };

            try {
                position = camera_json.at("position").get<std::vector<value_type>>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'position' key. Using " + vector_to_string(position));
            }
            if(position.size() != 3)
            {
                throw std::invalid_argument("'position' field must be specified in the form: [X, Y, Z]");
            }

            try {
                zAxis = camera_json.at("z_axis").get<std::vector<value_type>>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'z_axis' key. Using " + vector_to_string(zAxis));
            }
            if(zAxis.size() != 3)
            {
                throw std::invalid_argument("'z_axis' field must be specified in the form: [X, Y, Z]");
            }

            try {
                yAxis = camera_json.at("y_axis").get<std::vector<value_type>>();
            } catch(std::exception& e) {
                throw std::invalid_argument("Could not find the required 'y_axis' key. Using " + vector_to_string(yAxis));
            }
            if(yAxis.size() != 3)
            {
                throw std::invalid_argument("'y_axis' field must be specified in the form: [X, Y, Z]");
            }

            m_position = Point_3(position);
            m_zAxis = Vector_3(zAxis).normalize();
            m_yAxis = Vector_3(yAxis).normalize();

            if(!m_zAxis.isOrthogonalTo(m_yAxis))
            {
                throw std::invalid_argument("z_axis and y_axis must be orthogonal vectors");
            }
        }

    };
}