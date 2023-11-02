//
// Created by olber on 9/6/2022.
//

#pragma once

#include "LinearAlgebraTypeTraits.h"
#include "Ray.h"
#include "Geometry.h"
#include "Color.h"
#include "GeometryBuilder.h"
#include "json.h"

namespace environment
{
    using namespace linear_algebra_core;
    using namespace geometry;
    using namespace color_core;

    /*!
     * Environment class stores all the geometry in the environment, and provides access to that geometry.
     */
    template<IsFloatingPoint value_type>
    class Environment
    {
    public:
        using Geometry_Ptr = std::shared_ptr<Geometry<value_type>>;
        // TODO: use something smarter than just a vector to store these
        using GeometryContainer = std::vector<Geometry_Ptr>;
        using Ray_3 = Ray<3, value_type>;
        using Point_3 = Point_X<3, value_type>;

    private:
        GeometryContainer m_geometry;
        Color m_backgroundColor{};

    public:
        Environment() = default;
        ~Environment() = default;
        Environment(const Environment& other) = default;
        Environment(Environment&& other) noexcept = default;
        Environment& operator=(const Environment& other) = default;
        Environment& operator=(Environment&& other) noexcept = default;

        /*!
         * Construct the environment from a json object. Json object must have a "geometry" field containing all the
         * json definitions of the geometry.
         * @param environment_config Config file to load the geometry from
         */
        explicit Environment(const nlohmann::json& environment_config)
        {
            fromJson(environment_config);
        }

        const GeometryContainer& getGeometry() { return m_geometry; }
        [[nodiscard]] GeometryContainer getGeometry() const { return m_geometry; }

        /*!
         * @param new_geometry geometry to add
         */
        void addGeometry(const Geometry_Ptr& new_geometry) { m_geometry.push_back(new_geometry); }

        /*!
         * @param geometry_list list of geometry to add
         */
        void addGeometryList(const std::vector<Geometry_Ptr>& geometry_list)
        {
            for(const auto& geometry : geometry_list)
            {
                m_geometry.push_back(geometry);
            }
        }

        /*!
         * @param json_object json definition of the geometry to add
         */
        void addGeometry(const nlohmann::json& json_object)
        {
            m_geometry.push_back(geometry::GeometryBuilder<value_type>::FromJson(json_object));
        }

        /*!
         * @param json_list json list of geometry to add
         */
        void addGeometryList(const nlohmann::json& json_list)
        {
            for(const auto& json_object : json_list) {
                m_geometry.push_back(geometry::GeometryBuilder<value_type>::FromJson(json_object));
            }
        }

        /*!
         * Returns the geometry who's first intersection point is closest to the origin of the \p ray. If the \p ray origin
         * is inside of a geometry, that geometry will be ignored.
         * @param ray Ray to check for intersection
         * @return The closest geometry object
         */
        [[nodiscard]] Geometry_Ptr getFirstIntersectedGeometry(const Ray_3& ray) const
        {
            Geometry_Ptr geometry_to_return = nullptr;
            value_type current_shortest_distance = std::numeric_limits<value_type>::max();
            for(const auto& geometry : m_geometry) {
                std::optional<Point_3> intersection_point = geometry->getIntersectionPoint(ray);
                if(intersection_point.has_value()) {
                    value_type distance_to_object = (intersection_point.value() - ray.getOrigin()).getMagnitude();
                    if(distance_to_object < current_shortest_distance)
                    {
                        geometry_to_return = geometry;
                        current_shortest_distance = distance_to_object;
                    }
                }
            }
            return geometry_to_return;
        }

        /*!
         * Retrieves all geometry that the given \p ray intersects
         * @param ray Ray to check for intersection
         * @return a vector containing all intersected geometry
         */
        [[nodiscard]] std::vector<Geometry_Ptr> getIntersectingGeometry(const Ray_3& ray) const
        {
            std::vector<Geometry_Ptr> result;
            for(const auto& geometry_object : m_geometry) {
                if(geometry_object->intersects(ray)) {
                    result.push_back(geometry_object);
                }
            }
            return result;
        }

        /*!
         * Returns the background color, blended with white based on the y value of the given \p ray.
         * @param ray Ray to get the background color for
         * @return The blended color
         */
        [[nodiscard]] Color getBackgroundColor(const Ray_3& ray) const
        {
            // this should make a gradient from darker to lighter as it goes up the screen
            value_type t = (ray.getDirection()[1] + 1.0) / 2.0;
            static Color white(216, 232, 255);
            return Color::blend(m_backgroundColor, white, t);
        }

        void fromJson(const nlohmann::json& environment_json)
        {
            nlohmann::json geometry_json, background_color_json;
            try {
                geometry_json = environment_json.at("geometry");
            } catch(std::exception& e) {
                throw std::invalid_argument("could not find the required 'geometry' key.");
            }

            try {
                background_color_json = environment_json.at("background_color");
            } catch(std::exception& e) {
                throw std::invalid_argument("could not find the required 'background_color' key.");
            }

            addGeometryList(geometry_json);
            m_backgroundColor.fromJson(background_color_json);
        }
    };
}