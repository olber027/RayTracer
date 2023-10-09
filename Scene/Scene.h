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
        ~Scene() = default;
        Scene(const Scene& other) = default;
        Scene(Scene&& other) noexcept = default;
        Scene& operator=(const Scene& other) = default;
        Scene& operator=(Scene&& other) noexcept = default;

        /*!
         * Constructs a Scene from the given \p scene_config
         * @param scene_config the json config for the scene
         */
        explicit Scene(const nlohmann::json& scene_config) {
            fromJson(scene_config);
        }

        /*!
         * Get the ray produced by tracing from the camera position, to the corresponding \p i and \p j coordinate of the screen.
         * @param i value for the x coordinate of the screen.
         * @param j value for the y coordinate of the screen.
         * @return The resulting Ray
         */
        [[nodiscard]] Ray getRayFor(double i, double j) const {
            return Ray(m_camera.getPosition(), m_screen.getPointAt(i, j) - m_camera.getPosition());
        }

        /*!
         * @return the Camera object
         */
        [[nodiscard]] const Camera& getCamera() const { return m_camera; }

        /*!
         * @return the Scene object
         */
        [[nodiscard]] const Screen& getScreen() const { return m_screen; }

        void fromJson(const nlohmann::json& scene_json)
        {
            nlohmann::json camera_json, screen_json;

            try {
                camera_json = scene_json.at("camera_config");
            } catch(std::exception& e) {
                throw std::invalid_argument("Scene config must contain a 'camera_config' key");
            }

            try {
                screen_json = scene_json.at("screen_config");
            } catch(std::exception& e) {
                throw std::invalid_argument("Scene config must contain a 'screen_config' key");
            }

            m_camera.fromJson(camera_json);
            m_screen.fromJson(screen_json);
        }
    };
}
