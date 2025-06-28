#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 intersect_point;
        vec3 normal;
        double t;
};

class hittable{
    public:
        virtual ~hittable() = default;

        // =0 makes this a purely virtual fucntion
        virtual bool hit(const ray& r, double ray_tmin, double, double ray_tmax, hit_record& rec) const = 0;
};

#endif
