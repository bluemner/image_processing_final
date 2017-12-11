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
        static const int STATE_WATERSHED= 0 ;
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
    ~Pixel3d(){

    }
    void set_state(const int state){
        this->state = state;
    }
    int get_state(){
        int temp = this->state;
        return temp;
    }
    void add_neighbor(Pixel3d &n){
        neighbors.push_back(n);
    }
    bool operator< (const Pixel3d &other) const {
        return this->z < other.z;
    }
    bool neighbors_sate_is_watershed(){
        for(auto n : this->neighbors){
            if(n.get_state() == Pixel3d::STATE_WATERSHED){
                return true;
            }
        }
        return false;
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
        static const int PIXEL_MIN =  000;
        static const int PIXEL_MAX =  256;
        int height;
        int width;
        int mask_size;
        std::vector<unsigned char> image;
        std::vector<Pixel3d> pixels;
        std::queue<Pixel3d> fifo_queue;
        std::vector<unsigned char> *result;
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
            size_t a_start =0;
            size_t b_start =0;
            int current_state =0;
            #ifdef DEBUG
                std::cout<<"Let the flood begin!"<< std::endl;
            #endif
            for (int intensity =PIXEL_MIN; intensity<PIXEL_MAX; intensity++){
                //block 1
                for( size_t a =a_start; a <pixels.size(); a++ ){
                    Pixel3d *pixel = &pixels[a];
                    if( (int) pixel->z != intensity ){
                        //Next level
                        a_start = a;
                        break;
                    }

                    pixel->set_state( Pixel3d::STATE_VISITED);

                    auto neighbors = pixel->neighbors;
                    for(auto n : neighbors){
                        if (n.get_state() >= 0){
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
                     #ifdef DEBUG
                      //  std::cout<<"Working on:"<<pop.to_string() << std::endl;
                     #endif
                    if(pop.get_state() == Pixel3d::STATE_DUMMY){

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
                        std::cout<<"Working on neighbor:"<<n.to_string() << std::endl;
                        if(n.distance <=current_distance && n.get_state() >=0){
                            // in watershed  or existing valley 
                            if(n.get_state()>0){
                                if(pop.get_state()  == Pixel3d::STATE_VISITED){
                                    pop.set_state( n.get_state());
                                }else if (n.get_state() != pop.get_state()){
                                    pop.set_state(Pixel3d::STATE_WATERSHED);
                                }
                            }else if(pop.get_state() == Pixel3d::STATE_VISITED) {
                                 pop.set_state(Pixel3d::STATE_WATERSHED);
                            }
                        }else if(n.get_state()  == Pixel3d::STATE_VISITED && n.distance ==0){
                            n.distance += current_distance +1;
                            fifo_queue.push(n);
                        }
                    }//end-for
                }//end-while
                
                // find new minima 
                for(size_t b = b_start; b<pixels.size(); ++b){
                    auto pixel = &pixels[b];
                    if((int) pixel->z != intensity ){
                        b_start = b;
                        break;
                    }

                    pixel->distance =0;

                    if( pixel->get_state() == Pixel3d::STATE_VISITED){
                        ++current_state;
                        pixel->set_state(current_state);
                        fifo_queue.push(*pixel);

                        while(!fifo_queue.empty()){
                            auto pop = fifo_queue.front();
                            fifo_queue.pop();
                            for(auto n:pop.neighbors){
                                if(n.get_state()  == Pixel3d::STATE_VISITED){
                                    n.get_state()  == current_state;
                                    fifo_queue.push(n);
                                }
                            }
                        }
                    }
                }
            }
        }
        void transfer(){
            #ifdef DEBUG
                std::cout<<"Begin Transfer"<< std::endl;
            #endif
            if(result->size() <= (size_t) (height* width))
                result->resize(height* width);
            
            for(auto p : pixels)                
                if(p.state == Pixel3d::STATE_WATERSHED && !p.neighbors_sate_is_watershed() )
                {   
                   result->at(p.y*width+p.x) = (unsigned char)256;
                }else{
                    result->at(p.y*width+p.x) =(unsigned char) 0;
                }
        }           
    public:
        Watershed(const std::vector<unsigned char> &image,
                  int height,
                  int width,
                  int mask_size,
                  std::vector<unsigned char> &result){
            std::copy(image.begin(), image.end(),
                std::back_inserter(this->image));
            this->width = width;
            this->height = height;
            this->mask_size = mask_size;
            this->result = &result;
            setup();
            flood();
            transfer();
        }
       
        std::string to_string(){
            std::ostringstream result;
    
            result << "width:"<<this->width <<" height:"<<this->height<< "\n";;
            result << "min:" << this->pixels[0].to_string() << "\n";
            result <<" max:"<< this->pixels[pixels.size()-1].to_string();
            return  result.str();
        }

        
};

}
#endif