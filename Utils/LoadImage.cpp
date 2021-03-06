#include "LoadImage.h"

GLubyte* loadImage(const char* filename, unsigned int& width, unsigned int& height)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename, 0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, filename);

	imagen = FreeImage_ConvertTo32Bits(imagen);
	//FreeImage_FlipVertical(imagen);

	width = FreeImage_GetWidth(imagen);
	height = FreeImage_GetHeight(imagen);

	//std::cout << width << " | " << height << "\n";

	GLubyte* textura = new GLubyte[4 * width * height];
	char* pixels = (char*)FreeImage_GetBits(imagen);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for (int j = 0; j < width * height; j++) {
		textura[j * 4 + 0] = pixels[j * 4 + 2];
		textura[j * 4 + 1] = pixels[j * 4 + 1];
		textura[j * 4 + 2] = pixels[j * 4 + 0];
		textura[j * 4 + 3] = pixels[j * 4 + 3];
		//cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
	}

	FreeImage_Unload(imagen);
	return textura;

}
