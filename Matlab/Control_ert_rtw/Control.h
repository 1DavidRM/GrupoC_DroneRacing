/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 *
 * File: Control.h
 *
 * Code generated for Simulink model 'Control'.
 *
 * Model version                  : 1.22
 * Simulink Coder version         : 26.2 (R2026b) 22-May-2026
 * C/C++ source code generated on : Wed Jul 22 20:13:16 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#ifndef Control_h_
#define Control_h_
#ifndef Control_COMMON_INCLUDES_
#define Control_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif                                 /* Control_COMMON_INCLUDES_ */

#include <string.h>

/* Macros for accessing real-time model data structure */
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

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  double Angulos_deseados[3];          /* '<S1>/MATLAB Function1' */
  double FilterCoefficient;            /* '<S47>/Filter Coefficient' */
  double FilterCoefficient_c;          /* '<S103>/Filter Coefficient' */
  double FilterCoefficient_e;          /* '<S215>/Filter Coefficient' */
  double FilterCoefficient_m;          /* '<S159>/Filter Coefficient' */
  double IntegralGain;                 /* '<S41>/Integral Gain' */
  double IntegralGain_h;               /* '<S97>/Integral Gain' */
  double IntegralGain_e;               /* '<S153>/Integral Gain' */
  double IntegralGain_i;               /* '<S209>/Integral Gain' */
  double Vz_deseada;                   /* '<S1>/MATLAB Function1' */
} DW;

/* Continuous states (default storage) */
typedef struct {
  double Integrator_CSTATE;            /* '<S44>/Integrator' */
  double Filter_CSTATE;                /* '<S39>/Filter' */
  double Integrator_CSTATE_l;          /* '<S100>/Integrator' */
  double Filter_CSTATE_g;              /* '<S95>/Filter' */
  double Integrator_CSTATE_g;          /* '<S212>/Integrator' */
  double Filter_CSTATE_j;              /* '<S207>/Filter' */
  double Integrator_CSTATE_m;          /* '<S156>/Integrator' */
  double Filter_CSTATE_go;             /* '<S151>/Filter' */
} X;

/* State derivatives (default storage) */
typedef struct {
  double Integrator_CSTATE;            /* '<S44>/Integrator' */
  double Filter_CSTATE;                /* '<S39>/Filter' */
  double Integrator_CSTATE_l;          /* '<S100>/Integrator' */
  double Filter_CSTATE_g;              /* '<S95>/Filter' */
  double Integrator_CSTATE_g;          /* '<S212>/Integrator' */
  double Filter_CSTATE_j;              /* '<S207>/Filter' */
  double Integrator_CSTATE_m;          /* '<S156>/Integrator' */
  double Filter_CSTATE_go;             /* '<S151>/Filter' */
} XDot;

/* State disabled  */
typedef struct {
  bool Integrator_CSTATE;              /* '<S44>/Integrator' */
  bool Filter_CSTATE;                  /* '<S39>/Filter' */
  bool Integrator_CSTATE_l;            /* '<S100>/Integrator' */
  bool Filter_CSTATE_g;                /* '<S95>/Filter' */
  bool Integrator_CSTATE_g;            /* '<S212>/Integrator' */
  bool Filter_CSTATE_j;                /* '<S207>/Filter' */
  bool Integrator_CSTATE_m;            /* '<S156>/Integrator' */
  bool Filter_CSTATE_go;               /* '<S151>/Filter' */
} XDis;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  double *y;                           /* output */
  double *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  double CMD_porcentaje[4];            /* '<Root>/Señal de control' */
  double Angulosreales[3];             /* '<Root>/Angulos reales' */
  double VelocidadRealz;               /* '<Root>/Velocidad Real z' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  double F1;                           /* '<Root>/F1' */
  double F2;                           /* '<Root>/F2' */
  double F3;                           /* '<Root>/F3' */
  double F4;                           /* '<Root>/F4' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char *errorStatus;
  RTWSolverInfo solverInfo;
  X *contStates;
  int *periodicContStateIndices;
  double *periodicContStateRanges;
  double *derivs;
  XDis *contStateDisabled;
  bool zCCacheNeedsReset;
  bool derivCacheNeedsReset;
  bool CTOutputIncnstWithState;
  double odeY[8];
  double odeF[4][8];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int numContStates;
    int numPeriodicContStates;
    int numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_t clockTick0;
    double stepSize0;
    uint32_t clockTick1;
    struct {
      uint8_t TID[2];
    } TaskCounters;

    double tStart;
    SimTimeStep simTimeStep;
    bool stopRequestedFlag;
    double *t;
    double tArray[2];
  } Timing;
};

/* Continuous states (default storage) */
extern X rtX;

