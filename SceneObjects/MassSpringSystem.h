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
			Oscillator = 0,
			Rope,
			Jello,
			Cloth
		};

	std::vector<int> indices =
		{
				//Front face
				4, 0, 1, 4, 1, 5,

				//Left face
				6, 2, 0, 6, 0, 4,

				//Back face
				7, 3, 2, 7, 2, 6,

				//Right face
				5, 1, 3, 5, 3, 7,

				//Top face
				0, 2, 3, 0, 3, 1,

				//Bot face
				4, 6, 7, 4, 7, 5
		};

	MassSpringSystem();
	MassSpringSystem(SpringSystem);
	virtual ~MassSpringSystem();
	void update();
	void add(PointMass, PointMass, Spring);
	void add(PointMass, Spring);
	void add(std::vector<PointMass>, std::vector<Spring>);
	static constexpr float DAMPING_FORCE_CONST = 5;
	static constexpr float DELTA_T = 0.001;
	static constexpr float G = -9.81;
	std::vector<PointMass> masses;
	void addForce(Vec3f);

	bool collide = false;

private:
	void createOscillator();
	void createRope();
	void createJello();
	void createCloth();


	std::vector<Spring> springs;
};

#endif /* SCENEOBJECTS_MASSSPRINGSYSTEM_H_ */
