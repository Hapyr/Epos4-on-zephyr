/*
 * Atomic.c
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

/* Block signals (default storage) */
B_Atomic_T Atomic_B;

/* Continuous states */
X_Atomic_T Atomic_X;

/* External inputs (root inport signals with default storage) */
ExtU_Atomic_T Atomic_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Atomic_T Atomic_Y;

/* Real-time model */
static RT_MODEL_Atomic_T Atomic_M_;
RT_MODEL_Atomic_T *const Atomic_M = &Atomic_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Atomic_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Atomic_step();
  Atomic_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Atomic_step();
  Atomic_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void Atomic_step(void)
{
  if (rtmIsMajorTimeStep(Atomic_M)) {
    /* set solver stop time */
    if (!(Atomic_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Atomic_M->solverInfo,
                            ((Atomic_M->Timing.clockTickH0 + 1) *
        Atomic_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Atomic_M->solverInfo, ((Atomic_M->Timing.clockTick0
        + 1) * Atomic_M->Timing.stepSize0 + Atomic_M->Timing.clockTickH0 *
        Atomic_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Atomic_M)) {
    Atomic_M->Timing.t[0] = rtsiGetT(&Atomic_M->solverInfo);
  }

  /* Outputs for Atomic SubSystem: '<Root>/Atomic Subsystem' */
  /* Gain: '<S31>/Integral Gain' incorporates:
   *  Inport: '<Root>/In1'
   */
  Atomic_B.IntegralGain = Atomic_P.PIDController_I * Atomic_U.phi_m;

  /* Outport: '<Root>/Out1' incorporates:
   *  Gain: '<S39>/Proportional Gain'
   *  Inport: '<Root>/In1'
   *  Integrator: '<S34>/Integrator'
   *  Sum: '<S43>/Sum'
   */
  Atomic_Y.Out1 = Atomic_P.PIDController_P * Atomic_U.phi_m +
    Atomic_X.Integrator_CSTATE;

  /* End of Outputs for SubSystem: '<Root>/Atomic Subsystem' */
  if (rtmIsMajorTimeStep(Atomic_M)) {
    rt_ertODEUpdateContinuousStates(&Atomic_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++Atomic_M->Timing.clockTick0)) {
      ++Atomic_M->Timing.clockTickH0;
    }

    Atomic_M->Timing.t[0] = rtsiGetSolverStopTime(&Atomic_M->solverInfo);

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
      Atomic_M->Timing.clockTick1++;
      if (!Atomic_M->Timing.clockTick1) {
        Atomic_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Atomic_derivatives(void)
{
  XDot_Atomic_T *_rtXdot;
  _rtXdot = ((XDot_Atomic_T *) Atomic_M->derivs);

  /* Derivatives for Atomic SubSystem: '<Root>/Atomic Subsystem' */
  /* Derivatives for Integrator: '<S34>/Integrator' */
  _rtXdot->Integrator_CSTATE = Atomic_B.IntegralGain;

  /* End of Derivatives for SubSystem: '<Root>/Atomic Subsystem' */
}

/* Model initialize function */
void Atomic_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Atomic_M, 0,
                sizeof(RT_MODEL_Atomic_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Atomic_M->solverInfo, &Atomic_M->Timing.simTimeStep);
    rtsiSetTPtr(&Atomic_M->solverInfo, &rtmGetTPtr(Atomic_M));
    rtsiSetStepSizePtr(&Atomic_M->solverInfo, &Atomic_M->Timing.stepSize0);
    rtsiSetdXPtr(&Atomic_M->solverInfo, &Atomic_M->derivs);
    rtsiSetContStatesPtr(&Atomic_M->solverInfo, (real_T **)
                         &Atomic_M->contStates);
    rtsiSetNumContStatesPtr(&Atomic_M->solverInfo,
      &Atomic_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Atomic_M->solverInfo,
      &Atomic_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Atomic_M->solverInfo,
      &Atomic_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Atomic_M->solverInfo,
      &Atomic_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&Atomic_M->solverInfo, (&rtmGetErrorStatus(Atomic_M)));
    rtsiSetRTModelPtr(&Atomic_M->solverInfo, Atomic_M);
  }

  rtsiSetSimTimeStep(&Atomic_M->solverInfo, MAJOR_TIME_STEP);
  Atomic_M->intgData.y = Atomic_M->odeY;
  Atomic_M->intgData.f[0] = Atomic_M->odeF[0];
  Atomic_M->intgData.f[1] = Atomic_M->odeF[1];
  Atomic_M->intgData.f[2] = Atomic_M->odeF[2];
  Atomic_M->contStates = ((X_Atomic_T *) &Atomic_X);
  rtsiSetSolverData(&Atomic_M->solverInfo, (void *)&Atomic_M->intgData);
  rtsiSetSolverName(&Atomic_M->solverInfo,"ode3");
  rtmSetTPtr(Atomic_M, &Atomic_M->Timing.tArray[0]);
  Atomic_M->Timing.stepSize0 = 0.01;

  /* block I/O */
  (void) memset(((void *) &Atomic_B), 0,
                sizeof(B_Atomic_T));

  /* states (continuous) */
  {
    (void) memset((void *)&Atomic_X, 0,
                  sizeof(X_Atomic_T));
  }

  /* external inputs */
  Atomic_U.phi_m = 0.0;

  /* external outputs */
  Atomic_Y.Out1 = 0.0;

  /* SystemInitialize for Atomic SubSystem: '<Root>/Atomic Subsystem' */
  /* InitializeConditions for Integrator: '<S34>/Integrator' */
  Atomic_X.Integrator_CSTATE = Atomic_P.PIDController_InitialConditionF;

  /* End of SystemInitialize for SubSystem: '<Root>/Atomic Subsystem' */
}

/* Model terminate function */
void Atomic_terminate(void)
{
  /* (no terminate code required) */
}
