#ifndef SQUARE_H_
#define SQUARE_H_

#include "triangle.h"

class Square
{
    public :
    /**
     * @brief Initialize square with centre and orientation
     * 
     * Input 
     *      pose
     *          Position : Centre of square
     *          Orientation : Orientation of square assuming originally
     *                        it is along xy plane with sides paralled to x and y axis
     *      Lenght : length of eachs side
     */     
    Square(Pose,double);

    ~Square();

    /**
     * @brief function to set fill color
     */
    void setFillColor(int, int, int, int);

    /**
     * @brief function to set edge color
     */
    void setEdgeColor(int, int, int, int);

    /**
     * @brief function to set edge color
     */
    void setBorderWidth(int);

    /**
     * @brief function to set world offset
     */
    void setWorldPose(Pose);

    /**
     * @brief Function to render traingle object in the camera
     */
    void renderObject(Camera&, Renderer&);

    private :
    // Two triangle objects used to construct square
    Triangle t1_, t2_;

    // Pose wrt origin
    Pose pose_;
    
    // Edge and fill color
    Color edge_color_, fill_color_;

    // border width
    int border_width_;

};

#endif