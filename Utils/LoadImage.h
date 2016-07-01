#pragma once

#include <FreeImage.h>
#include <GL\glew.h>
#include <iostream>

GLubyte* loadImage(const char* filename, unsigned int& width, unsigned int& height);
