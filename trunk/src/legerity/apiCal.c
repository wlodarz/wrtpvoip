/** \file apiCal.c
 * apiCal.c
 *
 * This file contains the line and device calibration functions for
 * the Vp880 device API.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_880_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_api.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"
#include "vp_hal.h"
#include "sys_service.h"

/* Time to wait before starting calibration after state change */
#define VP880_MIN_CAL_WAIT_TIME (100)

#define VP880_ABV_INIT_WAIT     (10)

#define VP880_CAL_ABV_LONG      (200)
#define VP880_CAL_ABV_DELAY     (20)
#define VP880_CAL_ABV_ABS_INIT  (2500)

/* Per Nigel W., sample 100ms with error <= 3 */
#define VP880_CAL_ABV_SAMPLE        (100)
#define VP880_CAL_ABV_SAMPLE_ERR    (3)
#define VP880_CAL_ABV_SAMPLE_MAX    (40)    /* Limit the number of tries */

#define VP880_CAL_VOC_SHORT     (100)    /**< Time for converter to get data */
#define VP880_CAL_VOC_LONG      (100)    /**< Time for line to stabilize */

#define VP880_IMT_AVERAGE_CNT   (1)

#define VP880_VAS_INIT_WAIT     (200)
#define VP880_VAS_MEAS_DELAY    (20)

#define VP880_VAS_MEAS_ERR      (100)   /**< Times 1.817ua = 187uA */
#define VP880_VAS_OVERHEAD      (5250)  /**< 5.25V for Temperature Variation */
#define VP880_VAS_MAX           (14250) /**< 14.25V per silicon */

#define VP880_CAL_SET            (1)    /**< Minimum time between tick */

/* Functions that are called only inside this file. */
static void
Vp880CalInit(
     VpDevCtxType *pDevCtx);

static void
Vp880AbvInit(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvInitAbs(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvSetAdc(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvSetAdcAbs(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvStateChange(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvStateChangeAbs(
    VpDevCtxType *pDevCtx);

static bool
Vp880AbvMeasure(
    VpDevCtxType *pDevCtx);

static bool
Vp880AbvMeasureAbs(
    VpDevCtxType *pDevCtx);

static void
Vp880AbvMakeAdjustment(
    Vp880DeviceObjectType *pDevObj,
    int16 *targetVoltY,
    int16 *targetVoltZ);

static int16
Vp880AdcSettling(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 adcConfig);

/* Functions for Cal Line */
static void
Vp880CalLineInit(
     VpLineCtxType *pLineCtx);

static VpStatusType
Vp880CalVas(
    VpLineCtxType *pLineCtx);

static void
Vp880VasPolRev(
    VpLineCtxType *pLineCtx);

static VpStatusType
Vp880CalVoc(
    VpLineCtxType *pLineCtx);

static void
Vp880VocMeasure(
    VpLineCtxType *pLineCtx);

static void
Vp880VocMeasureInvert(
    VpLineCtxType *pLineCtx);

static void
Vp880CalDone(
    VpLineCtxType *pLineCtx);

static void
Vp880VocInit(
    VpLineCtxType *pLineCtx);

static void
Vp880VocSetAdc(
    VpLineCtxType *pLineCtx);

static void
Vp880VocOffset(
    VpLineCtxType *pLineCtx);

static void
Vp880VocDone(
    VpLineCtxType *pLineCtx);

/**
 * Vp880CalLine()
 *  This function initiates a calibration operation for analog circuits
 * associated with a given line. See VP-API reference guide for more
 * information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalLine(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint16 tickRate = pDevObj->devProfileData.tickRate;
    VpLineStateType currentState = pLineObj->lineState.currentState;
    uint8 channelId = pLineObj->channelId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 opCond[VP880_OP_COND_LEN];

    /* Proceed if device state is either in progress or complete */
    if (pDevObj->status.state & (VP_DEV_INIT_CMP | VP_DEV_INIT_IN_PROGRESS)) {
    } else {
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Do not proceed if the device calibration is in progress. This could
     * damage the device.
     */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        VP_DOUT(DBG_CALIBRATION, ("Return NOT_INITIALIZED from Vp880CalLine()"));
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VP_DOUT(DBG_CALIBRATION, ("Running Cal Line on Channel %d at time %d",
        pLineObj->channelId, pDevObj->timeStamp));

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_JE) {
        pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
        pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
        VP_DOUT(DBG_CALIBRATION, ("ABS Cal Line Complete"));
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    Vp880CalLineInit(pLineCtx);

    switch(pLineObj->lineState.calType) {
        case VP_CSLAC_CAL_NONE:
            VP_DOUT(DBG_CALIBRATION, ("Cal Line -- VP_CSLAC_CAL_NONE"));

            /*
             * Since we're starting with VOC, check to make sure it can
             * be run.
             */
            switch(currentState) {
                case VP_LINE_STANDBY:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT);
                    pLineObj->calLineData.reversePol = FALSE;
                    break;

                case VP_LINE_STANDBY_POLREV:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT_POLREV);
                    pLineObj->calLineData.reversePol = TRUE;
                    break;

                case VP_LINE_OHT:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_STANDBY);
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT);
                    pLineObj->calLineData.reversePol = FALSE;
                    break;

                case VP_LINE_OHT_POLREV:
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_STANDBY_POLREV);
                    Vp880SetLineStateInt(pLineCtx, VP_LINE_OHT_POLREV);
                    pLineObj->calLineData.reversePol = TRUE;
                    break;

                default:
                    pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
            }

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_MIN_CAL_WAIT_TIME, tickRate);
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                VP_ACTIVATE_TIMER;

            pLineObj->calLineData.calState =  VP880_CAL_INIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_VOC;
            break;

        default:
            VP_DOUT(DBG_CALIBRATION, ("Cal Line -- BUSY"));
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_DEVICE_BUSY;
    }

    /* Reprogram the Operating Conditions Register, affected by Set Line State */
    opCond[0] = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        opCond);

    pLineObj->lineState.calType = VP_CSLAC_CAL_VOC;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880CalCodec()
 *  This function initiates a calibration operation for analog circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 maxChannels = pDevObj->staticInfo.maxChannels;
    uint8 chanNum;
    VpLineStateType currentState;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_JE) {
        pDevObj->deviceEvents.response |= VP_EVID_CAL_CMP;
        pDevObj->responseData = VP_CAL_SUCCESS;
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    if (mode == VP_DEV_CAL_NBUSY) {
        for (chanNum = 0; chanNum < maxChannels; chanNum++) {
            if (pDevCtx->pLineCtx[chanNum] != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;
                currentState = pLineObj->lineState.currentState;

                if (pLineObj->status & VP880_IS_FXO) {
                    if ((currentState == VP_LINE_FXO_TALK)
                     || (currentState == VP_LINE_FXO_LOOP_CLOSE)) {
                        pDevObj->deviceEvents.response |= VP_EVID_CAL_BUSY;
                        pDevObj->deviceEvents.response &= ~VP_EVID_CAL_CMP;
                        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                        return VP_STATUS_SUCCESS;
                    }
                } else {
                    if ((currentState != VP_LINE_STANDBY)
                     && (currentState != VP_LINE_DISCONNECT)) {
                        pDevObj->deviceEvents.response |= VP_EVID_CAL_BUSY;
                        pDevObj->deviceEvents.response &= ~VP_EVID_CAL_CMP;
                        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                        return VP_STATUS_SUCCESS;
                    }
                }
            }
        }
    }

    pDevObj->status.state |= VP_DEV_IN_CAL;

    if (pDevObj->stateInt & VP880_IS_ABS) { /* Start for ABS Device */
        if (Vp880SetCalFlags(pDevObj) == TRUE) {
            Vp880CalCodecInt(pDevCtx);
        }
    } else {    /* Start for Tracker Device */
        pDevObj->status.state |= VP_DEV_ABV_CAL;
        Vp880CalCodecInt(pDevCtx);
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetCalFlags()
 *  This function sets the calibration flags to define start of calibration and
 * calibration function to start with for the ABS device.
 *
 * Preconditions:
 *  The device must be created and initialized before calling this function.
 * This function should be called only by API-II internal functions, generally
 * CalCodec and InitDevice.
 *
 * Postconditions:
 *  Flags in the device object are set appropriately for the silicon revision.
 */
bool
Vp880SetCalFlags(
    Vp880DeviceObjectType *pDevObj)
{
    bool calStatus = FALSE;

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_JE) {
        /* Later than VC requires ABV and ABS calibration, start with ABV */
        pDevObj->status.state |= VP_DEV_ABV_CAL_ABS;
        calStatus = TRUE;
    } else if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] == VP880_REV_VC) {
        /* VC requires ABS calibration only. */
        pDevObj->status.state |= VP_DEV_ABS_BAT_CAL;
        calStatus = TRUE;
    } else {
        /* No calibration required for earlier than VC silicon */
    }

    if (calStatus == TRUE) {
        pDevObj->status.state |= VP_DEV_IN_CAL;
        pDevObj->calData.calState = VP880_CAL_INIT;
    } else {
        pDevObj->status.state &= ~VP_DEV_IN_CAL;
        pDevObj->calData.calState &= ~VP880_CAL_INIT;
    }
    return calStatus;
}


/**
 * Vp880CalCodecInt()
 *  This function initiates a calibration operation for analog circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalCodecInt(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    /*
     * Run appropriate calibration currently set. Note that the same calibration
     * function may be run for either Tracker or ABS devices.
     */
    if (pDevObj->status.state & VP_DEV_ABV_CAL) {
        VP_DOUT(DBG_CALIBRATION, ("Calling ABV Cal inside Vp880CalCodecInt() for Tracker"));
        Vp880CalAbv(pDevCtx);
        return VP_STATUS_SUCCESS;
    }

    if (pDevObj->status.state & VP_DEV_ABS_BAT_CAL) {
        VP_DOUT(DBG_CALIBRATION, ("Calling ABS Cal inside Vp880CalCodecInt()"));
        Vp880AbsCalibration(pDevCtx);
        return VP_STATUS_SUCCESS;
    }

    if (pDevObj->status.state & VP_DEV_ABV_CAL_ABS) {
        VP_DOUT(DBG_CALIBRATION, ("Calling ABS Cal inside Vp880CalCodecInt() for ABS"));
        Vp880CalAbvAbsDev(pDevCtx);
        return VP_STATUS_SUCCESS;
    }

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880AbsCalibration()
 *  This function should is called only by Vp880ApiTick() to run an ABS battery
 * calibrary algorithm. The result is a computed DC offset for each polartiy
 * state.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  Upon completion, the Init Device Complete event is generated and the device
 * is considered initialized. The offset values for each line in both normal and
 * reverse polarity are stored in the device object. Note that the offset is
 * independent of the line object since it is a function only of the physical
 * silicon.
 *
 * Note regarding Low Power Mode:
 *  This function can set the System State (SLIC State) directly without any
 * problem because it all occurs prior to lines being initialized.
 */
