#pragma once

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include <Colour.h>
#include <map>
#include <vector>
#include <ModelTriangle.h>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

#include "DrawingWindow.h"


class utilityFunctions {
   public:
      static std::vector<ModelTriangle> readObjFile(std::string filename, std::vector<ModelTriangle>& triangles, float scaleFactor,std::map<std::string,Colour>& colourMap);
      static std::map<std::string, Colour> readMatFile(std::string filename, std::map<std::string,Colour>& colourTable);
      static void loadModel(std::string objFile, std::string matFile, std::vector<ModelTriangle> &triangles, std::map<std::string,Colour> &colourMap, float scaleFactor);
      static std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues);
      static std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues);
      static uint32_t convertColourToUint32(const Colour& colour);
      static glm::vec3 translateCameraPosition(glm::vec3 cameraPosition, glm::vec3 translationVector);
      static std::vector<float> clearDepthBuffer(std::vector<float> depthBuffer);
};