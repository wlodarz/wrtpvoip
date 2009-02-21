/** \file apiquery.c
 * apiquery.c
 *
 *  This file contains the query functions used in the Vp880 device API.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#include "vp_api_cfg.h"

#if defined (VP_CC_880_SERIES)

/* Project Includes */
#include "vp_api_types.h"
#include "sys_service.h"
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp880_api.h"
#include "vp880_api_int.h"

#ifdef VP880_INCLUDE_TESTLINE_CODE
#include "vp_api_test.h"
#endif

/* Private Functions */
static uint16 Vp880CheckLineEvent(uint16 event, uint16 eventMask,
    VpEventCategoryType eventCat, Vp880LineObjectType *pLineObj);
static uint16 Vp880CheckDevEvent(uint16 event, uint16 eventMask,
    VpEventCategoryType eventCat, Vp880DeviceObjectType *pDevObj);

/**
 * Vp880FindSoftwareInterrupts()
 *  This function checks for active non-masked device and line events.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  Returns true if there is an active, non-masked event on either the device
 * or on a line associated with the device.
 */
bool
Vp880FindSoftwareInterrupts(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpLineCtxType *pLineCtx;
    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

    VpOptionEventMaskType eventsMask = pDevObj->deviceEventsMask;
    VpOptionEventMaskType *pEvents = &(pDevObj->deviceEvents);

    /* First clear all device events that are masked */
    pEvents->faults &= ~(eventsMask.faults);
    pEvents->signaling &= ~(eventsMask.signaling);
    pEvents->response &= ~(eventsMask.response);
    pEvents->process &= ~(eventsMask.process);
    pEvents->test &= ~(eventsMask.test);
    pEvents->fxo &= ~(eventsMask.fxo);

    /* Evaluate if any events remain */
    if((pEvents->faults) || (pEvents->signaling) || (pEvents->response)
    || (pEvents->process) || (pEvents->test) || (pEvents->fxo)) {
        return TRUE;
    }

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            eventsMask = pLineObj->lineEventsMask;
            pEvents = &(pLineObj->lineEvents);

            /* Clear the line events that are masked */
            pEvents->faults &= ~(eventsMask.faults);
            pEvents->signaling &= ~(eventsMask.signaling);
            pEvents->response &= ~(eventsMask.response);
            pEvents->process &= ~(eventsMask.process);
            pEvents->test &= ~(eventsMask.test);
            pEvents->fxo &= ~(eventsMask.fxo);

            /* Evaluate if any events remain */
            if(pEvents->faults || pEvents->signaling || pEvents->response
            || pEvents->process || pEvents->test || pEvents->fxo) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
 * Vp880GetEvent()
 *  This function reports new events that occured on the device. This function
 * returns one event for each call to it. It should be called repeatedly until
 * no more events are reported for a specific device.  This function does not
 * access the device, it returns status from the phantom registers that are
 * maintained by the API tick routine.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  Returns true if there is an active event for the device.
 */
bool
Vp880GetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent)    /**< Pointer to the results event structure */
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj;
    VpLineCtxType *pLineCtx;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 i, eventCatLoop;
    uint8 chan;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

#ifdef VP880_INCLUDE_TESTLINE_CODE
#define EVENT_ARRAY_SIZE 6
#else
#define EVENT_ARRAY_SIZE 5
#endif

    uint16 eventArray[EVENT_ARRAY_SIZE];
    uint16 eventMaskArray[EVENT_ARRAY_SIZE];
    VpEventCategoryType eventCat[] = {
        VP_EVCAT_FAULT,
        VP_EVCAT_SIGNALING,
        VP_EVCAT_RESPONSE,
        VP_EVCAT_PROCESS,
        VP_EVCAT_FXO,
        VP_EVCAT_TEST
    };

    pEvent->status = VP_STATUS_SUCCESS;
    pEvent->hasResults = FALSE;

    /* Initialize the arrays for device events */
    for (i = 0; i < EVENT_ARRAY_SIZE; i++) {
        switch(eventCat[i]) {
            case VP_EVCAT_FAULT:
                eventArray[i] = pDevObj->deviceEvents.faults;
                eventMaskArray[i] = pDevObj->deviceEventsMask.faults;
                break;

            case VP_EVCAT_SIGNALING:
                eventArray[i] = pDevObj->deviceEvents.signaling;
                eventMaskArray[i] = pDevObj->deviceEventsMask.signaling;
                break;

            case VP_EVCAT_RESPONSE:
                eventArray[i] = pDevObj->deviceEvents.response;
                eventMaskArray[i] = pDevObj->deviceEventsMask.response;
                break;

            case VP_EVCAT_PROCESS:
                eventArray[i] = pDevObj->deviceEvents.process;
                eventMaskArray[i] = pDevObj->deviceEventsMask.process;
                break;

            case VP_EVCAT_FXO:
                eventArray[i] = pDevObj->deviceEvents.fxo;
                eventMaskArray[i] = pDevObj->deviceEventsMask.fxo;
                break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
            case VP_EVCAT_TEST:
                eventArray[i] = pDevObj->deviceEvents.test;
                eventMaskArray[i] = pDevObj->deviceEventsMask.test;
                break;
#endif

            default:
                /* This can only occur if there's a bug in this code */
                break;
        }
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Look for active device events first */
    for (eventCatLoop = 0; eventCatLoop < EVENT_ARRAY_SIZE; eventCatLoop++) {
        pEvent->eventId = Vp880CheckDevEvent(eventArray[eventCatLoop],
            eventMaskArray[eventCatLoop], eventCat[eventCatLoop], pDevObj);
        if (pEvent->eventId != 0x0000) {
            pEvent->deviceId = deviceId;
            pEvent->channelId = 0;
            pEvent->eventCategory = eventCat[eventCatLoop];
            pEvent->pDevCtx = pDevCtx;
            pEvent->pLineCtx = VP_NULL;
            pEvent->parmHandle = pDevObj->eventHandle;
            pEvent->hasResults = FALSE;

            if (pEvent->eventCategory == VP_EVCAT_RESPONSE) {
                /*
                 * For the events that require a read operation, set the has
                 * results indicator in the event structure
                 */

                switch (pEvent->eventId) {
                    case VP_LINE_EVID_RD_OPTION:
                        pEvent->channelId = pDevObj->getResultsOption.chanId;
                        pEvent->pLineCtx = pDevCtx->pLineCtx[pEvent->channelId];
                        if (pEvent->pLineCtx != VP_NULL) {
                            Vp880LineObjectType *pLineObj = pEvent->pLineCtx->pLineObj;
                            pEvent->lineId = pLineObj->lineId;
                        }
                        pEvent->hasResults = TRUE;
                        pEvent->eventData = pDevObj->getResultsOption.optionType;
                        break;

                    case VP_DEV_EVID_IO_ACCESS_CMP:
                        pEvent->hasResults = TRUE;
                        break;

                    case VP_DEV_EVID_DEV_INIT_CMP:
                        pEvent->eventData = 1;
                        break;

                    case VP_EVID_CAL_CMP:
                        pEvent->eventData = pDevObj->responseData;
                        break;

                    default:
                        break;
                }
            }

            if (pEvent->eventCategory == VP_EVCAT_FAULT) {
                switch(pEvent->eventId) {
                    case VP_DEV_EVID_CLK_FLT:
                        pEvent->eventData =
                            (pDevObj->dynamicInfo.clkFault ? TRUE : FALSE);
                        break;

                    case VP_DEV_EVID_BAT_FLT:
                        if ((pDevObj->dynamicInfo.bat1Fault == TRUE)
                         || (pDevObj->dynamicInfo.bat2Fault == TRUE)
                         || (pDevObj->dynamicInfo.bat3Fault == TRUE)) {
                            pEvent->eventData = TRUE;
                        } else {
                            pEvent->eventData = FALSE;
                        }
                        break;

                    default:
                        break;
                }
            }
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return TRUE;
        }
    }

    /*
     * No device events, now look for Line events -- but make sure the line
     * context is valid before looking for a line object
     */
    for(chan = pDevObj->dynamicInfo.lastChan; chan < maxChan; chan++) {
        pLineCtx = pDevCtx->pLineCtx[chan];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;
            /* The line context is valid, create a line object and initialize
             * the event arrays for this line
             */
            for (i = 0; i < EVENT_ARRAY_SIZE; i++) {
                switch(eventCat[i]) {
                    case VP_EVCAT_FAULT:
                        eventArray[i] = pLineObj->lineEvents.faults;
                        eventMaskArray[i] = pLineObj->lineEventsMask.faults;
                        break;

                    case VP_EVCAT_SIGNALING:
                        eventArray[i] = pLineObj->lineEvents.signaling;
                        eventMaskArray[i] = pLineObj->lineEventsMask.signaling;
                        break;

                    case VP_EVCAT_RESPONSE:
                        eventArray[i] = pLineObj->lineEvents.response;
                        eventMaskArray[i] = pLineObj->lineEventsMask.response;
                        break;

                    case VP_EVCAT_PROCESS:
                        eventArray[i] = pLineObj->lineEvents.process;
                        eventMaskArray[i] = pLineObj->lineEventsMask.process;
                        break;

                    case VP_EVCAT_FXO:
                        eventArray[i] = pLineObj->lineEvents.fxo;
                        eventMaskArray[i] = pLineObj->lineEventsMask.fxo;
                        break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                    case VP_EVCAT_TEST:
                        eventArray[i] = pLineObj->lineEvents.test;
                        eventMaskArray[i] = pLineObj->lineEventsMask.test;
                        break;
#endif
                    default:
                        /* This can only occur if there's a bug in this code */
                        break;

                }
            }

            /* Check this line events */
            for (eventCatLoop = 0;
                 eventCatLoop < EVENT_ARRAY_SIZE;
                 eventCatLoop++) {
                pEvent->eventId = Vp880CheckLineEvent(eventArray[eventCatLoop],
                    eventMaskArray[eventCatLoop], eventCat[eventCatLoop],
                    pLineObj);

                if (pEvent->eventId != 0x0000) {
                    pEvent->deviceId = deviceId;
                    pEvent->channelId = chan;
                    pEvent->pLineCtx = pDevCtx->pLineCtx[chan];
                    pEvent->pDevCtx = pDevCtx;
                    pEvent->eventCategory = eventCat[eventCatLoop];
                    pEvent->parmHandle = pLineObj->lineEventHandle;
                    pEvent->lineId = pLineObj->lineId;

                    switch(pEvent->eventCategory) {
                        case VP_EVCAT_RESPONSE:
                            pEvent->eventData = pLineObj->responseData;
                            switch(pEvent->eventId) {
                                case VP_LINE_EVID_LLCMD_RX_CMP:
                                case VP_LINE_EVID_GAIN_CMP:
                                case VP_LINE_EVID_RD_LOOP:
                                    pEvent->hasResults = TRUE;
                                    break;
                                default:
                                    break;
                            }
                            break;

                        case VP_EVCAT_SIGNALING:
                            if (pEvent->eventId == VP_LINE_EVID_DTMF_DIG) {
                                /*
                                 * Upper bits are used for the timestamp.
                                 * Lower bits are used for the digit and the
                                 * make/break bit.
                                 */
                                pEvent->eventData = (pDevObj->timeStamp << 5)
                                    | pLineObj->dtmfDigitSense;
                            } else {
                                pEvent->eventData = pLineObj->signalingData;
                            }
                            break;

                        case VP_EVCAT_FXO:
                            pEvent->eventData = pLineObj->fxoData;
                            break;

                        case VP_EVCAT_PROCESS:
                            pEvent->eventData = pLineObj->processData;
                            break;

                        case VP_EVCAT_FAULT:
                            if (pEvent->eventId == VP_LINE_EVID_THERM_FLT) {
                                pEvent->eventData =
                                    (pLineObj->lineState.condition
                                        & VP_CSLAC_THERM_FLT) ? TRUE : FALSE;
                            }
                            break;

                        case VP_EVCAT_TEST:
                            if ( VP_LINE_EVID_TEST_CMP == pEvent->eventId) {
                                pEvent->eventData = pDevObj->testResults.testId;
                                pEvent->hasResults = TRUE;
                            }
                            break;

                        default:
                            break;
                    }

                    /*
                     * We're returning, so update the device last channel that
                     * was checked so we start at the next channel
                     */
                    pDevObj->dynamicInfo.lastChan = chan + 1;
                    if (pDevObj->dynamicInfo.lastChan >= maxChan) {
                        pDevObj->dynamicInfo.lastChan = 0;
                    }
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return TRUE;
                }
            }
        }
    }
    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return FALSE;
} /* End Vp880GetEvent */