void
Vp880AbsCalibration(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 cmdData[VP880_MAX_MPI_DATA];
    uint8 switcherData[VP880_REGULATOR_CTRL_LEN];
    uint8 firstValue[VP880_DC_CAL_REG_LEN] = {0xF4, 0xE4};
    uint8 lastValue[VP880_DC_CAL_REG_LEN] = {0x00, 0x02};
    uint8 icr2Mods[VP880_ICR2_LEN];

    /*
     * To save on memory, the calibration state in terms of the state machine
     * state, channel, and which polarity is being tested are all stored in
     * a single byte. The mask for each parameter as follows.
     */
#define ABS_DC_CAL_STATE_BITS       0x3F
#define ABS_DC_CAL_CHAN_BITS        0x80
#define ABS_DC_CAL_POLREV_BITS      0x40

    /* State values corresponding to what the state is "doing" */
#define ABS_DC_CAL_INIT_STATE       0
#define ABS_DC_CAL_CONNECT_STATE    1
#define ABS_DC_CAL_MEAS_ADJ_STATE   2

#define ABS_CAL_INITIAL_DELAY   30
#define ABS_CAL_SAMPLE_DELAY    5

    uint8 channelId = ((pDevObj->calState & ABS_DC_CAL_CHAN_BITS) >> 7);

    /* If set, then running line in reverse polarity */
    uint8 revPolTest = (pDevObj->calState & ABS_DC_CAL_POLREV_BITS);

    uint8 chanNum;

    bool complete = FALSE;

    switch(pDevObj->calState & ABS_DC_CAL_STATE_BITS) {

        case ABS_DC_CAL_INIT_STATE:
            VP_DOUT(DBG_CALIBRATION, ("\n\rABS_DC_CAL_INIT_STATE at Time %d\n\r",
                pDevObj->timeStamp));

            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_RD,
                VP880_REGULATOR_CTRL_LEN, pDevObj->cachedSwCtrl);

            /* Steps 1 and first part of 2 */
            switcherData[0] = VP880_SWY_OFF;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, switcherData);

            cmdData[0] = VP880_SS_DISCONNECT;
            for (chanNum = 0; chanNum < maxChan; chanNum++) {
                VP_DOUT(DBG_CALIBRATION, ("Setting Channel %d to DISCONNECT at time %d",
                    chanNum, pDevObj->timeStamp));

                VpMpiCmd(deviceId, ecVal[chanNum], VP880_SYS_STATE_WRT,
                    VP880_SYS_STATE_LEN, cmdData);

                VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_RD,
                    VP880_ICR2_LEN, icr2Mods);
                icr2Mods[VP880_ICR2_FEED_CTRL_INDEX+1] |= VP880_ICR2_FEED_CTRL;
                icr2Mods[VP880_ICR2_FEED_CTRL_INDEX] &= ~VP880_ICR2_FEED_CTRL;
                VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_WRT,
                    VP880_ICR2_LEN, icr2Mods);
            }

            for (chanNum = 0; chanNum < maxChan; chanNum++) {
                VpMpiCmd(deviceId, ecVal[chanNum], VP880_DC_CAL_REG_WRT,
                    VP880_DC_CAL_REG_LEN, firstValue);
            }

            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
            pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;
            pDevObj->devTimer[VP_DEV_TIMER_ABSCAL] =
                MS_TO_TICKRATE(ABS_CAL_INITIAL_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            pDevObj->calState |= 0x80;  /* Start with Channel 2 */
            break;

        case ABS_DC_CAL_CONNECT_STATE:
            VP_DOUT(DBG_CALIBRATION, ("ABS_DC_CAL_CONNECT_STATE for Chan %d Polarity %d\n\r",
                channelId, revPolTest));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
                VP880_DC_CAL_REG_LEN, firstValue);

            /* Last part of 2 and Step 3 */
            cmdData[0] = (revPolTest) ?
                VP880_SS_ACTIVE_MID_BAT_PR : VP880_SS_ACTIVE_MID_BAT;

            VP_DOUT(DBG_CALIBRATION, ("Setting Channel %d to State 0x%02X at time %d",
                channelId, cmdData[0], pDevObj->timeStamp));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, cmdData);

            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
            pDevObj->calState |= ABS_DC_CAL_MEAS_ADJ_STATE;
            pDevObj->devTimer[VP_DEV_TIMER_ABSCAL] =
                MS_TO_TICKRATE(ABS_CAL_INITIAL_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            break;

        case ABS_DC_CAL_MEAS_ADJ_STATE:
            /* Most cases, we're returning. */
            pDevObj->devTimer[VP_DEV_TIMER_ABSCAL] =
                MS_TO_TICKRATE(ABS_CAL_SAMPLE_DELAY,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
                VP880_DC_CAL_REG_LEN, cmdData);

            VP_DOUT(DBG_CALIBRATION, ("\n\rABS_DC_CAL_MEAS_ADJ_STATE for Chan %d Polarity %d Value 0x%02X 0x%02X\n\r",
                channelId, revPolTest, cmdData[0], cmdData[1]));

            if (cmdData[VP880_DC_CAL_BLIM_INDEX] & VP880_DC_CAL_BLIM) {
                /* Good. Save this value */
                if (revPolTest) {
                    /* Saving the polarity reversal information */
                    pDevObj->absPolRevCal[channelId] =
                        cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0;

                    VP_DOUT(DBG_CALIBRATION, ("\n\rSaving PolRev 0x%02X for Ch %d\n\r",
                        pDevObj->absPolRevCal[channelId], channelId));
                } else {
                    /* Saving the normal polarity information */
                    pDevObj->absNormCal[channelId] =
                        cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0;

                    VP_DOUT(DBG_CALIBRATION, ("\n\rSaving Normal 0x%02X for Ch %d\n\r",
                        pDevObj->absNormCal[channelId], channelId));
                }

                /* Determine if there's anything else to do */
                switch (channelId) {
                    case 1:
                        if (revPolTest) {
                            /* Change to channel 1, Normal Polarity */
                            pDevObj->calState |= ABS_DC_CAL_CHAN_BITS;
                            pDevObj->calState &= ~ABS_DC_CAL_POLREV_BITS;
                            pDevObj->calState &= 0x7F;

                            VP_DOUT(DBG_CALIBRATION, ("\n\rChanging to Normal Polarity for Channel 1 0x%02X\n\r",
                                pDevObj->calState));
                        } else {
                            /* Change to Reverse Polarity */
                            VP_DOUT(DBG_CALIBRATION, ("\n\rChanging to PolRev, Channel %d 0x%02X\n\r",
                                channelId, pDevObj->calState));
                            pDevObj->calState |= ABS_DC_CAL_POLREV_BITS;
                        }

                        /* Repeat with new connections */
                        pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
                        pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;
                        break;

                    case 0:
                        if (revPolTest) {
                            /* Done */
                            VP_DOUT(DBG_CALIBRATION, ("\n\rCal Complete For Device %d\n\r", deviceId));
                            complete = TRUE;
                        } else {
                            /* Change to Reverse Polarity */
                            pDevObj->calState |= ABS_DC_CAL_POLREV_BITS;

                            /* Repeat with new connections */
                            pDevObj->calState &= ~ABS_DC_CAL_STATE_BITS;
                            pDevObj->calState |= ABS_DC_CAL_CONNECT_STATE;

                            VP_DOUT(DBG_CALIBRATION, ("\n\rChanging to PolRev for Channel %d 0x%02X\n\r",
                                channelId, pDevObj->calState));
                        }
                        break;

                    default:
                        /* Error */
                        VP_DOUT(VP_DBG_ERROR, ("\n\rChannelId Error %d\n\r", channelId));
                        complete = TRUE;
                        break;
                }
            } else {
                /* Change the current offset and try again */
                if (cmdData[VP880_DC_CAL_ABS_INDEX] & 0x80) {
                    if ((cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0) == 0x80) {
                        cmdData[VP880_DC_CAL_ABS_INDEX] = 0;
                    } else {
                        cmdData[VP880_DC_CAL_ABS_INDEX] -= 16;
                    }
                } else {
                    if ((cmdData[VP880_DC_CAL_ABS_INDEX] & 0xF0) == 0x70) {
                        /*
                         * Something wrong happened. Restore back to 0 and end
                         * algorithm.
                         */
                        complete = TRUE;
                        VP_DOUT(VP_DBG_ERROR, ("\n\rCalibration Algorithm Error 0x%02X\n\r",
                            pDevObj->status.state));
                    } else {
                        cmdData[VP880_DC_CAL_ABS_INDEX] += 16;
                    }
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
                    VP880_DC_CAL_REG_LEN, cmdData);

                VP_DOUT(DBG_CALIBRATION, ("\n\rAdjusting Offset 0x%02X\n\r",
                    cmdData[0]));
            }
            break;

        default:
            /* oops. shouldn't be here. */
            VP_DOUT(VP_DBG_ERROR, ("\n\rCalibration Case Error %d \n\r",
                (pDevObj->calState & ABS_DC_CAL_STATE_BITS)));
            complete = TRUE;
            break;
    }

    if (complete == TRUE) {
        /* We're done. Disable timers */
        pDevObj->devTimer[VP_DEV_TIMER_ABSCAL] = 0;

        cmdData[0] = VP880_SS_DISCONNECT;
        for (chanNum = 0; chanNum < maxChan; chanNum++) {
            VP_DOUT(DBG_CALIBRATION, ("Setting Channel %d to VP880_SS_DISCONNECT at time %d",
                cmdData[0], pDevObj->timeStamp));

            VpMpiCmd(deviceId, ecVal[chanNum], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, cmdData);

            VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_RD,
                VP880_ICR2_LEN, icr2Mods);
            icr2Mods[VP880_ICR2_FEED_CTRL_INDEX+1] &= ~VP880_ICR2_FEED_CTRL;
            VpMpiCmd(deviceId, ecVal[chanNum], VP880_ICR2_WRT,
                VP880_ICR2_LEN, icr2Mods);
        }

        VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_DC_CAL_REG_WRT,
            VP880_DC_CAL_REG_LEN, lastValue);

        VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
            VP880_REGULATOR_CTRL_LEN, pDevObj->cachedSwCtrl);

        if (pDevObj->status.state & VP_DEV_INIT_IN_PROGRESS) {
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
        } else {
            pDevObj->deviceEvents.response |= VP_EVID_CAL_CMP;
        }
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_IN_CAL);

        pDevObj->calState = VP880_CAL_INIT;
        pDevObj->status.state &= ~VP_DEV_ABS_BAT_CAL;
    }
}

/**
 * Vp880CalAbv()
 *  This function initiates a calibration operation for Absolute Switcher
 * circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information. SWYV SWZV are global for every Channels
 * Line must be in Disconnect state before to start the Calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalAbv(
     VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType  status = VP_STATUS_SUCCESS;
    uint8 ecVal;
    uint8 fxsChannels;
    bool calCleanup = FALSE;

    if ((pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL)
     || (pDevObj->stateInt & VP880_LINE1_IS_FXO)) {
        ecVal = VP880_EC_CH1;
        fxsChannels = 1;
    } else {
        ecVal = VP880_EC_CH1 | VP880_EC_CH2;
        fxsChannels = 2;
    }

    if (pDevObj->calData.calState == VP880_CAL_INIT
        || pDevObj->calData.calState == VP880_CAL_EXIT) {
        VP_DOUT(DBG_CALIBRATION,("\n Vp880CalAbv:  - Setting to Vp880CalInit"));

        pDevObj->calData.calState = VP880_CAL_INIT;
        Vp880CalInit(pDevCtx);
    }

    switch(pDevObj->calData.calState) {
        case VP880_CAL_INIT:
            VP_DOUT(DBG_CALIBRATION,("\nVp880CalAbv: - Running Vp880AbvInit"));
            Vp880AbvInit(pDevCtx);
            break;

        case VP880_CAL_ADC:
            VP_DOUT(DBG_CALIBRATION,("\nVp880CalAbv: - Running Vp880AbvSetAdc"));
            Vp880AbvSetAdc(pDevCtx);
            break;

        case VP880_CAL_STATE_CHANGE:
            VP_DOUT(DBG_CALIBRATION,("\nVp880CalAbv: - Running Vp880AbvStateChange"));
            Vp880AbvStateChange(pDevCtx);
            break;

        case VP880_CAL_MEASURE:
            VP_DOUT(DBG_CALIBRATION,("\nVp880CalAbv - Running Vp880AbvMeasure"));
            Vp880AbvMeasure(pDevCtx);
            break;

        case VP880_CAL_DONE:
            VP_DOUT(DBG_CALIBRATION, ("\nABV Cal Done"));
            calCleanup = TRUE;
            pDevObj->responseData = VP_CAL_SUCCESS;
            break;

        case VP880_CAL_ERROR:
            /* Fall through intentional */
        default:
            VP_DOUT(DBG_CALIBRATION,("\nVp880CalAbv: ERROR - Cal Done"));
            calCleanup = TRUE;
            status = VP_STATUS_FAILURE;
            pDevObj->responseData = VP_CAL_FAILURE;
            break;
    }

    if (calCleanup == TRUE) {
        uint8 convCfg[VP880_CONV_CFG_LEN];

        pDevObj->calData.calState = VP880_CAL_EXIT;
        if (pDevObj->status.state & VP_DEV_INIT_IN_PROGRESS) {
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
            pDevObj->status.state |= VP_DEV_INIT_CMP;
        } else {
            pDevObj->deviceEvents.response |= VP_EVID_CAL_CMP;
        }
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_IN_CAL);
        pDevObj->status.state &= ~VP_DEV_ABV_CAL;

        /* Reset Line states */
        VP_DOUT(DBG_CALIBRATION, ("Calibration Cleanup -- Setting Channel 0 to State 0x%02X at time %d",
            pDevObj->calData.abvData.sysState[0][0], pDevObj->timeStamp));
        VP_DOUT(DBG_CALIBRATION, ("Calibration Cleanup -- Setting Channel 1 to State 0x%02X at time %d",
            pDevObj->calData.abvData.sysState[1][0], pDevObj->timeStamp));

        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[0][0]);
        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[1][0]);

        /* Restore Converter Configuration */
        convCfg[0] = (VP880_METALLIC_AC_V | pDevObj->txBufferDataRate);
        VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_CONV_CFG_WRT,
            VP880_CONV_CFG_LEN, convCfg);
    }

    return status;
}

