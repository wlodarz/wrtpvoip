/** \file apiInit.c
 * apiInit.c
 *
 * This file contains the line and device initialization functions for
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
#include "vp_debug.h"
#include "vp_hal.h"
#include "sys_service.h"

/**< Vp880 Initalization Function Prototypes */
static VpStatusType
Vp880InitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

static void
Vp880DeviceReset(
    Vp880DeviceObjectType *pDevObj);

static VpStatusType
Vp880InitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

static VpStatusType
Vp880ConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

static VpStatusType
Vp880InitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

static void
Vp880InitDeviceObject(
    Vp880DeviceObjectType *pDevObj);

static void
Vp880InitLineObject(
    Vp880LineObjectType *pLineObj);

typedef enum
{
    VP880_DEV_PROFILE_PCLK_MSB = 6,
    VP880_DEV_PROFILE_PCLK_LSB = 7,
    VP880_DEV_PROFILE_DEVCFG1 = 8,
    VP880_DEV_PROFILE_MCLK_CORR = 9,
    VP880_DEV_PROFILE_CLOCK_SLOT = 10,
    VP880_DEV_PROFILE_MAX_EVENTS = 11,
    VP880_DEV_PROFILE_TICKRATE_MSB = 12,
    VP880_DEV_PROFILE_TICKRATE_LSB = 13,
    VP880_DEV_PROFILE_SWITCHER_CMD = 14,
    VP880_DEV_PROFILE_SWITCHER_DATA0 = 15,
    VP880_DEV_PROFILE_SWITCHER_DATA1 = 16,
    VP880_DEV_PROFILE_SWITCHER_DATA2 = 17,
    VP880_DEV_PROFILE_TRACKER_INT_SW_REG = 18,
    VP880_DEV_PROFILE_ABS_INT_SW_REG = 24,
    VP880_DEV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} vp880_deviceProfileParams;

/**
 * VpMakeVp880DeviceObject()
 *  This function performs the main tasks of VpMakeDeviceObject() for Vp880 type
 * of devices.
 *
 * Preconditions:
 *  Same as VpMakeDeviceObject(), and in addition the deviceType pointed to by
 * pDevCtx should be Vp880 series type.
 *
 * Postconditions:
 *  VpAPI Function pointers for pDevCtx are initialized to Vp880 specific
 * functions.  This completes the function abstraction for "this" device.
 */
VpStatusType
VpMakeVp880DeviceObject(
    VpDevCtxType *pDevCtx,  /**< Device context to be initialized with function
                             * pointers
                             */
    Vp880DeviceObjectType *pDevObj) /**< Device object containing information
                                     * for the device pointed to by pDevCtx
                                     */
{
    Vp880InitDeviceObject(pDevObj);

    /* Initialize other elements in the device object */
    return VpMakeVp880DeviceCtx(pDevCtx, pDevObj);
}

/**
 * Vp880InitDeviceObject()
 *  This function initializes the Vp880 Device object data structure. It is
 * called only in this file .
 */
static void
Vp880InitDeviceObject(
    Vp880DeviceObjectType *pDevObj)
{
    uint16 objSize;
    uint8 *objPtr = (uint8 *)pDevObj;

    for (objSize = 0;
         objSize < (sizeof(Vp880DeviceObjectType) / sizeof(uint8));
         objSize++) {

        *objPtr = 0;
        objPtr++;
    }

    pDevObj->staticInfo.maxChannels = VP880_MAX_NUM_CHANNELS;
}

/**
 * Vp880InitLineObject()
 *  This function initializes the Vp880 Line Object data structure. It is
 * called only in this file .
 */
static void
Vp880InitLineObject(
    Vp880LineObjectType *pLineObj)
{
    uint16 objSize;
    uint8 *objPtr = (uint8 *)pLineObj;

    for (objSize = 0;
         objSize < (sizeof(Vp880LineObjectType) / sizeof(uint8));
         objSize++) {

        *objPtr = 0;
        objPtr++;
    }
}

/**
 * VpMakeVp880DeviceCtx()
 *  This function initializes the device context to handle Vp880 functionality.
 *
 * Preconditions:
 *  This function should be called after initializing the device object. This
 * function can be called more than once since it does not modify the contents
 * of the device object.
 *
 * Postconditions:
 *  Initializes device context to be able to handle Vp780 functionality.
 */
VpStatusType
VpMakeVp880DeviceCtx(
    VpDevCtxType *pDevCtx,          /**< Device Context to be initialized */
    Vp880DeviceObjectType *pDevObj) /**< Device Object that has been already
                                     * initialized
                                     */
{
    uint8 channelCount, maxChan;

    if((pDevCtx == VP_NULL) || (pDevObj == VP_NULL)) {
        return VP_STATUS_INVALID_ARG;
    }

    /* Initialize Device context */
    pDevCtx->pDevObj = pDevObj;
    pDevCtx->deviceType = VP_DEV_880_SERIES;

    /*
     * Initialize all of the line context pointers to null in the device context
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (channelCount = 0; channelCount < maxChan; channelCount++) {
        pDevCtx->pLineCtx[channelCount] = VP_NULL;
    }

    /* Functions in apiInit.c */
    pDevCtx->funPtrsToApiFuncs.MakeLineObject = Vp880MakeLineObject;
    pDevCtx->funPtrsToApiFuncs.InitDevice = Vp880InitDevice;
    pDevCtx->funPtrsToApiFuncs.InitLine = Vp880InitLine;
    pDevCtx->funPtrsToApiFuncs.ConfigLine = Vp880ConfigLine;
    pDevCtx->funPtrsToApiFuncs.InitProfile = Vp880InitProfile;

#ifdef CSLAC_SEQ_EN
    pDevCtx->funPtrsToApiFuncs.InitRing = Vp880InitRing;
    pDevCtx->funPtrsToApiFuncs.InitCid = Vp880InitCid;
    pDevCtx->funPtrsToApiFuncs.InitMeter = VpCSLACInitMeter;
    pDevCtx->funPtrsToApiFuncs.DtmfDigitDetected = VpCSLACDtmfDigitDetected;
#endif

    pDevCtx->funPtrsToApiFuncs.ClearResults = VpCSLACClearResults;

    /* Functions in apicnt.c */
    pDevCtx->funPtrsToApiFuncs.SetLineState = Vp880SetLineState;
    pDevCtx->funPtrsToApiFuncs.SetLineTone = Vp880SetLineTone;
    pDevCtx->funPtrsToApiFuncs.SetRelGain = Vp880SetRelGain;
    pDevCtx->funPtrsToApiFuncs.SetRelayState = Vp880SetRelayState;

#ifdef CSLAC_SEQ_EN
    pDevCtx->funPtrsToApiFuncs.SendSignal = Vp880SendSignal;
    pDevCtx->funPtrsToApiFuncs.SendCid = Vp880SendCid;
    pDevCtx->funPtrsToApiFuncs.ContinueCid = Vp880ContinueCid;
    pDevCtx->funPtrsToApiFuncs.StartMeter = VpCSLACStartMeter;
#endif

    pDevCtx->funPtrsToApiFuncs.SetOption = Vp880SetOption;
    pDevCtx->funPtrsToApiFuncs.VirtualISR = Vp880VirtualISR;
    pDevCtx->funPtrsToApiFuncs.ApiTick = Vp880ApiTick;
    pDevCtx->funPtrsToApiFuncs.LowLevelCmd = Vp880LowLevelCmd;
    pDevCtx->funPtrsToApiFuncs.DeviceIoAccess = Vp880DeviceIoAccess;

    /* Functions in apiQuery.c */
    pDevCtx->funPtrsToApiFuncs.GetEvent = Vp880GetEvent;
    pDevCtx->funPtrsToApiFuncs.GetLineStatus = VpCSLACGetLineStatus;
    pDevCtx->funPtrsToApiFuncs.GetDeviceStatus = Vp880GetDeviceStatus;
    pDevCtx->funPtrsToApiFuncs.FlushEvents = Vp880FlushEvents;
    pDevCtx->funPtrsToApiFuncs.GetResults = Vp880GetResults;
    pDevCtx->funPtrsToApiFuncs.GetOption = Vp880GetOption;
#ifdef VP_DEBUG
    pDevCtx->funPtrsToApiFuncs.RegisterDump = Vp880RegisterDump;
#endif

    /* Functions in apiTestLine.c */
#ifdef VP880_INCLUDE_TESTLINE_CODE
    pDevCtx->funPtrsToApiFuncs.TestLineInt = Vp880TestLineInt;
    pDevCtx->funPtrsToApiFuncs.TestLineCallback = Vp880TestLineCallback;
    pDevCtx->funPtrsToApiFuncs.TestLine = Vp880TestLine;
#endif

    /* Functions in apiCal.c */
    pDevCtx->funPtrsToApiFuncs.CalCodec = Vp880CalCodec;
    pDevCtx->funPtrsToApiFuncs.CalLine = Vp880CalLine;

    return VP_STATUS_SUCCESS;
}

/**
 * VpMakeVp880LineObject()
 *  This function initializes a line context using the information that is
 * passed. This function is like a C++ constructor. It initializes the passed
 * line context and line object based on the paramters provided. The passed line
 * object type should match with the type of device object type. See VP-API
 * reference guide for more information.
 *
 * Preconditions:
 *  This function assumes device context has already been created and
 * initialized. This function should only be called after downloading the boot
 * image the device when applicable (like for VCP and VPP class of devices).
 *
 * Postconditions:
 *  This function initializes the line context/line object. Line related VP-API
 * functions can be called after calling this function.
 */
