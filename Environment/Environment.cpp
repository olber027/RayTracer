//
// Created by olber on 9/22/2022.
//
#include "Environment.h"
#include "Geometry.h"
#include "json.h"

#include <memory>
#include <vector>

namespace environment
{
    void Environment::addGeometry(const std::shared_ptr<Geometry>& new_geometry)
    {
        m_geometry.push_back(new_geometry);
    }

    void Environment::addGeometryList(const std::vector<std::shared_ptr<Geometry>>& geometry_list)
    {
        for(const auto& geometry : geometry_list)
        {
            m_geometry.push_back(geometry);
        }
    }

    void Environment::addGeometry(const nlohmann::json& json_object)
    {
        m_geometry.push_back(geometry::GeometryBuilder::FromJson(json_object));
    }

    void Environment::addGeometryList(const nlohmann::json& json_list)
    {
        for(const auto& json_object : json_list) {
            m_geometry.push_back(geometry::GeometryBuilder::FromJson(json_object));
        }
    }

    [[nodiscard]] std::shared_ptr<geometry::Geometry> Environment::getFirstIntersectedGeometry(const Ray& ray) const
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

    [[nodiscard]] std::vector<std::shared_ptr<Geometry>> Environment::getIntersectingGeometry(const Ray& ray) const
    {
        std::vector<std::shared_ptr<Geometry>> result;
        for(const auto& geometry_object : m_geometry) {
            if(geometry_object->intersects(ray)) {
                result.push_back(geometry_object);
            }
        }
        return result;
    }

    [[nodiscard]] Color Environment::getBackgroundColor(const Ray& ray) const
    {
        // this should make a gradient from darker to lighter as it goes up the screen
        double t = (ray.getDirection().normalize()[1] + 1.0) / 2.0;
        static Color white(216, 232, 255);
        return Color::blend(m_backgroundColor, white, t);
    }
}