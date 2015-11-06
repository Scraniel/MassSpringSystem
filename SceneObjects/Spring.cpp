/*
 * Spring.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: dglewis
 */

#include "Spring.h"

Spring::Spring() {
	// TODO Auto-generated constructor stub

}

Spring::~Spring() {
	// TODO Auto-generated destructor stub
}

int Spring::getEndPointIndex() const {
	return endPointIndex;
}

void Spring::setEndPointIndex(int endPointIndex) {
	this->endPointIndex = endPointIndex;
}

float Spring::getK() const {
	return k;
}

void Spring::setK(float k) {
	this->k = k;
}

float Spring::getRestLength() const {
	return restLength;
}

void Spring::setRestLength(float restLength) {
	this->restLength = restLength;
}

int Spring::getStartPointIndex() const {
	return startPointIndex;
}

void Spring::setStartPointIndex(int startPointIndex) {
	this->startPointIndex = startPointIndex;
}
