
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/interrupt.h>
#include <asm/ar7/ar7.h>
#include <asm/ar7/gpio.h>

typedef struct {
	char field0;
	char field1;
	char notused0;
	char notused1;
	int  field2;
	char  field3;
	char notused2;
	char notused3;
	char notused4;
	int  field4;
} struct_s1;

typedef struct {
	int (*field0)(void);
	int (*field1)(void);
	int (*field2)(void);
	int (*field3)(void);
} ptrs;

int tnetv1050_tid_init(struct_s1 *a);
int tnetv1050_tid_read(int tid, int b, int cnt, int a4);
int tnetv1050_tid_write(int a1, int a2, int a3);
int func03(void);
int func10(void);
int func11(void);
int func12(void);
int func13(void);
int func20(void);
int func21(void);
int func22(int a, int b);
int func23(int a, int b);

char *prom_getenv(char *);
irqreturn_t null_interrupt_handler1(int i, void *data);
int hwu_get_tiuhw_if(void);
int tiuhw_reset_tid(int a, int cmd);
void tiuhw_select_tid(int tid);
void tiuhw_deselect_tid(int tid);
int tiuhw_get_tnetv1050_tid_type(void);

#define TIHW_INTERNAL 1
#define TIHW_EXTERNAL 2

#define DSP_CORE_0 0
#define DSP_RESET_ON 1
#define DSP_RESET_OFF 0

/* pointers from data + 0x0000 */
ptrs tnetv1050_api[3] = {
	{
		tnetv1050_tid_init, // 0x000
		tnetv1050_tid_read, // 0x714
		tnetv1050_tid_write, // 0x7c8
		func03  // 0x878
	},
	{
		func10, // 0x888
		func11, // 0x880
		func12, // 0x890
		func13  // 0x898
	},
	{
		func20, // 0x8a0
		func21, // 0x8a8
		func22, // 0x8b0
		func23  // 0x904
	}
};

int unknown_global1 = 1; // data + 0x0034
ptrs hw_apis[3];	// data + 0x0038
int should_reset_tid = 1; // data + 0x0070
int tmp_variable0 = 0; // data + 0x00ac
int not_initialized = 1; // data + 0x00b0
int interface_type = TIHW_EXTERNAL; // data + 0x00b4
void *tiuhw_api = NULL;		    // data + 0x00b8
int tiuhw_dsp_clock_mult = 0; // data + 0x00bc
int tiuhw_dsp_input_clock_speed = 0; // data + 0x00c0
int hwu_settings_mod[4]; // data + 0x00c4
char *stub_text = "tele_if_stub";
int tiuhw_dsp_clock_mult;
int tiuhw_dsp_input_clock_speed;




void *other_pointer_src = 0;
void *other_pointer_dst = 0;

ptrs dst[3];
ptrs some_table_of_structures[3];


/*
tiuhal.o:     file format elf32-tradlittlemips

Disassembly of section .text:
*/

