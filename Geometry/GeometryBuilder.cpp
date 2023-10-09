//
// Created by olber on 9/6/2022.
//
#include <memory>

#include "GeometryBuilder.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BoundedPlane.h"

namespace geometry
{
    std::shared_ptr<Geometry> GeometryBuilder::FromJson(const nlohmann::json& json_object)
    {
        std::string object_type = json_object.at("type").get<std::string>();
        std::shared_ptr<Geometry> result;
        if(object_type == "sphere") {
            result = std::make_shared<Sphere>();
        } else if(object_type == "plane") {
            result = std::make_shared<Plane>();
        } else if(object_type == "bounded_plane") {
            result = std::make_shared<BoundedPlane>();
        } else if(object_type == "triangle") {
            result = std::make_shared<Triangle>();
        } else {
            throw std::invalid_argument("json geometry objects must contain a 'type' field with one of the following values: \n [sphere, plane, bounded_plane, triangle]");
        }
        result->fromJson(json_object);
        return result;
    }
}
