#pragma once

#include "Scene.h"
#include "Image.h"
#include "Environment.h"
#include "json.h"
#include "RandomNumberGenerator.h"

#include <future>
#include <string>
#include <chrono>
#include <sstream>

using namespace output;
using namespace scene;
using namespace environment;

template<IsFloatingPoint value_type>
class RayTracer
{
private:
    using Point_3 = Point_X<3, value_type>;

    Environment<value_type> m_environment;
    Scene<value_type>       m_scene;
    Image       m_image;
    size_t      m_samples_per_pixel;
    size_t      m_num_threads;
public:

    /*!
     * Construct the ray tracer from the given configurations
     * @param environment_config json config for the environment
     * @param scene_config json config for the scene
     * @param output_config json config for the output
     * @param ray_tracer_parameters json config for the ray tracer parameters
     */
    RayTracer(const nlohmann::json& environment_config, const nlohmann::json& scene_config,
              const nlohmann::json& output_config, const nlohmann::json& ray_tracer_parameters)
            : m_environment(environment_config), m_scene(scene_config), m_image(output_config)
    {
        m_samples_per_pixel = ray_tracer_parameters.at("samples_per_pixel");
        auto num_threads = ray_tracer_parameters.at("number_of_threads").get<int>();
        if(num_threads <= 0) {
            m_num_threads = std::thread::hardware_concurrency();
        } else {
            m_num_threads = num_threads;
        }
    }

    /*!
     * Run the ray tracing algorithm with the current scene and environment, placing the result in the current image object
     */
    void trace()
    {
        value_type x_step = 1.0 / static_cast<value_type>(m_image.width());
        value_type y_step = 1.0 / static_cast<value_type>(m_image.height());
        value_type per_pixel_fraction = 1.0 / static_cast<value_type>(m_samples_per_pixel);
        const Color white(255, 255, 255);

        std::vector<std::future<void>> thread_results(m_num_threads);
        for(int current_index = 0; current_index < m_num_threads; current_index++) {
            thread_results[current_index] = std::async(std::launch::async,
               [this, x_step, y_step, white, per_pixel_fraction, thread_num = current_index]()
               {
                   for(int j = thread_num; j < m_image.height(); j += static_cast<int>(m_num_threads))
                   {
                       value_type v = j * y_step;

                       for(int i = 0; i < m_image.width(); i++)
                       {
                           value_type u = i * x_step;
                           Color pixelColor{};

                           for(int _ = 0; _ < m_samples_per_pixel; _++)
                           {
                               value_type random_u = utility::get_random_number(u, u + x_step);
                               value_type random_v = utility::get_random_number(v, v + y_step);
                               Ray ray = m_scene.getRayFor(random_u, random_v);
                               std::shared_ptr<Geometry<value_type>> intersecting_geometry = m_environment.getFirstIntersectedGeometry(ray);

                               if(intersecting_geometry == nullptr)
                               {
                                   pixelColor += m_environment.getBackgroundColor(ray) * per_pixel_fraction;
                                   continue;
                               }

                               std::optional<Point_3> intersection_point = intersecting_geometry->getIntersectionPoint(ray);
                               Color shape_color = intersecting_geometry->getColorAt(intersection_point.value());
                               value_type t = ray.getDirection().getUnitVector() * intersecting_geometry->getNormalAt(intersection_point.value());
                               pixelColor += Color::blend(white, shape_color,
                                                          std::clamp(t, static_cast<value_type>(0.0), static_cast<value_type>(1.0))) * per_pixel_fraction;
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

    [[nodiscard]] const Image& getImage() { return m_image; }
    [[nodiscard]] Image getImage() const  { return m_image; }

    [[nodiscard]] const Scene<value_type>& getScene() { return m_scene; }
    [[nodiscard]] Scene<value_type> getScene() const  { return m_scene; }

    [[nodiscard]] const Environment<value_type>& getEnvironment() { return m_environment; }
    [[nodiscard]] Environment<value_type> getEnvironment() const  { return m_environment; }
};
