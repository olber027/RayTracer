//
// Created by olber on 9/6/2022.
//

#pragma once
#include <memory>
#include "Geometry.h"

namespace geometry
{
    class GeometryBuilder
    {
    public:
        /*!
         * Create a geometry object from the given \p json_object
         * @param json_object the json node to create the object from
         * @return A pointer to the newly constructed Geometry object
         */
        static std::shared_ptr<Geometry> FromJson(const nlohmann::json& json_object);
    };
}