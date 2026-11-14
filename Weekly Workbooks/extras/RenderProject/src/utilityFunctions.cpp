#include "Parser.h"
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




std::map<std::string, Colour> readMatFile(std::string filename, std::map<std::string,Colour> &colourMap) {
	std::ifstream file(filename); // read file
	std::string currentLine; // string to store each line
	std::string nameOfColour; // key Colour

	bool isLineRGB = false;

	while (getline (file, currentLine)) {

		std::vector<std::string> tokens = split(currentLine, ' '); // gets the first character after space of each line.

		if (isLineRGB) {
			Colour colour;
			// --> first number red , second number green, third number blue
			colour.red = std::round(std::stof(tokens[1]) *255.0f); // indexing starts at 1.
			colour.green = std::round(std::stof(tokens[2])*255.0f); // indexing starts at 2.
			colour.blue = std::round(std::stof(tokens[3])*255.0f); // indexing starts at 3.

			isLineRGB = false;
			colourMap.emplace(nameOfColour,colour);
		}
		// is a new colour being declared?
		if (tokens[0] == "newmtl") {
			// create new colour
			nameOfColour = tokens[1];
			isLineRGB = true;
		}
	}
	return colourMap;
}

std::vector<ModelTriangle> readObjFile(std::string filename, std::vector<ModelTriangle>& savedTriangle, float scaleFactor, std::map<std::string,Colour>& colourMap) {

	std::ifstream file(filename); // read file
	std::string currentLine; // string to store each line
	std::vector<glm::vec3> verticesList; // stores verticies
	std::vector<float> vertexList;
	Colour currentColour = Colour(255,255,255);
	std::map<std::vector<int>, Colour> faces;

	while (getline (file, currentLine)) {

		std::vector<std::string> tokens = split(currentLine, ' '); // gets the first character after space of each line
		if (tokens[0] == "usemtl") {
			// set the current colour being processed from the colour table
			currentColour = colourMap.find(tokens[1])->second;
		}

		if (currentLine[0] == 'v') {
			glm::vec3 vertices;

			std::vector<std::string> lineTokens = split(currentLine, ' ');

			for (int i = 1; i <= lineTokens.size(); i++) { // should always be 3.
				if (lineTokens[i] == "v") continue;  // avoid first letter
				// stof --> string to float.
				// scale factor --> avoids bunnyzilla

				if (i == 1) vertices.x = std::stof(lineTokens[i]) * scaleFactor;
				if (i == 2) vertices.y = std::stof(lineTokens[i]) * scaleFactor;
				if (i == 3) vertices.z = std::stof(lineTokens[i]) * scaleFactor;
			}


			verticesList.push_back(vertices);
		} else if (currentLine[0] == 'f') {
			std::vector<int> newFace;
			std::vector<std::string> lineTokens = split(currentLine, ' ');

			// tokens are formatted like '2/'; make sure we only get the number
			for (const auto& token : lineTokens) {
				if (token == "f") continue;  // avoid first letter
				// must split tokens again to get rid of //
				std::vector<std::string> splitToken = split(token, '/');
				if (!splitToken.empty() && !splitToken[0].empty()) {
					int vertexNumber = std::stoi(splitToken[0]);
					//vertices in OBJ are indexed from 1 --> hence when fetching the vertices from vertex list, subtract one from the index stored in the face list.
					newFace.push_back(vertexNumber-1); // push the index of the side to the vectorList
				}
			}
			faces.insert({newFace, currentColour});
		}
	}
	// go through all faces and create model triangles based on vertex information -- AFTER reading thru file
	for (const auto& [face, colour] : faces) {

		ModelTriangle modelTriangle;
		int counter = 0; // adds vertices to model triangle
		// iterate through all vertices in a face and find in vertex list
		for (const auto& vertexIndex : face) {
			modelTriangle.vertices[counter] = verticesList[vertexIndex];
			counter++;
		}
		modelTriangle.colour = colour;

		savedTriangle.push_back(modelTriangle);
	}


	return savedTriangle;
}

void loadObj(std::string objFile, std::string matFile, std::vector<ModelTriangle> &triangles, std::map<std::string,Colour> &colourMap, float scaleFactor) {
	colourMap = readMatFile(matFile, colourMap);
	triangles = readObjFile(objFile, triangles, scaleFactor, colourMap);
}




