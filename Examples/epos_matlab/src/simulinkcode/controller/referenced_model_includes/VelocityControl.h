/*
 * Code generation for system model 'VelocityControl'
 * For more details, see corresponding source file VelocityControl.c
 *
 */

#ifndef RTW_HEADER_VelocityControl_h_
#define RTW_HEADER_VelocityControl_h_
#include <string.h>
#ifndef VelocityControl_COMMON_INCLUDES_
# define VelocityControl_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* VelocityControl_COMMON_INCLUDES_ */

#include "VelocityControl_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Block signals for model 'VelocityControl' */
typedef struct {
  real_T Switch;                       /* '<S26>/Switch' */
  boolean_T AND3;                      /* '<S26>/AND3' */
  boolean_T Memory;                    /* '<S26>/Memory' */
} B_VelocityControl_c_T;

/* Block states (default storage) for model 'VelocityControl' */
typedef struct {
  boolean_T Memory_PreviousInput;      /* '<S26>/Memory' */
  boolean_T TmpModelReferenceSubsystem_MODE;/* synthesized block */
} DW_VelocityControl_f_T;

/* Continuous states for model 'VelocityControl' */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S54>/Integrator' */
} X_VelocityControl_n_T;

/* State derivatives for model 'VelocityControl' */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S54>/Integrator' */
} XDot_VelocityControl_n_T;

/* State Disabled for model 'VelocityControl' */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S54>/Integrator' */
} XDis_VelocityControl_n_T;

/* Real-time Model Data Structure */
struct tag_RTM_VelocityControl_T {
  const char_T **errorStatus;
  RTWSolverInfo *solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T stepSize0;
    SimTimeStep *simTimeStep;
    boolean_T *stopRequestedFlag;
  } Timing;
};

typedef struct {
  B_VelocityControl_c_T rtb;
  DW_VelocityControl_f_T rtdw;
  RT_MODEL_VelocityControl_T rtm;
} MdlrefDW_VelocityControl_T;

/* Model reference registration function */
extern void VelocityControl_initialize(const char_T **rt_errorStatus, boolean_T *
  rt_stopRequested, RTWSolverInfo *rt_solverInfo, RT_MODEL_VelocityControl_T *
  const VelocityControl_M, B_VelocityControl_c_T *localB, DW_VelocityControl_f_T
  *localDW);
extern void VelocityControl_Init(real_T *rty_tau_mNm, DW_VelocityControl_f_T
  *localDW, X_VelocityControl_n_T *localX);
extern void VelocityControl_Reset(DW_VelocityControl_f_T *localDW,
  X_VelocityControl_n_T *localX);
extern void VelocityControl_Deriv(B_VelocityControl_c_T *localB,
  DW_VelocityControl_f_T *localDW, XDot_VelocityControl_n_T *localXdot);
extern void VelocityControl_Disable(DW_VelocityControl_f_T *localDW);
extern void VelocityControl_Update(RT_MODEL_VelocityControl_T * const
  VelocityControl_M, B_VelocityControl_c_T *localB, DW_VelocityControl_f_T
  *localDW);
