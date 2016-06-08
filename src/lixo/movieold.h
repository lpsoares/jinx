//--- Este jbgjfvjn --- //

class Movie { 
	
	bool moreframes; // variavel que verifica se ainda existem frames a serem pesquisadas
	int counter; // contador de frames
	unsigned char *pixels; // ponteiro para alocar memoria para os pixels de cada frame
	unsigned char *rgb; //ponteiro para alocar os valores de rgb 
	
	ImageDesc	image; //inicializa a estrutura que contem as informacoes dos frames
	FILE		*filename; 			/*cria ponteiro para o arquivo a ser aberto*/

public:
		
	Movie(char *file); //constructor (abre o arquivo e conta qtos frames existem nele)
	~Movie(); // destructor fecha o arquivo e libera a memoria utilizada por "pixels"
	
	int get_Height(); //retorna a altura em pixels
	int get_Width(); //retorna a largura em pixels
	int get_Size(); //retorna o tamanho de memoria alocado em bytes para cada frame
	int get_number_of_Frames(); //retorna o numero de frames
	int get_required_Size(); //retorna o tamanho de memoria necessario para alocar o video todo em bytes
	void upload_Frames(); //le os valores de rgb de cada pixel do arquivo
	unsigned char *get_Frame(int frameNumber); // aaaaa

};
