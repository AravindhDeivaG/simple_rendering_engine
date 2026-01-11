#include "square.h"

// Helper function for creating members with non default constructors
Triangle getTriangle1(Pose p, double len)
{
    Vector a = {-1,1,0};
    Vector b = {-1,-1,0};
    Vector c = {1,-1,0};

    a*=len/2;
    b*=len/2;
    c*=len/2;

    Eigen::Matrix3d R = p.orientation.toRotationMatrix();
    a = R*a + p.position;
    b = R*b + p.position;
    c = R*c + p.position;
    return Triangle(a,b,c);
}


Triangle getTriangle2(Pose p, double len)
{
    Vector a = {1,1,0};
    Vector b = {-1,1,0};
    Vector c = {1,-1,0};

    a*=len/2;
    b*=len/2;
    c*=len/2;

    Eigen::Matrix3d R = p.orientation.toRotationMatrix();
    a = R*a + p.position;
    b = R*b + p.position;
    c = R*c + p.position;
    return Triangle(a,b,c);
}

//------------------------------------------------------

Square::Square(Pose p, double len) : 

        t1_(getTriangle1(p,len)), 
        t2_(getTriangle2(p,len))
{
    // Set border width to 1
    border_width_ = 1;
}

Square::~Square()
{

}


void Square::setFillColor(int r, int g, int b, int a)
{
    fill_color_.r = r;
    fill_color_.g = g;
    fill_color_.b = b;
    fill_color_.a = a;

    t1_.setFillColor(r,g,b,a);
    t2_.setFillColor(r,g,b,a);
}


void Square::setEdgeColor(int r, int g, int b, int a)
{
    edge_color_.r = r;
    edge_color_.g = g;
    edge_color_.b = b;
    edge_color_.a = a;

    t1_.setEdgeColor(r,g,b,a);
    t2_.setEdgeColor(r,g,b,a);
}


void Square::setBorderWidth(int border_widht)
{
    border_width_ = border_widht;
}


void Square::setWorldPose(Pose p)
{
    t1_.setWorldPose(p);
    t2_.setWorldPose(p);
}


void Square::renderObject(Camera& camera, Renderer& renderer)
{
    t1_.renderObject(camera,renderer,border_width_,3);
    t2_.renderObject(camera,renderer,border_width_,2);
}
