
#include "x3dEnvironmental.h"
#include "../render/render.h"

#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>


Background::Background() {
	
	skyColor[0][0]=0;				//color for first sky level
	skyColor[0][1]=0;				//color for first sky level
	skyColor[0][2]=0;				//color for first sky level
	
	this->point=0; 					// counter for sky angles
	this->gpoint=0; 				// counter for ground angles
	
	this->cskyAngle=0;				// counter for sky angles
	this->cgroundAngle=0;			// counter for ground angles
	
	strcpy(this->backUrl[0],"");	//reseting the url for sky
	strcpy(this->bottomUrl[0],"");	//reseting the url for sky
	strcpy(this->frontUrl[0],"");	//reseting the url for sky
	strcpy(this->leftUrl[0],"");	//reseting the url for sky
	strcpy(this->rightUrl[0],"");	//reseting the url for sky
	strcpy(this->topUrl[0],"");		//reseting the url for sky

	BackgrounfTexture=false;		// defines if the background has a texture applied or not
	
}

#define DIVISIONS 32		// divisions for the sphere that defines the sky

// draws the sky (and floor)
void Background::follow() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Background)" << endl;	// Message about debuging
	#endif
		
	int f;
	double ang;
	unsigned int counter=0;
	
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glPushAttrib(GL_POLYGON_BIT);						//
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);			//
	
	/* Somente para DEGUB */
	/*
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glTranslatef(0,0,-100);
	glScalef(0.01,0.01,0.01);
	*/

	glTranslatef(-Render::my_rrr->posRender->render.pos[0],
				//-Render::my_rrr->posRender->render.pos[1],
				0,
				 -Render::my_rrr->posRender->render.pos[2]);

	
	if(BackgrounfTexture) {
		
		#ifdef DEBUG2
			cout << "Drawing Background texture" << endl;
		#endif
		
		glEnable(GL_TEXTURE_2D);							//
		
		glScalef( (FRUSTUM_FAR*0.7), (FRUSTUM_FAR*0.7), (FRUSTUM_FAR*0.7) );

		float radius1;
		float radius2;
		float f;
		int i;

		if( strcmp(this->frontUrl[0],"") ) {
	
			Render::my_rrr->tex->readTexture(this->frontUrl[0],false,false);				//
	
			if(true) {
		
				f=0.0;
			
				glBegin(GL_TRIANGLE_STRIP);
		
				for(i=0; i<=8; i++) {
					radius1=cos(f);
					radius2=sin(f);
					glNormal3f( (radius1) ,0.0, (radius2));
					glTexCoord2f(f/(2*(PI/4.0)),0.0);
					glVertex3f(radius1, 0.0, radius2);
					glTexCoord2f(f/(2*(PI/4.0)),1.0);
					glVertex3f(radius1, 1.0, radius2);
					f+=(PI/2.0)/8.0;
				}	
		
				glEnd();

			} else {
		
				glBegin(GL_QUADS);
				// Front Face
				glNormal3f( 0.0, 0.0, 1.0);
				glTexCoord2f(1.0,0.0);
				glVertex3f( 1.0,-1.0, 1.0);
				glTexCoord2f(1.0,1.0);
				glVertex3f( 1.0, 1.0, 1.0);
				glTexCoord2f(0.0,1.0);
				glVertex3f(-1.0, 1.0, 1.0);
				glTexCoord2f(0.0,0.0);
				glVertex3f(-1.0,-1.0, 1.0);
				glEnd();
			
			}
	
		}
	
	
		if( strcmp(this->rightUrl[0],"") ) {
				
			Render::my_rrr->tex->readTexture(this->rightUrl[0],false,false);				//

			if(true) {
			
				f=PI/2.0;
		
				glBegin(GL_TRIANGLE_STRIP);
		
				for(i=0; i<=8; i++) {
					radius1=cos(f);
					radius2=sin(f);
					glNormal3f( (radius1) ,0, (radius2));
					glTexCoord2f( (f-(PI/2.0))/(2.0*(PI/4.0)),0.0);
					glVertex3f(radius1, 0.0, radius2);
					glTexCoord2f( (f-(PI/2.0))/(2.0*(PI/4.0)),1.0);
					glVertex3f(radius1,1.0, radius2);
					f+=(PI/2.0)/8.0;
				}
		
				glEnd();

			} else {
		
				glBegin(GL_QUADS);
				// Right Face
				glNormal3f( 1.0, 0.0, 0.0);
				glTexCoord2f(1.0,0.0);
				glVertex3f( 1.0,-1.0,-1.0);
				glTexCoord2f(1.0,1.0);
				glVertex3f( 1.0, 1.0,-1.0);
				glTexCoord2f(0.0,1.0);
					glVertex3f( 1.0, 1.0, 1.0);
				glTexCoord2f(0.0,0.0);
				glVertex3f( 1.0,-1.0, 1.0);
				glEnd();

			}
			
		}
	
	
		if( strcmp(this->backUrl[0],"") ) {
			
			Render::my_rrr->tex->readTexture(this->backUrl[0],false,false);				//

			if(true) {
		
				f=PI*1.0;
		
				glBegin(GL_TRIANGLE_STRIP);
		
				for(i=0; i<=8; i++) {
					radius1=cos(f);
					radius2=sin(f);
					glNormal3f( (radius1) ,0, (radius2));
					glTexCoord2f((f-(PI))/(2.0*(PI/4.0)),0.0);
					glVertex3f(radius1, 0.0, radius2);
					glTexCoord2f((f-(PI))/(2.0*(PI/4.0)),1.0);
					glVertex3f(radius1, 1.0, radius2);
					f+=(PI/2.0)/8.0;
				}
		
				glEnd();
		
			} else {
				
				glBegin(GL_QUADS);
				// Back Face
				glNormal3f( 0.0, 0.0,-1.0);
				glTexCoord2f(0.0,0.0);
				glVertex3f(-1.0,-1.0,-1.0);
				glTexCoord2f(0.0,1.0);
				glVertex3f(-1.0, 1.0,-1.0);
				glTexCoord2f(1.0,1.0);
				glVertex3f( 1.0, 1.0,-1.0);
				glTexCoord2f(1.0,0.0);
				glVertex3f( 1.0,-1.0,-1.0);
				glEnd();

			}
		
		}
	
	

		if( strcmp(this->leftUrl[0],"") ) {
		
			Render::my_rrr->tex->readTexture(this->leftUrl[0],false,false);				//

			if(true) {
		
				f=3.0*(PI/2.0);
				glBegin(GL_TRIANGLE_STRIP);
		
				for(i=0; i<=8; i++) {
					radius1=cos(f);
					radius2=sin(f);
					glNormal3f( (radius1) ,0, (radius2));
					glTexCoord2f((f-(3.0*PI/2.0))/(2.0*(PI/4.0)),0.0);
					glVertex3f(radius1, 0.0, radius2);
					glTexCoord2f((f-(3.0*PI/2.0))/(2.0*(PI/4.0)),1.0);
					glVertex3f(radius1, 1.0, radius2);
					f+=(PI/2.0)/8.0;}
		
				glEnd();

			} else {
		
				glBegin(GL_QUADS);
				// Left Face
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(1.0,0.0);
				glVertex3f(-1.0,-1.0, 1.0);
				glTexCoord2f(1.0,1.0);
				glVertex3f(-1.0, 1.0, 1.0);
				glTexCoord2f(0.0,1.0);
				glVertex3f(-1.0, 1.0,-1.0);
				glTexCoord2f(0.0,0.0);
				glVertex3f(-1.0,-1.0,-1.0);
				glEnd();
		
			}
		
		}
		
	
		if( strcmp(this->topUrl[0],"") ) {
	
			Render::my_rrr->tex->readTexture(this->topUrl[0],false,false);				//
			
			glBegin(GL_QUADS);
			// Top Face
			glNormal3f( 0.0, 1.0, 0.0);
			glTexCoord2f(1.0,0.0);
			glVertex3f( 1.0, 1.0, 1.0);
			glTexCoord2f(1.0,1.0);
			glVertex3f( 1.0, 1.0,-1.0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(-1.0, 1.0,-1.0);
			glTexCoord2f(0.0,0.0);
			glVertex3f(-1.0, 1.0, 1.0);
			glEnd();
		
		}
	
		if( strcmp(this->bottomUrl[0],"")) {
			
			Render::my_rrr->tex->readTexture(this->bottomUrl[0],false,false);				//
	
			glBegin(GL_QUADS);
			// bottom Face
			glNormal3f( 0.0,-1.0, 0.0);
			glTexCoord2f(1.0,0.0);
			glVertex3f( 1.0,-1.0,-1.0);
			glTexCoord2f(1.0,1.0);
			glVertex3f( 1.0,-1.0, 1.0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(-1.0,-1.0, 1.0);
			glTexCoord2f(0.0,0.0);
			glVertex3f(-1.0,-1.0,-1.0);
			glEnd();
	
		}
		
		glDisable(GL_TEXTURE_2D);							//
		
	
	}
	
	
	
	// point means up part (SKY)
	if(this->point>0) {

		#ifdef DEBUG2
			cout << "Drawing Sky" << endl;
		#endif
		
		glClearColor(skyColor[0][0],skyColor[0][1],skyColor[0][2], 0.0);
		#ifdef DEBUG3
			cout << "Clear Color because Background : " << skyColor[0][0] << " , " << skyColor[0][1] << " , " << skyColor[0][2] << endl;
		#endif
	
		// The top part		
		if(cskyAngle>0) {
			
			glFrontFace(GL_CW);
			
			// In the case that the first angle is bigget than 90 degrees
			if(skyAngle[0] <= (PI/2) ) {
			
				// just the cap border				
				glBegin(GL_TRIANGLE_FAN);
		
				glColor3dv(skyColor[0]);

				glNormal3d(0,-1,0);
				glVertex3d(0.0,FRUSTUM_FAR,0.0);
				
				glColor3dv(skyColor[counter+1]);
				for(f=0;f<=DIVISIONS;f++) {
					ang=(double)f*((2*PI)/DIVISIONS);
					glNormal3d(-sin(skyAngle[0])*sin(ang),-cos(skyAngle[0]),-sin(skyAngle[0])*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(skyAngle[0])*sin(ang),FRUSTUM_FAR*cos(skyAngle[0]),FRUSTUM_FAR*sin(skyAngle[0])*cos(ang));
				}
											
				glEnd();
				
				
				// the midle border up part
				while( (skyAngle[counter+1] <= (PI/2)) && (counter < (cskyAngle-1)) ) {  // ta ruim
					#ifdef DEBUG3
						cout << "midle border up part color : " << skyColor[counter+1][0] << " , " << skyColor[counter+1][1] << " , " << skyColor[counter+1][2] << endl;
						cout << "\t and : " << skyColor[counter+2][0] << " , " << skyColor[counter+2][1] << " , " << skyColor[counter+2][2] << endl;
					#endif
					glBegin(GL_TRIANGLE_STRIP);
					for(f=0;f<=DIVISIONS;f++) {
						ang=(double)f*((2*PI)/DIVISIONS);
		
						glColor3dv(skyColor[counter+1]);
						glNormal3d(-sin(skyAngle[counter])*sin(ang),-cos(skyAngle[counter]),-sin(skyAngle[counter])*cos(ang));
						glVertex3d(FRUSTUM_FAR*sin(skyAngle[counter])*sin(ang),FRUSTUM_FAR*cos(skyAngle[counter]),FRUSTUM_FAR*sin(skyAngle[counter])*cos(ang));

						glColor3dv(skyColor[counter+2]);
						glNormal3d(-sin(skyAngle[counter+1])*sin(ang),-cos(skyAngle[counter+1]),-sin(skyAngle[counter+1])*cos(ang));
						glVertex3d(FRUSTUM_FAR*sin(skyAngle[counter+1])*sin(ang),FRUSTUM_FAR*cos(skyAngle[counter+1]),FRUSTUM_FAR*sin(skyAngle[counter+1])*cos(ang));

					}
					glEnd();
					counter++;
				}
				
				// the midle border until the midle
				glBegin(GL_TRIANGLE_STRIP);
				#ifdef DEBUG3
					cout << "midle border until the midle part color : " << skyColor[counter+1][0] << " , " << skyColor[counter+1][1] << " , " << skyColor[counter+1][2] << endl;
				#endif
				for(f=0;f<=DIVISIONS;f++) {

					ang=(double)f*((2*PI)/DIVISIONS);
		
					glColor3dv(skyColor[counter+1]);
					glNormal3d(-sin(skyAngle[counter])*sin(ang),-cos(skyAngle[counter]),-sin(skyAngle[counter])*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(skyAngle[counter])*sin(ang),FRUSTUM_FAR*cos(skyAngle[counter]),FRUSTUM_FAR*sin(skyAngle[counter])*cos(ang));

					if(skyAngle[counter+1] <= (PI/2)) {
						glColor3dv(skyColor[counter+1]);
					} else {
						glColor3dv(skyColor[counter+2]);
					}
					glNormal3d(-sin(ang), 0 ,-cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(ang), 0 ,FRUSTUM_FAR*cos(ang));

				}
				counter++;
				glEnd();
				
				
			} else {
				
				cout << "ELSE NO BACKGROUND" << endl;
				glBegin(GL_TRIANGLE_FAN);
				
				glColor3dv(skyColor[0]);
				glNormal3d(0,-1,0); // EXPERIMENTAL
				glVertex3d(0.0,FRUSTUM_FAR,0.0);
				
				//the whole cap
				for(f=0;f<=DIVISIONS;f++) {
					ang=(double)f*((2*PI)/DIVISIONS);
					glNormal3d(-sin(PI/2)*sin(ang),-cos(PI/2),-sin(PI/2)*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(PI/2)*sin(ang),FRUSTUM_FAR*cos(PI/2),FRUSTUM_FAR*sin(PI/2)*cos(ang));
					
				}
				glEnd();
				
			}			
	
			
		}
	} else {
		glClearColor(0.0,0.0,0.0, 0.0);
	}

	
	
	counter=0;  // reseting again the counter

	// point means down part (GROUND)
	if(this->gpoint>0) {		
	
		#ifdef DEBUG2
			cout << "Drawing Ground" << endl;
		#endif
		
		// The top part		
		if(cgroundAngle>0) {
			
			glFrontFace(GL_CW);
			
			// In the case that the first angle is bigget than 90 degrees
			if(groundAngle[0] <= (PI/2) ) {
			
				// just the cap border				
				glBegin(GL_TRIANGLE_FAN);
		
				glColor3dv(groundColor[0]);

				glNormal3d(0,1,0);
				glVertex3d(0.0,-FRUSTUM_FAR,0.0);
				
				glColor3dv(groundColor[counter+1]);
				for(f=0;f<=DIVISIONS;f++) {
					ang=(double)f*((2*PI)/DIVISIONS);
					glNormal3d(-sin(groundAngle[0])*sin(ang),-cos(groundAngle[0]),-sin(groundAngle[0])*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(groundAngle[0])*sin(ang),-FRUSTUM_FAR*cos(groundAngle[0]),FRUSTUM_FAR*sin(groundAngle[0])*cos(ang));
				}
											
				glEnd();
				
				
				// the midle border up part
				while( (groundAngle[counter+1] <= (PI/2)) && (counter < (cgroundAngle-1)) ) {  // ta ruim
					#ifdef DEBUG3
						cout << "midle border up part color : " << groundColor[counter+1][0] << " , " << groundColor[counter+1][1] << " , " << groundColor[counter+1][2] << endl;
						cout << "\t and : " << groundColor[counter+2][0] << " , " << groundColor[counter+2][1] << " , " << groundColor[counter+2][2] << endl;
					#endif
					glBegin(GL_TRIANGLE_STRIP);
					for(f=0;f<=DIVISIONS;f++) {
						ang=(double)f*((2*PI)/DIVISIONS);
		
						glColor3dv(groundColor[counter+1]);
						glNormal3d(-sin(groundAngle[counter])*sin(ang),-cos(groundAngle[counter]),-sin(groundAngle[counter])*cos(ang));
						glVertex3d(FRUSTUM_FAR*sin(groundAngle[counter])*sin(ang),-FRUSTUM_FAR*cos(groundAngle[counter]),FRUSTUM_FAR*sin(groundAngle[counter])*cos(ang));

						glColor3dv(groundColor[counter+2]);
						glNormal3d(-sin(groundAngle[counter+1])*sin(ang),-cos(groundAngle[counter+1]),-sin(groundAngle[counter+1])*cos(ang));
						glVertex3d(FRUSTUM_FAR*sin(groundAngle[counter+1])*sin(ang),-FRUSTUM_FAR*cos(groundAngle[counter+1]),FRUSTUM_FAR*sin(groundAngle[counter+1])*cos(ang));

					}
					glEnd();
					counter++;
				}
				
				// the midle border until the midle
				glBegin(GL_TRIANGLE_STRIP);
				#ifdef DEBUG3
					cout << "midle border until the midle part color : " << skyColor[counter+1][0] << " , " << skyColor[counter+1][1] << " , " << skyColor[counter+1][2] << endl;
				#endif
				for(f=0;f<=DIVISIONS;f++) {

					ang=(double)f*((2*PI)/DIVISIONS);
		
					glColor3dv(groundColor[counter+1]);
					glNormal3d(-sin(groundAngle[counter])*sin(ang),-cos(groundAngle[counter]),-sin(groundAngle[counter])*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(groundAngle[counter])*sin(ang),-FRUSTUM_FAR*cos(groundAngle[counter]),FRUSTUM_FAR*sin(groundAngle[counter])*cos(ang));

					if(groundAngle[counter+1] <= (PI/2)) {
						glColor3dv(groundColor[counter+1]);
					} else {
						glColor3dv(groundColor[counter+2]);
					}
					glNormal3d(-sin(ang), 0 ,-cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(ang), 0 ,FRUSTUM_FAR*cos(ang));

				}
				counter++;
				glEnd();
				
				
			} else {
				
				cout << "ELSE NO BACKGROUND" << endl;
				glBegin(GL_TRIANGLE_FAN);
				
				glColor3dv(groundColor[0]);
				glNormal3d(0,-1,0); // EXPERIMENTAL
				glVertex3d(0.0,FRUSTUM_FAR,0.0);
				
				//the whole cap
				for(f=0;f<=DIVISIONS;f++) {
					ang=(double)f*((2*PI)/DIVISIONS);
					glNormal3d(-sin(PI/2)*sin(ang),-cos(PI/2),-sin(PI/2)*cos(ang));
					glVertex3d(FRUSTUM_FAR*sin(PI/2)*sin(ang),FRUSTUM_FAR*cos(PI/2),FRUSTUM_FAR*sin(PI/2)*cos(ang));
					
				}
				glEnd();
				
			}			
	
			
		}
	}
	
	
	glEnable(GL_LIGHTING);
	glPopAttrib();
	glPopMatrix();
	
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Background)" << endl;	// Message about debuging
	#endif
		
}

