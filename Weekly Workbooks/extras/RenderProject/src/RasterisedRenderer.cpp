#include "RasterisedRenderer.h"
#include "utilityFunctions.h"
#include <CanvasPoint.h>
#include <Colour.h>
#include <map>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "ColourSpectrumRenderer.h"
#include "../libs/sdw/CanvasTriangle.h"
#include "../libs/sdw/Utils.h"
#include <Colour.h>

#include "../libs/sdw/DrawingWindow.h"
#include "../libs/sdw/ModelTriangle.h"


void RasterisedRenderer::draw(DrawingWindow &window) {
	// window.clearPixels(); //--> this empties the screen.
	// Core 3D rendering data structures (for when we eventually get around to working in 3D ;o)

	std::vector<CanvasTriangle> wireFrame;
	std::vector<Colour> triangleColours; // colour of each canvas triangle
	//std::vector<CanvasPoint> pointCloud;

	for (auto&&modelTriangle : triangles) {
		std::vector<CanvasPoint> triangleVertices;
		for (auto&&vertex : modelTriangle.vertices) {
			CanvasPoint point = projectVertexOntoCanvasPoint(cameraPosition, focalLength,vertex,window);
			triangleVertices.push_back(point);
			//pointCloud.push_back(point);
		}
		CanvasTriangle canvasTriangle = CanvasTriangle(triangleVertices[0], triangleVertices[1], triangleVertices[2]);
		wireFrame.push_back(canvasTriangle);
		triangleColours.push_back(modelTriangle.colour);
	}

	// uint32_t colour = (255 << 24) + (0 << 16) + (255 << 8) + 0; // UNSIGNED INTEGER (THEREFORE, NEGATIVE INTEGER)
	int triangleColourIndex = 0;
	for (auto&&triangle : wireFrame) {
		drawFilledTriangles(triangle, window, triangleColours[triangleColourIndex], zDepthBuffer);
		triangleColourIndex++;
	}
}
CanvasPoint RasterisedRenderer::projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition, DrawingWindow &window) {

	// convert from model coordinate system to camera coordinate system. --> camera is origin.
	// model. verticies  relative to the centre of the room


	float imagePlaneScaling = 160.0;

	glm::vec3 posRelativeToCamera = vertexPosition - cameraPosition;
	float inverseZ = posRelativeToCamera.z;
	// subtract from window.width for u to shift right. -->
	// points with negative coordinates are meant to be projected onto the right of the plane.
	// because 0,0 starts in middle of model when it attempts to project right side adjusted to camera it gets skewed weird?
	// they are plotted outside the image plane --> so subtract from window width
	//positive points are supposted to be on the left of image plane. negative on the right.

	float u = window.width/2 -(focalLength * (posRelativeToCamera.x / posRelativeToCamera.z))*imagePlaneScaling ;
	float v = (focalLength * (posRelativeToCamera.y/ posRelativeToCamera.z))*imagePlaneScaling +window.height/2;

	CanvasPoint twoDPoint = CanvasPoint(round(u),round(v));
	twoDPoint.depth = 1/inverseZ;

	return twoDPoint;
}