/**
 * Vp880AbvInit()
 *  This function initiates a calibration operation for ABV
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880AbvInit(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 disnVal[VP880_DISN_LEN] = {0x00};
    uint8 channelId;

    uint8 isrpMods[VP880_INT_SWREG_PARAM_LEN] = {
        0x00, 0x40, 0x00, 0x40, 0x00, 0x40
    };

    uint8 icr3[VP880_ICR3_LEN] = {0x30, 0x20, 0x00, 0x00};
    uint8 icr4[VP880_ICR4_LEN] = {0x01, 0x01, 0x00, 0x00};
    uint8 icr2[VP880_ICR2_LEN] = {0x00, 0xEC, 0x2C, 0x2C};

    uint8 data, ecVal;
    uint8 swCal[VP880_BAT_CALIBRATION_LEN];

    /*
     * Initialize and use to measure each channels offset and voltage using
     * same functions.
     */
    pDevObj->calData.abvData.passCnt = 0;

    /* Read (for restore) internal switcher parameters */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_SWREG_PARAM_RD,
        VP880_INT_SWREG_PARAM_LEN, pDevObj->calData.abvData.isrpData);

    /* Channel specific registers to restore at end of calibration */
    for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
        ecVal = (channelId == 0) ? VP880_EC_CH1 : VP880_EC_CH2;

        /* Save off current slic state */
        VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_RD, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[channelId][0]);

        /*
         * Disable switcher by setting duty cycle = 0. Global, so only need
         * to do once.
         */
        if (channelId == 0) {
            VpMpiCmd(deviceId, ecVal, VP880_INT_SWREG_PARAM_WRT,
                VP880_INT_SWREG_PARAM_LEN, isrpMods);
        }

        /* Clear existing correction factors */
        VpMpiCmd(deviceId, ecVal, VP880_BAT_CALIBRATION_RD,
            VP880_BAT_CALIBRATION_LEN, swCal);
        swCal[0] &= ~(VP880_BAT_CAL_SWCAL_MASK);
        VpMpiCmd(deviceId, ecVal, VP880_BAT_CALIBRATION_WRT,
            VP880_BAT_CALIBRATION_LEN, swCal);

        VpMpiCmd(deviceId, ecVal, VP880_ICR2_RD,
            VP880_ICR2_LEN, &pDevObj->calData.abvData.icr2[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_ICR3_RD,
            VP880_ICR3_LEN,  &pDevObj->calData.abvData.icr3[channelId][0]);
        VP_DOUT(DBG_CALIBRATION, ("Saving ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
            pDevObj->calData.abvData.icr3[channelId][0],
            pDevObj->calData.abvData.icr3[channelId][1],
            pDevObj->calData.abvData.icr3[channelId][2],
            pDevObj->calData.abvData.icr3[channelId][3],
            channelId));

        VpMpiCmd(deviceId, ecVal, VP880_ICR4_RD,
            VP880_ICR4_LEN,  &pDevObj->calData.abvData.icr4[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_DISN_RD, VP880_DISN_LEN,
            &pDevObj->calData.abvData.disnVal[channelId][0]);
    }

    if ((pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL)
     || (pDevObj->stateInt & VP880_LINE1_IS_FXO)) {
        ecVal = VP880_EC_CH1;
    } else {
        ecVal = VP880_EC_CH1 | VP880_EC_CH2;
    }

    /* Disable the switchers */
    VpMpiCmd(deviceId, ecVal, VP880_ICR2_WRT, VP880_ICR2_LEN, icr2);

    /* Force sink supply current to reduce voltage */
    data = VP880_SS_ACTIVE;
    VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
        &data);

    /* Enable line control to access VBAT sense */
    VpMpiCmd(deviceId, ecVal, VP880_ICR3_WRT, VP880_ICR3_LEN, icr3);
    VP_DOUT(DBG_CALIBRATION, ("1. Calibration: Write ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
        icr3[0], icr3[1], icr3[2], icr3[3], channelId));

    /* Enable ADC */
    VpMpiCmd(deviceId, ecVal, VP880_ICR4_WRT, VP880_ICR4_LEN, icr4);

    /* Set compression to Linear Mode and default AC Coefficients */
    data = VP880_LINEAR_CODEC;
    VpMpiCmd(deviceId, ecVal, VP880_OP_FUNC_WRT, VP880_CODEC_REG_LEN,
        &data);

    /* Cut TX/RX PCM and disable HPF */
    data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
    VpMpiCmd(deviceId, ecVal, VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    /* Set DISN = 0. */
    VpMpiCmd(deviceId, ecVal, VP880_DISN_WRT, VP880_DISN_LEN, disnVal);

    /* Wait at least 100ms before collecting data */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] =
    MS_TO_TICKRATE(VP880_CAL_ABV_LONG,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    /* Advance state to measure ADC offset */
    pDevObj->calData.calState = VP880_CAL_STATE_CHANGE;
} /* end Vp880AbvInit */

/**
 * Vp880AbvInitAbs()
 *  This function initiates a calibration operation for ABV associated with all
 * the lines of an ABS device. See VP-API reference guide for more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880AbvInitAbs(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 disnVal[VP880_DISN_LEN] = {0x00};
    uint8 channelId, ecVal;
    uint8 data;
    uint8 swCal[VP880_BAT_CALIBRATION_LEN] = {0x00, 0x10};

    /*
     * Initialize and use to measure each channels offset and voltage using
     * same functions.
     */
    pDevObj->calData.abvData.passCnt = 0;

    /* Device Mode */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN,
        &data);
    data &= ~(VP880_DEV_MODE_TEST_DATA);
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
        &data);

    /* Channel specific registers to restore at end of calibration */
    for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
        ecVal = (channelId == 0) ? VP880_EC_CH1 : VP880_EC_CH2;

        /* Clear existing correction factors */
        VpMpiCmd(deviceId, ecVal, VP880_BAT_CALIBRATION_WRT,
            VP880_BAT_CALIBRATION_LEN, swCal);

        VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_RD, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[channelId][0]);

        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DISN_RD, VP880_DISN_LEN,
            &pDevObj->calData.abvData.disnVal[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_OP_FUNC_RD, VP880_OP_FUNC_LEN,
            &pDevObj->calData.abvData.opFunc[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_OP_COND_RD, VP880_OP_COND_LEN,
            &pDevObj->calData.abvData.opCond[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_CONV_CFG_RD, VP880_CONV_CFG_LEN,
            &pDevObj->calData.abvData.converterCfg[channelId][0]);
    }

    /* Set for Linear Mode and disable AC Coefficients */
    data = VP880_LINEAR_CODEC;
    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_OP_FUNC_WRT,
        VP880_OP_FUNC_LEN, &data);

    /* Cut TX/RX PCM and disable HPF */
    data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_OP_COND_WRT,
        VP880_OP_COND_LEN,  &data);

    /* Set DISN = 0. */
    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_DISN_WRT,
        VP880_DISN_LEN, disnVal);

    /*
     * Disable Switchers and wait for discharge. Typically, 2.5 seconds for a
     * warm re-cal
     */
    data = (VP880_SWY_OFF | VP880_SWZ_OFF);
    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_REGULATOR_CTRL_WRT,
        VP880_REGULATOR_CTRL_LEN, &data);

    /* Force sink supply current to reduce voltage. */
    data = VP880_SS_ACTIVE;
    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_SYS_STATE_WRT,
        VP880_SYS_STATE_LEN, &data);

    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] =
    MS_TO_TICKRATE(VP880_CAL_ABV_ABS_INIT,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    /* Advance state to measure ADC offset */
    pDevObj->calData.calState = VP880_CAL_STATE_CHANGE;
} /* end Vp880AbvInitAbs */

/**
 * Vp880AbvStateChange ()
 *  This function changes the line state and sets the converter configuration
 * in order to give time for the converter to stabilize before taking the first
 * set of data.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 */
void
Vp880AbvStateChange(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data, ecVal;
    uint8 converterCfg[VP880_CONV_CFG_LEN] = {VP880_SWITCHER_Y};

    if ((pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL)
     || (pDevObj->stateInt & VP880_LINE1_IS_FXO)) {
        ecVal = VP880_EC_CH1;
    } else {
        ecVal = VP880_EC_CH1 | VP880_EC_CH2;
    }

    /* Disconnect to enable switcher circuitry, codec activated */
    data = (VP880_SS_DISCONNECT | VP880_SS_ACTIVATE_MASK);
    VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
        &data);

    /* Don't care about the data, just force the converter configuration */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        converterCfg);

    /* Allow the converter to stabilize */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] =
        MS_TO_TICKRATE(VP880_CAL_ABV_LONG, /*VP880_CAL_ABV_DELAY,*/
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    pDevObj->calData.calState =  VP880_CAL_ADC;
} /* end Vp880AbvStateChange */

/**
 * Vp880AbvStateChangeAbs()
 *  This function TBD
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 */
void
Vp880AbvStateChangeAbs(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 converterCfg[VP880_CONV_CFG_LEN];

    /* Don't care about the data, just force the converter configuration */
    converterCfg[0] = VP880_SWITCHER_Y;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        converterCfg);

    converterCfg[0] = VP880_SWITCHER_Z;
    VpMpiCmd(deviceId, VP880_EC_CH2, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        converterCfg);

    /* Allow the converter to stabilize */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] = MS_TO_TICKRATE(VP880_CAL_ABV_LONG,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    pDevObj->calData.calState =  VP880_CAL_ADC;
} /* end Vp880AbvStateChangeAbs */

/**
 * Vp880AbvSetAdc()
 *  This function set the converter to read the right pcm set the right state
 * machine
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 */
void
Vp880AbvSetAdc(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal;
    uint8 swYZ[VP880_REGULATOR_PARAM_LEN];
    bool abvSetAdcDone = FALSE;

    if ((pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL)
     || (pDevObj->stateInt & VP880_LINE1_IS_FXO)) {
        ecVal = VP880_EC_CH1;
    } else {
        ecVal = VP880_EC_CH1 | VP880_EC_CH2;
    }

    /* Now we'll switch to channel specific measurements */
    /* Read SWY from first channel, SWZ from second channel */
    if (pDevObj->calData.abvData.passCnt == 0) {
        pDevObj->calData.abvData.swyOffset[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH1, VP880_SWITCHER_Y);

        VP_DOUT(DBG_CALIBRATION, ("Chan 0 Offset: SWY %d",
            ((pDevObj->calData.abvData.swyOffset[0] * 7324) / 10000)));

        if (ecVal == VP880_EC_CH1) {
            abvSetAdcDone = TRUE;
        } else {
            uint8 converterCfg[VP880_CONV_CFG_LEN] = {VP880_SWITCHER_Z};

            VpMpiCmd(deviceId, VP880_EC_CH2, VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            /* Wait for converter data to settle */
            pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] =
                MS_TO_TICKRATE(VP880_CAL_ABV_DELAY,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

            pDevObj->calData.abvData.passCnt++;
        }
    } else {
        pDevObj->calData.abvData.swzOffset[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH2, VP880_SWITCHER_Z);

        VP_DOUT(DBG_CALIBRATION, ("Chan 1 Offset: SWZ %d",
            ((pDevObj->calData.abvData.swzOffset[0] * 7324) / 10000)));
        abvSetAdcDone = TRUE;
    }

    if (abvSetAdcDone == TRUE) {
        /*
         * Restore internal switcher parameters to take voltage measurement. This
         * is a global register so it doesn't matter which EC value is used.
         */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_SWREG_PARAM_WRT,
            VP880_INT_SWREG_PARAM_LEN, pDevObj->calData.abvData.isrpData);

        pDevObj->calData.calSet = pDevObj->swParams[VP880_SWREG_RING_V_BYTE];

        VP_DOUT(DBG_CALIBRATION, ("Vp880AbvInit: ABV Set Value Target %d",
            ((pDevObj->calData.calSet * 5) + 5)));

        /*
         * Copy the Ringing Voltage to the Floor Voltage, everything else
         * directly from the device profile
         */
        swYZ[0] = pDevObj->swParams[0];

        swYZ[VP880_SWY_LOCATION] =
            (pDevObj->swParams[VP880_SWZ_LOCATION] & VP880_VOLTAGE_MASK);
        swYZ[VP880_SWY_LOCATION] |=
            (pDevObj->swParams[VP880_SWY_LOCATION] & ~VP880_VOLTAGE_MASK);

        swYZ[2] = pDevObj->swParams[2];

        VP_DOUT(DBG_CALIBRATION, ("Vp880AbvInit: swYZ: 0x%02X 0x%02X 0x%02X",
            swYZ[0], swYZ[1], swYZ[2]));

        /* Program switcher floor voltage to target ringing voltage */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_REGULATOR_PARAM_WRT,
            VP880_REGULATOR_PARAM_LEN, swYZ);

        /* Things will take time to settle after programming switcher. */
        pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] = MS_TO_TICKRATE(VP880_CAL_ABV_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

        pDevObj->calData.calState = VP880_CAL_MEASURE;
    }
} /* end Vp880AbvSetAdc  */