Background *Background::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Background *background;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Background *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	background = new Background();
	background->read(node);
	
	return background;
}

void Background::read(DOMNode *node) {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReading (Background)" << endl;	// Message about debuging
	#endif
	
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	unsigned int i; 				// variable to counter
	
	char *ctemp=NULL;	// temporary for parsing
	
	char *tmp1;				// just to read the temporary tokens
	int couter_urlbackUrl=0;		// in the case of many texture files in one specification
	int couter_urlbottomUrl=0;		// in the case of many texture files in one specification
	int couter_urlfrontUrl=0;		// in the case of many texture files in one specification
	int couter_urlleftUrl=0;		// in the case of many texture files in one specification
	int couter_urlrightUrl=0;		// in the case of many texture files in one specification
	int couter_urltopUrl=0;		// in the case of many texture files in one specification

	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "skyColor")) {
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				this->skyColor[point][0] = atof(ctemp);
				ctemp = strtok(NULL," ");
				this->skyColor[point][1] = atof(ctemp);
				ctemp = strtok(NULL," ");
				this->skyColor[point][2] = atof(ctemp);
				ctemp = strtok(NULL," ");
				#ifdef DEBUG
					cout << "Sky Color " << point << " : " << this->skyColor[point][0] << " , " 
														   << this->skyColor[point][1] << " , " 
														   << this->skyColor[point][2] << endl;
				#endif
				point++;
			}
		} else 
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "groundColor")) {
		ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				this->groundColor[gpoint][0] = atof(ctemp);
				ctemp = strtok(NULL," ");
				this->groundColor[gpoint][1] = atof(ctemp);
				ctemp = strtok(NULL," ");
				this->groundColor[gpoint][2] = atof(ctemp);
				ctemp = strtok(NULL," ");
				#ifdef DEBUG
					cout << "Ground Color " << point << " : " << this->groundColor[gpoint][0] << " , " 
															  << this->groundColor[gpoint][1] << " , " 
															  << this->groundColor[gpoint][2] << endl;
				#endif
				gpoint++;
			}
		} else 
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "skyAngle")) {
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				skyAngle[cskyAngle] = atof(ctemp);
				ctemp = strtok(NULL," ");
				#ifdef DEBUG
					cout << "Sky Angle " << cskyAngle << " : " << this->skyAngle[cskyAngle] << endl;
				#endif
				cskyAngle++;
			}
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "groundAngle")) {
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				groundAngle[cgroundAngle] = atof(ctemp);
				ctemp = strtok(NULL," ");
				#ifdef DEBUG
					cout << "Ground Angle " << cgroundAngle << " : " << this->groundAngle[cgroundAngle] << endl;
				#endif
				cgroundAngle++;
			}
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "backUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->backUrl[couter_urlbackUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->backUrl[couter_urlbackUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->backUrl[couter_urlbackUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->backUrl[couter_urlbackUrl],"")) {
					couter_urlbackUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "bottomUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->bottomUrl[couter_urlbottomUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->bottomUrl[couter_urlbottomUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->bottomUrl[couter_urlbottomUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->bottomUrl[couter_urlbottomUrl],"")) {
					couter_urlbottomUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "frontUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->frontUrl[couter_urlfrontUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->frontUrl[couter_urlfrontUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->frontUrl[couter_urlfrontUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->frontUrl[couter_urlfrontUrl],"")) {
					couter_urlfrontUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "leftUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->leftUrl[couter_urlleftUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->leftUrl[couter_urlleftUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->leftUrl[couter_urlleftUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->leftUrl[couter_urlleftUrl],"")) {
					couter_urlleftUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "rightUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->rightUrl[couter_urlrightUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->rightUrl[couter_urlrightUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->rightUrl[couter_urlrightUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->rightUrl[couter_urlrightUrl],"")) {
					couter_urlrightUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		} else
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "topUrl")) {
			tmp1 = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while(tmp1!=NULL) {
				if( (tmp1[0]=='"') || (tmp1[0]=='\'')) {
					strncpy(this->topUrl[couter_urltopUrl],tmp1+1,((int)(strlen(tmp1)))-2);
					this->topUrl[couter_urltopUrl][((int)(strlen(tmp1)))-2]='\0';
				} else {
					strcpy(this->topUrl[couter_urltopUrl],tmp1);
				}
				tmp1 = strtok(NULL," ");
				if(strcmp(this->topUrl[couter_urltopUrl],"")) {
					couter_urltopUrl++;
					this->BackgrounfTexture=true;
				}
			}
			
		}
		
		
	}

	#ifdef DEBUG1
		cout << "\t\t\t\tLeaving Reading (Background)" << endl;	// Message about debuging
	#endif
	
	
}


