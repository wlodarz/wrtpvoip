/** \file vp_api_cfg.h
 * vp_api_cfg.h
 *
 * This file contains the configuration and compile time settings for
 * building appropriate VP-API library modules needed for any application.
 *
 * Copyright Zarlink Semiconductor, Inc 2008
 */

#ifndef VP_API_CFG_H
#define VP_API_CFG_H

/******************************************************************************
 * COMPILE-TIME OPTIONS:: API DEBUG SPECIFICATIONS                            *
 *****************************************************************************/
/*
 * Define (or undefine) the appropriate compile time debug levels based on your
 * application needs.
 *
 */

#define  VP_DEBUG

/******************************************************************************
 * COMPILE-TIME OPTIONS:: Conditionally-Compiled API Libraries                *
 *****************************************************************************/
/*
 * Define (or undefine) the appropriate compile time switches based on your
 * application needs.
 *
 * NOTE: Such library selection compile time option(s) MUST be defined before
 * including any other file from VoicePath library.
 *
 * NOTE: More than one Library modules can be built simultaneosuly (if needed).
 */

/* Define the API libraries that will be included in this build */
#undef  VP_CC_790_SERIES    /**< define to build 790 specific API library;
                             *   undef to exclude this library. */

#undef  VP_CC_792_SERIES    /**< define to build 792 specific API library;
                             *   undef to exclude this library. */

#define  VP_CC_880_SERIES    /**< define to build 880 specific API library;
                             *   undef to exclude this library. */

#undef  VP_CC_890_SERIES    /**< define to build 890 specific API library;
                             *   undef to exclude this library. */

#undef  VP_CC_580_SERIES    /**< define to build 580 specific API library;
                             *   undef to exclude this library. */

/* VCP (VCP) library */
#undef  VP_CC_VCP_SERIES    /**< define to build VCP specific API library;
                             *   undef to exclude this library. */
/* VCP2 library */
#undef  VP_CC_VCP2_SERIES    /**< define to build VCP2 specific API library;
                             *   undef to exclude this library. */

/* VPP library */
#undef VP_CC_VPP_SERIES    /**< define to build VPP specific API library;
                             *   undef to exclude this library. */

/* Kernel library */
#ifndef __KERNEL__
#undef VP_CC_KWRAP
#endif                       /**< define to build Kernel specific API library;
                             *   undef to exclude this library. The kernel
                             *   wrapper code is not allowed to be in the kernel.
                             */

/******************************************************************************
 * Include Files for the API                                                  *
 *****************************************************************************/
/* Include the API types for this architecture */
#include "vp_api_types.h"

/******************************************************************************
 * Undef the following to remove the code necessary to run the API-II sequencer
 * for CSLAC devices and to enable compile of API-II Lite. This is useful to
 * reduce code space if the host is not using API-II "advanced" features.
 *****************************************************************************/
#if defined(VP_CC_790_SERIES) || defined(VP_CC_880_SERIES) || defined(VP_CC_890_SERIES)
#undef  CSLAC_SEQ_EN
#undef  EXTENDED_FLASH_HOOK
#endif

/******************************************************************************
 * Device Context Size                                                        *
 *****************************************************************************/
/*
 * Define the maximum number of lines per device in your system. Note that a
 * system (i.e., all devices controlled by one instance of VP-API) could have
 * more than one type of device, each of those devices may support different
 * number of lines. For example in a system, device A might support 4 lines
 * and device B might support 32 lines, in such a scenario, the following macro
 * should be defined as 32. */
#if defined(VP_CC_VCP2_SERIES) || defined(VP_CC_KERNEL_SERIES)
#define VP_MAX_LINES_PER_DEVICE     (128)
#elif defined(VP_CC_VCP_SERIES)
#define VP_MAX_LINES_PER_DEVICE     (32)
#elif defined(VP_CC_792_SERIES)
#define VP_MAX_LINES_PER_DEVICE     (8)
#elif defined(VP_CC_790_SERIES) || defined(VP_CC_580_SERIES)
#define VP_MAX_LINES_PER_DEVICE     (4)
#else /* VP_CC_880_SERIES + VP_CC_890_SERIES */
#define VP_MAX_LINES_PER_DEVICE     (2)
#endif

/* Define the following if you would like the API to protect(detect) the
 * applications in reentrant scenarios. When the following conditional compile
 * flag is enabled, it is the responsibility of the application to handle
 * VP_STATUS_IN_CRTCL_SECTN API function return code. This conditional flag may
 * need to be defined (enabled) in multitasking applications (where more than
 * one task directly interacts with the VP-API directly). In a single threaded
 * application this conditional flag could be left disabled (undefined).
 */
