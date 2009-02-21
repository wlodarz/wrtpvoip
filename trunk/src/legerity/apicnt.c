/** \file apicnt.c
 * apicnt.c
 *
 *  This file contains the control functions for the Vp880 device API.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_880_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"
#include "sys_service.h"

/**< Profile index for Generator A/B and C/D starting points (std tone) */
typedef enum
{
    VP880_SIGGEN_AB_START = 8,
    VP880_SIGGEN_CD_START = 16,
    VP880_SIGGEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp880_toneProfileParams;

/**< Functions called by Set Line State to abstract device states used for ABS
 * and non ABS devices (if they are different). Set line state then operates
 * on device state only (abstracted from API-II line state).
 */
static uint8
Vp880GetLineStateABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

static uint8
Vp880GetLineStateNonABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

static void
Vp880GroundStartProc(
    bool gsMode,
    VpLineCtxType *pLineCtx,
    uint8 currentLineState,
    uint8 userByte);

static void
Vp880RunLPDisc(
    VpLineCtxType *pLineCtx,
    bool discMode,
    uint8 nextSlicByte);

VpStatusType
Vp880SetFxoState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

static bool
Vp880SetStateRinging(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

/**< Function called by Set Option only. Implements the options specified by
 * the user. The calling function implements the Device/Line control. If a line
 * option is set and a device option is passed, the calling function will call
 * this function once for each line and pass it the line contexts. Therefore,
 * this function will only be subjected to either a device context and device
 * option, or a line context and a line option.
 */
static VpStatusType
Vp880SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value);

/* Function called by SetOptionInternal for Event Masking only */
static void
Vp880MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask,
    VpOptionEventMaskType *pDevEventsMask);

/* Function called by SetOptionInternal to set tx and rx timeslot */
static VpStatusType
Vp880SetTimeSlot(
    VpLineCtxType *pLineCtx,
    uint8 txSlot,
    uint8 rxSlot);

/**< Function called by Api Tick only. Processes the line passed for Api
 * Tick based operations
 */
static VpStatusType
Vp880ProcessFxsLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx);

static bool
Vp880ProcessFxoLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx);

static void
Vp880LowRingFreqDetect(
    VpLineCtxType *pLineCtx,
    VpCslacLineCondType *tempRingingSt,
    VpCslacLineCondType *tempPolRevSt,
    bool *retFlag);

/**< Function called by Api only to determine if a particular line is currently
 * running a test.
 */
static bool Vp880IsChnlUndrTst(
    Vp880DeviceObjectType *pDevObj,
    uint8 channelId);

/**
 * Vp880SetLineState()
 *  This function is the API-II wrapper function for Set Line State - Internal
 * for the Vp880 API.
 *
 * Preconditions:
 *  Same as Vp880SetLineStateInt()
 *
 * Postconditions:
 *  Same as Vp880SetLineStateInt()
 */
VpStatusType
Vp880SetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpStatusType status;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VP_DOUT(VP_DBG_API_FUNC, ("Vp880SetLineState"));

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
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    if(state == pLineObj->lineState.usrCurrent) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    VP_DOUT(DBG_LINE_STATE, ("Setting Device %d Channel %d to State %d at time %d",
        pDevObj->deviceId, pLineObj->channelId, state, pDevObj->timeStamp));

    /* Clear the "called from API" flag. This affects the cadencer */
    pLineObj->status &= ~(VP880_SLS_CALL_FROM_API);
    status = Vp880SetLineStateInt(pLineCtx, state);
    if (status == VP_STATUS_SUCCESS) {
        /*
         * Reset the "Count" for leaky line conditions because there are some
         * normal state change conditions that will increment the count, therefore
         * causing exit of LP for non-leaky line
         */
        pLineObj->leakyLineCnt = 0;
       /*
         * Special FXS handling to prevent setting the line to ringing if
         * off-hook
         */
        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
         && ((state == VP_LINE_RINGING_POLREV) || (state == VP_LINE_RINGING))) {
            pLineObj->lineState.usrCurrent = pLineObj->ringCtrl.ringTripExitSt;
        } else {
            pLineObj->lineState.usrCurrent = state;
        }
    }

    /*
     * Set the "called from API" flag. Convenience for API functions so setting
     * this flag does not need to occur in multiple locations
     */
    pLineObj->status |= VP880_SLS_CALL_FROM_API;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880SetLineStateInt()
 *  This function sets the line state for a given channel of a given device. The
 * valid line states are defined in the VpLineState type.
 *
 * Preconditions:
 *  The line must first be initialized prior to setting the line state.  The
 * state must be a valid line state as defined in the VpLineState type.
 *
 * Postconditions:
 *  Returns success code if the channel can be set and the line state is valid
 * for the type of line specified by the line context.  If successfull, the line
 * specified by the line context is set to the line state specified.
 */
VpStatusType
Vp880SetLineStateInt(
    VpLineCtxType *pLineCtx,    /**< Line context to change line state on */
    VpLineStateType state)      /**< The desired line state to set */
{
    uint8 userByte, currentStateByte, mpiData;
    uint8 mpiByte = 0;
    uint8 currentLineState;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    VpStatusType status = VP_STATUS_SUCCESS;

    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpLineStateType currentState = pLineObj->lineState.currentState;

    bool polarityInversion = FALSE;

    VP_DOUT(VP_DBG_API_FUNC, ("Vp880SetLineStateInt"));

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Modify Operating conditions ONLY if not running calibration */
    if (pLineObj->lineState.calType == VP_CSLAC_CAL_NONE) {
        /*
         * Read the status of the Operating Conditions register so we can change
         * only the TX and RX if the line state is a non-communication mode.
         * This also performs the line type/state verification.
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
            &mpiData);
        mpiData &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
        mpiData &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

        status = Vp880GetTxRxPcmMode(pLineObj, state, &mpiByte);
        if (status == VP_STATUS_SUCCESS) {
            mpiData |= mpiByte;
        } else {
            return status;
        }

        VP_DOUT(DBG_LINE_STATE, ("2. Writing 0x%02X to Operating Conditions", mpiData));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
            &mpiData);
    }

#ifdef CSLAC_SEQ_EN
    /* We're no longer in the middle of a time function */
    pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
    pLineObj->cadence.timeRemain = 0;
#endif
    /*
     * If this function is called by the application, stop the cadencer and
     * reset the Loop Supervision if it is incorrect
     */

    if (!(pLineObj->status & VP880_SLS_CALL_FROM_API)) {
        uint8 sigGenCtrl[VP880_GEN_CTRL_LEN];
#ifdef CSLAC_SEQ_EN

        /* If we're in the middle of active cadence, terminate it */
        if ((pLineObj->cadence.status & VP_CADENCE_STATUS_METERING)
         && (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
            pLineObj->lineEvents.process |= VP_LINE_EVID_MTR_ABORT;
            pLineObj->processData = pLineObj->cadence.meteringBurst;
        }
#endif
        /* Disable tones and cadencing */
        /*
         * Disable signal generator A/B/C/D before making any changes and stop
         * previous cadences
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, sigGenCtrl);
        sigGenCtrl[0] &= ~VP880_GEN_ALLON;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT, VP880_GEN_CTRL_LEN,
            sigGenCtrl);

        if (pLineObj->status & VP880_BAD_LOOP_SUP) {
            pLineObj->status &= ~(VP880_BAD_LOOP_SUP);
            VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                VP880_LOOP_SUP_LEN, pLineObj->loopSup);
        }

#ifdef CSLAC_SEQ_EN
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        pLineObj->cadence.pActiveCadence = VP_PTABLE_NULL;

        /*  If the user is changing the line state, we should stop callerId */
        if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
            VpCliStopCli(pLineCtx);
        }
#endif
    }

    /* FXO TYPE LINE HANDLING */
    if (pLineObj->status & VP880_IS_FXO) {
        status = Vp880SetFxoState(pLineCtx, state);
        if (status == VP_STATUS_SUCCESS) {
            pLineObj->lineState.previous = currentState;
            pLineObj->lineState.currentState = state;
        }
    } else {  /* FXS Handling */
        if (pDevObj->stateInt & VP880_IS_ABS) {
            userByte = Vp880GetLineStateABS(pLineCtx, state);
        } else {
            userByte = Vp880GetLineStateNonABS(pLineCtx, state);
        }

        if (userByte == 0xFF) {
            return VP_STATUS_INVALID_ARG;
        }

        /* Modify userByte depending on the current polarity */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
            VP880_SYS_STATE_LEN, &currentStateByte);
#ifdef CSLAC_SEQ_EN
        if (pLineObj->cadence.pActiveCadence != VP_NULL) {
            if ((pLineObj->cadence.status &
                (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_IGNORE_POLARITY)) ==
                (VP_CADENCE_STATUS_ACTIVE | VP_CADENCE_STATUS_IGNORE_POLARITY)) {

                userByte &= ~VP880_SS_POLARITY_MASK;
                userByte |= (currentStateByte & VP880_SS_POLARITY_MASK);
            }
        }
#endif
        if ((state == VP_LINE_RINGING) || (state == VP_LINE_RINGING_POLREV)
         || (pLineObj->lineState.currentState == VP_LINE_RINGING)
         || (pLineObj->lineState.currentState == VP_LINE_RINGING_POLREV)) {
            /*
             * Set State Ringing Returns TRUE if the line has NOT actually been
             * changed. So return at this point results in preventing the
             * internal line state values to be updated until the line is
             * changed by the cadencer.
             */
            if (Vp880SetStateRinging(pLineCtx, state) == TRUE) {
                return VP_STATUS_SUCCESS;
            }
        }

        /*
         * Implement Disconnect Recovery time for hook status if going FROM
         * a state that cannot detect off-hook to a state that can detect
         * off-hook.
         */
        switch (currentStateByte & VP880_SS_LINE_FEED_MASK) {
            case VP880_SS_DISCONNECT:
            case VP880_SS_TIP_OPEN:
            case VP880_SS_RING_OPEN:
                switch (userByte & VP880_SS_LINE_FEED_MASK) {
                    case VP880_SS_DISCONNECT:
                    case VP880_SS_TIP_OPEN:
                    case VP880_SS_RING_OPEN:
                        break;

                    default:
                        VP_DOUT(DBG_LINE_STATE, ("Setting Disconnect Recovery Timer on channel %d at time %d",
                            channelId, pDevObj->timeStamp));

                        pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] =
                            MS_TO_TICKRATE(VP_DISCONNECT_RECOVERY_TIME,
                                pDevObj->devProfileData.tickRate);

                        pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] |=
                            VP_ACTIVATE_TIMER;
                        break;
                }
                break;

            default:
                break;
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SLIC_STATE_RD,
            VP880_SLIC_STATE_LEN, &currentLineState);

        if ((currentLineState & VP880_SS_POLARITY_MASK) !=
            (userByte & VP880_SS_POLARITY_MASK)) {
            pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE] =
                MS_TO_TICKRATE(VP_POLREV_DEBOUNCE_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                |= VP_ACTIVATE_TIMER;

            polarityInversion = TRUE;
        }

        if (pLineObj->calLineData.calDone == TRUE) {
            if ((polarityInversion == TRUE) ||
                (pLineObj->calLineData.forceCalDataWrite == TRUE)) {
                pLineObj->calLineData.forceCalDataWrite = FALSE;

                if (userByte & VP880_SS_POLARITY_MASK) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, pLineObj->calLineData.dcFeedPr);
                } else {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_WRT,
                        VP880_DC_FEED_LEN, pLineObj->calLineData.dcFeed);
                }
            }
        }

        /*
         * Update the line object previous and current line state variables.
         * Note that this is not updated with ringing cadence until the line is
         * actually set to ringing (i.e., not when the application sets the line
         * to ringing with (possibly) a non-ringing state via the ringing
         * cadence.
         */
        pLineObj->lineState.previous = currentState;
        pLineObj->lineState.currentState = state;

        VP_DOUT(DBG_LINE_STATE, ("In Vp880SetLineStateInt() Current state %d, next State %d Chan %d at time %d",
            currentState, state, channelId, pDevObj->timeStamp));
        if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR)
         && ((state == VP_LINE_DISCONNECT) || (currentState == VP_LINE_DISCONNECT))) {
            if (currentState == state) {
                return VP_STATUS_SUCCESS;
            } else {
                if (currentState == VP_LINE_TIP_OPEN) {
                    Vp880GroundStartProc(FALSE, pLineCtx, currentStateByte, userByte);
                }

                if (state == VP_LINE_DISCONNECT) {
                    Vp880RunLPDisc(pLineCtx, TRUE, userByte);
                } else {
                    Vp880RunLPDisc(pLineCtx, FALSE, userByte);
                }
            }
        } else {
            if ((userByte & VP880_SS_LINE_FEED_MASK) == VP880_SS_TIP_OPEN) {
                Vp880GroundStartProc(TRUE, pLineCtx, currentStateByte, userByte);
            } else {
                Vp880GroundStartProc(FALSE, pLineCtx, currentStateByte, userByte);
            }
        }
    }

    return status;
}

/**
 * Vp880RunLPDisc()
 *  This function implements the Disconnect Enter/Exit for Low Power Mode.
 *
 * Preconditions:
 *  None. Calling function must know that this code should execute.
 *
 * Postconditions:
 *  Initial procedures are performed and timers set to enter or exit Disconnect
 * state for Low Power termination type.
 */
void
Vp880RunLPDisc(
    VpLineCtxType *pLineCtx,
    bool discMode,
    uint8 nextSlicByte)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 channelId = pLineObj->channelId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 sysState[] = {VP880_SS_ACTIVE};

    uint8 icr2Mods[VP880_ICR2_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (discMode == TRUE) {        /* Entering Disconnect */
        VP_DOUT(DBG_LINE_STATE, ("Setting Chan %d to DISCONNECT at time %d",
            channelId, pDevObj->timeStamp));

        /* Modify Register to allow System States with Switcher Disabled */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
            VP880_ICR2_LEN, icr2Mods);

        VP_DOUT(DBG_LINE_STATE, ("Disable Switcher on Chan %d at time %d",
            channelId, pDevObj->timeStamp));

        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] &= ~VP880_ICR2_ILA_DAC;
        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] |= VP880_ICR2_VOC_DAC_EN;
        icr2Mods[VP880_ICR2_VOC_DAC_INDEX+1] &= ~VP880_ICR2_VOC_DAC_EN;

        icr2Mods[VP880_ICR2_SWY_CTRL_INDEX] |= VP880_ICR2_SWY_CTRL_EN;
        icr2Mods[VP880_ICR2_SWY_CTRL_INDEX+1] &= ~VP880_ICR2_SWY_CTRL_EN;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
            VP880_ICR2_LEN, icr2Mods);

        /* Set line to discharge type state */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
            VP880_SYS_STATE_LEN, sysState);

        /* Set timer to wait before making final changes */
        pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE] =
            MS_TO_TICKRATE(VP880_TRACKER_DISABLE_TIME,
                pDevObj->devProfileData.tickRate);

        pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE]
            |= VP_ACTIVATE_TIMER;

        pDevObj->stateInt |=
            ((channelId == 0) ? VP880_LINE0_LP : VP880_LINE1_LP);

        VP_DOUT(DBG_LINE_STATE,("2. Setting LP flag for channel %d",channelId));

    } else {    /* Exiting Disconnect */
        VP_DOUT(DBG_LINE_STATE, ("Recovering Chan %d from DISCONNECT at time %d with value %d",
            channelId, pDevObj->timeStamp, nextSlicByte));

        /*
         * Modify Register to prevent System States with Switcher Disabled and
         * re-enable switcher
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
            VP880_ICR2_LEN, icr2Mods);

        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] &= ~VP880_ICR2_ILA_DAC;
        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] &= ~VP880_ICR2_VOC_DAC_EN;
        icr2Mods[VP880_ICR2_SWY_CTRL_INDEX] &= ~VP880_ICR2_SWY_CTRL_EN;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
            VP880_ICR2_LEN, icr2Mods);

        /*
         * Make sure the timer for going to disconnect stops running in case we
         * leave disconnect before it finishes.
         */
        pLineObj->lineTimers.timers.timer[VP_LINE_TRACKER_DISABLE]
            &= ~VP_ACTIVATE_TIMER;
        /*
         * If recovering from Disconnect to Standby, force the SLIC state to
         * Disconnect.
         */
        if (( nextSlicByte & VP880_SS_IDLE) == VP880_SS_IDLE) {
            Vp880DeviceStateIntType devState = (channelId == 0) ?
                (pDevObj->stateInt & VP880_LINE1_LP) :
                (pDevObj->stateInt & VP880_LINE0_LP);

            if (devState) {
                VP_DOUT(DBG_LINE_STATE,("DevState 0x%04X Forcing SLIC byte to DISCONNECT for channel %d",
                    (pDevObj->stateInt & (VP880_LINE0_LP | VP880_LINE1_LP)), channelId));

                nextSlicByte = VP880_SS_DISCONNECT;
            }
        }
        Vp880LLSetSysState(deviceId, pLineCtx, nextSlicByte, TRUE);
    }
}

/**
 * Vp880SetStateRinging()
 *  This function starts cadence and non-cadence ringing, as well as ringing
 * exit.
 *
 * Preconditions:
 *  None. Calling function must know that this code should execute.
 *
 * Postconditions:
 *  Line object is modified if ringing cadence or exiting (timers). If not
 * cadencing and ringing is started, then return TRUE. Otherwise return FALSE.
 */
bool
Vp880SetStateRinging(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

#ifdef CSLAC_SEQ_EN
    VpProfilePtrType pProfile;
#endif
    uint8 tempRingCtrlData = VP880_GEN_CTRL_EN_BIAS;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if ((state == VP_LINE_RINGING) || (state == VP_LINE_RINGING_POLREV)) {
#ifdef CSLAC_SEQ_EN
        pLineObj->cadence.pActiveCadence = pLineObj->pRingingCadence;
        /*
         * We're entering a ringing state, so determine if we need to
         * cadence. If we're not cadencing, this is "always on", so we can
         * disable the currently active cadence sequence and immediately
         * implement the ringing state change.
         */
        pProfile = pLineObj->cadence.pActiveCadence;
        if (pProfile == VP_PTABLE_NULL) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
        } else {
            /*
             * We have a non-null cadence. If the cadence was not previously
             * started, we'll start it here and let the sequencer take over.
             * Otherwise, it was previously started and this state change is
             * at the request of the sequencer.
             */
            if (!(pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)) {
                /* We have a cadence and are just starting it */
                pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
                pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
                pLineObj->cadence.pCurrentPos =
                    &pProfile[VP_PROFILE_TYPE_SEQUENCER_START];
                pLineObj->cadence.length = pProfile[VP_PROFILE_LENGTH];
                pLineObj->cadence.status &=
                    ~VP_CADENCE_STATUS_IGNORE_POLARITY;
                pLineObj->cadence.status |=
                    (pProfile[VP_PROFILE_MPI_LEN] & 0x01) ?
                        VP_CADENCE_STATUS_IGNORE_POLARITY : 0;
                return TRUE;
            }
        }
#endif
        /*
         * Cadencing already called or null cadence.  We're ready to set
         * the line to the Ringing State but we have to first make sure
         * that the signal generator parameters in the device are setup
         * for the ringing profile
         */

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, &tempRingCtrlData);

        if (pLineObj->ringingParams != VP_PTABLE_NULL) {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_RINGER_PARAMS_WRT,
                VP880_RINGER_PARAMS_LEN, pLineObj->ringingParams);
        }
#ifdef CSLAC_SEQ_EN
        /*
         * If we're in an active Ringing Cadence, and ready to go into the
         * Ringing state, generate the Ringing Event and indicate that this
         * is the Ringing On event
         */
        if (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) {
            if (pProfile[VP_PROFILE_TYPE_LSB] == VP_PRFWZ_PROFILE_RINGCAD) {
                pLineObj->lineEvents.process |= VP_LINE_EVID_RING_CAD;
                pLineObj->processData = VP_RING_CAD_MAKE;
            }
        }
#endif
    }

    /*
     * If we are exiting ringing and still on-hook at the time, set the
     * Ringing exit timer
     */
    switch (pLineObj->lineState.currentState) {
        case VP_LINE_RINGING_POLREV:
        case VP_LINE_RINGING:
            switch(state) {
                case VP_LINE_RINGING_POLREV:
                case VP_LINE_RINGING:
                    break;

                default:
                    if (!(pLineObj->lineState.condition & VP_CSLAC_HOOK)) {
                        pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE] =
                            MS_TO_TICKRATE(pLineObj->ringCtrl.ringExitDbncDur / 8,
                                pDevObj->devProfileData.tickRate);

                        if (pLineObj->ringCtrl.ringExitDbncDur) {
                            pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE]
                                |= VP_ACTIVATE_TIMER;
                        }
                    }
