#define DEBUG true
#include <iostream>
#include <string>
#include "./pgm.hpp"
#include "./watershed.hpp"

int main(int argc, char *argv[]){
	if( argc <3){
		std::cout<< "Useage:"
				 <<"\n\targ1 (string): name of source image"
				 <<"\n\targ2 (int): size of mask (odd number)" 
				 << std::endl;
	}
	
	std::string source_image=  std::string(argv[1]);
	int mask_size  = std::stoi(argv[2]);
	if (mask_size % 2 == 0){
		std::cout<< "mask size must be odd" <<std::endl;
		return -1;
	}
	std::vector<unsigned char>  image;
	
	int x_dimension, y_dimension;
	UWM::PGM().read(source_image, image, x_dimension, y_dimension);
	
	std::cout<<"Running watershed"<<std::endl;
	betacore::Watershed w(image,y_dimension,x_dimension,mask_size);

	std::cout<<w.to_string()<<std::endl;

	return 0;
}