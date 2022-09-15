//
// Created by olber on 9/2/2022.
//
#pragma once

#include <memory>

#include "Screen.h"
#include "Camera.h"
#include "json.h"

namespace scene {
    class Scene {
    private:
        Camera m_camera;
        Screen m_screen;
    public:
        Scene() = default;
        explicit Scene(const nlohmann::json& scene_config) : m_camera(scene_config.at("camera_config")), m_screen(scene_config.at("screen_config")) { }

        ~Scene() = default;
        Scene(const Scene& other) = default;
        Scene(Scene&& other) noexcept = default;
        Scene& operator=(const Scene& other) = default;
        Scene& operator=(Scene&& other) noexcept = default;

        [[nodiscard]] Ray getRayFor(double i, double j) const {
            return Ray(m_camera.getPosition(), m_screen.getPointAt(i, j) - m_camera.getPosition());
        }

        [[nodiscard]] const Camera& getCamera() const { return m_camera; }
        [[nodiscard]] const Screen& getScreen() const { return m_screen; }
    };
}
