/*
 * Code generation for system model 'VelocityControl'
 *
 * Model                      : VelocityControl
 * Model version              : 1.74
 * Simulink Coder version : 9.0 (R2018b) 24-May-2018
 * C source code generated on : Tue Jun 28 17:43:44 2022
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "VelocityControl.h"
#include "VelocityControl_private.h"

/* System initialize for referenced model: 'VelocityControl' */
void VelocityControl_Init(real_T *rty_tau_mNm, DW_VelocityControl_f_T *localDW,
  X_VelocityControl_n_T *localX)
{
  /* InitializeConditions for Integrator: '<S54>/Integrator' */
  localX->Integrator_CSTATE = 0.0;

  /* InitializeConditions for Memory: '<S26>/Memory' */
  localDW->Memory_PreviousInput = false;

  /* SystemInitialize for Outport: '<Root>/tau_m [Nm]' */
  *rty_tau_mNm = 0.0;
}

/* System reset for referenced model: 'VelocityControl' */
void VelocityControl_Reset(DW_VelocityControl_f_T *localDW,
  X_VelocityControl_n_T *localX)
{
  /* InitializeConditions for Integrator: '<S54>/Integrator' */
  localX->Integrator_CSTATE = 0.0;

  /* InitializeConditions for Memory: '<S26>/Memory' */
  localDW->Memory_PreviousInput = false;
}

/* Disable for referenced model: 'VelocityControl' */
void VelocityControl_Disable(DW_VelocityControl_f_T *localDW)
{
  if (localDW->TmpModelReferenceSubsystem_MODE) {
    localDW->TmpModelReferenceSubsystem_MODE = false;
  }
}

/* Outputs for referenced model: 'VelocityControl' */
void VelocityControl(RT_MODEL_VelocityControl_T * const VelocityControl_M, const
                     real_T *rtu_dphi_refrads, const real_T *rtu_dphirads, const
                     uint8_T *rtu_Enable, real_T *rty_tau_mNm,
                     B_VelocityControl_c_T *localB, DW_VelocityControl_f_T
                     *localDW, X_VelocityControl_n_T *localX)
{
  real_T rtb_IntegralGain;
  real_T rtb_Sum;
  real_T rtb_SignPreIntegrator;
  boolean_T rtb_NotEqual;
  real_T rtb_IntegralGain_0;
  if (rtmIsMajorTimeStep(VelocityControl_M) && rtmIsMajorTimeStep
      (VelocityControl_M)) {
    if (*rtu_Enable > 0) {
      if (!localDW->TmpModelReferenceSubsystem_MODE) {
        VelocityControl_Reset(localDW, localX);
        localDW->TmpModelReferenceSubsystem_MODE = true;
      }
    } else {
      VelocityControl_Disable(localDW);
    }
  }

  if (localDW->TmpModelReferenceSubsystem_MODE) {
    /* Sum: '<Root>/Sum' */
    rtb_IntegralGain = *rtu_dphi_refrads - *rtu_dphirads;

    /* Sum: '<S85>/Sum' incorporates:
     *  Gain: '<S73>/Proportional Gain'
     *  Integrator: '<S54>/Integrator'
     */
    rtb_Sum = 0.382327477488715 * rtb_IntegralGain + localX->Integrator_CSTATE;

    /* DeadZone: '<S26>/DeadZone' */
    if (rtb_Sum > 0.7) {
      rtb_SignPreIntegrator = rtb_Sum - 0.7;
    } else if (rtb_Sum >= -0.7) {
      rtb_SignPreIntegrator = 0.0;
    } else {
      rtb_SignPreIntegrator = rtb_Sum - -0.7;
    }

    /* End of DeadZone: '<S26>/DeadZone' */

    /* RelationalOperator: '<S26>/NotEqual' incorporates:
     *  Gain: '<S26>/ZeroGain'
     */
    rtb_NotEqual = (0.0 * rtb_Sum != rtb_SignPreIntegrator);

    /* Signum: '<S26>/SignPreSat' */
    if (rtb_SignPreIntegrator < 0.0) {
      rtb_SignPreIntegrator = -1.0;
    } else if (rtb_SignPreIntegrator > 0.0) {
      rtb_SignPreIntegrator = 1.0;
    } else if (rtb_SignPreIntegrator == 0.0) {
      rtb_SignPreIntegrator = 0.0;
    } else {
      rtb_SignPreIntegrator = (rtNaN);
    }

    /* End of Signum: '<S26>/SignPreSat' */

    /* Gain: '<S46>/Integral Gain' */
    rtb_IntegralGain *= 1.14452871958655;

    /* Signum: '<S26>/SignPreIntegrator' */
    if (rtb_IntegralGain < 0.0) {
      rtb_IntegralGain_0 = -1.0;
    } else if (rtb_IntegralGain > 0.0) {
      rtb_IntegralGain_0 = 1.0;
    } else if (rtb_IntegralGain == 0.0) {
      rtb_IntegralGain_0 = 0.0;
    } else {
      rtb_IntegralGain_0 = (rtNaN);
    }

    /* End of Signum: '<S26>/SignPreIntegrator' */

    /* Logic: '<S26>/AND3' incorporates:
     *  DataTypeConversion: '<S26>/DataTypeConv1'
     *  DataTypeConversion: '<S26>/DataTypeConv2'
     *  RelationalOperator: '<S26>/Equal1'
     */
    localB->AND3 = (rtb_NotEqual && ((int8_T)rtb_SignPreIntegrator == (int8_T)
      rtb_IntegralGain_0));
    if (rtmIsMajorTimeStep(VelocityControl_M)) {
      /* Memory: '<S26>/Memory' */
      localB->Memory = localDW->Memory_PreviousInput;
    }

    /* Switch: '<S26>/Switch' incorporates:
     *  Constant: '<S26>/Constant1'
     */
    if (localB->Memory) {
      localB->Switch = 0.0;
    } else {
      localB->Switch = rtb_IntegralGain;
    }

    /* End of Switch: '<S26>/Switch' */

    /* Saturate: '<S77>/Saturation' */
    if (rtb_Sum > 0.7) {
      *rty_tau_mNm = 0.7;
    } else if (rtb_Sum < -0.7) {
      *rty_tau_mNm = -0.7;
    } else {
      *rty_tau_mNm = rtb_Sum;
    }

    /* End of Saturate: '<S77>/Saturation' */
  }
}

