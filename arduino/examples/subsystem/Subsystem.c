/*
 * Subsystem.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Subsystem".
 *
 * Model version              : 1.11
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C source code generated on : Wed Mar 24 19:21:59 2021
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Atmel->AVR (8-bit)
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#include "Subsystem.h"
#include "Subsystem_private.h"

/* Block signals (default storage) */
B_Subsystem_T Subsystem_B;

/* Continuous states */
X_Subsystem_T Subsystem_X;

/* External inputs (root inport signals with default storage) */
ExtU_Subsystem_T Subsystem_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Subsystem_T Subsystem_Y;

/* Real-time model */
RT_MODEL_Subsystem_T Subsystem_M_;
RT_MODEL_Subsystem_T *const Subsystem_M = &Subsystem_M_;

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
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Subsystem_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  Subsystem_step();
  Subsystem_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  Subsystem_step();
  Subsystem_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  Subsystem_step();
  Subsystem_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void Subsystem_step(void)
{
  real_T rtb_Sum7;
  if (rtmIsMajorTimeStep(Subsystem_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&Subsystem_M->solverInfo,
                          ((Subsystem_M->Timing.clockTick0+1)*
      Subsystem_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Subsystem_M)) {
    Subsystem_M->Timing.t[0] = rtsiGetT(&Subsystem_M->solverInfo);
  }

  /* Sum: '<S1>/Sum7' incorporates:
   *  Inport: '<Root>/phi_L'
   *  Integrator: '<S1>/Integrator1'
   */
  rtb_Sum7 = Subsystem_U.phi_L - Subsystem_X.Integrator1_CSTATE;

  /* Outport: '<Root>/Out1' */
  Subsystem_Y.Out1 = rtb_Sum7;

  /* Integrator: '<S1>/Integrator' */
  Subsystem_B.dphi_m = Subsystem_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(Subsystem_M)) {
    /* Gain: '<S1>/Gain3' incorporates:
     *  Inport: '<Root>/phi_L'
     */
    Subsystem_B.Gain3 = 61.050649350649351 * Subsystem_U.phi_L;
  }

  /* Sum: '<S1>/Sum' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Gain: '<S1>/Gain2'
   *  Gain: '<S1>/Gain4'
   *  Gain: '<S1>/Gain5'
   *  Integrator: '<S1>/Integrator1'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S1>/Sum2'
   */
  Subsystem_B.d2phi_m = ((-61.050649350649351 * Subsystem_X.Integrator1_CSTATE +
    Subsystem_B.Gain3) + -9.9766233766233761 * Subsystem_B.dphi_m) + -470.09 *
    rtb_Sum7 * -12.987012987012987;
  if (rtmIsMajorTimeStep(Subsystem_M)) {
    rt_ertODEUpdateContinuousStates(&Subsystem_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++Subsystem_M->Timing.clockTick0;
    Subsystem_M->Timing.t[0] = rtsiGetSolverStopTime(&Subsystem_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      Subsystem_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Subsystem_derivatives(void)
{
  XDot_Subsystem_T *_rtXdot;
  _rtXdot = ((XDot_Subsystem_T *) Subsystem_M->derivs);

  /* Derivatives for Integrator: '<S1>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = Subsystem_B.dphi_m;

  /* Derivatives for Integrator: '<S1>/Integrator' */
  _rtXdot->Integrator_CSTATE = Subsystem_B.d2phi_m;
}

/* Model initialize function */
void Subsystem_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Subsystem_M, 0,
                sizeof(RT_MODEL_Subsystem_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Subsystem_M->solverInfo,
                          &Subsystem_M->Timing.simTimeStep);
    rtsiSetTPtr(&Subsystem_M->solverInfo, &rtmGetTPtr(Subsystem_M));
    rtsiSetStepSizePtr(&Subsystem_M->solverInfo, &Subsystem_M->Timing.stepSize0);
    rtsiSetdXPtr(&Subsystem_M->solverInfo, &Subsystem_M->derivs);
    rtsiSetContStatesPtr(&Subsystem_M->solverInfo, (real_T **)
                         &Subsystem_M->contStates);
    rtsiSetNumContStatesPtr(&Subsystem_M->solverInfo,
      &Subsystem_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Subsystem_M->solverInfo,
      &Subsystem_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Subsystem_M->solverInfo,
      &Subsystem_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Subsystem_M->solverInfo,
      &Subsystem_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&Subsystem_M->solverInfo, (&rtmGetErrorStatus
      (Subsystem_M)));
    rtsiSetRTModelPtr(&Subsystem_M->solverInfo, Subsystem_M);
  }

  rtsiSetSimTimeStep(&Subsystem_M->solverInfo, MAJOR_TIME_STEP);
  Subsystem_M->intgData.y = Subsystem_M->odeY;
  Subsystem_M->intgData.f[0] = Subsystem_M->odeF[0];
  Subsystem_M->intgData.f[1] = Subsystem_M->odeF[1];
  Subsystem_M->intgData.f[2] = Subsystem_M->odeF[2];
  Subsystem_M->intgData.f[3] = Subsystem_M->odeF[3];
  Subsystem_M->contStates = ((X_Subsystem_T *) &Subsystem_X);
  rtsiSetSolverData(&Subsystem_M->solverInfo, (void *)&Subsystem_M->intgData);
  rtsiSetSolverName(&Subsystem_M->solverInfo,"ode4");
  rtmSetTPtr(Subsystem_M, &Subsystem_M->Timing.tArray[0]);
  Subsystem_M->Timing.stepSize0 = 0.01;

  /* block I/O */
  (void) memset(((void *) &Subsystem_B), 0,
                sizeof(B_Subsystem_T));

  /* states (continuous) */
  {
    (void) memset((void *)&Subsystem_X, 0,
                  sizeof(X_Subsystem_T));
  }

  /* external inputs */
  Subsystem_U.phi_L = 0.0;

  /* external outputs */
  Subsystem_Y.Out1 = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator1' */
  Subsystem_X.Integrator1_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator' */
  Subsystem_X.Integrator_CSTATE = 0.0;
}

/* Model terminate function */
void Subsystem_terminate(void)
{
  /* (no terminate code required) */
}