VpStatusType
Vp880MakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pVoidLineObj,
    VpDevCtxType *pDevCtx)
{
    Vp880LineObjectType *pLineObj = pVoidLineObj;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    Vp880DeviceStateIntType chanMap[] = {VP880_LINE0_IS_FXO, VP880_LINE1_IS_FXO};

    if (channelId >= pDevObj->staticInfo.maxChannels) {
        return VP_STATUS_INVALID_ARG;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    Vp880InitLineObject(pLineObj);

    switch (termType) {
        case VP_TERM_FXO_GENERIC:
        case VP_TERM_FXO_DISC:
            /*
             * At this point, it is only a recommendation. We'll adjust this
             * when we determine the device type found in VpInitDevice()
             */
            if (pDevObj->stateInt & VP880_DEVICE_DETECTED) {
                if (chanMap[channelId] & pDevObj->stateInt) {
                    pLineObj->status |= VP880_IS_FXO;
                } else {
                    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                    return VP_STATUS_ERR_VTD_CODE;
                }
            } else {
                pLineObj->status |= VP880_IS_FXO;
            }

            pDevObj->stateInt |=
                ((channelId == 0) ? VP880_LINE0_LP : VP880_LINE1_LP);
            break;

        case VP_TERM_FXS_GENERIC:
        case VP_TERM_FXS_ISOLATE:
        case VP_TERM_FXS_SPLITTER:
            pLineObj->status = VP880_INIT_STATUS;
            pDevObj->stateInt &=
                ((channelId == 0) ? ~VP880_LINE0_LP : ~VP880_LINE1_LP);
            break;

        case VP_TERM_FXS_LOW_PWR:
            pLineObj->status = VP880_INIT_STATUS;
            pDevObj->stateInt |=
                ((channelId == 0) ? VP880_LINE0_LP : VP880_LINE1_LP);
            break;

        default:
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_VTD_CODE;
    }

    pLineCtx->pLineObj = pLineObj;
    pLineCtx->pDevCtx = pDevCtx;

    pDevCtx->pLineCtx[channelId] = pLineCtx;
    pLineObj->channelId = channelId;
    pLineObj->termType = termType;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Everything else done by device/line specific functions */
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880Init
 *  This function initializes the device, and (contrary to InitDevice) does
 * not initialize any channels. This function should be called internal to the
 * API only.
 *
 * Preconditions:
 *  The device context must be of a Vp880 device type.
 *
 * Postconditions:
 *  This function returns a failure code if the clock configuration is not set
 * correctly based on the device data set in InitDevice.
 */
VpStatusType
Vp880Init(
    VpDevCtxType *pDevCtx)
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 data, chanId, maxChan = 0;
    uint8 intMaskData[] = {0x7F, 0xFF};
    uint8 clkNotStable;
    uint8 clkTestCount;

    /*
     * When the device timing is cleared, so are the lines associated with
     * this device
     */
    maxChan = pDevObj->staticInfo.maxChannels;
    for (chanId = 0; chanId < maxChan; chanId++) {
        /*
         * It is possible that any channel on a device is not used and therefore
         * points to a NULL pointer. Check it and jump over if this is the case
         * to the next line until all possible line numbers for this device have
         * been checked.
         */
        pLineCtx = pDevCtx->pLineCtx[chanId];
        if (pLineCtx != VP_NULL) {
            /*
             * If the device is "dumping" it's timers, the lines associated with
             * this device better not be relying on timers.
             */
            InitTimerVars(pLineCtx);
        }
    }

    /*
     * If the MPI Bus gets out of sequence for any reason, a HW reset command
     * will not work and this function may fail. To be sure a reset occurs, the
     * following sequence is required.
     */
    Vp880DeviceReset(pDevObj);

    /*
     * Setup mclk. The MCLK mask set the mclk frequency, sets the mclk source
     * (the MCLK pin or the PCLK pin), and sets the interrupt pin output drive
     * mode (TTL or open collector)
     */
    data = pDevObj->devProfileData.devCfg1;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_MCLK_CNT_WRT, VP880_MCLK_CNT_LEN,
        &data);

    /* Setup the Clock Fail Interrupt */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_MASK_WRT, VP880_INT_MASK_LEN,
        intMaskData);

    /*
     * Wait for the CFAIL bit to clear before proceding. If the CFAIL bit does
     * not clear after several trys, give up and return an error condition. Wait
     * between each read of the status register.
     */

    clkNotStable = VP880_CFAIL_MASK;
    clkTestCount = MAX_CFAIL_TEST;
    while(clkNotStable && (--clkTestCount) != 0) {
        VpSysWait(CFAIL_TEST_INTERVAL*10);
        VpMpiCmd(deviceId, VP880_EC_CH1, VP880_UL_SIGREG_RD,
            VP880_UL_SIGREG_LEN, pDevObj->intReg);
        clkNotStable = pDevObj->intReg[0] & VP880_CFAIL_MASK;
    }

    /*
     * The CFAIL bit did not clear so the part will not complete initialization.
     * Return error status to indicate failure.
     */
    if(clkNotStable) {
        pDevObj->deviceEvents.faults |= VP_DEV_EVID_CLK_FLT;
        return VP_STATUS_FAILURE;
    }

    /* Setup interrupts back to default */
    intMaskData[0] = 0xFF;  /* Clear all Device Interrupt Masks */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_INT_MASK_WRT, VP880_INT_MASK_LEN,
        intMaskData);

    /*
     * The PCM mask tells the device which clock edge to grab and xmit the
     * PCM data on and also which clock period LSB of the PCM data starts on
     */
    data = pDevObj->devProfileData.clockSlot;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_XR_CS_WRT, VP880_XR_CS_LEN, &data);

    return VP_STATUS_SUCCESS;
} /* Vp880Init */

/**
 * Vp880DeviceReset
 *  This function resets the MPI buffer and HW reset of the device. The method
 * for doing this depends on the silicon revision due to I/O1 driver
 * requirements.
 *
 * Preconditions:
 *  None.
 *
 * Postconditions:
 *  The Device is reset, and I/O1 is in a safe condition.
 */
void
Vp880DeviceReset(
    Vp880DeviceObjectType *pDevObj)
{
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 mpiCmdData;
    uint8 mpiClear[] = {
        VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP,
        VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP,
        VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP, VP880_NO_OP,
        VP880_NO_OP
    };

    /*
     * First, make sure the MPI buffer is cleared so we can write to the
     * device correctly prior to HW reset.
     */
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_NO_OP, 16, mpiClear);

    /*
     * Force I/O1 to '1', and wait for (if present) the external relay to
     * open.
     */
    mpiCmdData = VP880_IODATA_IO1;
    VP_DOUT(VP_DBG_INFO, ("3. Write IODATA 0x%02X on Both Channels",
        VP880_IODATA_IO1));

    VpMpiCmd(deviceId, (VP880_EC_CH1 | VP880_EC_CH2), VP880_IODATA_REG_WRT,
        VP880_IODATA_REG_LEN, &mpiCmdData);
    VpSysWait(24);  /* EMR's take 2-3 ms to open/close */

    /* Proceed with normal device level reset and required delay */
    mpiCmdData = VP880_HW_RESET_CMD;
    VpMpiCmd(deviceId, VP880_EC_CH1, VP880_HW_RESET_CMD, NO_DATA, &mpiCmdData);
    VpSysWait(20);
}

/**
 * Vp880InitDevice
 *  This function initializes the device and all lines associated with this
 * device (if line profiles are passed to this function). The device profile
 * passed must be valid otherwise an error code is returned and the device
 * remains in it's previously initialized state.
 *
 * Preconditions:
 *  None (device context is not NULL and is of Vp880 type, which is handled in
 * higher level software)
 *
 * Postconditions:
 *  This device is initialized to the configuration specified in the device
 * profile, and the FXS lines associated with this device are initialized by the
 * FXS specific AC, DC, and Ringing profiles passed, and the FXO lines
 * associated with this device are initialized by the FXO specific AC and Config
 * profiles passed.  If the FXO/FXS profiles are all NULL, then only the device
 * initialization occurs. This function returns an error code if the device
 * profile trying to be used for initialization is VP_PTABLE_NULL (either
 * passed or by a non-initialized index).
 */
