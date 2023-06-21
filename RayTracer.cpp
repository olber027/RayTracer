//
// Created by olber on 9/6/2022.
//
#include "RayTracer.h"
#include "json.h"
#include "RandomNumberGenerator.h"

RayTracer::RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
                     const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters)
                     : m_environment(environment_config), m_scene(scene_config), m_image(output_config)
{
    m_samples_per_pixel = ray_tracer_parameters.at("samples_per_pixel");
}

void RayTracer::trace()
{
    double x_step = 1.0 / static_cast<double>(m_image.width());
    double y_step = 1.0 / static_cast<double>(m_image.height());
    static const Color white(255, 255, 255);
    for(int j = 0; j < m_image.height(); j++) {
        for(int i = 0; i < m_image.width(); i++) {
            double u = i * x_step;
            double v = j * y_step;
            Color pixelColor{};
            double per_pixel_fraction = 1.0 / static_cast<double>(m_samples_per_pixel);
            for(int k = 0; k < m_samples_per_pixel; k++)
            {
                double random_u = utility::get_random_number(u, u + x_step);
                double random_v = utility::get_random_number(v, v + y_step);
                Ray ray = m_scene.getRayFor(random_u, random_v);
                // TODO: can do some optimizing to not calculate the intersections here more than once
                std::shared_ptr<Geometry> intersecting_geometry = m_environment.getFirstIntersectedGeometry(ray);
                if(intersecting_geometry == nullptr)
                {
                    pixelColor += m_environment.getBackgroundColor(ray) * per_pixel_fraction;
                    continue;
                }
                std::optional<Point_3> intersection_point = intersecting_geometry->getIntersectionPoint(ray);
                Color shape_color = intersecting_geometry->getColorAt(intersection_point.value());
                double t = ray.getDirection().getUnitVector() * intersecting_geometry->getNormalAt(intersection_point.value());
                pixelColor += Color::blend(white, shape_color, std::clamp(t, 0.0, 1.0)) * per_pixel_fraction;
            }

            m_image.at(i, j) = pixelColor;
        }
        std::cout << j << "/" << m_image.height() << std::endl;
    }
}