#undef VP_CC_MULTI_THREAD_PROTECTION /* This has neither been completely
                                      * implemented nor tested yet, Please do
                                      * not define this */

/******************************************************************************
 * Library Specific COMPILE-TIME OPTIONS and defines                          *
 *****************************************************************************/
#ifdef VP_CC_790_SERIES
/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP790_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef VP790_EFFICIENT_POLLED_MODE      /* Define to use efficient polled
                                         * interrupt handling mode */
#undef VP790_INTERRUPT_LEVTRIG_MODE     /* Define to use level triggered
                                         * interrupt handling mode */
#undef VP790_INTERRUPT_EDGETRIG_MODE    /* Define to use edge triggered
                                         * interrupt handling mode */
#endif /* VP_CC_790_SERIES */

#ifdef VP_CC_792_SERIES

#define VP_CC_792_GROUP                 /* Define to include support for sharing
                                         * an interrupt pin between multiple
                                         * VP792 devices. */

#endif /* VP_CC_792_SERIES */


#ifdef VP_CC_880_SERIES

/*
 * Special Handling in the Init procedure is implemented to automatically detect
 * the battery configuration on the circuit. Damage can occur if the device is
 * programmed incorrectly, so it is highly recommended that either the Auto
 * Detect method be used under conditions when there is no other line activity,
 * or a "known" set of values.
 */
#undef  VP880_AUTO_BAT_DETECT

/*
 * Some VP880 Devices require calibration for improved feed performance. But
 * this adds initialization time and in some applications may not be needed.
 */
#define VP880_CAL_ENABLE

/*
 * Current Limiting option in the VP880 library limits the peak switcher current
 * such that a 3REN load can be supported at 90V peak. Disable this option if
 * 5REN is required.
 */
#undef  VP880_CURRENT_LIMIT

/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP880_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef  VP880_EFFICIENT_POLLED_MODE     /* Define to use efficient polled
                                         * interrupt handling mode */
#undef  VP880_INTERRUPT_LEVTRIG_MODE    /* Define to use level triggered
                                         * interrupt handling mode */
#undef  VP880_INTERRUPT_EDGETRIG_MODE   /* Define to use edge triggered
                                         * interrupt handling mode */
#undef  VP880_INCLUDE_TESTLINE_CODE     /* Defines whether or not to enable
                                         * 880 line test capabilities */

/*
 * Defines all possible Vp880 test line packages
 */
#define VP880_LINE_TEST_AUDITOR 1
#define VP880_LINE_TEST_PROFESSIONAL 2

#ifdef VP880_INCLUDE_TESTLINE_CODE

/* Define supported line test pacakge */
#define VP880_INCLUDE_LINE_TEST_PACKAGE VP880_LINE_TEST_PROFESSIONAL

#define  VP880_EZ_MPI_PCM_COLLECT    /* Define to use api to collect
                                     * PCM samples via the MPI bus */

/* Define the maximum expected computation time to arrive at results for
 * VpTestLineCallback() after having collected the necessary PCM samples. */
#define VP880_PCM_CALCULATION_TIME 1000     /* time is in ms */

#endif /* VP880_INCLUDE_TESTLINE_CODE */

#endif /* VP_CC_880_SERIES */

#ifdef VP_CC_580_SERIES
/* CSLAC library can be configured to handle the interrupt in four different
 * configurations. Choose (only) one of the mechanisms by defining one of the
 * following. */
#define VP580_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#undef VP580_EFFICIENT_POLLED_MODE      /* Define to use efficient polled
                                         * interrupt handling mode */
#undef VP580_INTERRUPT_LEVTRIG_MODE     /* Define to use level triggered
                                         * interrupt handling mode */
#undef VP580_INTERRUPT_EDGETRIG_MODE    /* Define to use edge triggered
                                         * interrupt handling mode */
#endif

#ifdef VP_CC_890_SERIES

#undef  VP890_INCLUDE_TESTLINE_CODE     /* Defines whether or not to enable
                                         * 890 line test capabilities */
#ifdef VP890_INCLUDE_TESTLINE_CODE

/* Define supported line test pacakge */
#define VP890_LINE_TEST_AUDITOR 1
#define VP890_LINE_TEST_PROFESSIONAL 2

#define VP890_INCLUDE_LINE_TEST_PACKAGE VP890_LINE_TEST_PROFESSIONAL

