#ifndef _BETACORE_WATERSHED_PIXEL_HPP_
#define _BETACORE_WATERSHED_PIXEL_HPP_

#include <limits>

namespace betacore{

class watershed_pixel{
public:
    static const int STATE_WATERSHED= 0 ;
    static const int STATE_INI = -1;
    static const int STATE_VISITED =-2;
    static const int STATE_DUMMY = INT_MIN;
    int x;
    int y;
    int height;
    int state;
    int distance;

    watershed_pixel(){
        this->x = STATE_INI;
        this->y = STATE_INI;
        this->height = STATE_DUMMY;
        this->state = STATE_DUMMY;
        this->distance= STATE_DUMMY;
    }
    
    watershed_pixel(int x, int y,int height){
        this->x =x;
        this->y =y;
        this->height = height;
        this->state = STATE_INI;
        this->distance =0;
    }
    
    bool operator==(const watershed_pixel &other)const {
        return (this->x == other.x && this->y == other.y);
    }
    bool operator!=(const watershed_pixel &other)const {
        return (this->x != other.x || this->y != other.y);
    }
    bool operator< (const watershed_pixel &other) const {
        return this->height < other.height;
    }
   
    std::string to_string(){
        std::ostringstream result;
        result << "Pixel(";
        result << "" << this->x;
        result << "," << this->y;
        result << "," << this->height;
        result << ")";
        result << "\t State:"<<this->state;
        return result.str();
    }
};

}
#endif