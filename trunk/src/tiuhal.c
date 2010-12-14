// loops_per_jiffy = 429496

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/mach-ar7/ar7.h>
#include <asm/mach-ar7/gpio.h>

#include "tiuhw.h"
#include "tiuhal.h"

#define DRV_VERSION "0.0.1"
#define DRV_DESC "TIUHal driver"


int tnetv1050_tid_init(tiuhw_device *a);
int tnetv1050_tid_read(char tid, int ecval_arg, int data, int len);
int tnetv1050_tid_write(char tid_arg, int ecval_arg, char *data, int len);
int func03(void);
int func10(void);
int func11(void);
int func12(void);
int func13(void);
int func20(void);
int func21(void);
int func22(int a, int b);
int func23(int a, int b);
int tnetv1050_get_tid_type(char a, int b);

char *prom_getenv(char *);
static irqreturn_t tnetv1050_tid_interrupt_handler(int i, void *data);
static irqreturn_t test_irq_handler(int i, void *data);
int hwu_get_tiuhw_if(void);
int tiuhw_reset_tid(char tid, int cmd);
void tiuhw_select_tid(char tid);
void tiuhw_deselect_tid(char tid);
int tiuhw_get_tnetv1050_tid_type(void);
int tnetv1050_get_tid_type(char tid, int tid_type);
int tiuhw_get_tid_type(char tid);


#define DSP_CORE_0 0
#define DSP_RESET_ON 1
#define DSP_RESET_OFF 0

#define DSP_REG_PCMCR1 	0xa5081000
#define DSP_REG_SPCR1 	0xa5081010
#define DSP_REG_DXR1    0xa5081018

/* pointers from data + 0x0000 */
api_type tnetv1050_api = {
		tnetv1050_tid_init, // 0x000
		tnetv1050_tid_read, // 0x714
		tnetv1050_tid_write, // 0x7c8
		func03, // 0x878
		func10, // 0x888
		func11, // 0x880
		func12, // 0x890
		func13, // 0x898
		func20, // 0x8a0
		func21, // 0x8a8
		func22, // 0x8b0
		tnetv1050_get_tid_type  // 0x904
};

typedef struct {
	char *tidname;
	int tidtype;
} s_tidname2type;

int unknown_global1 = 1; // data + 0x0034
api_type hw_apis;	// data + 0x0038 - (+0x0034)
int should_reset_tid = 1; // data + 0x0070
s_tidname2type tidname2type[8] = {
	{"SI3210_SI3050", 1},
	{"DUAL_SI3210", 2},
	{"DUAL_SI3050", 3},
	{"DUAL_SI3034", 5},
	{"VE882XX", 12},
	{"AUTO", 255},
	{NULL, 0},
};
int tmp_variable0 = 0; 			// data + 0x00ac
int not_initialized = 1; 		// data + 0x00b0
int interface_type = TIHW_EXTERNAL; 	// data + 0x00b4
api_type *tiuhw_api = NULL;		// data + 0x00b8
int tiuhw_dsp_clock_mult = 0; 		// data + 0x00bc
int tiuhw_dsp_input_clock_speed = 0; 	// data + 0x00c0
int hwu_settings_mod[4]; 		// data + 0x00c4
char *stub_text = "tele_if_stub";


void *other_pointer_src = 0;
void *other_pointer_dst = 0;

#define WAIT_LOOP \
	index = 0; \
        do { \
		ltmp1 = dsp_clock_mult * dsp_input_clock_speed; \
		tmp_v1 = ltmp1 & 0xffffffff; \
		ltmp1 = (unsigned long long)(tmp_v1) * (unsigned long long)0x431bde83; \
		tmp_v0 = ((((ltmp1 >> 0x20) >> 0x14) - 1) << 0x02); \
	} while(++index < tmp_v0); 


/*
tiuhal.o:     file format elf32-tradlittlemips

Disassembly of section .text:
*/

/* FUNCTION: DONE, need review */
/* probably init function */
int tnetv1050_tid_init(tiuhw_device *tiddev)
{
	int reg;
	int tmp0, tmp1, tmp3, tmp5, tmp6;
	int tmp;
	int tmp_v0, tmp_v1;
	int tmp_a1;
	int tid_type = 0;
	int tid=0;
	int index = 0;
	int dsp_clock_mult;
	int dsp_input_clock_speed;
	long long dsp_freq;
	long long ltmp1, ltmp2;
	//int tmp_s0, tmp_s3;
	int ret = 1;
	int const_0x4;

	tiddev->field2 = 11;
	tiddev->field0 = 1;
	tiddev->field1 = 0;
	tiddev->field3 = 2;

	tiddev->field4 = 0;
	if(unknown_global1 != 0) unknown_global1 = 0;

	/* DSP clock set to telephony clock I/O */
	reg = *(volatile int *)0xa8611a00;
	reg &= 0xffcfffff;
	*(volatile int *)0xa8611a00;

	ar7_device_enable(TITAN_RESET_BIT_DSP);

	reg = *(volatile int *)0xa5080000;
	reg &= 0xfffb;
	*(volatile int *)0xa5080000 = reg;

	*(volatile int *)0xa5080104 = 4;

	/* PIN_SEL_13 functions - telephony interface */
	reg = *(volatile int *)0xa861163c;
	reg &= 0x00f00fff;
	*(volatile int *)0xa861163c = reg;

	reg = *(volatile int *)0xa861163c;
	reg |= 0x55055000;
	*(volatile int *)0xa861163c = reg;

        /* PIN_SEL_14 functions - telephony interface */
	reg = *(volatile int *)0xa8611640;
	reg &= 0x00003000;
	*(volatile int *)0xa8611640 = reg;

	reg = *(volatile int *)0xa8611640;
	reg |= 0x00004144;
	*(volatile int *)0xa8611640 = reg;

	// ----
	const_0x4 = 0x431bde83;
	dsp_clock_mult = tiuhw_dsp_clock_mult; // t2
	dsp_input_clock_speed = tiuhw_dsp_input_clock_speed; // t3

	dsp_freq = dsp_clock_mult * dsp_input_clock_speed; // a1
	dsp_freq = (dsp_freq & 0xffffffff) * const_0x4; // a1 * v1
	*(volatile int *)0xa5080104 = (((dsp_clock_mult-1) << 12)|0x0000027e);
	reg = *(volatile int *)0xa5080000;
	reg |= 0x00000004;
	*(volatile int *)0xa5080000 = reg;
	index = 0;
	{
		const_0x4 = 0x431bde83;
		dsp_clock_mult = tiuhw_dsp_clock_mult; // t2
		dsp_input_clock_speed = tiuhw_dsp_input_clock_speed; // t3

		dsp_freq = dsp_clock_mult * dsp_input_clock_speed; // a1
		dsp_freq = (dsp_freq & 0xffffffff) * const_0x4; // a1 * v1

		tmp5 = ((((dsp_freq >> 32) & 0xffffffff) >> 0x14) - 1) << 0x02;
	} while(tmp5>index++);
	// ----


	// ----
	dsp_clock_mult = tiuhw_dsp_clock_mult;
	dsp_input_clock_speed = tiuhw_dsp_input_clock_speed;
	dsp_freq = dsp_clock_mult * dsp_input_clock_speed;
	dsp_freq = (dsp_freq & 0xffffffff) * 0x431bde83;
	tmp_v0 = ((((dsp_freq >> 32) & 0xffffffff) >> 0x14) - 1) | 0x81080000;
	*(volatile int *)DSP_REG_SPCR1 = tmp_v0;
	reg = *(volatile int *)DSP_REG_SPCR1; // SPCR1
	printk(KERN_ERR "SPCR1 = %08lx\n", reg);
	// ----

	// ----
	dsp_clock_mult = tiuhw_dsp_clock_mult;
	dsp_input_clock_speed = tiuhw_dsp_input_clock_speed;
	WAIT_LOOP
	// ----

	// ----
	dsp_freq = dsp_clock_mult * dsp_input_clock_speed;
	ltmp1 = dsp_freq * 0x431bde83;
	*(volatile int *)0xa5081004 = 0x1;
	tmp_v0 = ((((ltmp1 >> 32) & 0xffffffff) >> 0x14) - 1) >> 0x02;
	index = 0;
	{
		dsp_freq = dsp_clock_mult * dsp_input_clock_speed;
		ltmp1 = dsp_freq * 0x431bde83;
		tmp_v0 = ((((ltmp1 >> 32) & 0xffffffff) >> 0x14) - 1) >> 0x02;
	} while (tmp_v0 > ++index);
	// ----

	// ----
	dsp_freq = dsp_clock_mult * dsp_input_clock_speed;	
	ltmp1 = dsp_freq * 0x10624dd3;
	tmp_v1 = (ltmp1 >> 32) & 0xffffffff;
	tmp_v1 >>= 0x12;
	tmp_v1 -= 1;
	tmp_v1 = tmp_v1 | 0x00ff8000;
	*(volatile int *)DSP_REG_PCMCR1 = tmp_v1;

	reg = *(volatile int *)DSP_REG_PCMCR1;
	printk(KERN_ERR "PCMCR1 = %08lx\n", reg);

	index = 0;
	{
		dsp_clock_mult = tiuhw_dsp_clock_mult;
		dsp_input_clock_speed = tiuhw_dsp_input_clock_speed;
		dsp_freq = dsp_clock_mult * dsp_input_clock_speed;
		ltmp1 = dsp_freq * 0x431bde83;
		tmp_v0 = (((((ltmp1 >> 32) & 0xffffffff) >> 0x14) - 1) >> 0x02);
	} while (tmp_v0 > ++index);
	*(volatile int *)0xa5081020 = 0;
	// ----

	{
		tid_type = tiuhw_get_tid_type(tid);

		if((tid_type < 0) || (tid_type >= 4 && tid_type != 12)) {
			tid_type = tiuhw_get_tid_type(tid);
			printk(KERN_ERR "TNETV1050_HAL : unsupported TID type detected %u\n", tid_type);
			ret = 0;
		}
		tid = (tid + 1) & 0xff;
	} while(tid == 0);

	return ret;
}

