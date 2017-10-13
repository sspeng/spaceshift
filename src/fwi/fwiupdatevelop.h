/*
 * updatevelop.h
 *
 *  Created on: Mar 14, 2016
 *      Author: rice
 */

#ifndef SRC_FWI2D_UPDATEVELOP_H_
#define SRC_FWI2D_UPDATEVELOP_H_

#include "velocity.h"

class FwiUpdateVelOp {
public:
  FwiUpdateVelOp(float vmin, float vmax, float dx, float dt);
  void update(Velocity &newVel, const Velocity &vel, const std::vector<float> &grad, float steplen) const;

private:
  float vmin;
  float vmax;
};

#endif /* SRC_ESS_FWI2D_UPDATEVELOP_H_ */
