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
        Point_3  m_referenceCorner;
        Vector_3 m_width;
        Vector_3 m_height;

    public:
        Screen() = default;
        ~Screen() = default;
        Screen(const Screen& other) = default;
        Screen(Screen&& other) noexcept = default;
        Screen& operator=(const Screen& other) = default;
        Screen& operator=(Screen&& other) noexcept = default;

        /*!
         * Construct a Screen located at \p reference_corner with the unit X vector as the width and the
         * negative unit Y vector as the height
         * @param reference_corner The reference corner for the screen
         */
        explicit Screen(const Point_3& reference_corner) : m_referenceCorner(reference_corner),
                                                   m_width(getUnitX()),
                                                   m_height(Vector_3(0.0, -1.0, 0.0)) { }

       /*!
        * Construct a screen from the given parameters
        * @param reference_corner Reference corner for the screen
        * @param width Width of the screen
        * @param height Height of the screen
        */
        explicit Screen(const Point_3& reference_corner, const Vector_3& width, const Vector_3& height)
            : m_referenceCorner(reference_corner), m_width(width), m_height(height) { }

        /*!
         * Construct a screen object from the given \p screen_config
         * @param screen_config the json config to construct the screen from
         */
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

            m_referenceCorner = Point_3(top_left);
            m_width           = Vector_3(width);
            m_height = Vector_3(height);
        }

        /*!
         * Set the width of the screen to \p new_width
         * @param new_width new width of the screen
         * @return a reference to this Screen
         */
        Screen& setWidth(const Vector_3& new_width)
        {
            m_width = new_width;
            return (*this);
        }

        /*!
         * Set the height of the screen to the new_height
         * @param new_height new height of the screen
         * @return a reference to this screen
         */
        Screen& setHeight(const Vector_3& new_height)
        {
            m_height = new_height;
            return (*this);
        }

        /*!
         * Get the point of the screen at \p i and \p j. \p i and \p j must both be in the range of [0.0, 1.0]. (0,0) is
         * the reference corner of the screen, (1,1) is the reference corner of the screen + width + height.
         * @param i scalar value for the width
         * @param j scalar value for the height
         * @return The resulting point
         */
        [[nodiscard]] Point_3 getPointAt(double i, double j) const
        {
            if(i < 0.0 || i > 1.0 || j < 0.0 || j > 1.0) {
                throw std::invalid_argument("i and j must be in the range [0.0, 1.0]. got i=" + std::to_string(i) + " and j=" + std::to_string(j));
            }
            return m_referenceCorner + (m_width * i) + (m_height * j);
        }

        /*!
         * Translate the screen by the given \p translation_vector
         * @param translation_vector vector translate the screen by
         * @return a reference to this Screen
         */
        Screen& translate(const Vector_3& translation_vector)
        {
            m_referenceCorner += translation_vector;
            return (*this);
        }

        /*!
         * Translate the screen to the \p new_position
         * @param new_position the new position for the reference corner of the screen
         * @return a reference to this Screen
         */
        Screen& translateTo(const Point_3& new_position)
        {
            m_referenceCorner = new_position;
            return (*this);
        }

        /*!
         * @return The vector defining the width of the screen
         */
        [[nodiscard]] const Vector_3& getWidth() const { return m_width; }
        /*!
         * @return The vector defining the height of the screen
         */
        [[nodiscard]] const Vector_3& getHeight() const { return m_height; }
        /*!
         * @return the reference corner for the screen
         */
        [[nodiscard]] const Point_3& getReferenceCorner() const { return m_referenceCorner; }
    };
}