void RasterisedRenderer::lineDrawing(CanvasPoint from, CanvasPoint to, DrawingWindow &window, Colour colourOfLine, std::vector<float> &zDepthBuffer) {

	float xDiff = to.x - from.x;
	float yDiff = to.y - from.y;
	float depthDiff = to.depth - from.depth;

	// Colour WHITE = Colour(255,255,255);
	uint32_t colour = (255 << 24) + (colourOfLine.red << 16) + (colourOfLine.green << 8) + colourOfLine.blue; // UNSIGNED INTEGER (THEREFORE, NEGATIVE INTEGER)

	float numberOfSteps = fmax(abs(xDiff), abs(yDiff));

	if (numberOfSteps>0) {
		// always using the bigger number of steps ensures we don't skip pixels --> how does that ensure this?
		float xStepSize = xDiff / numberOfSteps;
		// work out step size of x or y by dividing total distance by number of steps take
		// (i.e. the n.o intervals --> similar to interpolation)
		float yStepSize = yDiff / numberOfSteps;

		float depStepSize = depthDiff / numberOfSteps;

		std::vector<float> zDepths = utilityFunctions::interpolateSingleFloats(from.depth, to.depth, numberOfSteps);

		for (int i=0; i <= numberOfSteps; i++) {
			float x = from.x + (xStepSize * i);
			float y = from.y + (yStepSize * i);

			if (x>0 && y> 0 && x < window.width && y < window.height) { // restrict the drawing space//
				float currentZDepth = abs(from.depth - abs(depStepSize));
				//currentZDepth = abs(zDepths[floor(abs((x-from.x)))]);
				int i = y * window.width + x;
				//std::cout<<"origin:" << currentZDepth <<std::endl;

				if (zDepthBuffer[i] <= currentZDepth) {
					//std::cout <<"X: "<< x <<"Y: "<< y<< "z: "<< currentZDepth <<std::endl;
					window.setPixelColour(floor(x), floor(y),  colour ); // can't have 1.5 pixel therefore need to round the x and y.
					zDepthBuffer[i] = currentZDepth; //update zdepth buffer
				}
			}
		}
	}
}

void RasterisedRenderer::drawStrokes(CanvasPoint from, CanvasPoint to, DrawingWindow &window, Colour colourOfLine) {

	int xDiff = to.x - from.x;
	int yDiff = to.y - from.y;

	// Colour WHITE = Colour(255,255,255);
	uint32_t colour = (255 << 24) + (colourOfLine.red << 16) + (colourOfLine.green << 8) + colourOfLine.blue; // UNSIGNED INTEGER (THEREFORE, NEGATIVE INTEGER)

	float numberOfSteps = fmax(abs(xDiff), abs(yDiff));
	// always using the bigger number of steps ensures we don't skip pixels --> how does that ensure this?
	float xStepSize = xDiff / numberOfSteps;
	// work out step size of x or y by dividing total distance by number of steps take
	// (i.e. the n.o intervals --> similar to interpolation)
	float yStepSize = yDiff / numberOfSteps;


	for (int i=0; i <= numberOfSteps; i++) {
		float x = from.x + (xStepSize * i);
		float y = from.y + (yStepSize * i);

		if (x>0 && y> 0 && x < window.width && y < window.height) { // restrict the drawing space//
				window.setPixelColour(floor(x), floor(y), colour ); // can't have 1.5 pixel therefore need to round the x and y.
			}
		}
	}

void RasterisedRenderer::drawStrokedTriangles(CanvasTriangle triangle, DrawingWindow &window, Colour colour) {
	drawStrokes(triangle.vertices[0], triangle.vertices[1], window, colour);
	drawStrokes(triangle.vertices[0], triangle.vertices[2], window, colour);
	drawStrokes(triangle.vertices[1], triangle.vertices[2], window, colour);
}

std::vector<CanvasPoint> RasterisedRenderer::sortCoordsByY(std::vector<CanvasPoint>  unsortedVertices) {

	int n = unsortedVertices.size();

	// Outer loop that corresponds to the number of elements to be sorted
	for (int i = 0; i < n - 1; i++) {
		// Last i elements are already in place
		for (int j = 0; j < n - i - 1; j++) {

			// Comparing adjacent elements
			if (unsortedVertices[j].y > unsortedVertices[j + 1].y)

				// Swapping if in the wrong order
					std::swap(unsortedVertices[j], unsortedVertices[j + 1]);
		}
	}
	return unsortedVertices;
}

float RasterisedRenderer::calculateGradient(CanvasPoint p1, CanvasPoint p2) {
	float yDiff = p2.y - p1.y;
	float xDiff = p2.x - p1.x;
	float gradient = yDiff;
	if (xDiff != 0) gradient = yDiff/xDiff;
	return gradient;
}


