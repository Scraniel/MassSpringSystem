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
	enum SpringSystem
		{
			Oscillator,
			Rope,
			Jello,
			Cloth
		};

	MassSpringSystem();
	MassSpringSystem(SpringSystem);
	virtual ~MassSpringSystem();
	void update();
	void add(PointMass, PointMass, Spring);
	void add(PointMass, Spring);
	static constexpr float DAMPING_FORCE_CONST = 5;
	static constexpr float DELTA_T = 0.001;
	static constexpr float G = -9.81;
	std::vector<PointMass> masses;
private:
	void createOscillator();
	void createRope();
	void createJello();
	void createCloth();


	std::vector<Spring> springs;
};

#endif /* SCENEOBJECTS_MASSSPRINGSYSTEM_H_ */
