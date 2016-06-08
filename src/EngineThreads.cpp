// Engine is the core class

// Jinx Includes
#include "engine.h"
#include "sound/sound.h"
#include "input.h"
#include "communication.h"
#include "joystick.h"
#include "wand/wand.h"

// PThread Includes
#include <pthread.h>

void *Engine::createxml(void *) {
	my_Engine->lll = new xml(my_Engine->configurationFileName); 
	if (my_Engine->lll == NULL) {
		cerr << "Error allocating space for xml parser" << endl;
    }
	return NULL;
}

void *Engine::createx3d(void *) {
	my_Engine->xxx = new x3d();
	if (my_Engine->xxx == NULL) {
		cerr << "Error allocating space for x3d" << endl;
    }
	my_Engine->xxx->load(my_Engine->x3dfile);
	return NULL;
}


void *Engine::tgraph(void *) {
	my_Engine->rrr = new Render(my_Engine->lll,my_Engine->xxx,0); // arrancar este zero
	if(my_Engine->func != NULL)	{
		my_Engine->rrr->OpenGL(my_Engine->func);
	}
	my_Engine->rrr->graphInit(my_Engine->argc, my_Engine->argv);	// Glut looks like not thread safe then it should be here
	my_Engine->rrr->loop();
	delete my_Engine->rrr;
	return NULL;
}

void *Engine::tsound(void *) {
	
	#ifdef DEBUG
		cout << "Creating Sound" << endl;
	#endif
	Sound* sound = new Sound(my_Engine->lll);
	
	
	#ifdef DEBUG
		cout << "Serializig Sound" << endl;
	#endif
	my_Engine->xxx->serializeSound(sound);
	
	
	#ifdef DEBUG
		cout << "Playing Sound" << endl;
	#endif	
	sound->playMusic();
	
	delete sound;
	return NULL;
}


void *Engine::tinput(void *) {
	//Render::graphCommunicatorGlobal();
	Input* input = new Input(my_Engine->xxx);
	input->movescene();
	delete input;   // repensar
	return NULL;
}


void *Engine::tjoyst(void *) {
	//Render::graphCommunicatorGlobal();
	
	
	Joystick* joystick = new Joystick(my_Engine->lll);
	joystick->JoyRead();
	delete joystick;
	return NULL;
}

void *Engine::twand(void *) {
	//Render::graphCommunicatorGlobal();
	Input* input = new Input(my_Engine->xxx);
	input->movewand();
	delete input;
	return NULL;
}
	
void *Engine::ttrack(void *) {
	//Render::graphCommunicatorGlobal();
	Wand* wand = new Wand();
	wand->readWand(my_Engine->lll->conf->getWandGlobal());
	delete wand;
	return NULL;
}

void *Engine::tmain(void *) {
	//Render::graphCommunicatorGlobal();
	my_Engine->manager();
	return NULL;
}