std::vector<CanvasPoint> RasterisedRenderer::obtainPoints(CanvasPoint firstPoint, CanvasPoint secondPoint, int pointCount) {
	float gradientInverse = 1/calculateGradient(firstPoint, secondPoint);

	std::vector<float> yPoints = utilityFunctions::interpolateSingleFloats(firstPoint.y, secondPoint.y, pointCount);

	std::vector<float> zDepths = utilityFunctions::interpolateSingleFloats(firstPoint.depth, secondPoint.depth, pointCount);

	std::vector<CanvasPoint> points;

	for (int i = 0; i < pointCount; i++) {
		float xPoint = firstPoint.x+((yPoints[i]-firstPoint.y) * gradientInverse); // IT WORKS :D
		CanvasPoint newPoint = CanvasPoint(xPoint,yPoints[i]);
		newPoint.depth = zDepths[i];
		points.push_back(newPoint);

	}
	return points;
}

void RasterisedRenderer::drawFilledTriangles(CanvasTriangle triangle, DrawingWindow &window, Colour colour, std::vector<float> &zDepthBuffer) {
	//drawStrokedTriangles(triangle, window, colour);
	// lineDrawing(triangle.vertices[0], triangle.vertices[1], window, colour, zDepthBuffer);
	// lineDrawing(triangle.vertices[0], triangle.vertices[2], window, colour, zDepthBuffer);
	// lineDrawing(triangle.vertices[1], triangle.vertices[2], window, colour,zDepthBuffer);
	std::vector<CanvasPoint> verticiesList;

	for (int i =0; i <3; i++) {
		verticiesList.push_back(triangle.vertices[i]);
	}
	verticiesList = sortCoordsByY(verticiesList);

	float gradientInverse = 1/calculateGradient(verticiesList[0], verticiesList[2]); // x/y to get 1/m (y=mx+c where c = 0 )
	//we have y dist between the middle point and top point by yCoords[1] - yCoords[0]
	//we can work out the x difference by multiplying the y difference by the gradient inverse
	// midYdiff*xdiff/ydiff= midXdiff.
	// add midXdiff to the xCoords[0] -> if midXdiff <0  xCoords[0] +- midXdiff subtracts it.
	// if its >0 then it adds as appropriate :) LORD WHY WAS THAT SO HARD???

	float extraPointX = verticiesList[0].x+((verticiesList[1].y-verticiesList[0].y) * gradientInverse); // IT WORKS :D
	CanvasPoint extraPoint = CanvasPoint(round(extraPointX),verticiesList[1].y);

	int numberOfRakes = verticiesList[1].y-verticiesList[0].y + 1;
	// shouldn't be the same number of points on both sides :/ --> because the lines have different gradients
	// due to rounding if you don't include an extra rake line then based on how the floats are rounded you could skip a line.
	// as a result the extra line could 'overlap' with a point that you think that has colour but it doesn't.
	// also starting point count list at 0 because then we are guaranteed to have a line at the top of the triangle.
	// ALSO for bottom rakes, the pointy[0] contains the middle line lol. top function doesn't colour that in !!!
	std::vector<CanvasPoint> firstLinePoints = obtainPoints(verticiesList[0],verticiesList[1], numberOfRakes);
	std::vector<CanvasPoint> secondLinePoints = obtainPoints(verticiesList[0],extraPoint, numberOfRakes);

	int bottomRakes =  verticiesList[2].y-verticiesList[1].y +1;
	std::vector<CanvasPoint> downLinePoints = obtainPoints(verticiesList[1],verticiesList[2], bottomRakes);
	std::vector<CanvasPoint> secondDownLinePoints = obtainPoints(extraPoint,verticiesList[2], bottomRakes);

	//top triangle
	for (int  y = verticiesList[0].y; y < verticiesList[1].y; y++) {
		for (int x = 0; x < numberOfRakes; x++) {
			lineDrawing(firstLinePoints[x], secondLinePoints[x],window,colour, zDepthBuffer);
		}
	}

	// bottom triangle
	for (int  y = verticiesList[1].y; y < verticiesList[2].y; y++) {
		for (int x = 0; x < bottomRakes; x++) {
			lineDrawing(downLinePoints[x], secondDownLinePoints[x],window,colour,zDepthBuffer);
		}
	}


}

	// texture map later. --> just need theory for midterm!!!!!! -_- doing easier way after reading week.
