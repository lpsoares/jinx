# include <iostream>
# include <mpeg.h>
# include "movie.h"

using namespace std;


Movie::Movie(char *file) /* a funcao construtora*/
{
	moreframes = TRUE;
	counter = 0;
	filename    =   fopen (file, "r"); 	/*abre o arquivo*/	
	
	SetMPEGOption (MPEG_DITHER,FULL_COLOR_DITHER); /*controla a maneira que o stream sera tratado pela biblioteca / full_color_dither => decodifica 24 bits por pixel*/
	OpenMPEG (filename, &image); /*prepara o stream do mpeg para ser lido e determina os campos da estrutara ImageDesc*/
	
	pixels = new unsigned char [image.Size]; /*aloca espaco na memoria para cada frame*/
	
	while (moreframes) /*varre todo o arquivo e conta qtos frames existem*/
	{
		moreframes = GetMPEGFrame ((char *) pixels);
		counter++;
	}


	RewindMPEG(filename,&image); /*volta a stream para seu comeco para que os pixels possam ser obtidos*/
	
}


Movie::~Movie()/*destrutora: fecha o arquivo e libera o espaco na memoria alocado para pixel*/
{
	RewindMPEG(filename,&image);
	CloseMPEG ();
	fclose (filename);
	delete [] pixels;
}

int Movie::get_Height()
{
	return image.Height;
}

int Movie::get_Width()
{
	return image.Width;
}

int Movie::get_Size()
{
	return image.Size;
}

int Movie::get_number_of_Frames()
{
	return counter;
}

int Movie::get_required_Size() /*como cada pixel possui 3 bytes (rgb) e' necessario multiplicar pelo fator 3 */
{
	return 3*(image.Height*image.Width)*counter;
}

void Movie::upload_Frames() /*varre toda a stream novamente e retorna os valores de rgb de cada pixel*/
{
	int Size = get_required_Size();

	int i,j = 0;
	
	moreframes = TRUE;
	
	rgb = new unsigned char [Size]; /*aloca espaco para Size unsigned chars*/
	
	while (moreframes) {
		
		moreframes = GetMPEGFrame ((char *) pixels);
		
		for(i=0;i<image.Size;i+=4)
		{
//			cout << j << "\n" << j+1 << "\n" << j+2 << "\n";
			//cout << (int) pixels[i] << " " << (int) pixels[i+1] << " " << (int) pixels[i+2] << " \n";
			rgb[j]=pixels[i];
			rgb[j+1]=pixels[i+1];
			rgb[j+2]=pixels[i+2];
			#ifdef DEBUG
				cout << (int) rgb[j] << " " << (int) rgb[j+1] << " " << (int) rgb[j+2] << " \n";
			#endif
			j+=3;
		}
		
	}
	
}


unsigned char *Movie::get_Frame(int frameNumber) /*varre toda a stream novamente e retorna os valores de rgb de cada pixel*/
{

	unsigned char *frame;

	frame = rgb + ( frameNumber * 3 * image.Height * image.Width );

	return(frame);

}

