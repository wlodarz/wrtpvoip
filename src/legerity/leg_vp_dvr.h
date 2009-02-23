/** \file leg_vp_dvr.h
 * leg_vp_dvr.h
 *
 * Header file for the Zarlink Semiconductor VoicePath Module c file.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#ifndef LEG_VP_DRV_H
#define LEG_VP_DRV

/* VP-API headers */
#include "vp_api.h"

#define LEG_VP_DVR_NUM_DEVS             4
#define LEG_VP_DVR_NAME                 "leg_vp_dvr"

#undef LEG_VP_DVR_RUN_TIME_DEV_LOADING

#define LEG_VP_DVR_IOCTL_MAGIC          'v'

/* VP-API System Configuration Functions */
#define LEG_VP_DVR_IOCTL_MK_DEV_OBJ     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 5, LegVpDvrMkDevObjType)
#define LEG_VP_DVR_IOCTL_MK_LN_OBJ      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 6, LegVpDvrMkLnObjType)
#define LEG_VP_DVR_IOCTL_FREE_LN_CTX    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 7, LegVpDvrFreeLnCtxType)
#define LEG_VP_DVR_IOCTL_GET_DEV_INFO   _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 8, LegVpDvrGetDevInfoType)
#define LEG_VP_DVR_IOCTL_GET_LN_INFO    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 9, LegVpDvrGetLnInfoType)

/* VP-API Initialization Functions */
#define LEG_VP_DVR_IOCTL_BOOT           _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 10, LegVpDvrBootType)
#define LEG_VP_DVR_IOCTL_INIT_DEV       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 11, LegVpDvrInitDevType)
#define LEG_VP_DVR_IOCTL_INIT_LN        _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 12, LegVpDvrInitLnType)
#define LEG_VP_DVR_IOCTL_CFG_LN         _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 13, LegVpDvrCfgLnType)
#define LEG_VP_DVR_IOCTL_CAL_CODEC      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 14, LegVpDvrCalCodecType)
#define LEG_VP_DVR_IOCTL_CAL_LN         _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 15, LegVpDvrCalLnType)
#define LEG_VP_DVR_IOCTL_INIT_RING      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 16, LegVpDvrInitRingType)
#define LEG_VP_DVR_IOCTL_INIT_CID       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 17, LegVpDvrInitCidType)
#define LEG_VP_DVR_IOCTL_INIT_MTR       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 18, LegVpDvrInitMtrType)
#define LEG_VP_DVR_IOCTL_INIT_CSTM_TERM _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 19, LegVpDvrInitCstmTermType)
#define LEG_VP_DVR_IOCTL_INIT_PROF      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 20, LegVpDvrInitProfType)
#define LEG_VP_DVR_IOCTL_SFT_RST        _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 21, LegVpDvrSftRstType)

/* VP-API Control Functions */
#define LEG_VP_DVR_IOCTL_SET_LN_ST      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 22, LegVpDvrSetLnStType)
#define LEG_VP_DVR_IOCTL_SET_LN_TN      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 23, LegVpDvrSetLnTnType)
#define LEG_VP_DVR_IOCTL_SET_RLY_ST     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 24, LegVpDvrSetRlyStType)
#define LEG_VP_DVR_IOCTL_SET_REL_GN     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 25, LegVpDvrSetRelGnType)
#define LEG_VP_DVR_IOCTL_SEND_SIG       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 26, LegVpDvrSendSigType)
#define LEG_VP_DVR_IOCTL_SEND_CID       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 27, LegVpDvrSendCidType)
#define LEG_VP_DVR_IOCTL_CNT_CID        _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 28, LegVpDvrCntCidType)
#define LEG_VP_DVR_IOCTL_START_MTR      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 29, LegVpDvrStartMtrType)
#define LEG_VP_DVR_IOCTL_SET_OPTION     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 30, LegVpDvrSetOptionType)
#define LEG_VP_DVR_IOCTL_TONE_DET_CTRL  _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 31, LegVpDvrToneDetCtrlType)
#define LEG_VP_DVR_IOCTL_DEV_IO_ACC     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 32, LegVpDvrDevIoAccType)
#define LEG_VP_DVR_IOCTL_VIRTUAL_ISR    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 33, LegVpDvrVirtualIsrType)
#define LEG_VP_DVR_IOCTL_API_TICK       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 34, LegVpDvrApiTickType)
#define LEG_VP_DVR_IOCTL_SELF_TEST      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 35, LegVpDvrSelfTestType)
#define LEG_VP_DVR_IOCTL_FILL_BUFF      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 36, LegVpDvrFillBuffType)
#define LEG_VP_DVR_IOCTL_LOW_LVL_CMD    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 37, LegVpDvrLowLvlCmdType)