#ifdef CSLAC_SEQ_EN
                    /*
                     * If we're in an active Ringing Cadence, and ready to
                     * go into another state, generate the Ringing Event
                     * and indicate that this is the Ringing Off event
                     */
                    if (pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE) {
                        pProfile = pLineObj->cadence.pActiveCadence;
                        if (pProfile[VP_PROFILE_TYPE_LSB] ==
                             VP_PRFWZ_PROFILE_RINGCAD) {
                            pLineObj->lineEvents.process
                                |= VP_LINE_EVID_RING_CAD;
                            pLineObj->processData = VP_RING_CAD_BREAK;
                        }
                    }
#endif
                    break;
            }
            break;

        default:
            break;
    }

    return FALSE;
}

/**
 * Vp880SetFxoState()
 *  This function sets the line state for an FXO line.
 *
 * Preconditions:
 *  See VpSetLineState()
 *
 * Postconditions:
 *  Returns success code if the channel can be set and the line state.
 */
VpStatusType
Vp880SetFxoState(
    VpLineCtxType *pLineCtx,    /**< Line context to change line state on */
    VpLineStateType state)      /**< The desired line state to set */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VpLineStateType currentState = pLineObj->lineState.currentState;
    uint8 fxoCidLine;
    uint8 userByte, userByteBefore;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * FXO is straightforward, just set as defined by termination type since
     * we already know it's not an unsupported state (except error maybe)
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
        VP880_IODATA_REG_LEN, &userByteBefore);
    userByte = userByteBefore;

    if ((pLineObj->termType == VP_TERM_FXO_GENERIC)
     || (pLineObj->termType == VP_TERM_FXO_DISC)) {
        /* Pre-clear both bits for convenience */
        if (pLineObj->termType == VP_TERM_FXO_DISC) {
            fxoCidLine = VP880_IODATA_IO3;
        } else {
            fxoCidLine = VP880_FXO_CID_LINE;
        }

        userByte &= ~(VP880_IODATA_IO1 | fxoCidLine);

        switch(state) {
            case VP_LINE_FXO_TALK:
                /* IO3/IO1 = 00, so ok */
                break;

            case VP_LINE_FXO_OHT:
                /* IO3/IO1 = 01, so set IO1 to 1 */
                userByte |= VP880_IODATA_IO1;
                break;

            case VP_LINE_FXO_LOOP_CLOSE:
                /* IO3/IO1 = 10, so set IO3 to 1 */
                userByte |= fxoCidLine;
                break;

            case VP_LINE_FXO_LOOP_OPEN:
                /* IO3/IO1 = 11, so set IO3 and IO1 to 1 */
                userByte |= (fxoCidLine | VP880_IODATA_IO1);
                break;

            default:
                /* This should be redundant from TX/RX PCM code section above */
                return VP_STATUS_INVALID_ARG;
        }
        /* Set the loop open/close bit */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
            VP880_IODATA_REG_LEN, &userByte);

    } else {
        return VP_STATUS_INVALID_ARG;
    }

    if ( ((state == VP_LINE_FXO_TALK) || (state == VP_LINE_FXO_LOOP_CLOSE))
      && ((currentState == VP_LINE_FXO_OHT) || (state == VP_LINE_FXO_LOOP_OPEN)) ) {
        pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 0;
    }

    if ( ((state == VP_LINE_FXO_OHT) || (state == VP_LINE_FXO_LOOP_OPEN))
      && ((currentState == VP_LINE_FXO_TALK) || (state == VP_LINE_FXO_LOOP_CLOSE)) ) {
        pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 0;
    }

    /* Set the FXO CODEC Mode */
    userByte = (VP880_FXO_ACTIVATE_CODEC | VP880_FXO_SUPERVISION_EN);

    /* Perform the FXO Line State change */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
        VP880_SYS_STATE_LEN, &userByte);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880GroundStartProc()
 *  This function implements the Ground Start procedures when entering or
 * exiting a ground start state. It should be called only by SetLineState
 *
 * Preconditions:
 *  None. Calling function must know that this code should execute.
 *
 * Postconditions:
 *  Procedures according to operational notes are implemented for enter/exit
 * a ground start state. A timer is set when exiting ground start to delay
 * disable of DC bias.
 */
void
Vp880GroundStartProc(
    bool gsMode,
    VpLineCtxType *pLineCtx,
    uint8 currentLineState,
    uint8 userByte)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 sysStateCfg[VP880_SS_CONFIG_LEN];

    uint8 icr1Values[VP880_ICR1_LEN];
    uint8 icr2Values[VP880_ICR2_LEN];
    uint8 icr3Values[VP880_ICR3_LEN];
    uint8 icr4Values[VP880_ICR4_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (gsMode == TRUE) {
        /*
         * Implement "Return to Ground Start Idle" part of Ground Start
         * workaround for VoicePort Devices
         */
        pLineObj->icr1Values[VP880_ICR1_RING_AND_DAC_LOCATION] |=
            VP880_ICR1_RING_AND_DAC_B2_3;
        pLineObj->icr1Values[VP880_ICR1_RING_AND_DAC_LOCATION+1] |=
            VP880_ICR1_RING_AND_DAC_B2_3;

        VP_DOUT(DBG_LINE_STATE, ("1. Write ICR1 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
            pLineObj->icr1Values[0], pLineObj->icr1Values[1],
            pLineObj->icr1Values[2], pLineObj->icr1Values[3], channelId));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
            VP880_ICR1_LEN, pLineObj->icr1Values);

        /*
         * Disable Auto Battery Switch so Ring stays near -48V for a
         * Ring to Ground
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
            VP880_SS_CONFIG_LEN, sysStateCfg);
        sysStateCfg[0] |= VP880_AUTO_BAT_SWITCH_DIS;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
            VP880_SS_CONFIG_LEN, sysStateCfg);

        /* Set Polarity of input to Gkey Detector */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_RD,
            VP880_ICR4_LEN, icr4Values);
        icr4Values[VP880_ICR4_GKEY_DET_LOCATION] |= VP880_ICR4_GKEY_DET;
        icr4Values[VP880_ICR4_GKEY_DET_LOCATION+1] &= ~VP880_ICR4_GKEY_DET;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_WRT,
            VP880_ICR4_LEN, icr4Values);

        /*
         * Perform the SLIC state change, controlled by logic that determines
         * if low power mode can be used.
         */
        Vp880LLSetSysState(deviceId, pLineCtx, userByte, TRUE);

        /*
         * Connect Longitudinal Loop Control to ILout Pin Override
         * After this point, the Hook Bit indicates Ground Key
         */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_RD,
            VP880_ICR3_LEN, icr3Values);
        icr3Values[VP880_ICR3_LONG_LOOP_CTRL_LOCATION] |=
            VP880_ICR3_LONG_LOOP_CONTROL;
        icr3Values[VP880_ICR3_LONG_LOOP_CTRL_LOCATION+1] &=
            ~VP880_ICR3_LONG_LOOP_CONTROL;

        VP_DOUT(DBG_LINE_STATE, ("1. Ground Start: Write ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
            icr3Values[0], icr3Values[1],
            icr3Values[2], icr3Values[3], channelId));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT,
            VP880_ICR3_LEN, icr3Values);
    } else {
        Vp880LLSetSysState(deviceId, pLineCtx, userByte, TRUE);

        if (pLineObj->termType != VP_TERM_FXS_LOW_PWR) {
            uint8 beforeState = (currentLineState & VP880_SS_LINE_FEED_MASK);
            uint8 afterState =  (userByte & VP880_SS_LINE_FEED_MASK);

            if (beforeState != afterState) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_RD,
                    VP880_ICR1_LEN, icr1Values);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                    VP880_ICR2_LEN, icr2Values);

                if (beforeState == VP880_SS_DISCONNECT) {
                    icr1Values[VP880_ICR1_BIAS_OVERRIDE_LOCATION] &=
                        ~0x0F;;

                    icr2Values[VP880_ICR2_SENSE_INDEX] &=
                        ~(VP880_ICR2_DAC_SENSE | VP880_ICR2_FEED_SENSE);
                } else if (afterState == VP880_SS_DISCONNECT) {
                    icr1Values[VP880_ICR1_BIAS_OVERRIDE_LOCATION] |=
                        0x0F;
                    icr1Values[VP880_ICR1_BIAS_OVERRIDE_LOCATION+1] &= 0xF8;
                    icr1Values[VP880_ICR1_BIAS_OVERRIDE_LOCATION+1] |= 0x08;

                    icr2Values[VP880_ICR2_SENSE_INDEX] |=
                        (VP880_ICR2_DAC_SENSE | VP880_ICR2_FEED_SENSE);
                    icr2Values[VP880_ICR2_SENSE_INDEX+1] &= ~VP880_ICR2_DAC_SENSE;
                    icr2Values[VP880_ICR2_SENSE_INDEX+1] |= VP880_ICR2_FEED_SENSE;
                }
                VP_DOUT(DBG_LINE_STATE, ("Per Data Sheet -- Write ICR1 Before %d After %d 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                    beforeState,
                    afterState,
                    icr1Values[0], icr1Values[1], icr1Values[2], icr1Values[3],
                    channelId));

                VP_DOUT(DBG_LINE_STATE, ("Per Data Sheet -- Write ICR2 Before %d After %d 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                    beforeState,
                    afterState,
                    icr2Values[0], icr2Values[1], icr2Values[2], icr2Values[3],
                    channelId));

                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
                    VP880_ICR1_LEN, icr1Values);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                    VP880_ICR2_LEN, icr2Values);
            }
        }

        /*
         * We are transferring from Tip Open to some other state. Need to remove
         * the Ground Start workarounds
         */
        if ((currentLineState & VP880_SS_LINE_FEED_MASK)
            == VP880_SS_TIP_OPEN) {

            /* Re-enable Auto Battery switch on VC and later silicon */
            if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]
                >= VP880_REV_VC) {

                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, sysStateCfg);
                sysStateCfg[0] &= ~VP880_AUTO_BAT_SWITCH_DIS;
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                    VP880_SS_CONFIG_LEN, sysStateCfg);
            }

            pLineObj->icr1Values[VP880_ICR1_RING_AND_DAC_LOCATION] &=
                ~VP880_ICR1_RING_AND_DAC_B2_3;
            pLineObj->icr1Values[VP880_ICR1_RING_AND_DAC_LOCATION+1] |=
                VP880_ICR1_RING_AND_DAC_B2_3;
            VP_DOUT(DBG_LINE_STATE, ("2. Write ICR1 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                pLineObj->icr1Values[0], pLineObj->icr1Values[1],
                pLineObj->icr1Values[2], pLineObj->icr1Values[3], channelId));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
                VP880_ICR1_LEN, pLineObj->icr1Values);

            /*
             * Set a timer for 210ms before disabling the DC bias. This
             * results in retaining the Tip voltage near ground for a time
             * long enough for the CPE to detect the Tip-Gnd voltage and
             * remove/apply the Ring Grnd/Hook
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_GND_START_TIMER] =
                MS_TO_TICKRATE(VP880_GND_START_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_GND_START_TIMER]
                |= VP_ACTIVATE_TIMER;

            /*
             * Hold off from reporting hook or ground key changes for a bit
             * (debounce) longer
             */
            pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] =
                MS_TO_TICKRATE((VP880_GND_START_TIME+VP880_PING_TIME),
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT] |=
                VP_ACTIVATE_TIMER;
        }
    }
}

/**
 * Vp880SetRelGain
 *  This function adjusts the GR and GX values for a given channel of a given
 * device.  It multiplies the profile values by a factor from 0.0 to 4.0.  The
 * adjustment factors are specified in the txLevel and rxLevel parameters,
 * which are 2.14 fixed-point numbers.
 *
 * Preconditions:
 *  The line must first be initialized prior to adjusting the gains.  Any
 * pre-existing results must be cleared by calling VpGetResults() before
 * calling this function.
 *
 * Postconditions:
 *  Returns error if device is not initialized or results are not cleared.
 * Otherwise, generates a VE_LINE_EVID_GAIN_CMP event and saves results in
 * the device object for later retrieval by VpGetResults().
 */
VpStatusType
Vp880SetRelGain(
    VpLineCtxType *pLineCtx,    /**< Line context to change gains on */
    uint16 txLevel,             /**< Adjustment to line's relative Tx level */
    uint16 rxLevel,             /**< Adjustment to line's relative Rx level */
    uint16 handle)              /**< Handle value returned with the event */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pLineCtx->pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpRelGainResultsType *relGainResults = &pDevObj->relGainResults;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint32 gxInt, grInt;
    uint8 gainCSD[VP880_GX_GAIN_LEN];

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
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
        ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_DEVICE_BUSY;
    }

    relGainResults->gResult = VP_GAIN_SUCCESS;

    /* Multiply the profile gain values by the requested adjustments. */
    gxInt = (uint32)pLineObj->gain.gxInt * txLevel / 16384;
    grInt = (uint32)pLineObj->gain.grInt * rxLevel / 16384;

    /* If overflow or underflow occurred, generate out-of-range result. */
    /* Requirement: 1.0 <= gxInt <= 4.0 */
    if ((gxInt < (uint32)0x4000) || (gxInt > (uint32)0x10000)) {
        relGainResults->gResult |= VP_GAIN_GX_OOR;
        gxInt = pLineObj->gain.gxInt;
    }
    /* Requirement: 0.25 <= grInt <= 1.0 */
    if ((grInt < (uint32)0x1000) || (grInt > (uint32)0x4000)) {
        relGainResults->gResult |= VP_GAIN_GR_OOR;
        grInt = pLineObj->gain.grInt;
    }

    /*
     * Write adjusted gain values to the device, and remember them for
     * VpGetResults().
     */
    VpConvertFixed2Csd((uint16)(gxInt - 0x4000), gainCSD);
    relGainResults->gxValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GX_GAIN_WRT, VP880_GX_GAIN_LEN,
        gainCSD);

    VpConvertFixed2Csd((uint16)grInt, gainCSD);
    relGainResults->grValue = ((uint16)gainCSD[0] << 8) + gainCSD[1];
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GR_GAIN_WRT, VP880_GR_GAIN_LEN,
        gainCSD);

    /* Generate the gain-complete event. */
    pLineObj->lineEvents.response |= VP_LINE_EVID_GAIN_CMP;
    pLineObj->lineEventHandle = handle;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880MuteChannel()
 *  This function disables or enables the PCM highway for the selected line and
 * should only be called by API internal functions.
 *
 * Preconditions:
 *  The line context must be valid (i.e., pointing to a valid Vp880 line object
 * type).
 *
 * Postconditions:
 *  If mode is TRUE the TX/RX path is cut. If FALSE, the TX/RX path is enabled
 * according to the current line state and mode used for talk states.
 */
void
Vp880MuteChannel(
    VpLineCtxType *pLineCtx,    /**< Line affected */
    bool mode)                  /**< TRUE = Disable TX/RX, FALSE = enable */
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 preState, postState;
    uint8 mpiByte = 0;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * Read the status of the Operating Conditions register so we can change
     * only the TX and RX if the line state is a non-communication mode.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
        &preState);
    postState = preState;
    postState &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
    postState &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

    /*
     * If disabling, simple. Otherwise enable based on the current line state
     * and the state of the "talk" option. The "talk" option is maintained in
     * the line object and abstracted in Vp880GetTxRxMode() function
     */

    Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState, &mpiByte);

    if (mode == TRUE) {
        /*
         * If awaiting DTMF detection, enable TX, disable RX. This is higher
         * priority than Mute mode. Otherwise, disable both TX and RX.
         */
        postState |= VP880_CUT_RXPATH;  /* Mute == TRUE always cuts RX path */
#ifdef CSLAC_SEQ_EN
        if (!(pLineObj->callerId.status & VP_CID_AWAIT_TONE)) {
#endif
            /* Not awaiting tone, TX Path is disabled as well */
            postState |= VP880_CUT_TXPATH;
#ifdef CSLAC_SEQ_EN
        }
#endif
    } else {
        /*
         * It's possible that a Mute off is occuring because of end of DTMF
         * detection, or end of data generation, or end of Mute period. However,
         * we only need to check if Mute On is still enabled since DTMF
         * detection will not occur while data is being generated.
         */
#ifdef CSLAC_SEQ_EN
        if (pLineObj->callerId.status & VP_CID_MUTE_ON) {
            /*
             * Some "other" operation completed, but we're still in a Mute On
             * period.
             */
            postState |= (VP880_CUT_RXPATH | VP880_CUT_TXPATH);
        } else  {
#endif
            postState |= mpiByte;
#ifdef CSLAC_SEQ_EN
        }
#endif
    }

    if (postState != preState) {
        VP_DOUT(VP_DBG_INFO, ("3. Writing 0x%02X to Operating Conditions",
            postState));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &postState);
    }
    return;
}

/**
 * Vp880GetTxRxPcmMode()
 *  This function returns the TX/RX PCM bits for the PCM (enable/disable) mode
 * corresponding to the state passed. The results should be or'-ed with the
 * bits set to 0 prior to calling this function.
 *
 * Preconditions:
 *  None. Mapping function only.
 *
 * Postconditions:
 *  None. Mapping function only.
 */
VpStatusType
Vp880GetTxRxPcmMode(
    Vp880LineObjectType *pLineObj,
    VpLineStateType state,  /**< The state associating with PCM mode */
    uint8 *mpiByte) /**< Device Specific byte */
{
    switch(pLineObj->pcmTxRxCtrl) {
        case VP_OPTION_PCM_BOTH:
            *mpiByte = 0x00;
            break;

        case VP_OPTION_PCM_RX_ONLY:
            *mpiByte = VP880_CUT_TXPATH;
            break;

        case VP_OPTION_PCM_TX_ONLY:
            *mpiByte = VP880_CUT_RXPATH;
            break;

        case VP_OPTION_PCM_ALWAYS_ON:
            *mpiByte = 0x00;
            return VP_STATUS_SUCCESS;

        default:
            *mpiByte = 0x00;
            break;
    }

    switch(state) {
        /* Non-Talk States */
        case VP_LINE_STANDBY:
        case VP_LINE_STANDBY_POLREV:
        case VP_LINE_TIP_OPEN:
        case VP_LINE_ACTIVE:
        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_DISCONNECT:
        case VP_LINE_RINGING:
        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_IS_FXO) {
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP880_CUT_TXPATH | VP880_CUT_RXPATH);
            break;

        case VP_LINE_FXO_LOOP_OPEN:
        case VP_LINE_FXO_LOOP_CLOSE:
        case VP_LINE_FXO_RING_GND:
            if (!(pLineObj->status & VP880_IS_FXO)) {
                return VP_STATUS_INVALID_ARG;
            }
            *mpiByte |= (VP880_CUT_TXPATH | VP880_CUT_RXPATH);
            break;

        /* Talk States */
        case VP_LINE_TALK:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT:
        case VP_LINE_OHT_POLREV:
            if (pLineObj->status & VP880_IS_FXO) {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_LINE_FXO_OHT:
        case VP_LINE_FXO_TALK:
            if (!(pLineObj->status & VP880_IS_FXO)) {
                return VP_STATUS_INVALID_ARG;
            }
            break;

        default:
            break;
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880GetLineStateABS
 *  Locally used function by Vp880SetLineState to get the line state byte used
 * for ABS devices.
 *
 * Preconditions:
 *  None. State to byte mapping only.
 *
 * Postconditions:
 *  Returns the byte that should be used in the device System State register
 * for the API State passed.
 */
uint8
Vp880GetLineStateABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 returnVal = 0xFF;
    uint8 dcCalVal[VP880_DC_CAL_REG_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_VC) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_RD,
            VP880_DC_CAL_REG_LEN, dcCalVal);
    }

    switch(state) {
        case VP_LINE_STANDBY:
            returnVal = VP880_SS_IDLE;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_TIP_OPEN:
            returnVal = VP880_SS_TIP_OPEN;
            break;

        case VP_LINE_ACTIVE:
        case VP_LINE_TALK:
            returnVal = VP880_SS_ACTIVE;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_STANDBY_POLREV:    /* Idle Polrev does not work */
            returnVal = VP880_SS_ACTIVE_POLREV;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        case VP_LINE_OHT:
            returnVal = VP880_SS_ACTIVE_MID_BAT;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_OHT_POLREV:
            returnVal = VP880_SS_ACTIVE_MID_BAT_PR;
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        case VP_LINE_DISCONNECT:
            returnVal = VP880_SS_DISCONNECT;
            break;

        case VP_LINE_RINGING:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                returnVal = VP880_SS_UNBALANCED_RINGING;
            } else {
                returnVal = VP880_SS_BALANCED_RINGING;
            }
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absNormCal[channelId];
            break;

        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                returnVal = VP880_SS_UNBALANCED_RINGING_PR;
            } else {
                returnVal = VP880_SS_BALANCED_RINGING_PR;
            }
            dcCalVal[0] &= 0x0F;
            dcCalVal[0] |= pDevObj->absPolRevCal[channelId];
            break;

        default:
            break;
    }

    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_VC) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_CAL_REG_WRT,
            VP880_DC_CAL_REG_LEN, dcCalVal);
    }

    return returnVal;
}

