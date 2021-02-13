#pragma once
#include "Texture.h"
#include "Scene.h"
#include "Rasterizer.h"

#include <GL\glew.h>
#include <cassert>
#include <chrono>

// Uncomment this define to render esactly 600 frames and display an average fps
//#define _BENCHMARK

class App
{
public:
	App(int argc, char* argv[]);

	void InitGL();
	void Display();
	void ReshapeWindow(GLsizei w, GLsizei h);

	void Run();
	void FPSCount();
	static void OnDisplay();
	static void OnReshape(GLsizei w, GLsizei h);
	void KeyDown(unsigned char key, int x, int y);
	static void OnKeyDown(unsigned char key, int x, int y);
	static void OnSpecialDown(int key, int x, int y);
	void SpecialDown(int key, int x, int y);
	static void OnSpecialUp(int key, int x, int y);
	void SpecialUp(int key, int x, int y);

	void InitScenes();

	void PrintInstructions();

private:

	static App* ms_instance;
	//unsigned int s_width = 1000;
	//unsigned int s_height = 1000;
	unsigned int s_width = 1024;
	unsigned int s_height = 768;
	GLuint	s_texture;
	Texture s_pixels;
	std::map<std::string, Scene*> s_scenes;
	std::string sceneIndex = "NULL";
	Rasterizer s_rasterizer;

	int index = 0;
#ifdef _BENCHMARK
	float frames[600];
#else
	float frames[50];
#endif

	std::chrono::high_resolution_clock::time_point lastFrameTime;
	float deltaTime = 0.0f;

	static Scene ms_CUBE;
	static Scene ms_CUBE3;
	static Scene ms_BUNNY;
	static Scene ms_TYRA;
	static Scene ms_ARMADILLO;
	static Scene ms_DRAGON;
	static Scene ms_BUDDHA;
	static Scene ms_PADORU;
};
