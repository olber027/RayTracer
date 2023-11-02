//
// Created by Matt on 10/9/2023.
//
#pragma once
#include "Vector_X.h"
#include "Point_X.h"
#include "Matrix_MxN.h"
#include "Ray.h"
#include "json.h"

namespace utility {

    template<size_t N, typename value_type>
    [[nodiscard]] inline linear_algebra_core::Point_X<N, value_type> PointFromJson(const nlohmann::json& point_json)
    {
        std::vector<value_type> point;
        try {
            point = point_json.get<decltype(point)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("point json must be in the form of [ a, b, c, ... , z ].");
        }
        if(point.size() != N) {
            throw std::invalid_argument("Tried to parse a point with the incorrect number of values");
        }

        return linear_algebra_core::Point_X<N, value_type>(point);
    }

    template<size_t N, typename value_type>
    [[nodiscard]] inline linear_algebra_core::Vector_X<N, value_type> VectorFromJson(const nlohmann::json& vector_json)
    {
        std::vector<value_type> vector;
        try {
            vector = vector_json.get<decltype(vector)>();
        } catch(std::exception& e) {
            throw std::invalid_argument("point json must be in the form of [ a, b, c, ... , z ].");
        }
        if(vector.size() != N) {
            throw std::invalid_argument("Tried to parse a point with the incorrect number of values");
        }

        return linear_algebra_core::Vector_X<N, value_type>(vector);
    }

    template<size_t N, typename value_type>
    [[nodiscard]] inline linear_algebra_core::Ray<N, value_type> RayFromJson(const nlohmann::json& ray_json)
    {
        nlohmann::json origin_json, direction_json;
        try {
            origin_json = ray_json.at("origin");
        } catch(std::exception& e) {
            throw std::invalid_argument("ray json must have the key 'origin'.");
        }

        try {
            direction_json = ray_json.at("direction");
        } catch(std::exception& e) {
            throw std::invalid_argument("ray json must have the key 'direction'.");
        }

        return linear_algebra_core::Ray<N, value_type>(PointFromJson<N, value_type>(origin_json),
                                                       VectorFromJson<N, value_type>(direction_json));
    }

}