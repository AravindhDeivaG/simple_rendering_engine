#ifndef CAMERA_H_
#define CAMERA_H_

#include "typedefs.h"
#include <iostream>

class Camera
{
    public :
    Camera(double, double, double, double);

    ~Camera();

    /**
     * @brief Set camera extrinsic params
     */
    void setCameraPose(Pose);
    
    /**
     * @brief Function to check if object is facing camera
     */
    bool isTriangleFacingCamera(Eigen::Matrix<double,3,-1>);

    /**
     * @brief Function to project images onto camera frame
     */
    Eigen::Matrix<double,2,-1> projectToCamera(Eigen::Matrix<double,3,-1>&);

    private :
    /**
     * @brief Intrinsic camera matrix
     */
    Eigen::Matrix<double,2,3> C_;

    /**
     * @brief Extrinsic camera matrix
     */
    Eigen::Matrix4d T_;
};

#endif