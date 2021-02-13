//// HelloGLUT.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
#include "pch.h"
//#include <iostream>
//#include <vector>
//
//
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//
//struct Pixel
//{
//	Pixel() = default;
//
//	Pixel(unsigned char r, unsigned char g, unsigned b, unsigned char a = 255) :
//		m_red(r), m_green(g), m_blue(b), m_alpha(a)
//	{}
//
//	unsigned char m_red = 0;
//	unsigned char m_green = 0;
//	unsigned char m_blue = 0;
//	unsigned char m_alpha = 255;
//};
//
//
//static GLuint	s_texture;
//
//static std::vector<Pixel>	s_imagePixels;
//
//static unsigned int s_width = 500;
//static unsigned int s_height = 500;
//
//
//
//void InitGL(void) {
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//
//	// create a buffer of black RGBA pixels.
//	s_imagePixels.resize(s_width * s_height);
//
//	// Enable textures
//	glEnable(GL_TEXTURE_2D);
//
//	// fill a texture with the pixel buffer
//	glGenTextures(1, &s_texture);
//
//	glBindTexture(GL_TEXTURE_2D, s_texture);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, 4, s_width, s_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_imagePixels.data());
//
//	// Set up the texture
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//
//}
//
//
//void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glBegin(GL_QUADS);
//
//	glTexCoord2f(0.f, 1.f);
//	glVertex2f(-1.f, 1.f);
//
//	glTexCoord2f(1.f, 1.f);
//	glVertex2f(1.f, 1.f);
//
//	glTexCoord2f(1.f, 0.f);
//	glVertex2f(1.f, -1.f);
//
//	glTexCoord2f(0.f, 0.f);
//	glVertex2f(-1.f, -1.f);
//
//	glEnd();
//
//	glutSwapBuffers();
//
//	glutPostRedisplay();
//}
//
//
//void reshape_window(GLsizei w, GLsizei h)
//{
//	// Prevent a divide by zero
//	if (h == 0)
//		h = 1;
//
//	// Set Viewport to window dimensions
//	s_width = w;
//	s_height = h;
//
//	glViewport(0, 0, s_width, s_height);
//
//	// Reset coordinate system
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	// Resize the texture
//	if (s_texture > 0)
//	{
//		// create a buffer of black RGBA pixels.
//		s_imagePixels.resize(s_width * s_height);
//		for (auto& pixel : s_imagePixels)
//		{
//			pixel = Pixel(255, 0, 0);
//		}
//
//		glBindTexture(GL_TEXTURE_2D, s_texture);
//
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, s_width, s_height, GL_RGBA, GL_UNSIGNED_BYTE, s_imagePixels.data());
//	}
//
//}
//
//
//int main(int argc, char **argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowSize(s_width, s_height);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("rasterize me");
//
//	glViewport(0, 0, s_width, s_height);        // Reset The Current Viewport
//
//	InitGL();
//
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape_window);
//
//	glutMainLoop();
//}