/* VP-API Status and Query Functions */
#define LEG_VP_DVR_IOCTL_GET_LN_STATUS  _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 38, LegVpDvrGetLnStatusType)
#define LEG_VP_DVR_IOCTL_GET_DEV_STATUS _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 39, LegVpDvrGetDevStatusType)
#define LEG_VP_DVR_IOCTL_GET_LP_COND    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 40, LegVpDvrGetLpCondType)
#define LEG_VP_DVR_IOCTL_GET_OPTION     _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 41, LegVpDvrGetOptionType)
#define LEG_VP_DVR_IOCTL_GET_LN_ST      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 42, LegVpDvrGetLnStType)
#define LEG_VP_DVR_IOCTL_FLSH_EVNT      _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 43, LegVpDvrFlshEvntType)
#define LEG_VP_DVR_IOCTL_GET_RSLT       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 44, LegVpDvrGetRsltType)
#define LEG_VP_DVR_IOCTL_CLR_RSLT       _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 45, LegVpDvrClrRsltType)
#define LEG_VP_DVR_IOCTL_DTMF_DETECT    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 46, LegVpDvrDtmfDetectType)
#define LEG_VP_DVR_IOCTL_CODE_CK_SUM    _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 47, LegVpDvrCodeCkSumType)

/* VP-API Line Tests */
#define LEG_VP_DVR_IOCTL_TEST_LN        _IOWR(LEG_VP_DVR_IOCTL_MAGIC, 48, LegVpDvrTestLnType)

/******************** VP-API System Configuration Structs *********************/
typedef struct LegVpDvrMkDevObj {
    VpStatusType        status;
    VpDeviceIdType      deviceId;
} LegVpDvrMkDevObjType;

typedef struct LegVpDvrMkLnObj {
    VpStatusType        status;
    uint8               channelId;
    VpTermType          termType;
} LegVpDvrMkLnObjType;

typedef struct LegVpDvrFreeLnCtx {
    VpStatusType        status;
    uint8               channelId;
} LegVpDvrFreeLnCtxType;

typedef struct LegVpDvrGetDevInfo {
    VpStatusType        status;
    VpDeviceInfoType    *pDeviceInfo;
} LegVpDvrGetDevInfoType;

typedef struct LegVpDvrGetLnInfo {
    VpStatusType        status;
    uint8               channelId;
    VpLineInfoType      *pLineInfo;
} LegVpDvrGetLnInfoType;

/************************ VP-API Initialization Structs ************************/
typedef struct LegVpDvrBoot {
    VpStatusType        status;
    VpBootStateType     state;
    VpImagePtrType      pImageBuffer;
    uint32              bufferSize;
    VpScratchMemType    *pScratchMem;
    VpBootModeType      validation;
} LegVpDvrBootType;

typedef struct LegVpDvrInitDev {
    VpStatusType        status;
    VpProfilePtrType    pDevProfile;
    VpProfilePtrType    pAcProfile;
    VpProfilePtrType    pDcProfile;
    VpProfilePtrType    pRIngProfile;
    VpProfilePtrType    pFxoAcProfile;
    VpProfilePtrType    pFxoCfgProfile;
} LegVpDvrInitDevType;

typedef struct LegVpDvrInitLn {
    VpStatusType        status;
    uint8               channelId;
    VpProfilePtrType    pAcProfile;
    VpProfilePtrType    pDcOrFxoProfile;
    VpProfilePtrType    pRIngProfile;
} LegVpDvrInitLnType;

typedef struct LegVpDvrCfgLn {
    VpStatusType        status;
    uint8               channelId;
    VpProfilePtrType    pAcProfile;
    VpProfilePtrType    pDcOrFxoProfile;
    VpProfilePtrType    pRIngProfile;
} LegVpDvrCfgLnType;

typedef struct LegVpDvrCalCodec {
    VpStatusType        status;
    uint8               channelId;
    VpDeviceCalType     mode;
} LegVpDvrCalCodecType;

typedef struct LegVpDvrCalLn {
    VpStatusType        status;
    uint8               channelId;
} LegVpDvrCalLnType;

typedef struct LegVpDvrInitRing {
    VpStatusType        status;
    uint8               channelId;
    VpProfilePtrType    pCadProfile;
    VpProfilePtrType    pCidProfile;
} LegVpDvrInitRingType;

typedef struct LegVpDvrInitCid {
    VpStatusType        status;
    uint8               channelId;
    uint8               length;
    uint8p              pCidData;
} LegVpDvrInitCidType;

typedef struct LegVpDvrInitMtr {
    VpStatusType        status;
    uint8               channelId;
    VpProfilePtrType    pMeterProfile;
} LegVpDvrInitMtrType;

typedef struct LegVpDvrInitCstmTerm {
    VpStatusType        status;
} LegVpDvrInitCstmTermType;

typedef struct LegVpDvrInitProf {
    VpStatusType        status;
    VpProfileType       type;
    VpProfilePtrType    pProfileIndex;
    VpProfilePtrType    pProfile;
} LegVpDvrInitProfType;

typedef struct LegVpDvrSftRst {
    VpStatusType        status;
} LegVpDvrSftRstType;

/****************************VP-API Control Structs ***************************/
typedef struct LegVpDvrSetLnSt {
    VpStatusType        status;
    uint8               channelId;
    VpLineStateType     state;
} LegVpDvrSetLnStType;