/* JUST REVIEWED */
/* probably init function */
int tnetv1050_tid_init(struct_s1 *a)
{
	int reg;
	int tmp0, tmp1, tmp2, tmp3, tmp4;
	int tmp;
	int tmp_v0, tmp_v1;
	int tmp_a1;

/* prologue
0000000000000000 <init_module-0x93c>:
       0:	27bdffd8 	addiu	$sp,$sp,-40
       4:	afbf0020 	sw	$ra,32($sp)
       8:	afb3001c 	sw	$s3,28($sp)
       c:	afb20018 	sw	$s2,24($sp)
      10:	afb10014 	sw	$s1,20($sp)
      14:	afb00010 	sw	$s0,16($sp)
*/
/*
      18:	24130001 	li	$s3,1
      1c:	2402000b 	li	$v0,11
      20:	ac820004 	sw	$v0,4($a0)
      24:	a0930000 	sb	$s3,0($a0)
      28:	a0800001 	sb	$zero,1($a0)
      2c:	24020002 	li	$v0,2
      30:	a0820008 	sb	$v0,8($a0)
*/
	a->field2 = 11;
	a->field0 = 1;
	a->field1 = 0;
	a->field3 = 2;

/*
      34:	3c020000 	lui	$v0,0x0
      38:	8c420034 	lw	$v0,52($v0)
      3c:	10400003 	beqz	$v0,4c <init_module-0x8f0>
      40:	ac80000c 	sw	$zero,12($a0)
      44:	3c010000 	lui	$at,0x0
      48:	ac200034 	sw	$zero,52($at)
*/
	a->field4 = 0;
	if(unknown_global1 != 0) unknown_global1 = 0;

/*
      4c:	3c06a861 	lui	$a2,0xa861
      50:	34c61a00 	ori	$a2,$a2,0x1a00
      54:	3c03ffcf 	lui	$v1,0xffcf
      58:	3463ffff 	ori	$v1,$v1,0xffff
      60:	8cc20000 	lw	$v0,0($a2)
      68:	00431024 	and	$v0,$v0,$v1
      6c:	acc20000 	sw	$v0,0($a2)
*/
	/* config latch */
	reg = *(volatile int *)0xa8611a00;
	reg &= 0xffcfffff;
	*(volatile int *)0xa8611a00;

/*
      5c:	24040014 	li	$a0,20
      64:	24050001 	li	$a1,1
      70:	3c020000 	lui	$v0,0x0
      74:	24420000 	addiu	$v0,$v0,0
      78:	0040f809 	jalr	$v0
      7c:	00000000 	nop
*/
	ar7_device_enable(AR7_RESET_BIT_DSP);

/*
      9c:	3c08a508 	lui	$t0,0xa508
      a0:	8d020000 	lw	$v0,0($t0)
      a4:	2403fffb 	li	$v1,-5
      a8:	00431024 	and	$v0,$v0,$v1
      ac:	ad020000 	sw	$v0,0($t0)
*/
	reg = *(volatile int *)0xa5080000;
	reg &= 0xfffffffa;
	*(volatile int *)0xa5080000 = reg;

/*
      80:	3c09a508 	lui	$t1,0xa508
      84:	35290104 	ori	$t1,$t1,0x104
      b0:	24020004 	li	$v0,4
      b4:	ad220000 	sw	$v0,0($t1)
*/
	*(volatile int *)0xa5080104 = 4;

/*

      88:	3c04a861 	lui	$a0,0xa861
      8c:	3484163c 	ori	$a0,$a0,0x163c
      b8:	8c820000 	lw	$v0,0($a0)
      90:	3c0500f0 	lui	$a1,0xf0
      94:	34a50fff 	ori	$a1,$a1,0xfff
      c0:	00451024 	and	$v0,$v0,$a1
      c4:	ac820000 	sw	$v0,0($a0)
*/
	/* some GPIO/PIN settings */
	reg = *(volatile int *)0xa861163c;
	reg &= 0x00f00fff;
	*(volatile int *)0xa861163c = reg;

/*



      98:	3c075505 	lui	$a3,0x5505
      bc:	34e75000 	ori	$a3,$a3,0x5000
      e4:	8c820000 	lw	$v0,0($a0)
      ec:	00471025 	or	$v0,$v0,$a3
      f0:	ac820000 	sw	$v0,0($a0)
*/
	// another GPIO/PIN?
	reg = *(volatile int *)0xa861163c;
	reg |= 0x00005000;
	*(volatile int *)0xa861163c = reg;

/*

      d8:	3c06a861 	lui	$a2,0xa861
      e0:	34c61640 	ori	$a2,$a2,0x1640
      f4:	8cc20000 	lw	$v0,0($a2)
      fc:	30423000 	andi	$v0,$v0,0x3000
     100:	acc20000 	sw	$v0,0($a2)
*/
	reg = *(volatile int *)0xa8611640;
	reg &= 0x00003000;
	*(volatile int *)0xa8611640 = reg;

/*


     108:	8cc20000 	lw	$v0,0($a2)
     10c:	00000000 	nop
     114:	34424144 	ori	$v0,$v0,0x4144
     118:	acc20000 	sw	$v0,0($a2)
*/
	reg = *(volatile int *)0xa8611640;
	reg |= 0x00004144;
	*(volatile int *)0xa8611640 = reg;


/*
*/

/*
      e8:	3c03431b 	lui	$v1,0x431b
      f8:	3463de83 	ori	$v1,$v1,0xde83

      c8:	3c0a0000 	lui	$t2,0x0
      cc:	8d4a0000 	lw	$t2,0($t2)
      d0:	3c0b0000 	lui	$t3,0x0
      d4:	8d6b0000 	lw	$t3,0($t3)
      dc:	014b0018 	mult	$t2,$t3
     104:	00002812 	mflo	$a1
     110:	00a30019 	multu	$a1,$v1
*/
	tmp0 = 0x431bde83;
	tmp1 = tiuhw_dsp_clock_mult; // t2
	tmp2 = tiuhw_dsp_input_clock_speed; // t3
	tmp3 = tmp1 * tmp2; // a1
	tmp4 = tmp3 * tmp0; // a1 * v1

/*
     11c:	2542ffff 	addiu	$v0,$t2,-1
     120:	00021300 	sll	$v0,$v0,0xc
     124:	3442027e 	ori	$v0,$v0,0x27e
     128:	ad220000 	sw	$v0,0($t1)
*/
	*(volatile int *)0xa5080104 = (((tmp1-1) << 12)|0x0000027e);

/*
     12c:	8d020000 	lw	$v0,0($t0)
     130:	00002021 	move	$a0,$zero
     134:	34420004 	ori	$v0,$v0,0x4
     13c:	ad020000 	sw	$v0,0($t0)
*/
	reg = *(volatile int *)0xa5080000;
	reg |= 0x00000004;
	*(volatile int *)0xa5080000 = reg;
/*
     138:	00001810 	mfhi	$v1
     140:	00031d02 	srl	$v1,$v1,0x14
     144:	2463ffff 	addiu	$v1,$v1,-1
     148:	00031880 	sll	$v1,$v1,0x2
     14c:	1060000f 	beqz	$v1,18c <init_module-0x7b0>
     150:	01402821 	move	$a1,$t2
     154:	00ab0018 	mult	$a1,$t3
     158:	3c02431b 	lui	$v0,0x431b
     15c:	00001812 	mflo	$v1
     160:	3442de83 	ori	$v0,$v0,0xde83
     164:	00000000 	nop
     168:	00620019 	multu	$v1,$v0
     16c:	00001010 	mfhi	$v0
     170:	24840001 	addiu	$a0,$a0,1
     174:	00021502 	srl	$v0,$v0,0x14
     178:	2442ffff 	addiu	$v0,$v0,-1
     17c:	00021080 	sll	$v0,$v0,0x2
     180:	0082102b 	sltu	$v0,$a0,$v0
     184:	1440fff4 	bnez	$v0,158 <init_module-0x7e4>
     188:	00ab0018 	mult	$a1,$t3
*/
/*
     18c:	3c020000 	lui	$v0,0x0
     190:	8c420000 	lw	$v0,0($v0)
*/
		tmp_v0 = tiuhw_dsp_clock_mult;
/*
     194:	3c030000 	lui	$v1,0x0
     198:	8c630000 	lw	$v1,0($v1)
*/
		tmp_v1 = tiuhw_dsp_input_clock_speed;
/*
     19c:	00430018 	mult	$v0,$v1
     1a0:	3c10431b 	lui	$s0,0x431b
     1a4:	00001012 	mflo	$v0
     1a8:	3610de83 	ori	$s0,$s0,0xde83
     1ac:	00000000 	nop
*/
/*
     1b0:	00500019 	multu	$v0,$s0
     1b4:	3c05a508 	lui	$a1,0xa508
     1b8:	34a51010 	ori	$a1,$a1,0x1010

/*
     1bc:	3c040000 	lui	$a0,0x0
     1c0:	248400c8 	addiu	$a0,$a0,200
*/
/*
     1c4:	00001010 	mfhi	$v0
     1c8:	3c038108 	lui	$v1,0x8108
     1cc:	00021502 	srl	$v0,$v0,0x14
     1d0:	2442ffff 	addiu	$v0,$v0,-1
     1d4:	00431025 	or	$v0,$v0,$v1
     1d8:	aca20000 	sw	$v0,0($a1)
*/
/*
     1dc:	8ca50000 	lw	$a1,0($a1)
*/
/*
     1e0:	3c120000 	lui	$s2,0x0
     1e4:	26520000 	addiu	$s2,$s2,0
     1e8:	0240f809 	jalr	$s2
     1ec:	00000000 	nop
*/
	reg = *(volatile int *)0xa5081010; // SPCR1
	printk(KERN_ERR "SPCR1 = %08lx\n", reg);
/*
     1f0:	3c060000 	lui	$a2,0x0
     1f4:	8cc60000 	lw	$a2,0($a2)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     1f8:	3c050000 	lui	$a1,0x0
     1fc:	8ca50000 	lw	$a1,0($a1)
*/
	tmp_a1 = tiuhw_dsp_input_clock_speed;
/*
     200:	00c50018 	mult	$a2,$a1
     204:	00001012 	mflo	$v0
	...
     210:	00500019 	multu	$v0,$s0
     214:	00008010 	mfhi	$s0
     218:	00108502 	srl	$s0,$s0,0x14
     21c:	2610ffff 	addiu	$s0,$s0,-1
     220:	00108080 	sll	$s0,$s0,0x2
     224:	12000013 	beqz	$s0,274 <init_module-0x6c8>
     228:	00002021 	move	$a0,$zero
     22c:	00c50018 	mult	$a2,$a1
     230:	3c02431b 	lui	$v0,0x431b
     234:	00001812 	mflo	$v1
     238:	3442de83 	ori	$v0,$v0,0xde83
     23c:	00000000 	nop
     240:	00620019 	multu	$v1,$v0
     244:	00001010 	mfhi	$v0
     248:	24840001 	addiu	$a0,$a0,1
     24c:	00021502 	srl	$v0,$v0,0x14
     250:	2442ffff 	addiu	$v0,$v0,-1
     254:	00021080 	sll	$v0,$v0,0x2
     258:	0082102b 	sltu	$v0,$a0,$v0
     25c:	1440fff4 	bnez	$v0,230 <init_module-0x70c>
     260:	00c50018 	mult	$a2,$a1
*/
/*
     264:	3c060000 	lui	$a2,0x0
     268:	8cc60000 	lw	$a2,0($a2)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     26c:	3c050000 	lui	$a1,0x0
     270:	8ca50000 	lw	$a1,0($a1)
*/
	tmp_a1 = tiuhw_dsp_input_clock_speed;
/*
     274:	00c50018 	mult	$a2,$a1
     278:	3c02431b 	lui	$v0,0x431b
     27c:	00001812 	mflo	$v1
     280:	3442de83 	ori	$v0,$v0,0xde83
     284:	00000000 	nop
     288:	00620019 	multu	$v1,$v0
     28c:	24040001 	li	$a0,1
     290:	3c03a508 	lui	$v1,0xa508
     294:	34631004 	ori	$v1,$v1,0x1004
     298:	ac640000 	sw	$a0,0($v1)
     29c:	00001010 	mfhi	$v0
     2a0:	00021502 	srl	$v0,$v0,0x14
     2a4:	2442ffff 	addiu	$v0,$v0,-1
     2a8:	00021080 	sll	$v0,$v0,0x2
     2ac:	1040000f 	beqz	$v0,2ec <init_module-0x650>
     2b0:	00002021 	move	$a0,$zero
     2b4:	00c50018 	mult	$a2,$a1
     2b8:	3c02431b 	lui	$v0,0x431b
     2bc:	00001812 	mflo	$v1
     2c0:	3442de83 	ori	$v0,$v0,0xde83
     2c4:	00000000 	nop
     2c8:	00620019 	multu	$v1,$v0
     2cc:	00001010 	mfhi	$v0
     2d0:	24840001 	addiu	$a0,$a0,1
     2d4:	00021502 	srl	$v0,$v0,0x14
     2d8:	2442ffff 	addiu	$v0,$v0,-1
     2dc:	00021080 	sll	$v0,$v0,0x2
     2e0:	0082102b 	sltu	$v0,$a0,$v0
     2e4:	1440fff4 	bnez	$v0,2b8 <init_module-0x684>
     2e8:	00c50018 	mult	$a2,$a1
*/
/*
     2ec:	3c030000 	lui	$v1,0x0
     2f0:	8c630000 	lw	$v1,0($v1)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     2f4:	3c020000 	lui	$v0,0x0
     2f8:	8c420000 	lw	$v0,0($v0)
*/
	tmp_v0 = tiuhw_dsp_input_clock_speed;
/*
     2fc:	00620018 	mult	$v1,$v0
     300:	3c021062 	lui	$v0,0x1062
     304:	00001812 	mflo	$v1
     308:	34424dd3 	ori	$v0,$v0,0x4dd3
     30c:	00000000 	nop
     310:	00620019 	multu	$v1,$v0
     314:	3c04a508 	lui	$a0,0xa508
     318:	34841000 	ori	$a0,$a0,0x1000
     31c:	3c0200ff 	lui	$v0,0xff
     320:	00001810 	mfhi	$v1
     324:	34428000 	ori	$v0,$v0,0x8000
     328:	00031c82 	srl	$v1,$v1,0x12
     32c:	2463ffff 	addiu	$v1,$v1,-1
     330:	00621825 	or	$v1,$v1,$v0
     334:	ac830000 	sw	$v1,0($a0)
     338:	8c850000 	lw	$a1,0($a0)
     33c:	3c040000 	lui	$a0,0x0
     340:	0240f809 	jalr	$s2
     344:	248400dc 	addiu	$a0,$a0,220
*/
/*
     348:	3c060000 	lui	$a2,0x0
     34c:	8cc60000 	lw	$a2,0($a2)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     350:	3c050000 	lui	$a1,0x0
     354:	8ca50000 	lw	$a1,0($a1)
*/
	tmp_a1 = tiuhw_dsp_input_clock_speed;
/*
     358:	00c50018 	mult	$a2,$a1
     35c:	3c02431b 	lui	$v0,0x431b
     360:	00001812 	mflo	$v1
     364:	3442de83 	ori	$v0,$v0,0xde83
     368:	00000000 	nop
     36c:	00620019 	multu	$v1,$v0
     370:	00001010 	mfhi	$v0
     374:	00021502 	srl	$v0,$v0,0x14
     378:	2442ffff 	addiu	$v0,$v0,-1
     37c:	00021080 	sll	$v0,$v0,0x2
     380:	1040000f 	beqz	$v0,3c0 <init_module-0x57c>
     384:	00002021 	move	$a0,$zero
     388:	00c50018 	mult	$a2,$a1
     38c:	3c02431b 	lui	$v0,0x431b
     390:	00001812 	mflo	$v1
     394:	3442de83 	ori	$v0,$v0,0xde83
     398:	00000000 	nop
     39c:	00620019 	multu	$v1,$v0
     3a0:	00001010 	mfhi	$v0
     3a4:	24840001 	addiu	$a0,$a0,1
     3a8:	00021502 	srl	$v0,$v0,0x14
     3ac:	2442ffff 	addiu	$v0,$v0,-1
     3b0:	00021080 	sll	$v0,$v0,0x2
     3b4:	0082102b 	sltu	$v0,$a0,$v0
     3b8:	1440fff4 	bnez	$v0,38c <init_module-0x5b0>
     3bc:	00c50018 	mult	$a2,$a1
     3c0:	3c02a508 	lui	$v0,0xa508
     3c4:	34421020 	ori	$v0,$v0,0x1020
     3c8:	ac400000 	sw	$zero,0($v0)
     3cc:	00008021 	move	$s0,$zero
     3d0:	3c110000 	lui	$s1,0x0
     3d4:	26310000 	addiu	$s1,$s1,0
     3d8:	0220f809 	jalr	$s1
     3dc:	02002021 	move	$a0,$s0
     3e0:	00401821 	move	$v1,$v0
     3e4:	18600006 	blez	$v1,400 <init_module-0x53c>
     3e8:	28620004 	slti	$v0,$v1,4
     3ec:	1440000c 	bnez	$v0,420 <init_module-0x51c>
     3f0:	26020001 	addiu	$v0,$s0,1
     3f4:	2402000c 	li	$v0,12
     3f8:	10620009 	beq	$v1,$v0,420 <init_module-0x51c>
     3fc:	26020001 	addiu	$v0,$s0,1
     400:	0220f809 	jalr	$s1
     404:	02002021 	move	$a0,$s0
     408:	3c040000 	lui	$a0,0x0
     40c:	248400f0 	addiu	$a0,$a0,240
     410:	0240f809 	jalr	$s2
     414:	00402821 	move	$a1,$v0
     418:	00009821 	move	$s3,$zero
     41c:	26020001 	addiu	$v0,$s0,1
     420:	305000ff 	andi	$s0,$v0,0xff
     424:	1200ffea 	beqz	$s0,3d0 <init_module-0x56c>
     428:	02601021 	move	$v0,$s3
     42c:	8fbf0020 	lw	$ra,32($sp)
     430:	8fb3001c 	lw	$s3,28($sp)
     434:	8fb20018 	lw	$s2,24($sp)
     438:	8fb10014 	lw	$s1,20($sp)
     43c:	8fb00010 	lw	$s0,16($sp)
     440:	03e00008 	jr	$ra
     444:	27bd0028 	addiu	$sp,$sp,40
*/
	return 0;
}

