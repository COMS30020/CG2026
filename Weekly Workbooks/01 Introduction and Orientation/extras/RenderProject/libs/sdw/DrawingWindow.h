#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "SDL.h"

#ifdef _WIN32
const std::string separator("\\");
#else
const std::string separator("/");
#endif

class DrawingWindow {

public:
	size_t width;
	size_t height;

private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	std::vector<uint32_t> pixelBuffer;
	bool isFullscreen = false;

public:
	DrawingWindow();
	DrawingWindow(int w, int h);
	void renderFrame();
	void savePPM(const std::string &foldername, const std::string &filename) const;
	void saveBMP(const std::string &foldername, const std::string &filename) const;
	bool pollForInputEvents(SDL_Event &event);
	void exitCleanly();
	void setPixelColour(size_t x, size_t y, uint32_t colour);
	uint32_t getPixelColour(size_t x, size_t y);
	void clearPixels();
	void toggleFullscreen();
};

void printMessageAndQuit(const std::string &message, const char *error);