/* Update for referenced model: 'VelocityControl' */
void VelocityControl_Update(RT_MODEL_VelocityControl_T * const VelocityControl_M,
  B_VelocityControl_c_T *localB, DW_VelocityControl_f_T *localDW)
{
  if (localDW->TmpModelReferenceSubsystem_MODE && rtmIsMajorTimeStep
      (VelocityControl_M)) {
    /* Update for Memory: '<S26>/Memory' */
    localDW->Memory_PreviousInput = localB->AND3;
  }
}

/* Derivatives for referenced model: 'VelocityControl' */
void VelocityControl_Deriv(B_VelocityControl_c_T *localB, DW_VelocityControl_f_T
  *localDW, XDot_VelocityControl_n_T *localXdot)
{
  if (localDW->TmpModelReferenceSubsystem_MODE) {
    /* Derivatives for Integrator: '<S54>/Integrator' */
    localXdot->Integrator_CSTATE = localB->Switch;
  } else {
    localXdot->Integrator_CSTATE = 0.0;
  }
}

/* Model initialize function */
void VelocityControl_initialize(const char_T **rt_errorStatus, boolean_T
  *rt_stopRequested, RTWSolverInfo *rt_solverInfo, RT_MODEL_VelocityControl_T *
  const VelocityControl_M, B_VelocityControl_c_T *localB, DW_VelocityControl_f_T
  *localDW)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)VelocityControl_M, 0,
                sizeof(RT_MODEL_VelocityControl_T));

  /* initialize error status */
  rtmSetErrorStatusPointer(VelocityControl_M, rt_errorStatus);

  /* initialize stop requested flag */
  rtmSetStopRequestedPtr(VelocityControl_M, rt_stopRequested);

  /* initialize RTWSolverInfo */
  VelocityControl_M->solverInfo = (rt_solverInfo);

  /* Set the Timing fields to the appropriate data in the RTWSolverInfo */
  rtmSetSimTimeStepPointer(VelocityControl_M, rtsiGetSimTimeStepPtr
    (VelocityControl_M->solverInfo));
  VelocityControl_M->Timing.stepSize0 = (rtsiGetStepSize
    (VelocityControl_M->solverInfo));

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(B_VelocityControl_c_T));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_VelocityControl_f_T));
}
