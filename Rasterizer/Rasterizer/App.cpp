#include "pch.h"
#include "App.h"

#include <GL\freeglut_std.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Parser.h"

App* App::ms_instance = nullptr;
Scene App::ms_CUBE;
Scene App::ms_CUBE3;
Scene App::ms_BUNNY;
Scene App::ms_TYRA;
Scene App::ms_ARMADILLO;
Scene App::ms_DRAGON;
Scene App::ms_BUDDHA;
Scene App::ms_PADORU;

App::App(int argc, char* argv[]) :
	s_pixels(s_width, s_height)
{
	assert(ms_instance == nullptr);
	ms_instance = this;
	lastFrameTime = std::chrono::high_resolution_clock::now();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(s_width, s_height);
	glutInitWindowPosition(460, 20);
	glutCreateWindow("Rasterize us");

	glViewport(0, 0, s_width, s_height);	// Reset The Current Viewport

	InitScenes();

	PrintInstructions();

	InitGL();

	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyDown);
	glutSpecialFunc(OnSpecialDown);
	glutSpecialUpFunc(OnSpecialUp);
}

void App::InitGL()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// create a buffer of black RGBA pixels.

	s_pixels.ClearTexture();

	// Enable textures
	glEnable(GL_TEXTURE_2D);

	// fill a texture with the pixel buffer
	glGenTextures(1, &s_texture);

	glBindTexture(GL_TEXTURE_2D, s_texture);

	s_rasterizer.RenderScene(s_scenes[sceneIndex], &s_pixels);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, s_width, s_height, 0, GL_RGBA, GL_FLOAT, s_pixels.GetPixels());

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void App::Display()
{
	FPSCount();

	s_pixels.ClearTexture();
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	s_rasterizer.RenderScene(s_scenes[sceneIndex], &s_pixels);

	glBindTexture(GL_TEXTURE_2D, s_texture);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, s_width, s_height, GL_RGBA, GL_FLOAT, s_pixels.GetPixels());

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);

	glEnd();

	glutSwapBuffers();

	glutPostRedisplay();
}

void App::OnDisplay()
{
	ms_instance->Display();
}

void App::ReshapeWindow(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	s_width = w;
	s_height = h;

	glViewport(0, 0, s_width, s_height);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Resize the texture
	if (s_texture > 0)
	{
		s_rasterizer.changeSize = true;
		// create a buffer of black RGBA pixels.
		s_pixels.ResizeTexture(s_width, s_height);
		s_rasterizer.RenderScene(s_scenes[sceneIndex], &s_pixels);

		glBindTexture(GL_TEXTURE_2D, s_texture);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, s_width, s_height, GL_RGBA, GL_FLOAT, s_pixels.GetPixels());
	}
}

void App::OnReshape(GLsizei w, GLsizei h)
{
	ms_instance->ReshapeWindow(w, h);
}

void App::Run()
{
	glutMainLoop();
}

void App::FPSCount()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaDuration = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastFrameTime);
	lastFrameTime = now;

	deltaTime = deltaDuration.count();

	float fps = 1.f / deltaTime;

#ifdef _BENCHMARK

	if (index >= 600)
	{
		float sum = 0;
		float min = frames[0];
		float max = frames[0];

		for (int i = 0; i < 600; i++)
		{
			sum += frames[i];
		}

		sum /= 600;
		std::cout << "Average FPS (Over 600 frames): " << std::setprecision(4) << sum << "\n";

		exit(0);
	}

#else

	if (index >= 50)
	{
		float sum = 0;

		for (int i = 0; i < 50; i++)
		{
			sum += frames[i];
		}

		sum /= 50;
		if (sceneIndex != "NULL")
			std::cout << "Average FPS (last 50 frames): " << std::setprecision(4) << sum << "\n";

		index = 0;
	}

#endif

	frames[index] = fps;
	index++;
}

