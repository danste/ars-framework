/*
  Copyright 2002 Colin Percival and the University of Oxford

  Copyright in this software is held by the University of Oxford
  and Colin Percival. Both the University of Oxford and Colin Percival
  have agreed that this software may be distributed free of charge and
  in accordance with the attached licence.

  The above notwithstanding, in order that the University as a
  charitable foundation protects its assets for the benefit of its
  educational and research purposes, the University makes clear that no
  condition is made or to be implied, nor is any warranty given or to
  be implied, as to the proper functioning of this work, or that it
  will be suitable for any particular purpose or for use under any
  specific conditions.  Furthermore, the University disclaims all
  responsibility for any use which is made of this work.

  For the terms under which this work may be distributed, please see
  the adjoining file "LICENSE".
*/
/*
  $Id: smpa.h,v 1.5 2002/04/25 06:13:52 cperciva Exp $
*/

#include "ptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void smpa_init(double * LUT,uint32 len);
extern void smpa_in(uint8 * A,double * A1,uint32 len);
extern void smpa_add(double * A1,double * B1,double * C1,uint32 len);
extern void smpa_sub(double * A1,double * B1,double * C1,uint32 len);
extern void smpa_premul(double * A1,uint32 len,double * LUT);
extern void smpa_mul(double * A1,double * B1,double * C1,
                                    uint32 len,double * LUT);
extern void smpa_prediv(double * M1,double * M2,double * K1,
                        double * T1,uint32 len,double * LUT);
extern void smpa_mulmod(double * A1,double * B1,double * C1,
                        double * M1,double * M2,double * K1,
                        double * T1,uint32 len,double * LUT);
extern void smpa_div(double * A1,double * Q1,double * R1,
                        double * M1,double * M2,double * K1,
                        double * T1,uint32 len,double * LUT);
extern void smpa_gcd(double * X1,double * Y1,
                        double * G1,double * A1,double *B1,
                        double * T1,uint32 len);
extern void smpa_out(double * A1,uint8 * A,uint32 len);
extern void smpa_doubleout(double * A1,uint8 * A,uint32 len);

#ifdef __cplusplus
}
#endif


