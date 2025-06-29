#ifndef Hittable_H
#define Hittable_H

#include "ray.h"

class hit_record {
    public:
        Point3 intersect_point;
        Vec3 normal;
        double t;
        bool front_face;

        void set_front_face(const Ray& r, const Vec3& outward_normal){
            front_face = dot(r.direction(), outward_normal) < 0;
            if(front_face){normal = outward_normal;}
            else{normal = -outward_normal;}
        }
};

class Hittable{
    public:
        virtual ~Hittable() = default;

        // =0 makes this a purely virtual fucntion
        virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
