/*
 * Control_System_types.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Control_System".
 *
 * Model version              : 1.119
 * Simulink Coder version : 9.0 (R2018b) 24-May-2018
 * C source code generated on : Tue Jun 28 17:20:04 2022
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Control_System_types_h_
#define RTW_HEADER_Control_System_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#ifndef typedef_dsp_private_SlidingWindowAver_T
#define typedef_dsp_private_SlidingWindowAver_T

typedef struct {
  int32_T isInitialized;
  boolean_T isSetupComplete;
  real_T pCumSum;
  real_T pCumSumRev[3];
  real_T pCumRevIndex;
} dsp_private_SlidingWindowAver_T;

#endif                                 /*typedef_dsp_private_SlidingWindowAver_T*/

#ifndef typedef_cell_wrap_Control_System_T
#define typedef_cell_wrap_Control_System_T

typedef struct {
  uint32_T f1[8];
} cell_wrap_Control_System_T;

#endif                                 /*typedef_cell_wrap_Control_System_T*/

#ifndef typedef_dsp_simulink_MovingAverage_Co_T
#define typedef_dsp_simulink_MovingAverage_Co_T

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  cell_wrap_Control_System_T inputVarSize;
  dsp_private_SlidingWindowAver_T *pStatistic;
  int32_T NumChannels;
} dsp_simulink_MovingAverage_Co_T;

#endif                                 /*typedef_dsp_simulink_MovingAverage_Co_T*/

/* Forward declaration for rtModel */
typedef struct tag_RTM_Control_System_T RT_MODEL_Control_System_T;

#endif                                 /* RTW_HEADER_Control_System_types_h_ */
