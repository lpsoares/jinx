#include "x3d.h" 
#include "x3dNurbs.h"


#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

 
NurbsCurve::NurbsCurve() {
	f=0;						//* counter to fill the variable coord 

	numberOfPoints=0;

}

bool NurbsCurve::ray(float o2[4], float d2[4], float m2[4][4]) {
		return(false);
}

void NurbsCurve::follow() {}

void NurbsCurve::draw() {
	
	// This fucntion could have problems if the number of points is different in the interpolator e nurbs curve
	#ifdef DEBUG
		cout << "NUMBER OF POINTS : " << numberOfPoints << endl;
	#endif
	
	if ( numberOfPoints == 0 ) cerr << "Not points defined in Nurbs Curve" << endl;
	
	glMap1f(GL_MAP1_VERTEX_3,0,100,3,numberOfPoints,controlPoint);
		
	glEnable(GL_MAP1_VERTEX_3);
	
	glBegin(GL_LINE_STRIP);
		
	float c;
	for(c=0;c<=100;c++) {
		glEvalCoord1f(c);
	}
	
	glEnd();
	
}
 

NurbsCurve *NurbsCurve::get(DOMNode *node) {
	
	NurbsCurve *nurbsCurve;
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((NurbsCurve *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	nurbsCurve = new NurbsCurve();
	nurbsCurve->read(node);
	
	
	return nurbsCurve;
	
}

void *NurbsCurve::read(DOMNode *node) {
	unsigned int i; 				//* variable to counter
	DOMNamedNodeMap *attributes;	//* variable to hold the node attributes
	
	char *ctemp;					//* temporary variable
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "DEF")) {
			strcpy(this->DEF,XMLString::transcode (attributes->item(i)->getNodeValue()));
			setLink(XMLString::transcode(attributes->item(i)->getNodeValue()),this);
		} else
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return(getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		}
	}
		
		
		
 	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "tessellation")) {
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "containerField")) {
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "order")) {
		} else
		if ( !strcmp(XMLString::transcode (attributes->item(i)->getNodeName()) , "controlPoint")) {
			numberOfPoints=0;
			ctemp = strtok(XMLString::transcode (attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				points[numberOfPoints][0] = atof(ctemp);
				ctemp = strtok(NULL," ");
				points[numberOfPoints][1] = atof(ctemp);
				ctemp = strtok(NULL," ");
				points[numberOfPoints++][2] = atof(ctemp);
				ctemp = strtok(NULL," ");
			
				#ifdef DEBUG
					cout << "Points : " << points[numberOfPoints-1][0] << " , " << 
										   points[numberOfPoints-1][1] << " , " << 
										   points[numberOfPoints-1][2] << endl;
				#endif
			
			}
		}
	}
	

	#ifdef DEBUG
		cout << "Number of Points : " << numberOfPoints << endl;
	#endif

	controlPoint = new float[3*numberOfPoints];

	unsigned int f;
	for(f=0;f<routeStruct::routeCount;f++) {
		if (!strcmp (x3d::my_x3d->routeTable[f]->toNode,DEF)) {
			if(!strcmp(x3d::my_x3d->routeTable[f]->toField,"set_controlPoint")) {
				if((*x3d::my_x3d->routeTable[f]->field2)==NULL) {
					(*x3d::my_x3d->routeTable[f]->field2) = new float[3*numberOfPoints];
				}
				delete controlPoint;
				controlPoint = (float *)(*x3d::my_x3d->routeTable[f]->field2);
			}
		}
	}
	

	return(NULL);
	
}
