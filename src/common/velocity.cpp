/*
 * velocity.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: rice
 */

#include "velocity.h"
#include "logger.h"

Velocity::Velocity() {
}

Velocity::Velocity(int _nx, int _nz) : dat(_nx *_nz, 0), nx(_nx), nz(_nz) {
}

Velocity::Velocity(const std::vector<float>& _dat, int _nx, int _nz) :
  dat(_dat), nx(_nx), nz(_nz)
{
}

void Velocity::resize(int _nx, int _nz) {
	nx = _nx;
	nz = _nz;
	dat.resize(nx, nz);
	dat.assign(nx * nz, 0.0f);
}
