/*
 * Code generation for system model 'PositionControl'
 *
 * Model                      : PositionControl
 * Model version              : 1.70
 * Simulink Coder version : 9.0 (R2018b) 24-May-2018
 * C source code generated on : Tue Jun 28 17:43:40 2022
 *
 * Note that the functions contained in this file are part of a Simulink
 * model, and are not self-contained algorithms.
 */

#include "PositionControl.h"
#include "PositionControl_private.h"

/* System initialize for referenced model: 'PositionControl' */
void PositionControl_Init(real_T *rty_dphi_refrads, DW_PositionControl_f_T
  *localDW, X_PositionControl_n_T *localX)
{
  /* InitializeConditions for Integrator: '<S35>/Filter' */
  localX->Filter_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S151>/Integrator' */
  localX->Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S131>/Filter' */
  localX->Filter_CSTATE_f = 0.0;

  /* InitializeConditions for Memory: '<S123>/Memory' */
  localDW->Memory_PreviousInput = false;

  /* SystemInitialize for Outport: '<Root>/dphi_ref [rad//s]' */
  *rty_dphi_refrads = 0.0;
}

/* System reset for referenced model: 'PositionControl' */
void PositionControl_Reset(DW_PositionControl_f_T *localDW,
  X_PositionControl_n_T *localX)
{
  /* InitializeConditions for Integrator: '<S35>/Filter' */
  localX->Filter_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S151>/Integrator' */
  localX->Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S131>/Filter' */
  localX->Filter_CSTATE_f = 0.0;

  /* InitializeConditions for Memory: '<S123>/Memory' */
  localDW->Memory_PreviousInput = false;
}

/* Disable for referenced model: 'PositionControl' */
void PositionControl_Disable(DW_PositionControl_f_T *localDW)
{
  if (localDW->TmpModelReferenceSubsystem_MODE) {
    localDW->TmpModelReferenceSubsystem_MODE = false;
  }
}

/* Outputs for referenced model: 'PositionControl' */
void PositionControl(RT_MODEL_PositionControl_T * const PositionControl_M, const
                     real_T *rtu_phi_refrad, const real_T *rtu_phirad, const
                     uint8_T *rtu_Enable, real_T *rty_dphi_refrads,
                     B_PositionControl_c_T *localB, DW_PositionControl_f_T
                     *localDW, X_PositionControl_n_T *localX)
{
  real_T rtb_Filter;
  real_T rtb_ZeroGain;
  boolean_T rtb_NotEqual;
  if (rtmIsMajorTimeStep(PositionControl_M) && rtmIsMajorTimeStep
      (PositionControl_M)) {
    if (*rtu_Enable > 0) {
      if (!localDW->TmpModelReferenceSubsystem_MODE) {
        PositionControl_Reset(localDW, localX);
        localDW->TmpModelReferenceSubsystem_MODE = true;
      }
    } else {
      PositionControl_Disable(localDW);
    }
  }

  if (localDW->TmpModelReferenceSubsystem_MODE) {
    /* Sum: '<Root>/Sum' */
    rtb_Filter = *rtu_phi_refrad - *rtu_phirad;

    /* Gain: '<S67>/Filter Coefficient' incorporates:
     *  Gain: '<S34>/Derivative Gain'
     *  Integrator: '<S35>/Filter'
     *  Sum: '<S35>/SumD'
     */
    localB->FilterCoefficient = (0.117155864593637 * rtb_Filter -
      localX->Filter_CSTATE) * 2.70289040596724;

    /* Sum: '<S85>/Sum' incorporates:
     *  Gain: '<S74>/Proportional Gain'
     */
    rtb_Filter = 1.7740510896785 * rtb_Filter + localB->FilterCoefficient;

    /* Saturate: '<S78>/Saturation' */
    if (rtb_Filter > 2.304) {
      *rty_dphi_refrads = 2.304;
    } else if (rtb_Filter < -2.304) {
      *rty_dphi_refrads = -2.304;
    } else {
      *rty_dphi_refrads = rtb_Filter;
    }

    /* End of Saturate: '<S78>/Saturation' */

    /* Gain: '<S163>/Filter Coefficient' incorporates:
     *  Integrator: '<S131>/Filter'
     *  Sum: '<S131>/SumD'
     */
    localB->FilterCoefficient_a = (0.0 - localX->Filter_CSTATE_f) *
      266.160326879404;

    /* Sum: '<S183>/Sum' incorporates:
     *  Integrator: '<S151>/Integrator'
     */
    rtb_Filter = localX->Integrator_CSTATE + localB->FilterCoefficient_a;

    /* Gain: '<S123>/ZeroGain' */
    rtb_ZeroGain = 0.0 * rtb_Filter;

    /* DeadZone: '<S123>/DeadZone' */
    if (rtb_Filter > 2.304) {
      rtb_Filter -= 2.304;
    } else if (rtb_Filter >= -2.304) {
      rtb_Filter = 0.0;
    } else {
      rtb_Filter -= -2.304;
    }

    /* End of DeadZone: '<S123>/DeadZone' */

    /* RelationalOperator: '<S123>/NotEqual' */
    rtb_NotEqual = (rtb_ZeroGain != rtb_Filter);

    /* Signum: '<S123>/SignPreSat' */
    if (rtb_Filter < 0.0) {
      rtb_Filter = -1.0;
    } else if (rtb_Filter > 0.0) {
      rtb_Filter = 1.0;
    } else if (rtb_Filter == 0.0) {
      rtb_Filter = 0.0;
    } else {
      rtb_Filter = (rtNaN);
    }

    /* End of Signum: '<S123>/SignPreSat' */

    /* Logic: '<S123>/AND3' incorporates:
     *  DataTypeConversion: '<S123>/DataTypeConv1'
     *  RelationalOperator: '<S123>/Equal1'
     */
    localB->AND3 = (rtb_NotEqual && ((int8_T)rtb_Filter == 0));
    if (rtmIsMajorTimeStep(PositionControl_M)) {
      /* Switch: '<S123>/Switch' incorporates:
       *  Constant: '<S123>/Constant1'
       */
      localB->Switch = 0.0;
    }
  }
}

