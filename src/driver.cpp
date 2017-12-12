#define DEBUG true
#include <iostream>
#include <string>
#include "./pgm.hpp"
#include "./watershed2.hpp"

int main(int argc, char *argv[]){
	if( argc <4){
		std::cout<< "Useage:"
				 <<"\n\targ_1 (string): name of source image"
				 <<"\n\targ_2 (string): name of result image" 
				 <<"\n\targ_3 (int): size of mask (odd number)"
				 << std::endl;
	}
	
	std::string source_image =  std::string(argv[1]);
	std::string result_image =  std::string(argv[2]);
	int mask_size  = std::stoi(argv[3]);
	if (mask_size % 2 == 0){
		std::cout<< "mask size must be odd" <<std::endl;
		return -1;
	}
	std::vector<unsigned char>  image;
	
	int x_dimension, y_dimension;
	std::vector<unsigned char> result;
	UWM::PGM().read(source_image, image, x_dimension, y_dimension);
	result.resize(x_dimension*y_dimension);
	std::cout<<"Running watershed"<<std::endl;
	betacore::watershed2 w(image,y_dimension,x_dimension,mask_size);
	std::cout<<"Done with watershed!"<<std::endl;
	std::cout<<"Saving..."<<std::endl;
	w.get_filter(result);
	UWM::PGM().write(result_image,result.data(),x_dimension,y_dimension);
	std::cout<<"Saving Completed"<<std::endl;
	std::cout<<w.to_string()<<std::endl;

	return 0;
}