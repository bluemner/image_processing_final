#ifndef _BETACORE_SOBEL_H_
#define _BETACORE_SOBEL_H_

#include <limits>
#include <cmath>
#include <vector>

namespace betacore{
	template <typename T>
	class sobel{
	public:
		sobel(){}
		
		void apply(std::vector<T> &image,
							int height,
							int width,
							std::vector<T> &result )
		{

			const float kernel_x[3][3] = {
				{-1, 0, 1},
				{-2, 0, 2},
				{-1, 0, 1}
			};
			const float kernel_y[3][3] = {
				{-1, -2, -1},
				{ 0,  0,  0},
				{ 1,  2,  1}
			};
			T magnitude = (T) 0.0;
			int x_weight=0;
			int y_weight=0;
			std::vector<int> temp_image;
			temp_image.resize(height * width);
			int size =0,
				norm_x,
				norm_y;
		
			for(size_t x =0; x< width; ++x)
			{
				for(size_t y =0; y< height; ++y)
				{
					x_weight=0;
					y_weight=0;
					for(size_t a=0; a < 3;a++)
					{
						for(size_t b=0; b<3; b++)
						{
							int xn = x + b - 1;
        					int yn = y + a - 1;
							if(xn<0 || width<= xn){
								xn=x;
							}
							if(yn<0 || width<= yn){
								yn=y;
							}
							x_weight+=image[yn * width + xn] * kernel_x[a][b];
							
							y_weight+=image[yn * width + xn]  * kernel_y[a][b];
							size += std::abs(kernel_y[a][b]);
						}
					}
				

					norm_x =x_weight/=size;
					norm_y =y_weight/=size;
					temp_image[y* width + x] = (T) std::abs(norm_x) + std::abs(norm_y);
					size=0;
					
				}
			}
			center_image_frequency(temp_image, width,height);
			for(size_t z = 0; z< temp_image.size(); ++z){
				result[z] = (T) temp_image[z];
			}
		}

		void center_image_frequency(
							std::vector<int> &image,
							const int width, 
							const int height
						   )
		{
			int min = image[0];
			int max = image[0];
			for(size_t z=0; z < image.size(); z++){
				if(min>image[z]) min = image[z];
				if(max<image[z]) max = image[z];
			}
			if(min-max == 0) 
				return;
			for(int i=0; i< width; i++){
				for( int j=0; j<height; j++){
					auto temp = image[j* width + i];
					image[j* width + i] = 255 * ( temp - min ) / ( max - min);
				}
			}
		}
	};
}

#endif