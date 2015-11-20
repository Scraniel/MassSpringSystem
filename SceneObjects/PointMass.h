/*
 * PointMass.h
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#ifndef SCENEOBJECTS_POINTMASS_H_
#define SCENEOBJECTS_POINTMASS_H_

#include "../MathTools/Vec3f.h"

class PointMass {
public:
	PointMass();
	virtual ~PointMass();
	PointMass(Vec3f, float);

	Vec3f getCurrentForces() const;
	void setCurrentForces(Vec3f);
	float getMass() const;
	void setMass(float);
	Vec3f getPosition() const;
	void setPosition(Vec3f);
	Vec3f getVelocity() const;
	void setVelocity(Vec3f);
	void addForce(Vec3f);
	void clearForces();
	void addVelocity(Vec3f);

	bool applyForces = true;
	bool noSprings = false;

private:
	Vec3f position, velocity, currentForces;
	float mass;


};

#endif /* SCENEOBJECTS_POINTMASS_H_ */
