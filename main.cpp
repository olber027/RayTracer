#include <memory>
#include <thread>
#include <chrono>

#include "Image.h"
#include "Color.h"
#include "Ray.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Plane.h"
#include "ImageWriter_I.h"
#include "ImageWriterBuilder.h"
#include "ArgParse.h"
#include "RayTracer.h"
#include "RandomNumberGenerator.h"

#include <SFML/Graphics.hpp>

using namespace output;
using namespace color_core;
using namespace linear_algebra_core;
using namespace geometry;

std::string getSuffix(const std::string& file_path) {
    return file_path.substr(file_path.find_last_of('.') + 1);
}

struct RayTracerArgs : public argparse::Args {
    std::string &output_config = kwarg("o,output", "The config file for the output image(s)");
    std::string &scene_config = kwarg("s,scene", "The config file for the scene (i.e. camera, viewport, etc.)");
    std::string &environment_config = kwarg("e,environment", "config file defining all the scene geometry and the environment");
    std::string &ray_tracer_parameters = kwarg("p,parameters", "config file containing all the ray tracer parameters");
};

void drawImage(sf::RenderWindow& window, const Image& image)
{
    for(int j = 0; j < image.height(); j++) {
        sf::VertexArray pixels(sf::Points, image.width());
        for(int i = 0; i < image.width(); i++) {
            pixels[i].position = sf::Vector2f(static_cast<float>(i)+1, static_cast<float>(j)+1);
            color_core::Color c = image.at(i, j);
            pixels[i].color = sf::Color(c.R(), c.G(), c.B());;
        }
        window.draw(pixels);
    }
}

int main(int argc, char** argv)
{
    auto args = argparse::parse<RayTracerArgs>(argc, argv);
    nlohmann::json output_json, scene_json, environment_json, ray_tracer_parameter_json;
    {
        std::ifstream output_config_file(args.output_config);
        if(output_config_file.is_open()) {
            output_config_file >> output_json;
        }

        std::ifstream scene_config_file(args.scene_config);
        if(scene_config_file.is_open()) {
            scene_config_file >> scene_json;
        }

        std::ifstream environment_config_file(args.environment_config);
        if(environment_config_file.is_open()) {
            environment_config_file >> environment_json;
        }

        std::ifstream ray_tracer_parameter_config_file(args.ray_tracer_parameters);
        if(ray_tracer_parameter_config_file.is_open()) {
            ray_tracer_parameter_config_file >> ray_tracer_parameter_json;
        }
    }

    RayTracer tracer(environment_json, scene_json, output_json, ray_tracer_parameter_json);
    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
    tracer.trace();
    auto end = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto difference = end - start;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(difference).count() << std::endl;

    std::string output_file_path = output_json.at("file_path").get<std::string>();
    std::unique_ptr<ImageWriter_I> image_writer = ImageWriterBuilder::createWriter(getSuffix(output_file_path));
    image_writer->write(tracer.getImage(), output_file_path);

    auto window = sf::RenderWindow{ sf::VideoMode(tracer.getImage().width(), tracer.getImage().height()), "Shatterpoint" };
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    // the event processing loop
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        drawImage(window, tracer.getImage());
        window.display();
    }

    return 0;
}

