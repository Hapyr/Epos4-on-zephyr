/*
 * Code generation for system model 'PositionControl'
 * For more details, see corresponding source file PositionControl.c
 *
 */

#ifndef RTW_HEADER_PositionControl_h_
#define RTW_HEADER_PositionControl_h_
#include <string.h>
#ifndef PositionControl_COMMON_INCLUDES_
# define PositionControl_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* PositionControl_COMMON_INCLUDES_ */

#include "PositionControl_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Block signals for model 'PositionControl' */
typedef struct {
  real_T FilterCoefficient;            /* '<S67>/Filter Coefficient' */
  real_T FilterCoefficient_a;          /* '<S163>/Filter Coefficient' */
  real_T Switch;                       /* '<S123>/Switch' */
  boolean_T AND3;                      /* '<S123>/AND3' */
} B_PositionControl_c_T;

/* Block states (default storage) for model 'PositionControl' */
typedef struct {
  boolean_T Memory_PreviousInput;      /* '<S123>/Memory' */
  boolean_T TmpModelReferenceSubsystem_MODE;/* synthesized block */
} DW_PositionControl_f_T;

/* Continuous states for model 'PositionControl' */
typedef struct {
  real_T Filter_CSTATE;                /* '<S35>/Filter' */
  real_T Integrator_CSTATE;            /* '<S151>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S131>/Filter' */
} X_PositionControl_n_T;

/* State derivatives for model 'PositionControl' */
typedef struct {
  real_T Filter_CSTATE;                /* '<S35>/Filter' */
  real_T Integrator_CSTATE;            /* '<S151>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S131>/Filter' */
} XDot_PositionControl_n_T;

/* State Disabled for model 'PositionControl' */
typedef struct {
  boolean_T Filter_CSTATE;             /* '<S35>/Filter' */
  boolean_T Integrator_CSTATE;         /* '<S151>/Integrator' */
  boolean_T Filter_CSTATE_f;           /* '<S131>/Filter' */
} XDis_PositionControl_n_T;

/* Real-time Model Data Structure */
struct tag_RTM_PositionControl_T {
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
  B_PositionControl_c_T rtb;
  DW_PositionControl_f_T rtdw;
  RT_MODEL_PositionControl_T rtm;
} MdlrefDW_PositionControl_T;

/* Model reference registration function */
extern void PositionControl_initialize(const char_T **rt_errorStatus, boolean_T *
  rt_stopRequested, RTWSolverInfo *rt_solverInfo, RT_MODEL_PositionControl_T *
  const PositionControl_M, B_PositionControl_c_T *localB, DW_PositionControl_f_T
  *localDW);
extern void PositionControl_Init(real_T *rty_dphi_refrads,
  DW_PositionControl_f_T *localDW, X_PositionControl_n_T *localX);
extern void PositionControl_Reset(DW_PositionControl_f_T *localDW,
  X_PositionControl_n_T *localX);
extern void PositionControl_Deriv(B_PositionControl_c_T *localB,
  DW_PositionControl_f_T *localDW, XDot_PositionControl_n_T *localXdot);
extern void PositionControl_Disable(DW_PositionControl_f_T *localDW);
extern void PositionControl_Update(RT_MODEL_PositionControl_T * const
  PositionControl_M, B_PositionControl_c_T *localB, DW_PositionControl_f_T
  *localDW);
