#include "renderer.h"

Renderer::Renderer(int size_x, int size_y) : 
    depth_buffer_(size_x, size_y)
{
    // Connect to display driver
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    window_ = SDL_CreateWindow("Render window", size_x,size_y,0);

    // Create renderer
    renderer_ = SDL_CreateRenderer(window_,NULL);

    // Set depth buffer to window size and intiailize value to inf
    depth_buffer_.setConstant(100000);

    clearWindow();
}


Renderer::~Renderer()
{
     // Destroy screen
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}


void Renderer::renderTriangle(Eigen::Matrix<double,2,-1>& pixels, 
                            Color fill_color,
                            Color edge_color,
                            int border_width,
                            int ignore_edge    
                        )
{
    // Get bounding box
    Pixel p_min, p_max;
    p_min = pixels.rowwise().minCoeff();
    p_max = pixels.rowwise().maxCoeff();
    // std::cout<<"Min max values : "<<p_min.transpose()<<" | "<<p_max.transpose()<<"\n\n";

    // Loop through all pixels in bounding box
    SDL_SetRenderDrawColor(renderer_,255,0,0,255);
    for(int x=p_min(0);x<=p_max(0);x++)
    {
        for(int y=p_min(1);y<=p_max(1);y++)
        {
            Pixel p = {x,y};
            auto res = isInTriangle_(p,pixels,border_width,ignore_edge);
            if(res==1)
            {
                SDL_SetRenderDrawColor(renderer_,
                                fill_color.r,
                                fill_color.g,
                                fill_color.b,
                                fill_color.a);
                SDL_RenderPoint(renderer_, x, y);
            }
            else if(res==2)
            {
               SDL_SetRenderDrawColor(renderer_,
                                edge_color.r,
                                edge_color.g,
                                edge_color.b,
                                edge_color.a);
                SDL_RenderPoint(renderer_, x, y);
            }
        }
    }
}


void Renderer::clearWindow()
{
    // Clear the window
    SDL_SetRenderDrawColor(renderer_,255,255,255,255);
    SDL_RenderClear(renderer_);
}


int Renderer::isInTriangle_(Pixel p, Eigen::Matrix<double,2,-1>& pixels, 
                            int border_width, 
                            int ignore_edge)
{
    Pixel a,b,c;
    a = pixels.block(0,0,2,1);
    b = pixels.block(0,1,2,1);
    c = pixels.block(0,2,2,1);

    // Calculate edge for each pixel centre
    p(0)+=0.5;
    p(1)+=0.5;

    double e_ab_p, e_bc_p, e_ca_p, e_ab_c;

    // Edge function of C wrt Ab (To calculate signed area)
    e_ab_c = edgeFunction_(c,a,b);

    e_ab_p = edgeFunction_(p,a,b)/e_ab_c;
    e_bc_p = edgeFunction_(p,b,c)/e_ab_c;
    e_ca_p = edgeFunction_(p,c,a)/e_ab_c;

    // Lies outside the triangle if all the values are not positive 
    if(e_ab_p <0 || e_bc_p<0 || e_ca_p<0)
    {
        return 0;
    }

    // If perpendicualr distance < 0.5 on the edge
    if( ( perpendicularDistance_(p,a,b)<=(border_width-0.5) && ignore_edge!=1 ) || 
        ( perpendicularDistance_(p,b,c)<=(border_width-0.5) && ignore_edge!=2 ) ||
        ( perpendicularDistance_(p,c,a)<=(border_width-0.5) && ignore_edge!=3 )
    )
    {
        return 2;
    }

    // All the above conditions fail point lies inside triangle
    return 1;
}


double Renderer::edgeFunction_(Pixel& p, Pixel& a, Pixel& b)
{
    return (p-a).cross((p-b));
}


double Renderer::perpendicularDistance_(Pixel& p,Pixel& a,Pixel& b)
{
    return std::abs( edgeFunction_(p,a,b)/(2*(a-b).norm()) );
}

