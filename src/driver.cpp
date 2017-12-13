#define DEBUG true
#include <iostream>
#include <string>
#include "./pgm.hpp"
#include "./watershed2.hpp"
#include "./sobel.hpp"

int main(int argc, char *argv[]){
	if( argc <2){
		std::cout<< "Useage:"
				 <<"\n\t-source (string): name of source image (required)"
				 <<"\n\t-dir    (string): save directory for files (required)"
				 <<"\n\t-name   (string): save filename (required) "
				 <<"\n\t-neighborhood (int): size of neighborhood (default 8)"
				  <<"\n\t-gradient: use gradient of the source image"
				 <<"\n\t-split: turn on split option"
				 << std::endl;
	}
	std::string source_image;
	std::string dir;
	std::string name;
	
	int neighborhood  = 8;
	bool split = false;
	bool gradient =false;
	for(int i=0; i<argc; i++){
		if(std::string(argv[i]) == "-source") {
			source_image =  std::string(argv[i+1]);
			
		}		
		if(std::string(argv[i]) == "-dir") {
			dir =  std::string(argv[i+1]);
			
		}
		if(std::string(argv[i]) == "-name") {
			name =  std::string(argv[i+1]);
			
		}
		if(std::string(argv[i]) == "-neighborhood") {
			neighborhood =  std::stoi(argv[i+1]);
			
		}
		if(std::string(argv[i]) == "-gradient") {
			gradient =true;
		
		}
	
		if(std::string(argv[i])== "-split"){
			split =true;
		}
	}
	  std::ostringstream arg_text;
	 	arg_text<< "_config_";
        arg_text << "_gradient_"<<gradient;
		arg_text << "_neighborhood_"<<neighborhood ;
		arg_text << "_split_"<<split;
		arg_text << "_";
	
	std::string result_image = dir + "result"+arg_text.str()+name;
	std::string inverted_image = dir + "inverted"+arg_text.str()+name;
	std::string binary_image = dir + "binary"+arg_text.str()+name;
	std::string gradient_image = dir + "gradient"+arg_text.str()+name;

	if (neighborhood  != 4 && neighborhood !=8){
		std::cout<< "neighborhood must be 4 or 8" <<std::endl;
		return -1;
	}
	std::vector<unsigned char>  image;

	int x_dimension, y_dimension;

	UWM::PGM().read(source_image, image, x_dimension, y_dimension);
	std::vector<unsigned char> result;
	result.resize(x_dimension*y_dimension);
	
	if(gradient == true){
		std::vector<unsigned char>  gradient;
		gradient.resize(x_dimension*y_dimension);
		betacore::sobel<unsigned char> sob;
		sob.apply(image,x_dimension,y_dimension,gradient);
		UWM::PGM().write(gradient_image ,gradient.data(),x_dimension,y_dimension);
		std::cout<<"Running watershed Gradient"<<std::endl;
		betacore::watershed2 w(gradient,y_dimension,x_dimension,neighborhood,split);
		w.get_filter(result);
		std::cout<<w.to_string()<<std::endl;
	}else{
		std::cout<<"Running watershed"<<std::endl;
		betacore::watershed2 w(image,y_dimension,x_dimension,neighborhood,split);
		w.get_filter(result);
		std::cout<<w.to_string()<<std::endl;
	}


	
	std::cout<<"Done with watershed!"<<std::endl;
	std::cout<<"Saving..."<<std::endl;

	UWM::PGM().write(binary_image,result.data(),x_dimension,y_dimension);
	std::cout<<"Saving Completed"<<std::endl;
	std::vector<unsigned char> temp;
	temp.resize(x_dimension*y_dimension);
	for(size_t i=0; i< image.size(); ++i){
		temp[i] = (result[i] == 255) image[i]:0 ;
	}
	UWM::PGM().write(result_image,temp.data(),x_dimension,y_dimension);
	for(size_t i=0; i< image.size(); ++i){
		temp[i] = (result[i] == 0)? image[i] : 0 ;
	}
	UWM::PGM().write(inverted_image,temp.data(),x_dimension,y_dimension);

	return 0;
}