#define VP890_EZ_MPI_PCM_COLLECT            /* Define to use api to collect
                                             * PCM samples via the MPI bus */

/* Define the maximum expected computation time to arrive at results for
 * Vp890TestLineCallback() after having collected the necessary PCM samples. */
#define VP890_PCM_CALCULATION_TIME 1000     /* time is in ms */

#endif /* VP890_INCLUDE_TESTLINE_CODE */

#define VP890_SIMPLE_POLLED_MODE        /* Define to use simple polled interrupt
                                         * handling mode */
#endif /* VP_CC_890_SERIES*/

/*
 * The following lines include necessary compile time options/definitions that
 * are required for boot loading VCP/VPP/VCP2 devices.  By default, compression
 * is not supported for boot images.  If you would to use compressed boot
 * images, the compile time options in the following lines need to be modified.
 */

#if !defined(VP_CC_VCP_SERIES) && !defined(VP_CC_VPP_SERIES) && !defined(VP_CC_VCP2_SERIES)
typedef uint8 VpScratchMemType;
#else
/* All the following definitions are necessary only for VCP/VPP/VCP2 devices */
/******************************************************************************
 * COMPILE-TIME OPTIONS:: Boot load compression method and size               *
 *****************************************************************************/
#define VP_CLEAR_CODE_MEM
    /*
     * After VpBootLoad(), a VP_LINE_EVID_BOOT_CMP event is generated, and
     * VpGetResults() is then used to retrieve the loadChkSum value, which is
     * a checksum calculated over the VCP's code memory.  VpCodeCheckSum()
     * can then be called at any later time to verify that VCP code memory
     * has not been corrupted.
     *
     * With VP_CLEAR_CODE_MEM defined, VpBootLoad() clears all VCP code memory
     * to zeroes before loading the boot image.
     * With VP_CLEAR_CODE_MEM disabled, unused VCP code memory is not
     * initialized after a hardware reset.  However, the loadChkSum is
     * calculated over the entire VCP code memory space, including uninitialized
     * parts.  If it is important to you that the loadChkSum value stay the
     * same after a hardware reset, then enable VP_CLEAR_CODE_MEM.
     *
     * This option adds a small amount of execution time to VpBootLoad(), when
     * state = VP_BOOT_STATE_FIRST or VP_BOOT_STATE_FIRSTLAST.
     */

/*
 * Options related to BootLoad() decompression
 */
#define VP_NO_COMPRESS
    /* Define this option to exclude all decompression code from VpBootLoad().*/
#define VP_COMPRESS_FAST
    /*
     *  Define this to include additional speed-optimized code to increase the
     * speed of VpBootLoad().
     */

    /*
     *  On our test platform, code size is affected by the above options as
     * shown:
     *  VP_NO_COMPRESS      VP_COMPRESS_FAST        added code size
     *  --------------      ----------------        ---------------
     *  #define             (don't care)            0
     *  #undef              #undef                  9.6 kB
     *  #undef              #define                 12.2 kB
     */

    /*
     * The following options determine the size of VpScratchMemType, and the
     * speed of VpBootLoad() when using a compressed VCP image.
     */
#define VP_COMPRESS_WINDOW_BITS 14
    /*
     * There is no advantage in increasing this to a value greater than the
     * value used when the VCP image was compressed.  Setting it to less than
     * the value used in compression will cause VpBootLoad() to fail.  [The
     * number of window bits used in compression is indicated in the VCP
     * image's filename; for example, dvp.bin10z was compressed with 10 window
     * bits, etc.  If the number of bits is not indicated (*.binz), then
     * 8 bits were used.]  Higher values improve compression.  Don't
     * change this value unless you change the corresponding value used
     * in compressing the VCP image.  Adds (1 << VP_COMPRESS_WINDOW_BITS)
     * bytes to sizeof(VpScratchMemType).  This option can range from
     * 8 to 15.  */
#define VP_COMPRESS_OUTBUF_PAGES 50
    /*
     * Size of the buffer for holding data after it is decompressed, and before
     * it is sent to the HBI bus.  Must be greater than 0 and less than 512.
     * Larger values increase the speed of VpBootLoad().  If VP_COMPRESS_FAST
     * is defined, VP_COMPRESS_OUTBUF_PAGES should be at least 3.
     * Adds (128 * VP_COMPRESS_OUTBUF_PAGES) bytes to sizeof(VpScratchMemType).
     */