/**
 * Vp880AbvSetAdcAbs()
 *  This function set the converter to read the right pcm set the right state
 * machine
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 */
void
Vp880AbvSetAdcAbs(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    int16 swyOffsetNew, swzOffsetNew;

    /* Read SWY from first channel, SWZ from second channel */
    if (pDevObj->calData.abvData.passCnt == 0) {
        /*
         * Take first measurement, than increment the pass counter to track
         * how long it takes to settle. The time it takes will determine an
         * offset correction to the final value.
         */
        pDevObj->calData.abvData.swyOffset[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH1, VP880_SWITCHER_Y);

        VP_DOUT(DBG_CALIBRATION, ("1. Chan 0 Offset: SWY %d",
            ((pDevObj->calData.abvData.swyOffset[0] * 7324) / 10000)));

        pDevObj->calData.abvData.swzOffset[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH2, VP880_SWITCHER_Z);

        VP_DOUT(DBG_CALIBRATION, ("1. Chan 1 Offset: SWZ %d",
            ((pDevObj->calData.abvData.swzOffset[0] * 7324) / 10000)));

        pDevObj->calData.abvData.passCnt++;
    } else {
        swyOffsetNew =
            Vp880AdcSettling(deviceId, VP880_EC_CH1, VP880_SWITCHER_Y);

        VP_DOUT(DBG_CALIBRATION, ("2. Chan 0 Offset: SWY New %d",
            ((swyOffsetNew * 7324) / 10000)));

        swzOffsetNew =
            Vp880AdcSettling(deviceId, VP880_EC_CH2, VP880_SWITCHER_Z);

        VP_DOUT(DBG_CALIBRATION, ("2. Chan 1 Offset: SWZ New %d",
            ((swzOffsetNew * 7324) / 10000)));

        /* Repeat until delta between two samples is less than max error */
        if ((ABS(swyOffsetNew - pDevObj->calData.abvData.swyOffset[0])
                <= VP880_CAL_ABV_SAMPLE_ERR) &&
            (ABS(swzOffsetNew - pDevObj->calData.abvData.swzOffset[0])
                <= VP880_CAL_ABV_SAMPLE_ERR)) {

            pDevObj->calData.calState = VP880_CAL_MEASURE;
        } else {
            pDevObj->calData.abvData.passCnt++;

            /*
             * Error is exceeded between consecutive values. Terminate the loop
             * if we reached max number of iterations.
             */
            if (pDevObj->calData.abvData.passCnt >= VP880_CAL_ABV_SAMPLE_MAX) {
                pDevObj->calData.calState = VP880_CAL_MEASURE;
            }
        }
        pDevObj->calData.abvData.swyOffset[0] = swyOffsetNew;
        pDevObj->calData.abvData.swzOffset[0] = swzOffsetNew;
    }

    if (pDevObj->calData.calState == VP880_CAL_MEASURE) {
        uint8 data = (VP880_SWY_LP | VP880_SWZ_LP);

        pDevObj->calData.abvData.initChange = TRUE;

        /* Re-enable the switchers for target measurement */
        VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2),
            VP880_REGULATOR_CTRL_WRT, VP880_REGULATOR_CTRL_LEN, &data);
    }

    /* Things will take time to settle after programming switcher. */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] = MS_TO_TICKRATE(VP880_CAL_ABV_SAMPLE,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

} /* end Vp880AbvSetAdcAbs */

/**
 * Vp880AbvMeasure()
 *  This function read switcher value and compare with the value read from the
 * pcm data if the value is bigger than 1.25v this function will make a
 * correction  voltage.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Battery calibration registers are adjusted. Channel specific registers are
 * restored.
 */
bool
Vp880AbvMeasure(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal;
    int32 abvError, abvTarget;
    uint8 swCal[VP880_BAT_CALIBRATION_LEN];
    uint16 swCalError;
    uint8 channelId;

    if ((pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL)
     || (pDevObj->stateInt & VP880_LINE1_IS_FXO)) {
        ecVal = VP880_EC_CH1;
    } else {
        ecVal = VP880_EC_CH1 | VP880_EC_CH2;
    }

    /* Now we'll switch to channel specific measurements */
    /* Read SWY from first channel */
    pDevObj->calData.abvData.swyVolt[0] =
        Vp880AdcSettling(deviceId, VP880_EC_CH1, VP880_SWITCHER_Y);

    VP_DOUT(DBG_CALIBRATION, ("Chan 0 Voltage: SWY %d",
        ((pDevObj->calData.abvData.swyVolt[0] * 7324) / 10000)));

    /* Now have all data necessary to compute error and adjust channel 0 */
    abvTarget = (pDevObj->calData.calSet * 5) + 5;   /* Gets it to V scale */
    abvTarget *= 1000;
    abvTarget *= 1000;
    abvTarget /= 7324;   /* Now we're scaled to the PCM data */

    abvError = abvTarget -
        (pDevObj->calData.abvData.swyVolt[0]
       - pDevObj->calData.abvData.swyOffset[0]
       - 41);   /* 41 = ~300mV at 7.324mV scale */

    VP_DOUT(DBG_CALIBRATION, ("Chan 0 Voltage Error: SWY %d",
        (((int16)abvError * 7324) / 10000)));

    /* Write the correction value to CH1 register. Steps in 1.25V increment */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_BAT_CALIBRATION_RD,
        VP880_BAT_CALIBRATION_LEN, swCal);
    swCal[0] &= ~(VP880_BAT_CAL_SWCAL_MASK);

    /* Conversion from 7.324mV to 1.25V */
    swCalError = (ABS(abvError) / 171);
    if (((ABS(abvError) + 85) /  171) > swCalError) {
        swCalError+=1;
    }
    swCalError = (swCalError > 3) ? 3 : swCalError;
    swCal[0] |= (swCalError << 3);

    /*
     * Positive error means voltage is too low (not negative enough). Positive
     * adjustment makes the battery voltage more negative.
     */
    swCal[0] |= (abvError > 0) ? 0 : VP880_BAT_CAL_SWCAL_SIGN;

    VP_DOUT(DBG_CALIBRATION, ("Ch 0: Battery Calibration Correction 0x%02X 0x%02X",
        swCal[0], swCal[1]));

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_BAT_CALIBRATION_WRT,
        VP880_BAT_CALIBRATION_LEN, swCal);

    if (ecVal == (VP880_EC_CH1 | VP880_EC_CH2)) {
        /* Read SWZ voltages from second channel */
        pDevObj->calData.abvData.swzVolt[1] =
            Vp880AdcSettling(deviceId, VP880_EC_CH2, VP880_SWITCHER_Z);

        VP_DOUT(DBG_CALIBRATION, ("Chan 1 Voltage: SWZ %d",
            ((pDevObj->calData.abvData.swzVolt[1] * 7324) / 10000)));

        abvError = abvTarget -
            (pDevObj->calData.abvData.swzVolt[1]
           - pDevObj->calData.abvData.swzOffset[1]
           - 41);   /* 41 = ~300mV at 7.324mV scale */

        VP_DOUT(DBG_CALIBRATION, ("Chan 1 (SWZ): Offset %d Voltage %d Error Raw %d Error Converted %d",
            pDevObj->calData.abvData.swzOffset[1],
            pDevObj->calData.abvData.swzVolt[1],
            (int16)abvError,
            (((int16)abvError * 7324) / 10000)));

        /* Write the correction value to CH2 register. Steps in 1.25V increment */
        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_BAT_CALIBRATION_RD,
            VP880_BAT_CALIBRATION_LEN, swCal);
        swCal[0] &= ~(VP880_BAT_CAL_SWCAL_MASK);

        /* Conversion from 7.324mV to 1.25V */
        swCalError = (ABS(abvError) / 171);
        if (((ABS(abvError) + 85) /  171) > swCalError) {
            swCalError+=1;
        }
        swCalError = (swCalError > 3) ? 3 : swCalError;
        swCal[0] |= (swCalError << 3);

        /*
         * Positive error means voltage is too low (not negative enough).
         * Positive adjustment makes the battery voltage more negative.
         */
        swCal[0] |= (abvError > 0) ? 0 : VP880_BAT_CAL_SWCAL_SIGN;

        VP_DOUT(DBG_CALIBRATION, ("Ch 1: Battery Calibration Correction 0x%02X 0x%02X",
            swCal[0], swCal[1]));

        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_BAT_CALIBRATION_WRT,
            VP880_BAT_CALIBRATION_LEN, swCal);
    }

    /* Restore Switching Regulator Parameters */
    VpMpiCmd(deviceId, ecVal, VP880_REGULATOR_PARAM_WRT,
        VP880_REGULATOR_PARAM_LEN, pDevObj->swParams);

    /* Channel specific registers to restore at end of calibration */
    for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
        ecVal = (channelId == 0) ? VP880_EC_CH1 : VP880_EC_CH2;

        VpMpiCmd(deviceId, ecVal, VP880_ICR2_WRT,
            VP880_ICR2_LEN, &pDevObj->calData.abvData.icr2[channelId][0]);

        VP_DOUT(DBG_CALIBRATION, ("Calibration Restore: ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
            pDevObj->calData.abvData.icr3[channelId][0],
            pDevObj->calData.abvData.icr3[channelId][1],
            pDevObj->calData.abvData.icr3[channelId][2],
            pDevObj->calData.abvData.icr3[channelId][3],
            channelId));

        VpMpiCmd(deviceId, ecVal, VP880_ICR3_WRT,
            VP880_ICR3_LEN,  &pDevObj->calData.abvData.icr3[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_ICR4_WRT,
            VP880_ICR4_LEN,  &pDevObj->calData.abvData.icr4[channelId][0]);

        VpMpiCmd(deviceId, ecVal, VP880_DISN_WRT, VP880_DISN_LEN,
            &pDevObj->calData.abvData.disnVal[channelId][0]);
    }

    pDevObj->calData.calState = VP880_CAL_DONE;

    /* Things will take time to settle. */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] = MS_TO_TICKRATE(VP880_CAL_ABV_LONG,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    return TRUE;
} /*end Vp880AbvMeasure */

/**
 * Vp880AbvMeasureAbs()
 *  This is the last functional step for ABV calibration on ABS devices. It
 * takes the SWY and SWZ measurements, computes the error, makes the adjustment,
 * and restores line registers to values prior to running calibration.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Battery calibration registers are adjusted. Channel specific registers are
 * restored.
 */
bool
Vp880AbvMeasureAbs(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal;
    uint8 channelId;

    if (pDevObj->calData.abvData.initChange == TRUE) {
        /* Make sure converters are configured correctly */
        uint8 converterCfg[VP880_CONV_CFG_LEN];
        uint8 data = (VP880_SWY_HP | VP880_SWZ_HP);

        /* Don't care about the data, just force the converter configuration */
        converterCfg[0] = VP880_SWITCHER_Y;
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
            converterCfg);

        converterCfg[0] = VP880_SWITCHER_Z;
        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
            converterCfg);

        /* Re-adjust switchers for target power control */
        VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2),
            VP880_REGULATOR_CTRL_WRT, VP880_REGULATOR_CTRL_LEN, &data);

        pDevObj->calData.abvData.initChange = FALSE;
    } else {
        uint8 swRegParam[VP880_REGULATOR_PARAM_LEN];
        int16 targetVoltY, targetVoltZ;
        uint8 data;

        pDevObj->calData.abvData.swyVolt[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH1, VP880_SWITCHER_Y);

        VP_DOUT(DBG_CALIBRATION, ("Chan 0 Voltage: SWY %d",
            ((pDevObj->calData.abvData.swyVolt[0] * 7324) / 10000)));

        pDevObj->calData.abvData.swzVolt[0] =
            Vp880AdcSettling(deviceId, VP880_EC_CH2, VP880_SWITCHER_Z);

        VP_DOUT(DBG_CALIBRATION, ("Chan 1 Voltage: SWZ %d",
            ((pDevObj->calData.abvData.swzVolt[0] * 7324) / 10000)));

        /* Compute Errors and make corrections */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_REGULATOR_PARAM_RD,
            VP880_REGULATOR_PARAM_LEN, swRegParam);

        /* Now have all data necessary to compute error and adjust channel 0 */
        targetVoltY = (swRegParam[VP880_SWY_LOCATION] & VP880_VOLTAGE_MASK);
        targetVoltZ = (swRegParam[VP880_SWZ_LOCATION] & VP880_VOLTAGE_MASK);

        Vp880AbvMakeAdjustment(pDevObj, &targetVoltY, &targetVoltZ);

        /* Channel specific registers to restore at end of calibration */
        for (channelId = 0;
             channelId < pDevObj->staticInfo.maxChannels;
             channelId++) {

            ecVal = (channelId == 0) ? VP880_EC_CH1 : VP880_EC_CH2;

            VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
                &pDevObj->calData.abvData.sysState[channelId][0]);

            VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DISN_WRT, VP880_DISN_LEN,
                &pDevObj->calData.abvData.disnVal[channelId][0]);

            VpMpiCmd(deviceId, ecVal, VP880_OP_FUNC_WRT, VP880_OP_FUNC_LEN,
                &pDevObj->calData.abvData.opFunc[channelId][0]);

            VpMpiCmd(deviceId, ecVal, VP880_OP_COND_WRT, VP880_OP_COND_LEN,
                &pDevObj->calData.abvData.opCond[channelId][0]);

            VpMpiCmd(deviceId, ecVal, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
                &pDevObj->calData.abvData.converterCfg[channelId][0]);
        }

        /* Device Mode */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN,
            &data);
        data |= VP880_DEV_MODE_TEST_DATA;
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
            &data);

        pDevObj->calData.calState = VP880_CAL_DONE;
    }

    /* Things will take time to settle. */
    pDevObj->devTimer[VP_DEV_TIMER_ABV_CAL] = MS_TO_TICKRATE(VP880_CAL_ABV_SAMPLE,
        pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    return TRUE;
} /* end Vp880AbvMeasureAbs */

