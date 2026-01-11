#include "rubiks_cube.h"

RubiksCube::RubiksCube()
{
    Pose p;

    // Init Red face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(0) = 1.5;
    p.orientation.x() = 0;
    p.orientation.y() = std::sin(45*M_PI/180);
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(45*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(1) = i;
            p.position(2) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(200,40,40,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }
    

    // Init green face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(1) = 1.5;
    p.orientation.x() = std::sin(-45*M_PI/180);
    p.orientation.y() = 0;
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(-45*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(0) = i;
            p.position(2) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(50,170,80,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }
    
    // Init orange face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(0) = -1.5;
    p.orientation.x() = 0;
    p.orientation.y() = std::sin(-45*M_PI/180);
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(-45*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(1) = i;
            p.position(2) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(220,120,40,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }


    // Init blue face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(1) = -1.5;
    p.orientation.x() = std::sin(45*M_PI/180);
    p.orientation.y() = 0;
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(45*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(0) = i;
            p.position(2) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(40,70,190,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }

    // Init yellow face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(2) = 1.5;
    p.orientation.x() = 0;
    p.orientation.y() = std::sin(0*M_PI/180);
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(0*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(0) = i;
            p.position(1) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(220,200,60,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }


    // Init white face
    p.position.setZero();
    p.orientation.setIdentity();

    p.position(2) = -1.5;
    p.orientation.x() = 0;
    p.orientation.y() = std::sin(90*M_PI/180);
    p.orientation.z() = 0;
    p.orientation.w() = std::cos(90*M_PI/180);
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            p.position(0) = i;
            p.position(1) = j;
            auto obj = std::make_shared<Square>(p,1);
            obj->setFillColor(170,170,170,255);
            obj->setEdgeColor(30,30,30,255);
            objects_.emplace_back(obj);
        }
    }
}



void RubiksCube::setWorldPose(Pose p)
{
    for(auto obj : objects_)
    {
        obj->setWorldPose(p);
    }
}


void RubiksCube::renderObject(Camera& camera, Renderer& renderer)
{
    for(auto obj : objects_)
    {
        obj->renderObject(camera,renderer);
    }
}