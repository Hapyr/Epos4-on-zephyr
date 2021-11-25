/*
 * assdf.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "assdf".
 *
 * Model version              : 1.1
 * Simulink Coder version : 9.5 (R2021a) 14-Nov-2020
 * C source code generated on : Wed May 19 11:33:56 2021
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "assdf.h"
#include "assdf_private.h"

/* Continuous states */
X_assdf_T assdf_X;

/* External inputs (root inport signals with default storage) */
ExtU_assdf_T assdf_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_assdf_T assdf_Y;

/* Real-time model */
static RT_MODEL_assdf_T assdf_M_;
RT_MODEL_assdf_T *const assdf_M = &assdf_M_;

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  assdf_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  assdf_step();
  assdf_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  assdf_step();
  assdf_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  assdf_step();
  assdf_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void assdf_step(void)
{
  if (rtmIsMajorTimeStep(assdf_M)) {
    /* set solver stop time */
    if (!(assdf_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&assdf_M->solverInfo, ((assdf_M->Timing.clockTickH0
        + 1) * assdf_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&assdf_M->solverInfo, ((assdf_M->Timing.clockTick0 +
        1) * assdf_M->Timing.stepSize0 + assdf_M->Timing.clockTickH0 *
        assdf_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(assdf_M)) {
    assdf_M->Timing.t[0] = rtsiGetT(&assdf_M->solverInfo);
  }

  /* Outputs for Atomic SubSystem: '<Root>/assdf' */
  /* Outport: '<Root>/Out1' incorporates:
   *  Gain: '<S1>/Gain'
   *  Integrator: '<S1>/Integrator'
   */
  assdf_Y.Out1 = assdf_P.Gain_Gain * assdf_X.Integrator_CSTATE;

  /* End of Outputs for SubSystem: '<Root>/assdf' */
  if (rtmIsMajorTimeStep(assdf_M)) {
    rt_ertODEUpdateContinuousStates(&assdf_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++assdf_M->Timing.clockTick0)) {
      ++assdf_M->Timing.clockTickH0;
    }

    assdf_M->Timing.t[0] = rtsiGetSolverStopTime(&assdf_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      assdf_M->Timing.clockTick1++;
      if (!assdf_M->Timing.clockTick1) {
        assdf_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void assdf_derivatives(void)
{
  XDot_assdf_T *_rtXdot;
  _rtXdot = ((XDot_assdf_T *) assdf_M->derivs);

  /* Derivatives for Atomic SubSystem: '<Root>/assdf' */
  /* Derivatives for Integrator: '<S1>/Integrator' incorporates:
   *  Inport: '<Root>/In1'
   */
  _rtXdot->Integrator_CSTATE = assdf_U.In1;

  /* End of Derivatives for SubSystem: '<Root>/assdf' */
}

/* Model initialize function */
void assdf_initialize(void)
{
  /* Registration code */ 

  /* initialize real-time model */
  (void) memset((void *)assdf_M, 0,
                sizeof(RT_MODEL_assdf_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&assdf_M->solverInfo, &assdf_M->Timing.simTimeStep);
    rtsiSetTPtr(&assdf_M->solverInfo, &rtmGetTPtr(assdf_M));
    rtsiSetStepSizePtr(&assdf_M->solverInfo, &assdf_M->Timing.stepSize0);
    rtsiSetdXPtr(&assdf_M->solverInfo, &assdf_M->derivs);
    rtsiSetContStatesPtr(&assdf_M->solverInfo, (real_T **) &assdf_M->contStates);
    rtsiSetNumContStatesPtr(&assdf_M->solverInfo, &assdf_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&assdf_M->solverInfo,
      &assdf_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&assdf_M->solverInfo,
      &assdf_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&assdf_M->solverInfo,
      &assdf_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&assdf_M->solverInfo, (&rtmGetErrorStatus(assdf_M)));
    rtsiSetRTModelPtr(&assdf_M->solverInfo, assdf_M);
  }

  rtsiSetSimTimeStep(&assdf_M->solverInfo, MAJOR_TIME_STEP);
  assdf_M->intgData.y = assdf_M->odeY;
  assdf_M->intgData.f[0] = assdf_M->odeF[0];
  assdf_M->intgData.f[1] = assdf_M->odeF[1];
  assdf_M->intgData.f[2] = assdf_M->odeF[2];
  assdf_M->intgData.f[3] = assdf_M->odeF[3];
  assdf_M->contStates = ((X_assdf_T *) &assdf_X);
  rtsiSetSolverData(&assdf_M->solverInfo, (void *)&assdf_M->intgData);
  rtsiSetSolverName(&assdf_M->solverInfo,"ode4");
  rtmSetTPtr(assdf_M, &assdf_M->Timing.tArray[0]);
  assdf_M->Timing.stepSize0 = 0.01;

  /* states (continuous) */
  {
    (void) memset((void *)&assdf_X, 0,
                  sizeof(X_assdf_T));
  }

  /* external inputs */
  assdf_U.In1 = 0.0;

  /* external outputs */
  assdf_Y.Out1 = 0.0;

  /* SystemInitialize for Atomic SubSystem: '<Root>/assdf' */
  /* InitializeConditions for Integrator: '<S1>/Integrator' */
  assdf_X.Integrator_CSTATE = assdf_P.Integrator_IC;

  /* End of SystemInitialize for SubSystem: '<Root>/assdf' */
}

/* Model terminate function */
void assdf_terminate(void)
{
  /* (no terminate code required) */
}
