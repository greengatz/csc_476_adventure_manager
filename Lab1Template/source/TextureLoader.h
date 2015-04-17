/**
 * TextureLoader.h
 * @author Brandon Clark
 */

#pragma once
#ifndef _TEXTURELOADER_H_
#include "GLSL.h"

/*data structure for the image used for  texture mapping */
typedef struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
} Image;



class TextureLoader
{
public:
	TextureLoader();
	virtual ~TextureLoader();
	Image *TextureImage;
	Image *imageData;
	GLvoid LoadTexture(char* image_file, int texID);
	
private:
	static unsigned int getint(FILE *fp);
	static unsigned int getshort(FILE *fp);
	int ImageLoad(char *filename, Image *image);
};

#endif