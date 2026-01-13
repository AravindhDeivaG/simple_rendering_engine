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


Pose interpolatePose(Pose p, Pose q, double t)
{
    Pose out;

    // Perform linear interpolation of position
    out.position = p.position*(1-t) + q.position*t;

    // Perform slerp for orientation
    double theta = std::acos(p.orientation.dot(q.orientation));
    out.orientation = (p.orientation.coeffs()*std::sin((1-t)*theta) + 
                       q.orientation.coeffs()*std::sin(t*theta)) / std::sin(theta);

    return out;
}


int numDigits(int num)
{
    int dig=0;
    while(num!=0)
    {
        num/=10;
        dig++;
    }
    return dig;
}


int main()
{
    Renderer renderer;
    Camera camera(300,300,320,240);

    RubiksCube cube;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Orientation o1, o2, o3;

    Pose p;
    p.position.setZero();
    
    p.orientation.setIdentity();
    p.position(1) = -4;
    o1.x() = 0; o1.y() = 0; o1.z() = std::sin(-20*M_PI/360); o1.w() = std::cos(-20*M_PI/360);
    p.orientation = o1;

    Pose q;
    q.position.setZero();
    q.position(1) = 4;
    o1.setIdentity();
    o2.setIdentity();
    o1.x() = 0; o1.y() = 0; o1.z() = std::sin(90*M_PI/360); o1.w() = std::cos(90*M_PI/360);
    o2.x() = 0; o2.y() = std::sin(-90*M_PI/360); o2.z() = 0; o2.w() = std::cos(-90*M_PI/360);
    o3.x() = 0; o3.y() = 0; o3.z() = std::sin(20*M_PI/360); o3.w() = std::cos(20*M_PI/360);
    q.orientation = o3*o2*o1;

    double T=5, t=0, frame_rate=60;
    int frame=1;
    std::string file_name;
    while(t<=T)
    {
        file_name = "C:/Users/nived/Desktop/cpp/dev/sdl_demo_renders/render1/image";
        for(int i=0;i<5-numDigits(frame);i++){file_name+="0";}
        file_name += (std::to_string(frame) + ".png");
        frame++;
        std::cout<<file_name<<"\n";
        cube.setWorldPose(interpolatePose(p,q,t/T));

        double theta=30, phi=0;
        camera.setCameraPose(cameraPose(8,theta*M_PI/180,phi*M_PI/180));
        cube.renderObject(camera,renderer);
        renderer.saveImage(file_name);
        renderer.show();
        renderer.clearWindow();
        t+=(1/frame_rate);
    } 
    std::cout<<"Saved all frames ...\n";
}