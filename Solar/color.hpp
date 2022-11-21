//
//  color.hpp
//  Solar
//
//  Created by Cyril Steger on 21.11.2022.
//

#ifndef color_hpp
#define color_hpp

#include <stdio.h>
class Color{
public:

    Color (float r=1.0f, float g=1.0f, float b=1.0f) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    float r, g, b;
};
#endif /* color_hpp */
