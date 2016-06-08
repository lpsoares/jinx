#include <iostream>
#include <mpeg.h>
#include "movie.h"

//# define DEBUG
using namespace std;

int main (int argc, char *argv[])

{
	
	Movie	*object = new Movie (argv[1]); /*cria o objeto da classe movie e inicializa o arquivo mpeg*/
		
#ifdef DEBUG 	
	cout << "\n";
	cout <<"Altura : " << object->get_Height() << " pixels\n";
	cout <<"Largura: " << object->get_Width()  << " pixels\n";
	cout <<"Frames : " << object->get_number_of_Frames()  << "\n";
	cout <<"Mem. necessaria p/ frame: " << object->get_Size() << " bytes\n";
	cout <<"Mem. necessaria total   : " << object->get_required_Size()  << " bytes\n";
	cout << "\n";
#endif	
	cout << "P3\n" << object->get_Width() << " " << object->get_number_of_Frames()*object->get_Height() << "\n255\n"; /*cabecalho do arquivo ppm*/

	object->upload_Frames() /*carrega os frames*/;
		
	unsigned char *image;

	image = object->get_Frame(28);

	for(int f=0; f< object->get_Height()*object->get_Width()*3 ; f++) {

		cout << (int)image[f] << " " ;

	}

	cout << endl;

	return 0;
	
}