/**
 * Vp880GetLineStateNonABS
 *  Locally used function by Vp880SetLineState to get the line state byte used
 * for non-ABS devices.
 *
 * Preconditions:
 *  None. State to byte mapping only.
 *
 * Postconditions:
 *  Returns the byte that should be used in the device System State register
 * for the API State passed.
 */
uint8
Vp880GetLineStateNonABS(
    VpLineCtxType *pLineCtx,
    VpLineStateType state)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;

    switch(state) {
        case VP_LINE_STANDBY:           return VP880_SS_IDLE;
        case VP_LINE_TIP_OPEN:          return VP880_SS_TIP_OPEN;

        case VP_LINE_ACTIVE:
        case VP_LINE_TALK:
        case VP_LINE_OHT:
            return VP880_SS_ACTIVE;

        case VP_LINE_ACTIVE_POLREV:
        case VP_LINE_TALK_POLREV:
        case VP_LINE_OHT_POLREV:
        case VP_LINE_STANDBY_POLREV:    /* Idle Polrev does not work */
            return VP880_SS_ACTIVE_POLREV;

        case VP_LINE_DISCONNECT:        return VP880_SS_DISCONNECT;
        case VP_LINE_RINGING:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                return VP880_SS_UNBALANCED_RINGING;
            } else {
                return VP880_SS_BALANCED_RINGING;
            }

        case VP_LINE_RINGING_POLREV:
            if (pLineObj->status & VP880_UNBAL_RINGING) {
                return VP880_SS_UNBALANCED_RINGING_PR;
            } else {
                return VP880_SS_BALANCED_RINGING_PR;
            }

        default:
            break;
    }

    return 0xFF;
}

/**
 * Vp880SetLineTone()
 *  This function sets the line tone with the cadence specified on the line.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The tone specified by the tone profile is sent on the line at the cadence
 * specified by the cadence profile.  If the tone is NULL, all line tones are
 * removed.  If the cadence is NULL, the cadence is set to "Always On".  This
 * function returns the success code if the tone cadence is a valid tone cadence
 * and the tone profile is a valid tone profile, or in the case where the user
 * passes in profile indexes, if the tone/cadence indexes are within the range
 * of the device.
 */
VpStatusType
Vp880SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,  /**< A pointer to a tone profile, or an
                                     * index into the profile table for the tone
                                     * to put on the line.
                                     */
    VpProfilePtrType pCadProfile,   /**< A pointer to a tone cadence profile, or
                                     * an index into the profile table for the
                                     * tone cadence to put on the line.
                                     */
    VpDtmfToneGenType *pDtmfControl)    /**< Indicates to send a DTMF tone
                                         * (either upstream or downstream) if
                                         * this parameter is not VP_NULL AND
                                         * the tone specified is VP_PTABLE_NULL
                                         */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpProfilePtrType pToneProf = VP_PTABLE_NULL;
    VpProfilePtrType pCadProf = VP_PTABLE_NULL;

    VpDigitType digit = VP_DIG_NONE;
    VpDirectionType direction = VP_DIRECTION_INVALID;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 opCond, sigGenCtrl, sigGenABCount;
    uint8 mpiByte = 0;

    /* Initialize SigGen A/B values to 0 */
    uint8 sigGenAB[VP880_SIGAB_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8 sigGenABOffset = 3;  /* Starting point after filler bytes */

    int toneIndex = VpGetProfileIndex(pToneProfile);
    int cadenceIndex = VpGetProfileIndex(pCadProfile);
    uint16 profIndex;   /* Used for bit masking the profile index table */

    VpDeviceIdType deviceId = pDevObj->deviceId;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

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
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    /*
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */

    /* Verify a good profile (index or pointer) for the tone */
    if (toneIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pToneProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONE, pToneProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pToneProf = pToneProfile;
    } else if (toneIndex < VP_CSLAC_TONE_PROF_TABLE_SIZE) {
        profIndex = (uint16)toneIndex;
        pToneProf = pDevObj->devProfileTable.pToneProfileTable[profIndex];
        if (!(pDevObj->profEntry.toneProfEntry & (0x0001 << profIndex))) {
            /* The profile is invalid -- error. */
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    /* Verify a good profile (index or pointer) for the cadence */
    if (cadenceIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pCadProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_TONECAD, pCadProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        pCadProf = pCadProfile;
    } else if (cadenceIndex < VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
        pCadProf = pDevObj->devProfileTable.pToneCadProfileTable[cadenceIndex];
        if (!(pDevObj->profEntry.toneCadProfEntry & (0x0001 << cadenceIndex))) {
            /* The profile is invalid -- error. */
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    if (pDtmfControl != VP_NULL) {
        digit = pDtmfControl->toneId;
        if (VpIsDigit(digit) == FALSE) {
            return VP_STATUS_INVALID_ARG;
        }

        direction = pDtmfControl->dir;
        if (direction != VP_DIRECTION_DS) {
            return VP_STATUS_INVALID_ARG;
        }
    }

    /* All input parameters are valid. */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    if (pLineObj->status & VP880_BAD_LOOP_SUP) {
        pLineObj->status &= ~(VP880_BAD_LOOP_SUP);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
            VP880_LOOP_SUP_LEN, pLineObj->loopSup);
    }

    /*
     * Disable signal generator A/B/C/D before making any changes and stop
     * previous cadences
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);
    sigGenCtrl &= ~VP880_GEN_ALLON;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT, VP880_GEN_CTRL_LEN,
        &sigGenCtrl);

#ifdef CSLAC_SEQ_EN
    if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
        pLineObj->cadence.pActiveCadence = pCadProf;
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;

        /* We're no longer in the middle of a time function */
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_MID_TIMER;
        pLineObj->cadence.timeRemain = 0;
    }
#endif

    /* We'll almost for sure update this register, so read it now */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD, VP880_OP_COND_LEN,
        &opCond);
    opCond &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

    /*
     * If tone profile is NULL, and either the pDtmfControl is NULL or it's
     * "digit" member is "Digit None", then shutoff the tone generators, stop
     * any active cadencing and restore the filter coefficients if they need
     * to be. Also, re-enable the audio path if it was disabled by a previous
     * DTMF generation command
     */
    if (((pToneProf == VP_PTABLE_NULL) && (pDtmfControl == VP_NULL))
     || ((pToneProf == VP_PTABLE_NULL) && (digit == VP_DIG_NONE))) {
        /* Shutoff all tones and stop the cadencing -- already done */

        /*
         * Pre-Or the bits and get the correct values based on the current
         * line state, then update the device.
         */
        opCond &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
        Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState, &mpiByte);
        opCond |= mpiByte;

        VP_DOUT(VP_DBG_INFO, ("4. Writing 0x%02X to Operating Conditions",
            opCond));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &opCond);

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    /*
     * If we're here, we're sending some tone.  If it's DTMF, we can stop the
     * active cadencer, set the time to "always on" (since the application will
     * tell us when to start/stop).
     *
     * If "direction" is some value other than the initialized value, then
     * the dtmf structure is passed and not NULL
     */
    if (direction != VP_DIRECTION_INVALID) {
#ifdef CSLAC_SEQ_EN
        /* Disable currently active cadence */
        pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
#endif
        /* Update the DTMF Generators and make the downstream connection */
        Vp880SetDTMFGenerators(pLineCtx, VP_CID_NO_CHANGE, digit);

        /*
         * Disable only the receive path since disabling the transmit path
         * also may generate noise upstream (e.g., an unterminated, but
         * assigned timeslot
         */
        opCond |= VP880_CUT_RXPATH;

        VP_DOUT(VP_DBG_INFO, ("5. Writing 0x%02X to Operating Conditions",
            opCond));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
            VP880_OP_COND_LEN, &opCond);

        /* Enable only generator A/B */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
        sigGenCtrl |= (VP880_GENB_EN | VP880_GENA_EN);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

#ifdef CSLAC_SEQ_EN
    /* If we're here, we're sending a Tone, not DTMF */
    if ((pCadProf != VP_PTABLE_NULL)
     && ((pCadProf[VP880_TONE_TYPE] == VP880_HOWLER_TONE)
      || (pCadProf[VP880_TONE_TYPE] == VP880_AUS_HOWLER_TONE)
      || (pCadProf[VP880_TONE_TYPE] == VP880_NTT_HOWLER_TONE))) {

        uint8 sigGenCD[VP880_SIGCD_PARAMS_LEN] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        if (pCadProf[VP880_TONE_TYPE] == VP880_HOWLER_TONE) {

            uint16 ukHowler[] = {
                0x08, 0x88, 0x07, 0xDB, /* Start Freqency/Amplitude (800Hz,-15dBm0) */
                0x01, 0x04, 0x00, 0x30, /* Step Frequency/Amplitude */
                0x22, 0x22, 0x7F, 0xFF  /* Stop Frequency/Amplitude (3200Hz, +3.14dBm0) */
            };

            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = ukHowler[0] << 8;
            pLineObj->cadence.startFreq |= ukHowler[1];

            pLineObj->cadence.startLevel = ukHowler[2] << 8;
            pLineObj->cadence.startLevel |= ukHowler[3];

            pLineObj->cadence.freqStep = ukHowler[4] << 8;
            pLineObj->cadence.freqStep |= ukHowler[5];

            pLineObj->cadence.levelStep = ukHowler[6] << 8;
            pLineObj->cadence.levelStep |= ukHowler[7];

            pLineObj->cadence.stopFreq = ukHowler[8] << 8;
            pLineObj->cadence.stopFreq |= ukHowler[9];

            pLineObj->cadence.stopLevel = ukHowler[10] << 8;
            pLineObj->cadence.stopLevel |= ukHowler[11];

            sigGenAB[3] = ukHowler[0];
            sigGenAB[4] = ukHowler[1];

            sigGenAB[5] = ukHowler[2];
            sigGenAB[6] = ukHowler[3];
        } else if (pCadProf[VP880_TONE_TYPE] == VP880_AUS_HOWLER_TONE) {
            uint16 ausHowler[] = {
                0x10, 0x00, 0x07, 0xDB, /* Start Freqency/Amplitude (1500Hz,-15dBm0) */
                0x00, 0xB8, 0x00, 0x18, /* Step Frequency/Amplitude  */
                0x22, 0x22, 0x7F, 0xFF  /* Stop Frequency/Amplitude (3200Hz, +3.14dBm0) */
            };

            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = ausHowler[0] << 8;
            pLineObj->cadence.startFreq |= ausHowler[1];

            pLineObj->cadence.startLevel = ausHowler[2] << 8;
            pLineObj->cadence.startLevel |= ausHowler[3];

            pLineObj->cadence.freqStep = ausHowler[4] << 8;
            pLineObj->cadence.freqStep |= ausHowler[5];

            pLineObj->cadence.levelStep = ausHowler[6] << 8;
            pLineObj->cadence.levelStep |= ausHowler[7];

            pLineObj->cadence.stopFreq = ausHowler[8] << 8;
            pLineObj->cadence.stopFreq |= ausHowler[9];

            pLineObj->cadence.stopLevel = ausHowler[10] << 8;
            pLineObj->cadence.stopLevel |= ausHowler[11];

            sigGenAB[3] = ausHowler[0];
            sigGenAB[4] = ausHowler[1];

            sigGenAB[5] = ausHowler[2];
            sigGenAB[6] = ausHowler[3];
        } else {
            uint16 nttHowler[] = {
                0x04, 0x44, 0x07, 0xDB, /* Start Freqency/Amplitude (400Hz,-15dBm0) */
                0x00, 0x00, 0x00, 0x40, /* Step Frequency/Amplitude  */
                0x04, 0x44, 0x7F, 0xFF  /* Stop Frequency/Amplitude (400Hz, +3.14dBm0) */
            };

            /* Save the parameters for use in the cadencer */
            pLineObj->cadence.startFreq = nttHowler[0] << 8;
            pLineObj->cadence.startFreq |= nttHowler[1];

            pLineObj->cadence.startLevel = nttHowler[2] << 8;
            pLineObj->cadence.startLevel |= nttHowler[3];

            pLineObj->cadence.freqStep = nttHowler[4] << 8;
            pLineObj->cadence.freqStep |= nttHowler[5];

            pLineObj->cadence.levelStep = nttHowler[6] << 8;
            pLineObj->cadence.levelStep |= nttHowler[7];

            pLineObj->cadence.stopFreq = nttHowler[8] << 8;
            pLineObj->cadence.stopFreq |= nttHowler[9];

            pLineObj->cadence.stopLevel = nttHowler[10] << 8;
            pLineObj->cadence.stopLevel |= nttHowler[11];

            sigGenAB[3] = nttHowler[0];
            sigGenAB[4] = nttHowler[1];

            sigGenAB[5] = nttHowler[2];
            sigGenAB[6] = nttHowler[3];
        }

        /* Set the signal generator A parameters to the initial value. */
        pLineObj->cadence.isFreqIncrease = TRUE;

        /* Make sure C/D are cleared */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGCD_PARAMS_WRT,
            VP880_SIGCD_PARAMS_LEN, sigGenCD);

        /* Program A/B */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
            VP880_SIGAB_PARAMS_LEN, sigGenAB);

        /* Set the parameters in the line object for cadence use */
        for (sigGenABCount = 0; sigGenABCount < VP880_SIGAB_PARAMS_LEN;
             sigGenABCount++) {
            pLineObj->cadence.regData[sigGenABCount] = sigGenAB[sigGenABCount];
        }
    } else {
#endif
        /*
         * Send the signal generator parameters to the device and enable the
         * Tone Generators -- add in the first 3 bytes (all 0x00)
         */
        for (sigGenABCount = sigGenABOffset, profIndex = 0;
             sigGenABCount < VP880_SIGAB_PARAMS_LEN;
             sigGenABCount++, profIndex++) {
            sigGenAB[sigGenABCount] =
                pToneProf[VP880_SIGGEN_AB_START + profIndex];
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
            VP880_SIGAB_PARAMS_LEN, sigGenAB);
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGCD_PARAMS_WRT,
            VP880_SIGCD_PARAMS_LEN,
            (VpProfileDataType *)(&pToneProf[VP880_SIGGEN_CD_START]));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
#ifdef CSLAC_SEQ_EN
    }
#endif

#ifdef CSLAC_SEQ_EN
    if (pCadProf == VP_PTABLE_NULL) {
        /*
         * If a tone is being actived due to caller ID, then do not stop the
         * cadencer
         */
        if (!(pLineObj->callerId.status & VP_CID_IN_PROGRESS)) {
            pLineObj->cadence.status = VP_CADENCE_RESET_VALUE;
            pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        }
#endif
        sigGenCtrl |= VP880_GEN_ALLON;

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, &sigGenCtrl);
#ifdef CSLAC_SEQ_EN
    } else {
        pLineObj->cadence.pCurrentPos =
            &(pCadProf[VP_PROFILE_TYPE_SEQUENCER_START]);
        pLineObj->cadence.status |= VP_CADENCE_STATUS_ACTIVE;
        pLineObj->cadence.length = pCadProf[VP_PROFILE_LENGTH];
        pLineObj->cadence.index = VP_PROFILE_TYPE_SEQUENCER_START;
        pLineObj->cadence.status &= ~VP_CADENCE_STATUS_IGNORE_POLARITY;
        pLineObj->cadence.status |= (pCadProf[VP_PROFILE_MPI_LEN] & 0x01) ?
            VP_CADENCE_STATUS_IGNORE_POLARITY : 0;
    }
#endif

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetRelayState()
 *  This function controls the state of controlled relays for the VP880 device.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  The indicated relay state is set for the given line.
 */
VpStatusType
Vp880SetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 ioDirection[VP880_IODIR_REG_LEN];
    uint8 ioData[VP880_IODATA_REG_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

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
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Read registers that may have partial modifications */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
        VP880_IODATA_REG_LEN, ioData);

    /* Always set the value for Test Load Control. */
    pLineObj->icr1Values[VP880_ICR1_TEST_LOAD_LOCATION-1] |=
        VP880_ICR1_TEST_LOAD_MASK;

    /* Preclear test load bits */
    pLineObj->icr1Values[VP880_ICR1_TEST_LOAD_LOCATION]
        &= ~(VP880_ICR1_TEST_LOAD_MASK);

    switch (pLineObj->termType) {
        case VP_TERM_FXS_GENERIC:
        case VP_TERM_FXS_LOW_PWR:
            switch (rState) {
                case VP_RELAY_BRIDGED_TEST:
                    if (!(pDevObj->stateInt & VP880_HAS_TEST_LOAD_SWITCH)) {
                        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                        return VP_STATUS_INVALID_ARG;
                    }
                    pLineObj->icr1Values[VP880_ICR1_TEST_LOAD_LOCATION]
                        |= VP880_ICR1_TEST_LOAD_METALLIC;

                    /* Test Load bits pre-cleared. Can pick up at Normal */

                case VP_RELAY_NORMAL:
                    break;

                default:
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_TERM_FXS_ISOLATE:
            ioDirection[0] &= ~VP880_IODIR_IO1_MASK;
            if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                ioDirection[0] |= VP880_IODIR_IO1_OPEN_DRAIN;
            } else {
                ioDirection[0] |= VP880_IODIR_IO1_OUTPUT;
            }

            switch (rState) {
                case VP_RELAY_BRIDGED_TEST:
                    if (!(pDevObj->stateInt & VP880_HAS_TEST_LOAD_SWITCH)) {
                        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                        return VP_STATUS_INVALID_ARG;
                    }
                    pLineObj->icr1Values[VP880_ICR1_TEST_LOAD_LOCATION]
                        |= VP880_ICR1_TEST_LOAD_METALLIC;

                    /* Test Load bits pre-cleared. Can pick up at Normal */

                case VP_RELAY_NORMAL:
                    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                        ioData[0] &= ~(VP880_IODATA_IO1);
                    } else {
                        ioData[0] |= VP880_IODATA_IO1;
                    }
                    break;

                case VP_RELAY_RESET:
                    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                        ioData[0] |= VP880_IODATA_IO1;
                    } else {
                        ioData[0] &= ~(VP880_IODATA_IO1);
                    }
                    break;

                default:
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        case VP_TERM_FXS_SPLITTER:
            ioDirection[0] &= ~VP880_IODIR_IO1_MASK;
            if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                ioDirection[0] |= VP880_IODIR_IO1_OPEN_DRAIN;
            } else {
                ioDirection[0] |= VP880_IODIR_IO1_OUTPUT;
            }

            switch (rState) {
                case VP_RELAY_BRIDGED_TEST:
                    if (!(pDevObj->stateInt & VP880_HAS_TEST_LOAD_SWITCH)) {
                        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                        return VP_STATUS_INVALID_ARG;
                    }
                    pLineObj->icr1Values[VP880_ICR1_TEST_LOAD_LOCATION]
                        |= VP880_ICR1_TEST_LOAD_METALLIC;

                    /* Test Load bits pre-cleared. Can pick up at Normal */

                case VP_RELAY_NORMAL:
                    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                        ioData[0] |= VP880_IODATA_IO1;
                    } else {
                        ioData[0] &= ~(VP880_IODATA_IO1);
                    }
                    break;

                case VP_RELAY_RESET:
                    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC) {
                        ioData[0] &= ~(VP880_IODATA_IO1);
                    } else {
                        ioData[0] |= VP880_IODATA_IO1;
                    }
                    break;

                default:
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_INVALID_ARG;
            }
            break;

        default:
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_INVALID_ARG;
    }


    VP_DOUT(VP_DBG_INFO, ("1. Write IODATA 0x%02X on Ch %d", ioData[0], channelId));

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
        VP880_IODATA_REG_LEN, ioData);

    VP_DOUT(VP_DBG_INFO, ("2. Write IODIR 0x%02X on Channel %d",
        ioDirection[0], channelId));
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
        VP880_IODIR_REG_LEN, ioDirection);

    VP_DOUT(VP_DBG_INFO, ("3. Write ICR1 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
        pLineObj->icr1Values[0], pLineObj->icr1Values[1],
        pLineObj->icr1Values[2], pLineObj->icr1Values[3], channelId));

    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
        VP880_ICR1_LEN, pLineObj->icr1Values);

    pLineObj->relayState = rState;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetDTMFGenerators()
 *  This function sets signal generator A/B for DTMF tone generation.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The signal generators A/B are set to the DTMF frequencies and level required
 * by the digit passed.
 */
