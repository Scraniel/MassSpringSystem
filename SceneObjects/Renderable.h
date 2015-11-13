/*
 * GLCurve.h
 *
 *  Created on: Sep 23, 2015
 *      Author: dglewis
 */

#ifndef GLCURVE_H_
#define GLCURVE_H_

#include <GL/glew.h> // include BEFORE glut
#include <GL/glut.h>
#include "../MathTools/Mat4f.h"
#include "../MathTools/OpenGLMatrixTools.h"
#include "../OpenGLTools/ShaderTools.h"
#include "../MathTools/Vec3f.h"

class Renderable {
public:
	Renderable();
	virtual ~Renderable();
	std::vector<Vec3f> getVerts();
	virtual void setVerts(std::vector<Vec3f>);
	void setVert(int, Vec3f);
	virtual std::vector<float> getColours();
	Vec3f getColour();
	void setColour(Vec3f);
	int getRenderMode();
	void setRenderMode(int);
	std::string getVertexShaderPath();
	std::string getFragmentShaderPath();
	void setVertexShaderPath(std::string);
	void setFragmentShaderPath(std::string);


	// Will most likely move to private, just for incremental testing
		GLuint vaoID;
		GLuint basicProgramID;

		// Could store these two in an array GLuint[]
		GLuint vertBufferID;
		GLuint colorBufferID;

		Mat4f M, MVP;

		void generateIDs();
		void deleteIDs();
private:
	// The vertices that make up the curve
	std::vector<Vec3f> verts;
	// The colour to draw the curve as
	Vec3f colour;
	int renderMode; // GLEnum that tells the GPU how to render this object, eg. GL_LINES
	std::string fragmentShaderPath, vertexShaderPath;


};

#endif /* GLCURVE_H_ */
