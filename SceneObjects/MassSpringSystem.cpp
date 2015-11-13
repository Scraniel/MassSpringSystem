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
	this->setRenderMode(GL_LINE_STRIP);
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

		//springForce = springForce - ((n * ((startPoint.getVelocity() * n) - (endPoint.getVelocity() * n))) * MassSpringSystem::DAMPING_FORCE_CONST);



		startPoint.addForce(springForce * (-1.0));
		endPoint.addForce(springForce );


	}

	for(int i = 0; i < masses.size(); i++)
	{
		PointMass &current = masses[i];
		if( i > 0){

			current.addForce(Vec3f(0, MassSpringSystem::G, 0) * current.getMass()); // THIS IS WHERE YOU ADD GLOBAL FORCES

			// Integration scheme (should wrap in a function pointer or something; right now only does semi-implicit Euler)
			current.addVelocity((current.getCurrentForces() * (1.0 / current.getMass())) * MassSpringSystem::DELTA_T);
			current.setPosition(current.getPosition() + (current.getVelocity() * MassSpringSystem::DELTA_T));
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

void MassSpringSystem::createOscillator()
{
	PointMass first, second;
	first.setMass(1);
	first.setPosition(Vec3f(0,0,0));
	second.setMass(1);
	second.setPosition(Vec3f(0,-1,0));

	Spring spring;
	spring.setK(20);
	spring.setRestLength(1);

	add(first, second, spring);
}

void MassSpringSystem::createRope()
{
	PointMass first, second;
	first.setMass(1);
	first.setPosition(Vec3f(0,0,0));
	second.setMass(1);
	second.setPosition(Vec3f(0.1,0,0));

	Spring spring;
	spring.setK(200);
	spring.setRestLength(1);

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

}

void MassSpringSystem::createCloth()
{

}
