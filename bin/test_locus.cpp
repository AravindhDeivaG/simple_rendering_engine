#include "locus.h"
#include <chrono>
#include <thread>
#include <iostream>


int main()
{
    Renderer renderer;
    Camera camera(300,300,320,240);

    Locus locus;
    Color color = {30,30,30,255};

    Vector center = {0,0,0}, y_unit, z_unit;
    y_unit.setUnit(1);
    z_unit.setUnit(2);
    double r=2;
    for(int theta=0;theta<=180;theta+=20)
    {
        locus.addPoint(r*(y_unit*std::cos(theta*M_PI/180) + z_unit*std::sin(theta*M_PI/180)));
    }
    

    Pose p;
    p.position.setZero();
    p.position(0) = 5;
    p.orientation.x() = -0.5;
    p.orientation.y() = -0.5;
    p.orientation.z() = 0.5;
    p.orientation.w() = 0.5;
    camera.setCameraPose(p);

    locus.setColor(color);
    locus.renderObject(camera,renderer,5);
    renderer.show();
    int x;
    std::cin>>x;
}