/*
 * helicopter_with_feedback.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter_with_feedback".
 *
 * Model version              : 1.205
 * Simulink Coder version : 8.9 (R2015b) 13-Aug-2015
 * C source code generated on : Fri Mar 08 22:00:11 2019
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter_with_feedback.h"
#include "helicopter_with_feedback_private.h"
#include "helicopter_with_feedback_dt.h"

/* Block signals (auto storage) */
B_helicopter_with_feedback_T helicopter_with_feedback_B;

/* Continuous states */
X_helicopter_with_feedback_T helicopter_with_feedback_X;

/* Block states (auto storage) */
DW_helicopter_with_feedback_T helicopter_with_feedback_DW;

/* Real-time model */
RT_MODEL_helicopter_with_feed_T helicopter_with_feedback_M_;
RT_MODEL_helicopter_with_feed_T *const helicopter_with_feedback_M =
  &helicopter_with_feedback_M_;

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
  helicopter_with_feedback_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_with_feedback_output(void)
{
  /* local block i/o variables */
  real_T rtb_FromWorkspace1[2];
  real_T rtb_Sum5[6];
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Backgain;
  real_T rtb_Gain1[6];
  int32_T i;
  int32_T i_0;
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
    /* set solver stop time */
    if (!(helicopter_with_feedback_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_with_feedback_M->solverInfo,
                            ((helicopter_with_feedback_M->Timing.clockTickH0 + 1)
        * helicopter_with_feedback_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_with_feedback_M->solverInfo,
                            ((helicopter_with_feedback_M->Timing.clockTick0 + 1)
        * helicopter_with_feedback_M->Timing.stepSize0 +
        helicopter_with_feedback_M->Timing.clockTickH0 *
        helicopter_with_feedback_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_with_feedback_M)) {
    helicopter_with_feedback_M->Timing.t[0] = rtsiGetT
      (&helicopter_with_feedback_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S7>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter_with_feedback/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder
        (helicopter_with_feedback_DW.HILReadEncoderTimebase_Task, 1,
         &helicopter_with_feedback_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_with_feedback_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_with_feedback_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_with_feedback_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S7>/Travel: Count to rad' incorporates:
     *  Gain: '<S7>/Travel_gain'
     */
    helicopter_with_feedback_B.TravelCounttorad =
      helicopter_with_feedback_P.travel_gain * rtb_HILReadEncoderTimebase_o1 *
      helicopter_with_feedback_P.TravelCounttorad_Gain;

    /* Gain: '<S15>/Gain' */
    helicopter_with_feedback_B.Gain = helicopter_with_feedback_P.Gain_Gain *
      helicopter_with_feedback_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]1'
     */
    helicopter_with_feedback_B.Sum3 =
      helicopter_with_feedback_P.elavation_offsetdeg1_Value +
      helicopter_with_feedback_B.Gain;

    /* Gain: '<S3>/Gain1' */
    helicopter_with_feedback_B.Gain1 = helicopter_with_feedback_P.Gain1_Gain *
      helicopter_with_feedback_B.Sum3;

    /* Gain: '<S7>/Elevation: Count to rad' */
    helicopter_with_feedback_B.ElevationCounttorad =
      helicopter_with_feedback_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S10>/Gain' */
    helicopter_with_feedback_B.Gain_e = helicopter_with_feedback_P.Gain_Gain_l *
      helicopter_with_feedback_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_with_feedback_B.Sum = helicopter_with_feedback_B.Gain_e +
      helicopter_with_feedback_P.elavation_offsetdeg_Value;

    /* Gain: '<S4>/Gain1' */
    helicopter_with_feedback_B.Gain1_e = helicopter_with_feedback_P.Gain1_Gain_d
      * helicopter_with_feedback_B.Sum;

    /* Gain: '<S7>/Pitch: Count to rad' */
    helicopter_with_feedback_B.PitchCounttorad =
      helicopter_with_feedback_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S12>/Gain' */
    helicopter_with_feedback_B.Gain_i = helicopter_with_feedback_P.Gain_Gain_a *
      helicopter_with_feedback_B.PitchCounttorad;

    /* Gain: '<S5>/Gain1' */
    helicopter_with_feedback_B.Gain1_g = helicopter_with_feedback_P.Gain1_Gain_b
      * helicopter_with_feedback_B.Gain_i;
  }

  /* FromWorkspace: '<S8>/From Workspace' */
  {
    real_T *pDataValues = (real_T *)
      helicopter_with_feedback_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *)
      helicopter_with_feedback_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex =
      helicopter_with_feedback_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = helicopter_with_feedback_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[80]) {
      currTimeIndex = 79;
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

    helicopter_with_feedback_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&rtb_Sum5[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              (&rtb_Sum5[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 81;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 6; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&rtb_Sum5[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1, f2);
            pDataValues += 81;
          }
        }
      }
    }
  }

  /* Gain: '<S16>/Gain' incorporates:
   *  TransferFcn: '<S7>/Travel: Transfer Fcn'
   */
  helicopter_with_feedback_B.Gain_d =
    (helicopter_with_feedback_P.TravelTransferFcn_C *
     helicopter_with_feedback_X.TravelTransferFcn_CSTATE +
     helicopter_with_feedback_P.TravelTransferFcn_D *
     helicopter_with_feedback_B.TravelCounttorad) *
    helicopter_with_feedback_P.Gain_Gain_lu;

  /* Gain: '<S13>/Gain' incorporates:
   *  TransferFcn: '<S7>/Pitch: Transfer Fcn'
   */
  helicopter_with_feedback_B.Gain_b =
    (helicopter_with_feedback_P.PitchTransferFcn_C *
     helicopter_with_feedback_X.PitchTransferFcn_CSTATE +
     helicopter_with_feedback_P.PitchTransferFcn_D *
     helicopter_with_feedback_B.PitchCounttorad) *
    helicopter_with_feedback_P.Gain_Gain_ae;

  /* Gain: '<S11>/Gain' incorporates:
   *  TransferFcn: '<S7>/Elevation: Transfer Fcn'
   */
  helicopter_with_feedback_B.Gain_dg =
    (helicopter_with_feedback_P.ElevationTransferFcn_C *
     helicopter_with_feedback_X.ElevationTransferFcn_CSTATE +
     helicopter_with_feedback_P.ElevationTransferFcn_D *
     helicopter_with_feedback_B.ElevationCounttorad) *
    helicopter_with_feedback_P.Gain_Gain_n;

  /* Gain: '<S2>/Gain1' */
  rtb_Gain1[0] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Sum3;
  rtb_Gain1[1] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Gain_d;
  rtb_Gain1[2] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Gain_i;
  rtb_Gain1[3] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Gain_b;
  rtb_Gain1[4] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Sum;
  rtb_Gain1[5] = helicopter_with_feedback_P.Gain1_Gain_f *
    helicopter_with_feedback_B.Gain_dg;

  /* Sum: '<S8>/Sum5' */
  for (i = 0; i < 6; i++) {
    rtb_Sum5[i] = rtb_Gain1[i] - rtb_Sum5[i];
  }

  /* End of Sum: '<S8>/Sum5' */

  /* FromWorkspace: '<S8>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *)
      helicopter_with_feedback_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *)
      helicopter_with_feedback_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex =
      helicopter_with_feedback_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = helicopter_with_feedback_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[80]) {
      currTimeIndex = 79;
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

    helicopter_with_feedback_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 81;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              (&rtb_FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 81;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 2; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&rtb_FromWorkspace1[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1,
              f2);
            pDataValues += 81;
          }
        }
      }
    }
  }

  /* Sum: '<S8>/Sum4' incorporates:
   *  Constant: '<S8>/Constant'
   *  Product: '<S8>/Matrix Multiply'
   */
  for (i = 0; i < 2; i++) {
    rtb_Backgain = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Backgain += helicopter_with_feedback_P.K_T[6 * i + i_0] * rtb_Sum5[i_0];
    }

    helicopter_with_feedback_B.Sum4[i] = rtb_FromWorkspace1[i] - rtb_Backgain;
  }

  /* End of Sum: '<S8>/Sum4' */

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S9>/K_pd'
   *  Gain: '<S9>/K_pp'
   *  Sum: '<S9>/Sum2'
   *  Sum: '<S9>/Sum3'
   */
  helicopter_with_feedback_B.Sum1 = ((helicopter_with_feedback_B.Sum4[0] -
    rtb_Gain1[2]) * helicopter_with_feedback_P.K_pp -
    helicopter_with_feedback_P.K_pd * rtb_Gain1[3]) +
    helicopter_with_feedback_P.Vd_ff;
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
  }

  /* Integrator: '<S6>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_with_feedback_X.Integrator_CSTATE >=
      helicopter_with_feedback_P.Integrator_UpperSat) {
    helicopter_with_feedback_X.Integrator_CSTATE =
      helicopter_with_feedback_P.Integrator_UpperSat;
  } else {
    if (helicopter_with_feedback_X.Integrator_CSTATE <=
        helicopter_with_feedback_P.Integrator_LowerSat) {
      helicopter_with_feedback_X.Integrator_CSTATE =
        helicopter_with_feedback_P.Integrator_LowerSat;
    }
  }

  /* Sum: '<S6>/Sum' */
  rtb_Backgain = helicopter_with_feedback_B.Sum4[1] - rtb_Gain1[4];

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S6>/K_ed'
   *  Gain: '<S6>/K_ep'
   *  Integrator: '<S6>/Integrator'
   *  Sum: '<S6>/Sum1'
   */
  helicopter_with_feedback_B.Sum2 = ((helicopter_with_feedback_P.K_ep *
    rtb_Backgain + helicopter_with_feedback_X.Integrator_CSTATE) -
    helicopter_with_feedback_P.K_ed * rtb_Gain1[5]) +
    helicopter_with_feedback_P.Vs_ff;
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
  }

  /* Gain: '<S6>/K_ei' */
  helicopter_with_feedback_B.K_ei = helicopter_with_feedback_P.K_ei *
    rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
  }

  /* Derivative: '<S7>/Derivative' */
  if ((helicopter_with_feedback_DW.TimeStampA >=
       helicopter_with_feedback_M->Timing.t[0]) &&
      (helicopter_with_feedback_DW.TimeStampB >=
       helicopter_with_feedback_M->Timing.t[0])) {
    rtb_Backgain = 0.0;
  } else {
    rtb_Backgain = helicopter_with_feedback_DW.TimeStampA;
    lastU = &helicopter_with_feedback_DW.LastUAtTimeA;
    if (helicopter_with_feedback_DW.TimeStampA <
        helicopter_with_feedback_DW.TimeStampB) {
      if (helicopter_with_feedback_DW.TimeStampB <
          helicopter_with_feedback_M->Timing.t[0]) {
        rtb_Backgain = helicopter_with_feedback_DW.TimeStampB;
        lastU = &helicopter_with_feedback_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_with_feedback_DW.TimeStampA >=
          helicopter_with_feedback_M->Timing.t[0]) {
        rtb_Backgain = helicopter_with_feedback_DW.TimeStampB;
        lastU = &helicopter_with_feedback_DW.LastUAtTimeB;
      }
    }

    rtb_Backgain = (helicopter_with_feedback_B.PitchCounttorad - *lastU) /
      (helicopter_with_feedback_M->Timing.t[0] - rtb_Backgain);
  }

  /* End of Derivative: '<S7>/Derivative' */

  /* Gain: '<S14>/Gain' */
  helicopter_with_feedback_B.Gain_l = helicopter_with_feedback_P.Gain_Gain_a1 *
    rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_with_feedback_B.Sum2 -
                  helicopter_with_feedback_B.Sum1) *
    helicopter_with_feedback_P.Backgain_Gain;

  /* Saturate: '<S7>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_with_feedback_P.BackmotorSaturation_UpperSat) {
    helicopter_with_feedback_B.BackmotorSaturation =
      helicopter_with_feedback_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain <
             helicopter_with_feedback_P.BackmotorSaturation_LowerSat) {
    helicopter_with_feedback_B.BackmotorSaturation =
      helicopter_with_feedback_P.BackmotorSaturation_LowerSat;
  } else {
    helicopter_with_feedback_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S7>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Backgain = (helicopter_with_feedback_B.Sum1 +
                  helicopter_with_feedback_B.Sum2) *
    helicopter_with_feedback_P.Frontgain_Gain;

  /* Saturate: '<S7>/Front motor: Saturation' */
  if (rtb_Backgain > helicopter_with_feedback_P.FrontmotorSaturation_UpperSat) {
    helicopter_with_feedback_B.FrontmotorSaturation =
      helicopter_with_feedback_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Backgain <
             helicopter_with_feedback_P.FrontmotorSaturation_LowerSat) {
    helicopter_with_feedback_B.FrontmotorSaturation =
      helicopter_with_feedback_P.FrontmotorSaturation_LowerSat;
  } else {
    helicopter_with_feedback_B.FrontmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S7>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
    /* S-Function (hil_write_analog_block): '<S7>/HIL Write Analog' */

    /* S-Function Block: helicopter_with_feedback/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_with_feedback_DW.HILWriteAnalog_Buffer[0] =
        helicopter_with_feedback_B.FrontmotorSaturation;
      helicopter_with_feedback_DW.HILWriteAnalog_Buffer[1] =
        helicopter_with_feedback_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_with_feedback_DW.HILInitialize_Card,
        helicopter_with_feedback_P.HILWriteAnalog_channels, 2,
        &helicopter_with_feedback_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_with_feedback_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S7>/Derivative' */
  if (helicopter_with_feedback_DW.TimeStampA == (rtInf)) {
    helicopter_with_feedback_DW.TimeStampA =
      helicopter_with_feedback_M->Timing.t[0];
    lastU = &helicopter_with_feedback_DW.LastUAtTimeA;
  } else if (helicopter_with_feedback_DW.TimeStampB == (rtInf)) {
    helicopter_with_feedback_DW.TimeStampB =
      helicopter_with_feedback_M->Timing.t[0];
    lastU = &helicopter_with_feedback_DW.LastUAtTimeB;
  } else if (helicopter_with_feedback_DW.TimeStampA <
             helicopter_with_feedback_DW.TimeStampB) {
    helicopter_with_feedback_DW.TimeStampA =
      helicopter_with_feedback_M->Timing.t[0];
    lastU = &helicopter_with_feedback_DW.LastUAtTimeA;
  } else {
    helicopter_with_feedback_DW.TimeStampB =
      helicopter_with_feedback_M->Timing.t[0];
    lastU = &helicopter_with_feedback_DW.LastUAtTimeB;
  }

  *lastU = helicopter_with_feedback_B.PitchCounttorad;

  /* End of Update for Derivative: '<S7>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_with_feedback_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_with_feedback_M->solverInfo);
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
  if (!(++helicopter_with_feedback_M->Timing.clockTick0)) {
    ++helicopter_with_feedback_M->Timing.clockTickH0;
  }

  helicopter_with_feedback_M->Timing.t[0] = rtsiGetSolverStopTime
    (&helicopter_with_feedback_M->solverInfo);

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
    if (!(++helicopter_with_feedback_M->Timing.clockTick1)) {
      ++helicopter_with_feedback_M->Timing.clockTickH1;
    }

    helicopter_with_feedback_M->Timing.t[1] =
      helicopter_with_feedback_M->Timing.clockTick1 *
      helicopter_with_feedback_M->Timing.stepSize1 +
      helicopter_with_feedback_M->Timing.clockTickH1 *
      helicopter_with_feedback_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_with_feedback_derivatives(void)
{
  boolean_T lsat;
  boolean_T usat;
  XDot_helicopter_with_feedback_T *_rtXdot;
  _rtXdot = ((XDot_helicopter_with_feedback_T *)
             helicopter_with_feedback_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S7>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE +=
    helicopter_with_feedback_P.TravelTransferFcn_A *
    helicopter_with_feedback_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE +=
    helicopter_with_feedback_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S7>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE +=
    helicopter_with_feedback_P.PitchTransferFcn_A *
    helicopter_with_feedback_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_with_feedback_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S7>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE +=
    helicopter_with_feedback_P.ElevationTransferFcn_A *
    helicopter_with_feedback_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE +=
    helicopter_with_feedback_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S6>/Integrator' */
  lsat = (helicopter_with_feedback_X.Integrator_CSTATE <=
          helicopter_with_feedback_P.Integrator_LowerSat);
  usat = (helicopter_with_feedback_X.Integrator_CSTATE >=
          helicopter_with_feedback_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_with_feedback_B.K_ei > 0.0)) ||
      (usat && (helicopter_with_feedback_B.K_ei < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_with_feedback_B.K_ei;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S6>/Integrator' */
}

/* Model initialize function */
void helicopter_with_feedback_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_with_feedback/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0",
                      &helicopter_with_feedback_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options
      (helicopter_with_feedback_DW.HILInitialize_Card,
       "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_with_feedback_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
      return;
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_analog_input_ &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_analog_inpu_m &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums =
          &helicopter_with_feedback_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] =
            helicopter_with_feedback_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums =
          &helicopter_with_feedback_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] =
            helicopter_with_feedback_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_analog_input_chan, 8U,
         &helicopter_with_feedback_DW.HILInitialize_AIMinimums[0],
         &helicopter_with_feedback_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_analog_output &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_analog_outp_b &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums =
          &helicopter_with_feedback_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] =
            helicopter_with_feedback_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums =
          &helicopter_with_feedback_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] =
            helicopter_with_feedback_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_with_feedback_DW.HILInitialize_AOMinimums[0],
         &helicopter_with_feedback_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_analog_outp_e &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_analog_outp_j &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages =
          &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] =
            helicopter_with_feedback_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(helicopter_with_feedback_DW.HILInitialize_Card,
        helicopter_with_feedback_P.HILInitialize_analog_output_cha, 8U,
        &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_with_feedback_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages =
          &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] =
            helicopter_with_feedback_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_analog_output_cha, 8U,
         &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_encoder_param &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_encoder_par_m &&
         is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_with_feedback_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] =
            helicopter_with_feedback_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_encoder_channels, 8U,
         (t_encoder_quadrature_mode *)
         &helicopter_with_feedback_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_encoder_count &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_encoder_cou_k &&
         is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_with_feedback_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] =
            helicopter_with_feedback_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_encoder_channels, 8U,
         &helicopter_with_feedback_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_pwm_params_at &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_pwm_params__f &&
         is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues =
          &helicopter_with_feedback_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(helicopter_with_feedback_DW.HILInitialize_Card,
        helicopter_with_feedback_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &helicopter_with_feedback_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          helicopter_with_feedback_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues =
          &helicopter_with_feedback_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            helicopter_with_feedback_DW.HILInitialize_POSortedChans[num_duty_cycle_modes]
              = p_HILInitialize_pwm_channels[i1];
            helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]
              = helicopter_with_feedback_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_with_feedback_DW.HILInitialize_POSortedChans[7U -
              num_frequency_modes] = p_HILInitialize_pwm_channels[i1];
            helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[7U -
              num_frequency_modes] =
              helicopter_with_feedback_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency
          (helicopter_with_feedback_DW.HILInitialize_Card,
           &helicopter_with_feedback_DW.HILInitialize_POSortedChans[0],
           num_duty_cycle_modes,
           &helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle
          (helicopter_with_feedback_DW.HILInitialize_Card,
           &helicopter_with_feedback_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
           num_frequency_modes,
           &helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues =
          &helicopter_with_feedback_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues =
          &helicopter_with_feedback_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_with_feedback_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_pwm_channels, 8U,
         (t_pwm_configuration *)
         &helicopter_with_feedback_DW.HILInitialize_POModeValues[0],
         (t_pwm_alignment *)
         &helicopter_with_feedback_DW.HILInitialize_POAlignValues[0],
         (t_pwm_polarity *)
         &helicopter_with_feedback_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs =
          &helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues =
          &helicopter_with_feedback_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] =
            helicopter_with_feedback_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_pwm_channels, 8U,
         &helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[0],
         &helicopter_with_feedback_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_pwm_outputs_a &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_pwm_outputs_g &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues =
          &helicopter_with_feedback_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] =
            helicopter_with_feedback_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(helicopter_with_feedback_DW.HILInitialize_Card,
        helicopter_with_feedback_P.HILInitialize_pwm_channels, 8U,
        &helicopter_with_feedback_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_with_feedback_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues =
          &helicopter_with_feedback_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] =
            helicopter_with_feedback_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_with_feedback_DW.HILInitialize_Card,
         helicopter_with_feedback_P.HILInitialize_pwm_channels, 8U,
         &helicopter_with_feedback_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S7>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter_with_feedback/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader
      (helicopter_with_feedback_DW.HILInitialize_Card,
       helicopter_with_feedback_P.HILReadEncoderTimebase_samples_,
       helicopter_with_feedback_P.HILReadEncoderTimebase_channels, 3,
       &helicopter_with_feedback_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<S8>/From Workspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413625261, 3.1262155534579983,
      3.1033093000299643, 3.0666274151911783, 3.0144539223941584,
      2.9456562771175667, 2.8597883849340486, 2.7630085954929711,
      2.663008595492971, 2.5630085954929709, 2.4630085954929708,
      2.3630085954929707, 2.2630085954929706, 2.1630085954929705,
      2.0630085954929704, 1.9630085954929704, 1.8630085954929703,
      1.7630085954929702, 1.66300859549297, 1.56300859549297, 1.46300859549297,
      1.3630085954929698, 1.2630085954929697, 1.1630085954929696,
      1.0630085954929696, 0.96300859549296958, 0.8630085954929696,
      0.76300859549296962, 0.66300859549296964, 0.56300859549296967,
      0.46300859549296969, 0.36302141233303764, 0.26315439018899522,
      0.16353415917499869, 0.064237129570891838, -0.034733037878568469,
      -0.13342772694236302, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, -0.015002048909068423, -0.046506351618112111,
      -0.091625013712135384, -0.14672753935514371, -0.20869397118807925,
      -0.27519058110636674, -0.34347156873407308, -0.38711915776430927, -0.4,
      -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4,
      -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4,
      -0.39994873263972813, -0.3994680885761695, -0.39848092405598617,
      -0.39718811841642737, -0.39588066979784126, -0.39477875625517816,
      -0.39396207452122356, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875205865551, 0.22266037932317656, 0.31888147181640641,
      0.38944360631144165, 0.43795507377677839, 0.46997264230390068,
      0.48258394245881492, 0.30848448923850241, 0.091036873429278867,
      8.0038211526652426E-17, -1.582532260816201E-16, 1.3491331642552022E-16,
      -8.3524338624755741E-17, -1.3043870589795218E-16, 1.2396027442653882E-16,
      1.8319685810988323E-16, -1.8111772308861987E-17, 5.999192391467082E-17,
      -4.4015034860958211E-17, 1.8164902991415473E-16, -1.5733751139513496E-17,
      1.7863819489487971E-16, -1.75922909815311E-17, -8.1552567942480385E-17,
      -1.1603683515861748E-16, 1.5127665407360738E-16, -1.4397480607105869E-16,
      -1.2290187663292355E-16, 8.8854592222131391E-17, 4.9032582826180681E-17,
      1.4598751555073997E-16, -0.00036233812220699314, -0.003397008672110275,
      -0.006976901807615633, -0.0091370565079251977, -0.0092405474911965036,
      -0.00778790407318773, -0.0057719945858840695, -0.0039935130385688727, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823462206,
      0.46652650905808424, 0.38488436997291947, 0.282248537980141,
      0.19404586986134689, 0.12807027410848895, 0.05044520061965703,
      -0.69639781288124991, -0.86979046323689424, -0.36414749371711519,
      -9.5316600704336963E-16, 1.1726666319565334E-15, -8.7375001052782964E-16,
      -1.8765730460262953E-16, 1.0175958685021468E-15, 2.3694633348511318E-16,
      -8.0523457543606371E-16, 3.1241483206703985E-16, -4.1602782661945637E-16,
      9.0265625571756942E-16, -7.895311280327313E-16, 7.77487784623775E-16,
      -7.8492194654322915E-16, -2.5584112206380683E-16, -1.3793708218597433E-16,
      1.0692539617475508E-15, -1.1810058292544259E-15, 8.4291704184969249E-17,
      8.4702584294057057E-16, -1.5928806278980864E-16, 3.8781969444224839E-16,
      -0.0014493524888285565, -0.012138682199613128, -0.014319572542021432,
      -0.0086406188012382555, -0.00041396393308522247, 0.0058105736720350928,
      0.0080636379492146429, 0.0071139261892607872, 0.0053880882264390785, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0033956952762319672, 0.0095949650106342127, 0.018104762765319325,
      0.028511485382292534, 0.0404635108101511, 0.053655320374928053,
      0.067812741738317112, 0.082678994058094174, 0.098000973864958069,
      0.11351552397135325, 0.12893510600706459, 0.14393255499105223,
      0.15812455925703051, 0.17105312475066012, 0.18216472470864889,
      0.19078639242738663, 0.19609811294620585, 0.19710087877555532,
      0.19478891618803293, 0.18997524726495441, 0.18332141396938287,
      0.175362676694831, 0.16652922127163741, 0.1571641014184349,
      0.14753823237876712, 0.13786315139854408, 0.12830156956108166,
      0.11897628039914286, 0.10997757964291184, 0.10136955395778978,
      0.093195141004400212, 0.085480428971333677, 0.078238212945250171,
      0.071470920201012852, 0.065172978846846988, 0.059332772036375547,
      0.05393430028926114, 0.048958363832393124, 0.044383686256151057, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.013582781104927869,
      0.024797078937608984, 0.034039191018740457, 0.041626890467892828,
      0.047808101711434278, 0.052767238259107806, 0.056629685453556226,
      0.05946500927910825, 0.061287919227455558, 0.062058200425580683,
      0.061678328142845358, 0.05998979593595042, 0.056768017063913177,
      0.051714261974518345, 0.044446399831955127, 0.034486670874950949,
      0.021246882075276936, 0.0040110633173978073, -0.009247850350089494,
      -0.019254675692314194, -0.026615333182285997, -0.031834949098207541,
      -0.035333821692774312, -0.0374604794128101, -0.03850347615867114,
      -0.038700323920892211, -0.038246327349849646, -0.037301156647755226,
      -0.035994803024924048, -0.034432102740488288, -0.03269765181355825,
      -0.030858848132266116, -0.028968864104334004, -0.027069170976949324,
      -0.025191765416663431, -0.023360827241885755, -0.021593886988457631,
      -0.019903745827472053, -0.018298710304968272, -0.016783930468250774, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0 } ;

    helicopter_with_feedback_DW.FromWorkspace_PWORK.TimePtr = (void *)
      pTimeValues0;
    helicopter_with_feedback_DW.FromWorkspace_PWORK.DataPtr = (void *)
      pDataValues0;
    helicopter_with_feedback_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<S8>/From Workspace1' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.48442195428147972,
      -0.396005546514884, -0.50525640562608531, -0.033698779148442944,
      0.13599335413509439, 1.6454498704343992E-15, -1.3817305739265879E-15,
      1.1107982334007058E-17, 1.1959344802836186E-15, 3.6458458244192605E-17,
      -8.994091633000865E-16, 6.6830585974964194E-16, -5.7029608501958005E-16,
      1.2257438259947755E-15, -1.1301838636143982E-15, 1.2389834682476428E-15,
      -1.0807593372416953E-15, -4.0311096535833465E-17, -1.5629966531716383E-16,
      1.2555433212679962E-15, -1.706075376381553E-15, 4.0114373131014341E-16,
      8.913297188041185E-16, -4.2412485342516329E-16, 5.8730954513546207E-16,
      -0.0017893240602821674, -0.014807095000901426, -0.016542220001293559,
      -0.012296590828944982, -0.0064212253297901969, -0.0019124021062664135,
      -2.7936862444877628E-6, -7.900061021466634E-7, 1.7036949961767109E-6,
      1.7036949961767109E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.2173244976788459, 0.23375988974260933, 0.25045780432477144,
      0.26715492027203397, 0.2835117045335539, 0.299090076990806,
      0.31333161895775713, 0.32553924339798535, 0.334839338028307,
      0.34015517013791841, 0.34015581904351566, 0.33321232123241562,
      0.31734582779827042, 0.29014454181638771, 0.24869581287409245,
      0.18948306076641377, 0.10827478741366844, 8.2060242830971807E-7,
      -2.524639997553938E-7, 2.7189960212542433E-7, -3.064207726782249E-7,
      5.98810657524078E-8, -3.4393651573772145E-7, 8.6640316122490822E-7,
      -6.44313379693074E-7, 6.3258821317385775E-7, 8.8183187932119817E-7,
      5.7323265616236071E-7, -1.3990646403641872E-6, 2.7285041881509461E-7,
      3.8351183939610741E-7, -1.9439576905279387E-7, -5.0707775047204781E-7,
      6.2592152543572334E-8, 1.8002027176808092E-8, -1.1306691980437648E-6,
      7.1393415393927819E-7, -5.1734168569792521E-7, -1.1466056663800504E-7, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_with_feedback_DW.FromWorkspace1_PWORK.TimePtr = (void *)
      pTimeValues0;
    helicopter_with_feedback_DW.FromWorkspace1_PWORK.DataPtr = (void *)
      pDataValues0;
    helicopter_with_feedback_DW.FromWorkspace1_IWORK.PrevIndex = 0;
  }

  /* InitializeConditions for TransferFcn: '<S7>/Travel: Transfer Fcn' */
  helicopter_with_feedback_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S7>/Pitch: Transfer Fcn' */
  helicopter_with_feedback_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S7>/Elevation: Transfer Fcn' */
  helicopter_with_feedback_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S6>/Integrator' */
  helicopter_with_feedback_X.Integrator_CSTATE =
    helicopter_with_feedback_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S7>/Derivative' */
  helicopter_with_feedback_DW.TimeStampA = (rtInf);
  helicopter_with_feedback_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_with_feedback_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter_with_feedback/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_with_feedback_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_with_feedback_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_with_feedback_P.HILInitialize_set_analog_out_ex &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_analog_outp_c &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages =
          &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] =
            helicopter_with_feedback_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((helicopter_with_feedback_P.HILInitialize_set_pwm_output_ap &&
         !is_switching) ||
        (helicopter_with_feedback_P.HILInitialize_set_pwm_outputs_p &&
         is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues =
          &helicopter_with_feedback_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] =
            helicopter_with_feedback_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_with_feedback_DW.HILInitialize_Card
                         ,
                         helicopter_with_feedback_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , helicopter_with_feedback_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         ,
                         &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_with_feedback_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog
            (helicopter_with_feedback_DW.HILInitialize_Card,
             helicopter_with_feedback_P.HILInitialize_analog_output_cha,
             num_final_analog_outputs,
             &helicopter_with_feedback_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm
            (helicopter_with_feedback_DW.HILInitialize_Card,
             helicopter_with_feedback_P.HILInitialize_pwm_channels,
             num_final_pwm_outputs,
             &helicopter_with_feedback_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_with_feedback_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_with_feedback_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_with_feedback_DW.HILInitialize_Card);
    hil_close(helicopter_with_feedback_DW.HILInitialize_Card);
    helicopter_with_feedback_DW.HILInitialize_Card = NULL;
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
  helicopter_with_feedback_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_with_feedback_update();
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
  helicopter_with_feedback_initialize();
}

void MdlTerminate(void)
{
  helicopter_with_feedback_terminate();
}

/* Registration function */
RT_MODEL_helicopter_with_feed_T *helicopter_with_feedback(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_with_feedback_P.Integrator_UpperSat = rtInf;
  helicopter_with_feedback_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_with_feedback_M, 0,
                sizeof(RT_MODEL_helicopter_with_feed_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_with_feedback_M->solverInfo,
                          &helicopter_with_feedback_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_with_feedback_M->solverInfo, &rtmGetTPtr
                (helicopter_with_feedback_M));
    rtsiSetStepSizePtr(&helicopter_with_feedback_M->solverInfo,
                       &helicopter_with_feedback_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_with_feedback_M->solverInfo,
                 &helicopter_with_feedback_M->ModelData.derivs);
    rtsiSetContStatesPtr(&helicopter_with_feedback_M->solverInfo, (real_T **)
                         &helicopter_with_feedback_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&helicopter_with_feedback_M->solverInfo,
      &helicopter_with_feedback_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_with_feedback_M->solverInfo,
      &helicopter_with_feedback_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_with_feedback_M->solverInfo,
      &helicopter_with_feedback_M->ModelData.periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_with_feedback_M->solverInfo,
      &helicopter_with_feedback_M->ModelData.periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_with_feedback_M->solverInfo,
                          (&rtmGetErrorStatus(helicopter_with_feedback_M)));
    rtsiSetRTModelPtr(&helicopter_with_feedback_M->solverInfo,
                      helicopter_with_feedback_M);
  }

  rtsiSetSimTimeStep(&helicopter_with_feedback_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_with_feedback_M->ModelData.intgData.f[0] =
    helicopter_with_feedback_M->ModelData.odeF[0];
  helicopter_with_feedback_M->ModelData.contStates = ((real_T *)
    &helicopter_with_feedback_X);
  rtsiSetSolverData(&helicopter_with_feedback_M->solverInfo, (void *)
                    &helicopter_with_feedback_M->ModelData.intgData);
  rtsiSetSolverName(&helicopter_with_feedback_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_with_feedback_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_with_feedback_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_with_feedback_M->Timing.sampleTimes =
      (&helicopter_with_feedback_M->Timing.sampleTimesArray[0]);
    helicopter_with_feedback_M->Timing.offsetTimes =
      (&helicopter_with_feedback_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_with_feedback_M->Timing.sampleTimes[0] = (0.0);
    helicopter_with_feedback_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_with_feedback_M->Timing.offsetTimes[0] = (0.0);
    helicopter_with_feedback_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_with_feedback_M,
             &helicopter_with_feedback_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_with_feedback_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_with_feedback_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_with_feedback_M, -1);
  helicopter_with_feedback_M->Timing.stepSize0 = 0.002;
  helicopter_with_feedback_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_with_feedback_M->Sizes.checksums[0] = (1366088435U);
  helicopter_with_feedback_M->Sizes.checksums[1] = (1392571513U);
  helicopter_with_feedback_M->Sizes.checksums[2] = (2755320153U);
  helicopter_with_feedback_M->Sizes.checksums[3] = (3976134373U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    helicopter_with_feedback_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(helicopter_with_feedback_M->extModeInfo,
      &helicopter_with_feedback_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_with_feedback_M->extModeInfo,
                        helicopter_with_feedback_M->Sizes.checksums);
    rteiSetTPtr(helicopter_with_feedback_M->extModeInfo, rtmGetTPtr
                (helicopter_with_feedback_M));
  }

  helicopter_with_feedback_M->solverInfoPtr =
    (&helicopter_with_feedback_M->solverInfo);
  helicopter_with_feedback_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_with_feedback_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_with_feedback_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_with_feedback_M->ModelData.blockIO = ((void *)
    &helicopter_with_feedback_B);

  {
    helicopter_with_feedback_B.TravelCounttorad = 0.0;
    helicopter_with_feedback_B.Gain = 0.0;
    helicopter_with_feedback_B.Sum3 = 0.0;
    helicopter_with_feedback_B.Gain1 = 0.0;
    helicopter_with_feedback_B.ElevationCounttorad = 0.0;
    helicopter_with_feedback_B.Gain_e = 0.0;
    helicopter_with_feedback_B.Sum = 0.0;
    helicopter_with_feedback_B.Gain1_e = 0.0;
    helicopter_with_feedback_B.PitchCounttorad = 0.0;
    helicopter_with_feedback_B.Gain_i = 0.0;
    helicopter_with_feedback_B.Gain1_g = 0.0;
    helicopter_with_feedback_B.Gain_d = 0.0;
    helicopter_with_feedback_B.Gain_b = 0.0;
    helicopter_with_feedback_B.Gain_dg = 0.0;
    helicopter_with_feedback_B.Sum4[0] = 0.0;
    helicopter_with_feedback_B.Sum4[1] = 0.0;
    helicopter_with_feedback_B.Sum1 = 0.0;
    helicopter_with_feedback_B.Sum2 = 0.0;
    helicopter_with_feedback_B.K_ei = 0.0;
    helicopter_with_feedback_B.Gain_l = 0.0;
    helicopter_with_feedback_B.BackmotorSaturation = 0.0;
    helicopter_with_feedback_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  helicopter_with_feedback_M->ModelData.defaultParam = ((real_T *)
    &helicopter_with_feedback_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_with_feedback_X;
    helicopter_with_feedback_M->ModelData.contStates = (x);
    (void) memset((void *)&helicopter_with_feedback_X, 0,
                  sizeof(X_helicopter_with_feedback_T));
  }

  /* states (dwork) */
  helicopter_with_feedback_M->ModelData.dwork = ((void *)
    &helicopter_with_feedback_DW);
  (void) memset((void *)&helicopter_with_feedback_DW, 0,
                sizeof(DW_helicopter_with_feedback_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_with_feedback_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_with_feedback_DW.TimeStampA = 0.0;
  helicopter_with_feedback_DW.LastUAtTimeA = 0.0;
  helicopter_with_feedback_DW.TimeStampB = 0.0;
  helicopter_with_feedback_DW.LastUAtTimeB = 0.0;
  helicopter_with_feedback_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_with_feedback_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_with_feedback_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_with_feedback_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_with_feedback_M->Sizes.numPeriodicContStates = (0);/* Number of periodic continuous states */
  helicopter_with_feedback_M->Sizes.numY = (0);/* Number of model outputs */
  helicopter_with_feedback_M->Sizes.numU = (0);/* Number of model inputs */
  helicopter_with_feedback_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_with_feedback_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_with_feedback_M->Sizes.numBlocks = (70);/* Number of blocks */
  helicopter_with_feedback_M->Sizes.numBlockIO = (21);/* Number of block outputs */
  helicopter_with_feedback_M->Sizes.numBlockPrms = (157);/* Sum of parameter "widths" */
  return helicopter_with_feedback_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
