/*
 * MassSpringSystem.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#include "MassSpringSystem.h"

MassSpringSystem::MassSpringSystem() {
	// TODO Auto-generated constructor stub

}

MassSpringSystem::~MassSpringSystem() {
	// TODO Auto-generated destructor stub
}

MassSpringSystem::MassSpringSystem(SpringSystem type)
{

	this->setFragmentShaderPath("Shaders/basic_fs.glsl");
	this->setVertexShaderPath("Shaders/basic_vs.glsl");
	this->setColour(Vec3f(1,1,1));

	switch(type)
	{
	case Oscillator:
		createOscillator();
		break;
	case Rope:
		createRope();
		break;
	case Jello:
		createJello();
		break;
	case Cloth:
		createCloth();
		break;
	}
}

void MassSpringSystem::update()
{
	// Accumulating spring forces
	for(Spring current : springs)
	{
		PointMass & startPoint = masses[current.getStartPointIndex()];
		PointMass & endPoint = masses[current.getEndPointIndex()];
		Vec3f startToEnd = endPoint.getPosition() - startPoint.getPosition();
		Vec3f n = startToEnd.normalized();

		Vec3f springForce = n * ((-current.getK()) * (startToEnd.length() - current.getRestLength()));

		springForce = springForce + ((n * ((startPoint.getVelocity() * n) - (endPoint.getVelocity() * n))) * MassSpringSystem::DAMPING_FORCE_CONST);

		startPoint.addForce(springForce * (-1.0));
		endPoint.addForce(springForce );


	}

	for(int i = 0; i < masses.size(); i++)
	{
		PointMass &current = masses[i];
		if( current.applyForces){

			current.addForce(Vec3f(0, MassSpringSystem::G, 0) * current.getMass()); // THIS IS WHERE YOU ADD GLOBAL FORCES

			// Integration scheme (should wrap in a function pointer or something; right now only does semi-implicit Euler)
			if(current.getPosition().y() > -3 || !this->collide) // TODO: create a wall or something, don't use this magic number
			{
				current.addVelocity((current.getCurrentForces() * (1.0 / current.getMass())) * MassSpringSystem::DELTA_T);
				current.setPosition(current.getPosition() + (current.getVelocity() * MassSpringSystem::DELTA_T));
			}

			setVert(i, current.getPosition());
		}


		current.clearForces();

	}

}

void MassSpringSystem::add(PointMass start, PointMass end, Spring spring)
{
	int index = this->masses.size();
	this->masses.push_back(start);
	this->masses.push_back(end);
	spring.setStartPointIndex(index);
	spring.setEndPointIndex(index+1);
	this->springs.push_back(spring);

	std::vector<Vec3f> newVerts = this->getVerts();
	newVerts.push_back(start.getPosition());
	newVerts.push_back(end.getPosition());

	this->setVerts(newVerts);
}


// assumes this is being added on to the end, ie. newMass is the end and the last added
// mass is the start
void MassSpringSystem::add(PointMass newMass, Spring spring)
{
	int index = this->masses.size();
	this->masses.push_back(newMass);
	spring.setStartPointIndex(index -1);
	spring.setEndPointIndex(index);
	this->springs.push_back(spring);

	std::vector<Vec3f> newVerts = this->getVerts();
	newVerts.push_back(newMass.getPosition());

	this->setVerts(newVerts);
}

// Replaces the current list of points / springs with these
void MassSpringSystem::add(std::vector<PointMass> points, std::vector<Spring> springs)
{
	this->masses = points;
	this->springs = springs;

	std::vector<Vec3f> newPoints;
	for(PointMass mass : masses)
	{
		newPoints.push_back(mass.getPosition());
	}

	this->setVerts(newPoints);
}



void MassSpringSystem::createOscillator()
{
	this->setRenderMode(GL_LINE_STRIP);

	PointMass first, second;
	first.setMass(1);
	first.setPosition(Vec3f(0,0,0));
	first.applyForces = false;
	second.setMass(1);
	second.setPosition(Vec3f(0,-0.25,0));

	Spring spring;
	spring.setK(20);
	spring.setRestLength(1);

	add(first, second, spring);
}

void MassSpringSystem::createRope()
{
	this->setRenderMode(GL_LINE_STRIP);

	PointMass first, second;
	first.setMass(1);
	first.setPosition(Vec3f(0,0,0));
	first.applyForces = false;
	second.setMass(1);
	second.setPosition(Vec3f(0.1,0,0));

	Spring spring;
	spring.setK(200);
	spring.setRestLength(0.1);

	add(first, second, spring);

	for(int i = 0; i < 10; i++)
	{
		PointMass currentMass;
		currentMass.setMass(1);
		currentMass.setPosition(Vec3f(0.2 + i*0.1, 0, 0));

		add(currentMass, spring);
	}
}

void MassSpringSystem::createJello()
{
	this->useIndexBuffer = true;
	this->useGeometryShader = true;
	this->collide = true;
	this->setRenderMode(GL_TRIANGLES);
	this->setFragmentShaderPath("Shaders/phong_fs.glsl");
	this->setVertexShaderPath("Shaders/phong_vs.glsl");
	this->setGeometryShaderPath("Shaders/phong_gs.glsl");

	PointMass topFrontLeft(Vec3f(-1,1,1), 1), topFrontRight(Vec3f(1,1,1), 1), topBackLeft(Vec3f(-1,1,-1), 1), topBackRight(Vec3f(1,1,-1), 1);
	PointMass botFrontLeft(Vec3f(-1,-1,1), 1), botFrontRight(Vec3f(1,-1,1), 1), botBackLeft(Vec3f(-1,-1,-1), 1), botBackRight(Vec3f(1,-1,-1), 1);

	std::vector<PointMass> pointList = {topFrontLeft, // 0
										topFrontRight,// 1
										topBackLeft,  // 2
										topBackRight, // 3
										botFrontLeft, // 4
										botFrontRight,// 5
										botBackLeft,  // 6
										botBackRight};// 7
	std::vector<Spring> springList;

	for(int i = 0; i < pointList.size(); i++)
	{
		for(int j = i + 1; j < pointList.size(); j++)
		{
			Spring newSpring;
			newSpring.setK(100);
			newSpring.setEndPointIndex(j);
			newSpring.setStartPointIndex(i);
			newSpring.setRestLength((pointList[i].getPosition() - pointList[j].getPosition()).length());

			springList.push_back(newSpring);
		}
	}

	add(pointList, springList);

	this->setIndices(indices);

}

int calcOffset(int col, int row, int width)
{
	return row * width + col;
}

void MassSpringSystem::createCloth()
{
	this->useIndexBuffer = true;
	this->useGeometryShader = true;

	this->setRenderMode(GL_TRIANGLES);
	this->setFragmentShaderPath("Shaders/phong_fs.glsl");
	this->setVertexShaderPath("Shaders/phong_vs.glsl");
	this->setGeometryShaderPath("Shaders/phong_gs.glsl");
	int width = 30;
	int height = 30;
	float maxDist = .5/3.;

	PointMass topLeft(Vec3f(-1, 1, 1), 1), topRight(Vec3f(1,1,0), 1), botLeft(Vec3f(-1,-1, 0), 1);//, botRight(Vec3f(1,-1,0), 1);
	topLeft.applyForces = false;

	std::vector<PointMass> pointList;
	std::vector<Spring> springList;
	std::vector<int> indices;

	Vec3f topLeftToRight = topRight.getPosition() - topLeft.getPosition();
	Vec3f topLeftToBotLeft = botLeft.getPosition() - topLeft.getPosition();
	float widthFactor = 1./width;
	float heightFactor = 1./height;


	// Grid of points
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(j == 0 && i == 0)
				pointList.push_back(topLeft);
			else
			{
				Vec3f position = topLeft.getPosition() + (topLeftToRight) * (j * widthFactor) + (topLeftToBotLeft) * (i * heightFactor);
				PointMass newPoint(position, 1);
				if((i == 0 && j%3 == 0) || (j == width - 1 && i == 0))
					newPoint.applyForces = false;
				pointList.push_back(newPoint);
			}
		}
	}

	// springs between them
	for(int i = 0; i < pointList.size(); i++)
	{
		for(int j = i; j < pointList.size(); j++)
		{
			float distance = (pointList[i].getPosition() - pointList[j].getPosition()).length();

			if(distance < maxDist)
			{
				Spring newSpring;
				newSpring.setK(2000);
				newSpring.setEndPointIndex(j);
				newSpring.setStartPointIndex(i);
				newSpring.setRestLength(distance);

				springList.push_back(newSpring);
			}

		}
	}

	// indices for drawing triangles

	for(int i = 0; i < height - 1; i++)
	{
		for(int j = 0; j < width - 1; j++)
		{
			// 6 points for 2 triangles = a square!
			indices.push_back(calcOffset(j,i,width));
			indices.push_back(calcOffset(j+1, i+1, width));
			indices.push_back(calcOffset(j, i+1, width));

			indices.push_back(calcOffset(j, i, width));
			indices.push_back(calcOffset(j+1, i, width));
			indices.push_back(calcOffset(j+1, i+1, width));
		}

	}

	this->setIndices(indices);
	add(pointList, springList);

}

void MassSpringSystem::addForce(Vec3f force)
{
	for(int i = 0; i < masses.size(); i++)
	{
		masses[i].addForce(force);
	}


}
