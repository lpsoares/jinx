/* Need to do */

#include "render.h"
#include "../x3d/x3d.h"


void Render::menu() {
	
	int shadingMenu;											// Case in the main menu was asked option Shading
	shadingMenu = glutCreateMenu(Render::HandleShadingMenu);
	glutAddMenuEntry("Wireframe",7);
	glutAddMenuEntry("Solid",8);	
	
	int cullingMenu;											// Case in the main menu was asked option Culling
	cullingMenu = glutCreateMenu(Render::HandleCullingMenu);
	glutAddMenuEntry("On",1);
	glutAddMenuEntry("Off",0);	

	int viewpointMenu;											// Case in the main menu was asked option Viewpoint
	viewpointMenu = glutCreateMenu(Render::HandleViewpointMenu);
	MPI_Status vstatus;
	int countViewpoint=0;
	Viewpoints viewpoints[256];
	MPI_Recv(&countViewpoint, 1, MPI_INT, INPUT, OUTPUT_NVIEWPOINT, MPI_COMM_WORLD, &vstatus);
		
	for(int h=0;h<countViewpoint;h++) {
		MPI_Recv(&viewpoints[h].description, 256, MPI_CHAR, INPUT, OUTPUT_VIEWPOINT, MPI_COMM_WORLD, &vstatus);
		glutAddMenuEntry(viewpoints[h].description,h);
	}

	int groupMenu;											// Case in the main menu was asked option Culling
	groupMenu = this->xxx->group();
	
	//int mainMenu;												// Main menu that appears with the right click
	//mainMenu = glutCreateMenu(Render::HandleMainMenu);
	glutCreateMenu(Render::HandleMainMenu);
	glutAddMenuEntry("Grid",1);
	glutAddMenuEntry("Camera",2);
	glutAddMenuEntry("Calibration",3);
	glutAddSubMenu("Shading",shadingMenu);
	glutAddSubMenu("Culling",cullingMenu);
	glutAddSubMenu("Viewpoint",viewpointMenu);
	glutAddSubMenu("Group",groupMenu);
	glutAddMenuEntry("Quit",9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
		
}



/* Main menu handle function */
void Render::HandleMainMenu(int op) {

    switch (op) {
    case 1:
		(my_rrr->grid?my_rrr->grid=false:my_rrr->grid=true);
		break;
	case 2:
		(my_rrr->camera?my_rrr->camera=false:my_rrr->camera=true);
		break;
	case 3:
		(my_rrr->calibration?my_rrr->calibration=false:my_rrr->calibration=true);
		break;
	case 9:
		cout << "Exiting by ESC key" << endl;
		glutDestroyWindow(my_rrr->window); 		// shut down our window
		MPI_Abort(MPI_COMM_WORLD,0);
		exit(0);
		break;
	
	default:
		break;
	
	}
	
}


void Render::HandleViewpointMenu(int op) {

		Input::setViewpoint(op);
}



void Render::HandleCullingMenu(int op) {

    switch (op) {
 	case 0:
		glDisable(GL_CULL_FACE);
		break;
	case 1:
		glEnable(GL_CULL_FACE);
		break;
	
	default:
		break;
	
	}
}

void Render::HandleShadingMenu(int op) {

    switch (op) {
 	case 7:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;
	case 8:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	
	default:
		break;
	
	}
}


void x3d::HandleGroupMenu(int op) {

	int p;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	if(op) { } // for the future implementation
	
	int menu = glutGetMenu();
	
	for(int destin = 0; destin < p; destin++) {
		if(destin>=GRAPH) {	
			MPI_Send(&menu, 1, MPI_INT, destin, GROUP_ONOFF, MPI_COMM_WORLD);
		}
	}

}
