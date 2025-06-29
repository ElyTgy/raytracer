#include <iostream>
#include <fstream>
#include <memory>
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hittable.h"
#include "hittablelist.h"
#include "utility.h"
#include "camera.h"

Color lerp_bg_color(const Ray& r) {
    Vec3 unit_dir = unit_vector(r.direction());
    double a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
    std::ofstream ofile("image.ppm");
    if (!ofile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    Hittable_list world;
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    camera cam;
    cam.render(world, ofile, lerp_bg_color);
}