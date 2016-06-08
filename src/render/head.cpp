
#include "render.h"

// function to calculate the head tracking view
void Render::headTracking() {
	
	bool Tracking_Head;
	//Tracking_Head=false;
	Tracking_Head=true;

				
	crossprod(this->cameraDir,this->cameraUp,this->distEye);// find the vector to rotate
	normalize(this->distEye);								// normalize it


	if(Tracking_Head) {

		rotate(this->HeadRotX,this->distEye,this->cameraDir);	// at last rotate the camera on X (UP and DOWN) for the Dir Vector
		rotate(this->HeadRotX,this->distEye,this->cameraUp);	// and the Up vector
	
	}
	
	if(Tracking_Head) {
	
		rotate(this->HeadRotY,this->cameraUp,this->cameraDir);  // first rotate the camera on Y (LEFT & RIGHT)
		rotate(this->HeadRotZ,this->cameraDir,this->cameraUp);	// second rotate the camera on Z (TRUNING)
		
		
	}
	
	
	
}
