#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray& r, double ray_tmin, double, double ray_tmax, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        double a = dot(r.direction(), r.direction());
        double b = -2.0 * dot(r.direction(), oc);
        double c = dot(oc, oc) - radius*radius;
        double discriminant = b*b - 4*a*c;

        if(discriminant < 0){return false;}

        //the smallest root, and then check if its in bound, return tru if it is 
        //if not, check the other root and if its in bound
        //return false if none of these are true
        double root = std::fmin(-b+std::sqrt(discriminant)/(2*a), -b+std::sqrt(discriminant)/(2*a));
        if(root <= ray_tmin || root >= ray_tmax){
            root = std::fmax(-b+std::sqrt(discriminant)/(2*a), -b+std::sqrt(discriminant)/(2*a));
            if(root <= ray_tmin || root >= ray_tmax){return false;}
        }

        rec.t = root;
        rec.intersect_point = r.at(rec.t);
        rec.normal = unit_vector(rec.intersect_point - center);

        return true;
    }

    private:
        point3 center;
        double radius;

    };

#endif