extern void VelocityControl(RT_MODEL_VelocityControl_T * const VelocityControl_M,
  const real_T *rtu_dphi_refrads, const real_T *rtu_dphirads, const uint8_T
  *rtu_Enable, real_T *rty_tau_mNm, B_VelocityControl_c_T *localB,
  DW_VelocityControl_f_T *localDW, X_VelocityControl_n_T *localX);

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
 * '<Root>' : 'VelocityControl'
 * '<S1>'   : 'VelocityControl/PID Controller'
 * '<S2>'   : 'VelocityControl/PID Controller/Anti-windup'
 * '<S3>'   : 'VelocityControl/PID Controller/D Gain'
 * '<S4>'   : 'VelocityControl/PID Controller/Filter'
 * '<S5>'   : 'VelocityControl/PID Controller/Filter ICs'
 * '<S6>'   : 'VelocityControl/PID Controller/I Gain'
 * '<S7>'   : 'VelocityControl/PID Controller/Ideal P Gain'
 * '<S8>'   : 'VelocityControl/PID Controller/Ideal P Gain Fdbk'
 * '<S9>'   : 'VelocityControl/PID Controller/Integrator'
 * '<S10>'  : 'VelocityControl/PID Controller/Integrator ICs'
 * '<S11>'  : 'VelocityControl/PID Controller/N Copy'
 * '<S12>'  : 'VelocityControl/PID Controller/N Gain'
 * '<S13>'  : 'VelocityControl/PID Controller/P Copy'
 * '<S14>'  : 'VelocityControl/PID Controller/Parallel P Gain'
 * '<S15>'  : 'VelocityControl/PID Controller/Reset Signal'
 * '<S16>'  : 'VelocityControl/PID Controller/Saturation'
 * '<S17>'  : 'VelocityControl/PID Controller/Saturation Fdbk'
 * '<S18>'  : 'VelocityControl/PID Controller/Sum'
 * '<S19>'  : 'VelocityControl/PID Controller/Sum Fdbk'
 * '<S20>'  : 'VelocityControl/PID Controller/Tracking Mode'
 * '<S21>'  : 'VelocityControl/PID Controller/Tracking Mode Sum'
 * '<S22>'  : 'VelocityControl/PID Controller/postSat Signal'
 * '<S23>'  : 'VelocityControl/PID Controller/preSat Signal'
 * '<S24>'  : 'VelocityControl/PID Controller/Anti-windup/Back Calculation'
 * '<S25>'  : 'VelocityControl/PID Controller/Anti-windup/Cont. Clamping Ideal'
 * '<S26>'  : 'VelocityControl/PID Controller/Anti-windup/Cont. Clamping Parallel'
 * '<S27>'  : 'VelocityControl/PID Controller/Anti-windup/Disabled'
 * '<S28>'  : 'VelocityControl/PID Controller/Anti-windup/Disc. Clamping Ideal'
 * '<S29>'  : 'VelocityControl/PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S30>'  : 'VelocityControl/PID Controller/Anti-windup/Passthrough'
 * '<S31>'  : 'VelocityControl/PID Controller/D Gain/Disabled'
 * '<S32>'  : 'VelocityControl/PID Controller/D Gain/External Parameters'
 * '<S33>'  : 'VelocityControl/PID Controller/D Gain/Internal Parameters'
 * '<S34>'  : 'VelocityControl/PID Controller/Filter/Cont. Filter'
 * '<S35>'  : 'VelocityControl/PID Controller/Filter/Differentiator'
 * '<S36>'  : 'VelocityControl/PID Controller/Filter/Disabled'
 * '<S37>'  : 'VelocityControl/PID Controller/Filter/Disc. Backward Euler Filter'
 * '<S38>'  : 'VelocityControl/PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S39>'  : 'VelocityControl/PID Controller/Filter/Disc. Trapezoidal Filter'
 * '<S40>'  : 'VelocityControl/PID Controller/Filter ICs/Disabled'
 * '<S41>'  : 'VelocityControl/PID Controller/Filter ICs/External IC'
 * '<S42>'  : 'VelocityControl/PID Controller/Filter ICs/Internal IC - Differentiator'
 * '<S43>'  : 'VelocityControl/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S44>'  : 'VelocityControl/PID Controller/I Gain/Disabled'
 * '<S45>'  : 'VelocityControl/PID Controller/I Gain/External Parameters'
 * '<S46>'  : 'VelocityControl/PID Controller/I Gain/Internal Parameters'
 * '<S47>'  : 'VelocityControl/PID Controller/Ideal P Gain/External Parameters'
 * '<S48>'  : 'VelocityControl/PID Controller/Ideal P Gain/Internal Parameters'
 * '<S49>'  : 'VelocityControl/PID Controller/Ideal P Gain/Passthrough'
 * '<S50>'  : 'VelocityControl/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S51>'  : 'VelocityControl/PID Controller/Ideal P Gain Fdbk/External Parameters'
 * '<S52>'  : 'VelocityControl/PID Controller/Ideal P Gain Fdbk/Internal Parameters'
 * '<S53>'  : 'VelocityControl/PID Controller/Ideal P Gain Fdbk/Passthrough'
 * '<S54>'  : 'VelocityControl/PID Controller/Integrator/Continuous'
 * '<S55>'  : 'VelocityControl/PID Controller/Integrator/Disabled'
 * '<S56>'  : 'VelocityControl/PID Controller/Integrator/Discrete'
 * '<S57>'  : 'VelocityControl/PID Controller/Integrator ICs/Disabled'
 * '<S58>'  : 'VelocityControl/PID Controller/Integrator ICs/External IC'
 * '<S59>'  : 'VelocityControl/PID Controller/Integrator ICs/Internal IC'
 * '<S60>'  : 'VelocityControl/PID Controller/N Copy/Disabled'
 * '<S61>'  : 'VelocityControl/PID Controller/N Copy/Disabled wSignal Specification'
 * '<S62>'  : 'VelocityControl/PID Controller/N Copy/External Parameters'
 * '<S63>'  : 'VelocityControl/PID Controller/N Copy/Internal Parameters'
 * '<S64>'  : 'VelocityControl/PID Controller/N Gain/Disabled'
 * '<S65>'  : 'VelocityControl/PID Controller/N Gain/External Parameters'
 * '<S66>'  : 'VelocityControl/PID Controller/N Gain/Internal Parameters'
 * '<S67>'  : 'VelocityControl/PID Controller/N Gain/Passthrough'
 * '<S68>'  : 'VelocityControl/PID Controller/P Copy/Disabled'
 * '<S69>'  : 'VelocityControl/PID Controller/P Copy/External Parameters Ideal'
 * '<S70>'  : 'VelocityControl/PID Controller/P Copy/Internal Parameters Ideal'
 * '<S71>'  : 'VelocityControl/PID Controller/Parallel P Gain/Disabled'
 * '<S72>'  : 'VelocityControl/PID Controller/Parallel P Gain/External Parameters'
 * '<S73>'  : 'VelocityControl/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S74>'  : 'VelocityControl/PID Controller/Parallel P Gain/Passthrough'
 * '<S75>'  : 'VelocityControl/PID Controller/Reset Signal/Disabled'
 * '<S76>'  : 'VelocityControl/PID Controller/Reset Signal/External Reset'
 * '<S77>'  : 'VelocityControl/PID Controller/Saturation/Enabled'
 * '<S78>'  : 'VelocityControl/PID Controller/Saturation/Passthrough'
 * '<S79>'  : 'VelocityControl/PID Controller/Saturation Fdbk/Disabled'
 * '<S80>'  : 'VelocityControl/PID Controller/Saturation Fdbk/Enabled'
 * '<S81>'  : 'VelocityControl/PID Controller/Saturation Fdbk/Passthrough'
 * '<S82>'  : 'VelocityControl/PID Controller/Sum/Passthrough_I'
 * '<S83>'  : 'VelocityControl/PID Controller/Sum/Passthrough_P'
 * '<S84>'  : 'VelocityControl/PID Controller/Sum/Sum_PD'
 * '<S85>'  : 'VelocityControl/PID Controller/Sum/Sum_PI'
 * '<S86>'  : 'VelocityControl/PID Controller/Sum/Sum_PID'
 * '<S87>'  : 'VelocityControl/PID Controller/Sum Fdbk/Disabled'
 * '<S88>'  : 'VelocityControl/PID Controller/Sum Fdbk/Enabled'
 * '<S89>'  : 'VelocityControl/PID Controller/Sum Fdbk/Passthrough'
 * '<S90>'  : 'VelocityControl/PID Controller/Tracking Mode/Disabled'
 * '<S91>'  : 'VelocityControl/PID Controller/Tracking Mode/Enabled'
 * '<S92>'  : 'VelocityControl/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S93>'  : 'VelocityControl/PID Controller/Tracking Mode Sum/Tracking Mode'
 * '<S94>'  : 'VelocityControl/PID Controller/postSat Signal/Feedback_Path'
 * '<S95>'  : 'VelocityControl/PID Controller/postSat Signal/Forward_Path'
 * '<S96>'  : 'VelocityControl/PID Controller/preSat Signal/Feedback_Path'
 * '<S97>'  : 'VelocityControl/PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* RTW_HEADER_VelocityControl_h_ */