#ifndef VP_NO_COMPRESS
#include "zstructs.h"    /**< Components of VpScratchMemType */
typedef struct {
    struct VpZlibInflateStateType state;
    VpZlibStreamType stream;
    uint8 window[1 << VP_COMPRESS_WINDOW_BITS];
    uint8 inflated[VP_COMPRESS_OUTBUF_PAGES * 128];
} VpScratchMemType;
#else /* VP_NO_COMPRESS */
typedef uint8 VpScratchMemType;
#endif /* VP_NO_COMPRESS */
#endif  /* !VP_CC_VCP_SERIES && !VP_CC_VPP_SERIES */

/******************************************************************************
 * COMPILE-TIME OPTIONS:: Top level VP-API Default Device and Line Options    *
 *****************************************************************************/

/* Debounce time to mask hook activity when a pol rev is generated */
#define VP_POLREV_DEBOUNCE_TIME     (100)

/* The following defines specify the default settings of the device and line
 * options used in the API */
#include "vp_api_option.h"
#include "vp_api_event.h"

/* Default Dial Pulse Options specified in units of 125micro sec  */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN          (33 * 8)    /* == 33 ms */
#define VP_OPTION_DEFAULT_DP_BREAK_MAX          (100 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN           (17 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX           (75 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN      (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN          (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX          (1300 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN        VP_OPTION_DEFAULT_DP_FLASH_MAX

