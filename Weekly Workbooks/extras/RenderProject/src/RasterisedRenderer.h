#pragma once

#include "Renderer.h"
#include "CanvasPoint.h"
#include "utilityFunctions.h"

#include "CanvasTriangle.h"
#include <Colour.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

extern int trianglesToDraw;

class RasterisedRenderer: public Renderer {

   public:
      const glm::vec3& cameraPosition;
      const glm::vec3& lightPosition;
      const glm::mat3& cameraOrientation;
      const float& focalLength;
      const std::map<std::string, Colour>& colourPalette;
      const std::vector<ModelTriangle>& triangles;
      std::vector<float>& zDepthBuffer;


      void draw(DrawingWindow &window) override;
      void lineDrawing(CanvasPoint &from, CanvasPoint &to, DrawingWindow &window,  Colour& colourOfLine);
      void drawStrokedTriangles(CanvasTriangle &triangle, DrawingWindow &window,  Colour& colour);
      void drawFilledTriangles(CanvasTriangle &triangle, DrawingWindow &window,  Colour& colour);
      std::vector<CanvasPoint> sortCoordsByY(std::vector<CanvasPoint>  unsortedVertices);
      std::vector<CanvasPoint> obtainPoints(CanvasPoint firstPoint, CanvasPoint secondPoint, int pointCount);
      std::vector<float> obtainZDepths(CanvasPoint firstPoint, CanvasPoint secondPoint, int pointCount);
      float calculateGradient(CanvasPoint p1, CanvasPoint p2);
      CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition, DrawingWindow &window);

      // constructor of rasterised renderer
      RasterisedRenderer(const glm::vec3 &cameraPosition_, glm::vec3 &lightPosition_, const glm::mat3 &cameraOrientation_, float &focalLength_,
                         const std::map<std::string, Colour> &colourPalette_, const std::vector<ModelTriangle> &triangles_, std::vector<float> &zDepthBuffer_) :
                         cameraPosition(cameraPosition_), lightPosition(lightPosition_), cameraOrientation(cameraOrientation_),
                         focalLength(focalLength_), colourPalette(colourPalette_),triangles(triangles_),zDepthBuffer(zDepthBuffer_) {}
};



