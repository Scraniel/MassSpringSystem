/*
 * PointMass.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#include "PointMass.h"

PointMass::PointMass() {
	// TODO Auto-generated constructor stub

}

PointMass::~PointMass() {
	// TODO Auto-generated destructor stub
}

PointMass::PointMass(Vec3f position, float mass)
{
	this->position = position;
	this->mass = mass;
}

Vec3f PointMass::getCurrentForces() const {
		return currentForces;
	}

void PointMass::setCurrentForces(Vec3f currentForces) {
	this->currentForces = currentForces;
}

float PointMass::getMass() const {
	return mass;
}

void PointMass::setMass(float mass) {
	this->mass = mass;
}

Vec3f PointMass::getPosition() const {
	return position;
}

void PointMass::setPosition(Vec3f position) {
	this->position = position;
}

Vec3f PointMass::getVelocity() const {
	return velocity;
}

void PointMass::setVelocity(Vec3f velocity) {
	this->velocity = velocity;
}

void PointMass::addForce(Vec3f force)
{
	this->currentForces = this->currentForces + force;
}

void PointMass::clearForces()
{
	this->currentForces.x(0);
	this->currentForces.y(0);
	this->currentForces.z(0);
}

void PointMass::addVelocity(Vec3f velocity)
{
	this->velocity = this->velocity + velocity;
}