/*
 * Specification 2 is usefull if you need to detect between two non-overlapping
 * dial pulse specifications.
 */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_BREAK_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN2     (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN2       (0 * 8)

/* Default setting for Critical fault option */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN     TRUE
/* CSLAC-880 and VCP-880 do not support the detection of AC, DC critical faults.
 * So, The VP-API uses the following default values for those devices.
 * Note that changing the following values will result in VpInitDevice()
 * failure. */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN_880      FALSE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN_880      FALSE

/* Default setting for Zero cross option */
#define VP_OPTION_DEFAULT_ZERO_CROSS            VP_OPTION_ZC_M4B

/* Default setting for Ramp to standby option */
#define VP_OPTION_DEFAULT_RAMP2STBY             0

/* Default setting for Pulse mode option */
#define VP_OPTION_DEFAULT_PULSE_MODE            VP_OPTION_PULSE_DECODE_OFF

/* Default CODEC Mode */
#define VP_OPTION_DEFAULT_CODEC_MODE            VP_OPTION_ALAW

/* Default setting for PCM Highway option */
#define VP_OPTION_DEFAULT_PCM_HWY               VP_OPTION_HWY_A

/* Default setting for Loop back option */
#define VP_OPTION_DEFAULT_LOOP_BACK             VP_OPTION_LB_OFF

/* Default setting for Line state option */
#define VP_OPTION_DEFAULT_LS_BAT_REV            FALSE
#define VP_OPTION_DEFAULT_LS_BAT                VP_OPTION_BAT_AUTO

/*
 * Default Event Masks
 * Some events are non-maskable and will be modified by the API.  The user can
 * arbitrarily set the mask bits here as needed.  A mask bit set to '0' means
 * the event is not masked.
 * As a default mask all the events that are not defined in the version of
 * the API.
 */
#define VP_OPTION_DEFAULT_FAULT_EVENT_MASK    \
        (((VP_EVCAT_FAULT_MASK_ALL) | /* Please change ONLY this */ \
           VP_EVCAT_FAULT_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_FAULT_EVENTS)) /*Unmask non maskable events */

#define VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK  \
        (((VP_EVCAT_SIGNALING_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_SIGNALING_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_SIGNALING_EVENTS))

#define VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK   \
        (((VP_EVCAT_RESPONSE_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_RESPONSE_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_RESPONSE_EVENTS))

#define VP_OPTION_DEFAULT_TEST_EVENT_MASK       \
        (((VP_EVCAT_TEST_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_TEST_UNDEFINED) & /* Mask those that are not defined */  \
         (~VP_API_NONMASK_TEST_EVENTS))

#define VP_OPTION_DEFAULT_PROCESS_EVENT_MASK    \
        (((VP_EVCAT_PROCESS_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_PROCESS_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_PROCESS_EVENTS))

#define VP_OPTION_DEFAULT_FXO_EVENT_MASK        \
        (((VP_EVCAT_FXO_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_FXO_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_FXO_EVENTS))

#define VP_OPTION_DEFAULT_PACKET_EVENT_MASK     \
        (((VP_EVCAT_PACKET_MASK_ALL) | /* Please change ONLY this */   \
           VP_EVCAT_PACKET_UNDEFINED) & /* Mask those that are not defined */ \
         (~VP_API_NONMASK_PACKET_EVENTS))

/* Default setting for Ring Control option */
#define VP_OPTION_DEFAULT_RC_ZERO_CROSS         VP_OPTION_ZC_M4B
#define VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL (100 * 8) /* 100ms */
#define VP_OPTION_DEFAULT_RC_RING_EXIT_STATE    VP_LINE_TALK

/* Default setting for DTMF Mode option */
#define VP_OPTION_DEFAULT_DM_DTMF_CONTROL_MODE  VP_OPTION_DTMF_DECODE_OFF
#define VP_OPTION_DEFAULT_DM_DIRECTION          VP_DIRECTION_US

/* Default setting for PCM TX/RX path control */
#define VP_OPTION_DEFAULT_PCM_TXRX_CNTRL        VP_OPTION_PCM_BOTH

/* Default setting for DTMF Spec option */
#define VP_OPTION_DEFAULT_DTMF_SPEC             VP_OPTION_DTMF_SPEC_ATT

/******************************************************************************
 *    VCP2 library specific other defines and conditional compile flags       *
 ******************************************************************************/
#if defined(VP_CC_VCP2_SERIES)

/* Specify the maximum number of SLACs which will be controlled by the VCP2 in
 * this application.  If controlling fewer than 16 SLACs, you can reduce this
 * value to save memory. */
#define VP_VCP2_MAX_NUM_SLACS 16

/* Specify the maximum number of channels controlled by any individual SLAC.
 * If you are controlling a VCP2-792 device, this should be 8.  If you are
 * controlling a VCP2-790 device, you can reduce this value to 4 to save
 * memory.  If you are controlling a VCP2-880 device, you can reduce this value
 * to 2 to save memory.  If you are controlling a combination of
 * SLACs (unlikely), use the number corresponding to the SLAC with the most
 * channels. */
#define VP_VCP2_MAX_NUM_CHANS  8

/*
 * Normally, when a VP-API function needs to write to the VCP2's command
 * mailbox, it checks to see if the mailbox is busy, and if so, returns
 * VP_STATUS_MAILBOX_BUSY.  The application can then wait for a
 * VP_DEV_EVID_DNSTR_MBOX ("mailbox free") event before retrying the VP-API
 * function call.
 *
 * Use the following option to include code which spin-waits for the VCP2 to
 * relinquish control of the command mailbox.  In this case, the VP-API
 * function will continuously poll the VCP2 device until the mailbox becomes
 * available.  The number of polling iterations is specified in the following
 * option.  If this option is 0, no spin-waiting occurs.  If this option is
 * nonzero, the VP-API functions will return VP_STATUS_MAILBOX_BUSY only if
 * unable to acquire control of the mailbox after the specified number of
 * polling iterations.
 *
 * Application programming may be simplified by defining this option to a
 * conservative value and treating VP_STATUS_MAILBOX_BUSY as an error.
 * However, this is at the expense of increased CPU utilization.
 */
#define VP_VCP2_MAILBOX_SPINWAIT 100000

/* When bootloading a VCP2-792, the API uses a special bootloading process to
 * load the firmware for both the VCP2 and the 792 SLACs.  This process
 * spinwaits for the command mailbox between sending blocks of the SLAC image.
 * The spinwait limit here should be a higher number than the general limit, and
 * should not be zero because we actually expect to wait in this case, and a
 * timeout aborts the SLAC bootloading. */
#define VP_VCP2_BOOTLOAD_MAILBOX_SPINWAIT 2000000

/* When spinwaiting for the command mailbox using either of the above
 * parameters, the VP-API can exit and re-enter the critical section between
 * each specified number of reads of the mailbox flags.  Doing this should
 * allow other threads to interrupt while spinwaiting.  Set this value to 0
 * to disable the exit/enter critical behavior. */
#define VP_VCP2_SPINS_PER_CRITICAL_SEC 1

/*
 * Define the following option to include some extra error checking in the
 * VCP2 boot code.  This is most useful to diagnose board problems during
 * initial board bring-up.
 */
#define VP_VCP2_BOARD_BOOT_DEBUG

/*
 * Set this option to reflect the CLKSRC pin value.  Possible values:
 *   1 => crystal oscillator
 *   0 => PCLK (worst case value)
 */
#define VP_VCP2_BOARD_CLKSRC 0

/*
 * Set these options to initialize the PCLKA_SEL and PCLKB_SEL registers.  See
 * the Le79114 data sheet for details.  A safe value is 0xC000.  These options
 * are only relevant if VCP2_BOARD_CLKSRC = 0.
 */
#define VP_VCP2_BOARD_PCLKA_SEL 0xC000
#define VP_VCP2_BOARD_PCLKB_SEL 0xC000

/* Set this option to support the VCP2 emulator. */
#define VP_VCP2_EMU_SUPPORT

#endif /* VP_CC_VCP2_SERIES */

/******************************************************************************
 *     VCP library specific other defines and conditional compile flags       *
 ******************************************************************************/
#if defined(VP_CC_VCP_SERIES)
/* Define the following to wait for certain amount of time for VCP mailbox
 * to become available. By enabling this conditional compile time option,
 * application would rarely have to handle the VP_STATUS_MAILBOX_BUSY error
 * (assuimg configured properly; more later). Basically when this conditional
 * compile option is defined, API repeats checking for mailbox ownership (if
 * it already does not own). This checking continues until either mailbox
 * ownership is acquired or maximum specified number of repetitions happen (in
 * which case API throws an error).
 * If this compile flag is undefined, API checks for mailbox only once.
 */
#define WAIT_TO_ACQUIRE_VCP_MB

/* Define the number iterations host should wait for mailbox owenership */
#define ITERATIONS_TO_WAIT_FOR_VCP_MB      (100000)

/* Define the following to enable debug print statements from the VCP API */
#undef DEBUG_VCP_API

#ifdef DEBUG_VCP_API
#include <stdio.h>
/* Define the following as necessary to redirect the debug output */
#define term_printf printf

/* Further classification enabling/disabling debug output */
#define DEBUG_BOOTLOAD              /* Debug bootload function */
#define DEBUG_VCP_API_MB            /* MailBox related debug output */
#define DEBUG_VCP_API_EVENT         /* Event masking related debug output */
#define DEBUG_VCP_API_DEFLT_OPTN    /* Default option debugging */
#endif /* DEBUG_VCP_API */

#endif /* VP_CC_VCP_SERIES */

/******************************************************************************
 *     VPP library Specific other defines and conditional compile flags       *
 ******************************************************************************/
#ifdef VP_CC_VPP_SERIES
/* Define the following to wait for certain amount of time for VPP mailbox
 * to become available. By enabling this conditional compile time option,
 * application would rarely have to handle the VP_STATUS_MAILBOX_BUSY error
 * (assuimg configured properly; more later). Basically when this conditional
 * compile option is defined, API repeats checking for mailbox ownership (if
 * it already does not own). This checking continues until either mailbox
 * ownership is acquired or maximum specified number of repetitions happen (in
 * which case API throws an error).
 * If this compile flag is undefined, API checks for mailbox only once.
 */
#define WAIT_TO_ACQUIRE_VPP_MB

/* Define the number iterations host should wait for mailbox owenership */
#define ITERATIONS_TO_WAIT_FOR_VPP_MB      (100000)

/* Define the following to enable exiting the application upon HBI errors. */
#define EXIT_ON_VPP_HBI_ACCESS_ERROR


/* Define the following to enable debug print statements from the VPP API */
#undef DEBUG_VPP_API

/* Further classification enabling/disabling debug output */
#ifdef DEBUG_VPP_API
#include <stdio.h>
#define term_printf printf

#undef DEBUG_VPP_API_EVENT      /* Conditional compile for debug messages from
                                 * Event related functions */
#undef DEBUG_VPP_API_MAILBOX    /* Conditional compile for mailbox messages */
#define DEBUG_VPP_API_PROFILE   /* Conditional compile for profile related
                                 * aspects*/
#endif /* DEBUG_VPP_API */

#endif /* VP_CC_VPP_SERIES */

/******************************************************************************
 *                      API Debug output redirection                          *
 ******************************************************************************/
#if defined(DEBUG_VCP_API) || defined(DEBUG_VPP_API)
#ifndef term_printf
#include <stdio.h>
/* Define the following as necessary to redirect the debug output */
#define term_printf printf
#endif /* term_printf */
#endif /* DEBUG_VCP_API || DEBUG_VPP_API */

/******************************************************************************
 * Library Specific default options                                           *
 *****************************************************************************/
/* VoicePath API provides a mechanism where individual library's default
 * options could be different compared to top level API default options.
 * If you need such a change please review the file included below.
 */
#include "vp_api_lib_cfg.h"

#endif /* VP_API_CFG_H */








