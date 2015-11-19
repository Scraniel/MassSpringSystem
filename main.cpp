// CPSC 587 Created By: Andrew Owens
// This is a (very) basic program to
// 1) load shaders from external files, and make a shader program
// 2) make Vertex Array Object and Vertex Buffer Object for the triangle

// take a look at the following sites for further readings:
// opengl-tutorial.org -> The first triangle (New OpenGL, great start)
// antongerdelan.net -> shaders pipeline explained
// ogldev.atspace.co.uk -> good resource 


// NOTE: this dependencies (include/library files) will need to be tweaked if
// you wish to run this on a non lab computer

#include<iostream>
#include<cmath>

#include<GL/glew.h> // include BEFORE glut
#include<GL/glut.h>

#include "MathTools/Mat4f.h"
#include "MathTools/OpenGLMatrixTools.h"
#include "MathTools/Vec3f.h"
#include "MathTools/VectorTools.h"
#include "Helpers/FileHelper.h"
#include "OpenGLTools/ShaderTools.h"
#include "OpenGLTools/RenderingTools.h"
#include "SceneObjects/Renderable.h"
#include "SceneObjects/MassSpringSystem.h"


using std::cout;
using std::endl;
using std::cerr;

MassSpringSystem systems [] = {MassSpringSystem(MassSpringSystem::Oscillator), MassSpringSystem(MassSpringSystem::Rope), MassSpringSystem(MassSpringSystem::Jello), MassSpringSystem(MassSpringSystem::Cloth)};
int currentlyRendering;
Mat4f MVP;
Mat4f M; // Every model should have it's own Model matrix
Mat4f V;
Mat4f P;

float thetaYRotate = 0;
float thetaXRotate = 0;
float mousePreviousX = 0;
float mousePreviousY = 0;

std::vector<Renderable*> toRender = {};

int WIN_WIDTH = 800, WIN_HEIGHT = 600;

// function declarations... just to keep things kinda organized.
void displayFunc();
void resizeFunc(int, int);
void idleFunc();
void init();
void loadProjectionMatrix();
void loadModelViewMatrix();
void setupModelViewProjectionTransform();
int main( int, char** );
void mouseButtonFunc( int button, int state, int x, int y);
void reloadMVPUniformAllObjects();
// function declarations

void mouseButtonFunc( int button, int state, int x, int y)
{
	if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		systems[currentlyRendering].addForce(Vec3f(0.,10000., 0.));
}


void mouseMotionFunc(int x, int y)
{

	thetaYRotate = ((x - mousePreviousX) / WIN_WIDTH) * 360; // May not be necessary
	mousePreviousY = y;
	mousePreviousX = x;

	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);
		current.M = current.M * RotateAboutYMatrix( thetaYRotate );
	}

	setupModelViewProjectionTransform();
	reloadMVPUniformAllObjects();
}

// Reloads the MVP uniform for all objects in the scene, and calls for a redisplay
void reloadMVPUniformAllObjects()
{
	setupModelViewProjectionTransform();
	// For every object, setup VAO's / buffers
	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);
		RenderingTools::reloadMVPUniform(current);
	}
	glutPostRedisplay();
}

void displayFunc()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// Render all objects!
	//TODO: move this into renderable as an overridable
	//		function so each child can render differently
	//		(eg. pass in additional buffers)
	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);
		RenderingTools::loadBuffer(current);

		// Use our shader
		glUseProgram( current.basicProgramID );

		// Use VAO that holds buffer bindings
		// and attribute config of buffers
		glBindVertexArray( current.vaoID );

		if(current.useIndexBuffer)
		{

			glDrawElements(GL_TRIANGLES,               // mode
			                 current.getIndices().size(), // count
			                 GL_UNSIGNED_INT,            // type
			                 (void *)0                   // element array buffer offset
			                 );
		}
		else
		{
			glDrawArrays( current.getRenderMode(), 0, current.getVerts().size() );
		}

	}

	glutSwapBuffers();
}


void idleFunc()
{
	for(int i = 0 ; i < 10; i++)
		systems[currentlyRendering].update();

	//M = M * RotateAboutYMatrix(1);
	setupModelViewProjectionTransform();
    reloadMVPUniformAllObjects();

	glutPostRedisplay();
}

void resizeFunc( int width, int height )
{
    WIN_WIDTH = width;
    WIN_HEIGHT = height;

    glViewport( 0, 0, width, height );

    loadProjectionMatrix();
    reloadMVPUniformAllObjects();

    glutPostRedisplay();
}


void loadProjectionMatrix()
{
    // Perspective Only

	// field of view angle 60 degrees
	// window aspect ratio
	// near Z plane > 0
	// far Z plane

    P = PerspectiveProjection(  60, // FOV
                                static_cast<float>(WIN_WIDTH)/WIN_HEIGHT, // Aspect
                                0.01,   // near plane
                                5 ); // far plane depth
}

void loadModelViewMatrix()
{
    M = UniformScaleMatrix( 0.25 );	// scale Scene First

    M = TranslateMatrix( 0, 0, -2 ) * M;	// translate away from (0,0,0)

    // view doesn't change, but if it did you would use this
    V = IdentityMatrix();
}

void setupModelViewProjectionTransform()
{
	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);

		current.MVP = P * V * M * current.M;
	}
}

void init()
{
	glEnable( GL_DEPTH_TEST );

	currentlyRendering = MassSpringSystem::Cloth;
	toRender.push_back(&systems[currentlyRendering]);

	// SETUP SHADERS, BUFFERS, VAOs

    loadModelViewMatrix();
    loadProjectionMatrix();
	setupModelViewProjectionTransform();

	// For every object, setup VAO's / buffers
	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);
		current.generateIDs();
		RenderingTools::setupVAO(current);
		RenderingTools::loadBuffer(current);
		if(current.useIndexBuffer)
			RenderingTools::loadIndexBuffer(current);
		RenderingTools::reloadMVPUniform(current);
	}


}

void menu(int choice)
{
	switch(choice)
	{
	case MassSpringSystem::Oscillator:
		currentlyRendering = MassSpringSystem::Oscillator;
		break;
	case 1:
		currentlyRendering = MassSpringSystem::Oscillator;
		break;
	}

}


int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	// Setup FB configuration
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
	glutInitWindowPosition( 0, 0 );

	glutCreateWindow( "Tut08" );

	glewExperimental=true; // Needed in Core Profile
	// Comment out if you want to us glBeign etc...
	if( glewInit() != GLEW_OK )
	{
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	cout << "GL Version: :" << glGetString(GL_VERSION) << endl;

    glutDisplayFunc( displayFunc );
	glutReshapeFunc( resizeFunc );
    glutIdleFunc( idleFunc );
    glutMouseFunc( mouseButtonFunc );
    glutMotionFunc( mouseMotionFunc);

    glutCreateMenu(menu);
    glutAddMenuEntry("Render Oscillator",0);
    glutAddMenuEntry("Render Rope",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	init(); // our own initialize stuff func

	glutMainLoop();

	// clean up after loop
	for(unsigned int i = 0; i < toRender.size(); i++)
	{
		Renderable & current = *toRender.at(i);

		current.deleteIDs();
	}

	return 0;
}
