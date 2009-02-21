/*
 * This kernel module will act as a library module for the VP-API.
 * All user accesable functions will be available to other modules
 * via the EXPORT
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

/* linux/module specific headers */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>           /* loacation of file_operations structure*/
#include <asm/uaccess.h>        /* loacation copy_xyz_user*/
#include <linux/slab.h>         /* kmalloc and kfree */

#include "leg_vp_dvr.h"

/* ****************************************************************************
 * This is a union of all possible VP-API Device Object types.
 * The size of the union will only be as large as the largest
 * device object type defined.
 * ****************************************************************************/
typedef union {
    #ifdef VP_CC_580_SERIES
    Vp580DeviceObjectType *pVp580DevObj;
    #endif

    #ifdef VP_CC_790_SERIES
    Vp790DeviceObjectType *pVp790DevObj;
    #endif

    #ifdef VP_CC_880_SERIES
    Vp880DeviceObjectType *pVp880DevObj;
    #endif

    #ifdef VP_CC_890_SERIES
    Vp880DeviceObjectType *pVp890DevObj;
    #endif

    #ifdef VP_CC_VCP_SERIES
    VpVcpDeviceObjectType *pVpVcpDevObj;
    #endif

    #ifdef VP_CC_VCP2_SERIES
    VpVcp2DeviceObjectType *pVpVcp2DevObj;
    #endif

    uint8 dummy; /* preventing an empty union */
} LegVpDvrDevObjType;

/* ****************************************************************************
 * This is a union of all possible VP-API Line Object types.
 * The size of the union will only be as large as the larges
 * line object type defined.
 * ****************************************************************************/
typedef union {
    #ifdef VP_CC_580_SERIES
    Vp580LineObjectType *pVp580LineObj;
    #endif

    #ifdef VP_CC_790_SERIES
    Vp790LineObjectType *pVp790LineObj;
    #endif

    #ifdef VP_CC_880_SERIES
    Vp880LineObjectType *pVp880LineObj;
    #endif

    #ifdef VP_CC_890_SERIES
    Vp890LineObjectType *pVp890LineObj;
    #endif

    #ifdef VP_CC_VCP_SERIES
    VpVcpLineObjectType *pVpVCPLineObj;
    #endif

    #ifdef VP_CC_VCP2_SERIES
    VpVcp2LineObjectType *pVpVCP2LineObj;
    #endif

    uint8 dummy; /* preventing an empty union */
} LegVpDvrLineObjType;

/* ****************************************************************************
 * This struct definition contains all necessary information
 * to access any line or device contained in an instantiation
 * of this module.
 * ****************************************************************************/
typedef struct LegVpDvrInfo {
    int                 major;
    int                 makeComplete;
    int                 lineCount;
    VpDeviceIdType      deviceId;
    VpDeviceType        devType;
    VpDevCtxType        *pDevCtx;
    LegVpDvrDevObjType  *pDevObj;
    /*
     * TODO: come up with a better way of doing this
     * I dont like that (at least on UVB) I am staticly allocating
     * memory for up to 32 lines per device even though I may
     * only use one or two of them. This is a waste of memeory.
     */
    VpLineCtxType       *pLnCtx[VP_MAX_LINES_PER_DEVICE];
    LegVpDvrLineObjType *pLnObj[VP_MAX_LINES_PER_DEVICE];

} LegVpDvrInfoType;