VpStatusType
Vp880SetDTMFGenerators(
    VpLineCtxType *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 sigGenCtrl[VP880_GEN_CTRL_LEN];
    VpDeviceIdType deviceId = pDevObj->deviceId;

#ifdef CSLAC_SEQ_EN
    uint8 sigByteCount;
    uint8 sigOffset = VP_CID_PROFILE_FSK_PARAM_LEN + 2;
#endif

    uint8 sigGenABParams[VP880_SIGAB_PARAMS_LEN] = {
        0x00, 0x00, 0x00,  /* RSVD */
        0x00, 0x00, /* Replace with required column Frequency */
        0x1C, 0x32, /* Level = -10dBm */
        0x00, 0x00, /* Replace with required row Frequency */
        0x1C, 0x32  /* Level = -10dBm */
    };

    uint8 columnFreqs[] = {
        0x0C, 0xE5,    /* 1209Hz (1, 4, 7, *) */
        0x0E, 0x40,    /* 1336Hz (2, 5, 8, 0) */
        0x0F, 0xC1,    /* 1477Hz (3, 6, 9, #) */
        0x11, 0x6B     /* 1633Hz (A, B, C, D) */
    };

    uint8 rowFreqs[] = {
        0x07, 0x6F,    /* 697Hz (1, 2, 3, A) */
        0x08, 0x36,    /* 770Hz (4, 5, 6, B) */
        0x09, 0x16,    /* 852Hz (7, 8, 9, C) */
        0x0A, 0x09     /* 941Hz (*, 0, #, D) */
    };

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

#ifdef CSLAC_SEQ_EN
    /*
     * If we're generating caller ID data set the levels based on the data in
     * the CID profile
     */
    if ((pLineObj->callerId.status & VP_CID_IN_PROGRESS) &&
        (pLineObj->callerId.pCliProfile != VP_PTABLE_NULL)) {
        for (sigByteCount = 0; sigByteCount < (VP880_SIGAB_PARAMS_LEN - 3);
             sigByteCount++) {
            sigGenABParams[sigByteCount+3] =
                pLineObj->callerId.pCliProfile[sigOffset + sigByteCount];
        }
    } else {
#endif
        /*
         * If it's an FXO line then the DTMF high and low frequency levels are
         * specified in the FXO/Dialing Profile, cached in the line object.
         */
        if (pLineObj->status & VP880_IS_FXO) {
            sigGenABParams[5] = pLineObj->digitGenStruct.dtmfHighFreqLevel[0];
            sigGenABParams[6] = pLineObj->digitGenStruct.dtmfHighFreqLevel[1];
            sigGenABParams[9] = pLineObj->digitGenStruct.dtmfLowFreqLevel[0];
            sigGenABParams[10] = pLineObj->digitGenStruct.dtmfLowFreqLevel[1];
        }
#ifdef CSLAC_SEQ_EN
    }
#endif

    /* Set the Column Freqs first */
    switch(digit) {
        case 1:
        case 4:
        case 7:
        case VP_DIG_ASTER:
            sigGenABParams[3] = columnFreqs[0];
            sigGenABParams[4] = columnFreqs[1];
            break;

        case 2:
        case 5:
        case 8:
        case VP_DIG_ZERO:
            sigGenABParams[3] = columnFreqs[2];
            sigGenABParams[4] = columnFreqs[3];
            break;

        case 3:
        case 6:
        case 9:
        case VP_DIG_POUND:
            sigGenABParams[3] = columnFreqs[4];
            sigGenABParams[4] = columnFreqs[5];
            break;

        case VP_DIG_A:
        case VP_DIG_B:
        case VP_DIG_C:
        case VP_DIG_D:
            sigGenABParams[3] = columnFreqs[6];
            sigGenABParams[4] = columnFreqs[7];
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    /* Now set the row freqs */
    switch(digit) {
        case 1:
        case 2:
        case 3:
        case VP_DIG_A:
            sigGenABParams[7] = rowFreqs[0];
            sigGenABParams[8] = rowFreqs[1];
            break;

        case 4:
        case 5:
        case 6:
        case VP_DIG_B:
            sigGenABParams[7] = rowFreqs[2];
            sigGenABParams[8] = rowFreqs[3];
            break;

        case 7:
        case 8:
        case 9:
        case VP_DIG_C:
            sigGenABParams[7] = rowFreqs[4];
            sigGenABParams[8] = rowFreqs[5];
            break;

        case VP_DIG_ASTER:
        case VP_DIG_ZERO:
        case VP_DIG_POUND:
        case VP_DIG_D:
            sigGenABParams[7] = rowFreqs[6];
            sigGenABParams[8] = rowFreqs[7];
            break;

        default:
            return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SIGAB_PARAMS_WRT,
        VP880_SIGAB_PARAMS_LEN, sigGenABParams);

    /*
     * If there is no change to generator control required, it is assumed to be
     * set properly prior to this function call.
     */
    if (mode != VP_CID_NO_CHANGE) {
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_RD,
            VP880_GEN_CTRL_LEN, sigGenCtrl);
        sigGenCtrl[0] = VP880_GEN_ALLOFF;
        /*
         * For DTMF CID, the data passed may be message data, a keyed character
         * (e.g., Mark, Channel Seizure), or End of Transmission. If it's End
         * of Transmission, disable the DTMF generators immediately. Otherwise,
         * enable the DTMF generators
         */
        if ((mode == VP_CID_GENERATOR_DATA)
         || (mode == VP_CID_GENERATOR_KEYED_CHAR)) {
            sigGenCtrl[0] |= (VP880_GENA_EN | VP880_GENB_EN);

            /* Setup the line timer for the on-time for DTMF CID */
            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF] =
                MS_TO_TICKRATE(VP_CID_DTMF_ON_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_TIMER_CID_DTMF]
                |= VP_ACTIVATE_TIMER;
#ifdef CSLAC_SEQ_EN
            pLineObj->callerId.dtmfStatus |= VP_CID_ACTIVE_ON_TIME;
#endif
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GEN_CTRL_WRT,
            VP880_GEN_CTRL_LEN, sigGenCtrl);
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetOption()
 *  This function determines how to process the Option based on pDevCtx,
 * pLineCtx, and option type.  The actual options are implemented in
 * Vp880SetOptionInternal
 *
 * Preconditions:
 *  The line must first be initialized if a line context is passed, or the
 * device must be initialized if a device context is passed.
 *
 * Postconditions:
 *  The option specified is implemented either on the line, or on the device, or
 * on all lines associated with the device (see the API Reference Guide for
 * details).
 */
VpStatusType
Vp880SetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    uint8 channelId;
    Vp880DeviceObjectType *pDevObj;
    VpStatusType status = VP_STATUS_INVALID_ARG;

    VpDevCtxType *pDevCtxLocal;
    VpLineCtxType *pLineCtxLocal;
    VpDeviceIdType deviceId;

    if (pDevCtx != VP_NULL) {
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;

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
            return VP_STATUS_DEV_NOT_INITIALIZED;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

        /*
         * Valid Device Context, we already know Line context is NULL (higher
         * layer SW, process on device if device option, or process on all lines
         * associated with device if line option
         */
        switch (option) {
            case VP_OPTION_ID_EVENT_MASK:  /* Line and Device */
                Vp880SetOptionInternal(VP_NULL, pDevCtx, option, value);

            /* Line Options */
            case VP_OPTION_ID_ZERO_CROSS:
            case VP_OPTION_ID_PULSE_MODE:
            case VP_OPTION_ID_TIMESLOT:
            case VP_OPTION_ID_CODEC:
            case VP_OPTION_ID_PCM_HWY:
            case VP_OPTION_ID_LOOPBACK:
            case VP_OPTION_ID_LINE_STATE:
            case VP_OPTION_ID_RING_CNTRL:
            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                /*
                 * Loop through all of the valid channels associated with this
                 * device. Init status variable in case there are currently no
                 * line contexts associated with this device
                 */
                status = VP_STATUS_SUCCESS;
                for (channelId = 0;
                     channelId < pDevObj->staticInfo.maxChannels;
                     channelId++) {
                    pLineCtxLocal = pDevCtx->pLineCtx[channelId];
                    if (pLineCtxLocal != VP_NULL) {
                        status = Vp880SetOptionInternal(pLineCtxLocal, VP_NULL,
                            option, value);

                        if (status != VP_STATUS_SUCCESS) {
                            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                            return status;
                        }
                    }
                }
                break;

            default:
                /*
                 * Device option, or option unknown option.  Handle in lower
                 * layer
                 */
                status = Vp880SetOptionInternal(VP_NULL, pDevCtx, option, value);
                break;
        }
    } else {
        /*
         * Line context must be valid, device context is NULL, proceed as
         * normal
         */
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;

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
            return VP_STATUS_DEV_NOT_INITIALIZED;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
        status = Vp880SetOptionInternal(pLineCtx, VP_NULL, option, value);
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
}

/**
 * Vp880SetOptionInternal()
 *  This function implements on the Vp880 device the options specified from
 * Vp880SetOption().  No other function should call this function.
 *
 * Preconditions:
 *  See Vp880SetOption()
 *
 * Postconditions:
 *  See Vp880SetOption()
 */
VpStatusType
Vp880SetOptionInternal(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *value)
{
    VpDevCtxType *pDevCtxLocal;
    VpLineCtxType *pLineCtxLocal;

    VpStatusType status = VP_STATUS_SUCCESS;

    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj;
    uint8 tempData[VP880_INT_MASK_LEN], channelId, txSlot, rxSlot;

    VpDeviceIdType deviceId;

    VpOptionDeviceIoType deviceIo;

    uint8 maxChan, mpiData;
    uint8 mpiByte = 0;
    uint8 ioDirection[2] = {0x00, 0x00};
    uint8 tempSysConfig[VP880_SS_CONFIG_LEN];
    uint8 tempLoopBack[VP880_LOOPBACK_LEN];
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint16 eventMask;

    VpOptionEventMaskType *pEventsMask, *pNewEventsMask;
    Vp880DeviceStateIntType lineIsFxo;

    if (pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;

        /* Adjust the EC value for Wideband mode as needed */
        ecVal[channelId] |=
           ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

        switch (option) {
            /* Line Options */
            case VP_OPTION_ID_PULSE_MODE:
                pLineObj->pulseMode = *((VpOptionPulseModeType *)value);
                VpInitDP(&pLineObj->dpStruct);
                VpInitDP(&pLineObj->dpStruct2);
                break;

            case VP_OPTION_ID_TIMESLOT:
                txSlot = ((VpOptionTimeslotType *)value)->tx;
                rxSlot = ((VpOptionTimeslotType *)value)->rx;
                status = Vp880SetTimeSlot(pLineCtx, txSlot, rxSlot);
                break;

            case VP_OPTION_ID_CODEC:
                status = Vp880SetCodec(pLineCtx, *((VpOptionCodecType *)value));
                break;

            case VP_OPTION_ID_PCM_HWY:
                if (*((VpOptionPcmHwyType *)value) != VP_OPTION_HWY_A) {
                    return VP_STATUS_INVALID_ARG;
                }
                break;

            case VP_OPTION_ID_LOOPBACK:
                /* Timeslot loopback via loopback register */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_RD,
                    VP880_LOOPBACK_LEN, tempLoopBack);

                switch(*((VpOptionLoopbackType *)value)) {
                    case VP_OPTION_LB_TIMESLOT:
                        tempLoopBack[0] |= VP880_INTERFACE_LOOPBACK_EN;
                        break;

                    case VP_OPTION_LB_OFF:
                        tempLoopBack[0] &= ~(VP880_INTERFACE_LOOPBACK_EN);
                        break;

                    case VP_OPTION_LB_DIGITAL:
                    default:
                        return VP_STATUS_INVALID_ARG;
                }
                VP_DOUT(VP_DBG_INFO,("Writing Op Cond (Loopback) 0x%02X", tempLoopBack[0]));

                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_WRT,
                    VP880_LOOPBACK_LEN, tempLoopBack);
                break;

            case VP_OPTION_ID_LINE_STATE:
                /*
                 * Only supports one type of battery control, so make sure it
                 * is set correctly. If not, return error otherwise continue
                 */
                if (((VpOptionLineStateType *)value)->bat
                    != VP_OPTION_BAT_AUTO) {
                    return VP_STATUS_INVALID_ARG;
                }

                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (((VpOptionLineStateType *)value)->battRev == TRUE) {
                    tempSysConfig[0] &= ~(VP880_SMOOTH_PR_EN);
                } else {
                    tempSysConfig[0] |= VP880_SMOOTH_PR_EN;
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                break;

            case VP_OPTION_ID_EVENT_MASK:
                pNewEventsMask = (VpOptionEventMaskType *)value;

                /* Zero out the line-specific bits before setting the
                 * deviceEventsMask in the device object. */
                pEventsMask = &pDevObj->deviceEventsMask;
                pEventsMask->faults =
                    pNewEventsMask->faults & VP_EVCAT_FAULT_DEV_EVENTS;
                pEventsMask->signaling =
                    pNewEventsMask->signaling & VP_EVCAT_SIGNALING_DEV_EVENTS;
                pEventsMask->response =
                    pNewEventsMask->response & VP_EVCAT_RESPONSE_DEV_EVENTS;
                pEventsMask->test =
                    pNewEventsMask->test & VP_EVCAT_TEST_DEV_EVENTS;
                pEventsMask->process =
                    pNewEventsMask->process & VP_EVCAT_PROCESS_DEV_EVENTS;
                pEventsMask->fxo =
                    pNewEventsMask->fxo & VP_EVCAT_FXO_DEV_EVENTS;
                pEventsMask->packet =
                    pNewEventsMask->packet & VP_EVCAT_PACKET_DEV_EVENTS;

                /* Zero out the device-specific bits before setting the
                 * lineEventsMask in the line object. */
                pEventsMask = &pLineObj->lineEventsMask;
                pEventsMask->faults =
                    pNewEventsMask->faults & ~VP_EVCAT_FAULT_DEV_EVENTS;
                pEventsMask->signaling =
                    pNewEventsMask->signaling & ~VP_EVCAT_SIGNALING_DEV_EVENTS;
                pEventsMask->response =
                    pNewEventsMask->response & ~VP_EVCAT_RESPONSE_DEV_EVENTS;
                pEventsMask->test =
                    pNewEventsMask->test & ~VP_EVCAT_TEST_DEV_EVENTS;
                pEventsMask->process =
                    pNewEventsMask->process & ~VP_EVCAT_PROCESS_DEV_EVENTS;
                pEventsMask->fxo =
                    pNewEventsMask->fxo & ~VP_EVCAT_FXO_DEV_EVENTS;
                pEventsMask->packet =
                    pNewEventsMask->packet & ~VP_EVCAT_PACKET_DEV_EVENTS;

                /* Unmask the unmaskable */
                VpImplementNonMaskEvents(&pLineObj->lineEventsMask,
                    &pDevObj->deviceEventsMask);

                /* Mask those events that the VP880 API-II cannot generate */
                Vp880MaskNonSupportedEvents(&pLineObj->lineEventsMask,
                    &pDevObj->deviceEventsMask);

                /*
                 * The next code section prevents the device from interrupting
                 * the processor if all of the events associated with the
                 * specific hardware interrupt are masked
                 */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_INT_MASK_RD,
                    VP880_INT_MASK_LEN, tempData);

                if (pDevObj->deviceEventsMask.faults & VP_DEV_EVID_CLK_FLT) {
                    tempData[0] &= ~VP880_CFAIL_MASK;
                }

                if (!(pLineObj->status & VP880_IS_FXO)) {  /* Line is FXS */
                    /* Mask off the FXO events */
                    pLineObj->lineEventsMask.fxo |= VP_EVCAT_FXO_MASK_ALL;

                    /* Evaluate for line faults events */
                    eventMask = pLineObj->lineEventsMask.faults;
                    if (eventMask & VP_LINE_EVID_THERM_FLT) {
                        tempData[channelId] |= VP880_TEMPA1_MASK;
                    } else {
                        tempData[channelId] &= ~VP880_TEMPA1_MASK;
                    }

                    /* Evaluate for signaling events */
                    eventMask = pLineObj->lineEventsMask.signaling;
                    if ((eventMask & VP_LINE_EVID_HOOK_OFF)
                     && (eventMask & VP_LINE_EVID_HOOK_ON)
                     && (eventMask & VP_LINE_EVID_FLASH)
                     && (eventMask & VP_LINE_EVID_STARTPULSE)
                     && (eventMask & VP_LINE_EVID_PULSE_DIG)
                     && (eventMask & VP_LINE_EVID_BREAK_MAX)) {
                        tempData[channelId] |= VP880_HOOK1_MASK;
                    } else {
                        tempData[channelId] &= ~VP880_HOOK1_MASK;
                    }
                    if ((eventMask & VP_LINE_EVID_GKEY_DET)
                     && (eventMask & VP_LINE_EVID_GKEY_REL)) {
                        tempData[channelId] |= VP880_GNK1_MASK;
                    } else {
                        tempData[channelId] &= ~VP880_GNK1_MASK;
                    }

                    /* Implement Operation Note 8 on errata notice V103 */
                    tempData[channelId] &= ~(VP880_OCALMY_MASK);
                } else {  /* Line is FXO */
                    /* Mask off the FXS events */
                    pLineObj->lineEventsMask.signaling
                        |= VP880_FXS_SIGNALING_EVENTS;

                    /* Evaluate for fxo events */
                    eventMask = pLineObj->lineEventsMask.fxo;
                    if ((eventMask & VP_LINE_EVID_LIU)
                     && (eventMask & VP_LINE_EVID_LNIU)) {
                        tempData[channelId] |= VP880_LIU1_MASK;
                    } else {
                        tempData[channelId] &= ~VP880_LIU1_MASK;
                    }

                    if ((eventMask & VP_LINE_EVID_RING_ON)
                     && (eventMask & VP_LINE_EVID_RING_OFF)) {
                        tempData[channelId] |= VP880_RING1_DET_MASK;
                        if (eventMask & VP_LINE_EVID_POLREV) {
                            tempData[channelId] |= VP880_POL1_MASK;
                        }
                    } else {
                        tempData[channelId] &= ~VP880_RING1_DET_MASK;
                        tempData[channelId] &= ~VP880_POL1_MASK;
                    }

                    if ((eventMask & VP_LINE_EVID_DISCONNECT)
                     && (eventMask & VP_LINE_EVID_RECONNECT)
                     && (eventMask & VP_LINE_EVID_FEED_DIS)
                     && (eventMask & VP_LINE_EVID_FEED_EN)) {
                        tempData[channelId] |= VP880_DISC1_MASK;
                    } else {
                        tempData[channelId] &= ~VP880_DISC1_MASK;
                    }
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_INT_MASK_WRT,
                    VP880_INT_MASK_LEN, tempData);
                break;

            case VP_OPTION_ID_ZERO_CROSS:
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (*(VpOptionZeroCrossType *)value == VP_OPTION_ZC_NONE) {
                    tempSysConfig[0] |= VP880_ZXR_DIS;
                } else {
                    tempSysConfig[0] &= ~(VP880_ZXR_DIS);
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                    VP880_SS_CONFIG_LEN, tempSysConfig);

                pLineObj->ringCtrl.zeroCross = *((VpOptionZeroCrossType *)value);
                break;

            case VP_OPTION_ID_RING_CNTRL:
                pLineObj->ringCtrl = *((VpOptionRingControlType *)value);

                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                if (pLineObj->ringCtrl.zeroCross == VP_OPTION_ZC_NONE) {
                    tempSysConfig[0] |= VP880_ZXR_DIS;
                } else {
                    tempSysConfig[0] &= ~(VP880_ZXR_DIS);
                }

                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                    VP880_SS_CONFIG_LEN, tempSysConfig);
                break;

            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                pLineObj->pcmTxRxCtrl = *((VpOptionPcmTxRxCntrlType *)value);
                VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_RD,
                    VP880_OP_COND_LEN, &mpiData);
                mpiData &= ~(VP880_CUT_TXPATH | VP880_CUT_RXPATH);
                mpiData &= ~(VP880_HIGH_PASS_DIS | VP880_OPCOND_RSVD_MASK);

                Vp880GetTxRxPcmMode(pLineObj, pLineObj->lineState.currentState,
                    &mpiByte);
                mpiData |= mpiByte;

                VP_DOUT(VP_DBG_INFO, ("6. Writing 0x%02X to Operating Conditions",
                    mpiData));
                VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT,
                    VP880_OP_COND_LEN, &mpiData);
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    } else {
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;
        maxChan = pDevObj->staticInfo.maxChannels;

        for (channelId = 0; channelId < maxChan; channelId++) {
            pLineCtxLocal = pDevCtx->pLineCtx[channelId];
            if (pLineCtxLocal != VP_NULL) {
                pLineObj = pLineCtxLocal->pLineObj;

                /* Adjust the EC value for Wideband mode as needed */
                ecVal[channelId] |=
                   ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
                       VP880_WIDEBAND_MODE : 0);
            }
        }

        switch (option) {
            case VP_DEVICE_OPTION_ID_PULSE:
                pDevObj->pulseSpecs = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_PULSE2:
                pDevObj->pulseSpecs2 = *((VpOptionPulseType *)value);
                break;

            case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                pDevObj->criticalFault = *((VpOptionCriticalFltType *)value);

                if ((pDevObj->criticalFault.acFltDiscEn == TRUE)
                 || (pDevObj->criticalFault.dcFltDiscEn == TRUE)) {
                    pDevObj->criticalFault.acFltDiscEn = FALSE;
                    pDevObj->criticalFault.dcFltDiscEn = FALSE;
                    return VP_STATUS_INVALID_ARG;
                }

                if (pDevObj->criticalFault.thermFltDiscEn == TRUE) {
                    for (channelId = 0; channelId < maxChan; channelId++) {
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                            VP880_SS_CONFIG_LEN, tempSysConfig);

                        tempSysConfig[0] |= VP880_ATFS_EN;
                        VpMpiCmd(deviceId, ecVal[channelId],
                            VP880_SS_CONFIG_WRT, VP880_SS_CONFIG_LEN,
                            tempSysConfig);
                    }
                } else {
                    for (channelId = 0; channelId < maxChan; channelId++) {
                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                            VP880_SS_CONFIG_LEN, tempSysConfig);

                        tempSysConfig[0] &= ~(VP880_ATFS_EN);
                        VpMpiCmd(deviceId, ecVal[channelId],
                            VP880_SS_CONFIG_WRT, VP880_SS_CONFIG_LEN,
                            tempSysConfig);
                    }
                }
                break;

            case VP_DEVICE_OPTION_ID_DEVICE_IO:
                deviceIo = *(VpOptionDeviceIoType *)(value);

                /* Read the current direction pins */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }

                for (channelId = 0; channelId < maxChan; channelId++) {
                    pLineCtxLocal = pDevCtx->pLineCtx[channelId];

                    /* Provide a parameter for the line being FXO type if the
                     * line context does not exist. This is the only way to
                     * know which I/O lines can be controlled on the device
                     * when there are less lines associated with the device
                     * than are possible.
                     */
                    lineIsFxo = (channelId == 0) ?
                        (pDevObj->stateInt & VP880_LINE0_IS_FXO) :
                        (pDevObj->stateInt & VP880_LINE1_IS_FXO);

                    /*
                     * I/O 1 on each channel is special because it uses two
                     * bits in the IO direction register to setup I/O and
                     * Output mode and it's used by some termination types.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O 1 if the line is FXS Isolate,
                         * Splitter, or FXO
                         */
                        if ((pLineObj->termType == VP_TERM_FXS_ISOLATE)
                         || (pLineObj->termType == VP_TERM_FXS_SPLITTER)
                         || (pLineObj->status & VP880_IS_FXO)) {
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO1_MASK;

                            if (deviceIo.directionPins_31_0
                              & (0x01 << channelId)) {
                                if (deviceIo.outputTypePins_31_0
                                  & (1 << channelId)) {
                                    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]
                                        <= VP880_REV_VC) {
                                        ioDirection[channelId]
                                            |= VP880_IODIR_IO1_OPEN_DRAIN;
                                    }
                                } else {
                                    ioDirection[channelId]
                                        |= VP880_IODIR_IO1_OUTPUT;
                                }
                            } else {
                                ioDirection[channelId]
                                    |= VP880_IODIR_IO1_INPUT;
                            }
                        }
                    } else {
                        /* No line associated. So we can change I/O1 */
                        ioDirection[channelId] &= ~VP880_IODIR_IO1_MASK;

                        if (deviceIo.directionPins_31_0 & (0x01 << channelId)) {
                            if (deviceIo.outputTypePins_31_0
                                & (1 << channelId)) {
                                if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]
                                    <= VP880_REV_VC) {
                                    ioDirection[channelId] |=
                                        VP880_IODIR_IO1_OPEN_DRAIN;
                                }
                            } else {
                                ioDirection[channelId]
                                    |= VP880_IODIR_IO1_OUTPUT;
                            }
                        } else {
                            ioDirection[channelId]
                                |= VP880_IODIR_IO1_INPUT;
                        }
                    }

                    /*
                     * I/O 2 is special because it is used by some termination
                     * types.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O2 if line is FXO and is defined
                         * for use
                         */
                        if (((pLineObj->status & VP880_IS_FXO)
                           && (VP880_FXO_CID_LINE == VP880_IODATA_IO2))
                         || (pLineObj->termType == VP_TERM_FXO_DISC)) {
                        } else {
                            if (deviceIo.directionPins_31_0 & (4 << channelId)) {
                                ioDirection[channelId] |= VP880_IODIR_IO2_OUTPUT;
                            } else {
                                ioDirection[channelId] &= ~VP880_IODIR_IO2_OUTPUT;
                            }
                        }
                    } else {
                        if (deviceIo.directionPins_31_0 & (4 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO2_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO2_OUTPUT;
                        }
                    }

                    /*
                     * I/O 3 is special because it is used by some termination
                     * types and exists only on FXO devices.
                     */
                    if (pLineCtxLocal != VP_NULL) {
                        pLineObj = pLineCtxLocal->pLineObj;

                        /*
                         * Prevent changes to I/O3 if line is FXO and is defined
                         * for use
                         */

                        if (((pLineObj->status & VP880_IS_FXO)
                          && (VP880_FXO_CID_LINE == VP880_IODATA_IO3))
                         || (pLineObj->termType == VP_TERM_FXO_DISC)) {
                        } else {
                            if (deviceIo.directionPins_31_0 & (16 << channelId)) {
                                ioDirection[channelId] |= VP880_IODIR_IO3_OUTPUT;
                            } else {
                                ioDirection[channelId] &= ~VP880_IODIR_IO3_OUTPUT;
                            }
                        }
                    } else {
                        if (deviceIo.directionPins_31_0 & (16 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO3_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO3_OUTPUT;
                        }
                    }

                    /*
                     * I/O 4 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (64 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO4_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO4_OUTPUT;
                        }
                    }

                    /*
                     * I/O 5 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (256 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO5_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO5_OUTPUT;
                        }
                    }

                    /*
                     * I/O 6 is special because it exists only on FXO devices.
                     */
                    if (lineIsFxo) {
                        if (deviceIo.directionPins_31_0 & (1024 << channelId)) {
                            ioDirection[channelId] |= VP880_IODIR_IO6_OUTPUT;
                        } else {
                            ioDirection[channelId] &= ~VP880_IODIR_IO6_OUTPUT;
                        }
                    }
                }

                /* Set the current device IO control information */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VP_DOUT(VP_DBG_INFO, ("1. Write IODIR 0x%02X on Channel %d",
                        ioDirection[channelId], channelId));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    }

    return status;
}

