
#ifndef __TIUHW_H
#define __TIUHW_H

#define SOURCE_DATE "$Date$"

#include "legerity/vp_api_types.h"

#define TIHW_INTERNAL 1
#define TIHW_EXTERNAL 2

#define DSP_CORE_0 0
#define DSP_RESET_ON 1
#define DSP_RESET_OFF 0

#define DSP_MAX_CLOCK_SPEED 125000000


void hwu_lin_register_halt_hooks(void *, void *);
void VpMpiCmd(VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire interface for command */
	      uint8 ecVal,                /**< Value to write to the EC register */
	      uint8 cmd,                  /**< Command number */
	      uint8 cmdLen,               /**< Number of bytes used by command (cmd) */
	      uint8 *dataPtr);            /**< Pointer to the data location */


#endif //  __TIUHW_H
