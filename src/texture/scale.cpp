
#include "texture.h"

/* routine for rescaling */
void Tex::rescale(Image *image) {

	#ifdef DEBUG
		cout << " size X " << image->sizeX << " size Y " << image->sizeY << endl;
	#endif
	
	char *new_image;
	
	unsigned int nsizeX=0;		// new size in X to used in pot 2, if necessary
	unsigned int nsizeY=0;		// new size in Y to used in pot 2, if necessary
	
	unsigned int size_pot2;
	
	for(size_pot2=2 ; ; size_pot2*=2 ) {
		if(image->sizeX <= size_pot2 ) {
			if(image->sizeX == size_pot2 ) {
				nsizeX=image->sizeX;
				break;	
			} else {
				nsizeX=size_pot2;
				break;
			}
		}
	}
	
	for(size_pot2=2 ; ; size_pot2*=2 ) {
		if(image->sizeY <= size_pot2 ) {
			if(image->sizeY == size_pot2 ) {
				nsizeY=image->sizeY;
				break;	
			} else {
				nsizeY=size_pot2;
				break;
			}
		}
	}
	
	if( !( (nsizeX==image->sizeX) && (nsizeY==image->sizeY) ) ) 
	{
	
		#ifdef DEBUG
			cout << " size X " << nsizeX << " size Y " << nsizeY << endl;
		#endif
		
		if(image->alpha) {
			
			new_image = new char[nsizeX*nsizeY*4];
	
			for(unsigned int f=0;f<nsizeY;f++) {
				for(unsigned int g=0;g<nsizeX;g++) {
			
					unsigned int newposition = ((int)( 4 * (g+(f*nsizeX) ) ) );
					unsigned int oldposition = ((int)( 4 * (
								  ( (int)( g * ( ((float)(image->sizeX)) / ((float)(nsizeX)) ) ) ) +
								( ( (int)( f * ( ((float)(image->sizeY)) / ((float)(nsizeY)) ) ) ) * ((float)(image->sizeX)) ) 
								) ) );
					
					new_image[ 0 + newposition ] = image->data[ 0 + oldposition ];
					new_image[ 1 + newposition ] = image->data[ 1 + oldposition ];
					new_image[ 2 + newposition ] = image->data[ 2 + oldposition ];
					new_image[ 3 + newposition ] = image->data[ 3 + oldposition ];
			
					#ifdef DEBUG
						cout << "new position " << newposition << endl;
						cout << "old position " << oldposition << endl;
					#endif
					
				}
			}
	
		} else {
	
			new_image = new char[nsizeX*nsizeY*3];
			
			for(unsigned int f=0;f<nsizeY;f++) {
				for(unsigned int g=0;g<nsizeX;g++) {
			
					unsigned int newposition = ((int)( 3 * (g+(f*nsizeX) ) ) );
					unsigned int oldposition = ((int)( 3 * (
								  ( (int)( g * ( ((float)(image->sizeX)) / ((float)(nsizeX)) ) ) ) +
								( ( (int)( f * ( ((float)(image->sizeY)) / ((float)(nsizeY)) ) ) ) * ((float)(image->sizeX)) ) 
								) ) );
					
					new_image[ 0 + newposition ] = image->data[ 0 + oldposition ];
					new_image[ 1 + newposition ] = image->data[ 1 + oldposition ];
					new_image[ 2 + newposition ] = image->data[ 2 + oldposition ];
			
					#ifdef DEBUG
						cout << "new position " << newposition << endl;
						cout << "old position " << oldposition << endl;
					#endif
					
				}
			}
		}
	
		image->sizeX = nsizeX;
		image->sizeY = nsizeY;

		delete[] image->data;
		
		image->data = new char[((nsizeX) * (nsizeY) * (image->alpha?4:3))];
		memcpy(image->data,new_image,((nsizeX) * (nsizeY) * (image->alpha?4:3)));
		delete[] new_image;

	}	
}