/**
 * Vp880AbvMakeAdjustment()
 *  This function computes the measured error of ABV voltage and uses some
 * logic based on decay time to determine the device correction.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Battery calibration registers are adjusted.
 */
void
Vp880AbvMakeAdjustment(
    Vp880DeviceObjectType *pDevObj,
    int16 *targetVoltY,
    int16 *targetVoltZ)
{
    VpDeviceIdType deviceId = pDevObj->deviceId;
    int32 abvError, abvTarget;
    uint8 swCal[VP880_BAT_CALIBRATION_LEN];
    uint16 swCalError;

    /*
     * Offset correction is 600mV if decay took a long time, -300mV if it
     * occurred rapidly.
     */
    int8 offsetCorrection =
        (pDevObj->calData.abvData.passCnt > 2) ? 82 : -41;

    abvTarget = (*targetVoltY * 5) + 5;   /* Gets it to V scale */
    abvTarget *= 1000;
    abvTarget *= 1000;
    abvTarget /= 7324;   /* Now we're scaled to the PCM data */

    abvError = abvTarget -
        (pDevObj->calData.abvData.swyVolt[0]
       - pDevObj->calData.abvData.swyOffset[0]
       - offsetCorrection);

    VP_DOUT(DBG_CALIBRATION, ("Chan 0 Voltage Error: SWY %d, Correction %d",
        (((int16)abvError * 7324) / 10000), offsetCorrection));

    if (((ABS(abvError) * 7324) / 10000) < 5000) {
        /* Write the correction value to CH1 register. Steps in 1.25V increment */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_BAT_CALIBRATION_RD,
            VP880_BAT_CALIBRATION_LEN, swCal);
        swCal[0] &= ~(VP880_BAT_CAL_SWCAL_MASK);

        /* Conversion from 7.324mV to 1.25V */
        swCalError = (ABS(abvError) / 171);
        if (((ABS(abvError) + 85) /  171) > swCalError) {
            swCalError+=1;
        }
        swCalError = (swCalError > 3) ? 3 : swCalError;
        swCal[0] |= (swCalError << 3);

        /*
         * Positive error means voltage is too low (not negative enough). Positive
         * adjustment makes the battery voltage more negative.
         */
        swCal[0] |= (abvError > 0) ? 0 : VP880_BAT_CAL_SWCAL_SIGN;

        VP_DOUT(DBG_CALIBRATION, ("Ch 0: Battery Calibration Correction 0x%02X 0x%02X",
            swCal[0], swCal[1]));

        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_BAT_CALIBRATION_WRT,
            VP880_BAT_CALIBRATION_LEN, swCal);
    }

    /* Now have all data necessary to compute error and adjust channel 1 */
    abvTarget = (*targetVoltZ * 5) + 5;   /* Gets it to V scale */
    abvTarget *= 1000;
    abvTarget *= 1000;
    abvTarget /= 7324;   /* Now we're scaled to the PCM data */

    abvError = abvTarget -
        (pDevObj->calData.abvData.swzVolt[0]
       - pDevObj->calData.abvData.swzOffset[0]
       - offsetCorrection);

    VP_DOUT(DBG_CALIBRATION, ("Chan 1 Voltage Error: SWZ %d",
        (((int16)abvError * 7324) / 10000)));

    VP_DOUT(DBG_CALIBRATION, ("Chan 1 Offset %d Voltage %d Error Raw %d Error Converted %d",
        pDevObj->calData.abvData.swzOffset[1],
        pDevObj->calData.abvData.swzVolt[1],
        (int16)abvError,
        (((int16)abvError * 7324) / 10000)));

    if (((ABS(abvError) * 7324) / 10000) < 5000) {

        /* Write the correction value to CH2 register. Steps in 1.25V increment */
        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_BAT_CALIBRATION_RD,
            VP880_BAT_CALIBRATION_LEN, swCal);
        swCal[0] &= ~(VP880_BAT_CAL_SWCAL_MASK);

        /* Conversion from 7.324mV to 1.25V */
        swCalError = (ABS(abvError) / 171);
        if (((ABS(abvError) + 85) /  171) > swCalError) {
            swCalError+=1;
        }
        swCalError = (swCalError > 3) ? 3 : swCalError;
        swCal[0] |= (swCalError << 3);

        /*
         * Positive error means voltage is too low (not negative enough).
         * Positive adjustment makes the battery voltage more negative.
         */
        swCal[0] |= (abvError > 0) ? 0 : VP880_BAT_CAL_SWCAL_SIGN;

        VP_DOUT(DBG_CALIBRATION, ("Ch 1: Battery Calibration Correction 0x%02X 0x%02X",
            swCal[0], swCal[1]));

        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_BAT_CALIBRATION_WRT,
            VP880_BAT_CALIBRATION_LEN, swCal);
    }
} /* end Vp880AbvMakeAdjustment */

/**
 * Vp880CalAbvAbsDev()
 *  This function initiates a calibration operation for Absolute Switcher
 * circuits
 * associated with all the lines of a device. See VP-API reference guide for
 * more information. SWYV SWZV are global for every Channels
 * Line must be in Disconnect state before to start the Calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalAbvAbsDev(
     VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType  status = VP_STATUS_SUCCESS;
    uint8 ecVal;
    uint8 fxsChannels;
    bool calCleanup = FALSE;

    ecVal = VP880_EC_CH1 | VP880_EC_CH2;
    fxsChannels = 2;

    if (pDevObj->calData.calState == VP880_CAL_INIT
        || pDevObj->calData.calState == VP880_CAL_EXIT) {
        VP_DOUT(DBG_CALIBRATION,("Vp880CalAbvAbsDev:  - Setting to Vp880CalInit"));

        pDevObj->calData.calState = VP880_CAL_INIT;
    }

    switch(pDevObj->calData.calState) {
        case VP880_CAL_INIT:
            VP_DOUT(DBG_CALIBRATION,("Vp880CalAbv: - Running Vp880AbvInitAbs"));
            Vp880AbvInitAbs(pDevCtx);
            break;

        case VP880_CAL_ADC:
            VP_DOUT(DBG_CALIBRATION,("Vp880CalAbv: - Running Vp880AbvSetAdcAbs"));
            Vp880AbvSetAdcAbs(pDevCtx);
            break;

        case VP880_CAL_STATE_CHANGE:
            VP_DOUT(DBG_CALIBRATION,("Vp880CalAbv: - Running Vp880AbvStateChangeAbs"));
            Vp880AbvStateChangeAbs(pDevCtx);
            break;

        case VP880_CAL_MEASURE:
            VP_DOUT(DBG_CALIBRATION,("Vp880CalAbv - Running Vp880AbvMeasureAbs"));
            Vp880AbvMeasureAbs(pDevCtx);
            break;

        case VP880_CAL_DONE:
            VP_DOUT(DBG_CALIBRATION, ("ABV Cal Done for ABS"));
            calCleanup = TRUE;
            pDevObj->responseData = VP_CAL_SUCCESS;
            break;

        case VP880_CAL_ERROR:
            /* Fall through intentional */
        default:
            VP_DOUT(DBG_CALIBRATION,("Vp880CalAbvAbsDev: ERROR - Cal Done"));
            calCleanup = TRUE;
            status = VP_STATUS_FAILURE;
            pDevObj->responseData = VP_CAL_FAILURE;
            break;
    }

    if (calCleanup == TRUE) {
        pDevObj->calData.calState = VP880_CAL_EXIT;
        if (pDevObj->status.state & VP_DEV_INIT_IN_PROGRESS) {
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
            pDevObj->status.state |= VP_DEV_INIT_CMP;
        } else {
            pDevObj->deviceEvents.response |= VP_EVID_CAL_CMP;
        }
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_IN_CAL);
        pDevObj->status.state &= ~VP_DEV_ABV_CAL_ABS;

        /* Reset Line states */
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[0][0]);
        VpMpiCmd(deviceId, VP880_EC_CH2, VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
            &pDevObj->calData.abvData.sysState[1][0]);
    }

    return status;
}

/**
 * Vp880CalInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880CalInit(
     VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 channelId = 0;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data;

    /* Set for Linear Mode and disable AC Coefficients */
    data = VP880_LINEAR_CODEC;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_WRT, VP880_CODEC_REG_LEN,
        &data);

    /* Cut TX/RX PCM and disable HPF */
    data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    /* Device Mode */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN,
        &data);
    data &= ~(VP880_DEV_MODE_TEST_DATA);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
        &data);
}

/**
 * Vp880CalLineInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880CalLineInit(
     VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 channelId = pLineObj->channelId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data;
    uint8 disnVal[VP880_DISN_LEN] = {0x00};

    /*
     * Clear flag to indicate Calibration is NOT done. Init calibration type
     * (i.e., what algorithm to start with) and state (step inside the starting
     * algorithm.
     */
    pLineObj->calLineData.calDone = FALSE;
    pLineObj->calLineData.calState = VP880_CAL_INIT;

     /* Save PCM For Linear Mode and disable AC Coefficients */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_RD, VP880_OP_FUNC_LEN,
        &pLineObj->calLineData.codecReg);

    /* Set compression to Linear Mode */
    data = VP880_LINEAR_CODEC;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_WRT, VP880_CODEC_REG_LEN,
        &data);

    /* Cut TX/RX PCM and disable HPF */
    data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH | VP880_HIGH_PASS_DIS);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_DISN_RD, VP880_DISN_LEN,
        pLineObj->calLineData.disnVal);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DISN_WRT, VP880_DISN_LEN, disnVal);

    /* Device Mode */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN,
        &data);
    data &= ~(VP880_DEV_MODE_TEST_DATA);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
        &data);
}

/**
 * Vp880AdcSettling()
 *  This function read ADC/PCM and set the converter register
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function return the value pcm red.
 */
int16
Vp880AdcSettling(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 adcConfig)
{
    uint8 xdataTemp[VP880_TX_PCM_DATA_LEN];
    int16 tempNew;
    uint8 devMode[VP880_DEV_MODE_LEN];
    uint8 sampleCnt;

    VpMpiCmd(deviceId, ecVal, VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN, devMode);

    /*
     * If the device mode was changed (by other channel), need to change it back
     * so data can be taken. But then need to wait for 1 8KHz sample to update
     * PCM buffer which is taken care of in "for" loop when data is collected.
     */
    if (devMode[0] & VP880_DEV_MODE_TEST_DATA) {
        devMode[0] &= ~(VP880_DEV_MODE_TEST_DATA);
        VpMpiCmd(deviceId, ecVal, VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
            devMode);
    }

    /* Set converter configuration register to measured data point */
    VpMpiCmd(deviceId, ecVal, VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        &adcConfig);

    /* First sample is from previous setting, second will be new data */
    for (sampleCnt = 0; sampleCnt < 2; sampleCnt++) {
        VpMpiCmd(deviceId, ecVal, VP880_TX_PCM_DATA_RD, VP880_TX_PCM_DATA_LEN,
            xdataTemp);
        tempNew = ( (xdataTemp[0] << 8) | xdataTemp[1]);

        VP_DOUT(DBG_CALIBRATION,("Vp880AdcSettling:LOOP: AdcPcm %d ecVal %d iteration %d",
            tempNew, ecVal, sampleCnt));
    }
    return tempNew;
}

/**
 * Vp880CalLineInt()
 *  This function initiates a calibration operation for analog circuits
 * associated with a given line. See VP-API reference guide for more
 * information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
VpStatusType
Vp880CalLineInt(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    switch(pLineObj->lineState.calType) {
        case VP_CSLAC_CAL_VOC:
            VP_DOUT(DBG_CALIBRATION,("\nRunning VOC Calibration on line %d",
                pLineObj->channelId));
            Vp880CalVoc(pLineCtx);
            break;

        case VP_CSLAC_CAL_VAS:
            VP_DOUT(DBG_CALIBRATION,("\nRunning VAS Calibration on line %d",
                pLineObj->channelId));
            Vp880CalVas(pLineCtx);
            break;

        default:
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;
            pLineObj->calLineData.calDone = TRUE;
            break;
    }

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880VocMeasure()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
void
Vp880VocMeasure(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    int16 imtValue;
    uint8 sysState[VP880_SYS_STATE_LEN];
    uint8 dcCal[VP880_DC_CAL_REG_LEN];

    if (pLineObj->calLineData.calState == VP880_CAL_IMT_OFFSET_SET) {
        uint8 converterCfg[VP880_CONV_CFG_LEN] = {VP880_METALLIC_DC_I};

        /* Force the converter change. Don't care about data yet. */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
            VP880_CONV_CFG_LEN, converterCfg);

        pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_READ;
        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        return;
    } else if (pLineObj->calLineData.calState == VP880_CAL_IMT_OFFSET_READ) {
        if (pLineObj->calLineData.reversePol == FALSE) {

            /* Read the metallic current. */
            imtValue = Vp880AdcSettling(deviceId, ecVal[channelId],
                VP880_METALLIC_DC_I);

            VP_DOUT(DBG_CALIBRATION, ("ILA OffsetNorm %d", imtValue));
            pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm = imtValue;
        } else {
            /*
             * The error in reverse polarity is less than what can actually be
             * measured. So it's better to ignore what is being read and assume
             * the part is "perfect".
             */
        }
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
        VP880_SYS_STATE_LEN, sysState);

    /* We're changing the polarity. So set flag as it will be. */
    if (sysState[0] & VP880_SS_POLARITY_MASK) {
        pLineObj->calLineData.reversePol = FALSE;
        sysState[0] &= ~VP880_SS_POLARITY_MASK;
    } else {
        pLineObj->calLineData.reversePol = TRUE;
        sysState[0] |= VP880_SS_POLARITY_MASK;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
        VP880_SYS_STATE_LEN, sysState);

    /* Re-Disable TRDC Sense */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
        VP880_DC_CAL_REG_LEN, dcCal);

    if (pDevObj->stateInt & VP880_IS_ABS) {
        dcCal[0] &= VP880_DC_CAL_ABS_MASK;
        dcCal[1] &= VP880_DCCAL_BAT_SW_HYST_MASK;
        dcCal[1] |= (VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT);
    } else {
        dcCal[1] |= (VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT | VP880_DCCAL_BAT_SW_HYST_5V);
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
        VP880_DC_CAL_REG_LEN, dcCal);

    /* Advance to Next State */
    pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_SET_INVERT;

    /*
     * Need to wait for line to settle before changing converter. Time is
     * based on polarity change which is "known" in API-II.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    return;
} /* end Vp880VocMeasure */

