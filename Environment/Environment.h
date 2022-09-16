//
// Created by olber on 9/6/2022.
//

#pragma once

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

    class Environment
    {
    public:
        using GeometryContainer = std::vector<std::shared_ptr<Geometry>>;

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

        explicit Environment(const nlohmann::json& environment_config)
        {
            addGeometryList(environment_config.at("geometry"));
            std::vector<int> backgroundColor = environment_config.at("background_color");
            m_backgroundColor = Color(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
        }

        const GeometryContainer& getGeometry() { return m_geometry; }
        [[nodiscard]] GeometryContainer getGeometry() const { return m_geometry; }

        void addGeometry(const std::shared_ptr<Geometry>& new_geometry)
        {
            m_geometry.push_back(new_geometry);
        }

        void addGeometry(const nlohmann::json& json_object)
        {
            m_geometry.push_back(geometry::GeometryBuilder::FromJson(json_object));
        }

        void addGeometryList(const nlohmann::json& json_list)
        {
            for(const auto& json_object : json_list) {
                m_geometry.push_back(geometry::GeometryBuilder::FromJson(json_object));
            }
        }

        [[nodiscard]] GeometryContainer getIntersectingGeometry(const Ray& ray) const
        {
            GeometryContainer result;
            for(const auto& geometry_object : m_geometry) {
                if(geometry_object->intersects(ray)) {
                    result.push_back(geometry_object);
                }
            }
            return result;
        }

        [[nodiscard]] std::shared_ptr<geometry::Geometry> getFirstIntersectedGeometry(const Ray& ray) const
        {
            std::shared_ptr<geometry::Geometry> geometry_to_return = nullptr;
            double current_shortest_distance = std::numeric_limits<double>::max();
            for(const auto& geometry_object : m_geometry) {
                std::optional<Point_3> intersection_point = geometry_object->getIntersectionPoint(ray);
                if(intersection_point.has_value()) {
                    double distance_to_object = (intersection_point.value() - ray.getOrigin()).getMagnitude();
                    if(distance_to_object < current_shortest_distance)
                    {
                        geometry_to_return = geometry_object;
                        current_shortest_distance = distance_to_object;
                    }
                }
            }
            return geometry_to_return;
        }

        [[nodiscard]] Color getBackgroundColor(const Ray& ray) const
        {
            double t = (ray.getDirection().normalize()[1] + 1.0) / 2.0;
            static Color white(216, 232, 255);
            return Color::blend(m_backgroundColor, white, t);
        }
    };
}