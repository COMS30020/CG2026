#include "RayTracedRenderer.h"
#include "utilityFunctions.h"
#include <CanvasPoint.h>
#include <Colour.h>
#include <map>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include "../libs/sdw/CanvasTriangle.h"
#include "../libs/sdw/Utils.h"
#include <Colour.h>

#include "../libs/sdw/DrawingWindow.h"
#include "../libs/sdw/ModelTriangle.h"
#include "../libs/sdw/RayTriangleIntersection.h"


class DrawingWindow;

void RayTracedRenderer::draw(DrawingWindow &window) {
	//window.clearPixels(); //--> this empies the screen.

	for (int x = 0; x < window.width; x++) {
		for (int y = 0; y < window.height; y++) {

			// glm::vec3 rayDirection(float(x), float(y), 1.0);
			// RayTriangleIntersection intersection = getClosestIntersection(camera, rayDirection, triangleList);

		}
	}
}

RayTriangleIntersection RayTracedRenderer::getClosestIntersection(glm::vec3 cameraPosition, glm::vec3 directionOfRay,std::vector<ModelTriangle> &triangleList) {

	float absDistance = 0.0;
	std::vector<RayTriangleIntersection> intersections;
	RayTriangleIntersection closestIntersection;

	for (int triangle = 0 ; triangle < triangleList.size(); triangle++) {

		glm::vec3 e0 = triangleList[triangle].vertices[1] - triangleList[triangle].vertices[0]; // p1-p0
		glm::vec3 e1 = triangleList[triangle].vertices[2] - triangleList[triangle].vertices[0]; //p2 -p0
		glm::vec3 SPVector = cameraPosition - triangleList[triangle].vertices[0];
		glm::mat3 DEMatrix(-directionOfRay, e0, e1); // -ray direction because negative z is away from us.
		// and screen is in direction opposite us :)
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
		glm::vec3 r = triangleList[triangle].vertices[0] + possibleSolution.y*e0 + possibleSolution.z*e1;
		glm::vec3 positionVector = cameraPosition + (possibleSolution.x*directionOfRay);

		if (positionVector == r) {
			RayTriangleIntersection intersection = RayTriangleIntersection(r, possibleSolution.t, triangleList[triangle], triangle);
			// collect all intersections and get one with smallest absolute dist.
			intersections.push_back(intersection);
		}
	}

	for (auto&& intersect : intersections) {
		if (intersect.distanceFromCamera < absDistance) {
			absDistance = intersect.distanceFromCamera;
			closestIntersection = intersect;
		}
	}
	return closestIntersection;
}

