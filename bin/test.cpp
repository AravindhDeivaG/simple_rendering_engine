#include "square.h"
#include "rubiks_cube.h"
#include <chrono>
#include <thread>
#include <iostream>


Pose cameraPose(double r, double theta, double phi)
{
    Pose p;
    p.position(0) = r*std::cos(theta)*std::cos(phi);
    p.position(1) = r*std::cos(theta)*std::sin(phi);
    p.position(2) = r*std::sin(theta);

    Eigen::Matrix3d R;
    Vector x,y,z;
    Vector z_g = {0,0,1};

    z = -1*p.position;
    x = z.cross(z_g);
    y = z.cross(x);

    z = z/z.norm();
    y = y/y.norm();
    x = x/x.norm();

    R.block(0,0,3,1) = x;
    R.block(0,1,3,1) = y;
    R.block(0,2,3,1) = z;

    Eigen::Quaterniond q(R);

    p.orientation = q;
    return p;
}


int main()
{
    Renderer renderer;
    Camera camera(300,300,320,240);

    RubiksCube cube;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Pose p;
    p.position.setZero();
    p.orientation.x() = 0;
    p.orientation.y() = 0;
    p.orientation.z() = std::sin(0*M_PI/360);
    p.orientation.w() = std::cos(0*M_PI/360);
    cube.setWorldPose(p);

    SDL_Event e;
    double theta=30, phi=45;
    camera.setCameraPose(cameraPose(6,theta*M_PI/180,phi*M_PI/180));
    cube.renderObject(camera,renderer);
    renderer.show();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    bool exit=false;
    while (!exit) {
        SDL_PollEvent(&e);
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if(e.key.scancode == SDL_SCANCODE_UP)
            {
                theta = std::min((theta+5),60.0);
            }
            else if(e.key.scancode == SDL_SCANCODE_DOWN)
            {
                theta = std::max((theta-5),-60.0);
            }
            else if(e.key.scancode == SDL_SCANCODE_LEFT)
            {
                phi = (((phi-5)/360) - std::floor((phi-5)/360))*360;
            }
            else if(e.key.scancode == SDL_SCANCODE_RIGHT)
            {
                phi = ( ((phi+5)/360) - std::floor((phi+5)/360) )*360;
            }
            else if(e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                exit = true;
            }

            camera.setCameraPose(cameraPose(6,theta*M_PI/180,phi*M_PI/180));
            renderer.clearWindow();
            cube.renderObject(camera,renderer);
            renderer.show();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

}






// Renderer renderer;
//     Camera camera(300,300,320,240);
//     Pose p;
//     p.position(0) = 0.5;
//     p.position(1) = 0.5;
//     p.position(2) = 0;
//     p.orientation.x() = 0;
//     p.orientation.y() = std::sin(45*M_PI/180);
//     p.orientation.z() = 0;
//     p.orientation.w() = std::cos(45*M_PI/180);
//     Square square(p,1);

//     Pose q;
//     q.position(0) = 4;
//     q.position(1) = 0;
//     q.position(2) = 0;
//     q.orientation.x() = -0.5;
//     q.orientation.y() = -0.5;
//     q.orientation.z() = 0.5;
//     q.orientation.w() = 0.5;
//     camera.setCameraPose(q);

//     square.setFillColor(0,255,255,255);
//     square.setEdgeColor(0,0,0,255);
//     square.setBorderWidth(3);
//     square.renderObject(camera,renderer);

//     renderer.show();
//     int x;
//     std::cin >> x;