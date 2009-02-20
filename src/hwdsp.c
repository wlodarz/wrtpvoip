#define DRV_VERSION "0.0.1"
#define DRV_DESC "HWDSP driver"


// mailbox base?
#define TNETV1050_DSP_BASE 0xa5080000

#define DSP_REG1 (TNETV1050_DSP_BASE + 0x000000)
#define DSP_RST_REG (TNETV1050_DSP_BASE + 0x000014)
#define DSP_ADDR100 (TNETV1050_DSP_BASE + 0x000100)
#define DSP_ADDR104 (TNETV1050_DSP_BASE + 0x000104)

#define DSP_RESET_ON 1
#define DSP_RESET_OFF 0

/* I hope I understand it correctly. */
#define DSP_CORE_0 0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <asm/ar7/ar7.h>

#define DSP_SYSCALL_NR 0x08

void *hwdsp_pre_hook = NULL;
void *hwdsp_post_hook = NULL;

void hwu_lin_dsp_loop(int, int);
int hwu_get_tiuhw_if();

// supporting functions:
int ar7_get_chip_version_info(void) {
    u8 rev;
    u16 id;

    rev = ar7_chip_rev();
    id = ar7_chip_id();

    return (rev << 16) | id;
}

void hwu_lin_titan_dsp_reset(int core, int cmd) {
    int status;

    printk(KERN_ERR "hwu_lin_titan_dsp_reset(%d,%d)\n", core, cmd);

    if (cmd == DSP_RESET_ON) {
	/* put DSP into reset */

#warning MAGIC NUMBERS
        int status = *(int *) (DSP_REG1);
        status &= (0x000000fb);

    }
    else {
	/* release DSP from reset */
#warning MAGIC NUMBERS
        int status = *(int *) (DSP_REG1);
        status |= 0x000000fb;
    }
    *(int *) (DSP_REG1) = status;

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
    int cnt1, cnt2;
    int tmp;
    long long test;
    int ret;
    int status;

    printk(KERN_ERR "hwu_lin_titan_dsp_halt(%d)\n", core);

    hwu_lin_titan_pre_halt_hook(core);

    ar7_device_disable(AR7_RESET_BIT_DSP);

#warning LEARN : delay, let's find out how long & replace it with some defines

    cnt1 = 99;
    do {

        cnt2 = 0x199996c0;

        tmp = loops_per_jiffy;

        test = tmp * cnt2;
        tmp = (test >> 16) & 0xffffffff;

        while (tmp--) ;

    }
    while ((cnt1--) >= 0);

    ar7_device_enable(AR7_RESET_BIT_DSP);

#warning LEARN : addesses & constants meaning

    *(volatile int *) (DSP_RST_REG) = 0x80;
    tmp = *(volatile int *) (DSP_REG1);
    printk(KERN_ERR "hwu_lin_titan_dsp_halt() Before: DSP_RST_REG=0x%08x\n",
           tmp);

#warning WHAT ABOUT THAT?!?!?!?!?!?!?!
    ret = hwu_get_tiuhw_if(0);

#warning TO LEARN : what those constants mean

    if (ret == 1)
        status = 0x300;
    else
        status = 0x304;

    *(volatile int *) (DSP_REG1) = status;

    status = *(volatile int *) (DSP_REG1);
    printk(KERN_ERR "hwu_lin_titan_dsp_halt() After: DSP_RST_REG=0x%08x\n", 1);

    hwu_lin_titan_post_halt_hook(core);

    printk(KERN_ERR "Putting DSP into reset\n");

    hwu_lin_titan_dsp_reset(core, DSP_RESET_ON);

    printk(KERN_ERR "Installing tight loop\n");

    hwu_lin_dsp_loop(core, 1);

}

void hwu_lin_dsp_loop(int core, int flag) {

    core &= 0xffff;

    printk(KERN_ERR "hwu_lin_dsp_loop(%d,%d)\n", core, flag);

#warning TO LEARN : what this code means

    if (flag != 0)
    {

	// just small loop, (NOP? , JUMP to 0x100
        *(int *) (DSP_ADDR100) = 0x20202020; 0x100;
        *(int *) (DSP_ADDR104) = 0x6a000100; 0x104;

        printk(KERN_ERR "putting dsp in tight loop status=%d\n", flag);

    }
    else
    {

	// this causes DSP to jump to 0x4000 (start of the program?)
        *(int *) DSP_ADDR104 = 0x6a004000;

        printk(KERN_ERR "jumping to 0x4000 status=%d\n", 0);
    }

    return;
}

void hwu_dsp_init(void) {

    hwu_lin_titan_dsp_halt(DSP_CORE_0);

    printk(KERN_ERR "Taking DSP out of reset\n");

    hwu_lin_titan_dsp_reset(DSP_CORE_0, DSP_RESET_OFF);

    return;
}

/* the syscall is used to take DSP from reset */
int dsp_syscall(void *a) {
    int ret = 0;
    int reg = 0;

    reg = *(int *) a;
    if (reg >= 10) {

        reg = (reg << 2);

#warning TODO: where goto
        //              goto out;

        ret = 0x0f;
        goto out;

        ret = 1;


        hwu_lin_titan_dsp_reset(DSP_CORE_0, DSP_RESET_OFF);

#warning TODO : should be finished and checked
        // ret = ..
    }
    else {

        ret = 1;
    }

out:

    return ret;
}

static int __init tidsp_init_module(void) {
    int chipid = 0;

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

    ar7_device_disable(AR7_RESET_BIT_DSP);

    printk(KERN_ERR "HW_DSP module unloaded\n");

}

module_init(tidsp_init_module);
module_exit(tidsp_cleanup_module);

MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Wlodzimierz Kalawski <wlk at poczta.fm>");
MODULE_LICENSE("GPL v2");



EXPORT_SYMBOL_GPL(hwu_lin_register_halt_hooks);


