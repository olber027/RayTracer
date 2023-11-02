#include <thread>
#include <chrono>
#include "Color.h"
#include "ImageWriter_I.h"
#include "ImageWriterBuilder.h"
#include "ArgParse.h"
#include "RayTracer.h"
#include "RandomNumberGenerator.h"

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
    utility::initialize_randomizer(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    RayTracer<double> tracer(environment_json, scene_json, output_json, ray_tracer_parameter_json);
    auto start = std::chrono::high_resolution_clock::now();
    tracer.trace();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


    std::string output_file_path = output_json.at("file_path").get<std::string>();
    std::unique_ptr<ImageWriter_I> image_writer = ImageWriterBuilder::createWriter(getSuffix(output_file_path));
    image_writer->write(tracer.getImage(), output_file_path);

    return 0;
}