/**
 * Vp880CheckDevEvent()
 *  This function performs a check on active device events and compares the
 * event with the event mask.  The event is cleared, and if the event is
 * unmasked it gets returned to the calling function via the return value.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *  If the returned value is other than 0x0000, the event being returned is
 * cleared in the device object.
 */
uint16
Vp880CheckDevEvent(
    uint16 event,
    uint16 eventMask,
    VpEventCategoryType eventCat,
    Vp880DeviceObjectType *pDevObj)
{
    uint8 i;
    uint16 mask;

    for (i = 0, mask = 0x0001; i < 16; i++, (mask = mask << 1)) {
        /* Check to see if an event MAY be reported */
        if ((mask & event) != 0) {
            /*
             * Have to clear the device event so we don't report this event
             * again
             */
            switch(eventCat) {
                case VP_EVCAT_FAULT:
                    pDevObj->deviceEvents.faults &= (~mask);
                    break;

                case VP_EVCAT_SIGNALING:
                    pDevObj->deviceEvents.signaling &= (~mask);
                    break;

                case VP_EVCAT_RESPONSE:
                    pDevObj->deviceEvents.response &= (~mask);
                    break;

                case VP_EVCAT_PROCESS:
                    pDevObj->deviceEvents.process &= (~mask);
                    break;

                case VP_EVCAT_FXO:
                    pDevObj->deviceEvents.fxo &= (~mask);
                    break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                case VP_EVCAT_TEST:
                    pDevObj->deviceEvents.test &= (~mask);
                    break;
#endif
                default:
                    break;
            }

            /* If the event is not masked, return the event */
            if ((mask & eventMask) == 0) {
                return mask;
            }
        }
    }

    return 0x0000;
}

/**
 * Vp880CheckLineEvent()
 *  This function performs a check on active line events and compares the
 * event with the event mask.  The event is cleared, and if the event is
 * unmasked it gets returned to the calling function via the return value.
 *
 * Preconditions:
 *  None. This is an internal API function call only and it is assumed all error
 * checking necessary is performed by higher level functions.
 *
 * Postconditions:
 *  If the returned value is other than 0x0000, the event being returned is
 * cleared in the line object.
 */
uint16
Vp880CheckLineEvent(
    uint16 event,
    uint16 eventMask,
    VpEventCategoryType eventCat,
    Vp880LineObjectType *pLineObj)
{
    uint8 i;
    uint16 mask;

    for (i = 0, mask = 0x0001; i < 16; i++, (mask = mask << 1)) {
        /* Check to see if an event MAY be reported */
        if ((mask & event) != 0) {
            /*
             * Have to clear the line event so we don't report this event
             * again
             */
            switch(eventCat) {
                case VP_EVCAT_FAULT:
                    pLineObj->lineEvents.faults &= (~mask);
                    break;

                case VP_EVCAT_SIGNALING:
                    pLineObj->lineEvents.signaling &= (~mask);
                    break;

                case VP_EVCAT_RESPONSE:
                    pLineObj->lineEvents.response &= (~mask);
                    break;

                case VP_EVCAT_PROCESS:
                    pLineObj->lineEvents.process &= (~mask);
                    break;

                case VP_EVCAT_FXO:
                    pLineObj->lineEvents.fxo &= (~mask);
                    break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
               case VP_EVCAT_TEST:
                    pLineObj->lineEvents.test &= (~mask);
                    break;
#endif

                default:
                    break;
            }

            /* If the event is not masked, return the event */
            if ((mask & eventMask) == 0) {
                return mask;
            }
        }
    }
    return 0x0000;
}

/**
 * Vp880GetOption()
 *  This function accesses the option being requested, fills the device object
 * with the data to be returned, and sets the Read Option complete event.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  The device object is filled with the results of the option type being
 * requested and the Read Option Event flag is set.  This function returns the
 * success code if the option type being requested is supported.
 */
