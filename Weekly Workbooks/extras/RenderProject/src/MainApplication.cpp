#include "Renderer.h"
#include "RasterisedRenderer.h"
#include "utilityFunctions.h"
#include "RayTracedRenderer.h"

#include <Colour.h>

#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp> // allows you to output vec3 in standard output.

#include "../libs/sdw/CanvasTriangle.h"

struct Colour;
// Define globals for WIDTH and HEIGHT of the window (can be accessed from any renderer)
extern const int WIDTH = 320;
extern const int HEIGHT = 240;

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT);
// RedNoiseRenderer redNoise = RedNoiseRenderer();
// ColourSpectrumRenderer colourSpectrumRenderer = ColourSpectrumRenderer();

bool savingFrames = false;
int frameCounter = 0;

// // Core 3D rendering data structures (for when we eventually get around to working in 3D ;o)
std::vector<ModelTriangle> triangles; // week 4
std::map<std::string, Colour> colourPalette;
glm::vec3 cameraPosition(0.0, 0.0, 4.0); // camera centered in the x and  y dimension but stepped a 'back a bit' in the z
glm::mat3 cameraOrientation;
glm::vec3 lightPosition(0.0, 0.8, 0.0);
float focalLength = 2.0; // setting the focal length at 2.0 positions the image plane halfway between the camera and the centre of the model.
std::vector <float> zDepthBuffer(WIDTH*HEIGHT, 0.0);

//Renderers
RasterisedRenderer rasterisedRenderer = RasterisedRenderer(cameraPosition, lightPosition, cameraOrientation, focalLength, colourPalette, triangles, zDepthBuffer);
RayTracedRenderer rayTracedRenderer = RayTracedRenderer(cameraPosition, lightPosition, cameraOrientation, focalLength, colourPalette, triangles);
Renderer* currentRenderer = &rayTracedRenderer; // pointer to current renderer


void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u) {
			// limit these to visual points in the screen.
			// CanvasPoint v1 = CanvasPoint(rand()%window.width, rand()%window.height);
			// CanvasPoint v2 = CanvasPoint(rand()%window.width, rand()%window.height);
			// CanvasPoint v3 = CanvasPoint(rand()%window.width, rand()%window.height);

			// Colour colour = Colour(rand()%256, rand()%256, rand()%256);
			//CanvasTriangle triangle = CanvasTriangle(v1, v2, v3);
			//rasterisedRenderer.drawStrokedTriangles(triangle, window, Colour(255,255,255));
		}
		else if (event.key.keysym.sym == SDLK_f) {
			// limit these to visual points in the screen.
			// CanvasPoint v1 = CanvasPoint(rand()%window.width, rand()%window.height);
			// CanvasPoint v2 = CanvasPoint(rand()%window.width, rand()%window.height);
			// CanvasPoint v3 = CanvasPoint(rand()%window.width, rand()%window.height);

			// Colour colour = Colour(rand()%256, rand()%256, rand()%256);

			//CanvasTriangle triangle = CanvasTriangle(v1, v2, v3);

			//rasterisedRenderer.drawFilledTriangles(triangle, window, colour);
			//rasterisedRenderer.drawStrokedTriangles(triangle, window, Colour(255,255,255));
		}
		else if (event.key.keysym.sym == SDLK_TAB) savingFrames = !savingFrames;
		else if (event.key.keysym.sym == SDLK_RETURN) window.toggleFullscreen();
	}
}

int main(int argc, char *argv[]) {
	SDL_Event event;
	// if i run with make file (i.e. terminal) relative path for these  won't lead to a render?
	std::string matFile = "../src/models/cornellBox/cornell-box.mtl";
	std::string objFile = "../src/models/cornellBox/cornell-box.obj";
	float scaleFactor = 0.35;

	utilityFunctions::loadModel(objFile, matFile, triangles, colourPalette, scaleFactor);

	// colourPalette = utilityFunctions::readMatFile(matFile, colourPalette);
	// triangles = utilityFunctions::readObjFile(objFile, triangles, scaleFactor, colourPalette);

	while (true) { // forever loop
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);

		/// polling for incoming mouse and keyboard events
		// In C++ we can't use "." notation for dynamic dispatch, so we have to use -> instead :o(
		currentRenderer->draw(window);/// draws screen with current renderer --> if something already drawn by event handline the window.clearPixels() cleans it.
		// Need to ask the SDL window to render the frame, otherwise nothing actually gets shown on the screen !
		window.renderFrame(); // renders the memory buffers

		if(savingFrames) {
			std::string paddedCounter = std::to_string(frameCounter);
			while(paddedCounter.length() < 6) paddedCounter = "0" + paddedCounter;
			// Use whichever image file format works best on your platform (comment out the one you don't need)
			window.savePPM("saved-frames", paddedCounter + ".ppm");
			window.saveBMP("saved-frames", paddedCounter + ".bmp");
			frameCounter++;
		}
	}
}