// Constructor
Fog::Fog() {
	
	this->color = new SFColor[3];
	*this->color[0] = 1.0f;
	*this->color[1] = 1.0f;
	*this->color[2] = 1.0f;
	
	this->fogType = new SFString[256];
	strcpy(*this->fogType,"LINEAR");
	
	this->visibilityRange = new SFFloat;
	*this->visibilityRange = 0.0f;
}

void Fog::follow() {

	#ifdef DEBUG1
		cout << "\t\t\t\tFollowing (Fog)" << endl;	// Message about debuging
	#endif
	
	GLfloat fogColor[4];
	
	if(*this->visibilityRange!=0.0f) {
	
		glEnable(GL_FOG);
		
		fogColor[0]=*this->color[0];
		fogColor[1]=*this->color[1];
		fogColor[2]=*this->color[2];
		fogColor[3]=1.0f;
	
		if(!strcasecmp( *this->fogType,"LINEAR" )) {
			glFogi(GL_FOG_MODE, GL_LINEAR);
		} else {  // EXPONENTIAL
			glFogi(GL_FOG_MODE, GL_EXP);
		}
	
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, 0.35);
		glHint(GL_FOG_HINT,GL_DONT_CARE);
		glFogf(GL_FOG_START, 0.0f);
		
		#ifdef DEBUG1
			cout << "Visibily Range : " << *this->visibilityRange << endl;
		#endif
		glFogf(GL_FOG_END, *this->visibilityRange);
	
	} else {
		
		glDisable(GL_FOG);
	
	}
		
	
	#ifdef DEBUG1
		cout << "\t\t\t\tReturning (Fog)" << endl;	// Message about debuging
	#endif
		
}