VpStatusType
Vp880GetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle)
{
    VpDevCtxType *pDevCtxLocal;
    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj;
    VpStatusType status = VP_STATUS_SUCCESS;
    VpGetResultsOptionsDataType *pOptionData;

    uint8 channelId, txSlot, rxSlot;
    VpDeviceIdType deviceId;
    uint8 tempLoopBack[VP880_LOOPBACK_LEN];
    uint8 tempSysConfig, maxChan;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 ioDirection[2] = {0x00, 0x00};

    VpLineCtxType *pLineCtxLocal;

    if (pLineCtx != VP_NULL) {
        pDevCtxLocal = pLineCtx->pDevCtx;
        pDevObj = pDevCtxLocal->pDevObj;
        deviceId = pDevObj->deviceId;
        pLineObj = pLineCtx->pLineObj;
        channelId = pLineObj->channelId;
        pOptionData = &(pDevObj->getResultsOption.optionData);

        /* Adjust the EC value for Wideband mode as needed */
        ecVal[channelId] |=
           ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

        if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
            return VP_STATUS_DEVICE_BUSY;
        }

        /*
         * If this function can be executed, we will either access the MPI
         * and/or shared data. So it is best to label the entire function as
         * Code Critical so the data being accessed cannot be changed while
         * trying to be accessed
         */
        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

        pDevObj->getResultsOption.chanId = channelId;

        switch (option) {
            /* Line Options */
            case VP_OPTION_ID_PULSE_MODE:
                pOptionData->pulseModeOption = pLineObj->pulseMode;
                break;

            case VP_OPTION_ID_TIMESLOT:
                VpMpiCmd(deviceId, ecVal[channelId], VP880_TX_TS_RD,
                    VP880_TX_TS_LEN, &txSlot);

                VpMpiCmd(deviceId, ecVal[channelId], VP880_RX_TS_RD,
                    VP880_RX_TS_LEN, &rxSlot);

                pOptionData->timeSlotOption.tx = (txSlot & VP880_TX_TS_MASK);

                pOptionData->timeSlotOption.rx = (rxSlot & VP880_RX_TS_MASK);
                break;

            case VP_OPTION_ID_CODEC:
                pOptionData->codecOption = pLineObj->codec;
                break;

            case VP_OPTION_ID_PCM_HWY:
                pOptionData->pcmHwyOption = VP_OPTION_HWY_A;
                break;

            case VP_OPTION_ID_LOOPBACK:
                /* Timeslot loopback via loopback register */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOPBACK_RD,
                    VP880_LOOPBACK_LEN, tempLoopBack);

                if ((tempLoopBack[0] & VP880_INTERFACE_LOOPBACK_EN) ==
                     VP880_INTERFACE_LOOPBACK_EN) {
                    pOptionData->loopBackOption = VP_OPTION_LB_TIMESLOT;
                } else {
                    pOptionData->loopBackOption = VP_OPTION_LB_OFF;
                }
                break;

            case VP_OPTION_ID_LINE_STATE:
                /* Battery control is automatic, so force it */
                pOptionData->lineStateOption.bat = VP_OPTION_BAT_AUTO;

                /* Smooth/Abrupt PolRev is controlled in the device */
                VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                    VP880_SS_CONFIG_LEN, &tempSysConfig);

                if (tempSysConfig & VP880_SMOOTH_PR_EN) {
                    pOptionData->lineStateOption.battRev = FALSE;
                } else {
                    pOptionData->lineStateOption.battRev = TRUE;
                }
                break;

            case VP_OPTION_ID_EVENT_MASK:
                /*
                 * In SetOption(), we force all line-specific bits in the
                 * deviceEventsMask to zero.  Likewise, we force all device-
                 * specific bits in the lineEventsMask to zero.  This allows
                 * us to simply OR the two together here.
                 */
                pOptionData->eventMaskOption.faults =
                    pLineObj->lineEventsMask.faults |
                    pDevObj->deviceEventsMask.faults;
                pOptionData->eventMaskOption.signaling =
                    pLineObj->lineEventsMask.signaling |
                    pDevObj->deviceEventsMask.signaling;
                pOptionData->eventMaskOption.response =
                    pLineObj->lineEventsMask.response |
                    pDevObj->deviceEventsMask.response;
                pOptionData->eventMaskOption.test =
                    pLineObj->lineEventsMask.test |
                    pDevObj->deviceEventsMask.test;
                pOptionData->eventMaskOption.process =
                    pLineObj->lineEventsMask.process |
                    pDevObj->deviceEventsMask.process;
                pOptionData->eventMaskOption.fxo =
                    pLineObj->lineEventsMask.fxo |
                    pDevObj->deviceEventsMask.fxo;
                pOptionData->eventMaskOption.packet =
                    pLineObj->lineEventsMask.packet |
                    pDevObj->deviceEventsMask.packet;
                break;

            case VP_OPTION_ID_ZERO_CROSS:
                pOptionData->zeroCross = pLineObj->ringCtrl.zeroCross;
                break;

            case VP_OPTION_ID_RING_CNTRL:
                pOptionData->ringControlOption = pLineObj->ringCtrl;
                break;

            case VP_OPTION_ID_PCM_TXRX_CNTRL:
                pOptionData->pcmTxRxCtrl = pLineObj->pcmTxRxCtrl;
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    } else {
        /*
         * Upper layer checks to be sure that either device context or line
         * context pointers are not null -- so the device context is not null
         * in this case.
         */
        pDevObj = pDevCtx->pDevObj;
        deviceId = pDevObj->deviceId;
        pOptionData = &(pDevObj->getResultsOption.optionData);
        maxChan = pDevObj->staticInfo.maxChannels;

        if (pDevObj->deviceEvents.response & VP880_READ_RESPONSE_MASK) {
            return VP_STATUS_DEVICE_BUSY;
        }

        VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

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
                pOptionData->pulseTypeOption = pDevObj->pulseSpecs;
                break;

            case VP_DEVICE_OPTION_ID_PULSE2:
                pOptionData->pulseTypeOption = pDevObj->pulseSpecs2;
                break;

            case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                pOptionData->criticalFaultOption = pDevObj->criticalFault;
                break;

            case VP_OPTION_ID_EVENT_MASK:
                pOptionData->eventMaskOption = pDevObj->deviceEventsMask;
                break;

            case VP_DEVICE_OPTION_ID_DEVICE_IO:
                /* Get the current device IO control information */
                for (channelId = 0; channelId < maxChan; channelId++) {
                    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
                        VP880_IODIR_REG_LEN, &ioDirection[channelId]);
                }

                pOptionData->deviceIo.outputTypePins_63_32 = 0x00000000;
                pOptionData->deviceIo.directionPins_63_32 = 0x00000000;

                /*
                 * I/O 1 on each channel is special because it uses two bits
                 * in the IO direction register to setup I/O and Output mode
                 */

                if (ioDirection[0] & VP880_IODIR_IO1_OPEN_DRAIN) {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000001;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000001;
                } else if (ioDirection[0] & VP880_IODIR_IO1_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000000;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000001;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 = 0x00000000;
                    pOptionData->deviceIo.directionPins_31_0 = 0x00000000;
                }

                /* I/O 2 on each channel can only be either input or output */
                if (ioDirection[0] & VP880_IODIR_IO2_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000004);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000004;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000004);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000004);
                }

                if (ioDirection[1] & VP880_IODIR_IO1_OPEN_DRAIN) {
                    pOptionData->deviceIo.outputTypePins_31_0 |= 0x00000002;
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000002;
                } else if (ioDirection[1] & VP880_IODIR_IO1_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000002);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000002;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000002);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000002);
                }

                if (ioDirection[1] & VP880_IODIR_IO2_OUTPUT) {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000008);
                    pOptionData->deviceIo.directionPins_31_0 |= 0x00000008;
                } else {
                    pOptionData->deviceIo.outputTypePins_31_0 &= ~(0x00000008);
                    pOptionData->deviceIo.directionPins_31_0 &= ~(0x00000008);
                }
                break;

            default:
                status = VP_STATUS_OPTION_NOT_SUPPORTED;
                break;
        }
    }

    if (status == VP_STATUS_SUCCESS) {
        pDevObj->getResultsOption.optionType = option;
        pDevObj->deviceEvents.response |= VP_LINE_EVID_RD_OPTION;
        pDevObj->eventHandle = handle;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880GetDeviceStatus()
 *  This function returns the status of all lines on a device for the type being
 * requested.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  The location pointed to by the uint32 pointer passed is set (on a per line
 * basis) to either '1' if the status if TRUE on the given line, or '0' if the
 * status is FALSE on the given line for the status being requested.
 */
VpStatusType
Vp880GetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    uint8 channelId;
    bool status = FALSE;
    VpLineCtxType *pLineCtx;

    *pDeviceStatus = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];

        if(pLineCtx != VP_NULL) {
            VpCSLACGetLineStatus(pLineCtx, input, &status);
        } else {
            status = FALSE;
        }
        *pDeviceStatus |= (((status == TRUE) ? 1 : 0) << channelId);
    }
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880FlushEvents()
 *  This function clears out all events on the device and all events on all
 * lines associated with the device passed.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  All active device events are cleared, and all active line events associated
 * with this device are cleared.
 */
