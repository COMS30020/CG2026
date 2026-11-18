#pragma once

#include "Renderer.h"
#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include <Colour.h>
#include <map>
#include "../libs/sdw/RayTriangleIntersection.h"


#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


class RayTracedRenderer: public Renderer {
   public:
      const glm::vec3& cameraPosition;
      const glm::vec3& lightPosition;
      const glm::mat3& cameraOrientation;
      const float& focalLength;
      const std::map<std::string, Colour>& colourPalette;
      const std::vector<ModelTriangle>& triangles;

      void draw(DrawingWindow &window) override;
      RayTriangleIntersection getClosestIntersection(glm::vec3 rayDirection);
      std::vector<RayTriangleIntersection> sortPointsByT(std::vector<RayTriangleIntersection>  unsortedIntersections);
      glm::vec3 transposePlaneCoordsToRay(glm::vec3 planePoint);

    // constructor of raytraced renderer
    RayTracedRenderer(const glm::vec3 &cameraPosition_, glm::vec3 &lightPosition_, const glm::mat3 &cameraOrientation_, float &focalLength_,
                       const std::map<std::string, Colour> &colourPalette_, const std::vector<ModelTriangle> &triangles_) :
                       cameraPosition(cameraPosition_), lightPosition(lightPosition_), cameraOrientation(cameraOrientation_),
                       focalLength(focalLength_), colourPalette(colourPalette_),triangles(triangles_) {}

};


