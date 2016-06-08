

#include "avatars.h"

Avatars::Avatars() {
	
}

// Draws the cubic mouse that looks like the Flock of Birds 3dmouse
void Avatars::wandgl() {
	
	float witheColor[4]={0.8,0.8,0.8,1.0f};
	float redColor[4]={0.8f,0.0f,0.0f,1.0f};
	float greenColor[4]={0.0f,0.8f,0.0f,1.0f};
	float blueColor[4]={0.0f,0.0f,0.8f,1.0f};
	float blackColor[4]={0.0f,0.0f,0.0f,1.0f};
	
	bool texturized = false;		// if the user intends to use some texture effect over the mouse body
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, witheColor);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.1f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColor);

	
	// Errado mais certo agora
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.6f,0.6f,0.6f,1.0f);
	glDisable(GL_COLOR_MATERIAL);
	
	
	glPushMatrix();
	
	glScalef(0.8,0.3,1);
	

	if(texturized) {

		glBegin(GL_QUADS);
	
		glNormal3d( 0.0, 0.0, 1.0);
		glTexCoord2d(1.0,0.0);	glVertex3d( 0.5,-0.5, 0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d( 0.5, 0.5, 0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d(-0.5, 0.5, 0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d(-0.5,-0.5, 0.5);
	
		glNormal3d( 0.0, 0.0,-1.0);
		glTexCoord2d(1.0,0.0);	glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d(-0.5, 0.5,-0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d( 0.5, 0.5,-0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d( 0.5,-0.5,-0.5);
	
		glNormal3d( 1.0, 0.0, 0.0);
		glTexCoord2d(1.0,0.0);	glVertex3d( 0.5,-0.5,-0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d( 0.5, 0.5,-0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d( 0.5, 0.5, 0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d( 0.5,-0.5, 0.5);
	
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2d(1.0,0.0);	glVertex3d(-0.5,-0.5, 0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d(-0.5, 0.5, 0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d(-0.5, 0.5,-0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d(-0.5,-0.5,-0.5);
	
		glNormal3d( 0.0, 1.0, 0.0);
		glTexCoord2d(1.0,0.0);	glVertex3d( 0.5, 0.5, 0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d( 0.5, 0.5,-0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d(-0.5, 0.5,-0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d(-0.5, 0.5, 0.5);
	
		glNormal3d( 0.0,-1.0, 0.0);
		glTexCoord2d(1.0,0.0);	glVertex3d( 0.5,-0.5,-0.5);
		glTexCoord2d(1.0,1.0);	glVertex3d( 0.5,-0.5, 0.5);
		glTexCoord2d(0.0,1.0);	glVertex3d(-0.5,-0.5, 0.5);
		glTexCoord2d(0.0,0.0);	glVertex3d(-0.5,-0.5,-0.5);
	
		glEnd();
	
	} else {
		
			glBegin(GL_QUADS);
	
		glNormal3d( 0.0, 0.0, 1.0);
		glVertex3d( 0.5,-0.5, 0.5);		glVertex3d( 0.5, 0.5, 0.5);		glVertex3d(-0.5, 0.5, 0.5);		glVertex3d(-0.5,-0.5, 0.5);
	
		glNormal3d( 0.0, 0.0,-1.0);
		glVertex3d(-0.5,-0.5,-0.5);		glVertex3d(-0.5, 0.5,-0.5);		glVertex3d( 0.5, 0.5,-0.5);		glVertex3d( 0.5,-0.5,-0.5);
	
		glNormal3d( 1.0, 0.0, 0.0);
		glVertex3d( 0.5,-0.5,-0.5);		glVertex3d( 0.5, 0.5,-0.5);		glVertex3d( 0.5, 0.5, 0.5);		glVertex3d( 0.5,-0.5, 0.5);
	
		glNormal3d(-1.0, 0.0, 0.0);
		glVertex3d(-0.5,-0.5, 0.5);		glVertex3d(-0.5, 0.5, 0.5);		glVertex3d(-0.5, 0.5,-0.5);		glVertex3d(-0.5,-0.5,-0.5);
	
		glNormal3d( 0.0, 1.0, 0.0);
		glVertex3d( 0.5, 0.5, 0.5);		glVertex3d( 0.5, 0.5,-0.5);		glVertex3d(-0.5, 0.5,-0.5);		glVertex3d(-0.5, 0.5, 0.5);
	
		glNormal3d( 0.0,-1.0, 0.0);
		glVertex3d( 0.5,-0.5,-0.5);		glVertex3d( 0.5,-0.5, 0.5);		glVertex3d(-0.5,-0.5, 0.5);		glVertex3d(-0.5,-0.5,-0.5);
		
		glEnd();
	}
	
	glTranslatef(0.0f,0.5f,0.2f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColor);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.6f,0.0f,0.0f,1.0f);
	glDisable(GL_COLOR_MATERIAL);
	glutSolidCube(0.25);
	
	
	glTranslatef(-0.3f,0.0f,0.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, greenColor);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.0f,0.6f,0.0f,1.0f);
	glDisable(GL_COLOR_MATERIAL);
	glutSolidCube(0.25);
	
	
	glTranslatef(0.6f,0.0f,0.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blueColor);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.0f,0.0f,0.6f,1.0f);
	glDisable(GL_COLOR_MATERIAL);
	glutSolidCube(0.25);
	
	glPopMatrix();
		
}


void Avatars::glove(float val[20]) {

	glPushMatrix();
		
	glColor3f(0.8f,0.8f,0.8f);

	glRotatef(-180,0,1,0);
	glRotatef(-90,1,0,0);

	glScalef(0.2,0.2,0.2);
	
	// prono supinação	
				
	glScalef(1,0.4,1);
	glutSolidSphere(0.2,8,8);		// MAO
	glScalef(1,1,1);
	
	
	glPushMatrix();
	glTranslatef(-0.25,0.0,0.0);
	glRotatef(90*(val[2]-0.5),0,1,0);
	glRotatef(-120*val[0],1,0,0);
	glutSolidCone(0.05,0.18,8,8);
	glTranslatef( 0.0,0.0,0.18);		// DEDAO
	glRotatef(-120*val[1],1,0,0);
	glutSolidCone(0.05,0.1,8,8);
	glPopMatrix();
	glTranslatef(0.0,0.0,0.2);
	
	glPushMatrix();
	glTranslatef(-0.1,0.0,0.0);
	glRotatef(-60*(1-val[5]),0,1,0);
	glRotatef(-120*val[3],1,0,0);
	glutSolidCone(0.05,0.27,8,8);
	glTranslatef( 0.0,0.0,0.27);
	glRotatef(-120*val[4],1,0,0);
	glutSolidCone(0.05,0.17,8,8);	// INDICADOR
	glTranslatef( 0.0,0.0,0.17);
	glutSolidCone(0.05,0.1,8,8);
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef( 0.0,0.0,0.0);
	//glRotatef(90*(val[8]-0.5),0,1,0);
	glRotatef(-120*val[6],1,0,0);
	glutSolidCone(0.05,0.28,8,8);
	glTranslatef( 0.0,0.0,0.28);
	glRotatef(-120*val[7],1,0,0);
	glutSolidCone(0.05,0.19,8,8);	// MEDIO
	glTranslatef( 0.0,0.0,0.19);
	glutSolidCone(0.05,0.1,8,8);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0.1,0.0,0.0);
	glRotatef(60*(1-val[8]),0,1,0);
	glRotatef(-120*val[9],1,0,0);
	glutSolidCone(0.05,0.27,8,8);
	glTranslatef( 0.0,0.0,0.27);
	glRotatef(-120*val[10],1,0,0);
	glutSolidCone(0.05,0.17,8,8);	// ANELAR
	glTranslatef( 0.0,0.0,0.17);
	glutSolidCone(0.05,0.1,8,8);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 0.21,0.0,-0.05);
	glRotatef(60*(1-val[11]),0,1,0);
	glRotatef(-120*val[12],1,0,0);
	glutSolidCone(0.04,0.25,8,8);
	glTranslatef( 0.0,0.0,0.25);
	glRotatef(-120*val[13],1,0,0);
	glutSolidCone(0.05,0.14,8,8);	// MINDINHO
	glTranslatef( 0.0,0.0,0.14);
	glutSolidCone(0.05,0.08,8,8);
	glPopMatrix();

	glPopMatrix();
	
}
