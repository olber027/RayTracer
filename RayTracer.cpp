//
// Created by olber on 9/6/2022.
//
#include "RayTracer.h"
#include "json.h"

RayTracer::RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
                     const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters)
                     : m_environment(environment_config), m_scene(scene_config), m_image(output_config)
{
    m_samples_per_pixel = ray_tracer_parameters.at("samples_per_pixel");
}

void RayTracer::trace()
{
    double x_step = 1.0 / m_image.width();
    double y_step = 1.0 / m_image.height();

    for(int j = 0; j < m_image.height(); j++) {
        for(int i = 0; i < m_image.width(); i++) {
            double u = i * x_step;
            double v = j * y_step;
            Color pixelColor{};
            // skip sampling for now
            Ray ray = m_scene.getRayFor(u, v);
            // TODO: can do some optimizing to not calculate the intersections here more than once
            std::shared_ptr<Geometry> intersecting_geometry = m_environment.getFirstIntersectedGeometry(ray);
            if(intersecting_geometry == nullptr)
            {
                m_image.at(i, j) = m_environment.getBackgroundColor(ray);
                continue;
            }
            std::optional<Point_3> intersection_point = intersecting_geometry->intersectsAt(ray);
            m_image.at(i, j) = intersecting_geometry->getColorAt(intersection_point.value());
        }
    }
}

