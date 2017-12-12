#ifndef _BETACORE_WATERSHED2_HPP_
#define _BETACORE_WATERSHED2_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <memory>
#include <map>
#include "./watershedpixel.hpp"

namespace betacore
{
class watershed2
{
	private:
		int threshold;
		static const int PIXEL_MIN =  000;
		static const int PIXEL_MAX =  256;
		int height;
		int width;
		int mask_size;
		int watershed_count;
		int current_state;
		std::vector<unsigned char> image;
		std::map<int,std::shared_ptr<watershed_pixel>> pixels;
		std::vector<std::vector<watershed_pixel>> sorted_pixels;
		std::queue<std::shared_ptr<watershed_pixel>> fifo_queue;
		watershed_pixel DUMMY;
		watershed_pixel *dummy;
		void setup(){
			sorted_pixels.resize(PIXEL_MAX);
			// for(size_t i=(size_t) PIXEL_MIN; i < (size_t) PIXEL_MAX; ++i ){
			// }
			#ifdef DEBUG
				std::cout<<"Adding Pixels"<< std::endl;
			#endif
			for(int i=0; i < width; i++){
				for(int j=0; j< height; j++){
					watershed_pixel *pixel = new watershed_pixel(i,j,image[j*width+i]);
					int id = (j*width+i);
					pixels.insert(std::pair<int,std::shared_ptr<watershed_pixel>>
										    (id,std::shared_ptr<watershed_pixel>(pixel)));
					sorted_pixels[pixel->height].push_back(*pixel);

				}
			}
			int mp = (int)  mask_size / 2;
			#ifdef DEBUG
				std::cout<<"Adding neighbors"<< std::endl;
			#endif
			

		}
		 void segment(){
			 int current_distance;
			 for(size_t intensity =0;  intensity < sorted_pixels.size(); intensity++){
				
				//Part 1: Get all pixels at the current intensity
				for(size_t i =0; i <  sorted_pixels[intensity].size(); ++i ){
					auto pixel =  &sorted_pixels[intensity][i];
					pixel->state  = watershed_pixel::STATE_VISITED;
					std::vector<std::shared_ptr<watershed_pixel>> neighbor_pixels = neighbors(*pixel);
					//initialize queue with n at intensity of current basins or watersheds
					for(auto neighbor : neighbor_pixels){
						if (neighbor->state > 0 || neighbor->state == watershed_pixel::STATE_WATERSHED)
						{
							pixel->distance = 1;
							fifo_queue.push(std::shared_ptr<watershed_pixel>(pixel));
							break;
						}
					}
				}

				current_distance = 1;		
				fifo_queue.push(std::shared_ptr<watershed_pixel>(dummy));
				// Part 2 extending the basins
				bool loop_true = true ;
				while(loop_true){
					auto p = fifo_queue.front();
					fifo_queue.pop();

					if(p->x == DUMMY.x && p->y == DUMMY.y){
						if(fifo_queue.empty()){
							loop_true =false;
							break;
						}
						else{
							
							fifo_queue.push(std::shared_ptr<watershed_pixel>(dummy));
							current_distance++;
							p = fifo_queue.front();
							fifo_queue.pop();
						}
					}

					std::vector<std::shared_ptr<watershed_pixel>> neighbor_pixels = neighbors(*p);
					for(size_t i =0; i <  neighbor_pixels.size(); ++i ){
						auto neighbor = neighbor_pixels[i];
						if (neighbor->distance <= current_distance && 
                           (neighbor->state > 0 || neighbor->state == watershed_pixel::STATE_WATERSHED))
                        {
                            if (neighbor->state > 0)
                            {
                                // the commented condition is also in the original algorithm 
                                // but it also gives incomplete borders
                                if (p->state == watershed_pixel::STATE_VISITED /*|| p.Label == WatershedCommon.WSHED*/) 
                                    p->state = neighbor->state;
                                else if (p->state != neighbor->state)
                                {
                                    p->state = watershed_pixel::STATE_WATERSHED;
                                    watershed_count++;
                                }
                            }
                            else if (p->state == watershed_pixel::STATE_VISITED)
                            {
                                p->state = watershed_pixel::STATE_WATERSHED;
                                watershed_count++;
                            }
                        }
                        // plateau 
                        else if (neighbor->state == watershed_pixel::STATE_VISITED && neighbor->distance == 0)
                        {
                            neighbor->distance = current_distance + 1;
							
                            fifo_queue.push(std::shared_ptr<watershed_pixel>(neighbor));
                        }
					}
				}
				
				// Part 3 find new minima at current intensity
				for(size_t i=0; i < sorted_pixels[intensity].size(); i++){
					auto pixel = &sorted_pixels[intensity][i];
					pixel->distance = 0;
					// if true then p is inside a new minimum 
                    if (pixel->state == watershed_pixel::STATE_VISITED)
                    {
                        // create new label
                        current_state++;
                        pixel->state = current_state;
						
                        fifo_queue.push(std::shared_ptr<watershed_pixel>(pixel));
                        while (!fifo_queue.empty())
                        {
                            auto q = fifo_queue.front();
							fifo_queue.pop();
                            // check neighbors of q
                            std::vector<std::shared_ptr<watershed_pixel>> neighbor_pixels = neighbors(*q);
                            for(size_t j =0; j <  neighbor_pixels.size(); ++j ){
								auto neighbor = neighbor_pixels[j];
                            
                                if (neighbor->state == watershed_pixel::STATE_VISITED)
                                {
                                    neighbor->state = current_state;
									
                                    fifo_queue.push(std::shared_ptr<watershed_pixel>(neighbor));
                                }
                            }
						
                        }
                    }
				}

			 }
		 }
		 

