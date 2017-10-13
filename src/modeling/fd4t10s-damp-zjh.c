/*
 * fd4t10s-damp-zjh.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: rice
 */

#include <stdio.h>
#include "fd4t10s-damp-zjh.h"

/**
 * please note that the velocity is transformed
 */
void fd4t10s_damp_zjh_2d_vtrans(float *prev_wave, const float *curr_wave, const float *vel, float *u2, int nx, int nz, int nb, int freeSurface) {
  float a[6];

  const int d = 6;
  const int bz = nb;
  const int bx = nb;
  const float max_delta = 0.05;
  int ix, iz;

  /// Zhang, Jinhai's method
  a[0] = +1.53400796;
  a[1] = +1.78858721;
  a[2] = -0.31660756;
  a[3] = +0.07612173;
  a[4] = -0.01626042;
  a[5] = +0.00216736;

  //printf("fm 1\n");
#ifdef USE_OPENMP
  #pragma omp parallel for default(shared) private(ix, iz)
#endif


#ifdef USE_SWACC
/*#pragma acc parallel loop copyin(nx,nz,d) annotate(readonly=(nx,nz,d))*/
#pragma acc parallel loop
#endif
  for (ix = d - 1; ix < nx - (d - 1); ix++) {
    for (iz = d - 1; iz < nz - (d - 1); iz++) {
      int curPos = ix * nz + iz;
      u2[curPos] = -4.0 * a[0] * curr_wave[curPos] +
                   a[1] * (curr_wave[curPos - 1]  +  curr_wave[curPos + 1]  +
                           curr_wave[curPos - nz]  +  curr_wave[curPos + nz])  +
                   a[2] * (curr_wave[curPos - 2]  +  curr_wave[curPos + 2]  +
                           curr_wave[curPos - 2 * nz]  +  curr_wave[curPos + 2 * nz])  +
                   a[3] * (curr_wave[curPos - 3]  +  curr_wave[curPos + 3]  +
                           curr_wave[curPos - 3 * nz]  +  curr_wave[curPos + 3 * nz])  +
                   a[4] * (curr_wave[curPos - 4]  +  curr_wave[curPos + 4]  +
                           curr_wave[curPos - 4 * nz]  +  curr_wave[curPos + 4 * nz])  +
                   a[5] * (curr_wave[curPos - 5]  +  curr_wave[curPos + 5]  +
                           curr_wave[curPos - 5 * nz]  +  curr_wave[curPos + 5 * nz]);

    }
  }

  //printf("fm 2\n");
#ifdef USE_OPENMP
  #pragma omp parallel for default(shared) private(ix, iz)
#endif
  for (ix = d; ix < nx - d; ix++) { /// the range of ix is different from that in previous for loop
    for (iz = d; iz < nz - d; iz++) { /// be careful of the range of iz
      //printf("check 1\n");
      float delta;
      float dist = 0;
			if(freeSurface) {
				if (ix >= bx && ix < nx - bx &&
						iz < nz - bz) {
					dist = 0;
				}
			}
			else {
				if (ix >= bx && ix < nx - bx &&
						iz >= bz && iz < nz - bz) {
					dist = 0;
				}
				if (iz < bz) {
					dist = (float)(bz - iz) / bz;
				}
			}
      if (ix < bx) {
        dist = (float)(bx - ix) / bx;
      }
      if (ix >= nx - bx) {
        dist = (float)(ix - (nx - bx) + 1) / bx;
      }
      if (iz >= nz - bz) {
        dist = (float)(iz - (nz - bz) + 1) / bz;
      }

      delta = max_delta * dist * dist;

      int curPos = ix * nz + iz;
      float curvel = vel[curPos];

      prev_wave[curPos] = (2. - 2 * delta + delta * delta) * curr_wave[curPos] - (1 - 2 * delta) * prev_wave[curPos]  +
                          (1.0f / curvel) * u2[curPos] + /// 2nd order
                          1.0f / 12 * (1.0f / curvel) * (1.0f / curvel) *
                          (u2[curPos - 1] + u2[curPos + 1] + u2[curPos - nz] + u2[curPos + nz] - 4 * u2[curPos]); /// 4th order
    }
  }
  //printf("fm 3\n");

}

