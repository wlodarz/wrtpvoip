Modules description:
1. hwdsp.c
The module initializes DSP hardware, and provides external API for reseting (probably needed for DSP firmware loading)
2. tiuhal.c
Provides low-level API for accessing telephony interface (init/read/write byte(s)), and some supporting functions.
3. tiuhw_an.c
Will be incorporated into legerity voicepath HAL module. Right now only for VpMpiCmd
The original version contains voicepath api. I will move it to the separate module.
4. mxpmod.c
Mailbox interface

