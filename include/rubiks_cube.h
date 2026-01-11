#ifndef RUBIKS_CUBE_H_
#define RUBIKS_CUBE_H_

#include "square.h"
#include <memory>

// Color RED({200,40,40,255});
// Color BLUE({40,70,190,255});
// Color GREEN({50,170,80,255});
// Color YELLOW({220,200,60,255});
// Color ORANGE({220,120,40,255});
// Color WHITE({170,170,170,255});
// Color BLACK({30,30,30,255});

class RubiksCube
{
    public :
    RubiksCube();

    ~RubiksCube(){}

    // Function to set cube pose
    void setWorldPose(Pose);

    // Function to render cube
    void renderObject(Camera&, Renderer&);

    private :
    // Group all squares into one object
    std::vector<std::shared_ptr<Square>> objects_;
};

#endif