/**
 * Vp880VocMeasureInvert()
 *  This function read switcher value and compare with
 *  the value read from the pcm data if the value is bigger than 1.25v
 * this function will make a correction  voltage .
 *
 */
void
Vp880VocMeasureInvert(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    int16 imtValue, vocValue;
    uint8 dcCal[VP880_DC_CAL_REG_LEN];
    uint8 converterCfg[VP880_CONV_CFG_LEN];

    switch (pLineObj->calLineData.calState) {
        case VP880_CAL_IMT_OFFSET_SET_INVERT:
            converterCfg[0] = VP880_METALLIC_DC_I;

            /* Force the converter change. Don't care about data yet. */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_READ_INVERT;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP880_CAL_IMT_OFFSET_READ_INVERT:
            imtValue = Vp880AdcSettling(deviceId, ecVal[channelId],
                VP880_METALLIC_DC_I);

            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_DOUT(DBG_CALIBRATION, ("ILA Norm %d", imtValue));
                pLineObj->calLineData.typeData.ilaData.ilaNorm = imtValue;
            } else {
                /*
                 * Don't care about ila reverse polarity. Device is better than
                 * it can measure.
                 */
            }

            converterCfg[0] = VP880_METALLIC_DC_V;

            /* Force the converter change. Don't care about data yet. */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            pLineObj->calLineData.calState = VP880_CAL_VOC_READ_INVERT;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP880_CAL_VOC_READ_INVERT:
            vocValue = Vp880AdcSettling(deviceId, ecVal[channelId],
                VP880_METALLIC_DC_V);

            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_DOUT(DBG_CALIBRATION, ("VOC OffsetNorm %d", vocValue));
                pLineObj->calLineData.typeData.vocData.vocOffsetNorm = vocValue;
            } else {
                VP_DOUT(DBG_CALIBRATION, ("VOC OffsetRev %d", vocValue));
                pLineObj->calLineData.typeData.vocData.vocOffsetRev = vocValue;
            }
            break;

        default:
            break;
    };

    /* Re-Enable TRDC Sense */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
        VP880_DC_CAL_REG_LEN, dcCal);
    dcCal[1] &= ~(VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
        VP880_DC_CAL_REG_LEN, dcCal);

    /* Return to VOC feed, speedup enabled */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
        pLineObj->calLineData.typeData.ilaData.icr2Data);

    /* Advance to Final States */
    pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_SET_DONE;

    /* Need to wait for line to settle before changing converter. */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

    return;
} /* end Vp880VocMeasureInvert */

#if 1
/**
 * Vp880CalVas()
 *  This function determines the optimal VAS value for both normal and reverse
 * polarity on the line specified by the line context. It should only be called
 * by API-II internal functions.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  VAS values for normal and reverse polarity are computed and stored in the
 * line object. The line state is returned to state it was in prior to starting
 * calibration.
 */
VpStatusType
Vp880CalVas(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 convCfg[VP880_CONV_CFG_LEN];
    uint8 dcFeed[VP880_DC_FEED_LEN];
    uint8 imtAvgLoopCnt;

    int16 imtNew[VP880_IMT_AVERAGE_CNT];
    int32 imtSumAvg;

    uint16 imtErr;
    uint16 vasValue;
    uint8 icr2Mods[VP880_ICR2_LEN] = {0x00, 0x00, 0xCC, 0xCC};
    uint8 deviceMode[VP880_DEV_MODE_LEN];
    uint8 sysState[VP880_SYS_STATE_LEN];

    switch(pLineObj->calLineData.calState) {
        case VP880_CAL_INIT:
            VP_DOUT(DBG_CALIBRATION,("VAS: Cal Init"));
            pLineObj->calLineData.typeData.vasData.secondPass = FALSE;

            /* Read system State and keep Pol sign */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
                VP880_SYS_STATE_LEN, sysState);

            /*
             * Save current dc calibration data. This value is adjusted by cal
             * algorithms to finalize a set that is programmed in the device
             * by Set Line State. It will contain adjusted VOC values for each
             * polarity.
             */
            if (sysState[0] & VP880_SS_POLARITY_MASK) {
                pLineObj->calLineData.reversePol = TRUE;
                dcFeed[0] = pLineObj->calLineData.dcFeedPr[0];
                dcFeed[1] = pLineObj->calLineData.dcFeedPr[1];
            } else {
                pLineObj->calLineData.reversePol = FALSE;
                dcFeed[0] = pLineObj->calLineData.dcFeed[0];
                dcFeed[1] = pLineObj->calLineData.dcFeed[1];
            }

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
                pLineObj->calLineData.typeData.vasData.icr2Data);

            /* Set Battery and Feed for High-Speed switching */
            icr2Mods[0] = pLineObj->calLineData.typeData.vasData.icr2Data[0];
            icr2Mods[1] = pLineObj->calLineData.typeData.vasData.icr2Data[1];
            icr2Mods[2] |= pLineObj->calLineData.typeData.vasData.icr2Data[2];
            icr2Mods[3] |= pLineObj->calLineData.typeData.vasData.icr2Data[3];

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
                icr2Mods);

            /* Set VAS to minimum */
            VpCSLACSetVas(dcFeed, 0);
            VP_DOUT(DBG_CALIBRATION, ("1. VAS Writing 0x%04X to DC Feed",
                ((dcFeed[0] << 8) & 0xFF00) | dcFeed[1]));
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                VP880_DC_FEED_LEN, dcFeed);

            /* Make sure samples are collected from the PCM Test Buffer */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD,
                VP880_DEV_MODE_LEN, deviceMode);
            deviceMode[0] &= ~VP880_DEV_MODE_TEST_DATA;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT,
                VP880_DEV_MODE_LEN, deviceMode);

            /*
             * Start timers to wait for supply to settle before taking the
             * first measurement
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
               MS_TO_TICKRATE(VP880_VAS_INIT_WAIT,
                   pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;

            /* Increment the state machine */
            pLineObj->calLineData.calState = VP880_CONVERTER_CHECK;
            break;

        case VP880_CONVERTER_CHECK:
            VP_DOUT(DBG_CALIBRATION, ("\nVAS Cal: Converter Check"));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_RD,
                VP880_CONV_CFG_LEN, convCfg);

            if ((convCfg[0] & VP880_CONV_CONNECT_BITS) != VP880_METALLIC_DC_I) {
                VP_DOUT(DBG_CALIBRATION, ("Converter Check Fail"));

                /*
                 * The device internally changed the converter configuration.
                 * So we're in a bad condition, no need to measure. Increase
                 * VAS and try again.
                 */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_RD,
                    VP880_DC_FEED_LEN, dcFeed);

                vasValue = VP880_VAS_CONVERSION(dcFeed[0], dcFeed[1]);
                VpCSLACSetVas(dcFeed, vasValue + 750);

                if (vasValue >= (VP880_VAS_MAX - VP880_VAS_OVERHEAD)) {
                    /*
                     * If we're at the maximum VAS value, then that's the
                     * best we can and no sense continuing.
                     */
                    VpCSLACSetVas(dcFeed, VP880_VAS_MAX);
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, dcFeed);

                    pLineObj->calLineData.calState = VP880_CAL_DONE;
                    VP_DOUT(DBG_CALIBRATION, ("Giving Up on Converter Check"));
                } else {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, dcFeed);

                    convCfg[0] = VP880_METALLIC_DC_I;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                        VP880_CONV_CFG_LEN, convCfg);

                    VP_DOUT(DBG_CALIBRATION, ("2. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                        vasValue, dcFeed[0], dcFeed[1]));
                }
            } else {
                pLineObj->calLineData.calState = VP880_CAL_OFFSET;
            }

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
               MS_TO_TICKRATE(VP880_VAS_MEAS_DELAY,
                pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;
            break;

        case VP880_CAL_OFFSET:
            VP_DOUT(DBG_CALIBRATION,("\nVAS Cal: Cal Offset"));

            imtSumAvg = 0;
            for (imtAvgLoopCnt = 0;
                 imtAvgLoopCnt < VP880_IMT_AVERAGE_CNT;
                 imtAvgLoopCnt++) {

                imtNew[imtAvgLoopCnt] = Vp880AdcSettling(deviceId,
                    ecVal[channelId], VP880_METALLIC_DC_I);

                imtSumAvg += imtNew[imtAvgLoopCnt];
            }
            imtSumAvg /= VP880_IMT_AVERAGE_CNT;
            pLineObj->calLineData.typeData.vasData.imtPrev = (int16)imtSumAvg;

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_RD,
                VP880_DC_FEED_LEN, dcFeed);

            vasValue = VP880_VAS_CONVERSION(dcFeed[0], dcFeed[1]);
            VpCSLACSetVas(dcFeed, vasValue + 750);

            VP_DOUT(DBG_CALIBRATION, ("3. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                vasValue, dcFeed[0], dcFeed[1]));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                VP880_DC_FEED_LEN, dcFeed);

            if (vasValue >= (VP880_VAS_MAX - VP880_VAS_OVERHEAD)) {
                /*
                 * If we're at the maximum VAS value, then that's the
                 * best we can and no sense continuing.
                 */
                VP_DOUT(DBG_CALIBRATION, ("1. Max Overhead Reached"));

                VpCSLACSetVas(dcFeed, VP880_VAS_MAX);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                    VP880_DC_FEED_LEN, dcFeed);

                Vp880VasPolRev(pLineCtx);
            } else {
                pLineObj->calLineData.calState = VP880_CAL_MEASURE;
            }
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_VAS_MEAS_DELAY,
                pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                VP_ACTIVATE_TIMER;
            break;

        case VP880_CAL_MEASURE:
            VP_DOUT(DBG_CALIBRATION,("VAS: Cal Measure"));

            imtSumAvg = 0;
            for (imtAvgLoopCnt = 0;
                 imtAvgLoopCnt < VP880_IMT_AVERAGE_CNT;
                 imtAvgLoopCnt++) {

                imtNew[imtAvgLoopCnt] = Vp880AdcSettling(deviceId,
                    ecVal[channelId], VP880_METALLIC_DC_I);

                imtSumAvg += imtNew[imtAvgLoopCnt];
            }
            imtSumAvg /= VP880_IMT_AVERAGE_CNT;

            imtNew[0] = (int16)imtSumAvg;
            imtErr = (imtNew[0] >= pLineObj->calLineData.typeData.vasData.imtPrev)
                ? (imtNew[0] - pLineObj->calLineData.typeData.vasData.imtPrev)
                : (pLineObj->calLineData.typeData.vasData.imtPrev - imtNew[0]);

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_RD,
                VP880_DC_FEED_LEN, dcFeed);
            vasValue = VP880_VAS_CONVERSION(dcFeed[0], dcFeed[1]);

            if (imtErr > VP880_VAS_MEAS_ERR) {
                pLineObj->calLineData.typeData.vasData.imtPrev = imtNew[0];

                VpCSLACSetVas(dcFeed, vasValue + 750);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                    VP880_DC_FEED_LEN, dcFeed);

                VP_DOUT(DBG_CALIBRATION, ("3. VAS (%d) - VOC Reg 0x%02X 0x%02X",
                    vasValue, dcFeed[0], dcFeed[1]));

                pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                    MS_TO_TICKRATE(VP880_VAS_MEAS_DELAY,
                        pDevObj->devProfileData.tickRate);

                pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                    VP_ACTIVATE_TIMER;

                if (vasValue >= (VP880_VAS_MAX - VP880_VAS_OVERHEAD)) {
                    /*
                     * If we're at the maximum VAS value, then that's the
                     * best we can and no sense continuing.
                     */
                    VP_DOUT(DBG_CALIBRATION, ("2. Max Overhead Reached"));

                    VpCSLACSetVas(dcFeed, VP880_VAS_MAX);
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, dcFeed);

                    if (pLineObj->calLineData.typeData.vasData.secondPass == FALSE) {
                        pLineObj->calLineData.typeData.vasData.secondPass = TRUE;
                        Vp880VasPolRev(pLineCtx);
                        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                            MS_TO_TICKRATE(VP880_VAS_MEAS_DELAY,
                                pDevObj->devProfileData.tickRate);

                        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                            VP_ACTIVATE_TIMER;

                        /* Set VAS to minimum */
                        VpCSLACSetVas(dcFeed, 0);
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                            VP880_DC_FEED_LEN, dcFeed);
                    } else {
                        pLineObj->calLineData.calState = VP880_CAL_DONE;
                    }
                }
            } else {
                VpCSLACSetVas(dcFeed, vasValue + VP880_VAS_OVERHEAD);

                if (pLineObj->calLineData.reversePol == FALSE) {
                    pLineObj->calLineData.reversePol = TRUE;
                    pLineObj->calLineData.dcFeed[0] = dcFeed[0];
                    pLineObj->calLineData.dcFeed[1] = dcFeed[1];

                    VP_DOUT(DBG_CALIBRATION, ("VAS Normal Polarity Cal OK -- Saving 0x%02X 0x%02X",
                        pLineObj->calLineData.dcFeed[0],
                        pLineObj->calLineData.dcFeed[1]));

                    dcFeed[0] = pLineObj->calLineData.dcFeedPr[0];
                    dcFeed[1] = pLineObj->calLineData.dcFeedPr[1];
                } else {
                    pLineObj->calLineData.reversePol = FALSE;
                    pLineObj->calLineData.dcFeedPr[0] = dcFeed[0];
                    pLineObj->calLineData.dcFeedPr[1] = dcFeed[1];

                    VP_DOUT(DBG_CALIBRATION, ("VAS Reverse Polarity Cal OK -- Saving 0x%02X 0x%02X",
                        pLineObj->calLineData.dcFeedPr[0],
                        pLineObj->calLineData.dcFeedPr[1]));

                    dcFeed[0] = pLineObj->calLineData.dcFeed[0];
                    dcFeed[1] = pLineObj->calLineData.dcFeed[1];
                }

                if (pLineObj->calLineData.typeData.vasData.secondPass == FALSE) {
                    pLineObj->calLineData.typeData.vasData.secondPass = TRUE;
                    Vp880VasPolRev(pLineCtx);
                    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                        MS_TO_TICKRATE(VP880_VAS_MEAS_DELAY,
                            pDevObj->devProfileData.tickRate);

                    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
                        VP_ACTIVATE_TIMER;

                    /* Set VAS to minimum */
                    VpCSLACSetVas(dcFeed, 0);
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, dcFeed);
                } else {
                    pLineObj->calLineData.calState = VP880_CAL_DONE;
                }
            }
            break;

        case VP880_CAL_INVERT_POL:
            VP_DOUT(DBG_CALIBRATION,("VAS: Reversing Polarity"));
            Vp880SetLineStateInt(pLineCtx,
                VpGetReverseState(pLineObj->lineState.currentState));

            /* Set VAS to minimum */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_RD,
                VP880_DC_FEED_LEN, pLineObj->calLineData.dcFeed);

            dcFeed[0] = pLineObj->calLineData.dcFeed[0];
            dcFeed[1] = pLineObj->calLineData.dcFeed[1];

            VpCSLACSetVas(dcFeed, 0);
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                VP880_DC_FEED_LEN, dcFeed);
            VP_DOUT(DBG_CALIBRATION, ("1. VAS - VOC Reg 0x%02X 0x%02X",
                dcFeed[0], dcFeed[1]));

            /*
             * Start timers to wait for supply to settle before taking the
             * measurement
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
               VP_ACTIVATE_TIMER;

            /* Increment the state machine */
            pLineObj->calLineData.calState = VP880_CONVERTER_CHECK;
            break;

        default:
            VP_DOUT(DBG_CALIBRATION,("VAS Cal Done"));
            pLineObj->calLineData.calState = VP880_CAL_DONE;
            break;
    }

    if (pLineObj->calLineData.calState == VP880_CAL_DONE) {
        VP_DOUT(DBG_CALIBRATION,("VAS: Cal Completion"));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
            pLineObj->calLineData.typeData.vasData.icr2Data);

        /*
         * Both dcFeed and dcFeedPr contain the same maximum VAS setting that
         * is required for both normal and polarity reversal conditions to
         * prevent saturation condition. So using either value in the line
         * object is ok.
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
            VP880_DC_FEED_LEN, pLineObj->calLineData.dcFeed);

        VP_DOUT(DBG_CALIBRATION, ("5. VAS - VOC Normal 0x%02X 0x%02X Reverse 0x%02X 0x%02X",
            pLineObj->calLineData.dcFeed[0],
            pLineObj->calLineData.dcFeed[1],
            pLineObj->calLineData.dcFeedPr[0],
            pLineObj->calLineData.dcFeedPr[1]));

        pLineObj->responseData = VP_CAL_SUCCESS;
        Vp880CalDone(pLineCtx);
    }

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880VasPolRev()
 *  This function inverts polarity of line and sets reversePol flag in line
 * object (calibration) appropriately.
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  Line polarity is reversed. Line Object updated.
 */
