#pragma once

#include "Scene.h"
#include "Image.h"
#include "Environment.h"
#include "Environment.h"

using namespace output;
using namespace scene;
using namespace environment;

class RayTracer
{
private:
    Environment m_environment;
    Scene       m_scene;
    Image       m_image;
    size_t m_samples_per_pixel;
public:
    /*!
     * Construct the ray tracer from the given configurations
     * @param environment_config json config for the environment
     * @param scene_config json config for the scene
     * @param output_config json config for the output
     * @param ray_tracer_parameters json config for the ray tracer parameters
     */
    RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
              const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters);

    /*!
     * Run the ray tracing algorithm with the current scene and environment, placing the result in the current image object
     */
    void trace();

    [[nodiscard]] inline const Image& getImage() { return m_image; }
    [[nodiscard]] inline Image getImage() const { return m_image; }

    [[nodiscard]] inline const Scene& getScene() { return m_scene; }
    [[nodiscard]] inline Scene getScene() const { return m_scene; }

    [[nodiscard]] inline const Environment& getEnvironment() { return m_environment; }
    [[nodiscard]] inline Environment getEnvironment() const { return m_environment; }
};
