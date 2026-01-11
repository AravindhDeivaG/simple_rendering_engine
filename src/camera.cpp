#include "camera.h"

Camera::Camera(double fx, double fy, double cx, double cy)
{
    // Populate intrinsic matrix
    C_(0,0) = fx;
    C_(1,1) = fy;
    C_(0,2) = cx;
    C_(1,2) = cy;

    // Set Extrinsic matrix to Identity
    T_.setIdentity();
}

Camera::~Camera()
{

}

void Camera::setCameraPose(Pose p)
{
    T_.block(0,0,3,3) = p.orientation.toRotationMatrix();
    T_.block(0,3,3,1) = p.position;
}  


bool Camera::isTriangleFacingCamera(Eigen::Matrix<double,3,-1> points)
{
    // Project points to camera frame
    Eigen::Matrix<double,4,-1> points4;
    points4.resize(4,points.cols());
    points4.setOnes();
    points4.block(0,0,3,points.cols()) = points;
    points4 = T_.inverse()*points4;
    points = points4.block(0,0,3,points4.cols());

    Vector a,b,c, normal;
    a = points.block(0,0,3,1);
    b = points.block(0,1,3,1);
    c = points.block(0,2,3,1);
    normal = (c-a).cross(c-b);
    // std::cout<<"Normal : "<<normal.transpose()<<"\n\n";
    if(a.dot(normal)<0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


Eigen::Matrix<double,2,-1> Camera::projectToCamera(Eigen::Matrix<double,3,-1>& points)
{
    // Project points to camera frame
    Eigen::Matrix<double,4,-1> points4;
    points4.resize(4,points.cols());
    points4.setOnes();
    points4.block(0,0,3,points.cols()) = points;
    points4 = T_.inverse()*points4;
    points = points4.block(0,0,3,points4.cols());

    // Z normalize the values
    Eigen::Matrix<double,1,-1> z;
    z = points.block(2,0,1,points.cols());
    points = points.array().rowwise()/z.array();

    // Convert to pixels
    Eigen::Matrix<double,2,-1> pixels;
    pixels = C_*points;

    return pixels;
}