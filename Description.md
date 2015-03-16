Drivers for TNETV1050 C55x DSP, telephony interface, etc.

# Introduction #

The drivers are done using reverse engineering technique.


# Details #

Modules:
  * hwdsp: initialization, reset, and "tight loop" functions for C55x module. Mostly completed, not tested. I think hwu\_lin\_dsp\_loop shows how to execute code on DSP (code should be mmap'ed).
  * tiuhal: TNETV1050's TELEPHONY module driver. Fairly completed. First test with osciloscope. Tx works, Rx not.


Future:
  * implement some MIPS to DSP msg exchange system (dspgateway?). It should be "timer" based, since there's no interrupt from DSP to MIPS.
  * run and test the whole Legerity API.
  * write some "external" API, and asterisk module for it, or use some existing kernel api (telephony api?).

Any help would be really appreciated