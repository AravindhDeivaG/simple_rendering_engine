#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "typedefs.h"
#include "camera.h"
#include "renderer.h"
#include <iostream>

class Triangle
{
    public :
    Triangle(Vector, Vector, Vector);

    ~Triangle(){}

    /**
     * @brief function to set world offset
     */
    void setWorldPose(Pose);

    /**
     * @brief function to set fill color
     */
    void setFillColor(int, int, int, int);

    /**
     * @brief function to set edge color
     */
    void setEdgeColor(int, int, int, int);

    /**
     * @brief Function to get vertices of the triangle WRT it's own frame
     */
    Eigen::Matrix<double,3,3> getVertices();

    /**
     * @brief Function to get vertices of the triangle WRT world frame
     */
    Eigen::Matrix<double,3,3> getVerticesWorldFrame();

    /**
     * @brief Function to get normal about world frame
     */
    Vector getNormalWorldFrame();

    /**
     * @brief Function to render traingle object in the camera
     * 
     * edge : param to ignoer a particular edge for shading edges while rendering
     *      0 : No edges are ignored
     *      1 : Edge AB is ignored
     *      2 : Edge BC is ignored
     *      3 : Edge CA is ignored
     */
    void renderObject(Camera&, Renderer&, int border_width=1, int ignore_edge = 0);

    private :
    // Vertices of triangle
    Vector a_, b_, c_;
    Eigen::Matrix<double,3,3> vertices_;

    // Pose wrt origin
    Pose pose_;
    
    // Outward normal
    Eigen::Vector3d normal_;

    // Rotation matrix
    Eigen::Matrix3d R_;

    // Rigid body transformation matrix
    Eigen::Matrix4d T_;

    // Edge color
    Color fill_color_;

    // Fill color
    Color edge_color_;

    /**
     * @brief function to rotate normal vector with the world
     */
    Vector rotateVector_(Vector ,Eigen::Quaterniond&);
};

#endif