/**
 * Vp880MaskNonSupportedEvents()
 *  This function masks the events that are not supported by the VP880 API-II.
 * It should only be called by SetOptionInternal when event masks are being
 * modified.
 *
 * Preconditions:
 *  None. Utility function to modify event structures only.
 *
 * Postconditions:
 *  Event structures passed are modified with masked bits for non-supported
 * VP880 API-II events.
 */
void
Vp880MaskNonSupportedEvents(
    VpOptionEventMaskType *pLineEventsMask, /**< Line Events Mask to modify for
                                             * non-masking
                                             */
    VpOptionEventMaskType *pDevEventsMask)  /**< Device Events Mask to modify
                                             * for non-masking
                                             */
{
    pLineEventsMask->faults |= VP880_NONSUPPORT_FAULT_EVENTS;
    pLineEventsMask->signaling |= VP880_NONSUPPORT_SIGNALING_EVENTS;
    pLineEventsMask->response |= VP880_NONSUPPORT_RESPONSE_EVENTS;
    pLineEventsMask->test |= VP880_NONSUPPORT_TEST_EVENTS;
    pLineEventsMask->process |= VP880_NONSUPPORT_PROCESS_EVENTS;
    pLineEventsMask->fxo |= VP880_NONSUPPORT_FXO_EVENTS;
    pLineEventsMask->packet |= VP880_NONSUPPORT_PACKET_EVENTS;

    pDevEventsMask->faults |= VP880_NONSUPPORT_FAULT_EVENTS;
    pDevEventsMask->signaling |= VP880_NONSUPPORT_SIGNALING_EVENTS;
    pDevEventsMask->response |= VP880_NONSUPPORT_RESPONSE_EVENTS;
    pDevEventsMask->test |= VP880_NONSUPPORT_TEST_EVENTS;
    pDevEventsMask->process |= VP880_NONSUPPORT_PROCESS_EVENTS;
    pDevEventsMask->fxo |= VP880_NONSUPPORT_FXO_EVENTS;
    pDevEventsMask->packet |= VP880_NONSUPPORT_PACKET_EVENTS;

    return;
}

/**
 * Vp880DeviceIoAccess()
 *  This function is used to access device IO pins of the Vp880. See API-II
 * documentation for more information about this function.
 *
 * Preconditions:
 *  Device/Line context should be created and initialized. For applicable
 * devices bootload should be performed before calling the function.
 *
 * Postconditions:
 *  Reads/Writes from device IO pins.
 */
VpStatusType
Vp880DeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;

    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 chanNum, maxChan;
    uint8 ioDataReg[2] = {0x00, 0x00};  /* IO Status from each channel */
    uint32 bitToDevMap[] = {
        0x00000001, /* IO1 Channel 1 */
        0x00000004, /* IO2 Channel 1 */
        0x00000002, /* IO1 Channel 2 */
        0x00000008, /* IO2 Channel 2 */

        /* These signals only exist if FXO type is detected on device */
        0x00000010, /* IO3 Channel 1 */
        0x00000040, /* IO3 Channel 2 */
        0x00000020, /* IO4 Channel 1 */
        0x00000080, /* IO4 Channel 2 */
        0x00000100, /* IO5 Channel 1 */
        0x00000400, /* IO5 Channel 2 */
        0x00000200, /* IO6 Channel 1 */
        0x00000800  /* IO6 Channel 2 */
    };
    uint8 shiftMask[8] = {
        0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE
    };

    VpDeviceIoAccessDataType *pAccessData =
        &(pDevObj->getResultsOption.optionData.deviceIoData);

    maxChan = pDevObj->staticInfo.maxChannels;

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
        return VP_STATUS_DEV_NOT_INITIALIZED;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Read the current state of the IO lines */
    for (chanNum = 0; chanNum < maxChan; chanNum++) {
        pLineCtx = pDevCtx->pLineCtx[chanNum];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[chanNum] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }

        VpMpiCmd(deviceId, ecVal[chanNum], VP880_IODATA_REG_RD,
            VP880_IODATA_REG_LEN, &ioDataReg[chanNum]);
    }

    *pAccessData = *pDeviceIoData;

    /*
     * If this is a write operation, detect the bits that should be changed
     * and send to appropriate channel
     */
    if (pDeviceIoData->accessType == VP_DEVICE_IO_WRITE) {
        for (chanNum = 0; chanNum < maxChan; chanNum++) {
            pLineCtx = pDevCtx->pLineCtx[chanNum];

            if (pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;

                /* Protect access to I/O1 if FXO or Isolate Type termination */
                if ((pLineObj->status & VP880_IS_FXO)
                 || (pLineObj->termType == VP_TERM_FXS_ISOLATE)) {
                } else {
                    uint8 io1Mask = ~((VP880_IODATA_IO1 << chanNum) & pDeviceIoData->accessMask_31_0);
                    uint8 io1Data = ((VP880_IODATA_IO1 << chanNum) & pDeviceIoData->accessMask_31_0 & pDeviceIoData->deviceIOData_31_0);
                    io1Mask = (io1Mask >> chanNum) | shiftMask[chanNum];
                    io1Data >>= chanNum;

                    ioDataReg[chanNum] &= io1Mask;
                    ioDataReg[chanNum] |= (~(io1Mask) & io1Data);
                }

                /* Protect access to I/O2 if FXO and being used */
                if (((pLineObj->status & VP880_IS_FXO)
                   && (VP880_FXO_CID_LINE == VP880_IODATA_IO2))
                 || (pLineObj->termType == VP_TERM_FXO_DISC)) {
                } else {
                    uint8 io2Mask = ~((VP880_IODATA_IO2 << (chanNum+1)) & pDeviceIoData->accessMask_31_0);
                    uint8 io2Data = ((VP880_IODATA_IO2 << (chanNum+1)) & pDeviceIoData->accessMask_31_0 & pDeviceIoData->deviceIOData_31_0);
                    io2Mask = (io2Mask >> (chanNum+1)) | shiftMask[(chanNum+1)];
                    io2Data >>= (chanNum+1);

                    ioDataReg[chanNum] &= io2Mask;
                    ioDataReg[chanNum] |= (~(io2Mask) & io2Data);
                }
            } else {
                ioDataReg[chanNum] &= ~(VP880_IODATA_IO1
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= (VP880_IODATA_IO1
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);

                ioDataReg[chanNum] &= ~((VP880_IODATA_IO2 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO2 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);
            }
#if 1
            /*
             * Protect access to I/O3 if FXO and being used. Prevent
             * access if the device is not detected as FXO on this line
             * since I/O3 is not available
             */
            if (pLineCtx != VP_NULL) {
                pLineObj = pLineCtx->pLineObj;
                if (((pLineObj->status & VP880_IS_FXO)
                   && (VP880_FXO_CID_LINE == VP880_IODATA_IO3))
                 || (pLineObj->termType == VP_TERM_FXO_DISC)) {
                } else {
                    if (pDevObj->stateInt &
                        (chanNum == 0 ? VP880_LINE0_IS_FXO : VP880_LINE1_IS_FXO)) {
                        ioDataReg[chanNum] &= ~((VP880_IODATA_IO3 << 1)
                            & pDeviceIoData->accessMask_31_0);

                        ioDataReg[chanNum] |= ((VP880_IODATA_IO3 << 1)
                            & pDeviceIoData->accessMask_31_0
                            & pDeviceIoData->deviceIOData_31_0);
                    }
                }
            } else {
                ioDataReg[chanNum] &= ~((VP880_IODATA_IO3 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO3 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);
            }

            /*
             * All other I/O Access lines are not protected, but require that
             * the line on the device is FXO type
             */
            if (pDevObj->stateInt &
                (chanNum == 0 ? VP880_LINE0_IS_FXO : VP880_LINE1_IS_FXO)) {
                ioDataReg[chanNum] &= ~((VP880_IODATA_IO4 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO4 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);

                ioDataReg[chanNum] &= ~((VP880_IODATA_IO5 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO5 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);

                ioDataReg[chanNum] &= ~((VP880_IODATA_IO6 << 1)
                    & pDeviceIoData->accessMask_31_0);

                ioDataReg[chanNum] |= ((VP880_IODATA_IO6 << 1)
                    & pDeviceIoData->accessMask_31_0
                    & pDeviceIoData->deviceIOData_31_0);
            }
#endif
            VP_DOUT(VP_DBG_INFO, ("2. Write IODATA 0x%02X on Ch %d",
                ioDataReg[chanNum], chanNum));

            VpMpiCmd(deviceId, ecVal[chanNum], VP880_IODATA_REG_WRT,
                VP880_IODATA_REG_LEN, &ioDataReg[chanNum]);
        }
    } else {
        /*
         * We're reading values, replace the data passed with the data from
         * the device
         */

        /* Clear all bits */
        pAccessData->deviceIOData_31_0 = 0;
        pAccessData->deviceIOData_63_32 = 0;

        /* Set bits from device content */
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO1)
            ? bitToDevMap[VP880_CH1_IO1] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO2)
            ? bitToDevMap[VP880_CH1_IO2] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO3)
            ? bitToDevMap[VP880_CH1_IO3] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO4)
            ? bitToDevMap[VP880_CH1_IO4] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO5)
            ? bitToDevMap[VP880_CH1_IO5] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[0] & VP880_IODATA_IO6)
            ? bitToDevMap[VP880_CH1_IO6] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO1)
            ? bitToDevMap[VP880_CH2_IO1] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO2)
            ? bitToDevMap[VP880_CH2_IO2] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO3)
            ? bitToDevMap[VP880_CH2_IO3] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO4)
            ? bitToDevMap[VP880_CH2_IO4] : 0;

        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO5)
            ? bitToDevMap[VP880_CH2_IO5] : 0;
        pAccessData->deviceIOData_31_0 |= (ioDataReg[1] & VP880_IODATA_IO6)
            ? bitToDevMap[VP880_CH2_IO6] : 0;
    }

    pDevObj->deviceEvents.response |= VP_DEV_EVID_IO_ACCESS_CMP;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetCodec()
 *  This function sets the codec mode on the line specified.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The codec mode on the line is set.  This function returns the success code
 * if the codec mode specified is supported.
 */
VpStatusType
Vp880SetCodec(
    VpLineCtxType *pLineCtx,
    VpOptionCodecType codec)    /* Encoding, as defined by LineCodec typedef */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 codecReg;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Basic error checking */
    if ((codec != VP_OPTION_LINEAR) && (codec != VP_OPTION_ALAW)
     && (codec != VP_OPTION_MLAW) && (codec != VP_OPTION_WIDEBAND)) {
        return VP_STATUS_INVALID_ARG;
    }

    if ((codec == VP_OPTION_WIDEBAND)
     && (!(pDevObj->stateInt & VP880_WIDEBAND))) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Read the current state of the codec register */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CODEC_REG_RD,
        VP880_CODEC_REG_LEN, &codecReg);

    /* Enable the desired CODEC mode */
    switch(codec) {
        case VP_OPTION_LINEAR:      /* 16 bit linear PCM */
        case VP_OPTION_WIDEBAND:    /* Wideband asumes Linear PCM */
            codecReg |= VP880_LINEAR_CODEC;
            break;

        case VP_OPTION_ALAW:                /* A-law PCM */
            codecReg &= ~(VP880_LINEAR_CODEC | VP880_ULAW_CODEC);
            break;

        case VP_OPTION_MLAW:                /* u-law PCM */
            codecReg |= VP880_ULAW_CODEC;
            codecReg &= ~(VP880_LINEAR_CODEC);
            break;

        default:
            /* Cannot reach here.  Error checking at top */
            break;
    } /* Switch */

    VpMpiCmd(deviceId, ecVal[channelId], VP880_CODEC_REG_WRT,
        VP880_CODEC_REG_LEN, &codecReg);

    pLineObj->codec = codec;

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880SetTimeSlot()
 *  This function set the RX and TX timeslot for a device channel. Valid
 * timeslot numbers start at zero. The upper bound is system dependent.
 *
 * Preconditions:
 *  The line must first be initialized.
 *
 * Postconditions:
 *  The timeslots on the line are set.  This function returns the success code
 * if the timeslot numbers specified are within the range of the device based on
 * the PCLK rate.
 */
