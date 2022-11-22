//
//  point.hpp
//  Solar
//
//  Created by Cyril Steger on 22.11.2022.
//

#ifndef point_hpp
#define point_hpp

#include <stdio.h>
class Point{
private:
    /* data */
public:
    Point (int x=0, int y=0) {
        this->x = x;
        this->y = y;
    }
    int x, y;
};
#endif /* point_hpp */
