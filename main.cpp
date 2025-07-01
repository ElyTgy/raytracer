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


int main() {
    //file to write images to
    std::ofstream ofile("image_diffused.ppm");
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
    Hittable_list world;
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    camera cam;
    cam.render(world, ofile, lerp_bg_color); // <-- This actually writes to the file!

    return 0;
    
}
// make the u and v vectors spanning width and height, and also the deltas
//translate from the camera center to the top left of viewpoint, then get the pixel center from that
//construct a ray vector pointing from camera center to the pixel center

//lerp, making the a y of unit ray vector, and have that value as the pixel color