/* DONE */
irqreturn_t null_interrupt_handler1(int i, void *data)
{
/*
     448:	03e00008 	jr	$ra
     44c:	00000000 	nop
*/
	return IRQ_HANDLED;
}

/* TODO: implement */
int strange_function1(int a) {
	int dsp_mult;
	int dsp_speed;
	int a0, a1;
	long long tmp0, tmp1;
	int tmp2, tmp3, tmp4;
	int reg;
/*
     450:	308400ff 	andi	$a0,$a0,0xff
*/
	a1 = a&0xff;
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
	tmp0 = dsp_mult * dsp_speed;

/*
     468:	3c02431b 	lui	$v0,0x431b
     46c:	00002812 	mflo	$a1
     470:	3442de83 	ori	$v0,$v0,0xde83
     474:	00000000 	nop
     478:	00a20019 	multu	$a1,$v0
*/
	tmp1 = (tmp0 & 0xffffffff) * 0x431bde83;

/*
     47c:	3c07a508 	lui	$a3,0xa508
     480:	34e71018 	ori	$a3,$a3,0x1018  // a3 = 0xa5081018
     484:	3c06a508 	lui	$a2,0xa508
     488:	34c61010 	ori	$a2,$a2,0x1010  // a2 = 0xa5081010
     48c:	3c038148 	lui	$v1,0x8148	// v1 = 0x8148
     490:	00002810 	mfhi	$a1 		// a1 - tmp2
     494:	ace40000 	sw	$a0,0($a3)
*/
	tmp2 = (tmp1 >> 32) & 0xffffffff;
	*(volatile int *)0xa5081018 = a1;
//	0xa5081010;					// a2
/*
     498:	00051502 	srl	$v0,$a1,0x14
     49c:	2442ffff 	addiu	$v0,$v0,-1
     4a0:	00431025 	or	$v0,$v0,$v1
     4a4:	acc20000 	sw	$v0,0($a2)
*/
	tmp3 = (tmp2 >> 0x14);
	tmp3 -= 1;
	tmp3 |= 0x8148;
	*(volatile int *)0xa5081010 = tmp3;

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
	tmp3 >>= 0x03;
	tmp3 -= tmp2;

/*
     4c0:	00021100 	sll	$v0,$v0,0x4
     4c4:	00451021 	addu	$v0,$v0,$a1
*/
	tmp3 >>= 0x04;
	tmp3 += tmp2;

/*
     4c8:	8ce30000 	lw	$v1,0($a3)
     4cc:	04610029 	bgez	$v1,574 <init_module-0x3c8>
     4d0:	00022100 	sll	$a0,$v0,0x4
*/
	a1 = tmp3 >> 0x04;
	reg = *(volatile int *)0xa5081018;
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
		if(reg != -1) {

/*
     4e4:	00a04021 	move	$t0,$a1
     4e8:	01203821 	move	$a3,$t1
     4ec:	01070018 	mult	$t0,$a3
     4f0:	3c02431b 	lui	$v0,0x431b
     4f4:	00001812 	mflo	$v1
     4f8:	3442de83 	ori	$v0,$v0,0xde83
     4fc:	00000000 	nop
     500:	00620019 	multu	$v1,$v0
     504:	00001010 	mfhi	$v0
     508:	00021502 	srl	$v0,$v0,0x14
     50c:	2442ffff 	addiu	$v0,$v0,-1
     510:	00021100 	sll	$v0,$v0,0x4
     514:	1040000f 	beqz	$v0,554 <init_module-0x3e8>
     518:	00003021 	move	$a2,$zero
     51c:	01070018 	mult	$t0,$a3
     520:	3c02431b 	lui	$v0,0x431b
     524:	00001812 	mflo	$v1
     528:	3442de83 	ori	$v0,$v0,0xde83
     52c:	00000000 	nop
     530:	00620019 	multu	$v1,$v0
     534:	00001010 	mfhi	$v0
     538:	24c60001 	addiu	$a2,$a2,1
     53c:	00021502 	srl	$v0,$v0,0x14
     540:	2442ffff 	addiu	$v0,$v0,-1
     544:	00021100 	sll	$v0,$v0,0x4
     548:	00c2102b 	sltu	$v0,$a2,$v0
     54c:	1440fff4 	bnez	$v0,520 <init_module-0x41c>
     550:	01070018 	mult	$t0,$a3
     554:	3c02a508 	lui	$v0,0xa508
     558:	34421018 	ori	$v0,$v0,0x1018
     55c:	8c420000 	lw	$v0,0($v0)
     560:	04410004 	bgez	$v0,574 <init_module-0x3c8>
     564:	2402ffff 	li	$v0,-1
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

/* TODO: implement */
int strange_function2()
{
	int tmp;
	int tmp_t0, tmp_t1;
/*
     57c:	00805021 	move	$t2,$a0
*/
/*
     580:	3c080000 	lui	$t0,0x0
     584:	8d080000 	lw	$t0,0($t0)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     588:	3c090000 	lui	$t1,0x0
     58c:	8d290000 	lw	$t1,0($t1)
*/
	tmp_t1 = tiuhw_dsp_input_clock_speed;
/*
     590:	01090018 	mult	$t0,$t1
     594:	3c02431b 	lui	$v0,0x431b
     598:	00002012 	mflo	$a0
     59c:	3442de83 	ori	$v0,$v0,0xde83
     5a0:	00000000 	nop
     5a4:	00820019 	multu	$a0,$v0
     5a8:	3c05a508 	lui	$a1,0xa508
     5ac:	34a51010 	ori	$a1,$a1,0x1010
     5b0:	00002010 	mfhi	$a0
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
     5f4:	10800012 	beqz	$a0,640 <init_module-0x2fc>
     5f8:	3c02a508 	lui	$v0,0xa508
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
     640:	3442101c 	ori	$v0,$v0,0x101c
     644:	8c430000 	lw	$v1,0($v0)
     648:	0461002c 	bgez	$v1,6fc <init_module-0x240>
     64c:	2402ffff 	li	$v0,-1
     650:	24c6ffff 	addiu	$a2,$a2,-1
     654:	10c20029 	beq	$a2,$v0,6fc <init_module-0x240>
     658:	00000000 	nop
*/
/*
     65c:	3c090000 	lui	$t1,0x0
     660:	8d290000 	lw	$t1,0($t1)
*/
	tmp = tiuhw_dsp_clock_mult;
/*
     664:	3c080000 	lui	$t0,0x0
     668:	8d080000 	lw	$t0,0($t0)
*/
	tmp_t0 = tiuhw_dsp_input_clock_speed;
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
     6dc:	3c02a508 	lui	$v0,0xa508
     6e0:	3442101c 	ori	$v0,$v0,0x101c
     6e4:	8c430000 	lw	$v1,0($v0)
     6e8:	04610004 	bgez	$v1,6fc <init_module-0x240>
     6ec:	2402ffff 	li	$v0,-1
     6f0:	24c6ffff 	addiu	$a2,$a2,-1
     6f4:	54c2ffde 	0x54c2ffde
     6f8:	01203821 	move	$a3,$t1
     6fc:	10c00003 	beqz	$a2,70c <init_module-0x230>
     700:	24020001 	li	$v0,1
     704:	03e00008 	jr	$ra
     708:	a1430000 	sb	$v1,0($t2)
     70c:	03e00008 	jr	$ra
     710:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* probably read function */
/* DONE, TODO: magic numbers, magic functions */
int tnetv1050_tid_read(int tid, int ecVal, int ptr, int count)
{
	int ret = 1;
/*
     714:	27bdffd8 	addiu	$sp,$sp,-40
     718:	afbf0024 	sw	$ra,36($sp)
     71c:	afb40020 	sw	$s4,32($sp)
     720:	afb3001c 	sw	$s3,28($sp)
     724:	afb20018 	sw	$s2,24($sp)
     728:	afb10014 	sw	$s1,20($sp)
     72c:	afb00010 	sw	$s0,16($sp)
     730:	00e09021 	move	$s2,$a3
     734:	00c08821 	move	$s1,$a2
     738:	24130001 	li	$s3,1
     73c:	3094ffff 	andi	$s4,$a0,0xffff
     740:	02802021 	move	$a0,$s4
*/
	tid &= 0xffff;
	
/*
     744:	3c020000 	lui	$v0,0x0
     748:	24420000 	addiu	$v0,$v0,0
     74c:	0040f809 	jalr	$v0
     750:	30b000ff 	andi	$s0,$a1,0xff
*/
	tiuhw_select_tid(tid);


/*
     754:	3c020000 	lui	$v0,0x0
     758:	24420450 	addiu	$v0,$v0,1104
     75c:	0040f809 	jalr	$v0
     760:	02002021 	move	$a0,$s0
     764:	080001dc 	j	770 <init_module-0x1cc>
     768:	00000000 	nop
*/
	strange_function1(ecVal&0xff);

/*
     76c:	2652ffff 	addiu	$s2,$s2,-1

     770:	12400008 	beqz	$s2,794 <init_module-0x1a8>
     774:	00000000 	nop

     778:	3c020000 	lui	$v0,0x0
     77c:	2442057c 	addiu	$v0,$v0,1404
     780:	0040f809 	jalr	$v0

     784:	02202021 	move	$a0,$s1
     788:	00409821 	move	$s3,$v0
     78c:	1660fff7 	bnez	$s3,76c <init_module-0x1d0>
     790:	26310001 	addiu	$s1,$s1,1
*/
	while(count !=0  && ((ret = strange_function2(ptr)) != 0)) { ptr++; count--; }

/*
     794:	3c020000 	lui	$v0,0x0
     798:	24420000 	addiu	$v0,$v0,0
     79c:	0040f809 	jalr	$v0
     7a0:	02802021 	move	$a0,$s4
     7a4:	02601021 	move	$v0,$s3
*/
	tiuhw_deselect_tid(tid);
/*
     7a8:	8fbf0024 	lw	$ra,36($sp)
     7ac:	8fb40020 	lw	$s4,32($sp)
     7b0:	8fb3001c 	lw	$s3,28($sp)
     7b4:	8fb20018 	lw	$s2,24($sp)
     7b8:	8fb10014 	lw	$s1,20($sp)
     7bc:	8fb00010 	lw	$s0,16($sp)
     7c0:	03e00008 	jr	$ra
     7c4:	27bd0028 	addiu	$sp,$sp,40
*/
	return ret;
}

/* probably write function */
/* TODO: implement */
int tnetv1050_tid_write(int a1, int a2, int a3)
{
	int tid;
	int tmp1;
/*
     7c8:	27bdffd8 	addiu	$sp,$sp,-40
     7cc:	afbf0024 	sw	$ra,36($sp)
     7d0:	afb40020 	sw	$s4,32($sp)
     7d4:	afb3001c 	sw	$s3,28($sp)
     7d8:	afb20018 	sw	$s2,24($sp)
     7dc:	afb10014 	sw	$s1,20($sp)
     7e0:	afb00010 	sw	$s0,16($sp)
*/

/*
     7e4:	00e09021 	move	$s2,$a3
     7e8:	00c08821 	move	$s1,$a2
     7ec:	24130001 	li	$s3,1
     7f0:	3094ffff 	andi	$s4,$a0,0xffff
*/
	tid = (a1 & 0xffff);

/*
     7f4:	02802021 	move	$a0,$s4
     7f8:	3c020000 	lui	$v0,0x0
     7fc:	24420000 	addiu	$v0,$v0,0
     800:	0040f809 	jalr	$v0
*/
	tiuhw_select_tid(tid);
/*
     804:	30b000ff 	andi	$s0,$a1,0xff
*/
	tmp1 = a2&0xff;

/*
     808:	02002021 	move	$a0,$s0
     80c:	3c100000 	lui	$s0,0x0
     810:	26100450 	addiu	$s0,$s0,1104
     814:	0200f809 	jalr	$s0
     818:	00000000 	nop
*/
	strange_function1(tmp1);

/*
     81c:	0800020a 	j	828 <init_module-0x114>
     820:	00000000 	nop
*/

/*
     824:	2652ffff 	addiu	$s2,$s2,-1
*/

/*
     828:	12400006 	beqz	$s2,844 <init_module-0xf8>
     82c:	00000000 	nop
     830:	0200f809 	jalr	$s0
     834:	92240000 	lbu	$a0,0($s1)
     838:	00409821 	move	$s3,$v0
     83c:	1660fff9 	bnez	$s3,824 <init_module-0x118>
     840:	26310001 	addiu	$s1,$s1,1
     844:	3c020000 	lui	$v0,0x0
     848:	24420000 	addiu	$v0,$v0,0
     84c:	0040f809 	jalr	$v0
     850:	02802021 	move	$a0,$s4
     854:	02601021 	move	$v0,$s3
     858:	8fbf0024 	lw	$ra,36($sp)
     85c:	8fb40020 	lw	$s4,32($sp)
     860:	8fb3001c 	lw	$s3,28($sp)
     864:	8fb20018 	lw	$s2,24($sp)
     868:	8fb10014 	lw	$s1,20($sp)
     86c:	8fb00010 	lw	$s0,16($sp)
     870:	03e00008 	jr	$ra
     874:	27bd0028 	addiu	$sp,$sp,40
*/
	return 0;
}

/* DONE */
int func03(void)
{
/*
     878:	03e00008 	jr	$ra
     87c:	00001021 	move	$v0,$zero
*/
	return 0;
}


/* DONE */
int func11(void)
{
/*
     880:	03e00008 	jr	$ra
     884:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* DONE */
int func10(void)
{
/*
     888:	03e00008 	jr	$ra
     88c:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* DONE */
int func12(void)
{
/*
     890:	03e00008 	jr	$ra
     894:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* DONE */
int func13(void)
{
/*
     898:	03e00008 	jr	$ra
     89c:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* DONE */
int func20(void)
{
/*
     8a0:	03e00008 	jr	$ra
     8a4:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* DONE */
int func21(void)
{
/*
     8a8:	03e00008 	jr	$ra
     8ac:	00001021 	move	$v0,$zero
*/
	return 0;
}

/* TODO: NOR, 'magic number sections' -> ar7_* functions */
int func22(int a, int b)
{
	int c=a&0xffff;
	int reg;
/*
     8b0:	3084ffff 	andi	$a0,$a0,0xffff
     8b4:	2c820004 	sltiu	$v0,$a0,4
     8b8:	10400010 	beqz	$v0,8fc <init_module-0x40>
*/
	if(c < 4) {

/*
     8bc:	24060001 	li	$a2,1
     8c0:	14a60008 	bne	$a1,$a2,8e4 <init_module-0x58>
*/
		if(b == TIHW_INTERNAL) {

/*
     8c4:	3c02a508 	lui	$v0,0xa508
     8c8:	34421020 	ori	$v0,$v0,0x1020
     8cc:	8c430000 	lw	$v1,0($v0)
     8d0:	00862004 	sllv	$a0,$a2,$a0
     8d4:	00641825 	or	$v1,$v1,$a0
     8d8:	ac430000 	sw	$v1,0($v0)
     8dc:	03e00008 	jr	$ra
     8e0:	24020001 	li	$v0,1
*/
			reg = *(volatile int *)0xa5081020;
			reg |= (1 << c);
			*(volatile int *)0xa5081020 = reg;
		} else {
/*
     8e4:	34421020 	ori	$v0,$v0,0x1020
     8e8:	00861804 	sllv	$v1,$a2,$a0
     8ec:	8c440000 	lw	$a0,0($v0)
     8f0:	00031827 	nor	$v1,$zero,$v1
     8f4:	00832024 	and	$a0,$a0,$v1
     8f8:	ac440000 	sw	$a0,0($v0)
*/
			reg = *(volatile int *)0xa5081020;
			// TODO : NOR
			*(volatile int *)0xa5081020 = reg;
		}
	}
/*
     8fc:	03e00008 	jr	$ra
     900:	24020001 	li	$v0,1
*/
	return 1;
}

/* DONE */
int func23(int a, int b)
{
	int a1;
	int ret;
/*
     904:	27bdffe8 	addiu	$sp,$sp,-24
     908:	afbf0010 	sw	$ra,16($sp)
*/
/*
     90c:	24020001 	li	$v0,1
     910:	10a20003 	beq	$a1,$v0,920 <init_module-0x1c>
     914:	3084ffff 	andi	$a0,$a0,0xffff
*/
	a1 = a&0xffff;
	if(b != 1) {
/*
     918:	0800024c 	j	930 <init_module-0xc>
     91c:	00001021 	move	$v0,$zero
*/
		ret = 0;
/*
	} else {
     920:	3c020000 	lui	$v0,0x0
     924:	24420000 	addiu	$v0,$v0,0
     928:	0040f809 	jalr	$v0
     92c:	00000000 	nop
*/
		ret = tiuhw_get_tnetv1050_tid_type();
	}
/*
     930:	8fbf0010 	lw	$ra,16($sp)
     934:	03e00008 	jr	$ra
     938:	27bd0018 	addiu	$sp,$sp,24
*/
	return ret;
}

/* TODO : pointers & internal structures */
static int __init tihw_an_init_module(void) {
	int i;
	int ret;
/*
000000000000093c <init_module>:
     93c:	27bdffe0 	addiu	$sp,$sp,-32
     940:	afbf0018 	sw	$ra,24($sp)
*/

/*
     944:	3c030000 	lui	$v1,0x0
     948:	24630038 	addiu	$v1,$v1,56
     94c:	3c020000 	lui	$v0,0x0
     950:	24420000 	addiu	$v0,$v0,0
     954:	24440030 	addiu	$a0,$v0,48
     958:	8c450000 	lw	$a1,0($v0)
     95c:	8c460004 	lw	$a2,4($v0)
     960:	8c470008 	lw	$a3,8($v0)
     964:	8c48000c 	lw	$t0,12($v0)
     968:	ac650000 	sw	$a1,0($v1)
     96c:	ac660004 	sw	$a2,4($v1)
     970:	ac670008 	sw	$a3,8($v1)
     974:	ac68000c 	sw	$t0,12($v1)
     978:	24420010 	addiu	$v0,$v0,16
     97c:	1444fff6 	bne	$v0,$a0,958 <init_module+0x1c>
     980:	24630010 	addiu	$v1,$v1,16
*/
	for(i=0; i<3; i++) {
	// 	v0 = pointer_with_defaults[i];
	// 	v1 = some_table_of_structures[i];
	// 	v0 = pointer_with_defaults[i];
		some_table_of_structures[i].field0 = tnetv1050_api[i].field0;

		// v1->field0 = v0->field0;
		// v1->field1 = v0->field1;
	// 	v1->field2 = v0->field2;
	// 	v1->field3 = v0->field3;

	}

#warning WHAT IT MEANS: saving on stack?
/*
     984:	8c440000 	lw	$a0,0($v0)
     988:	ac640000 	sw	$a0,0($v1)
*/
	
	other_pointer_dst = other_pointer_src;
/*
     98c:	afa00010 	sw	$zero,16($sp)
*/

/*
     990:	24040028 	li	$a0,40
     994:	3c050000 	lui	$a1,0x0
     998:	24a50448 	addiu	$a1,$a1,1096
     99c:	3c070000 	lui	$a3,0x0
     9a0:	24e70124 	addiu	$a3,$a3,292
     9a4:	3c020000 	lui	$v0,0x0
     9a8:	24420000 	addiu	$v0,$v0,0
     9ac:	0040f809 	jalr	$v0
     9b0:	00003021 	move	$a2,$zero
*/
	ret = request_irq(AR7_IRQ_DSP,null_interrupt_handler1, IRQF_SHARED, stub_text,0);

/*
     9b4:	8fbf0018 	lw	$ra,24($sp)
     9b8:	00001021 	move	$v0,$zero
     9bc:	03e00008 	jr	$ra
     9c0:	27bd0020 	addiu	$sp,$sp,32
*/
	return 0;
}

/* TODO: implement */
static void __exit tihw_an_cleanup_module(void)
{
/*
00000000000009c4 <cleanup_module>:
     9c4:	27bdffe8 	addiu	$sp,$sp,-24
     9c8:	afbf0010 	sw	$ra,16($sp)
*/
/*
     9cc:	24040028 	li	$a0,40
     9d0:	3c020000 	lui	$v0,0x0
     9d4:	24420000 	addiu	$v0,$v0,0
     9d8:	0040f809 	jalr	$v0
     9dc:	00002821 	move	$a1,$zero
*/
	free_irq(AR7_IRQ_DSP, NULL);
/*
     9e0:	8fbf0010 	lw	$ra,16($sp)
     9e4:	03e00008 	jr	$ra
     9e8:	27bd0018 	addiu	$sp,$sp,24
     9ec:	00000000 	nop
*/
	return;
}

/* TODO : check, and make it more complete */
int tiuhw_get_dsp_clk_values(void)
{
	int ret = -1;
	char *dsp_clk_str;
	unsigned long int a1, a2, a3;

/* prologue
00000000000009f0 <tiuhw_get_dsp_clk_values>:
     9f0:	27bdff60 	addiu	$sp,$sp,-160
     9f4:	afbf009c 	sw	$ra,156($sp)
     9f8:	afb20098 	sw	$s2,152($sp)
     9fc:	afb10094 	sw	$s1,148($sp)
     a00:	afb00090 	sw	$s0,144($sp)
*/
/*
     a04:	3c120000 	lui	$s2,0x0
     a08:	26520208 	addiu	$s2,$s2,520
     a0c:	3c020000 	lui	$v0,0x0
     a10:	24420000 	addiu	$v0,$v0,0
     a14:	0040f809 	jalr	$v0
     a18:	02402021 	move	$a0,$s2
*/
	dsp_clk_str = prom_getenv("DSP_CLK");

/*
     a1c:	00401821 	move	$v1,$v0
     a20:	10600042 	beqz	$v1,b2c <tiuhw_get_dsp_clk_values+0x13c>
*/
	if(dsp_clk_str) {

/*
     a24:	27a40010 	addiu	$a0,$sp,16
     a28:	24020080 	li	$v0,128
     a2c:	00802821 	move	$a1,$a0
     a30:	90610000 	lbu	$at,0($v1)
     a34:	2442ffff 	addiu	$v0,$v0,-1
     a38:	a0a10000 	sb	$at,0($a1)
     a3c:	10200003 	beqz	$at,a4c <tiuhw_get_dsp_clk_values+0x5c>
     a40:	24a50001 	addiu	$a1,$a1,1
     a44:	1440fffa 	bnez	$v0,a30 <tiuhw_get_dsp_clk_values+0x40>
     a48:	24630001 	addiu	$v1,$v1,1
     a4c:	3c100000 	lui	$s0,0x0
     a50:	26100210 	addiu	$s0,$s0,528
*/

/*
     a54:	3c110000 	lui	$s1,0x0
     a58:	26310000 	addiu	$s1,$s1,0
     a5c:	0220f809 	jalr	$s1
     a60:	02002821 	move	$a1,$s0
     a64:	00002021 	move	$a0,$zero
*/
		// dsp_clk = strtok(dsp_clk_str);

/*
     a68:	02002821 	move	$a1,$s0
     a6c:	0220f809 	jalr	$s1
     a70:	00408021 	move	$s0,$v0
     a74:	02002021 	move	$a0,$s0
     a78:	00002821 	move	$a1,$zero
*/
/*
     a7c:	2406000a 	li	$a2,10
     a80:	3c110000 	lui	$s1,0x0
     a84:	26310000 	addiu	$s1,$s1,0
     a88:	0220f809 	jalr	$s1
     a8c:	00408021 	move	$s0,$v0
*/

/*
     a90:	3c010000 	lui	$at,0x0
     a94:	ac2200c0 	sw	$v0,192($at)
*/

/*
     a98:	02002021 	move	$a0,$s0
     a9c:	00002821 	move	$a1,$zero
     aa0:	0220f809 	jalr	$s1
     aa4:	2406000a 	li	$a2,10
*/
/*
     aa8:	3c040000 	lui	$a0,0x0
     aac:	8c8400c0 	lw	$a0,192($a0)
     ab0:	00820018 	mult	$a0,$v0
     ab4:	3c010000 	lui	$at,0x0
     ab8:	ac2200bc 	sw	$v0,188($at)
     abc:	3c070773 	lui	$a3,0x773
     ac0:	00001812 	mflo	$v1
     ac4:	34e75940 	ori	$a3,$a3,0x5940
     ac8:	00e3182b 	sltu	$v1,$a3,$v1
     acc:	1060001d 	beqz	$v1,b44 <tiuhw_get_dsp_clk_values+0x154>
     ad0:	00000000 	nop
     ad4:	00e4001b 	divu	$zero,$a3,$a0
     ad8:	00001812 	mflo	$v1
     adc:	50800001 	0x50800001
     ae0:	0007000d 	break	0x7
     ae4:	3c010000 	lui	$at,0x0
     ae8:	ac2300bc 	sw	$v1,188($at)
     aec:	14600003 	bnez	$v1,afc <tiuhw_get_dsp_clk_values+0x10c>
     af0:	2467ffff 	addiu	$a3,$v1,-1
     af4:	080002de 	j	b78 <tiuhw_get_dsp_clk_values+0x188>
     af8:	00001021 	move	$v0,$zero
     afc:	3c010000 	lui	$at,0x0
     b00:	ac2700bc 	sw	$a3,188($at)
*/
/*
     b04:	3c040000 	lui	$a0,0x0
     b08:	24840214 	addiu	$a0,$a0,532
     b0c:	02402821 	move	$a1,$s2
     b10:	3c060773 	lui	$a2,0x773
     b14:	3c020000 	lui	$v0,0x0
     b18:	24420000 	addiu	$v0,$v0,0
     b1c:	0040f809 	jalr	$v0
     b20:	34c65940 	ori	$a2,$a2,0x5940
*/
		a1 = 0x7735940; // 120MHz
		printk(KERN_ERR "%s exceeds %lu, dropping multiplier to %lu\n", "DSP_CLK", a1, a2);

/*
     b24:	080002d1 	j	b44 <tiuhw_get_dsp_clk_values+0x154>
     b28:	00000000 	nop
*/
	} else {

/*
     b2c:	2402000f 	li	$v0,15
     b30:	3c010000 	lui	$at,0x0
     b34:	ac2200bc 	sw	$v0,188($at)
*/
		tiuhw_dsp_clock_mult = 0x1f;

/*
     b38:	3c02007d 	lui	$v0,0x7d
     b3c:	3c010000 	lui	$at,0x0
     b40:	ac2200c0 	sw	$v0,192($at)
*/
		tiuhw_dsp_input_clock_speed = 0x7d;
	}

/*
     b44:	3c050000 	lui	$a1,0x0
     b48:	8ca500bc 	lw	$a1,188($a1)
*/
	a1 = tiuhw_dsp_clock_mult;

/*
     b4c:	3c060000 	lui	$a2,0x0
     b50:	8cc600c0 	lw	$a2,192($a2)
*/
	a2 = tiuhw_dsp_input_clock_speed;
	a3 = a1 * a2;

/*
     b54:	00a60018 	mult	$a1,$a2
     b58:	3c040000 	lui	$a0,0x0
     b5c:	24840244 	addiu	$a0,$a0,580
     b60:	00003812 	mflo	$a3
     b64:	3c020000 	lui	$v0,0x0
     b68:	24420000 	addiu	$v0,$v0,0
     b6c:	0040f809 	jalr	$v0
     b70:	00000000 	nop
     b74:	24020001 	li	$v0,1
*/
	printk(KERN_ERR "Internal TNETV1060 DSP settings:\n DSP mult: %lu Input clock: %lu, final DSP speed: %lu\n", a1, a2, a3);
	ret = 1;
/*
     b78:	8fbf009c 	lw	$ra,156($sp)
     b7c:	8fb20098 	lw	$s2,152($sp)
     b80:	8fb10094 	lw	$s1,148($sp)
     b84:	8fb00090 	lw	$s0,144($sp)
     b88:	03e00008 	jr	$ra
     b8c:	27bd00a0 	addiu	$sp,$sp,160
*/
	return ret;
}

/* ALMOST DONE, fix pointers, change 'magic number sections' to ar7_* functions */
int tiuhw_init_hal(int a, int b)
{
	int if_type = -1;
	int reg;
	int tmp;

/* prologue
0000000000000b90 <tiuhw_init_hal>:
     b90:	27bdffd8 	addiu	$sp,$sp,-40
     b94:	afbf0020 	sw	$ra,32($sp)
     b98:	afb3001c 	sw	$s3,28($sp)
     b9c:	afb20018 	sw	$s2,24($sp)
     ba0:	afb10014 	sw	$s1,20($sp)
     ba4:	afb00010 	sw	$s0,16($sp)
*/

/*
     ba8:	00809821 	move	$s3,$a0
     bac:	3c020000 	lui	$v0,0x0
     bb0:	2442127c 	addiu	$v0,$v0,4732
     bb4:	0040f809 	jalr	$v0
     bb8:	00a08821 	move	$s1,$a1
*/
	if_type = hwu_get_tiuhw_if();

/*
     bbc:	00401821 	move	$v1,$v0
     bc0:	24020001 	li	$v0,1
     bc4:	10620005 	beq	$v1,$v0,bdc <tiuhw_init_hal+0x4c>
     bc8:	24020002 	li	$v0,2
     bcc:	10620055 	beq	$v1,$v0,d24 <tiuhw_init_hal+0x194>
     bd0:	00000000 	nop
     bd4:	08000371 	j	dc4 <tiuhw_init_hal+0x234>
     bd8:	00000000 	nop
*/
	if(if_type == TIHW_INTERNAL) {


/*   tiuhw_get_dsp_clk_values
     bdc:	3c020000 	lui	$v0,0x0 
     be0:	244209f0 	addiu	$v0,$v0,2544
     be4:	0040f809 	jalr	$v0
     be8:	00000000 	nop
*/
		tiuhw_get_dsp_clk_values();

/* printk
     bec:	3c040000 	lui	$a0,0x0
     bf0:	248402a0 	addiu	$a0,$a0,672
     bf4:	3c020000 	lui	$v0,0x0
     bf8:	24420000 	addiu	$v0,$v0,0
     bfc:	0040f809 	jalr	$v0
     c00:	00000000 	nop
*/
		printk(KERN_ERR "Internal TNETV1060 Telephony HAL initializing\n");

/*
     c04:	3c020000 	lui	$v0,0x0
     c08:	8c420070 	lw	$v0,112($v0)
     c0c:	10400026 	beqz	$v0,ca8 <tiuhw_init_hal+0x118>
     c10:	3c03a861 	lui	$v1,0xa861
*/
		if(should_reset_tid) {

/*
     c14:	34631660 	ori	$v1,$v1,0x1660
     c18:	3c04a861 	lui	$a0,0xa861
     c1c:	34840914 	ori	$a0,$a0,0x914

     c20:	3c07fff7 	lui	$a3,0xfff7
     c24:	34e7ffff 	ori	$a3,$a3,0xffff

     c28:	3c05a861 	lui	$a1,0xa861
     c2c:	34a5090c 	ori	$a1,$a1,0x90c

     c30:	3c08a861 	lui	$t0,0xa861
     c34:	8c620000 	lw	$v0,0($v1)

     c38:	3508091c 	ori	$t0,$t0,0x91c

     c3c:	34420001 	ori	$v0,$v0,0x1
     c40:	ac620000 	sw	$v0,0($v1)

     c44:	8c820000 	lw	$v0,0($a0)
     c48:	3c06a861 	lui	$a2,0xa861
     c4c:	00471024 	and	$v0,$v0,$a3
     c50:	ac820000 	sw	$v0,0($a0)

     c54:	8ca20000 	lw	$v0,0($a1)
     c58:	34c61640 	ori	$a2,$a2,0x1640
     c5c:	00471024 	and	$v0,$v0,$a3
     c60:	aca20000 	sw	$v0,0($a1)
     c64:	8d020000 	lw	$v0,0($t0)
     c68:	3c030008 	lui	$v1,0x8
     c6c:	00431025 	or	$v0,$v0,$v1
     c70:	ad020000 	sw	$v0,0($t0)

     c74:	8cc20000 	lw	$v0,0($a2)
     c78:	00008021 	move	$s0,$zero
     c7c:	34423000 	ori	$v0,$v0,0x3000
     c80:	acc20000 	sw	$v0,0($a2)
*/
			// reset
			reg = *(volatile int *)0xa8611660; // v1
			reg |= 0x01;
			*(volatile int *)0xa8611660 = reg;

			// gpio
			reg = *(volatile int *)0xa8610914; // a0
			reg &= 0xfff7ffff; // 0xfff7ffff is in a3
			*(volatile int *)0xa8610914 = reg;

			// gpio
			reg = *(volatile int *)0xa861090c; // a1
			reg &= 0xfff7ffff;
			*(volatile int *)0xa861090c = reg;

			// gpio
			reg = *(volatile int *)0xa861091c; // t0
			reg |= 0x00080000;
			*(volatile int *)0xa861091c = reg; // t0

			// reset
			reg = *(volatile int *)0xa8611640; // a2
			reg |= 0x00003000;
			*(volatile int *)0xa8611640 = reg; // a2

/* tiuhw_reset_tid
     c84:	02002021 	move	$a0,$s0
     c88:	3c020000 	lui	$v0,0x0
     c8c:	24421200 	addiu	$v0,$v0,4608
     c90:	0040f809 	jalr	$v0
     c94:	24050001 	li	$a1,1

     c98:	26020001 	addiu	$v0,$s0,1
     c9c:	305000ff 	andi	$s0,$v0,0xff
     ca0:	1200fff9 	beqz	$s0,c88 <tiuhw_init_hal+0xf8>
     ca4:	02002021 	move	$a0,$s0
*/
			tiuhw_reset_tid(DSP_CORE_0, DSP_RESET_ON);
		}

/* hw_apis
     ca8:	3c020000 	lui	$v0,0x0
     cac:	24420000 	addiu	$v0,$v0,0
     cb0:	3c010000 	lui	$at,0x0
     cb4:	ac2200b8 	sw	$v0,184($at)
*/
#warning TODO:
/*			my_hardware_api_pointers = hw_apis; */

/*
     cb8:	02602021 	move	$a0,$s3
     cbc:	8c420000 	lw	$v0,0($v0)
     cc0:	0040f809 	jalr	$v0
     cc4:	02202821 	move	$a1,$s1
     			// call hw_apis + 0(a,b);

     cc8:	3c021fff 	lui	$v0,0x1fff
     ccc:	3442fc70 	ori	$v0,$v0,0xfc70
*/
/*
     cd0:	3c030000 	lui	$v1,0x0
     cd4:	8c630000 	lw	$v1,0($v1)
*/
	tmp = loops_per_jiffy;
/*
     cd8:	00430019 	multu	$v0,$v1
     cdc:	00001010 	mfhi	$v0
     ce0:	1440ffff 	bnez	$v0,ce0 <tiuhw_init_hal+0x150>
     ce4:	2442ffff 	addiu	$v0,$v0,-1

     ce8:	3c020000 	lui	$v0,0x0
     cec:	8c420070 	lw	$v0,112($v0)
     cf0:	10400034 	beqz	$v0,dc4 <tiuhw_init_hal+0x234>
     cf4:	00008021 	move	$s0,$zero
*/
     			if(should_reset_tid) {

/*
     cf8:	02002021 	move	$a0,$s0
     cfc:	3c020000 	lui	$v0,0x0
     d00:	24421200 	addiu	$v0,$v0,4608
     d04:	0040f809 	jalr	$v0
     d08:	00002821 	move	$a1,$zero
*/
     				tiuhw_reset_tid(DSP_CORE_0, DSP_RESET_OFF);

/*
     d0c:	26020001 	addiu	$v0,$s0,1
     d10:	305000ff 	andi	$s0,$v0,0xff
     d14:	1200fff9 	beqz	$s0,cfc <tiuhw_init_hal+0x16c>
     d18:	02002021 	move	$a0,$s0

     d1c:	08000371 	j	dc4 <tiuhw_init_hal+0x234>
     d20:	00000000 	nop
*/
			}
	} else if(if_type == TIHW_EXTERNAL) {
		// not necessery - cause we have INTERNAL (no-FPGA)
/*
     d24:	3c040000 	lui	$a0,0x0
     d28:	248402d4 	addiu	$a0,$a0,724
     d2c:	3c120000 	lui	$s2,0x0
     d30:	26520000 	addiu	$s2,$s2,0
     d34:	0240f809 	jalr	$s2
     d38:	00008021 	move	$s0,$zero
*/
/*
     d3c:	3c020000 	lui	$v0,0x0
     d40:	24420000 	addiu	$v0,$v0,0
     d44:	3c010000 	lui	$at,0x0
     d48:	ac2200b8 	sw	$v0,184($at)
     d4c:	02602021 	move	$a0,$s3
     d50:	00002821 	move	$a1,$zero
     d54:	8c420000 	lw	$v0,0($v0)
     d58:	0040f809 	jalr	$v0
     d5c:	24110001 	li	$s1,1
     d60:	3c040000 	lui	$a0,0x0
     d64:	24840308 	addiu	$a0,$a0,776
     d68:	0240f809 	jalr	$s2
     d6c:	92650000 	lbu	$a1,0($s3)
     d70:	8e620004 	lw	$v0,4($s3)
     d74:	00511024 	and	$v0,$v0,$s1
     d78:	10400006 	beqz	$v0,d94 <tiuhw_init_hal+0x204>
     d7c:	00111040 	sll	$v0,$s1,0x1
     d80:	3c040000 	lui	$a0,0x0
     d84:	24840338 	addiu	$a0,$a0,824
     d88:	0240f809 	jalr	$s2
     d8c:	02002821 	move	$a1,$s0
     d90:	00111040 	sll	$v0,$s1,0x1
     d94:	305100fe 	andi	$s1,$v0,0xfe
     d98:	26020001 	addiu	$v0,$s0,1
     d9c:	305000ff 	andi	$s0,$v0,0xff
     da0:	2e020008 	sltiu	$v0,$s0,8
     da4:	5440fff3 	0x5440fff3
     da8:	8e620004 	lw	$v0,4($s3)
     dac:	3c040000 	lui	$a0,0x0
     db0:	24840340 	addiu	$a0,$a0,832
     db4:	3c020000 	lui	$v0,0x0
     db8:	24420000 	addiu	$v0,$v0,0
     dbc:	0040f809 	jalr	$v0
     dc0:	00000000 	nop
*/
	}
/*
     dc4:	3c040000 	lui	$a0,0x0
     dc8:	24840344 	addiu	$a0,$a0,836
     dcc:	3c020000 	lui	$v0,0x0
     dd0:	24420000 	addiu	$v0,$v0,0
     dd4:	0040f809 	jalr	$v0
     dd8:	00000000 	nop
*/
	printk(KERN_ERR "DONE w/ HAL init\n");
/*
     ddc:	3c010000 	lui	$at,0x0
     de0:	ac200070 	sw	$zero,112($at)
*/
	should_reset_tid = 0;

/*
     de4:	8fbf0020 	lw	$ra,32($sp)
     de8:	8fb3001c 	lw	$s3,28($sp)
     dec:	8fb20018 	lw	$s2,24($sp)
     df0:	8fb10014 	lw	$s1,20($sp)
     df4:	8fb00010 	lw	$s0,16($sp)
     df8:	24020001 	li	$v0,1
     dfc:	03e00008 	jr	$ra
     e00:	27bd0028 	addiu	$sp,$sp,40
*/
	return 1;
}

/* DONE */
void tiuhw_select_tid(int tid)
{

/*
0000000000000e04 <tiuhw_select_tid>:
     e04:	03e00008 	jr	$ra
     e08:	00000000 	nop
*/
	return;
}

/* DONE */
void tiuhw_deselect_tid(int tid)
{
/*
0000000000000e0c <tiuhw_deselect_tid>:
     e0c:	03e00008 	jr	$ra
     e10:	00000000 	nop
*/
	return;
}

/* TODO: implement */
int tiuhw_get_tid_type(int tid)
{
	int ret = -1;
	char *num_tids_str = NULL;
	int num_tids = 0;
	int tmp0, tmp1, tmp3;
	char *tele_id = NULL, *tmp_tele_id = NULL, *end_buf = NULL;
	int some_tmp;

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
	tmp0 = 1; // s3
	tmp1 = tid & 0xff; // s6

/*
     e40:	3c040000 	lui	$a0,0x0
     e44:	248403a0 	addiu	$a0,$a0,928
     e48:	3c110000 	lui	$s1,0x0
     e4c:	26310000 	addiu	$s1,$s1,0
     e50:	0220f809 	jalr	$s1
     e54:	00009021 	move	$s2,$zero
*/
	num_tids_str = prom_getenv("NUM_TIDS");
	tmp3 = 0; // s2

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
		if(num_tids<2) tmp0 = 1;
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
			char substring[255];


/*
     ed8:	24020080 	li	$v0,128
     edc:	00802821 	move	$a1,$a0

     ee0:	92010000 	lbu	$at,0($s0)
     ee4:	2442ffff 	addiu	$v0,$v0,-1
     ee8:	a0a10000 	sb	$at,0($a1)

     eec:	10200003 	beqz	$at,efc <tiuhw_get_tid_type+0xe8>
     ef0:	24a50001 	addiu	$a1,$a1,1

     ef4:	1440fffa 	bnez	$v0,ee0 <tiuhw_get_tid_type+0xcc>
     ef8:	26100001 	addiu	$s0,$s0,1
*/
			strcpy(substring, tele_id);

/*

     efc:	3c150000 	lui	$s5,0x0
     f00:	26b50210 	addiu	$s5,$s5,528
     f04:	3c140000 	lui	$s4,0x0
     f08:	26940000 	addiu	$s4,$s4,0
     f0c:	0280f809 	jalr	$s4
     f10:	02a02821 	move	$a1,$s5
*/
			tmp_tele_id = substring;
			strsep(&tmp_tele_id, ":");
		
			// HERE

/*
     f14:	00402821 	move	$a1,$v0
     f18:	10a00082 	beqz	$a1,1124 <tiuhw_get_tid_type+0x310>
     f1c:	00000000 	nop
*/
			// tmp_tele_id now holds reference to integer value
			// if(tmp_tele_id == NULL) goto out;

/*
     f20:	12600080 	beqz	$s3,1124 <tiuhw_get_tid_type+0x310>
     f24:	00008821 	move	$s1,$zero
*/
     			if(tmp0) {
				some_tmp = 0; // s1
/*
     f28:	080003cd 	j	f34 <tiuhw_get_tid_type+0x120>
     f2c:	00000000 	nop
*/
			}

/*
     f30:	305100ff 	andi	$s1,$v0,0xff
     f34:	001110c0 	sll	$v0,$s1,0x3
     f38:	3c100000 	lui	$s0,0x0
     f3c:	26100074 	addiu	$s0,$s0,116
     f40:	02028021 	addu	$s0,$s0,$v0
     f44:	92060004 	lbu	$a2,4($s0)
     f48:	10c0000e 	beqz	$a2,f84 <tiuhw_get_tid_type+0x170>
     f4c:	00a02021 	move	$a0,$a1
*/
			

/*
     f50:	8e020000 	lw	$v0,0($s0)
     f54:	00401821 	move	$v1,$v0
     f58:	90820000 	lbu	$v0,0($a0)
     f5c:	90610000 	lbu	$at,0($v1)
     f60:	24840001 	addiu	$a0,$a0,1
     f64:	14220004 	bne	$at,$v0,f78 <tiuhw_get_tid_type+0x164>
     f68:	24630001 	addiu	$v1,$v1,1
     f6c:	1440fffb 	bnez	$v0,f5c <tiuhw_get_tid_type+0x148>
     f70:	90820000 	lbu	$v0,0($a0)
     f74:	00201021 	move	$v0,$at
     f78:	00411023 	subu	$v0,$v0,$at
     f7c:	1440ffec 	bnez	$v0,f30 <tiuhw_get_tid_type+0x11c>
     f80:	26220001 	addiu	$v0,$s1,1
     f84:	241100ff 	li	$s1,255
     f88:	14d1001d 	bne	$a2,$s1,1000 <tiuhw_get_tid_type+0x1ec>
     f8c:	02402021 	move	$a0,$s2
     f90:	24050001 	li	$a1,1
     f94:	3c020000 	lui	$v0,0x0
     f98:	8c4200b8 	lw	$v0,184($v0)
     f9c:	00003021 	move	$a2,$zero
     fa0:	8c42002c 	lw	$v0,44($v0)
     fa4:	0040f809 	jalr	$v0
     fa8:	00003821 	move	$a3,$zero
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
    1000:	54c0000c 	0x54c0000c
    1004:	02402821 	move	$a1,$s2
    1008:	3c040000 	lui	$a0,0x0
    100c:	248403c0 	addiu	$a0,$a0,960
    1010:	3c020000 	lui	$v0,0x0
    1014:	24420000 	addiu	$v0,$v0,0
    1018:	0040f809 	jalr	$v0
    101c:	00000000 	nop
*/
		// printk(KERN_ERR "Unrecognized TID ID string detected: %s\n", some_string_from_s2);
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
    1050:	3c010000 	lui	$at,0x0
    1054:	00320821 	addu	$at,$at,$s2
    1058:	a0220010 	sb	$v0,16($at)
    105c:	26420001 	addiu	$v0,$s2,1
    1060:	305200ff 	andi	$s2,$v0,0xff
    1064:	00002021 	move	$a0,$zero
    1068:	0280f809 	jalr	$s4
    106c:	02a02821 	move	$a1,$s5
    1070:	00402821 	move	$a1,$v0
    1074:	10a0002b 	beqz	$a1,1124 <tiuhw_get_tid_type+0x310>
    1078:	0253102b 	sltu	$v0,$s2,$s3
    107c:	1440ffad 	bnez	$v0,f34 <tiuhw_get_tid_type+0x120>
    1080:	00008821 	move	$s1,$zero
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
	ret += tmp1;
	return ret; // something from structure
}

/* DONE TILL HERE : FROM THE BOTTOM */

/* DONE */
int tiuhw_map_tcid_to_tid(int a)
{
	int ret = 255;
	int tmp;
	int if_type;
/* prologue
0000000000001158 <tiuhw_map_tcid_to_tid>:
    1158:	27bdffe8 	addiu	$sp,$sp,-24
    115c:	afbf0014 	sw	$ra,20($sp)
    1160:	afb00010 	sw	$s0,16($sp)
*/

/*
    1164:	3090ffff 	andi	$s0,$a0,0xffff
    1168:	2e020004 	sltiu	$v0,$s0,4
    116c:	10400010 	beqz	$v0,11b0 <tiuhw_map_tcid_to_tid+0x58>
    1170:	240200ff 	li	$v0,255
*/
	tmp = a&0xffff;
	if(tmp < 4) {
/*
    1174:	3c020000 	lui	$v0,0x0
    1178:	2442127c 	addiu	$v0,$v0,4732
    117c:	0040f809 	jalr	$v0
    1180:	00000000 	nop
*/
    		if_type = hwu_get_tiuhw_if();

/*
    1184:	00401821 	move	$v1,$v0
    1188:	24020001 	li	$v0,1
    118c:	10620005 	beq	$v1,$v0,11a4 <tiuhw_map_tcid_to_tid+0x4c>
    1190:	24020002 	li	$v0,2
*/
		if(if_type == TIHW_INTERNAL) {
			ret = 0;
/*
    1194:	10620005 	beq	$v1,$v0,11ac <tiuhw_map_tcid_to_tid+0x54>
    1198:	00101042 	srl	$v0,$s0,0x1
*/
		} else if(if_type == TIHW_EXTERNAL) {
			ret = (a << 1) & 0xff;	
/*
    119c:	0800046c 	j	11b0 <tiuhw_map_tcid_to_tid+0x58>
    11a0:	240200ff 	li	$v0,255
*/
		} else ret = 255;

/*
    11a4:	0800046c 	j	11b0 <tiuhw_map_tcid_to_tid+0x58>
    11a8:	00001021 	move	$v0,$zero
*/
/*
    11ac:	304200ff 	andi	$v0,$v0,0xff
*/
	}
/*
    11b0:	8fbf0014 	lw	$ra,20($sp)
    11b4:	8fb00010 	lw	$s0,16($sp)
    11b8:	03e00008 	jr	$ra
    11bc:	27bd0018 	addiu	$sp,$sp,24
*/
	return ret;
}

/* DONE */
int tiuhw_get_tnetv1050_tid_type(void)
{
/*
00000000000011c0 <tiuhw_get_tnetv1050_tid_type>:
    11c0:	03e00008 	jr	$ra
    11c4:	24020004 	li	$v0,4
*/
/* TODO: MAGIC_NUMBER?? */
	return 4;
}

/* DONE */
int tiuhw_get_dsp_mult(int interface)
{
	int if_type;
/*
00000000000011c8 <tiuhw_get_dsp_mult>:
    11c8:	27bdffe8 	addiu	$sp,$sp,-24
    11cc:	afbf0010 	sw	$ra,16($sp)
    11d0:	3c020000 	lui	$v0,0x0
    11d4:	2442127c 	addiu	$v0,$v0,4732
    11d8:	0040f809 	jalr	$v0
    11dc:	00000000 	nop
*/
	if_type = hwu_get_tiuhw_if();

/*
    11e0:	00402021 	move	$a0,$v0
    11e4:	24030001 	li	$v1,1
    11e8:	10830002 	beq	$a0,$v1,11f4 <tiuhw_get_dsp_mult+0x2c>
    11ec:	00001021 	move	$v0,$zero
*/
	if(if_type != TIHW_INTERNAL) return 5;
	return if_type;

/*
    11f0:	24020005 	li	$v0,5
    11f4:	8fbf0010 	lw	$ra,16($sp)
    11f8:	03e00008 	jr	$ra
    11fc:	27bd0018 	addiu	$sp,$sp,24
*/
}

/* DONE */
int tiuhw_reset_tid(int a, int cmd)
{
	int tmp1, reg;
/*
0000000000001200 <tiuhw_reset_tid>:
    1200:	27bdffe8 	addiu	$sp,$sp,-24
    1204:	afbf0014 	sw	$ra,20($sp)
    1208:	afb00010 	sw	$s0,16($sp)
*/

/*
    120c:	00a08021 	move	$s0,$a1
    1210:	308500ff 	andi	$a1,$a0,0xff
    1214:	3c040000 	lui	$a0,0x0
    1218:	24840404 	addiu	$a0,$a0,1028
    121c:	3c020000 	lui	$v0,0x0
    1220:	24420000 	addiu	$v0,$v0,0
    1224:	0040f809 	jalr	$v0
    1228:	02003021 	move	$a2,$s0
*/
	
	tmp1 = a & 0xff;
	printk(KERN_ERR "tiuhw_reset_tid: %u %u\n", tmp1, cmd);
/*
    122c:	12000009 	beqz	$s0,1254 <tiuhw_reset_tid+0x54>
*/
	if(cmd) {
/*
    1230:	3c02a861 	lui	$v0,0xa861
    1234:	3442090c 	ori	$v0,$v0,0x90c
    1238:	3c03fff7 	lui	$v1,0xfff7
    123c:	8c440000 	lw	$a0,0($v0)
    1240:	3463ffff 	ori	$v1,$v1,0xffff
    1244:	00832024 	and	$a0,$a0,$v1
    1248:	ac440000 	sw	$a0,0($v0)
*/
/*
		reg = *(volatile int *)0xa861090c;
		reg = (reg & 0xfff7ffff);
		*(volatile int *)0xa861090c = reg;
*/
		ar7_gpio_disable(AR7_RESET_BIT_TID);

/*
    124c:	0800049b 	j	126c <tiuhw_reset_tid+0x6c>
    1250:	8fbf0014 	lw	$ra,20($sp)
*/
	} else {

/*
    1254:	3442090c 	ori	$v0,$v0,0x90c
    1258:	8c430000 	lw	$v1,0($v0)
    125c:	3c040008 	lui	$a0,0x8
    1260:	00641825 	or	$v1,$v1,$a0
    1264:	ac430000 	sw	$v1,0($v0)
    1268:	8fbf0014 	lw	$ra,20($sp)
*/
/*
		reg = *(volatile int *)0xa861090c;
		reg |= 0x00080000;
		*(volatile int *)0xa861090c = reg;
*/
		ar7_gpio_enable(AR7_RESET_BIT_TID);
	}

/*
    126c:	8fb00010 	lw	$s0,16($sp)
    1270:	00001021 	move	$v0,$zero
    1274:	03e00008 	jr	$ra
    1278:	27bd0018 	addiu	$sp,$sp,24
*/
	return 0;
}

/* DONE */
int hwu_get_tiuhw_if(void) {
	char *teleif=NULL;
	int ret=-1;
/* prologue
000000000000127c <hwu_get_tiuhw_if>:
    127c:	27bdffe8 	addiu	$sp,$sp,-24
*/
/*
    1280:	3c020000 	lui	$v0,0x0
    1284:	8c4200b0 	lw	$v0,176($v0)
    1288:	1040003c 	beqz	$v0,137c <hwu_get_tiuhw_if+0x100>
    128c:	afbf0010 	sw	$ra,16($sp)
*/
#warning TODO: check this value meaning
	if(not_initialized) {
/*
    1290:	3c010000 	lui	$at,0x0
    1294:	ac2000b0 	sw	$zero,176($at)
*/
		not_initialized = 0;

/*
    1298:	3c040000 	lui	$a0,0x0
    129c:	24840420 	addiu	$a0,$a0,1056
    12a0:	3c020000 	lui	$v0,0x0
    12a4:	24420000 	addiu	$v0,$v0,0
    12a8:	0040f809 	jalr	$v0
    12ac:	00000000 	nop
*/
		teleif = prom_getenv("TELE_IF");
/*
    12b0:	00403021 	move	$a2,$v0
    12b4:	10c0002b 	beqz	$a2,1364 <hwu_get_tiuhw_if+0xe8>
*/
		if(teleif) {
/*
    12b8:	00c02021 	move	$a0,$a2 // w A0 mamy teleif
    12bc:	3c050000 	lui	$a1,0x0
    12c0:	24a50428 	addiu	$a1,$a1,1064
    12c4:	00a01821 	move	$v1,$a1	// V1 - mamy "EXTERNAL"

    12c8:	90820000 	lbu	$v0,0($a0)
    12cc:	90610000 	lbu	$at,0($v1)
    12d0:	24840001 	addiu	$a0,$a0,1
    12d4:	14220004 	bne	$at,$v0,12e8 <hwu_get_tiuhw_if+0x6c>
    12d8:	24630001 	addiu	$v1,$v1,1
    12dc:	1440fffb 	bnez	$v0,12cc <hwu_get_tiuhw_if+0x50>
    12e0:	90820000 	lbu	$v0,0($a0)
    12e4:	00201021 	move	$v0,$at

    12e8:	00411023 	subu	$v0,$v0,$at
    12ec:	14400004 	bnez	$v0,1300 <hwu_get_tiuhw_if+0x84>
*/
			if(!strcmp(teleif, "EXTERNAL")) {
/*
    12f0:	24020002 	li	$v0,2
    12f4:	3c010000 	lui	$at,0x0
    12f8:	080004df 	j	137c <hwu_get_tiuhw_if+0x100>
    12fc:	a02200b4 	sb	$v0,180($at)
*/
				interface_type = TIHW_EXTERNAL;
			} else if(!strcmp(teleif, "INTERNAL")) {
/*
    1300:	3c020000 	lui	$v0,0x0
    1304:	24420434 	addiu	$v0,$v0,1076
    1308:	00401821 	move	$v1,$v0
    130c:	90c20000 	lbu	$v0,0($a2)
    1310:	90610000 	lbu	$at,0($v1)
    1314:	24c60001 	addiu	$a2,$a2,1
    1318:	14220004 	bne	$at,$v0,132c <hwu_get_tiuhw_if+0xb0>
    131c:	24630001 	addiu	$v1,$v1,1
    1320:	1440fffb 	bnez	$v0,1310 <hwu_get_tiuhw_if+0x94>
    1324:	90c20000 	lbu	$v0,0($a2)
    1328:	00201021 	move	$v0,$at
    132c:	00411023 	subu	$v0,$v0,$at
    1330:	14400004 	bnez	$v0,1344 <hwu_get_tiuhw_if+0xc8>
    1334:	24020001 	li	$v0,1
    1338:	3c010000 	lui	$at,0x0
    133c:	080004df 	j	137c <hwu_get_tiuhw_if+0x100>
    1340:	a02200b4 	sb	$v0,180($at)
*/
				interface_type = TIHW_INTERNAL;
			} else {

/*
    1344:	3c040000 	lui	$a0,0x0
    1348:	24840440 	addiu	$a0,$a0,1088
    134c:	3c020000 	lui	$v0,0x0
    1350:	24420000 	addiu	$v0,$v0,0
    1354:	0040f809 	jalr	$v0
    1358:	00000000 	nop
    135c:	080004df 	j	137c <hwu_get_tiuhw_if+0x100>
    1360:	00000000 	nop
*/
				printk(KERN_ERR "Invalid Telephony interface specfied, defaulting to %s\n", "EXTERNAL");
					
			}
		} else {

/*
    1364:	3c040000 	lui	$a0,0x0
    1368:	2484047c 	addiu	$a0,$a0,1148
    136c:	3c020000 	lui	$v0,0x0
    1370:	24420000 	addiu	$v0,$v0,0
    1374:	0040f809 	jalr	$v0
    1378:	00000000 	nop
*/
			printk(KERN_ERR "Environment variable for telephony interface not set, assuming FPGA\n");
		}
    	}

/*
    137c:	3c020000 	lui	$v0,0x0
    1380:	904200b4 	lbu	$v0,180($v0)
*/
	ret = interface_type;
/* epilogue
    1384:	8fbf0010 	lw	$ra,16($sp)
    1388:	03e00008 	jr	$ra
    138c:	27bd0018 	addiu	$sp,$sp,24
*/
	return ret;
}

/*
0000000000001390 <tiuhw_led>:
    1390:	03e00008 	jr	$ra
	...
*/ 
/* DONE */
void tiuhw_led(void) {
	return;
}

module_init(tihw_an_init_module);
module_exit(tihw_an_cleanup_module);
