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

bool hits_sphere(point3 sphere_center, double radius, const ray& r){
    vec3 oc = sphere_center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
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

            //color curr i,j
            //write_color(std::cout, lerp_bg_color(curr_r));
            if(hits_sphere(point3(0,0,-1), 0.7, curr_r)){
                write_color(ofile, color(1, 0, 0));
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