void App::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		s_rasterizer.GetCamera().ForwardMove();
		break;
	case 's':
		s_rasterizer.GetCamera().BackwardMove();
		break;
	case 'a':
		s_rasterizer.GetCamera().LeftMove();
		break;
	case 'd':
		s_rasterizer.GetCamera().RightMove();
		break;
	case 'x':
		s_rasterizer.ToggleXRot();
		break;
	case 'y':
		s_rasterizer.ToggleYRot();
		break;
	case 'z':
		s_rasterizer.ToggleZRot();
		break;

		// OBJ Scenes
	case '1':
		sceneIndex = "cube";
		break;
	case '2':
		sceneIndex = "cube3";
		break;
	case '3':
		sceneIndex = "bunny";
		break;
	case '4':
		sceneIndex = "tyra";
		break;
	case '5':
		sceneIndex = "armadillo";
		break;
	case '6':
		sceneIndex = "dragon";
		break;
	case '7':
		sceneIndex = "buddha";
		break;
	case '8':
		sceneIndex = "padoru";
		break;

		// Clear Scene
	case 'n':
		sceneIndex = "NULL";
		break;

	default:
		break;
	}
}

void App::SpecialDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		s_rasterizer.ToggleWireframe();
		break;
	case GLUT_KEY_F3:
		s_rasterizer.ToggleBackfaceCulling();
		break;
	case GLUT_KEY_LEFT:
		s_rasterizer.GetCamera().LeftRotation();
		break;
	case GLUT_KEY_RIGHT:
		s_rasterizer.GetCamera().RightRotation();
		break;
	default:
		break;
	}
}

void App::SpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		s_rasterizer.ToggleWireframe();
		break;
	case GLUT_KEY_F3:
		s_rasterizer.ToggleBackfaceCulling();
		break;

	case GLUT_KEY_F9:
		if (sceneIndex != "NULL")
			s_scenes[sceneIndex]->lights[0].ToggleOn();
		break;
	case GLUT_KEY_F10:
		if (sceneIndex != "NULL")
			s_scenes[sceneIndex]->lights[1].ToggleOn();
		break;
	case GLUT_KEY_F11:
		if (sceneIndex != "NULL")
			s_scenes[sceneIndex]->lights[2].ToggleOn();
		break;
	}
}

void App::OnKeyDown(unsigned char key, int x, int y)
{
	ms_instance->KeyDown(key, x, y);
}

void App::OnSpecialDown(int key, int x, int y)
{
	ms_instance->SpecialDown(key, x, y);
}

void App::OnSpecialUp(int key, int x, int y)
{
	ms_instance->SpecialUp(key, x, y);
}

