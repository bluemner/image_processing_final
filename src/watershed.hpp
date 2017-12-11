#ifndef _BETACORE_WATERSHED_HPP_
#define _BETACORE_WATERSHED_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace betacore
{


class Pixel3d{
    public:
        static const int STATE_INI = -1;
        static const int STATE_DAM= 0 ;
        static const int STATE_VISITED =-2;
        static const int STATE_DUMMY =-3;
        int x;
        int y;
        unsigned char z; //height
        int state;
        int distance;
        std::vector<betacore::Pixel3d> neighbors;
public:
    Pixel3d(int x, int y, unsigned intensity ){
        this->state = STATE_INI;
        this->x = x;
        this->y = y;
        this->z = intensity;
    }
    Pixel3d(){
        this->state = STATE_DUMMY;
    }
    void add_neighbor(Pixel3d &n){
        neighbors.push_back(n);
    }
    bool operator< (const Pixel3d &other) const {
        return this->z < other.z;
    }
    std::string to_string(){
        std::ostringstream result;
        result << "Pixel(";
        result << "" << this->x;
        result << "," << this->y;
        result << "," << (int) this->z;
        result << ")";
        result << "\t State:"<<this->state;
        return result.str();
    }
        
};
class Watershed
{
    private:
        int threshold;
        static const int PIXEL_MIN = 000;
        static const int PIXEL_MAX = 256;
        int height;
        int width;
        int mask_size;
        std::vector<unsigned char> image;
        std::vector<Pixel3d> pixels;
        std::queue<Pixel3d> fifo_queue;
        void setup(){
             #ifdef DEBUG
            std::cout<<"Adding Pixels"<< std::endl;
            #endif
            for(int i=0; i < width; i++){
                for(int j=0; j< height; j++){
                    Pixel3d temp(i,j,image[j*width+i]);
                    pixels.push_back(temp);

                }
            }
            int mp;
            #ifdef DEBUG
            std::cout<<"Adding neighbors"<< std::endl;
            #endif
            for(int i=0; i < width; i++){
                for(int j=0; j< height; j++){
                    auto temp = pixels.at(j*width+i);
                    mp = (int)  mask_size / 2;
                    for(int p=i-mp; p < i+mp;++p){
                        for(int v=j-mp;v < j+mp; ++v){
                            if(p > 0 && p < this->width &&
                               v > 0 && v < this->height){
                               temp.add_neighbor(pixels[v*this->width+p]);
                            }
                        }
                    }
                }
            }
            #ifdef DEBUG
            std::cout<<"Sorting"<< std::endl;
            #endif
            std::sort(pixels.begin(), pixels.end());
        }
        void flood(){
            int a_start =0;
            for (int intensity =PIXEL_MIN; intensity<PIXEL_MAX; intensity++){
               
                for( int a =a_start; a <pixels.size(); a++ ){
                    auto pixel = pixels[a];
                    if( pixel.z != intensity ){
                        //Next level
                        a_start = a;
                        break;
                    }

                    pixel.state = Pixel3d::STATE_VISITED;

                    auto neighbors = pixel.neighbors;
                    for(auto n : neighbors){
                        if (n.state >= 0){
                            n.distance = 1;
                            fifo_queue.push(n);
                            break;
                        }
                    }                    
                }

                int current_distance = 1;
                
                this->fifo_queue.push(Pixel3d());

                while(true){
                    Pixel3d pop = fifo_queue.front();
                    fifo_queue.pop();
                    if(pop.state == Pixel3d::STATE_DUMMY){
                        if(fifo_queue.empty()){
                            break;
                        }else{
                            this->fifo_queue.push(Pixel3d());
                            ++current_distance;
                            pop = fifo_queue.front();
                            fifo_queue.pop();
                        }
                    }

                    auto neighbors = pop.neighbors;
                    for (auto n : neighbors){
                        
                    }

                }


            }
        }
    public:
        Watershed(const std::vector<unsigned char> &image,int height, int width,int mask_size){
            std::copy(image.begin(), image.end(),
                std::back_inserter(this->image));
            this->width = width;
            this->height = height;
            this->mask_size = mask_size;
            setup();
            flood();
        }
       
        std::string to_string(){
            std::ostringstream result;
        
            // for(auto p : this->pixels){
            //     result << p.to_string() << "\n";
            // }

            result << "width:"<<this->width <<" height:"<<this->height<< "\n";;
            result << "min:" << this->pixels[0].to_string() << "\n";
            result <<" max:"<< this->pixels[pixels.size()-1].to_string();
            return  result.str();
        }
};

}
#endif