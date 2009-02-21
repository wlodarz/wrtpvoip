/** \file hbi_common.c
 * hbi_common.c
 *
 *  This file contains the platform-independent Host Bus Interface layer.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */
#include "vp_api.h"

/* Compile Further only if required */
#if defined (VP_CC_VCP_SERIES) || defined (VP_CC_VPP_SERIES) || defined (VP_CC_VCP2_SERIES)

#include "hbi_common.h"

#if defined (VP_CC_VCP_SERIES) || defined (VP_CC_VPP_SERIES)
/* This function tests for page 255 CRC equal to expected value. */
bool
VpHbiVcpVppValidate(
    VpDeviceIdType deviceId)
{
    uint16 checksum;

    if (VpHbiRd(deviceId, HW_Reg_CRC255, &checksum) == FALSE) {
        return FALSE;
    }

    return (checksum == 0xAA55);
}

/*
 * This function resets the VTD and puts it into a state where an application
 * image can be loaded through HBI.
 */
bool
VpHbiVcpVppReset(
    VpDeviceIdType deviceId)
{
    uint16 cFailCount = 0;
    uint16 data;

    /* Reset the VTD. */
    if (VpHbiVcpVppHalt(deviceId) == FALSE) {
        return FALSE;
    }

    /* Check to make sure clock fault is not present; Provide
     * enough time such that internal PLL settles.
     * Removal of CFAIL bit also means clock autodetection
     * is complete.
     */
    #define SYSINTSTAT_CFAIL_BIT_MASK 0x1000
    #define CFAIL_LOOP_COUNT 2000 /* Assuming 100nS per access,
       * and to wait a maximum of 200micro seconds */
    do {
        if (VpHbiRd(deviceId, HW_Reg_SYSINTSTAT, &data) == FALSE) {
            return FALSE;
        }
        cFailCount++;
    } while (
        ((data & SYSINTSTAT_CFAIL_BIT_MASK) == SYSINTSTAT_CFAIL_BIT_MASK) &&
        (cFailCount < CFAIL_LOOP_COUNT)
    );

    /* DevNotes: It is probably OK to continue; We will probably
     * get a Page 255 CRC error */
    if (cFailCount >= CFAIL_LOOP_COUNT) {
        return FALSE;
    }

    /* Ready to load application image through HBI. */
    return TRUE;
}

