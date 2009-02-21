/** \file vp_api_option.h
 * vp_api_option.h
 *
 * This file contains declaration associated with VP-API Options.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */

#ifndef VP_API_OPTION
#define VP_API_OPTION

#include "vp_api_types.h"
#include "vp_api_cfg.h"

/* Option IDs.  (See Options chapter in VP-API-2 Reference Guide.) */
typedef enum {
    /* Line-specific option IDs begin with "VP_OPTION_ID_".  Device-specific
    option IDs begin with "VP_DEVICE_OPTION_ID_". */
    VP_DEVICE_OPTION_ID_PULSE            = 0x00,
    VP_DEVICE_OPTION_ID_CRITICAL_FLT     = 0x01,
    VP_OPTION_ID_ZERO_CROSS              = 0x02,  /* For the option IDs whose */
    VP_DEVICE_OPTION_ID_RAMP2STBY        = 0x03,  /*  values are explicitly   */
    VP_OPTION_ID_PULSE_MODE              = 0x04,  /*  stated, the values must */
    VP_OPTION_ID_TIMESLOT                = 0x05,  /*  not change.             */
    VP_OPTION_ID_CODEC                   = 0x06,
    VP_OPTION_ID_PCM_HWY                 = 0x07,
    VP_OPTION_ID_LOOPBACK                = 0x08,
    VP_OPTION_ID_LINE_STATE              = 0x09,
    VP_OPTION_ID_EVENT_MASK              = 0x0A,
    VP_OPTION_ID_RESERVED_1              = 0x0B,
    VP_OPTION_ID_RING_CNTRL              = 0x0C,
    VP_OPTION_ID_RESERVED_2              = 0x0D,
    VP_OPTION_ID_DTMF_MODE               = 0x0E,
    VP_DEVICE_OPTION_ID_DEVICE_IO        = 0x0F,
    VP_OPTION_ID_RESERVED_EVENT_MASK_VCP = 0x10,
    VP_OPTION_ID_PCM_TXRX_CNTRL          = 0x11,
    VP_DEVICE_OPTION_ID_PULSE2           = 0x12,
    VP_OPTION_ID_LINE_IO_CFG             = 0x13,
    VP_DEVICE_OPTION_ID_DEV_IO_CFG       = 0x14,
    VP_OPTION_ID_DTMF_SPEC               = 0x15,
    VP_DEVICE_OPTION_ID_PARK_MODE        = 0x16,
    VP_OPTION_ID_DCFEED_SLOPE            = 0x17,
    VP_OPTION_ID_SWITCHER_CTRL           = 0x18,
    /* It is fine to add new option values in this gap. */
    VP_OPTION_ID_PULSE                   = 0x24,
    VP_OPTION_ID_DEBUG_SELECT,
    VP_OPTION_ID_US_TRANSCODEC,
    VP_OPTION_ID_DS_TRANSCODEC,
    VP_OPTION_ID_ECHO_CANCELER,
    VP_OPTION_ID_UTD_1_COEF,
    VP_OPTION_ID_UTD_2_COEF,
    VP_OPTION_ID_UTD_3_COEF,
    VP_OPTION_ID_UTD_4_COEF,
    VP_OPTION_ID_UTD_5_COEF,
    VP_OPTION_ID_UTD_6_COEF,
    VP_OPTION_ID_UTD_7_COEF,
    VP_OPTION_ID_UTD_8_COEF,
    VP_OPTION_ID_CLR_PKT_CNTR,
    VP_NUM_OPTION_IDS,
    VP_OPTION_ID_ENUM_RSVD    = FORCE_SIGNED_ENUM,
    VP_OPTION_ID_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpOptionIdType;

/** Parameters for dial pulse, flash, and on-hook */
typedef struct {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
#ifdef EXTENDED_FLASH_HOOK
    uint16 onHookMin;       /**< Minimum on-hook time (in 125uS) */
#endif
} VpOptionPulseType;

typedef struct {
    uint16 breakMin;        /**< Minimum pulse break time (in 125uS) */
    uint16 breakMax;        /**< Maximum pulse break time (in 125uS) */
    uint16 makeMin;         /**< Minimum pulse make time (in 125uS) */
    uint16 makeMax;         /**< Maximum pulse make time (in 125uS) */
    uint16 interDigitMin;   /**< Minimum pulse interdigit time (in 125uS) */
    uint16 flashMin;        /**< Minimum flash break time (in 125uS) */
    uint16 flashMax;        /**< Maximum flash break time (in 125uS) */
    uint16 onHookMin;       /**< Minimum on-hook time (in 125uS) */
    uint16 offHookMin;      /**< Minimum off-hook time (in 125uS) */
} VpOptionLinePulseType;

/** Method for line control when critical faults are detected */
typedef struct {
    /**< The line is set to disconnect when the specified fault is active and
     * the "En" bit is set TRUE
     */
    bool acFltDiscEn;           /**< AC fault detected */
    bool dcFltDiscEn;           /**< DC fault detected */
    bool thermFltDiscEn;        /**< Thermal fault detected */
} VpOptionCriticalFltType;

/** Method for zero-cross control */
typedef enum {
    VP_OPTION_ZC_M4B,   /**< Zero-Cross On - Make before break */
    VP_OPTION_ZC_B4M,   /**< Zero-Cross On - Break before make */
    VP_OPTION_ZC_NONE,   /**< Turn Zero-Cross control off */
    VP_OPTION_ZC_ENUM_RSVD    = FORCE_SIGNED_ENUM,
    VP_OPTION_ZC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpOptionZeroCrossType;

/** Dial Pulse decode enable/disable */
typedef enum {
    VP_OPTION_PULSE_DECODE_OFF, /**< Disable Pulse Decode */
    VP_OPTION_PULSE_DECODE_ON,   /**< Enable Pulse Decode */
    VP_OPTION_PULSE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPulseModeType;

/** Transmit/Receive Timeslot setting (timeslot and control) */
typedef struct {
    uint8 tx;   /**< 8-bit TX timeslot */
    uint8 rx;   /**< 8-bit RX timeslot */
} VpOptionTimeslotType;

typedef enum {
    VP_OPTION_ALAW,                 /**< Select G.711 A-Law PCM encoding */
    VP_OPTION_MLAW,                 /**< Select G.711 Mu-Law PCM encoding */
    VP_OPTION_LINEAR,               /**< Select Linear PCM encoding */
    VP_OPTION_WIDEBAND,             /**< Select Wideband PCM encoding */
    VP_NUM_OPTION_CODEC_TYPE_IDS,   /**< Select Linear PCM encoding */
    VP_OPTION_CODEC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionCodecType;

/** PCM Highway Selection (B valid on select devices only) */
typedef enum {
    VP_OPTION_HWY_A,            /**< Select the 'A' PCM Highway */
    VP_OPTION_HWY_B,            /**< Select the 'B' PCM Highway */
    VP_OPTION_HWY_TX_A_RX_B,    /**< Transmit on Highway A, receive on B */
    VP_OPTION_HWY_TX_B_RX_A,    /**< Transmit on Highway A, receive on A */
    VP_OPTION_HWY_TX_AB_RX_A,   /**< Transmit on Highway A and B, receive on A */
    VP_OPTION_HWY_TX_AB_RX_B,   /**< Transmit on Highway A and B, receive on B */
    VP_OPTION_HWY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmHwyType;

/** Loopback option selection */
typedef enum {
    VP_OPTION_LB_OFF,           /**< All loopbacks off */

    /* Following loopback options are supported for CSLAC and VCP only */
    VP_OPTION_LB_TIMESLOT,      /**< Perform a timeslot loopback */
    VP_OPTION_LB_DIGITAL,       /**< Perform a full-digital loopback */
    VP_OPTION_LB_CHANNELS,      /**< Connects FXO to FXS line on same device */

    /* Following loopback options are supported for VPP only */
    VP_OPTION_LB_DS2US_PKT,     /**< Loopback downstream packets to upstream
                                 * packets at the HBI interface. */
    VP_OPTION_LB_US2DS_PKT,     /**< Loopback upstream packets to downstream
                                 * packets at the HBI interface. */
    VP_OPTION_LB_US2DS_SMPL,    /**< Loopback encoded upstream samples to
                                 * downstream sample decoder */
    VP_OPTION_LB_DS2US_SMPL,    /**< Loopback decoded downstream samples to
                                 * upstream sample encoder. */
    VP_NUM_LB_OPTIONS,
    VP_OPTION_LB_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionLoopbackType;

/* DevNotes: The names of the following type need to be changed to make use
 * of mapped battery names like Bat1, Bat2 ...*/
/** Active Line State battery supply selection */
typedef enum {
    VP_OPTION_BAT_AUTO,     /**< Automatic Batery selection */
    VP_OPTION_BAT_HIGH,     /**< Use High Batery */
    VP_OPTION_BAT_LOW,      /**< Use Low Batery */
    VP_OPTION_BAT_BOOST,    /**< Include Positive Batery */
    VP_NUM_OPTION_BAT_IDS,
    VP_OPTION_BAT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionBatType;

/** Active Line State battery supply selection */
typedef struct {
    bool battRev;       /**< Smooth/Abrupt Battery Reversal (TRUE = abrupt) */

    VpOptionBatType bat;    /**< Battery selection for Active line state */
} VpOptionLineStateType;

/** Ring control option */
typedef enum {
    VP_LINE_STANDBY,        /**< Low power line feed state */
    VP_LINE_TIP_OPEN,       /**< Tip open circuit state */
    VP_LINE_ACTIVE,         /**< Line Feed w/out VF */
    VP_LINE_ACTIVE_POLREV,  /**< Polarity Reversal Line Feed w/out VF */
    VP_LINE_TALK,           /**< Normal off-hook Active State; Voice Enabled */
    VP_LINE_TALK_POLREV,    /**< Normal Active with reverse polarity;
                             *   Voice Enabled */

    VP_LINE_OHT,            /**< On-Hook tranmission state */
    VP_LINE_OHT_POLREV,     /**< Polarity Reversal On-Hook tranmission state */

    VP_LINE_DISCONNECT,     /**< Denial of service */
    VP_LINE_RINGING,        /**< Ringing state */
    VP_LINE_RINGING_POLREV, /**< Ringing w/Polarity Reversal */

    VP_LINE_FXO_OHT,        /**< FXO Line providing Loop Open w/VF */
    VP_LINE_FXO_LOOP_OPEN,  /**< FXO Line providing Loop Open w/out VF */
    VP_LINE_FXO_LOOP_CLOSE, /**< FXO Line providing Loop Close w/out VF */
    VP_LINE_FXO_TALK,       /**< FXO Line providing Loop Close w/VF */
    VP_LINE_FXO_RING_GND,   /**< FXO Line providing Ring Ground (GS only)*/

    VP_LINE_STANDBY_POLREV, /**< Low power polrev line feed state */
    VP_LINE_PARK,           /**< Park mode */
    VP_LINE_RING_OPEN,      /**< Ring open */
    VP_LINE_HOWLER,         /**< Howler */
    VP_LINE_TESTING,        /**< Testing */
    VP_LINE_DISABLED,       /**< Disabled */
    VP_LINE_NULLFEED,       /**< Null-feed */

    VP_NUM_LINE_STATES,
    VP_LINE_STATE_ENUM_RSVD = FORCE_SIGNED_ENUM,
    VP_LINE_STATE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpLineStateType;

typedef struct {
    VpOptionZeroCrossType zeroCross;    /**< LCAS zero cross control */

    uint16 ringExitDbncDur; /**< Ringing Exit Debounce Duration; Used during end
                             * of ON periods of ringing cadences; 125uS
                             * resolution
                             */

    VpLineStateType ringTripExitSt; /**< State to automatically switch to upon
                                     * ring trip
                                     */
}  VpOptionRingControlType;

/** DTMF detection option */
typedef enum {
    VP_OPTION_DTMF_DECODE_OFF,      /**< Disable DTMF Digit Decode */
    VP_OPTION_DTMF_DECODE_ON,       /**< Enable DTMF Digit  Decode */
    VP_OPTION_DTMF_GET_STATUS,      /**< Do not change anything; Just get the
                                     *   DTMF status  */
    VP_NUM_OPTION_DTMF_IDS,
    VP_OPTION_DTMF_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionDtmfModeControlType;

/* Device I/O Option related definitions */
typedef enum {
    VP_IO_INPUT_PIN,                /* Configure GPIO pin as input pin */
    VP_IO_OUTPUT_PIN,               /* Configure GPIO pin as output pin */
    VP_IO_DIR_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceIoDirectionType;

typedef enum {
    VP_OUTPUT_DRIVEN_PIN,           /* Configure as TTL/CMOS output pin */
    VP_OUTPUT_OPEN_PIN,             /* Configure as open collector/drain
                                     * output pin */
    VP_OUTPUT_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDeviceOutputPinType;

typedef struct {
    uint32 directionPins_31_0;      /* Device specific IO pin direction
                                     * (Pins 0 - 31) */
    uint32 directionPins_63_32;     /* Device specific IO pin direction
                                     * (Pins 32 - 63) */
    uint32 outputTypePins_31_0;     /* Output pin type (Pins 0 - 31) */
    uint32 outputTypePins_63_32;    /* Output pin type (Pins 32 - 63) */
} VpOptionDeviceIoType;

/* Definition for line I/O config option */
typedef struct {
    uint8 direction;
    uint8 outputType;
} VpOptionLineIoConfigType;

/* Definition for device I/O config option */
typedef struct {
    VpOptionLineIoConfigType lineIoConfig[VP_MAX_LINES_PER_DEVICE];
} VpOptionDeviceIoConfigType;

typedef enum {
    VP_OPTION_PCM_BOTH,             /* Enable both PCM transmit and receive
                                     * paths */
    VP_OPTION_PCM_RX_ONLY,          /* Enable PCM receive path only */
    VP_OPTION_PCM_TX_ONLY,          /* Enable PCM transmit path only */
    VP_OPTION_PCM_ALWAYS_ON,        /* Prevents disabling of PCM path */
    VP_PCM_TXRX_CNTRL_ENUM_RSVD    = FORCE_SIGNED_ENUM,
    VP_PCM_TXRX_CNTRL_ENUM_SIZE=FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpOptionPcmTxRxCntrlType;

/** Direction Specification */
typedef enum {
    VP_DIRECTION_DS,
    VP_DIRECTION_US,
    VP_DIRECTION_INVALID,   /**< Used by the API to determine if the direction
                             * field is valid */
    VP_DIRECTION_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpDirectionType;

#define VP_LINE_FLAG_BYTES ((VP_MAX_LINES_PER_DEVICE + 7) / 8)

/** DTMF detection option control */
typedef struct {
    VpOptionDtmfModeControlType dtmfControlMode; /**< DTMF detection
                                                  * Enable/Disable */
    VpDirectionType direction;                  /**< Detection direction */
    uint32 dtmfDetectionSetting;                /**< Indicates the DTMF
                                                 *   detection setting for first
                                                 *    32 lines */
    uint8 dtmfResourcesRemaining;               /**< DTMF decoder resources
                                                 *   remaining */
    uint8 dtmfDetectionEnabled[VP_LINE_FLAG_BYTES];
                                                /**< DTMF detection setting for
                                                 *   lines 7-0, 15-8, etc. */
} VpOptionDtmfModeType;

/** Regional DTMF Specs */
typedef enum {
    VP_OPTION_DTMF_SPEC_ATT,    /* Q.24 AT&T */
    VP_OPTION_DTMF_SPEC_NTT,    /* Q.24 NTT */
    VP_OPTION_DTMF_SPEC_AUS,    /* Q.24 Australian */
    VP_OPTION_DTMF_SPEC_BRZL,   /* Q.24 Brazilian */
    VP_OPTION_DTMF_SPEC_ETSI    /* ETSI ES 201 235-3 v1.3.1 */
} VpOptionDtmfSpecType;

/**< The following types are for dealing with upstream/downstream transcodec
 * Options
 */
/* VPP Packet types */
typedef enum {
    VP_PKT_G711_MULAW        = 0x00, /**< Select G.711 Mu-Law PCM
                                      * encoding
                                      */
    VP_PKT_G726_32K_AAL2_SO  = 0x02, /**< G.726, 32Kbps CODEC; Least
                                      * significant nibble(4-bits)
                                      * contains the first sample to be
                                      * played out.
                                      */
    VP_PKT_G711_ALAW         = 0x08, /**< Select G.711 A-Law PCM
                                      * encoding
                                      */
    VP_PKT_G729AB            = 0x12, /**< Select G.729AB */
    VP_PKT_LINEAR            = 0x60, /**< Select Linear PCM encoding */
    VP_PKT_G726_32K          = 0x6E, /**< G.726, 32Kbps CODEC; Most
                                      * significant nibble(4-bits)
                                      * contains the first sample to be
                                      * played out.
                                      */
    VP_PKT_SID               = 0x80, /**< Silence Insertion Description
                                      * packet
                                      */
    VP_PKT_CID               = 0x82, /**< CallerID Packet */
    VP_PKT_NOP               = 0xFF,  /**< No operation packet */
    VP_PKT_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpPacketType;

typedef struct {
    VpPacketType packetType;  /**< Upstream Transcoder Type */
    uint8 packetSize;               /**< Packet size */

    bool silenceSupression;     /**< if TRUE, silence suppression is enabled */
} VpOptionUsTranscoderType;

/**< The following types are for dealing with Downstream Transcoder Options */
typedef enum {
    VP_PLC_TYPE_SILENCE,        /**< Lost packets are replaced with silence */
    VP_PLC_TYPE_PREVIOUS_PKT,   /**< Lost packets are replaced with previous
                                 * packet
                                 */
    VP_PLC_TYPE_LEG_ALG,        /**< Lost packets are replaced using Zarlink Semiconductor
                                 * proprietary algorithm
                                 */
    VP_PLC_TYPE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpPlcType;

typedef struct {
    VpPacketType packetType;  /**< Downstream Transcoder Type */
    uint8 packetSize;               /**< Packet size */
    uint8 framesToMute;             /**< Consecutive lost frames after which
                                     * audio mutes; valid range 0 to 7
                                     */
    uint8 ramp;                     /**< Number of samples to ramp into
                                     * correction valid range 0 to 31
                                     */
    VpPlcType plc;                  /**< PLC Algorithm type */
} VpOptionDsTranscoderType;

/**< The following types are for Echo Canceler Options */
typedef enum {
    VP_EC_TAIL_LENGTH_4MS,
    VP_EC_TAIL_LENGTH_8MS,
    VP_EC_TAIL_LENGTH_12MS,
    VP_EC_TAIL_LENGTH_16MS,
    VP_EC_TAIL_LENGTH_20MS,
    VP_EC_TAIL_LENGTH_24MS,
    VP_EC_TAIL_LENGTH_28MS,
    VP_EC_TAIL_LENGTH_32MS,
    VP_EC_TAIL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpECTailLengthType;

typedef struct {
    VpECTailLengthType tailLength;  /**< Echo Canceler Tail length */

    bool toneDisable;       /**< If TRUE, EC remains active on 2100Hz tone
                             * detection with phase reversal
                             */
    bool echoCanDisable;    /**< If TRUE, disables EC */
    bool adaptDisable;      /**< If TRUE, disables EC adaption */
    bool resetHRegister;    /**< If TRUE, clears H-Register */
    bool cngDisable;        /**< If TRUE, disables Comfort Noise Generation */
    bool nlpDisable;        /**< If TRUE, disables Non-Linear processing */
} VpOptionEchoCancelerType;

/**< The following types are for Upstream Tone Detect Filter Options */
typedef struct {
    uint8 utd_id;     /**< UTD Filter number */
    uint8 threshold;  /**< Filter detection threshold */
    uint16 coeff_b2;
    uint16 coeff_b1;
    uint16 coeff_b0;
    uint16 coeff_a2;
    uint16 coeff_a1;
    uint8 debounce;    /**< Debouce time in multiples of 500uS */
} VpOptionUtdCoeffType;

/**< The following types are for Park Mode options */
typedef struct {
    uint16 discTime;    /**< Specified in 500ms increments, up to 8 seconds */
    uint16 standbyTime; /**< Specified in 100ms increments, up to 8 seconds */
} VpOptionParkModeType;

#endif /* VP_API_OPTION */