/* FUNCTION: DONE, need review */
static irqreturn_t tnetv1050_tid_interrupt_handler(int i, void *data)
{
	return IRQ_HANDLED;
}

static irqreturn_t test_irq_handler(int i, void *data)
{
	return IRQ_HANDLED;
}

/* FUNCTION: TODO, NEED CAREFULL REVIEW */
int tnetv1050_tid_writebyte(int byte) {
	int a0, a1;
	long long tmp1, ltmp2;
	int tmp2, tmp3, tmp4, tmp9, tmp10;
	int reg;
	int tmp_s3;
	unsigned int dsp_mult, dsp_speed;
	unsigned long dsp_freq;
/*
     450:	308400ff 	andi	$a0,$a0,0xff
*/
	//byte = a&0xff;
/*
     454:	3c080000 	lui	$t0,0x0
     458:	8d080000 	lw	$t0,0($t0)
*/
	dsp_mult = tiuhw_dsp_clock_mult;
/*
     45c:	3c090000 	lui	$t1,0x0
     460:	8d290000 	lw	$t1,0($t1)
*/
	dsp_speed = tiuhw_dsp_input_clock_speed;
/*
     464:	01090018 	mult	$t0,$t1
*/
	dsp_freq = dsp_mult * dsp_speed;

/*
     468:	3c02431b 	lui	$v0,0x431b
     46c:	00002812 	mflo	$a1
     470:	3442de83 	ori	$v0,$v0,0xde83
     474:	00000000 	nop
     478:	00a20019 	multu	$a1,$v0
     47c:	3c07a508 	lui	$a3,0xa508
     480:	34e71018 	ori	$a3,$a3,0x1018  // a3 = 0xa5081018
     484:	3c06a508 	lui	$a2,0xa508
     488:	34c61010 	ori	$a2,$a2,0x1010  // a2 = DSP_REG_SPCR1
     48c:	3c038148 	lui	$v1,0x8148	// v1 = 0x8148
     490:	00002810 	mfhi	$a1 		// a1 - tmp2
     494:	ace40000 	sw	$a0,0($a3)
*/
	*(volatile int *)DSP_REG_DXR1 = byte;


/*
     498:	00051502 	srl	$v0,$a1,0x14
     49c:	2442ffff 	addiu	$v0,$v0,-1
     4a0:	00431025 	or	$v0,$v0,$v1
     4a4:	acc20000 	sw	$v0,0($a2)
*/
	tmp2 = (unsigned int)(((long long)(dsp_freq * 0x431bde83) >> 32)); // & 0xffffffff;
	tmp3 = (((tmp2 >> 0x14) - 1) | 0x81480000);
	*(volatile int *)DSP_REG_SPCR1 = tmp3;

/*
     4a8:	00052cc2 	srl	$a1,$a1,0x13
     4ac:	24a50001 	addiu	$a1,$a1,1
*/
	tmp2 >>= 0x13;
	tmp2 += 1;

/*
     4b0:	00051080 	sll	$v0,$a1,0x2
     4b4:	00451021 	addu	$v0,$v0,$a1
*/
	tmp3 = (tmp2 << 2);
	tmp3 += tmp2;
	

/*
     4b8:	000210c0 	sll	$v0,$v0,0x3
     4bc:	00451023 	subu	$v0,$v0,$a1
*/
	tmp3 <<= 0x03;
	tmp3 -= tmp2;

/*
     4c0:	00021100 	sll	$v0,$v0,0x4
     4c4:	00451021 	addu	$v0,$v0,$a1
*/
	tmp3 <<= 0x04;
	tmp3 += tmp2;

/*
     4c8:	8ce30000 	lw	$v1,0($a3)
     4cc:	04610029 	bgez	$v1,574 <init_module-0x3c8>
     4d0:	00022100 	sll	$a0,$v0,0x4
*/
	a1 = tmp3 << 0x04;
	reg = *(volatile int *)DSP_REG_DXR1;
	a0 = reg;
	if(reg<0) {

/*
     4d4:	2484ffff 	addiu	$a0,$a0,-1
*/
		reg -= 1;

/*
     4d8:	2402ffff 	li	$v0,-1
     4dc:	10820025 	beq	$a0,$v0,574 <init_module-0x3c8>
     4e0:	01002821 	move	$a1,$t0
*/
		tmp3 = -1;	
		tmp2 = dsp_mult;
		while(reg != -1) {

/*
     4e4:	00a04021 	move	$t0,$a1
     4e8:	01203821 	move	$a3,$t1
     4ec:	01070018 	mult	$t0,$a3
     4f0:	3c02431b 	lui	$v0,0x431b
     4f4:	00001812 	mflo	$v1
     4f8:	3442de83 	ori	$v0,$v0,0xde83
     4fc:	00000000 	nop
*/
			dsp_mult = tmp2;
			tmp9 = dsp_speed;
			ltmp2 = dsp_mult * tmp9;
			
/*
     500:	00620019 	multu	$v1,$v0
     504:	00001010 	mfhi	$v0
     508:	00021502 	srl	$v0,$v0,0x14
     50c:	2442ffff 	addiu	$v0,$v0,-1
     510:	00021100 	sll	$v0,$v0,0x4
*/
			tmp10 = ((((ltmp2 >> 32) & 0xffffffff)) * 0x431bde83);
			tmp10 >>= 0x14;
			tmp10 -= 1;
			tmp10 <<= 0x04;
/*
     514:	1040000f 	beqz	$v0,554 <init_module-0x3e8>
     518:	00003021 	move	$a2,$zero
*/
			// a2 = 0;
// #error ALA
			if(tmp10 != 0) {
/*
     51c:	01070018 	mult	$t0,$a3
     520:	3c02431b 	lui	$v0,0x431b
     524:	00001812 	mflo	$v1
     528:	3442de83 	ori	$v0,$v0,0xde83
     52c:	00000000 	nop
*/
				ltmp2 = dsp_mult * tmp9;
/*
     530:	00620019 	multu	$v1,$v0
     534:	00001010 	mfhi	$v0
     538:	24c60001 	addiu	$a2,$a2,1
     53c:	00021502 	srl	$v0,$v0,0x14
     540:	2442ffff 	addiu	$v0,$v0,-1
     544:	00021100 	sll	$v0,$v0,0x4
*/
				tmp10 = ((((ltmp2 >> 32) & 0xffffffff)) * 0x431bde83);
				tmp10 >>= 0x14;
				tmp10 -= 1;
				tmp10 <<= 0x04;
/*
     548:	00c2102b 	sltu	$v0,$a2,$v0
*/
/*
     54c:	1440fff4 	bnez	$v0,520 <init_module-0x41c>
     550:	01070018 	mult	$t0,$a3
*/
			}
/*
     554:	3c02a508 	lui	$v0,0xa508
     558:	34421018 	ori	$v0,$v0,0x1018
     55c:	8c420000 	lw	$v0,0($v0)
     560:	04410004 	bgez	$v0,574 <init_module-0x3c8>
     564:	2402ffff 	li	$v0,-1
*/
			reg = *(volatile int *)DSP_REG_DXR1;
			if(reg < 0) {
				a0 = -1;
				break;
			}
/*
     568:	2484ffff 	addiu	$a0,$a0,-1
     56c:	1482ffde 	bne	$a0,$v0,4e8 <init_module-0x454>
     570:	00a04021 	move	$t0,$a1
*/
		}
	}
/*
     574:	03e00008 	jr	$ra
     578:	0004102b 	sltu	$v0,$zero,$a0
*/
	return (0 < a0);
}

