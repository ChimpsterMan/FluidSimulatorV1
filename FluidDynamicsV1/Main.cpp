// utility
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/stb_image.h>
// idk but manually importing fixes the problem
// #include <img/ImageLoader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <shader.h>

#include <FileSystem>
#include <iostream>

#include <vector>
#include <chrono>
#include <cmath>
#include <ctime>
#include <windows.h>

// written class refrences
#include "GraphicsEngine.h"
#include "Asset.h"

#include "TextManager.h"

#include "FluidSimulator.h"

// graphics links
// models
const char* BarModelPath = "resources\\objects\\bar\\bar.obj";

// skybox paths
const std::vector<const char*> cloudySkybox
{
	"resources/textures/CloudySkyBox/cubemap_1.jpg",
	"resources/textures/CloudySkyBox/cubemap_3.jpg",
	"resources/textures/CloudySkyBox/cubemap_4.jpg",
	"resources/textures/CloudySkyBox/cubemap_5.jpg",
	"resources/textures/CloudySkyBox/cubemap_0.jpg",
	"resources/textures/CloudySkyBox/cubemap_2.jpg"
};

// prototypes
// control callback for clicking the mouse
void mouse_button_callback_custom(GLFWwindow* window, int button, int action, int mods);
// control callback for moving the mouse
void mouse_callback_custom(GLFWwindow* window, double xpos, double ypos);

// settings
float relativeScreenSize = 0.85;
float aspectRatio = 16 / 9;

// color (controls the height of the sin function for the color rainbow)
int colorRange = 25;

// default
unsigned int SCR_WIDTH = 1600 * relativeScreenSize;
unsigned int SCR_HEIGHT = 900 * relativeScreenSize;

// timing stuff
bool fpsCounterEnabled = true;
const double fps = 60;

int fpsCount;
int fpsCounter;

// status vars
int gameState;

// fluid drawing information
vector<vector<Asset*>> fluidAssets;

FluidSimulator simulation;

// main class vars
GraphicsEngine graphics;

void initAssets(glm::vec3 pos, glm::vec2 size, float scale, float divider = 0) {
	fluidAssets = vector<vector<Asset*>>();

	for (int y = 0; y < size.y; y++) {
		fluidAssets.push_back(vector<Asset*>());

		for (int x = 0; x < size.x; x++) {
			fluidAssets[y].push_back(new Asset(graphics.getModel("bar.obj"), pos + glm::vec3(x + x*divider, 1, y + y*divider), glm::vec3(0), glm::vec3(scale)));
			graphics.addAsset(fluidAssets[y][x]);
		}
	}
}

// set the y scales to the appropriate values of the sim
void updateAssets() {
	vector<vector<float>> floatField = simulation.getFloatField();

	for (int y = 0; y < fluidAssets.size(); y++) {
		for (int x = 0; x < fluidAssets[y].size(); x++) {
			fluidAssets[y][x]->setScale(glm::vec3(fluidAssets[y][x]->scale.x, floatField[y][x], fluidAssets[y][x]->scale.z));

			float frequency = 0.33;

			glm::vec3 color = glm::vec3(1);
			color.x = sin((0 * frequency + (floatField[y][x] / colorRange))*(2 * 3.1415926535))*0.5 + 0.5;
			color.y = sin((2 * frequency + (floatField[y][x] / colorRange))*(2 * 3.1415926535))*0.5 + 0.5;
			color.z = sin((4 * frequency + (floatField[y][x] / colorRange))*(2 * 3.1415926535))*0.5 + 0.5;
			fluidAssets[y][x]->setOverrideColor(color);
		}
	}
}

void setup() {
	// set window size to max while also maintaining size ratio
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);

	SCR_WIDTH = (rect.right - rect.left) * relativeScreenSize;
	SCR_HEIGHT = (rect.bottom - rect.top) * relativeScreenSize;

	if (SCR_WIDTH / SCR_HEIGHT < aspectRatio) {
		// base the size off the width
		SCR_HEIGHT = SCR_WIDTH * (1 / aspectRatio);
	}
	if (SCR_HEIGHT / SCR_WIDTH > aspectRatio) {
		// base the size off the height
		SCR_WIDTH = SCR_HEIGHT * (aspectRatio);
	}

	// make the graphics engine (Jordan: Do not focus too much on this, it is very complicated and not relevant to the problem.
	graphics = GraphicsEngine("Fluid Simulator V1", &SCR_WIDTH, &SCR_HEIGHT, 1, false);

	// add all the models that are going to be used immediatley
	graphics.addModel(BarModelPath);

	// set skybox
	graphics.setSkybox(cloudySkybox);

	// set light
	graphics.setLight(glm::vec3(100, 150, 100), glm::vec3(1));
	graphics.getLight()->visible = false;

	// set camera starting pos
	graphics.camera.setPos(glm::vec3(50, 100, -300));
	graphics.camera.speedMultiplier = 40;

	// set callbacks
	glfwSetCursorPosCallback(graphics.window, mouse_callback_custom);
	glfwSetMouseButtonCallback(graphics.window, mouse_button_callback_custom);

	// status info
	fpsCount = 0;
	fpsCounter = 0;

	gameState = 1;

	// setup fluid info
	simulation = FluidSimulator(100, 100);
	initAssets(glm::vec3(0), glm::vec2(100, 100), 1);

	simulation.addFluid(25, 25, 50, 50, 25);
	//simulation.addFluid(0, 0, 25, 25, 25);
	//simulation.addFluid(75, 75, 25, 25, 25);
}

int main() {
	// init
	setup();

	// run
	while (gameState == 1) {
		// START timer
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

		// MAIN
		simulation.update();

		// update assets
		updateAssets();

		// render frame
		gameState = graphics.renderFrame();

		// END of timer sleep and normalize the clock
		std::chrono::system_clock::time_point after = std::chrono::system_clock::now();
		std::chrono::microseconds difference(std::chrono::time_point_cast<std::chrono::microseconds>(after) - std::chrono::time_point_cast<std::chrono::microseconds>(now));

		// count the fps
		int diffCount = difference.count();
		if (diffCount == 0) {
			diffCount = 1;
		}

		int sleepDuration = ((1000000 / fps * 1000) - diffCount) / 1000000;

		// output fps
		fpsCount += 1;
		fpsCounter += 1000000 / diffCount;

		if (fpsCount % int(fps) == 0) {
			if (fpsCounterEnabled) {
				std::cout << "\rFPS: " << fpsCounter / fpsCount;
			}
			fpsCount = 0;
			fpsCounter = 0;
		}

		if (sleepDuration < 0) {
			sleepDuration = 0;
		}

		// std::cout << sleepDuration << std::endl;
		Sleep(sleepDuration);
	}

	std::cout << "Closing" << std::endl;
	return gameState;
}

// callbacks
// clicking
void mouse_button_callback_custom(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		
	}
}

// mouse movement
void mouse_callback_custom(GLFWwindow* window, double xpos, double ypos)
{
	
}