VpStatusType
Vp880InitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,   /**< The profile pointer for the device
                                     * configuration parameters
                                     */
    VpProfilePtrType pAcProfile,    /**< The profile pointer (or index) for
                                     * the AC characteristic to apply to the
                                     * FXS lines
                                     */
    VpProfilePtrType pDcProfile,    /**< The profile pointer (or index) for
                                     * the DC characteristic to apply to the
                                     * FXS lines
                                     */
    VpProfilePtrType pRingProfile,  /**< The profile pointer (or index) for
                                     * the Ringing characteristic to apply to
                                     * the FXS lines
                                     */
    VpProfilePtrType pFxoAcProfile, /**< The profile pointer (or index) for
                                     * the AC characteristic to apply to the
                                     * FXO lines
                                     */
    VpProfilePtrType pFxoCfgProfile)/**< The profile pointer for the FXO
                                     * specific supervision paramaters.
                                     */
{
    VpLineCtxType *pLineCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    Vp880LineObjectType *pLineObj;
    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 maxChan = pDevObj->staticInfo.maxChannels;
    VpProfilePtrType pDevProf;
    uint8 intSwParam[VP880_INT_SWREG_PARAM_LEN] =
        {0x5C, 0x4B, 0xC4, 0x4B, 0xC4, 0x4B};

#ifdef VP880_CURRENT_LIMIT
    uint8 intSwParamLimit[VP880_INT_SWREG_PARAM_LEN] =
        {0xB2, 0x00, 0xB1, 0x00, 0x60, 0x40};
#endif

    uint8 deviceRcn, chan, data, indexToSwProf;

    VpStatusType status = VP_STATUS_SUCCESS;
    Vp880PcnType devicePcn;

    int profIndex = VpGetProfileIndex(pDevProfile);

    /*
     * Get Profile Index returns -1 if the profile passed is a pointer or
     * of VP_PTABLE_NULL type. Otherwise it returns the index
     */

    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pDevProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_DEVICE, pDevProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        } else {
            pDevObj->status.state &= ~VP_DEV_INIT_IN_PROGRESS;
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return VP_STATUS_ERR_PROFILE;
        }
        pDevProf = pDevProfile;
    } else if (profIndex < VP_CSLAC_DEV_PROF_TABLE_SIZE) {
        pDevProf = pDevObj->devProfileTable.pDevProfileTable[profIndex];
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Initialize the API's device status variables */
    pDevObj->status.state = VP_DEV_INIT_IN_PROGRESS;
    pDevObj->timeStamp = 0;

    /* Initialize the API's device dynamic variables */
    pDevObj->dynamicInfo.lastChan = 0;
    pDevObj->dynamicInfo.bat1Fault = FALSE;
    pDevObj->dynamicInfo.bat2Fault = FALSE;
    pDevObj->dynamicInfo.bat3Fault = FALSE;
    pDevObj->dynamicInfo.clkFault = FALSE;

    pDevObj->stateInt = 0;  /* Reset the internal state information */

    if (pDevProf != VP_PTABLE_NULL) {
        pDevObj->devProfileData.pcmClkRate =
            (uint16)(((pDevProf[VP880_DEV_PROFILE_PCLK_MSB] << 8) & 0xFF00)
                    | (pDevProf[VP880_DEV_PROFILE_PCLK_LSB] & 0x00FF));

        pDevObj->devProfileData.devCfg1 =
            (uint8)(pDevProf[VP880_DEV_PROFILE_DEVCFG1]);
        pDevObj->devProfileData.clockSlot =
            (uint8)(pDevProf[VP880_DEV_PROFILE_CLOCK_SLOT]);

        /* Device has a depth of 2, so no point reading more than that */
        pDevObj->devProfileData.maxNumInterrupts = 2;

        pDevObj->devProfileData.tickRate =
            (uint16)(((pDevProf[VP880_DEV_PROFILE_TICKRATE_MSB] << 8) & 0xFF00)
                    | (pDevProf[VP880_DEV_PROFILE_TICKRATE_LSB] & 0x00FF));
    }

    /* Initialize device */
    /*
     * If not successful, the Clock Fail bit did not clear so return error code
     */
    if ((status = Vp880Init(pDevCtx)) != VP_STATUS_SUCCESS) {
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return status;
    }

    /*
     * Read in the PCN so we will know what type of device specifically we
     * are working with.  This affects the max number of lines supported by the
     * device as well as the type of lines (and may affect the line init)
     *
     * Note: EC value is not important for this command
     */

    VpMpiCmd(deviceId, ecVal[0], VP880_RCN_PCN_RD, VP880_RCN_PCN_LEN,
        pDevObj->staticInfo.rcnPcn);
    devicePcn = (Vp880PcnType)(pDevObj->staticInfo.rcnPcn[VP880_PCN_LOCATION]);
    deviceRcn = (uint8)(pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]);

    /*
     * Verify that we recognize the device by limiting to the range of those
     * supported in the Vp880PcnType table. If not recognized (although may
     * be a valid Zarlink Semiconductor PN) return an error because the API-II
     * does not know how to handle it. More often, the error is occuring because
     * the hardware cannot talk to the device.
     */

    if ((devicePcn > VP880_FIRST_OF_ONE_CHANNEL_DEVICES)
     && (devicePcn < VP880_LAST_OF_ONE_CHANNEL_DEVICES)) {
        pDevObj->staticInfo.maxChannels = 1;
        maxChan = 1;
        pDevObj->stateInt |= VP880_IS_SINGLE_CHANNEL;
    } else if ((devicePcn > VP880_FIRST_OF_TWO_CHANNEL_DEVICES)
            && (devicePcn < VP880_LAST_OF_TWO_CHANNEL_DEVICES)) {
        pDevObj->staticInfo.maxChannels = 2;
        maxChan = 2;
        pDevObj->stateInt &= ~VP880_IS_SINGLE_CHANNEL;
    } else {
        pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);
        VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
        return VP_STATUS_FAILURE;
    }

    pDevObj->stateInt |= VP880_DEVICE_DETECTED;

    /*
     * Make initial assumption that the device has lines such that low power
     * mode is in affect. The line controls will change as necessary.
     */
    pDevObj->stateInt |= (VP880_LINE0_LP | VP880_LINE1_LP);

    /* Check if device is ABS or Not */
    switch (devicePcn) {
        case VP880_DEV_PCN_88320:
        case VP880_DEV_PCN_88321:
        case VP880_DEV_PCN_88326:
        case VP880_DEV_PCN_88340:
        case VP880_DEV_PCN_88341:
        case VP880_DEV_PCN_88346:
        case VP880_DEV_PCN_88220:
        case VP880_DEV_PCN_88221:
        case VP880_DEV_PCN_88226:
        case VP880_DEV_PCN_88240:
        case VP880_DEV_PCN_88241:
        case VP880_DEV_PCN_88246:
            /* Mark device as ABS type */
            pDevObj->stateInt |= VP880_IS_ABS;
            if (pDevProf[VP_PROFILE_VERSION] >=
                VP_CSLAC_DEV_PROFILE_VERSION_INT_SW_CONFIG) {
                uint8 intSwParamByte;
                for (intSwParamByte = 0;
                     intSwParamByte < VP880_INT_SWREG_PARAM_LEN;
                     intSwParamByte++) {
                        pDevObj->intSwParams[intSwParamByte] =
                            pDevProf[VP880_DEV_PROFILE_ABS_INT_SW_REG+intSwParamByte];
                }
            }

            /*
             * Make sure lines are in disconnect state (recover from shutdown)
             * even if there are no line contexts associated with this device.
             */
            data = VP880_SS_DISCONNECT;
            for (chan = 0; chan < maxChan; chan++) {
                VpMpiCmd(deviceId, ecVal[chan], VP880_SYS_STATE_WRT,
                    VP880_SYS_STATE_LEN, &data);
            }

            data = VP880_SWY_OFF;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);

#ifdef VP880_CURRENT_LIMIT
            /* Implement a user compile option to limit the switcher power */
            VpMpiCmd(deviceId, ecVal[0], VP880_INT_SWREG_PARAM_WRT,
                VP880_INT_SWREG_PARAM_LEN, intSwParamLimit);
#else
            if (pDevProf[VP_PROFILE_VERSION] >=
                VP_CSLAC_DEV_PROFILE_VERSION_INT_SW_CONFIG) {

                VpMpiCmd(deviceId, ecVal[0], VP880_INT_SWREG_PARAM_WRT,
                    VP880_INT_SWREG_PARAM_LEN, pDevObj->intSwParams);
            }
#endif

#ifdef VP880_AUTO_BAT_DETECT
            if ((status = Vp880AutoBatDetect(pDevObj,
                &pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1])) != VP_STATUS_SUCCESS) {
                pDevObj->status.state &=
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);

                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return status;
            }
#else
            if (pDevProf[VP_PROFILE_VERSION] >= VP_CSLAC_DEV_PROFILE_VERSION_SW_CONFIG) {
                VpMpiCmd(deviceId, ecVal[0],
                    pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD],
                    VP880_REGULATOR_PARAM_LEN,
                    (VpProfileDataType *)(&pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1]));
            } else {
                pDevObj->status.state &=
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);

                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return VP_STATUS_FAILURE;
            }

            /* Low power mode first, wait at least 20ms, then to high power. */
            data = VP880_SWY_LP | VP880_SWZ_LP;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);

            VpSysWait(160); /* 125us * 160 = 20ms */

            data = VP880_SWY_HP | VP880_SWZ_HP;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);
