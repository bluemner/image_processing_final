#ifndef _BETACORE_WATERSHED_HPP_
#define _BETACORE_WATERSHED_HPP_

#include <queue>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace betacore
{

template<typename T>
class Pixel3d{
    T x;
    T y;
    T z; //height
    std::vector<betacore::Pixel3d> neighbors;

};
class Watershed
{
    private:
        int threshold;
        static const int PIXEL_MIN = 000;
        static const int PIXEL_MAX = 256;
        std::vector<char> image;
    public:
        Watershed(const std::vector<char> &image){
             std::copy(image.begin(), image.end(),
                std::back_inserter(this->image));
        
        }

};

}
#endif