extern void PositionControl(RT_MODEL_PositionControl_T * const PositionControl_M,
  const real_T *rtu_phi_refrad, const real_T *rtu_phirad, const uint8_T
  *rtu_Enable, real_T *rty_dphi_refrads, B_PositionControl_c_T *localB,
  DW_PositionControl_f_T *localDW, X_PositionControl_n_T *localX);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S174>/Saturation' : Unused code path elimination
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
 * '<Root>' : 'PositionControl'
 * '<S1>'   : 'PositionControl/PID Controller'
 * '<S2>'   : 'PositionControl/PID Controller1'
 * '<S3>'   : 'PositionControl/PID Controller/Anti-windup'
 * '<S4>'   : 'PositionControl/PID Controller/D Gain'
 * '<S5>'   : 'PositionControl/PID Controller/Filter'
 * '<S6>'   : 'PositionControl/PID Controller/Filter ICs'
 * '<S7>'   : 'PositionControl/PID Controller/I Gain'
 * '<S8>'   : 'PositionControl/PID Controller/Ideal P Gain'
 * '<S9>'   : 'PositionControl/PID Controller/Ideal P Gain Fdbk'
 * '<S10>'  : 'PositionControl/PID Controller/Integrator'
 * '<S11>'  : 'PositionControl/PID Controller/Integrator ICs'
 * '<S12>'  : 'PositionControl/PID Controller/N Copy'
 * '<S13>'  : 'PositionControl/PID Controller/N Gain'
 * '<S14>'  : 'PositionControl/PID Controller/P Copy'
 * '<S15>'  : 'PositionControl/PID Controller/Parallel P Gain'
 * '<S16>'  : 'PositionControl/PID Controller/Reset Signal'
 * '<S17>'  : 'PositionControl/PID Controller/Saturation'
 * '<S18>'  : 'PositionControl/PID Controller/Saturation Fdbk'
 * '<S19>'  : 'PositionControl/PID Controller/Sum'
 * '<S20>'  : 'PositionControl/PID Controller/Sum Fdbk'
 * '<S21>'  : 'PositionControl/PID Controller/Tracking Mode'
 * '<S22>'  : 'PositionControl/PID Controller/Tracking Mode Sum'
 * '<S23>'  : 'PositionControl/PID Controller/postSat Signal'
 * '<S24>'  : 'PositionControl/PID Controller/preSat Signal'
 * '<S25>'  : 'PositionControl/PID Controller/Anti-windup/Back Calculation'
 * '<S26>'  : 'PositionControl/PID Controller/Anti-windup/Cont. Clamping Ideal'
 * '<S27>'  : 'PositionControl/PID Controller/Anti-windup/Cont. Clamping Parallel'
 * '<S28>'  : 'PositionControl/PID Controller/Anti-windup/Disabled'
 * '<S29>'  : 'PositionControl/PID Controller/Anti-windup/Disc. Clamping Ideal'
 * '<S30>'  : 'PositionControl/PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S31>'  : 'PositionControl/PID Controller/Anti-windup/Passthrough'
 * '<S32>'  : 'PositionControl/PID Controller/D Gain/Disabled'
 * '<S33>'  : 'PositionControl/PID Controller/D Gain/External Parameters'
 * '<S34>'  : 'PositionControl/PID Controller/D Gain/Internal Parameters'
 * '<S35>'  : 'PositionControl/PID Controller/Filter/Cont. Filter'
 * '<S36>'  : 'PositionControl/PID Controller/Filter/Differentiator'
 * '<S37>'  : 'PositionControl/PID Controller/Filter/Disabled'
 * '<S38>'  : 'PositionControl/PID Controller/Filter/Disc. Backward Euler Filter'
 * '<S39>'  : 'PositionControl/PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S40>'  : 'PositionControl/PID Controller/Filter/Disc. Trapezoidal Filter'
 * '<S41>'  : 'PositionControl/PID Controller/Filter ICs/Disabled'
 * '<S42>'  : 'PositionControl/PID Controller/Filter ICs/External IC'
 * '<S43>'  : 'PositionControl/PID Controller/Filter ICs/Internal IC - Differentiator'
 * '<S44>'  : 'PositionControl/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S45>'  : 'PositionControl/PID Controller/I Gain/Disabled'
 * '<S46>'  : 'PositionControl/PID Controller/I Gain/External Parameters'
 * '<S47>'  : 'PositionControl/PID Controller/I Gain/Internal Parameters'
 * '<S48>'  : 'PositionControl/PID Controller/Ideal P Gain/External Parameters'
 * '<S49>'  : 'PositionControl/PID Controller/Ideal P Gain/Internal Parameters'
 * '<S50>'  : 'PositionControl/PID Controller/Ideal P Gain/Passthrough'
 * '<S51>'  : 'PositionControl/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S52>'  : 'PositionControl/PID Controller/Ideal P Gain Fdbk/External Parameters'
 * '<S53>'  : 'PositionControl/PID Controller/Ideal P Gain Fdbk/Internal Parameters'
 * '<S54>'  : 'PositionControl/PID Controller/Ideal P Gain Fdbk/Passthrough'
 * '<S55>'  : 'PositionControl/PID Controller/Integrator/Continuous'
 * '<S56>'  : 'PositionControl/PID Controller/Integrator/Disabled'
 * '<S57>'  : 'PositionControl/PID Controller/Integrator/Discrete'
 * '<S58>'  : 'PositionControl/PID Controller/Integrator ICs/Disabled'
 * '<S59>'  : 'PositionControl/PID Controller/Integrator ICs/External IC'
 * '<S60>'  : 'PositionControl/PID Controller/Integrator ICs/Internal IC'
 * '<S61>'  : 'PositionControl/PID Controller/N Copy/Disabled'
 * '<S62>'  : 'PositionControl/PID Controller/N Copy/Disabled wSignal Specification'
 * '<S63>'  : 'PositionControl/PID Controller/N Copy/External Parameters'
 * '<S64>'  : 'PositionControl/PID Controller/N Copy/Internal Parameters'
 * '<S65>'  : 'PositionControl/PID Controller/N Gain/Disabled'
 * '<S66>'  : 'PositionControl/PID Controller/N Gain/External Parameters'
 * '<S67>'  : 'PositionControl/PID Controller/N Gain/Internal Parameters'
 * '<S68>'  : 'PositionControl/PID Controller/N Gain/Passthrough'
 * '<S69>'  : 'PositionControl/PID Controller/P Copy/Disabled'
 * '<S70>'  : 'PositionControl/PID Controller/P Copy/External Parameters Ideal'
 * '<S71>'  : 'PositionControl/PID Controller/P Copy/Internal Parameters Ideal'
 * '<S72>'  : 'PositionControl/PID Controller/Parallel P Gain/Disabled'
 * '<S73>'  : 'PositionControl/PID Controller/Parallel P Gain/External Parameters'
 * '<S74>'  : 'PositionControl/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S75>'  : 'PositionControl/PID Controller/Parallel P Gain/Passthrough'
 * '<S76>'  : 'PositionControl/PID Controller/Reset Signal/Disabled'
 * '<S77>'  : 'PositionControl/PID Controller/Reset Signal/External Reset'
 * '<S78>'  : 'PositionControl/PID Controller/Saturation/Enabled'
 * '<S79>'  : 'PositionControl/PID Controller/Saturation/Passthrough'
 * '<S80>'  : 'PositionControl/PID Controller/Saturation Fdbk/Disabled'
 * '<S81>'  : 'PositionControl/PID Controller/Saturation Fdbk/Enabled'
 * '<S82>'  : 'PositionControl/PID Controller/Saturation Fdbk/Passthrough'
 * '<S83>'  : 'PositionControl/PID Controller/Sum/Passthrough_I'
 * '<S84>'  : 'PositionControl/PID Controller/Sum/Passthrough_P'
 * '<S85>'  : 'PositionControl/PID Controller/Sum/Sum_PD'
 * '<S86>'  : 'PositionControl/PID Controller/Sum/Sum_PI'
 * '<S87>'  : 'PositionControl/PID Controller/Sum/Sum_PID'
 * '<S88>'  : 'PositionControl/PID Controller/Sum Fdbk/Disabled'
 * '<S89>'  : 'PositionControl/PID Controller/Sum Fdbk/Enabled'
 * '<S90>'  : 'PositionControl/PID Controller/Sum Fdbk/Passthrough'
 * '<S91>'  : 'PositionControl/PID Controller/Tracking Mode/Disabled'
 * '<S92>'  : 'PositionControl/PID Controller/Tracking Mode/Enabled'
 * '<S93>'  : 'PositionControl/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S94>'  : 'PositionControl/PID Controller/Tracking Mode Sum/Tracking Mode'
 * '<S95>'  : 'PositionControl/PID Controller/postSat Signal/Feedback_Path'
 * '<S96>'  : 'PositionControl/PID Controller/postSat Signal/Forward_Path'
 * '<S97>'  : 'PositionControl/PID Controller/preSat Signal/Feedback_Path'
 * '<S98>'  : 'PositionControl/PID Controller/preSat Signal/Forward_Path'
 * '<S99>'  : 'PositionControl/PID Controller1/Anti-windup'
 * '<S100>' : 'PositionControl/PID Controller1/D Gain'
 * '<S101>' : 'PositionControl/PID Controller1/Filter'
 * '<S102>' : 'PositionControl/PID Controller1/Filter ICs'
 * '<S103>' : 'PositionControl/PID Controller1/I Gain'
 * '<S104>' : 'PositionControl/PID Controller1/Ideal P Gain'
 * '<S105>' : 'PositionControl/PID Controller1/Ideal P Gain Fdbk'
 * '<S106>' : 'PositionControl/PID Controller1/Integrator'
 * '<S107>' : 'PositionControl/PID Controller1/Integrator ICs'
 * '<S108>' : 'PositionControl/PID Controller1/N Copy'
 * '<S109>' : 'PositionControl/PID Controller1/N Gain'
 * '<S110>' : 'PositionControl/PID Controller1/P Copy'
 * '<S111>' : 'PositionControl/PID Controller1/Parallel P Gain'
 * '<S112>' : 'PositionControl/PID Controller1/Reset Signal'
 * '<S113>' : 'PositionControl/PID Controller1/Saturation'
 * '<S114>' : 'PositionControl/PID Controller1/Saturation Fdbk'
 * '<S115>' : 'PositionControl/PID Controller1/Sum'
 * '<S116>' : 'PositionControl/PID Controller1/Sum Fdbk'
 * '<S117>' : 'PositionControl/PID Controller1/Tracking Mode'
 * '<S118>' : 'PositionControl/PID Controller1/Tracking Mode Sum'
 * '<S119>' : 'PositionControl/PID Controller1/postSat Signal'
 * '<S120>' : 'PositionControl/PID Controller1/preSat Signal'
 * '<S121>' : 'PositionControl/PID Controller1/Anti-windup/Back Calculation'
 * '<S122>' : 'PositionControl/PID Controller1/Anti-windup/Cont. Clamping Ideal'
 * '<S123>' : 'PositionControl/PID Controller1/Anti-windup/Cont. Clamping Parallel'
 * '<S124>' : 'PositionControl/PID Controller1/Anti-windup/Disabled'
 * '<S125>' : 'PositionControl/PID Controller1/Anti-windup/Disc. Clamping Ideal'
 * '<S126>' : 'PositionControl/PID Controller1/Anti-windup/Disc. Clamping Parallel'
 * '<S127>' : 'PositionControl/PID Controller1/Anti-windup/Passthrough'
 * '<S128>' : 'PositionControl/PID Controller1/D Gain/Disabled'
 * '<S129>' : 'PositionControl/PID Controller1/D Gain/External Parameters'
 * '<S130>' : 'PositionControl/PID Controller1/D Gain/Internal Parameters'
 * '<S131>' : 'PositionControl/PID Controller1/Filter/Cont. Filter'
 * '<S132>' : 'PositionControl/PID Controller1/Filter/Differentiator'
 * '<S133>' : 'PositionControl/PID Controller1/Filter/Disabled'
 * '<S134>' : 'PositionControl/PID Controller1/Filter/Disc. Backward Euler Filter'
 * '<S135>' : 'PositionControl/PID Controller1/Filter/Disc. Forward Euler Filter'
 * '<S136>' : 'PositionControl/PID Controller1/Filter/Disc. Trapezoidal Filter'
 * '<S137>' : 'PositionControl/PID Controller1/Filter ICs/Disabled'
 * '<S138>' : 'PositionControl/PID Controller1/Filter ICs/External IC'
 * '<S139>' : 'PositionControl/PID Controller1/Filter ICs/Internal IC - Differentiator'
 * '<S140>' : 'PositionControl/PID Controller1/Filter ICs/Internal IC - Filter'
 * '<S141>' : 'PositionControl/PID Controller1/I Gain/Disabled'
 * '<S142>' : 'PositionControl/PID Controller1/I Gain/External Parameters'
 * '<S143>' : 'PositionControl/PID Controller1/I Gain/Internal Parameters'
 * '<S144>' : 'PositionControl/PID Controller1/Ideal P Gain/External Parameters'
 * '<S145>' : 'PositionControl/PID Controller1/Ideal P Gain/Internal Parameters'
 * '<S146>' : 'PositionControl/PID Controller1/Ideal P Gain/Passthrough'
 * '<S147>' : 'PositionControl/PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S148>' : 'PositionControl/PID Controller1/Ideal P Gain Fdbk/External Parameters'
 * '<S149>' : 'PositionControl/PID Controller1/Ideal P Gain Fdbk/Internal Parameters'
 * '<S150>' : 'PositionControl/PID Controller1/Ideal P Gain Fdbk/Passthrough'
 * '<S151>' : 'PositionControl/PID Controller1/Integrator/Continuous'
 * '<S152>' : 'PositionControl/PID Controller1/Integrator/Disabled'
 * '<S153>' : 'PositionControl/PID Controller1/Integrator/Discrete'
 * '<S154>' : 'PositionControl/PID Controller1/Integrator ICs/Disabled'
 * '<S155>' : 'PositionControl/PID Controller1/Integrator ICs/External IC'
 * '<S156>' : 'PositionControl/PID Controller1/Integrator ICs/Internal IC'
 * '<S157>' : 'PositionControl/PID Controller1/N Copy/Disabled'
 * '<S158>' : 'PositionControl/PID Controller1/N Copy/Disabled wSignal Specification'
 * '<S159>' : 'PositionControl/PID Controller1/N Copy/External Parameters'
 * '<S160>' : 'PositionControl/PID Controller1/N Copy/Internal Parameters'
 * '<S161>' : 'PositionControl/PID Controller1/N Gain/Disabled'
 * '<S162>' : 'PositionControl/PID Controller1/N Gain/External Parameters'
 * '<S163>' : 'PositionControl/PID Controller1/N Gain/Internal Parameters'
 * '<S164>' : 'PositionControl/PID Controller1/N Gain/Passthrough'
 * '<S165>' : 'PositionControl/PID Controller1/P Copy/Disabled'
 * '<S166>' : 'PositionControl/PID Controller1/P Copy/External Parameters Ideal'
 * '<S167>' : 'PositionControl/PID Controller1/P Copy/Internal Parameters Ideal'
 * '<S168>' : 'PositionControl/PID Controller1/Parallel P Gain/Disabled'
 * '<S169>' : 'PositionControl/PID Controller1/Parallel P Gain/External Parameters'
 * '<S170>' : 'PositionControl/PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S171>' : 'PositionControl/PID Controller1/Parallel P Gain/Passthrough'
 * '<S172>' : 'PositionControl/PID Controller1/Reset Signal/Disabled'
 * '<S173>' : 'PositionControl/PID Controller1/Reset Signal/External Reset'
 * '<S174>' : 'PositionControl/PID Controller1/Saturation/Enabled'
 * '<S175>' : 'PositionControl/PID Controller1/Saturation/Passthrough'
 * '<S176>' : 'PositionControl/PID Controller1/Saturation Fdbk/Disabled'
 * '<S177>' : 'PositionControl/PID Controller1/Saturation Fdbk/Enabled'
 * '<S178>' : 'PositionControl/PID Controller1/Saturation Fdbk/Passthrough'
 * '<S179>' : 'PositionControl/PID Controller1/Sum/Passthrough_I'
 * '<S180>' : 'PositionControl/PID Controller1/Sum/Passthrough_P'
 * '<S181>' : 'PositionControl/PID Controller1/Sum/Sum_PD'
 * '<S182>' : 'PositionControl/PID Controller1/Sum/Sum_PI'
 * '<S183>' : 'PositionControl/PID Controller1/Sum/Sum_PID'
 * '<S184>' : 'PositionControl/PID Controller1/Sum Fdbk/Disabled'
 * '<S185>' : 'PositionControl/PID Controller1/Sum Fdbk/Enabled'
 * '<S186>' : 'PositionControl/PID Controller1/Sum Fdbk/Passthrough'
 * '<S187>' : 'PositionControl/PID Controller1/Tracking Mode/Disabled'
 * '<S188>' : 'PositionControl/PID Controller1/Tracking Mode/Enabled'
 * '<S189>' : 'PositionControl/PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S190>' : 'PositionControl/PID Controller1/Tracking Mode Sum/Tracking Mode'
 * '<S191>' : 'PositionControl/PID Controller1/postSat Signal/Feedback_Path'
 * '<S192>' : 'PositionControl/PID Controller1/postSat Signal/Forward_Path'
 * '<S193>' : 'PositionControl/PID Controller1/preSat Signal/Feedback_Path'
 * '<S194>' : 'PositionControl/PID Controller1/preSat Signal/Forward_Path'
 */
#endif                                 /* RTW_HEADER_PositionControl_h_ */
