/*
 * helicopter_p3.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter_p3".
 *
 * Model version              : 1.195
 * Simulink Coder version : 8.9 (R2015b) 13-Aug-2015
 * C source code generated on : Fri Mar 08 19:21:34 2019
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter_p3.h"
#include "helicopter_p3_private.h"
#include "helicopter_p3_dt.h"

/* Block signals (auto storage) */
B_helicopter_p3_T helicopter_p3_B;

/* Continuous states */
X_helicopter_p3_T helicopter_p3_X;

/* Block states (auto storage) */
DW_helicopter_p3_T helicopter_p3_DW;

/* Real-time model */
RT_MODEL_helicopter_p3_T helicopter_p3_M_;
RT_MODEL_helicopter_p3_T *const helicopter_p3_M = &helicopter_p3_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_p3_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_p3_output(void)
{
  /* local block i/o variables */
  real_T rtb_Derivative;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* set solver stop time */
    if (!(helicopter_p3_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_p3_M->solverInfo,
                            ((helicopter_p3_M->Timing.clockTickH0 + 1) *
        helicopter_p3_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_p3_M->solverInfo,
                            ((helicopter_p3_M->Timing.clockTick0 + 1) *
        helicopter_p3_M->Timing.stepSize0 + helicopter_p3_M->Timing.clockTickH0 *
        helicopter_p3_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_p3_M)) {
    helicopter_p3_M->Timing.t[0] = rtsiGetT(&helicopter_p3_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S6>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder
        (helicopter_p3_DW.HILReadEncoderTimebase_Task, 1,
         &helicopter_p3_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_p3_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S6>/Travel: Count to rad' incorporates:
     *  Gain: '<S6>/Travel_gain'
     */
    helicopter_p3_B.TravelCounttorad = helicopter_p3_P.travel_gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_p3_P.TravelCounttorad_Gain;

    /* Gain: '<S13>/Gain' */
    helicopter_p3_B.Gain = helicopter_p3_P.Gain_Gain *
      helicopter_p3_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]1'
     */
    helicopter_p3_B.Sum3 = helicopter_p3_P.elavation_offsetdeg1_Value +
      helicopter_p3_B.Gain;

    /* Gain: '<S3>/Gain1' */
    helicopter_p3_B.Gain1 = helicopter_p3_P.Gain1_Gain * helicopter_p3_B.Sum3;

    /* Gain: '<S6>/Pitch: Count to rad' */
    helicopter_p3_B.PitchCounttorad = helicopter_p3_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S10>/Gain' */
    helicopter_p3_B.Gain_i = helicopter_p3_P.Gain_Gain_a *
      helicopter_p3_B.PitchCounttorad;

    /* Gain: '<S4>/Gain1' */
    helicopter_p3_B.Gain1_g = helicopter_p3_P.Gain1_Gain_n *
      helicopter_p3_B.Gain_i;
  }

  /* FromWorkspace: '<Root>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *)
      helicopter_p3_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *)
      helicopter_p3_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_p3_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = helicopter_p3_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_p3_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Derivative = pDataValues[currTimeIndex];
        } else {
          rtb_Derivative = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Derivative = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* Gain: '<S14>/Gain' incorporates:
   *  TransferFcn: '<S6>/Travel: Transfer Fcn'
   */
  helicopter_p3_B.Gain_d = (helicopter_p3_P.TravelTransferFcn_C *
    helicopter_p3_X.TravelTransferFcn_CSTATE +
    helicopter_p3_P.TravelTransferFcn_D * helicopter_p3_B.TravelCounttorad) *
    helicopter_p3_P.Gain_Gain_l;

  /* Gain: '<S11>/Gain' incorporates:
   *  TransferFcn: '<S6>/Pitch: Transfer Fcn'
   */
  helicopter_p3_B.Gain_b = (helicopter_p3_P.PitchTransferFcn_C *
    helicopter_p3_X.PitchTransferFcn_CSTATE + helicopter_p3_P.PitchTransferFcn_D
    * helicopter_p3_B.PitchCounttorad) * helicopter_p3_P.Gain_Gain_ae;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* Gain: '<S6>/Elevation: Count to rad' */
    helicopter_p3_B.ElevationCounttorad =
      helicopter_p3_P.ElevationCounttorad_Gain * rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S8>/Gain' */
    helicopter_p3_B.Gain_e = helicopter_p3_P.Gain_Gain_lv *
      helicopter_p3_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_p3_B.Sum = helicopter_p3_B.Gain_e +
      helicopter_p3_P.elavation_offsetdeg_Value;
  }

  /* Gain: '<S9>/Gain' incorporates:
   *  TransferFcn: '<S6>/Elevation: Transfer Fcn'
   */
  helicopter_p3_B.Gain_dg = (helicopter_p3_P.ElevationTransferFcn_C *
    helicopter_p3_X.ElevationTransferFcn_CSTATE +
    helicopter_p3_P.ElevationTransferFcn_D * helicopter_p3_B.ElevationCounttorad)
    * helicopter_p3_P.Gain_Gain_n;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S2>/Gain1'
   *  Gain: '<S7>/K_pd'
   *  Gain: '<S7>/K_pp'
   *  Sum: '<S7>/Sum2'
   *  Sum: '<S7>/Sum3'
   */
  helicopter_p3_B.Sum1 = ((rtb_Derivative - helicopter_p3_P.Gain1_Gain_f *
    helicopter_p3_B.Gain_i) * helicopter_p3_P.K_pp -
    helicopter_p3_P.Gain1_Gain_f * helicopter_p3_B.Gain_b * helicopter_p3_P.K_pd)
    + helicopter_p3_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Integrator: '<S5>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_p3_X.Integrator_CSTATE >= helicopter_p3_P.Integrator_UpperSat)
  {
    helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_UpperSat;
  } else {
    if (helicopter_p3_X.Integrator_CSTATE <= helicopter_p3_P.Integrator_LowerSat)
    {
      helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_LowerSat;
    }
  }

  /* Sum: '<S5>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   *  Gain: '<S2>/Gain1'
   */
  rtb_Derivative = helicopter_p3_P.elevation_ref_Value -
    helicopter_p3_P.Gain1_Gain_f * helicopter_p3_B.Sum;

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S2>/Gain1'
   *  Gain: '<S5>/K_ed'
   *  Gain: '<S5>/K_ep'
   *  Integrator: '<S5>/Integrator'
   *  Sum: '<S5>/Sum1'
   */
  helicopter_p3_B.Sum2 = ((helicopter_p3_P.K_ep * rtb_Derivative +
    helicopter_p3_X.Integrator_CSTATE) - helicopter_p3_P.Gain1_Gain_f *
    helicopter_p3_B.Gain_dg * helicopter_p3_P.K_ed) + helicopter_p3_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S5>/K_ei' */
  helicopter_p3_B.K_ei = helicopter_p3_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Derivative: '<S6>/Derivative' */
  if ((helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) &&
      (helicopter_p3_DW.TimeStampB >= helicopter_p3_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Backgain = helicopter_p3_DW.TimeStampA;
    lastU = &helicopter_p3_DW.LastUAtTimeA;
    if (helicopter_p3_DW.TimeStampA < helicopter_p3_DW.TimeStampB) {
      if (helicopter_p3_DW.TimeStampB < helicopter_p3_M->Timing.t[0]) {
        rtb_Backgain = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_p3_DW.TimeStampA >= helicopter_p3_M->Timing.t[0]) {
        rtb_Backgain = helicopter_p3_DW.TimeStampB;
        lastU = &helicopter_p3_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (helicopter_p3_B.PitchCounttorad - *lastU) /
      (helicopter_p3_M->Timing.t[0] - rtb_Backgain);
  }

  /* End of Derivative: '<S6>/Derivative' */

  /* Gain: '<S12>/Gain' */
  helicopter_p3_B.Gain_l = helicopter_p3_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_p3_B.Sum2 - helicopter_p3_B.Sum1) *
    helicopter_p3_P.Backgain_Gain;

  /* Saturate: '<S6>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_p3_P.BackmotorSaturation_UpperSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p3_P.BackmotorSaturation_LowerSat) {
    helicopter_p3_B.BackmotorSaturation =
      helicopter_p3_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S6>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Backgain = (helicopter_p3_B.Sum1 + helicopter_p3_B.Sum2) *
    helicopter_p3_P.Frontgain_Gain;

  /* Saturate: '<S6>/Front motor: Saturation' */
  if (rtb_Backgain > helicopter_p3_P.FrontmotorSaturation_UpperSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_p3_P.FrontmotorSaturation_LowerSat) {
    helicopter_p3_B.FrontmotorSaturation =
      helicopter_p3_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_p3_B.FrontmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S6>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    /* S-Function (hil_write_analog_block): '<S6>/HIL Write Analog' */

    /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_p3_DW.HILWriteAnalog_Buffer[0] =
        helicopter_p3_B.FrontmotorSaturation;
      helicopter_p3_DW.HILWriteAnalog_Buffer[1] =
        helicopter_p3_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILWriteAnalog_channels, 2,
        &helicopter_p3_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_p3_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S6>/Derivative' */
  if (helicopter_p3_DW.TimeStampA == (rtInf)) {
    helicopter_p3_DW.TimeStampA = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeA;
  } else if (helicopter_p3_DW.TimeStampB == (rtInf)) {
    helicopter_p3_DW.TimeStampB = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeB;
  } else if (helicopter_p3_DW.TimeStampA < helicopter_p3_DW.TimeStampB) {
    helicopter_p3_DW.TimeStampA = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeA;
  } else {
    helicopter_p3_DW.TimeStampB = helicopter_p3_M->Timing.t[0];
    lastU = &helicopter_p3_DW.LastUAtTimeB;
  }

  *lastU = helicopter_p3_B.PitchCounttorad;

  /* End of Update for Derivative: '<S6>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_p3_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_p3_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter_p3_M->Timing.clockTick0)) {
    ++helicopter_p3_M->Timing.clockTickH0;
  }

  helicopter_p3_M->Timing.t[0] = rtsiGetSolverStopTime
    (&helicopter_p3_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_p3_M->Timing.clockTick1)) {
      ++helicopter_p3_M->Timing.clockTickH1;
    }

    helicopter_p3_M->Timing.t[1] = helicopter_p3_M->Timing.clockTick1 *
      helicopter_p3_M->Timing.stepSize1 + helicopter_p3_M->Timing.clockTickH1 *
      helicopter_p3_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_p3_derivatives(void)
{
  boolean_T lsat;
  boolean_T usat;
  XDot_helicopter_p3_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_p3_T *) helicopter_p3_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S6>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_P.TravelTransferFcn_A *
    helicopter_p3_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_p3_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S6>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_P.PitchTransferFcn_A *
    helicopter_p3_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_p3_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S6>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_P.ElevationTransferFcn_A
    * helicopter_p3_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_p3_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S5>/Integrator' */
  lsat = (helicopter_p3_X.Integrator_CSTATE <=
          helicopter_p3_P.Integrator_LowerSat);
  usat = (helicopter_p3_X.Integrator_CSTATE >=
          helicopter_p3_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_p3_B.K_ei > 0.0)) || (usat &&
       (helicopter_p3_B.K_ei < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_p3_B.K_ei;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S5>/Integrator' */
}

/* Model initialize function */
void helicopter_p3_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p3/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_p3_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_p3_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_p3_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
      return;
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_p3_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = helicopter_p3_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_p3_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_p3_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_input_chan, 8U,
        &helicopter_p3_DW.HILInitialize_AIMinimums[0],
        &helicopter_p3_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_output && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_p3_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = helicopter_p3_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_p3_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_p3_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p3_DW.HILInitialize_AOMinimums[0],
        &helicopter_p3_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p3_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_encoder_param && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_p3_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_p3_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_encoder_channels, 8U,
         (t_encoder_quadrature_mode *)
         &helicopter_p3_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_encoder_count && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_p3_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] =
            helicopter_p3_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_encoder_channels, 8U,
        &helicopter_p3_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p3_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_p3_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_p3_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_p3_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            helicopter_p3_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_p3_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_p3_DW.HILInitialize_POSortedChans[7U -
              num_frequency_modes] = p_HILInitialize_pwm_channels[i1];
            helicopter_p3_DW.HILInitialize_POSortedFreqs[7U -
              num_frequency_modes] = helicopter_p3_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_p3_DW.HILInitialize_Card,
          &helicopter_p3_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_p3_DW.HILInitialize_Card,
          &helicopter_p3_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_p3_DW.HILInitialize_POModeValues
          [0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_p3_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues =
          &helicopter_p3_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_p3_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_p3_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_p3_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &helicopter_p3_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_p3_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_p3_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs =
          &helicopter_p3_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_p3_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p3_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_p3_DW.HILInitialize_Card,
        helicopter_p3_P.HILInitialize_pwm_channels, 8U,
        &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_p3_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_p3_DW.HILInitialize_Card,
         helicopter_p3_P.HILInitialize_pwm_channels, 8U,
         &helicopter_p3_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S6>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter_p3/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_p3_DW.HILInitialize_Card,
      helicopter_p3_P.HILReadEncoderTimebase_samples_,
      helicopter_p3_P.HILReadEncoderTimebase_channels, 3,
      &helicopter_p3_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/From Workspace1' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.523598775489244,
      0.5235987753664243, 0.52359877530534793, 0.52359877524000986,
      0.52359877520072629, 0.52359877513609066, 0.52359877499742247,
      0.52359877476164918, 0.5235987744249504, 0.5235987742745225,
      0.5235987740063518, 0.5235987748712484, 0.52359876945759276,
      0.015328855285386807, -0.52359875831543423, -0.523598771805789,
      -0.52359877327732873, -0.52359877394449927, -0.5235987746198314,
      -0.5235987752669955, -0.523598774991644, -0.523598772142101,
      -0.5235987688277135, -0.52359877423702872, -0.52359877520423137,
      -0.52359873234831555, -0.52359877471828542, -0.52359835796510046,
      -0.52359877354014306, -0.21994779376870849, 0.0059171571220551483,
      0.14586191764781978, 0.21733842372930948, 0.23814327023859125,
      0.22473049337548603, 0.19112251909702194, 0.14838043160269637,
      0.10452895050474287, 0.064805650601934911, 0.03210598176810877,
      0.0075142640335661018, -0.0091629615051318971, -0.018917430171140635,
      -0.023145051597936523, -0.023348923132811762, -0.020936814347147146,
      -0.017101340829637456, -0.012766343630361197, -0.0085817025827475361,
      -0.0049498881597276646, -0.00207005308662521, 1.1387831672228384E-5,
      0.0013515134990698928, 0.0020678816797199268, 0.0023042170721352985,
      0.0022054473569010379, 0.001901483970638019, 0.0014984058165054786,
      0.0010753744707237053, 0.000685579174301621, 0.00035967583743458869,
      0.00011045857484108053, -6.2183485048698781E-5, -0.00016666565597862071,
      -0.00021583803079861462, -0.00022400847909894751, -0.00020486529621273775,
      -0.00017020597985495207, -0.00012932402555242064, -8.8886593489743775E-5,
      -5.3141843119891325E-5, -2.4316127615863791E-5, -3.090262573729657E-6,
      1.0925141723554514E-5, 1.87667441651042E-5, 2.1775385657417192E-5,
      2.1341841363631476E-5, 1.8737525116596613E-5, 1.5018383569624033E-5,
      1.0986424483282738E-5, 7.19257456739071E-6, 3.9658569927343617E-6,
      1.4563496096591062E-6, -3.1765515818013216E-7, -1.4249150908595055E-6,
      -1.9829277262336592E-6, -2.1279154302673569E-6, -1.9934003358337442E-6,
      -1.6966165539778936E-6, -1.3314173835028247E-6, -9.6610730078175822E-7,
      -6.446621461827093E-7, -3.9000376322626856E-7, -2.0828953573040271E-7,
      -9.350131027538142E-8, -3.1924315461178781E-8, -6.3531681898575586E-9,
      2.1477624955441957E-12, 2.375820575203859E-12, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0 } ;

    helicopter_p3_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_p3_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_p3_DW.FromWorkspace1_IWORK.PrevIndex = 0;
  }

  /* InitializeConditions for TransferFcn: '<S6>/Travel: Transfer Fcn' */
  helicopter_p3_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S6>/Pitch: Transfer Fcn' */
  helicopter_p3_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S6>/Elevation: Transfer Fcn' */
  helicopter_p3_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S5>/Integrator' */
  helicopter_p3_X.Integrator_CSTATE = helicopter_p3_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S6>/Derivative' */
  helicopter_p3_DW.TimeStampA = (rtInf);
  helicopter_p3_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_p3_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_p3/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_p3_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_p3_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_p3_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_p3_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_p3_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_p3_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (helicopter_p3_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_p3_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_p3_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_p3_DW.HILInitialize_Card
                         , helicopter_p3_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_p3_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_p3_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_p3_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_p3_DW.HILInitialize_Card,
            helicopter_p3_P.HILInitialize_analog_output_cha,
            num_final_analog_outputs,
            &helicopter_p3_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_p3_DW.HILInitialize_Card,
            helicopter_p3_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &helicopter_p3_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_p3_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_p3_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_p3_DW.HILInitialize_Card);
    hil_close(helicopter_p3_DW.HILInitialize_Card);
    helicopter_p3_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_p3_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_p3_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter_p3_initialize();
}

void MdlTerminate(void)
{
  helicopter_p3_terminate();
}

/* Registration function */
RT_MODEL_helicopter_p3_T *helicopter_p3(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_p3_P.Integrator_UpperSat = rtInf;
  helicopter_p3_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_p3_M, 0,
                sizeof(RT_MODEL_helicopter_p3_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_p3_M->solverInfo,
                          &helicopter_p3_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_p3_M->solverInfo, &rtmGetTPtr(helicopter_p3_M));
    rtsiSetStepSizePtr(&helicopter_p3_M->solverInfo,
                       &helicopter_p3_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_p3_M->solverInfo,
                 &helicopter_p3_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_p3_M->solverInfo, (real_T **)
                         &helicopter_p3_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_p3_M->solverInfo,
      &helicopter_p3_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_p3_M->solverInfo,
      &helicopter_p3_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_p3_M->solverInfo,
      &helicopter_p3_M->ModelData.periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_p3_M->solverInfo,
      &helicopter_p3_M->ModelData.periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_p3_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_p3_M)));
    rtsiSetRTModelPtr(&helicopter_p3_M->solverInfo, helicopter_p3_M);
  }

  rtsiSetSimTimeStep(&helicopter_p3_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_p3_M->ModelData.intgData.f[0] = helicopter_p3_M->ModelData.odeF[0];
  helicopter_p3_M->ModelData.contStates = ((real_T *) &helicopter_p3_X);
  rtsiSetSolverData(&helicopter_p3_M->solverInfo, (void *)
                    &helicopter_p3_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_p3_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_p3_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_p3_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_p3_M->Timing.sampleTimes =
      (&helicopter_p3_M->Timing.sampleTimesArray[0]);
    helicopter_p3_M->Timing.offsetTimes =
      (&helicopter_p3_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_p3_M->Timing.sampleTimes[0] = (0.0);
    helicopter_p3_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_p3_M->Timing.offsetTimes[0] = (0.0);
    helicopter_p3_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_p3_M, &helicopter_p3_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_p3_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_p3_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_p3_M, -1);
  helicopter_p3_M->Timing.stepSize0 = 0.002;
  helicopter_p3_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_p3_M->Sizes.checksums[0] = (2689452365U);
  helicopter_p3_M->Sizes.checksums[1] = (1103693080U);
  helicopter_p3_M->Sizes.checksums[2] = (3675127837U);
  helicopter_p3_M->Sizes.checksums[3] = (1429509037U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_p3_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_p3_M->extModeInfo,
      &helicopter_p3_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_p3_M->extModeInfo,
                        helicopter_p3_M->Sizes.checksums);
    rteiSetTPtr(helicopter_p3_M->extModeInfo, rtmGetTPtr(helicopter_p3_M));
  }

  helicopter_p3_M->solverInfoPtr = (&helicopter_p3_M->solverInfo);
  helicopter_p3_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_p3_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_p3_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_p3_M->ModelData.blockIO = ((void *) &helicopter_p3_B);

  {
    helicopter_p3_B.TravelCounttorad = 0.0;
    helicopter_p3_B.Gain = 0.0;
    helicopter_p3_B.Sum3 = 0.0;
    helicopter_p3_B.Gain1 = 0.0;
    helicopter_p3_B.PitchCounttorad = 0.0;
    helicopter_p3_B.Gain_i = 0.0;
    helicopter_p3_B.Gain1_g = 0.0;
    helicopter_p3_B.Gain_d = 0.0;
    helicopter_p3_B.Gain_b = 0.0;
    helicopter_p3_B.ElevationCounttorad = 0.0;
    helicopter_p3_B.Gain_e = 0.0;
    helicopter_p3_B.Sum = 0.0;
    helicopter_p3_B.Gain_dg = 0.0;
    helicopter_p3_B.Sum1 = 0.0;
    helicopter_p3_B.Sum2 = 0.0;
    helicopter_p3_B.K_ei = 0.0;
    helicopter_p3_B.Gain_l = 0.0;
    helicopter_p3_B.BackmotorSaturation = 0.0;
    helicopter_p3_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_p3_M->ModelData.defaultParam = ((real_T *)&helicopter_p3_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_p3_X;
    helicopter_p3_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_p3_X, 0,
                  sizeof(X_helicopter_p3_T));
  }

  /* states (dwork) */
  helicopter_p3_M->ModelData.dwork = ((void *) &helicopter_p3_DW);
  (void) memset((void *)&helicopter_p3_DW, 0,
                sizeof(DW_helicopter_p3_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_p3_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_p3_DW.TimeStampA = 0.0;
  helicopter_p3_DW.LastUAtTimeA = 0.0;
  helicopter_p3_DW.TimeStampB = 0.0;
  helicopter_p3_DW.LastUAtTimeB = 0.0;
  helicopter_p3_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_p3_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_p3_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_p3_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_p3_M->Sizes.numPeriodicContStates = (0);/* Number of periodic continuous states */
  helicopter_p3_M->Sizes.numY = (0);   /* Number of model outputs */
  helicopter_p3_M->Sizes.numU = (0);   /* Number of model inputs */
  helicopter_p3_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_p3_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_p3_M->Sizes.numBlocks = (62);/* Number of blocks */
  helicopter_p3_M->Sizes.numBlockIO = (19);/* Number of block outputs */
  helicopter_p3_M->Sizes.numBlockPrms = (145);/* Sum of parameter "widths" */
  return helicopter_p3_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