#endif
            break;

        default: {
            uint8 icr3Vals[VP880_ICR3_LEN];
            uint8 ecAll = (VP880_EC_CH1 | VP880_EC_CH2);

            pDevObj->stateInt &= ~VP880_IS_ABS;

            /* Initialize Non-ABS device sensitve items */
            /*
             * Configure the Switcher for Flyback or Buckboost per device
             * profile. If the device is in Buckboost mode, config the internal
             * switcher
             */
            if (pDevProf[VP_PROFILE_VERSION] >=
                VP_CSLAC_DEV_PROFILE_VERSION_INT_SW_CONFIG) {
                uint8 intSwParamByte;
                for (intSwParamByte = 0;
                     intSwParamByte < VP880_INT_SWREG_PARAM_LEN;
                     intSwParamByte++) {
                        pDevObj->intSwParams[intSwParamByte] =
                            pDevProf[VP880_DEV_PROFILE_TRACKER_INT_SW_REG+intSwParamByte];
                }
                VpMpiCmd(deviceId, ecVal[0], VP880_INT_SWREG_PARAM_WRT,
                    VP880_INT_SWREG_PARAM_LEN, pDevObj->intSwParams);

                VP_DOUT(VP_DBG_API_FUNC, ("Internal Switching Params 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                    pDevObj->intSwParams[0], pDevObj->intSwParams[1], pDevObj->intSwParams[2],
                    pDevObj->intSwParams[3], pDevObj->intSwParams[4], pDevObj->intSwParams[5]));
            } else {
                if (pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1] & 0x20) {
                    VpMpiCmd(deviceId, ecVal[0], VP880_INT_SWREG_PARAM_WRT,
                        VP880_INT_SWREG_PARAM_LEN, intSwParam);

                    pDevObj->intSwParams[0] = intSwParam[0];
                    pDevObj->intSwParams[1] = intSwParam[1];
                    pDevObj->intSwParams[2] = intSwParam[2];
                    pDevObj->intSwParams[3] = intSwParam[3];
                    pDevObj->intSwParams[4] = intSwParam[4];
                    pDevObj->intSwParams[5] = intSwParam[5];

                    VP_DOUT(VP_DBG_API_FUNC, ("Internal Switching Params 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                        intSwParam[0], intSwParam[1], intSwParam[2],
                        intSwParam[3], intSwParam[4], intSwParam[5]));
                }
            }

            VpMpiCmd(deviceId, ecVal[0], pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD],
                VP880_REGULATOR_PARAM_LEN,
                (VpProfileDataType *)(&pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1]));

            for (indexToSwProf = 0; indexToSwProf < VP880_REGULATOR_PARAM_LEN;
                 indexToSwProf++) {

                pDevObj->swParams[indexToSwProf] =
                    pDevProf[VP880_DEV_PROFILE_SWITCHER_CMD+1+indexToSwProf];
            }

            if ((pDevObj->swParams[VP880_SWREG_RING_V_BYTE]
               & VP880_VOLTAGE_MASK) < VP880_VOLTAGE_60V) {
                pDevObj->swParams[VP880_SWREG_RING_V_BYTE]
                    &= ~VP880_VOLTAGE_MASK;
                pDevObj->swParams[VP880_SWREG_RING_V_BYTE]
                    |= VP880_VOLTAGE_60V;

                VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_PARAM_WRT,
                    VP880_REGULATOR_PARAM_LEN, pDevObj->swParams);
            }

            /* Device is reset, so ch1 and 2 are at same values for ICR's
             * NOTE: This also has to be done in VpInitLine() due to SW
             * Reset.
             */
            VpMpiCmd(deviceId, ecVal[0], VP880_ICR3_RD, VP880_ICR3_LEN,
                icr3Vals);
            icr3Vals[VP880_ICR3_LINE_CTRL_INDEX] |=
                VP880_ICR3_VREF_CTRL;
            icr3Vals[VP880_ICR3_LINE_CTRL_INDEX+1] |=
                VP880_ICR3_VREF_CTRL;
            VpMpiCmd(deviceId, ecAll, VP880_ICR3_WRT, VP880_ICR3_LEN,
                icr3Vals);
            VP_DOUT(DBG_LINE_STATE, ("Init: ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                icr3Vals[0], icr3Vals[1], icr3Vals[2], icr3Vals[3], ecAll));

            /*
             * Wait at least 5ms before turning the switchers on for Vref to
             * stabilize. We'll wait 10ms to be safe.
             */

            VpSysWait(80);

            /* Enable the switchers in med power */
            data = VP880_SWY_LP | VP880_SWZ_LP;
            VpMpiCmd(deviceId, ecVal[0], VP880_REGULATOR_CTRL_WRT,
                VP880_REGULATOR_CTRL_LEN, &data);
            }
            break;
    }

    /*
     * Check for High Voltage Device and line test switch
     * Currently high voltage and test switch go hand in hand but may not in
     * the future that is why there are two bits but only a test for one.
     */
    if ((devicePcn & VP880_HV_MASK) == VP880_HV_MASK) {
        pDevObj->stateInt |= VP880_IS_HIGH_VOLTAGE;
        /* VE8820 does not support a test load but is high voltage */
        if (devicePcn != VP880_DEV_PCN_VE8820) {
            pDevObj->stateInt |= VP880_HAS_TEST_LOAD_SWITCH;
        }
    } else {
        pDevObj->stateInt &= ~VP880_IS_HIGH_VOLTAGE;
        pDevObj->stateInt &= ~VP880_HAS_TEST_LOAD_SWITCH;
    }

    /* Check for devices that have an FXO on line 0 and/or line 1*/
    switch (devicePcn) {
        case VP880_DEV_PCN_88020:
        case VP880_DEV_PCN_88026:
            pDevObj->stateInt |= VP880_LINE1_IS_FXO;

        case VP880_DEV_PCN_88010:
        case VP880_DEV_PCN_88016:
        case VP880_DEV_PCN_88320:
        case VP880_DEV_PCN_88321:
        case VP880_DEV_PCN_88326:
        case VP880_DEV_PCN_88340:
        case VP880_DEV_PCN_88341:
        case VP880_DEV_PCN_88346:

        case VP880_DEV_PCN_88310:
        case VP880_DEV_PCN_88311:
        case VP880_DEV_PCN_88316:
        case VP880_DEV_PCN_88330:
        case VP880_DEV_PCN_88331:
        case VP880_DEV_PCN_88336:
            pDevObj->stateInt |= VP880_LINE0_IS_FXO;
            break;

        default:
            break;
    }

    /* Check for Wideband Mode support */
    if ((devicePcn == VP880_DEV_PCN_VE8820) ||
        ((devicePcn & (VP880_CODEC_MASK | VP880_WIDEBAND_MASK))
         == (VP880_CODEC_MASK | VP880_WIDEBAND_MASK))) {
        pDevObj->stateInt |= VP880_WIDEBAND;
    }

    /* Check for Cal Circuit */
    if (VP880_REV_VA == deviceRcn) {
        /* none of the rev 1 devices have cal circuit */
        pDevObj->stateInt &= ~VP880_HAS_CALIBRATE_CIRCUIT;

    } else if (VP880_REV_VC == deviceRcn) {
        if (pDevObj->stateInt & VP880_IS_SINGLE_CHANNEL) {
            /* none of the single channel rev 2 devices have a cal circuit */
            pDevObj->stateInt &= ~VP880_HAS_CALIBRATE_CIRCUIT;
        } else {
            pDevObj->stateInt |= VP880_HAS_CALIBRATE_CIRCUIT;
        }
    } else {
        /* all other revs should have cal circuit */
        pDevObj->stateInt |= VP880_HAS_CALIBRATE_CIRCUIT;
    }

    /* Initialize each channel */
    for (chan = 0; chan < maxChan; chan++) {
        /*
         * For Init Line to work, the device cannot be non-initialized because
         * the init line function tries to set the line state.  Therefore,
         * temporarily set the device init flag to TRUE then immediately after
         * line init, set back to FALSE until device init is complete
         */
        pLineCtx = pDevCtx->pLineCtx[chan];
        if (pLineCtx != VP_NULL) {
            pLineObj = pLineCtx->pLineObj;

            if (pLineObj->status & VP880_IS_FXO) {
                status = Vp880InitLine(pLineCtx, pFxoAcProfile, pFxoCfgProfile,
                    VP_PTABLE_NULL);
            } else {
                status = Vp880InitLine(pLineCtx, pAcProfile, pDcProfile,
                    pRingProfile);
#ifdef VP880_INCLUDE_TESTLINE_CODE
                /* initialize the calibration coeffs */
                pDevObj->calOffsets[chan].nullOffset = 0;
                pDevObj->calOffsets[chan].vabOffset = 0;
                pDevObj->calOffsets[chan].vahOffset = 0;
                pDevObj->calOffsets[chan].vbhOffset = 0;
#endif /* VP880_INCLUDE_TESTLINE_CODE */
            }

            if (status != VP_STATUS_SUCCESS) {
                pDevObj->status.state &=
                    ~(VP_DEV_INIT_IN_PROGRESS | VP_DEV_INIT_CMP);

                VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
                return status;
            }
        }
    }

    status = VpImplementDefaultSettings(pDevCtx, VP_NULL);

    /*
     * This clears the Init Line Events and any other erroneous event that
     * may have been created due to initialization
     */
    Vp880FlushEvents(pDevCtx);

#ifdef VP880_INCLUDE_TESTLINE_CODE
    /*
     * This clears the Test structure
     */
    pDevObj->currentTest.prepared = FALSE;
    pDevObj->currentTest.testState = -1;
    pDevObj->currentTest.testId = VP_NUM_TEST_IDS;
#endif /* VP880_INCLUDE_TESTLINE_CODE */

    /*
     * Set a Device Init Complete Event if status is succesfull and it's
     * silicon revision is < VC.
     */
    if (status == VP_STATUS_SUCCESS) {
        if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC) {
            VP_DOUT(VP_DBG_INFO, ("\n\rCal Not Required: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));

            pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
            pDevObj->status.state &= ~VP_DEV_IN_CAL;
        } else {
            /*
             * This forces the initial I/O setting to set an external
             * relay if present and masks an I/O1 relay driver issue.
             */
            data = VP880_IODATA_IO1;
            VP_DOUT(VP_DBG_INFO, ("4. Write IODATA 0x%02X on Both Channels",
                VP880_IODATA_IO1));
            VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_IODATA_REG_WRT,
                VP880_IODATA_REG_LEN, &data);

            data = VP880_IODIR_IO1_OUTPUT;
            VP_DOUT(VP_DBG_INFO, ("4. Write IODIR 0x%02X on Both Channels",
                VP880_IODIR_IO1_OUTPUT));
            VpMpiCmd(deviceId, (ecVal[0] | ecVal[1]), VP880_IODIR_REG_WRT,
                VP880_IODIR_REG_LEN, &data);

#ifdef VP880_CAL_ENABLE
            VP_DOUT(DBG_CALIBRATION, ("\n\rCal Required: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));

            pDevObj->status.state |= VP_DEV_IN_CAL;
            pDevObj->calData.calState = VP880_CAL_INIT;

            if (pDevObj->stateInt & VP880_IS_ABS) { /* Start for ABS Device */
                if (Vp880SetCalFlags(pDevObj) == TRUE) {
                    Vp880CalCodecInt(pDevCtx);
                } else {
                    pDevObj->status.state &= ~VP_DEV_IN_CAL;
                    pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
                    pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
                }
            } else {    /* Start for Tracker Device if JE (JA 8827x) silicon */
                if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_JE) {
                    pDevObj->status.state |= VP_DEV_ABV_CAL;
                    Vp880CalCodecInt(pDevCtx);
                } else {
                    pDevObj->status.state &= ~VP_DEV_IN_CAL;
                    pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
                    pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
                }
            }
#else
            VP_DOUT(VP_DBG_INFO, ("\n\rCal Required, Disabled at Compile: Device 0x%04X RCN %d",
                pDevObj->stateInt, pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]));
            pDevObj->status.state &= ~VP_DEV_IN_CAL;
            pDevObj->status.state &= ~(VP_DEV_INIT_IN_PROGRESS);
            pDevObj->deviceEvents.response |= VP_DEV_EVID_DEV_INIT_CMP;
#endif
        }
    }


    /*
     * Success, Failure, or Calibration started -- we're not in "InitDevice"
     * function anymore. So normal rules apply.
     */
    pDevObj->status.state |= VP_DEV_INIT_CMP;

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp880InitDevice */

