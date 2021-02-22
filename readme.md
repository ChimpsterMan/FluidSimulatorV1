Fluid Simulator V1
Written in c++ and compiled with libraries for 64bit;

Requires VC_Redist 2015 or Later

Condition:
	Unfinished while I work on other projects. There is a very annoying bug that I can't 
quite seem to fix and other things are coming up at the time of development. If you wanna 
try to fix it or see it check void update() in the FluidSimulator.h file.

Installation (Project Files):
1. Download the entire repository
2. If the libraries imported are out of date or not working:
	1. Check that the project properties is importing them correctly
	2. Rebuild the libs (Listed Below) (Recommend CMAKE Compiler: https://cmake.org/download/)

Libraries
OpenGL: https://www.opengl.org//
GLFW: https://www.glfw.org/download.html
GLAD: https://glad.dav1d.de/
Assimp: http://assimp.org/
FreeType: https://www.freetype.org/

Credits:

Educational Links:
https://learnopengl.com/Getting-started/Creating-a-window