#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"
#include "ray.h"

color lerp_bg_color(const ray& r){
    vec3 unit_dir = unit_vector(r.direction());
    double a = 0.5*(unit_dir.x() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

double hits_sphere(point3 sphere_center, double radius, const ray& r){
    vec3 oc = sphere_center - r.origin();
    double a = dot(r.direction(), r.direction());
    double b = -2.0 * dot(r.direction(), oc);
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0){return -1.0;}
    else{return (-b - std::sqrt(discriminant))/(2.0*a);} //we are only considerfing -b-sqrt(b^2-4ac)/2a bc we dont need to worry about the second solution for now and can assume the closest point
}

int main() {
    //file to write images to
    std::ofstream ofile("image.ppm");
    if (!ofile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    //image and viewport aspect ratios
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    double viewport_width = 2.0;
    double viewport_height = viewport_width / ((double)image_width/image_height); 

    //camera
    double focal_length = 1.0;
    point3 camera_center = point3(0,0,0);

    //viewport vectors
    vec3 viewport_u(viewport_width, 0, 0);
    vec3 viewport_v(0, -viewport_height, 0);

    vec3 delta_viewport_u = viewport_u / image_width;
    vec3 delta_viewport_v = viewport_v / image_height;

    //curr scene objects
    point3 sphere_center(0,0,-1);
    double sphere_r = 0.5;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    ofile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    point3 viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    point3 pixel00_loc = viewport_upper_left + 0.5*(delta_viewport_u + delta_viewport_v);

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {       
            point3 curr_pixel_center = pixel00_loc + i * delta_viewport_u + j * delta_viewport_v;
            vec3 ray_dir = curr_pixel_center - camera_center;

            ray curr_r(camera_center, ray_dir);

            double t = hits_sphere(sphere_center, sphere_r, curr_r);
            if(t > 0){
                vec3 surface_normal = unit_vector(curr_r.at(t) - sphere_center);
                color shading_color(1+surface_normal.x(), 1+surface_normal.y(), 1+surface_normal.z());
                write_color(ofile, 0.5*shading_color);
            }
            else{write_color(ofile, lerp_bg_color(curr_r));}
        }
    }
    std::clog << "\rDone.\n";

}
// make the u and v vectors spanning width and height, and also the deltas
//translate from the camera center to the top left of viewpoint, then get the pixel center from that
//construct a ray vector pointing from camera center to the pixel center

//lerp, making the a y of unit ray vector, and have that value as the pixel color