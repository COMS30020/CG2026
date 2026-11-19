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
			glm::vec3 rayDirection = transposePlaneCoordsToRay(glm::vec3(x, y, -focalLength*160));
			RayTriangleIntersection intersection = getClosestIntersection(rayDirection);

			if (intersection.distanceFromCamera != -1.0) {
				//std::cout << intersection.intersectedTriangle.colour <<  "intersection" << std::endl;
				Colour triangleColour = intersection.intersectedTriangle.colour;
				//std::cout << intersection.intersectedTriangle.colour << "intersection" << std::endl;
				uint32_t colour = utilityFunctions::convertColourToUint32(triangleColour);
				window.setPixelColour(x, y, colour);
				//std::cout << "bleh" << std::endl;
			}
		}
	}
}


glm::vec3 RayTracedRenderer::transposePlaneCoordsToRay(glm::vec3 planePoint) {
	float x=planePoint.x -WIDTH/2;
	float y= HEIGHT/2-planePoint.y; //otherwise box is upside down.
	glm::vec3 rayDirection = glm::vec3(x, y, planePoint.z);
	glm::vec3 rayNormalised = glm::normalize(rayDirection);
	//std::cout << rayNormalised.x << " " << rayNormalised.y << std::endl;

	return rayNormalised;
}

std::vector<RayTriangleIntersection> RayTracedRenderer::sortPointsByT(std::vector<RayTriangleIntersection>  unsortedIntersections) {

	int n = unsortedIntersections.size();

	// Outer loop that corresponds to the number of elements to be sorted
	for (int i = 0; i < n - 1; i++) {
		// Last i elements are already in place
		for (int j = 0; j < n - i - 1; j++) {

			// Comparing adjacent elements
			if (unsortedIntersections[j].distanceFromCamera > unsortedIntersections[j + 1].distanceFromCamera)

				// Swapping if in the wrong order
				std::swap(unsortedIntersections[j], unsortedIntersections[j + 1]);
		}
	}
	return unsortedIntersections;
}



// what happens if no intersection?? --> that's why validation.
RayTriangleIntersection RayTracedRenderer::getClosestIntersection(glm::vec3 rayDirection) {
	std::vector<RayTriangleIntersection> intersections;
	RayTriangleIntersection closestIntersection = RayTriangleIntersection(cameraPosition, -1.0, triangles[0],-1);

	size_t triangleInd = 0;
	for (auto&&triangle : triangles) {

		//std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << std::endl;

		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = cameraPosition - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		// and screen is in direction opposite us :)
		// t, u, v vector
		float t = possibleSolution.x;
		float u = possibleSolution.y;
		float v = possibleSolution.z;
		// std::cout << t << ", " << u << ", " << v << std::endl;

		// adds intersection to intersection list if u and v are within the triangle
		// and if t is positive --> if t < 0 then the triangle renders behind camera so don't need the intersection.
		if (((u >= 0.0)&& (u <= 1.0)) ) {
			if ((v >= 0.0)&& (v <= 1.0)) {
				if (((u+v) <= 1.0) && (t >= 0.0)){
					glm::vec3 positionVector = cameraPosition + (t*rayDirection);

					RayTriangleIntersection intersection = RayTriangleIntersection(positionVector, possibleSolution.x, triangle, triangleInd);
					// collect all intersections and get one with smallest absolute dist.
					intersections.push_back(intersection);
				}
			}
		}
		triangleInd++;
	}

	if (intersections.empty()) return closestIntersection;
	// do after u check for all possible intersects.

    intersections = sortPointsByT(intersections);
	closestIntersection = intersections[0];

	return closestIntersection;
}