bool
VpHbiVcpVppClearCodeMem(
    VpDeviceIdType deviceId)
{
    if (

        /* Reset the device. */
        (VpHbiVcpVppReset(deviceId) == FALSE)

#ifdef VP_CLEAR_CODE_MEM
        /* Clear CODE PMRAM Pages 0, 1, 2, 3 */
        /* Leave CM 0 untouched, so the VCP won't start running. */
        || (VpHbiAmbaWrite(deviceId, 0x10000, 0x0BFFF, (uint16p)0) ==
            FALSE) ||

        /* Clear CODE PMRAM Page 4 */
        (VpHbiAmbaWrite(deviceId, 0x08000, 0x02FFF, (uint16p)0) ==
            FALSE) ||

        /* Clear CODE PMRAM Fixed Page, but skip over CM 0 */
        (VpHbiAmbaWrite(deviceId, 0x1C002, 0x023FD, (uint16p)0) ==
            FALSE)
#endif /* VP_CLEAR_CODE_MEM */

    ) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/* VpHbiVcpVppHalt()
 *
 *  This function simply causes the VCP/VPP to reset and halt.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *
 * Returns
 *  none
 */
bool
VpHbiVcpVppHalt(
    VpDeviceIdType deviceId)
{
    uint16 regVal = 0x0005;
    /* Reset the VCP. */
    if (VpHbiWr(deviceId, HW_Reg_HWRES, &regVal) == FALSE)
        return FALSE;
    /* Configure the HBI. */
    if (VpHalHbiCmd(deviceId, HBI_CMD_CONFIGURE(HBI_PINCONFIG)) == FALSE)
        return FALSE;

    return TRUE;
} /* VpHbiVcpVppHalt() */


/*
 * Sets page 255 base register to point to the specified AMBA address on a
 * VCP/VPP device, returning the new register value in base255reg.
 */
bool
VpHbiVcpVppSetBase255(
    VpDeviceIdType deviceId,
    uint32 amba_addr)
{
    uint16 base255reg;

    /* Page 255 acts as a sliding window to the AMBA bus.  You slide the window
       by setting the Page 255 Base Register (VP_REGID_BASE255).  Read the
       existing value so we won't disturb the reserved bits. */
    if (!VpHbiRd(deviceId, HW_Reg_BASE255, &base255reg))
        return FALSE;

    /* Set the Page 255 Base Register to the page containing amba_addr.
       The upper 10 bits of the AMBA address go into the non-reserved bits
       [14:5] of the Page 255 Base Register.  Don't disturb the reserved
       bits. */
#define RSVD_bits       0x801F /* selects the RSVD bits in the BASE255 reg */
#define upper_10_bits   0x1FF80/* selects the upper 10 bits of an AMBA address*/
    base255reg &= RSVD_bits;
    base255reg |= (amba_addr & upper_10_bits) >> 2;
    if (!VpHbiWr(deviceId, HW_Reg_BASE255, &base255reg))
        return FALSE;

    return TRUE;
} /* VpHbiVcpVppSetBase255 */


/* VpHbiAmbaRdWr()
 *
 *  This function reads/writes 'numwords' words of VCP/VPP memory starting at the
 *  specified AMBA address.
 *
 * Params
 *  'readWrite' - FALSE for read, TRUE for write
 *  'deviceId' - deviceId of the VCP/VPP device
 *  'amba_addr' - the AMBA address to start reading/writing at
 *  'numwords' - the number of data words to read/write, minus 1
 *  'data' - a pointer to the data to be read/written; use data = (uint16p)0
 *      to write zeroes for all data words
 *
 * Returns
 *  TRUE on success, FALSE on failure
 *
 * Notes
 */
bool
VpHbiVcpVppAmbaRdWr(
    bool readWrite,
    VpDeviceIdType deviceId,
    uint32 amba_addr,
    uint32 numwords,
    uint16p data)
{
    return VpHbiXfer(readWrite, deviceId, amba_addr, numwords + 1,
        data, FALSE, VpHbiVcpVppSetBase255);
} /* VpHbiVcpVppAmbaRdWr() */
#endif /* VP_CC_VCP_SERIES || VP_CC_VPP_SERIES */


/* VpHbiXfer()
 *
 *  This function reads/writes 'length' words of memory on an HBI device,
 *  starting at the specified AMBA address.  This function is used for
 *  transferring large amounts of contiguous data.
 *
 * Params
 *  'readWrite' - FALSE for read, TRUE for write
 *  'deviceId' - deviceId of the HBI (VCP, VPP, or VCP2) device
 *  'amba_addr' - the AMBA address to start reading/writing at
 *  'length' - the number of data words to read/write
 *  'data' - a pointer to the data to be read/written; use data = (uint16p)0
 *      to write zeroes for all data words
 *  'SetCodeLoadBase' - pointer to a device-dependent function for setting the
 *      Code Load Page base address
 *
 * Returns
 *  TRUE on success, FALSE on failure
 *
 * Notes
 */
bool
VpHbiXfer(
    bool readWrite,
    VpDeviceIdType deviceId,
    uint32 ambaAddr,
    uint32 length,
    uint16p data,
    uint8 byteAddressable,
    bool (*SetCodeLoadBase)(VpDeviceIdType deviceId, uint32 amba_addr))
{
    uint16 hbiPageSize;
    uint8 offset;
    uint16 xferSize;
    
    /* On a byte-addressable architecture, the number of AMBA addresses per HBI
       page is VP_HBI_PAGE_SIZE * 2.  On a word-addressable architecture
       (VCP1), the number of AMBA addresses per HBI page is VP_HBI_PAGE_SIZE. */
    if (byteAddressable) {
        hbiPageSize = VP_HBI_PAGE_SIZE * 2;

        /* Can't access an odd-byte AMBA address via HBI. */
        if (ambaAddr & 1) {
            return FALSE;
        }
    } else {
        hbiPageSize = VP_HBI_PAGE_SIZE;
    }

    /* Determine offset and block size of first transfer. */
    offset = (ambaAddr & (hbiPageSize - 1)) >> byteAddressable;
    xferSize = VP_HBI_PAGE_SIZE - offset;

    /* Enforce alignment restriction of Code Load Page base address. */
    ambaAddr &= ~(hbiPageSize - 1);

    /* If a small amount of data is to be transferred, don't overdo it. */
    if (xferSize > length) {
        xferSize = length;
    }

    /* Set Code Load Page base register to specified AMBA address and select
       Code Load Page for paged offset access. */
    if (
        (VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(255)) == FALSE) ||
        (SetCodeLoadBase(deviceId, ambaAddr) == FALSE)
    ) {
        return FALSE;
    }

    while (xferSize > 0) {

        /* Read or write the next block of data. */
        if (readWrite == TRUE) {
            uint16 cmd = HBI_CMD_PAGED_WRITE(offset, xferSize - 1);
            if (!VpHalHbiWrite(deviceId, cmd, xferSize - 1, data)) {
                return FALSE;
            }
        } else {
            uint16 cmd = HBI_CMD_PAGED_READ(offset, xferSize - 1);
            if (!VpHalHbiRead(deviceId, cmd, xferSize - 1, data)) {
                return FALSE;
            }
        }

        /* Increment the data pointer by the amount of data read/written. */
        if (data != (uint16p)0) {
            data += xferSize;
        }

        /* Decrement the length by the amount of data read/written. */
        length -= xferSize;

        /* Set amount of data to be transferred in next block. */
        if (length < VP_HBI_PAGE_SIZE) {
            xferSize = length;
        } else {
            xferSize = VP_HBI_PAGE_SIZE;
        }

        if (xferSize > 0) {

            /* Increment the AMBA address in the Code Load Page base register by
               VP_HBI_PAGE_SIZE. */
            ambaAddr += hbiPageSize;
            if (SetCodeLoadBase(deviceId, ambaAddr) == FALSE) {
                return FALSE;
            }

            /* In all but the first block, offset = 0. */
            offset = 0;
        }
	}

    return TRUE;
}