void
Vp880VasPolRev(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 sysState[VP880_SYS_STATE_LEN];

    /* Read system State */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
        VP880_SYS_STATE_LEN, sysState);

    if (sysState[0] & VP880_SS_POLARITY_MASK) {
        sysState[0] &= ~VP880_SS_POLARITY_MASK;
        pLineObj->calLineData.reversePol = FALSE;
    } else {
        sysState[0] |= VP880_SS_POLARITY_MASK;
        pLineObj->calLineData.reversePol = TRUE;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
        VP880_SYS_STATE_LEN, sysState);
}

#endif

/**
 * Vp880CalVoc()
 *  This function sets the VOC values in dcFeed as specified by the device
 * dc feed register, with VOS value passed. It does not actually access the
 * device, just simply computes the correct hex values for the dc feed reg.
 *
 * Preconditions:
 *  None. Helper function only.
 *
 * Postconditions:
 *  Line not affected. Values in dcFeed contain the VOC values passed.
 */

VpStatusType
Vp880CalVoc(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VP_DOUT(DBG_CALIBRATION, ("Chan %d VOC Cal State %d",
        pLineObj->channelId, pLineObj->calLineData.calState));

    switch(pLineObj->calLineData.calState) {
        case VP880_CAL_INIT:
            Vp880VocInit(pLineCtx);
            break;

        case VP880_CAL_ADC:
            Vp880VocSetAdc(pLineCtx);
            break;

        case VP880_CAL_OFFSET:
        case VP880_CAL_FIRST_IMT_SET:
        case VP880_CAL_FIRST_IMT_READ:
        case VP880_CAL_FIRST_VOC_READ:
            Vp880VocOffset(pLineCtx);
            break;

        case VP880_CAL_MEASURE:
        case VP880_CAL_IMT_OFFSET_SET:
        case VP880_CAL_IMT_OFFSET_READ:
            Vp880VocMeasure(pLineCtx);
            break;

        case VP880_CAL_INVERT_POL:
        case VP880_CAL_IMT_OFFSET_SET_INVERT:
        case VP880_CAL_IMT_OFFSET_READ_INVERT:
        case VP880_CAL_VOC_READ_INVERT:
            Vp880VocMeasureInvert(pLineCtx);
            break;

        case VP880_CAL_DONE:
        case VP880_CAL_IMT_OFFSET_SET_DONE:
        case VP880_CAL_IMT_OFFSET_READ_DONE:
        case VP880_CAL_VOC_READ_DONE:
            Vp880VocDone(pLineCtx);
            break;

        case VP880_CAL_ERROR:
            VP_DOUT(DBG_CALIBRATION,("\nVOC Cal Error"));
            pLineObj->responseData = VP_CAL_FAILURE;
            Vp880VocDone(pLineCtx);
            Vp880CalDone(pLineCtx);

            pLineObj->calLineData.calState = VP880_CAL_EXIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE((VP880_CAL_SET),
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            break;

        default:
            pLineObj->calLineData.calState = VP880_CAL_INIT;
            pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
            break;
    }

    return VP_STATUS_SUCCESS;

} /*end Vp880CalVoc */

/**
 * Vp880CalDone()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880CalDone(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 data;
    uint8 convCfg[VP880_CONV_CFG_LEN];

    VP_DOUT(DBG_CALIBRATION,("\nCalibration Done"));

    /* Set use of programmed coefficients and Codec Mode */
    data = VP880_ENABLE_LOADED_COEFFICIENTS;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_WRT, VP880_OP_FUNC_LEN,
        &data);
    Vp880SetCodec(pLineCtx, pLineObj->codec);

    /* Cut TX/RX PCM and enable HPF */
    data = (VP880_CUT_TXPATH | VP880_CUT_RXPATH);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    /* Restore DISN */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DISN_WRT, VP880_DISN_LEN,
        pLineObj->calLineData.disnVal);

    /* Restore Device Mode */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN,
        &data);
    data |= VP880_DEV_MODE_TEST_DATA;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT, VP880_DEV_MODE_LEN,
        &data);

    /* Connect A/D to AC T/R Input, restore device sample rate */
    convCfg[0] = (VP880_METALLIC_AC_V | pDevObj->txBufferDataRate);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        convCfg);

    /* Restore Line State */
    Vp880SetLineStateInt(pLineCtx, pLineObj->lineState.usrCurrent);

    if (pLineObj->responseData == VP_CAL_SUCCESS) {
        pLineObj->calLineData.calDone = TRUE;
    }

    pLineObj->lineState.calType = VP_CSLAC_CAL_NONE;
    pLineObj->lineEvents.response |= VP_EVID_CAL_CMP;

    Vp880LLSetSysState(deviceId, pLineCtx, 0, FALSE);

    /* Force an update on the line */
    Vp880LowPowerMode(pDevCtx);
}

/**
 * Vp880VocInit()
 *  This function initiates a calibration operation for VOC
 * associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880VocInit(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 converterCfg[VP880_CONV_CFG_LEN]  = {VP880_METALLIC_DC_V};
    uint8 icr2[VP880_ICR2_LEN] = {0x00, 0x00, 0xC0, 0xC0};
    uint8 xdataTemp[VP880_TX_PCM_DATA_LEN];
    int16 tempNew;

    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
        pLineObj->calLineData.typeData.ilaData.icr2Data);

    VP_DOUT(DBG_CALIBRATION, ("Vp880VocInit: Register 0x%02X 0x%02X",
        pLineObj->calLineData.dcFeedRef[0],
        pLineObj->calLineData.dcFeedRef[1]));

    pLineObj->calLineData.dcFeed[0] = pLineObj->calLineData.dcFeedRef[0];
    pLineObj->calLineData.dcFeed[1] = pLineObj->calLineData.dcFeedRef[1];
    pLineObj->calLineData.dcFeedPr[0] = pLineObj->calLineData.dcFeedRef[0];
    pLineObj->calLineData.dcFeedPr[1] = pLineObj->calLineData.dcFeedRef[1];

    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT, VP880_DC_FEED_LEN,
        pLineObj->calLineData.dcFeedRef);

    if (pDevObj->stateInt & VP880_IS_ABS) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
            icr2);
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_TX_PCM_DATA_RD,
        VP880_TX_PCM_DATA_LEN, xdataTemp);
    tempNew = ( (xdataTemp[0] << 8) | xdataTemp[1]);
    VP_DOUT(DBG_CALIBRATION,("2. Vp880AdcSettling:LOOP: AdcPcm %d ecVal %d",
        tempNew, ecVal[channelId]));

    /* Sense VAB Voltage. Can't use the value yet, so need to wait. */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT, VP880_CONV_CFG_LEN,
        converterCfg);

    /* Advanced to state for ADC calibration */
    pLineObj->calLineData.calState = VP880_CAL_ADC;

    /* Start timer to cause internal line calibration function to execute */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
} /* end Vp880VocInit */