/* FUNCTION: TODO, NEED CAREFULL REVIEW */
int tnetv1050_tid_readbyte(char *pbuffer)
{
	int tmp;
	int tmp_t0, tmp_t1;
	signed int reg;
	long long ltmp1;
	int tmp_v0, tmp_v1;
	int dsp_clock_mult, dsp_input_clock_speed;
	int tmp_a0, tmp_a2, tmp_a3;
/*
     57c:	00805021 	move	$t2,$a0
*/
/*
     580:	3c080000 	lui	$t0,0x0
     584:	8d080000 	lw	$t0,0($t0)
*/
	dsp_clock_mult = tiuhw_dsp_clock_mult;
/*
     588:	3c090000 	lui	$t1,0x0
     58c:	8d290000 	lw	$t1,0($t1)
*/
	dsp_input_clock_speed = tiuhw_dsp_input_clock_speed;
/*
     590:	01090018 	mult	$t0,$t1
     594:	3c02431b 	lui	$v0,0x431b
     598:	00002012 	mflo	$a0
     59c:	3442de83 	ori	$v0,$v0,0xde83
     5a0:	00000000 	nop
     5a4:	00820019 	multu	$a0,$v0
*/
	ltmp1 = dsp_clock_mult * dsp_input_clock_speed;
	ltmp1 = (ltmp1 & 0xffffffff) * 0x431bde83;
/*
     5a8:	3c05a508 	lui	$a1,0xa508
     5ac:	34a51010 	ori	$a1,$a1,0x1010
     5b0:	00002010 	mfhi	$a0
*/

/*
     5b4:	00003821 	move	$a3,$zero
     5b8:	00041cc2 	srl	$v1,$a0,0x13
     5bc:	24630001 	addiu	$v1,$v1,1
     5c0:	00031080 	sll	$v0,$v1,0x2
     5c4:	00431021 	addu	$v0,$v0,$v1
     5c8:	000210c0 	sll	$v0,$v0,0x3
     5cc:	00431023 	subu	$v0,$v0,$v1
     5d0:	00021100 	sll	$v0,$v0,0x4
     5d4:	00431021 	addu	$v0,$v0,$v1
     5d8:	00023100 	sll	$a2,$v0,0x4
     5dc:	00042502 	srl	$a0,$a0,0x14
     5e0:	2484ffff 	addiu	$a0,$a0,-1
     5e4:	3c028128 	lui	$v0,0x8128
     5e8:	00821025 	or	$v0,$a0,$v0
     5ec:	aca20000 	sw	$v0,0($a1)
     5f0:	00042100 	sll	$a0,$a0,0x4
*/
	tmp_a0 = (ltmp1 >> 32) & 0xffffffff;
	tmp_v1 = tmp_a0 >> 0x13;
	tmp_v1 += 1;
	tmp_v0 = tmp_v1 << 0x02;
	tmp_v0 = tmp_v0 + tmp_v1;
	tmp_v0 = tmp_v0 << 0x03;
	tmp_v0 = tmp_v0 - tmp_v1;
	tmp_v0 = tmp_v0 << 0x04;
	tmp_v0 = tmp_v0 + tmp_v1;
	tmp_a2 = tmp_v0 << 0x04;
	tmp_a0 = tmp_a0 >> 0x014;
	tmp_a0 -= 1;
	tmp_v0 = tmp_a0 | 0x81280000;
	reg = tmp_v0;
	// 0x8128 - (2) read operation
	*(volatile int *)DSP_REG_SPCR1 = reg;
	tmp_a3 = 0;
/*
     5f4:	10800012 	beqz	$a0,640 <init_module-0x2fc>
     5f8:	3c02a508 	lui	$v0,0xa508
*/
	tmp_a0 <<= 0x04;
	if(tmp_a0) {
		{
/*
     5fc:	01002021 	move	$a0,$t0
     600:	01202821 	move	$a1,$t1
     604:	00850018 	mult	$a0,$a1
     608:	3c02431b 	lui	$v0,0x431b
     60c:	00001812 	mflo	$v1
     610:	3442de83 	ori	$v0,$v0,0xde83
     614:	00000000 	nop
     618:	00620019 	multu	$v1,$v0
     61c:	00001010 	mfhi	$v0
     620:	24e70001 	addiu	$a3,$a3,1
     624:	00021502 	srl	$v0,$v0,0x14
     628:	2442ffff 	addiu	$v0,$v0,-1
     62c:	00021100 	sll	$v0,$v0,0x4
     630:	00e2102b 	sltu	$v0,$a3,$v0
     634:	1440fff4 	bnez	$v0,608 <init_module-0x334>
     638:	00850018 	mult	$a0,$a1
     63c:	3c02a508 	lui	$v0,0xa508
*/
			ltmp1 = dsp_clock_mult * dsp_input_clock_speed;
			tmp_v1 = ltmp1 & 0xffffffff;
			ltmp1 = tmp_v1 * 0x431bde83;
			tmp_v0 = (ltmp1 >> 32) & 0xffffffff;
			tmp_a3 += 1;
			tmp_v0 >>= 0x14;
			tmp_v0 -= 1; 
			tmp_v0 <<= 0x04;
		} while(tmp_a3 < tmp_v0);
	}
/*
     640:	3442101c 	ori	$v0,$v0,0x101c
     644:	8c430000 	lw	$v1,0($v0)
     648:	0461002c 	bgez	$v1,6fc <init_module-0x240>
     64c:	2402ffff 	li	$v0,-1
     650:	24c6ffff 	addiu	$a2,$a2,-1
     654:	10c20029 	beq	$a2,$v0,6fc <init_module-0x240>
     658:	00000000 	nop
*/
	reg = *(volatile int *)0xa508101c;
	tmp_v0 = -1;
	if(reg < 0) {
		tmp_a2 -= 1;
		if(tmp_a2 != -1) {
/*
     65c:	3c090000 	lui	$t1,0x0
     660:	8d290000 	lw	$t1,0($t1)
*/
			dsp_clock_mult = tiuhw_dsp_clock_mult;
/*
     664:	3c080000 	lui	$t0,0x0
     668:	8d080000 	lw	$t0,0($t0)
*/
			dsp_input_clock_speed = tiuhw_dsp_input_clock_speed;
/*
     66c:	01203821 	move	$a3,$t1
     670:	01002821 	move	$a1,$t0
     674:	00e50018 	mult	$a3,$a1
     678:	3c02431b 	lui	$v0,0x431b
     67c:	00001812 	mflo	$v1
     680:	3442de83 	ori	$v0,$v0,0xde83
     684:	00000000 	nop
     688:	00620019 	multu	$v1,$v0
     68c:	00001010 	mfhi	$v0
     690:	00021502 	srl	$v0,$v0,0x14
     694:	2442ffff 	addiu	$v0,$v0,-1
     698:	00021100 	sll	$v0,$v0,0x4
     69c:	1040000f 	beqz	$v0,6dc <init_module-0x260>
     6a0:	00002021 	move	$a0,$zero
*/
			ltmp1 = dsp_clock_mult * dsp_input_clock_speed;
			tmp_v1 = ltmp1 & 0xffffffff;
			ltmp1 = tmp_v1 * 0x431bde83;
			tmp_v0 = (ltmp1 >> 32) & 0xffffffff;
			tmp_v0 >>= 0x14;
			tmp_v0 -= 1;
			tmp_v0 <<= 0x04;
			tmp_a0 = 0;
			if(tmp_v0 != 0) {
				{
/*
     6a4:	00e50018 	mult	$a3,$a1
     6a8:	3c02431b 	lui	$v0,0x431b
     6ac:	00001812 	mflo	$v1
     6b0:	3442de83 	ori	$v0,$v0,0xde83
     6b4:	00000000 	nop
     6b8:	00620019 	multu	$v1,$v0
     6bc:	00001010 	mfhi	$v0
     6c0:	24840001 	addiu	$a0,$a0,1
     6c4:	00021502 	srl	$v0,$v0,0x14
     6c8:	2442ffff 	addiu	$v0,$v0,-1
     6cc:	00021100 	sll	$v0,$v0,0x4
     6d0:	0082102b 	sltu	$v0,$a0,$v0
     6d4:	1440fff4 	bnez	$v0,6a8 <init_module-0x294>
     6d8:	00e50018 	mult	$a3,$a1
*/
					ltmp1 = dsp_clock_mult * dsp_input_clock_speed;
					tmp_v1 = ltmp1 & 0xffffffff;
					ltmp1 = tmp_v1 * 0x431bde83;
					tmp_v0 = (ltmp1 >> 32) & 0xffffffff;
					tmp_a0 += 1;
					tmp_v0 >>= 0x14;
					tmp_v0 -= 1;
					tmp_v0 <<= 0x04;
				} while(tmp_a0 < tmp_v0);
			}
/*
     6dc:	3c02a508 	lui	$v0,0xa508
     6e0:	3442101c 	ori	$v0,$v0,0x101c
     6e4:	8c430000 	lw	$v1,0($v0)
     6e8:	04610004 	bgez	$v1,6fc <init_module-0x240>
     6ec:	2402ffff 	li	$v0,-1
     6f0:	24c6ffff 	addiu	$a2,$a2,-1
     6f4:	54c2ffde 	0x54c2ffde
     6f8:	01203821 	move	$a3,$t1
*/
#warning MUST BE SIGNED
			reg = *(volatile int *)0xa508101c;
			if(reg < 0) {
				tmp_v0 = -1;
				tmp_a2 -= 1;
				
			}
		}
	}
/*
     6fc:	10c00003 	beqz	$a2,70c <init_module-0x230>
     700:	24020001 	li	$v0,1
     704:	03e00008 	jr	$ra
     708:	a1430000 	sb	$v1,0($t2)
     70c:	03e00008 	jr	$ra
     710:	00001021 	move	$v0,$zero
*/
	if(tmp_a2 != 0) {
		*pbuffer = tmp_v1;		
		return 1;
	} 
	return 0;
}