/**
 * Vp880InitLine
 *  This function initializes a line of a device with the specified parameters
 * and API default values. It is a "Line Reset".
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to be the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 */
VpStatusType
Vp880InitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,    /**< Pointer to AC coefficient data or
                                     * profile index to be applied to this line.
                                     */

    VpProfilePtrType pDcOrFxoProfile,   /**< Pointer to DC Feed (FXS) or Cfg
                                         * (FX0) profile or profile index to be
                                         * applied to this line.
                                         */

    VpProfilePtrType pRingProfile)  /**< Pointer to Ringing profile or profile
                                     * index to apply to this line
                                     */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;

    uint8 alwaysOn[VP880_CADENCE_TIMER_LEN] = {0x3F, 0xFF, 0x00, 0x00};
    uint8 absFix2ndOn[VP880_ICR6_LEN] =  {0x00, 0x02};

#ifdef CSLAC_SEQ_EN
    uint8 seqByte;
#endif

    uint8 ringingParamCount;
    uint8 defaultRingParams[VP880_RINGER_PARAMS_LEN] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    VpProfileDataType dcDefaultProf[] = {
        0x00, 0x01, 0x01, 0x0A, 0x00, 0x08, 0xC2, 0x1B, 0x84, 0xB3, 0x05, 0xC6,
        0x13, 0x08
    };

    VpProfileDataType fxoDefaultProf[] =
    {
        /* FXO/Dialing Profile */
        0x00, 0xFE, 0x00, 0x12, 0x00, 0x00, 0x00, 0x27, 0x00, 0x28, 0x00, 0x78,
        0x0C, 0x08, 0x00, 0x28, 0xEB, 0x79, 0x04, 0x03, 0x26, 0x3A
    };

    /*
     * IO Direction and Control used to restore the device IO to the state
     * set prior to the channel Software Reset
     */
    uint8 ioDirection[VP880_IODIR_REG_LEN];
    uint8 ioData[VP880_IODATA_REG_LEN];
    uint8 swCal[VP880_BAT_CALIBRATION_LEN];

    VpStatusType status = VP_STATUS_SUCCESS;
    uint8 data;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    uint8 fxoCidLine;

#ifdef VP880_CURRENT_LIMIT
    uint8 swyLimit[VP880_ICR2_LEN];
#else
    uint8 opNote4Step1Data[VP880_ICR2_LEN];
#endif

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
     * Clear flag indicating line has not yet been calibrated. This results in
     * SetLineState() NOT using cached calibration values.
     */
    pLineObj->calLineData.calDone = FALSE;

#ifdef CSLAC_SEQ_EN
    for (seqByte = 0; seqByte < VP880_INT_SEQ_LEN; seqByte++) {
        pLineObj->intSequence[seqByte] = 0x00;
    }

    pLineObj->callerId.status = 0x00;
    pLineObj->suspendCid = FALSE;
#endif
    pLineObj->leakyLineCnt = 0;    /* Used only for LP Mode */

    pLineObj->status &= ~(VP880_BAD_LOOP_SUP);

    pLineObj->pRingingCadence = VP_PTABLE_NULL;
    pLineObj->pCidProfileType1 = VP_PTABLE_NULL;
    pLineObj->pCidProfileType2 = VP_PTABLE_NULL;

    /* Initialize cached transmit and receive gains for SetRelGain to 1.0. */
    pLineObj->gain.gxInt = 0x4000;
    pLineObj->gain.grInt = 0x4000;

    /* Inititialize API line state variables */
    if (pLineObj->status & VP880_IS_FXO) {
        pLineObj->lineState.currentState = VP_LINE_FXO_LOOP_OPEN;
        pLineObj->lineState.previous = VP_LINE_FXO_LOOP_OPEN;
    } else {
        pLineObj->lineState.currentState = VP_LINE_DISCONNECT;
        pLineObj->lineState.previous = VP_LINE_DISCONNECT;
    }

    /* Force a line state check */
    pLineObj->lineState.condition = VP_CSLAC_STATUS_INVALID;
    pLineObj->dpStruct.hookSt = FALSE;
    pLineObj->dpStruct2.hookSt = FALSE;

    /*
     * Cache the ICR1 values to avoid read/delay/write operation. It's possible
     * to have a read conflict with the device such that read-modify-write
     * would cause an error. Write only operation does not have this problem.
     */
    pLineObj->icr1Values[0] = 0x00;
    pLineObj->icr1Values[1] = 0x00;
    pLineObj->icr1Values[2] = 0x00;
    pLineObj->icr1Values[3] = 0x00;

    /* Force a codec update */
    pLineObj->codec = VP_NUM_OPTION_CODEC_TYPE_IDS;

    VpInitDP(&pLineObj->dpStruct);
    VpInitDP(&pLineObj->dpStruct2);

    /*
     * Read the IO direction and data for the device IO that will be affected
     * by a software reset. Also read the calibrated battery voltage.
     */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_RD,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_RD,
        VP880_IODATA_REG_LEN, ioData);

    VpMpiCmd(deviceId,ecVal[channelId], VP880_BAT_CALIBRATION_RD,
        VP880_BAT_CALIBRATION_LEN, swCal);

    /* Workaround for I/O1 relay driver. Problem is the I/O1 pin does not have
     * proper voltage clamps to protect against normal voltage spikes that
     * occur as a result of transitioning to Input or Open Drain on a driven
     * relay coil. So this workaround will check to see if the pin is currently
     * being used to close an external relay, and force it open. Then it has
     * to wait for 3ms for the relay to fully open (coil to fully discharge).
     */
    if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] > VP880_REV_VC) {
        if ((ioDirection[0] & VP880_IODIR_IO1_MASK) == VP880_IODIR_IO1_OUTPUT) {
            /* Currently used as output. May be driving a relay closed. */

            if (!(ioData[0] & VP880_IODATA_IO1)) {
                /* Output set to 0, condition for closed relay. Need to open */
                ioData[0] |= VP880_IODATA_IO1;
                VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
                    VP880_IODATA_REG_LEN, ioData);

                /* Wait for relay coil to fully discharge */
                VpSysWait(24);
            }
        }
    }

    /* Software reset the channel */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_SW_RESET_CMD, NO_DATA, &data);
    VpSysWait(3);

    /* Restore the previous device IO direction and control */
    VP_DOUT(VP_DBG_INFO, ("5. Write IODATA 0x%02X on Channel %d",
        ioData[0], channelId));

    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODATA_REG_WRT,
        VP880_IODATA_REG_LEN, ioData);

    VP_DOUT(VP_DBG_INFO, ("3. Write IODIR 0x%02X on Channel %d",
        ioDirection[0], channelId));
    VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
        VP880_IODIR_REG_LEN, ioDirection);

    VpMpiCmd(deviceId,ecVal[channelId], VP880_BAT_CALIBRATION_WRT,
        VP880_BAT_CALIBRATION_LEN, swCal);

    /*
     * Operating Conditions - Remove all loopbacks, connect TX/RX PCM Hwy
     * Note that TX/RX PCM Highway is set when Set Linestate function is
     * called.
     */
    data = VP880_NORMAL_OP_COND_MODE;
    VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_COND_WRT, VP880_OP_COND_LEN,
        &data);

    /* Disable the internal device cadencer .. done in the API */
    VpMpiCmd(deviceId, ecVal[channelId], VP880_CADENCE_TIMER_WRT,
        VP880_CADENCE_TIMER_LEN, alwaysOn);

    /* Start the channel out in the standby state or loop open (if FXO)  */
    if (pLineObj->status & VP880_IS_FXO) {
        pLineObj->lineTimers.type = VP_CSLAC_FXO_TIMER;

        /* Disable auto system state control */
        data = VP880_AUTO_SSC_DIS;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
            VP880_SS_CONFIG_LEN, &data);

        if (pLineObj->termType == VP_TERM_FXO_DISC) {
            fxoCidLine = VP880_IODATA_IO3;
        } else {
            fxoCidLine = VP880_FXO_CID_LINE;
        }

        data &= ~VP880_IODIR_IO1_MASK;
        data = (VP880_IODIR_IO1_OUTPUT | (fxoCidLine << 1));
        VpMpiCmd(deviceId, ecVal[channelId], VP880_IODIR_REG_WRT,
            VP880_IODIR_REG_LEN, &data);

        pLineObj->digitGenStruct.dtmfOnTime = VP_FXO_DTMF_ON_DEFAULT;
        pLineObj->digitGenStruct.dtmfOffTime = VP_FXO_DTMF_OFF_DEFAULT;
        pLineObj->digitGenStruct.breakTime = VP_FXO_PULSE_BREAK_DEFAULT;
        pLineObj->digitGenStruct.makeTime = VP_FXO_PULSE_MAKE_DEFAULT;
        pLineObj->digitGenStruct.flashTime = VP_FXO_FLASH_HOOK_DEFAULT;
        pLineObj->digitGenStruct.dpInterDigitTime = VP_FXO_INTERDIG_DEFAULT;
        pLineObj->digitGenStruct.dtmfHighFreqLevel[0] = 0x1C;
        pLineObj->digitGenStruct.dtmfHighFreqLevel[1] = 0x32;
        pLineObj->digitGenStruct.dtmfLowFreqLevel[0] = 0x1C;
        pLineObj->digitGenStruct.dtmfLowFreqLevel[1] = 0x32;

        if (pDcOrFxoProfile == VP_NULL) {
            status = Vp880ConfigLine(pLineCtx, pAcProfile, fxoDefaultProf,
                VP_PTABLE_NULL);
        } else {
            status = Vp880ConfigLine(pLineCtx, pAcProfile, pDcOrFxoProfile,
                VP_PTABLE_NULL);
        }

        if (status != VP_STATUS_SUCCESS) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return status;
        }
        /* Activate Codec and enable Supervision */
        Vp880SetLineStateInt(pLineCtx, VP_LINE_FXO_LOOP_OPEN);
        pLineObj->lineState.usrCurrent = VP_LINE_FXO_LOOP_OPEN;
    } else {
        pLineObj->lineTimers.type = VP_CSLAC_FXS_TIMER;

        /* Enable Auto Bat Shutdown and disable Auto State Control */
        data = VP880_AUTO_SSC_DIS;

        if ((pDevObj->stateInt & VP880_IS_ABS) &&
             (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION]
                < VP880_REV_VC)) {

            VpMpiCmd(deviceId, ecVal[pLineObj->channelId], VP880_ICR6_WRT,
                VP880_ICR6_LEN, absFix2ndOn);
        }

        VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
            VP880_SS_CONFIG_LEN, &data);

        pLineObj->onHookTicks = -1;

        /* Complete all other non device senstive items */

        /* Set to Disconnect */
        Vp880SetLineStateInt(pLineCtx, VP_LINE_DISCONNECT);
        pLineObj->lineState.usrCurrent = VP_LINE_DISCONNECT;

        /* Initialize default values for Ringing */
        for (ringingParamCount = 0;
             ringingParamCount < VP880_RINGER_PARAMS_LEN;
             ringingParamCount++) {
            pLineObj->ringingParams[ringingParamCount] =
                defaultRingParams[ringingParamCount];
        }
        pLineObj->status &= ~(VP880_UNBAL_RINGING);

        if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] > VP880_REV_VC) {
            uint8 converterCfg[VP880_CONV_CFG_LEN];
            uint8 deviceMode[VP880_DEV_MODE_LEN];

            /* Set the pcm buffer update rate based on the tickrate */
            if(pDevObj->devProfileData.tickRate <=160) {
                converterCfg[0] = VP880_CC_8KHZ_RATE;
                pDevObj->txBufferDataRate = VP880_CC_8KHZ_RATE;

            } else if(pDevObj->devProfileData.tickRate <=320){
                converterCfg[0] = VP880_CC_4KHZ_RATE;
                pDevObj->txBufferDataRate = VP880_CC_4KHZ_RATE;

            } else if(pDevObj->devProfileData.tickRate <=640){
                converterCfg[0] = VP880_CC_2KHZ_RATE;
                pDevObj->txBufferDataRate = VP880_CC_2KHZ_RATE;

            } else if(pDevObj->devProfileData.tickRate <=1280){
                converterCfg[0] = VP880_CC_1KHZ_RATE;
                pDevObj->txBufferDataRate = VP880_CC_1KHZ_RATE;
            } else {
                converterCfg[0] = VP880_CC_500HZ_RATE;
                pDevObj->txBufferDataRate = VP880_CC_500HZ_RATE;
            }

            VpMpiCmd(deviceId, ecVal[channelId], VP880_CONV_CFG_WRT,
                VP880_CONV_CFG_LEN, converterCfg);

            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_RD,
                VP880_DEV_MODE_LEN, deviceMode);
            deviceMode[0] |= VP880_DEV_MODE_TEST_DATA;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DEV_MODE_WRT,
                VP880_DEV_MODE_LEN, deviceMode);
        }

