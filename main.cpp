#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hittable.h"
#include "utility.h"


Color lerp_bg_color(const Ray& r){
    Vec3 unit_dir = unit_vector(r.direction());
    double a = 0.5*(unit_dir.x() + 1.0);
    return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
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
    Point3 camera_center = Point3(0,0,0);

    //viewport vectors
    Vec3 viewport_u(viewport_width, 0, 0);
    Vec3 viewport_v(0, -viewport_height, 0);

    Vec3 delta_viewport_u = viewport_u / image_width;
    Vec3 delta_viewport_v = viewport_v / image_height;

    //curr scene objects
    /*Point3 Sphere_center(0,0,-1);
    double Sphere_r = 0.5;*/
    Sphere sphere(Vec3(0,0,-1), 0.5);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    ofile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    Point3 viewport_upper_left = camera_center - Vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    Point3 pixel00_loc = viewport_upper_left + 0.5*(delta_viewport_u + delta_viewport_v);

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {       
            Point3 curr_pixel_center = pixel00_loc + i * delta_viewport_u + j * delta_viewport_v;
            Vec3 ray_dir = curr_pixel_center - camera_center;

            Ray curr_r(camera_center, ray_dir);
            
            hit_record rec;
            if(sphere.hit(curr_r, 0, infinity, rec)){
                Color shading_color(1+rec.normal.x(), 1+rec.normal.y(), 1+rec.normal.z());
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
