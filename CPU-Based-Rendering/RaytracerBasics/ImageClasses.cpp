#include<iostream>
#include<fstream>
#include "ImageClasses.hpp"
#include "UtilityClasses.hpp"

//----------------Image class definitions --------//

Image::Image(int h, int w): height(h), width(w){
	size_t size = size_t(h) * size_t(w);	//to avoid overflow as h * w will have int output and then it will be converted to size_t if we do not do this, resuting in overflow
	data.resize(size);
}

int Image::getHeight() const{
	return height;
}

int Image::getWidth() const {
	return width;
}

void Image::setPixel(int x, int y, Color color) {
	//the color is in normalized form, it will be converted to correct form when writing to file
	size_t idx = (size_t(y) * size_t(width)) + size_t(x);
	data[idx] = color;
}


Color Image:: readPixel(int x, int y) const {
	size_t idx = (size_t(y) * size_t(width)) + size_t(x);
	if (idx > data.size()) {
		return Color(0, 0, 0);
	}
	else {
		return data[idx];
	}
}



//---------PPMWriter class definitions-------//

void PPMImageWriter::writeData(std::string& filename, Image& img, int colorDepth) {
	
	std::ofstream file;

	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << "\n";
	}
	int colorRes = std::pow(2, colorDepth) - 1;
	int height = img.getHeight();
	int width = img.getWidth();
	//writing the PPM header
	file << "P3\n"; // PPM format
	file << width <<" "<< height <<"\n"; // width and height
	file << colorRes << "\n"; // max color value

	//write the image data
	Color color;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//j = x co-ordinate, i = y co-ordinate
			color = img.readPixel(j, i);	//converting to proper bit resolution
			
			file << int(color.x * colorRes) << " " << int(color.y * colorRes) << " " << int(color.z * colorRes) << "\n";
		}
	}

	//close the file 
	if (file.is_open()){
		file.close();
	}
}