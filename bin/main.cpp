#include <Eigen/Core>
#include <Eigen/Dense>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>

struct Position
{
    double x,y,z;
};

struct Orientation
{
    double x,y,z,w;
};

struct Pose
{
    Position position;
    Orientation orientation;
};



Eigen::Matrix4d pose2Transform(Pose p)
{
    Eigen::Matrix4d res;
    res.setZero();

    double x,y,z,w;
    x = p.orientation.x;
    y = p.orientation.y;
    z = p.orientation.z;
    w = p.orientation.w;

    res(0,3) = p.position.x;
    res(1,3) = p.position.y;
    res(2,3) = p.position.z;
    res(3,3) = 1;

    res(0,0) = 1-2*(pow(y,2) + pow(z,2));
    res(0,1) = 2*(x*y - z*w);
    res(0,2) = 2*(x*z + y*w);

    res(1,0) = 2*(x*y + z*w);
    res(1,1) = 1-2*(pow(x,2) + pow(z,2));
    res(1,2) = 2*(y*z - x*w);

    res(2,0) = 2*(x*z - y*w);
    res(2,1) = 2*(y*z + x*w);
    res(2,2) = 1-2*(pow(x,2) + pow(y,2));

    return res;
}



std::vector<Eigen::Vector3d> getCubePoints(Eigen::Vector3d centre, double len)
{
    std::vector<Eigen::Vector3d> res;
    Eigen::Vector3d p;

    for(int i=0;i<8;i++)
    {
        p.setConstant(len/2);
        if(i%2 != 0)
        {
            p(2)*=-1;
        }
        if( (int (i/2))%2 != 0 )
        {
            p(1)*=-1;
        }
        if( (int (i/4))%2 != 0 )
        {
            p(0)*=-1;
        }
        p = p+centre;
        res.emplace_back(p);
    }
    return res;
}


std::vector<Eigen::Vector2d> transformPoints(
    const std::vector<Eigen::Vector3d>& points,
    Eigen::Matrix4d T, Eigen::Matrix3d C)
{
    std::vector<Eigen::Vector2d> pixel_points;
    for(auto const p: points)
    {
        Eigen::Vector4d p_;
        p_(0) = p(0);
        p_(1) = p(1);
        p_(2) = p(2);
        p_(3) = 1;

        Eigen::Vector3d vec;
        p_ = T*p_;
        vec(0) = p_(0)/p_(2);
        vec(1) = p_(1)/p_(2);
        vec(2) = 1;

        Eigen::Vector2d pix;
        vec = C*vec;
        pix(0) = vec(0);
        pix(1) = vec(1);
        pixel_points.emplace_back(pix);
    }
    return pixel_points;
}


void renderCube(SDL_Renderer* renderer, const std::vector<Eigen::Vector2d>& pixel_points)
{
    // render the pixel points
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    SDL_RenderLine(renderer,pixel_points[0](0),pixel_points[0](1),pixel_points[1](0),pixel_points[1](1));
    SDL_RenderLine(renderer,pixel_points[1](0),pixel_points[1](1),pixel_points[3](0),pixel_points[3](1));
    SDL_RenderLine(renderer,pixel_points[3](0),pixel_points[3](1),pixel_points[2](0),pixel_points[2](1));
    SDL_RenderLine(renderer,pixel_points[2](0),pixel_points[2](1),pixel_points[0](0),pixel_points[0](1));
    SDL_RenderLine(renderer,pixel_points[4](0),pixel_points[4](1),pixel_points[5](0),pixel_points[5](1));
    SDL_RenderLine(renderer,pixel_points[5](0),pixel_points[5](1),pixel_points[7](0),pixel_points[7](1));
    SDL_RenderLine(renderer,pixel_points[7](0),pixel_points[7](1),pixel_points[6](0),pixel_points[6](1));
    SDL_RenderLine(renderer,pixel_points[6](0),pixel_points[6](1),pixel_points[4](0),pixel_points[4](1));
    SDL_RenderLine(renderer,pixel_points[3](0),pixel_points[3](1),pixel_points[7](0),pixel_points[7](1));
    SDL_RenderLine(renderer,pixel_points[5](0),pixel_points[5](1),pixel_points[1](0),pixel_points[1](1));
    SDL_RenderLine(renderer,pixel_points[4](0),pixel_points[4](1),pixel_points[0](0),pixel_points[0](1));
    SDL_RenderLine(renderer,pixel_points[6](0),pixel_points[6](1),pixel_points[2](0),pixel_points[2](1));

    SDL_RenderPresent(renderer);
}