/* probably read function */
/* DONE, TODO: magic numbers, magic functions */
/* FUNCTION: DONE, need review */
// 0x714
int tnetv1050_tid_read(char tid, int ecval, int ptr, int count)
{
	int ret = 1;

	tiuhw_select_tid(tid & 0xffff);

	tnetv1050_tid_writebyte(ecval & 0xff);
	while(count !=0  && ((ret = tnetv1050_tid_readbyte(ptr)) != 0)) { ptr++; count--; }

	tiuhw_deselect_tid(tid & 0xffff);

	return ret;
}

// 0x7c8
/* FUNCTION: DONE, need review */
int tnetv1050_tid_write(char tid, int ecval, char *data, int len)
{
	int tmp1;
	int cnt = len;
	int byte;
	int ret;

	tiuhw_select_tid(tid & 0xffff);

	tnetv1050_tid_writebyte(ecval & 0xff);

	ret = 1;
	while(ret && cnt) {
		byte = *data;
		ret = tnetv1050_tid_writebyte(byte);
		data++;
		cnt--;
     	}

	tiuhw_deselect_tid(tid & 0xffff);

	return ret;
}

// 0x778
/* FUNCTION: DONE, need review */
int func03(void)
{
	return 0;
}


// 0x888
/* FUNCTION: DONE, need review */
int func11(void)
{
	return 0;
}