#ifdef VP880_CURRENT_LIMIT
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
            swyLimit);
        swyLimit[VP880_ICR2_SWY_LIM_INDEX] |= VP880_ICR2_SWY_LIM_CTRL;
        swyLimit[VP880_ICR2_SWY_LIM_INDEX+1] &= ~VP880_ICR2_SWY_LIM_CTRL;

        /*
         * Implement ICR2 workaround for ABS version VA only related to
         * Clare issue.
         */
        if ((pDevObj->stateInt & VP880_IS_ABS) &&
            (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
            swyLimit[VP880_ICR2_MET_SPEED_INDEX]
                |= VP880_ICR2_MET_SPEED_CTRL;
            swyLimit[VP880_ICR2_MET_SPEED_INDEX+1]
                &= ~VP880_ICR2_MET_SPEED_CTRL;
        }
        VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
            swyLimit);
#else
        /*
         * Implement ICR2 workaround for ABS version VA only related to
         * Clare issue.
         */
        if ((pDevObj->stateInt & VP880_IS_ABS) &&
            (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
                opNote4Step1Data);
            opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX]
                |= VP880_ICR2_MET_SPEED_CTRL;
            opNote4Step1Data[VP880_ICR2_MET_SPEED_INDEX+1]
                &= ~VP880_ICR2_MET_SPEED_CTRL;
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
                opNote4Step1Data);
        }
#endif
        if (!(pDevObj->stateInt & VP880_IS_ABS)) {
            /*
             * Tracker Workaround:
             * SW Reset takes this back to "all zeros" so we don't need a
             * read/modify/write.
             */
            uint8 icr3Vals[] = {0x00, 0x00, 0x00, 0x00};
            icr3Vals[VP880_ICR3_LINE_CTRL_INDEX] = VP880_ICR3_VREF_CTRL;
            icr3Vals[VP880_ICR3_LINE_CTRL_INDEX+1] = VP880_ICR3_VREF_CTRL;

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT, VP880_ICR3_LEN,
                icr3Vals);
            VP_DOUT(DBG_LINE_STATE, ("Line Init: ICR3 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                icr3Vals[0], icr3Vals[1], icr3Vals[2], icr3Vals[3], channelId));
        }

        if (pDcOrFxoProfile == VP_PTABLE_NULL) {
            status = Vp880ConfigLine(pLineCtx, pAcProfile, dcDefaultProf,
                pRingProfile);
        } else {
            status = Vp880ConfigLine(pLineCtx, pAcProfile, pDcOrFxoProfile,
                pRingProfile);
        }

        if (status != VP_STATUS_SUCCESS) {
            VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
            return status;
        }
    }

    status = VpImplementDefaultSettings(VP_NULL, pLineCtx);

    /* Post the line init complete event if status is succesfull */
    if (status == VP_STATUS_SUCCESS) {
        pLineObj->lineEvents.response |= VP_LINE_EVID_LINE_INIT_CMP;
    }

#ifdef CSLAC_SEQ_EN
    InitCadenceVars(pLineCtx);
#endif

    InitTimerVars(pLineCtx);

    /* Set to initial Relay State */
    Vp880SetRelayState(pLineCtx, VP_RELAY_NORMAL);

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return status;
} /* Vp880InitLine */

/**
 * Vp880ConfigLine
 *  This function reloads a line of a device with the specified parameters.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  The line pointed to be the line context passed is initialized with the
 * profile data specified.  This function returns the success code if the device
 * associated with this line is initialized.
 */
