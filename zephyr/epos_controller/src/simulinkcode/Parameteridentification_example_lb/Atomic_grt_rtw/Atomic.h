/*
 * Atomic.h
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

#ifndef RTW_HEADER_Atomic_h_
#define RTW_HEADER_Atomic_h_
#include <string.h>
#ifndef Atomic_COMMON_INCLUDES_
#define Atomic_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Atomic_COMMON_INCLUDES_ */

#include "Atomic_types.h"

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T IntegralGain;                 /* '<S31>/Integral Gain' */
} B_Atomic_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S34>/Integrator' */
} X_Atomic_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S34>/Integrator' */
} XDot_Atomic_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S34>/Integrator' */
} XDis_Atomic_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T phi_m;                        /* '<Root>/In1' */
} ExtU_Atomic_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out1;                         /* '<Root>/Out1' */
} ExtY_Atomic_T;

/* Parameters (default storage) */
struct P_Atomic_T_ {
  real_T PIDController_I;              /* Mask Parameter: PIDController_I
                                        * Referenced by: '<S31>/Integral Gain'
                                        */
  real_T PIDController_InitialConditionF;
                              /* Mask Parameter: PIDController_InitialConditionF
                               * Referenced by: '<S34>/Integrator'
                               */
  real_T PIDController_P;              /* Mask Parameter: PIDController_P
                                        * Referenced by: '<S39>/Proportional Gain'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Atomic_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_Atomic_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[1];
  real_T odeF[3][1];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_Atomic_T Atomic_P;

/* Block signals (default storage) */
extern B_Atomic_T Atomic_B;

/* Continuous states (default storage) */
extern X_Atomic_T Atomic_X;

/* External inputs (root inport signals with default storage) */
extern ExtU_Atomic_T Atomic_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Atomic_T Atomic_Y;

/* Model entry point functions */
extern void Atomic_initialize(void);
extern void Atomic_step(void);
extern void Atomic_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Atomic_T *const Atomic_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('non_lin_controll/Atomic Subsystem')    - opens subsystem non_lin_controll/Atomic Subsystem
 * hilite_system('non_lin_controll/Atomic Subsystem/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'non_lin_controll'
 * '<S1>'   : 'non_lin_controll/Atomic Subsystem'
 * '<S2>'   : 'non_lin_controll/Atomic Subsystem/PID Controller'
 * '<S3>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/Anti-windup'
 * '<S4>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/D Gain'
 * '<S5>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/Filter'
 * '<S6>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/Filter ICs'
 * '<S7>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/I Gain'
 * '<S8>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/Ideal P Gain'
 * '<S9>'   : 'non_lin_controll/Atomic Subsystem/PID Controller/Ideal P Gain Fdbk'
 * '<S10>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Integrator'
 * '<S11>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Integrator ICs'
 * '<S12>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/N Copy'
 * '<S13>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/N Gain'
 * '<S14>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/P Copy'
 * '<S15>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Parallel P Gain'
 * '<S16>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Reset Signal'
 * '<S17>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Saturation'
 * '<S18>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Saturation Fdbk'
 * '<S19>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Sum'
 * '<S20>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Sum Fdbk'
 * '<S21>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tracking Mode'
 * '<S22>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tracking Mode Sum'
 * '<S23>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tsamp - Integral'
 * '<S24>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tsamp - Ngain'
 * '<S25>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/postSat Signal'
 * '<S26>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/preSat Signal'
 * '<S27>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Anti-windup/Passthrough'
 * '<S28>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/D Gain/Disabled'
 * '<S29>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Filter/Disabled'
 * '<S30>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Filter ICs/Disabled'
 * '<S31>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/I Gain/Internal Parameters'
 * '<S32>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Ideal P Gain/Passthrough'
 * '<S33>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S34>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Integrator/Continuous'
 * '<S35>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Integrator ICs/Internal IC'
 * '<S36>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/N Copy/Disabled wSignal Specification'
 * '<S37>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/N Gain/Disabled'
 * '<S38>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/P Copy/Disabled'
 * '<S39>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S40>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Reset Signal/Disabled'
 * '<S41>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Saturation/Passthrough'
 * '<S42>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Saturation Fdbk/Disabled'
 * '<S43>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Sum/Sum_PI'
 * '<S44>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Sum Fdbk/Disabled'
 * '<S45>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tracking Mode/Disabled'
 * '<S46>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S47>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tsamp - Integral/Passthrough'
 * '<S48>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S49>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/postSat Signal/Forward_Path'
 * '<S50>'  : 'non_lin_controll/Atomic Subsystem/PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* RTW_HEADER_Atomic_h_ */
