#pragma once

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include <Colour.h>
#include <map>
#include <ModelTriangle.h>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


class Parser {
   public:
      static std::vector<ModelTriangle> readObjFile(std::string filename, std::vector<ModelTriangle>& triangles, float scaleFactor,std::map<std::string,Colour>& colourMap);
      static std::map<std::string, Colour> readMatFile(std::string filename, std::map<std::string,Colour>& colourTable);
      static void loadObj(std::string objFile, std::string matFile, std::vector<ModelTriangle> &triangles, std::map<std::string,Colour> &colourMap, float scaleFactor);
};


