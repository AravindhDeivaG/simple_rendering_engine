#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>

using Vector = Eigen::Vector3d;
using Pixel = Eigen::Vector2d;
using Orientation = Eigen::Quaterniond;

struct Pose
{
    Vector position;
    Orientation orientation;
};

#endif