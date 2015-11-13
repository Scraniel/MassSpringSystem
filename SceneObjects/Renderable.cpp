/*
 * GLCurve.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: dglewis
 */

#include "Renderable.h"

Renderable::Renderable() {
	// TODO Auto-generated constructor stub
	MVP = IdentityMatrix();
	M = IdentityMatrix();

}

Renderable::~Renderable() {
	// TODO Auto-generated destructor stub
}


std::vector<Vec3f> Renderable::getVerts()
{
	return verts;
}

void Renderable::setVerts(std::vector<Vec3f> newVerts)
{
	verts = newVerts;
}

std::vector<float> Renderable::getColours()
{
	std::vector<float> colours;

	for(unsigned int i = 0; i < verts.size(); i++)
	{
		colours.push_back(colour.x());
		colours.push_back(colour.y());
		colours.push_back(colour.z());
	}

	return colours;
}

Vec3f Renderable::getColour()
{
	return colour;
}

void Renderable::setColour(Vec3f newColour)
{
	colour = newColour;
}

void Renderable::setRenderMode(int newRenderMode)
{
	renderMode = newRenderMode;
}

int Renderable::getRenderMode()
{
	return renderMode;
}

void Renderable::setVertexShaderPath(std::string newVertexShaderPath)
{
	vertexShaderPath = newVertexShaderPath;
}

std::string Renderable::getVertexShaderPath()
{
	return vertexShaderPath;
}

void Renderable::setFragmentShaderPath(std::string newfragmentShaderPath)
{
	fragmentShaderPath = newfragmentShaderPath;
}

std::string Renderable::getFragmentShaderPath()
{
	return fragmentShaderPath;
}

void Renderable::generateIDs()
{
	std::string vsSource = loadShaderStringfromFile( vertexShaderPath );
	std::string fsSource = loadShaderStringfromFile( fragmentShaderPath );
	basicProgramID = CreateShaderProgram( vsSource, fsSource );

	// load IDs given from OpenGL
	glGenVertexArrays( 1, &vaoID );
	glGenBuffers( 1, &vertBufferID );
	glGenBuffers( 1, &colorBufferID );
}

void Renderable::deleteIDs()
{
	glDeleteProgram( basicProgramID );
	glDeleteVertexArrays( 1, &vaoID );
	glDeleteBuffers( 1, &vertBufferID );
	glDeleteBuffers( 1, &colorBufferID );
}

void Renderable::setVert(int index, Vec3f newVert)
{
	if(index >= verts.size())
	{
		std::cerr << "ERROR: Index " << index << " outside of range. Cannot update vertex.";
		return;
	}

	verts.at(index) = newVert;

}