/**
 * Vp880VocSetAdc ()
 *  This function set the converter to read the right pcm
 *  and set the right state machine , takes care for Pol Rev
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880VocSetAdc(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    int16 vocVolt;
    uint8 dcCal[VP880_DC_CAL_REG_LEN];

    /* Read the Normal VOC Value raw from PCM */
    vocVolt = Vp880AdcSettling(deviceId, ecVal[channelId], VP880_METALLIC_DC_V);

    if (pLineObj->calLineData.reversePol == FALSE) {
        VP_DOUT(DBG_CALIBRATION, ("1. VOC Norm: %d", vocVolt));
        pLineObj->calLineData.typeData.vocData.vocNorm = vocVolt;

    } else {
        VP_DOUT(DBG_CALIBRATION, ("1. VOC Rev: %d", vocVolt));
        pLineObj->calLineData.typeData.vocData.vocRev = vocVolt;
    }

    /* Setup channel for Normal ILA and VAB offset read. Allow VAB to collapse */
    /* Disable TRDC sense -- forces ILA when on-hook */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
        VP880_DC_CAL_REG_LEN, dcCal);

    if (pDevObj->stateInt & VP880_IS_ABS) {
        dcCal[0] &= VP880_DC_CAL_ABS_MASK;
        dcCal[1] &= VP880_DCCAL_BAT_SW_HYST_MASK;
        dcCal[1] |= (VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT);
    } else {
        dcCal[1] |= (VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT | VP880_DCCAL_BAT_SW_HYST_5V);
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
        VP880_DC_CAL_REG_LEN, dcCal);

    /* Advance to next state, where we can read Metalic Current */
    pLineObj->calLineData.calState = VP880_CAL_FIRST_IMT_SET;

    /*
     * Wait for line to settle before making converter configuration change.
     * Otherwise, it can change back to T/R AC voltage.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP880_CAL_VOC_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
} /* end Vp880VocSetAdc */

/**
 * Vp880VocOffset ()
 *
 *  This function computes and save the offset value for the ABV calibration
 *
 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *
 */
void
Vp880VocOffset(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    int16 imtValue, vabOffset;
    uint8 icr2Mods[VP880_ICR2_LEN];
    uint8 dcCal[VP880_DC_CAL_REG_LEN];
    uint8 byteCount;

    if (pLineObj->calLineData.calState == VP880_CAL_FIRST_IMT_SET) {
        uint8 converterCfg[] = {VP880_METALLIC_DC_I};

        /* Force the converter change. Don't care about data yet. */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
            VP880_CONV_CFG_LEN, converterCfg);

        /* Update the state */
        pLineObj->calLineData.calState = VP880_CAL_FIRST_IMT_READ;

        /* Wait for converter and data to stabilize */
        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;

        return;

    } else if (pLineObj->calLineData.calState == VP880_CAL_FIRST_IMT_READ) {
        uint8 converterCfg[] = {VP880_METALLIC_DC_V};

        /* First read the metallic current. Should be "high". */
        imtValue = Vp880AdcSettling(deviceId, ecVal[channelId],
            VP880_METALLIC_DC_I);
        if (pLineObj->calLineData.reversePol == FALSE) {
            VP_DOUT(DBG_CALIBRATION, ("ILA Norm: %d", imtValue));
            pLineObj->calLineData.typeData.ilaData.ilaNorm = imtValue;
        } else {
            /*
             * Don't care about ila in reverse polarity. It's not calibrated
             * because device drive capability is better than the device
             * measurement capability.
             */
        }

        /* Force the converter change. Don't care about data yet. */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
            VP880_CONV_CFG_LEN, converterCfg);

        /* Update the state */
        pLineObj->calLineData.calState = VP880_CAL_FIRST_VOC_READ;

        /* Wait for converter to stabilize */
        pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
            MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
        return;
    } else if (pLineObj->calLineData.calState == VP880_CAL_FIRST_VOC_READ) {
        /* Read the VAB Offset Voltage (should be 0 at this point) */
        vabOffset = Vp880AdcSettling(deviceId, ecVal[channelId],
            VP880_METALLIC_DC_V);

        if (pLineObj->calLineData.reversePol == FALSE) {
            VP_DOUT(DBG_CALIBRATION, ("VOC Norm Offset: %d", vabOffset));
            pLineObj->calLineData.typeData.vocData.vocOffsetNorm = vabOffset;
        } else {
            VP_DOUT(DBG_CALIBRATION, ("VOC Rev Offset: %d", vabOffset));
            pLineObj->calLineData.typeData.vocData.vocOffsetRev = vabOffset;
        }
    }

    /*
     * Disable the VOC DAC, re-enable the TRDC sense. Then need to wait before
     * switching converter.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
        pLineObj->calLineData.typeData.ilaData.icr2Data);
    for (byteCount = 0; byteCount < VP880_ICR2_LEN; byteCount++) {
        icr2Mods[byteCount] =
            pLineObj->calLineData.typeData.ilaData.icr2Data[byteCount];
    }
    icr2Mods[VP880_ICR2_VOC_DAC_INDEX] |= VP880_ICR2_VOC_DAC_EN;
    icr2Mods[VP880_ICR2_VOC_DAC_INDEX+1] &= ~VP880_ICR2_VOC_DAC_EN;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
        icr2Mods);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
        VP880_DC_CAL_REG_LEN, dcCal);
    dcCal[1] &= ~(VP880_C_RING_SNS_CUT | VP880_C_TIP_SNS_CUT);
    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
        VP880_DC_CAL_REG_LEN, dcCal);

    /* Advance to next state */
    pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_SET;

    /*
     * Wait for line to stabilize before attempting converter change. Otherwise
     * it will go back to T/R AC Voltage.
     */
    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
        MS_TO_TICKRATE(VP880_CAL_VOC_LONG,
            pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
} /* end Vp880VocOffset */

/**
 * Vp880VocDone()
 *  This function end the calibration operation for VOC
 *  associated with all the lines of a device. See VP-API reference guide for
 * more information.

 * Preconditions:
 *  The device and line context must be created and initialized before calling
 * this function.
 *
 * Postconditions:
 *  This function generates an event upon completing the requested action.
 */
void
Vp880VocDone(
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    int16 imtValue, vocValue, imtActual, vocActual, vocActualRev;
    int16 vocError, vocErrorRev;
    int16 ilaError;
    int32 targetVoc, targetIla;
    uint8 ilaAdjust;
    uint8 converterCfg[VP880_CONV_CFG_LEN];

    switch (pLineObj->calLineData.calState) {
        case VP880_CAL_IMT_OFFSET_SET_DONE:
            converterCfg[0] = VP880_METALLIC_DC_I;

            /* Force the converter change. Don't care about data yet. */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            pLineObj->calLineData.calState = VP880_CAL_IMT_OFFSET_READ_DONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP880_CAL_IMT_OFFSET_READ_DONE:
            if (pLineObj->calLineData.reversePol == FALSE) {
                imtValue = Vp880AdcSettling(deviceId, ecVal[channelId],
                    VP880_METALLIC_DC_I);
                VP_DOUT(DBG_CALIBRATION, ("ILA OffsetNorm %d", imtValue));
                pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm = imtValue;
            } else {
                /*
                 * The error in reverse polarity is less than what can actually
                 * be measured. So it's better to ignore what is being read and
                 * assume the part is "perfect".
                 */
            }

            converterCfg[0] = VP880_METALLIC_DC_V;

            /* Force the converter change. Don't care about data yet. */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            pLineObj->calLineData.calState = VP880_CAL_VOC_READ_DONE;

            pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
                MS_TO_TICKRATE(VP880_CAL_VOC_SHORT,
                    pDevObj->devProfileData.tickRate) | VP_ACTIVATE_TIMER;
            return;

        case VP880_CAL_VOC_READ_DONE:
            vocValue = Vp880AdcSettling(deviceId, ecVal[channelId],
                VP880_METALLIC_DC_V);

            if (pLineObj->calLineData.reversePol == FALSE) {
                VP_DOUT(DBG_CALIBRATION, ("2. VOC Norm: %d", vocValue));
                pLineObj->calLineData.typeData.vocData.vocNorm = vocValue;
            } else {
                VP_DOUT(DBG_CALIBRATION, ("2. VOC Rev: %d", vocValue));
                pLineObj->calLineData.typeData.vocData.vocRev = vocValue;
            }
            break;

        default:
            break;
    };

    /* Bring Target value up to PCM scale */
    targetVoc = ((pLineObj->calLineData.dcFeedRef[0] >> 2) & 0x7);
    targetVoc *= 4096;
    targetVoc += 49153;
    targetVoc /= 10;

    targetIla = (pLineObj->calLineData.dcFeedRef[1] & 0x1F);
    targetIla *= ((VP880_ILA_SCALE_2MA / 2) * 10); /* 5682; */
    targetIla += (VP880_ILA_SCALE_18MA * 10); /* Base of 18ma 102272; */
    targetIla /= 10;

    imtActual =
        pLineObj->calLineData.typeData.ilaData.ilaNorm
      - pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm;

    vocActual =
        pLineObj->calLineData.typeData.vocData.vocNorm
      - pLineObj->calLineData.typeData.vocData.vocOffsetNorm;

    vocActualRev =
        pLineObj->calLineData.typeData.vocData.vocRev
      - pLineObj->calLineData.typeData.vocData.vocOffsetRev;

    VP_DOUT(DBG_CALIBRATION, ("VOC Norm %d Rev %d OffsetNorm %d OffsetRev %d",
        pLineObj->calLineData.typeData.vocData.vocNorm,
        pLineObj->calLineData.typeData.vocData.vocRev,
        pLineObj->calLineData.typeData.vocData.vocOffsetNorm,
        pLineObj->calLineData.typeData.vocData.vocOffsetRev));

    VP_DOUT(DBG_CALIBRATION, ("ILA Norm %d OffsetNorm %d",
        pLineObj->calLineData.typeData.ilaData.ilaNorm,
        pLineObj->calLineData.typeData.ilaData.ilaOffsetNorm));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: ILA Actual Norm %d",
        channelId, imtActual));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: VOC Actual Norm %d Rev Norm %d",
        channelId, vocActual, vocActualRev));

    /*
     * Target is always positive. Normal feed is positive. Negative error means
     * voltage is too low (magnitude), positive means too high (magnitude).
     */
    vocError = (vocActual - targetVoc);
    ilaError = (imtActual - targetIla);

    /*
     * Target is always positive. Reverse feed is negative. Negative error means
     * voltage is too low (magnitude), positive means too high (magnitude).
     */
    vocErrorRev = (-vocActualRev - targetVoc);

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: VOC Target %d VOC Error Norm %d Error Rev %d",
        channelId, (int16)targetVoc, vocError, vocErrorRev));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: ILA Target %d ILA Error Norm %d",
        channelId, (int16)targetIla, ilaError));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: DC Feed Values Normal Before 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeed[0], pLineObj->calLineData.dcFeed[1]));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: DC Feed Values Reverse Before 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeedPr[0], pLineObj->calLineData.dcFeedPr[1]));

    /*
     * Adjust if error is more than 1/2 a step size for each parameter based
     * on PCM scale.
     */
    /* VOC Scale: 500mV = 204.8 at PCM. Adjust to account for bit shift */
    if (ABS(vocError) >= 205) {
        if (vocError < 0) {
            pLineObj->calLineData.dcFeed[0] += 0x04;
        } else {
            pLineObj->calLineData.dcFeed[0] -= 0x04;
        }
    }
    if (ABS(vocErrorRev) >= 205) {
        if (vocErrorRev < 0) {
            pLineObj->calLineData.dcFeedPr[0] += 0x04;
        } else {
            pLineObj->calLineData.dcFeedPr[0] -= 0x04;
        }
    }

    /* ILA Scale: 500uA = 273 at PCM */
    if (ABS(ilaError) >= (VP880_ILA_SCALE_1MA / 2)) {
        uint8 tempIlaValue = pLineObj->calLineData.dcFeed[1] & 0x1F;
        int8 tempLowValue = pLineObj->calLineData.dcFeed[1] & 0x1F;
        ilaAdjust = ((ABS(ilaError)+(VP880_ILA_SCALE_1MA / 2)) / VP880_ILA_SCALE_1MA);
        if (ilaError < 0) {
            tempIlaValue += ilaAdjust;
            if (tempIlaValue <= 0x1F) {
                pLineObj->calLineData.dcFeed[1] += ilaAdjust;
            } else {
                pLineObj->calLineData.dcFeed[1] |= 0x1F;
            }
        } else {
            tempLowValue -= ilaAdjust;
            if (tempLowValue >= 0) {
                pLineObj->calLineData.dcFeed[1] -= ilaAdjust;
            } else {
                pLineObj->calLineData.dcFeed[1] &= ~0x1F;
            }
        }
    }

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: DC Feed Values Normal After 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeed[0], pLineObj->calLineData.dcFeed[1]));

    VP_DOUT(DBG_CALIBRATION, ("Chan %d: DC Feed Values Reverse After 0x%02X 0x%02X",
        channelId, pLineObj->calLineData.dcFeedPr[0], pLineObj->calLineData.dcFeedPr[1]));


    /* Move on to VAS Calibration... */
    pLineObj->lineState.calType = VP_CSLAC_CAL_VAS;

    if (pDevObj->stateInt & VP880_IS_ABS) {
        /* ...at state done if ABS device */
        pLineObj->calLineData.calState = VP880_CAL_DONE;
    } else {
        pLineObj->calLineData.calState = VP880_CAL_INIT;
    }

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] =
       MS_TO_TICKRATE(VP880_VAS_INIT_WAIT,
           pDevObj->devProfileData.tickRate);

    pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER] |=
       VP_ACTIVATE_TIMER;
} /* end Vp880VOCDone */
#endif /* VP_CC_880_SERIES */
