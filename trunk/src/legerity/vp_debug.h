/** \file vp_debug.h
 * vp_debug.h
 *
 * This file contains the configuration and compile time settings for
 * the VoicePath API debug statements.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#ifndef VP_DEBUG_H
#define VP_DEBUG_H

#include "vp_api_cfg.h"

#ifdef VP_DEBUG

#include "vp_api_types.h"
#include "vp_debug_colors.h"
#include "sys_service.h"

/* Macros for displaying VpDeviceIdType and VpLineIdType values.  If you have
   defined these (in vp_api_types.h) as something other than simple integers,
   you should modify the printf format strings as needed: */
#define VP_PRINT_DEVICE_ID(deviceId)  VpSysDebugPrintf(" (deviceId %2.2X)", (int)deviceId)
#define VP_PRINT_LINE_ID(lineId)      VpSysDebugPrintf(" (lineId %d)", (int)lineId)

/* VP-API debug message types: */
#define VP_DBG_ERROR        0x00000001 /* Any error condition */
#define VP_DBG_WARNING      0x00000002 /* Any warning condition */
#define VP_DBG_INFO         0x00000004 /* Un-categorized information */
#define VP_DBG_API_FUNC     0x00000008 /* API function entry/exit (except tick) */
#define VP_DBG_API_FUNC_INT 0x00000010 /* Internal API function entry/exit */
#define VP_DBG_HAL          0x00000020 /* HAL traffic */
#define VP_DBG_SSL          0x00000040 /* SSL function entry/exit */
/* These are not documented in the API-II and should be used for internal
   purposes only. Reserved range (0x00010000 to 0x80000000) */
#define DBG_TEST_FUNC   0x01000000 /* Test I/F function enter and exit */
#define DBG_TEST        0x02000000 /* Verbose test debug */
#define DBG_FXO_FUNC    0x04000000 /* FXO Detection Code */
#define DBG_HOOK        0x08000000 /* Hook Based Code */
#define DBG_SEQUENCER   0x10000000 /* Sequencer Based Code */
#define DBG_LINE_STATE  0x20000000 /* Set Line State Based Code */
#define DBG_CALIBRATION 0x40000000 /* VpCalCodec(), VpCalLine(), VpCal() */
#define DBG_CID         0x80000000 /* Caller ID funcitons */

/* Choose which types of messages to be compiled in.  Only these message
   types will be selectable at runtime (with VP_OPTION_ID_DEBUG_SELECT): */
#define VP_CC_DEBUG_SELECT (VP_DBG_API_FUNC | DBG_CALIBRATION | DBG_LINE_STATE)

/* Colors for debug messages (if VP_DEBUG_COLOR is defined): */
#define VP_DBG_ERROR_COLOR     red
#define VP_DBG_WARNING_COLOR   dark_yellow
#define VP_DBG_INFO_COLOR      bright_white
#define VP_DBG_API_FUNC_COLOR  cyan
#define VP_DBG_HAL_COLOR       blue
#define VP_DBG_SSL_COLOR       green

#ifdef VP_DEBUG_LOC
#define VP_SHOW_LOC() VpSysDebugPrintf(" (" __FILE__ ":%d)", __LINE__)
#else
#define VP_SHOW_LOC()
#endif

/* Macro for displaying various types of values, by calling a type-specific
   function in vp_debug.c. */
#define VP_GETSTR(objType, obj) \
    VpGetString_ ## objType (obj)

#if 0
/* Basic debug output macro: */
#define VP_DOUT_(msgType, msgColor, objType, pObj, printf_args) \
    if (VpDebugEnabled_ ## objType (VP_DBG_ ## msgType, color_fg(msgColor) #msgType, pObj)) { \
        VpSysDebugPrintf printf_args; \
        VP_SHOW_LOC(); \
        VpSysDebugPrintf(color_reset "\n"); \
    }
#else
/* Basic debug output macro: */
#define VP_DOUT_(msgType, msgColor, objType, pObj, printf_args) \
    if (VpDebugEnabled_ ) { \
        VpSysDebugPrintf printf_args; \
        VP_SHOW_LOC(); \
    }
#endif