/* Update for referenced model: 'PositionControl' */
void PositionControl_Update(RT_MODEL_PositionControl_T * const PositionControl_M,
  B_PositionControl_c_T *localB, DW_PositionControl_f_T *localDW)
{
  if (localDW->TmpModelReferenceSubsystem_MODE && rtmIsMajorTimeStep
      (PositionControl_M)) {
    /* Update for Memory: '<S123>/Memory' */
    localDW->Memory_PreviousInput = localB->AND3;
  }
}

/* Derivatives for referenced model: 'PositionControl' */
void PositionControl_Deriv(B_PositionControl_c_T *localB, DW_PositionControl_f_T
  *localDW, XDot_PositionControl_n_T *localXdot)
{
  if (localDW->TmpModelReferenceSubsystem_MODE) {
    /* Derivatives for Integrator: '<S35>/Filter' */
    localXdot->Filter_CSTATE = localB->FilterCoefficient;

    /* Derivatives for Integrator: '<S151>/Integrator' */
    localXdot->Integrator_CSTATE = localB->Switch;

    /* Derivatives for Integrator: '<S131>/Filter' */
    localXdot->Filter_CSTATE_f = localB->FilterCoefficient_a;
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(localXdot->Filter_CSTATE);
      for (i=0; i < 3; i++) {
        dx[i] = 0.0;
      }
    }
  }
}

/* Model initialize function */
void PositionControl_initialize(const char_T **rt_errorStatus, boolean_T
  *rt_stopRequested, RTWSolverInfo *rt_solverInfo, RT_MODEL_PositionControl_T *
  const PositionControl_M, B_PositionControl_c_T *localB, DW_PositionControl_f_T
  *localDW)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)PositionControl_M, 0,
                sizeof(RT_MODEL_PositionControl_T));

  /* initialize error status */
  rtmSetErrorStatusPointer(PositionControl_M, rt_errorStatus);

  /* initialize stop requested flag */
  rtmSetStopRequestedPtr(PositionControl_M, rt_stopRequested);

  /* initialize RTWSolverInfo */
  PositionControl_M->solverInfo = (rt_solverInfo);

  /* Set the Timing fields to the appropriate data in the RTWSolverInfo */
  rtmSetSimTimeStepPointer(PositionControl_M, rtsiGetSimTimeStepPtr
    (PositionControl_M->solverInfo));
  PositionControl_M->Timing.stepSize0 = (rtsiGetStepSize
    (PositionControl_M->solverInfo));

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(B_PositionControl_c_T));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(DW_PositionControl_f_T));
}
