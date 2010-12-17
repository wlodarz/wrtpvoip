
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <asm/mach-ar7/ar7.h>

#include "tiuhw.h"

#define DRV_VERSION "0.0.1"
#define DRV_DESC "HWDSP driver"


#define TITAN_DSP_SUBSYSTEM_MEM_BASE (0xa4000000)
#define TNETV1050_DSP_CONTROL 	(TITAN_DSP_SUBSYSTEM_MEM_BASE + 0x01080000)

// some control area?
#define DSP_RST_REG 		(TNETV1050_DSP_CONTROL + 0x00000000)
#define DSP_REG_UNKNOWN_0 	(TNETV1050_DSP_CONTROL + 0x00000014)

// program area?
#define DSP_PROGRAM_AREA	(TITAN_DSP_SUBSYSTEM_MEM_BASE + 0x00000000)
#define DSP_ADDR100 		(DSP_PROGRAM_AREA + 0x00000100)
#define DSP_ADDR104 		(DSP_PROGRAM_AREA + 0x00000104)


// some opcodes
#define OPC_NOP 0x20
#define OPC_B   0x6a

// more complex opcode sequences
#define NOP_NOP_NOP_NOP ((OPC_NOP << 24) | (OPC_NOP << 16) | (OPC_NOP << 8) | (OPC_NOP))
#define BRANCH_0x100    ((OPC_B   << 24) | (0x100))
#define BRANCH_0x4000    ((OPC_B   << 24) | (0x4000))

/* I hope I understand it correctly. */
#define DSP_GO_INTO_TIGHT_LOOP 1

#define DSP_SYSCALL_NR 0x08

void *hwdsp_pre_hook = NULL;
void *hwdsp_post_hook = NULL;

void hwu_lin_dsp_loop(int, int);
extern int hwu_get_tiuhw_if(void);

// supporting functions:
int ar7_get_chip_version_info(void) {
    u8 rev;
    u16 id;

    rev = ar7_chip_rev();
    id = ar7_chip_id();

    return (rev << 16) | id;
}

// checked, shoule be OK
void hwu_lin_titan_dsp_reset(int core, int cmd) {
    unsigned int status;

    printk(KERN_ERR "hwu_lin_titan_dsp_reset(%d,%d)\n", core, cmd);

    if (cmd == DSP_RESET_ON) {
	/* put DSP into reset */

#warning MAGIC NUMBERS
        status = *(volatile unsigned int *) (DSP_RST_REG);
        status &= 0xff04;

    } else {
	/* release DSP from reset */
#warning MAGIC NUMBERS
        status = *(volatile unsigned int *) (DSP_RST_REG);
        status |= 0x00fb;
    }
    printk(KERN_ERR "a1\n");
    *(volatile unsigned int *) (DSP_RST_REG) = status;

    return;
}

void hwu_lin_titan_pre_halt_hook(int a) {

    printk(KERN_ERR "Titan DSP halt initiated (Default)\n");

}

void hwu_lin_titan_post_halt_hook(int a) {

    printk(KERN_ERR "Titan DSP halt done (Default)\n");

}

void hwu_lin_register_halt_hooks(void *prehook, void *posthook) {

    hwdsp_pre_hook = prehook;
    hwdsp_post_hook = posthook;

    return;
}

void hwu_lin_titan_dsp_halt(int core) {
    int tmp;
    int ret;
    unsigned int status;
    int i;

    printk(KERN_ERR "hwu_lin_titan_dsp_halt(%d)\n", core);

    hwu_lin_titan_pre_halt_hook(core);

    // this is device reset
    ar7_device_disable(TITAN_RESET_BIT_DSP);
    for(i=0; i<100; i++) udelay(1000);
    ar7_device_enable(TITAN_RESET_BIT_DSP);

	printk(KERN_ERR "a1\n");

#warning LEARN : addesses & constants meaning

	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
    *(volatile unsigned int *) (DSP_REG_UNKNOWN_0) = 80;
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
    tmp = *(volatile unsigned int *) (DSP_RST_REG);
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
	printk(KERN_ERR "a1\n");
    printk(KERN_ERR "hwu_lin_titan_dsp_halt() Before: DSP_RST_REG=0x%08x\n", tmp);

	printk(KERN_ERR "a1\n");
    ret = hwu_get_tiuhw_if();
	printk(KERN_ERR "a1\n");
    printk(KERN_ERR "hwu_get_tiuhw_if = %d\n", ret);

#warning TO LEARN : what those constants mean

	printk(KERN_ERR "a1\n");
    if (ret != TIHW_INTERNAL)
        status = 0x300; // with FPGA - only software reset?
    else
        status = 0x304; // internal - so software reset with peripherals?

    *(volatile unsigned int *) (DSP_RST_REG) = status;
	printk(KERN_ERR "a1\n");

    status = *(volatile unsigned int *) (DSP_RST_REG);
    printk(KERN_ERR "hwu_lin_titan_dsp_halt() After: DSP_RST_REG=0x%08x\n", status);

    hwu_lin_titan_post_halt_hook(core);

    printk(KERN_ERR "Putting DSP into reset\n");

    hwu_lin_titan_dsp_reset(core, DSP_RESET_ON);

    printk(KERN_ERR "Installing tight loop\n");

    hwu_lin_dsp_loop(core, DSP_GO_INTO_TIGHT_LOOP);

}

