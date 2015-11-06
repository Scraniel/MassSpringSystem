/*
 * Spring.h
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#ifndef SCENEOBJECTS_SPRING_H_
#define SCENEOBJECTS_SPRING_H_

class Spring {
public:
	Spring();
	virtual ~Spring();

	int getEndPointIndex() const;
	void setEndPointIndex(int);
	float getK() const;
	void setK(float);
	float getRestLength() const;
	void setRestLength(float);
	int getStartPointIndex() const;
	void setStartPointIndex(int);

private:
	int startPointIndex, endPointIndex;
	float k, restLength;

};

#endif /* SCENEOBJECTS_SPRING_H_ */
