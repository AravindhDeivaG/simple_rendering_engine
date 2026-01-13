#ifndef RENDERER_H_
#define RENDERER_H_

#include "typedefs.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <string>


struct Color
{
    int r,g,b,a;
};

class Renderer
{
    public :
    Renderer(int size_x=640, int size_y=480);

    ~Renderer();

    /**
     * @brief Function to render a triangle
     * edge : param to ignoer a particular edge for shading edges while rendering
     *      0 : No edges are ignored
     *      1 : Edge AB is ignored
     *      2 : Edge BC is ignored
     *      3 : Edge CA is ignored
     */
    void renderTriangle(Eigen::Matrix<double,2,-1>& points, Color, Color, int border_widht=1, int ignore_edge = 0);

    /**
     * Function to show the rendered image
     */
    void show()
    {
        SDL_RenderPresent(renderer_);
    }

    /**
     * @brief Function to save the current rendered image
     */
    void saveImage(std::string);

    /**
     * @brief function to clear the window
     */
    void clearWindow();

    private :

    /**
     * @brief window size
     */
    int size_x_, size_y_;

    /**
     * @brief SDL window object
     */
    SDL_Window* window_;

    /**
     * @brief SDL renderer object
     */
    SDL_Renderer* renderer_;

    /**
     * @brief depth buffer for shading objects on top
     */
    Eigen::Matrix<double,-1,-1> depth_buffer_;


    /**
     * @brief Function to calculate if a point is within a triangle
     * f E_AB(P) >= 0 && E_BC(P) >=0 && E_CA(P) >=0 
     *      point lies inside the triangle
     * 
     * @return
     * 0 : If outside triangle
     * 1 : If inside triangle
     * 2 : On triangle edge
     */
    int isInTriangle_(Pixel, Eigen::Matrix<double,2,-1>&, int border_width=1, int ignore_edge=0);

    /**
     * @brief Edge function of a triangle
     * E_AB(P) = ((P-A) cross (P-B))/((C-A) cross (C-B))
     * 
     */
    double edgeFunction_(Pixel& p, Pixel& a, Pixel& b);

    /**
     * @brief Calculates perpendicular distance from a line segment
     */
    double perpendicularDistance_(Pixel&,Pixel&,Pixel&);
};

#endif