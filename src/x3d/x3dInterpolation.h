#ifndef __x3dInterpolation_h
#define __x3dInterpolation_h

using namespace std;

#include <xercesc/dom/DOMNode.hpp>

#include "x3dGrouping2.h"
#include "x3dFields.h"
#include "x3d.h"

//class X3DInterpolatorNode : public X3DChildNode
class X3DInterpolatorNode : public X3DGroupingNode { // corrigir
	
	public:
	
	virtual ~X3DInterpolatorNode() { }
	
	virtual void *read(DOMNode *node)=0;	// pure virtual function for read the dataset
	virtual void follow()=0;	// pure virtual function to draw
	
};

class OrientationInterpolator : public X3DInterpolatorNode {
	
	public:
	
	OrientationInterpolator();
	
	// temporario, não sei se isto faz sentido mas estou precisando para a rotina do spy
	float set_fraction_tmp;

	SFFloat    *set_fraction;	//* fraction variable that defines where is positioneted the values	
	SFRotation *value_changed;	//* the return value
	
	static OrientationInterpolator *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	float key[512];
	unsigned int nkeys;	//* the keys and the number of components
	
	float keyValues[4][512];		//* the possible return values
	
	SFRotation   **value_changedPointer;
	unsigned short int countValue_changedPointer;	
	bool set[64];
	
};

class PositionInterpolator : public X3DInterpolatorNode {
	
	public:
	
	PositionInterpolator();

	// temporario, não sei se isto faz sentido mas estou precisando para a rotina do spy
	float set_fraction_tmp;
	
	SFFloat *set_fraction;	//* fraction variable that defines where is positioneted the values	
	//MFFloat key;
	float key[512];
	//MFVec3f keyValue;
	float keyValues[3][512];		//* the possible return values
	//SFNode  metadata;
	SFVec3f *value_changed;			//* the return value

	
	static PositionInterpolator *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	unsigned int nkeys;	//* the keys and the number of components
		
	SFVec3f   **value_changedPointer;
	unsigned short int countValue_changedPointer;
	bool set[64];
};

class CoordinateInterpolator : public X3DInterpolatorNode {
	
	public:
	
	CoordinateInterpolator();

	// temporario, não sei se isto faz sentido mas estou precisando para a rotina do spy
	float set_fraction_tmp;
	
	float   key[512];
	float   *value_changed;			// the return value
	float   keyValues[3][512];		//* the possible return values [xyz][no mesmo nivel]
	SFFloat *set_fraction;			//* fraction variable that defines where is positioneted the values	
	
	static CoordinateInterpolator *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	unsigned int nkeys;				//* the keys and the number of components
	unsigned int nvalues;
	
};


class ColorInterpolator : public X3DInterpolatorNode {
	
	public:
	
	ColorInterpolator();

	// temporario, não sei se isto faz sentido mas estou precisando para a rotina do spy
	float set_fraction_tmp;
	
	float key[512];
	float keyValues[3][512];		//* the possible return values [xyz][no mesmo nivel]
	
	static ColorInterpolator *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);

	unsigned int nkeys;	//* the keys and the number of components
	unsigned int nvalues;
	
};



class ScalarInterpolator : public X3DInterpolatorNode {
	
	public:
	
	ScalarInterpolator();

	// temporario, não sei se isto faz sentido mas estou precisando para a rotina do spy
	float set_fraction_tmp;
	
	SFFloat *set_fraction;	//* fraction variable that defines where is positioneted the values	
	//MFFloat key;
	float key[512];
	//MFFloat keyValue;
	float keyValues[512];		//* the possible return values
	//SFNode  metadata;
	SFFloat *value_changed;			//* the return value

	
	static ScalarInterpolator *get(DOMNode *node);
	void *read(DOMNode *node);
	void follow();
	bool ray(float o[4],float d[4],float m[4][4]);
	
	unsigned int nkeys;	//* the keys and the number of components
		
	SFFloat   **value_changedPointer;
	unsigned short int countValue_changedPointer;
	bool set[64];	
};



#endif
