
#include "x3dCore.h"

map<const char*, void *, ltstr> X3DNode::nodesLink;

X3DNode::X3DNode() {
	strcpy(this->DEF,"");
}

void X3DNode::setLink(const char* DEF, void *pointer) { 
	
	if(nodesLink[DEF]!=NULL) {
		cout << "Warning DEF alread defined : " << DEF << endl;
	}		
	
	nodesLink[DEF] = pointer; 
	
}

void *X3DNode::getLink(const char* DEF) { 

	if(nodesLink[DEF]==NULL) {
		cout << " Error : There is a USE without a predefined DEF ( " 
			 << DEF << " ) " << endl;
	}
	
	return(nodesLink[DEF]); 
		
};