/* Conditionally compile in various types of debug messages: */
#if (VP_CC_DEBUG_SELECT & VP_DBG_ERROR)
#define VP_ERROR(objType, pObj, printf_args) VP_DOUT_(ERROR, VP_DBG_ERROR_COLOR, objType, pObj, printf_args)
#else
#define VP_ERROR(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_WARNING)
#define VP_WARNING(objType, pObj, printf_args) VP_DOUT_(WARNING, VP_DBG_WARNING_COLOR, objType, pObj, printf_args)
#else
#define VP_WARNING(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_INFO)
#define VP_INFO(objType, pObj, printf_args) VP_DOUT_(INFO, VP_DBG_INFO_COLOR, objType, pObj, printf_args)
#else
#define VP_INFO(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
#define VP_API_FUNC(objType, pObj, printf_args) VP_DOUT_(API_FUNC, VP_DBG_API_FUNC_COLOR, objType, pObj, printf_args)
#else
#define VP_API_FUNC(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_HAL)
#define VP_HAL(objType, pObj, printf_args) VP_DOUT_(HAL, VP_DBG_HAL_COLOR, objType, pObj, printf_args)
#else
#define VP_HAL(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_SSL)
#define VP_SSL(objType, pObj, printf_args) VP_DOUT_(SSL, VP_DBG_SSL_COLOR, objType, pObj, printf_args)
#else
#define VP_SSL(objType, pObj, printf_args)
#endif

/* Basic assert macro: */
//#include <stdlib.h> /* exit() */
#if 0
#define VP_ASSERT(condition) \
    if (!(condition)) { \
        VP_ERROR(None, VP_NULL, ("Assertion failed: %s\n", #condition)); \
        exit(-1); \
    }
#else
#define VP_ASSERT(condition) \
    if (!(condition)) { \
        VP_ERROR(None, VP_NULL, ("Assertion failed: %s\n", #condition)); \
        panic("aaaa"); \
    }
#endif

/* OLD_DEBUG authorize the old debug (1 = on / 0 = off) */
#define OLD_DEBUG 1

#if 0
/* Support old-style debug output: */
#define VP_DOUT(msgType, printf_args) \
    if ((vpDebugSelectMask & msgType) && OLD_DEBUG) { \
        VpSysDebugPrintf(color_fg(yellow) " [%s] ", #msgType); \
        VpSysDebugPrintf printf_args; \
        VP_SHOW_LOC(); \
        VpSysDebugPrintf("\n" color_reset); \
    }
#else
/* Support old-style debug output: */
#define VP_DOUT(msgType, printf_args) \
    if ((vpDebugSelectMask & msgType) && OLD_DEBUG) { \
        VpSysDebugPrintf printf_args; \
        VP_SHOW_LOC(); \
    }
#endif

/* Global debug select mask (for messages that are not specific to a particular
   line or device): */
EXTERN uint32 vpDebugSelectMask;

#if 0
EXTERN bool
VpDebugEnabled_VpVcp2DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_VpVcp2LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2LineObjectType *pLineObj);

EXTERN bool
VpDebugEnabled_Vp890DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_Vp890LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890LineObjectType *pLineObj);
#endif

EXTERN bool
VpDebugEnabled_VpDeviceIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpDeviceIdType *pDeviceId);

EXTERN bool
VpDebugEnabled_VpLineIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineIdType *pLineId);

EXTERN bool
VpDebugEnabled_VpDevCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpDevCtxType *pDevCtx);

EXTERN bool
VpDebugEnabled_VpLineCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineCtxType *pLineCtx);

EXTERN bool
VpDebugEnabled_None(
    uint32 msgType,
    char *msgTypeStr,
    void *nothing);

EXTERN const char *
VpGetString_VpStatusType(
    VpStatusType status);

#else /* !defined(VP_DEBUG) */

/* Compile out all debug messages: */
#define VP_ASSERT(condition)
#define VP_ERROR(objType, pObj, printf_args)
#define VP_WARNING(objType, pObj, printf_args)
#define VP_INFO(objType, pObj, printf_args)
#define VP_API_FUNC(objType, pObj, printf_args)
#define VP_HAL(objType, pObj, printf_args)
#define VP_SSL(objType, pObj, printf_args)
#define VP_DOUT(msgType, printf_args)

#endif

/* Derivative debug macros: */

#define VP_API_ENTER(objType, pObj, funcName) \
    VP_API_FUNC(objType, pObj, ("Entering %s()", funcName))

#define VP_API_EXIT(objType, pObj, funcName, retType, retVal) \
    VP_API_FUNC(objType, pObj, ("%s() = %s", funcName, VP_GETSTR(retType, retVal)) \

#endif /* VP_DEBUG_H */