/* Zarlink Semiconductor VP-API Driver Init/Release Functions */
static void LegVpDvrInitDvrInfo         (void);
static int  LegVpDvrParamCheck          (void);
static int  LegVpDvrRegMajorNums        (void);
static void LegVpDvrUnRegMajorNums      (void);
static int  LegVpDvrKmallocDevObjs      (void);
static void LegVpDvrKfree               (void);
/* VP-API System Configuration Functions */
static int LegVpDvrIoctlMkDevObj        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlMkLnObj         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlFreeLnCtx       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetDevInfo      (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetLnInfo       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
/* VP-API Initialization Functions */
static int LegVpDvrIoctlBoot            (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitDev         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitLn          (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlCfgLn           (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlCalCodec        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlCalLn           (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitRing        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitCid         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitMtr         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitCstmTerm    (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlInitProf        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSftRst          (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
/* VP-API Control Functions */
static int LegVpDvrIoctlSetLnSt         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSetLnTn         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSetRlySt        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSetRelGn        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSendSig         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSendCid         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlCntCid          (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlStartMtr        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSetOption       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlToneDetCtrl     (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlDevIoAcc        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlVirtualIsr      (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlApiTick         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlSelfTest        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlFillBuff        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlLowLvlCmd       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
/* VP-API Status and Query Functions */
static int LegVpDvrIoctlGetLnStatus     (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetDevStatus    (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetLpCond       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetOption       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetLnSt         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlFlshEvnt        (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlGetRslt         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlClrRslt         (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlDtmfDetect      (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
static int LegVpDvrIoctlCodeCkSum       (LegVpDvrInfoType *pDvrInfo, unsigned long arg);
/* VP-API Line Tests */
static int LegVpDvrIoctlTestLn          (LegVpDvrInfoType *pDvrInfo, unsigned long arg);

// #ifdef LEG_VP_DVR_RUN_TIME_DEV_LOADING
#if 0

    /* majornum, deviceType, linecount */
    static int dev0_info[3] = {-1, -1, -1};
    static int dev1_info[3] = {-1, -1, -1};
    static int dev2_info[3] = {-1, -1, -1};
    static int dev3_info[3] = {-1, -1, -1};

    /* registering 3 interger parameters with the insmod command */
    MODULE_PARM (dev0_info, "3i");
    MODULE_PARM (dev1_info, "3i");
    MODULE_PARM (dev2_info, "3i");
    MODULE_PARM (dev3_info, "3i");
    #if (LEG_VP_DVR_NUM_DEVS != 4)
        #error (LEG_VP_DVR_NUM_DEVS does not match MODULE_PARMS)
    #endif

    /* add all devX_info arrays to this pointer */
    int *pDev_Info[LEG_VP_DVR_NUM_DEVS] =
        {dev0_info, dev1_info, dev2_info, dev3_info};

    LegVpDvrInfoType gLegVpDvrInfo[LEG_VP_DVR_NUM_DEVS];
#else
    /* User defined section of code */
    static int dev0_info[3] = {-1, -1, -1};

    /* add all devX_info arrays to this pointer */
    int *pDev_Info[LEG_VP_DVR_NUM_DEVS] = {dev0_info};

    LegVpDvrInfoType gLegVpDvrInfo[LEG_VP_DVR_NUM_DEVS];
#endif


/* ****************************************************************************
 * This function initializes each of the gLegVpDvrInfo structs to know values
 * ****************************************************************************/
static void LegVpDvrInitDvrInfo(void)
{
    int dev, line;
    for (dev = 0; dev < LEG_VP_DVR_NUM_DEVS; dev++) {
        gLegVpDvrInfo[dev].major        = -1;
        gLegVpDvrInfo[dev].makeComplete = -1;
        gLegVpDvrInfo[dev].lineCount    = -1;
        gLegVpDvrInfo[dev].deviceId     = -1;
        gLegVpDvrInfo[dev].devType      = VP_DEV_RSRVD1;

        gLegVpDvrInfo[dev].pDevCtx      = VP_NULL;
        gLegVpDvrInfo[dev].pDevObj      = VP_NULL;

        for (line = 0; line < VP_MAX_LINES_PER_DEVICE; line++) {
            gLegVpDvrInfo[dev].pLnCtx[line]     = VP_NULL;
            gLegVpDvrInfo[dev].pLnObj[line]     = VP_NULL;
        }
    }
    return;
}

static void LegVpDvrKfree(void)
{
    int dev, line;
    printk(KERN_ALERT "Freeing VoicePath Driver Memory\n");

    for (dev = 0; dev < LEG_VP_DVR_NUM_DEVS; dev++) {

        /* Only free memory of registered devices */
        if (-1 == gLegVpDvrInfo[dev].major) {
            continue;
        }

        /* free memory for all lines */
        for (line = 0; line < VP_MAX_LINES_PER_DEVICE; line++) {
            /* free the line context memory foreach device */
            if (VP_NULL != gLegVpDvrInfo[dev].pLnCtx[line]) {
                kfree(gLegVpDvrInfo[dev].pLnCtx[line]);
                gLegVpDvrInfo[dev].pLnCtx[line] = VP_NULL;
            }

            /* free the device object memory*/
            if (VP_NULL != gLegVpDvrInfo[dev].pLnObj[line]) {
                kfree(gLegVpDvrInfo[dev].pLnObj[line]);
                gLegVpDvrInfo[dev].pLnObj[line] = VP_NULL;
            }
        }

        /* free the device context memory */
        if (VP_NULL != gLegVpDvrInfo[dev].pDevCtx) {
            kfree(gLegVpDvrInfo[dev].pDevCtx);
            gLegVpDvrInfo[dev].pDevCtx = VP_NULL;
        }

        /* free the device object memory*/
        if (VP_NULL != gLegVpDvrInfo[dev].pDevObj) {
            kfree(gLegVpDvrInfo[dev].pDevObj);
            gLegVpDvrInfo[dev].pDevObj = VP_NULL;
        }
    }
    return;
}
/**#****************************************************************************
 * This function is
 * The driver will only allow this function to be called once after the device
 * has been made once it will return true from then on to an process
 * that attempts to call it again..
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlMkDevObj(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    VpStatusType                status;
    LegVpDvrMkDevObjType        mkDevObjInfo;

    copy_from_user (&mkDevObjInfo, (void*)arg, sizeof(LegVpDvrMkDevObjType));

    /*
     * only perform the make if it has not already been done.
     */

    if (-1 != pDvrInfo->makeComplete) {
        return 0;
    }

    /*
     * The device type cannot be passed in at run time due to
     * the fact that the linux kernel wrapper layer VTD would not
     * allow anything but VP_KWRAP_DEV to be passed in.
     */

    /* Perform the make device object */
    status = VpMakeDeviceObject(
        pDvrInfo->devType,
        mkDevObjInfo.deviceId,
        pDvrInfo->pDevCtx,
        pDvrInfo->pDevObj);

    if (status != VP_STATUS_SUCCESS) {
        printk(KERN_ALERT "\tVpMakeDeviceObject Failed: %i\n", status);
        return -EAGAIN;
    }

    pDvrInfo->makeComplete = 1;
    pDvrInfo->deviceId     = mkDevObjInfo.deviceId;

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &mkDevObjInfo, sizeof(LegVpDvrMkDevObjType));

    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlMkLnObj(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    VpLineCtxType               *pLineCtx;
    LegVpDvrLineObjType         *pLineObj;
    LegVpDvrMkLnObjType         mkLnObjInfo;

    copy_from_user ( &mkLnObjInfo, (void*)arg, sizeof(LegVpDvrMkLnObjType));

    printk(KERN_ALERT "LegVpDvrIoctlMkLnObj: %i, %i, %i\n",
        mkLnObjInfo.status, mkLnObjInfo.termType, mkLnObjInfo.channelId);

    /* make sure the requested channel is supported by the driver */
    if (VP_MAX_LINES_PER_DEVICE < mkLnObjInfo.channelId) {
        return -EINVAL;
    }

    pLineCtx = pDvrInfo->pLnCtx[mkLnObjInfo.channelId];
    pLineObj = pDvrInfo->pLnObj[mkLnObjInfo.channelId];

    /* return if make line object has already been called */
    if (VP_NULL != pLineCtx) {
        return 0;
    }

    /* kmalloc memory for the line context */
    pLineCtx = (VpLineCtxType*)kmalloc(sizeof(VpLineCtxType), GFP_KERNEL);

    /* if the memory allocation fails get out */
    if (VP_NULL == pLineCtx) {
        printk(KERN_ALERT "\tLine Context Memory (%i,%i) not available\n",
            pDvrInfo->major, mkLnObjInfo.channelId);
        return -EAGAIN;
    }

    /* kmalloc memory for the line object */
    pLineObj = (LegVpDvrLineObjType*)kmalloc(sizeof(LegVpDvrLineObjType), GFP_KERNEL);

    /* if the memory allocation fails get out */
    if (VP_NULL == pLineObj) {
        printk(KERN_ALERT "\tLine Object Memory (%i,%i) not available\n",
            pDvrInfo->major, mkLnObjInfo.channelId);
        kfree(pLineCtx);
        pLineCtx = VP_NULL;
        return -EAGAIN;
    }

    /* perform the make line object */
    mkLnObjInfo.status = VpMakeLineObject(
        mkLnObjInfo.termType,
        mkLnObjInfo.channelId,
        pLineCtx,
        pLineObj,
        pDvrInfo->pDevCtx);

    /* if the make line object fails then we need to free up the memory */
    if (VP_STATUS_SUCCESS != mkLnObjInfo.status) {
        printk(KERN_ALERT "VpMakeLineObject Failed. Freeing memory: %i\n", mkLnObjInfo.status);
        kfree(pLineCtx);
        kfree(pLineObj);
        pLineCtx = VP_NULL;
        pLineObj = VP_NULL;
    }

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &mkLnObjInfo, sizeof(LegVpDvrMkLnObjType));

    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlFreeLnCtx(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrFreeLnCtxType   freeLnCtxInfo;
    VpLineCtxType           *pLineCtx;
    LegVpDvrLineObjType     *pLineObj;
    VpStatusType            status = VP_STATUS_SUCCESS;
    int                     result = 0;

    copy_from_user ( &freeLnCtxInfo, (void*)arg, sizeof(LegVpDvrFreeLnCtxType));

    pLineCtx = pDvrInfo->pLnCtx[freeLnCtxInfo.channelId];
    pLineObj = pDvrInfo->pLnObj[freeLnCtxInfo.channelId];

    /* perform the free line context */
    status = VpFreeLineCtx(pLineCtx);

    /*
     * if the free line context is a success then we need to free up the
     * memory associated with the line and set it to VP_NULL;
     */
    if (VP_STATUS_SUCCESS == status) {
        kfree(pLineCtx);
        kfree(pLineObj);

        pLineCtx = VP_NULL;
        pLineObj = VP_NULL;
    }

    freeLnCtxInfo.status = status;
    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &freeLnCtxInfo, sizeof(LegVpDvrFreeLnCtxType));
    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetDevInfo(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetDevInfoType  getDevInfo;
    int                     result = 0;

    copy_from_user (&getDevInfo, (void*)arg, sizeof(LegVpDvrGetDevInfoType));

    getDevInfo.status = VpGetDeviceInfo(
        getDevInfo.pDeviceInfo);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getDevInfo, sizeof(LegVpDvrGetDevInfoType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetLnInfo(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetLnInfoType   getLnInfo;
    int                     result = 0;

    copy_from_user (&getLnInfo, (void*)arg, sizeof(LegVpDvrGetLnInfoType));

    getLnInfo.status = VpGetLineInfo(
        getLnInfo.pLineInfo);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getLnInfo, sizeof(LegVpDvrGetLnInfoType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlBoot(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrBootType        bootInfo;
    int                     result = 0;

    copy_from_user (&bootInfo, (void*)arg, sizeof(LegVpDvrBootType));

    bootInfo.status = VpBootLoad(
        pDvrInfo->pDevCtx,
        bootInfo.state,
        bootInfo.pImageBuffer,
        bootInfo.bufferSize,
        bootInfo.pScratchMem,
        bootInfo.validation);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &bootInfo, sizeof(LegVpDvrBootType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitDev(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitDevType     initDevInfo;
    int                     result = 0;

    copy_from_user (&initDevInfo, (void*)arg, sizeof(LegVpDvrInitDevType));

    initDevInfo.status = VpInitDevice(
        pDvrInfo->pDevCtx,
        initDevInfo.pDevProfile,
        initDevInfo.pAcProfile,
        initDevInfo.pDcProfile,
        initDevInfo.pRIngProfile,
        initDevInfo.pFxoAcProfile,
        initDevInfo.pFxoCfgProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initDevInfo, sizeof(LegVpDvrInitDevType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitLn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitLnType      initLnInfo;
    int                     result = 0;

    copy_from_user (&initLnInfo, (void*)arg, sizeof(LegVpDvrInitLnType));

    initLnInfo.status = VpInitLine(
        pDvrInfo->pLnCtx[initLnInfo.channelId],
        initLnInfo.pAcProfile,
        initLnInfo.pDcOrFxoProfile,
        initLnInfo.pRIngProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initLnInfo, sizeof(LegVpDvrInitLnType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlCfgLn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrCfgLnType       cfgLnInfo;
    int                     result = 0;

    copy_from_user (&cfgLnInfo, (void*)arg, sizeof(LegVpDvrCfgLnType));

    cfgLnInfo.status = VpConfigLine(
        pDvrInfo->pLnCtx[cfgLnInfo.channelId],
        cfgLnInfo.pAcProfile,
        cfgLnInfo.pDcOrFxoProfile,
        cfgLnInfo.pRIngProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &cfgLnInfo, sizeof(LegVpDvrCfgLnType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlCalCodec(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrCalCodecType    calCodecInfo;
    int                     result = 0;

    copy_from_user (&calCodecInfo, (void*)arg, sizeof(LegVpDvrCalCodecType));

    calCodecInfo.status = VpCalCodec(
        pDvrInfo->pLnCtx[calCodecInfo.channelId],
        calCodecInfo.mode);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &calCodecInfo, sizeof(LegVpDvrCalCodecType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlCalLn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrCalLnType       calLnInfo;
    VpLineCtxType           *pLineCtx;
    int                     result = 0;

    copy_from_user (&calLnInfo, (void*)arg, sizeof(LegVpDvrCalLnType));

    pLineCtx = pDvrInfo->pLnCtx[calLnInfo.channelId];

    calLnInfo.status = VpCalLine(pLineCtx);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &calLnInfo, sizeof(LegVpDvrCalLnType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitRing(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitRingType    initRingInfo;
    int                     result = 0;

    copy_from_user (&initRingInfo, (void*)arg, sizeof(LegVpDvrInitRingType));

    initRingInfo.status = VpInitRing(
        pDvrInfo->pLnCtx[initRingInfo.channelId],
        initRingInfo.pCadProfile,
        initRingInfo.pCidProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initRingInfo, sizeof(LegVpDvrInitRingType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitCid(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitCidType     initCidInfo;
    int                     result = 0;

    copy_from_user (&initCidInfo, (void*)arg, sizeof(LegVpDvrInitCidType));

    initCidInfo.status = VpInitCid(
        pDvrInfo->pLnCtx[initCidInfo.channelId],
        initCidInfo.length,
        initCidInfo.pCidData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initCidInfo, sizeof(LegVpDvrInitCidType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitMtr(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitMtrType     initMtrInfo;
    int                     result = 0;

    copy_from_user (&initMtrInfo, (void*)arg, sizeof(LegVpDvrInitMtrType));

    initMtrInfo.status = VpInitMeter(
        pDvrInfo->pLnCtx[initMtrInfo.channelId],
        initMtrInfo.pMeterProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initMtrInfo, sizeof(LegVpDvrInitMtrType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitCstmTerm(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlInitProf(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrInitProfType    initProfInfo;
    int                     result = 0;

    copy_from_user (&initProfInfo, (void*)arg, sizeof(LegVpDvrInitProfType));

    initProfInfo.status = VpInitProfile(
        pDvrInfo->pDevCtx,
        initProfInfo.type,
        initProfInfo.pProfileIndex,
        initProfInfo.pProfile);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &initProfInfo, sizeof(LegVpDvrInitProfType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSftRst(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSftRstType      sftRstInfo;
    int                     result = 0;

    copy_from_user (&sftRstInfo, (void*)arg, sizeof(LegVpDvrSftRstType));

    sftRstInfo.status = VpSoftReset(pDvrInfo->pDevCtx);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &sftRstInfo, sizeof(LegVpDvrSftRstType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSetLnSt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSetLnStType     setLnStInfo;
    int                     result = 0;

    copy_from_user (&setLnStInfo, (void*)arg, sizeof(LegVpDvrSetLnStType));

    setLnStInfo.status =  VpSetLineState(
        pDvrInfo->pLnCtx[setLnStInfo.channelId],
        setLnStInfo.state);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &setLnStInfo, sizeof(LegVpDvrSetLnStType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSetLnTn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSetLnTnType     setLnTnInfo;
    int                     result = 0;

    copy_from_user (&setLnTnInfo, (void*)arg, sizeof(LegVpDvrSetLnTnType));

    setLnTnInfo.status =  VpSetLineTone(
        pDvrInfo->pLnCtx[setLnTnInfo.channelId],
        setLnTnInfo.pToneProfile,
        setLnTnInfo.pCadProfile,
        setLnTnInfo.pDtmfControl);


    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &setLnTnInfo, sizeof(LegVpDvrSetLnTnType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSetRlySt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSetRlyStType    setRlyStInfo;
    int                     result = 0;

    copy_from_user (&setRlyStInfo, (void*)arg, sizeof(LegVpDvrSetRlyStType));

    setRlyStInfo.status = VpSetRelayState(
        pDvrInfo->pLnCtx[setRlyStInfo.channelId],
        setRlyStInfo.rState);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &setRlyStInfo, sizeof(LegVpDvrSetRlyStType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSetRelGn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSetRelGnType    setRelGn;
    int                     result = 0;

    copy_from_user (&setRelGn, (void*)arg, sizeof(LegVpDvrSetRelGnType));

    setRelGn.status = VpSetRelGain(
        pDvrInfo->pLnCtx[setRelGn.channelId],
        setRelGn.txLevel,
        setRelGn.rxLevel,
        setRelGn.handle);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &setRelGn, sizeof(LegVpDvrSetRelGnType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSendSig(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSendSigType     sendSigInfo;
    int                     result = 0;

    copy_from_user (&sendSigInfo, (void*)arg, sizeof(LegVpDvrSendSigType));

    sendSigInfo.status = VpSendSignal(
        pDvrInfo->pLnCtx[sendSigInfo.channelId],
        sendSigInfo.signalType,
        sendSigInfo.pSignalData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &sendSigInfo, sizeof(LegVpDvrSendSigType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSendCid(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSendCidType     sendCidInfo;
    int                     result = 0;

    copy_from_user (&sendCidInfo, (void*)arg, sizeof(LegVpDvrSendCidType));

    sendCidInfo.status = VpSendCid(
        pDvrInfo->pLnCtx[sendCidInfo.channelId],
        sendCidInfo.length,
        sendCidInfo.pCidProfile,
        sendCidInfo.pCidData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &sendCidInfo, sizeof(LegVpDvrSendCidType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlCntCid(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrCntCidType      cntCidInfo;
    int                     result = 0;

    copy_from_user (&cntCidInfo, (void*)arg, sizeof(LegVpDvrCntCidType));

    cntCidInfo.status = VpContinueCid(
        pDvrInfo->pLnCtx[cntCidInfo.channelId],
        cntCidInfo.length,
        cntCidInfo.pCidData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &cntCidInfo, sizeof(LegVpDvrCntCidType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlStartMtr(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrStartMtrType    startMtrInfo;
    int                     result = 0;

    copy_from_user (&startMtrInfo, (void*)arg, sizeof(LegVpDvrStartMtrType));

    startMtrInfo.status = VpStartMeter(
        pDvrInfo->pLnCtx[startMtrInfo.channelId],
        startMtrInfo.onTime,
        startMtrInfo.offTime,
        startMtrInfo.numMeters);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &startMtrInfo, sizeof(LegVpDvrStartMtrType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSetOption(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSetOptionType   setOptionInfo;
    int                     result = 0;

    copy_from_user (&setOptionInfo, (void*)arg, sizeof(LegVpDvrSetOptionType));

    /* TODO:  Need an member in the SetOption struct to indicate dev or ln ctx */

    if (setOptionInfo.lnRequest) {
        setOptionInfo.status = VpSetOption(
            pDvrInfo->pLnCtx[setOptionInfo.channelId],
            VP_NULL,
            setOptionInfo.option,
            setOptionInfo.pValue);
    } else {
        setOptionInfo.status = VpSetOption(
            VP_NULL,
            pDvrInfo->pDevCtx,
            setOptionInfo.option,
            setOptionInfo.pValue);
    }

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &setOptionInfo, sizeof(LegVpDvrSetOptionType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlToneDetCtrl(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlDevIoAcc(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrDevIoAccType    devIoAccInfo;
    int                     result = 0;

    copy_from_user (&devIoAccInfo, (void*)arg, sizeof(LegVpDvrDevIoAccType));

    devIoAccInfo.status = VpDeviceIoAccess(
        pDvrInfo->pDevCtx,
        devIoAccInfo.pDeviceIoData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &devIoAccInfo, sizeof(LegVpDvrDevIoAccType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlVirtualIsr(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlApiTick(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlSelfTest(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrSelfTestType    selfTestInfo;
    int                     result = 0;

    copy_from_user (&selfTestInfo, (void*)arg, sizeof(LegVpDvrSelfTestType));

    selfTestInfo.status = VpSelfTest(
        pDvrInfo->pLnCtx[selfTestInfo.channelId]);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &selfTestInfo, sizeof(LegVpDvrSelfTestType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlFillBuff(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrFillBuffType    fillBuffInfo;
    int                     result = 0;

    copy_from_user (&fillBuffInfo, (void*)arg, sizeof(LegVpDvrFillBuffType));

    fillBuffInfo.status = VpFillTestBuf(
        pDvrInfo->pLnCtx[fillBuffInfo.channelId],
        fillBuffInfo.length,
        fillBuffInfo.pData);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &fillBuffInfo, sizeof(LegVpDvrFillBuffType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlLowLvlCmd(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrLowLvlCmdType   lowLvlCmdInfo;
    int                     result = 0;

    copy_from_user (&lowLvlCmdInfo, (void*)arg, sizeof(LegVpDvrLowLvlCmdType));

    lowLvlCmdInfo.status = VpLowLevelCmd(
        pDvrInfo->pLnCtx[lowLvlCmdInfo.channelId],
        lowLvlCmdInfo.pCmdData,
        lowLvlCmdInfo.len,
        lowLvlCmdInfo.handle);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &lowLvlCmdInfo, sizeof(LegVpDvrLowLvlCmdType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetLnStatus(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetLnStatusType getLnStatusInfo;
    int                     result = 0;

    copy_from_user (&getLnStatusInfo, (void*)arg, sizeof(LegVpDvrGetLnStatusType));

    getLnStatusInfo.status = VpGetLineStatus(
        pDvrInfo->pLnCtx[getLnStatusInfo.channelId],
        getLnStatusInfo.input,
        getLnStatusInfo.pStatus);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getLnStatusInfo, sizeof(LegVpDvrGetLnStatusType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetDevStatus(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetDevStatusType getDevStatusInfo;
    int                     result = 0;

    copy_from_user (&getDevStatusInfo, (void*)arg, sizeof(LegVpDvrGetDevStatusType));

    getDevStatusInfo.status = VpGetDeviceStatus(
        pDvrInfo->pDevCtx,
        getDevStatusInfo.input,
        getDevStatusInfo.pDeviceStatus);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getDevStatusInfo, sizeof(LegVpDvrGetDevStatusType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetLpCond(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetLpCondType   getLpCondInfo;
    int                     result = 0;

    copy_from_user (&getLpCondInfo, (void*)arg, sizeof(LegVpDvrGetLpCondType));

    getLpCondInfo.status = VpGetLoopCond(
        pDvrInfo->pLnCtx[getLpCondInfo.channelId],
        getLpCondInfo.handle);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getLpCondInfo, sizeof(LegVpDvrGetLpCondType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetOption(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetOptionType   getOptionInfo;
    int                     result = 0;

    copy_from_user (&getOptionInfo, (void*)arg, sizeof(LegVpDvrGetOptionType));

    if (getOptionInfo.lnRequest) {
        getOptionInfo.status = VpGetOption(
            pDvrInfo->pLnCtx[getOptionInfo.channelId],
            VP_NULL,
            getOptionInfo.option,
            getOptionInfo.handle);
    } else {
        getOptionInfo.status = VpGetOption(
            VP_NULL,
            pDvrInfo->pDevCtx,
            getOptionInfo.option,
            getOptionInfo.handle);
    }

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getOptionInfo, sizeof(LegVpDvrGetOptionType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetLnSt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetLnStType     getLnStInfo;
    int                     result = 0;

    copy_from_user (&getLnStInfo, (void*)arg, sizeof(LegVpDvrGetLnStType));

    getLnStInfo.status = VpGetLineState(
        pDvrInfo->pLnCtx[getLnStInfo.channelId],
        getLnStInfo.pCurrentState);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getLnStInfo, sizeof(LegVpDvrGetLnStType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlFlshEvnt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrFlshEvntType    flshEvntInfo;
    int                     result = 0;

    copy_from_user (&flshEvntInfo, (void*)arg, sizeof(LegVpDvrFlshEvntType));

    flshEvntInfo.status = VpFlushEvents(pDvrInfo->pDevCtx);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &flshEvntInfo, sizeof(LegVpDvrFlshEvntType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlGetRslt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrGetRsltType     getRsltInfo;
    int                     result = 0;

    copy_from_user (&getRsltInfo, (void*)arg, sizeof(LegVpDvrGetRsltType));

    getRsltInfo.status = VpGetResults(
        getRsltInfo.pEvent,
        getRsltInfo.pResults);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &getRsltInfo, sizeof(LegVpDvrGetRsltType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlClrRslt(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrClrRsltType     clrRsltInfo;
    int                     result = 0;

    copy_from_user (&clrRsltInfo, (void*)arg, sizeof(LegVpDvrClrRsltType));

    clrRsltInfo.status = VpClearResults(pDvrInfo->pDevCtx);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &clrRsltInfo, sizeof(LegVpDvrClrRsltType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlDtmfDetect(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    return 0;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlCodeCkSum(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrCodeCkSumType   codeCkSumInfo;
    int                     result = 0;

    copy_from_user (&codeCkSumInfo, (void*)arg, sizeof(LegVpDvrCodeCkSumType));

    codeCkSumInfo.status = VpCodeCheckSum(
        pDvrInfo->pDevCtx,
        codeCkSumInfo.handle);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &codeCkSumInfo, sizeof(LegVpDvrCodeCkSumType));

    return result;
}

/**#****************************************************************************
 * This function ...
 *
 * \param[in]   pDvrInfo            is a pointer to a driver info structure
 *                                  which was stored in the file pointer's
 *                                  private_data member when the driver was
 *                                  opened.
 *
 * \param[in,out]   *arg            void pointer to the ??? structure.
 *
 * \return      Returns an error code, or a 0 on success.
 *
 *
 * \ingroup FUNCTION_MESSAGE
 **#****************************************************************************/
static int LegVpDvrIoctlTestLn(
    LegVpDvrInfoType *pDvrInfo,
    unsigned long arg)
{
    LegVpDvrTestLnType      testLnInfo;
    int                     result = 0;

    copy_from_user (&testLnInfo, (void*)arg, sizeof(LegVpDvrTestLnType));

    testLnInfo.status = VpTestLine(
        pDvrInfo->pLnCtx[testLnInfo.channelId],
        testLnInfo.test,
        testLnInfo.pArgs,
        testLnInfo.handle);

    /* need to do some error checking on this step */
    copy_to_user((void*)arg, &testLnInfo, sizeof(LegVpDvrTestLnType));

    return result;
}


/*V**************************** Driver Interface Code ************************V*/

static int LegVpDvrOpen(struct inode *pInode, struct file *pFile);
static int LegVpDvrRelease(struct inode *pInode, struct file *pFile);
static int LegVpDvrIoctl (struct inode *pInode, struct file *pFile, unsigned int, unsigned long);

static struct file_operations leg_vp_dvr_fops = {
    owner:  THIS_MODULE,
    llseek: NULL,
    read:   NULL,
    write:  NULL,
    poll:   NULL,
    ioctl:  LegVpDvrIoctl,
    open:   LegVpDvrOpen,
    release:LegVpDvrRelease,
    fasync: NULL
};

/* ****************************************************************************
 * This function implements the drivers ioctl interface.
 * ****************************************************************************/
static int LegVpDvrIoctl (
    struct inode *pInode,
    struct file *pfile,
    unsigned int cmd,
    unsigned long arg)
{
    int result = 0;
    LegVpDvrInfoType *pDvrInfo = pfile->private_data;

    /* Argument checking */

    switch ( cmd ) {
        /* VP-API System Configuration Functions */
        case LEG_VP_DVR_IOCTL_MK_DEV_OBJ:
            result = LegVpDvrIoctlMkDevObj(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_MK_LN_OBJ:
            result = LegVpDvrIoctlMkLnObj(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_FREE_LN_CTX:
            result = LegVpDvrIoctlFreeLnCtx(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_DEV_INFO:
            result = LegVpDvrIoctlGetDevInfo(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_LN_INFO:
            result = LegVpDvrIoctlGetLnInfo(pDvrInfo, arg);
            break;

        /* VP-API Initialization Functions */
        case LEG_VP_DVR_IOCTL_BOOT:
            result = LegVpDvrIoctlBoot(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_DEV:
            result = LegVpDvrIoctlInitDev(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_LN:
            result = LegVpDvrIoctlInitLn(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CFG_LN:
            result = LegVpDvrIoctlCfgLn(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CAL_CODEC:
            result = LegVpDvrIoctlCalCodec(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CAL_LN:
            result = LegVpDvrIoctlCalLn(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_RING:
            result = LegVpDvrIoctlInitRing(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_CID:
            result = LegVpDvrIoctlInitCid(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_MTR:
            result = LegVpDvrIoctlInitMtr(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_CSTM_TERM:
            result = LegVpDvrIoctlInitCstmTerm(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_INIT_PROF:
            result = LegVpDvrIoctlInitProf(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SFT_RST:
            result = LegVpDvrIoctlSftRst(pDvrInfo, arg);
            break;

        /* VP-API Control Functions */
        case LEG_VP_DVR_IOCTL_SET_LN_ST:
            result = LegVpDvrIoctlSetLnSt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SET_LN_TN:
            result = LegVpDvrIoctlSetLnTn(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SET_RLY_ST:
            result = LegVpDvrIoctlSetRlySt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SET_REL_GN:
            result = LegVpDvrIoctlSetRelGn(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SEND_SIG:
            result = LegVpDvrIoctlSendSig(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SEND_CID:
            result = LegVpDvrIoctlSendCid(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CNT_CID:
            result = LegVpDvrIoctlCntCid(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_START_MTR:
            result = LegVpDvrIoctlStartMtr(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SET_OPTION:
            result = LegVpDvrIoctlSetOption(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_TONE_DET_CTRL:
            result = LegVpDvrIoctlToneDetCtrl(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_DEV_IO_ACC:
            result = LegVpDvrIoctlDevIoAcc(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_VIRTUAL_ISR:
            result = LegVpDvrIoctlVirtualIsr(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_API_TICK:
            result = LegVpDvrIoctlApiTick(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_SELF_TEST:
            result = LegVpDvrIoctlSelfTest(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_FILL_BUFF:
            result = LegVpDvrIoctlFillBuff(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_LOW_LVL_CMD:
            result = LegVpDvrIoctlLowLvlCmd(pDvrInfo, arg);
            break;

        /* VP-API Status and Query Functions */
        case LEG_VP_DVR_IOCTL_GET_LN_STATUS:
            result = LegVpDvrIoctlGetLnStatus(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_DEV_STATUS:
            result = LegVpDvrIoctlGetDevStatus(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_LP_COND:
            result = LegVpDvrIoctlGetLpCond(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_OPTION:
            result = LegVpDvrIoctlGetOption(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_LN_ST:
            result = LegVpDvrIoctlGetLnSt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_FLSH_EVNT:
            result = LegVpDvrIoctlFlshEvnt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_GET_RSLT:
            result = LegVpDvrIoctlGetRslt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CLR_RSLT:
            result = LegVpDvrIoctlClrRslt(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_DTMF_DETECT:
            result = LegVpDvrIoctlDtmfDetect(pDvrInfo, arg);
            break;
        case LEG_VP_DVR_IOCTL_CODE_CK_SUM:
            result = LegVpDvrIoctlCodeCkSum(pDvrInfo, arg);
            break;

        /* VP-API Line Tests */
        case LEG_VP_DVR_IOCTL_TEST_LN:
            result = LegVpDvrIoctlTestLn(pDvrInfo, arg);
            break;
        default:
            printk(KERN_ALERT "Inappropriate LEG_VP_DVR_IOCTL access %i\n", cmd);
            return -ENOTTY;
    }
    return result;
}

/* ****************************************************************************
 * This function implements the drivers open interface.
 * ****************************************************************************/
static int LegVpDvrOpen(
    struct inode *pInode,
    struct file *pfile)
{
    int idx, major, result;
    LegVpDvrInfoType *pDvrInfo = VP_NULL;
    major = MAJOR(pInode->i_rdev);

    /*
     * figure out which global driver info struct goes with this major number.
     */
    result = -ENODEV;
    for (idx = 0; idx < LEG_VP_DVR_NUM_DEVS; idx++) {
        if (major == gLegVpDvrInfo[idx].major) {
            pDvrInfo= &gLegVpDvrInfo[idx];
            result = 0;
            break;
        }
    }
    /*
     * if the input major number does not match any of the registered
     * major number for this driver then return an error.
     */
    if (0 > result) {
        printk(KERN_ALERT "UnRegistered Zarlink Semiconductor Driver major #: %i\n", major);
        return result;
    }

    /*
     * if the make device object was sucessfull then store the global driver
     * info into the file pointers private data so that it can be used by other
     * functions later.
     */
    if (0 <= result) {
        pfile->private_data = pDvrInfo;
        // MOD_INC_USE_COUNT;
    }

    return result;
}

/* ****************************************************************************
 * This function implements the drivers release interface.
 * ****************************************************************************/
static int LegVpDvrRelease(
    struct inode *pInode,
    struct file *pfile)
{
    // MOD_DEC_USE_COUNT;
    return 0;
}

/* ****************************************************************************
 * This function is called by the OS and registers the Voice Path driver with
 * the system. This function can run in one of two methods.
 *
 * Method 1 (Dynamic):
 * In dynamic mode the device driver accepts parameters from the command line
 * when insmod or modprobe are called. This method allows for dynamic
 * configuration of the driver each time it is initialized. Meaning a different
 * configuration of Device and Line Objects can be initialized each time
 * the driver is insmoded.
 *
 * The input parameters allow an outer app to specify Major Number, Device Type,
 * Line Count and Device Id on 4 separate chip selects:
 * .
 *
 * Method 2 (Static):
 * In static mode the device driver ignores command line parameters and forces
 * the driver to open with a predefined set of Device and Line Objects.
 *
 * The two different modes are compile time options determined by
 * #define VP_MOD_DYNAMIC_DEV_LOADING
 * ****************************************************************************/
static int __init LegVpDvr_init(void)
{
    int result = 0;

    /* Initialize the drivers info structs */
    LegVpDvrInitDvrInfo();

    /* Check the parameters for each of the chip selects */
    result = LegVpDvrParamCheck();
    if ( 0 >  result) goto fail_arg;

    /*
     * Attempt to register the driver on the requested major number.
     * If anything goes wrong the we unregister all of the devices.
     */
    result = LegVpDvrRegMajorNums();
    if ( 0 >  result) goto fail_reg;

    /*
     * Allocate the memory for the specified device object and create it.
     * If anything goes wrong delete any device objects.
     */
    result = LegVpDvrKmallocDevObjs();
    if ( 0 >  result) goto fail_make;

    printk(KERN_ALERT "VoicePath module initialization successful!!!\n");
    return result; /* sucess */

    fail_make:  LegVpDvrKfree();
    fail_reg:   LegVpDvrUnRegMajorNums();
    fail_arg:   return -ENODEV;

    return -ENODEV;

    printk(KERN_ALERT "VoicePath module initialization complete\n");
    return 0;
}

/* ****************************************************************************
 * This function is called by the OS and unregisters the Voice Path driver with
 * the system. This function will call necessary functions to properly
 * close down the driver and free up any resources it may have be using.
 * ****************************************************************************/
static void __exit LegVpDvr_exit(void)
{
    // need to add the code to free the kernel memory for all lines.
    LegVpDvrKfree();
    LegVpDvrUnRegMajorNums();

    printk(KERN_ALERT "Exiting VoicePath Driver\n");
    return;
}


module_init(LegVpDvr_init);
module_exit(LegVpDvr_exit);
MODULE_LICENSE("Proprietary");
MODULE_DESCRIPTION("Zarlink Semiconductor VoicePath Module");

/* ****************************************************************************
 * This function checks the validity of each of the elements in the command
 * line parameters.
 *
 * If any element of an input parameter is still set to its default value then
 * the function assumes the device driver is skipping init for the
 * corresponding chip select.
 *
 * If all elements of an input parameter are legal the function increments
 * the result varaible.
 *
 * If any of the elements fail the parameter check then a -1 is returned.
 *
 * ****************************************************************************/
static int LegVpDvrParamCheck(void)
{
    int result = -ENODEV;
    int i;

    printk(KERN_ALERT "Checking VoicePath: Chip Select parameters\n");
    for (i = 0; i < LEG_VP_DVR_NUM_DEVS; i++) {

        int major       = pDev_Info[i][0];
        int devType     = pDev_Info[i][1];
        int lineCount   = pDev_Info[i][2];

        printk(KERN_ALERT "  Major Number: %i, Device Type %i, Line Count %i...\n",
            major, devType, lineCount);

        if ((major == -1) || (devType == -1) || (lineCount == -1)) {
            /*
             *  If any of the parameters elements were not changed we are
             *  assuming that the chip select is being skipped on purpose.
             */
            printk(KERN_ALERT "\tSkipped\n");
            continue;
        }

        /* if the device type is not supported then stop the init */
        if (VP_DEV_RSRVD1 >= devType || VP_NUM_DEV_TYPES <= devType) {
            printk(KERN_ALERT "\tInvalid Device Type. Aborting Init\n");
            return -ENODEV;
        }

        printk(KERN_ALERT "\tARGS OK\n");
        gLegVpDvrInfo[i].devType = devType;
        gLegVpDvrInfo[i].lineCount = lineCount;
        /* major is not saved until driver registration */
        result = 0;
    }

    return result;
}

/* ****************************************************************************
 * This function attempts to register each of the requested major numbers.
 * If any of the registrations fail the function returns the error -ENODEV.
 * ****************************************************************************/
static int LegVpDvrRegMajorNums(void)
{
    int res;
    int i = 0;

    printk(KERN_ALERT "Registering Voice Path Char Drivers\n");
    for (i = 0; i < LEG_VP_DVR_NUM_DEVS; i++) {
        int major = pDev_Info[i][0];
        /* only attempt to register dev with a non neg value */
        if (major == -1) {
            continue;
        }

        /* attempt the registration */
        if (( res = register_chrdev (major, LEG_VP_DVR_NAME, &leg_vp_dvr_fops)) ) {
            printk(KERN_ALERT "\tMajor Num %i unavailable\n", major);
            return -ENODEV;
        } else {
            printk(KERN_ALERT "\tMajor Num %i registered\n", major);
            gLegVpDvrInfo[i].major = major;
        }
    }
    return 0;
}

/* ****************************************************************************
 * This function unregisters each any device that was successfully registered
 * in the LegVpDvrRegMajorNums() function.
 * ****************************************************************************/
static void LegVpDvrUnRegMajorNums(void)
{
    int i = 0;

    printk(KERN_ALERT "Unregistering Voice Path Char Drivers\n");
    for (i = 0; i < LEG_VP_DVR_NUM_DEVS; i++) {
        int major = gLegVpDvrInfo[i].major;
        if (major != -1) {
            /* unreg the device and set the major num to -1 */
            unregister_chrdev (major, LEG_VP_DVR_NAME);
            gLegVpDvrInfo[i].major =  -1;
         }
    }
    return;
}

/* ****************************************************************************
 * This function attempts to malloc the memory required for each of the possible
 * device objects. If the memory cannot be allocated the function will return
 * an error code. If the MakeLineObject fails the function will return an
 * error code.
 * ****************************************************************************/
static int LegVpDvrKmallocDevObjs(void)
{
    int i = 0;

    printk(KERN_ALERT "Making Device Objects\n");
    for (i = 0; i < LEG_VP_DVR_NUM_DEVS; i++) {
        /* Only declare memory for registered devices */
        if (gLegVpDvrInfo[i].major == -1) {
            continue;
        }

        /* declare memory for device context */
         gLegVpDvrInfo[i].pDevCtx = \
                (VpDevCtxType*)kmalloc(sizeof(VpDevCtxType), GFP_KERNEL);

        /* if the memory allocation fails get out */
        if (VP_NULL == gLegVpDvrInfo[i].pDevCtx) {
            printk(KERN_ALERT "\tDevice Context Memory not available\n");
            return -EAGAIN;
        } else {
            printk(KERN_ALERT "\tDevice Context (%i) memory kmalloced\n", gLegVpDvrInfo[i].devType);
        }

        /* declare memory for device object*/
        gLegVpDvrInfo[i].pDevObj = \
            (LegVpDvrDevObjType*)kmalloc(sizeof(LegVpDvrDevObjType), GFP_KERNEL);

        /* if the memory allocation fails get out */
        if (VP_NULL == gLegVpDvrInfo[i].pDevObj) {
            printk(KERN_ALERT "\tMemory not available\n");
            return -EAGAIN;
        } else {
            printk(KERN_ALERT "\tDevice Object (%i) memory kmalloced\n", gLegVpDvrInfo[i].devType);
        }
    }
    return 0;
}
