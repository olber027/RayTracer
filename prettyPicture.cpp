#include <memory>

#include "Image.h"
#include "Color.h"
#include "Ray.h"
#include "Geometry.h"
#include "Sphere.h"
#include "ImageWriter_I.h"
#include "PPMImageWriter.h"

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
    double t = ((ray.getDirection().normalize()[1] + 1.0) / 2.0);
    static Color white(216, 232, 255);
    static Color blue(122, 178, 255);
    return Color::blend(blue, white, t);
}

Color getRayColor(const Ray& ray, const std::vector<std::shared_ptr<Geometry>>& sceneGeometry)
{
    for(const auto& shape : sceneGeometry) {
        if(shape->intersects(ray)) {
            return Color(255, 0, 0);
        }
    }
    return getBackgroundColor(ray);
}

int main() {
    constexpr int COLOR_RANGE = 255;
    constexpr double ASPECT_RATIO = 16.0 / 9.0;
    constexpr int WIDTH = 400;
    constexpr int HEIGHT = static_cast<int>(WIDTH / ASPECT_RATIO);

    constexpr double VIEWPORT_HEIGHT = 2.0;
    constexpr double VIEWPORT_WIDTH = ASPECT_RATIO / VIEWPORT_HEIGHT;
    constexpr double FOCAL_LENGTH = 1.0;

    Point_3 origin {0.0, 0.0, 0.0};
    Vector_3 horizontal_axis {VIEWPORT_WIDTH, 0.0, 0.0};
    Vector_3 vertical_axis {0.0, VIEWPORT_HEIGHT, 0.0};
    Point_3 lower_left_corner = origin - (horizontal_axis / 2) - (vertical_axis / 2) - Vector_3(0.0, 0.0, FOCAL_LENGTH);

    std::vector<std::shared_ptr<Geometry>> scene_geometry;
    populateScene(scene_geometry);

    Image image(WIDTH, HEIGHT, COLOR_RANGE);
    for(int j = 0; j < HEIGHT; j++) {
        for(int i = 0; i < WIDTH; i++) {
            double u = i / (WIDTH - 1.0);
            double v = j / (HEIGHT - 1.0);
            Ray ray(origin, lower_left_corner + (u * horizontal_axis) + (v * vertical_axis) - origin);
            image.at(i, j) = getRayColor(ray, scene_geometry);
        }
    }

    std::unique_ptr<ImageWriter_I> image_writer = std::make_unique<PPMImageWriter>();
    image_writer->write(image, R"(C:\Users\olber\CLionProjects\RayTracer\created_images\test.ppm)");

	return 0;
}