// 0x888
/* FUNCTION: DONE, need review */
int func10(void)
{
	return 0;
}

// 0x890
/* FUNCTION: DONE, need review */
int func12(void)
{
	return 0;
}

// 0x898
/* FUNCTION: DONE, need review */
int func13(void)
{
	return 0;
}

// 0x8a0
/* FUNCTION: DONE, need review */
int func20(void)
{
	return 0;
}

// 0x8a8
/* FUNCTION: DONE, need review */
int func21(void)
{
	return 0;
}

// 0x8b0
/* TODO: 'magic number sections' -some DSP settings? */
/* FUNCTION: TODO, need review */
int func22(int tid, int if_type)
{
	int reg;

	tid &= 0xffff;
	if(tid < 4) {
		if(if_type == TIHW_INTERNAL) {
			reg = *(volatile int *)0xa5081020;
			reg |= (1 << tid);
			*(volatile int *)0xa5081020 = reg;
		} else {
			reg = *(volatile int *)0xa5081020;
			reg &= (~(1 << if_type));
			*(volatile int *)0xa5081020 = reg;
		}
	}

	return 1;
}

// 0x904
// something like get_type
/* FUNCTION: DONE, need review */
int tnetv1050_get_tid_type(char tid, int if_type)
{
	int ret;

	if(if_type != TIHW_INTERNAL) {
		ret = 0;
	} else {
		ret = tiuhw_get_tnetv1050_tid_type();
	}

	return ret;
}

/* TODO : pointers & internal structures */
/* FUNCTION: DONE, need review */
static int __init tihw_hal_init_module(void) {
	int i;
	int ret;

	printk(KERN_ERR "HAL initializing\n");
 	printk(KERN_ERR "Version: %s\n", SOURCE_DATE);

	hw_apis.init = tnetv1050_api.init;
	hw_apis.read = tnetv1050_api.read;
	hw_apis.write = tnetv1050_api.write;
	hw_apis.field3 = tnetv1050_api.field3;
	hw_apis.field4 = tnetv1050_api.field4;
	hw_apis.field5 = tnetv1050_api.field5;
	hw_apis.field6 = tnetv1050_api.field6;
	hw_apis.field7 = tnetv1050_api.field7;
	hw_apis.field8 = tnetv1050_api.field8;
	hw_apis.field9 = tnetv1050_api.field9;
	hw_apis.field10 = tnetv1050_api.field10;
	hw_apis.field11 = tnetv1050_api.field11;

	other_pointer_dst = other_pointer_src;

	ret = request_irq(TITAN_IRQ_TELEIF, tnetv1050_tid_interrupt_handler, IRQF_SHARED, stub_text, 0);
	ret = request_irq(13, test_irq_handler, IRQF_SHARED, "test", 0);

	printk(KERN_ERR "HAL initializing DONE\n");

	return 0;
}

/* FUNCTION: DONE, need review */
static void __exit tihw_hal_cleanup_module(void)
{
	free_irq(TITAN_IRQ_TELEIF, NULL);

	return;
}

/* TODO : check, and make it more complete */
/* FUNCTION: TODO, need review */
int tiuhw_get_dsp_clk_values(void)
{
	int ret = -1;
	char *dsp_clk_str;
	unsigned int dsp_freq, dsp_clk_input_speed_val=0, dsp_clk_mult_val=0, dsp_clock = 0, clock_div = 0;

	dsp_clk_str = prom_getenv("DSP_CLK");
	if(dsp_clk_str) {
		char tmp_dsp_clk_str[128], *tmp_dsp_clk_str_ptr = tmp_dsp_clk_str;;
		char *dsp_clk_ptr = NULL;

		strncpy(tmp_dsp_clk_str, dsp_clk_str, sizeof(tmp_dsp_clk_str));

		dsp_clk_ptr = strsep(&tmp_dsp_clk_str_ptr, ":");
		dsp_clk_input_speed_val = simple_strtol(dsp_clk_ptr, NULL, 10);

		tiuhw_dsp_input_clock_speed = dsp_clk_input_speed_val; // 12288000

		// mult
		dsp_clk_ptr = strsep(&tmp_dsp_clk_str_ptr, ":");
		dsp_clk_mult_val = simple_strtol(dsp_clk_ptr, NULL, 10);
		tiuhw_dsp_clock_mult = dsp_clk_mult_val; // 10

		dsp_clock = tiuhw_dsp_clock_mult * tiuhw_dsp_input_clock_speed;

		if(dsp_clock > DSP_MAX_CLOCK_SPEED) { // 125MHz
			clock_div = dsp_clock / tiuhw_dsp_input_clock_speed;
			tiuhw_dsp_clock_mult = clock_div;

			if(clock_div == 0) {
				ret = 0;
				goto out;
			}
			clock_div -= 1;
			tiuhw_dsp_clock_mult = clock_div;

			printk(KERN_ERR "%s exceeds %lu, dropping multiplier to %lu\n", "DSP_CLK", DSP_MAX_CLOCK_SPEED, tiuhw_dsp_clock_mult);
		}
	} else {
		tiuhw_dsp_clock_mult = 0x0f;
		tiuhw_dsp_input_clock_speed = 0x7d;
	}
	dsp_freq = tiuhw_dsp_clock_mult * tiuhw_dsp_input_clock_speed;
	printk(KERN_ERR "Internal TNETV1060 DSP settings:\n DSP mult: %lu Input clock: %lu, final DSP speed: %lu\n", tiuhw_dsp_clock_mult, tiuhw_dsp_input_clock_speed, dsp_freq);
	ret = 1;

out:
	return ret;
}