Orientation calcSlerp(const Orientation& p1,const Orientation& p2, double t)
{
    double theta;
    
    theta = acos(p1.x*p2.x+
                 p1.y*p2.y+
                 p1.z*p2.z+
                 p1.w*p2.w);
    Orientation p;
    p.x = (p1.x*sin((1-t)*theta) + p2.x*sin(t*theta))/sin(theta);
    p.y = (p1.y*sin((1-t)*theta) + p2.y*sin(t*theta))/sin(theta);
    p.z = (p1.z*sin((1-t)*theta) + p2.z*sin(t*theta))/sin(theta);
    p.w = (p1.w*sin((1-t)*theta) + p2.w*sin(t*theta))/sin(theta);

    
    // Orientation p;
    // p.x = p1.x*cos(t) + p2.x*sin(t);
    // p.y = p1.y*cos(t) + p2.y*sin(t);
    // p.z = p1.z*cos(t) + p2.z*sin(t);
    // p.w = p1.w*cos(t) + p2.w*sin(t);

    return p;
}


int main()
{
    // Connect to display driver
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Window name", 640,480,0);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL);

    Eigen::Matrix<double,4,4> T; // Rigid body transformation matrix
    Eigen::Matrix<double,3,3> C; // Camera matrix
    std::vector<Eigen::Vector3d> points;
    std::vector<Eigen::Vector2d> pixel_points;

    T.setZero();
    C.setZero();

    points = getCubePoints(Eigen::Vector3d({0,0,0.5}),1);

    T(0,0) = 0;
    T(1,0) = 1;
    T(2,0) = 0;
    T(0,1) = 0;
    T(1,1) = 0;
    T(2,1) = -1;
    T(0,2) = -1;
    T(1,2) = 0;
    T(2,2) = 0;
    T(0,3) = 2;
    T(1,3) = 0;
    T(2,3) = 0.2;
    T(3,3) = 1;

    Pose p;
    p.position.x = 0;
    p.position.y = 0;
    p.position.z = 0;
    p.orientation.x = 0;
    p.orientation.y = 0;
    p.orientation.z = 0;
    p.orientation.w = 1;

    Pose q;
    q.position = p.position;
    q.orientation.x = 0;
    q.orientation.y = 0;
    q.orientation.z = 1;
    q.orientation.w = 0;

    Orientation o1, o2;
    o1 = p.orientation;
    o2 = q.orientation;

    C(0,0) = 300;
    C(1,0) = 0;
    C(2,0) = 0;
    C(0,1) = 0;
    C(1,1) = 300;
    C(2,1) = 0;
    C(0,2) = 320;
    C(1,2) = 240;
    C(2,2) = 1;

    //Keep the screen alive for some time
    bool running = true;
    double t=0;
    while (running) 
    {
        auto start = std::chrono::high_resolution_clock::now();
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Transform points
        q.orientation = calcSlerp(o1,o2,t);
        auto pixel_points = transformPoints(points,T.inverse()*pose2Transform(q),C);
        renderCube(renderer,pixel_points);

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        // std::cout<<"Time elapsed : "<<elapsed_time.count()<<"\n";
        SDL_Delay(25-elapsed_time.count());  // small sleep to avoid 100% CPU
        t+=0.025*0.1;
        // T(1,3) = T(1,3) + 0.025*0.1;
    }

    // Destroy screen
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}