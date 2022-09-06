#include <memory>

#include "Image.h"
#include "Color.h"
#include "Ray.h"
#include "Geometry.h"
#include "Sphere.h"
#include "ImageWriter_I.h"
#include "ImageWriterBuilder.h"
#include "ArgParse.h"

using namespace output;
using namespace color_core;
using namespace linear_algebra_core;
using namespace geometry;

void populateScene(std::vector<std::shared_ptr<Geometry>>& sceneGeometry)
{
    sceneGeometry.push_back(std::make_shared<Sphere>(Point_3(0.0, 0.0, -1.0), 0.25));
}

Color getBackgroundColor(const Ray& ray)
{
    double t = (ray.getDirection().normalize()[1] + 1.0) / 2.0;
    static Color white(216, 232, 255);
    static Color blue(122, 178, 255);
    return Color::blend(blue, white, t);
}

Color getRayColor(const Ray& ray, const std::vector<std::shared_ptr<Geometry>>& sceneGeometry)
{
//    for(const auto& shape : sceneGeometry) {
//        if(shape->intersects(ray)) {
//            return Color(255, 0, 0);
//        }
//    }
    return getBackgroundColor(ray);
}

std::string getSuffix(const std::string& file_path) {
    return file_path.substr(file_path.find_last_of('.') + 1);
}

struct RayTracerArgs : public argparse::Args {
    std::string &output_config = kwarg("o,output", "The config file for the output image(s)");
    std::string &scene_config = kwarg("s,scene", "The config file for the scene (i.e. camera, viewport, etc.)");
    std::string &geometry_config = kwarg("g,geometry", "config file defining all the scene geometry");
};

int main(int argc, char** argv) {
    auto args = argparse::parse<RayTracerArgs>(argc, argv);
    nlohmann::json output_json;
    {
        std::ifstream output_config_file(args.output_config);
        output_config_file >> output_json;
    }
    Image image(output_json);

    double VIEWPORT_HEIGHT = 2.0;
    double VIEWPORT_WIDTH = image.aspectRatio() * VIEWPORT_HEIGHT;
    double FOCAL_LENGTH = 1.0;

    Point_3 origin {0.0, 0.0, 0.0};
    Vector_3 horizontal_axis {VIEWPORT_WIDTH, 0.0, 0.0};
    Vector_3 vertical_axis {0.0, VIEWPORT_HEIGHT, 0.0};
    Point_3 lower_left_corner = origin - (horizontal_axis / 2) - (vertical_axis / 2) - Vector_3(0.0, 0.0, FOCAL_LENGTH);

    std::vector<std::shared_ptr<Geometry>> scene_geometry;
    populateScene(scene_geometry);

    for(int j = image.height() - 1; j >= 0; j--) {
        for(int i = 0; i < image.width(); i++) {
            double u = double(i) / (image.width() - 1);
            double v = double(j) / (image.height() - 1);
            Ray ray(origin, lower_left_corner + (u * horizontal_axis) + (v * vertical_axis) - origin);
            image.at(i, j) = getRayColor(ray, scene_geometry);
        }
    }

    std::string output_file_path = output_json.at("file_path");
    std::unique_ptr<ImageWriter_I> image_writer = ImageWriterBuilder::createWriter(getSuffix(output_file_path));
    image_writer->write(image, output_file_path);

	return 0;
}