VpStatusType
Vp880ConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,    /**< Pointer to AC coefficient data or
                                     * profile index to be applied to this line.
                                     */

    VpProfilePtrType pDcOrFxoProfile,   /**< Pointer to DC Feed (FXS) or Cfg
                                         * (FX0) profile or profile index to be
                                         * applied to this line.
                                         */

    VpProfilePtrType pRingProfile)  /**< Pointer to Ringing profile or profile
                                     * index to apply to this line
                                     */
{
    Vp880LineObjectType *pLineObj = pLineCtx->pLineObj;
    VpDevCtxType *pDevCtx = pLineCtx->pDevCtx;
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;

    uint8 ecVal[] = {VP880_EC_CH1, VP880_EC_CH2};
    uint8 channelId = pLineObj->channelId;
    uint8 loopSuperParams, profileIndex;
    uint8 sysStateConfig[VP880_SS_CONFIG_LEN];

    VpProfileDataType *pMpiData;

    VpProfilePtrType pAcProf = VP_PTABLE_NULL;
    VpProfilePtrType pDcFxoCfgProf = VP_PTABLE_NULL;
    VpProfilePtrType pRingProf = VP_PTABLE_NULL;

    int profIndex;
    uint8 ringTypeByte;

    uint8 data, ringingParamCount;
    VpDeviceIdType deviceId = pDevObj->deviceId;

    uint8 opNote4Step1Data[VP880_ICR2_LEN];
    uint8 opNote4Step2Data[VP880_ICR3_LEN];
    uint8 gainCSD[VP880_GR_GAIN_LEN];

    uint8 fxoLoopThreshLmLow[VP880_LOOP_SUP_LEN] = {0x1C, 0xE1, 0x79, 0xEB};
    uint8 fxoLoopThreshLmHigh[VP880_LOOP_SUP_LEN] = {0x19, 0xF4, 0x79, 0xEB};
    uint8 fxoLoopThreshLm[VP880_LOOP_SUP_LEN];
    uint8 fxoLoopThreshCount;

    uint8 vocThresh;
    uint8 icr5Speedup[VP880_ICR5_LEN];

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

    profIndex = VpGetProfileIndex(pAcProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pAcProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_AC, pAcProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pAcProf = pAcProfile;
    } else if (profIndex < VP_CSLAC_AC_PROF_TABLE_SIZE) {
        pAcProf = pDevObj->devProfileTable.pAcProfileTable[profIndex];
        if (!(pDevObj->profEntry.acProfEntry & (0x01 << profIndex))) {
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    profIndex = VpGetProfileIndex(pDcOrFxoProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pDcOrFxoProfile != VP_PTABLE_NULL) {
            if (pLineObj->status & VP880_IS_FXO) {
                if (VpVerifyProfileType(VP_PROFILE_FXO_CONFIG, pDcOrFxoProfile)
                     != TRUE) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                if (VpVerifyProfileType(VP_PROFILE_DC, pDcOrFxoProfile)
                    != TRUE) {
                    return VP_STATUS_ERR_PROFILE;
                }
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pDcFxoCfgProf = pDcOrFxoProfile;
    } else {
        if (pLineObj->status & VP880_IS_FXO) {
            if (profIndex < VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE) {
                pDcFxoCfgProf =
                    pDevObj->devProfileTable.pFxoConfigProfileTable[profIndex];

                if (!(pDevObj->profEntry.fxoConfigProfEntry
                    & (0x01 << profIndex))) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                return VP_STATUS_ERR_PROFILE;
            }
        } else {
            if (profIndex < VP_CSLAC_DC_PROF_TABLE_SIZE) {
                pDcFxoCfgProf =
                    pDevObj->devProfileTable.pDcProfileTable[profIndex];

                if (!(pDevObj->profEntry.dcProfEntry & (0x01 << profIndex))) {
                    return VP_STATUS_ERR_PROFILE;
                }
            } else {
                return VP_STATUS_ERR_PROFILE;
            }
        }
    }

    profIndex = VpGetProfileIndex(pRingProfile);
    if (profIndex < 0) {
        /*
         * A pointer is passed or VP_PTABLE_NULL.  If it's a pointer, make
         * sure the content is valid for the profile type.
         */
        if (pRingProfile != VP_PTABLE_NULL) {
            if(VpVerifyProfileType(VP_PROFILE_RING, pRingProfile) != TRUE) {
                return VP_STATUS_ERR_PROFILE;
            }
        }
        /* If we're here, it's a valid profile pointer -- even if NULL */
        pRingProf = pRingProfile;
    } else if (profIndex < VP_CSLAC_RINGING_PROF_TABLE_SIZE) {
        pRingProf = pDevObj->devProfileTable.pRingingProfileTable[profIndex];
        if (!(pDevObj->profEntry.ringingProfEntry & (0x01 << profIndex))) {
            return VP_STATUS_ERR_PROFILE;
        }
    } else {
        return VP_STATUS_ERR_PROFILE;
    }

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);

    /* Load AC Coefficients */
    if (pAcProf != VP_PTABLE_NULL) {
        profileIndex = VP_PROFILE_MPI_LEN + 1;
        pMpiData = (VpProfileDataType *)(&pAcProfile[profileIndex]);
        VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
            pAcProfile[VP_PROFILE_MPI_LEN], pMpiData);

        /* Operating Functions - Use loaded coefficients */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_RD,
            VP880_OP_FUNC_LEN, &data);
        data |= VP880_ENABLE_LOADED_COEFFICIENTS;
        VpMpiCmd(deviceId, ecVal[channelId], VP880_OP_FUNC_WRT,
            VP880_OP_FUNC_LEN, &data);

        /* Update cached transmit and receive gains for SetRelGain */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_GX_GAIN_RD,
            VP880_GX_GAIN_LEN, gainCSD);
        pLineObj->gain.gxInt = 0x4000 + VpConvertCsd2Fixed(gainCSD);

        VpMpiCmd(deviceId, ecVal[channelId], VP880_GR_GAIN_RD,
            VP880_GR_GAIN_LEN, gainCSD);
        pLineObj->gain.grInt = VpConvertCsd2Fixed(gainCSD);
    }

    if (pLineObj->status & VP880_IS_FXO) {
        /* Configure an FXO line type */
        if (pDcFxoCfgProf != VP_PTABLE_NULL) {
            /*
             * Force device Ringing Detector unless the minimum frequency is
             * lower than device can support.
             */
            if (pDcFxoCfgProf[VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX_ACT]
             >= VP880_MAX_RING_DET_PERIOD) {
                for (fxoLoopThreshCount = 0;
                    fxoLoopThreshCount < VP880_LOOP_SUP_LEN;
                    fxoLoopThreshCount++) {
                    fxoLoopThreshLm[fxoLoopThreshCount] =
                        fxoLoopThreshLmLow[fxoLoopThreshCount];
                }
            } else {
                for (fxoLoopThreshCount = 0;
                    fxoLoopThreshCount < VP880_LOOP_SUP_LEN;
                    fxoLoopThreshCount++) {
                    fxoLoopThreshLm[fxoLoopThreshCount] =
                        fxoLoopThreshLmHigh[fxoLoopThreshCount];
                }
            }

            profileIndex = VP_FXO_DIALING_PROFILE_DISC_VOLTAGE_MIN;
            loopSuperParams = (pDcFxoCfgProf[profileIndex] << 3);
            loopSuperParams &= 0x38;
            fxoLoopThreshLm[0] &= ~(0x38);
            fxoLoopThreshLm[0] |= loopSuperParams;

            /*
             * Use the profile parameters for Ringing Detect minimum ONLY if
             * the minimum ringing detect frequency is within range of device.
             * Otherwise, force LIU/Ring Detect to 60V.
             */
            if (pDcFxoCfgProf[VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX_ACT]
             < VP880_MAX_RING_DET_PERIOD) {
                profileIndex = VP_FXO_DIALING_PROFILE_RING_VOLTAGE_MIN;
                loopSuperParams =
                    (pDcFxoCfgProf[profileIndex] & VP880_LOOP_SUP_LIU_THRESH_BITS);
                fxoLoopThreshLm[0] &= ~(VP880_LOOP_SUP_LIU_THRESH_BITS);
                fxoLoopThreshLm[0] |= loopSuperParams;
            }

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MIN;
            fxoLoopThreshLm[2] = pDcFxoCfgProf[profileIndex];

            /*
             * Cache the Minimum Ringing Detect Period that is implemented in
             * SW.
             */
            pLineObj->ringDetMin = pDcFxoCfgProf[profileIndex];
            pLineObj->ringDetMin /= 4;

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX_ACT;
            pLineObj->ringDetMax = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_RING_PERIOD_MAX;
            fxoLoopThreshLm[3] = pDcFxoCfgProf[profileIndex];

            if (pLineObj->ringDetMax == 0) {
                pLineObj->ringDetMax = fxoLoopThreshLm[3] / 4;
            }

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_MSB;
            pLineObj->digitGenStruct.dtmfOnTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_ON_LSB;
            pLineObj->digitGenStruct.dtmfOnTime |=  pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_MSB;
            pLineObj->digitGenStruct.dtmfOffTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_DTMF_OFF_LSB;
            pLineObj->digitGenStruct.dtmfOffTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_BREAK;
            pLineObj->digitGenStruct.breakTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_PULSE_MAKE;
            pLineObj->digitGenStruct.makeTime = pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_MSB;
            pLineObj->digitGenStruct.flashTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_FLASH_HOOK_LSB;
            pLineObj->digitGenStruct.flashTime |= pDcFxoCfgProf[profileIndex];

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_MSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                (pDcFxoCfgProf[profileIndex] << 8)&0xFF00;

            profileIndex = VP_FXO_DIALING_PROFILE_INTERDIGIT_LSB;
            pLineObj->digitGenStruct.dpInterDigitTime =
                pDcFxoCfgProf[profileIndex];

            profileIndex = VP_PROFILE_VERSION;
            if (pDcFxoCfgProf[profileIndex] >= VP_CSLAC_FXO_VERSION_DTMF_LEVEL) {
                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_MSB;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[0] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_HIGH_LVL_LSB;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[1] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_MSB;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[0] =
                    pDcFxoCfgProf[profileIndex];

                profileIndex = VP_FXO_DIALING_PROFILE_DTMF_LOW_LVL_LSB;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[1] =
                    pDcFxoCfgProf[profileIndex];
            } else {
                pLineObj->digitGenStruct.dtmfHighFreqLevel[0] = 0x1C;
                pLineObj->digitGenStruct.dtmfHighFreqLevel[1] = 0x32;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[0] = 0x1C;
                pLineObj->digitGenStruct.dtmfLowFreqLevel[1] = 0x32;
            }

            VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_WRT,
                VP880_LOOP_SUP_LEN, fxoLoopThreshLm);
        }

        /* Cache this so we don't have to read it all the time */
        VpMpiCmd(deviceId, ecVal[channelId], VP880_LOOP_SUP_RD,
            VP880_LOOP_SUP_LEN, fxoLoopThreshLm);
        pLineObj->lineTimers.timers.fxoTimer.maxPeriod = fxoLoopThreshLm[3];
    } else {
        /* Configure an FXS line type */

        /* Ringing changed if profile passed */
        if (pRingProf != VP_PTABLE_NULL) {
            /*
             * Ringing Profile May affect the system state register, so read
             * what it is before the profile, and set it back to all values
             * except what can change in the profile
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_RD,
                VP880_SS_CONFIG_LEN, sysStateConfig);

            profileIndex = VP_PROFILE_MPI_LEN + 1;
            pMpiData = (VpProfileDataType *)&pRingProf[profileIndex];
            VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
                pRingProf[VP_PROFILE_MPI_LEN], pMpiData);

            for (ringingParamCount = 0;
                 ringingParamCount < VP880_RINGER_PARAMS_LEN;
                 ringingParamCount++) {
                pLineObj->ringingParams[ringingParamCount] =
                    pRingProf[profileIndex+1+ringingParamCount];
            }
            ringTypeByte = pRingProf[VP_PROFILE_MPI_LEN +
                pRingProf[VP_PROFILE_MPI_LEN] + VP_PROFILE_RING_TYPE_OFFSET];

            pLineObj->status &= ~(VP880_UNBAL_RINGING);
            pLineObj->status |= (ringTypeByte ? VP880_UNBAL_RINGING : 0x0000);

            /*
             * Nothing in this register should be allowed to change, but the
             * Ringing profile may have changed this value to be compatible
             * with other device profiles. So correct it.
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_SS_CONFIG_WRT,
                VP880_SS_CONFIG_LEN, sysStateConfig);
        }

        /* Set Loop Supervision and DC Feed */
        if (pDcFxoCfgProf != VP_PTABLE_NULL) {
            profileIndex = VP_PROFILE_MPI_LEN + 1;
            pMpiData = (VpProfileDataType *)&pDcFxoCfgProf[profileIndex];

            /*
             * Read both ICR2 and ICR3. Clear bits that may be modified by
             * VOC setting and workaround.
             */
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_RD, VP880_ICR2_LEN,
                opNote4Step1Data);

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_RD, VP880_ICR3_LEN,
                opNote4Step2Data);

            VP_DOUT(DBG_LINE_STATE, ("ICR3 Workaround READ: 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                opNote4Step2Data[0], opNote4Step2Data[1], opNote4Step2Data[2],
                opNote4Step2Data[3], channelId));

            if (pDevObj->stateInt & VP880_IS_ABS) { /* ABS Workaround */
                if ((pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] < VP880_REV_VC)) {
                    vocThresh = VP880_VOC_48V;
                } else {
                    vocThresh = VP880_VOC_51V;
                }

                /* If the VOC set >= theshold, disable longitudinal clamps */
                if ((pDcFxoCfgProf[VP880_VOC_PROFILE_POSITION] & VP880_VOC_MASK)
                    >= vocThresh) {
                    opNote4Step2Data[VP880_ICR3_LINE_CTRL_INDEX]
                        &= ~VP880_ICR3_SAT_LIM_25_CTRL;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        |= VP880_ICR3_LONG_UNCLAMP;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX+1]
                        |= VP880_ICR3_LONG_UNCLAMP;
                } else {
                    opNote4Step2Data[VP880_ICR3_LINE_CTRL_INDEX]
                        |= VP880_ICR3_SAT_LIM_25_CTRL;
                    opNote4Step2Data[VP880_ICR3_LINE_CTRL_INDEX+1]
                        &= ~VP880_ICR3_SAT_LIM_25_CTRL;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        &= ~VP880_ICR3_LONG_UNCLAMP;
                }
            } else { /* Tracker Workaround */
#ifndef VP880_CURRENT_LIMIT
                /* Eliminate use of 50V clamp for all conditions */
                opNote4Step1Data[VP880_ICR2_SWY_LIM_INDEX] |=
                    (VP880_ICR2_SWY_LIM_CTRL1 | VP880_ICR2_SWY_LIM_CTRL);
                opNote4Step1Data[VP880_ICR2_SWY_LIM_INDEX+1] |=
                    (VP880_ICR2_SWY_LIM_CTRL1);

                /* If High Voltage Device, fix clamp for 150V also */
                if (pDevObj->stateInt & VP880_IS_HIGH_VOLTAGE) {
                    opNote4Step1Data[VP880_ICR2_SWY_LIM_INDEX+1] |=
                        (VP880_ICR2_SWY_LIM_CTRL);
                }
#endif
                /* If the VOC set > 48V, disable longitudinal clamps */
                if ((pDcFxoCfgProf[VP880_VOC_PROFILE_POSITION] & VP880_VOC_MASK)
                    > VP880_VOC_48V) {
                    /* Disable longitudinal clamps */
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        |= VP880_ICR3_LONG_UNCLAMP;
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX+1]
                        |= VP880_ICR3_LONG_UNCLAMP;

                    /* Remove Workaround from other conditions */
                    opNote4Step2Data[VP880_ICR3_LONG_FIXED_INDEX]
                        &= ~VP880_ICR3_LONG_FIXED;
                } else {
                    /* If the VOC <= 48V, enable longitudinal clamps */
                    opNote4Step2Data[VP880_ICR3_LONG_FIXED_INDEX]
                        |= VP880_ICR3_LONG_FIXED;
                    opNote4Step2Data[VP880_ICR3_LONG_FIXED_INDEX+1]
                        &= ~VP880_ICR3_LONG_FIXED;

                    /* Remove Workaround from other conditions */
                    opNote4Step2Data[VP880_ICR3_LONG_UNCLAMP_INDEX]
                        &= ~VP880_ICR3_LONG_UNCLAMP;
                }
            }
            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR2_WRT, VP880_ICR2_LEN,
                opNote4Step1Data);

            VP_DOUT(DBG_LINE_STATE, ("ICR3 Workaround: 0x%02X 0x%02X 0x%02X 0x%02X Ch %d",
                opNote4Step2Data[0], opNote4Step2Data[1], opNote4Step2Data[2],
                opNote4Step2Data[3], channelId));

            VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR3_WRT, VP880_ICR3_LEN,
                opNote4Step2Data);

            /* Write the Profile Data */
            VpMpiCmd(deviceId, ecVal[channelId], NOOP_CMD,
                pDcFxoCfgProf[VP_PROFILE_MPI_LEN], pMpiData);

            /* To Do: Is this needed for ABS? */
            if (pDevObj->staticInfo.rcnPcn[VP880_RCN_LOCATION] >= VP880_REV_JE) {
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR5_RD,
                    VP880_ICR5_LEN, icr5Speedup);

                icr5Speedup[VP880_ICR5_FEED_HOLD_INDEX]
                    &= ~VP880_ICR5_FEED_HOLD_MASK;

                /* Device value is x + 18mA, so threshold is > 35mA */
                if ((pDcFxoCfgProf[VP880_ILA_PROFILE_POSITION] & VP880_ILA_MASK)
                    > 17) {
                    icr5Speedup[VP880_ICR5_FEED_HOLD_INDEX]
                        |= 0xF0;  /* Hardcode 15ms. */
                } else {
                    icr5Speedup[VP880_ICR5_FEED_HOLD_INDEX]
                        |= 0xA0;  /* Hardcode 10ms. */
                }
                VpMpiCmd(deviceId, ecVal[channelId], VP880_ICR5_WRT,
                    VP880_ICR5_LEN, icr5Speedup);
            }
            /* Copy Feed for Calibration reference */
            VP_DOUT(VP_DBG_INFO, ("Copying DC Feed Reference in VpConfigLine()"));
            VpMpiCmd(deviceId, ecVal[channelId], VP880_DC_FEED_RD,
                VP880_DC_FEED_LEN, pLineObj->calLineData.dcFeedRef);
        }
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);
    return VP_STATUS_SUCCESS;
}

