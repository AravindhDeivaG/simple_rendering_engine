#include "locus.h"

Locus::Locus()
{
    pose_.position.setZero();
    pose_.orientation.setIdentity();
    T_.setIdentity();
    R_.setIdentity();
}


Locus::~Locus()
{

}


void Locus::addPoint(Vector point)
{
    points_.conservativeResize(Eigen::NoChange,points_.cols()+1);
    points_.col(points_.cols()-1) = point;
}


void Locus::setWorldPose(Pose pose)
{
    pose_ = pose;
    R_ = pose_.orientation.toRotationMatrix();
    T_.block(0,0,3,3) = R_;
    T_.block(0,3,3,1) = pose_.position;
}


void Locus::setColor(Color color)
{
    color_ = color;
}


void Locus::renderObject(Camera& camera, Renderer& renderer, int line_width)
{
    // Project locus to world coordinates
    Eigen::Matrix<double,4,-1> points4_;
    points4_.resize(4,points_.cols());
    points4_.setOnes();
    points4_.block(0,0,3,points4_.cols()) = points_;
    points4_ = T_*points4_;
    Eigen::Matrix<double,3,-1> world_points = points4_.block(0,0,3,points4_.cols());
    // std::cout<<"World points : \n"<<world_points<<"\n";

    // Project points to camera frame
    pixels_ = camera.projectToCamera(world_points);
    // std::cout<<"Pixel points : \n"<<pixels_<<"\n";

    // Calculate slope of all line segments in the pixels
    slopes_.clear();
    calculateSlopes_();
    // std::cout<<"Calculate slopes over\n";

    // Construct rectangles of thickness 2 pixels for all line segments
    line_width_ = line_width;
    renderRectangles_(renderer);
    // std::cout<<"render Rectangles over\n";

    // Fill triangular cracks
    fillTriangularCracks_(renderer);
    // std::cout<<"Fill triangular cracks over\n";

}



void Locus::calculateSlopes_()
{
    for(int i=0;i<int(pixels_.cols()-1);i++)
    {
        Eigen::Vector2d slope;
        slope = pixels_.block(0,i+1,2,1)-pixels_.block(0,i,2,1);
        if(slope.norm()!=0)
        {
            slope = slope/slope.norm(); 
        }
        slopes_.emplace_back(slope); 
    // std::cout<<"Slopes size : "<<slopes_.size()-1<<"\n";
    }
}


void Locus::renderRectangles_(Renderer& renderer)
{
    Eigen::Matrix<double,2,-1> triangle_pixels;
    triangle_pixels.resize(2,3);
    Eigen::Vector2d slope_per_;
    for(int i=0;i<int(pixels_.cols()-1);i++)
    {
        // Render two separate triangles to render the rectangle

        slope_per_[0] = -1*slopes_[i](1);
        slope_per_[1] = slopes_[i](0);

        // Rectangle 1
        triangle_pixels.block(0,0,2,1) = pixels_.block(0,i,2,1) + slope_per_*line_width_/2;
        triangle_pixels.block(0,1,2,1) = pixels_.block(0,i,2,1) - slope_per_*line_width_/2;
        triangle_pixels.block(0,2,2,1) = pixels_.block(0,i+1,2,1) + slope_per_*line_width_/2;
        renderer.renderTriangle(triangle_pixels,color_,color_);

        // Rectangle 2
        triangle_pixels.block(0,0,2,1) = pixels_.block(0,i,2,1) - slope_per_*line_width_/2;
        triangle_pixels.block(0,1,2,1) = pixels_.block(0,i+1,2,1) + slope_per_*line_width_/2;
        triangle_pixels.block(0,2,2,1) = pixels_.block(0,i+1,2,1) - slope_per_*line_width_/2;
        renderer.renderTriangle(triangle_pixels,color_,color_);
    }
}


void Locus::fillTriangularCracks_(Renderer& renderer)
{
    Eigen::Matrix<double,2,-1> triangle_pixels;
    triangle_pixels.resize(2,3);
    Eigen::Vector2d slope_per1_, slope_per2_;
    for(int i=0;i<int(slopes_.size()-1);i++)
    {
        slope_per1_[0] = -1*slopes_[i](1);
        slope_per1_[1] = slopes_[i](0);

        slope_per2_[0] = -1*slopes_[i+1](1);
        slope_per2_[1] = slopes_[i+1](0);

        // Fill triangle 1
        triangle_pixels.block(0,0,2,1) = pixels_.block(0,i+1,2,1) + slope_per1_*line_width_/2;
        triangle_pixels.block(0,1,2,1) = pixels_.block(0,i+1,2,1) + slope_per2_*line_width_/2;
        triangle_pixels.block(0,2,2,1) = pixels_.block(0,i+1,2,1);
        renderer.renderTriangle(triangle_pixels,color_,color_);

        // Fill triangle 2
        triangle_pixels.block(0,0,2,1) = pixels_.block(0,i+1,2,1) - slope_per1_*line_width_/2;
        triangle_pixels.block(0,1,2,1) = pixels_.block(0,i+1,2,1) - slope_per2_*line_width_/2;
        triangle_pixels.block(0,2,2,1) = pixels_.block(0,i+1,2,1);
        renderer.renderTriangle(triangle_pixels,color_,color_);
    }
}


