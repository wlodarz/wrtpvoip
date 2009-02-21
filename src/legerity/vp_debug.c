/** \file vp_debug.c
 * vp_api.c
 *
 *  This file contains the implementation of VP_DEBUG MACRO.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#include "vp_api_cfg.h"

#ifdef VP_DEBUG

#include "vp_api.h"
#include "vp_debug.h"

uint32 vpDebugSelectMask = VP_CC_DEBUG_SELECT;

static bool
DeviceDebugEnabled(
    uint32 debugSelectMask,
    char *msgTypeStr,
    VpDeviceIdType deviceId)
{
    if (!debugSelectMask)
        return FALSE;

    /* Display the message type and device ID. */
    VpSysDebugPrintf(msgTypeStr);
    VP_PRINT_DEVICE_ID((deviceId));
    VpSysDebugPrintf(": ");
    return TRUE;
}

static bool
LineDebugEnabled(
    uint32 debugSelectMask,
    char *msgTypeStr,
    VpLineIdType lineId)
{
    if (!debugSelectMask)
        return FALSE;

    /* Display the message type and line ID. */
    VpSysDebugPrintf(msgTypeStr);
    VP_PRINT_LINE_ID((lineId));
    VpSysDebugPrintf(": ");
    return TRUE;
}

#ifdef VP_CC_VCP2_SERIES
bool
VpDebugEnabled_VpVcp2DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pDevObj == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }
    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_VpVcp2LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pLineObj == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }
    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_VCP2_SERIES */

#ifdef VP_CC_890_SERIES
bool
VpDebugEnabled_Vp890DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pDevObj == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }
    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_Vp890LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pLineObj == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }
    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_890_SERIES */

bool
VpDebugEnabled_VpDeviceIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpDeviceIdType *pDeviceId)
{
    VP_ASSERT(pDeviceId != VP_NULL);
    return DeviceDebugEnabled(msgType & vpDebugSelectMask, msgTypeStr, *pDeviceId);
}

bool
VpDebugEnabled_VpLineIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineIdType *pLineId)
{
    VP_ASSERT(pLineId != VP_NULL);
    return LineDebugEnabled(msgType & vpDebugSelectMask, msgTypeStr, *pLineId);
}


/*
  * This function returns TRUE if the specified message type is enabled for the specified device.
  * As a side effect, it displays the message type and device ID.
  */
bool
VpDebugEnabled_VpDevCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpDevCtxType *pDevCtx)
{
    if (pDevCtx == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pDevCtx == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }

    switch (pDevCtx->deviceType) {

#ifdef VP_CC_VCP2_SERIES
        case VP_DEV_VCP2_SERIES:
            return VpDebugEnabled_VpVcp2DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            return VpDebugEnabled_Vp890DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
            default:
                return FALSE;
        }
}

/*
  * This function returns TRUE if the specified message type is enabled for the specified line.
  * As a side effect, it displays the message type and line ID.
  */
bool
VpDebugEnabled_VpLineCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineCtxType *pLineCtx)
{
    if (pLineCtx == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pLineCtx == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }

    if (pLineCtx->pDevCtx == VP_NULL) {
        VpSysDebugPrintf(color_fg(red) "(pDevCtx == VP_NULL) ");
        return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
    }

    switch (pLineCtx->pDevCtx->deviceType) {

#ifdef VP_CC_VCP2_SERIES
        case VP_DEV_VCP2_SERIES:
            return VpDebugEnabled_VpVcp2LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            return VpDebugEnabled_Vp890LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
        default:
            return FALSE;
    }
}

bool
VpDebugEnabled_None(
    uint32 msgType,
    char *msgTypeStr,
    void *nothing)
{
    if (!(msgType & vpDebugSelectMask))
        return FALSE;

    VpSysDebugPrintf(msgTypeStr);
    VpSysDebugPrintf(": ");
    return TRUE;
}

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
static const char *
VpGetEnumString(
    const char *strTable[],
    int numStrings,
    int value)
{
    static char buf[7] = "0x0000"; /* not reentrant */
    static const char hex[16] = "0123456789ABCD";

    if ((value < 0) || (value >= numStrings)) {

        /* Can't find a string for the requested value. */
        buf[5] = hex[value & 0xF];
        value >>= 4;
        buf[4] = hex[value & 0xF];
        value >>= 4;
        buf[3] = hex[value & 0xF];
        value >>= 4;
        buf[2] = hex[value & 0xF];
        return buf;
    }

    /* Found the requested string in the table. */
    return strTable[value];
}

const char *
VpGetString_VpStatusType(
    VpStatusType status)
{
    static const char *strTable[VP_STATUS_NUM_TYPES] = {
        "VP_STATUS_SUCCESS",
        "VP_STATUS_FAILURE",
        "VP_STATUS_FUNC_NOT_SUPPORTED",
        "VP_STATUS_INVALID_ARG",
        "VP_STATUS_MAILBOX_BUSY",
        "VP_STATUS_ERR_VTD_CODE",
        "VP_STATUS_OPTION_NOT_SUPPORTED",
        "VP_STATUS_ERR_VERIFY",
        "VP_STATUS_DEVICE_BUSY",
        "VP_STATUS_MAILBOX_EMPTY",
        "VP_STATUS_ERR_MAILBOX_DATA",
        "VP_STATUS_ERR_HBI",
        "VP_STATUS_ERR_IMAGE",
        "VP_STATUS_IN_CRTCL_SECTN",
        "VP_STATUS_DEV_NOT_INITIALIZED",
        "VP_STATUS_ERR_PROFILE",
        "VP_STATUS_INVALID_VOICE_STREAM",
        "VP_STATUS_CUSTOM_TERM_NOT_CFG",
        "VP_STATUS_DEDICATED_PINS"
    };

    return VpGetEnumString(strTable, VP_STATUS_NUM_TYPES, (int)status);
}

#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC) */

#endif /* VP_DEBUG */