/* ALMOST DONE, fix pointers, change 'magic number sections' to ar7_* functions */
/* FUNCTION: DONE, need review */
int tiuhw_init_hal(tiuhw_device *a, int b)
{
	int if_type = -1;
	int reg;
	int tmp;
	int (*init_function)(int a, int b);
	int ret;
	unsigned long long ltmp1;
	volatile int cnt1;

	if_type = hwu_get_tiuhw_if();
	if(if_type == TIHW_INTERNAL) {
		tiuhw_get_dsp_clk_values();
		printk(KERN_ERR "Internal TNETV1060 Telephony HAL initializing\n");

		if(should_reset_tid) {

			// reset - ok
			reg = *(volatile int *)0xa8611660; // v1
			reg |= 0x01;
			*(volatile int *)0xa8611660 = reg;

			// gpio - set direction 1
			reg = *(volatile int *)0xa8610914; // a0
			reg &= 0xfff7ffff; // 0xfff7ffff is in a3
			*(volatile int *)0xa8610914 = reg;

			// gpio - data out 1
			reg = *(volatile int *)0xa861090c; // a1
			reg &= 0xfff7ffff;
			*(volatile int *)0xa861090c = reg;

			// gpio - enable 1
			reg = *(volatile int *)0xa861091c; // t0
			reg |= 0x00080000;
			*(volatile int *)0xa861091c = reg; // t0

			// reset TELE_RESET (telephony interface) - PIN_SEL_14
			reg = *(volatile int *)0xa8611640; // a2
			reg |= 0x00003000;
			*(volatile int *)0xa8611640 = reg; // a2

			tiuhw_reset_tid(DSP_CORE_0, DSP_RESET_ON);
		}

		tiuhw_api = &hw_apis;

		init_function = hw_apis.init;
		ret = init_function(a,b);

		ret = 0x1ffffc70;
		tmp = loops_per_jiffy;

		ltmp1 = ret * tmp;
		cnt1 = (ltmp1 >> 23) & 0xffffffff;
		while(cnt1--);

     		if(should_reset_tid) {
     				tiuhw_reset_tid(DSP_CORE_0, DSP_RESET_OFF);
		}
	} else if(if_type == TIHW_EXTERNAL) {
		// not necessery - cause we have INTERNAL (no-FPGA)
		printk("UNSUPPORTED if_type - EXTERNAL\n");
	}

	printk(KERN_ERR "DONE w/ HAL init\n");
	should_reset_tid = 0;

	return 1;
}

/* DONE */
/* FUNCTION: DONE, need review */
void tiuhw_select_tid(char tid)
{
	return;
}

/* DONE */
/* FUNCTION: DONE, need review */
void tiuhw_deselect_tid(char tid)
{
	return;
}