VpStatusType
Vp880SetTimeSlot(
    VpLineCtxType *pLineCtx,
    uint8 txSlot,       /**< The TX PCM timeslot */
    uint8 rxSlot)       /**< The RX PCM timeslot */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Validate the tx time slot value */
    if(txSlot >= pDevObj->devProfileData.pcmClkRate/64) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Validate the rx time slot value */
    if(rxSlot >= pDevObj->devProfileData.pcmClkRate/64) {
        return VP_STATUS_INVALID_ARG;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_TX_TS_WRT,
        VP880_TX_TS_LEN, &txSlot);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_RX_TS_WRT,
        VP880_RX_TS_LEN, &rxSlot);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880VirtualISR()
 *  This function is called everytime the device causes an interrupt
 *
 * Preconditions
 *  A device interrupt has just occured
 *
 * Postcondition
 *  This function should be called from the each device's ISR.
 *  This function could be inlined to improve ISR performance.
 */
VpStatusType
Vp880VirtualISR(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

#if defined (VP880_INTERRUPT_LEVTRIG_MODE)
    VpSysDisableInt(deviceId);
#endif
    /* Device Interrupt Received */
    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    pDevObj->status.state |= VP_DEV_PENDING_INT;
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ApiTick()
 *  This function should be called on a periodic basis or attached to an
 * interrupt.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  The value passed (by pointer) is set to TRUE if there is an updated event.
 * The user should call the GetEventStatus function to determine the cause of
 * the event (TRUE value set).  This function always returns the success code.
 */
VpStatusType
Vp880ApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    bool tempClkFault, tempBat1Fault, tempBat2Fault, lineInTest;
    uint16 timeStampPre, tickAdder;

    *pEventStatus = FALSE;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /*
     * The timestamp is in 0.5mS increments, but the device tickrate is
     * something else. So increment by the scaled amount and detect rollover
     * by finding if the previous value is greater than the new value.
     */
    timeStampPre = pDevObj->timeStamp;
    tickAdder = pDevObj->devProfileData.tickRate / VP_CSLAC_TICKSTEP_0_5MS;
    pDevObj->timeStamp+=tickAdder;

    if (timeStampPre > pDevObj->timeStamp) {
        pDevObj->deviceEvents.signaling |= VP_DEV_EVID_TS_ROLLOVER;
    }

#if defined (VP880_INTERRUPT_LEVTRIG_MODE)
    VpSysEnableInt(deviceId);
#endif

    /* Ensure that device is initialized */
    if (!(pDevObj->status.state & VP_DEV_INIT_CMP)) {
        if (Vp880FindSoftwareInterrupts(pDevCtx)) {
            *pEventStatus = TRUE;
        }

        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
    }

    Vp880LowPowerMode(pDevCtx);

    /* Service API Timers */
    Vp880ServiceTimers(pDevCtx);

    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_SUCCESS;
   }

    /* Reset event pointers pointers */
    pDevObj->dynamicInfo.lastChan = 0;

#ifdef CSLAC_SEQ_EN
    VpServiceSeq(pDevCtx);
#endif

    /*
     * Test the interrupt to see if there is a pending interrupt.  If there is,
     * read the interrupt registers (if running in an interrupt driven mode).
     * If running in polled mode, automatically read the interrupt/status
     * registers.
     */

#if defined (VP880_EFFICIENT_POLLED_MODE)
    /* Poll the device PIO-INT line */
    pDevObj->status.state |=
        (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);
#elif defined (VP880_SIMPLE_POLLED_MODE)
    pDevObj->status.state |= VP_DEV_PENDING_INT;
#endif

    /*
     * Adjust the EC value for Wideband mode as needed and set the line test
     * flag if any line is under test.
     */
    lineInTest = FALSE;
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
        lineInTest = (Vp880IsChnlUndrTst(pDevObj, channelId) == TRUE) ?
            TRUE : lineInTest;
    }

    /* Limit the number of interrupts serviced during one tick */
    pDevObj->status.numIntServiced = pDevObj->devProfileData.maxNumInterrupts;

    /*
     * Read this buffer once per tick IF there is an active interrupt, or if
     * running line test or being forced to.
     */
    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] > VP880_REV_VC) {
        if ((pDevObj->status.state & VP_DEV_PENDING_INT)
         || (pDevObj->status.state & VP_DEV_FORCE_SIG_READ)
         || (lineInTest == TRUE)) {
            pDevObj->status.state &= ~VP_DEV_FORCE_SIG_READ;
            VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_TX_PCM_BUFF_RD,
                VP880_TX_PCM_BUFF_LEN, pDevObj->txBuffer);
        }
    }

    /* Service all pending interrupts (up to maxNumInterrupts) */
    while ((pDevObj->status.state & VP_DEV_PENDING_INT)
        && (pDevObj->status.numIntServiced > 0)) {
        VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_UL_SIGREG_RD,
            VP880_UL_SIGREG_LEN, pDevObj->intReg);

        /*******************************************************
         *         HANDLE Clock Fail Events                    *
         *******************************************************/

        /* Get the current status of the fault bit */
        tempClkFault = (pDevObj->intReg[0] & VP880_CFAIL_MASK) ? TRUE : FALSE;
        /*
         * Compare it with what we already know.  If different, generate
         * events and update the line status bits
         */
        if(tempClkFault ^ pDevObj->dynamicInfo.clkFault) {
            pDevObj->dynamicInfo.clkFault = tempClkFault;
            pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
        }

        /* Get the current status of the first battery fault bit */
        tempBat1Fault = (pDevObj->intReg[0] & VP880_OCALMY_MASK) ? TRUE : FALSE;
        tempBat2Fault = (pDevObj->intReg[0] & VP880_OCALMZ_MASK) ? TRUE : FALSE;

        /* If line 1 is FXO, the Y supply is ignored */
        pLineCtx = pDevCtx->pLineCtx[0];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                if(tempBat1Fault ^ pDevObj->dynamicInfo.bat1Fault) {
                    pDevObj->dynamicInfo.bat1Fault = tempBat1Fault;
                    pDevObj->deviceEvents.faults |= VP_DEV_EVID_BAT_FLT;
                }
            }
        }

        /* If line 2 is FXO, the Z supply is ignored */
        pLineCtx = pDevCtx->pLineCtx[1];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                if(tempBat2Fault ^ pDevObj->dynamicInfo.bat2Fault) {
                    pDevObj->dynamicInfo.bat2Fault = tempBat2Fault;
                    pDevObj->deviceEvents.faults |= VP_DEV_EVID_BAT_FLT;
                }
            }
        }

        /*
         * Compare it with what we already know.  If different, generate
         * events and update the line status bits
         */
        Vp880ServiceInterrupts(pDevCtx);

        /*
         * If level triggered, the interrupt may have been disabled (to prevent
         * a flood of interrupts), so reenable it.
         */
    #if defined (VP880_INTERRUPT_LEVTRIG_MODE)
        VpSysEnableInt(deviceId);
    #endif

        /* Clear the current interrupt indication */
        pDevObj->status.state &= ~(VP_DEV_PENDING_INT);
        pDevObj->status.numIntServiced--;

        /*
         * If operating in Efficient Polled Mode, check to see if the interrupt
         * line is still indicating an active interrupt. If in simple polled mode,
         * repeat the loop and service interrupts (if anything is changed).
         */
    #if defined (VP880_EFFICIENT_POLLED_MODE)
        /* Poll the PIO-INT line */
        pDevObj->status.state |=
            (VpSysTestInt(deviceId) ? VP_DEV_PENDING_INT : 0x00);
    #elif defined (VP880_SIMPLE_POLLED_MODE)
        pDevObj->status.state |= VP_DEV_PENDING_INT;
    #endif
    }/* End while Interrupts*/

    /* Update the dial pulse handler for lines that are set for pulse decode */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            if (!(pLineObj->status & VP880_IS_FXO)) {
                Vp880ProcessFxsLine(pDevObj, pLineCtx);
            } else {
                Vp880ProcessFxoLine(pDevObj, pLineCtx);
            }
        }
    }

    /* Collect all event activity and report to the calling function */
    if (Vp880FindSoftwareInterrupts(pDevCtx)) {
        *pEventStatus = TRUE;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ProcessFxsLine()
 *  This function should only be called by Vp880ApiTick on FXS lines. It
 * performs all line processing for operations that are Tick based
 *
 * Preconditions:
 *  Conditions defined by purpose of Api Tick.
 *
 * Postconditions:
 *  The Api variables and events (as appropriate) for the line passed have been
 * updated.
 */
VpStatusType
Vp880ProcessFxsLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x00, 0x02};
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 opNote4Step1Data[VP880_ICR2_LEN];
    uint8 channelId = pLineObj->channelId;
    bool dpStatus[2] = {FALSE, FALSE};
    VpOptionEventMaskType lineEvents1;
    VpOptionEventMaskType lineEvents2;
    uint8 devRev = pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION];
    VpDialPulseDetectStatesType beforeState, afterState;
    uint8 testDataBuffer[VP880_TX_PCM_BUFF_LEN];
    uint8 hookStatus = 0, i, validSamples;
    uint8 hookIncrement;

#ifdef CSLAC_SEQ_EN
    uint8 cidParam[VP880_CID_PARAM_LEN];
    uint8 cidState;
#endif

    lineEvents1.signaling = 0;
    lineEvents2.signaling = 0;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /*
     * If the line is configured for Dial Pulse Detection, run the Dial Pulse
     * detection code. Dial Pulse detection code will generate the appropriate
     * events
     */
    if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_ON) {

        beforeState = pLineObj->dpStruct.state;
        dpStatus[0] = VpUpdateDP(pDevObj->devProfileData.tickRate,
            &pDevObj->pulseSpecs, &pLineObj->dpStruct, &lineEvents1);
        afterState = pLineObj->dpStruct.state;

        /*
         * Update the loop open and close times according to the hook change
         * within a tick, if the device is not VA or VC revision.
         */
        if (devRev > VP880_REV_VC) {
            switch (pDevObj->txBufferDataRate) {
                case VP880_CC_500HZ_RATE:
                    hookIncrement = 16;
                    break;

                case VP880_CC_1KHZ_RATE:
                    hookIncrement = 8;
                    break;

                case VP880_CC_2KHZ_RATE:
                    hookIncrement = 4;
                    break;

                case VP880_CC_4KHZ_RATE:
                    hookIncrement = 2;
                    break;

                case VP880_CC_8KHZ_RATE:
                    hookIncrement = 1;
                    break;

                default:
                    /* We should never reach here */
                    hookIncrement = 16;
                    break;
            }

            /* If the state changed, update the hook timings */
            if (beforeState != afterState) {

                /* Collect data from the device test data buffer */
                validSamples = ((pDevObj->txBuffer[VP880_TX_BUF_HOOK_MSB_INDEX]
                    & VP880_TX_BUF_LEN_MASK) >> 4);

                if (validSamples == 7) {
                    validSamples = 6;
                }

                if (channelId == 0) {
                    hookStatus = testDataBuffer[VP880_TX_BUF_HOOK_LSB_INDEX]
                        & VP880_TX_BUF_HOOK_CHAN1_MASK;
                } else {
                    hookStatus = testDataBuffer[VP880_TX_BUF_HOOK_MSB_INDEX]
                        & VP880_TX_BUF_HOOK_MSB_MASK;
                    hookStatus <<= 2;
                    hookStatus |= ((testDataBuffer[VP880_TX_BUF_HOOK_LSB_INDEX]
                        & VP880_TX_BUF_HOOK_CHAN2_MASK) >> 6);
                }

                /* Algorithm to determine increment to the hook status */
                for (i = 1; i < (1 << validSamples); i <<= 1) {
                    if (afterState == VP_DP_DETECT_STATE_LOOP_CLOSE) {
                        if (hookStatus & i) {
                            pLineObj->dpStruct.lc_time += hookIncrement;
                            pLineObj->dpStruct.lo_time -= hookIncrement;
                        } else {
                            break;
                        }
                    } else if (afterState == VP_DP_DETECT_STATE_LOOP_OPEN) {
                        if (!(hookStatus & i)) {
                            pLineObj->dpStruct.lo_time += hookIncrement;
                            pLineObj->dpStruct.lc_time -= hookIncrement;
                        } else {
                            break;
                        }
                    }
                }
            }
        }

        dpStatus[1] = VpUpdateDP(pDevObj->devProfileData.tickRate,
            &pDevObj->pulseSpecs2, &pLineObj->dpStruct2, &lineEvents2);
        /*
         * Copy the Loop open and close time from the computed values of dpStrut
         * above.
         */
        pLineObj->dpStruct2.lc_time = pLineObj->dpStruct.lc_time;
        pLineObj->dpStruct2.lo_time = pLineObj->dpStruct.lo_time;

        /*
         * The state machines will not necessarily complete at the same time, so
         * keep track of each and when both are done, report a passing digit if
         * one exists, or invalid if no criteria was met.
         */
        if (dpStatus[0] == TRUE) {
            VP_DOUT(DBG_HOOK, ("DP Set 1 Done at %d", pDevObj->timeStamp));
            pLineObj->signaling1 = lineEvents1.signaling;
            pLineObj->status |= VP880_DP_SET1_DONE;
        }

        if (dpStatus[1] == TRUE) {
            VP_DOUT(DBG_HOOK, ("DP Set 2 Done at %d", pDevObj->timeStamp));
            pLineObj->signaling2 = lineEvents2.signaling;
            pLineObj->status |= VP880_DP_SET2_DONE;
        }

        if ((pLineObj->status & (VP880_DP_SET1_DONE | VP880_DP_SET2_DONE)) ==
            (VP880_DP_SET1_DONE | VP880_DP_SET2_DONE)) {
            if (pLineObj->dpStruct.digits != -1) {
                pLineObj->signalingData = pLineObj->dpStruct.digits;
                pLineObj->lineEvents.signaling |= pLineObj->signaling1;
                pLineObj->lineEventHandle = VP_DP_PARAM1;
            } else {
                pLineObj->signalingData = pLineObj->dpStruct2.digits;
                pLineObj->lineEvents.signaling |= pLineObj->signaling2;
                pLineObj->lineEventHandle = VP_DP_PARAM2;
            }

            if (pLineObj->signalingData == 0) {
                pLineObj->signalingData = pLineObj->lineEventHandle;
                pLineObj->lineEventHandle = pDevObj->timeStamp;
            }

            pLineObj->status &= ~(VP880_DP_SET1_DONE | VP880_DP_SET2_DONE);
            pLineObj->signaling1 = 0;
            pLineObj->signaling2 = 0;
        }
    }

#ifdef CSLAC_SEQ_EN
    /*
     * If Caller ID sequencer is in progress, update unless it's in a state of
     * suspension. If suspended, re-enable if device is in underrun (no more
     * data to transmit).
     */
    if ((pLineObj->callerId.status & VP_CID_IN_PROGRESS)
     || (pLineObj->suspendCid == TRUE)) {
        if (pLineObj->suspendCid == TRUE) {
            /*
             * Check to see if the Device Buffer is empty. If it is, continue
             * with CID.
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_RD,
                VP880_CID_PARAM_LEN, cidParam);
            cidState = (cidParam[0] & VP880_CID_STATE_MASK);

            if ((cidState == VP880_CID_STATE_URUN)
             || (cidState == VP880_CID_STATE_IDLE)) {
                pLineObj->suspendCid = FALSE;
                cidParam[0] &= ~(VP880_CID_FRAME_BITS);
                cidParam[0] |= VP880_CID_DIS;

                Vp880MuteChannel(pLineCtx, FALSE);
                VP_DOUT(DBG_CID, ("Writing 0x%02X to CID Params", cidParam[0]));
                VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_WRT,
                    VP880_CID_PARAM_LEN, cidParam);
                VpCidSeq(pLineCtx);
            }
        } else {
            VpCidSeq(pLineCtx);
        }
    }
#endif

    /* The following code is an ABS workaround for the Clare FXO interface */
    /* onhook tick counter for ABS devices*/
    /* Implementing the BSW ABS Debounce Work around */

    if ((pDevObj->stateInt & VP880_IS_ABS) &&
        (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
        if (-1 != pLineObj->onHookTicks) {
            if (0 == pLineObj->onHookTicks) {
                /* When we're done, restore the ICR6 values */
                VpMpiCmd(deviceId, ecVal[channelId],
                    VP880_ICR6_WRT, VP880_ICR6_LEN, absFix2ndOn);
            }

            if ((ON_HOOK_TIMER - ON_HOOK_ABS_CLARE_DELAY) ==
                pLineObj->onHookTicks) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                    VP880_ICR2_LEN, opNote4Step1Data);
                opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX]
                    |= VP880_ICR2_MET_SPEED_CTRL;
                opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX+1]
                    &= ~VP880_ICR2_MET_SPEED_CTRL;
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                    VP880_ICR2_LEN, opNote4Step1Data);
            }
            pLineObj->onHookTicks--;
        }
    }

    return VP_STATUS_SUCCESS;
}

/**
 * Vp880ServiceInterrupts()
 *  This function should only be called by Vp880ApiTick when an interrupt
 * occurs.
 *
 * Preconditions:
 *  The device must first be initialized.
 *
 * Postconditions:
 *  The Global Signaling Register is read and the data is stored in the device
 * object.  Depending on the dial pulse mode option set, the hook event (on/off)
 * is generated if a hook status changed.  All FXO events are reported by this
 * function (i.e., no other processing necessary). This function will return
 * TRUE if an event has been generated.
 */
bool
Vp880ServiceInterrupts(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

#ifdef CSLAC_SEQ_EN
    VpProfilePtrType pCadence;
#endif

    uint8 channelId, slacState;
    VpCslacLineCondType tempHookSt, tempGnkSt, tempThermFault;
    VpLineStateType state;

    uint8 absFix1stOn[VP880_ICR6_LEN] = {0x00, 0x02};
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x08, 0x02};
    uint8 icr2Reg[VP880_ICR2_LEN];
    bool retFlag = FALSE;

    uint8 rcn = pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION];
    uint8 sysCfg[VP880_SS_CONFIG_LEN];

    /* Adjust the EC value for Wideband mode as needed */
    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
    }

    for (channelId = 0;
         channelId < pDevObj->staticInfo.maxChannels;
         channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            state = pLineObj->lineState.currentState;


            if (!(pLineObj->status & VP880_IS_FXO)) {
                /*
                 * If debouncing for Ring Exit or Caller ID, ignore hook.
                 * Otherwise process.
                 */
                if ((pLineObj->lineTimers.timers.timer[VP_LINE_CID_DEBOUNCE]
                    & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE]
                    & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                    & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT]
                    & VP_ACTIVATE_TIMER)
                 || (pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE]
                    & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_CAL_LINE_TIMER]
                    & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineState.calType != VP_CSLAC_CAL_NONE)
                 || (pDevObj->status.state & VP_DEV_IN_CAL)

#ifdef CSLAC_SEQ_EN
                 || ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)
                 && (pLineObj->intSequence[VP_PROFILE_TYPE_LSB] ==
                        VP_PRFWZ_PROFILE_FWD_DISC_INT))

                 || ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)
                 && (pLineObj->intSequence[VP_PROFILE_TYPE_LSB] ==
                        VP_PRFWZ_PROFILE_TIP_OPEN_INT))
