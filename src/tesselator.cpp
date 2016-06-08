
#include "tesselator.h"

#include <iostream>


using namespace std;

// Includes do OpenGL
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef CALLBACK
	#define CALLBACK
#endif


Tesselator::Tesselator() {
	
	#ifdef DEBUG3
		cout << "GLU Version : " << gluGetString(GLU_VERSION) << endl;
	#endif
	
	indices = new GLuint*[NUM_INDICES];
	
	values = new float[NUM_INDICES*8];	// hold vertex, normal, texture, color
	type = new GLenum[NUM_INDICES];
	indicesSize = new unsigned int[NUM_INDICES]; // holt the number of values
	tmpIndices = new GLuint[NUM_INDICES];
	
	
	this->tess = NULL;
	this->tess = gluNewTess();
	if(this->tess==0) cerr << "Error not enough memory for tesselator" << endl;
	gluTessCallback(this->tess, (GLenum) GLU_TESS_BEGIN_DATA,(void (CALLBACK *) (void)) Tesselator::begin);
	gluTessCallback(this->tess, (GLenum) GLU_TESS_VERTEX_DATA,(void (CALLBACK *) (void)) Tesselator::vertex);
	gluTessCallback(this->tess, (GLenum) GLU_TESS_END_DATA,(void (CALLBACK *) (void)) Tesselator::end);
	gluTessCallback(this->tess, (GLenum) GLU_TESS_COMBINE_DATA,(void (CALLBACK *) (void)) Tesselator::combine);
	
	gluTessCallback(this->tess, (GLenum) GLU_TESS_ERROR,(void (CALLBACK *) (void)) Tesselator::error);

	// Does not uncoment this line, because it will decrease a lot the speed
	//gluTessCallback(this->tess, (GLenum) GLU_TESS_EDGE_FLAG_DATA,(void (CALLBACK *) Tesselator::edgeFlag);
	
	gluTessProperty(this->tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
	gluTessProperty(this->tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	gluTessProperty(this->tess, GLU_TESS_TOLERANCE, 0);

}


Tesselator::~Tesselator() {
	
	for(unsigned int f=0;f<this->counterIndices;f++) {
		delete[] indices[f];
	}
	
	delete[] indices;
	
	gluDeleteTess(tess); // deleting the tesselator
}

void Tesselator::begin(GLenum which, void * polygon_data) {
	
	Tesselator *t = (Tesselator *)polygon_data;
	
	t->type[t->counterIndices] = which;
	
	t->tmpCounterIndices = 0;
	
	#ifdef DEBUG4
		cout << "Tesselator Begin is : ";
		if(which==GL_POINTS) 			{ cout << "GL_POINTS" << endl; }
		if(which==GL_LINES) 			{ cout << "GL_LINES" << endl; }
		if(which==GL_LINE_STRIP) 		{ cout << "GL_LINE_STRIP" << endl; }
		if(which==GL_LINE_LOOP) 		{ cout << "GL_LINE_LOOP" << endl; }
		if(which==GL_TRIANGLES) 		{ cout << "GL_TRIANGLES" << endl; }
		if(which==GL_TRIANGLE_STRIP) 	{ cout << "GL_TRIANGLES_STRIP" << endl; }
		if(which==GL_TRIANGLE_FAN) 		{ cout << "GL_TRIANGLE_FAN" << endl; }
		if(which==GL_QUADS) 			{ cout << "GL_QUADS" << endl; }
		if(which==GL_QUAD_STRIP) 		{ cout << "GL_QUADS_STRIP" << endl; }
		if(which==GL_POLYGON) 			{ cout << "GL_POLYGON" << endl; }
	#endif
}

void Tesselator::end(void * polygon_data) {
	
	Tesselator *t = (Tesselator *)polygon_data;
		
	t->indices[t->counterIndices] = new GLuint[t->tmpCounterIndices];
		
	
	t->indicesSize[t->counterIndices] = t->tmpCounterIndices;
	
	for(unsigned int f=0;f<t->tmpCounterIndices;f++) {
		t->indices[t->counterIndices][f] = t->tmpIndices[f];
	}
	
	t->tmpCounterIndices = 0;
	t->counterIndices++;
	
	if(t->counterIndices>=NUM_INDICES) {
		cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
	}
		
	
	
}

void Tesselator::error(GLenum error_number) {
	cerr << "Tesselator error: ";
	cerr << gluErrorString(error_number) << endl;
}

//#ifdef JINX_DOUBLE
void Tesselator::combine(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut, void * polygon_data) {
//#else
//void Tesselator::combine(GLfloat coords[3], GLfloat *vertex_data[4], GLfloat weight[4], GLfloat **dataOut) {
//#endif
	
	unsigned short int i;	
	unsigned short int sizeVariables = 11;
	
	//#ifdef JINX_DOUBLE
		GLdouble *vertex;
		vertex = new GLdouble[sizeVariables];
	//#else
	//	GLfloat *vertex;
	//	vertex = new GLfloat[sizeVariables];
	//#endif
	
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	
	
	
	#ifdef DEBUG
	
	cout << "vertex 0 : " << vertex[0] << endl;
	cout << "vertex 1 : " << vertex[1] << endl;
	cout << "vertex 2 : " << vertex[2] << endl;
	
	cout << "weight 0 : " << weight[0] << endl;
	cout << "weight 1 : " << weight[1] << endl;
	cout << "weight 2 : " << weight[2] << endl;
	cout << "weight 3 : " << weight[3] << endl;
	
	for (i = 3; i < sizeVariables; i++) {
		cout << "Line ( " << i << " ) > ";
		cout << "Value 0 : " << vertex_data[0][i];
		cout << "Value 1 : " << vertex_data[1][i];
		if(weight[2]!=0.0f) cout << "Value 2 : " << vertex_data[2][i];
		if(weight[3]!=0.0f) cout << "Value 3 : " << vertex_data[3][i];
		cout << endl;
	}
	
	#endif
	
	
	for (i = 3; i < sizeVariables; i++) {
	
		//#ifdef JINX_DOUBLE		
			
			double temp[4]={0.0f,0.0f,0.0f,0.0f};
			
			temp[0] = ( ((double)weight[0]) * vertex_data[0][i] );
			temp[1] = ( ((double)weight[1]) * vertex_data[1][i] );
			if(weight[2]!=0.0f) temp[2] = ( ((double)weight[2]) * vertex_data[2][i] );  // if not needed the value it doesn't exist and crash if accessed
			if(weight[3]!=0.0f) temp[3] = ( ((double)weight[3]) * vertex_data[3][i] );  // if not needed the value it doesn't exist and crash if accessed
			
			/*
			Esta foi sim, e varias vezes
			if(weight[2]!=0.0f) cout << "FOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl << endl;
			if(weight[3]!=0.0f) cout << "FOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl << endl;
			*/
		
		//#else 
		//	float temp[4]={0,0,0,0};
		//	if(((int)weight[0])) temp[0] = ( ((float)weight[0]) * vertex_data[0][i] );
		//	if(((int)weight[1])) temp[1] = ( ((float)weight[1]) * vertex_data[1][i] );
		//	if(((int)weight[2])) temp[2] = ( ((float)weight[2]) * vertex_data[2][i] );
		//	if(((int)weight[3])) temp[3] = ( ((float)weight[3]) * vertex_data[3][i] );
		//#endif
			
		#ifdef DEBUG
		cout << "Combine " << i << " : " ;
		cout << temp[0] << "  " ;
		cout << temp[1] << "  " ;
		cout << temp[2] << "  " ;
		cout << temp[3] << "  " ;
		cout << endl ;
		#endif
		
		vertex[i] = temp[0] + temp[1] + temp[2] + temp[3];

	}
	
	*dataOut = vertex;
}
	
void Tesselator::vertex(void *id, void *polygon_data) {

		unsigned int f;
	
		GLdouble *e2 = (GLdouble *)id;
		Tesselator *t = (Tesselator *)polygon_data;

		float temp[11];
		
		for(f=0;f<11;f++) { 
			temp[f] = ((float)(*(e2+f)));
		}
	
		for(f=0;f<t->counter;f++) {
			
			
			
			if(t->format==GL_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_N3F_V3F" << endl;
				#endif
				
				if( 
			
					t->values[(6*(f))+0] == temp[5] &&
					t->values[(6*(f))+1] == temp[6] &&
					t->values[(6*(f))+2] == temp[7] &&
			
					t->values[(6*(f))+3] == temp[0] &&
					t->values[(6*(f))+4] == temp[1] &&
					t->values[(6*(f))+5] == temp[2]

						) {

					t->tmpIndices[t->tmpCounterIndices] = f;
					t->tmpCounterIndices++;
		
				
					if(t->tmpCounterIndices>=NUM_INDICES) {
						cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
					}
		
					return;

				}
				
				
				

			} else 
			
			if(t->format==GL_C4F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_C4F_N3F_V3F" << endl;
				#endif
				
				if( 
					
					t->values[(10*(f))+0]  == temp[8] &&
					t->values[(10*(f))+1]  == temp[9] &&
					t->values[(10*(f))+2]  == temp[10] &&
					t->values[(10*(f))+3]  == 1.0f &&
				
					t->values[(10*(f))+4]  == temp[5] &&
					t->values[(10*(f))+5]  == temp[6] &&
					t->values[(10*(f))+6]  == temp[7] &&
			
					t->values[(10*(f))+7]  == temp[0] &&
					t->values[(10*(f))+8] == temp[1] &&
					t->values[(10*(f))+9] == temp[2]
				
						) {

					t->tmpIndices[t->tmpCounterIndices] = f;
					t->tmpCounterIndices++;
		
				
					if(t->tmpCounterIndices>=NUM_INDICES) {
						cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
					}
		
					return;
				}
				
			} else 
			
			if(t->format==GL_T2F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_T2F_N3F_V3F" << endl;
				#endif
				
				if( 
			
					t->values[(8*(f))+0] == temp[3] &&
					t->values[(8*(f))+1] == temp[4] &&
			
					t->values[(8*(f))+2] == temp[5] &&
					t->values[(8*(f))+3] == temp[6] &&
					t->values[(8*(f))+4] == temp[7] &&
			
					t->values[(8*(f))+5] == temp[0] &&
					t->values[(8*(f))+6] == temp[1] &&
					t->values[(8*(f))+7] == temp[2]

						) {

					t->tmpIndices[t->tmpCounterIndices] = f;
					t->tmpCounterIndices++;
		
				
					if(t->tmpCounterIndices>=NUM_INDICES) {
						cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
					}
		
					return;

				}
			} else 
			if(t->format==GL_T2F_C4F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_T2F_C4F_N3F_V3F" << endl;
				#endif
				
				if( 
			
					t->values[(12*(f))+0]  == temp[3] &&
					t->values[(12*(f))+1]  == temp[4] &&
				
					t->values[(12*(f))+2]  == temp[8] &&
					t->values[(12*(f))+3]  == temp[9] &&
					t->values[(12*(f))+4]  == temp[10] &&
					t->values[(12*(f))+5]  == 1.0f &&
				
					t->values[(12*(f))+6]  == temp[5] &&
					t->values[(12*(f))+7]  == temp[6] &&
					t->values[(12*(f))+8]  == temp[7] &&
			
					t->values[(12*(f))+9]  == temp[0] &&
					t->values[(12*(f))+10] == temp[1] &&
					t->values[(12*(f))+11] == temp[2]

						) {

					t->tmpIndices[t->tmpCounterIndices] = f;
					t->tmpCounterIndices++;
		
				
					if(t->tmpCounterIndices>=NUM_INDICES) {
						cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
					}
		
					return;

				}
			}
			
			
		}
	
		if( e2!=NULL ) {
			
			
			if(t->format==GL_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_N3F_V3F" << endl;
				#endif
				
				t->values[(6*(t->counter))+0] = temp[5];
				t->values[(6*(t->counter))+1] = temp[6];
				t->values[(6*(t->counter))+2] = temp[7];
			
				t->values[(6*(t->counter))+3] = temp[0];
				t->values[(6*(t->counter))+4] = temp[1];
				t->values[(6*(t->counter))+5] = temp[2];
				
			
			} else 
				
			if(t->format==GL_C4F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_C4F_N3F_V3F" << endl;
				#endif
					
				t->values[(10*(t->counter))+0] = temp[8];
				t->values[(10*(t->counter))+1] = temp[9];
				t->values[(10*(t->counter))+2] = temp[10];
				t->values[(10*(t->counter))+3] = 1.0f;
				
				t->values[(10*(t->counter))+4] = temp[5];
				t->values[(10*(t->counter))+5] = temp[6];
				t->values[(10*(t->counter))+6] = temp[7];
			
				t->values[(10*(t->counter))+7] = temp[0];
				t->values[(10*(t->counter))+8] = temp[1];
				t->values[(10*(t->counter))+9] = temp[2];
				
				
			} else 
			
			
			if(t->format==GL_T2F_C4F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_T2F_C4F_N3F_V3F" << endl;
				#endif
				
				t->values[(12*(t->counter))+0]  = temp[3];
				t->values[(12*(t->counter))+1]  = temp[4];
				
				t->values[(12*(t->counter))+2]  = temp[8];
				t->values[(12*(t->counter))+3]  = temp[9];
				t->values[(12*(t->counter))+4]  = temp[10];
				t->values[(12*(t->counter))+5]  = 1.0f;
				
				#ifdef DEBUG3
					cout << "Cor : " << t->values[(12*(t->counter))+2] << " , " <<
										 t->values[(12*(t->counter))+3] << " , " <<
										 t->values[(12*(t->counter))+4] << endl;
				#endif
				
				t->values[(12*(t->counter))+6]  = temp[5];
				t->values[(12*(t->counter))+7]  = temp[6];
				t->values[(12*(t->counter))+8]  = temp[7];
			
				t->values[(12*(t->counter))+9]  = temp[0];
				t->values[(12*(t->counter))+10] = temp[1];
				t->values[(12*(t->counter))+11] = temp[2];
				
				
			} else 
			
			
			if(t->format==GL_T2F_N3F_V3F) {
				#ifdef DEBUG3
					cout << "Format : GL_T2F_N3F_V3F" << endl;
				#endif
				t->values[(8*(t->counter))+0] = temp[3];
				t->values[(8*(t->counter))+1] = temp[4];
			
				t->values[(8*(t->counter))+2] = temp[5];
				t->values[(8*(t->counter))+3] = temp[6];
				t->values[(8*(t->counter))+4] = temp[7];
			
				t->values[(8*(t->counter))+5] = temp[0];
				t->values[(8*(t->counter))+6] = temp[1];
				t->values[(8*(t->counter))+7] = temp[2];

			} 
			
			
			t->tmpIndices[t->tmpCounterIndices] = t->counter;
		
			t->tmpCounterIndices++;
		
			t->counter++;
	
		}

		if(t->tmpCounterIndices>=NUM_INDICES) {
			cout << "Error maximum number of indexes : " << t->tmpCounterIndices << endl;
		}
		
		
		if(t->counter>=(NUM_INDICES)) {
			cout << "Error maximum number of vertices : " << t->counter << endl;
		}
		
	#ifdef DEBUG
	cout << "vertex tesselator" << endl;
	cout << "Vertex : ";
	cout << *(e2+0)  << "  " ;
	cout << *(e2+1)  << "  " ;
	cout << *(e2+2)  << "  " ;
	cout << "Texture : ";
	cout << *(e2+3)  << "  " ;
	cout << *(e2+4)  << "  " ;
	cout << "Normal : ";
	cout << *(e2+5)  << "  " ;
	cout << *(e2+6)  << "  " ;
	cout << *(e2+7)  << "  " ;
	cout << "Color : ";
	cout << *(e2+8)  << "  " ;
	cout << *(e2+9)  << "  " ;
	cout << *(e2+10) << endl ;
	#endif
	
}

void Tesselator::start() {

	for(unsigned int f=0;f<this->counterIndices;f++) {
		delete[] indices[f];
	}
		
	this->counter=0;
	this->counterIndices=0;
			
}

void Tesselator::draw() {

	
		
}
