/*
 * Atomic_data.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Atomic".
 *
 * Model version              : 4.4
 * Simulink Coder version : 9.5 (R2021a) 14-Nov-2020
 * C source code generated on : Wed Jun 16 17:30:13 2021
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Atomic.h"
#include "Atomic_private.h"

/* Block parameters (default storage) */
P_Atomic_T Atomic_P = {
  /* Mask Parameter: PIDController_I
   * Referenced by: '<S31>/Integral Gain'
   */
  -10.0,

  /* Mask Parameter: PIDController_InitialConditionF
   * Referenced by: '<S34>/Integrator'
   */
  0.0,

  /* Mask Parameter: PIDController_P
   * Referenced by: '<S39>/Proportional Gain'
   */
  -20.0
};