#endif
                 || ((state == VP_LINE_DISCONNECT))
                 || ((state == VP_LINE_TIP_OPEN))) {
                    tempHookSt = (pLineObj->lineState.condition & VP_CSLAC_HOOK);
                } else {
                    if (pLineObj->status & VP880_LOW_POWER_EN) {
                        if (pDevObj->intReg[channelId] & VP880_HOOK1_MASK) {
                            tempHookSt = 0;
                        } else {
                            tempHookSt = VP_CSLAC_HOOK;
                        }
                    } else {
                        if (pDevObj->intReg[channelId] & VP880_HOOK1_MASK) {
                            tempHookSt = VP_CSLAC_HOOK;
                        } else {
                            tempHookSt = 0;
                        }
                    }
                }

                if (pDevObj->intReg[channelId] & VP880_TEMPA1_MASK) {
                    tempThermFault = VP_CSLAC_THERM_FLT;
                } else {
                    tempThermFault = 0;
                }

                if ((pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE]
                     & VP_ACTIVATE_TIMER)
                 || (pLineObj->lineState.calType != VP_CSLAC_CAL_NONE)
                 || (state == VP_LINE_DISCONNECT)
                 || (pLineObj->lineTimers.timers.timer[VP_LINE_DISCONNECT_EXIT]
                     & VP_ACTIVATE_TIMER)) {
                    tempGnkSt = (pLineObj->lineState.condition & VP_CSLAC_GKEY);
                } else {
                    if (pDevObj->intReg[channelId] & VP880_GNK1_MASK) {
                        tempGnkSt = VP_CSLAC_GKEY;
                    } else {
                        tempGnkSt = 0;
                    }
                }

                /*
                 * We "think" we know what Hook and Gkey are now, but it's
                 * possible the API-II is in the middle of the VoicePort Ground
                 * Start workaround. Check for the conditions where what is
                 * detected MUST be a Ground Key and not a Hook
                 */
                if (!(pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE]
                      & VP_ACTIVATE_TIMER)) {
                    if ((state == VP_LINE_TIP_OPEN)
                     || (pLineObj->lineTimers.timers.timer[VP_LINE_GND_START_TIMER]
                         & VP_ACTIVATE_TIMER)) {
                        tempGnkSt = (tempHookSt || tempGnkSt) ? VP_CSLAC_GKEY : 0;
                        tempHookSt = 0;
                    }
                }

                /* If the hook conditions changed, continue line processing */
                if((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                    != tempHookSt) {
                    pLineObj->lineState.condition &= ~VP_CSLAC_HOOK;
                    pLineObj->lineState.condition |= tempHookSt;

                    if ((pLineObj->status & VP880_LOW_POWER_EN) && (tempHookSt)){
                        VP_DOUT(DBG_HOOK, ("Off-Hook Detected in Low Power Mode on line %d time %d",
                            channelId, pDevObj->timeStamp));

                        /* Force line to feed state and start leaky line detection */
                        pLineObj->lineState.currentState = VP_LINE_OHT;
                        pDevObj->stateInt &= ~((channelId == 0) ? VP880_LINE0_LP : VP880_LINE1_LP);
                        break;
                    }

#ifdef CSLAC_SEQ_EN
                    /*
                     * There was a sufficient hook activity to stop the active
                     * CID -- unless the CID sequence knew this would happen and
                     * set the debounce flag. In which case, let CID continue.
                     */
                    if (pLineObj->callerId.status & VP_CID_IN_PROGRESS) {
                        if (pLineObj->callerId.status & VP_CID_IS_DEBOUNCE) {
                            /* Hook event is fully debounced and ready to go */
                            pLineObj->callerId.status &= ~VP_CID_IS_DEBOUNCE;
                        } else {
                            VpCliStopCli(pLineCtx);
                            Vp880SetLineTone(pLineCtx, VP_PTABLE_NULL,
                                VP_PTABLE_NULL, VP_NULL);
                        }
                    }
#endif

                    if (tempHookSt) {
                        VP_DOUT(DBG_HOOK, ("Off-Hook on Line %d at Time %d Low Power 0x%02X",
                            channelId, pDevObj->timeStamp, (pLineObj->status & VP880_LOW_POWER_EN)));

                        pLineObj->dpStruct.hookSt = TRUE;
                        pLineObj->dpStruct2.hookSt = TRUE;

                        pLineObj->leakyLineCnt = 0;
                        pLineObj->status &= ~VP880_LINE_LEAK;

                        if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_OFF) {
                            pLineObj->lineEvents.signaling |=
                                VP_LINE_EVID_HOOK_OFF;
                            pLineObj->lineEventHandle = pDevObj->timeStamp;
                            retFlag = TRUE;
                        }

                        /*
                         * Implement the CLARE ABS Debounce Work around if line
                         * is not under test.
                         */
                        if (((pDevObj->stateInt & VP880_IS_ABS) &&
                             (rcn < VP880_REV_VC))
                         && (Vp880IsChnlUndrTst(pDevObj, channelId) == FALSE)) {
                            /* stopping abs BSW glitch work around */
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                                VP880_ICR2_LEN, icr2Reg);
                            icr2Reg[VP880_ICR2_MET_SPEED_INDEX]
                                &= ~VP880_ICR2_MET_SPEED_CTRL;
                            icr2Reg[VP880_ICR2_MET_SPEED_INDEX+1]
                                &= ~VP880_ICR2_MET_SPEED_CTRL;
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                                VP880_ICR2_LEN, icr2Reg);

                            /* Backdoor command for ABS errata */
                            VpMpiCmd(deviceId, ecVal[channelId],
                                VP880_ICR6_WRT, VP880_ICR6_LEN,
                                absFix2ndOn);
                            pLineObj->onHookTicks = ON_HOOK_TIMER;

                            VpMpiCmd(deviceId, ecVal[channelId],
                                VP880_SYS_STATE_RD,
                                VP880_SYS_STATE_LEN,
                                &slacState);

                            VP_DOUT(DBG_HOOK, ("Line State is 0x%02X",
                                slacState));

                            switch(slacState & VP880_SS_LINE_FEED_MASK) {
                                /*
                                 * Feed states where the SLIC needs to take
                                 * control of it's own battery.
                                 */
                                case (VP880_SS_ACTIVE & VP880_SS_LINE_FEED_MASK):
                                case (VP880_SS_IDLE & VP880_SS_LINE_FEED_MASK):
                                case (VP880_SS_ACTIVE_MID_BAT & VP880_SS_LINE_FEED_MASK):
                                    VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                                        VP880_SS_CONFIG_LEN, sysCfg);
                                    sysCfg[0] &= ~VP880_AUTO_BAT_SHUTDOWN_EN;
                                    VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                                        VP880_SS_CONFIG_LEN, sysCfg);
                                    break;

                                default:
                                    /*
                                     * Another state that either should not
                                     * cause off-hook detection, or state that
                                     * is handled by API-II functionality (e.g.,
                                     * Ring Trip).
                                     */
                                    break;
                            }
                        }

#ifdef CSLAC_SEQ_EN
                        /*
                         * If an off-hook is detected when the active cadence
                         * is a Message Waiting Pulse on the line, restore the
                         * line state.
                         */
                        pCadence = pLineObj->cadence.pActiveCadence;
                        if (pCadence != VP_PTABLE_NULL) {
                            if (pCadence[VP_PROFILE_TYPE_LSB] ==
                                VP_PRFWZ_PROFILE_MSG_WAIT_PULSE_INT) {
                                Vp880SetLineState(pLineCtx, state);
                            }
                        }
#endif
                        /*
                         * If an off-hook is detected during the user set state
                         * of Ringing (incl. ringing and silent interval) while
                         * a test is running, don't allow the api to go to the
                         * ringtrip state
                         */
                        if(Vp880IsChnlUndrTst(pDevObj, channelId) == TRUE) {
                            /* Do not change line state during test */
                        } else {
                            if ((state == VP_LINE_RINGING)
                             || (state == VP_LINE_RINGING_POLREV)) {
                                Vp880SetLineState(pLineCtx,
                                    pLineObj->ringCtrl.ringTripExitSt);

                                /* If ringtrip occurs (ringing then offhook), debounce the hook bit */
                                pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE] =
                                    MS_TO_TICKRATE(VP880_RING_TRIP_DEBOUNCE, pDevObj->devProfileData.tickRate);
                                pLineObj->lineTimers.timers.timer[VP_LINE_RING_EXIT_DEBOUNCE]
                                    |= VP_ACTIVATE_TIMER;
                            }
                        }
                    } else {
                        VP_DOUT(DBG_HOOK, ("On-Hook on Line %d at Time %d",
                            channelId, pDevObj->timeStamp));

                        pLineObj->dpStruct.hookSt = FALSE;
                        pLineObj->dpStruct2.hookSt = FALSE;

                        if(pLineObj->pulseMode == VP_OPTION_PULSE_DECODE_OFF) {
                            /*
                             * If this is the first time after initialization
                             * that we are checking for on-hook and it is
                             * on-hook, don't generate an interrupt
                             */
                            if (!(pLineObj->lineState.condition & VP_CSLAC_STATUS_VALID)) {
                                pLineObj->lineEvents.signaling |=
                                    VP_LINE_EVID_HOOK_ON;
                                pLineObj->lineEventHandle = pDevObj->timeStamp;
                                retFlag = TRUE;
                            }
                        }

                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
                            VP880_SYS_STATE_LEN, &slacState);

                        /* Implementing the BSW ABS Debounce Work around */
                        if ((pDevObj->stateInt & VP880_IS_ABS) &&
                             (rcn < VP880_REV_VC)) {

                            /* Let the API-II Take control over the battery */
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                                VP880_SS_CONFIG_LEN, sysCfg);
                            sysCfg[0] |= VP880_AUTO_BAT_SHUTDOWN_EN;
                            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                                VP880_SS_CONFIG_LEN, sysCfg);

                            switch(slacState & VP880_SS_LINE_FEED_MASK) {
                                /*
                                 * If we are detecting on-hook because we caused
                                 * it, do not implement the errata fix.
                                 */
                                case VP880_SS_TIP_OPEN:
                                case VP880_SS_DISCONNECT:
                                case VP880_SS_SHUTDOWN:
                                    break;

                                default:
                                    /* make sure the line is not being tested*/
                                    if(Vp880IsChnlUndrTst(pDevObj,
                                        channelId) == TRUE) {
                                        /*
                                         * skip the work around if chan is
                                         * not under test
                                         */
                                        break;
                                    } else {
                                        /* Backdoor command for ABS errata */
                                        VpMpiCmd(deviceId, ecVal[channelId],
                                            VP880_ICR6_WRT, VP880_ICR6_LEN,
                                            absFix1stOn);
                                        pLineObj->onHookTicks = -1;
                                    }
                                    break;
                            }
                        }

                        switch(slacState & VP880_SS_LINE_FEED_MASK) {
                            /*
                             * Feed states where the SLIC needs to be put
                             * into high battery mode to optimize
                             */
                            case (VP880_SS_ACTIVE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_IDLE & VP880_SS_LINE_FEED_MASK):
                            case (VP880_SS_ACTIVE_MID_BAT & VP880_SS_LINE_FEED_MASK):
                                slacState &= ~VP880_SS_LINE_FEED_MASK;
                                if(pLineObj->lineState.usrCurrent == VP_LINE_STANDBY) {
                                    slacState |= VP880_SS_IDLE;
                                } else {
                                    slacState |= VP880_SS_ACTIVE;
                                }
                                Vp880LLSetSysState(deviceId, pLineCtx, slacState, TRUE);
                                break;

                            default:
                                /*
                                 * Another state that either should not
                                 * cause off-hook detection, or state that
                                 * is handled by API-II functionality (e.g.,
                                 * Ring Trip).
                                 */
                                break;
                        }
                    }
                }

                /* If the gkey conditions changed, continue line processing */
                if((pLineObj->lineState.condition & VP_CSLAC_GKEY)
                    != tempGnkSt) {

                    if (tempGnkSt) {
                        pLineObj->lineEvents.signaling |= VP_LINE_EVID_GKEY_DET;
                        pLineObj->lineState.condition |= VP_CSLAC_GKEY;
                    } else {
                        pLineObj->lineEvents.signaling |= VP_LINE_EVID_GKEY_REL;
                        pLineObj->lineState.condition &= ~(VP_CSLAC_GKEY);
                    }
                    retFlag = TRUE;
                    pLineObj->lineEventHandle = pDevObj->timeStamp;
                }

                if((pLineObj->lineState.condition & VP_CSLAC_THERM_FLT)
                    != tempThermFault) {
                    pLineObj->lineEventHandle = pDevObj->timeStamp;
                    pLineObj->lineState.condition &= ~(VP_CSLAC_THERM_FLT);
                    pLineObj->lineState.condition |= tempThermFault;

                    pLineObj->lineEvents.faults |= VP_LINE_EVID_THERM_FLT;
                    retFlag = TRUE;

                    if ((tempThermFault == VP_CSLAC_THERM_FLT)
                     && (pDevObj->criticalFault.thermFltDiscEn == TRUE)) {
                        Vp880SetLineState(pLineCtx, VP_LINE_DISCONNECT);
                    }
                }
            }
        }
    }

    return retFlag;
}

/**
 * Vp880LLSetSysState()
 *  This function writes to the System State register and based on the state
 * being set, determines if low power mode can or cannot be used, or if the line
 * is recovering from Disconnect to a Feed state. In the latter case, a timer
 * is set to transition through Tip Open first to prevent "ping" of the phone.
 * In this case, the final state is set when the timer expires.
 *
 * Preconditions:
 *  This function is called internally by the API-II only.
 *
 * Postconditions:
 *  The System State Register is updated with either the value passed, or
 * Tip Open. If low power mode termination type exists, a flag in the device
 * object indicating that low power can or cannot be used is modified. If coming
 * out of Disconnect, Tip Open is written and a timer is set.
 */
void
Vp880LLSetSysState(
    VpDeviceIdType deviceId,
    VpLineCtxType *pLineCtx,
    uint8 lineState,
    bool writeToDevice)
{
    Vp880LineObjectType *pLineObj = VP_NULL;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = 0;
    VpDevCtxType *pDevCtx = VP_NULL;
    Vp880DeviceObjectType *pDevObj = VP_NULL;
    uint8 lineStatePre[VP880_SYS_STATE_LEN];
    bool lineIsFxs = FALSE;

    if (pLineCtx != VP_NULL) {
        pLineObj = pLineCtx->pLineObj;
        pDevCtx = pLineCtx->pDevCtx;
        pDevObj = pDevCtx->pDevObj;
        channelId = pLineObj->channelId;

        /* Adjust the EC value for Wideband mode as needed */
        ecVal[channelId] |=
            ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

        if (!(pLineObj->status & VP880_IS_FXO)) {
            lineIsFxs = TRUE;
            if (((Vp880IsChnlUndrTst(pDevObj, channelId) == TRUE)) ||
                 (pLineObj->lineState.calType != VP_CSLAC_CAL_NONE)) {
                pDevObj->stateInt &=
                    ((channelId == 0) ? ~VP880_LINE0_LP : ~VP880_LINE1_LP);
                VP_DOUT(DBG_LINE_STATE,("3. Clearing LP flag for channel %d",channelId));
            } else {
                if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR)
                 && (!(pLineObj->status & VP880_LINE_LEAK))) {
                    if ((pLineObj->lineState.currentState == VP_LINE_DISCONNECT) ||
                        (pLineObj->lineState.currentState == VP_LINE_STANDBY)) {
                        pDevObj->stateInt |=
                            ((channelId == 0) ? VP880_LINE0_LP : VP880_LINE1_LP);

                        VP_DOUT(DBG_LINE_STATE,("1. Setting LP flag for channel %d",channelId));
                    } else {
                        pDevObj->stateInt &=
                            ((channelId == 0) ? ~VP880_LINE0_LP : ~VP880_LINE1_LP);
                        VP_DOUT(DBG_LINE_STATE,("1. Clearing LP flag for channel %d",channelId));
                    }
                } else {
                    pDevObj->stateInt &=
                        ((channelId == 0) ? ~VP880_LINE0_LP : ~VP880_LINE1_LP);
                    VP_DOUT(DBG_LINE_STATE,("2. Clearing LP flag for channel %d Status 0x%04X",
                        channelId, pLineObj->status));
                }
            }
        }
    }

    /* Device Write Override: setting flags without a device write */
    if (writeToDevice == FALSE) {
        return;
    }

    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_RD,
        VP880_SYS_STATE_LEN, lineStatePre);

    if (((lineStatePre[0] & VP880_SS_LINE_FEED_MASK) == VP880_SS_DISCONNECT)
      && (lineIsFxs == TRUE)) {
        pLineObj->nextSlicValue = lineState;

        if ((pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] <= VP880_REV_VC)) {
            lineState = VP880_SS_TIP_OPEN;

            VP_DOUT(DBG_LINE_STATE, ("2. Setting Chan %d to TIP_OPEN at Time %d",
                channelId, pDevObj->timeStamp));

            pLineObj->lineTimers.timers.timer[VP_LINE_PING_TIMER] =
                MS_TO_TICKRATE(VP880_PING_TIME,
                    pDevObj->devProfileData.tickRate);

            pLineObj->lineTimers.timers.timer[VP_LINE_PING_TIMER]
                |= VP_ACTIVATE_TIMER;
        }

        VP_DOUT(DBG_LINE_STATE, ("2. Setting Chan %d to System State 0x%02X at Time %d",
            channelId, lineState, pDevObj->timeStamp));

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
            VP880_SYS_STATE_LEN, &lineState);
    } else {
        VP_DOUT(DBG_LINE_STATE, ("1. Setting Chan %d to System State 0x%02X at Time %d",
            channelId, lineState, pDevObj->timeStamp));

        if (pLineObj->lineTimers.timers.timer[VP_LINE_PING_TIMER] & VP_ACTIVATE_TIMER) {
            pLineObj->nextSlicValue = lineState;
        } else {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                VP880_SYS_STATE_LEN, &lineState);
        }
    }
}

/**
 * Vp880LowPowerMode()
 *  This function is called when the device should be updated for Low Power
 * mode. It determines if the device can be put into low power mode and does
 * (if it can), sets a flag in the device object, and sets the device timer
 * for hook debounce.
 *
 * Preconditions:
 *
 * Postconditions:
 */