/* TODO: implement */
/* FUNCTION: TODO, need review */
/* This functions should be checked. It checks undefined variable. tmp_variable0 */
int tiuhw_get_tid_type(char tid)
{
	int ret = -1;
	char *num_tids_str = NULL;
	int num_tids = 0;
	int tmp0, tmp3;
	char *tele_id = NULL, *tmp_tele_id = NULL, *end_buf = NULL;
	int some_tmp;
	char *str1 = NULL;
	int tmp_s3, tmp_s1, tmp_s2, tmp_v0;
	int index, tele_id_index=0;
	int (*func_ptr)(int a1, int a2, int a3, int a4);
	int reg;
	int tmp_tid_type;

/* prologue
0000000000000e14 <tiuhw_get_tid_type>:
     e14:	27bdff50 	addiu	$sp,$sp,-176
     e18:	afbf00ac 	sw	$ra,172($sp)
     e1c:	afb600a8 	sw	$s6,168($sp)
     e20:	afb500a4 	sw	$s5,164($sp)
     e24:	afb400a0 	sw	$s4,160($sp)
     e28:	afb3009c 	sw	$s3,156($sp)
     e2c:	afb20098 	sw	$s2,152($sp)
     e30:	afb10094 	sw	$s1,148($sp)
     e34:	afb00090 	sw	$s0,144($sp)
*/
/*
     e38:	24130001 	li	$s3,1
     e3c:	309600ff 	andi	$s6,$a0,0xff
*/
	num_tids = 1; // s3

/*
     e40:	3c040000 	lui	$a0,0x0
     e44:	248403a0 	addiu	$a0,$a0,928
     e48:	3c110000 	lui	$s1,0x0
     e4c:	26310000 	addiu	$s1,$s1,0
     e50:	0220f809 	jalr	$s1
     e54:	00009021 	move	$s2,$zero
*/
	num_tids_str = prom_getenv("NUM_TIDS");
	tmp_s2 = 0; // s2

/*
     e58:	1040000a 	beqz	$v0,e84 <tiuhw_get_tid_type+0x70>
     e5c:	00402021 	move	$a0,$v0
*/
	if(num_tids_str) {
/*
     e60:	00002821 	move	$a1,$zero
     e64:	3c020000 	lui	$v0,0x0
     e68:	24420000 	addiu	$v0,$v0,0
     e6c:	0040f809 	jalr	$v0
     e70:	2406000a 	li	$a2,10
*/
		num_tids = simple_strtol(num_tids_str, 0, 10);
/*
     e74:	305300ff 	andi	$s3,$v0,0xff
     e78:	2e620002 	sltiu	$v0,$s3,2
     e7c:	50400001 	0x50400001 // sb 
     e80:	24130001 	li	$s3,1
*/
		num_tids &= 0xff;
		if(num_tids < 2) num_tids = 1;
	}
/*
     e84:	3c030000 	lui	$v1,0x0
     e88:	8c6300ac 	lw	$v1,172($v1)
*/
	tmp0 = tmp_variable0;

/*
     e8c:	24020001 	li	$v0,1
     e90:	146200a4 	bne	$v1,$v0,1124 <tiuhw_get_tid_type+0x310>
     e94:	00000000 	nop
*/
	if(tmp0 == TIHW_INTERNAL) {

/*
     e98:	3c010000 	lui	$at,0x0
     e9c:	ac2000ac 	sw	$zero,172($at)
*/
		tmp_variable0 = 0;

/*
     ea0:	3c100000 	lui	$s0,0x0
     ea4:	261003ac 	addiu	$s0,$s0,940
     ea8:	0220f809 	jalr	$s1
     eac:	02002021 	move	$a0,$s0
*/
		tele_id = prom_getenv("TELE_ID");	

/*
     eb0:	3c040000 	lui	$a0,0x0
     eb4:	248403b4 	addiu	$a0,$a0,948
     eb8:	02002821 	move	$a1,$s0
     ebc:	00408021 	move	$s0,$v0 // pointer to tele_id
     ec0:	3c020000 	lui	$v0,0x0
     ec4:	24420000 	addiu	$v0,$v0,0
     ec8:	0040f809 	jalr	$v0
     ecc:	02003021 	move	$a2,$s0
*/
		printk(KERN_ERR "%s = %s\n", "TELE_ID", tele_id);
/*
     ed0:	1200006e 	beqz	$s0,108c <tiuhw_get_tid_type+0x278>
     ed4:	27a40010 	addiu	$a0,$sp,16
*/
		if(tele_id) {
			char substring[128];


/*
     ed8:	24020080 	li	$v0,128
     edc:	00802821 	move	$a1,$a0

     ee0:	92010000 	lbu	$at,0($s0)
     ee4:	2442ffff 	addiu	$v0,$v0,-1  // counter
     ee8:	a0a10000 	sb	$at,0($a1)

     eec:	10200003 	beqz	$at,efc <tiuhw_get_tid_type+0xe8>
     ef0:	24a50001 	addiu	$a1,$a1,1 //substring

     ef4:	1440fffa 	bnez	$v0,ee0 <tiuhw_get_tid_type+0xcc>
     ef8:	26100001 	addiu	$s0,$s0,1 // tele_id
*/
			strncpy(substring, tele_id, sizeof(substring));

/*

     efc:	3c150000 	lui	$s5,0x0
     f00:	26b50210 	addiu	$s5,$s5,528
     
     f04:	3c140000 	lui	$s4,0x0
     f08:	26940000 	addiu	$s4,$s4,0
     f0c:	0280f809 	jalr	$s4
     f10:	02a02821 	move	$a1,$s5
*/
			tmp_tele_id = substring;
			// f04 - a0 jest tmp_tele_id
			while(((str1 = strsep(&tmp_tele_id, ":")) != NULL) && (tele_id_index < num_tids)) {
		
			// HERE

/*
     f14:	00402821 	move	$a1,$v0
     f18:	10a00082 	beqz	$a1,1124 <tiuhw_get_tid_type+0x310>
     f1c:	00000000 	nop
*/
			// tmp_tele_id now holds reference to 'AUTO' string (WRTP54G)
			if(tmp_tele_id == NULL) goto out;

/*
     f20:	12600080 	beqz	$s3,1124 <tiuhw_get_tid_type+0x310>
     f24:	00008821 	move	$s1,$zero
*/
     		if(num_tids == 0) goto out;

/*
     f28:	080003cd 	j	f34 <tiuhw_get_tid_type+0x120>
     f2c:	00000000 	nop
*/

/* while
     f30:	305100ff 	andi	$s1,$v0,0xff
-----
     f34:	001110c0 	sll	$v0,$s1,0x3
     f38:	3c100000 	lui	$s0,0x0
     f3c:	26100074 	addiu	$s0,$s0,116
     f40:	02028021 	addu	$s0,$s0,$v0 // first entry in tid2name table
     f44:	92060004 	lbu	$a2,4($s0) /// get pointer to tidname
     f48:	10c0000e 	beqz	$a2,f84 <tiuhw_get_tid_type+0x170>
     f4c:	00a02021 	move	$a0,$a1
*/

			index = 0;
			while(tidname2type[index].tidtype && strcmp(tidname2type[index].tidname, str1)) {
/*
     f50:	8e020000 	lw	$v0,0($s0)
     f54:	00401821 	move	$v1,$v0 // pointer to 
     f58:	90820000 	lbu	$v0,0($a0)
     f5c:	90610000 	lbu	$at,0($v1)
     f60:	24840001 	addiu	$a0,$a0,1
     f64:	14220004 	bne	$at,$v0,f78 <tiuhw_get_tid_type+0x164>
     f68:	24630001 	addiu	$v1,$v1,1
     f6c:	1440fffb 	bnez	$v0,f5c <tiuhw_get_tid_type+0x148>
     f70:	90820000 	lbu	$v0,0($a0)  // strcmp
*/
/*
     f74:	00201021 	move	$v0,$at
     f78:	00411023 	subu	$v0,$v0,$at
     f7c:	1440ffec 	bnez	$v0,f30 <tiuhw_get_tid_type+0x11c>
     f80:	26220001 	addiu	$v0,$s1,1 // index++
while */
				index++;
			}
/*
     f84:	241100ff 	li	$s1,255
     f88:	14d1001d 	bne	$a2,$s1,1000 <tiuhw_get_tid_type+0x1ec>
     f8c:	02402021 	move	$a0,$s2
*/
			if(tidname2type[index].tidtype == 255) {
/*
     f90:	24050001 	li	$a1,1
     f94:	3c020000 	lui	$v0,0x0 // tiuhw_api
     f98:	8c4200b8 	lw	$v0,184($v0)
     f9c:	00003021 	move	$a2,$zero
     fa0:	8c42002c 	lw	$v0,44($v0)
     fa4:	0040f809 	jalr	$v0
     fa8:	00003821 	move	$a3,$zero
*/
//				func_ptr = tiuhw_api[2]->field3; /// check this out
//				ret = func_ptr(index, 1, 0, 0);
				ret = 4;
/*
     fac:	304300ff 	andi	$v1,$v0,0xff
     fb0:	24020003 	li	$v0,3
     fb4:	1062000c 	beq	$v1,$v0,fe8 <tiuhw_get_tid_type+0x1d4>
     fb8:	28620004 	slti	$v0,$v1,4
     fbc:	10400005 	beqz	$v0,fd4 <tiuhw_get_tid_type+0x1c0>
     fc0:	24020001 	li	$v0,1
     fc4:	1062000c 	beq	$v1,$v0,ff8 <tiuhw_get_tid_type+0x1e4>
     fc8:	240200ff 	li	$v0,255
     fcc:	08000414 	j	1050 <tiuhw_get_tid_type+0x23c>
     fd0:	00000000 	nop
*/
/*
     fd4:	24020004 	li	$v0,4
     fd8:	10620005 	beq	$v1,$v0,ff0 <tiuhw_get_tid_type+0x1dc>
     fdc:	240200ff 	li	$v0,255
     fe0:	08000414 	j	1050 <tiuhw_get_tid_type+0x23c>
     fe4:	00000000 	nop
     fe8:	08000414 	j	1050 <tiuhw_get_tid_type+0x23c>
     fec:	24020005 	li	$v0,5
     ff0:	08000414 	j	1050 <tiuhw_get_tid_type+0x23c>
     ff4:	2402000c 	li	$v0,12
     ff8:	08000414 	j	1050 <tiuhw_get_tid_type+0x23c>
     ffc:	24020002 	li	$v0,2
*/
				reg &= 0xffff;
				if(reg == 3) { // fe8
					ret = 5;
				} else if(reg >= 4) { // fd4
					if(reg == 4) {
						ret = 12;
					} else ret = 255;
				} else if(reg == 1) { // ff8
					ret = 2;
				} else {
					ret = 255;
				}

			} else { //tidtype == 255
/*
    1000:	54c0000c 	0x54c0000c
    1004:	02402821 	move	$a1,$s2
    1008:	3c040000 	lui	$a0,0x0
    100c:	248403c0 	addiu	$a0,$a0,960
    1010:	3c020000 	lui	$v0,0x0
    1014:	24420000 	addiu	$v0,$v0,0
    1018:	0040f809 	jalr	$v0
    101c:	00000000 	nop
*/
				if(tmp_tid_type == 0) {
					printk(KERN_ERR "Unrecognized TID ID string detected: %s\n", tmp_s2);

/*
    1020:	3c010000 	lui	$at,0x0
    1024:	00320821 	addu	$at,$at,$s2

    1028:	a0310010 	sb	$s1,16($at)
    102c:	08000418 	j	1060 <tiuhw_get_tid_type+0x24c>
    1030:	26420001 	addiu	$v0,$s2,1
    

    1034:	3c040000 	lui	$a0,0x0
    1038:	248403ec 	addiu	$a0,$a0,1004
    103c:	3c020000 	lui	$v0,0x0
    1040:	24420000 	addiu	$v0,$v0,0
    1044:	0040f809 	jalr	$v0
    1048:	8e060000 	lw	$a2,0($s0)
*/
#warning ALA
		// printk(KERN_ERR "TID(M) %u is a %s\n", 0, str);
/*
    104c:	92020004 	lbu	$v0,4($s0)
*/
			}
/*
    1050:	3c010000 	lui	$at,0x0
    1054:	00320821 	addu	$at,$at,$s2
    1058:	a0220010 	sb	$v0,16($at)
    105c:	26420001 	addiu	$v0,$s2,1
*/
    	}
/*
    1060:	305200ff 	andi	$s2,$v0,0xff
    1064:	00002021 	move	$a0,$zero
    1068:	0280f809 	jalr	$s4			// strtok (which is in "while"
    106c:	02a02821 	move	$a1,$s5
*/
/*
    1070:	00402821 	move	$a1,$v0
    1074:	10a0002b 	beqz	$a1,1124 <tiuhw_get_tid_type+0x310>
    1078:	0253102b 	sltu	$v0,$s2,$s3
    107c:	1440ffad 	bnez	$v0,f34 <tiuhw_get_tid_type+0x120>
    1080:	00008821 	move	$s1,$zero
*/
		}
/*
    1084:	08000449 	j	1124 <tiuhw_get_tid_type+0x310>
    1088:	00000000 	nop
*/
		}
/*
    108c:	12600025 	beqz	$s3,1124 <tiuhw_get_tid_type+0x310>
    1090:	00008821 	move	$s1,$zero
    1094:	02202021 	move	$a0,$s1
    1098:	24050001 	li	$a1,1
    109c:	3c020000 	lui	$v0,0x0
    10a0:	8c4200b8 	lw	$v0,184($v0)
    10a4:	00003021 	move	$a2,$zero
    10a8:	8c42002c 	lw	$v0,44($v0)
    10ac:	0040f809 	jalr	$v0
    10b0:	00003821 	move	$a3,$zero
    10b4:	304300ff 	andi	$v1,$v0,0xff
    10b8:	24020003 	li	$v0,3
    10bc:	1062000c 	beq	$v1,$v0,10f0 <tiuhw_get_tid_type+0x2dc>
    10c0:	28620004 	slti	$v0,$v1,4
    10c4:	10400005 	beqz	$v0,10dc <tiuhw_get_tid_type+0x2c8>
    10c8:	24020001 	li	$v0,1
    10cc:	1062000c 	beq	$v1,$v0,1100 <tiuhw_get_tid_type+0x2ec>
    10d0:	240200ff 	li	$v0,255
    10d4:	08000441 	j	1104 <tiuhw_get_tid_type+0x2f0>
    10d8:	00000000 	nop
    10dc:	24020004 	li	$v0,4
    10e0:	10620005 	beq	$v1,$v0,10f8 <tiuhw_get_tid_type+0x2e4>
    10e4:	240200ff 	li	$v0,255
    10e8:	08000441 	j	1104 <tiuhw_get_tid_type+0x2f0>
    10ec:	00000000 	nop
    10f0:	08000441 	j	1104 <tiuhw_get_tid_type+0x2f0>
    10f4:	24020005 	li	$v0,5
    10f8:	08000441 	j	1104 <tiuhw_get_tid_type+0x2f0>
    10fc:	2402000c 	li	$v0,12
    1100:	24020002 	li	$v0,2
    1104:	3c010000 	lui	$at,0x0
    1108:	00310821 	addu	$at,$at,$s1
    110c:	a0220010 	sb	$v0,16($at)
    1110:	26220001 	addiu	$v0,$s1,1
    1114:	305100ff 	andi	$s1,$v0,0xff
    1118:	0233102b 	sltu	$v0,$s1,$s3
    111c:	1440ffde 	bnez	$v0,1098 <tiuhw_get_tid_type+0x284>
    1120:	02202021 	move	$a0,$s1
*/
    }
out:
/*
    1124:	3c020000 	lui	$v0,0x0
    1128:	00561021 	addu	$v0,$v0,$s6
    112c:	90420010 	lbu	$v0,16($v0)
    1130:	8fbf00ac 	lw	$ra,172($sp)
    1134:	8fb600a8 	lw	$s6,168($sp)
    1138:	8fb500a4 	lw	$s5,164($sp)
    113c:	8fb400a0 	lw	$s4,160($sp)
    1140:	8fb3009c 	lw	$s3,156($sp)
    1144:	8fb20098 	lw	$s2,152($sp)
    1148:	8fb10094 	lw	$s1,148($sp)
    114c:	8fb00090 	lw	$s0,144($sp)
    1150:	03e00008 	jr	$ra
    1154:	27bd00b0 	addiu	$sp,$sp,176
*/
	ret += tid;

	return ret; // something from structure
}

