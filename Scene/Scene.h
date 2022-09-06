//
// Created by olber on 9/2/2022.
//
#pragma once

#include <memory>

#include "Screen.h"
#include "Camera.h"
#include "Geometry.h"

namespace scene {
    class Scene {
    private:
        std::vector<std::shared_ptr<geometry::Geometry>> m_geometry;
        Camera m_camera;
        Screen m_screen;

    public:

        void populate(const std::shared_ptr<geometry::Geometry>& new_geometry) {
            m_geometry.push_back(new_geometry);
        }
    };
}