void
Vp880LowPowerMode(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpLineCtxType *pLineCtx;
    bool isValidCtx[VP880_MAX_NUM_CHANNELS] = {FALSE, FALSE};
    Vp880LineObjectType *pLineObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    bool lowPower;

    uint8 maxChannels = pDevObj->staticInfo.maxChannels;
    uint8 channelId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 icr1Data[VP880_ICR1_LEN] = {0x00, 0x00, 0x00, 0x00};
    uint8 icr2Data[VP880_ICR2_LEN] = {0x00, 0x00, 0x00, 0x00};
    uint8 icr3Data[VP880_ICR3_LEN] = {0x00, 0x00, 0x00, 0x00};
    uint8 icr4Data[VP880_ICR4_LEN] = {0x00, 0x00, 0x00, 0x00};
    uint8 lineState[VP880_SYS_STATE_LEN];

    uint8 swParams[VP880_REGULATOR_PARAM_LEN] = {0x00, 0x0D, 0x0A};

    /* Don't do anything if device is in calibration */
    if (pDevObj->status.state & VP_DEV_IN_CAL) {
        return;
    }

    /*
     * Low Power is only possible if both lines can use low power mode.
     * Otherwise, exit.
     */
    if ((pDevObj->stateInt & (VP880_LINE0_LP | VP880_LINE1_LP)) !=
        (VP880_LINE0_LP | VP880_LINE1_LP)) {
        lowPower = FALSE;
    } else {
        lowPower = TRUE;
    }

    /*
     * Determine which lines are valid in case we have to adjust their line
     * states. Consider "valid" only those lines that are FXS Low Power type.
     */
    for (channelId = 0; channelId < maxChannels; channelId++) {
        if (pDevCtx->pLineCtx[channelId] != VP_NULL) {
            pLineCtx = pDevCtx->pLineCtx[channelId];
            pLineObj = pLineCtx->pLineObj;
            if ((!(pLineObj->status & VP880_IS_FXO))
               && (pLineObj->termType == VP_TERM_FXS_LOW_PWR)) {
                isValidCtx[channelId] = TRUE;
                if(Vp880IsChnlUndrTst(pDevObj, channelId) == TRUE) {
                    lowPower = FALSE;
                }

                if ((pLineObj->lineTimers.timers.timer[VP_LINE_POLREV_DEBOUNCE]
                    & VP_ACTIVATE_TIMER) && (lowPower == TRUE)){
                    VP_DOUT(DBG_LINE_STATE, ("Delay LP Enter for PolRev at time %d",
                        pDevObj->timeStamp));
                    return;
                }
            }

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[channelId] |=
                ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);
        }
    }

    if (lowPower == FALSE) {
        /*
         * Take the device out of low power mode and set channels to correct
         * states. Do not affect device or channels if change has already
         * been made.
         */
        for (channelId = 0; channelId < maxChannels; channelId++) {
            if (isValidCtx[channelId] == TRUE) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                pLineObj = pLineCtx->pLineObj;

                if (pLineObj->status & VP880_LOW_POWER_EN) {
                    VP_DOUT(DBG_LINE_STATE, ("Taking Channel %d out of Low Power Mode at time %d",
                        channelId, pDevObj->timeStamp));

                    pLineObj->status &= ~VP880_LOW_POWER_EN;

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_RD,
                        VP880_ICR3_LEN, icr3Data);
                    icr3Data[VP880_ICR3_LINE_CTRL_INDEX] &= ~VP880_ICR3_LINE_CTRL;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT,
                        VP880_ICR3_LEN, icr3Data);
                    VP_DOUT(DBG_LINE_STATE, ("LP Exit: Writing ICR3 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr3Data[0], icr3Data[1], icr3Data[2], icr3Data[3]));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_RD,
                        VP880_ICR4_LEN, icr4Data);
                    icr4Data[VP880_ICR4_SUP_INDEX] &=
                        ~(VP880_ICR4_SUP_DAC_CTRL | VP880_ICR4_SUP_DET_CTRL
                        | VP880_ICR4_SUP_POL_CTRL);
                    icr4Data[VP880_ICR4_VOICE_DAC_INDEX] &= ~VP880_ICR4_VOICE_DAC_CTRL;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_WRT, VP880_ICR4_LEN,
                        icr4Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR4 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr4Data[0], icr4Data[1], icr4Data[2], icr4Data[3]));

                    VP_DOUT(DBG_LINE_STATE, ("Current %d, User Current %d",
                        pLineObj->lineState.currentState, pLineObj->lineState.usrCurrent));

                    Vp880SetLineStateInt(pLineCtx,
                        pLineObj->lineState.currentState);

                    /* Remove previously set SW control of ICR1 */
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
                        VP880_ICR1_LEN, icr1Data);
                    icr1Data[VP880_ICR1_BIAS_OVERRIDE_LOCATION] &=
                        ~VP880_ICR1_LINE_BIAS_OVERRIDE;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
                        VP880_ICR1_LEN, icr1Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr1Data[0], icr1Data[1], icr1Data[2], icr1Data[3]));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                        VP880_ICR2_LEN, icr2Data);
                    icr2Data[VP880_ICR2_SENSE_INDEX] &=
                        ~(VP880_ICR2_RING_SENSE | VP880_ICR2_TIP_SENSE |
                          VP880_ICR2_DAC_SENSE | VP880_ICR2_FEED_SENSE);
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                        VP880_ICR2_LEN, icr2Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr2Data[0], icr2Data[1], icr2Data[2], icr2Data[3]));

                    VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
                        VP880_REGULATOR_PARAM_LEN, pDevObj->swParams);

                    /* Need timer here to allow switcher to stabilize */
                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] =
                        MS_TO_TICKRATE(VP880_PWR_SWITCH_DEBOUNCE,
                            pDevObj->devProfileData.tickRate);

                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] |=
                        VP_ACTIVATE_TIMER;
                }
            }
        }
    } else {
        /*
         * We should be in low power mode because both lines can be put into
         * low power mode. Don't need to call Set Line State in this case for
         * each line because there are a limited number of API-II states that
         * can allow Low Power, and all required the SLIC state to be set to
         * Disconnect.
         */
        for (channelId = 0; channelId < maxChannels; channelId++) {
            if (isValidCtx[channelId] == TRUE) {
                pLineCtx = pDevCtx->pLineCtx[channelId];
                pLineObj = pLineCtx->pLineObj;

                if (!(pLineObj->status & VP880_LOW_POWER_EN)) {
                    VP_DOUT(DBG_LINE_STATE, ("Putting Channel %d in Low Power Mode at time %d",
                        channelId, pDevObj->timeStamp));

                    pLineObj->status |= VP880_LOW_POWER_EN;

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_RD,
                        VP880_ICR1_LEN, icr1Data);
                    icr1Data[VP880_ICR1_BIAS_OVERRIDE_LOCATION] |=
						VP880_ICR1_LINE_BIAS_OVERRIDE;
                    icr1Data[VP880_ICR1_BIAS_OVERRIDE_LOCATION+1] &= 0xF8;
                    icr1Data[VP880_ICR1_BIAS_OVERRIDE_LOCATION+1] |= 0x08;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR1_WRT,
                        VP880_ICR1_LEN, icr1Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR1 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr1Data[0], icr1Data[1], icr1Data[2], icr1Data[3]));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                        VP880_ICR2_LEN, icr2Data);
                    icr2Data[VP880_ICR2_SENSE_INDEX] |=
                        (VP880_ICR2_RING_SENSE | VP880_ICR2_TIP_SENSE |
                         VP880_ICR2_DAC_SENSE | VP880_ICR2_FEED_SENSE);
                    icr2Data[VP880_ICR2_SENSE_INDEX+1] &= ~VP880_ICR2_DAC_SENSE;
                    icr2Data[VP880_ICR2_SENSE_INDEX+1] |=
                        (VP880_ICR2_RING_SENSE | VP880_ICR2_TIP_SENSE |
                         VP880_ICR2_FEED_SENSE);
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                        VP880_ICR2_LEN, icr2Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR2 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr2Data[0], icr2Data[1], icr2Data[2], icr2Data[3]));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_RD,
                        VP880_ICR3_LEN, icr3Data);
                    icr3Data[VP880_ICR3_LINE_CTRL_INDEX] |= VP880_ICR3_LINE_CTRL;
                    icr3Data[VP880_ICR3_LINE_CTRL_INDEX+1] |= VP880_ICR3_LINE_CTRL;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT,
                        VP880_ICR3_LEN, icr3Data);
                    VP_DOUT(DBG_LINE_STATE, ("LP Enter: Writing ICR3 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr3Data[0], icr3Data[1], icr3Data[2], icr3Data[3]));

                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_RD,
                        VP880_ICR4_LEN, icr4Data);
                    icr4Data[VP880_ICR4_SUP_INDEX] |=
                        (VP880_ICR4_SUP_DAC_CTRL | VP880_ICR4_SUP_DET_CTRL
                        | VP880_ICR4_SUP_POL_CTRL);
                    icr4Data[VP880_ICR4_SUP_INDEX+1] |=
                        (VP880_ICR4_SUP_DAC_CTRL | VP880_ICR4_SUP_DET_CTRL
                        | VP880_ICR4_SUP_POL_CTRL);
                    icr4Data[VP880_ICR4_VOICE_DAC_INDEX] |= VP880_ICR4_VOICE_DAC_CTRL;
                    icr4Data[VP880_ICR4_VOICE_DAC_INDEX+1] |= VP880_ICR4_VOICE_DAC_CTRL;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR4_WRT,
                        VP880_ICR4_LEN, icr4Data);
                    VP_DOUT(DBG_LINE_STATE, (" -- Writing ICR4 0x%02X 0x%02X 0x%02X 0x%02X",
                        icr4Data[0], icr4Data[1], icr4Data[2], icr4Data[3]));

                    lineState[0] = VP880_SS_DISCONNECT;
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                        VP880_SYS_STATE_LEN, lineState);

                    VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
                        VP880_REGULATOR_PARAM_LEN, swParams);

                    if (pLineObj->lineState.currentState == VP_LINE_DISCONNECT) {
                        Vp880RunLPDisc(pLineCtx, TRUE, 0x00);
                    }

                    /* Need timer here to allow switcher to stabilize */
                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] =
                        MS_TO_TICKRATE(VP880_PWR_SWITCH_DEBOUNCE,
                            pDevObj->devProfileData.tickRate);

                    pDevObj->devTimer[VP_DEV_TIMER_LP_CHANGE] |=
                        VP_ACTIVATE_TIMER;
                }
            }
        }
    }
}

/**
 * Vp880ProcessFxoLine()
 *  This function should only be called by Vp880ServiceInterrupts on FXO lines.
 * It performs all line processing for operations that are Tick based
 *
 * Preconditions:
 *  Conditions defined by purpose of Api Tick.
 *
 * Postconditions:
 *  The Api variables and events (as appropriate) for the line passed have been
 * updated.
 */
bool
Vp880ProcessFxoLine(
    Vp880DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    bool retFlag = FALSE;
    VpCslacLineCondType tempRingingSt, tempDiscSt, tempPolRevSt, tempLiu;
    uint8 channelId = pLineObj->channelId, discMask = 0;
    VpLineStateType currentState = pLineObj->lineState.currentState;

    /*
     * Ignore the detector for a period after the last hook state change, or
     * a longer period after the last hook state change AND if the previous
     * line condition was Ringing
     */
    if ((pLineObj->lineTimers.timers.fxoTimer.lastStateChange
         < VP_FXO_STATE_CHANGE_DEBOUNCE) ||
        ((pLineObj->lineTimers.timers.fxoTimer.lastStateChange
         < VP_FXO_RING_TRIP_DEBOUNCE)
      && (pLineObj->lineState.condition & VP_CSLAC_RINGING))
#ifdef CSLAC_SEQ_EN
      || ((pLineObj->cadence.status & VP_CADENCE_STATUS_ACTIVE)
       && (pLineObj->intSequence[VP_PROFILE_TYPE_LSB]
            == VP_PRFWZ_PROFILE_MOMENTARY_LOOP_OPEN_INT))
#endif
            ) {
        /* Middle of detector Mask. Skip this process */
    } else {
        if ((pLineObj->termType == VP_TERM_FXO_DISC)
        && ((currentState == VP_LINE_FXO_TALK)
         || (currentState == VP_LINE_FXO_LOOP_CLOSE))) {
            discMask = pDevObj->intReg[channelId] & VP880_IO2_1_MASK;
        } else {
            discMask = pDevObj->intReg[channelId] & VP880_DISC1_MASK;
        }

        if (discMask) {
            tempDiscSt = VP_CSLAC_DISC;
        } else {
            tempDiscSt = 0;
        }

        if (pDevObj->intReg[channelId] & VP880_POL1_MASK) {
            tempPolRevSt = VP_CSLAC_POLREV;
        } else {
            tempPolRevSt = 0;
        }

        if (pLineObj->ringDetMax >= VP880_MAX_RING_DET_PERIOD) {
            Vp880LowRingFreqDetect(pLineCtx, &tempRingingSt, &tempPolRevSt,
                &retFlag);
        } else {
            if (pDevObj->intReg[channelId] & VP880_LIU1_MASK) {
                tempLiu = VP_CSLAC_LIU;
            } else {
                tempLiu = 0;
            }

            if (tempLiu != (pLineObj->lineState.condition & VP_CSLAC_LIU)) {
                pLineObj->lineState.condition &= ~VP_CSLAC_LIU;
                pLineObj->lineState.condition |= tempLiu;

                pLineObj->lineEventHandle = pDevObj->timeStamp;
                pLineObj->lineEvents.fxo |=
                    ((tempLiu) ? VP_LINE_EVID_LIU : VP_LINE_EVID_LNIU);
                retFlag = TRUE;
            }

            if (pDevObj->intReg[channelId] & VP880_RING1_DET_MASK) {
                tempRingingSt = VP_CSLAC_RINGING;
            } else {
                tempRingingSt = 0;
            }

            if (tempPolRevSt !=
                (pLineObj->lineState.condition & VP_CSLAC_POLREV)) {
                pLineObj->lineState.condition &= ~VP_CSLAC_POLREV;
                pLineObj->lineState.condition |= tempPolRevSt;
                pLineObj->lineState.condition &= ~VP_CSLAC_POLREV_REPORTED;

                if ((tempRingingSt != VP_CSLAC_RINGING)
                 && ((pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev / 4)
                    > pLineObj->ringDetMax)) {
                    pLineObj->lineEventHandle = pDevObj->timeStamp;
                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                    retFlag = TRUE;
                    pLineObj->preRingPolRev =
                        (pLineObj->lineState.condition & VP_CSLAC_POLREV) ?
                        VP_POLREV_REVERSE : VP_POLREV_NORMAL;
                }

                pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev = 0;

                if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                    pLineObj->fxoData = VP_POLREV_REVERSE;
                } else {
                    pLineObj->fxoData = VP_POLREV_NORMAL;
                }
            }
        }

        /*
         * Our cached state is inconsistent with recently detected conditions.
         * Generate the event.
         */
        if ((pLineObj->lineState.condition & VP_CSLAC_RINGING) != tempRingingSt) {
            pLineObj->lineEventHandle = pDevObj->timeStamp;

            if (tempRingingSt) {
                pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_ON;
                pLineObj->lineState.condition |= VP_CSLAC_RINGING;
            } else {
                pLineObj->lineEvents.fxo |= VP_LINE_EVID_RING_OFF;
                pLineObj->lineState.condition &= ~(VP_CSLAC_RINGING);

                if (pLineObj->preRingPolRev != pLineObj->fxoData) {
                    pLineObj->preRingPolRev = pLineObj->fxoData;
                    pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                }
            }
            retFlag = TRUE;
        }

         /* If the feed conditions changed, continue line processing */
        if((pLineObj->lineState.condition & VP_CSLAC_DISC) != tempDiscSt) {
            /*
             * Update actual line condition, even if not reporting an
             * event
             */
            if (tempDiscSt) {
                pLineObj->lineState.condition |= VP_CSLAC_DISC;
            } else {
                pLineObj->lineState.condition &= ~(VP_CSLAC_DISC);
            }

            /*
             * If line has been stable, update the pre-Disconnect value that
             * is used at end of timer to determine if event should be
             * generated.
             */
            if (pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce == 0) {
                pLineObj->preDisconnect = tempDiscSt;
            }

            /* Line status changed. Therefore, reset timer */
            pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce =
                VP_FXO_DISCONNECT_DEBOUNCE;

            /*
             * Immediate disconnect changes detected do not result in
             * API-II event. So retFlag remains as set previously.
             */
        } else {
            /*
             * If the disconnect signal came back to the current state, stop
             * the debounce count
             */
            if (pLineObj->termType == VP_TERM_FXO_DISC) {
                pLineObj->lineTimers.timers.fxoTimer.noCount = TRUE;
                pLineObj->lineTimers.timers.fxoTimer.fxoDiscIO2Change = 0;
            }
        }
    } /* end interrupt detect */
    return retFlag;
} /* end Vp880ProcessFxoLine */

/**
 * Vp880LowRingFreqDetect()
 *  This function should only be called by Vp880ProcessFxoLine. It performs
 * Ringing and PolRev detection on lines configured to detect ringing
 * frequencies below what the device can support.
 *
 * Preconditions:
 *  Conditions defined by purpose of Vp880ProcessFxoLine.
 *
 * Postconditions:
 *  The Api variables and events (as appropriate) for the line passed have been
 * updated. Return Flag passed is set to TRUE if an event is pending.
 */
void
Vp880LowRingFreqDetect(
    VpLineCtxType *pLineCtx,
    VpCslacLineCondType *tempRingingSt,
    VpCslacLineCondType *tempPolRevSt,
    bool *retFlag)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 channelId = pLineObj->channelId;
    uint16 polRevPeriod;

    if (*tempPolRevSt !=
        (pLineObj->lineState.condition & VP_CSLAC_POLREV)) {
        pLineObj->lineState.condition &= ~VP_CSLAC_POLREV;
        pLineObj->lineState.condition |= *tempPolRevSt;
        pLineObj->lineState.condition &= ~VP_CSLAC_POLREV_REPORTED;

        /*
         * Capture the period of the last two pol revs. Used for
         * Ringing Detection
         */
        pLineObj->lineTimers.timers.fxoTimer.prevHighToLowTime =
            ((pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev
            + pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev)
            / 4);

        pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev =
            pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev;
        pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev = 0;
    }

    if (pDevObj->intReg[channelId] & VP880_LIU1_MASK) {
        pLineObj->lineState.condition |= VP_CSLAC_LIU;
    } else {
        pLineObj->lineState.condition &= ~VP_CSLAC_LIU;
        pLineObj->lineTimers.timers.fxoTimer.lastNotLiu =
            pLineObj->ringDetMax * 2;
    }

    polRevPeriod =
        pLineObj->lineTimers.timers.fxoTimer.prevHighToLowTime;

    if ((pLineObj->lineTimers.timers.fxoTimer.timePrevPolRev / 4)
        < pLineObj->ringDetMax) {
        if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
            pLineObj->fxoData = VP_POLREV_REVERSE;
        } else {
            pLineObj->fxoData = VP_POLREV_NORMAL;
        }
    }

    /* Evaluate the detectors */
    /*
     * If the LIU Threshold has been exceeded, it's definitely not
     * PolRev, but may be ringing. If it has been completely
     * debounced, then Ringing is removed if we previously had
     * Ringing.
     */
    if (pLineObj->lineTimers.timers.fxoTimer.lastNotLiu) {
        /*
         * The threshold has been exceeded for a period within the
         * debounce interval. Check on Ringing condition.
         */

        if ((pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev / 4)
            > pLineObj->ringDetMax) {
            /*
             * This occurs because we had a recent LIU threshold,
             * but the frequency is not correct. No action other
             * than clearing Ringing state is necessary.
             */
            *tempRingingSt = 0;
            if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                pLineObj->fxoData = VP_POLREV_REVERSE;
            } else {
                pLineObj->fxoData = VP_POLREV_NORMAL;
            }
        } else if ((polRevPeriod <= pLineObj->ringDetMax)
                && (polRevPeriod >= pLineObj->ringDetMin)) {
            *tempRingingSt = VP_CSLAC_RINGING;
        } else {
            /*
             * This prevents compiler warning because it forces
             * the value to an initialized state
             */
            *tempRingingSt =
                (pLineObj->lineState.condition & VP_CSLAC_RINGING);
        }
    } else {
        *tempRingingSt = 0;

        /* We were not ringing, so process for polrev event */
        if (!(pLineObj->lineState.condition & VP_CSLAC_RINGING)) {
            /*
             * Require a 5ms delay (plus LIU 2ms the debounce time)
             * from previous polrev to occur before allowing it to
             * be detected as "Not Ringing". This gives time for
             * most ringing signals to exceed the LIU threshold.
             */
            if ((pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev / 4) >= 5) {
                if (!(pLineObj->lineState.condition & VP_CSLAC_POLREV_REPORTED)) {
                    pLineObj->lineState.condition |= VP_CSLAC_POLREV_REPORTED;

                    /*
                     * Based on how Ringing behaves, we could get out
                     * of sync w.r.t., PolRev. So don't report an event
                     * unless the PolRev changed.
                     */
                    if (pLineObj->lineState.condition & VP_CSLAC_POLREV) {
                        if (pLineObj->fxoData != VP_POLREV_REVERSE) {
                            pLineObj->fxoData = VP_POLREV_REVERSE;
                            pLineObj->lineEventHandle = pDevObj->timeStamp;
                            pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                            *retFlag = TRUE;
                            pLineObj->preRingPolRev = VP_POLREV_REVERSE;
                        }
                    } else {
                        if (pLineObj->fxoData != VP_POLREV_NORMAL) {
                            pLineObj->fxoData = VP_POLREV_NORMAL;
                            pLineObj->lineEventHandle = pDevObj->timeStamp;
                            pLineObj->lineEvents.fxo |= VP_LINE_EVID_POLREV;
                            pLineObj->preRingPolRev = VP_POLREV_NORMAL;
                            *retFlag = TRUE;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Vp880IsChnlUndrTst()
 *  This function determines if a particular line of a device is currently
 * running a test.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Device not affected. Return value TRUE if the line is currently running a
 * test, FALSE otherwise.
 */
static bool
Vp880IsChnlUndrTst(
    Vp880DeviceObjectType *pDevObj,
    uint8 channelId)
{
#ifdef VP880_INCLUDE_TESTLINE_CODE
    if ((TRUE == pDevObj->currentTest.prepared) &&
        (channelId == pDevObj->currentTest.channelId)) {
        return TRUE;
    }
#endif
    return FALSE;
}

/**
 * Vp880LowLevelCmd()
 *  This function provides direct MPI access to the line/device.
 *
 * Preconditions:
 *  The device associated with the line, and the line must first be initialized.
 *
 * Postconditions:
 *  The command data is passed over the MPI bus and affects only the line passed
 * if the command is line specific, and an event is generated.  If a read
 * command is performed, the user must read the results or flush events.  This
 * function returns the success code if the device is not already in a state
 * where the results must be read.
 */
VpStatusType
Vp880LowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle)
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    int i;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
        return VP_STATUS_DEVICE_BUSY;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    if(pCmdData[0] & 0x01) { /* Read Command */
        VpMpiCmd(deviceId, ecVal[channelId], pCmdData[0], len,
            &(pDevObj->mpiData[0]));
        pDevObj->mpiLen = len;
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_RX_CMP;
    } else {
        VpMpiCmd(deviceId, ecVal[channelId], pCmdData[0], len, &pCmdData[1]);
        for (i = 0; i < len; i++) {
            pDevObj->mpiData[i] = pCmdData[i];
        }
        pLineObj->lineEvents.response |= VP_LINE_EVID_LLCMD_TX_CMP;
    }
    pLineObj->lineEventHandle = handle;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return VP_STATUS_SUCCESS;
}

#endif
