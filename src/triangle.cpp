#include "triangle.h"

Triangle::Triangle(Vector a, Vector b, Vector c) : a_(a), b_(b), c_(c)
{
    // Initialize vertices
    vertices_.block(0,0,3,1) = a;
    vertices_.block(0,1,3,1) = b;
    vertices_.block(0,2,3,1) = c;

    /**
     * @brief Calculate signed area E_AB(C)
     * 
     * A(xa,ya), B(xb,yb), C(xc,yc)
     * E_AB(C) = (xa-xc)*(yb-yc) - (xb-xc)*(ya-yc)
     */

    /**
     * @brief Calculate outward normal (CCW rotation is considered positive)
     */
    normal_ = (c_-a_).cross(c_-b_);
    assert(normal_.norm() !=0 );
    normal_ = normal_/normal_.norm();

    /**
     * @brief initialize Pose to origin
     */
    pose_.position.setZero();
    pose_.orientation.setIdentity();
    R_.setIdentity();
    T_.setIdentity();

    // Set color to white
    fill_color_.r = 255;
    fill_color_.g = 255;
    fill_color_.b = 255;
    fill_color_.a = 255;

    edge_color_.r = 255;
    edge_color_.g = 255;
    edge_color_.b = 255;
    edge_color_.a = 255;
}



void Triangle::setWorldPose(Pose pose)
{
    pose_ = pose;
    R_ = pose_.orientation.toRotationMatrix();
    T_.block(0,0,3,3) = R_;
    T_.block(0,3,3,1) = pose_.position;
}


void Triangle::setFillColor(int r, int g, int b, int a)
{
    fill_color_.r = r;
    fill_color_.g = g;
    fill_color_.b = b;
    fill_color_.a = a;
}


void Triangle::setEdgeColor(int r, int g, int b, int a)
{
    edge_color_.r = r;
    edge_color_.g = g;
    edge_color_.b = b;
    edge_color_.a = a;
}


Eigen::Matrix3d Triangle::getVertices()
{
    return vertices_;
}



Eigen::Matrix3d Triangle::getVerticesWorldFrame()
{
    Eigen::Matrix3d ver;
    Eigen::Matrix<double,4,3> ver4;
    ver4.setOnes();
    ver4.block(0,0,3,3) = vertices_;
    ver4 = T_*ver4;
    ver = ver4.block(0,0,3,3);
    return ver;
}


Vector Triangle::getNormalWorldFrame()
{
    return rotateVector_(normal_,pose_.orientation);
}



void Triangle::renderObject(Camera& camera, Renderer& renderer, int border_width, int ignore_edge)
{
    // Transform vertices to global frame
    Eigen::Matrix<double,3,-1> ver;
    ver = getVerticesWorldFrame();

    // Direction culling
    if(!camera.isTriangleFacingCamera(ver))
    {
        return;
    }

    // Project to camera
    auto pixels = camera.projectToCamera(ver);
    // std::cout<<"Camera frame : \n"<<pixels<<"\n\n";


    // Render the given primitive onto the window
    renderer.renderTriangle(pixels,fill_color_, edge_color_, border_width, ignore_edge);
}



Vector Triangle::rotateVector_(Vector nor,Eigen::Quaterniond& q)
{
    Eigen::Quaterniond v;
    v.x() = nor(0);
    v.y() = nor(1);
    v.z() = nor(2);
    v.w() = 0;
    v = q*v*q.inverse();
    Vector v_;
    v_(0) = v.x();
    v_(1) = v.y();
    v_(2) = v.z();
    return v_;
}

