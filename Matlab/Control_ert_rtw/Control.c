/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 *
 * File: Control.c
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

#include "Control.h"
#include <emmintrin.h>

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

/* Continuous states */
X rtX;

/* Disabled State Vector */
XDis rtXDis;

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* private model entry point functions */
extern void Control_derivatives(void);

/* Forward declaration for local functions */
static double deg2rad(double angleInDegrees);

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  double t = rtsiGetT(si);
  double tnew = rtsiGetSolverStopTime(si);
  double h = rtsiGetStepSize(si);
  double *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  double *y = id->y;
  double *f0 = id->f[0];
  double *f1 = id->f[1];
  double *f2 = id->f[2];
  double *f3 = id->f[3];
  double temp;
  int i;
  int nXc = 8;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (unsigned int)nXc*sizeof(double));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Control_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  Control_step();
  Control_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  Control_step();
  Control_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  Control_step();
  Control_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Function for MATLAB Function: '<S1>/MATLAB Function1' */
static double deg2rad(double angleInDegrees)
{
  return 0.017453292519943295 * angleInDegrees;
}

/* Model step function */
void Control_step(void)
{
  double tmp[2];
  double F1_tmp;
  double F1_tmp_0;
  double F1_tmp_1;
  double FilterCoefficient_c_tmp;
  double FilterCoefficient_e_tmp;
  double FilterCoefficient_m_tmp;
  double FilterCoefficient_tmp;
  if (rtmIsMajorTimeStep(rtM)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&rtM->solverInfo,((rtM->Timing.clockTick0+1)*
      rtM->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(rtM)) {
    rtM->Timing.t[0] = rtsiGetT(&rtM->solverInfo);
  }

  if (rtmIsMajorTimeStep(rtM) &&
      rtM->Timing.TaskCounters.TID[1] == 0) {
    /* MATLAB Function: '<S1>/MATLAB Function1' incorporates:
     *  Inport: '<Root>/Señal de control'
     */
    rtDW.Vz_deseada = (rtU.CMD_porcentaje[0] - 50.0) / 50.0 * 4.0;

    /* Inport: '<Root>/Señal de control' incorporates:
     *  MATLAB Function: '<S1>/MATLAB Function1'
     */
    _mm_storeu_pd(&rtDW.Angulos_deseados[0], _mm_mul_pd(_mm_div_pd(_mm_loadu_pd(
      &rtU.CMD_porcentaje[1]), _mm_set1_pd(100.0)), _mm_set1_pd(deg2rad(20.0))));

    /* MATLAB Function: '<S1>/MATLAB Function1' incorporates:
     *  Inport: '<Root>/Señal de control'
     */
    rtDW.Angulos_deseados[2] = rtU.CMD_porcentaje[3] / 100.0 * deg2rad(400.0);
  }

  /* Sum: '<S4>/Sum3' incorporates:
   *  Inport: '<Root>/Velocidad Real z'
   *  Sum: '<S4>/Sum1'
   *  Sum: '<S4>/Sum2'
   */
  FilterCoefficient_tmp = rtDW.Vz_deseada - rtU.VelocidadRealz;

  /* Gain: '<S47>/Filter Coefficient' incorporates:
   *  Gain: '<S37>/Derivative Gain'
   *  Integrator: '<S39>/Filter'
   *  Sum: '<S39>/SumD'
   *  Sum: '<S4>/Sum3'
   */
  rtDW.FilterCoefficient = (FilterCoefficient_tmp * 0.09 - rtX.Filter_CSTATE) *
    50.0;

  /* Sum: '<S5>/Sum3' incorporates:
   *  Inport: '<Root>/Angulos reales'
   *  Sum: '<S5>/Sum1'
   */
  FilterCoefficient_c_tmp = rtDW.Angulos_deseados[0] - rtU.Angulosreales[0];

  /* Gain: '<S103>/Filter Coefficient' incorporates:
   *  Gain: '<S93>/Derivative Gain'
   *  Integrator: '<S95>/Filter'
   *  Sum: '<S5>/Sum3'
   *  Sum: '<S95>/SumD'
   */
  rtDW.FilterCoefficient_c = (FilterCoefficient_c_tmp * 3.0 -
    rtX.Filter_CSTATE_g) * 100.0;

  /* Sum: '<S7>/Sum3' incorporates:
   *  Inport: '<Root>/Angulos reales'
   *  Sum: '<S7>/Sum1'
   *  Sum: '<S7>/Sum2'
   */
  FilterCoefficient_e_tmp = rtDW.Angulos_deseados[1] - rtU.Angulosreales[1];

  /* Gain: '<S215>/Filter Coefficient' incorporates:
   *  Gain: '<S205>/Derivative Gain'
   *  Integrator: '<S207>/Filter'
   *  Sum: '<S207>/SumD'
   *  Sum: '<S7>/Sum3'
   */
  rtDW.FilterCoefficient_e = (FilterCoefficient_e_tmp * 12.0 -
    rtX.Filter_CSTATE_j) * 100.0;

  /* Sum: '<S6>/Sum3' incorporates:
   *  Inport: '<Root>/Angulos reales'
   *  Sum: '<S6>/Sum1'
   */
  FilterCoefficient_m_tmp = rtDW.Angulos_deseados[2] - rtU.Angulosreales[2];

  /* Gain: '<S159>/Filter Coefficient' incorporates:
   *  Gain: '<S149>/Derivative Gain'
   *  Integrator: '<S151>/Filter'
   *  Sum: '<S151>/SumD'
   *  Sum: '<S6>/Sum3'
   */
  rtDW.FilterCoefficient_m = (FilterCoefficient_m_tmp * 3.0 -
    rtX.Filter_CSTATE_go) * 100.0;

  /* MATLAB Function: '<S1>/MATLAB Function' incorporates:
   *  Gain: '<S105>/Proportional Gain'
   *  Gain: '<S161>/Proportional Gain'
   *  Gain: '<S217>/Proportional Gain'
   *  Gain: '<S49>/Proportional Gain'
   *  Integrator: '<S100>/Integrator'
   *  Integrator: '<S156>/Integrator'
   *  Integrator: '<S212>/Integrator'
   *  Integrator: '<S44>/Integrator'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S165>/Sum'
   *  Sum: '<S221>/Sum'
   *  Sum: '<S53>/Sum'
   */
  F1_tmp = ((FilterCoefficient_tmp * 0.5 + rtX.Integrator_CSTATE) +
            rtDW.FilterCoefficient) * 0.25;
  F1_tmp_0 = ((FilterCoefficient_c_tmp * 2.0 + rtX.Integrator_CSTATE_l) +
              rtDW.FilterCoefficient_c) * 2.5;
  FilterCoefficient_c_tmp = ((FilterCoefficient_e_tmp * 10.0 +
    rtX.Integrator_CSTATE_g) + rtDW.FilterCoefficient_e) * 2.941176470588235;
  FilterCoefficient_m_tmp = ((FilterCoefficient_m_tmp * 2.0 +
    rtX.Integrator_CSTATE_m) + rtDW.FilterCoefficient_m) * 12.5;
  F1_tmp_1 = F1_tmp + F1_tmp_0;

  /* Outport: '<Root>/F1' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function'
   */
  rtY.F1 = (F1_tmp_1 + FilterCoefficient_c_tmp) + FilterCoefficient_m_tmp;

  /* MATLAB Function: '<S1>/MATLAB Function' */
  F1_tmp -= F1_tmp_0;

  /* Outport: '<Root>/F2' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function'
   */
  rtY.F2 = (F1_tmp + FilterCoefficient_c_tmp) - FilterCoefficient_m_tmp;

  /* Outport: '<Root>/F3' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function'
   */
  rtY.F3 = (F1_tmp - FilterCoefficient_c_tmp) + FilterCoefficient_m_tmp;

  /* Outport: '<Root>/F4' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function'
   */
  rtY.F4 = (F1_tmp_1 - FilterCoefficient_c_tmp) - FilterCoefficient_m_tmp;

  /* Gain: '<S41>/Integral Gain' */
  rtDW.IntegralGain = FilterCoefficient_tmp * 0.15;

  /* Sum: '<S6>/Sum2' incorporates:
   *  Gain: '<S97>/Integral Gain'
   *  Inport: '<Root>/Angulos reales'
   *  Sum: '<S5>/Sum2'
   */
  _mm_storeu_pd(&tmp[0], _mm_mul_pd(_mm_sub_pd(_mm_set_pd(rtDW.Angulos_deseados
    [2], rtDW.Angulos_deseados[0]), _mm_set_pd(rtU.Angulosreales[2],
    rtU.Angulosreales[0])), _mm_set1_pd(0.0)));

  /* Gain: '<S97>/Integral Gain' */
  rtDW.IntegralGain_h = tmp[0];

  /* Gain: '<S153>/Integral Gain' */
  rtDW.IntegralGain_e = tmp[1];

  /* Gain: '<S209>/Integral Gain' */
  rtDW.IntegralGain_i = FilterCoefficient_e_tmp * 0.0;
  if (rtmIsMajorTimeStep(rtM)) {
    rt_ertODEUpdateContinuousStates(&rtM->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++rtM->Timing.clockTick0;
    rtM->Timing.t[0] = rtsiGetSolverStopTime(&rtM->solverInfo);

    {
      /* Update absolute timer for sample time: [0.2s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.2, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      rtM->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Control_derivatives(void)
{
  XDot *_rtXdot;
  _rtXdot = ((XDot *) rtM->derivs);

  /* Derivatives for Integrator: '<S44>/Integrator' */
  _rtXdot->Integrator_CSTATE = rtDW.IntegralGain;

  /* Derivatives for Integrator: '<S39>/Filter' */
  _rtXdot->Filter_CSTATE = rtDW.FilterCoefficient;

  /* Derivatives for Integrator: '<S100>/Integrator' */
  _rtXdot->Integrator_CSTATE_l = rtDW.IntegralGain_h;

  /* Derivatives for Integrator: '<S95>/Filter' */
  _rtXdot->Filter_CSTATE_g = rtDW.FilterCoefficient_c;

  /* Derivatives for Integrator: '<S212>/Integrator' */
  _rtXdot->Integrator_CSTATE_g = rtDW.IntegralGain_i;

  /* Derivatives for Integrator: '<S207>/Filter' */
  _rtXdot->Filter_CSTATE_j = rtDW.FilterCoefficient_e;

  /* Derivatives for Integrator: '<S156>/Integrator' */
  _rtXdot->Integrator_CSTATE_m = rtDW.IntegralGain_e;

  /* Derivatives for Integrator: '<S151>/Filter' */
  _rtXdot->Filter_CSTATE_go = rtDW.FilterCoefficient_m;
}

/* Model initialize function */
void Control_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&rtM->solverInfo, &rtM->Timing.simTimeStep);
    rtsiSetTPtr(&rtM->solverInfo, &rtmGetTPtr(rtM));
    rtsiSetStepSizePtr(&rtM->solverInfo, &rtM->Timing.stepSize0);
    rtsiSetdXPtr(&rtM->solverInfo, &rtM->derivs);
    rtsiSetContStatesPtr(&rtM->solverInfo, (double **) &rtM->contStates);
    rtsiSetNumContStatesPtr(&rtM->solverInfo, &rtM->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&rtM->solverInfo,
      &rtM->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&rtM->solverInfo,
      &rtM->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&rtM->solverInfo,
      &rtM->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&rtM->solverInfo, (bool**)
      &rtM->contStateDisabled);
    rtsiSetErrorStatusPtr(&rtM->solverInfo, (&rtmGetErrorStatus(rtM)));
    rtsiSetRTModelPtr(&rtM->solverInfo, rtM);
  }

  rtsiSetSimTimeStep(&rtM->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&rtM->solverInfo, false);
  rtsiSetIsContModeFrozen(&rtM->solverInfo, false);
  rtM->intgData.y = rtM->odeY;
  rtM->intgData.f[0] = rtM->odeF[0];
  rtM->intgData.f[1] = rtM->odeF[1];
  rtM->intgData.f[2] = rtM->odeF[2];
  rtM->intgData.f[3] = rtM->odeF[3];
  rtM->contStates = ((X *) &rtX);
  rtM->contStateDisabled = ((XDis *) &rtXDis);
  rtM->Timing.tStart = (0.0);
  rtsiSetSolverData(&rtM->solverInfo, (void *)&rtM->intgData);
  rtsiSetSolverName(&rtM->solverInfo,"ode4");
  rtmSetTPtr(rtM, &rtM->Timing.tArray[0]);
  rtM->Timing.stepSize0 = 0.2;

  /* InitializeConditions for Integrator: '<S44>/Integrator' */
  rtX.Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S39>/Filter' */
  rtX.Filter_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S100>/Integrator' */
  rtX.Integrator_CSTATE_l = 0.0;

  /* InitializeConditions for Integrator: '<S95>/Filter' */
  rtX.Filter_CSTATE_g = 0.0;

  /* InitializeConditions for Integrator: '<S212>/Integrator' */
  rtX.Integrator_CSTATE_g = 0.0;

  /* InitializeConditions for Integrator: '<S207>/Filter' */
  rtX.Filter_CSTATE_j = 0.0;

  /* InitializeConditions for Integrator: '<S156>/Integrator' */
  rtX.Integrator_CSTATE_m = 0.0;

  /* InitializeConditions for Integrator: '<S151>/Filter' */
  rtX.Filter_CSTATE_go = 0.0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