typedef struct LegVpDvrSetLnTn {
    VpStatusType        status;
    uint8               channelId;
    VpProfilePtrType    pToneProfile;
    VpProfilePtrType    pCadProfile;
    VpDtmfToneGenType  *pDtmfControl;
} LegVpDvrSetLnTnType;

typedef struct LegVpDvrSetRlySt {
    VpStatusType        status;
    uint8               channelId;
    VpRelayControlType  rState;
} LegVpDvrSetRlyStType;

typedef struct LegVpDvrSetRelGn {
    VpStatusType        status;
    uint8               channelId;
    uint16              txLevel;
    uint16              rxLevel;
    uint16              handle;
} LegVpDvrSetRelGnType;

typedef struct LegVpDvrSendSig {
    VpStatusType        status;
    uint8               channelId;
    VpSendSignalType    signalType;
    void                *pSignalData;
} LegVpDvrSendSigType;

typedef struct LegVpDvrSendCid {
    VpStatusType        status;
    uint8               channelId;
    uint8               length;
    VpProfilePtrType    pCidProfile;
    uint8p              pCidData;
} LegVpDvrSendCidType;

typedef struct LegVpDvrCntCid {
    VpStatusType        status;
    uint8               channelId;
    uint8               length;
    uint8p              pCidData;
} LegVpDvrCntCidType;

typedef struct LegVpDvrStartMtr {
    VpStatusType        status;
    uint8               channelId;
    uint16              onTime;
    uint16              offTime;
    uint16              numMeters;
} LegVpDvrStartMtrType;

typedef struct LegVpDvrToneDetCtrl {
    VpStatusType        status;
} LegVpDvrToneDetCtrlType;

typedef struct LegVpDvrSetOption {
    VpStatusType        status;
    uint8               channelId;
    VpOptionIdType      option;
    void                *pValue;
    bool                lnRequest;
} LegVpDvrSetOptionType;

typedef struct LegVpDvrDevIoAcc {
    VpStatusType                status;
    VpDeviceIoAccessDataType    *pDeviceIoData;
} LegVpDvrDevIoAccType;

typedef struct LegVpDvrVirtualIsr {
    VpStatusType                status;
} LegVpDvrVirtualIsrType;

typedef struct LegVpDvrApiTick {
    VpStatusType                status;
} LegVpDvrApiTickType;

typedef struct LegVpDvrSelfTest {
    VpStatusType        status;
    uint8               channelId;
} LegVpDvrSelfTestType;

typedef struct LegVpDvrFillBuff {
    VpStatusType        status;
    uint8               channelId;
    uint16              length;
    VpVectorPtrType     pData;
} LegVpDvrFillBuffType;

typedef struct LegVpDvrLowLvlCmd {
    VpStatusType        status;
    uint8               channelId;
    uint8               *pCmdData;
    uint8               len;
    uint16              handle;
} LegVpDvrLowLvlCmdType;

/********************** VP-API Status and Query Structs ***********************/
typedef struct LegVpDvrGetLnStatus {
    VpStatusType        status;
    uint8               channelId;
    VpInputType         input;
    bool                *pStatus;
} LegVpDvrGetLnStatusType;

typedef struct LegVpDvrGetDevStatus {
    VpStatusType        status;
    VpInputType         input;
    uint32              *pDeviceStatus;
} LegVpDvrGetDevStatusType;

typedef struct LegVpDvrGetLpCond {
    VpStatusType        status;
    uint8               channelId;
    uint16              handle;
} LegVpDvrGetLpCondType;

typedef struct LegVpDvrGetOption {
    VpStatusType        status;
    uint8               channelId;
    VpOptionIdType      option;
    uint16              handle;
    bool                lnRequest;
} LegVpDvrGetOptionType;

typedef struct LegVpDvrGetLnSt {
    VpStatusType        status;
    uint8               channelId;
    VpLineStateType     *pCurrentState;
} LegVpDvrGetLnStType;

typedef struct LegVpDvrFlshEvnt {
    VpStatusType        status;
} LegVpDvrFlshEvntType;

typedef struct LegVpDvrGetRslt {
    VpStatusType        status;
    VpEventType         *pEvent;
    void                *pResults;
} LegVpDvrGetRsltType;

typedef struct LegVpDvrClrRslt {
    VpStatusType        status;
} LegVpDvrClrRsltType;

typedef struct LegVpDvrDtmfDetect {
    VpStatusType        status;
} LegVpDvrDtmfDetectType;

typedef struct LegVpDvrCodeCkSum {
    VpStatusType        status;
    uint16              handle;
} LegVpDvrCodeCkSumType;

/***************************** VP-API Line Tests Structs ***********************/
typedef struct LegVpDvrTestLn {
    VpStatusType        status;
    uint8               channelId;
    VpTestIdType        test;
    const void          *pArgs;
    uint16              handle;
} LegVpDvrTestLnType;
#endif /* LEG_VP_DRV_H */