void hwu_lin_dsp_loop(int core, int flag) {

    core &= 0xffff;

    printk(KERN_ERR "hwu_lin_dsp_loop(%d,%d)\n", core, flag);

    if (flag == DSP_GO_INTO_TIGHT_LOOP)
    {

	// just small loop, (NOP? , JUMP to 0x100
        //*(volatile unsigned int *) (DSP_ADDR100) = 0x20202020; // 0x00100:
        *(volatile unsigned int *) (DSP_ADDR100) = NOP_NOP_NOP_NOP; // 0x001000:
							       //   NOP; NOP; NOP; NOP;
        //*(volatile unsigned int *) (DSP_ADDR104) = 0x6a000100; // 0x00104:
        *(volatile unsigned int *) (DSP_ADDR104) = BRANCH_0x100; // 0x00104:
							       //   B 0x000100;

        printk(KERN_ERR "putting dsp in tight loop status=%d\n", flag);

    }
    else
    {

	// this causes DSP to jump to 0x4000 (start of the program?)
        //*(volatile unsigned int *) (DSP_ADDR104) = 0x6a004000; // 0x00104:
        *(volatile unsigned int *) DSP_ADDR104 = BRANCH_0x4000; // 0x001040:
							     //   B 0x004000

        printk(KERN_ERR "jumping to 0x4000 status=%d\n", 0);
    }

    return;
}

void hwu_dsp_init(void) {

    hwu_lin_titan_dsp_halt(DSP_CORE_0);

    printk(KERN_ERR "Taking DSP out of reset\n");

    hwu_lin_titan_dsp_reset(DSP_CORE_0, DSP_RESET_OFF);
    printk(KERN_ERR "Took DSP out of reset\n");

    return;
}

/* the syscall is used to take DSP from reset */
int dsp_syscall(void *a) {
    int ret = 0;
    int op = 0, core = 0, cmd = 0;
    int *msg = (int *)a;

    op = msg[0];
    if (op < 10) {
        switch(op) {
		case 0: // 3ec
		case 5:
		case 7:
			ret = 0;
			break;
		case 1: // 368
#warning TODO: what we return
			ret = 0;
			// save on 12(a) = 0xf;
			break;
		case 2: // 3e8
			ret = 1;
			break;
		case 3: // 380
			// adr = 4(a);
			// call some function at 384
			core = msg[1];
			cmd = msg[2];
			hwu_lin_titan_dsp_reset(core, cmd);
			break;
		case 4: // 374
#warning TODO: what we return
			ret = 0;
			// save on 12(a) = 1;
			break;
		case 6: // 3b4
			// tight loop, or jump to 0x4000
			core = msg[1];
			cmd = msg[2];
			hwu_lin_dsp_loop(core, cmd);
			break;
		case 8: // 39c
			core = msg[1];
			hwu_lin_titan_dsp_halt(core);
			break;
		case 9: // 3d0
#warning IMPLEMENT ME!!
			// avalanche_get_chip_version_info
			break;
		default:
			printk(KERN_ERR "ERROR at %s:%d\n", __FUNCTION__, __LINE__);
	}
    }
    else 
        ret = 1;

out:
    return ret;
}

static int __init tidsp_init_module(void) {
    int chipid = 0;

    printk(KERN_ERR "HW_DSP module loading\n");
    printk(KERN_ERR "Version: %s\n", SOURCE_DATE);

    hwu_dsp_init();
#warning TODO: syscalls
    // ti_syscalls[DSP_SYSCALL_NR] = dsp_syscall;

    printk(KERN_ERR "HW_DSP module loaded\n");

    chipid = ar7_get_chip_version_info();

    printk(KERN_ERR "Silicon revision = %0x\n", chipid);

    return 0;
}

static void __exit tidsp_cleanup_module(void) {

#warning TODO: syscall
    // ti_syscalls[DSP_SYSCALL_NR] = NULL;

    ar7_device_disable(TITAN_RESET_BIT_DSP);

    printk(KERN_ERR "HW_DSP module unloaded\n");

}

module_init(tidsp_init_module);
module_exit(tidsp_cleanup_module);

MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Wlodzimierz Kalawski <wlk at poczta.fm>");
MODULE_LICENSE("GPL v2");



EXPORT_SYMBOL_GPL(hwu_lin_register_halt_hooks);