/* Disabled states (default storage) */
extern XDis rtXDis;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void Control_initialize(void);
extern void Control_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope1' : Unused code path elimination
 * Block '<S59>/Setpoint Weighting (Proportional)' : Eliminated nontunable gain of 1
 * Block '<S60>/Setpoint Weighting (Derivative)' : Eliminated nontunable gain of 1
 * Block '<S115>/Setpoint Weighting (Proportional)' : Eliminated nontunable gain of 1
 * Block '<S116>/Setpoint Weighting (Derivative)' : Eliminated nontunable gain of 1
 * Block '<S171>/Setpoint Weighting (Proportional)' : Eliminated nontunable gain of 1
 * Block '<S172>/Setpoint Weighting (Derivative)' : Eliminated nontunable gain of 1
 * Block '<S227>/Setpoint Weighting (Proportional)' : Eliminated nontunable gain of 1
 * Block '<S228>/Setpoint Weighting (Derivative)' : Eliminated nontunable gain of 1
 */

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
 * hilite_system('Panchito_Modelo_Matematico/Control')    - opens subsystem Panchito_Modelo_Matematico/Control
 * hilite_system('Panchito_Modelo_Matematico/Control/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Panchito_Modelo_Matematico'
 * '<S1>'   : 'Panchito_Modelo_Matematico/Control'
 * '<S2>'   : 'Panchito_Modelo_Matematico/Control/MATLAB Function'
 * '<S3>'   : 'Panchito_Modelo_Matematico/Control/MATLAB Function1'
 * '<S4>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1'
 * '<S5>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2'
 * '<S6>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3'
 * '<S7>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4'
 * '<S8>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Anti-windup'
 * '<S9>'   : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/D Gain'
 * '<S10>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/External Derivative'
 * '<S11>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Filter'
 * '<S12>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Filter ICs'
 * '<S13>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/I Gain'
 * '<S14>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Ideal P Gain'
 * '<S15>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Ideal P Gain Fdbk'
 * '<S16>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Integrator'
 * '<S17>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Integrator ICs'
 * '<S18>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/N Copy'
 * '<S19>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/N Gain'
 * '<S20>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/P Copy'
 * '<S21>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Parallel P Gain'
 * '<S22>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Reset Signal'
 * '<S23>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Saturation'
 * '<S24>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Saturation Fdbk'
 * '<S25>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Sum'
 * '<S26>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Sum Fdbk'
 * '<S27>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tracking Mode'
 * '<S28>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tracking Mode Sum'
 * '<S29>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tsamp - Integral'
 * '<S30>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tsamp - Ngain'
 * '<S31>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/b Gain'
 * '<S32>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/c Gain'
 * '<S33>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/postSat Signal'
 * '<S34>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/preInt Signal'
 * '<S35>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/preSat Signal'
 * '<S36>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Anti-windup/Passthrough'
 * '<S37>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/D Gain/Internal Parameters'
 * '<S38>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/External Derivative/Error'
 * '<S39>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Filter/Cont. Filter'
 * '<S40>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Filter ICs/Internal IC - Filter'
 * '<S41>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/I Gain/Internal Parameters'
 * '<S42>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Ideal P Gain/Passthrough'
 * '<S43>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Ideal P Gain Fdbk/Disabled'
 * '<S44>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Integrator/Continuous'
 * '<S45>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Integrator ICs/Internal IC'
 * '<S46>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/N Copy/Disabled'
 * '<S47>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/N Gain/Internal Parameters'
 * '<S48>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/P Copy/Disabled'
 * '<S49>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Parallel P Gain/Internal Parameters'
 * '<S50>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Reset Signal/Disabled'
 * '<S51>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Saturation/Passthrough'
 * '<S52>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Saturation Fdbk/Disabled'
 * '<S53>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Sum/Sum_PID'
 * '<S54>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Sum Fdbk/Disabled'
 * '<S55>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tracking Mode/Disabled'
 * '<S56>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tracking Mode Sum/Passthrough'
 * '<S57>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tsamp - Integral/TsSignalSpecification'
 * '<S58>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/Tsamp - Ngain/Passthrough'
 * '<S59>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/b Gain/Internal Parameters'
 * '<S60>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/c Gain/Internal Parameters'
 * '<S61>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/postSat Signal/Forward_Path'
 * '<S62>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/preInt Signal/Internal PreInt'
 * '<S63>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)1/preSat Signal/Forward_Path'
 * '<S64>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Anti-windup'
 * '<S65>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/D Gain'
 * '<S66>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/External Derivative'
 * '<S67>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Filter'
 * '<S68>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Filter ICs'
 * '<S69>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/I Gain'
 * '<S70>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Ideal P Gain'
 * '<S71>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Ideal P Gain Fdbk'
 * '<S72>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Integrator'
 * '<S73>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Integrator ICs'
 * '<S74>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/N Copy'
 * '<S75>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/N Gain'
 * '<S76>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/P Copy'
 * '<S77>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Parallel P Gain'
 * '<S78>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Reset Signal'
 * '<S79>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Saturation'
 * '<S80>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Saturation Fdbk'
 * '<S81>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Sum'
 * '<S82>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Sum Fdbk'
 * '<S83>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tracking Mode'
 * '<S84>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tracking Mode Sum'
 * '<S85>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tsamp - Integral'
 * '<S86>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tsamp - Ngain'
 * '<S87>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/b Gain'
 * '<S88>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/c Gain'
 * '<S89>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/postSat Signal'
 * '<S90>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/preInt Signal'
 * '<S91>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/preSat Signal'
 * '<S92>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Anti-windup/Passthrough'
 * '<S93>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/D Gain/Internal Parameters'
 * '<S94>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/External Derivative/Error'
 * '<S95>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Filter/Cont. Filter'
 * '<S96>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Filter ICs/Internal IC - Filter'
 * '<S97>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/I Gain/Internal Parameters'
 * '<S98>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Ideal P Gain/Passthrough'
 * '<S99>'  : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Ideal P Gain Fdbk/Disabled'
 * '<S100>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Integrator/Continuous'
 * '<S101>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Integrator ICs/Internal IC'
 * '<S102>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/N Copy/Disabled'
 * '<S103>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/N Gain/Internal Parameters'
 * '<S104>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/P Copy/Disabled'
 * '<S105>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Parallel P Gain/Internal Parameters'
 * '<S106>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Reset Signal/Disabled'
 * '<S107>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Saturation/Passthrough'
 * '<S108>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Saturation Fdbk/Disabled'
 * '<S109>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Sum/Sum_PID'
 * '<S110>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Sum Fdbk/Disabled'
 * '<S111>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tracking Mode/Disabled'
 * '<S112>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tracking Mode Sum/Passthrough'
 * '<S113>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tsamp - Integral/TsSignalSpecification'
 * '<S114>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/Tsamp - Ngain/Passthrough'
 * '<S115>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/b Gain/Internal Parameters'
 * '<S116>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/c Gain/Internal Parameters'
 * '<S117>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/postSat Signal/Forward_Path'
 * '<S118>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/preInt Signal/Internal PreInt'
 * '<S119>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)2/preSat Signal/Forward_Path'
 * '<S120>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Anti-windup'
 * '<S121>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/D Gain'
 * '<S122>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/External Derivative'
 * '<S123>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Filter'
 * '<S124>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Filter ICs'
 * '<S125>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/I Gain'
 * '<S126>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Ideal P Gain'
 * '<S127>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Ideal P Gain Fdbk'
 * '<S128>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Integrator'
 * '<S129>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Integrator ICs'
 * '<S130>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/N Copy'
 * '<S131>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/N Gain'
 * '<S132>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/P Copy'
 * '<S133>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Parallel P Gain'
 * '<S134>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Reset Signal'
 * '<S135>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Saturation'
 * '<S136>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Saturation Fdbk'
 * '<S137>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Sum'
 * '<S138>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Sum Fdbk'
 * '<S139>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tracking Mode'
 * '<S140>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tracking Mode Sum'
 * '<S141>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tsamp - Integral'
 * '<S142>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tsamp - Ngain'
 * '<S143>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/b Gain'
 * '<S144>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/c Gain'
 * '<S145>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/postSat Signal'
 * '<S146>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/preInt Signal'
 * '<S147>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/preSat Signal'
 * '<S148>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Anti-windup/Passthrough'
 * '<S149>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/D Gain/Internal Parameters'
 * '<S150>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/External Derivative/Error'
 * '<S151>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Filter/Cont. Filter'
 * '<S152>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Filter ICs/Internal IC - Filter'
 * '<S153>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/I Gain/Internal Parameters'
 * '<S154>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Ideal P Gain/Passthrough'
 * '<S155>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Ideal P Gain Fdbk/Disabled'
 * '<S156>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Integrator/Continuous'
 * '<S157>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Integrator ICs/Internal IC'
 * '<S158>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/N Copy/Disabled'
 * '<S159>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/N Gain/Internal Parameters'
 * '<S160>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/P Copy/Disabled'
 * '<S161>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Parallel P Gain/Internal Parameters'
 * '<S162>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Reset Signal/Disabled'
 * '<S163>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Saturation/Passthrough'
 * '<S164>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Saturation Fdbk/Disabled'
 * '<S165>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Sum/Sum_PID'
 * '<S166>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Sum Fdbk/Disabled'
 * '<S167>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tracking Mode/Disabled'
 * '<S168>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tracking Mode Sum/Passthrough'
 * '<S169>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tsamp - Integral/TsSignalSpecification'
 * '<S170>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/Tsamp - Ngain/Passthrough'
 * '<S171>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/b Gain/Internal Parameters'
 * '<S172>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/c Gain/Internal Parameters'
 * '<S173>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/postSat Signal/Forward_Path'
 * '<S174>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/preInt Signal/Internal PreInt'
 * '<S175>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)3/preSat Signal/Forward_Path'
 * '<S176>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Anti-windup'
 * '<S177>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/D Gain'
 * '<S178>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/External Derivative'
 * '<S179>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Filter'
 * '<S180>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Filter ICs'
 * '<S181>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/I Gain'
 * '<S182>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Ideal P Gain'
 * '<S183>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Ideal P Gain Fdbk'
 * '<S184>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Integrator'
 * '<S185>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Integrator ICs'
 * '<S186>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/N Copy'
 * '<S187>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/N Gain'
 * '<S188>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/P Copy'
 * '<S189>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Parallel P Gain'
 * '<S190>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Reset Signal'
 * '<S191>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Saturation'
 * '<S192>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Saturation Fdbk'
 * '<S193>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Sum'
 * '<S194>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Sum Fdbk'
 * '<S195>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tracking Mode'
 * '<S196>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tracking Mode Sum'
 * '<S197>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tsamp - Integral'
 * '<S198>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tsamp - Ngain'
 * '<S199>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/b Gain'
 * '<S200>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/c Gain'
 * '<S201>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/postSat Signal'
 * '<S202>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/preInt Signal'
 * '<S203>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/preSat Signal'
 * '<S204>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Anti-windup/Passthrough'
 * '<S205>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/D Gain/Internal Parameters'
 * '<S206>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/External Derivative/Error'
 * '<S207>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Filter/Cont. Filter'
 * '<S208>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Filter ICs/Internal IC - Filter'
 * '<S209>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/I Gain/Internal Parameters'
 * '<S210>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Ideal P Gain/Passthrough'
 * '<S211>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Ideal P Gain Fdbk/Disabled'
 * '<S212>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Integrator/Continuous'
 * '<S213>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Integrator ICs/Internal IC'
 * '<S214>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/N Copy/Disabled'
 * '<S215>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/N Gain/Internal Parameters'
 * '<S216>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/P Copy/Disabled'
 * '<S217>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Parallel P Gain/Internal Parameters'
 * '<S218>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Reset Signal/Disabled'
 * '<S219>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Saturation/Passthrough'
 * '<S220>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Saturation Fdbk/Disabled'
 * '<S221>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Sum/Sum_PID'
 * '<S222>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Sum Fdbk/Disabled'
 * '<S223>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tracking Mode/Disabled'
 * '<S224>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tracking Mode Sum/Passthrough'
 * '<S225>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tsamp - Integral/TsSignalSpecification'
 * '<S226>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/Tsamp - Ngain/Passthrough'
 * '<S227>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/b Gain/Internal Parameters'
 * '<S228>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/c Gain/Internal Parameters'
 * '<S229>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/postSat Signal/Forward_Path'
 * '<S230>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/preInt Signal/Internal PreInt'
 * '<S231>' : 'Panchito_Modelo_Matematico/Control/PID Controller (2DOF)4/preSat Signal/Forward_Path'
 */
#endif                                 /* Control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
