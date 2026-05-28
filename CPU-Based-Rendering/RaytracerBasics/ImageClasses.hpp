#ifndef IMAGECLASSES_HPP
#define IMAGECLASSES_HPP

#include<vector>
#include<string>
#include "UtilityClasses.hpp"

class Image {
	int height, width;
	std::vector<Color> data;	//store image as 1D array with three values (RGB) for each pixel
public:

	Image(int h, int w);
	void setPixel(int x, int y, Color color) ;
	int getHeight() const;
	int getWidth() const;
	Color readPixel(int x, int y) const;

};


//Future improvement maybe use factory pattern to define the output file

class ImageWriter {
public:
	virtual void writeData(std::string& filename, Image& img) = 0;	
};


class PPMImageWriter : public ImageWriter {
public:

	void writeData(std::string& filename, Image& img);

};


#endif // !IMAGECLASSES_HPP
