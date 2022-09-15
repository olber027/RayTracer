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
    RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
              const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters);

    void trace();

    [[nodiscard]] const Image& getImage() { return m_image; }
    [[nodiscard]] Image getImage() const { return m_image; }

    const Scene& getScene() { return m_scene; }
    [[nodiscard]] Scene getScene() const { return m_scene; }

    const Environment& getEnvironment() { return m_environment; }
    [[nodiscard]] Environment getEnvironment() const { return m_environment; }
};