VpStatusType
Vp880FlushEvents(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;
    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    pDevObj->deviceEvents.faults = 0;
    pDevObj->deviceEvents.signaling = 0;
    pDevObj->deviceEvents.response = 0;
    pDevObj->deviceEvents.test = 0;
    pDevObj->deviceEvents.process = 0;
    pDevObj->deviceEvents.fxo = 0;
    pDevObj->deviceEvents.packet = 0;

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if(pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            pLineObj->lineEvents.faults = 0;
            pLineObj->lineEvents.signaling = 0;
            pLineObj->lineEvents.response = 0;
            pLineObj->lineEvents.test = 0;
            pLineObj->lineEvents.process = 0;
            pLineObj->lineEvents.fxo = 0;
            pLineObj->lineEvents.packet = 0;
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880GetResults()
 *  This function fills the results structure passed with the results data found
 * from the event that caused new results.
 *
 * Preconditions:
 *  None. All error checking required is assumed to exist in common interface
 * file.
 *
 * Postconditions:
 *  If the event structure passed provides the event catagory and ID for a valid
 * results type to read, then the structure passed is filled with the results
 * data.  This function returns the success code if the event catagory and ID is
 * supported by the device.
 */
VpStatusType
Vp880GetResults(
    VpEventType *pEvent,
    void *pResults)
{
    VpDevCtxType *pDevCtx = pEvent->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;
    uint8 commandByte;
    uint8 *pMpiData;
    uint8 mpiDataLen = pDevObj->mpiLen;

    VpGetResultsOptionsDataType *pOptionData =
        &(pDevObj->getResultsOption.optionData);

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    switch(pEvent->eventCategory) {
        case VP_EVCAT_RESPONSE:
            switch (pEvent->eventId) {
                case VP_LINE_EVID_LLCMD_RX_CMP:
                    pMpiData = (uint8 *)pResults;
                    for (commandByte = 0;
                         commandByte < mpiDataLen;
                         commandByte++) {
                        pMpiData[commandByte] = pDevObj->mpiData[commandByte];
                    }
                    break;

                case VP_LINE_EVID_GAIN_CMP:
                    *(VpRelGainResultsType *)pResults =
                        pDevObj->relGainResults;
                    break;

                case VP_DEV_EVID_IO_ACCESS_CMP:
                    *((VpDeviceIoAccessDataType *)pResults) =
                        pOptionData->deviceIoData;
                    break;

                case VP_LINE_EVID_RD_OPTION:
                    switch(pDevObj->getResultsOption.optionType) {
                        case VP_DEVICE_OPTION_ID_PULSE:
                            *(VpOptionPulseType *)pResults =
                                pDevObj->pulseSpecs;
                            break;

                        case VP_DEVICE_OPTION_ID_PULSE2:
                            *(VpOptionPulseType *)pResults =
                                pDevObj->pulseSpecs2;
                            break;

                        case VP_DEVICE_OPTION_ID_CRITICAL_FLT:
                            *(VpOptionCriticalFltType *)pResults =
                                pOptionData->criticalFaultOption;
                            break;

                        case VP_DEVICE_OPTION_ID_DEVICE_IO:
                            *(VpOptionDeviceIoType *)pResults =
                                pOptionData->deviceIo;
                            break;

                        case VP_OPTION_ID_RING_CNTRL:
                            *(VpOptionRingControlType *)pResults =
                                pOptionData->ringControlOption;
                            break;

                        case VP_OPTION_ID_ZERO_CROSS:
                            *(VpOptionZeroCrossType *)pResults =
                                pOptionData->zeroCross;
                            break;

                        case VP_OPTION_ID_PULSE_MODE:
                            *((VpOptionPulseModeType *)pResults) =
                                pOptionData->pulseModeOption;
                            break;

                        case VP_OPTION_ID_TIMESLOT:
                            *(VpOptionTimeslotType *)pResults =
                                pOptionData->timeSlotOption;
                            break;

                        case VP_OPTION_ID_CODEC:
                            *((VpOptionCodecType *)pResults) =
                                pOptionData->codecOption;
                            break;

                        case VP_OPTION_ID_PCM_HWY:
                            *((VpOptionCodecType *)pResults) =
                                pOptionData->pcmHwyOption;
                            break;

                        case VP_OPTION_ID_LOOPBACK:
                            *((VpOptionLoopbackType *)pResults) =
                                pOptionData->loopBackOption;
                            break;

                        case VP_OPTION_ID_LINE_STATE:
                            *((VpOptionLineStateType *)pResults) =
                                pOptionData->lineStateOption;
                            break;

                        case VP_OPTION_ID_EVENT_MASK:
                            *((VpOptionEventMaskType *)pResults) =
                                pOptionData->eventMaskOption;
                            break;

                        case VP_OPTION_ID_PCM_TXRX_CNTRL:
                            *((VpOptionPcmTxRxCntrlType *)pResults) =
                                pOptionData->pcmTxRxCtrl;
                            break;

                        default:
                            status = VP_STATUS_INVALID_ARG;
                            break;
                    }
                    break;

                default:
                    status = VP_STATUS_INVALID_ARG;
                    break;
            }
            break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
        case VP_EVCAT_TEST:
            switch (pEvent->eventId) {
                case VP_LINE_EVID_TEST_CMP:
                    *((VpTestResultType *)pResults) = pDevObj->testResults;
                    break;

                 default:
                    break;
            }
#endif
            break;
        default:
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
}

/**
 * Vp880ServiceTimers()
 *  This function services active API timers on all channels of deviceId.
 *
 * Preconditions:
 *  This Function must be called from the ApiTick function once per device.
 *
 * Postconditions:
 *  All Active Timers have been serviced.
 */
bool
Vp880ServiceTimers(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880LineObjectType *pLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    uint16 tempTimer;
    VpLineTimerType lineTimerType;
    VpDevTimerType devTimerType;

    uint8 channelId;
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    bool retFlag = FALSE;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 icr3Values[VP880_ICR3_LEN];
    uint8 icr4Values[VP880_ICR4_LEN];
    uint16 tickAdder =
        pDevObj->devProfileData.tickRate / (VP_CSLAC_TICKSTEP_0_5MS / 2);

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};

    for (channelId = 0; channelId < maxChan; channelId++) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND) ?
               VP880_WIDEBAND_MODE : 0);
        }
    }

    for (devTimerType = 0; devTimerType < VP_DEV_TIMER_LAST; devTimerType++) {

        if (pDevObj->devTimer[devTimerType] & VP_ACTIVATE_TIMER) {

            /* get the bits associated with the timer into a temp varaible*/
            tempTimer = (pDevObj->devTimer[devTimerType] & VP_TIMER_TIME_MASK);

            /* decrement the timer */
            if (tempTimer > 0) {
                tempTimer--;
            }

            /* reset the device timer to the new value*/
            pDevObj->devTimer[devTimerType] = tempTimer;

            /* if the timer has expired then run the timer code */
            if (pDevObj->devTimer[devTimerType] == 0) {

                VP_DOUT(DBG_LINE_STATE, ("Device timer %d Expired at %d",
                    devTimerType, pDevObj->timeStamp));

                switch(devTimerType) {
                    case VP_DEV_TIMER_CLKFAIL:
                        retFlag = TRUE;
                        break;

                    case VP_DEV_TIMER_LP_CHANGE: {
                        uint8 channelId;
                        Vp880LineObjectType *pLineObj;
                        VpLineCtxType *pLineCtx;

                        VP_DOUT(DBG_HOOK, ("Signaling 0x%02X 0x%02X",
                            pDevObj->intReg[0], pDevObj->intReg[1]));
                        for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
                            pLineCtx = pDevCtx->pLineCtx[channelId];
                            if (pLineCtx != VP_NULL) {
                                bool failureMode = FALSE;
                                pLineObj = pLineCtx->pLineObj;

                                VP_DOUT(DBG_HOOK, ("Last Hook State on line %d = %d LP Mode %d",
                                    channelId, (pLineObj->lineState.condition & VP_CSLAC_HOOK),
                                    (pLineObj->status & VP880_LOW_POWER_EN)));

                                if (pLineObj->lineState.currentState != VP_LINE_DISCONNECT) {
                                    if (pLineObj->status & VP880_LOW_POWER_EN) {
                                        /*
                                         * If we're in LP Mode, then the line should be
                                         * detecting on-hook. All other conditions mean
                                         * there could be a leaky line.
                                         */
                                        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                          && (!(pDevObj->intReg[channelId]) & VP880_HOOK1_MASK)) {
                                            failureMode = TRUE;
                                        }
                                    } else {
                                        /*
                                         * If we're not in LP Mode, then the line should be
                                         * detecting off-hook and the signaling bit should
                                         * be high. Otherwise, error.
                                         */

                                        if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                          && (!(pDevObj->intReg[channelId]) & VP880_HOOK1_MASK)) {
                                            failureMode = TRUE;
                                         }
                                    }
                                }

                                /*
                                 * If the line was last seen off-hook and is now on-hook as a result
                                 * of exiting LP Mode, it could be a leaky line.
                                 */
                                if (failureMode == TRUE) {
                                    if (pLineObj->leakyLineCnt >= 3) {
                                        VP_DOUT(DBG_HOOK, ("Flag Channel %d for Leaky Line at time %d Signaling 0x%02X LineState %d",
                                            channelId, pDevObj->timeStamp, (pDevObj->intReg[channelId] & VP880_HOOK1_MASK),
                                            pLineObj->lineState.usrCurrent));

                                        pLineObj->status |= VP880_LINE_LEAK;
                                        pDevObj->stateInt &=
                                            ((channelId == 0) ? ~VP880_LINE0_LP : ~VP880_LINE1_LP);
                                        pLineObj->lineEvents.faults |= VP_LINE_EVID_RES_LEAK_FLT;
                                    } else {
                                        VP_DOUT(DBG_HOOK, ("Potential Leaky Line %d at time %d  ...retry",
                                            channelId, pDevObj->timeStamp));

                                        pLineObj->leakyLineCnt++;
                                    }

                                    /* Update the line state */
                                    for (channelId = 0;
                                         channelId < pDevObj->staticInfo.maxChannels;
                                         channelId++) {
                                        Vp880LineObjectType *pLineObjInt;

                                        pLineCtx = pDevCtx->pLineCtx[channelId];
                                        if (pLineCtx != VP_NULL) {
                                            pLineObjInt = pLineCtx->pLineObj;
                                            VP_DOUT(DBG_HOOK, ("1. Channel %d Current Linestate %d Current User Linestate %d",
                                                channelId, pLineObjInt->lineState.currentState, pLineObjInt->lineState.usrCurrent));

                                            Vp880SetLineStateInt(pLineCtx, pLineObjInt->lineState.usrCurrent);
                                        }
                                    }
                                } else if ((pLineObj->lineState.condition & VP_CSLAC_HOOK)
                                        && (pDevObj->intReg[channelId]) & VP880_HOOK1_MASK) {

                                    if ((pLineObj->lineState.condition & VP_CSLAC_HOOK) &&
                                        (pLineObj->status & VP880_LOW_POWER_EN)) {
                                        /* The line is on-hook */
                                        pLineObj->lineState.condition &= ~VP_CSLAC_HOOK;
                                    } else {
                                        /* Valid off-hook */
                                        VP_DOUT(DBG_HOOK, ("Valid Off-Hook on line %d at time %d",
                                            channelId, pDevObj->timeStamp));

                                        pLineObj->leakyLineCnt = 0;
                                        pLineObj->status &= ~VP880_LINE_LEAK;

                                        if(pLineObj->pulseMode != VP_OPTION_PULSE_DECODE_OFF) {
                                            pLineObj->dpStruct.hookSt = TRUE;
                                            pLineObj->dpStruct.state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                                            pLineObj->dpStruct.lc_time = 0;

                                            pLineObj->dpStruct2.hookSt = TRUE;
                                            pLineObj->dpStruct2.state = VP_DP_DETECT_STATE_LOOP_CLOSE;
                                            pLineObj->dpStruct2.lc_time = 0;
                                        }

                                        pLineObj->lineEvents.signaling |=
                                            VP_LINE_EVID_HOOK_OFF;

                                        pLineObj->lineState.condition |= VP_CSLAC_HOOK;
                                        pLineObj->lineEventHandle = pDevObj->timeStamp;
                                    }
                                }
                            }
                        }
                        }
                        break;

#ifdef VP880_INCLUDE_TESTLINE_CODE
                    case VP_DEV_TIMER_TESTLINE:
                        {
                            const void *pTestArgs =
                                (const void*)&pDevObj->currentTest.pTestHeap->testArgs;
                            uint8 testChanId = pDevObj->currentTest.channelId;
                            VpTestLineIntFuncPtrType testline =
                                pDevCtx->funPtrsToApiFuncs.TestLineInt;
                            if ((testline != VP_NULL)
                              && (pDevObj->currentTest.testState != -1)) {
                                /*
                                 * if the TestLineInt function exists and the
                                 * current test state has not been set back to
                                 * -1 by test conclude before the timer expired
                                 * then run the call back
                                 */

                                testline(
                                    pDevCtx->pLineCtx[testChanId],
                                    pDevObj->currentTest.testId,
                                    pTestArgs,
                                    pDevObj->currentTest.handle,
                                    TRUE);
                            }
                        }
                        break;

#endif /* VP880_INCLUDE_TESTLINE_CODE */

                    case VP_DEV_TIMER_ABV_CAL:
                        if (pDevObj->stateInt & VP880_IS_ABS) {
                            Vp880CalAbvAbsDev(pDevCtx);
                        } else {
                            Vp880CalAbv(pDevCtx);
                        }
                        break;

                    case VP_DEV_TIMER_ABSCAL:
                        Vp880AbsCalibration(pDevCtx);
                        break;

                    default:
                        break;
                } /* Switch (timerType) */

                /*
                 * This is a check to make sure that one of the call back
                 * functions has not reset the value of the devTimer. If
                 * the call back function has not then just clear the timer bits
                 * if it has then we need to enable the activate mask.
                 */
                if (pDevObj->devTimer[devTimerType] == 0) {
                    pDevObj->devTimer[devTimerType] &= ~(VP_ACTIVATE_TIMER);
                } else {
                    pDevObj->devTimer[devTimerType] |= VP_ACTIVATE_TIMER;
                }
            } else { /* If timer has not expired */
                pDevObj->devTimer[devTimerType] |= VP_ACTIVATE_TIMER;
            }
        } /* if timerType is active     */
    } /* Loop through all device timers */

    /* Iterate through the channels until all timers are serviced */
    for(channelId=0; channelId < maxChan; channelId++ ) {
        pLineCtx = pDevCtx->pLineCtx[channelId];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            /* Adjust the EC value for Wideband mode as needed */
            ecVal[channelId] |= ((pLineObj->codec == VP_OPTION_WIDEBAND)
                ? VP880_WIDEBAND_MODE : 0);

            if (!(pLineObj->status & VP880_IS_FXO)) {
                for (lineTimerType = 0; lineTimerType < VP_LINE_TIMER_LAST;
                     lineTimerType++) {

                    if (pLineObj->lineTimers.timers.timer[lineTimerType]
                      & VP_ACTIVATE_TIMER) {

                        tempTimer = (pLineObj->lineTimers.timers.timer[lineTimerType]
                            & VP_TIMER_TIME_MASK);

                        if (tempTimer > 0) {
                            tempTimer--;
                        }

                        pLineObj->lineTimers.timers.timer[lineTimerType] = tempTimer;

                        if (tempTimer == 0) {
                            pLineObj->lineTimers.timers.timer[lineTimerType] &=
                                ~(VP_ACTIVATE_TIMER);

                            VP_DOUT(DBG_LINE_STATE, ("Line timer %d Expired at %d",
                                lineTimerType, pDevObj->timeStamp));

                            /* Perform appropriate action based on timerType */
                            switch (lineTimerType) {
                                case VP_LINE_POLREV_DEBOUNCE:
                                case VP_LINE_RING_EXIT_DEBOUNCE:
                                case VP_LINE_DISCONNECT_EXIT:
                                    pDevObj->status.state |= VP_DEV_FORCE_SIG_READ;
                                    break;

                                case VP_LINE_CID_DEBOUNCE:
                                case VP_LINE_TIMER_CID_DTMF:
                                case VP_LINE_TIMER_FAULT:
                                    break;

                                case VP_LINE_GND_START_TIMER:
                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR3_RD, VP880_ICR3_LEN, icr3Values);
                                    icr3Values[VP880_ICR3_LONG_LOOP_CTRL_LOCATION] |=
                                        VP880_ICR3_LONG_LOOP_CONTROL;
                                    icr3Values[VP880_ICR3_LONG_LOOP_CTRL_LOCATION+1] |=
                                        VP880_ICR3_LONG_LOOP_CONTROL;

                                    VP_DOUT(DBG_LINE_STATE, ("1. Ground Key Timer: Write ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                                        icr3Values[0], icr3Values[1],
                                        icr3Values[2], icr3Values[3], channelId));
                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR3_WRT, VP880_ICR3_LEN, icr3Values);

                                    icr3Values[VP880_ICR3_LONG_LOOP_CTRL_LOCATION] &=
                                        ~VP880_ICR3_LONG_LOOP_CONTROL;

                                    VP_DOUT(DBG_LINE_STATE, ("2. Ground Key Timer: Write ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                                        icr3Values[0], icr3Values[1],
                                        icr3Values[2], icr3Values[3], channelId));

                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR3_WRT, VP880_ICR3_LEN, icr3Values);

                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR4_RD, VP880_ICR4_LEN, icr4Values);
                                    icr4Values[VP880_ICR4_GKEY_DET_LOCATION] |=
                                        VP880_ICR4_GKEY_DET;
                                    icr4Values[VP880_ICR4_GKEY_DET_LOCATION+1] |=
                                        VP880_ICR4_GKEY_DET;
                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR4_WRT, VP880_ICR4_LEN, icr4Values);

                                    icr4Values[VP880_ICR4_GKEY_DET_LOCATION] &=
                                        ~VP880_ICR4_GKEY_DET;
                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_ICR4_WRT, VP880_ICR4_LEN, icr4Values);
                                    break;

                                case VP_LINE_CAL_LINE_TIMER:
                                    Vp880CalLineInt(pLineCtx);
                                    break;

                               case VP_LINE_PING_TIMER:
                                    if ((pLineObj->termType == VP_TERM_FXS_LOW_PWR)
                                     && (!(pLineObj->status & VP880_LINE_LEAK))
                                     && (pLineObj->lineState.usrCurrent == VP_LINE_STANDBY)) {
                                        if ((pDevObj->stateInt & (VP880_LINE0_LP | VP880_LINE1_LP)) ==
                                            (VP880_LINE0_LP | VP880_LINE1_LP)) {
                                            pLineObj->nextSlicValue = VP880_SS_DISCONNECT;
                                        }
                                    }

                                    VP_DOUT(DBG_LINE_STATE, ("\n\r3. PING TIMER -- Setting Chan %d to Value 0x%02X at Time %d",
                                        channelId, pLineObj->nextSlicValue, pDevObj->timeStamp));

                                    VpMpiCmd(deviceId, ecVal[channelId],
                                        VP880_SYS_STATE_WRT, VP880_SYS_STATE_LEN,
                                        &pLineObj->nextSlicValue);
                                    break;

                                case VP_LINE_TRACKER_DISABLE:
                                    {
                                        uint8 sysState[VP880_SYS_STATE_LEN] =
                                            {VP880_SS_DISCONNECT};
                                        uint8 icr2Mods[VP880_ICR2_LEN];

                                        VP_DOUT(DBG_LINE_STATE, ("Tracker Disable Expired at Time %d",
                                            pDevObj->timeStamp));

                                        /* Set line to Disconnect */
                                        VpMpiCmd(deviceId, ecVal[channelId], VP880_SYS_STATE_WRT,
                                            VP880_SYS_STATE_LEN, sysState);

                                        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD,
                                            VP880_ICR2_LEN, icr2Mods);

                                        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] |= VP880_ICR2_ILA_DAC;
                                        icr2Mods[VP880_ICR2_VOC_DAC_INDEX] &= ~VP880_ICR2_VOC_DAC_EN;
                                        icr2Mods[VP880_ICR2_VOC_DAC_INDEX+1] &= ~VP880_ICR2_ILA_DAC;

                                        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT,
                                            VP880_ICR2_LEN, icr2Mods);
                                    }
                                    break;

                                default:
                                    break;
                            } /* Switch (timerType) */
                        } else { /* If timer has not expired, keep it going */
                            pLineObj->lineTimers.timers.timer[lineTimerType]
                                |= VP_ACTIVATE_TIMER;
                        }
                    } /* if timerType is active     */
                } /* Loop through all timerTypes for chanID */
            } else {
                /* Increment the time since polrev was observed */
                if (pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev
                    < (0x7FFF - tickAdder)) {
                    /*
                     * The time is in 0.25mS increments, but the device
                     * tickrate is something else. Increment by the scaled
                     * amount.
                     */
                    pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev
                        += tickAdder;
                } else {
                    /* Max limit the value of last polrev value */
                    pLineObj->lineTimers.timers.fxoTimer.timeLastPolRev =
                        0x7FFF;
                }

                /* Set tick adder for 1ms increments */
                tickAdder =
                    pDevObj->devProfileData.tickRate / (VP_CSLAC_TICKSTEP_0_5MS * 2);

                if (((uint16)pLineObj->lineTimers.timers.fxoTimer.lastStateChange + tickAdder) > 255) {
                    pLineObj->lineTimers.timers.fxoTimer.lastStateChange = 255;
                } else {
                    pLineObj->lineTimers.timers.fxoTimer.lastStateChange+=tickAdder;
                }

                if (((uint16)pLineObj->lineTimers.timers.fxoTimer.lastNotLiu - tickAdder) <= 0) {
                    pLineObj->lineTimers.timers.fxoTimer.lastNotLiu = 0;
                } else {
                    pLineObj->lineTimers.timers.fxoTimer.lastNotLiu-=tickAdder;
                }

                if (((uint16)pLineObj->lineTimers.timers.fxoTimer.fxoDiscIO2Change - tickAdder) <= 0) {
                    pLineObj->lineTimers.timers.fxoTimer.fxoDiscIO2Change = 0;
                } else {
                    pLineObj->lineTimers.timers.fxoTimer.fxoDiscIO2Change-=tickAdder;
                }

                if (pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce >= tickAdder) {
                    pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce-=tickAdder;

                    if (pLineObj->lineTimers.timers.fxoTimer.disconnectDebounce ==0) {
                        if (pLineObj->preDisconnect ==
                            (pLineObj->lineState.condition & VP_CSLAC_DISC)) {
                            /*
                             * There is a change that persisted longer than the
                             * debounce interval. Evaluate and generate event
                             */
                            pLineObj->lineEventHandle = pDevObj->timeStamp;

                            switch(pLineObj->lineState.usrCurrent) {
                                case VP_LINE_FXO_TALK:
                                case VP_LINE_FXO_LOOP_CLOSE:
                                    if (pLineObj->preDisconnect == VP_CSLAC_DISC) {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_DISCONNECT;
                                    } else {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_RECONNECT;
                                    }
                                    break;

                                default:
                                    if (pLineObj->preDisconnect == VP_CSLAC_DISC) {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_FEED_DIS;
                                    } else {
                                        pLineObj->lineEvents.fxo |= VP_LINE_EVID_FEED_EN;
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
        } /* Line Context Check */
    } /* Loop through channels until no more tests */

    return retFlag;
} /* Vp880ServiceTimers() */

/**
 * Vp880FSKGeneratorReady()
 *  This function is used for Caller ID to determine if the FSK generator is
 * ready to accept another byte. It uses the device caller ID state machine
 * and signaling (caller ID status) register. This function should be called
 * from an API internal function only.
 *
 * Returns:
 *  TRUE if the FSK generator for Caller ID can accept a byte, FALSE otherwise.
 */
bool
Vp880FSKGeneratorReady(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    uint8 channelId = pLineObj->channelId;
    uint8 cidParam[VP880_CID_PARAM_LEN];
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 cidState;
    uint8 devRev = pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION];

    /* Adjust the EC value for Wideband mode as needed */
    ecVal[channelId] |=
       ((pLineObj->codec == VP_OPTION_WIDEBAND) ? VP880_WIDEBAND_MODE : 0);

    /* Check the Generator State */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CID_PARAM_RD,
        VP880_CID_PARAM_LEN, cidParam);
    cidState = (cidParam[0] & VP880_CID_STATE_MASK);

    VP_DOUT(DBG_CID, ("CID Param 0x%02X, State 0x%02X", cidParam[0], cidState));

    /* Check to see if the device is ready to accept (more) CID data */
    if (devRev <= VP880_REV_VC) {
        if (cidState == VP880_CID_STATE_FULL) {
            return FALSE;
        } else {
            return TRUE;
        }
    } else {
        if (cidState == VP880_CID_STATE_FULL_D) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

#ifdef VP_DEBUG
/**
 * Vp880RegisterDump()
 *  Dump all known 880 device and line specific registers (for debug purposes).
 *
 * Returns:
 */
VpStatusType
Vp880RegisterDump(
    VpDevCtxType *pDevCtx)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 channelId;

    uint8 intSwRegParam[VP880_INT_SWREG_PARAM_LEN];
    uint8 swYZ[VP880_REGULATOR_PARAM_LEN];
    uint8 devType[VP880_DEVTYPE_LEN];
    uint8 testReg1[VP880_TEST_REG1_LEN];
    uint8 testReg2[VP880_TEST_REG2_LEN];
    uint8 txRxSlot[VP880_XR_CS_LEN];
    uint8 devCfg[VP880_DCR_LEN];
    uint8 opMode[VP880_OP_MODE_LEN];
    uint8 sigReg[VP880_NO_UL_SIGREG_LEN];
    uint8 intMask[VP880_INT_MASK_LEN];
    uint8 intRev[VP880_REV_INFO_LEN];
    uint8 swCtrl[VP880_REGULATOR_CTRL_LEN];
    uint8 devMode[VP880_DEV_MODE_LEN];

    uint8 registerIndex;

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEVTYPE_CMD, VP880_DEVTYPE_LEN, devType);
    VP_DOUT(VP_DBG_ERROR, ("DEV TYPE "));
    for (registerIndex = 0; registerIndex < VP880_DEVTYPE_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", devType[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DEV_MODE_RD, VP880_DEV_MODE_LEN, devMode);
    VP_DOUT(VP_DBG_ERROR, ("DEV MODE "));
    for (registerIndex = 0; registerIndex < VP880_DEV_MODE_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", devMode[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_TEST_REG1_RD, VP880_TEST_REG1_LEN, testReg1);
    VP_DOUT(VP_DBG_ERROR, ("TEST REG1 "));
    for (registerIndex = 0; registerIndex < VP880_TEST_REG1_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", testReg1[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_TEST_REG2_RD, VP880_TEST_REG2_LEN, testReg2);
    VP_DOUT(VP_DBG_ERROR, ("TEST REG2 "));
    for (registerIndex = 0; registerIndex < VP880_TEST_REG2_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", testReg1[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_XR_CS_RD, VP880_XR_CS_LEN, txRxSlot);
    VP_DOUT(VP_DBG_ERROR, ("TX_RX_SLOT "));
    for (registerIndex = 0; registerIndex < VP880_XR_CS_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", txRxSlot[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_DCR_RD, VP880_DCR_LEN, devCfg);
    VP_DOUT(VP_DBG_ERROR, ("DEV CFG "));
    for (registerIndex = 0; registerIndex < VP880_DCR_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", devCfg[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_OP_MODE_RD, VP880_OP_MODE_LEN, opMode);
    VP_DOUT(VP_DBG_ERROR, ("OP MODE "));
    for (registerIndex = 0; registerIndex < VP880_OP_MODE_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", opMode[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_NO_UL_SIGREG_RD, VP880_NO_UL_SIGREG_LEN, sigReg);
    VP_DOUT(VP_DBG_ERROR, ("SIG REG "));
    for (registerIndex = 0; registerIndex < VP880_NO_UL_SIGREG_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", sigReg[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_MASK_RD, VP880_INT_MASK_LEN, intMask);
    VP_DOUT(VP_DBG_ERROR, ("INT MASK "));
    for (registerIndex = 0; registerIndex < VP880_INT_MASK_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", intMask[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_REV_INFO_RD, VP880_REV_INFO_LEN, intRev);
    VP_DOUT(VP_DBG_ERROR, ("INT REV "));
    for (registerIndex = 0; registerIndex < VP880_REV_INFO_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", intRev[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_REGULATOR_PARAM_RD, VP880_REGULATOR_PARAM_LEN, swYZ);
    VP_DOUT(VP_DBG_ERROR, ("REGULATOR "));
    for (registerIndex = 0; registerIndex < VP880_REGULATOR_PARAM_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", swYZ[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_REGULATOR_CTRL_RD, VP880_REGULATOR_CTRL_LEN, swCtrl);
    VP_DOUT(VP_DBG_ERROR, ("REGULATOR CTRL"));
    for (registerIndex = 0; registerIndex < VP880_REGULATOR_CTRL_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", swCtrl[registerIndex]));
    }

    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_SWREG_PARAM_RD, VP880_INT_SWREG_PARAM_LEN, intSwRegParam);
    VP_DOUT(VP_DBG_ERROR, ("INT SWREG "));
    for (registerIndex = 0; registerIndex < VP880_INT_SWREG_PARAM_LEN; registerIndex++) {
        VP_DOUT(VP_DBG_ERROR, ("0x%02X ", intSwRegParam[registerIndex]));
    }

    for (channelId = 0; channelId < pDevObj->staticInfo.maxChannels; channelId++) {
        uint8 sysState[VP880_SYS_STATE_LEN];
        uint8 sysStateCfg[VP880_SS_CONFIG_LEN];
        uint8 convCfg[VP880_CONV_CFG_LEN];

        uint8 icr1[VP880_ICR1_LEN];
        uint8 icr2[VP880_ICR2_LEN];
        uint8 icr3[VP880_ICR3_LEN];
        uint8 icr4[VP880_ICR4_LEN];
        uint8 icr5[VP880_ICR5_LEN];
        uint8 dcCal[VP880_DC_CAL_REG_LEN];

        uint8 disn[VP880_DISN_LEN];
        uint8 vpGain[VP880_VP_GAIN_LEN];
        uint8 grGain[VP880_GR_GAIN_LEN];
        uint8 gxGain[VP880_GX_GAIN_LEN];

        uint8 x[VP880_X_FILTER_LEN];
        uint8 r[VP880_R_FILTER_LEN];

        uint8 b1[VP880_B1_FILTER_LEN];
        uint8 b2[VP880_B2_FILTER_LEN];

        uint8 z1[VP880_Z1_FILTER_LEN];
        uint8 z2[VP880_Z2_FILTER_LEN];

        uint8 opFunc[VP880_CODEC_REG_LEN];
        uint8 opCond[VP880_OP_COND_LEN];

        uint8 ioData[VP880_IODATA_REG_LEN];
        uint8 ioDir[VP880_IODIR_REG_LEN];
        uint8 batCal[VP880_BAT_CALIBRATION_LEN];

        uint8 txSlot[VP880_TX_TS_LEN];
        uint8 rxSlot[VP880_RX_TS_LEN];

        uint8 dcFeed[VP880_DC_FEED_LEN];
        uint8 loopSup[VP880_LOOP_SUP_LEN];

        uint8 meteringParams[VP880_METERING_PARAM_LEN];

        uint8 sigABParams[VP880_SIGA_PARAMS_LEN];
        uint8 sigCDParams[VP880_SIGCD_PARAMS_LEN];

        uint8 cadenceParams[VP880_CADENCE_TIMER_LEN];
        uint8 cidParams[VP880_CID_PARAM_LEN];
        uint8 ecVal = (channelId == 0) ? VP880_EC_CH1 : VP880_EC_CH2;

        VP_DOUT(VP_DBG_ERROR, ("CHANNEL %d", channelId));

        VpMpiCmd(deviceId, ecVal, VP880_VP_GAIN_RD, VP880_VP_GAIN_LEN, vpGain);
        VP_DOUT(VP_DBG_ERROR, ("VP GAIN "));
        for (registerIndex = 0; registerIndex < VP880_VP_GAIN_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", vpGain[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_IODATA_REG_RD, VP880_IODATA_REG_LEN, ioData);
        VP_DOUT(VP_DBG_ERROR, ("IO DATA "));
        for (registerIndex = 0; registerIndex < VP880_IODATA_REG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", ioData[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_IODIR_REG_RD, VP880_IODIR_REG_LEN, ioDir);
        VP_DOUT(VP_DBG_ERROR, ("IO DIR "));
        for (registerIndex = 0; registerIndex < VP880_IODIR_REG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", ioDir[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_BAT_CALIBRATION_RD, VP880_BAT_CALIBRATION_LEN, batCal);
        VP_DOUT(VP_DBG_ERROR, ("BAT CAL REG "));
        for (registerIndex = 0; registerIndex < VP880_BAT_CALIBRATION_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", batCal[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_SYS_STATE_RD, VP880_SYS_STATE_LEN, sysState);
        VP_DOUT(VP_DBG_ERROR, ("SYSTEM STATE "));
        for (registerIndex = 0; registerIndex < VP880_SYS_STATE_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", sysState[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_SS_CONFIG_RD, VP880_SS_CONFIG_LEN, sysStateCfg);
        VP_DOUT(VP_DBG_ERROR, ("SYSTEM STATE CFG "));
        for (registerIndex = 0; registerIndex < VP880_SS_CONFIG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", sysStateCfg[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_CONV_CFG_RD, VP880_CONV_CFG_LEN, convCfg);
        VP_DOUT(VP_DBG_ERROR, ("CONVERTER CFG "));
        for (registerIndex = 0; registerIndex < VP880_CONV_CFG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", convCfg[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_OP_FUNC_RD, VP880_CODEC_REG_LEN, opFunc);
        VP_DOUT(VP_DBG_ERROR, ("OP_FUNC "));
        for (registerIndex = 0; registerIndex < VP880_CODEC_REG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", opFunc[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_OP_COND_RD, VP880_OP_COND_LEN, opCond);
        VP_DOUT(VP_DBG_ERROR, ("OP_COND "));
        for (registerIndex = 0; registerIndex < VP880_OP_COND_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", opCond[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_DISN_RD, VP880_DISN_LEN, disn);
        VP_DOUT(VP_DBG_ERROR, ("DISN "));
        for (registerIndex = 0; registerIndex < VP880_DISN_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", disn[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_GX_GAIN_RD, VP880_GX_GAIN_LEN, gxGain);
        VP_DOUT(VP_DBG_ERROR, ("GX GAIN "));
        for (registerIndex = 0; registerIndex < VP880_GX_GAIN_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", gxGain[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_GR_GAIN_RD, VP880_GR_GAIN_LEN, grGain);
        VP_DOUT(VP_DBG_ERROR, ("GR GAIN "));
        for (registerIndex = 0; registerIndex < VP880_GR_GAIN_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", grGain[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_B1_FILTER_RD, VP880_B1_FILTER_LEN, b1);
        VP_DOUT(VP_DBG_ERROR, ("B1 "));
        for (registerIndex = 0; registerIndex < VP880_B1_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", b1[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_B2_FILTER_RD, VP880_B2_FILTER_LEN, b2);
        VP_DOUT(VP_DBG_ERROR, ("B2 "));
        for (registerIndex = 0; registerIndex < VP880_B2_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", b2[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_X_FILTER_RD, VP880_X_FILTER_LEN, x);
        VP_DOUT(VP_DBG_ERROR, ("X "));
        for (registerIndex = 0; registerIndex < VP880_X_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", x[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_R_FILTER_RD, VP880_R_FILTER_LEN, r);
        VP_DOUT(VP_DBG_ERROR, ("R "));
        for (registerIndex = 0; registerIndex < VP880_R_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", r[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_Z1_FILTER_RD, VP880_Z1_FILTER_LEN, z1);
        VP_DOUT(VP_DBG_ERROR, ("Z1 "));
        for (registerIndex = 0; registerIndex < VP880_Z1_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", z1[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_Z2_FILTER_RD, VP880_Z2_FILTER_LEN, z2);
        VP_DOUT(VP_DBG_ERROR, ("Z2 "));
        for (registerIndex = 0; registerIndex < VP880_Z2_FILTER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", z2[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_TX_TS_RD, VP880_TX_TS_LEN, txSlot);
        VP_DOUT(VP_DBG_ERROR, ("TX_TS "));
        for (registerIndex = 0; registerIndex < VP880_TX_TS_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", txSlot[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_RX_TS_RD, VP880_RX_TS_LEN, rxSlot);
        VP_DOUT(VP_DBG_ERROR, ("RX_TS "));
        for (registerIndex = 0; registerIndex < VP880_RX_TS_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", rxSlot[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_DC_FEED_RD, VP880_DC_FEED_LEN, dcFeed);
        VP_DOUT(VP_DBG_ERROR, ("DC_FEED "));
        for (registerIndex = 0; registerIndex < VP880_DC_FEED_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", dcFeed[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_LOOP_SUP_RD, VP880_LOOP_SUP_LEN, loopSup);
        VP_DOUT(VP_DBG_ERROR, ("LOOP_SUP "));
        for (registerIndex = 0; registerIndex < VP880_LOOP_SUP_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", loopSup[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_METERING_PARAM_RD, VP880_METERING_PARAM_LEN, meteringParams);
        VP_DOUT(VP_DBG_ERROR, ("METERING "));
        for (registerIndex = 0; registerIndex < VP880_METERING_PARAM_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", meteringParams[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_SIGA_PARAMS_RD, VP880_SIGA_PARAMS_LEN, sigABParams);
        VP_DOUT(VP_DBG_ERROR, ("SIG_AB "));
        for (registerIndex = 0; registerIndex < VP880_SIGA_PARAMS_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", sigABParams[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_SIGCD_PARAMS_RD, VP880_SIGCD_PARAMS_LEN, sigCDParams);
        VP_DOUT(VP_DBG_ERROR, ("SIG_CD "));
        for (registerIndex = 0; registerIndex < VP880_SIGCD_PARAMS_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", sigCDParams[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_CADENCE_TIMER_RD, VP880_CADENCE_TIMER_LEN, cadenceParams);
        VP_DOUT(VP_DBG_ERROR, ("CADENCE "));
        for (registerIndex = 0; registerIndex < VP880_CADENCE_TIMER_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", cadenceParams[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_CID_PARAM_RD, VP880_CID_PARAM_LEN, cidParams);
        VP_DOUT(VP_DBG_ERROR, ("CID PARAMS "));
        for (registerIndex = 0; registerIndex < VP880_CID_PARAM_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", cidParams[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_ICR1_RD, VP880_ICR1_LEN, icr1);
        VP_DOUT(VP_DBG_ERROR, ("ICR1 "));
        for (registerIndex = 0; registerIndex < VP880_ICR1_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", icr1[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_ICR2_RD, VP880_ICR2_LEN, icr2);
        VP_DOUT(VP_DBG_ERROR, ("ICR2 "));
        for (registerIndex = 0; registerIndex < VP880_ICR2_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", icr2[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_ICR3_RD, VP880_ICR3_LEN, icr3);
        VP_DOUT(VP_DBG_ERROR, ("ICR3 "));
        for (registerIndex = 0; registerIndex < VP880_ICR3_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", icr3[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_ICR4_RD, VP880_ICR4_LEN, icr4);
        VP_DOUT(VP_DBG_ERROR, ("ICR4 "));
        for (registerIndex = 0; registerIndex < VP880_ICR4_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", icr4[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_ICR5_RD, VP880_ICR5_LEN, icr5);
        VP_DOUT(VP_DBG_ERROR, ("ICR5 "));
        for (registerIndex = 0; registerIndex < VP880_ICR5_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", icr5[registerIndex]));
        }

        VpMpiCmd(deviceId, ecVal, VP880_DC_CAL_REG_RD, VP880_DC_CAL_REG_LEN, dcCal);
        VP_DOUT(VP_DBG_ERROR, ("DC CAL "));
        for (registerIndex = 0; registerIndex < VP880_DC_CAL_REG_LEN; registerIndex++) {
            VP_DOUT(VP_DBG_ERROR, ("0x%02X ", dcCal[registerIndex]));
        }
    }

    return VP_STATUS_SUCCESS;
}
#endif
#endif