void App::InitScenes()
{
	std::cout << "Initializing scenes..." << std::endl;

	// pseudo sunlight (orange, mostly specular)
	Light light1({ -1, 2, 10 }, 0.1f, 0.4f, 0.7f, { 1.f, 0.96471f, 0.81961f });
	// pseudo sky (blue, mostly diffuse)
	Light light2({ 1, 2, 10 }, 0.05f, 0.7f, 0.3f, { 0.87059f, 1.f, 0.99608f });
	// White Light (mostly ambient)
	Light light3({ 0, 0, 10 }, 0.4f, 0.1f, 0.1f, { 1.f, 1.f, 1.f });

	s_scenes.insert(std::pair<std::string, Scene*>("NULL", nullptr));
	s_scenes.insert(std::pair<std::string, Scene*>("cube", &ms_CUBE));
	s_scenes.insert(std::pair<std::string, Scene*>("cube3", &ms_CUBE3));
	s_scenes.insert(std::pair<std::string, Scene*>("bunny", &ms_BUNNY));
	s_scenes.insert(std::pair<std::string, Scene*>("tyra", &ms_TYRA));
	s_scenes.insert(std::pair<std::string, Scene*>("armadillo", &ms_ARMADILLO));
	s_scenes.insert(std::pair<std::string, Scene*>("dragon", &ms_DRAGON));
	s_scenes.insert(std::pair<std::string, Scene*>("buddha", &ms_BUDDHA));
	s_scenes.insert(std::pair<std::string, Scene*>("padoru", &ms_PADORU));

	// CUBE
	Entity cube(LoadOBJ("cube.obj"));
	cube.SetTranslation({ 0.f, 0.f, -5.f });
	ms_CUBE.entities.push_back(cube);
	ms_CUBE.lights.push_back(light1);
	ms_CUBE.lights.push_back(light2);
	ms_CUBE.lights.push_back(light3);

	// CUBE3
	Entity cube3(LoadOBJ("cube3.obj"));
	cube3.SetTranslation({ 0.f, 0.f, -5.f });
	cube3.SetScale({ 0.5f, 0.5f, 0.5f });
	ms_CUBE3.entities.push_back(cube3);
	ms_CUBE3.lights.push_back(light1);
	ms_CUBE3.lights.push_back(light2);
	ms_CUBE3.lights.push_back(light3);

	// BUNNY
	Entity bunny(LoadOBJ("bunny.obj"));
	bunny.SetTranslation({ 0.f, 0.f, -5.f });
	bunny.SetScale({ 0.4f, 0.4f, 0.4f });
	bunny.SetRotation({ 0.f, 0.f, 180.f });
	ms_BUNNY.entities.push_back(bunny);
	ms_BUNNY.lights.push_back(light1);
	ms_BUNNY.lights.push_back(light2);
	ms_BUNNY.lights.push_back(light3);

	// TYRA
	Entity tyra(LoadOBJ("tyra.obj"));
	tyra.SetTranslation({ 0.f, 0.f, -5.f });
	tyra.SetScale({ -0.4f, -0.4f, -0.4f });
	tyra.SetRotation({ 0.f, 0.f, 0.f });
	ms_TYRA.entities.push_back(tyra);
	ms_TYRA.lights.push_back(light1);
	ms_TYRA.lights.push_back(light2);
	ms_TYRA.lights.push_back(light3);

	// ARMADILLO
	Entity armadillo(LoadOBJ("armadillo.obj"));
	armadillo.SetTranslation({ 0.f, 0.f, -5.f });
	armadillo.SetScale({ 0.4f, 0.4f, 0.4f });
	armadillo.SetRotation({ 0.f, 180.f, 180.f });
	ms_ARMADILLO.entities.push_back(armadillo);
	ms_ARMADILLO.lights.push_back(light1);
	ms_ARMADILLO.lights.push_back(light2);
	ms_ARMADILLO.lights.push_back(light3);

	// DRAGON
	Entity dragon(LoadOBJ("dragon.obj"));
	dragon.SetTranslation({ 0.f, 0.f, -5.f });
	dragon.SetScale({ 1.2f, 1.2f, 1.2f });
	dragon.SetRotation({ 0.f, 90.f, 180.f });
	ms_DRAGON.entities.push_back(dragon);
	ms_DRAGON.lights.push_back(light1);
	ms_DRAGON.lights.push_back(light2);
	ms_DRAGON.lights.push_back(light3);

	// BUDDHA
	Entity buddha(LoadOBJ("buddha.obj"));
	buddha.SetTranslation({ 0.f, 0.f, -5.f });
	buddha.SetScale({ 1.2f, 1.2f, 1.2f });
	buddha.SetRotation({ 0.f, 180.f, 180.f });
	ms_BUDDHA.entities.push_back(buddha);
	ms_BUDDHA.lights.push_back(light1);
	ms_BUDDHA.lights.push_back(light2);
	ms_BUDDHA.lights.push_back(light3);

	// PADORU
	Entity padoru(LoadOBJ("padoru.obj"));
	padoru.SetTranslation({ 0.f, 1.0f, -5.f });
	padoru.SetScale({ 0.35f, 0.35f, 0.35f });
	padoru.SetRotation({ 0.f, 0.0f, 180.f });
	ms_PADORU.entities.push_back(padoru);
	ms_PADORU.lights.push_back(light1);
	ms_PADORU.lights.push_back(light2);
	ms_PADORU.lights.push_back(light3);
}

void App::PrintInstructions()
{
	printf("\n\n\n\n\nControls:\n\n");
	printf("\tMovement: \tWASD\n");
	printf("\t\t\tL_ARROW and R_ARROW to turn camera\n");
	printf("\tScenes: \n");
	printf("\t\tN-\tClear Scene\n");
	printf("\t\t1-\tRegular white cube\n");
	printf("\t\t2-\tCube with 2 different textures\n");
	printf("\t\t3-\tBunny*\n");
	printf("\t\t4-\tVelociraptor**\n");
	printf("\t\t5-\tArmadillo**\n");
	printf("\t\t6-\tDragon***\n");
	printf("\t\t7-\tBuddha***\n");
	printf("\t\t8-\tHASHIRE SORI YO*\n");
	printf("\t( * = performance hit)\n");
	printf("\tDisplay:\n");
	printf("\t\tToggle wireframe: F1\n");
	printf("\t\tToggle backface culling: F3\n");
	printf("\t\tToggle light1: F9\n");
	printf("\t\tToggle light2: F10\n");
	printf("\t\tToggle light3: F11\n");
	printf("\t(lights may need a movement command in order to update)\n\n\n");
}