/* The following functions take less codespace when inlined.  Not all compilers
 * support inlining, though. */


/* VpHbiRd()
 *
 *  This is the primary HBI "read" function used by the VP API and is
 *  designed to read one instance of an object of the specified type from the
 *  VCP/VPP. HbiHalRd() is useful for reading VP objects such as "profiles"
 *  where the length of the object is fixed and there is more than one instance
 *  of that object. This function hides some of the details VpHalHbiRead(),
 *  making it safer and easier to use.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', 'offset', 'words' - these are filled in by the register info macros
 *      found above in this file.
 *  'words' - number of words to read, minus 1 (if zero, use VpHalHbiCmd)
 *  'pDest' - poiunter to buffer in which to place the data
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 *
 * Notes
 *  The reentrancy of this function depends on the reentrancy of
 *  VpHalHbiRead().
 *  If your compiler supports inline functions, this is a good candidate.
 *  On our system, inlining VpHbiWr() and VpHbiRd() reduces codesize.
 */
bool VpHbiRd(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words,
                uint16p pDest) {
    if (page == VP_DP) {
        return VpHalHbiRead(deviceId, HBI_CMD_DIRECT_READ(offset, words), words,
            pDest);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
        VpHalHbiRead(deviceId, HBI_CMD_PAGED_READ(offset, words), words, pDest);
} /* VpHbiRd() */


/* VpHbiWr()
 *
 *  This is the primary HBI "write" function used by the VP API and is
 *  designed to write one instance of an object of the specified type to the
 *  VCP/VPP. Like VpHbiRd(), VpHbiWr() is most useful for copying VP objects
 *  with fixed lengths and multiple instances. This function provides a slightly
 *  higher level of abstraction than VpHalHbiWrite().
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', 'offset', 'words' - these are filled in by the register info macros
 *      found above in this file.
 *  'words' - number of words to write, minus 1 (if zero, use VpHalHbiCmd)
 *  'pSrc' points to the source buffer from which the data is copied to the
 *      HBI; use pSrc = (uint8p)0 to pass zeroes for all data words.
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 *
 * Notes
 *  The reentrancy of this function depends on the reentrancy of
 *  VpHalHbiWrite().
 *  If your compiler supports inline functions, this is a good candidate.
 *  On our system, inlining VpHbiWr() and VpHbiRd() reduces codesize.
 */
bool VpHbiWr(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words,
                uint16p pSrc) {
    if (page == VP_DP) {
        return VpHalHbiWrite(deviceId, HBI_CMD_DIRECT_WRITE(offset, words),
            words, pSrc);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
        VpHalHbiWrite(deviceId, HBI_CMD_PAGED_WRITE(offset, words),
            words, pSrc);
} /* VpHbiWr() */

#ifdef VP_CC_VPP_SERIES
/* VpHbiRd8()
 *  This function is similar to VpHbiRd except for it is used to read a stream
 * of bytes. This function is useful when reading packets for VPP devices.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', Page from where the access needs to be performed.
 *  'words' - number of words to read, minus 1 (if zero, use VpHalHbiCmd)
 *  'pDest' - poiunter to buffer in which to place the data
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 */
bool VpHbiRd8(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words,
                uint8p pDest) {
    if (page == VP_DP) {
        return VpHalHbiRead8(deviceId, HBI_CMD_DIRECT_READ(offset, words),
                                words, pDest);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
           VpHalHbiRead8(deviceId, HBI_CMD_PAGED_READ(offset, words),
                            words, pDest);
} /* VpHbiRd8() */


/* VpHbiWr8()
 *  This function is similar to VpHbiWr except for it is used to write a stream
 * of bytes. This function is useful when writing packets for VPP devices.
 *
 * Params
 *  'deviceId' - identifies the chip select for the device
 *  'page', Page from where the access needs to be performed.
 *  'words' - number of words to write, minus 1 (if zero, use VpHalHbiCmd)
 *  'pSrc' points to the source buffer from which the data is copied to the
 *      HBI; use pSrc = (uint8p)0 to pass zeroes for all data words.
 *
 * Returns
 *  This function returns TRUE on success, or FALSE on failure.
 */
bool VpHbiWr8(VpDeviceIdType deviceId, uint8 page, uint8 offset, uint8 words,
                uint8p pSrc) {
    if (page == VP_DP) {
        return VpHalHbiWrite8(deviceId, HBI_CMD_DIRECT_WRITE(offset, words),
                                words, pSrc);
    }
    return VpHalHbiCmd(deviceId, HBI_CMD_SELECT_PAGE(page)) &&
	       VpHalHbiWrite8(deviceId, HBI_CMD_PAGED_WRITE(offset, words),
	                        words, pSrc);
} /* VpHbiWr8() */
#endif /* VP_CC_VPP_SERIES */

#endif /* VP_CC_VCP_SERIES || VP_CC_VPP_SERIES || VP_CC_VCP2_SERIES */

