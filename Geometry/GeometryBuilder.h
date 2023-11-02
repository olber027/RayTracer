//
// Created by olber on 9/6/2022.
//

#pragma once
#include <memory>
#include "LinearAlgebraTypeTraits.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BoundedPlane.h"

namespace geometry
{
    template<IsArithmetic value_type>
    class GeometryBuilder
    {
    public:
        /*!
         * Create a geometry object from the given \p json_object
         * @param json_object the json node to create the object from
         * @return A pointer to the newly constructed Geometry object
         */
        static std::shared_ptr<Geometry<value_type>> FromJson(const nlohmann::json& json_object)
        {
            std::string object_type = json_object.at("type").get<std::string>();
            std::shared_ptr<Geometry<value_type>> result;
            if(object_type == "sphere") {
                result = std::make_shared<Sphere<value_type>>();
            } else if(object_type == "plane") {
                result = std::make_shared<Plane<value_type>>();
            } else if(object_type == "bounded_plane") {
                result = std::make_shared<BoundedPlane<value_type>>();
            } else if(object_type == "triangle") {
                result = std::make_shared<Triangle<value_type>>();
            } else {
                throw std::invalid_argument("json geometry objects must contain a 'type' field with one of the following values: \n [sphere, plane, bounded_plane, triangle]");
            }
            result->fromJson(json_object);
            return result;
        }
    };
}