/* DONE TILL HERE : FROM THE BOTTOM */

/* DONE */
/* FUNCTION: DONE, need review */
int tiuhw_map_tcid_to_tid(int tcid)
{
	int ret = -1;
	int tmp;
	int if_type;

	if(tcid & 0xffff < 4) {
    		if_type = hwu_get_tiuhw_if();

		if(if_type == TIHW_INTERNAL) {
			ret = 0;
		} else if(if_type == TIHW_EXTERNAL) {
			ret = (tcid << 1) & 0xff;	
		} else ret = -1;
	}

	return ret;
}

/* DONE */
/* FUNCTION: DONE, need review */
int tiuhw_get_tnetv1050_tid_type(void)
{
#warning MAGIC_NUMBER!!!
	return 4;
}

/* DONE */
/* FUNCTION: DONE, need review */
int tiuhw_get_dsp_mult(int interface)
{
	int if_type;

	if_type = hwu_get_tiuhw_if();

	if(if_type != TIHW_INTERNAL) return 5;
	return if_type;
}

/* DONE */
/* FUNCTION: DONE, need review */
int tiuhw_reset_tid(char tid, int cmd)
{
	int reg;

	printk(KERN_ERR "tiuhw_reset_tid: %u %u\n", tid & 0xff, cmd);

	if(cmd) {
		gpio_set_value(19, 0);
	} else {
		gpio_set_value(19, 1);
	}

	return 0;
}

/* DONE */
/* FUNCTION: DONE, need review */
int hwu_get_tiuhw_if(void) {
	char *teleif=NULL;
	int ret=-1;

	if(not_initialized) {
		not_initialized = 0;
		teleif = prom_getenv("TELE_IF");
		if(teleif) {
			if(!strcmp(teleif, "EXTERNAL")) {
				interface_type = TIHW_EXTERNAL;
			} else if(!strcmp(teleif, "INTERNAL")) {
				interface_type = TIHW_INTERNAL;
			} else {
				printk(KERN_ERR "Invalid Telephony interface specfied, defaulting to %s\n", "EXTERNAL");
			}
		} else {
			printk(KERN_ERR "Environment variable for telephony interface not set, assuming FPGA\n");
		}
    	}
	ret = interface_type;

	return ret;
}

/* DONE */
/* FUNCTION: DONE, need review */
void tiuhw_led(void) {
	return;
}

module_init(tihw_hal_init_module);
module_exit(tihw_hal_cleanup_module);


MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Wlodzimierz Kalawski <wlk at poczta.fm>");
MODULE_LICENSE("GPL v2");

EXPORT_SYMBOL_GPL(hwu_get_tiuhw_if);
EXPORT_SYMBOL_GPL(tiuhw_get_dsp_mult);
EXPORT_SYMBOL_GPL(hwu_settings_mod);
EXPORT_SYMBOL_GPL(tiuhw_get_tid_type);
EXPORT_SYMBOL_GPL(tiuhw_map_tcid_to_tid);
EXPORT_SYMBOL_GPL(tiuhw_init_hal);
EXPORT_SYMBOL_GPL(tiuhw_reset_tid);
EXPORT_SYMBOL_GPL(tiuhw_led);
EXPORT_SYMBOL_GPL(tiuhw_api);
