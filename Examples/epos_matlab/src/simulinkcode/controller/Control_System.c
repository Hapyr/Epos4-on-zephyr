/*
 * Control_System.c
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

#include "Control_System.h"
#include "Control_System_private.h"

/* Block signals (default storage) */
B_Control_System_T Control_System_B;

/* Continuous states */
X_Control_System_T Control_System_X;

/* Block states (default storage) */
DW_Control_System_T Control_System_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Control_System_T Control_System_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Control_System_T Control_System_Y;

/* Real-time model */
RT_MODEL_Control_System_T Control_System_M_;
RT_MODEL_Control_System_T *const Control_System_M = &Control_System_M_;

/* Forward declaration for local functions */
static void Control_Syst_SystemCore_release(dsp_simulink_MovingAverage_Co_T *obj);
static void Control_Syste_SystemCore_delete(dsp_simulink_MovingAverage_Co_T *obj);
static void matlabCodegenHandle_matlabCodeg(dsp_simulink_MovingAverage_Co_T *obj);

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
  int_T nXc = 5;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Control_System_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Control_System_step();
  Control_System_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Control_System_step();
  Control_System_derivatives();

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

static void Control_Syst_SystemCore_release(dsp_simulink_MovingAverage_Co_T *obj)
{
  dsp_private_SlidingWindowAver_T *obj_0;
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    obj_0 = obj->pStatistic;
    if (obj_0->isInitialized == 1) {
      obj_0->isInitialized = 2;
    }

    obj->NumChannels = -1;
  }
}

static void Control_Syste_SystemCore_delete(dsp_simulink_MovingAverage_Co_T *obj)
{
  Control_Syst_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(dsp_simulink_MovingAverage_Co_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Control_Syste_SystemCore_delete(obj);
  }
}

