#ifndef __datavector_h
#define __datavector_h

//#define Minimum_MFVec3f 65536	// 3*15,000 values.  ISO/IEC 19775:200x, X3D
#include "x3d/x3d.h"


class datavector {
	
	public:
	
	datavector();					// Constructor
	~datavector();									// Destructor

	static float temp[Minimum_MFVec3f];

	//#ifdef JINX_DOUBLE	
		double *data;
	//#else
	//	float *data;
	//#endif

	bool aloced;
	int size; // hold the size of the vector
	
	void aloc(int size);
	//#ifdef JINX_DOUBLE	
		double get(int pos);
	//#else
	//	float get(int pos);
	//#endif
	
};

#endif
