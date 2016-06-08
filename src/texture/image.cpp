
#include "texture.h"

/* Constructor */
Image::Image() {
	
	this->data = NULL;
	
}

/* Destructor */
Image::~Image() {
	
	if(this->data!=NULL) 
	{
		delete[] this->data;
	}
	
}
