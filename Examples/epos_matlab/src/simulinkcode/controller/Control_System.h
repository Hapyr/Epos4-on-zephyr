/*
 * Control_System.h
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

#ifndef RTW_HEADER_Control_System_h_
#define RTW_HEADER_Control_System_h_
#include <string.h>
#ifndef Control_System_COMMON_INCLUDES_
# define Control_System_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Control_System_COMMON_INCLUDES_ */

#include "Control_System_types.h"

/* Shared type includes */
#include "multiword_types.h"

/* Child system includes */
#include "VelocityControl.h"
#include "PositionControl.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
# define rtmGetContStateDisabled(rtm)  ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
# define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
# define rtmGetContStates(rtm)         ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
# define rtmSetContStates(rtm, val)    ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
# define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
# define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
# define rtmGetIntgData(rtm)           ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
# define rtmSetIntgData(rtm, val)      ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
# define rtmGetOdeF(rtm)               ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
# define rtmSetOdeF(rtm, val)          ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
# define rtmGetOdeY(rtm)               ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
# define rtmSetOdeY(rtm, val)          ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
# define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
# define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
# define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
# define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
# define rtmGetZCCacheNeedsReset(rtm)  ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
# define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
# define rtmGetdX(rtm)                 ((rtm)->derivs)
#endif

#ifndef rtmSetdX
# define rtmSetdX(rtm, val)            ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetErrorStatusPointer
# define rtmGetErrorStatusPointer(rtm) ((const char_T **)(&((rtm)->errorStatus)))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Delay;                        /* '<S5>/Delay' */
  real_T HiddenBuf_InsertedFor_Subsystem;/* '<Root>/EnableMotor' */
  real_T Delay1;                       /* '<S5>/Delay1' */
  real_T Add1;                         /* '<S5>/Add1' */
  real_T ausgabe[3];                   /* '<Root>/User Input Function' */
  real_T PositionCtr;                  /* '<S4>/Position Ctr' */
  real_T VelocityCtrCascaded;          /* '<S4>/Velocity Ctr (Cascaded)' */
  real_T VelocityCtr;                  /* '<S4>/Velocity Ctr' */
  real_T MultiportSwitch;              /* '<S4>/Multiport Switch' */
  real_T ausgabe_g[3];                 /* '<Root>/Measurement Function' */
  real_T enableMotor;                  /* '<Root>/EnableMotor' */
  uint8_T Compare;                     /* '<S8>/Compare' */
  uint8_T Compare_g;                   /* '<S9>/Compare' */
} B_Control_System_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  dsp_simulink_MovingAverage_Co_T obj; /* '<Root>/Moving Average' */
  dsp_private_SlidingWindowAver_T gobj_0;/* '<Root>/Moving Average' */
  dsp_private_SlidingWindowAver_T gobj_1;/* '<Root>/Moving Average' */
  real_T Delay_DSTATE;                 /* '<S5>/Delay' */
  real_T Delay1_DSTATE;                /* '<S5>/Delay1' */
  boolean_T objisempty;                /* '<Root>/Moving Average' */
  boolean_T Subsystem_MODE;            /* '<Root>/Subsystem' */
  MdlrefDW_PositionControl_T PositionCtr_InstanceData;/* '<S4>/Position Ctr' */
  MdlrefDW_VelocityControl_T VelocityCtrCascaded_InstanceDat;/* '<S4>/Velocity Ctr (Cascaded)' */
  MdlrefDW_VelocityControl_T VelocityCtr_InstanceData;/* '<S4>/Velocity Ctr' */
} DW_Control_System_T;

/* Continuous states (default storage) */
typedef struct {
  X_PositionControl_n_T PositionCtr_CSTATE;/* '<S4>/Position Ctr' */
  X_VelocityControl_n_T VelocityCtrCascaded_CSTATE;/* '<S4>/Velocity Ctr (Cascaded)' */
  X_VelocityControl_n_T VelocityCtr_CSTATE;/* '<S4>/Velocity Ctr' */
} X_Control_System_T;

/* State derivatives (default storage) */
typedef struct {
  XDot_PositionControl_n_T PositionCtr_CSTATE;/* '<S4>/Position Ctr' */
  XDot_VelocityControl_n_T VelocityCtrCascaded_CSTATE;/* '<S4>/Velocity Ctr (Cascaded)' */
  XDot_VelocityControl_n_T VelocityCtr_CSTATE;/* '<S4>/Velocity Ctr' */
} XDot_Control_System_T;

/* State disabled  */
typedef struct {
  XDis_PositionControl_n_T PositionCtr_CSTATE;/* '<S4>/Position Ctr' */
  XDis_VelocityControl_n_T VelocityCtrCascaded_CSTATE;/* '<S4>/Velocity Ctr (Cascaded)' */
  XDis_VelocityControl_n_T VelocityCtr_CSTATE;/* '<S4>/Velocity Ctr' */
} XDis_Control_System_T;

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
  real_T Unlock_Motor;                 /* '<Root>/Unlock_Motor' */
  real_T User_ControlMode;             /* '<Root>/User_ControlMode' */
  real_T User_Posdeg;                  /* '<Root>/User_Pos [deg]' */
  real_T User_Velrpm;                  /* '<Root>/User_Vel [rpm]' */
  real_T User_TorqueNm;                /* '<Root>/User_Torque [Nm]' */
  real_T torque_meas_Nm;               /* '<Root>/torque_meas_Nm' */
  real_T vel_meas_rads;                /* '<Root>/vel_meas_rads' */
  real_T pos_meas_rad;                 /* '<Root>/pos_meas_rad' */
} ExtU_Control_System_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Output_Torque;                /* '<Root>/Output_Torque' */
} ExtY_Control_System_T;

/* Real-time Model Data Structure */
struct tag_RTM_Control_System_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_Control_System_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[5];
  real_T odeF[3][5];
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

/* Block signals (default storage) */
extern B_Control_System_T Control_System_B;

/* Continuous states (default storage) */
extern X_Control_System_T Control_System_X;

/* Block states (default storage) */
extern DW_Control_System_T Control_System_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Control_System_T Control_System_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Control_System_T Control_System_Y;

/* Model entry point functions */
extern void Control_System_initialize(void);
extern void Control_System_step(void);
extern void Control_System_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Control_System_T *const Control_System_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Scope' : Unused code path elimination
 * Block '<S4>/Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Control_System'
 * '<S1>'   : 'Control_System/Degrees to Radians'
 * '<S2>'   : 'Control_System/EnableMotor'
 * '<S3>'   : 'Control_System/Measurement Function'
 * '<S4>'   : 'Control_System/Subsystem'
 * '<S5>'   : 'Control_System/Subsystem1'
 * '<S6>'   : 'Control_System/Torque Limitation'
 * '<S7>'   : 'Control_System/User Input Function'
 * '<S8>'   : 'Control_System/Subsystem/Compare To Constant'
 * '<S9>'   : 'Control_System/Subsystem/Compare To Constant1'
 */
#endif                                 /* RTW_HEADER_Control_System_h_ */