/**
 * Vp880InitProfile()
 *  This function is used to initialize profile tables in Vp880.
 *
 * Preconditions:
 *  The device associated with this line must be initialized.
 *
 * Postconditions:
 *  Stores the given profile at the specified index of the profile table.
 */
VpStatusType
Vp880InitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile)
{
    Vp880DeviceObjectType *pDevObj = pDevCtx->pDevObj;
    VpDeviceIdType deviceId = pDevObj->deviceId;
    VpStatusType status = VP_STATUS_SUCCESS;

    uint8 profIndex8;   /* Used for 8-bit profile table masking */
    uint16 profIndex16; /* Used for 16-bit profile table masking */

    /*
     * If the profile data is an index, indicated by Get Profile Index return
     * value of > -1, return an error (cannot init an indexed entry with an
     * index).
     */
    int profileIndex = VpGetProfileIndex(pProfile);

    if (profileIndex >= 0) {
        return VP_STATUS_INVALID_ARG;
    }

    /*
     * If pProfileIndex is -1, the profile is of pointer type and invalid,
     * otherwise it is an index.  If it's an index, make sure the range is
     * valid.
     */
    profileIndex = VpGetProfileIndex(pProfileIndex);
    if (profileIndex < 0) {
        return VP_STATUS_INVALID_ARG;
    }

    profIndex8 = (uint8)profileIndex;
    profIndex16 = (uint16)profileIndex;

    VpSysEnterCritical(deviceId, VP_CODE_CRITICAL_SEC);
    /*
     * The correct types are passed, but check to make sure the specific profile
     * type being initialized is valid as well as the index value
     */
    switch(type) {
        case VP_PROFILE_DEVICE:
            if (profIndex8 >= VP_CSLAC_DEV_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_DEVICE, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pDevProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.devProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.devProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_AC:
            if (profIndex8 >= VP_CSLAC_AC_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_AC, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pAcProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.acProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.acProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_DC:
            if (profIndex8 >= VP_CSLAC_DC_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_DC, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pDcProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.dcProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.dcProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_RING:
            if (profIndex8 >= VP_CSLAC_RINGING_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_RING, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pRingingProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.ringingProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.ringingProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_RINGCAD:
            if (profIndex8 >= VP_CSLAC_RING_CADENCE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_RINGCAD, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pRingingCadProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.ringCadProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.ringCadProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_TONE:
            if (profIndex16 >= VP_CSLAC_TONE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_TONE, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pToneProfileTable[profIndex16] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.toneProfEntry &=
                            ~(0x01 << profIndex16);
                    } else {
                        pDevObj->profEntry.toneProfEntry |=
                            (0x01 << profIndex16);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_TONECAD:
            if (profIndex16 >= VP_CSLAC_TONE_CADENCE_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_TONECAD, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pToneCadProfileTable[profIndex16] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.toneCadProfEntry &=
                            ~(0x01 << profIndex16);
                    } else {
                        pDevObj->profEntry.toneCadProfEntry |=
                            (0x01 << profIndex16);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_METER:
            if (profIndex8 >= VP_CSLAC_METERING_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_METER, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pMeteringProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.meterProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.meterProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_CID:
            if (profIndex8 >= VP_CSLAC_CALLERID_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_CID, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pCallerIdProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.cidCadProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.cidCadProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        case VP_PROFILE_FXO_CONFIG:
            if (profIndex8 >= VP_CSLAC_FXO_CONFIG_PROF_TABLE_SIZE) {
                status = VP_STATUS_INVALID_ARG;
            } else {
                if(VpVerifyProfileType(VP_PROFILE_FXO_CONFIG, pProfile) == TRUE) {
                    pDevObj->devProfileTable.pFxoConfigProfileTable[profIndex8] =
                        pProfile;
                    /*
                     * If the profile is null, then clear the flag in the
                     * profile entry table to indicate that this profile is no
                     * longer valid.
                     */
                    if (pProfile == VP_PTABLE_NULL) {
                        pDevObj->profEntry.fxoConfigProfEntry &=
                            ~(0x01 << profIndex8);
                    } else {
                        pDevObj->profEntry.fxoConfigProfEntry |=
                            (0x01 << profIndex8);
                    }
                } else {
                    status = VP_STATUS_ERR_PROFILE;
                }
            }
            break;

        default:
            status = VP_STATUS_INVALID_ARG;
            break;
    }

    VpSysExitCritical(deviceId, VP_CODE_CRITICAL_SEC);

    return status;
} /* Vp880InitProfile() */

#endif