Fog *Fog::get(DOMNode *node) {
	
	unsigned int i; 				// variable to counter
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	
	Fog *fog;
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "USE")) {
			return((Fog *)getLink(XMLString::transcode(attributes->item(i)->getNodeValue())));
		} 
	}
	
	fog = new Fog();
	fog->read(node);
	
	return fog;
}


// Ray casting fog (always false)
bool Fog::ray(float o[4],float d[4], float m[4][4]) {
	
	#ifdef DEBUG1
		cout << "\t\t\t\tRay casting (Fog)" << endl;	// Message about debuging
	#endif
	
	return(false);
}

// Reading Fog
void *Fog::read(DOMNode *node) {
	
	
	#ifdef DEBUG1
		cout << "\t\t\t\tRead (Fog)" << endl;	// Message about debuging
	#endif
	
	
	DOMNamedNodeMap *attributes;	// variable to hold the node attributes
	unsigned int i; 				// variable to counter
	
	char *ctemp=NULL;	// temporary for parsing
	
	attributes = node->getAttributes();
	for (i = 0; i < attributes->getLength(); i++) {		
		if ( !strcmp(XMLString::transcode(attributes->item(i)->getNodeName()) , "color")) {
			ctemp = strtok(XMLString::transcode(attributes->item(i)->getNodeValue())," ");
			while (ctemp != NULL) {
				*this->color[0] = atof(ctemp);
				ctemp = strtok(NULL," ");
				*this->color[1] = atof(ctemp);
				ctemp = strtok(NULL," ");
				*this->color[2] = atof(ctemp);
				ctemp = strtok(NULL," ");
				#ifdef DEBUG
					cout << "Sky Color  : " << this->color[0] << " , " 
											 << this->color[1] << " , " 
											 << this->color[2] << endl;
				#endif

			}
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"fogType")) {
			strcpy(*this->fogType,XMLString::transcode(attributes->item(i)->getNodeValue()));
		} else 
		if (!strcmp(XMLString::transcode(attributes->item(i)->getNodeName()),"visibilityRange")) {
			*this->visibilityRange = atof(XMLString::transcode(attributes->item(i)->getNodeValue()));
		}
		
	}

	
	#ifdef DEBUG1
		cout << "\t\t\t\tLeaving Read (Fog)" << endl;	// Message about debuging
	#endif
	
	
	return(NULL);
	
}