		 std::vector<std::shared_ptr<watershed_pixel>> neighbors(watershed_pixel &px){
			 
			 std::vector<std::shared_ptr<watershed_pixel>> temp;
			 
			 /*
			   |-1,-1|0,-1|1,-1|
			   |-1, 0| PX |1, 0|
			   |-1,+1|0,+1|1,+1|
			 */
			  // -1, -1                
			if ((px.x - 1) >= 0 && (px.y - 1) >= 0)
				temp.push_back(pixels[get_formatted_pair((px.x - 1), (px.y - 1))]);
			//  0, -1
			if ((px.y - 1) >= 0)
				temp.push_back(pixels[ get_formatted_pair(px.x,(px.y - 1))]);
			//  1, -1
			if ((px.x + 1) < this->width && (px.y - 1) >= 0)
				temp.push_back(pixels[get_formatted_pair((px.x + 1),(px.y - 1))]);
			// -1, 0
			if ((px.x - 1) >= 0)
				temp.push_back(pixels[get_formatted_pair((px.x - 1), px.y)]);
			//  1, 0
			if ((px.x + 1) < this->width)
				temp.push_back(pixels[get_formatted_pair((px.x + 1), px.y)]);
			// -1, 1
			if ((px.x - 1) >= 0 && (px.y + 1) < this->height)
				temp.push_back(pixels[get_formatted_pair((px.x - 1),(px.y + 1))]);
			//  0, 1
			if ((px.y + 1) < this->height)
				temp.push_back(pixels[get_formatted_pair(px.x,(px.y + 1))]);
			//  1, 1
			if ((px.x + 1) < this->width && (px.y + 1) < this->height)
				temp.push_back(pixels[get_formatted_pair((px.x + 1), (px.y + 1))]);
			return temp;
		 }
	public:
		watershed2(const std::vector<unsigned char> &image,
				  int height,
				  int width,
				  int mask_size){
			std::copy(image.begin(), image.end(),std::back_inserter(this->image));
			this->width = width;
			this->height = height;
			this->mask_size = mask_size;
			//this->result = &result;
			this->watershed_count = 0;
			this->current_state =0;
			dummy=new watershed_pixel();
			setup();
			segment();
		   
		}
		int get_formatted_pair(int x,int y){
			return y*width+x;
		}   
		std::string to_string(){
			std::ostringstream result;
	
			result << "width:"<<this->width <<" height:"<<this->height<< "\n";;
			// result << "min:" << this->pixels[0].to_string() << "\n";
			// result <<" max:"<< this->pixels[pixels.size()-1].to_string();
			return  result.str();
		}

		void get_filter(std::vector<unsigned char> &filter){
			if(filter.size() != (width * height)){
				filter.empty();
				filter.clear();
				filter.resize(width * height);
			}
			for (int y = 0; y < this->height; y++)
            {
				for (int x = 0; x < this->width; x++)
				{
					// if the pixel in our map is watershed pixel then draw it
					if (pixels[get_formatted_pair(x,y)]->state == watershed_pixel::STATE_WATERSHED)
						filter[y*width+x] = PIXEL_MAX;
					else 
						filter[y*width+x] = PIXEL_MIN;
				}
			}
		}
};
}
#endif