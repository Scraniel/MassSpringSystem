/*
 * MassSpringSystem.h
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#ifndef SCENEOBJECTS_MASSSPRINGSYSTEM_H_
#define SCENEOBJECTS_MASSSPRINGSYSTEM_H_

#include "PointMass.h"
#include "Spring.h"
#include "Renderable.h"

class MassSpringSystem : public Renderable {
public:
	MassSpringSystem();
	virtual ~MassSpringSystem();

private:
	std::vector<PointMass> masses;
	std::vector<Spring> springs;
};

#endif /* SCENEOBJECTS_MASSSPRINGSYSTEM_H_ */
