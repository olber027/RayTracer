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
        static std::shared_ptr<Geometry> FromJson(const nlohmann::json& json_object);
    };
}