void fd4t10s_damp_zjh_2d_vtrans_test(float *prev_wave, const float *curr_wave, const float *vel, float *u2, int nx, int nz, int nb, int freeSurface) {
  float a[6];

  const int d = 6;
  const int bz = nb;
  const int bx = nb;
  const float max_delta = 0.05;
  int ix, iz;

  /// Zhang, Jinhai's method
  a[0] = +1.53400796;
  a[1] = +1.78858721;
  a[2] = -0.31660756;
  a[3] = +0.07612173;
  a[4] = -0.01626042;
  a[5] = +0.00216736;

  for (ix = d - 1; ix < nx - (d - 1); ix++) {
    for (iz = d - 1; iz < nz - (d - 1); iz++) {
      int curPos = ix * nz + iz;
      u2[curPos] = -4.0 * a[0] * curr_wave[curPos] +
                   a[1] * (curr_wave[curPos - 1]  +  curr_wave[curPos + 1]  +
                           curr_wave[curPos - nz]  +  curr_wave[curPos + nz])  +
                   a[2] * (curr_wave[curPos - 2]  +  curr_wave[curPos + 2]  +
                           curr_wave[curPos - 2 * nz]  +  curr_wave[curPos + 2 * nz])  +
                   a[3] * (curr_wave[curPos - 3]  +  curr_wave[curPos + 3]  +
                           curr_wave[curPos - 3 * nz]  +  curr_wave[curPos + 3 * nz])  +
                   a[4] * (curr_wave[curPos - 4]  +  curr_wave[curPos + 4]  +
                           curr_wave[curPos - 4 * nz]  +  curr_wave[curPos + 4 * nz])  +
                   a[5] * (curr_wave[curPos - 5]  +  curr_wave[curPos + 5]  +
                           curr_wave[curPos - 5 * nz]  +  curr_wave[curPos + 5 * nz]);

    }
  }

  for (ix = d ; ix < nx - d; ix++) {
    for (iz = d ; iz < nz - d; iz++) {
      int curPos = ix * nz + iz;
      float curvel = vel[curPos];

      prev_wave[curPos] = 2. * curr_wave[curPos] - prev_wave[curPos] +
                          (1.0f / curvel) * u2[curPos] + /// 2nd order
                          1.0f / 12 * (1.0f / curvel) * (1.0f / curvel) *
                          (u2[curPos - 1] + u2[curPos + 1] + u2[curPos - nz] + u2[curPos + nz] - 4 * u2[curPos]); /// 4th order
		}
	}
	/*
  //printf("fm 1\n");
#ifdef USE_OPENMP
  #pragma omp parallel for default(shared) private(ix, iz)
#endif


#ifdef USE_SWACC
//#pragma acc parallel loop copyin(nx,nz,d) annotate(readonly=(nx,nz,d))
#pragma acc parallel loop
#endif
  for (ix = d - 1; ix < nx - (d - 1); ix++) {
    for (iz = d - 1; iz < nz - (d - 1); iz++) {
      int curPos = ix * nz + iz;
      u2[curPos] = -4.0 * a[0] * curr_wave[curPos] +
                   a[1] * (curr_wave[curPos - 1]  +  curr_wave[curPos + 1]  +
                           curr_wave[curPos - nz]  +  curr_wave[curPos + nz])  +
                   a[2] * (curr_wave[curPos - 2]  +  curr_wave[curPos + 2]  +
                           curr_wave[curPos - 2 * nz]  +  curr_wave[curPos + 2 * nz])  +
                   a[3] * (curr_wave[curPos - 3]  +  curr_wave[curPos + 3]  +
                           curr_wave[curPos - 3 * nz]  +  curr_wave[curPos + 3 * nz])  +
                   a[4] * (curr_wave[curPos - 4]  +  curr_wave[curPos + 4]  +
                           curr_wave[curPos - 4 * nz]  +  curr_wave[curPos + 4 * nz])  +
                   a[5] * (curr_wave[curPos - 5]  +  curr_wave[curPos + 5]  +
                           curr_wave[curPos - 5 * nz]  +  curr_wave[curPos + 5 * nz]);

    }
  }

  //printf("fm 2\n");
#ifdef USE_OPENMP
  #pragma omp parallel for default(shared) private(ix, iz)
#endif
  for (ix = d; ix < nx - d; ix++) { /// the range of ix is different from that in previous for loop
    for (iz = d; iz < nz - d; iz++) { /// be careful of the range of iz
      int curPos = ix * nz + iz;

      prev_wave[curPos] = u2[curPos]; /// 2nd order
    }
  }
  //printf("fm 3\n");
	//
	*/
}
