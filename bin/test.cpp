#include "square.h"
#include "rubiks_cube.h"
#include "locus.h"
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


Eigen::Vector3d quaternion2RPY(Eigen::Quaterniond q)
{
    Eigen::Vector3d v1,v2;
    v1 = q.toRotationMatrix().eulerAngles(2,1,0);
    v2(0) = v1(2);
    v2(1) = v1(1);
    v2(2) = v1(0);
    return v2;
}

Eigen::Quaterniond rpy2Quaternion(Eigen::Vector3d rpy)
{

    Eigen::AngleAxisd rollAngle(rpy(0),   Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd pitchAngle(rpy(1), Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd yawAngle(rpy(2),     Eigen::Vector3d::UnitZ());

    Eigen::Quaterniond q =yawAngle * pitchAngle * rollAngle;
    return q;
}


Pose interpolatePoseSmooth(Pose p, Pose q, double t)
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

Pose interpolatePoseRough(Pose p, Pose q, double t)
{
    Pose out;

    // Perform linear interpolation of position
    out.position = p.position*(1-t) + q.position*t;

    // Perform lerp in rpy
    Eigen::Vector3d p_rpy, q_rpy, pq_rpy;
    p_rpy = quaternion2RPY(p.orientation);
    q_rpy = quaternion2RPY(q.orientation);

    pq_rpy = p_rpy*(1-t) + q_rpy*t;
    out.orientation = rpy2Quaternion(pq_rpy);
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
    Renderer renderer(400,400);
    Camera camera(300,300,200,200);

    RubiksCube cube;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Orientation o1, o2, o3;

    Pose p;
    p.position.setZero();
    
    p.orientation.setIdentity();
    p.position(1) = 0;
    o1.x() = 0; o1.y() = 0; o1.z() = std::sin(-45*M_PI/360); o1.w() = std::cos(-45*M_PI/360);
    p.orientation = o1;

    Pose q;
    q.position.setZero();
    q.position(1) = 0;
    o1.setIdentity();
    o2.setIdentity();
    o1.x() = 0; o1.y() = 0; o1.z() = std::sin(0*M_PI/360); o1.w() = std::cos(0*M_PI/360);
    o2.x() = 0; o2.y() = std::sin(180*M_PI/360); o2.z() = 0; o2.w() = std::cos(180*M_PI/360);
    o3.x() = 0; o3.y() = 0; o3.z() = std::sin(-135*M_PI/360); o3.w() = std::cos(-135*M_PI/360);
    q.orientation = (o3*o2*o1).coeffs()*-1;

    std::cout<<"Original orientation : "<<q.orientation<<"\n";
    std::cout<<"Original orientation : "<<rpy2Quaternion(quaternion2RPY(q.orientation))<<"\n";

    double T=5, delay_start=0.5, delay_end = 2, frame_rate=60;
    double t=0;
    int frame=1;
    std::string file_name;
    Pose cube_pose;

    // Tracked point on the cube
    Vector point = {1.5,-1.5,1.5};

    // Locus object to track
    Locus locus;
    locus.setColor(Color({30,30,30,255}));

    while(t<=T+delay_start+delay_end)
    {
        if(t<=delay_start)
        {
            cube_pose = interpolatePoseRough(p,q,0);
            // cube_pose = interpolatePoseSmooth(p,q,0);
        }
        else if(t>=delay_start && t<=delay_start+T)
        {
            cube_pose = interpolatePoseRough(p,q,(t-delay_start)/T);
            // cube_pose = interpolatePoseSmooth(p,q,(t-delay_start)/T);
            locus.addPoint(
                cube_pose.orientation.toRotationMatrix()*point + cube_pose.position
            );
        }
        else
        {
            cube_pose = interpolatePoseRough(p,q,1);
            // cube_pose = interpolatePoseSmooth(p,q,1);

        }
        cube.setWorldPose(cube_pose);
        file_name = "C:/Users/nived/Desktop/cpp/dev/sdl_demo_renders/render3/image";
        for(int i=0;i<4-numDigits(frame);i++){file_name+="0";}
        file_name += (std::to_string(frame) + ".png");
        frame++;
        std::cout<<file_name<<"\n";

        double theta=30, phi=0;
        camera.setCameraPose(cameraPose(6,theta*M_PI/180,phi*M_PI/180));
        cube.renderObject(camera,renderer);
        if(t>=delay_start)
        {
            locus.renderObject(camera,renderer,5);
        }
        renderer.saveImage(file_name);
        renderer.show();
        renderer.clearWindow();
        t+=(1/frame_rate);
    } 
    std::cout<<"Saved all frames ...\n";
}