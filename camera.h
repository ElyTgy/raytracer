#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "utility.h"
#include "ray.h"
#include <ostream>


Color lerp_bg_color(const Ray& r){
    Vec3 unit_dir = unit_vector(r.direction());
    double a = 0.5*(unit_dir.x() + 1.0);
    return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
}

class camera {
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height;
    double viewport_width = 2.0;
    double viewport_height;
    double focal_length = 1.0;
    Point3 camera_center = Point3(0,0,0);
    int    samples_per_pixel = 10;

    Vec3 viewport_u;
    Vec3 viewport_v;
    Vec3 delta_viewport_u;
    Vec3 delta_viewport_v;
    Point3 viewport_upper_left;
    Point3 pixel00_loc;

    camera() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        viewport_height = viewport_width / (static_cast<double>(image_width) / image_height);

        viewport_u = Vec3(viewport_width, 0, 0);
        viewport_v = Vec3(0, -viewport_height, 0);

        delta_viewport_u = viewport_u / image_width;
        delta_viewport_v = viewport_v / image_height;

        viewport_upper_left = camera_center - Vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(delta_viewport_u + delta_viewport_v);
    }

    void render(const Hittable& world, std::ostream& out, Color (*bg_color)(const Ray&)) {
        out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                Point3 curr_pixel_center = pixel00_loc + i * delta_viewport_u + j * delta_viewport_v;
                Vec3 ray_dir = curr_pixel_center - camera_center;
                Ray curr_r(camera_center, ray_dir);

                Color c = ray_color(curr_r, world);
                write_color(out, c);

            }
        }
        std::clog << "\rDone.\n";
    }

    Color ray_color(const Ray& r, const Hittable& world) const {
        hit_record rec;
        if (world.hit(r, 0, infinity, rec)) {
            Vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(Ray(rec.intersect_point, direction), world);
        } else {
            return lerp_bg_color(r);
        }
    }
};

#endif