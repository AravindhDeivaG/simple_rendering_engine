#ifndef LOCUS_H_
#define LOCUS_H_

#include "typedefs.h"
#include "camera.h"
#include "renderer.h"
#include <iostream>

class Locus
{
    public :
    Locus();

    ~Locus();

    /**
     * @brief Function to add a point to the locus 
     */
    void addPoint(Vector);

    /**
     * @brief Function to set pose of locus
     */
    void setWorldPose(Pose);

    /**
     * @brief Function to set color of locus
     */
    void setColor(Color);

    /**
     * @brief Function to render the locus
     */
    void renderObject(Camera&, Renderer&, int line_width=1);

    private:
    /**
     * @brief Vector of points representing the locus
     */
    Eigen::Matrix<double,3,-1> points_;

    /**
     * @brief Pose of locus
     */
    Pose pose_;

    /**
     * @brief World frame transformation matrix
     */
    Eigen::Matrix4d T_;

    /**
     * @brief World frame rotation matrix
     */
    Eigen::Matrix3d R_;

    /**
     * @brief locus color
     */
    Color color_;

    /**
     * @brief Thickness of locus
     */
    int line_width_;

    /**
     * @brief Pixels of locus in camera frame
     */
    Eigen::Matrix<double,2,-1> pixels_;

    /**
     * @brief Slope of all line segments (dx,dy)
     */
    std::vector<Eigen::Vector2d> slopes_;

    /**
     * @brief Function to calculate slopes of all line segments in the pixels
     */
    void calculateSlopes_();

    /**
     * @brief Function to render rectangles
     */
    void renderRectangles_(Renderer&);

    /**
     * @brief Function to fill triangular cracks
     */
    void fillTriangularCracks_(Renderer&);
};

#endif