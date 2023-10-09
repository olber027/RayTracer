//
// Created by olber on 9/6/2022.
//
#include "RayTracer.h"
#include "json.h"
#include "RandomNumberGenerator.h"

#include <future>

RayTracer::RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
                     const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters)
                     : m_environment(environment_config), m_scene(scene_config), m_image(output_config)
{
    m_samples_per_pixel = ray_tracer_parameters.at("samples_per_pixel");
    m_num_threads = ray_tracer_parameters.at("number_of_threads");
}

void RayTracer::trace()
{
    double x_step = 1.0 / static_cast<double>(m_image.width());
    double y_step = 1.0 / static_cast<double>(m_image.height());
    double per_pixel_fraction = 1.0 / static_cast<double>(m_samples_per_pixel);
    const Color white(255, 255, 255);

    std::vector<std::vector<int>> thread_indices(m_num_threads);
    int row_index = 0;
    int thread_index = 0;
    while(row_index < m_image.height()) {
        thread_indices[thread_index++].push_back(row_index++);
        if(thread_index >= m_num_threads) {
            thread_index = 0;
        }
    }

    std::vector<std::future<void>> thread_results(m_num_threads);
    for(int i = 0; i < m_num_threads; i++) {
        thread_results[i] = std::async(std::launch::async,
        [this, x_step, y_step, white, per_pixel_fraction, indices = thread_indices[i]]()
            {
                for(const int j : indices)
                {
                    double v = j * y_step;

                    for(int i = 0; i < m_image.width(); i++)
                    {
                        double u = i * x_step;
                        Color pixelColor{};

                        for(int _ = 0; _ < m_samples_per_pixel; _++)
                        {
                            double random_u = utility::get_random_number(u, u + x_step);
                            double random_v = utility::get_random_number(v, v + y_step);
                            Ray ray = m_scene.getRayFor(random_u, random_v);
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
                }
        });
    }

    for(int i = 0; i < m_num_threads; i++) {
        thread_results[i].get();
    }
}