/* Model step function */
void Control_System_step(void)
{
  real_T Torque_Meas;
  real_T Vel_Meas;
  real_T Pos_Meas;
  real_T Torque_Ref;
  real_T csumrev[3];
  int32_T Torque_Ref_tmp;
  if (rtmIsMajorTimeStep(Control_System_M)) {
    /* set solver stop time */
    if (!(Control_System_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Control_System_M->solverInfo,
                            ((Control_System_M->Timing.clockTickH0 + 1) *
        Control_System_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Control_System_M->solverInfo,
                            ((Control_System_M->Timing.clockTick0 + 1) *
        Control_System_M->Timing.stepSize0 +
        Control_System_M->Timing.clockTickH0 *
        Control_System_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Control_System_M)) {
    Control_System_M->Timing.t[0] = rtsiGetT(&Control_System_M->solverInfo);
  }

  /* MATLAB Function: '<Root>/Measurement Function' incorporates:
   *  Inport: '<Root>/User_ControlMode'
   *  Inport: '<Root>/pos_meas_rad'
   *  Inport: '<Root>/torque_meas_Nm'
   *  Inport: '<Root>/vel_meas_rads'
   */
  if (Control_System_U.User_ControlMode == 1.0) {
    Control_System_B.ausgabe_g[0] = Control_System_U.torque_meas_Nm;
    Control_System_B.ausgabe_g[1] = Control_System_U.vel_meas_rads;
    Control_System_B.ausgabe_g[2] = Control_System_U.pos_meas_rad;
  } else if (Control_System_U.User_ControlMode == 2.0) {
    Control_System_B.ausgabe_g[0] = Control_System_U.torque_meas_Nm;
    Control_System_B.ausgabe_g[1] = Control_System_U.vel_meas_rads;
    Control_System_B.ausgabe_g[2] = 0.0;
  } else if (Control_System_U.User_ControlMode == 3.0) {
    Control_System_B.ausgabe_g[0] = Control_System_U.torque_meas_Nm;
    Control_System_B.ausgabe_g[1] = 0.0;
    Control_System_B.ausgabe_g[2] = 0.0;
  } else {
    Control_System_B.ausgabe_g[0] = 0.0;
    Control_System_B.ausgabe_g[1] = 0.0;
    Control_System_B.ausgabe_g[2] = 0.0;
  }

  /* End of MATLAB Function: '<Root>/Measurement Function' */

  /* Gain: '<S1>/Gain1' incorporates:
   *  Inport: '<Root>/User_Pos [deg]'
   */
  Torque_Meas = 0.017453292519943295 * Control_System_U.User_Posdeg;

  /* Gain: '<Root>/rpm to rad//s' incorporates:
   *  Inport: '<Root>/User_Vel [rpm]'
   */
  Vel_Meas = 0.10471975511965977 * Control_System_U.User_Velrpm;

  /* MATLAB Function: '<Root>/User Input Function' incorporates:
   *  Inport: '<Root>/User_ControlMode'
   *  Inport: '<Root>/User_Torque [Nm]'
   */
  Pos_Meas = Control_System_U.User_TorqueNm;
  Control_System_B.ausgabe[0] = 0.0;
  Control_System_B.ausgabe[1] = 0.0;
  Control_System_B.ausgabe[2] = 0.0;
  if (Torque_Meas > 3.3107963267948968) {
    Torque_Meas = 3.3107963267948968;
  } else {
    if (Torque_Meas < -0.16920367320510343) {
      Torque_Meas = -0.16920367320510343;
    }
  }

  if (Vel_Meas > 2.304) {
    Vel_Meas = 2.304;
  } else {
    if (Vel_Meas < -2.304) {
      Vel_Meas = -2.304;
    }
  }

  if (Control_System_U.User_TorqueNm > 0.7) {
    Pos_Meas = 0.7;
  } else {
    if (Control_System_U.User_TorqueNm < -0.7) {
      Pos_Meas = -0.7;
    }
  }

  if (Control_System_U.User_ControlMode == 1.0) {
    Control_System_B.ausgabe[2] = Torque_Meas;
  } else if (Control_System_U.User_ControlMode == 2.0) {
    Control_System_B.ausgabe[1] = Vel_Meas;
  } else {
    if (Control_System_U.User_ControlMode == 3.0) {
      Control_System_B.ausgabe[0] = Pos_Meas;
    }
  }

  /* End of MATLAB Function: '<Root>/User Input Function' */
  if (rtmIsMajorTimeStep(Control_System_M)) {
    /* Delay: '<S5>/Delay' */
    Control_System_B.Delay = Control_System_DW.Delay_DSTATE;
  }

  /* MATLAB Function: '<Root>/EnableMotor' incorporates:
   *  Inport: '<Root>/pos_meas_rad'
   *  Inport: '<Root>/vel_meas_rads'
   */
  Torque_Meas = 0.7;
  Vel_Meas = -0.7;
  Control_System_B.enableMotor = (Control_System_B.Delay >= 1.0);
  if ((Control_System_U.pos_meas_rad > 3.3107963267948968) ||
      (Control_System_U.pos_meas_rad < -0.16920367320510343)) {
    Control_System_B.enableMotor = 0.0;
  } else if ((Control_System_U.pos_meas_rad < 3.136263401595464) &&
             (Control_System_U.vel_meas_rads > 2.304)) {
    Torque_Meas = 0.7 - (Control_System_U.vel_meas_rads - 2.304);
  } else if ((Control_System_U.pos_meas_rad > 0.0053292519943295147) &&
             (Control_System_U.vel_meas_rads < -2.304)) {
    Vel_Meas = (-2.304 - Control_System_U.vel_meas_rads) + -0.7;
  } else if ((Control_System_U.pos_meas_rad > 3.136263401595464) &&
             (Control_System_U.vel_meas_rads < 1.152)) {
    Torque_Meas = (3.3107963267948968 - Control_System_U.pos_meas_rad) * 0.7 /
      0.17453292519943295;
  } else if ((Control_System_U.pos_meas_rad > 3.136263401595464) &&
             (Control_System_U.vel_meas_rads > 1.152)) {
    Torque_Meas = (3.3107963267948968 - Control_System_U.pos_meas_rad) * -0.7 /
      0.17453292519943295;
  } else if ((Control_System_U.pos_meas_rad < 0.0053292519943295147) &&
             (Control_System_U.vel_meas_rads > -1.152)) {
    Vel_Meas = (Control_System_U.pos_meas_rad - -0.16920367320510343) * -0.7 /
      0.17453292519943295;
  } else {
    if ((Control_System_U.pos_meas_rad < 0.0053292519943295147) &&
        (Control_System_U.vel_meas_rads < -1.152)) {
      Vel_Meas = (Control_System_U.pos_meas_rad - -0.16920367320510343) * 0.7 /
        0.17453292519943295;
    }
  }

  /* End of MATLAB Function: '<Root>/EnableMotor' */
  if (rtmIsMajorTimeStep(Control_System_M)) {
    /* SignalConversion: '<Root>/HiddenBuf_InsertedFor_Subsystem_at_inport_7' */
    Control_System_B.HiddenBuf_InsertedFor_Subsystem =
      Control_System_B.enableMotor;

    /* Outputs for Enabled SubSystem: '<Root>/Subsystem' incorporates:
     *  EnablePort: '<S4>/Enable'
     */
    if (rtmIsMajorTimeStep(Control_System_M)) {
      if (Control_System_B.HiddenBuf_InsertedFor_Subsystem > 0.0) {
        if (!Control_System_DW.Subsystem_MODE) {
          Control_System_DW.Subsystem_MODE = true;
        }
      } else {
        if (Control_System_DW.Subsystem_MODE) {
          /* Disable for ModelReference: '<S4>/Position Ctr' */
          PositionControl_Disable
            (&(Control_System_DW.PositionCtr_InstanceData.rtdw));

          /* Disable for ModelReference: '<S4>/Velocity Ctr (Cascaded)' */
          VelocityControl_Disable
            (&(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw));

          /* Disable for ModelReference: '<S4>/Velocity Ctr' */
          VelocityControl_Disable
            (&(Control_System_DW.VelocityCtr_InstanceData.rtdw));
          Control_System_DW.Subsystem_MODE = false;
        }
      }
    }

    /* End of Outputs for SubSystem: '<Root>/Subsystem' */
  }

  /* Outputs for Enabled SubSystem: '<Root>/Subsystem' incorporates:
   *  EnablePort: '<S4>/Enable'
   */
  if (Control_System_DW.Subsystem_MODE) {
    /* RelationalOperator: '<S8>/Compare' incorporates:
     *  Constant: '<S8>/Constant'
     *  Inport: '<Root>/User_ControlMode'
     */
    Control_System_B.Compare = (uint8_T)(Control_System_U.User_ControlMode ==
      1.0);

    /* ModelReference: '<S4>/Position Ctr' */
    PositionControl(&(Control_System_DW.PositionCtr_InstanceData.rtm),
                    &Control_System_B.ausgabe[2], &Control_System_B.ausgabe_g[2],
                    &Control_System_B.Compare, &Control_System_B.PositionCtr,
                    &(Control_System_DW.PositionCtr_InstanceData.rtb),
                    &(Control_System_DW.PositionCtr_InstanceData.rtdw),
                    &(Control_System_X.PositionCtr_CSTATE));

    /* ModelReference: '<S4>/Velocity Ctr (Cascaded)' */
    VelocityControl(&(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtm),
                    &Control_System_B.PositionCtr, &Control_System_B.ausgabe_g[1],
                    &Control_System_B.Compare,
                    &Control_System_B.VelocityCtrCascaded,
                    &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtb),
                    &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw),
                    &(Control_System_X.VelocityCtrCascaded_CSTATE));

    /* RelationalOperator: '<S9>/Compare' incorporates:
     *  Constant: '<S9>/Constant'
     *  Inport: '<Root>/User_ControlMode'
     */
    Control_System_B.Compare_g = (uint8_T)(Control_System_U.User_ControlMode ==
      2.0);

    /* ModelReference: '<S4>/Velocity Ctr' */
    VelocityControl(&(Control_System_DW.VelocityCtr_InstanceData.rtm),
                    &Control_System_B.ausgabe[1], &Control_System_B.ausgabe_g[1],
                    &Control_System_B.Compare_g, &Control_System_B.VelocityCtr,
                    &(Control_System_DW.VelocityCtr_InstanceData.rtb),
                    &(Control_System_DW.VelocityCtr_InstanceData.rtdw),
                    &(Control_System_X.VelocityCtr_CSTATE));

    /* MultiPortSwitch: '<S4>/Multiport Switch' incorporates:
     *  Inport: '<Root>/User_ControlMode'
     */
    switch ((int32_T)Control_System_U.User_ControlMode) {
     case 1:
      Control_System_B.MultiportSwitch = Control_System_B.VelocityCtrCascaded;
      break;

     case 2:
      Control_System_B.MultiportSwitch = Control_System_B.VelocityCtr;
      break;

     default:
      Control_System_B.MultiportSwitch = Control_System_B.ausgabe[0];
      break;
    }

    /* End of MultiPortSwitch: '<S4>/Multiport Switch' */
  }

  /* End of Outputs for SubSystem: '<Root>/Subsystem' */

  /* MATLAB Function: '<Root>/Torque Limitation' */
  if (Control_System_B.enableMotor == 0.0) {
    Torque_Meas = 0.0;
  } else {
    if (!(Control_System_B.MultiportSwitch > Torque_Meas)) {
      if (Control_System_B.MultiportSwitch < Vel_Meas) {
        Torque_Meas = Vel_Meas;
      } else {
        Torque_Meas = Control_System_B.MultiportSwitch;
      }
    }
  }

  /* End of MATLAB Function: '<Root>/Torque Limitation' */

  /* MATLABSystem: '<Root>/Moving Average' */
  if (Control_System_DW.obj.TunablePropsChanged) {
    Control_System_DW.obj.TunablePropsChanged = false;
  }

  if (Control_System_DW.obj.pStatistic->isInitialized != 1) {
    Control_System_DW.obj.pStatistic->isSetupComplete = false;
    Control_System_DW.obj.pStatistic->isInitialized = 1;
    Control_System_DW.obj.pStatistic->pCumSum = 0.0;
    Control_System_DW.obj.pStatistic->pCumRevIndex = 1.0;
    Control_System_DW.obj.pStatistic->isSetupComplete = true;
    Control_System_DW.obj.pStatistic->pCumSum = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[0] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[0] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[1] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[1] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[2] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[2] = 0.0;
    Control_System_DW.obj.pStatistic->pCumRevIndex = 1.0;
  }

  Vel_Meas = Control_System_DW.obj.pStatistic->pCumRevIndex;
  Pos_Meas = Control_System_DW.obj.pStatistic->pCumSum;
  csumrev[0] = Control_System_DW.obj.pStatistic->pCumSumRev[0];
  csumrev[1] = Control_System_DW.obj.pStatistic->pCumSumRev[1];
  csumrev[2] = Control_System_DW.obj.pStatistic->pCumSumRev[2];
  Pos_Meas += Torque_Meas;
  Torque_Ref_tmp = (int32_T)Vel_Meas - 1;
  Torque_Ref = csumrev[Torque_Ref_tmp] + Pos_Meas;
  csumrev[Torque_Ref_tmp] = Torque_Meas;
  if (Vel_Meas != 3.0) {
    Vel_Meas++;
  } else {
    Vel_Meas = 1.0;
    Pos_Meas = 0.0;
    csumrev[1] += csumrev[2];
    csumrev[0] += csumrev[1];
  }

  Control_System_DW.obj.pStatistic->pCumSum = Pos_Meas;
  Control_System_DW.obj.pStatistic->pCumSumRev[0] = csumrev[0];
  Control_System_DW.obj.pStatistic->pCumSumRev[1] = csumrev[1];
  Control_System_DW.obj.pStatistic->pCumSumRev[2] = csumrev[2];
  Control_System_DW.obj.pStatistic->pCumRevIndex = Vel_Meas;

  /* Outport: '<Root>/Output_Torque' incorporates:
   *  MATLABSystem: '<Root>/Moving Average'
   */
  Control_System_Y.Output_Torque = Torque_Ref / 4.0;
  if (rtmIsMajorTimeStep(Control_System_M)) {
    /* Delay: '<S5>/Delay1' */
    Control_System_B.Delay1 = Control_System_DW.Delay1_DSTATE;
  }

  /* Sum: '<S5>/Add1' incorporates:
   *  Inport: '<Root>/Unlock_Motor'
   *  Sum: '<S5>/Add'
   */
  Control_System_B.Add1 = (Control_System_U.Unlock_Motor -
    Control_System_B.Delay1) + Control_System_B.enableMotor;
  if (rtmIsMajorTimeStep(Control_System_M)) {
    if (rtmIsMajorTimeStep(Control_System_M)) {
      /* Update for Delay: '<S5>/Delay' */
      Control_System_DW.Delay_DSTATE = Control_System_B.Add1;
    }

    /* Update for Enabled SubSystem: '<Root>/Subsystem' incorporates:
     *  EnablePort: '<S4>/Enable'
     */
    if (Control_System_DW.Subsystem_MODE) {
      /* Update for ModelReference: '<S4>/Position Ctr' */
      PositionControl_Update(&(Control_System_DW.PositionCtr_InstanceData.rtm),
        &(Control_System_DW.PositionCtr_InstanceData.rtb),
        &(Control_System_DW.PositionCtr_InstanceData.rtdw));

      /* Update for ModelReference: '<S4>/Velocity Ctr (Cascaded)' */
      VelocityControl_Update
        (&(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtm),
         &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtb),
         &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw));

      /* Update for ModelReference: '<S4>/Velocity Ctr' */
      VelocityControl_Update(&(Control_System_DW.VelocityCtr_InstanceData.rtm),
        &(Control_System_DW.VelocityCtr_InstanceData.rtb),
        &(Control_System_DW.VelocityCtr_InstanceData.rtdw));
    }

    /* End of Update for SubSystem: '<Root>/Subsystem' */
    if (rtmIsMajorTimeStep(Control_System_M)) {
      /* Update for Delay: '<S5>/Delay1' incorporates:
       *  Inport: '<Root>/Unlock_Motor'
       */
      Control_System_DW.Delay1_DSTATE = Control_System_U.Unlock_Motor;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(Control_System_M)) {
    rt_ertODEUpdateContinuousStates(&Control_System_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++Control_System_M->Timing.clockTick0)) {
      ++Control_System_M->Timing.clockTickH0;
    }

    Control_System_M->Timing.t[0] = rtsiGetSolverStopTime
      (&Control_System_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      Control_System_M->Timing.clockTick1++;
      if (!Control_System_M->Timing.clockTick1) {
        Control_System_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Control_System_derivatives(void)
{
  /* Derivatives for Enabled SubSystem: '<Root>/Subsystem' */
  if (Control_System_DW.Subsystem_MODE) {
    /* Derivatives for ModelReference: '<S4>/Position Ctr' */
    PositionControl_Deriv(&(Control_System_DW.PositionCtr_InstanceData.rtb),
                          &(Control_System_DW.PositionCtr_InstanceData.rtdw),
                          &(((XDot_Control_System_T *) Control_System_M->derivs
      )->PositionCtr_CSTATE));

    /* Derivatives for ModelReference: '<S4>/Velocity Ctr (Cascaded)' */
    VelocityControl_Deriv
      (&(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtb),
       &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw),
       &(((XDot_Control_System_T *) Control_System_M->derivs)
         ->VelocityCtrCascaded_CSTATE));

    /* Derivatives for ModelReference: '<S4>/Velocity Ctr' */
    VelocityControl_Deriv(&(Control_System_DW.VelocityCtr_InstanceData.rtb),
                          &(Control_System_DW.VelocityCtr_InstanceData.rtdw),
                          &(((XDot_Control_System_T *) Control_System_M->derivs
      )->VelocityCtr_CSTATE));
  } else {
    {
      real_T *dx;
      int_T i;
      dx = (real_T *) &(((XDot_Control_System_T *) Control_System_M->derivs)
                        ->PositionCtr_CSTATE);
      for (i=0; i < 5; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<Root>/Subsystem' */
}

/* Model initialize function */
void Control_System_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Control_System_M, 0,
                sizeof(RT_MODEL_Control_System_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Control_System_M->solverInfo,
                          &Control_System_M->Timing.simTimeStep);
    rtsiSetTPtr(&Control_System_M->solverInfo, &rtmGetTPtr(Control_System_M));
    rtsiSetStepSizePtr(&Control_System_M->solverInfo,
                       &Control_System_M->Timing.stepSize0);
    rtsiSetdXPtr(&Control_System_M->solverInfo, &Control_System_M->derivs);
    rtsiSetContStatesPtr(&Control_System_M->solverInfo, (real_T **)
                         &Control_System_M->contStates);
    rtsiSetNumContStatesPtr(&Control_System_M->solverInfo,
      &Control_System_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Control_System_M->solverInfo,
      &Control_System_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Control_System_M->solverInfo,
      &Control_System_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Control_System_M->solverInfo,
      &Control_System_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&Control_System_M->solverInfo, (&rtmGetErrorStatus
      (Control_System_M)));
    rtsiSetRTModelPtr(&Control_System_M->solverInfo, Control_System_M);
  }

  rtsiSetSimTimeStep(&Control_System_M->solverInfo, MAJOR_TIME_STEP);
  Control_System_M->intgData.y = Control_System_M->odeY;
  Control_System_M->intgData.f[0] = Control_System_M->odeF[0];
  Control_System_M->intgData.f[1] = Control_System_M->odeF[1];
  Control_System_M->intgData.f[2] = Control_System_M->odeF[2];
  Control_System_M->contStates = ((X_Control_System_T *) &Control_System_X);
  rtsiSetSolverData(&Control_System_M->solverInfo, (void *)
                    &Control_System_M->intgData);
  rtsiSetSolverName(&Control_System_M->solverInfo,"ode3");
  rtmSetTPtr(Control_System_M, &Control_System_M->Timing.tArray[0]);
  Control_System_M->Timing.stepSize0 = 0.001;

  /* block I/O */
  (void) memset(((void *) &Control_System_B), 0,
                sizeof(B_Control_System_T));

  /* states (continuous) */
  {
    (void) memset((void *)&Control_System_X, 0,
                  sizeof(X_Control_System_T));
  }

  /* states (dwork) */
  (void) memset((void *)&Control_System_DW, 0,
                sizeof(DW_Control_System_T));

  /* external inputs */
  (void)memset(&Control_System_U, 0, sizeof(ExtU_Control_System_T));

  /* external outputs */
  Control_System_Y.Output_Torque = 0.0;

  /* Model Initialize function for ModelReference Block: '<S4>/Position Ctr' */
  PositionControl_initialize(rtmGetErrorStatusPointer(Control_System_M),
    rtmGetStopRequestedPtr(Control_System_M), &(Control_System_M->solverInfo),
    &(Control_System_DW.PositionCtr_InstanceData.rtm),
    &(Control_System_DW.PositionCtr_InstanceData.rtb),
    &(Control_System_DW.PositionCtr_InstanceData.rtdw));

  /* Model Initialize function for ModelReference Block: '<S4>/Velocity Ctr' */
  VelocityControl_initialize(rtmGetErrorStatusPointer(Control_System_M),
    rtmGetStopRequestedPtr(Control_System_M), &(Control_System_M->solverInfo),
    &(Control_System_DW.VelocityCtr_InstanceData.rtm),
    &(Control_System_DW.VelocityCtr_InstanceData.rtb),
    &(Control_System_DW.VelocityCtr_InstanceData.rtdw));

  /* Model Initialize function for ModelReference Block: '<S4>/Velocity Ctr (Cascaded)' */
  VelocityControl_initialize(rtmGetErrorStatusPointer(Control_System_M),
    rtmGetStopRequestedPtr(Control_System_M), &(Control_System_M->solverInfo),
    &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtm),
    &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtb),
    &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw));

  /* Start for MATLABSystem: '<Root>/Moving Average' */
  Control_System_DW.obj.matlabCodegenIsDeleted = true;
  Control_System_DW.obj.isInitialized = 0;
  Control_System_DW.obj.NumChannels = -1;
  Control_System_DW.obj.matlabCodegenIsDeleted = false;
  Control_System_DW.objisempty = true;
  Control_System_DW.obj.isSetupComplete = false;
  Control_System_DW.obj.isInitialized = 1;
  Control_System_DW.obj.NumChannels = 1;
  Control_System_DW.gobj_0.isInitialized = 0;
  Control_System_DW.obj.pStatistic = &Control_System_DW.gobj_0;
  Control_System_DW.obj.isSetupComplete = true;
  Control_System_DW.obj.TunablePropsChanged = false;

  /* InitializeConditions for Delay: '<S5>/Delay' */
  Control_System_DW.Delay_DSTATE = 0.0;

  /* InitializeConditions for Delay: '<S5>/Delay1' */
  Control_System_DW.Delay1_DSTATE = 0.0;

  /* SystemInitialize for Enabled SubSystem: '<Root>/Subsystem' */
  /* SystemInitialize for ModelReference: '<S4>/Position Ctr' */
  PositionControl_Init(&Control_System_B.PositionCtr,
                       &(Control_System_DW.PositionCtr_InstanceData.rtdw),
                       &(Control_System_X.PositionCtr_CSTATE));

  /* SystemInitialize for ModelReference: '<S4>/Velocity Ctr (Cascaded)' */
  VelocityControl_Init(&Control_System_B.VelocityCtrCascaded,
                       &(Control_System_DW.VelocityCtrCascaded_InstanceDat.rtdw),
                       &(Control_System_X.VelocityCtrCascaded_CSTATE));

  /* SystemInitialize for ModelReference: '<S4>/Velocity Ctr' */
  VelocityControl_Init(&Control_System_B.VelocityCtr,
                       &(Control_System_DW.VelocityCtr_InstanceData.rtdw),
                       &(Control_System_X.VelocityCtr_CSTATE));

  /* SystemInitialize for Outport: '<S4>/u' */
  Control_System_B.MultiportSwitch = 0.0;

  /* End of SystemInitialize for SubSystem: '<Root>/Subsystem' */

  /* InitializeConditions for MATLABSystem: '<Root>/Moving Average' */
  if (Control_System_DW.obj.pStatistic->isInitialized == 1) {
    Control_System_DW.obj.pStatistic->pCumSum = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[0] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[1] = 0.0;
    Control_System_DW.obj.pStatistic->pCumSumRev[2] = 0.0;
    Control_System_DW.obj.pStatistic->pCumRevIndex = 1.0;
  }

  /* End of InitializeConditions for MATLABSystem: '<Root>/Moving Average' */
}

/* Model terminate function */
void Control_System_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/Moving Average' */
  matlabCodegenHandle_matlabCodeg(&Control_System_DW.obj);
}
