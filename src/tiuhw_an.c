
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/ar7/ar7.h>
#include <asm/ar7/gpio.h>

#include "legerity/vp_api_types.h"
#include "legerity/sys_service.h"
#include "legerity/hbi_hal.h"
#include "legerity/mpi_hal.h"

#include "tiuhw.h"
#include "tiuhal.h"


#define DRV_VERSION "0.0.1"
#define DRV_DESC "TIUHw_An driver"

#define WRITE_COMMAND 0
#define READ_COMMAND 1
static uint8 tempBuf[512]; /* All zero buffer */

void *globalApiPointer = NULL;

int tiuo_fill_drv_api(void *);

/*
../../../wrtp300-rootfs/lib/modules/2.4.17_mvl21-malta-mips_fp_le/kernel/drivers/tiuhw_an.o:     file format elf32-tradlittlemips

Disassembly of section .text:

0000000000000000 <tiuhw_set_default_hwparam-0x4c>:
       0:	94820000 	lhu	$v0,0($a0)
       4:	3c030000 	lui	$v1,0x0
       8:	9063006c 	lbu	$v1,108($v1)
       c:	0043102b 	sltu	$v0,$v0,$v1
      10:	1040000c 	beqz	$v0,44 <tiuhw_set_default_hwparam-0x8>
      14:	00000000 	nop
      18:	ac800004 	sw	$zero,4($a0)
      1c:	ac800008 	sw	$zero,8($a0)
      20:	ac80000c 	sw	$zero,12($a0)
      24:	ac800010 	sw	$zero,16($a0)
      28:	ac800014 	sw	$zero,20($a0)
      2c:	a480002c 	sh	$zero,44($a0)
      30:	ac800018 	sw	$zero,24($a0)
      34:	ac80001c 	sw	$zero,28($a0)
      38:	a4800028 	sh	$zero,40($a0)
      3c:	a480002a 	sh	$zero,42($a0)
      40:	ac800020 	sw	$zero,32($a0)
      44:	03e00008 	jr	$ra
      48:	00000000 	nop

000000000000004c <tiuhw_set_default_hwparam>:
      4c:	24020001 	li	$v0,1
      50:	ac820058 	sw	$v0,88($a0)
      54:	ac82005c 	sw	$v0,92($a0)
      58:	3c030000 	lui	$v1,0x0
      5c:	24630000 	addiu	$v1,$v1,0
      60:	8c620000 	lw	$v0,0($v1)
      64:	ac820060 	sw	$v0,96($a0)
      68:	24020030 	li	$v0,48
      6c:	8c630004 	lw	$v1,4($v1)
      70:	a0820068 	sb	$v0,104($a0)
      74:	24020017 	li	$v0,23
      78:	a482006a 	sh	$v0,106($a0)
      7c:	ac830064 	sw	$v1,100($a0)
      80:	3c030000 	lui	$v1,0x0
      84:	24630008 	addiu	$v1,$v1,8
      88:	8c620000 	lw	$v0,0($v1)
      8c:	ac82006c 	sw	$v0,108($a0)
      90:	8c620004 	lw	$v0,4($v1)
      94:	ac820070 	sw	$v0,112($a0)
      98:	8c620000 	lw	$v0,0($v1)
      9c:	ac820074 	sw	$v0,116($a0)
      a0:	8c620004 	lw	$v0,4($v1)
      a4:	ac800080 	sw	$zero,128($a0)
      a8:	ac80007c 	sw	$zero,124($a0)
      ac:	ac800088 	sw	$zero,136($a0)
      b0:	ac800084 	sw	$zero,132($a0)
      b4:	03e00008 	jr	$ra
      b8:	ac820078 	sw	$v0,120($a0)

00000000000000bc <tiuhw_sys_call>:
      bc:	27bdffc8 	addiu	$sp,$sp,-56
      c0:	afbf0034 	sw	$ra,52($sp)
      c4:	afb60030 	sw	$s6,48($sp)
      c8:	afb5002c 	sw	$s5,44($sp)
      cc:	afb40028 	sw	$s4,40($sp)
      d0:	afb30024 	sw	$s3,36($sp)
      d4:	afb20020 	sw	$s2,32($sp)
      d8:	afb1001c 	sw	$s1,28($sp)
      dc:	afb00018 	sw	$s0,24($sp)
      e0:	0000b021 	move	$s6,$zero
      e4:	00809021 	move	$s2,$a0
      e8:	8e420004 	lw	$v0,4($s2)
      ec:	1040000b 	beqz	$v0,11c <tiuhw_sys_call+0x60>
      f0:	0000a021 	move	$s4,$zero
      f4:	94540000 	lhu	$s4,0($v0)
      f8:	3c020000 	lui	$v0,0x0
      fc:	9042006c 	lbu	$v0,108($v0)
     100:	0282102b 	sltu	$v0,$s4,$v0
     104:	54400006 	0x54400006
     108:	8e450000 	lw	$a1,0($s2)
     10c:	ae400008 	sw	$zero,8($s2)
     110:	a240000c 	sb	$zero,12($s2)
     114:	08000248 	j	920 <tiuhw_sys_call+0x864>
     118:	24020001 	li	$v0,1
     11c:	8e450000 	lw	$a1,0($s2)
     120:	2402001c 	li	$v0,28
     124:	10a20011 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     128:	24020012 	li	$v0,18
     12c:	10a2000f 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     130:	2402000b 	li	$v0,11
     134:	10a2000d 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     138:	24020009 	li	$v0,9
     13c:	10a2000b 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     140:	2402000a 	li	$v0,10
     144:	10a20009 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     148:	2402001e 	li	$v0,30
     14c:	10a20007 	beq	$a1,$v0,16c <tiuhw_sys_call+0xb0>
     150:	02803021 	move	$a2,$s4
     154:	3c040000 	lui	$a0,0x0
     158:	24840120 	addiu	$a0,$a0,288
     15c:	3c020000 	lui	$v0,0x0
     160:	24420000 	addiu	$v0,$v0,0
     164:	0040f809 	jalr	$v0
     168:	8e470008 	lw	$a3,8($s2)
     16c:	001410c0 	sll	$v0,$s4,0x3
     170:	00541021 	addu	$v0,$v0,$s4
     174:	00021080 	sll	$v0,$v0,0x2
     178:	00541023 	subu	$v0,$v0,$s4
     17c:	00021080 	sll	$v0,$v0,0x2
     180:	3c030000 	lui	$v1,0x0
     184:	8c630068 	lw	$v1,104($v1)
     188:	8e440000 	lw	$a0,0($s2)
     18c:	00628021 	addu	$s0,$v1,$v0
     190:	2484ffff 	addiu	$a0,$a0,-1
     194:	2c82001f 	sltiu	$v0,$a0,31
     198:	104001df 	beqz	$v0,918 <tiuhw_sys_call+0x85c>
     19c:	8e13003c 	lw	$s3,60($s0)
     1a0:	00041080 	sll	$v0,$a0,0x2
     1a4:	3c010000 	lui	$at,0x0
     1a8:	00220821 	addu	$at,$at,$v0
     1ac:	8c220280 	lw	$v0,640($at)
     1b0:	00400008 	jr	$v0
     1b4:	00000000 	nop
     1b8:	08000247 	j	91c <tiuhw_sys_call+0x860>
     1bc:	a240000c 	sb	$zero,12($s2)
     1c0:	08000247 	j	91c <tiuhw_sys_call+0x860>
     1c4:	ae400008 	sw	$zero,8($s2)
     1c8:	92020002 	lbu	$v0,2($s0)
     1cc:	30420002 	andi	$v0,$v0,0x2
     1d0:	10400006 	beqz	$v0,1ec <tiuhw_sys_call+0x130>
     1d4:	02002021 	move	$a0,$s0
     1d8:	8e62001c 	lw	$v0,28($s3)
     1dc:	0040f809 	jalr	$v0
     1e0:	26450008 	addiu	$a1,$s2,8
     1e4:	08000123 	j	48c <tiuhw_sys_call+0x3d0>
     1e8:	8e420008 	lw	$v0,8($s2)
     1ec:	3c020000 	lui	$v0,0x0
     1f0:	8c42001c 	lw	$v0,28($v0)
     1f4:	08000120 	j	480 <tiuhw_sys_call+0x3c4>
     1f8:	26450008 	addiu	$a1,$s2,8
     1fc:	92020002 	lbu	$v0,2($s0)
     200:	30420002 	andi	$v0,$v0,0x2
     204:	10400003 	beqz	$v0,214 <tiuhw_sys_call+0x158>
     208:	02002021 	move	$a0,$s0
     20c:	0800012e 	j	4b8 <tiuhw_sys_call+0x3fc>
     210:	8e620020 	lw	$v0,32($s3)
     214:	3c020000 	lui	$v0,0x0
     218:	8c420020 	lw	$v0,32($v0)
     21c:	0800012e 	j	4b8 <tiuhw_sys_call+0x3fc>
     220:	00000000 	nop
     224:	92020002 	lbu	$v0,2($s0)
     228:	30420002 	andi	$v0,$v0,0x2
     22c:	10400003 	beqz	$v0,23c <tiuhw_sys_call+0x180>
     230:	02002021 	move	$a0,$s0
     234:	0800012e 	j	4b8 <tiuhw_sys_call+0x3fc>
     238:	8e620028 	lw	$v0,40($s3)
     23c:	3c020000 	lui	$v0,0x0
     240:	8c420028 	lw	$v0,40($v0)
     244:	0800012e 	j	4b8 <tiuhw_sys_call+0x3fc>
     248:	00000000 	nop
     24c:	92020002 	lbu	$v0,2($s0)
     250:	30420002 	andi	$v0,$v0,0x2
     254:	10400006 	beqz	$v0,270 <tiuhw_sys_call+0x1b4>
     258:	02002021 	move	$a0,$s0
     25c:	8e620024 	lw	$v0,36($s3)
     260:	0040f809 	jalr	$v0
     264:	26450008 	addiu	$a1,$s2,8
     268:	080000a1 	j	284 <tiuhw_sys_call+0x1c8>
     26c:	8e420008 	lw	$v0,8($s2)
     270:	3c020000 	lui	$v0,0x0
     274:	8c420024 	lw	$v0,36($v0)
     278:	0040f809 	jalr	$v0
     27c:	26450008 	addiu	$a1,$s2,8
     280:	8e420008 	lw	$v0,8($s2)
     284:	10400003 	beqz	$v0,294 <tiuhw_sys_call+0x1d8>
     288:	24050002 	li	$a1,2
     28c:	08000178 	j	5e0 <tiuhw_sys_call+0x524>
     290:	96040000 	lhu	$a0,0($s0)
     294:	96040000 	lhu	$a0,0($s0)
     298:	08000178 	j	5e0 <tiuhw_sys_call+0x524>
     29c:	24050001 	li	$a1,1
     2a0:	8e020020 	lw	$v0,32($s0)
     2a4:	14400009 	bnez	$v0,2cc <tiuhw_sys_call+0x210>
     2a8:	92020002 	lbu	$v0,2($s0)
     2ac:	30420002 	andi	$v0,$v0,0x2
     2b0:	1040019b 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     2b4:	02c01021 	move	$v0,$s6
     2b8:	8e620000 	lw	$v0,0($s3)
     2bc:	0040f809 	jalr	$v0
     2c0:	02002021 	move	$a0,$s0
     2c4:	ae020020 	sw	$v0,32($s0)
     2c8:	92020002 	lbu	$v0,2($s0)
     2cc:	30420002 	andi	$v0,$v0,0x2
     2d0:	10400192 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     2d4:	02002021 	move	$a0,$s0
     2d8:	8e620010 	lw	$v0,16($s3)
     2dc:	0040f809 	jalr	$v0
     2e0:	00002821 	move	$a1,$zero
     2e4:	08000248 	j	920 <tiuhw_sys_call+0x864>
     2e8:	02c01021 	move	$v0,$s6
     2ec:	92020002 	lbu	$v0,2($s0)
     2f0:	0800014c 	j	530 <tiuhw_sys_call+0x474>
     2f4:	30420002 	andi	$v0,$v0,0x2
     2f8:	ae000014 	sw	$zero,20($s0)
     2fc:	8e020020 	lw	$v0,32($s0)
     300:	14400009 	bnez	$v0,328 <tiuhw_sys_call+0x26c>
     304:	a600002a 	sh	$zero,42($s0)
     308:	92020002 	lbu	$v0,2($s0)
     30c:	30420002 	andi	$v0,$v0,0x2
     310:	10400183 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     314:	02c01021 	move	$v0,$s6
     318:	8e620000 	lw	$v0,0($s3)
     31c:	0040f809 	jalr	$v0
     320:	02002021 	move	$a0,$s0
     324:	ae020020 	sw	$v0,32($s0)
     328:	92020002 	lbu	$v0,2($s0)
     32c:	30420002 	andi	$v0,$v0,0x2
     330:	1040017a 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     334:	02002021 	move	$a0,$s0
     338:	8e62000c 	lw	$v0,12($s3)
     33c:	0040f809 	jalr	$v0
     340:	00002821 	move	$a1,$zero
     344:	104000a4 	beqz	$v0,5d8 <tiuhw_sys_call+0x51c>
     348:	00002821 	move	$a1,$zero
     34c:	08000178 	j	5e0 <tiuhw_sys_call+0x524>
     350:	96040000 	lhu	$a0,0($s0)
     354:	92020002 	lbu	$v0,2($s0)
     358:	30420002 	andi	$v0,$v0,0x2
     35c:	50400006 	0x50400006
     360:	8e020014 	lw	$v0,20($s0)
     364:	8e450008 	lw	$a1,8($s2)
     368:	8e62002c 	lw	$v0,44($s3)
     36c:	0040f809 	jalr	$v0
     370:	02002021 	move	$a0,$s0
     374:	8e020014 	lw	$v0,20($s0)
     378:	8e430008 	lw	$v1,8($s2)
     37c:	5043000e 	0x5043000e
     380:	8e420008 	lw	$v0,8($s2)
     384:	10600007 	beqz	$v1,3a4 <tiuhw_sys_call+0x2e8>
     388:	96040000 	lhu	$a0,0($s0)
     38c:	3c020000 	lui	$v0,0x0
     390:	24420000 	addiu	$v0,$v0,0
     394:	0040f809 	jalr	$v0
     398:	24050002 	li	$a1,2
     39c:	080000ee 	j	3b8 <tiuhw_sys_call+0x2fc>
     3a0:	8e420008 	lw	$v0,8($s2)
     3a4:	3c020000 	lui	$v0,0x0
     3a8:	24420000 	addiu	$v0,$v0,0
     3ac:	0040f809 	jalr	$v0
     3b0:	24050001 	li	$a1,1
     3b4:	8e420008 	lw	$v0,8($s2)
     3b8:	08000247 	j	91c <tiuhw_sys_call+0x860>
     3bc:	ae020014 	sw	$v0,20($s0)
     3c0:	92020002 	lbu	$v0,2($s0)
     3c4:	30420002 	andi	$v0,$v0,0x2
     3c8:	50400006 	0x50400006
     3cc:	8e420008 	lw	$v0,8($s2)
     3d0:	8e450008 	lw	$a1,8($s2)
     3d4:	8e620030 	lw	$v0,48($s3)
     3d8:	0040f809 	jalr	$v0
     3dc:	02002021 	move	$a0,$s0
     3e0:	8e420008 	lw	$v0,8($s2)
     3e4:	08000247 	j	91c <tiuhw_sys_call+0x860>
     3e8:	ae02000c 	sw	$v0,12($s0)
     3ec:	ae400008 	sw	$zero,8($s2)
     3f0:	96020028 	lhu	$v0,40($s0)
     3f4:	1440014a 	bnez	$v0,920 <tiuhw_sys_call+0x864>
     3f8:	02c01021 	move	$v0,$s6
     3fc:	92020002 	lbu	$v0,2($s0)
     400:	30420001 	andi	$v0,$v0,0x1
     404:	10400146 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     408:	02c01021 	move	$v0,$s6
     40c:	8e020010 	lw	$v0,16($s0)
     410:	10400004 	beqz	$v0,424 <tiuhw_sys_call+0x368>
     414:	02002021 	move	$a0,$s0
     418:	8e020004 	lw	$v0,4($s0)
     41c:	08000247 	j	91c <tiuhw_sys_call+0x860>
     420:	ae420008 	sw	$v0,8($s2)
     424:	8e620034 	lw	$v0,52($s3)
     428:	0040f809 	jalr	$v0
     42c:	26450008 	addiu	$a1,$s2,8
     430:	8e460008 	lw	$a2,8($s2)
     434:	8e020004 	lw	$v0,4($s0)
     438:	50c20014 	0x50c20014
     43c:	8e420008 	lw	$v0,8($s2)
     440:	8e05001c 	lw	$a1,28($s0)
     444:	8e62003c 	lw	$v0,60($s3)
     448:	0040f809 	jalr	$v0
     44c:	02002021 	move	$a0,$s0
     450:	8e420008 	lw	$v0,8($s2)
     454:	10400007 	beqz	$v0,474 <tiuhw_sys_call+0x3b8>
     458:	96040000 	lhu	$a0,0($s0)
     45c:	3c020000 	lui	$v0,0x0
     460:	24420000 	addiu	$v0,$v0,0
     464:	0040f809 	jalr	$v0
     468:	24050002 	li	$a1,2
     46c:	08000123 	j	48c <tiuhw_sys_call+0x3d0>
     470:	8e420008 	lw	$v0,8($s2)
     474:	24050001 	li	$a1,1
     478:	3c020000 	lui	$v0,0x0
     47c:	24420000 	addiu	$v0,$v0,0
     480:	0040f809 	jalr	$v0
     484:	00000000 	nop
     488:	8e420008 	lw	$v0,8($s2)
     48c:	08000247 	j	91c <tiuhw_sys_call+0x860>
     490:	ae020004 	sw	$v0,4($s0)
     494:	ae400008 	sw	$zero,8($s2)
     498:	9602002c 	lhu	$v0,44($s0)
     49c:	14400120 	bnez	$v0,920 <tiuhw_sys_call+0x864>
     4a0:	02c01021 	move	$v0,$s6
     4a4:	92020002 	lbu	$v0,2($s0)
     4a8:	30420001 	andi	$v0,$v0,0x1
     4ac:	1040011b 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     4b0:	02002021 	move	$a0,$s0
     4b4:	8e620038 	lw	$v0,56($s3)
     4b8:	0040f809 	jalr	$v0
     4bc:	26450008 	addiu	$a1,$s2,8
     4c0:	08000248 	j	920 <tiuhw_sys_call+0x864>
     4c4:	02c01021 	move	$v0,$s6
     4c8:	ae00000c 	sw	$zero,12($s0)
     4cc:	ae000004 	sw	$zero,4($s0)
     4d0:	ae000008 	sw	$zero,8($s0)
     4d4:	ae000010 	sw	$zero,16($s0)
     4d8:	24020001 	li	$v0,1
     4dc:	8e030020 	lw	$v1,32($s0)
     4e0:	14600009 	bnez	$v1,508 <tiuhw_sys_call+0x44c>
     4e4:	ae02001c 	sw	$v0,28($s0)
     4e8:	92020002 	lbu	$v0,2($s0)
     4ec:	30420001 	andi	$v0,$v0,0x1
     4f0:	1040010b 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     4f4:	02c01021 	move	$v0,$s6
     4f8:	8e620000 	lw	$v0,0($s3)
     4fc:	0040f809 	jalr	$v0
     500:	02002021 	move	$a0,$s0
     504:	ae020020 	sw	$v0,32($s0)
     508:	92020002 	lbu	$v0,2($s0)
     50c:	30420001 	andi	$v0,$v0,0x1
     510:	10400102 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     514:	02002021 	move	$a0,$s0
     518:	0800014f 	j	53c <tiuhw_sys_call+0x480>
     51c:	8e620010 	lw	$v0,16($s3)
     520:	24030001 	li	$v1,1
     524:	92020002 	lbu	$v0,2($s0)
     528:	ae030010 	sw	$v1,16($s0)
     52c:	30420001 	andi	$v0,$v0,0x1
     530:	104000fa 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     534:	02002021 	move	$a0,$s0
     538:	8e620014 	lw	$v0,20($s3)
     53c:	0040f809 	jalr	$v0
     540:	24050001 	li	$a1,1
     544:	08000248 	j	920 <tiuhw_sys_call+0x864>
     548:	02c01021 	move	$v0,$s6
     54c:	ae000004 	sw	$zero,4($s0)
     550:	ae000008 	sw	$zero,8($s0)
     554:	ae000010 	sw	$zero,16($s0)
     558:	ae00001c 	sw	$zero,28($s0)
     55c:	24020001 	li	$v0,1
     560:	8e030020 	lw	$v1,32($s0)
     564:	14600011 	bnez	$v1,5ac <tiuhw_sys_call+0x4f0>
     568:	ae02000c 	sw	$v0,12($s0)
     56c:	92020002 	lbu	$v0,2($s0)
     570:	30420001 	andi	$v0,$v0,0x1
     574:	104000ea 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     578:	02c01021 	move	$v0,$s6
     57c:	8e02005c 	lw	$v0,92($s0)
     580:	54400007 	0x54400007
     584:	8e620000 	lw	$v0,0($s3)
     588:	02002021 	move	$a0,$s0
     58c:	3c020000 	lui	$v0,0x0
     590:	2442004c 	addiu	$v0,$v0,76
     594:	0040f809 	jalr	$v0
     598:	24050001 	li	$a1,1
     59c:	8e620000 	lw	$v0,0($s3)
     5a0:	0040f809 	jalr	$v0
     5a4:	02002021 	move	$a0,$s0
     5a8:	ae020020 	sw	$v0,32($s0)
     5ac:	92020002 	lbu	$v0,2($s0)
     5b0:	30420001 	andi	$v0,$v0,0x1
     5b4:	104000d9 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     5b8:	02002021 	move	$a0,$s0
     5bc:	8e62000c 	lw	$v0,12($s3)
     5c0:	0040f809 	jalr	$v0
     5c4:	00002821 	move	$a1,$zero
     5c8:	10400003 	beqz	$v0,5d8 <tiuhw_sys_call+0x51c>
     5cc:	00002821 	move	$a1,$zero
     5d0:	08000178 	j	5e0 <tiuhw_sys_call+0x524>
     5d4:	96040000 	lhu	$a0,0($s0)
     5d8:	96040000 	lhu	$a0,0($s0)
     5dc:	24050003 	li	$a1,3
     5e0:	3c020000 	lui	$v0,0x0
     5e4:	24420000 	addiu	$v0,$v0,0
     5e8:	0040f809 	jalr	$v0
     5ec:	00000000 	nop
     5f0:	08000248 	j	920 <tiuhw_sys_call+0x864>
     5f4:	02c01021 	move	$v0,$s6
     5f8:	92020002 	lbu	$v0,2($s0)
     5fc:	30420001 	andi	$v0,$v0,0x1
     600:	10400003 	beqz	$v0,610 <tiuhw_sys_call+0x554>
     604:	02002021 	move	$a0,$s0
     608:	8e420008 	lw	$v0,8($s2)
     60c:	ae020010 	sw	$v0,16($s0)
     610:	8e450008 	lw	$a1,8($s2)
     614:	8e62003c 	lw	$v0,60($s3)
     618:	0040f809 	jalr	$v0
     61c:	00003021 	move	$a2,$zero
     620:	08000248 	j	920 <tiuhw_sys_call+0x864>
     624:	02c01021 	move	$v0,$s6
     628:	92020002 	lbu	$v0,2($s0)
     62c:	30420001 	andi	$v0,$v0,0x1
     630:	104000bb 	beqz	$v0,920 <tiuhw_sys_call+0x864>
     634:	02c01021 	move	$v0,$s6
     638:	8e450008 	lw	$a1,8($s2)
     63c:	080001b3 	j	6cc <tiuhw_sys_call+0x610>
     640:	8e620040 	lw	$v0,64($s3)
     644:	92020002 	lbu	$v0,2($s0)
     648:	30420001 	andi	$v0,$v0,0x1
     64c:	50400006 	0x50400006
     650:	8e020008 	lw	$v0,8($s0)
     654:	8e450008 	lw	$a1,8($s2)
     658:	8e620044 	lw	$v0,68($s3)
     65c:	0040f809 	jalr	$v0
     660:	02002021 	move	$a0,$s0
     664:	8e020008 	lw	$v0,8($s0)
     668:	8e430008 	lw	$v1,8($s2)
     66c:	5043000e 	0x5043000e
     670:	8e420008 	lw	$v0,8($s2)
     674:	10600007 	beqz	$v1,694 <tiuhw_sys_call+0x5d8>
     678:	96040000 	lhu	$a0,0($s0)
     67c:	3c020000 	lui	$v0,0x0
     680:	24420000 	addiu	$v0,$v0,0
     684:	0040f809 	jalr	$v0
     688:	24050002 	li	$a1,2
     68c:	080001aa 	j	6a8 <tiuhw_sys_call+0x5ec>
     690:	8e420008 	lw	$v0,8($s2)
     694:	3c020000 	lui	$v0,0x0
     698:	24420000 	addiu	$v0,$v0,0
     69c:	0040f809 	jalr	$v0
     6a0:	24050001 	li	$a1,1
     6a4:	8e420008 	lw	$v0,8($s2)
     6a8:	08000247 	j	91c <tiuhw_sys_call+0x860>
     6ac:	ae020008 	sw	$v0,8($s0)
     6b0:	8e430008 	lw	$v1,8($s2)
     6b4:	92020002 	lbu	$v0,2($s0)
     6b8:	30420001 	andi	$v0,$v0,0x1
     6bc:	10400097 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     6c0:	ae03000c 	sw	$v1,12($s0)
     6c4:	8e450008 	lw	$a1,8($s2)
     6c8:	8e620048 	lw	$v0,72($s3)
     6cc:	0040f809 	jalr	$v0
     6d0:	02002021 	move	$a0,$s0
     6d4:	08000248 	j	920 <tiuhw_sys_call+0x864>
     6d8:	02c01021 	move	$v0,$s6
     6dc:	3c020000 	lui	$v0,0x0
     6e0:	24420000 	addiu	$v0,$v0,0
     6e4:	0040f809 	jalr	$v0
     6e8:	00000000 	nop
     6ec:	24030001 	li	$v1,1
     6f0:	14430003 	bne	$v0,$v1,700 <tiuhw_sys_call+0x644>
     6f4:	32820001 	andi	$v0,$s4,0x1
     6f8:	080001c1 	j	704 <tiuhw_sys_call+0x648>
     6fc:	26820001 	addiu	$v0,$s4,1
     700:	24420001 	addiu	$v0,$v0,1
     704:	3045ffff 	andi	$a1,$v0,0xffff
     708:	8e02005c 	lw	$v0,92($s0)
     70c:	14400006 	bnez	$v0,728 <tiuhw_sys_call+0x66c>
     710:	a605002e 	sh	$a1,46($s0)
     714:	02002021 	move	$a0,$s0
     718:	3c020000 	lui	$v0,0x0
     71c:	2442004c 	addiu	$v0,$v0,76
     720:	0040f809 	jalr	$v0
     724:	24050001 	li	$a1,1
     728:	8e620000 	lw	$v0,0($s3)
     72c:	0040f809 	jalr	$v0
     730:	02002021 	move	$a0,$s0
     734:	08000247 	j	91c <tiuhw_sys_call+0x860>
     738:	ae020020 	sw	$v0,32($s0)
     73c:	8e620004 	lw	$v0,4($s3)
     740:	0040f809 	jalr	$v0
     744:	02002021 	move	$a0,$s0
     748:	9602002c 	lhu	$v0,44($s0)
     74c:	10400002 	beqz	$v0,758 <tiuhw_sys_call+0x69c>
     750:	2442ffff 	addiu	$v0,$v0,-1
     754:	a602002c 	sh	$v0,44($s0)
     758:	9602002a 	lhu	$v0,42($s0)
     75c:	1040006f 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     760:	2442ffff 	addiu	$v0,$v0,-1
     764:	08000247 	j	91c <tiuhw_sys_call+0x860>
     768:	a602002a 	sh	$v0,42($s0)
     76c:	3c040000 	lui	$a0,0x0
     770:	24840150 	addiu	$a0,$a0,336
     774:	02802821 	move	$a1,$s4
     778:	3c150000 	lui	$s5,0x0
     77c:	26b50000 	addiu	$s5,$s5,0
     780:	02a0f809 	jalr	$s5
     784:	8e460010 	lw	$a2,16($s2)
     788:	8e430010 	lw	$v1,16($s2)
     78c:	28622713 	slti	$v0,$v1,10003
     790:	10400009 	beqz	$v0,7b8 <tiuhw_sys_call+0x6fc>
     794:	28622711 	slti	$v0,$v1,10001
     798:	10400044 	beqz	$v0,8ac <tiuhw_sys_call+0x7f0>
     79c:	24020003 	li	$v0,3
     7a0:	1062005d 	beq	$v1,$v0,918 <tiuhw_sys_call+0x85c>
     7a4:	24020004 	li	$v0,4
     7a8:	10620013 	beq	$v1,$v0,7f8 <tiuhw_sys_call+0x73c>
     7ac:	02c01021 	move	$v0,$s6
     7b0:	08000249 	j	924 <tiuhw_sys_call+0x868>
     7b4:	8fbf0034 	lw	$ra,52($sp)
     7b8:	24022715 	li	$v0,10005
     7bc:	10620023 	beq	$v1,$v0,84c <tiuhw_sys_call+0x790>
     7c0:	28622716 	slti	$v0,$v1,10006
     7c4:	10400005 	beqz	$v0,7dc <tiuhw_sys_call+0x720>
     7c8:	24022714 	li	$v0,10004
     7cc:	10620010 	beq	$v1,$v0,810 <tiuhw_sys_call+0x754>
     7d0:	02c01021 	move	$v0,$s6
     7d4:	08000249 	j	924 <tiuhw_sys_call+0x868>
     7d8:	8fbf0034 	lw	$ra,52($sp)
     7dc:	2862271e 	slti	$v0,$v1,10014
     7e0:	1040004e 	beqz	$v0,91c <tiuhw_sys_call+0x860>
     7e4:	2862271a 	slti	$v0,$v1,10010
     7e8:	1440004d 	bnez	$v0,920 <tiuhw_sys_call+0x864>
     7ec:	02c01021 	move	$v0,$s6
     7f0:	0800022c 	j	8b0 <tiuhw_sys_call+0x7f4>
     7f4:	8e450010 	lw	$a1,16($s2)
     7f8:	3c020000 	lui	$v0,0x0
     7fc:	24420000 	addiu	$v0,$v0,0
     800:	0040f809 	jalr	$v0
     804:	00000000 	nop
     808:	08000247 	j	91c <tiuhw_sys_call+0x860>
     80c:	0040b021 	move	$s6,$v0
     810:	8e430018 	lw	$v1,24($s2)
     814:	10600043 	beqz	$v1,924 <tiuhw_sys_call+0x868>
     818:	8fbf0034 	lw	$ra,52($sp)
     81c:	8c62000c 	lw	$v0,12($v1)
     820:	afa20010 	sw	$v0,16($sp)
     824:	8c620008 	lw	$v0,8($v1)
     828:	3c040000 	lui	$a0,0x0
     82c:	24840178 	addiu	$a0,$a0,376
     830:	afa20014 	sw	$v0,20($sp)
     834:	8c660000 	lw	$a2,0($v1)
     838:	8c670004 	lw	$a3,4($v1)
     83c:	02a0f809 	jalr	$s5
     840:	02802821 	move	$a1,$s4
     844:	0800022c 	j	8b0 <tiuhw_sys_call+0x7f4>
     848:	8e450010 	lw	$a1,16($s2)
     84c:	8e510018 	lw	$s1,24($s2)
     850:	12200032 	beqz	$s1,91c <tiuhw_sys_call+0x860>
     854:	24020001 	li	$v0,1
     858:	8e230000 	lw	$v1,0($s1)
     85c:	54620014 	0x54620014
     860:	8e450010 	lw	$a1,16($s2)
     864:	3c040000 	lui	$a0,0x0
     868:	248401dc 	addiu	$a0,$a0,476
     86c:	02802821 	move	$a1,$s4
     870:	92260010 	lbu	$a2,16($s1)
     874:	8e220004 	lw	$v0,4($s1)
     878:	96270012 	lhu	$a3,18($s1)
     87c:	02a0f809 	jalr	$s5
     880:	afa20010 	sw	$v0,16($sp)
     884:	9222000c 	lbu	$v0,12($s1)
     888:	9227000d 	lbu	$a3,13($s1)
     88c:	afa20010 	sw	$v0,16($sp)
     890:	9222000e 	lbu	$v0,14($s1)
     894:	3c040000 	lui	$a0,0x0
     898:	2484022c 	addiu	$a0,$a0,556
     89c:	afa20014 	sw	$v0,20($sp)
     8a0:	8e260008 	lw	$a2,8($s1)
     8a4:	02a0f809 	jalr	$s5
     8a8:	02802821 	move	$a1,$s4
     8ac:	8e450010 	lw	$a1,16($s2)
     8b0:	8e460018 	lw	$a2,24($s2)
     8b4:	8e620008 	lw	$v0,8($s3)
     8b8:	0040f809 	jalr	$v0
     8bc:	02002021 	move	$a0,$s0
     8c0:	08000248 	j	920 <tiuhw_sys_call+0x864>
     8c4:	02c01021 	move	$v0,$s6
     8c8:	3c030000 	lui	$v1,0x0
     8cc:	24630064 	addiu	$v1,$v1,100
     8d0:	8e440010 	lw	$a0,16($s2)
     8d4:	8c620000 	lw	$v0,0($v1)
     8d8:	ac820000 	sw	$v0,0($a0)
     8dc:	8c620004 	lw	$v0,4($v1)
     8e0:	ac820004 	sw	$v0,4($a0)
     8e4:	8c620008 	lw	$v0,8($v1)
     8e8:	ac820008 	sw	$v0,8($a0)
     8ec:	8c62000c 	lw	$v0,12($v1)
     8f0:	ac82000c 	sw	$v0,12($a0)
     8f4:	8c620010 	lw	$v0,16($v1)
     8f8:	ac820010 	sw	$v0,16($a0)
     8fc:	8c620014 	lw	$v0,20($v1)
     900:	ac820014 	sw	$v0,20($a0)
     904:	8c620018 	lw	$v0,24($v1)
     908:	ac820018 	sw	$v0,24($a0)
     90c:	8c62001c 	lw	$v0,28($v1)
     910:	08000247 	j	91c <tiuhw_sys_call+0x860>
     914:	ac82001c 	sw	$v0,28($a0)
     918:	24160001 	li	$s6,1
     91c:	02c01021 	move	$v0,$s6
     920:	8fbf0034 	lw	$ra,52($sp)
     924:	8fb60030 	lw	$s6,48($sp)
     928:	8fb5002c 	lw	$s5,44($sp)
     92c:	8fb40028 	lw	$s4,40($sp)
     930:	8fb30024 	lw	$s3,36($sp)
     934:	8fb20020 	lw	$s2,32($sp)
     938:	8fb1001c 	lw	$s1,28($sp)
     93c:	8fb00018 	lw	$s0,24($sp)
     940:	03e00008 	jr	$ra
     944:	27bd0038 	addiu	$sp,$sp,56
*/
void somefunction()
{
/*
     948:	27bdffd0 	addiu	$sp,$sp,-48
     94c:	afbf002c 	sw	$ra,44($sp)
     950:	afb60028 	sw	$s6,40($sp)
     954:	afb50024 	sw	$s5,36($sp)
     958:	afb40020 	sw	$s4,32($sp)
     95c:	afb3001c 	sw	$s3,28($sp)
     960:	afb20018 	sw	$s2,24($sp)
     964:	afb10014 	sw	$s1,20($sp)
     968:	afb00010 	sw	$s0,16($sp)
     96c:	00809821 	move	$s3,$a0
     970:	00002821 	move	$a1,$zero
     974:	3c150000 	lui	$s5,0x0
     978:	26b50000 	addiu	$s5,$s5,0
     97c:	02a0f809 	jalr	$s5
     980:	24060020 	li	$a2,32
     984:	24100002 	li	$s0,2
     988:	a6700010 	sh	$s0,16($s3)
     98c:	3c110000 	lui	$s1,0x0
     990:	263102fc 	addiu	$s1,$s1,764
     994:	3c120000 	lui	$s2,0x0
     998:	26520000 	addiu	$s2,$s2,0
     99c:	0240f809 	jalr	$s2
     9a0:	02202021 	move	$a0,$s1
     9a4:	00402821 	move	$a1,$v0
     9a8:	10a0003b 	beqz	$a1,a98 <tiuhw_sys_call+0x9dc>
     9ac:	00a02021 	move	$a0,$a1
     9b0:	3c020000 	lui	$v0,0x0
     9b4:	2442030c 	addiu	$v0,$v0,780
     9b8:	00401821 	move	$v1,$v0
     9bc:	90820000 	lbu	$v0,0($a0)
     9c0:	90610000 	lbu	$at,0($v1)
     9c4:	24840001 	addiu	$a0,$a0,1
     9c8:	14220004 	bne	$at,$v0,9dc <tiuhw_sys_call+0x920>
     9cc:	24630001 	addiu	$v1,$v1,1
     9d0:	1440fffb 	bnez	$v0,9c0 <tiuhw_sys_call+0x904>
     9d4:	90820000 	lbu	$v0,0($a0)
     9d8:	00201021 	move	$v0,$at
     9dc:	00411023 	subu	$v0,$v0,$at
     9e0:	14400005 	bnez	$v0,9f8 <tiuhw_sys_call+0x93c>
     9e4:	00a02021 	move	$a0,$a1
     9e8:	a6700010 	sh	$s0,16($s3)
     9ec:	24020001 	li	$v0,1
     9f0:	080002a6 	j	a98 <tiuhw_sys_call+0x9dc>
     9f4:	a6620012 	sh	$v0,18($s3)
     9f8:	3c020000 	lui	$v0,0x0
     9fc:	24420314 	addiu	$v0,$v0,788
     a00:	00401821 	move	$v1,$v0
     a04:	90820000 	lbu	$v0,0($a0)
     a08:	90610000 	lbu	$at,0($v1)
     a0c:	24840001 	addiu	$a0,$a0,1
     a10:	14220004 	bne	$at,$v0,a24 <tiuhw_sys_call+0x968>
     a14:	24630001 	addiu	$v1,$v1,1
     a18:	1440fffb 	bnez	$v0,a08 <tiuhw_sys_call+0x94c>
     a1c:	90820000 	lbu	$v0,0($a0)
     a20:	00201021 	move	$v0,$at
     a24:	00411023 	subu	$v0,$v0,$at
     a28:	14400004 	bnez	$v0,a3c <tiuhw_sys_call+0x980>
     a2c:	24020001 	li	$v0,1
     a30:	a6600012 	sh	$zero,18($s3)
     a34:	080002a6 	j	a98 <tiuhw_sys_call+0x9dc>
     a38:	a6620010 	sh	$v0,16($s3)
     a3c:	3c020000 	lui	$v0,0x0
     a40:	2442031c 	addiu	$v0,$v0,796
     a44:	00401821 	move	$v1,$v0
     a48:	90a20000 	lbu	$v0,0($a1)
     a4c:	90610000 	lbu	$at,0($v1)
     a50:	24a50001 	addiu	$a1,$a1,1
     a54:	14220004 	bne	$at,$v0,a68 <tiuhw_sys_call+0x9ac>
     a58:	24630001 	addiu	$v1,$v1,1
     a5c:	1440fffb 	bnez	$v0,a4c <tiuhw_sys_call+0x990>
     a60:	90a20000 	lbu	$v0,0($a1)
     a64:	00201021 	move	$v0,$at
     a68:	00411023 	subu	$v0,$v0,$at
     a6c:	14400004 	bnez	$v0,a80 <tiuhw_sys_call+0x9c4>
     a70:	00000000 	nop
     a74:	a6600012 	sh	$zero,18($s3)
     a78:	080002a6 	j	a98 <tiuhw_sys_call+0x9dc>
     a7c:	a6600010 	sh	$zero,16($s3)
     a80:	3c040000 	lui	$a0,0x0
     a84:	24840324 	addiu	$a0,$a0,804
     a88:	3c020000 	lui	$v0,0x0
     a8c:	24420000 	addiu	$v0,$v0,0
     a90:	0040f809 	jalr	$v0
     a94:	02202821 	move	$a1,$s1
     a98:	24110010 	li	$s1,16
     a9c:	a6710016 	sh	$s1,22($s3)
     aa0:	3c100000 	lui	$s0,0x0
     aa4:	26100340 	addiu	$s0,$s0,832
     aa8:	0240f809 	jalr	$s2
     aac:	02002021 	move	$a0,$s0
     ab0:	00402821 	move	$a1,$v0
     ab4:	10a00012 	beqz	$a1,b00 <tiuhw_sys_call+0xa44>
     ab8:	00a02021 	move	$a0,$a1
     abc:	00002821 	move	$a1,$zero
     ac0:	3c020000 	lui	$v0,0x0
     ac4:	24420000 	addiu	$v0,$v0,0
     ac8:	0040f809 	jalr	$v0
     acc:	2406000a 	li	$a2,10
     ad0:	a6620016 	sh	$v0,22($s3)
     ad4:	3042ffff 	andi	$v0,$v0,0xffff
     ad8:	2c420081 	sltiu	$v0,$v0,129
     adc:	54400009 	0x54400009
     ae0:	24100001 	li	$s0,1
     ae4:	3c040000 	lui	$a0,0x0
     ae8:	24840348 	addiu	$a0,$a0,840
     aec:	3c020000 	lui	$v0,0x0
     af0:	24420000 	addiu	$v0,$v0,0
     af4:	0040f809 	jalr	$v0
     af8:	02002821 	move	$a1,$s0
     afc:	a6710016 	sh	$s1,22($s3)
     b00:	24100001 	li	$s0,1
     b04:	a6700014 	sh	$s0,20($s3)
     b08:	3c040000 	lui	$a0,0x0
     b0c:	24840360 	addiu	$a0,$a0,864
     b10:	0240f809 	jalr	$s2
     b14:	a2700009 	sb	$s0,9($s3)
     b18:	00402821 	move	$a1,$v0
     b1c:	50a0000d 	0x50a0000d
     b20:	240501f2 	li	$a1,498
     b24:	00a02021 	move	$a0,$a1
     b28:	00002821 	move	$a1,$zero
     b2c:	3c020000 	lui	$v0,0x0
     b30:	24420000 	addiu	$v0,$v0,0
     b34:	0040f809 	jalr	$v0
     b38:	2406000a 	li	$a2,10
     b3c:	a2620009 	sb	$v0,9($s3)
     b40:	304200ff 	andi	$v0,$v0,0xff
     b44:	2c420002 	sltiu	$v0,$v0,2
     b48:	50400001 	0x50400001
     b4c:	a2700009 	sb	$s0,9($s3)
     b50:	240501f2 	li	$a1,498
     b54:	92640009 	lbu	$a0,9($s3)
     b58:	2406079f 	li	$a2,1951
     b5c:	3c110000 	lui	$s1,0x0
     b60:	26310000 	addiu	$s1,$s1,0
     b64:	0220f809 	jalr	$s1
     b68:	00042080 	sll	$a0,$a0,0x2
     b6c:	1040000d 	beqz	$v0,ba4 <tiuhw_sys_call+0xae8>
     b70:	ae62001c 	sw	$v0,28($s3)
     b74:	00402021 	move	$a0,$v0
     b78:	92660009 	lbu	$a2,9($s3)
     b7c:	00002821 	move	$a1,$zero
     b80:	02a0f809 	jalr	$s5
     b84:	00063100 	sll	$a2,$a2,0x4
     b88:	240501f2 	li	$a1,498
     b8c:	92640009 	lbu	$a0,9($s3)
     b90:	2406079f 	li	$a2,1951
     b94:	0220f809 	jalr	$s1
     b98:	00042080 	sll	$a0,$a0,0x2
     b9c:	14400003 	bnez	$v0,bac <tiuhw_sys_call+0xaf0>
     ba0:	ae620018 	sw	$v0,24($s3)
     ba4:	08000441 	j	1104 <tiuhw_sys_call+0x1048>
     ba8:	00001021 	move	$v0,$zero
     bac:	92620009 	lbu	$v0,9($s3)
     bb0:	1040004e 	beqz	$v0,cec <tiuhw_sys_call+0xc30>
     bb4:	0000a021 	move	$s4,$zero
     bb8:	3c020000 	lui	$v0,0x0
     bbc:	24420000 	addiu	$v0,$v0,0
     bc0:	0040f809 	jalr	$v0
     bc4:	02802021 	move	$a0,$s4
     bc8:	00409021 	move	$s2,$v0
     bcc:	24020003 	li	$v0,3
     bd0:	1242001b 	beq	$s2,$v0,c40 <tiuhw_sys_call+0xb84>
     bd4:	2a420004 	slti	$v0,$s2,4
     bd8:	10400005 	beqz	$v0,bf0 <tiuhw_sys_call+0xb34>
     bdc:	24020002 	li	$v0,2
     be0:	1242000a 	beq	$s2,$v0,c0c <tiuhw_sys_call+0xb50>
     be4:	8e62001c 	lw	$v0,28($s3)
     be8:	0800031d 	j	c74 <tiuhw_sys_call+0xbb8>
     bec:	00141880 	sll	$v1,$s4,0x2
     bf0:	24020005 	li	$v0,5
     bf4:	1242000b 	beq	$s2,$v0,c24 <tiuhw_sys_call+0xb68>
     bf8:	2402000c 	li	$v0,12
     bfc:	12420017 	beq	$s2,$v0,c5c <tiuhw_sys_call+0xba0>
     c00:	8e62001c 	lw	$v0,28($s3)
     c04:	0800031d 	j	c74 <tiuhw_sys_call+0xbb8>
     c08:	00141880 	sll	$v1,$s4,0x2
     c0c:	00141880 	sll	$v1,$s4,0x2
     c10:	00621821 	addu	$v1,$v1,$v0
     c14:	3c020000 	lui	$v0,0x0
     c18:	24420000 	addiu	$v0,$v0,0
     c1c:	08000321 	j	c84 <tiuhw_sys_call+0xbc8>
     c20:	ac620000 	sw	$v0,0($v1)
     c24:	8e62001c 	lw	$v0,28($s3)
     c28:	00141880 	sll	$v1,$s4,0x2
     c2c:	00621821 	addu	$v1,$v1,$v0
     c30:	3c020000 	lui	$v0,0x0
     c34:	24420000 	addiu	$v0,$v0,0
     c38:	08000321 	j	c84 <tiuhw_sys_call+0xbc8>
     c3c:	ac620000 	sw	$v0,0($v1)
     c40:	8e62001c 	lw	$v0,28($s3)
     c44:	00141880 	sll	$v1,$s4,0x2
     c48:	00621821 	addu	$v1,$v1,$v0
     c4c:	3c020000 	lui	$v0,0x0
     c50:	24420000 	addiu	$v0,$v0,0
     c54:	08000321 	j	c84 <tiuhw_sys_call+0xbc8>
     c58:	ac620000 	sw	$v0,0($v1)
     c5c:	00141880 	sll	$v1,$s4,0x2
     c60:	00621821 	addu	$v1,$v1,$v0
     c64:	3c020000 	lui	$v0,0x0
     c68:	24420000 	addiu	$v0,$v0,0
     c6c:	08000321 	j	c84 <tiuhw_sys_call+0xbc8>
     c70:	ac620000 	sw	$v0,0($v1)
     c74:	00621821 	addu	$v1,$v1,$v0
     c78:	3c020000 	lui	$v0,0x0
     c7c:	24420000 	addiu	$v0,$v0,0
     c80:	ac620000 	sw	$v0,0($v1)
     c84:	8e62001c 	lw	$v0,28($s3)
     c88:	00148080 	sll	$s0,$s4,0x2
     c8c:	02021021 	addu	$v0,$s0,$v0
     c90:	8c420000 	lw	$v0,0($v0)
     c94:	1040000f 	beqz	$v0,cd4 <tiuhw_sys_call+0xc18>
     c98:	02402021 	move	$a0,$s2
     c9c:	92650008 	lbu	$a1,8($s3)
     ca0:	8e660018 	lw	$a2,24($s3)
     ca4:	8c420000 	lw	$v0,0($v0)
     ca8:	0040f809 	jalr	$v0
     cac:	00d03021 	addu	$a2,$a2,$s0
     cb0:	8e62001c 	lw	$v0,28($s3)
     cb4:	02021021 	addu	$v0,$s0,$v0
     cb8:	8c420000 	lw	$v0,0($v0)
     cbc:	8c420008 	lw	$v0,8($v0)
     cc0:	0040f809 	jalr	$v0
     cc4:	00000000 	nop
     cc8:	92630008 	lbu	$v1,8($s3)
     ccc:	00621821 	addu	$v1,$v1,$v0
     cd0:	a2630008 	sb	$v1,8($s3)
     cd4:	26830001 	addiu	$v1,$s4,1
     cd8:	92620009 	lbu	$v0,9($s3)
     cdc:	307400ff 	andi	$s4,$v1,0xff
     ce0:	0282102b 	sltu	$v0,$s4,$v0
     ce4:	1440ffb4 	bnez	$v0,bb8 <tiuhw_sys_call+0xafc>
     ce8:	00000000 	nop
     cec:	92620008 	lbu	$v0,8($s3)
     cf0:	10400104 	beqz	$v0,1104 <tiuhw_sys_call+0x1048>
     cf4:	00001021 	move	$v0,$zero
     cf8:	3c040000 	lui	$a0,0x0
     cfc:	2484036c 	addiu	$a0,$a0,876
     d00:	3c020000 	lui	$v0,0x0
     d04:	24420000 	addiu	$v0,$v0,0
     d08:	0040f809 	jalr	$v0
     d0c:	92650008 	lbu	$a1,8($s3)
     d10:	240501f2 	li	$a1,498
     d14:	92620008 	lbu	$v0,8($s3)
     d18:	2406079f 	li	$a2,1951
     d1c:	000220c0 	sll	$a0,$v0,0x3
     d20:	00822021 	addu	$a0,$a0,$v0
     d24:	00042080 	sll	$a0,$a0,0x2
     d28:	00822023 	subu	$a0,$a0,$v0
     d2c:	0220f809 	jalr	$s1
     d30:	00042080 	sll	$a0,$a0,0x2
     d34:	1040ff9b 	beqz	$v0,ba4 <tiuhw_sys_call+0xae8>
     d38:	ae620004 	sw	$v0,4($s3)
     d3c:	3c010000 	lui	$at,0x0
     d40:	ac220084 	sw	$v0,132($at)
     d44:	00402021 	move	$a0,$v0
     d48:	92620008 	lbu	$v0,8($s3)
     d4c:	00002821 	move	$a1,$zero
     d50:	000230c0 	sll	$a2,$v0,0x3
     d54:	00c23021 	addu	$a2,$a2,$v0
     d58:	00063080 	sll	$a2,$a2,0x2
     d5c:	00c23023 	subu	$a2,$a2,$v0
     d60:	02a0f809 	jalr	$s5
     d64:	00063080 	sll	$a2,$a2,0x2
     d68:	240501f2 	li	$a1,498
     d6c:	92640008 	lbu	$a0,8($s3)
     d70:	0220f809 	jalr	$s1
     d74:	2406079f 	li	$a2,1951
     d78:	1040ff8a 	beqz	$v0,ba4 <tiuhw_sys_call+0xae8>
     d7c:	ae62000c 	sw	$v0,12($s3)
     d80:	0000a821 	move	$s5,$zero
     d84:	92620009 	lbu	$v0,9($s3)
     d88:	104000dd 	beqz	$v0,1100 <tiuhw_sys_call+0x1044>
     d8c:	0000a021 	move	$s4,$zero
     d90:	3c020000 	lui	$v0,0x0
     d94:	24420000 	addiu	$v0,$v0,0
     d98:	0040f809 	jalr	$v0
     d9c:	02802021 	move	$a0,$s4
     da0:	8e63000c 	lw	$v1,12($s3)
     da4:	00741821 	addu	$v1,$v1,$s4
     da8:	a0750000 	sb	$s5,0($v1)
     dac:	8e63001c 	lw	$v1,28($s3)
     db0:	0014b080 	sll	$s6,$s4,0x2
     db4:	02c31821 	addu	$v1,$s6,$v1
     db8:	8c630000 	lw	$v1,0($v1)
     dbc:	00409021 	move	$s2,$v0
     dc0:	8c620008 	lw	$v0,8($v1)
     dc4:	0040f809 	jalr	$v0
     dc8:	02a08821 	move	$s1,$s5
     dcc:	02221021 	addu	$v0,$s1,$v0
     dd0:	3055ffff 	andi	$s5,$v0,0xffff
     dd4:	24020003 	li	$v0,3
     dd8:	12420059 	beq	$s2,$v0,f40 <tiuhw_sys_call+0xe84>
     ddc:	2a420004 	slti	$v0,$s2,4
     de0:	10400005 	beqz	$v0,df8 <tiuhw_sys_call+0xd3c>
     de4:	24020002 	li	$v0,2
     de8:	1242000a 	beq	$s2,$v0,e14 <tiuhw_sys_call+0xd58>
     dec:	0235102b 	sltu	$v0,$s1,$s5
     df0:	0800041f 	j	107c <tiuhw_sys_call+0xfc0>
     df4:	00000000 	nop
     df8:	24020005 	li	$v0,5
     dfc:	1242002e 	beq	$s2,$v0,eb8 <tiuhw_sys_call+0xdfc>
     e00:	2402000c 	li	$v0,12
     e04:	12420070 	beq	$s2,$v0,fc8 <tiuhw_sys_call+0xf0c>
     e08:	0235102b 	sltu	$v0,$s1,$s5
     e0c:	0800041f 	j	107c <tiuhw_sys_call+0xfc0>
     e10:	00000000 	nop
     e14:	5040001f 	0x5040001f
     e18:	8e620004 	lw	$v0,4($s3)
     e1c:	001180c0 	sll	$s0,$s1,0x3
     e20:	02118021 	addu	$s0,$s0,$s1
     e24:	00108080 	sll	$s0,$s0,0x2
     e28:	02118023 	subu	$s0,$s0,$s1
     e2c:	00108080 	sll	$s0,$s0,0x2
     e30:	8e640004 	lw	$a0,4($s3)
     e34:	3c020000 	lui	$v0,0x0
     e38:	24420000 	addiu	$v0,$v0,0
     e3c:	0040f809 	jalr	$v0
     e40:	00902021 	addu	$a0,$a0,$s0
     e44:	8e620004 	lw	$v0,4($s3)
     e48:	3c030000 	lui	$v1,0x0
     e4c:	24630000 	addiu	$v1,$v1,0
     e50:	02021021 	addu	$v0,$s0,$v0
     e54:	ac43003c 	sw	$v1,60($v0)
     e58:	8e620004 	lw	$v0,4($s3)
     e5c:	02021021 	addu	$v0,$s0,$v0
     e60:	a4510000 	sh	$s1,0($v0)
     e64:	8e620004 	lw	$v0,4($s3)
     e68:	24030001 	li	$v1,1
     e6c:	02021021 	addu	$v0,$s0,$v0
     e70:	a0430002 	sb	$v1,2($v0)
     e74:	26220001 	addiu	$v0,$s1,1
     e78:	3051ffff 	andi	$s1,$v0,0xffff
     e7c:	8e630004 	lw	$v1,4($s3)
     e80:	0235102b 	sltu	$v0,$s1,$s5
     e84:	02038021 	addu	$s0,$s0,$v1
     e88:	1440ffe4 	bnez	$v0,e1c <tiuhw_sys_call+0xd60>
     e8c:	a2140003 	sb	$s4,3($s0)
     e90:	8e620004 	lw	$v0,4($s3)
     e94:	001118c0 	sll	$v1,$s1,0x3
     e98:	00711821 	addu	$v1,$v1,$s1
     e9c:	00031880 	sll	$v1,$v1,0x2
     ea0:	00711823 	subu	$v1,$v1,$s1
     ea4:	00031880 	sll	$v1,$v1,0x2
     ea8:	00621821 	addu	$v1,$v1,$v0
     eac:	24020001 	li	$v0,1
     eb0:	0800043a 	j	10e8 <tiuhw_sys_call+0x102c>
     eb4:	ac62ffb4 	sw	$v0,-76($v1)
     eb8:	0235102b 	sltu	$v0,$s1,$s5
     ebc:	1040008b 	beqz	$v0,10ec <tiuhw_sys_call+0x1030>
     ec0:	26830001 	addiu	$v1,$s4,1
     ec4:	001180c0 	sll	$s0,$s1,0x3
     ec8:	02118021 	addu	$s0,$s0,$s1
     ecc:	00108080 	sll	$s0,$s0,0x2
     ed0:	02118023 	subu	$s0,$s0,$s1
     ed4:	00108080 	sll	$s0,$s0,0x2
     ed8:	8e640004 	lw	$a0,4($s3)
     edc:	3c020000 	lui	$v0,0x0
     ee0:	24420000 	addiu	$v0,$v0,0
     ee4:	0040f809 	jalr	$v0
     ee8:	00902021 	addu	$a0,$a0,$s0
     eec:	8e620004 	lw	$v0,4($s3)
     ef0:	3c030000 	lui	$v1,0x0
     ef4:	24630000 	addiu	$v1,$v1,0
     ef8:	02021021 	addu	$v0,$s0,$v0
     efc:	ac43003c 	sw	$v1,60($v0)
     f00:	8e620004 	lw	$v0,4($s3)
     f04:	02021021 	addu	$v0,$s0,$v0
     f08:	a4510000 	sh	$s1,0($v0)
     f0c:	8e620004 	lw	$v0,4($s3)
     f10:	24030002 	li	$v1,2
     f14:	02021021 	addu	$v0,$s0,$v0
     f18:	a0430002 	sb	$v1,2($v0)
     f1c:	26220001 	addiu	$v0,$s1,1
     f20:	3051ffff 	andi	$s1,$v0,0xffff
     f24:	8e630004 	lw	$v1,4($s3)
     f28:	0235102b 	sltu	$v0,$s1,$s5
     f2c:	02038021 	addu	$s0,$s0,$v1
     f30:	1440ffe4 	bnez	$v0,ec4 <tiuhw_sys_call+0xe08>
     f34:	a2140003 	sb	$s4,3($s0)
     f38:	0800043b 	j	10ec <tiuhw_sys_call+0x1030>
     f3c:	26830001 	addiu	$v1,$s4,1
     f40:	0235102b 	sltu	$v0,$s1,$s5
     f44:	10400069 	beqz	$v0,10ec <tiuhw_sys_call+0x1030>
     f48:	26830001 	addiu	$v1,$s4,1
     f4c:	001180c0 	sll	$s0,$s1,0x3
     f50:	02118021 	addu	$s0,$s0,$s1
     f54:	00108080 	sll	$s0,$s0,0x2
     f58:	02118023 	subu	$s0,$s0,$s1
     f5c:	00108080 	sll	$s0,$s0,0x2
     f60:	8e640004 	lw	$a0,4($s3)
     f64:	3c020000 	lui	$v0,0x0
     f68:	24420000 	addiu	$v0,$v0,0
     f6c:	0040f809 	jalr	$v0
     f70:	00902021 	addu	$a0,$a0,$s0
     f74:	8e620004 	lw	$v0,4($s3)
     f78:	3c030000 	lui	$v1,0x0
     f7c:	24630000 	addiu	$v1,$v1,0
     f80:	02021021 	addu	$v0,$s0,$v0
     f84:	ac43003c 	sw	$v1,60($v0)
     f88:	8e620004 	lw	$v0,4($s3)
     f8c:	02021021 	addu	$v0,$s0,$v0
     f90:	a4510000 	sh	$s1,0($v0)
     f94:	8e620004 	lw	$v0,4($s3)
     f98:	24030002 	li	$v1,2
     f9c:	02021021 	addu	$v0,$s0,$v0
     fa0:	a0430002 	sb	$v1,2($v0)
     fa4:	26220001 	addiu	$v0,$s1,1
     fa8:	3051ffff 	andi	$s1,$v0,0xffff
     fac:	8e630004 	lw	$v1,4($s3)
     fb0:	0235102b 	sltu	$v0,$s1,$s5
     fb4:	02038021 	addu	$s0,$s0,$v1
     fb8:	1440ffe4 	bnez	$v0,f4c <tiuhw_sys_call+0xe90>
     fbc:	a2140003 	sb	$s4,3($s0)
     fc0:	0800043b 	j	10ec <tiuhw_sys_call+0x1030>
     fc4:	26830001 	addiu	$v1,$s4,1
     fc8:	10400047 	beqz	$v0,10e8 <tiuhw_sys_call+0x102c>
     fcc:	00009021 	move	$s2,$zero
     fd0:	001180c0 	sll	$s0,$s1,0x3
     fd4:	02118021 	addu	$s0,$s0,$s1
     fd8:	00108080 	sll	$s0,$s0,0x2
     fdc:	02118023 	subu	$s0,$s0,$s1
     fe0:	00108080 	sll	$s0,$s0,0x2
     fe4:	8e640004 	lw	$a0,4($s3)
     fe8:	3c020000 	lui	$v0,0x0
     fec:	24420000 	addiu	$v0,$v0,0
     ff0:	0040f809 	jalr	$v0
     ff4:	00902021 	addu	$a0,$a0,$s0
     ff8:	3c030000 	lui	$v1,0x0
     ffc:	24630000 	addiu	$v1,$v1,0
    1000:	2e440001 	sltiu	$a0,$s2,1
    1004:	00042100 	sll	$a0,$a0,0x4
    1008:	8e620004 	lw	$v0,4($s3)
    100c:	24840078 	addiu	$a0,$a0,120
    1010:	02021021 	addu	$v0,$s0,$v0
    1014:	ac43003c 	sw	$v1,60($v0)
    1018:	26430001 	addiu	$v1,$s2,1
    101c:	8e620004 	lw	$v0,4($s3)
    1020:	307200ff 	andi	$s2,$v1,0xff
    1024:	02021021 	addu	$v0,$s0,$v0
    1028:	a4510000 	sh	$s1,0($v0)
    102c:	8e620004 	lw	$v0,4($s3)
    1030:	24030001 	li	$v1,1
    1034:	02021021 	addu	$v0,$s0,$v0
    1038:	a0430002 	sb	$v1,2($v0)
    103c:	8e620018 	lw	$v0,24($s3)
    1040:	8e630004 	lw	$v1,4($s3)
    1044:	02c21021 	addu	$v0,$s6,$v0
    1048:	8c420000 	lw	$v0,0($v0)
    104c:	02031821 	addu	$v1,$s0,$v1
    1050:	00441021 	addu	$v0,$v0,$a0
    1054:	ac620044 	sw	$v0,68($v1)
    1058:	26220001 	addiu	$v0,$s1,1
    105c:	3051ffff 	andi	$s1,$v0,0xffff
    1060:	8e630004 	lw	$v1,4($s3)
    1064:	0235102b 	sltu	$v0,$s1,$s5
    1068:	02038021 	addu	$s0,$s0,$v1
    106c:	1440ffd8 	bnez	$v0,fd0 <tiuhw_sys_call+0xf14>
    1070:	a2140003 	sb	$s4,3($s0)
    1074:	0800043b 	j	10ec <tiuhw_sys_call+0x1030>
    1078:	26830001 	addiu	$v1,$s4,1
    107c:	1040001b 	beqz	$v0,10ec <tiuhw_sys_call+0x1030>
    1080:	26830001 	addiu	$v1,$s4,1
    1084:	001180c0 	sll	$s0,$s1,0x3
    1088:	02118021 	addu	$s0,$s0,$s1
    108c:	00108080 	sll	$s0,$s0,0x2
    1090:	02118023 	subu	$s0,$s0,$s1
    1094:	00108080 	sll	$s0,$s0,0x2
    1098:	8e640004 	lw	$a0,4($s3)
    109c:	3c020000 	lui	$v0,0x0
    10a0:	24420000 	addiu	$v0,$v0,0
    10a4:	0040f809 	jalr	$v0
    10a8:	00902021 	addu	$a0,$a0,$s0
    10ac:	8e620004 	lw	$v0,4($s3)
    10b0:	3c030000 	lui	$v1,0x0
    10b4:	24630000 	addiu	$v1,$v1,0
    10b8:	02021021 	addu	$v0,$s0,$v0
    10bc:	ac43003c 	sw	$v1,60($v0)
    10c0:	8e620004 	lw	$v0,4($s3)
    10c4:	02021021 	addu	$v0,$s0,$v0
    10c8:	a4510000 	sh	$s1,0($v0)
    10cc:	26220001 	addiu	$v0,$s1,1
    10d0:	3051ffff 	andi	$s1,$v0,0xffff
    10d4:	8e630004 	lw	$v1,4($s3)
    10d8:	0235102b 	sltu	$v0,$s1,$s5
    10dc:	02038021 	addu	$s0,$s0,$v1
    10e0:	1440ffe8 	bnez	$v0,1084 <tiuhw_sys_call+0xfc8>
    10e4:	a2140003 	sb	$s4,3($s0)
    10e8:	26830001 	addiu	$v1,$s4,1
    10ec:	92620009 	lbu	$v0,9($s3)
    10f0:	307400ff 	andi	$s4,$v1,0xff
    10f4:	0282102b 	sltu	$v0,$s4,$v0
    10f8:	1440ff25 	bnez	$v0,d90 <tiuhw_sys_call+0xcd4>
    10fc:	00000000 	nop
    1100:	24020001 	li	$v0,1
    1104:	8fbf002c 	lw	$ra,44($sp)
    1108:	8fb60028 	lw	$s6,40($sp)
    110c:	8fb50024 	lw	$s5,36($sp)
    1110:	8fb40020 	lw	$s4,32($sp)
    1114:	8fb3001c 	lw	$s3,28($sp)
    1118:	8fb20018 	lw	$s2,24($sp)
    111c:	8fb10014 	lw	$s1,20($sp)
    1120:	8fb00010 	lw	$s0,16($sp)
    1124:	03e00008 	jr	$ra
    1128:	27bd0030 	addiu	$sp,$sp,48
*/
}

/*
000000000000112c <tiuhw_vma_open>:
    112c:	24020001 	li	$v0,1
    1130:	3c030000 	lui	$v1,0x0
    1134:	24630010 	addiu	$v1,$v1,16
    1138:	c0640000 	lwc0	$4,0($v1)
    113c:	00822021 	addu	$a0,$a0,$v0
    1140:	e0640000 	swc0	$4,0($v1)
    1144:	1080fffc 	beqz	$a0,1138 <tiuhw_vma_open+0xc>
    1148:	00000000 	nop
    114c:	8c620004 	lw	$v0,4($v1)
    1150:	34420018 	ori	$v0,$v0,0x18
    1154:	03e00008 	jr	$ra
    1158:	ac620004 	sw	$v0,4($v1)

000000000000115c <tiuhw_vma_close>:
    115c:	24020001 	li	$v0,1
    1160:	3c030000 	lui	$v1,0x0
    1164:	24630010 	addiu	$v1,$v1,16
    1168:	c0640000 	lwc0	$4,0($v1)
    116c:	00822023 	subu	$a0,$a0,$v0
    1170:	e0640000 	swc0	$4,0($v1)
    1174:	1080fffc 	beqz	$a0,1168 <tiuhw_vma_close+0xc>
    1178:	00000000 	nop
    117c:	8c620004 	lw	$v0,4($v1)
    1180:	34420008 	ori	$v0,$v0,0x8
    1184:	03e00008 	jr	$ra
    1188:	ac620004 	sw	$v0,4($v1)

000000000000118c <tiuhw_vma_nopage>:
    118c:	3c040000 	lui	$a0,0x0
    1190:	8c840084 	lw	$a0,132($a0)
    1194:	3c030000 	lui	$v1,0x0
    1198:	8c63000c 	lw	$v1,12($v1)
    119c:	00041582 	srl	$v0,$a0,0x16
    11a0:	00021080 	sll	$v0,$v0,0x2
    11a4:	00621821 	addu	$v1,$v1,$v0
    11a8:	00042282 	srl	$a0,$a0,0xa
    11ac:	8c620000 	lw	$v0,0($v1)
    11b0:	30840ffc 	andi	$a0,$a0,0xffc
    11b4:	00441021 	addu	$v0,$v0,$a0
    11b8:	8c430000 	lw	$v1,0($v0)
    11bc:	3c020000 	lui	$v0,0x0
    11c0:	8c420000 	lw	$v0,0($v0)
    11c4:	3c01ec00 	lui	$at,0xec00
    11c8:	00231821 	addu	$v1,$at,$v1
    11cc:	00031b02 	srl	$v1,$v1,0xc
    11d0:	00031980 	sll	$v1,$v1,0x6
    11d4:	00431021 	addu	$v0,$v0,$v1
    11d8:	24030001 	li	$v1,1
    11dc:	c0440014 	lwc0	$4,20($v0)
    11e0:	00832021 	addu	$a0,$a0,$v1
    11e4:	e0440014 	swc0	$4,20($v0)
    11e8:	1080fffc 	beqz	$a0,11dc <tiuhw_vma_nopage+0x50>
    11ec:	00000000 	nop
    11f0:	03e00008 	jr	$ra
    11f4:	00000000 	nop

00000000000011f8 <tiuhw_mmap>:
    11f8:	27bdffe8 	addiu	$sp,$sp,-24
    11fc:	afbf0010 	sw	$ra,16($sp)
    1200:	00803021 	move	$a2,$a0
    1204:	00a02021 	move	$a0,$a1
    1208:	8c830034 	lw	$v1,52($a0)
    120c:	3c020000 	lui	$v0,0x0
    1210:	8c420000 	lw	$v0,0($v0)
    1214:	00031b00 	sll	$v1,$v1,0xc
    1218:	3c018000 	lui	$at,0x8000
    121c:	00221021 	addu	$v0,$at,$v0
    1220:	0062182b 	sltu	$v1,$v1,$v0
    1224:	50600006 	0x50600006
    1228:	8c820014 	lw	$v0,20($a0)
    122c:	8cc20018 	lw	$v0,24($a2)
    1230:	30420010 	andi	$v0,$v0,0x10
    1234:	10400004 	beqz	$v0,1248 <tiuhw_mmap+0x50>
    1238:	00000000 	nop
    123c:	8c820014 	lw	$v0,20($a0)
    1240:	34424000 	ori	$v0,$v0,0x4000
    1244:	ac820014 	sw	$v0,20($a0)
    1248:	3c020000 	lui	$v0,0x0
    124c:	24420010 	addiu	$v0,$v0,16
    1250:	ac820030 	sw	$v0,48($a0)
    1254:	8c820014 	lw	$v0,20($a0)
    1258:	3c030008 	lui	$v1,0x8
    125c:	00431025 	or	$v0,$v0,$v1
    1260:	ac820014 	sw	$v0,20($a0)
    1264:	3c020000 	lui	$v0,0x0
    1268:	2442112c 	addiu	$v0,$v0,4396
    126c:	0040f809 	jalr	$v0
    1270:	00000000 	nop
    1274:	8fbf0010 	lw	$ra,16($sp)
    1278:	00001021 	move	$v0,$zero
    127c:	03e00008 	jr	$ra
    1280:	27bd0018 	addiu	$sp,$sp,24

0000000000001284 <tiuhw_open>:
    1284:	24020001 	li	$v0,1
    1288:	3c030000 	lui	$v1,0x0
    128c:	24630010 	addiu	$v1,$v1,16
    1290:	c0640000 	lwc0	$4,0($v1)
    1294:	00822021 	addu	$a0,$a0,$v0
    1298:	e0640000 	swc0	$4,0($v1)
    129c:	1080fffc 	beqz	$a0,1290 <tiuhw_open+0xc>
    12a0:	00000000 	nop
    12a4:	8c620004 	lw	$v0,4($v1)
    12a8:	34420018 	ori	$v0,$v0,0x18
    12ac:	ac620004 	sw	$v0,4($v1)
    12b0:	03e00008 	jr	$ra
    12b4:	00001021 	move	$v0,$zero

00000000000012b8 <tiuhw_close>:
    12b8:	24020001 	li	$v0,1
    12bc:	3c030000 	lui	$v1,0x0
    12c0:	24630010 	addiu	$v1,$v1,16
    12c4:	c0640000 	lwc0	$4,0($v1)
    12c8:	00822023 	subu	$a0,$a0,$v0
    12cc:	e0640000 	swc0	$4,0($v1)
    12d0:	1080fffc 	beqz	$a0,12c4 <tiuhw_close+0xc>
    12d4:	00000000 	nop
    12d8:	8c620004 	lw	$v0,4($v1)
    12dc:	34420008 	ori	$v0,$v0,0x8
    12e0:	ac620004 	sw	$v0,4($v1)
    12e4:	03e00008 	jr	$ra
    12e8:	00001021 	move	$v0,$zero
    12ec:	27bdffb0 	addiu	$sp,$sp,-80
    12f0:	afbf0048 	sw	$ra,72($sp)
    12f4:	afb50044 	sw	$s5,68($sp)
    12f8:	afb40040 	sw	$s4,64($sp)
    12fc:	afb3003c 	sw	$s3,60($sp)
    1300:	afb20038 	sw	$s2,56($sp)
    1304:	afb10034 	sw	$s1,52($sp)
    1308:	afb00030 	sw	$s0,48($sp)
    130c:	3c050000 	lui	$a1,0x0
    1310:	24a50380 	addiu	$a1,$a1,896
    1314:	3c140000 	lui	$s4,0x0
    1318:	26940000 	addiu	$s4,$s4,0
    131c:	0280f809 	jalr	$s4
    1320:	0080a821 	move	$s5,$a0
    1324:	00408021 	move	$s0,$v0
    1328:	3c130000 	lui	$s3,0x0
    132c:	2673006c 	addiu	$s3,$s3,108
    1330:	92620000 	lbu	$v0,0($s3)
    1334:	10400024 	beqz	$v0,13c8 <tiuhw_close+0x110>
    1338:	00008821 	move	$s1,$zero
    133c:	00009021 	move	$s2,$zero
    1340:	3c040000 	lui	$a0,0x0
    1344:	8c840084 	lw	$a0,132($a0)
    1348:	02442021 	addu	$a0,$s2,$a0
    134c:	8c82003c 	lw	$v0,60($a0)
    1350:	8c42006c 	lw	$v0,108($v0)
    1354:	0040f809 	jalr	$v0
    1358:	26310001 	addiu	$s1,$s1,1
    135c:	3c030000 	lui	$v1,0x0
    1360:	8c630084 	lw	$v1,132($v1)
    1364:	02431821 	addu	$v1,$s2,$v1
    1368:	8c640008 	lw	$a0,8($v1)
    136c:	94660000 	lhu	$a2,0($v1)
    1370:	afa40010 	sw	$a0,16($sp)
    1374:	8c64000c 	lw	$a0,12($v1)
    1378:	afa40014 	sw	$a0,20($sp)
    137c:	8c650010 	lw	$a1,16($v1)
    1380:	afa50018 	sw	$a1,24($sp)
    1384:	8c670014 	lw	$a3,20($v1)
    1388:	2652008c 	addiu	$s2,$s2,140
    138c:	afa7001c 	sw	$a3,28($sp)
    1390:	8c670040 	lw	$a3,64($v1)
    1394:	02b02021 	addu	$a0,$s5,$s0
    1398:	afa70020 	sw	$a3,32($sp)
    139c:	8c670020 	lw	$a3,32($v1)
    13a0:	afa20028 	sw	$v0,40($sp)
    13a4:	afa70024 	sw	$a3,36($sp)
    13a8:	3c050000 	lui	$a1,0x0
    13ac:	24a5039c 	addiu	$a1,$a1,924
    13b0:	0280f809 	jalr	$s4
    13b4:	8c670004 	lw	$a3,4($v1)
    13b8:	92630000 	lbu	$v1,0($s3)
    13bc:	0223182a 	slt	$v1,$s1,$v1
    13c0:	1460ffdf 	bnez	$v1,1340 <tiuhw_close+0x88>
    13c4:	02028021 	addu	$s0,$s0,$v0
    13c8:	24030001 	li	$v1,1
    13cc:	8fa20060 	lw	$v0,96($sp)
    13d0:	8fbf0048 	lw	$ra,72($sp)
    13d4:	8fb50044 	lw	$s5,68($sp)
    13d8:	8fb40040 	lw	$s4,64($sp)
    13dc:	8fb3003c 	lw	$s3,60($sp)
    13e0:	8fb20038 	lw	$s2,56($sp)
    13e4:	8fb10034 	lw	$s1,52($sp)
    13e8:	ac430000 	sw	$v1,0($v0)
    13ec:	02001021 	move	$v0,$s0
    13f0:	8fb00030 	lw	$s0,48($sp)
    13f4:	03e00008 	jr	$ra
    13f8:	27bd0050 	addiu	$sp,$sp,80

00000000000013fc <tiu_if_quit>:
    13fc:	27bdffe0 	addiu	$sp,$sp,-32
    1400:	afbf0018 	sw	$ra,24($sp)
    1404:	afb10014 	sw	$s1,20($sp)
    1408:	afb00010 	sw	$s0,16($sp)
    140c:	00808821 	move	$s1,$a0
    1410:	8e22001c 	lw	$v0,28($s1)
    1414:	50400023 	0x50400023
    1418:	8e240004 	lw	$a0,4($s1)
    141c:	92220009 	lbu	$v0,9($s1)
    1420:	10400018 	beqz	$v0,1484 <tiu_if_quit+0x88>
    1424:	00008021 	move	$s0,$zero
    1428:	3c020000 	lui	$v0,0x0
    142c:	8c420080 	lw	$v0,128($v0)
    1430:	00103080 	sll	$a2,$s0,0x2
    1434:	00c21021 	addu	$v0,$a2,$v0
    1438:	8c420000 	lw	$v0,0($v0)
    143c:	1040000c 	beqz	$v0,1470 <tiu_if_quit+0x74>
    1440:	26030001 	addiu	$v1,$s0,1
    1444:	8e22000c 	lw	$v0,12($s1)
    1448:	8e23001c 	lw	$v1,28($s1)
    144c:	00501021 	addu	$v0,$v0,$s0
    1450:	00c31821 	addu	$v1,$a2,$v1
    1454:	90440000 	lbu	$a0,0($v0)
    1458:	8c620000 	lw	$v0,0($v1)
    145c:	8e250018 	lw	$a1,24($s1)
    1460:	8c420004 	lw	$v0,4($v0)
    1464:	0040f809 	jalr	$v0
    1468:	00a62821 	addu	$a1,$a1,$a2
    146c:	26030001 	addiu	$v1,$s0,1
    1470:	92220009 	lbu	$v0,9($s1)
    1474:	307000ff 	andi	$s0,$v1,0xff
    1478:	0202102b 	sltu	$v0,$s0,$v0
    147c:	1440ffea 	bnez	$v0,1428 <tiu_if_quit+0x2c>
    1480:	00000000 	nop
    1484:	8e24001c 	lw	$a0,28($s1)
    1488:	50800006 	0x50800006
    148c:	8e240004 	lw	$a0,4($s1)
    1490:	3c020000 	lui	$v0,0x0
    1494:	24420000 	addiu	$v0,$v0,0
    1498:	0040f809 	jalr	$v0
    149c:	00000000 	nop
    14a0:	8e240004 	lw	$a0,4($s1)
    14a4:	10800005 	beqz	$a0,14bc <tiu_if_quit+0xc0>
    14a8:	ae20001c 	sw	$zero,28($s1)
    14ac:	3c020000 	lui	$v0,0x0
    14b0:	24420000 	addiu	$v0,$v0,0
    14b4:	0040f809 	jalr	$v0
    14b8:	00000000 	nop
    14bc:	8e24000c 	lw	$a0,12($s1)
    14c0:	10800005 	beqz	$a0,14d8 <tiu_if_quit+0xdc>
    14c4:	ae200004 	sw	$zero,4($s1)
    14c8:	3c020000 	lui	$v0,0x0
    14cc:	24420000 	addiu	$v0,$v0,0
    14d0:	0040f809 	jalr	$v0
    14d4:	00000000 	nop
    14d8:	8e240018 	lw	$a0,24($s1)
    14dc:	10800005 	beqz	$a0,14f4 <tiu_if_quit+0xf8>
    14e0:	ae20000c 	sw	$zero,12($s1)
    14e4:	3c020000 	lui	$v0,0x0
    14e8:	24420000 	addiu	$v0,$v0,0
    14ec:	0040f809 	jalr	$v0
    14f0:	00000000 	nop
    14f4:	ae200018 	sw	$zero,24($s1)
    14f8:	8fbf0018 	lw	$ra,24($sp)
    14fc:	8fb10014 	lw	$s1,20($sp)
    1500:	8fb00010 	lw	$s0,16($sp)
    1504:	03e00008 	jr	$ra
    1508:	27bd0020 	addiu	$sp,$sp,32
*/

static void __exit tiuhw_an_cleanup_module(void)
{
/*
000000000000150c <cleanup_module>:
    150c:	27bdffe0 	addiu	$sp,$sp,-32
    1510:	afbf0018 	sw	$ra,24($sp)
    1514:	afb10014 	sw	$s1,20($sp)
    1518:	afb00010 	sw	$s0,16($sp)
    151c:	3c010000 	lui	$at,0x0
    1520:	ac200004 	sw	$zero,4($at)
    1524:	3c110000 	lui	$s1,0x0
    1528:	263103b8 	addiu	$s1,$s1,952
    152c:	02202021 	move	$a0,$s1
    1530:	3c020000 	lui	$v0,0x0
    1534:	24420000 	addiu	$v0,$v0,0
    1538:	0040f809 	jalr	$v0
    153c:	00002821 	move	$a1,$zero
    1540:	3c040000 	lui	$a0,0x0
    1544:	24840064 	addiu	$a0,$a0,100
    1548:	3c020000 	lui	$v0,0x0
    154c:	244213fc 	addiu	$v0,$v0,5116
    1550:	0040f809 	jalr	$v0
    1554:	00008021 	move	$s0,$zero
    1558:	02002021 	move	$a0,$s0
    155c:	3c020000 	lui	$v0,0x0
    1560:	24420000 	addiu	$v0,$v0,0
    1564:	0040f809 	jalr	$v0
    1568:	24050001 	li	$a1,1
    156c:	26020001 	addiu	$v0,$s0,1
    1570:	305000ff 	andi	$s0,$v0,0xff
    1574:	2e020002 	sltiu	$v0,$s0,2
    1578:	1440fff8 	bnez	$v0,155c <cleanup_module+0x50>
    157c:	02002021 	move	$a0,$s0
    1580:	3c040000 	lui	$a0,0x0
    1584:	8c840000 	lw	$a0,0($a0)
    1588:	3c020000 	lui	$v0,0x0
    158c:	24420000 	addiu	$v0,$v0,0
    1590:	0040f809 	jalr	$v0
    1594:	00000000 	nop
    1598:	240400f0 	li	$a0,240
    159c:	3c020000 	lui	$v0,0x0
    15a0:	24420000 	addiu	$v0,$v0,0
    15a4:	0040f809 	jalr	$v0
    15a8:	02202821 	move	$a1,$s1
    15ac:	3c040000 	lui	$a0,0x0
    15b0:	248403c0 	addiu	$a0,$a0,960
    15b4:	3c020000 	lui	$v0,0x0
    15b8:	24420000 	addiu	$v0,$v0,0
    15bc:	0040f809 	jalr	$v0
    15c0:	00000000 	nop
    15c4:	8fbf0018 	lw	$ra,24($sp)
    15c8:	8fb10014 	lw	$s1,20($sp)
    15cc:	8fb00010 	lw	$s0,16($sp)
    15d0:	03e00008 	jr	$ra
    15d4:	27bd0020 	addiu	$sp,$sp,32
*/
}

static int __init tiuhw_an_init_module(void)
{
	int ret;
/* prologue
00000000000015d8 <init_module>:
    15d8:	27bdffc8 	addiu	$sp,$sp,-56
    15dc:	afbf0030 	sw	$ra,48($sp)
    15e0:	afb3002c 	sw	$s3,44($sp)
    15e4:	afb20028 	sw	$s2,40($sp)
    15e8:	afb10024 	sw	$s1,36($sp)
*/

/*
    15ec:	3c040000 	lui	$a0,0x0
    15f0:	248403dc 	addiu	$a0,$a0,988
    15f4:	3c120000 	lui	$s2,0x0
    15f8:	26520000 	addiu	$s2,$s2,0
    15fc:	0240f809 	jalr	$s2
    1600:	afb00020 	sw	$s0,32($sp)
*/
	printk(KERN_ERR "TIUHW module start loading\n");

/*
    1604:	3c100000 	lui	$s0,0x0
    1608:	26100064 	addiu	$s0,$s0,100
    160c:	3c020000 	lui	$v0,0x0
    1610:	24420948 	addiu	$v0,$v0,2376
    1614:	0040f809 	jalr	$v0
    1618:	02002021 	move	$a0,$s0
*/
	// ret = somefunction();

/*
    161c:	1440000a 	bnez	$v0,1648 <init_module+0x70>
    1620:	240400f0 	li	$a0,240
    1624:	3c020000 	lui	$v0,0x0
    1628:	244213fc 	addiu	$v0,$v0,5116
    162c:	0040f809 	jalr	$v0
    1630:	02002021 	move	$a0,$s0
*/
	if(ret == 0) {
/*
    1634:	3c040000 	lui	$a0,0x0
    1638:	0240f809 	jalr	$s2
    163c:	248403fc 	addiu	$a0,$a0,1020
    1640:	080005dd 	j	1774 <init_module+0x19c>
    1644:	2402fff4 	li	$v0,-12
*/
	} else {

/*
    1648:	3c110000 	lui	$s1,0x0
    164c:	263103b8 	addiu	$s1,$s1,952
    1650:	02202821 	move	$a1,$s1
    1654:	3c130000 	lui	$s3,0x0
    1658:	2673001c 	addiu	$s3,$s3,28
    165c:	3c020000 	lui	$v0,0x0
    1660:	24420000 	addiu	$v0,$v0,0
    1664:	0040f809 	jalr	$v0
    1668:	02603021 	move	$a2,$s3
    166c:	04430008 	0x4430008
    1670:	92020009 	lbu	$v0,9($s0)
*/
	}
/*
    1674:	3c020000 	lui	$v0,0x0
    1678:	244213fc 	addiu	$v0,$v0,5116
    167c:	0040f809 	jalr	$v0
    1680:	02002021 	move	$a0,$s0
    1684:	3c040000 	lui	$a0,0x0
    1688:	080005ac 	j	16b0 <init_module+0xd8>
    168c:	24840428 	addiu	$a0,$a0,1064
    1690:	1440000b 	bnez	$v0,16c0 <init_module+0xe8>
    1694:	02202021 	move	$a0,$s1
    1698:	3c020000 	lui	$v0,0x0
    169c:	244213fc 	addiu	$v0,$v0,5116
    16a0:	0040f809 	jalr	$v0
    16a4:	02002021 	move	$a0,$s0
    16a8:	3c040000 	lui	$a0,0x0
    16ac:	24840448 	addiu	$a0,$a0,1096
    16b0:	0240f809 	jalr	$s2
    16b4:	00000000 	nop
    16b8:	080005dd 	j	1774 <init_module+0x19c>
    16bc:	2402fffb 	li	$v0,-5
    16c0:	00002821 	move	$a1,$zero
    16c4:	3c020000 	lui	$v0,0x0
    16c8:	24420000 	addiu	$v0,$v0,0
    16cc:	0040f809 	jalr	$v0
    16d0:	00003021 	move	$a2,$zero
    16d4:	00401821 	move	$v1,$v0
    16d8:	50600006 	0x50600006
    16dc:	afa00010 	sw	$zero,16($sp)
    16e0:	3c020000 	lui	$v0,0x0
    16e4:	244212ec 	addiu	$v0,$v0,4844
    16e8:	ac62003c 	sw	$v0,60($v1)
    16ec:	ac600038 	sw	$zero,56($v1)
    16f0:	afa00010 	sw	$zero,16($sp)
    16f4:	240221b6 	li	$v0,8630
    16f8:	afa20014 	sw	$v0,20($sp)
    16fc:	afb30018 	sw	$s3,24($sp)
    1700:	afa0001c 	sw	$zero,28($sp)
    1704:	00002021 	move	$a0,$zero
    1708:	02202821 	move	$a1,$s1
    170c:	00003021 	move	$a2,$zero
    1710:	3c020000 	lui	$v0,0x0
    1714:	24420000 	addiu	$v0,$v0,0
    1718:	0040f809 	jalr	$v0
    171c:	240700f0 	li	$a3,240
    1720:	3c010000 	lui	$at,0x0
    1724:	ac220000 	sw	$v0,0($at)
    1728:	3c040000 	lui	$a0,0x0
    172c:	24841978 	addiu	$a0,$a0,6520
    1730:	3c050000 	lui	$a1,0x0
    1734:	24a5193c 	addiu	$a1,$a1,6460
    1738:	3c020000 	lui	$v0,0x0
    173c:	24420000 	addiu	$v0,$v0,0
    1740:	0040f809 	jalr	$v0
    1744:	00000000 	nop
    1748:	3c040000 	lui	$a0,0x0
    174c:	0240f809 	jalr	$s2
    1750:	2484045c 	addiu	$a0,$a0,1116
    1754:	3c020000 	lui	$v0,0x0
    1758:	244200bc 	addiu	$v0,$v0,188
    175c:	3c010000 	lui	$at,0x0
    1760:	ac220004 	sw	$v0,4($at)
    1764:	3c040000 	lui	$a0,0x0
    1768:	0240f809 	jalr	$s2
    176c:	24840480 	addiu	$a0,$a0,1152
    1770:	00001021 	move	$v0,$zero
    1774:	8fbf0030 	lw	$ra,48($sp)
    1778:	8fb3002c 	lw	$s3,44($sp)
    177c:	8fb20028 	lw	$s2,40($sp)
    1780:	8fb10024 	lw	$s1,36($sp)
    1784:	8fb00020 	lw	$s0,32($sp)
    1788:	03e00008 	jr	$ra
    178c:	27bd0038 	addiu	$sp,$sp,56
*/
}

void tiuhw_powerup(void)
{
	int if_type;
	void *apitab[16];
	int ret;

/* prologue
0000000000001790 <tiuhw_powerup>:
    1790:	27bdffd0 	addiu	$sp,$sp,-48
    1794:	afbf0028 	sw	$ra,40($sp)
    1798:	afb10024 	sw	$s1,36($sp)
*/
/*
    179c:	3c020000 	lui	$v0,0x0
    17a0:	24420000 	addiu	$v0,$v0,0
    17a4:	0040f809 	jalr	$v0
    17a8:	afb00020 	sw	$s0,32($sp)
*/
	if_type = hwu_get_tiuhw_if();

/*
    17ac:	24030001 	li	$v1,1
    17b0:	1443002e 	bne	$v0,$v1,186c <tiuhw_powerup+0xdc>
    17b4:	8fbf0028 	lw	$ra,40($sp)
*/

	if(if_type == TIHW_INTERNAL) {
/*
    17b8:	3c040000 	lui	$a0,0x0
    17bc:	2484049c 	addiu	$a0,$a0,1180
    17c0:	3c100000 	lui	$s0,0x0
    17c4:	26100000 	addiu	$s0,$s0,0
    17c8:	0200f809 	jalr	$s0
    17cc:	00000000 	nop
*/
		printk(KERN_ERR "Inside tiuhw_powerup()\n");

/*
    17d0:	27a40010 	addiu	$a0,$sp,16
    17d4:	3c020000 	lui	$v0,0x0
    17d8:	24420000 	addiu	$v0,$v0,0
    17dc:	0040f809 	jalr	$v0
    17e0:	00002821 	move	$a1,$zero
*/
		ret = tiuhw_init_hal(apitab, 0);

/*
    17e4:	14400006 	bnez	$v0,1800 <tiuhw_powerup+0x70>
    17e8:	00008821 	move	$s1,$zero
*/
		if(ret == 0) {

/*
    17ec:	3c040000 	lui	$a0,0x0
    17f0:	0200f809 	jalr	$s0
    17f4:	248404b8 	addiu	$a0,$a0,1208
    17f8:	0800061b 	j	186c <tiuhw_powerup+0xdc>
    17fc:	8fbf0028 	lw	$ra,40($sp)
*/
			printk(KERN_ERR "TIUHW HAL Reset failed\n");
		} else {
/*
    1800:	3c100000 	lui	$s0,0x0
    1804:	8e100068 	lw	$s0,104($s0)
    1808:	8e030020 	lw	$v1,32($s0)
    180c:	24020001 	li	$v0,1
    1810:	14620011 	bne	$v1,$v0,1858 <tiuhw_powerup+0xc8>
    1814:	26220001 	addiu	$v0,$s1,1
    1818:	8e02003c 	lw	$v0,60($s0)
    181c:	02002021 	move	$a0,$s0
    1820:	8c420018 	lw	$v0,24($v0)
    1824:	0040f809 	jalr	$v0
    1828:	24050001 	li	$a1,1
    182c:	8e02001c 	lw	$v0,28($s0)
    1830:	54400008 	0x54400008
    1834:	a600002c 	sh	$zero,44($s0)
    1838:	02002021 	move	$a0,$s0
    183c:	8e02003c 	lw	$v0,60($s0)
    1840:	8e060004 	lw	$a2,4($s0)
    1844:	8c42003c 	lw	$v0,60($v0)
    1848:	0040f809 	jalr	$v0
    184c:	00002821 	move	$a1,$zero
    1850:	a600002c 	sh	$zero,44($s0)
    1854:	26220001 	addiu	$v0,$s1,1
    1858:	305100ff 	andi	$s1,$v0,0xff
    185c:	2e220004 	sltiu	$v0,$s1,4
    1860:	1440ffe9 	bnez	$v0,1808 <tiuhw_powerup+0x78>
    1864:	2610008c 	addiu	$s0,$s0,140
    1868:	8fbf0028 	lw	$ra,40($sp)
*/
		}
	}
/* epilogue
    186c:	8fb10024 	lw	$s1,36($sp)
    1870:	8fb00020 	lw	$s0,32($sp)
    1874:	03e00008 	jr	$ra
    1878:	27bd0030 	addiu	$sp,$sp,48
*/
	return;
}

void tiuhw_powerdown(void)
{
/* prologue
000000000000187c <tiuhw_powerdown>:
    187c:	27bdffe0 	addiu	$sp,$sp,-32
    1880:	afbf001c 	sw	$ra,28($sp)
    1884:	afb20018 	sw	$s2,24($sp)
    1888:	afb10014 	sw	$s1,20($sp)
    188c:	3c020000 	lui	$v0,0x0
    1890:	24420000 	addiu	$v0,$v0,0
    1894:	0040f809 	jalr	$v0
    1898:	afb00010 	sw	$s0,16($sp)
    189c:	24030001 	li	$v1,1
    18a0:	14430021 	bne	$v0,$v1,1928 <tiuhw_powerdown+0xac>
    18a4:	8fbf001c 	lw	$ra,28($sp)
    18a8:	00009021 	move	$s2,$zero
    18ac:	3c100000 	lui	$s0,0x0
    18b0:	8e100068 	lw	$s0,104($s0)
    18b4:	8e110020 	lw	$s1,32($s0)
    18b8:	24020001 	li	$v0,1
    18bc:	16220015 	bne	$s1,$v0,1914 <tiuhw_powerdown+0x98>
    18c0:	26420001 	addiu	$v0,$s2,1
    18c4:	8e02003c 	lw	$v0,60($s0)
    18c8:	02002021 	move	$a0,$s0
    18cc:	8c420018 	lw	$v0,24($v0)
    18d0:	0040f809 	jalr	$v0
    18d4:	00002821 	move	$a1,$zero
    18d8:	8e02001c 	lw	$v0,28($s0)
    18dc:	14400006 	bnez	$v0,18f8 <tiuhw_powerdown+0x7c>
    18e0:	02002021 	move	$a0,$s0
    18e4:	8e02003c 	lw	$v0,60($s0)
    18e8:	8e060004 	lw	$a2,4($s0)
    18ec:	8c42003c 	lw	$v0,60($v0)
    18f0:	0040f809 	jalr	$v0
    18f4:	24050001 	li	$a1,1
    18f8:	3c020000 	lui	$v0,0x0
    18fc:	24420000 	addiu	$v0,$v0,0
    1900:	0040f809 	jalr	$v0
    1904:	02002021 	move	$a0,$s0
    1908:	ae110020 	sw	$s1,32($s0)
    190c:	a611002c 	sh	$s1,44($s0)
    1910:	26420001 	addiu	$v0,$s2,1
    1914:	305200ff 	andi	$s2,$v0,0xff
    1918:	2e420004 	sltiu	$v0,$s2,4
    191c:	1440ffe5 	bnez	$v0,18b4 <tiuhw_powerdown+0x38>
    1920:	2610008c 	addiu	$s0,$s0,140
    1924:	8fbf001c 	lw	$ra,28($sp)
    1928:	8fb20018 	lw	$s2,24($sp)
    192c:	8fb10014 	lw	$s1,20($sp)
    1930:	8fb00010 	lw	$s0,16($sp)
    1934:	03e00008 	jr	$ra
    1938:	27bd0020 	addiu	$sp,$sp,32
*/
}

void tiuhw_lin_post_halt_hook(void)
{
/* prologue
000000000000193c <tiuhw_lin_post_halt_hook>:
    193c:	27bdffe8 	addiu	$sp,$sp,-24
    1940:	afbf0010 	sw	$ra,16($sp)
    1944:	3c040000 	lui	$a0,0x0
    1948:	248404d4 	addiu	$a0,$a0,1236
    194c:	3c020000 	lui	$v0,0x0
    1950:	24420000 	addiu	$v0,$v0,0
    1954:	0040f809 	jalr	$v0
    1958:	00000000 	nop
*/
	printk(KERN_ERR "Titan DSP halt done (TIU)\n");
/*
    195c:	3c020000 	lui	$v0,0x0
    1960:	24421790 	addiu	$v0,$v0,6032
    1964:	0040f809 	jalr	$v0
    1968:	00000000 	nop
*/
	tiuhw_powerup();

/* epilogue
    196c:	8fbf0010 	lw	$ra,16($sp)
    1970:	03e00008 	jr	$ra
    1974:	27bd0018 	addiu	$sp,$sp,24
*/
	return;
}

void tiuhw_lin_pre_halt_hook(void)
{
/* prologue
0000000000001978 <tiuhw_lin_pre_halt_hook>:
    1978:	27bdffe8 	addiu	$sp,$sp,-24
    197c:	afbf0010 	sw	$ra,16($sp)
*/
/*
    1980:	3c040000 	lui	$a0,0x0
    1984:	248404f4 	addiu	$a0,$a0,1268
    1988:	3c020000 	lui	$v0,0x0
    198c:	24420000 	addiu	$v0,$v0,0
    1990:	0040f809 	jalr	$v0
    1994:	00000000 	nop
*/
	printk(KERN_ERR "Titan DSP halt initiated (TIU)\n");
/*
    1998:	3c020000 	lui	$v0,0x0
    199c:	2442187c 	addiu	$v0,$v0,6268
    19a0:	0040f809 	jalr	$v0
    19a4:	00000000 	nop
*/
	tiuhw_powerdown();
/*
    19a8:	8fbf0010 	lw	$ra,16($sp)
    19ac:	03e00008 	jr	$ra
    19b0:	27bd0018 	addiu	$sp,$sp,24
*/
	return;
}

/*
00000000000019b4 <tuihw_read_reg>:
    19b4:	27bdffd8 	addiu	$sp,$sp,-40
    19b8:	afbf0020 	sw	$ra,32($sp)
    19bc:	afb1001c 	sw	$s1,28($sp)
    19c0:	afb00018 	sw	$s0,24($sp)
    19c4:	a3a00010 	sb	$zero,16($sp)
    19c8:	309100ff 	andi	$s1,$a0,0xff
    19cc:	3c020000 	lui	$v0,0x0
    19d0:	24420000 	addiu	$v0,$v0,0
    19d4:	0040f809 	jalr	$v0
    19d8:	30b0ffff 	andi	$s0,$a1,0xffff
    19dc:	00402021 	move	$a0,$v0
    19e0:	24020001 	li	$v0,1
    19e4:	10820005 	beq	$a0,$v0,19fc <tuihw_read_reg+0x48>
    19e8:	24020002 	li	$v0,2
    19ec:	10820024 	beq	$a0,$v0,1a80 <tuihw_read_reg+0xcc>
    19f0:	93a20010 	lbu	$v0,16($sp)
    19f4:	080006b4 	j	1ad0 <tuihw_read_reg+0x11c>
    19f8:	8fbf0020 	lw	$ra,32($sp)
    19fc:	001010c0 	sll	$v0,$s0,0x3
    1a00:	00501021 	addu	$v0,$v0,$s0
    1a04:	00021080 	sll	$v0,$v0,0x2
    1a08:	00501023 	subu	$v0,$v0,$s0
    1a0c:	3c030000 	lui	$v1,0x0
    1a10:	8c630068 	lw	$v1,104($v1)
    1a14:	00021080 	sll	$v0,$v0,0x2
    1a18:	00431021 	addu	$v0,$v0,$v1
    1a1c:	8c420040 	lw	$v0,64($v0)
    1a20:	1444000e 	bne	$v0,$a0,1a5c <tuihw_read_reg+0xa8>
    1a24:	02002021 	move	$a0,$s0
    1a28:	32020001 	andi	$v0,$s0,0x1
    1a2c:	10400003 	beqz	$v0,1a3c <tuihw_read_reg+0x88>
    1a30:	24050001 	li	$a1,1
    1a34:	24050002 	li	$a1,2
    1a38:	02002021 	move	$a0,$s0
    1a3c:	3c020000 	lui	$v0,0x0
    1a40:	8c420000 	lw	$v0,0($v0)
    1a44:	00003021 	move	$a2,$zero
    1a48:	8c420008 	lw	$v0,8($v0)
    1a4c:	0040f809 	jalr	$v0
    1a50:	00003821 	move	$a3,$zero
    1a54:	1040001c 	beqz	$v0,1ac8 <tuihw_read_reg+0x114>
    1a58:	02002021 	move	$a0,$s0
    1a5c:	02202821 	move	$a1,$s1
    1a60:	3c020000 	lui	$v0,0x0
    1a64:	8c420000 	lw	$v0,0($v0)
    1a68:	27a60010 	addiu	$a2,$sp,16
    1a6c:	8c420004 	lw	$v0,4($v0)
    1a70:	0040f809 	jalr	$v0
    1a74:	24070001 	li	$a3,1
    1a78:	080006b3 	j	1acc <tuihw_read_reg+0x118>
    1a7c:	93a20010 	lbu	$v0,16($sp)
    1a80:	02002021 	move	$a0,$s0
    1a84:	02202821 	move	$a1,$s1
    1a88:	3c020000 	lui	$v0,0x0
    1a8c:	8c420000 	lw	$v0,0($v0)
    1a90:	27a60010 	addiu	$a2,$sp,16
    1a94:	8c420004 	lw	$v0,4($v0)
    1a98:	0040f809 	jalr	$v0
    1a9c:	24070001 	li	$a3,1
    1aa0:	1440000a 	bnez	$v0,1acc <tuihw_read_reg+0x118>
    1aa4:	93a20010 	lbu	$v0,16($sp)
    1aa8:	3c040000 	lui	$a0,0x0
    1aac:	24840518 	addiu	$a0,$a0,1304
    1ab0:	02002821 	move	$a1,$s0
    1ab4:	02203021 	move	$a2,$s1
    1ab8:	3c020000 	lui	$v0,0x0
    1abc:	24420000 	addiu	$v0,$v0,0
    1ac0:	0040f809 	jalr	$v0
    1ac4:	93a70010 	lbu	$a3,16($sp)
    1ac8:	93a20010 	lbu	$v0,16($sp)
    1acc:	8fbf0020 	lw	$ra,32($sp)
    1ad0:	8fb1001c 	lw	$s1,28($sp)
    1ad4:	8fb00018 	lw	$s0,24($sp)
    1ad8:	03e00008 	jr	$ra
    1adc:	27bd0028 	addiu	$sp,$sp,40

0000000000001ae0 <tuihw_write_reg>:
    1ae0:	27bdffd0 	addiu	$sp,$sp,-48
    1ae4:	afbf0028 	sw	$ra,40($sp)
    1ae8:	afb10024 	sw	$s1,36($sp)
    1aec:	afb00020 	sw	$s0,32($sp)
    1af0:	a3a50018 	sb	$a1,24($sp)
    1af4:	309100ff 	andi	$s1,$a0,0xff
    1af8:	3c020000 	lui	$v0,0x0
    1afc:	24420000 	addiu	$v0,$v0,0
    1b00:	0040f809 	jalr	$v0
    1b04:	30d0ffff 	andi	$s0,$a2,0xffff
    1b08:	00402021 	move	$a0,$v0
    1b0c:	24020001 	li	$v0,1
    1b10:	10820005 	beq	$a0,$v0,1b28 <tuihw_write_reg+0x48>
    1b14:	24020002 	li	$v0,2
    1b18:	10820027 	beq	$a0,$v0,1bb8 <tuihw_write_reg+0xd8>
    1b1c:	8fbf0028 	lw	$ra,40($sp)
    1b20:	08000702 	j	1c08 <tuihw_write_reg+0x128>
    1b24:	8fb10024 	lw	$s1,36($sp)
    1b28:	001010c0 	sll	$v0,$s0,0x3
    1b2c:	00501021 	addu	$v0,$v0,$s0
    1b30:	00021080 	sll	$v0,$v0,0x2
    1b34:	00501023 	subu	$v0,$v0,$s0
    1b38:	3c030000 	lui	$v1,0x0
    1b3c:	8c630068 	lw	$v1,104($v1)
    1b40:	00021080 	sll	$v0,$v0,0x2
    1b44:	00431021 	addu	$v0,$v0,$v1
    1b48:	8c420040 	lw	$v0,64($v0)
    1b4c:	14440011 	bne	$v0,$a0,1b94 <tuihw_write_reg+0xb4>
    1b50:	02002021 	move	$a0,$s0
    1b54:	32020001 	andi	$v0,$s0,0x1
    1b58:	10400002 	beqz	$v0,1b64 <tuihw_write_reg+0x84>
    1b5c:	24050001 	li	$a1,1
    1b60:	24050002 	li	$a1,2
    1b64:	a3b10010 	sb	$s1,16($sp)
    1b68:	02002021 	move	$a0,$s0
    1b6c:	27a60010 	addiu	$a2,$sp,16
    1b70:	93a20018 	lbu	$v0,24($sp)
    1b74:	3c030000 	lui	$v1,0x0
    1b78:	8c630000 	lw	$v1,0($v1)
    1b7c:	a3a20011 	sb	$v0,17($sp)
    1b80:	8c620008 	lw	$v0,8($v1)
    1b84:	0040f809 	jalr	$v0
    1b88:	24070002 	li	$a3,2
    1b8c:	08000701 	j	1c04 <tuihw_write_reg+0x124>
    1b90:	8fbf0028 	lw	$ra,40($sp)
    1b94:	02202821 	move	$a1,$s1
    1b98:	3c020000 	lui	$v0,0x0
    1b9c:	8c420000 	lw	$v0,0($v0)
    1ba0:	27a60018 	addiu	$a2,$sp,24
    1ba4:	8c420008 	lw	$v0,8($v0)
    1ba8:	0040f809 	jalr	$v0
    1bac:	24070001 	li	$a3,1
    1bb0:	08000701 	j	1c04 <tuihw_write_reg+0x124>
    1bb4:	8fbf0028 	lw	$ra,40($sp)
    1bb8:	02002021 	move	$a0,$s0
    1bbc:	02202821 	move	$a1,$s1
    1bc0:	3c020000 	lui	$v0,0x0
    1bc4:	8c420000 	lw	$v0,0($v0)
    1bc8:	27a60018 	addiu	$a2,$sp,24
    1bcc:	8c420008 	lw	$v0,8($v0)
    1bd0:	0040f809 	jalr	$v0
    1bd4:	24070001 	li	$a3,1
    1bd8:	1440000a 	bnez	$v0,1c04 <tuihw_write_reg+0x124>
    1bdc:	8fbf0028 	lw	$ra,40($sp)
    1be0:	3c040000 	lui	$a0,0x0
    1be4:	24840554 	addiu	$a0,$a0,1364
    1be8:	02002821 	move	$a1,$s0
    1bec:	02203021 	move	$a2,$s1
    1bf0:	3c020000 	lui	$v0,0x0
    1bf4:	24420000 	addiu	$v0,$v0,0
    1bf8:	0040f809 	jalr	$v0
    1bfc:	93a70018 	lbu	$a3,24($sp)
    1c00:	8fbf0028 	lw	$ra,40($sp)
    1c04:	8fb10024 	lw	$s1,36($sp)
    1c08:	8fb00020 	lw	$s0,32($sp)
    1c0c:	03e00008 	jr	$ra
    1c10:	27bd0030 	addiu	$sp,$sp,48

0000000000001c14 <hwu_get_companding>:
    1c14:	3c020000 	lui	$v0,0x0
    1c18:	90420074 	lbu	$v0,116($v0)
    1c1c:	03e00008 	jr	$ra
	...
    1c30:	27bdffe0 	addiu	$sp,$sp,-32
    1c34:	afbf001c 	sw	$ra,28($sp)
    1c38:	afb00018 	sw	$s0,24($sp)
    1c3c:	2405000b 	li	$a1,11
    1c40:	00808021 	move	$s0,$a0
    1c44:	27a60010 	addiu	$a2,$sp,16
    1c48:	3c020000 	lui	$v0,0x0
    1c4c:	8c420000 	lw	$v0,0($v0)
    1c50:	96040000 	lhu	$a0,0($s0)
    1c54:	8c420004 	lw	$v0,4($v0)
    1c58:	0040f809 	jalr	$v0
    1c5c:	24070001 	li	$a3,1
    1c60:	3c040000 	lui	$a0,0x0
    1c64:	24840658 	addiu	$a0,$a0,1624
    1c68:	93a50010 	lbu	$a1,16($sp)
    1c6c:	3c020000 	lui	$v0,0x0
    1c70:	24420000 	addiu	$v0,$v0,0
    1c74:	30a5000f 	andi	$a1,$a1,0xf
    1c78:	0040f809 	jalr	$v0
    1c7c:	24a50031 	addiu	$a1,$a1,49
    1c80:	3c020000 	lui	$v0,0x0
    1c84:	24421f38 	addiu	$v0,$v0,7992
    1c88:	0040f809 	jalr	$v0
    1c8c:	02002021 	move	$a0,$s0
    1c90:	02002021 	move	$a0,$s0
    1c94:	3c020000 	lui	$v0,0x0
    1c98:	24422198 	addiu	$v0,$v0,8600
    1c9c:	0040f809 	jalr	$v0
    1ca0:	24050001 	li	$a1,1
    1ca4:	8fbf001c 	lw	$ra,28($sp)
    1ca8:	8fb00018 	lw	$s0,24($sp)
    1cac:	03e00008 	jr	$ra
    1cb0:	27bd0020 	addiu	$sp,$sp,32
    1cb4:	27bdffd8 	addiu	$sp,$sp,-40
    1cb8:	afbf0024 	sw	$ra,36($sp)
    1cbc:	afb20020 	sw	$s2,32($sp)
    1cc0:	afb1001c 	sw	$s1,28($sp)
    1cc4:	afb00018 	sw	$s0,24($sp)
    1cc8:	00808021 	move	$s0,$a0
    1ccc:	8e020004 	lw	$v0,4($s0)
    1cd0:	24110001 	li	$s1,1
    1cd4:	14510002 	bne	$v0,$s1,1ce0 <hwu_get_companding+0xcc>
    1cd8:	00a09021 	move	$s2,$a1
    1cdc:	ae400000 	sw	$zero,0($s2)
    1ce0:	2405000c 	li	$a1,12
    1ce4:	27a60010 	addiu	$a2,$sp,16
    1ce8:	3c020000 	lui	$v0,0x0
    1cec:	8c420000 	lw	$v0,0($v0)
    1cf0:	96040000 	lhu	$a0,0($s0)
    1cf4:	8c420004 	lw	$v0,4($v0)
    1cf8:	0040f809 	jalr	$v0
    1cfc:	24070001 	li	$a3,1
    1d00:	93a20010 	lbu	$v0,16($sp)
    1d04:	3042000f 	andi	$v0,$v0,0xf
    1d08:	1040000c 	beqz	$v0,1d3c <hwu_get_companding+0x128>
    1d0c:	a3a20010 	sb	$v0,16($sp)
    1d10:	8e020004 	lw	$v0,4($s0)
    1d14:	54400013 	0x54400013
    1d18:	ae510000 	sw	$s1,0($s2)
    1d1c:	3c040000 	lui	$a0,0x0
    1d20:	24840674 	addiu	$a0,$a0,1652
    1d24:	3c020000 	lui	$v0,0x0
    1d28:	24420000 	addiu	$v0,$v0,0
    1d2c:	0040f809 	jalr	$v0
    1d30:	93a50010 	lbu	$a1,16($sp)
    1d34:	08000759 	j	1d64 <hwu_get_companding+0x150>
    1d38:	ae510000 	sw	$s1,0($s2)
    1d3c:	8e020004 	lw	$v0,4($s0)
    1d40:	54510008 	0x54510008
    1d44:	ae400000 	sw	$zero,0($s2)
    1d48:	3c040000 	lui	$a0,0x0
    1d4c:	248406a4 	addiu	$a0,$a0,1700
    1d50:	3c020000 	lui	$v0,0x0
    1d54:	24420000 	addiu	$v0,$v0,0
    1d58:	0040f809 	jalr	$v0
    1d5c:	00000000 	nop
    1d60:	ae400000 	sw	$zero,0($s2)
    1d64:	8fbf0024 	lw	$ra,36($sp)
    1d68:	8fb20020 	lw	$s2,32($sp)
    1d6c:	8fb1001c 	lw	$s1,28($sp)
    1d70:	8fb00018 	lw	$s0,24($sp)
    1d74:	03e00008 	jr	$ra
    1d78:	27bd0028 	addiu	$sp,$sp,40
    1d7c:	27bdffe0 	addiu	$sp,$sp,-32
    1d80:	afbf001c 	sw	$ra,28($sp)
    1d84:	afb20018 	sw	$s2,24($sp)
    1d88:	afb10014 	sw	$s1,20($sp)
    1d8c:	afb00010 	sw	$s0,16($sp)
    1d90:	00808821 	move	$s1,$a0
    1d94:	8e220004 	lw	$v0,4($s1)
    1d98:	10400003 	beqz	$v0,1da8 <hwu_get_companding+0x194>
    1d9c:	00a09021 	move	$s2,$a1
    1da0:	24020001 	li	$v0,1
    1da4:	ae420000 	sw	$v0,0($s2)
    1da8:	3c020000 	lui	$v0,0x0
    1dac:	8c420000 	lw	$v0,0($v0)
    1db0:	8c42001c 	lw	$v0,28($v0)
    1db4:	0040f809 	jalr	$v0
    1db8:	00000000 	nop
    1dbc:	3c030000 	lui	$v1,0x0
    1dc0:	94630010 	lhu	$v1,16($v1)
    1dc4:	3050ffff 	andi	$s0,$v0,0xffff
    1dc8:	12030009 	beq	$s0,$v1,1df0 <hwu_get_companding+0x1dc>
    1dcc:	00000000 	nop
    1dd0:	3c040000 	lui	$a0,0x0
    1dd4:	248406d0 	addiu	$a0,$a0,1744
    1dd8:	3c020000 	lui	$v0,0x0
    1ddc:	24420000 	addiu	$v0,$v0,0
    1de0:	0040f809 	jalr	$v0
    1de4:	02002821 	move	$a1,$s0
    1de8:	3c010000 	lui	$at,0x0
    1dec:	a4300010 	sh	$s0,16($at)
    1df0:	3c020000 	lui	$v0,0x0
    1df4:	8c420000 	lw	$v0,0($v0)
    1df8:	8c42001c 	lw	$v0,28($v0)
    1dfc:	0040f809 	jalr	$v0
    1e00:	00000000 	nop
    1e04:	00021027 	nor	$v0,$zero,$v0
    1e08:	96240000 	lhu	$a0,0($s1)
    1e0c:	24030001 	li	$v1,1
    1e10:	00831804 	sllv	$v1,$v1,$a0
    1e14:	00431024 	and	$v0,$v0,$v1
    1e18:	3050ffff 	andi	$s0,$v0,0xffff
    1e1c:	9222004c 	lbu	$v0,76($s1)
    1e20:	92230048 	lbu	$v1,72($s1)
    1e24:	30420001 	andi	$v0,$v0,0x1
    1e28:	00621823 	subu	$v1,$v1,$v0
    1e2c:	8e22004c 	lw	$v0,76($s1)
    1e30:	a2230048 	sb	$v1,72($s1)
    1e34:	00021042 	srl	$v0,$v0,0x1
    1e38:	12000003 	beqz	$s0,1e48 <hwu_get_companding+0x234>
    1e3c:	ae22004c 	sw	$v0,76($s1)
    1e40:	34428000 	ori	$v0,$v0,0x8000
    1e44:	ae22004c 	sw	$v0,76($s1)
    1e48:	0010182b 	sltu	$v1,$zero,$s0
    1e4c:	92220048 	lbu	$v0,72($s1)
    1e50:	8fbf001c 	lw	$ra,28($sp)
    1e54:	8fb00010 	lw	$s0,16($sp)
    1e58:	00431021 	addu	$v0,$v0,$v1
    1e5c:	a2220048 	sb	$v0,72($s1)
    1e60:	304200ff 	andi	$v0,$v0,0xff
    1e64:	2c420002 	sltiu	$v0,$v0,2
    1e68:	38420001 	xori	$v0,$v0,0x1
    1e6c:	ae420000 	sw	$v0,0($s2)
    1e70:	8fb20018 	lw	$s2,24($sp)
    1e74:	8fb10014 	lw	$s1,20($sp)
    1e78:	03e00008 	jr	$ra
    1e7c:	27bd0020 	addiu	$sp,$sp,32
    1e80:	27bdffd8 	addiu	$sp,$sp,-40
    1e84:	afbf0020 	sw	$ra,32($sp)
    1e88:	afb1001c 	sw	$s1,28($sp)
    1e8c:	afb00018 	sw	$s0,24($sp)
    1e90:	a3a00010 	sb	$zero,16($sp)
    1e94:	00a08821 	move	$s1,$a1
    1e98:	12200013 	beqz	$s1,1ee8 <hwu_get_companding+0x2d4>
    1e9c:	00808021 	move	$s0,$a0
    1ea0:	24020011 	li	$v0,17
    1ea4:	a3a20010 	sb	$v0,16($sp)
    1ea8:	24050005 	li	$a1,5
    1eac:	27a60010 	addiu	$a2,$sp,16
    1eb0:	3c020000 	lui	$v0,0x0
    1eb4:	8c420000 	lw	$v0,0($v0)
    1eb8:	96040000 	lhu	$a0,0($s0)
    1ebc:	8c420008 	lw	$v0,8($v0)
    1ec0:	0040f809 	jalr	$v0
    1ec4:	24070001 	li	$a3,1
    1ec8:	3c040000 	lui	$a0,0x0
    1ecc:	248406e4 	addiu	$a0,$a0,1764
    1ed0:	3c020000 	lui	$v0,0x0
    1ed4:	24420000 	addiu	$v0,$v0,0
    1ed8:	0040f809 	jalr	$v0
    1edc:	00000000 	nop
    1ee0:	080007c9 	j	1f24 <hwu_get_companding+0x310>
    1ee4:	ae110004 	sw	$s1,4($s0)
    1ee8:	24050005 	li	$a1,5
    1eec:	27a60010 	addiu	$a2,$sp,16
    1ef0:	3c020000 	lui	$v0,0x0
    1ef4:	8c420000 	lw	$v0,0($v0)
    1ef8:	96040000 	lhu	$a0,0($s0)
    1efc:	8c420008 	lw	$v0,8($v0)
    1f00:	0040f809 	jalr	$v0
    1f04:	24070001 	li	$a3,1
    1f08:	3c040000 	lui	$a0,0x0
    1f0c:	24840700 	addiu	$a0,$a0,1792
    1f10:	3c020000 	lui	$v0,0x0
    1f14:	24420000 	addiu	$v0,$v0,0
    1f18:	0040f809 	jalr	$v0
    1f1c:	00000000 	nop
    1f20:	ae110004 	sw	$s1,4($s0)
    1f24:	8fbf0020 	lw	$ra,32($sp)
    1f28:	8fb1001c 	lw	$s1,28($sp)
    1f2c:	8fb00018 	lw	$s0,24($sp)
    1f30:	03e00008 	jr	$ra
    1f34:	27bd0028 	addiu	$sp,$sp,40

0000000000001f38 <si3034_hw_reset>:
    1f38:	27bdffe0 	addiu	$sp,$sp,-32
    1f3c:	afbf001c 	sw	$ra,28($sp)
    1f40:	afb00018 	sw	$s0,24($sp)
    1f44:	a3a00010 	sb	$zero,16($sp)
    1f48:	00808021 	move	$s0,$a0
    1f4c:	3c040000 	lui	$a0,0x0
    1f50:	2484071c 	addiu	$a0,$a0,1820
    1f54:	3c020000 	lui	$v0,0x0
    1f58:	24420000 	addiu	$v0,$v0,0
    1f5c:	0040f809 	jalr	$v0
    1f60:	96050000 	lhu	$a1,0($s0)
    1f64:	24020080 	li	$v0,128
    1f68:	a3a20010 	sb	$v0,16($sp)
    1f6c:	02002021 	move	$a0,$s0
    1f70:	3c020000 	lui	$v0,0x0
    1f74:	24421e80 	addiu	$v0,$v0,7808
    1f78:	0040f809 	jalr	$v0
    1f7c:	00002821 	move	$a1,$zero
    1f80:	24050001 	li	$a1,1
    1f84:	27a60010 	addiu	$a2,$sp,16
    1f88:	3c020000 	lui	$v0,0x0
    1f8c:	8c420000 	lw	$v0,0($v0)
    1f90:	96040000 	lhu	$a0,0($s0)
    1f94:	8c420008 	lw	$v0,8($v0)
    1f98:	0040f809 	jalr	$v0
    1f9c:	24070001 	li	$a3,1
    1fa0:	240200e0 	li	$v0,224
    1fa4:	a3a20010 	sb	$v0,16($sp)
    1fa8:	24050006 	li	$a1,6
    1fac:	27a60010 	addiu	$a2,$sp,16
    1fb0:	3c020000 	lui	$v0,0x0
    1fb4:	8c420000 	lw	$v0,0($v0)
    1fb8:	96040000 	lhu	$a0,0($s0)
    1fbc:	8c420008 	lw	$v0,8($v0)
    1fc0:	0040f809 	jalr	$v0
    1fc4:	24070001 	li	$a3,1
    1fc8:	96020000 	lhu	$v0,0($s0)
    1fcc:	30420001 	andi	$v0,$v0,0x1
    1fd0:	10400002 	beqz	$v0,1fdc <si3034_hw_reset+0xa4>
    1fd4:	2403001d 	li	$v1,29
    1fd8:	2403003d 	li	$v1,61
    1fdc:	a3a30010 	sb	$v1,16($sp)
    1fe0:	2405000e 	li	$a1,14
    1fe4:	27a60010 	addiu	$a2,$sp,16
    1fe8:	3c020000 	lui	$v0,0x0
    1fec:	8c420000 	lw	$v0,0($v0)
    1ff0:	96040000 	lhu	$a0,0($s0)
    1ff4:	8c420008 	lw	$v0,8($v0)
    1ff8:	0040f809 	jalr	$v0
    1ffc:	24070001 	li	$a3,1
    2000:	ae00004c 	sw	$zero,76($s0)
    2004:	a2000048 	sb	$zero,72($s0)
    2008:	a3a00010 	sb	$zero,16($sp)
    200c:	2405000d 	li	$a1,13
    2010:	27a60010 	addiu	$a2,$sp,16
    2014:	3c020000 	lui	$v0,0x0
    2018:	8c420000 	lw	$v0,0($v0)
    201c:	96040000 	lhu	$a0,0($s0)
    2020:	8c420008 	lw	$v0,8($v0)
    2024:	0040f809 	jalr	$v0
    2028:	24070001 	li	$a3,1
    202c:	8fbf001c 	lw	$ra,28($sp)
    2030:	8fb00018 	lw	$s0,24($sp)
    2034:	03e00008 	jr	$ra
    2038:	27bd0020 	addiu	$sp,$sp,32

000000000000203c <si3034_hw_gain_control>:
    203c:	27bdffd0 	addiu	$sp,$sp,-48
    2040:	afbf0028 	sw	$ra,40($sp)
    2044:	afb30024 	sw	$s3,36($sp)
    2048:	afb20020 	sw	$s2,32($sp)
    204c:	afb1001c 	sw	$s1,28($sp)
    2050:	afb00018 	sw	$s0,24($sp)
    2054:	3092ffff 	andi	$s2,$a0,0xffff
    2058:	02402021 	move	$a0,$s2
    205c:	30b000ff 	andi	$s0,$a1,0xff
    2060:	2405000f 	li	$a1,15
    2064:	00c09821 	move	$s3,$a2
    2068:	3c020000 	lui	$v0,0x0
    206c:	8c420000 	lw	$v0,0($v0)
    2070:	27a60010 	addiu	$a2,$sp,16
    2074:	8c420004 	lw	$v0,4($v0)
    2078:	0040f809 	jalr	$v0
    207c:	24070001 	li	$a3,1
    2080:	3c040000 	lui	$a0,0x0
    2084:	24840744 	addiu	$a0,$a0,1860
    2088:	3c110000 	lui	$s1,0x0
    208c:	26310000 	addiu	$s1,$s1,0
    2090:	0220f809 	jalr	$s1
    2094:	00000000 	nop
    2098:	2e02000d 	sltiu	$v0,$s0,13
    209c:	10400014 	beqz	$v0,20f0 <si3034_hw_gain_control+0xb4>
    20a0:	00101080 	sll	$v0,$s0,0x2
    20a4:	3c010000 	lui	$at,0x0
    20a8:	00220821 	addu	$at,$at,$v0
    20ac:	8c220790 	lw	$v0,1936($at)
    20b0:	00400008 	jr	$v0
    20b4:	00000000 	nop
    20b8:	08000841 	j	2104 <si3034_hw_gain_control+0xc8>
    20bc:	a3a00011 	sb	$zero,17($sp)
    20c0:	24020001 	li	$v0,1
    20c4:	08000841 	j	2104 <si3034_hw_gain_control+0xc8>
    20c8:	a3a20011 	sb	$v0,17($sp)
    20cc:	24020002 	li	$v0,2
    20d0:	08000841 	j	2104 <si3034_hw_gain_control+0xc8>
    20d4:	a3a20011 	sb	$v0,17($sp)
    20d8:	24020003 	li	$v0,3
    20dc:	08000841 	j	2104 <si3034_hw_gain_control+0xc8>
    20e0:	a3a20011 	sb	$v0,17($sp)
    20e4:	24020004 	li	$v0,4
    20e8:	08000841 	j	2104 <si3034_hw_gain_control+0xc8>
    20ec:	a3a20011 	sb	$v0,17($sp)
    20f0:	3c040000 	lui	$a0,0x0
    20f4:	0220f809 	jalr	$s1
    20f8:	24840760 	addiu	$a0,$a0,1888
    20fc:	08000855 	j	2154 <si3034_hw_gain_control+0x118>
    2100:	8fbf0028 	lw	$ra,40($sp)
    2104:	12600005 	beqz	$s3,211c <si3034_hw_gain_control+0xe0>
    2108:	93a30010 	lbu	$v1,16($sp)
    210c:	93a20011 	lbu	$v0,17($sp)
    2110:	3063000f 	andi	$v1,$v1,0xf
    2114:	0800084a 	j	2128 <si3034_hw_gain_control+0xec>
    2118:	00021100 	sll	$v0,$v0,0x4
    211c:	93a20010 	lbu	$v0,16($sp)
    2120:	93a30011 	lbu	$v1,17($sp)
    2124:	304200f0 	andi	$v0,$v0,0xf0
    2128:	00621825 	or	$v1,$v1,$v0
    212c:	a3a30011 	sb	$v1,17($sp)
    2130:	02402021 	move	$a0,$s2
    2134:	2405000f 	li	$a1,15
    2138:	3c020000 	lui	$v0,0x0
    213c:	8c420000 	lw	$v0,0($v0)
    2140:	27a60011 	addiu	$a2,$sp,17
    2144:	8c420008 	lw	$v0,8($v0)
    2148:	0040f809 	jalr	$v0
    214c:	24070001 	li	$a3,1
    2150:	8fbf0028 	lw	$ra,40($sp)
    2154:	8fb30024 	lw	$s3,36($sp)
    2158:	8fb20020 	lw	$s2,32($sp)
    215c:	8fb1001c 	lw	$s1,28($sp)
    2160:	8fb00018 	lw	$s0,24($sp)
    2164:	03e00008 	jr	$ra
    2168:	27bd0030 	addiu	$sp,$sp,48

000000000000216c <si3034_set_loopback>:
    216c:	27bdffe8 	addiu	$sp,$sp,-24
    2170:	afbf0010 	sw	$ra,16($sp)
    2174:	3c040000 	lui	$a0,0x0
    2178:	248407c4 	addiu	$a0,$a0,1988
    217c:	3c020000 	lui	$v0,0x0
    2180:	24420000 	addiu	$v0,$v0,0
    2184:	0040f809 	jalr	$v0
    2188:	00000000 	nop
    218c:	8fbf0010 	lw	$ra,16($sp)
    2190:	03e00008 	jr	$ra
    2194:	27bd0018 	addiu	$sp,$sp,24
    2198:	27bdffe0 	addiu	$sp,$sp,-32
    219c:	afbf001c 	sw	$ra,28($sp)
    21a0:	afb00018 	sw	$s0,24($sp)
    21a4:	24020070 	li	$v0,112
    21a8:	a3a20010 	sb	$v0,16($sp)
    21ac:	10a00015 	beqz	$a1,2204 <si3034_set_loopback+0x98>
    21b0:	00808021 	move	$s0,$a0
    21b4:	3c020000 	lui	$v0,0x0
    21b8:	24421e80 	addiu	$v0,$v0,7808
    21bc:	0040f809 	jalr	$v0
    21c0:	00002821 	move	$a1,$zero
    21c4:	24050006 	li	$a1,6
    21c8:	27a60010 	addiu	$a2,$sp,16
    21cc:	3c020000 	lui	$v0,0x0
    21d0:	8c420000 	lw	$v0,0($v0)
    21d4:	96040000 	lhu	$a0,0($s0)
    21d8:	8c420008 	lw	$v0,8($v0)
    21dc:	0040f809 	jalr	$v0
    21e0:	24070001 	li	$a3,1
    21e4:	3c040000 	lui	$a0,0x0
    21e8:	248407f4 	addiu	$a0,$a0,2036
    21ec:	3c020000 	lui	$v0,0x0
    21f0:	24420000 	addiu	$v0,$v0,0
    21f4:	0040f809 	jalr	$v0
    21f8:	96050000 	lhu	$a1,0($s0)
    21fc:	0800088c 	j	2230 <si3034_set_loopback+0xc4>
    2200:	8fbf001c 	lw	$ra,28($sp)
    2204:	240200e0 	li	$v0,224
    2208:	a3a20010 	sb	$v0,16($sp)
    220c:	24050006 	li	$a1,6
    2210:	27a60010 	addiu	$a2,$sp,16
    2214:	3c020000 	lui	$v0,0x0
    2218:	8c420000 	lw	$v0,0($v0)
    221c:	96040000 	lhu	$a0,0($s0)
    2220:	8c420008 	lw	$v0,8($v0)
    2224:	0040f809 	jalr	$v0
    2228:	24070001 	li	$a3,1
    222c:	8fbf001c 	lw	$ra,28($sp)
    2230:	8fb00018 	lw	$s0,24($sp)
    2234:	24020001 	li	$v0,1
    2238:	03e00008 	jr	$ra
    223c:	27bd0020 	addiu	$sp,$sp,32

0000000000002240 <si3034_set_lowpwr_path>:
    2240:	27bdffe0 	addiu	$sp,$sp,-32
    2244:	afbf001c 	sw	$ra,28($sp)
    2248:	afb00018 	sw	$s0,24($sp)
    224c:	24020001 	li	$v0,1
    2250:	a3a20010 	sb	$v0,16($sp)
    2254:	24050012 	li	$a1,18
    2258:	27a60010 	addiu	$a2,$sp,16
    225c:	00808021 	move	$s0,$a0
    2260:	3c020000 	lui	$v0,0x0
    2264:	8c420000 	lw	$v0,0($v0)
    2268:	96040000 	lhu	$a0,0($s0)
    226c:	8c420008 	lw	$v0,8($v0)
    2270:	0040f809 	jalr	$v0
    2274:	24070001 	li	$a3,1
    2278:	3c026666 	lui	$v0,0x6666
    227c:	34425b00 	ori	$v0,$v0,0x5b00
    2280:	3c030000 	lui	$v1,0x0
    2284:	8c630000 	lw	$v1,0($v1)
    2288:	00430019 	multu	$v0,$v1
    228c:	00001010 	mfhi	$v0
    2290:	1440ffff 	bnez	$v0,2290 <si3034_set_lowpwr_path+0x50>
    2294:	2442ffff 	addiu	$v0,$v0,-1
    2298:	a3a00010 	sb	$zero,16($sp)
    229c:	24050012 	li	$a1,18
    22a0:	27a60010 	addiu	$a2,$sp,16
    22a4:	3c020000 	lui	$v0,0x0
    22a8:	8c420000 	lw	$v0,0($v0)
    22ac:	96040000 	lhu	$a0,0($s0)
    22b0:	8c420008 	lw	$v0,8($v0)
    22b4:	0040f809 	jalr	$v0
    22b8:	24070001 	li	$a3,1
    22bc:	24020008 	li	$v0,8
    22c0:	a3a20010 	sb	$v0,16($sp)
    22c4:	24050005 	li	$a1,5
    22c8:	27a60010 	addiu	$a2,$sp,16
    22cc:	3c020000 	lui	$v0,0x0
    22d0:	8c420000 	lw	$v0,0($v0)
    22d4:	96040000 	lhu	$a0,0($s0)
    22d8:	8c420008 	lw	$v0,8($v0)
    22dc:	0040f809 	jalr	$v0
    22e0:	24070001 	li	$a3,1
    22e4:	8fbf001c 	lw	$ra,28($sp)
    22e8:	8fb00018 	lw	$s0,24($sp)
    22ec:	03e00008 	jr	$ra
    22f0:	27bd0020 	addiu	$sp,$sp,32

00000000000022f4 <si3034_clear_lowpwr_path>:
    22f4:	27bdffe0 	addiu	$sp,$sp,-32
    22f8:	afbf0018 	sw	$ra,24($sp)
    22fc:	a3a00010 	sb	$zero,16($sp)
    2300:	24050005 	li	$a1,5
    2304:	27a60010 	addiu	$a2,$sp,16
    2308:	3c020000 	lui	$v0,0x0
    230c:	8c420000 	lw	$v0,0($v0)
    2310:	94840000 	lhu	$a0,0($a0)
    2314:	8c420008 	lw	$v0,8($v0)
    2318:	0040f809 	jalr	$v0
    231c:	24070001 	li	$a3,1
    2320:	8fbf0018 	lw	$ra,24($sp)
    2324:	03e00008 	jr	$ra
    2328:	27bd0020 	addiu	$sp,$sp,32
    232c:	27bdffe8 	addiu	$sp,$sp,-24
    2330:	afbf0010 	sw	$ra,16($sp)
    2334:	3c020000 	lui	$v0,0x0
    2338:	24422198 	addiu	$v0,$v0,8600
    233c:	0040f809 	jalr	$v0
    2340:	00002821 	move	$a1,$zero
    2344:	8fbf0010 	lw	$ra,16($sp)
    2348:	24020001 	li	$v0,1
    234c:	03e00008 	jr	$ra
    2350:	27bd0018 	addiu	$sp,$sp,24
    2354:	27bdffe8 	addiu	$sp,$sp,-24
    2358:	24022712 	li	$v0,10002
    235c:	10a20015 	beq	$a1,$v0,23b4 <si3034_clear_lowpwr_path+0xc0>
    2360:	afbf0010 	sw	$ra,16($sp)
    2364:	28a22713 	slti	$v0,$a1,10003
    2368:	10400005 	beqz	$v0,2380 <si3034_clear_lowpwr_path+0x8c>
    236c:	24022711 	li	$v0,10001
    2370:	10a2000a 	beq	$a1,$v0,239c <si3034_clear_lowpwr_path+0xa8>
    2374:	8fbf0010 	lw	$ra,16($sp)
    2378:	08000906 	j	2418 <si3034_clear_lowpwr_path+0x124>
    237c:	00000000 	nop
    2380:	2402271b 	li	$v0,10011
    2384:	10a2001b 	beq	$a1,$v0,23f4 <si3034_clear_lowpwr_path+0x100>
    2388:	2402271d 	li	$v0,10013
    238c:	10a2000f 	beq	$a1,$v0,23cc <si3034_clear_lowpwr_path+0xd8>
    2390:	8fbf0010 	lw	$ra,16($sp)
    2394:	08000906 	j	2418 <si3034_clear_lowpwr_path+0x124>
    2398:	00000000 	nop
    239c:	3c020000 	lui	$v0,0x0
    23a0:	24422240 	addiu	$v0,$v0,8768
    23a4:	0040f809 	jalr	$v0
    23a8:	00000000 	nop
    23ac:	08000906 	j	2418 <si3034_clear_lowpwr_path+0x124>
    23b0:	8fbf0010 	lw	$ra,16($sp)
    23b4:	3c020000 	lui	$v0,0x0
    23b8:	244222f4 	addiu	$v0,$v0,8948
    23bc:	0040f809 	jalr	$v0
    23c0:	00000000 	nop
    23c4:	08000906 	j	2418 <si3034_clear_lowpwr_path+0x124>
    23c8:	8fbf0010 	lw	$ra,16($sp)
    23cc:	94840000 	lhu	$a0,0($a0)
    23d0:	90c50000 	lbu	$a1,0($a2)
    23d4:	3c020000 	lui	$v0,0x0
    23d8:	8c420000 	lw	$v0,0($v0)
    23dc:	90c70001 	lbu	$a3,1($a2)
    23e0:	8c420004 	lw	$v0,4($v0)
    23e4:	0040f809 	jalr	$v0
    23e8:	24c60002 	addiu	$a2,$a2,2
    23ec:	08000906 	j	2418 <si3034_clear_lowpwr_path+0x124>
    23f0:	8fbf0010 	lw	$ra,16($sp)
    23f4:	94840000 	lhu	$a0,0($a0)
    23f8:	90c50000 	lbu	$a1,0($a2)
    23fc:	3c020000 	lui	$v0,0x0
    2400:	8c420000 	lw	$v0,0($v0)
    2404:	90c70001 	lbu	$a3,1($a2)
    2408:	8c420008 	lw	$v0,8($v0)
    240c:	0040f809 	jalr	$v0
    2410:	24c60002 	addiu	$a2,$a2,2
    2414:	8fbf0010 	lw	$ra,16($sp)
    2418:	03e00008 	jr	$ra
    241c:	27bd0018 	addiu	$sp,$sp,24
    2420:	03e00008 	jr	$ra
    2424:	24020002 	li	$v0,2
    2428:	27bdffe0 	addiu	$sp,$sp,-32
    242c:	afbf001c 	sw	$ra,28($sp)
    2430:	afb00018 	sw	$s0,24($sp)
    2434:	2405002b 	li	$a1,43
    2438:	27a60010 	addiu	$a2,$sp,16
    243c:	3c020000 	lui	$v0,0x0
    2440:	8c420000 	lw	$v0,0($v0)
    2444:	94840000 	lhu	$a0,0($a0)
    2448:	8c420004 	lw	$v0,4($v0)
    244c:	0040f809 	jalr	$v0
    2450:	24070001 	li	$a3,1
    2454:	3c100000 	lui	$s0,0x0
    2458:	26100018 	addiu	$s0,$s0,24
    245c:	02002021 	move	$a0,$s0
    2460:	3c050000 	lui	$a1,0x0
    2464:	24a50810 	addiu	$a1,$a1,2064
    2468:	93a60010 	lbu	$a2,16($sp)
    246c:	3c020000 	lui	$v0,0x0
    2470:	24420000 	addiu	$v0,$v0,0
    2474:	30c6000f 	andi	$a2,$a2,0xf
    2478:	0040f809 	jalr	$v0
    247c:	24c60031 	addiu	$a2,$a2,49
    2480:	02001021 	move	$v0,$s0
    2484:	8fbf001c 	lw	$ra,28($sp)
    2488:	8fb00018 	lw	$s0,24($sp)
    248c:	03e00008 	jr	$ra
    2490:	27bd0020 	addiu	$sp,$sp,32

0000000000002494 <si3034_api_init>:
    2494:	3c030000 	lui	$v1,0x0
    2498:	8c630090 	lw	$v1,144($v1)
    249c:	24020001 	li	$v0,1
    24a0:	1462002d 	bne	$v1,$v0,2558 <si3034_api_init+0xc4>
    24a4:	00000000 	nop
    24a8:	3c010000 	lui	$at,0x0
    24ac:	ac200090 	sw	$zero,144($at)
    24b0:	3c020000 	lui	$v0,0x0
    24b4:	24420004 	addiu	$v0,$v0,4
    24b8:	8c440000 	lw	$a0,0($v0)
    24bc:	3c030000 	lui	$v1,0x0
    24c0:	24630098 	addiu	$v1,$v1,152
    24c4:	ac640000 	sw	$a0,0($v1)
    24c8:	8c44000c 	lw	$a0,12($v0)
    24cc:	8c450014 	lw	$a1,20($v0)
    24d0:	ac64000c 	sw	$a0,12($v1)
    24d4:	ac650014 	sw	$a1,20($v1)
    24d8:	8c44001c 	lw	$a0,28($v0)
    24dc:	8c450024 	lw	$a1,36($v0)
    24e0:	ac64001c 	sw	$a0,28($v1)
    24e4:	ac650024 	sw	$a1,36($v1)
    24e8:	8c44002c 	lw	$a0,44($v0)
    24ec:	8c450030 	lw	$a1,48($v0)
    24f0:	ac64002c 	sw	$a0,44($v1)
    24f4:	ac650030 	sw	$a1,48($v1)
    24f8:	8c440034 	lw	$a0,52($v0)
    24fc:	8c450038 	lw	$a1,56($v0)
    2500:	ac640034 	sw	$a0,52($v1)
    2504:	ac650038 	sw	$a1,56($v1)
    2508:	8c44003c 	lw	$a0,60($v0)
    250c:	8c450040 	lw	$a1,64($v0)
    2510:	ac64003c 	sw	$a0,60($v1)
    2514:	ac650040 	sw	$a1,64($v1)
    2518:	8c440044 	lw	$a0,68($v0)
    251c:	8c450048 	lw	$a1,72($v0)
    2520:	ac640044 	sw	$a0,68($v1)
    2524:	ac650048 	sw	$a1,72($v1)
    2528:	8c44004c 	lw	$a0,76($v0)
    252c:	8c450050 	lw	$a1,80($v0)
    2530:	ac64004c 	sw	$a0,76($v1)
    2534:	ac650050 	sw	$a1,80($v1)
    2538:	8c440054 	lw	$a0,84($v0)
    253c:	8c450058 	lw	$a1,88($v0)
    2540:	ac640054 	sw	$a0,84($v1)
    2544:	ac650058 	sw	$a1,88($v1)
    2548:	8c44005c 	lw	$a0,92($v0)
    254c:	8c420060 	lw	$v0,96($v0)
    2550:	ac64005c 	sw	$a0,92($v1)
    2554:	ac620060 	sw	$v0,96($v1)
    2558:	03e00008 	jr	$ra
    255c:	00000000 	nop
    2560:	27bdffd8 	addiu	$sp,$sp,-40
    2564:	afbf0024 	sw	$ra,36($sp)
    2568:	afb20020 	sw	$s2,32($sp)
    256c:	afb1001c 	sw	$s1,28($sp)
    2570:	afb00018 	sw	$s0,24($sp)
    2574:	24020001 	li	$v0,1
    2578:	a3a20010 	sb	$v0,16($sp)
    257c:	24050002 	li	$a1,2
    2580:	27a60010 	addiu	$a2,$sp,16
    2584:	00003821 	move	$a3,$zero
    2588:	00809021 	move	$s2,$a0
    258c:	3c020000 	lui	$v0,0x0
    2590:	8c420000 	lw	$v0,0($v0)
    2594:	96440000 	lhu	$a0,0($s2)
    2598:	8c42002c 	lw	$v0,44($v0)
    259c:	0040f809 	jalr	$v0
    25a0:	00808821 	move	$s1,$a0
    25a4:	02402021 	move	$a0,$s2
    25a8:	92420003 	lbu	$v0,3($s2)
    25ac:	3c030000 	lui	$v1,0x0
    25b0:	8c63001c 	lw	$v1,28($v1)
    25b4:	00021080 	sll	$v0,$v0,0x2
    25b8:	00431021 	addu	$v0,$v0,$v1
    25bc:	8c420000 	lw	$v0,0($v0)
    25c0:	24051001 	li	$a1,4097
    25c4:	8c42000c 	lw	$v0,12($v0)
    25c8:	0040f809 	jalr	$v0
    25cc:	00003021 	move	$a2,$zero
    25d0:	3c020000 	lui	$v0,0x0
    25d4:	244226b8 	addiu	$v0,$v0,9912
    25d8:	0040f809 	jalr	$v0
    25dc:	02202021 	move	$a0,$s1
    25e0:	1040002e 	beqz	$v0,269c <si3034_api_init+0x208>
    25e4:	2404000b 	li	$a0,11
    25e8:	3c020000 	lui	$v0,0x0
    25ec:	24422f88 	addiu	$v0,$v0,12168
    25f0:	0040f809 	jalr	$v0
    25f4:	02202821 	move	$a1,$s1
    25f8:	00408021 	move	$s0,$v0
    25fc:	00101102 	srl	$v0,$s0,0x4
    2600:	304200ff 	andi	$v0,$v0,0xff
    2604:	2c420004 	sltiu	$v0,$v0,4
    2608:	14400008 	bnez	$v0,262c <si3034_api_init+0x198>
    260c:	00000000 	nop
    2610:	3210000f 	andi	$s0,$s0,0xf
    2614:	3c040000 	lui	$a0,0x0
    2618:	248408f8 	addiu	$a0,$a0,2296
    261c:	3c020000 	lui	$v0,0x0
    2620:	24420000 	addiu	$v0,$v0,0
    2624:	0040f809 	jalr	$v0
    2628:	02002821 	move	$a1,$s0
    262c:	3c040000 	lui	$a0,0x0
    2630:	24840914 	addiu	$a0,$a0,2324
    2634:	3205000f 	andi	$a1,$s0,0xf
    2638:	00101102 	srl	$v0,$s0,0x4
    263c:	3042000f 	andi	$v0,$v0,0xf
    2640:	00023080 	sll	$a2,$v0,0x2
    2644:	00c23021 	addu	$a2,$a2,$v0
    2648:	3c010000 	lui	$at,0x0
    264c:	24210110 	addiu	$at,$at,272
    2650:	00263021 	addu	$a2,$at,$a2
    2654:	3c020000 	lui	$v0,0x0
    2658:	24420000 	addiu	$v0,$v0,0
    265c:	0040f809 	jalr	$v0
    2660:	02203821 	move	$a3,$s1
    2664:	5200000e 	0x5200000e
    2668:	00001021 	move	$v0,$zero
    266c:	9644002e 	lhu	$a0,46($s2)
    2670:	3c020000 	lui	$v0,0x0
    2674:	24422838 	addiu	$v0,$v0,10296
    2678:	0040f809 	jalr	$v0
    267c:	02202821 	move	$a1,$s1
    2680:	02402021 	move	$a0,$s2
    2684:	3c020000 	lui	$v0,0x0
    2688:	24422b08 	addiu	$v0,$v0,11016
    268c:	0040f809 	jalr	$v0
    2690:	24050001 	li	$a1,1
    2694:	080009a8 	j	26a0 <si3034_api_init+0x20c>
    2698:	24020001 	li	$v0,1
    269c:	00001021 	move	$v0,$zero
    26a0:	8fbf0024 	lw	$ra,36($sp)
    26a4:	8fb20020 	lw	$s2,32($sp)
    26a8:	8fb1001c 	lw	$s1,28($sp)
    26ac:	8fb00018 	lw	$s0,24($sp)
    26b0:	03e00008 	jr	$ra
    26b4:	27bd0028 	addiu	$sp,$sp,40

00000000000026b8 <si3050_hw_reset>:
    26b8:	27bdffe0 	addiu	$sp,$sp,-32
    26bc:	afbf001c 	sw	$ra,28($sp)
    26c0:	afb20018 	sw	$s2,24($sp)
    26c4:	afb10014 	sw	$s1,20($sp)
    26c8:	afb00010 	sw	$s0,16($sp)
    26cc:	00009021 	move	$s2,$zero
    26d0:	3090ffff 	andi	$s0,$a0,0xffff
    26d4:	24040006 	li	$a0,6
    26d8:	00002821 	move	$a1,$zero
    26dc:	3c110000 	lui	$s1,0x0
    26e0:	26312ee4 	addiu	$s1,$s1,12004
    26e4:	0220f809 	jalr	$s1
    26e8:	02003021 	move	$a2,$s0
    26ec:	080009cc 	j	2730 <si3050_hw_reset+0x78>
    26f0:	2404000c 	li	$a0,12
    26f4:	3c021999 	lui	$v0,0x1999
    26f8:	344296c0 	ori	$v0,$v0,0x96c0
    26fc:	3c030000 	lui	$v1,0x0
    2700:	8c630000 	lw	$v1,0($v1)
    2704:	00430019 	multu	$v0,$v1
    2708:	00001010 	mfhi	$v0
    270c:	1440ffff 	bnez	$v0,270c <si3050_hw_reset+0x54>
    2710:	2442ffff 	addiu	$v0,$v0,-1
    2714:	2484ffff 	addiu	$a0,$a0,-1
    2718:	2402ffff 	li	$v0,-1
    271c:	1482fff6 	bne	$a0,$v0,26f8 <si3050_hw_reset+0x40>
    2720:	3c021999 	lui	$v0,0x1999
    2724:	26420001 	addiu	$v0,$s2,1
    2728:	305200ff 	andi	$s2,$v0,0xff
    272c:	2404000c 	li	$a0,12
    2730:	3c020000 	lui	$v0,0x0
    2734:	24422f88 	addiu	$v0,$v0,12168
    2738:	0040f809 	jalr	$v0
    273c:	02002821 	move	$a1,$s0
    2740:	30420040 	andi	$v0,$v0,0x40
    2744:	14400003 	bnez	$v0,2754 <si3050_hw_reset+0x9c>
    2748:	2e420019 	sltiu	$v0,$s2,25
    274c:	1440ffe9 	bnez	$v0,26f4 <si3050_hw_reset+0x3c>
    2750:	240400c7 	li	$a0,199
    2754:	2e420019 	sltiu	$v0,$s2,25
    2758:	1040002a 	beqz	$v0,2804 <si3050_hw_reset+0x14c>
    275c:	2404001e 	li	$a0,30
    2760:	00002821 	move	$a1,$zero
    2764:	0220f809 	jalr	$s1
    2768:	02003021 	move	$a2,$s0
    276c:	2404001a 	li	$a0,26
    2770:	240500c0 	li	$a1,192
    2774:	0220f809 	jalr	$s1
    2778:	02003021 	move	$a2,$s0
    277c:	24040010 	li	$a0,16
    2780:	00002821 	move	$a1,$zero
    2784:	0220f809 	jalr	$s1
    2788:	02003021 	move	$a2,$s0
    278c:	2404001f 	li	$a0,31
    2790:	24050022 	li	$a1,34
    2794:	0220f809 	jalr	$s1
    2798:	02003021 	move	$a2,$s0
    279c:	24040016 	li	$a0,22
    27a0:	24050096 	li	$a1,150
    27a4:	0220f809 	jalr	$s1
    27a8:	02003021 	move	$a2,$s0
    27ac:	24040017 	li	$a0,23
    27b0:	2405002d 	li	$a1,45
    27b4:	0220f809 	jalr	$s1
    27b8:	02003021 	move	$a2,$s0
    27bc:	24040018 	li	$a0,24
    27c0:	24050099 	li	$a1,153
    27c4:	0220f809 	jalr	$s1
    27c8:	02003021 	move	$a2,$s0
    27cc:	24040011 	li	$a0,17
    27d0:	00002821 	move	$a1,$zero
    27d4:	0220f809 	jalr	$s1
    27d8:	02003021 	move	$a2,$s0
    27dc:	24040012 	li	$a0,18
    27e0:	00002821 	move	$a1,$zero
    27e4:	0220f809 	jalr	$s1
    27e8:	02003021 	move	$a2,$s0
    27ec:	24040013 	li	$a0,19
    27f0:	00002821 	move	$a1,$zero
    27f4:	0220f809 	jalr	$s1
    27f8:	02003021 	move	$a2,$s0
    27fc:	08000a08 	j	2820 <si3050_hw_reset+0x168>
    2800:	24020001 	li	$v0,1
    2804:	3c040000 	lui	$a0,0x0
    2808:	24840948 	addiu	$a0,$a0,2376
    280c:	3c020000 	lui	$v0,0x0
    2810:	24420000 	addiu	$v0,$v0,0
    2814:	0040f809 	jalr	$v0
    2818:	00000000 	nop
    281c:	00001021 	move	$v0,$zero
    2820:	8fbf001c 	lw	$ra,28($sp)
    2824:	8fb20018 	lw	$s2,24($sp)
    2828:	8fb10014 	lw	$s1,20($sp)
    282c:	8fb00010 	lw	$s0,16($sp)
    2830:	03e00008 	jr	$ra
    2834:	27bd0020 	addiu	$sp,$sp,32
    2838:	27bdffd8 	addiu	$sp,$sp,-40
    283c:	afbf0024 	sw	$ra,36($sp)
    2840:	afb40020 	sw	$s4,32($sp)
    2844:	afb3001c 	sw	$s3,28($sp)
    2848:	afb20018 	sw	$s2,24($sp)
    284c:	afb10014 	sw	$s1,20($sp)
    2850:	afb00010 	sw	$s0,16($sp)
    2854:	3090ffff 	andi	$s0,$a0,0xffff
    2858:	3c110000 	lui	$s1,0x0
    285c:	26310000 	addiu	$s1,$s1,0
    2860:	0220f809 	jalr	$s1
    2864:	30b3ffff 	andi	$s3,$a1,0xffff
    2868:	24030002 	li	$v1,2
    286c:	14430006 	bne	$v0,$v1,2888 <si3050_hw_reset+0x1d0>
    2870:	2602ffff 	addiu	$v0,$s0,-1
    2874:	00021100 	sll	$v0,$v0,0x4
    2878:	24420001 	addiu	$v0,$v0,1
    287c:	3050ffff 	andi	$s0,$v0,0xffff
    2880:	08000a30 	j	28c0 <si3050_hw_reset+0x208>
    2884:	24140038 	li	$s4,56
    2888:	0220f809 	jalr	$s1
    288c:	00000000 	nop
    2890:	24030001 	li	$v1,1
    2894:	14430006 	bne	$v0,$v1,28b0 <si3050_hw_reset+0x1f8>
    2898:	2602ffff 	addiu	$v0,$s0,-1
    289c:	000210c0 	sll	$v0,$v0,0x3
    28a0:	24420001 	addiu	$v0,$v0,1
    28a4:	3050ffff 	andi	$s0,$v0,0xffff
    28a8:	08000a30 	j	28c0 <si3050_hw_reset+0x208>
    28ac:	24140029 	li	$s4,41
    28b0:	000210c0 	sll	$v0,$v0,0x3
    28b4:	24420001 	addiu	$v0,$v0,1
    28b8:	3050ffff 	andi	$s0,$v0,0xffff
    28bc:	24140021 	li	$s4,33
    28c0:	24040022 	li	$a0,34
    28c4:	321200ff 	andi	$s2,$s0,0xff
    28c8:	02402821 	move	$a1,$s2
    28cc:	3c110000 	lui	$s1,0x0
    28d0:	26312ee4 	addiu	$s1,$s1,12004
    28d4:	0220f809 	jalr	$s1
    28d8:	02603021 	move	$a2,$s3
    28dc:	24040023 	li	$a0,35
    28e0:	00108202 	srl	$s0,$s0,0x8
    28e4:	32100003 	andi	$s0,$s0,0x3
    28e8:	02002821 	move	$a1,$s0
    28ec:	0220f809 	jalr	$s1
    28f0:	02603021 	move	$a2,$s3
    28f4:	24040024 	li	$a0,36
    28f8:	02402821 	move	$a1,$s2
    28fc:	0220f809 	jalr	$s1
    2900:	02603021 	move	$a2,$s3
    2904:	24040025 	li	$a0,37
    2908:	02002821 	move	$a1,$s0
    290c:	0220f809 	jalr	$s1
    2910:	02603021 	move	$a2,$s3
    2914:	24040021 	li	$a0,33
    2918:	02802821 	move	$a1,$s4
    291c:	0220f809 	jalr	$s1
    2920:	02603021 	move	$a2,$s3
    2924:	8fbf0024 	lw	$ra,36($sp)
    2928:	8fb40020 	lw	$s4,32($sp)
    292c:	8fb3001c 	lw	$s3,28($sp)
    2930:	8fb20018 	lw	$s2,24($sp)
    2934:	8fb10014 	lw	$s1,20($sp)
    2938:	8fb00010 	lw	$s0,16($sp)
    293c:	03e00008 	jr	$ra
    2940:	27bd0028 	addiu	$sp,$sp,40
    2944:	27bdffe0 	addiu	$sp,$sp,-32
    2948:	afbf001c 	sw	$ra,28($sp)
    294c:	afb20018 	sw	$s2,24($sp)
    2950:	afb10014 	sw	$s1,20($sp)
    2954:	afb00010 	sw	$s0,16($sp)
    2958:	00a09021 	move	$s2,$a1
    295c:	94900000 	lhu	$s0,0($a0)
    2960:	ae400000 	sw	$zero,0($s2)
    2964:	24040005 	li	$a0,5
    2968:	3c110000 	lui	$s1,0x0
    296c:	26312f88 	addiu	$s1,$s1,12168
    2970:	0220f809 	jalr	$s1
    2974:	02002821 	move	$a1,$s0
    2978:	30420001 	andi	$v0,$v0,0x1
    297c:	10400009 	beqz	$v0,29a4 <si3050_hw_reset+0x2ec>
    2980:	2404000c 	li	$a0,12
    2984:	0220f809 	jalr	$s1
    2988:	02002821 	move	$a1,$s0
    298c:	3042001f 	andi	$v0,$v0,0x1f
    2990:	2c420003 	sltiu	$v0,$v0,3
    2994:	14400004 	bnez	$v0,29a8 <si3050_hw_reset+0x2f0>
    2998:	8fbf001c 	lw	$ra,28($sp)
    299c:	24020001 	li	$v0,1
    29a0:	ae420000 	sw	$v0,0($s2)
    29a4:	8fbf001c 	lw	$ra,28($sp)
    29a8:	8fb20018 	lw	$s2,24($sp)
    29ac:	8fb10014 	lw	$s1,20($sp)
    29b0:	8fb00010 	lw	$s0,16($sp)
    29b4:	03e00008 	jr	$ra
    29b8:	27bd0020 	addiu	$sp,$sp,32
    29bc:	27bdffe0 	addiu	$sp,$sp,-32
    29c0:	afbf001c 	sw	$ra,28($sp)
    29c4:	afb20018 	sw	$s2,24($sp)
    29c8:	afb10014 	sw	$s1,20($sp)
    29cc:	afb00010 	sw	$s0,16($sp)
    29d0:	94900000 	lhu	$s0,0($a0)
    29d4:	24040005 	li	$a0,5
    29d8:	00a09021 	move	$s2,$a1
    29dc:	3c110000 	lui	$s1,0x0
    29e0:	26312f88 	addiu	$s1,$s1,12168
    29e4:	0220f809 	jalr	$s1
    29e8:	02002821 	move	$a1,$s0
    29ec:	30420001 	andi	$v0,$v0,0x1
    29f0:	10400003 	beqz	$v0,2a00 <si3050_hw_reset+0x348>
    29f4:	24040005 	li	$a0,5
    29f8:	08000a8e 	j	2a38 <si3050_hw_reset+0x380>
    29fc:	ae400000 	sw	$zero,0($s2)
    2a00:	0220f809 	jalr	$s1
    2a04:	02002821 	move	$a1,$s0
    2a08:	00408021 	move	$s0,$v0
    2a0c:	240200ff 	li	$v0,255
    2a10:	16020008 	bne	$s0,$v0,2a34 <si3050_hw_reset+0x37c>
    2a14:	32020004 	andi	$v0,$s0,0x4
    2a18:	3c040000 	lui	$a0,0x0
    2a1c:	2484096c 	addiu	$a0,$a0,2412
    2a20:	3c020000 	lui	$v0,0x0
    2a24:	24420000 	addiu	$v0,$v0,0
    2a28:	0040f809 	jalr	$v0
    2a2c:	00000000 	nop
    2a30:	32020004 	andi	$v0,$s0,0x4
    2a34:	ae420000 	sw	$v0,0($s2)
    2a38:	8fbf001c 	lw	$ra,28($sp)
    2a3c:	8fb20018 	lw	$s2,24($sp)
    2a40:	8fb10014 	lw	$s1,20($sp)
    2a44:	8fb00010 	lw	$s0,16($sp)
    2a48:	03e00008 	jr	$ra
    2a4c:	27bd0020 	addiu	$sp,$sp,32
    2a50:	27bdffe8 	addiu	$sp,$sp,-24
    2a54:	afbf0010 	sw	$ra,16($sp)
    2a58:	94860000 	lhu	$a2,0($a0)
    2a5c:	24040005 	li	$a0,5
    2a60:	3c020000 	lui	$v0,0x0
    2a64:	24422ee4 	addiu	$v0,$v0,12004
    2a68:	0040f809 	jalr	$v0
    2a6c:	0005282b 	sltu	$a1,$zero,$a1
    2a70:	8fbf0010 	lw	$ra,16($sp)
    2a74:	03e00008 	jr	$ra
    2a78:	27bd0018 	addiu	$sp,$sp,24

0000000000002a7c <si3050_hw_gain_control>:
    2a7c:	27bdffe0 	addiu	$sp,$sp,-32
    2a80:	afbf001c 	sw	$ra,28($sp)
    2a84:	afb20018 	sw	$s2,24($sp)
    2a88:	afb10014 	sw	$s1,20($sp)
    2a8c:	afb00010 	sw	$s0,16($sp)
    2a90:	3091ffff 	andi	$s1,$a0,0xffff
    2a94:	30a500ff 	andi	$a1,$a1,0xff
    2a98:	10e0000c 	beqz	$a3,2acc <si3050_hw_gain_control+0x50>
    2a9c:	30d200ff 	andi	$s2,$a2,0xff
    2aa0:	24040026 	li	$a0,38
    2aa4:	3c100000 	lui	$s0,0x0
    2aa8:	26102ee4 	addiu	$s0,$s0,12004
    2aac:	0200f809 	jalr	$s0
    2ab0:	02203021 	move	$a2,$s1
    2ab4:	24040028 	li	$a0,40
    2ab8:	02402821 	move	$a1,$s2
    2abc:	0200f809 	jalr	$s0
    2ac0:	02203021 	move	$a2,$s1
    2ac4:	08000abd 	j	2af4 <si3050_hw_gain_control+0x78>
    2ac8:	8fbf001c 	lw	$ra,28($sp)
    2acc:	24040027 	li	$a0,39
    2ad0:	3c100000 	lui	$s0,0x0
    2ad4:	26102ee4 	addiu	$s0,$s0,12004
    2ad8:	0200f809 	jalr	$s0
    2adc:	02203021 	move	$a2,$s1
    2ae0:	24040029 	li	$a0,41
    2ae4:	02402821 	move	$a1,$s2
    2ae8:	0200f809 	jalr	$s0
    2aec:	02203021 	move	$a2,$s1
    2af0:	8fbf001c 	lw	$ra,28($sp)
    2af4:	8fb20018 	lw	$s2,24($sp)
    2af8:	8fb10014 	lw	$s1,20($sp)
    2afc:	8fb00010 	lw	$s0,16($sp)
    2b00:	03e00008 	jr	$ra
    2b04:	27bd0020 	addiu	$sp,$sp,32
    2b08:	27bdffe8 	addiu	$sp,$sp,-24
    2b0c:	afbf0014 	sw	$ra,20($sp)
    2b10:	afb00010 	sw	$s0,16($sp)
    2b14:	24020001 	li	$v0,1
    2b18:	14a2000d 	bne	$a1,$v0,2b50 <si3050_hw_gain_control+0xd4>
    2b1c:	94900000 	lhu	$s0,0($a0)
    2b20:	3c020000 	lui	$v0,0x0
    2b24:	24422a50 	addiu	$v0,$v0,10832
    2b28:	0040f809 	jalr	$v0
    2b2c:	00002821 	move	$a1,$zero
    2b30:	24040006 	li	$a0,6
    2b34:	24050010 	li	$a1,16
    2b38:	3c020000 	lui	$v0,0x0
    2b3c:	24422ee4 	addiu	$v0,$v0,12004
    2b40:	0040f809 	jalr	$v0
    2b44:	02003021 	move	$a2,$s0
    2b48:	08000adb 	j	2b6c <si3050_hw_gain_control+0xf0>
    2b4c:	8fbf0014 	lw	$ra,20($sp)
    2b50:	24040006 	li	$a0,6
    2b54:	00002821 	move	$a1,$zero
    2b58:	3c020000 	lui	$v0,0x0
    2b5c:	24422ee4 	addiu	$v0,$v0,12004
    2b60:	0040f809 	jalr	$v0
    2b64:	02003021 	move	$a2,$s0
    2b68:	8fbf0014 	lw	$ra,20($sp)
    2b6c:	8fb00010 	lw	$s0,16($sp)
    2b70:	24020001 	li	$v0,1
    2b74:	03e00008 	jr	$ra
    2b78:	27bd0018 	addiu	$sp,$sp,24
    2b7c:	27bdffe8 	addiu	$sp,$sp,-24
    2b80:	14a00003 	bnez	$a1,2b90 <si3050_hw_gain_control+0x114>
    2b84:	afbf0010 	sw	$ra,16($sp)
    2b88:	08000ae8 	j	2ba0 <si3050_hw_gain_control+0x124>
    2b8c:	24020001 	li	$v0,1
    2b90:	3c020000 	lui	$v0,0x0
    2b94:	24422b08 	addiu	$v0,$v0,11016
    2b98:	0040f809 	jalr	$v0
    2b9c:	00002821 	move	$a1,$zero
    2ba0:	8fbf0010 	lw	$ra,16($sp)
    2ba4:	03e00008 	jr	$ra
    2ba8:	27bd0018 	addiu	$sp,$sp,24

0000000000002bac <si3050_set_lowpwr_path>:
    2bac:	27bdffe8 	addiu	$sp,$sp,-24
    2bb0:	afbf0010 	sw	$ra,16($sp)
    2bb4:	3086ffff 	andi	$a2,$a0,0xffff
    2bb8:	24040005 	li	$a0,5
    2bbc:	3c020000 	lui	$v0,0x0
    2bc0:	24422ee4 	addiu	$v0,$v0,12004
    2bc4:	0040f809 	jalr	$v0
    2bc8:	24050008 	li	$a1,8
    2bcc:	8fbf0010 	lw	$ra,16($sp)
    2bd0:	03e00008 	jr	$ra
    2bd4:	27bd0018 	addiu	$sp,$sp,24

0000000000002bd8 <si3050_clear_lowpwr_path>:
    2bd8:	27bdffe8 	addiu	$sp,$sp,-24
    2bdc:	afbf0010 	sw	$ra,16($sp)
    2be0:	3086ffff 	andi	$a2,$a0,0xffff
    2be4:	24040005 	li	$a0,5
    2be8:	3c020000 	lui	$v0,0x0
    2bec:	24422ee4 	addiu	$v0,$v0,12004
    2bf0:	0040f809 	jalr	$v0
    2bf4:	00002821 	move	$a1,$zero
    2bf8:	8fbf0010 	lw	$ra,16($sp)
    2bfc:	03e00008 	jr	$ra
    2c00:	27bd0018 	addiu	$sp,$sp,24
    2c04:	27bdffe8 	addiu	$sp,$sp,-24
    2c08:	afbf0014 	sw	$ra,20($sp)
    2c0c:	afb00010 	sw	$s0,16($sp)
    2c10:	00801821 	move	$v1,$a0
    2c14:	24022712 	li	$v0,10002
    2c18:	10a20015 	beq	$a1,$v0,2c70 <si3050_clear_lowpwr_path+0x98>
    2c1c:	00c08021 	move	$s0,$a2
    2c20:	28a22713 	slti	$v0,$a1,10003
    2c24:	10400005 	beqz	$v0,2c3c <si3050_clear_lowpwr_path+0x64>
    2c28:	24022711 	li	$v0,10001
    2c2c:	10a2000a 	beq	$a1,$v0,2c58 <si3050_clear_lowpwr_path+0x80>
    2c30:	8fbf0014 	lw	$ra,20($sp)
    2c34:	08000b31 	j	2cc4 <si3050_clear_lowpwr_path+0xec>
    2c38:	8fb00010 	lw	$s0,16($sp)
    2c3c:	2402271b 	li	$v0,10011
    2c40:	10a20018 	beq	$a1,$v0,2ca4 <si3050_clear_lowpwr_path+0xcc>
    2c44:	2402271d 	li	$v0,10013
    2c48:	10a2000f 	beq	$a1,$v0,2c88 <si3050_clear_lowpwr_path+0xb0>
    2c4c:	8fbf0014 	lw	$ra,20($sp)
    2c50:	08000b31 	j	2cc4 <si3050_clear_lowpwr_path+0xec>
    2c54:	8fb00010 	lw	$s0,16($sp)
    2c58:	3c020000 	lui	$v0,0x0
    2c5c:	24422bac 	addiu	$v0,$v0,11180
    2c60:	0040f809 	jalr	$v0
    2c64:	94640000 	lhu	$a0,0($v1)
    2c68:	08000b30 	j	2cc0 <si3050_clear_lowpwr_path+0xe8>
    2c6c:	8fbf0014 	lw	$ra,20($sp)
    2c70:	3c020000 	lui	$v0,0x0
    2c74:	24422bd8 	addiu	$v0,$v0,11224
    2c78:	0040f809 	jalr	$v0
    2c7c:	94640000 	lhu	$a0,0($v1)
    2c80:	08000b30 	j	2cc0 <si3050_clear_lowpwr_path+0xe8>
    2c84:	8fbf0014 	lw	$ra,20($sp)
    2c88:	92040000 	lbu	$a0,0($s0)
    2c8c:	3c020000 	lui	$v0,0x0
    2c90:	24422f88 	addiu	$v0,$v0,12168
    2c94:	0040f809 	jalr	$v0
    2c98:	94650000 	lhu	$a1,0($v1)
    2c9c:	08000b2f 	j	2cbc <si3050_clear_lowpwr_path+0xe4>
    2ca0:	a2020002 	sb	$v0,2($s0)
    2ca4:	92040000 	lbu	$a0,0($s0)
    2ca8:	92050002 	lbu	$a1,2($s0)
    2cac:	3c020000 	lui	$v0,0x0
    2cb0:	24422ee4 	addiu	$v0,$v0,12004
    2cb4:	0040f809 	jalr	$v0
    2cb8:	94660000 	lhu	$a2,0($v1)
    2cbc:	8fbf0014 	lw	$ra,20($sp)
    2cc0:	8fb00010 	lw	$s0,16($sp)
    2cc4:	03e00008 	jr	$ra
    2cc8:	27bd0018 	addiu	$sp,$sp,24
    2ccc:	27bdffe8 	addiu	$sp,$sp,-24
    2cd0:	afbf0010 	sw	$ra,16($sp)
    2cd4:	3c020000 	lui	$v0,0x0
    2cd8:	24422b08 	addiu	$v0,$v0,11016
    2cdc:	0040f809 	jalr	$v0
    2ce0:	00002821 	move	$a1,$zero
    2ce4:	8fbf0010 	lw	$ra,16($sp)
    2ce8:	03e00008 	jr	$ra
    2cec:	27bd0018 	addiu	$sp,$sp,24
    2cf0:	03e00008 	jr	$ra
    2cf4:	24020001 	li	$v0,1
    2cf8:	27bdffe0 	addiu	$sp,$sp,-32
    2cfc:	afbf0018 	sw	$ra,24($sp)
    2d00:	afb10014 	sw	$s1,20($sp)
    2d04:	afb00010 	sw	$s0,16($sp)
    2d08:	94850000 	lhu	$a1,0($a0)
    2d0c:	3c020000 	lui	$v0,0x0
    2d10:	24422f88 	addiu	$v0,$v0,12168
    2d14:	0040f809 	jalr	$v0
    2d18:	2404000b 	li	$a0,11
    2d1c:	00408821 	move	$s1,$v0
    2d20:	00111102 	srl	$v0,$s1,0x4
    2d24:	304200ff 	andi	$v0,$v0,0xff
    2d28:	2c420004 	sltiu	$v0,$v0,4
    2d2c:	14400008 	bnez	$v0,2d50 <si3050_clear_lowpwr_path+0x178>
    2d30:	00000000 	nop
    2d34:	3231000f 	andi	$s1,$s1,0xf
    2d38:	3c040000 	lui	$a0,0x0
    2d3c:	248408f8 	addiu	$a0,$a0,2296
    2d40:	3c020000 	lui	$v0,0x0
    2d44:	24420000 	addiu	$v0,$v0,0
    2d48:	0040f809 	jalr	$v0
    2d4c:	02202821 	move	$a1,$s1
    2d50:	3c100000 	lui	$s0,0x0
    2d54:	26100040 	addiu	$s0,$s0,64
    2d58:	02002021 	move	$a0,$s0
    2d5c:	3c050000 	lui	$a1,0x0
    2d60:	24a50998 	addiu	$a1,$a1,2456
    2d64:	00111102 	srl	$v0,$s1,0x4
    2d68:	3042000f 	andi	$v0,$v0,0xf
    2d6c:	00023880 	sll	$a3,$v0,0x2
    2d70:	00e23821 	addu	$a3,$a3,$v0
    2d74:	3c010000 	lui	$at,0x0
    2d78:	24210110 	addiu	$at,$at,272
    2d7c:	00273821 	addu	$a3,$at,$a3
    2d80:	3c020000 	lui	$v0,0x0
    2d84:	24420000 	addiu	$v0,$v0,0
    2d88:	0040f809 	jalr	$v0
    2d8c:	3226000f 	andi	$a2,$s1,0xf
    2d90:	02001021 	move	$v0,$s0
    2d94:	8fbf0018 	lw	$ra,24($sp)
    2d98:	8fb10014 	lw	$s1,20($sp)
    2d9c:	8fb00010 	lw	$s0,16($sp)
    2da0:	03e00008 	jr	$ra
    2da4:	27bd0020 	addiu	$sp,$sp,32

0000000000002da8 <si3050_api_init>:
    2da8:	3c030000 	lui	$v1,0x0
    2dac:	8c630124 	lw	$v1,292($v1)
    2db0:	24020001 	li	$v0,1
    2db4:	1462002b 	bne	$v1,$v0,2e64 <si3050_api_init+0xbc>
    2db8:	00000000 	nop
    2dbc:	3c010000 	lui	$at,0x0
    2dc0:	ac200124 	sw	$zero,292($at)
    2dc4:	3c030000 	lui	$v1,0x0
    2dc8:	24630004 	addiu	$v1,$v1,4
    2dcc:	8c640000 	lw	$a0,0($v1)
    2dd0:	3c020000 	lui	$v0,0x0
    2dd4:	2442012c 	addiu	$v0,$v0,300
    2dd8:	ac440000 	sw	$a0,0($v0)
    2ddc:	8c64000c 	lw	$a0,12($v1)
    2de0:	8c65001c 	lw	$a1,28($v1)
    2de4:	ac44000c 	sw	$a0,12($v0)
    2de8:	ac45001c 	sw	$a1,28($v0)
    2dec:	8c640024 	lw	$a0,36($v1)
    2df0:	8c65002c 	lw	$a1,44($v1)
    2df4:	ac440024 	sw	$a0,36($v0)
    2df8:	ac45002c 	sw	$a1,44($v0)
    2dfc:	8c640030 	lw	$a0,48($v1)
    2e00:	8c650034 	lw	$a1,52($v1)
    2e04:	ac440030 	sw	$a0,48($v0)
    2e08:	ac450034 	sw	$a1,52($v0)
    2e0c:	8c640038 	lw	$a0,56($v1)
    2e10:	8c65003c 	lw	$a1,60($v1)
    2e14:	ac440038 	sw	$a0,56($v0)
    2e18:	ac45003c 	sw	$a1,60($v0)
    2e1c:	8c640040 	lw	$a0,64($v1)
    2e20:	8c650044 	lw	$a1,68($v1)
    2e24:	ac440040 	sw	$a0,64($v0)
    2e28:	ac450044 	sw	$a1,68($v0)
    2e2c:	8c640048 	lw	$a0,72($v1)
    2e30:	8c65004c 	lw	$a1,76($v1)
    2e34:	ac440048 	sw	$a0,72($v0)
    2e38:	ac45004c 	sw	$a1,76($v0)
    2e3c:	8c640050 	lw	$a0,80($v1)
    2e40:	8c650054 	lw	$a1,84($v1)
    2e44:	ac440050 	sw	$a0,80($v0)
    2e48:	ac450054 	sw	$a1,84($v0)
    2e4c:	8c640058 	lw	$a0,88($v1)
    2e50:	8c65005c 	lw	$a1,92($v1)
    2e54:	ac440058 	sw	$a0,88($v0)
    2e58:	ac45005c 	sw	$a1,92($v0)
    2e5c:	8c630060 	lw	$v1,96($v1)
    2e60:	ac430060 	sw	$v1,96($v0)
    2e64:	03e00008 	jr	$ra
    2e68:	00000000 	nop
    2e6c:	3084ffff 	andi	$a0,$a0,0xffff
    2e70:	000410c0 	sll	$v0,$a0,0x3
    2e74:	00441021 	addu	$v0,$v0,$a0
    2e78:	00021080 	sll	$v0,$v0,0x2
    2e7c:	00441023 	subu	$v0,$v0,$a0
    2e80:	3c030000 	lui	$v1,0x0
    2e84:	8c630004 	lw	$v1,4($v1)
    2e88:	00021080 	sll	$v0,$v0,0x2
    2e8c:	00431021 	addu	$v0,$v0,$v1
    2e90:	90420003 	lbu	$v0,3($v0)
    2e94:	3c030000 	lui	$v1,0x0
    2e98:	8c630018 	lw	$v1,24($v1)
    2e9c:	00021080 	sll	$v0,$v0,0x2
    2ea0:	00431021 	addu	$v0,$v0,$v1
    2ea4:	8c420000 	lw	$v0,0($v0)
    2ea8:	90420000 	lbu	$v0,0($v0)
    2eac:	00822023 	subu	$a0,$a0,$v0
    2eb0:	308400ff 	andi	$a0,$a0,0xff
    2eb4:	30820001 	andi	$v0,$a0,0x1
    2eb8:	000210c0 	sll	$v0,$v0,0x3
    2ebc:	30830002 	andi	$v1,$a0,0x2
    2ec0:	00031840 	sll	$v1,$v1,0x1
    2ec4:	00431025 	or	$v0,$v0,$v1
    2ec8:	30830004 	andi	$v1,$a0,0x4
    2ecc:	00031842 	srl	$v1,$v1,0x1
    2ed0:	00431025 	or	$v0,$v0,$v1
    2ed4:	30840008 	andi	$a0,$a0,0x8
    2ed8:	000420c2 	srl	$a0,$a0,0x3
    2edc:	03e00008 	jr	$ra
    2ee0:	00441025 	or	$v0,$v0,$a0

0000000000002ee4 <si3050_write_reg>:
    2ee4:	27bdffd8 	addiu	$sp,$sp,-40
    2ee8:	afbf0024 	sw	$ra,36($sp)
    2eec:	afb20020 	sw	$s2,32($sp)
    2ef0:	afb1001c 	sw	$s1,28($sp)
    2ef4:	afb00018 	sw	$s0,24($sp)
    2ef8:	24070020 	li	$a3,32
    2efc:	309200ff 	andi	$s2,$a0,0xff
    2f00:	30d0ffff 	andi	$s0,$a2,0xffff
    2f04:	001010c0 	sll	$v0,$s0,0x3
    2f08:	00501021 	addu	$v0,$v0,$s0
    2f0c:	00021080 	sll	$v0,$v0,0x2
    2f10:	00501023 	subu	$v0,$v0,$s0
    2f14:	3c030000 	lui	$v1,0x0
    2f18:	8c630004 	lw	$v1,4($v1)
    2f1c:	00021080 	sll	$v0,$v0,0x2
    2f20:	00431021 	addu	$v0,$v0,$v1
    2f24:	8c420040 	lw	$v0,64($v0)
    2f28:	10400007 	beqz	$v0,2f48 <si3050_write_reg+0x64>
    2f2c:	30b100ff 	andi	$s1,$a1,0xff
    2f30:	3c020000 	lui	$v0,0x0
    2f34:	24422e6c 	addiu	$v0,$v0,11884
    2f38:	0040f809 	jalr	$v0
    2f3c:	02002021 	move	$a0,$s0
    2f40:	34420020 	ori	$v0,$v0,0x20
    2f44:	304700ff 	andi	$a3,$v0,0xff
    2f48:	a3b20010 	sb	$s2,16($sp)
    2f4c:	a3b10011 	sb	$s1,17($sp)
    2f50:	02002021 	move	$a0,$s0
    2f54:	00e02821 	move	$a1,$a3
    2f58:	3c020000 	lui	$v0,0x0
    2f5c:	8c420000 	lw	$v0,0($v0)
    2f60:	27a60010 	addiu	$a2,$sp,16
    2f64:	8c420008 	lw	$v0,8($v0)
    2f68:	0040f809 	jalr	$v0
    2f6c:	24070002 	li	$a3,2
    2f70:	8fbf0024 	lw	$ra,36($sp)
    2f74:	8fb20020 	lw	$s2,32($sp)
    2f78:	8fb1001c 	lw	$s1,28($sp)
    2f7c:	8fb00018 	lw	$s0,24($sp)
    2f80:	03e00008 	jr	$ra
    2f84:	27bd0028 	addiu	$sp,$sp,40

0000000000002f88 <si3050_read_reg>:
    2f88:	27bdffd8 	addiu	$sp,$sp,-40
    2f8c:	afbf0020 	sw	$ra,32($sp)
    2f90:	afb1001c 	sw	$s1,28($sp)
    2f94:	afb00018 	sw	$s0,24($sp)
    2f98:	24060060 	li	$a2,96
    2f9c:	30b0ffff 	andi	$s0,$a1,0xffff
    2fa0:	001010c0 	sll	$v0,$s0,0x3
    2fa4:	00501021 	addu	$v0,$v0,$s0
    2fa8:	00021080 	sll	$v0,$v0,0x2
    2fac:	00501023 	subu	$v0,$v0,$s0
    2fb0:	3c030000 	lui	$v1,0x0
    2fb4:	8c630004 	lw	$v1,4($v1)
    2fb8:	00021080 	sll	$v0,$v0,0x2
    2fbc:	00431021 	addu	$v0,$v0,$v1
    2fc0:	8c420040 	lw	$v0,64($v0)
    2fc4:	10400007 	beqz	$v0,2fe4 <si3050_read_reg+0x5c>
    2fc8:	309100ff 	andi	$s1,$a0,0xff
    2fcc:	3c020000 	lui	$v0,0x0
    2fd0:	24422e6c 	addiu	$v0,$v0,11884
    2fd4:	0040f809 	jalr	$v0
    2fd8:	02002021 	move	$a0,$s0
    2fdc:	34420060 	ori	$v0,$v0,0x60
    2fe0:	304600ff 	andi	$a2,$v0,0xff
    2fe4:	02002021 	move	$a0,$s0
    2fe8:	00c02821 	move	$a1,$a2
    2fec:	3c020000 	lui	$v0,0x0
    2ff0:	8c420000 	lw	$v0,0($v0)
    2ff4:	00003021 	move	$a2,$zero
    2ff8:	8c420008 	lw	$v0,8($v0)
    2ffc:	0040f809 	jalr	$v0
    3000:	00003821 	move	$a3,$zero
    3004:	02002021 	move	$a0,$s0
    3008:	02202821 	move	$a1,$s1
    300c:	3c020000 	lui	$v0,0x0
    3010:	8c420000 	lw	$v0,0($v0)
    3014:	27a60010 	addiu	$a2,$sp,16
    3018:	8c420004 	lw	$v0,4($v0)
    301c:	0040f809 	jalr	$v0
    3020:	24070001 	li	$a3,1
    3024:	93a20010 	lbu	$v0,16($sp)
    3028:	8fbf0020 	lw	$ra,32($sp)
    302c:	8fb1001c 	lw	$s1,28($sp)
    3030:	8fb00018 	lw	$s0,24($sp)
    3034:	03e00008 	jr	$ra
    3038:	27bd0028 	addiu	$sp,$sp,40

000000000000303c <si3050_set_daisychain>:
    303c:	3084ffff 	andi	$a0,$a0,0xffff
    3040:	3c030000 	lui	$v1,0x0
    3044:	8c630004 	lw	$v1,4($v1)
    3048:	000410c0 	sll	$v0,$a0,0x3
    304c:	00441021 	addu	$v0,$v0,$a0
    3050:	00021080 	sll	$v0,$v0,0x2
    3054:	00441023 	subu	$v0,$v0,$a0
    3058:	00021080 	sll	$v0,$v0,0x2
    305c:	00431021 	addu	$v0,$v0,$v1
    3060:	03e00008 	jr	$ra
    3064:	ac450040 	sw	$a1,64($v0)
	...
    3070:	27bdffc0 	addiu	$sp,$sp,-64
    3074:	afbf003c 	sw	$ra,60($sp)
    3078:	afb60038 	sw	$s6,56($sp)
    307c:	afb50034 	sw	$s5,52($sp)
    3080:	afb40030 	sw	$s4,48($sp)
    3084:	afb3002c 	sw	$s3,44($sp)
    3088:	afb20028 	sw	$s2,40($sp)
    308c:	afb10024 	sw	$s1,36($sp)
    3090:	afb00020 	sw	$s0,32($sp)
    3094:	24160001 	li	$s6,1
    3098:	0080a021 	move	$s4,$a0
    309c:	3c020000 	lui	$v0,0x0
    30a0:	8c420404 	lw	$v0,1028($v0)
    30a4:	10400003 	beqz	$v0,30b4 <si3050_set_daisychain+0x78>
    30a8:	96930000 	lhu	$s3,0($s4)
    30ac:	3c010000 	lui	$at,0x0
    30b0:	ac200404 	sw	$zero,1028($at)
    30b4:	00131080 	sll	$v0,$s3,0x2
    30b8:	3c010000 	lui	$at,0x0
    30bc:	00220821 	addu	$at,$at,$v0
    30c0:	ac200080 	sw	$zero,128($at)
    30c4:	02802021 	move	$a0,$s4
    30c8:	92820003 	lbu	$v0,3($s4)
    30cc:	3c030000 	lui	$v1,0x0
    30d0:	8c63001c 	lw	$v1,28($v1)
    30d4:	00021080 	sll	$v0,$v0,0x2
    30d8:	00431021 	addu	$v0,$v0,$v1
    30dc:	8c420000 	lw	$v0,0($v0)
    30e0:	24051001 	li	$a1,4097
    30e4:	8c42000c 	lw	$v0,12($v0)
    30e8:	0040f809 	jalr	$v0
    30ec:	00003021 	move	$a2,$zero
    30f0:	24040080 	li	$a0,128
    30f4:	3c020000 	lui	$v0,0x0
    30f8:	24420000 	addiu	$v0,$v0,0
    30fc:	0040f809 	jalr	$v0
    3100:	02602821 	move	$a1,$s3
    3104:	3c040000 	lui	$a0,0x0
    3108:	24840d8c 	addiu	$a0,$a0,3468
    310c:	02602821 	move	$a1,$s3
    3110:	00409021 	move	$s2,$v0
    3114:	00123102 	srl	$a2,$s2,0x4
    3118:	30c60003 	andi	$a2,$a2,0x3
    311c:	3250000f 	andi	$s0,$s2,0xf
    3120:	3211ffff 	andi	$s1,$s0,0xffff
    3124:	3c150000 	lui	$s5,0x0
    3128:	26b50000 	addiu	$s5,$s5,0
    312c:	02a0f809 	jalr	$s5
    3130:	02203821 	move	$a3,$s1
    3134:	12200004 	beqz	$s1,3148 <si3050_set_daisychain+0x10c>
    3138:	321000ff 	andi	$s0,$s0,0xff
    313c:	32420030 	andi	$v0,$s2,0x30
    3140:	10400003 	beqz	$v0,3150 <si3050_set_daisychain+0x114>
    3144:	02602021 	move	$a0,$s3
    3148:	08000cd5 	j	3354 <si3050_set_daisychain+0x318>
    314c:	00001021 	move	$v0,$zero
    3150:	3c120000 	lui	$s2,0x0
    3154:	26524078 	addiu	$s2,$s2,16504
    3158:	0240f809 	jalr	$s2
    315c:	24050001 	li	$a1,1
    3160:	24040061 	li	$a0,97
    3164:	00002821 	move	$a1,$zero
    3168:	3c020000 	lui	$v0,0x0
    316c:	24424190 	addiu	$v0,$v0,16784
    3170:	0040f809 	jalr	$v0
    3174:	02603021 	move	$a2,$s3
    3178:	24040008 	li	$a0,8
    317c:	00002821 	move	$a1,$zero
    3180:	3c110000 	lui	$s1,0x0
    3184:	26310000 	addiu	$s1,$s1,0
    3188:	0220f809 	jalr	$s1
    318c:	02603021 	move	$a2,$s3
    3190:	2e020003 	sltiu	$v0,$s0,3
    3194:	14400005 	bnez	$v0,31ac <si3050_set_daisychain+0x170>
    3198:	00000000 	nop
    319c:	2404006c 	li	$a0,108
    31a0:	240500eb 	li	$a1,235
    31a4:	0220f809 	jalr	$s1
    31a8:	02603021 	move	$a2,$s3
    31ac:	3c040000 	lui	$a0,0x0
    31b0:	02a0f809 	jalr	$s5
    31b4:	24840db8 	addiu	$a0,$a0,3512
    31b8:	2404004b 	li	$a0,75
    31bc:	24050010 	li	$a1,16
    31c0:	0220f809 	jalr	$s1
    31c4:	02603021 	move	$a2,$s3
    31c8:	02802021 	move	$a0,$s4
    31cc:	3c020000 	lui	$v0,0x0
    31d0:	24424ad8 	addiu	$v0,$v0,19160
    31d4:	0040f809 	jalr	$v0
    31d8:	26850058 	addiu	$a1,$s4,88
    31dc:	afa0001c 	sw	$zero,28($sp)
    31e0:	afa00018 	sw	$zero,24($sp)
    31e4:	02802021 	move	$a0,$s4
    31e8:	27a50010 	addiu	$a1,$sp,16
    31ec:	8e820074 	lw	$v0,116($s4)
    31f0:	8e830078 	lw	$v1,120($s4)
    31f4:	afa20010 	sw	$v0,16($sp)
    31f8:	3c020000 	lui	$v0,0x0
    31fc:	24424348 	addiu	$v0,$v0,17224
    3200:	0040f809 	jalr	$v0
    3204:	afa30014 	sw	$v1,20($sp)
    3208:	3c020000 	lui	$v0,0x0
    320c:	244238e0 	addiu	$v0,$v0,14560
    3210:	0040f809 	jalr	$v0
    3214:	02802021 	move	$a0,$s4
    3218:	3c040000 	lui	$a0,0x0
    321c:	24840de4 	addiu	$a0,$a0,3556
    3220:	02a0f809 	jalr	$s5
    3224:	00402821 	move	$a1,$v0
    3228:	24040040 	li	$a0,64
    322c:	00002821 	move	$a1,$zero
    3230:	0220f809 	jalr	$s1
    3234:	02603021 	move	$a2,$s3
    3238:	3c040000 	lui	$a0,0x0
    323c:	02a0f809 	jalr	$s5
    3240:	24840df4 	addiu	$a0,$a0,3572
    3244:	ae960050 	sw	$s6,80($s4)
    3248:	02802021 	move	$a0,$s4
    324c:	3c020000 	lui	$v0,0x0
    3250:	24423b08 	addiu	$v0,$v0,15112
    3254:	0040f809 	jalr	$v0
    3258:	24050190 	li	$a1,400
    325c:	02802021 	move	$a0,$s4
    3260:	3c020000 	lui	$v0,0x0
    3264:	24423c18 	addiu	$v0,$v0,15384
    3268:	0040f809 	jalr	$v0
    326c:	24050190 	li	$a1,400
    3270:	14400002 	bnez	$v0,327c <si3050_set_daisychain+0x240>
    3274:	24020004 	li	$v0,4
    3278:	ae820050 	sw	$v0,80($s4)
    327c:	3c020000 	lui	$v0,0x0
    3280:	24423db4 	addiu	$v0,$v0,15796
    3284:	0040f809 	jalr	$v0
    3288:	02602021 	move	$a0,$s3
    328c:	24040017 	li	$a0,23
    3290:	00002821 	move	$a1,$zero
    3294:	0220f809 	jalr	$s1
    3298:	02603021 	move	$a2,$s3
    329c:	24040014 	li	$a0,20
    32a0:	240500ff 	li	$a1,255
    32a4:	0220f809 	jalr	$s1
    32a8:	02603021 	move	$a2,$s3
    32ac:	24040016 	li	$a0,22
    32b0:	240500ff 	li	$a1,255
    32b4:	0220f809 	jalr	$s1
    32b8:	02603021 	move	$a2,$s3
    32bc:	24040013 	li	$a0,19
    32c0:	240500ff 	li	$a1,255
    32c4:	0220f809 	jalr	$s1
    32c8:	02603021 	move	$a2,$s3
    32cc:	24040015 	li	$a0,21
    32d0:	00002821 	move	$a1,$zero
    32d4:	0220f809 	jalr	$s1
    32d8:	02603021 	move	$a2,$s3
    32dc:	24040012 	li	$a0,18
    32e0:	240500ff 	li	$a1,255
    32e4:	0220f809 	jalr	$s1
    32e8:	02603021 	move	$a2,$s3
    32ec:	9684002e 	lhu	$a0,46($s4)
    32f0:	3c020000 	lui	$v0,0x0
    32f4:	24423f70 	addiu	$v0,$v0,16240
    32f8:	0040f809 	jalr	$v0
    32fc:	02602821 	move	$a1,$s3
    3300:	24040041 	li	$a0,65
    3304:	24050060 	li	$a1,96
    3308:	0220f809 	jalr	$s1
    330c:	02603021 	move	$a2,$s3
    3310:	02602021 	move	$a0,$s3
    3314:	0240f809 	jalr	$s2
    3318:	00002821 	move	$a1,$zero
    331c:	3c020000 	lui	$v0,0x0
    3320:	24423e44 	addiu	$v0,$v0,15940
    3324:	0040f809 	jalr	$v0
    3328:	02602021 	move	$a0,$s3
    332c:	14400004 	bnez	$v0,3340 <si3050_set_daisychain+0x304>
    3330:	02802021 	move	$a0,$s4
    3334:	0000b021 	move	$s6,$zero
    3338:	24020002 	li	$v0,2
    333c:	ae820050 	sw	$v0,80($s4)
    3340:	3c020000 	lui	$v0,0x0
    3344:	24423538 	addiu	$v0,$v0,13624
    3348:	0040f809 	jalr	$v0
    334c:	24050001 	li	$a1,1
    3350:	02c01021 	move	$v0,$s6
    3354:	8fbf003c 	lw	$ra,60($sp)
    3358:	8fb60038 	lw	$s6,56($sp)
    335c:	8fb50034 	lw	$s5,52($sp)
    3360:	8fb40030 	lw	$s4,48($sp)
    3364:	8fb3002c 	lw	$s3,44($sp)
    3368:	8fb20028 	lw	$s2,40($sp)
    336c:	8fb10024 	lw	$s1,36($sp)
    3370:	8fb00020 	lw	$s0,32($sp)
    3374:	03e00008 	jr	$ra
    3378:	27bd0040 	addiu	$sp,$sp,64

000000000000337c <poll>:
    337c:	27bdffe0 	addiu	$sp,$sp,-32
    3380:	afbf001c 	sw	$ra,28($sp)
    3384:	afb00018 	sw	$s0,24($sp)
    3388:	27a50010 	addiu	$a1,$sp,16
    338c:	00808021 	move	$s0,$a0
    3390:	3c020000 	lui	$v0,0x0
    3394:	24424e20 	addiu	$v0,$v0,20000
    3398:	0040f809 	jalr	$v0
    339c:	96040000 	lhu	$a0,0($s0)
    33a0:	8e030050 	lw	$v1,80($s0)
    33a4:	24020001 	li	$v0,1
    33a8:	1462000e 	bne	$v1,$v0,33e4 <poll+0x68>
    33ac:	8fbf001c 	lw	$ra,28($sp)
    33b0:	93a20010 	lbu	$v0,16($sp)
    33b4:	1040000b 	beqz	$v0,33e4 <poll+0x68>
    33b8:	24020005 	li	$v0,5
    33bc:	ae020050 	sw	$v0,80($s0)
    33c0:	240203e8 	li	$v0,1000
    33c4:	ae020054 	sw	$v0,84($s0)
    33c8:	3c040000 	lui	$a0,0x0
    33cc:	24840e18 	addiu	$a0,$a0,3608
    33d0:	3c020000 	lui	$v0,0x0
    33d4:	24420000 	addiu	$v0,$v0,0
    33d8:	0040f809 	jalr	$v0
    33dc:	96050000 	lhu	$a1,0($s0)
    33e0:	8fbf001c 	lw	$ra,28($sp)
    33e4:	8fb00018 	lw	$s0,24($sp)
    33e8:	03e00008 	jr	$ra
    33ec:	27bd0020 	addiu	$sp,$sp,32
    33f0:	27bdffd0 	addiu	$sp,$sp,-48
    33f4:	afbf002c 	sw	$ra,44($sp)
    33f8:	afb40028 	sw	$s4,40($sp)
    33fc:	afb30024 	sw	$s3,36($sp)
    3400:	afb20020 	sw	$s2,32($sp)
    3404:	afb1001c 	sw	$s1,28($sp)
    3408:	afb00018 	sw	$s0,24($sp)
    340c:	94900000 	lhu	$s0,0($a0)
    3410:	240400c4 	li	$a0,196
    3414:	00a0a021 	move	$s4,$a1
    3418:	3c130000 	lui	$s3,0x0
    341c:	26730000 	addiu	$s3,$s3,0
    3420:	0260f809 	jalr	$s3
    3424:	02002821 	move	$a1,$s0
    3428:	02002021 	move	$a0,$s0
    342c:	00409021 	move	$s2,$v0
    3430:	3c020000 	lui	$v0,0x0
    3434:	24424e20 	addiu	$v0,$v0,20000
    3438:	0040f809 	jalr	$v0
    343c:	27a50010 	addiu	$a1,$sp,16
    3440:	93a20010 	lbu	$v0,16($sp)
    3444:	10400005 	beqz	$v0,345c <poll+0xe0>
    3448:	24110001 	li	$s1,1
    344c:	240400c0 	li	$a0,192
    3450:	0260f809 	jalr	$s3
    3454:	02002821 	move	$a1,$s0
    3458:	00408821 	move	$s1,$v0
    345c:	32420003 	andi	$v0,$s2,0x3
    3460:	50400006 	0x50400006
    3464:	ae800000 	sw	$zero,0($s4)
    3468:	12200003 	beqz	$s1,3478 <poll+0xfc>
    346c:	24020001 	li	$v0,1
    3470:	08000d1f 	j	347c <poll+0x100>
    3474:	ae820000 	sw	$v0,0($s4)
    3478:	ae800000 	sw	$zero,0($s4)
    347c:	8fbf002c 	lw	$ra,44($sp)
    3480:	8fb40028 	lw	$s4,40($sp)
    3484:	8fb30024 	lw	$s3,36($sp)
    3488:	8fb20020 	lw	$s2,32($sp)
    348c:	8fb1001c 	lw	$s1,28($sp)
    3490:	8fb00018 	lw	$s0,24($sp)
    3494:	03e00008 	jr	$ra
    3498:	27bd0030 	addiu	$sp,$sp,48
    349c:	27bdffe8 	addiu	$sp,$sp,-24
    34a0:	afbf0014 	sw	$ra,20($sp)
    34a4:	afb00010 	sw	$s0,16($sp)
    34a8:	00a08021 	move	$s0,$a1
    34ac:	94850000 	lhu	$a1,0($a0)
    34b0:	3c020000 	lui	$v0,0x0
    34b4:	24420000 	addiu	$v0,$v0,0
    34b8:	0040f809 	jalr	$v0
    34bc:	240400c4 	li	$a0,196
    34c0:	30420003 	andi	$v0,$v0,0x3
    34c4:	10400003 	beqz	$v0,34d4 <poll+0x158>
    34c8:	24020001 	li	$v0,1
    34cc:	08000d36 	j	34d8 <poll+0x15c>
    34d0:	ae020000 	sw	$v0,0($s0)
    34d4:	ae000000 	sw	$zero,0($s0)
    34d8:	8fbf0014 	lw	$ra,20($sp)
    34dc:	8fb00010 	lw	$s0,16($sp)
    34e0:	03e00008 	jr	$ra
    34e4:	27bd0018 	addiu	$sp,$sp,24
    34e8:	27bdffe8 	addiu	$sp,$sp,-24
    34ec:	afbf0010 	sw	$ra,16($sp)
    34f0:	10a00009 	beqz	$a1,3518 <poll+0x19c>
    34f4:	94860000 	lhu	$a2,0($a0)
    34f8:	00061080 	sll	$v0,$a2,0x2
    34fc:	3c010000 	lui	$at,0x0
    3500:	00220821 	addu	$at,$at,$v0
    3504:	8c220080 	lw	$v0,128($at)
    3508:	10400004 	beqz	$v0,351c <poll+0x1a0>
    350c:	24050001 	li	$a1,1
    3510:	08000d47 	j	351c <poll+0x1a0>
    3514:	24050005 	li	$a1,5
    3518:	24050003 	li	$a1,3
    351c:	3c020000 	lui	$v0,0x0
    3520:	24420000 	addiu	$v0,$v0,0
    3524:	0040f809 	jalr	$v0
    3528:	24040040 	li	$a0,64
    352c:	8fbf0010 	lw	$ra,16($sp)
    3530:	03e00008 	jr	$ra
    3534:	27bd0018 	addiu	$sp,$sp,24
    3538:	27bdffe0 	addiu	$sp,$sp,-32
    353c:	afbf0018 	sw	$ra,24($sp)
    3540:	afb10014 	sw	$s1,20($sp)
    3544:	afb00010 	sw	$s0,16($sp)
    3548:	24060001 	li	$a2,1
    354c:	94900000 	lhu	$s0,0($a0)
    3550:	14a6000a 	bne	$a1,$a2,357c <poll+0x200>
    3554:	8c83001c 	lw	$v1,28($a0)
    3558:	00008821 	move	$s1,$zero
    355c:	2404000e 	li	$a0,14
    3560:	24050010 	li	$a1,16
    3564:	3c020000 	lui	$v0,0x0
    3568:	24420000 	addiu	$v0,$v0,0
    356c:	0040f809 	jalr	$v0
    3570:	02003021 	move	$a2,$s0
    3574:	08000d73 	j	35cc <poll+0x250>
    3578:	24040040 	li	$a0,64
    357c:	00101080 	sll	$v0,$s0,0x2
    3580:	3c010000 	lui	$at,0x0
    3584:	00220821 	addu	$at,$at,$v0
    3588:	8c220080 	lw	$v0,128($at)
    358c:	10400005 	beqz	$v0,35a4 <poll+0x228>
    3590:	24050001 	li	$a1,1
    3594:	14660005 	bne	$v1,$a2,35ac <poll+0x230>
    3598:	24050005 	li	$a1,5
    359c:	08000d6b 	j	35ac <poll+0x230>
    35a0:	24050003 	li	$a1,3
    35a4:	50650001 	0x50650001
    35a8:	24050003 	li	$a1,3
    35ac:	00a08821 	move	$s1,$a1
    35b0:	2404000e 	li	$a0,14
    35b4:	00002821 	move	$a1,$zero
    35b8:	3c020000 	lui	$v0,0x0
    35bc:	24420000 	addiu	$v0,$v0,0
    35c0:	0040f809 	jalr	$v0
    35c4:	02003021 	move	$a2,$s0
    35c8:	24040040 	li	$a0,64
    35cc:	02202821 	move	$a1,$s1
    35d0:	3c020000 	lui	$v0,0x0
    35d4:	24420000 	addiu	$v0,$v0,0
    35d8:	0040f809 	jalr	$v0
    35dc:	02003021 	move	$a2,$s0
    35e0:	8fbf0018 	lw	$ra,24($sp)
    35e4:	8fb10014 	lw	$s1,20($sp)
    35e8:	8fb00010 	lw	$s0,16($sp)
    35ec:	24020001 	li	$v0,1
    35f0:	03e00008 	jr	$ra
    35f4:	27bd0020 	addiu	$sp,$sp,32
    35f8:	27bdffe8 	addiu	$sp,$sp,-24
    35fc:	afbf0010 	sw	$ra,16($sp)
    3600:	24030001 	li	$v1,1
    3604:	10a00002 	beqz	$a1,3610 <poll+0x294>
    3608:	94860000 	lhu	$a2,0($a0)
    360c:	24030005 	li	$v1,5
    3610:	00061080 	sll	$v0,$a2,0x2
    3614:	3c010000 	lui	$at,0x0
    3618:	00220821 	addu	$at,$at,$v0
    361c:	ac250080 	sw	$a1,128($at)
    3620:	24040040 	li	$a0,64
    3624:	3c020000 	lui	$v0,0x0
    3628:	24420000 	addiu	$v0,$v0,0
    362c:	0040f809 	jalr	$v0
    3630:	00602821 	move	$a1,$v1
    3634:	8fbf0010 	lw	$ra,16($sp)
    3638:	03e00008 	jr	$ra
    363c:	27bd0018 	addiu	$sp,$sp,24
    3640:	27bdffd8 	addiu	$sp,$sp,-40
    3644:	afbf0024 	sw	$ra,36($sp)
    3648:	afb20020 	sw	$s2,32($sp)
    364c:	afb1001c 	sw	$s1,28($sp)
    3650:	afb00018 	sw	$s0,24($sp)
    3654:	afa00010 	sw	$zero,16($sp)
    3658:	24120001 	li	$s2,1
    365c:	94900000 	lhu	$s0,0($a0)
    3660:	14b20034 	bne	$a1,$s2,3734 <poll+0x3b8>
    3664:	8c91001c 	lw	$s1,28($a0)
    3668:	3c020000 	lui	$v0,0x0
    366c:	244233f0 	addiu	$v0,$v0,13296
    3670:	0040f809 	jalr	$v0
    3674:	27a50010 	addiu	$a1,$sp,16
    3678:	8fa20010 	lw	$v0,16($sp)
    367c:	1440000e 	bnez	$v0,36b8 <poll+0x33c>
    3680:	240400a2 	li	$a0,162
    3684:	3c020000 	lui	$v0,0x0
    3688:	24420000 	addiu	$v0,$v0,0
    368c:	0040f809 	jalr	$v0
    3690:	02002821 	move	$a1,$s0
    3694:	24040022 	li	$a0,34
    3698:	34420004 	ori	$v0,$v0,0x4
    369c:	304500ff 	andi	$a1,$v0,0xff
    36a0:	3c020000 	lui	$v0,0x0
    36a4:	24420000 	addiu	$v0,$v0,0
    36a8:	0040f809 	jalr	$v0
    36ac:	02003021 	move	$a2,$s0
    36b0:	08000de4 	j	3790 <poll+0x414>
    36b4:	24050004 	li	$a1,4
    36b8:	3c040000 	lui	$a0,0x0
    36bc:	24840e74 	addiu	$a0,$a0,3700
    36c0:	3c020000 	lui	$v0,0x0
    36c4:	24420000 	addiu	$v0,$v0,0
    36c8:	0040f809 	jalr	$v0
    36cc:	02002821 	move	$a1,$s0
    36d0:	240400a2 	li	$a0,162
    36d4:	3c020000 	lui	$v0,0x0
    36d8:	24420000 	addiu	$v0,$v0,0
    36dc:	0040f809 	jalr	$v0
    36e0:	02002821 	move	$a1,$s0
    36e4:	24040022 	li	$a0,34
    36e8:	304500fb 	andi	$a1,$v0,0xfb
    36ec:	3c020000 	lui	$v0,0x0
    36f0:	24420000 	addiu	$v0,$v0,0
    36f4:	0040f809 	jalr	$v0
    36f8:	02003021 	move	$a2,$s0
    36fc:	00101080 	sll	$v0,$s0,0x2
    3700:	3c010000 	lui	$at,0x0
    3704:	00220821 	addu	$at,$at,$v0
    3708:	8c220080 	lw	$v0,128($at)
    370c:	10400005 	beqz	$v0,3724 <poll+0x3a8>
    3710:	00000000 	nop
    3714:	1632001e 	bne	$s1,$s2,3790 <poll+0x414>
    3718:	24050005 	li	$a1,5
    371c:	08000de4 	j	3790 <poll+0x414>
    3720:	24050003 	li	$a1,3
    3724:	1632001a 	bne	$s1,$s2,3790 <poll+0x414>
    3728:	24050001 	li	$a1,1
    372c:	08000de4 	j	3790 <poll+0x414>
    3730:	24050003 	li	$a1,3
    3734:	240400a2 	li	$a0,162
    3738:	3c020000 	lui	$v0,0x0
    373c:	24420000 	addiu	$v0,$v0,0
    3740:	0040f809 	jalr	$v0
    3744:	02002821 	move	$a1,$s0
    3748:	24040022 	li	$a0,34
    374c:	304500fb 	andi	$a1,$v0,0xfb
    3750:	3c020000 	lui	$v0,0x0
    3754:	24420000 	addiu	$v0,$v0,0
    3758:	0040f809 	jalr	$v0
    375c:	02003021 	move	$a2,$s0
    3760:	00101080 	sll	$v0,$s0,0x2
    3764:	3c010000 	lui	$at,0x0
    3768:	00220821 	addu	$at,$at,$v0
    376c:	8c220080 	lw	$v0,128($at)
    3770:	10400005 	beqz	$v0,3788 <poll+0x40c>
    3774:	24050001 	li	$a1,1
    3778:	16320005 	bne	$s1,$s2,3790 <poll+0x414>
    377c:	24050005 	li	$a1,5
    3780:	08000de4 	j	3790 <poll+0x414>
    3784:	24050003 	li	$a1,3
    3788:	52250001 	0x52250001
    378c:	24050003 	li	$a1,3
    3790:	24040040 	li	$a0,64
    3794:	3c020000 	lui	$v0,0x0
    3798:	24420000 	addiu	$v0,$v0,0
    379c:	0040f809 	jalr	$v0
    37a0:	02003021 	move	$a2,$s0
    37a4:	8fbf0024 	lw	$ra,36($sp)
    37a8:	8fb20020 	lw	$s2,32($sp)
    37ac:	8fb1001c 	lw	$s1,28($sp)
    37b0:	8fb00018 	lw	$s0,24($sp)
    37b4:	03e00008 	jr	$ra
    37b8:	27bd0028 	addiu	$sp,$sp,40
    37bc:	27bdffe0 	addiu	$sp,$sp,-32
    37c0:	afbf0018 	sw	$ra,24($sp)
    37c4:	afb10014 	sw	$s1,20($sp)
    37c8:	afb00010 	sw	$s0,16($sp)
    37cc:	94910000 	lhu	$s1,0($a0)
    37d0:	24040093 	li	$a0,147
    37d4:	3c020000 	lui	$v0,0x0
    37d8:	24420000 	addiu	$v0,$v0,0
    37dc:	0040f809 	jalr	$v0
    37e0:	02202821 	move	$a1,$s1
    37e4:	24040013 	li	$a0,19
    37e8:	30500003 	andi	$s0,$v0,0x3
    37ec:	02002821 	move	$a1,$s0
    37f0:	3c020000 	lui	$v0,0x0
    37f4:	24420000 	addiu	$v0,$v0,0
    37f8:	0040f809 	jalr	$v0
    37fc:	02203021 	move	$a2,$s1
    3800:	0010102b 	sltu	$v0,$zero,$s0
    3804:	8fbf0018 	lw	$ra,24($sp)
    3808:	8fb10014 	lw	$s1,20($sp)
    380c:	8fb00010 	lw	$s0,16($sp)
    3810:	03e00008 	jr	$ra
    3814:	27bd0020 	addiu	$sp,$sp,32
    3818:	27bdffe0 	addiu	$sp,$sp,-32
    381c:	afbf001c 	sw	$ra,28($sp)
    3820:	afb20018 	sw	$s2,24($sp)
    3824:	afb10014 	sw	$s1,20($sp)
    3828:	afb00010 	sw	$s0,16($sp)
    382c:	00008821 	move	$s1,$zero
    3830:	3090ffff 	andi	$s0,$a0,0xffff
    3834:	2404005d 	li	$a0,93
    3838:	2405009a 	li	$a1,154
    383c:	3c020000 	lui	$v0,0x0
    3840:	24420000 	addiu	$v0,$v0,0
    3844:	0040f809 	jalr	$v0
    3848:	02003021 	move	$a2,$s0
    384c:	08000e2a 	j	38a8 <poll+0x52c>
    3850:	240400dd 	li	$a0,221
    3854:	34425b00 	ori	$v0,$v0,0x5b00
    3858:	3c030000 	lui	$v1,0x0
    385c:	8c630000 	lw	$v1,0($v1)
    3860:	00430019 	multu	$v0,$v1
    3864:	00001010 	mfhi	$v0
    3868:	1440ffff 	bnez	$v0,3868 <poll+0x4ec>
    386c:	2442ffff 	addiu	$v0,$v0,-1
    3870:	2631000a 	addiu	$s1,$s1,10
    3874:	2a2201f5 	slti	$v0,$s1,501
    3878:	1440000b 	bnez	$v0,38a8 <poll+0x52c>
    387c:	240400dd 	li	$a0,221
    3880:	0240f809 	jalr	$s2
    3884:	02002821 	move	$a1,$s0
    3888:	3c040000 	lui	$a0,0x0
    388c:	24840eb8 	addiu	$a0,$a0,3768
    3890:	00403021 	move	$a2,$v0
    3894:	3c020000 	lui	$v0,0x0
    3898:	24420000 	addiu	$v0,$v0,0
    389c:	0040f809 	jalr	$v0
    38a0:	02002821 	move	$a1,$s0
    38a4:	240400dd 	li	$a0,221
    38a8:	3c120000 	lui	$s2,0x0
    38ac:	26520000 	addiu	$s2,$s2,0
    38b0:	0240f809 	jalr	$s2
    38b4:	02002821 	move	$a1,$s0
    38b8:	30420080 	andi	$v0,$v0,0x80
    38bc:	1440ffe5 	bnez	$v0,3854 <poll+0x4d8>
    38c0:	3c026666 	lui	$v0,0x6666
    38c4:	02201021 	move	$v0,$s1
    38c8:	8fbf001c 	lw	$ra,28($sp)
    38cc:	8fb20018 	lw	$s2,24($sp)
    38d0:	8fb10014 	lw	$s1,20($sp)
    38d4:	8fb00010 	lw	$s0,16($sp)
    38d8:	03e00008 	jr	$ra
    38dc:	27bd0020 	addiu	$sp,$sp,32
    38e0:	27bdffd0 	addiu	$sp,$sp,-48
    38e4:	afbf0028 	sw	$ra,40($sp)
    38e8:	afb50024 	sw	$s5,36($sp)
    38ec:	afb40020 	sw	$s4,32($sp)
    38f0:	afb3001c 	sw	$s3,28($sp)
    38f4:	afb20018 	sw	$s2,24($sp)
    38f8:	afb10014 	sw	$s1,20($sp)
    38fc:	afb00010 	sw	$s0,16($sp)
    3900:	00009021 	move	$s2,$zero
    3904:	0000a821 	move	$s5,$zero
    3908:	94910000 	lhu	$s1,0($a0)
    390c:	2404005c 	li	$a0,92
    3910:	240500ff 	li	$a1,255
    3914:	3c100000 	lui	$s0,0x0
    3918:	26100000 	addiu	$s0,$s0,0
    391c:	0200f809 	jalr	$s0
    3920:	02203021 	move	$a2,$s1
    3924:	2404005d 	li	$a0,93
    3928:	2405000c 	li	$a1,12
    392c:	0200f809 	jalr	$s0
    3930:	02203021 	move	$a2,$s1
    3934:	3c140000 	lui	$s4,0x0
    3938:	26943818 	addiu	$s4,$s4,14360
    393c:	0280f809 	jalr	$s4
    3940:	02202021 	move	$a0,$s1
    3944:	2404000e 	li	$a0,14
    3948:	00002821 	move	$a1,$zero
    394c:	0200f809 	jalr	$s0
    3950:	02203021 	move	$a2,$s1
    3954:	08000e85 	j	3a14 <poll+0x698>
    3958:	240400d2 	li	$a0,210
    395c:	3c021999 	lui	$v0,0x1999
    3960:	344296c0 	ori	$v0,$v0,0x96c0
    3964:	3c030000 	lui	$v1,0x0
    3968:	8c630000 	lw	$v1,0($v1)
    396c:	00430019 	multu	$v0,$v1
    3970:	00001010 	mfhi	$v0
    3974:	1440ffff 	bnez	$v0,3974 <poll+0x5f8>
    3978:	2442ffff 	addiu	$v0,$v0,-1
    397c:	2484ffff 	addiu	$a0,$a0,-1
    3980:	2402ffff 	li	$v0,-1
    3984:	1482fff6 	bne	$a0,$v0,3960 <poll+0x5e4>
    3988:	3c021999 	lui	$v0,0x1999
    398c:	2652000a 	addiu	$s2,$s2,10
    3990:	2a4201f5 	slti	$v0,$s2,501
    3994:	5440001f 	0x5440001f
    3998:	240400d2 	li	$a0,210
    399c:	3c0251eb 	lui	$v0,0x51eb
    39a0:	3442851f 	ori	$v0,$v0,0x851f
    39a4:	02420018 	mult	$s2,$v0
    39a8:	00001010 	mfhi	$v0
    39ac:	00121fc3 	sra	$v1,$s2,0x1f
    39b0:	00021143 	sra	$v0,$v0,0x5
    39b4:	00431023 	subu	$v0,$v0,$v1
    39b8:	00021840 	sll	$v1,$v0,0x1
    39bc:	00621821 	addu	$v1,$v1,$v0
    39c0:	000318c0 	sll	$v1,$v1,0x3
    39c4:	00621821 	addu	$v1,$v1,$v0
    39c8:	00031880 	sll	$v1,$v1,0x2
    39cc:	1643001a 	bne	$s2,$v1,3a38 <poll+0x6bc>
    39d0:	2642ffff 	addiu	$v0,$s2,-1
    39d4:	3c010000 	lui	$at,0x0
    39d8:	d4200fe8 	0xd4200fe8
    39dc:	44931000 	mtc1	$s3,$f2
    39e0:	00000000 	nop
    39e4:	468010a1 	cvt.d.w	$f2,$f2
    39e8:	46201082 	mul.d	$f2,$f2,$f0
    39ec:	3c040000 	lui	$a0,0x0
    39f0:	24840f00 	addiu	$a0,$a0,3840
    39f4:	4620100d 	c1	0x20100d
    39f8:	44060000 	mfc1	$a2,$f0
    39fc:	3c020000 	lui	$v0,0x0
    3a00:	24420000 	addiu	$v0,$v0,0
    3a04:	0040f809 	jalr	$v0
    3a08:	02202821 	move	$a1,$s1
    3a0c:	08000e8e 	j	3a38 <poll+0x6bc>
    3a10:	2642ffff 	addiu	$v0,$s2,-1
    3a14:	3c020000 	lui	$v0,0x0
    3a18:	24420000 	addiu	$v0,$v0,0
    3a1c:	0040f809 	jalr	$v0
    3a20:	02202821 	move	$a1,$s1
    3a24:	00409821 	move	$s3,$v0
    3a28:	2e6200bf 	sltiu	$v0,$s3,191
    3a2c:	1440ffcb 	bnez	$v0,395c <poll+0x5e0>
    3a30:	24040009 	li	$a0,9
    3a34:	2642ffff 	addiu	$v0,$s2,-1
    3a38:	2c4203e7 	sltiu	$v0,$v0,999
    3a3c:	1040000d 	beqz	$v0,3a74 <poll+0x6f8>
    3a40:	02402821 	move	$a1,$s2
    3a44:	3c040000 	lui	$a0,0x0
    3a48:	24840f34 	addiu	$a0,$a0,3892
    3a4c:	3c100000 	lui	$s0,0x0
    3a50:	26100000 	addiu	$s0,$s0,0
    3a54:	0200f809 	jalr	$s0
    3a58:	02203021 	move	$a2,$s1
    3a5c:	3c040000 	lui	$a0,0x0
    3a60:	24840f68 	addiu	$a0,$a0,3944
    3a64:	0200f809 	jalr	$s0
    3a68:	02602821 	move	$a1,$s3
    3a6c:	08000ea4 	j	3a90 <poll+0x714>
    3a70:	2a4201f5 	slti	$v0,$s2,501
    3a74:	3c040000 	lui	$a0,0x0
    3a78:	24840f84 	addiu	$a0,$a0,3972
    3a7c:	3c020000 	lui	$v0,0x0
    3a80:	24420000 	addiu	$v0,$v0,0
    3a84:	0040f809 	jalr	$v0
    3a88:	02203021 	move	$a2,$s1
    3a8c:	2a4201f5 	slti	$v0,$s2,501
    3a90:	10400004 	beqz	$v0,3aa4 <poll+0x728>
    3a94:	00000000 	nop
    3a98:	0280f809 	jalr	$s4
    3a9c:	02202021 	move	$a0,$s1
    3aa0:	0040a821 	move	$s5,$v0
    3aa4:	3c010000 	lui	$at,0x0
    3aa8:	d4200ff0 	0xd4200ff0
    3aac:	44931000 	mtc1	$s3,$f2
    3ab0:	00000000 	nop
    3ab4:	468010a1 	cvt.d.w	$f2,$f2
    3ab8:	46201082 	mul.d	$f2,$f2,$f0
    3abc:	3c040000 	lui	$a0,0x0
    3ac0:	24840fa8 	addiu	$a0,$a0,4008
    3ac4:	02202821 	move	$a1,$s1
    3ac8:	4620100d 	c1	0x20100d
    3acc:	44060000 	mfc1	$a2,$f0
    3ad0:	3c020000 	lui	$v0,0x0
    3ad4:	24420000 	addiu	$v0,$v0,0
    3ad8:	0040f809 	jalr	$v0
    3adc:	02a03821 	move	$a3,$s5
    3ae0:	02601021 	move	$v0,$s3
    3ae4:	8fbf0028 	lw	$ra,40($sp)
    3ae8:	8fb50024 	lw	$s5,36($sp)
    3aec:	8fb40020 	lw	$s4,32($sp)
    3af0:	8fb3001c 	lw	$s3,28($sp)
    3af4:	8fb20018 	lw	$s2,24($sp)
    3af8:	8fb10014 	lw	$s1,20($sp)
    3afc:	8fb00010 	lw	$s0,16($sp)
    3b00:	03e00008 	jr	$ra
    3b04:	27bd0030 	addiu	$sp,$sp,48
    3b08:	27bdffd8 	addiu	$sp,$sp,-40
    3b0c:	afbf0020 	sw	$ra,32($sp)
    3b10:	afb3001c 	sw	$s3,28($sp)
    3b14:	afb20018 	sw	$s2,24($sp)
    3b18:	afb10014 	sw	$s1,20($sp)
    3b1c:	afb00010 	sw	$s0,16($sp)
    3b20:	00009021 	move	$s2,$zero
    3b24:	94910000 	lhu	$s1,0($a0)
    3b28:	24040061 	li	$a0,97
    3b2c:	00a09821 	move	$s3,$a1
    3b30:	2405001e 	li	$a1,30
    3b34:	3c100000 	lui	$s0,0x0
    3b38:	26100000 	addiu	$s0,$s0,0
    3b3c:	0200f809 	jalr	$s0
    3b40:	02203021 	move	$a2,$s1
    3b44:	24040060 	li	$a0,96
    3b48:	24050047 	li	$a1,71
    3b4c:	0200f809 	jalr	$s0
    3b50:	02203021 	move	$a2,$s1
    3b54:	08000ef0 	j	3bc0 <poll+0x844>
    3b58:	240400e0 	li	$a0,224
    3b5c:	3c021999 	lui	$v0,0x1999
    3b60:	344296c0 	ori	$v0,$v0,0x96c0
    3b64:	3c030000 	lui	$v1,0x0
    3b68:	8c630000 	lw	$v1,0($v1)
    3b6c:	00430019 	multu	$v0,$v1
    3b70:	00001010 	mfhi	$v0
    3b74:	1440ffff 	bnez	$v0,3b74 <poll+0x7f8>
    3b78:	2442ffff 	addiu	$v0,$v0,-1
    3b7c:	2484ffff 	addiu	$a0,$a0,-1
    3b80:	2402ffff 	li	$v0,-1
    3b84:	1482fff6 	bne	$a0,$v0,3b60 <poll+0x7e4>
    3b88:	3c021999 	lui	$v0,0x1999
    3b8c:	02401021 	move	$v0,$s2
    3b90:	0262102b 	sltu	$v0,$s3,$v0
    3b94:	10400009 	beqz	$v0,3bbc <poll+0x840>
    3b98:	26520001 	addiu	$s2,$s2,1
    3b9c:	3c040000 	lui	$a0,0x0
    3ba0:	24840ff8 	addiu	$a0,$a0,4088
    3ba4:	3c020000 	lui	$v0,0x0
    3ba8:	24420000 	addiu	$v0,$v0,0
    3bac:	0040f809 	jalr	$v0
    3bb0:	02202821 	move	$a1,$s1
    3bb4:	08000ef7 	j	3bdc <poll+0x860>
    3bb8:	0272102b 	sltu	$v0,$s3,$s2
    3bbc:	240400e0 	li	$a0,224
    3bc0:	3c020000 	lui	$v0,0x0
    3bc4:	24420000 	addiu	$v0,$v0,0
    3bc8:	0040f809 	jalr	$v0
    3bcc:	02202821 	move	$a1,$s1
    3bd0:	1440ffe2 	bnez	$v0,3b5c <poll+0x7e0>
    3bd4:	24040009 	li	$a0,9
    3bd8:	0272102b 	sltu	$v0,$s3,$s2
    3bdc:	14400008 	bnez	$v0,3c00 <poll+0x884>
    3be0:	8fbf0020 	lw	$ra,32($sp)
    3be4:	3c040000 	lui	$a0,0x0
    3be8:	24841028 	addiu	$a0,$a0,4136
    3bec:	3c020000 	lui	$v0,0x0
    3bf0:	24420000 	addiu	$v0,$v0,0
    3bf4:	0040f809 	jalr	$v0
    3bf8:	02202821 	move	$a1,$s1
    3bfc:	8fbf0020 	lw	$ra,32($sp)
    3c00:	8fb3001c 	lw	$s3,28($sp)
    3c04:	8fb20018 	lw	$s2,24($sp)
    3c08:	8fb10014 	lw	$s1,20($sp)
    3c0c:	8fb00010 	lw	$s0,16($sp)
    3c10:	03e00008 	jr	$ra
    3c14:	27bd0028 	addiu	$sp,$sp,40
    3c18:	27bdffd0 	addiu	$sp,$sp,-48
    3c1c:	afbf0028 	sw	$ra,40($sp)
    3c20:	afb30024 	sw	$s3,36($sp)
    3c24:	afb20020 	sw	$s2,32($sp)
    3c28:	afb1001c 	sw	$s1,28($sp)
    3c2c:	afb00018 	sw	$s0,24($sp)
    3c30:	00808021 	move	$s0,$a0
    3c34:	24040017 	li	$a0,23
    3c38:	00a09821 	move	$s3,$a1
    3c3c:	24050004 	li	$a1,4
    3c40:	96110000 	lhu	$s1,0($s0)
    3c44:	3c120000 	lui	$s2,0x0
    3c48:	26520000 	addiu	$s2,$s2,0
    3c4c:	0240f809 	jalr	$s2
    3c50:	02203021 	move	$a2,$s1
    3c54:	24040014 	li	$a0,20
    3c58:	24050004 	li	$a1,4
    3c5c:	0240f809 	jalr	$s2
    3c60:	02203021 	move	$a2,$s1
    3c64:	02002021 	move	$a0,$s0
    3c68:	3c020000 	lui	$v0,0x0
    3c6c:	244233f0 	addiu	$v0,$v0,13296
    3c70:	0040f809 	jalr	$v0
    3c74:	27a50010 	addiu	$a1,$sp,16
    3c78:	8fa20010 	lw	$v0,16($sp)
    3c7c:	10400007 	beqz	$v0,3c9c <poll+0x920>
    3c80:	00008021 	move	$s0,$zero
    3c84:	3c040000 	lui	$a0,0x0
    3c88:	2484104c 	addiu	$a0,$a0,4172
    3c8c:	3c020000 	lui	$v0,0x0
    3c90:	24420000 	addiu	$v0,$v0,0
    3c94:	0040f809 	jalr	$v0
    3c98:	02202821 	move	$a1,$s1
    3c9c:	24040061 	li	$a0,97
    3ca0:	24050001 	li	$a1,1
    3ca4:	0240f809 	jalr	$s2
    3ca8:	02203021 	move	$a2,$s1
    3cac:	24040060 	li	$a0,96
    3cb0:	24050040 	li	$a1,64
    3cb4:	0240f809 	jalr	$s2
    3cb8:	02203021 	move	$a2,$s1
    3cbc:	08000f42 	j	3d08 <poll+0x98c>
    3cc0:	240400e0 	li	$a0,224
    3cc4:	3c021999 	lui	$v0,0x1999
    3cc8:	344296c0 	ori	$v0,$v0,0x96c0
    3ccc:	3c030000 	lui	$v1,0x0
    3cd0:	8c630000 	lw	$v1,0($v1)
    3cd4:	00430019 	multu	$v0,$v1
    3cd8:	00001010 	mfhi	$v0
    3cdc:	1440ffff 	bnez	$v0,3cdc <poll+0x960>
    3ce0:	2442ffff 	addiu	$v0,$v0,-1
    3ce4:	2484ffff 	addiu	$a0,$a0,-1
    3ce8:	2402ffff 	li	$v0,-1
    3cec:	1482fff6 	bne	$a0,$v0,3cc8 <poll+0x94c>
    3cf0:	3c021999 	lui	$v0,0x1999
    3cf4:	02001021 	move	$v0,$s0
    3cf8:	0262102b 	sltu	$v0,$s3,$v0
    3cfc:	1440000d 	bnez	$v0,3d34 <poll+0x9b8>
    3d00:	26100001 	addiu	$s0,$s0,1
    3d04:	240400e0 	li	$a0,224
    3d08:	3c120000 	lui	$s2,0x0
    3d0c:	26520000 	addiu	$s2,$s2,0
    3d10:	0240f809 	jalr	$s2
    3d14:	02202821 	move	$a1,$s1
    3d18:	1440ffea 	bnez	$v0,3cc4 <poll+0x948>
    3d1c:	24040009 	li	$a0,9
    3d20:	0270102b 	sltu	$v0,$s3,$s0
    3d24:	1040000b 	beqz	$v0,3d54 <poll+0x9d8>
    3d28:	00001021 	move	$v0,$zero
    3d2c:	08000f67 	j	3d9c <poll+0xa20>
    3d30:	8fbf0028 	lw	$ra,40($sp)
    3d34:	3c040000 	lui	$a0,0x0
    3d38:	24840ff8 	addiu	$a0,$a0,4088
    3d3c:	3c020000 	lui	$v0,0x0
    3d40:	24420000 	addiu	$v0,$v0,0
    3d44:	0040f809 	jalr	$v0
    3d48:	02202821 	move	$a1,$s1
    3d4c:	08000f49 	j	3d24 <poll+0x9a8>
    3d50:	0270102b 	sltu	$v0,$s3,$s0
    3d54:	3c040000 	lui	$a0,0x0
    3d58:	248410a0 	addiu	$a0,$a0,4256
    3d5c:	3c100000 	lui	$s0,0x0
    3d60:	26100000 	addiu	$s0,$s0,0
    3d64:	0200f809 	jalr	$s0
    3d68:	02202821 	move	$a1,$s1
    3d6c:	24040094 	li	$a0,148
    3d70:	0240f809 	jalr	$s2
    3d74:	02202821 	move	$a1,$s1
    3d78:	30420004 	andi	$v0,$v0,0x4
    3d7c:	10400006 	beqz	$v0,3d98 <poll+0xa1c>
    3d80:	24020001 	li	$v0,1
    3d84:	3c040000 	lui	$a0,0x0
    3d88:	248410d4 	addiu	$a0,$a0,4308
    3d8c:	0200f809 	jalr	$s0
    3d90:	02202821 	move	$a1,$s1
    3d94:	00001021 	move	$v0,$zero
    3d98:	8fbf0028 	lw	$ra,40($sp)
    3d9c:	8fb30024 	lw	$s3,36($sp)
    3da0:	8fb20020 	lw	$s2,32($sp)
    3da4:	8fb1001c 	lw	$s1,28($sp)
    3da8:	8fb00018 	lw	$s0,24($sp)
    3dac:	03e00008 	jr	$ra
    3db0:	27bd0030 	addiu	$sp,$sp,48
    3db4:	27bdffe0 	addiu	$sp,$sp,-32
    3db8:	afbf001c 	sw	$ra,28($sp)
    3dbc:	afb20018 	sw	$s2,24($sp)
    3dc0:	afb10014 	sw	$s1,20($sp)
    3dc4:	afb00010 	sw	$s0,16($sp)
    3dc8:	3091ffff 	andi	$s1,$a0,0xffff
    3dcc:	24100058 	li	$s0,88
    3dd0:	320400ff 	andi	$a0,$s0,0xff
    3dd4:	00002821 	move	$a1,$zero
    3dd8:	3c120000 	lui	$s2,0x0
    3ddc:	26524190 	addiu	$s2,$s2,16784
    3de0:	0240f809 	jalr	$s2
    3de4:	02203021 	move	$a2,$s1
    3de8:	26100001 	addiu	$s0,$s0,1
    3dec:	2a020060 	slti	$v0,$s0,96
    3df0:	1440fff8 	bnez	$v0,3dd4 <poll+0xa58>
    3df4:	320400ff 	andi	$a0,$s0,0xff
    3df8:	24040061 	li	$a0,97
    3dfc:	00002821 	move	$a1,$zero
    3e00:	0240f809 	jalr	$s2
    3e04:	02203021 	move	$a2,$s1
    3e08:	241000c1 	li	$s0,193
    3e0c:	320400ff 	andi	$a0,$s0,0xff
    3e10:	00002821 	move	$a1,$zero
    3e14:	0240f809 	jalr	$s2
    3e18:	02203021 	move	$a2,$s1
    3e1c:	26100001 	addiu	$s0,$s0,1
    3e20:	2a0200d4 	slti	$v0,$s0,212
    3e24:	1440fffa 	bnez	$v0,3e10 <poll+0xa94>
    3e28:	320400ff 	andi	$a0,$s0,0xff
    3e2c:	8fbf001c 	lw	$ra,28($sp)
    3e30:	8fb20018 	lw	$s2,24($sp)
    3e34:	8fb10014 	lw	$s1,20($sp)
    3e38:	8fb00010 	lw	$s0,16($sp)
    3e3c:	03e00008 	jr	$ra
    3e40:	27bd0020 	addiu	$sp,$sp,32
    3e44:	27bdffc8 	addiu	$sp,$sp,-56
    3e48:	afbf0034 	sw	$ra,52($sp)
    3e4c:	afb60030 	sw	$s6,48($sp)
    3e50:	afb5002c 	sw	$s5,44($sp)
    3e54:	afb40028 	sw	$s4,40($sp)
    3e58:	afb30024 	sw	$s3,36($sp)
    3e5c:	afb20020 	sw	$s2,32($sp)
    3e60:	afb1001c 	sw	$s1,28($sp)
    3e64:	afb00018 	sw	$s0,24($sp)
    3e68:	3092ffff 	andi	$s2,$a0,0xffff
    3e6c:	24160001 	li	$s6,1
    3e70:	00009821 	move	$s3,$zero
    3e74:	00131040 	sll	$v0,$s3,0x1
    3e78:	00531021 	addu	$v0,$v0,$s3
    3e7c:	0002a080 	sll	$s4,$v0,0x2
    3e80:	3c150000 	lui	$s5,0x0
    3e84:	26b501a0 	addiu	$s5,$s5,416
    3e88:	02951821 	addu	$v1,$s4,$s5
    3e8c:	02402821 	move	$a1,$s2
    3e90:	3c020000 	lui	$v0,0x0
    3e94:	24424218 	addiu	$v0,$v0,16920
    3e98:	90700000 	lbu	$s0,0($v1)
    3e9c:	94710008 	lhu	$s1,8($v1)
    3ea0:	0040f809 	jalr	$v0
    3ea4:	02002021 	move	$a0,$s0
    3ea8:	00401821 	move	$v1,$v0
    3eac:	10710010 	beq	$v1,$s1,3ef0 <poll+0xb74>
    3eb0:	2602ffed 	addiu	$v0,$s0,-19
    3eb4:	2c420004 	sltiu	$v0,$v0,4
    3eb8:	1440000e 	bnez	$v0,3ef4 <poll+0xb78>
    3ebc:	26620001 	addiu	$v0,$s3,1
    3ec0:	afa30010 	sw	$v1,16($sp)
    3ec4:	afb10014 	sw	$s1,20($sp)
    3ec8:	3c040000 	lui	$a0,0x0
    3ecc:	24841120 	addiu	$a0,$a0,4384
    3ed0:	02402821 	move	$a1,$s2
    3ed4:	0000b021 	move	$s6,$zero
    3ed8:	02b41021 	addu	$v0,$s5,$s4
    3edc:	8c460004 	lw	$a2,4($v0)
    3ee0:	3c020000 	lui	$v0,0x0
    3ee4:	24420000 	addiu	$v0,$v0,0
    3ee8:	0040f809 	jalr	$v0
    3eec:	02003821 	move	$a3,$s0
    3ef0:	26620001 	addiu	$v0,$s3,1
    3ef4:	305300ff 	andi	$s3,$v0,0xff
    3ef8:	2e620029 	sltiu	$v0,$s3,41
    3efc:	5440ffde 	0x5440ffde
    3f00:	00131040 	sll	$v0,$s3,0x1
    3f04:	16c00009 	bnez	$s6,3f2c <poll+0xbb0>
    3f08:	00000000 	nop
    3f0c:	3c040000 	lui	$a0,0x0
    3f10:	24841190 	addiu	$a0,$a0,4496
    3f14:	3c020000 	lui	$v0,0x0
    3f18:	24420000 	addiu	$v0,$v0,0
    3f1c:	0040f809 	jalr	$v0
    3f20:	02402821 	move	$a1,$s2
    3f24:	08000fd2 	j	3f48 <poll+0xbcc>
    3f28:	00001021 	move	$v0,$zero
    3f2c:	3c040000 	lui	$a0,0x0
    3f30:	24841154 	addiu	$a0,$a0,4436
    3f34:	3c020000 	lui	$v0,0x0
    3f38:	24420000 	addiu	$v0,$v0,0
    3f3c:	0040f809 	jalr	$v0
    3f40:	02402821 	move	$a1,$s2
    3f44:	24020001 	li	$v0,1
    3f48:	8fbf0034 	lw	$ra,52($sp)
    3f4c:	8fb60030 	lw	$s6,48($sp)
    3f50:	8fb5002c 	lw	$s5,44($sp)
    3f54:	8fb40028 	lw	$s4,40($sp)
    3f58:	8fb30024 	lw	$s3,36($sp)
    3f5c:	8fb20020 	lw	$s2,32($sp)
    3f60:	8fb1001c 	lw	$s1,28($sp)
    3f64:	8fb00018 	lw	$s0,24($sp)
    3f68:	03e00008 	jr	$ra
    3f6c:	27bd0038 	addiu	$sp,$sp,56
    3f70:	27bdffd8 	addiu	$sp,$sp,-40
    3f74:	afbf0024 	sw	$ra,36($sp)
    3f78:	afb40020 	sw	$s4,32($sp)
    3f7c:	afb3001c 	sw	$s3,28($sp)
    3f80:	afb20018 	sw	$s2,24($sp)
    3f84:	afb10014 	sw	$s1,20($sp)
    3f88:	afb00010 	sw	$s0,16($sp)
    3f8c:	3090ffff 	andi	$s0,$a0,0xffff
    3f90:	3c110000 	lui	$s1,0x0
    3f94:	26310000 	addiu	$s1,$s1,0
    3f98:	0220f809 	jalr	$s1
    3f9c:	30b3ffff 	andi	$s3,$a1,0xffff
    3fa0:	24030002 	li	$v1,2
    3fa4:	14430006 	bne	$v0,$v1,3fc0 <poll+0xc44>
    3fa8:	2602ffff 	addiu	$v0,$s0,-1
    3fac:	00021100 	sll	$v0,$v0,0x4
    3fb0:	24420001 	addiu	$v0,$v0,1
    3fb4:	3052ffff 	andi	$s2,$v0,0xffff
    3fb8:	08000ffe 	j	3ff8 <poll+0xc7c>
    3fbc:	2414003c 	li	$s4,60
    3fc0:	0220f809 	jalr	$s1
    3fc4:	00000000 	nop
    3fc8:	24030001 	li	$v1,1
    3fcc:	14430006 	bne	$v0,$v1,3fe8 <poll+0xc6c>
    3fd0:	2602ffff 	addiu	$v0,$s0,-1
    3fd4:	000210c0 	sll	$v0,$v0,0x3
    3fd8:	24420001 	addiu	$v0,$v0,1
    3fdc:	3052ffff 	andi	$s2,$v0,0xffff
    3fe0:	08000ffe 	j	3ff8 <poll+0xc7c>
    3fe4:	24140029 	li	$s4,41
    3fe8:	000210c0 	sll	$v0,$v0,0x3
    3fec:	24420001 	addiu	$v0,$v0,1
    3ff0:	3052ffff 	andi	$s2,$v0,0xffff
    3ff4:	24140021 	li	$s4,33
    3ff8:	24040002 	li	$a0,2
    3ffc:	325000ff 	andi	$s0,$s2,0xff
    4000:	02002821 	move	$a1,$s0
    4004:	3c110000 	lui	$s1,0x0
    4008:	26310000 	addiu	$s1,$s1,0
    400c:	0220f809 	jalr	$s1
    4010:	02603021 	move	$a2,$s3
    4014:	24040003 	li	$a0,3
    4018:	00129202 	srl	$s2,$s2,0x8
    401c:	02402821 	move	$a1,$s2
    4020:	0220f809 	jalr	$s1
    4024:	02603021 	move	$a2,$s3
    4028:	24040004 	li	$a0,4
    402c:	02002821 	move	$a1,$s0
    4030:	0220f809 	jalr	$s1
    4034:	02603021 	move	$a2,$s3
    4038:	24040005 	li	$a0,5
    403c:	02402821 	move	$a1,$s2
    4040:	0220f809 	jalr	$s1
    4044:	02603021 	move	$a2,$s3
    4048:	24040001 	li	$a0,1
    404c:	02802821 	move	$a1,$s4
    4050:	0220f809 	jalr	$s1
    4054:	02603021 	move	$a2,$s3
    4058:	8fbf0024 	lw	$ra,36($sp)
    405c:	8fb40020 	lw	$s4,32($sp)
    4060:	8fb3001c 	lw	$s3,28($sp)
    4064:	8fb20018 	lw	$s2,24($sp)
    4068:	8fb10014 	lw	$s1,20($sp)
    406c:	8fb00010 	lw	$s0,16($sp)
    4070:	03e00008 	jr	$ra
    4074:	27bd0028 	addiu	$sp,$sp,40
    4078:	27bdffe0 	addiu	$sp,$sp,-32
    407c:	afbf001c 	sw	$ra,28($sp)
    4080:	afb20018 	sw	$s2,24($sp)
    4084:	afb10014 	sw	$s1,20($sp)
    4088:	afb00010 	sw	$s0,16($sp)
    408c:	00008021 	move	$s0,$zero
    4090:	00a09021 	move	$s2,$a1
    4094:	3c030000 	lui	$v1,0x0
    4098:	246301a0 	addiu	$v1,$v1,416
    409c:	94620008 	lhu	$v0,8($v1)
    40a0:	0800105a 	j	4168 <poll+0xdec>
    40a4:	3091ffff 	andi	$s1,$a0,0xffff
    40a8:	12400017 	beqz	$s2,4108 <poll+0xd8c>
    40ac:	00101040 	sll	$v0,$s0,0x1
    40b0:	00501021 	addu	$v0,$v0,$s0
    40b4:	00021080 	sll	$v0,$v0,0x2
    40b8:	3c030000 	lui	$v1,0x0
    40bc:	246301a0 	addiu	$v1,$v1,416
    40c0:	00621821 	addu	$v1,$v1,$v0
    40c4:	90620000 	lbu	$v0,0($v1)
    40c8:	2442ffdd 	addiu	$v0,$v0,-35
    40cc:	2c420005 	sltiu	$v0,$v0,5
    40d0:	10400004 	beqz	$v0,40e4 <poll+0xd68>
    40d4:	34058000 	li	$a1,0x8000
    40d8:	90640000 	lbu	$a0,0($v1)
    40dc:	0800103c 	j	40f0 <poll+0xd74>
    40e0:	02203021 	move	$a2,$s1
    40e4:	02203021 	move	$a2,$s1
    40e8:	90640000 	lbu	$a0,0($v1)
    40ec:	94650008 	lhu	$a1,8($v1)
    40f0:	3c020000 	lui	$v0,0x0
    40f4:	24424190 	addiu	$v0,$v0,16784
    40f8:	0040f809 	jalr	$v0
    40fc:	00000000 	nop
    4100:	08001052 	j	4148 <poll+0xdcc>
    4104:	26020001 	addiu	$v0,$s0,1
    4108:	00501021 	addu	$v0,$v0,$s0
    410c:	00021080 	sll	$v0,$v0,0x2
    4110:	3c030000 	lui	$v1,0x0
    4114:	246301a0 	addiu	$v1,$v1,416
    4118:	00621821 	addu	$v1,$v1,$v0
    411c:	90620000 	lbu	$v0,0($v1)
    4120:	2442ffdd 	addiu	$v0,$v0,-35
    4124:	2c420005 	sltiu	$v0,$v0,5
    4128:	10400006 	beqz	$v0,4144 <poll+0xdc8>
    412c:	02203021 	move	$a2,$s1
    4130:	90640000 	lbu	$a0,0($v1)
    4134:	3c020000 	lui	$v0,0x0
    4138:	24424190 	addiu	$v0,$v0,16784
    413c:	0040f809 	jalr	$v0
    4140:	94650008 	lhu	$a1,8($v1)
    4144:	26020001 	addiu	$v0,$s0,1
    4148:	305000ff 	andi	$s0,$v0,0xff
    414c:	00101040 	sll	$v0,$s0,0x1
    4150:	00501021 	addu	$v0,$v0,$s0
    4154:	00021080 	sll	$v0,$v0,0x2
    4158:	3c030000 	lui	$v1,0x0
    415c:	246301a0 	addiu	$v1,$v1,416
    4160:	00621821 	addu	$v1,$v1,$v0
    4164:	94620008 	lhu	$v0,8($v1)
    4168:	1440ffcf 	bnez	$v0,40a8 <poll+0xd2c>
    416c:	00000000 	nop
    4170:	90620000 	lbu	$v0,0($v1)
    4174:	1440ffcc 	bnez	$v0,40a8 <poll+0xd2c>
    4178:	8fbf001c 	lw	$ra,28($sp)
    417c:	8fb20018 	lw	$s2,24($sp)
    4180:	8fb10014 	lw	$s1,20($sp)
    4184:	8fb00010 	lw	$s0,16($sp)
    4188:	03e00008 	jr	$ra
    418c:	27bd0020 	addiu	$sp,$sp,32
    4190:	27bdffd8 	addiu	$sp,$sp,-40
    4194:	afbf0020 	sw	$ra,32($sp)
    4198:	afb3001c 	sw	$s3,28($sp)
    419c:	afb20018 	sw	$s2,24($sp)
    41a0:	afb10014 	sw	$s1,20($sp)
    41a4:	afb00010 	sw	$s0,16($sp)
    41a8:	309300ff 	andi	$s3,$a0,0xff
    41ac:	30d1ffff 	andi	$s1,$a2,0xffff
    41b0:	02202021 	move	$a0,$s1
    41b4:	3c020000 	lui	$v0,0x0
    41b8:	244242ac 	addiu	$v0,$v0,17068
    41bc:	0040f809 	jalr	$v0
    41c0:	30b2ffff 	andi	$s2,$a1,0xffff
    41c4:	2404001c 	li	$a0,28
    41c8:	324500ff 	andi	$a1,$s2,0xff
    41cc:	3c100000 	lui	$s0,0x0
    41d0:	26100000 	addiu	$s0,$s0,0
    41d4:	0200f809 	jalr	$s0
    41d8:	02203021 	move	$a2,$s1
    41dc:	2404001d 	li	$a0,29
    41e0:	00122a02 	srl	$a1,$s2,0x8
    41e4:	0200f809 	jalr	$s0
    41e8:	02203021 	move	$a2,$s1
    41ec:	2404001e 	li	$a0,30
    41f0:	02602821 	move	$a1,$s3
    41f4:	0200f809 	jalr	$s0
    41f8:	02203021 	move	$a2,$s1
    41fc:	8fbf0020 	lw	$ra,32($sp)
    4200:	8fb3001c 	lw	$s3,28($sp)
    4204:	8fb20018 	lw	$s2,24($sp)
    4208:	8fb10014 	lw	$s1,20($sp)
    420c:	8fb00010 	lw	$s0,16($sp)
    4210:	03e00008 	jr	$ra
    4214:	27bd0028 	addiu	$sp,$sp,40
    4218:	27bdffe0 	addiu	$sp,$sp,-32
    421c:	afbf001c 	sw	$ra,28($sp)
    4220:	afb20018 	sw	$s2,24($sp)
    4224:	afb10014 	sw	$s1,20($sp)
    4228:	afb00010 	sw	$s0,16($sp)
    422c:	309200ff 	andi	$s2,$a0,0xff
    4230:	30b0ffff 	andi	$s0,$a1,0xffff
    4234:	3c110000 	lui	$s1,0x0
    4238:	263142ac 	addiu	$s1,$s1,17068
    423c:	0220f809 	jalr	$s1
    4240:	02002021 	move	$a0,$s0
    4244:	2404001e 	li	$a0,30
    4248:	02402821 	move	$a1,$s2
    424c:	3c020000 	lui	$v0,0x0
    4250:	24420000 	addiu	$v0,$v0,0
    4254:	0040f809 	jalr	$v0
    4258:	02003021 	move	$a2,$s0
    425c:	0220f809 	jalr	$s1
    4260:	02002021 	move	$a0,$s0
    4264:	2404009c 	li	$a0,156
    4268:	3c110000 	lui	$s1,0x0
    426c:	26310000 	addiu	$s1,$s1,0
    4270:	0220f809 	jalr	$s1
    4274:	02002821 	move	$a1,$s0
    4278:	2404009d 	li	$a0,157
    427c:	02002821 	move	$a1,$s0
    4280:	0220f809 	jalr	$s1
    4284:	00408021 	move	$s0,$v0
    4288:	00021200 	sll	$v0,$v0,0x8
    428c:	02028025 	or	$s0,$s0,$v0
    4290:	3202ffff 	andi	$v0,$s0,0xffff
    4294:	8fbf001c 	lw	$ra,28($sp)
    4298:	8fb20018 	lw	$s2,24($sp)
    429c:	8fb10014 	lw	$s1,20($sp)
    42a0:	8fb00010 	lw	$s0,16($sp)
    42a4:	03e00008 	jr	$ra
    42a8:	27bd0020 	addiu	$sp,$sp,32
    42ac:	27bdffe0 	addiu	$sp,$sp,-32
    42b0:	afbf0018 	sw	$ra,24($sp)
    42b4:	afb10014 	sw	$s1,20($sp)
    42b8:	afb00010 	sw	$s0,16($sp)
    42bc:	00008021 	move	$s0,$zero
    42c0:	080010c4 	j	4310 <poll+0xf94>
    42c4:	3091ffff 	andi	$s1,$a0,0xffff
    42c8:	34425b00 	ori	$v0,$v0,0x5b00
    42cc:	3c030000 	lui	$v1,0x0
    42d0:	8c630000 	lw	$v1,0($v1)
    42d4:	00430019 	multu	$v0,$v1
    42d8:	00001010 	mfhi	$v0
    42dc:	1440ffff 	bnez	$v0,42dc <poll+0xf60>
    42e0:	2442ffff 	addiu	$v0,$v0,-1
    42e4:	2e020065 	sltiu	$v0,$s0,101
    42e8:	14400009 	bnez	$v0,4310 <poll+0xf94>
    42ec:	26100001 	addiu	$s0,$s0,1
    42f0:	3c040000 	lui	$a0,0x0
    42f4:	248411d0 	addiu	$a0,$a0,4560
    42f8:	3c020000 	lui	$v0,0x0
    42fc:	24420000 	addiu	$v0,$v0,0
    4300:	0040f809 	jalr	$v0
    4304:	02202821 	move	$a1,$s1
    4308:	080010cc 	j	4330 <poll+0xfb4>
    430c:	8fbf0018 	lw	$ra,24($sp)
    4310:	2404009f 	li	$a0,159
    4314:	3c020000 	lui	$v0,0x0
    4318:	24420000 	addiu	$v0,$v0,0
    431c:	0040f809 	jalr	$v0
    4320:	02202821 	move	$a1,$s1
    4324:	1440ffe8 	bnez	$v0,42c8 <poll+0xf4c>
    4328:	3c026666 	lui	$v0,0x6666
    432c:	8fbf0018 	lw	$ra,24($sp)
    4330:	8fb10014 	lw	$s1,20($sp)
    4334:	8fb00010 	lw	$s0,16($sp)
    4338:	03e00008 	jr	$ra
    433c:	27bd0020 	addiu	$sp,$sp,32

0000000000004340 <si3210_set_power_ring_freq>:
    4340:	03e00008 	jr	$ra
    4344:	00000000 	nop

0000000000004348 <si3210_set_gain>:
    4348:	27bdff98 	addiu	$sp,$sp,-104
    434c:	afbf0064 	sw	$ra,100($sp)
    4350:	afb20060 	sw	$s2,96($sp)
    4354:	afb1005c 	sw	$s1,92($sp)
    4358:	afb00058 	sw	$s0,88($sp)
    435c:	27b20010 	addiu	$s2,$sp,16
    4360:	02403021 	move	$a2,$s2
    4364:	3c030000 	lui	$v1,0x0
    4368:	24631200 	addiu	$v1,$v1,4608
    436c:	00808821 	move	$s1,$a0
    4370:	30620003 	andi	$v0,$v1,0x3
    4374:	10400017 	beqz	$v0,43d4 <si3210_set_gain+0x8c>
    4378:	00a08021 	move	$s0,$a1
    437c:	24620040 	addiu	$v0,$v1,64
    4380:	88640003 	lwl	$a0,3($v1)
    4384:	98640000 	lwr	$a0,0($v1)
    4388:	88650007 	lwl	$a1,7($v1)
    438c:	98650004 	lwr	$a1,4($v1)
    4390:	8867000b 	lwl	$a3,11($v1)
    4394:	98670008 	lwr	$a3,8($v1)
    4398:	8868000f 	lwl	$t0,15($v1)
    439c:	9868000c 	lwr	$t0,12($v1)
    43a0:	a8c40003 	swl	$a0,3($a2)
    43a4:	b8c40000 	swr	$a0,0($a2)
    43a8:	a8c50007 	swl	$a1,7($a2)
    43ac:	b8c50004 	swr	$a1,4($a2)
    43b0:	a8c7000b 	swl	$a3,11($a2)
    43b4:	b8c70008 	swr	$a3,8($a2)
    43b8:	a8c8000f 	swl	$t0,15($a2)
    43bc:	b8c8000c 	swr	$t0,12($a2)
    43c0:	24630010 	addiu	$v1,$v1,16
    43c4:	1462ffee 	bne	$v1,$v0,4380 <si3210_set_gain+0x38>
    43c8:	24c60010 	addiu	$a2,$a2,16
    43cc:	08001102 	j	4408 <si3210_set_gain+0xc0>
    43d0:	8e02000c 	lw	$v0,12($s0)
    43d4:	24620040 	addiu	$v0,$v1,64
    43d8:	8c640000 	lw	$a0,0($v1)
    43dc:	8c650004 	lw	$a1,4($v1)
    43e0:	8c670008 	lw	$a3,8($v1)
    43e4:	8c68000c 	lw	$t0,12($v1)
    43e8:	acc40000 	sw	$a0,0($a2)
    43ec:	acc50004 	sw	$a1,4($a2)
    43f0:	acc70008 	sw	$a3,8($a2)
    43f4:	acc8000c 	sw	$t0,12($a2)
    43f8:	24630010 	addiu	$v1,$v1,16
    43fc:	1462fff6 	bne	$v1,$v0,43d8 <si3210_set_gain+0x90>
    4400:	24c60010 	addiu	$a2,$a2,16
    4404:	8e02000c 	lw	$v0,12($s0)
    4408:	88640003 	lwl	$a0,3($v1)
    440c:	98640000 	lwr	$a0,0($v1)
    4410:	84650004 	lh	$a1,4($v1)
    4414:	a8c40003 	swl	$a0,3($a2)
    4418:	b8c40000 	swr	$a0,0($a2)
    441c:	1040002c 	beqz	$v0,44d0 <si3210_set_gain+0x188>
    4420:	a4c50004 	sh	$a1,4($a2)
    4424:	8e020008 	lw	$v0,8($s0)
    4428:	1040002a 	beqz	$v0,44d4 <si3210_set_gain+0x18c>
    442c:	8fbf0064 	lw	$ra,100($sp)
    4430:	3c040000 	lui	$a0,0x0
    4434:	24841248 	addiu	$a0,$a0,4680
    4438:	96250000 	lhu	$a1,0($s1)
    443c:	8e060000 	lw	$a2,0($s0)
    4440:	3c020000 	lui	$v0,0x0
    4444:	24420000 	addiu	$v0,$v0,0
    4448:	0040f809 	jalr	$v0
    444c:	8e070004 	lw	$a3,4($s0)
    4450:	8e020000 	lw	$v0,0($s0)
    4454:	2443001e 	addiu	$v1,$v0,30
    4458:	2c620023 	sltiu	$v0,$v1,35
    445c:	1040000c 	beqz	$v0,4490 <si3210_set_gain+0x148>
    4460:	2404001b 	li	$a0,27
    4464:	00031040 	sll	$v0,$v1,0x1
    4468:	02421021 	addu	$v0,$s2,$v0
    446c:	94450000 	lhu	$a1,0($v0)
    4470:	3c020000 	lui	$v0,0x0
    4474:	24424190 	addiu	$v0,$v0,16784
    4478:	0040f809 	jalr	$v0
    447c:	96260000 	lhu	$a2,0($s1)
    4480:	8e020000 	lw	$v0,0($s0)
    4484:	ae22006c 	sw	$v0,108($s1)
    4488:	8e020000 	lw	$v0,0($s0)
    448c:	ae220074 	sw	$v0,116($s1)
    4490:	8e020004 	lw	$v0,4($s0)
    4494:	2443001c 	addiu	$v1,$v0,28
    4498:	2c620023 	sltiu	$v0,$v1,35
    449c:	1040000c 	beqz	$v0,44d0 <si3210_set_gain+0x188>
    44a0:	2404001a 	li	$a0,26
    44a4:	00031040 	sll	$v0,$v1,0x1
    44a8:	03a21021 	addu	$v0,$sp,$v0
    44ac:	94450010 	lhu	$a1,16($v0)
    44b0:	3c020000 	lui	$v0,0x0
    44b4:	24424190 	addiu	$v0,$v0,16784
    44b8:	0040f809 	jalr	$v0
    44bc:	96260000 	lhu	$a2,0($s1)
    44c0:	8e020004 	lw	$v0,4($s0)
    44c4:	ae220070 	sw	$v0,112($s1)
    44c8:	8e020004 	lw	$v0,4($s0)
    44cc:	ae220078 	sw	$v0,120($s1)
    44d0:	8fbf0064 	lw	$ra,100($sp)
    44d4:	8fb20060 	lw	$s2,96($sp)
    44d8:	8fb1005c 	lw	$s1,92($sp)
    44dc:	8fb00058 	lw	$s0,88($sp)
    44e0:	03e00008 	jr	$ra
    44e4:	27bd0068 	addiu	$sp,$sp,104

00000000000044e8 <si3210_set_ring_type>:
    44e8:	27bdffc0 	addiu	$sp,$sp,-64
    44ec:	afbf0034 	sw	$ra,52($sp)
    44f0:	afbe0030 	sw	$s8,48($sp)
    44f4:	afb7002c 	sw	$s7,44($sp)
    44f8:	afb60028 	sw	$s6,40($sp)
    44fc:	afb50024 	sw	$s5,36($sp)
    4500:	afb40020 	sw	$s4,32($sp)
    4504:	afb3001c 	sw	$s3,28($sp)
    4508:	afb20018 	sw	$s2,24($sp)
    450c:	afb10014 	sw	$s1,20($sp)
    4510:	afb00010 	sw	$s0,16($sp)
    4514:	f7b40038 	0xf7b40038
    4518:	0000f021 	move	$s8,$zero
    451c:	0000b821 	move	$s7,$zero
    4520:	0000a821 	move	$s5,$zero
    4524:	00008821 	move	$s1,$zero
    4528:	00809821 	move	$s3,$a0
    452c:	240400a2 	li	$a0,162
    4530:	00a0a021 	move	$s4,$a1
    4534:	3c100000 	lui	$s0,0x0
    4538:	26100000 	addiu	$s0,$s0,0
    453c:	0200f809 	jalr	$s0
    4540:	96650000 	lhu	$a1,0($s3)
    4544:	3052fffb 	andi	$s2,$v0,0xfffb
    4548:	24040022 	li	$a0,34
    454c:	324500fb 	andi	$a1,$s2,0xfb
    4550:	3c160000 	lui	$s6,0x0
    4554:	26d60000 	addiu	$s6,$s6,0
    4558:	02c0f809 	jalr	$s6
    455c:	96660000 	lhu	$a2,0($s3)
    4560:	8e830000 	lw	$v1,0($s4)
    4564:	24020001 	li	$v0,1
    4568:	10620007 	beq	$v1,$v0,4588 <si3210_set_ring_type+0xa0>
    456c:	240400a2 	li	$a0,162
    4570:	10600098 	beqz	$v1,47d4 <si3210_set_ring_type+0x2ec>
    4574:	2c620005 	sltiu	$v0,$v1,5
    4578:	50400097 	0x50400097
    457c:	24020001 	li	$v0,1
    4580:	08001198 	j	4660 <si3210_set_ring_type+0x178>
    4584:	92820005 	lbu	$v0,5($s4)
    4588:	0200f809 	jalr	$s0
    458c:	96650000 	lhu	$a1,0($s3)
    4590:	24040022 	li	$a0,34
    4594:	96660000 	lhu	$a2,0($s3)
    4598:	02c0f809 	jalr	$s6
    459c:	304500fe 	andi	$a1,$v0,0xfe
    45a0:	00002821 	move	$a1,$zero
    45a4:	00002021 	move	$a0,$zero
    45a8:	3c020000 	lui	$v0,0x0
    45ac:	00441021 	addu	$v0,$v0,$a0
    45b0:	90420434 	lbu	$v0,1076($v0)
    45b4:	92830005 	lbu	$v1,5($s4)
    45b8:	3c100000 	lui	$s0,0x0
    45bc:	26100434 	addiu	$s0,$s0,1076
    45c0:	0043102b 	sltu	$v0,$v0,$v1
    45c4:	10400006 	beqz	$v0,45e0 <si3210_set_ring_type+0xf8>
    45c8:	24a50006 	addiu	$a1,$a1,6
    45cc:	24840006 	addiu	$a0,$a0,6
    45d0:	00b01021 	addu	$v0,$a1,$s0
    45d4:	90420000 	lbu	$v0,0($v0)
    45d8:	1440fff3 	bnez	$v0,45a8 <si3210_set_ring_type+0xc0>
    45dc:	26310001 	addiu	$s1,$s1,1
    45e0:	00111040 	sll	$v0,$s1,0x1
    45e4:	00511021 	addu	$v0,$v0,$s1
    45e8:	00021040 	sll	$v0,$v0,0x1
    45ec:	00501021 	addu	$v0,$v0,$s0
    45f0:	90420000 	lbu	$v0,0($v0)
    45f4:	50400001 	0x50400001
    45f8:	2631ffff 	addiu	$s1,$s1,-1
    45fc:	00111840 	sll	$v1,$s1,0x1
    4600:	00711821 	addu	$v1,$v1,$s1
    4604:	00031840 	sll	$v1,$v1,0x1
    4608:	02031021 	addu	$v0,$s0,$v1
    460c:	94440004 	lhu	$a0,4($v0)
    4610:	92820004 	lbu	$v0,4($s4)
    4614:	00820018 	mult	$a0,$v0
    4618:	3c022aaa 	lui	$v0,0x2aaa
    461c:	00002012 	mflo	$a0
    4620:	3442aaab 	ori	$v0,$v0,0xaaab
    4624:	00000000 	nop
    4628:	00820018 	mult	$a0,$v0
    462c:	0000f021 	move	$s8,$zero
    4630:	24150001 	li	$s5,1
    4634:	02032821 	addu	$a1,$s0,$v1
    4638:	00701821 	addu	$v1,$v1,$s0
    463c:	90630000 	lbu	$v1,0($v1)
    4640:	94b70002 	lhu	$s7,2($a1)
    4644:	a2630065 	sb	$v1,101($s3)
    4648:	00001010 	mfhi	$v0
    464c:	000427c3 	sra	$a0,$a0,0x1f
    4650:	00021103 	sra	$v0,$v0,0x4
    4654:	00441023 	subu	$v0,$v0,$a0
    4658:	080011f5 	j	47d4 <si3210_set_ring_type+0x2ec>
    465c:	3052ffff 	andi	$s2,$v0,0xffff
    4660:	2c42003d 	sltiu	$v0,$v0,61
    4664:	54400004 	0x54400004
    4668:	92820005 	lbu	$v0,5($s4)
    466c:	2402003c 	li	$v0,60
    4670:	a2820005 	sb	$v0,5($s4)
    4674:	92820005 	lbu	$v0,5($s4)
    4678:	2c42000a 	sltiu	$v0,$v0,10
    467c:	10400002 	beqz	$v0,4688 <si3210_set_ring_type+0x1a0>
    4680:	2402000a 	li	$v0,10
    4684:	a2820005 	sb	$v0,5($s4)
    4688:	96650000 	lhu	$a1,0($s3)
    468c:	0200f809 	jalr	$s0
    4690:	240400a2 	li	$a0,162
    4694:	24040022 	li	$a0,34
    4698:	34420001 	ori	$v0,$v0,0x1
    469c:	96660000 	lhu	$a2,0($s3)
    46a0:	02c0f809 	jalr	$s6
    46a4:	304500ff 	andi	$a1,$v0,0xff
    46a8:	92860005 	lbu	$a2,5($s4)
    46ac:	24040fa0 	li	$a0,4000
    46b0:	0086001a 	div	$zero,$a0,$a2
    46b4:	00002012 	mflo	$a0
    46b8:	50c00001 	0x50c00001
    46bc:	0007000d 	break	0x7
    46c0:	3c022aaa 	lui	$v0,0x2aaa
    46c4:	92830004 	lbu	$v1,4($s4)
    46c8:	3442aaab 	ori	$v0,$v0,0xaaab
    46cc:	00031bc0 	sll	$v1,$v1,0xf
    46d0:	00620018 	mult	$v1,$v0
    46d4:	00001010 	mfhi	$v0
    46d8:	00031fc3 	sra	$v1,$v1,0x1f
    46dc:	00021103 	sra	$v0,$v0,0x4
    46e0:	00431023 	subu	$v0,$v0,$v1
    46e4:	3052ffff 	andi	$s2,$v0,0xffff
    46e8:	8e830000 	lw	$v1,0($s4)
    46ec:	24020003 	li	$v0,3
    46f0:	10620011 	beq	$v1,$v0,4738 <si3210_set_ring_type+0x250>
    46f4:	309effff 	andi	$s8,$a0,0xffff
    46f8:	2c620004 	sltiu	$v0,$v1,4
    46fc:	10400005 	beqz	$v0,4714 <si3210_set_ring_type+0x22c>
    4700:	24020002 	li	$v0,2
    4704:	10620008 	beq	$v1,$v0,4728 <si3210_set_ring_type+0x240>
    4708:	00000000 	nop
    470c:	080011d6 	j	4758 <si3210_set_ring_type+0x270>
    4710:	00000000 	nop
    4714:	24020004 	li	$v0,4
    4718:	1062000b 	beq	$v1,$v0,4748 <si3210_set_ring_type+0x260>
    471c:	00000000 	nop
    4720:	080011d6 	j	4758 <si3210_set_ring_type+0x270>
    4724:	00000000 	nop
    4728:	3c010000 	lui	$at,0x0
    472c:	d4221270 	0xd4221270
    4730:	080011d9 	j	4764 <si3210_set_ring_type+0x27c>
    4734:	02460018 	mult	$s2,$a2
    4738:	3c010000 	lui	$at,0x0
    473c:	d4221278 	0xd4221278
    4740:	080011d9 	j	4764 <si3210_set_ring_type+0x27c>
    4744:	02460018 	mult	$s2,$a2
    4748:	3c010000 	lui	$at,0x0
    474c:	d4221280 	0xd4221280
    4750:	080011d9 	j	4764 <si3210_set_ring_type+0x27c>
    4754:	02460018 	mult	$s2,$a2
    4758:	44801000 	mtc1	$zero,$f2
    475c:	44801800 	mtc1	$zero,$f3
    4760:	02460018 	mult	$s2,$a2
    4764:	3c010000 	lui	$at,0x0
    4768:	d4201288 	0xd4201288
    476c:	46201002 	mul.d	$f0,$f2,$f0
    4770:	00001012 	mflo	$v0
    4774:	44821000 	mtc1	$v0,$f2
    4778:	00000000 	nop
    477c:	468010a1 	cvt.d.w	$f2,$f2
    4780:	46201003 	div.d	$f0,$f2,$f0
    4784:	3c0141e0 	lui	$at,0x41e0
    4788:	44811800 	mtc1	$at,$f3
    478c:	44801000 	mtc1	$zero,$f2
    4790:	00000000 	nop
    4794:	4620103e 	c.le.d	$f2,$f0
    4798:	00000000 	nop
    479c:	45030005 	0x45030005
    47a0:	46220001 	sub.d	$f0,$f0,$f2
    47a4:	4620008d 	c1	0x20008d
    47a8:	44051000 	mfc1	$a1,$f2
    47ac:	080011f2 	j	47c8 <si3210_set_ring_type+0x2e0>
    47b0:	30b7ffff 	andi	$s7,$a1,0xffff
    47b4:	4620008d 	c1	0x20008d
    47b8:	44051000 	mfc1	$a1,$f2
    47bc:	3c028000 	lui	$v0,0x8000
    47c0:	00a22825 	or	$a1,$a1,$v0
    47c4:	30b7ffff 	andi	$s7,$a1,0xffff
    47c8:	92820005 	lbu	$v0,5($s4)
    47cc:	24150001 	li	$s5,1
    47d0:	a2620065 	sb	$v0,101($s3)
    47d4:	24020001 	li	$v0,1
    47d8:	56a20012 	0x56a20012
    47dc:	92820006 	lbu	$v0,6($s4)
    47e0:	24040014 	li	$a0,20
    47e4:	02e02821 	move	$a1,$s7
    47e8:	3c100000 	lui	$s0,0x0
    47ec:	26104190 	addiu	$s0,$s0,16784
    47f0:	0200f809 	jalr	$s0
    47f4:	96660000 	lhu	$a2,0($s3)
    47f8:	24040015 	li	$a0,21
    47fc:	96660000 	lhu	$a2,0($s3)
    4800:	0200f809 	jalr	$s0
    4804:	02402821 	move	$a1,$s2
    4808:	24040016 	li	$a0,22
    480c:	96660000 	lhu	$a2,0($s3)
    4810:	0200f809 	jalr	$s0
    4814:	03c02821 	move	$a1,$s8
    4818:	8e820000 	lw	$v0,0($s4)
    481c:	ae620060 	sw	$v0,96($s3)
    4820:	92820006 	lbu	$v0,6($s4)
    4824:	10400023 	beqz	$v0,48b4 <si3210_set_ring_type+0x3cc>
    4828:	3c102aaa 	lui	$s0,0x2aaa
    482c:	3610aaab 	ori	$s0,$s0,0xaaab
    4830:	000213c0 	sll	$v0,$v0,0xf
    4834:	00500018 	mult	$v0,$s0
    4838:	24040013 	li	$a0,19
    483c:	000217c3 	sra	$v0,$v0,0x1f
    4840:	00008010 	mfhi	$s0
    4844:	96660000 	lhu	$a2,0($s3)
    4848:	00108103 	sra	$s0,$s0,0x4
    484c:	02028023 	subu	$s0,$s0,$v0
    4850:	3210ffff 	andi	$s0,$s0,0xffff
    4854:	3c020000 	lui	$v0,0x0
    4858:	24424190 	addiu	$v0,$v0,16784
    485c:	0040f809 	jalr	$v0
    4860:	02002821 	move	$a1,$s0
    4864:	240400a2 	li	$a0,162
    4868:	3c020000 	lui	$v0,0x0
    486c:	24420000 	addiu	$v0,$v0,0
    4870:	0040f809 	jalr	$v0
    4874:	96650000 	lhu	$a1,0($s3)
    4878:	24040022 	li	$a0,34
    487c:	34420002 	ori	$v0,$v0,0x2
    4880:	304500ff 	andi	$a1,$v0,0xff
    4884:	3c020000 	lui	$v0,0x0
    4888:	24420000 	addiu	$v0,$v0,0
    488c:	0040f809 	jalr	$v0
    4890:	96660000 	lhu	$a2,0($s3)
    4894:	00101040 	sll	$v0,$s0,0x1
    4898:	00501021 	addu	$v0,$v0,$s0
    489c:	00021140 	sll	$v0,$v0,0x5
    48a0:	04420001 	0x4420001
    48a4:	24427fff 	addiu	$v0,$v0,32767
    48a8:	000213c3 	sra	$v0,$v0,0xf
    48ac:	08001239 	j	48e4 <si3210_set_ring_type+0x3fc>
    48b0:	a2620066 	sb	$v0,102($s3)
    48b4:	240400a2 	li	$a0,162
    48b8:	3c020000 	lui	$v0,0x0
    48bc:	24420000 	addiu	$v0,$v0,0
    48c0:	0040f809 	jalr	$v0
    48c4:	96650000 	lhu	$a1,0($s3)
    48c8:	24040022 	li	$a0,34
    48cc:	304500fd 	andi	$a1,$v0,0xfd
    48d0:	3c020000 	lui	$v0,0x0
    48d4:	24420000 	addiu	$v0,$v0,0
    48d8:	0040f809 	jalr	$v0
    48dc:	96660000 	lhu	$a2,0($s3)
    48e0:	a2600066 	sb	$zero,102($s3)
    48e4:	00008821 	move	$s1,$zero
    48e8:	00002821 	move	$a1,$zero
    48ec:	3c040000 	lui	$a0,0x0
    48f0:	24840434 	addiu	$a0,$a0,1076
    48f4:	90830000 	lbu	$v1,0($a0)
    48f8:	08001247 	j	491c <si3210_set_ring_type+0x434>
    48fc:	00003021 	move	$a2,$zero
    4900:	26310001 	addiu	$s1,$s1,1
    4904:	00113040 	sll	$a2,$s1,0x1
    4908:	3c030000 	lui	$v1,0x0
    490c:	00651821 	addu	$v1,$v1,$a1
    4910:	90630434 	lbu	$v1,1076($v1)
    4914:	3c040000 	lui	$a0,0x0
    4918:	24840434 	addiu	$a0,$a0,1076
    491c:	5060000c 	0x5060000c
    4920:	2631ffff 	addiu	$s1,$s1,-1
    4924:	92820005 	lbu	$v0,5($s4)
    4928:	0062102b 	sltu	$v0,$v1,$v0
    492c:	1440fff4 	bnez	$v0,4900 <si3210_set_ring_type+0x418>
    4930:	24a50006 	addiu	$a1,$a1,6
    4934:	00d11021 	addu	$v0,$a2,$s1
    4938:	00021040 	sll	$v0,$v0,0x1
    493c:	00441021 	addu	$v0,$v0,$a0
    4940:	90420000 	lbu	$v0,0($v0)
    4944:	14400003 	bnez	$v0,4954 <si3210_set_ring_type+0x46c>
    4948:	24040024 	li	$a0,36
    494c:	2631ffff 	addiu	$s1,$s1,-1
    4950:	24040024 	li	$a0,36
    4954:	00118040 	sll	$s0,$s1,0x1
    4958:	02118021 	addu	$s0,$s0,$s1
    495c:	00108040 	sll	$s0,$s0,0x1
    4960:	3c050000 	lui	$a1,0x0
    4964:	00b02821 	addu	$a1,$a1,$s0
    4968:	94a5040a 	lhu	$a1,1034($a1)
    496c:	3c020000 	lui	$v0,0x0
    4970:	24424190 	addiu	$v0,$v0,16784
    4974:	0040f809 	jalr	$v0
    4978:	96660000 	lhu	$a2,0($s3)
    497c:	24040046 	li	$a0,70
    4980:	3c050000 	lui	$a1,0x0
    4984:	00b02821 	addu	$a1,$a1,$s0
    4988:	90a5040c 	lbu	$a1,1036($a1)
    498c:	3c110000 	lui	$s1,0x0
    4990:	26310000 	addiu	$s1,$s1,0
    4994:	0220f809 	jalr	$s1
    4998:	96660000 	lhu	$a2,0($s3)
    499c:	92820006 	lbu	$v0,6($s4)
    49a0:	92830004 	lbu	$v1,4($s4)
    49a4:	3c010000 	lui	$at,0x0
    49a8:	d4221290 	0xd4221290
    49ac:	00021040 	sll	$v0,$v0,0x1
    49b0:	00621821 	addu	$v1,$v1,$v0
    49b4:	44830000 	mtc1	$v1,$f0
    49b8:	00000000 	nop
    49bc:	46800021 	cvt.d.w	$f0,$f0
    49c0:	46220000 	add.d	$f0,$f0,$f2
    49c4:	3c013ff8 	lui	$at,0x3ff8
    49c8:	44811800 	mtc1	$at,$f3
    49cc:	44801000 	mtc1	$zero,$f2
    49d0:	00000000 	nop
    49d4:	46220503 	div.d	$f20,$f0,$f2
    49d8:	3c0141e0 	lui	$at,0x41e0
    49dc:	44810800 	mtc1	$at,$f1
    49e0:	44800000 	mtc1	$zero,$f0
    49e4:	00000000 	nop
    49e8:	4634003e 	c.le.d	$f0,$f20
    49ec:	00000000 	nop
    49f0:	45030005 	0x45030005
    49f4:	4620a001 	sub.d	$f0,$f20,$f0
    49f8:	4620a00d 	c1	0x20a00d
    49fc:	44100000 	mfc1	$s0,$f0
    4a00:	08001287 	j	4a1c <si3210_set_ring_type+0x534>
    4a04:	2404004a 	li	$a0,74
    4a08:	4620008d 	c1	0x20008d
    4a0c:	44101000 	mfc1	$s0,$f2
    4a10:	3c028000 	lui	$v0,0x8000
    4a14:	02028025 	or	$s0,$s0,$v0
    4a18:	2404004a 	li	$a0,74
    4a1c:	321000ff 	andi	$s0,$s0,0xff
    4a20:	96660000 	lhu	$a2,0($s3)
    4a24:	0220f809 	jalr	$s1
    4a28:	02002821 	move	$a1,$s0
    4a2c:	240400cb 	li	$a0,203
    4a30:	3c020000 	lui	$v0,0x0
    4a34:	24420000 	addiu	$v0,$v0,0
    4a38:	0040f809 	jalr	$v0
    4a3c:	96650000 	lhu	$a1,0($s3)
    4a40:	0202802b 	sltu	$s0,$s0,$v0
    4a44:	52000016 	0x52000016
    4a48:	92820004 	lbu	$v0,4($s4)
    4a4c:	3c0141e0 	lui	$at,0x41e0
    4a50:	44810800 	mtc1	$at,$f1
    4a54:	44800000 	mtc1	$zero,$f0
    4a58:	00000000 	nop
    4a5c:	4634003e 	c.le.d	$f0,$f20
    4a60:	00000000 	nop
    4a64:	45030005 	0x45030005
    4a68:	4620a001 	sub.d	$f0,$f20,$f0
    4a6c:	4620a00d 	c1	0x20a00d
    4a70:	44050000 	mfc1	$a1,$f0
    4a74:	080012a4 	j	4a90 <si3210_set_ring_type+0x5a8>
    4a78:	2404004b 	li	$a0,75
    4a7c:	4620008d 	c1	0x20008d
    4a80:	44051000 	mfc1	$a1,$f2
    4a84:	3c028000 	lui	$v0,0x8000
    4a88:	00a22825 	or	$a1,$a1,$v0
    4a8c:	2404004b 	li	$a0,75
    4a90:	96660000 	lhu	$a2,0($s3)
    4a94:	0220f809 	jalr	$s1
    4a98:	30a500ff 	andi	$a1,$a1,0xff
    4a9c:	92820004 	lbu	$v0,4($s4)
    4aa0:	8fbf0034 	lw	$ra,52($sp)
    4aa4:	8fbe0030 	lw	$s8,48($sp)
    4aa8:	8fb7002c 	lw	$s7,44($sp)
    4aac:	8fb60028 	lw	$s6,40($sp)
    4ab0:	8fb50024 	lw	$s5,36($sp)
    4ab4:	8fb40020 	lw	$s4,32($sp)
    4ab8:	8fb20018 	lw	$s2,24($sp)
    4abc:	8fb10014 	lw	$s1,20($sp)
    4ac0:	8fb00010 	lw	$s0,16($sp)
    4ac4:	d7b40038 	0xd7b40038
    4ac8:	a2620064 	sb	$v0,100($s3)
    4acc:	8fb3001c 	lw	$s3,28($sp)
    4ad0:	03e00008 	jr	$ra
    4ad4:	27bd0040 	addiu	$sp,$sp,64

0000000000004ad8 <si3210_hw_config>:
    4ad8:	27bdffd8 	addiu	$sp,$sp,-40
    4adc:	afbf0020 	sw	$ra,32($sp)
    4ae0:	afb3001c 	sw	$s3,28($sp)
    4ae4:	afb20018 	sw	$s2,24($sp)
    4ae8:	afb10014 	sw	$s1,20($sp)
    4aec:	afb00010 	sw	$s0,16($sp)
    4af0:	00a09021 	move	$s2,$a1
    4af4:	8e430000 	lw	$v1,0($s2)
    4af8:	24020001 	li	$v0,1
    4afc:	1462007b 	bne	$v1,$v0,4cec <si3210_hw_config+0x214>
    4b00:	00808821 	move	$s1,$a0
    4b04:	3c020000 	lui	$v0,0x0
    4b08:	244244e8 	addiu	$v0,$v0,17640
    4b0c:	0040f809 	jalr	$v0
    4b10:	26450008 	addiu	$a1,$s2,8
    4b14:	240400c8 	li	$a0,200
    4b18:	3c020000 	lui	$v0,0x0
    4b1c:	24420000 	addiu	$v0,$v0,0
    4b20:	0040f809 	jalr	$v0
    4b24:	96250000 	lhu	$a1,0($s1)
    4b28:	92430010 	lbu	$v1,16($s2)
    4b2c:	3c013ff8 	lui	$at,0x3ff8
    4b30:	44811800 	mtc1	$at,$f3
    4b34:	44801000 	mtc1	$zero,$f2
    4b38:	44830000 	mtc1	$v1,$f0
    4b3c:	00000000 	nop
    4b40:	46800021 	cvt.d.w	$f0,$f0
    4b44:	46220083 	div.d	$f2,$f0,$f2
    4b48:	30500040 	andi	$s0,$v0,0x40
    4b4c:	44900000 	mtc1	$s0,$f0
    4b50:	00000000 	nop
    4b54:	46800021 	cvt.d.w	$f0,$f0
    4b58:	46220000 	add.d	$f0,$f0,$f2
    4b5c:	3c0141e0 	lui	$at,0x41e0
    4b60:	44811800 	mtc1	$at,$f3
    4b64:	44801000 	mtc1	$zero,$f2
    4b68:	00000000 	nop
    4b6c:	4620103e 	c.le.d	$f2,$f0
    4b70:	00000000 	nop
    4b74:	45030005 	0x45030005
    4b78:	46220001 	sub.d	$f0,$f0,$f2
    4b7c:	4620008d 	c1	0x20008d
    4b80:	44051000 	mfc1	$a1,$f2
    4b84:	080012e8 	j	4ba0 <si3210_hw_config+0xc8>
    4b88:	30b000ff 	andi	$s0,$a1,0xff
    4b8c:	4620008d 	c1	0x20008d
    4b90:	44051000 	mfc1	$a1,$f2
    4b94:	3c028000 	lui	$v0,0x8000
    4b98:	00a22825 	or	$a1,$a1,$v0
    4b9c:	30b000ff 	andi	$s0,$a1,0xff
    4ba0:	24040048 	li	$a0,72
    4ba4:	02002821 	move	$a1,$s0
    4ba8:	3c130000 	lui	$s3,0x0
    4bac:	26730000 	addiu	$s3,$s3,0
    4bb0:	0260f809 	jalr	$s3
    4bb4:	96260000 	lhu	$a2,0($s1)
    4bb8:	3c013ff8 	lui	$at,0x3ff8
    4bbc:	44811800 	mtc1	$at,$f3
    4bc0:	44801000 	mtc1	$zero,$f2
    4bc4:	44900000 	mtc1	$s0,$f0
    4bc8:	00000000 	nop
    4bcc:	46800021 	cvt.d.w	$f0,$f0
    4bd0:	46220082 	mul.d	$f2,$f0,$f2
    4bd4:	3c0141e0 	lui	$at,0x41e0
    4bd8:	44810800 	mtc1	$at,$f1
    4bdc:	44800000 	mtc1	$zero,$f0
    4be0:	00000000 	nop
    4be4:	4622003e 	c.le.d	$f0,$f2
    4be8:	00000000 	nop
    4bec:	45030005 	0x45030005
    4bf0:	46201001 	sub.d	$f0,$f2,$f0
    4bf4:	4620100d 	c1	0x20100d
    4bf8:	44050000 	mfc1	$a1,$f0
    4bfc:	08001306 	j	4c18 <si3210_hw_config+0x140>
    4c00:	a2250068 	sb	$a1,104($s1)
    4c04:	4620008d 	c1	0x20008d
    4c08:	44051000 	mfc1	$a1,$f2
    4c0c:	3c028000 	lui	$v0,0x8000
    4c10:	00a22825 	or	$a1,$a1,$v0
    4c14:	a2250068 	sb	$a1,104($s1)
    4c18:	96420012 	lhu	$v0,18($s2)
    4c1c:	2c420015 	sltiu	$v0,$v0,21
    4c20:	1440000f 	bnez	$v0,4c60 <si3210_hw_config+0x188>
    4c24:	00008021 	move	$s0,$zero
    4c28:	3c025555 	lui	$v0,0x5555
    4c2c:	96430012 	lhu	$v1,18($s2)
    4c30:	34425556 	ori	$v0,$v0,0x5556
    4c34:	2463ffec 	addiu	$v1,$v1,-20
    4c38:	00620018 	mult	$v1,$v0
    4c3c:	00001010 	mfhi	$v0
    4c40:	00031fc3 	sra	$v1,$v1,0x1f
    4c44:	00431023 	subu	$v0,$v0,$v1
    4c48:	305000ff 	andi	$s0,$v0,0xff
    4c4c:	2e020008 	sltiu	$v0,$s0,8
    4c50:	50400003 	0x50400003
    4c54:	24100007 	li	$s0,7
    4c58:	08001319 	j	4c64 <si3210_hw_config+0x18c>
    4c5c:	24040047 	li	$a0,71
    4c60:	24040047 	li	$a0,71
    4c64:	96260000 	lhu	$a2,0($s1)
    4c68:	0260f809 	jalr	$s3
    4c6c:	02002821 	move	$a1,$s0
    4c70:	00101040 	sll	$v0,$s0,0x1
    4c74:	00501021 	addu	$v0,$v0,$s0
    4c78:	24420014 	addiu	$v0,$v0,20
    4c7c:	a622006a 	sh	$v0,106($s1)
    4c80:	3c040000 	lui	$a0,0x0
    4c84:	24840458 	addiu	$a0,$a0,1112
    4c88:	8c820000 	lw	$v0,0($a0)
    4c8c:	1040000d 	beqz	$v0,4cc4 <si3210_hw_config+0x1ec>
    4c90:	00008021 	move	$s0,$zero
    4c94:	8e430004 	lw	$v1,4($s2)
    4c98:	5043000b 	0x5043000b
    4c9c:	001080c0 	sll	$s0,$s0,0x3
    4ca0:	26020001 	addiu	$v0,$s0,1
    4ca4:	305000ff 	andi	$s0,$v0,0xff
    4ca8:	001010c0 	sll	$v0,$s0,0x3
    4cac:	00441021 	addu	$v0,$v0,$a0
    4cb0:	8c420000 	lw	$v0,0($v0)
    4cb4:	50400004 	0x50400004
    4cb8:	001080c0 	sll	$s0,$s0,0x3
    4cbc:	1443fff9 	bne	$v0,$v1,4ca4 <si3210_hw_config+0x1cc>
    4cc0:	26020001 	addiu	$v0,$s0,1
    4cc4:	001080c0 	sll	$s0,$s0,0x3
    4cc8:	3c010000 	lui	$at,0x0
    4ccc:	24210458 	addiu	$at,$at,1112
    4cd0:	00308021 	addu	$s0,$at,$s0
    4cd4:	92050004 	lbu	$a1,4($s0)
    4cd8:	96260000 	lhu	$a2,0($s1)
    4cdc:	0260f809 	jalr	$s3
    4ce0:	2404000a 	li	$a0,10
    4ce4:	8e020000 	lw	$v0,0($s0)
    4ce8:	ae22005c 	sw	$v0,92($s1)
    4cec:	8fbf0020 	lw	$ra,32($sp)
    4cf0:	8fb3001c 	lw	$s3,28($sp)
    4cf4:	8fb20018 	lw	$s2,24($sp)
    4cf8:	8fb10014 	lw	$s1,20($sp)
    4cfc:	8fb00010 	lw	$s0,16($sp)
    4d00:	03e00008 	jr	$ra
    4d04:	27bd0028 	addiu	$sp,$sp,40

0000000000004d08 <si3210_set_txpath_gain>:
    4d08:	27bdffe8 	addiu	$sp,$sp,-24
    4d0c:	afbf0010 	sw	$ra,16($sp)
    4d10:	3082ffff 	andi	$v0,$a0,0xffff
    4d14:	30a6ffff 	andi	$a2,$a1,0xffff
    4d18:	00402821 	move	$a1,$v0
    4d1c:	3c020000 	lui	$v0,0x0
    4d20:	24424190 	addiu	$v0,$v0,16784
    4d24:	0040f809 	jalr	$v0
    4d28:	2404001b 	li	$a0,27
    4d2c:	8fbf0010 	lw	$ra,16($sp)
    4d30:	03e00008 	jr	$ra
    4d34:	27bd0018 	addiu	$sp,$sp,24

0000000000004d38 <si3210_set_rxpath_gain>:
    4d38:	27bdffe8 	addiu	$sp,$sp,-24
    4d3c:	afbf0010 	sw	$ra,16($sp)
    4d40:	3082ffff 	andi	$v0,$a0,0xffff
    4d44:	30a6ffff 	andi	$a2,$a1,0xffff
    4d48:	00402821 	move	$a1,$v0
    4d4c:	3c020000 	lui	$v0,0x0
    4d50:	24424190 	addiu	$v0,$v0,16784
    4d54:	0040f809 	jalr	$v0
    4d58:	2404001a 	li	$a0,26
    4d5c:	8fbf0010 	lw	$ra,16($sp)
    4d60:	03e00008 	jr	$ra
    4d64:	27bd0018 	addiu	$sp,$sp,24
    4d68:	27bdffd8 	addiu	$sp,$sp,-40
    4d6c:	afbf0020 	sw	$ra,32($sp)
    4d70:	afb1001c 	sw	$s1,28($sp)
    4d74:	afb00018 	sw	$s0,24($sp)
    4d78:	00801821 	move	$v1,$a0
    4d7c:	00a08821 	move	$s1,$a1
    4d80:	27a50010 	addiu	$a1,$sp,16
    4d84:	3c020000 	lui	$v0,0x0
    4d88:	244233f0 	addiu	$v0,$v0,13296
    4d8c:	0040f809 	jalr	$v0
    4d90:	94700000 	lhu	$s0,0($v1)
    4d94:	3c040000 	lui	$a0,0x0
    4d98:	24841298 	addiu	$a0,$a0,4760
    4d9c:	02203021 	move	$a2,$s1
    4da0:	3c020000 	lui	$v0,0x0
    4da4:	24420000 	addiu	$v0,$v0,0
    4da8:	0040f809 	jalr	$v0
    4dac:	02002821 	move	$a1,$s0
    4db0:	00101080 	sll	$v0,$s0,0x2
    4db4:	3c010000 	lui	$at,0x0
    4db8:	00220821 	addu	$at,$at,$v0
    4dbc:	8c220080 	lw	$v0,128($at)
    4dc0:	10400008 	beqz	$v0,4de4 <si3210_set_rxpath_gain+0xac>
    4dc4:	00000000 	nop
    4dc8:	1220000b 	beqz	$s1,4df8 <si3210_set_rxpath_gain+0xc0>
    4dcc:	24050005 	li	$a1,5
    4dd0:	8fa20010 	lw	$v0,16($sp)
    4dd4:	50400008 	0x50400008
    4dd8:	24050006 	li	$a1,6
    4ddc:	0800137f 	j	4dfc <si3210_set_rxpath_gain+0xc4>
    4de0:	24040040 	li	$a0,64
    4de4:	12200004 	beqz	$s1,4df8 <si3210_set_rxpath_gain+0xc0>
    4de8:	24050001 	li	$a1,1
    4dec:	8fa20010 	lw	$v0,16($sp)
    4df0:	50400001 	0x50400001
    4df4:	24050002 	li	$a1,2
    4df8:	24040040 	li	$a0,64
    4dfc:	3c020000 	lui	$v0,0x0
    4e00:	24420000 	addiu	$v0,$v0,0
    4e04:	0040f809 	jalr	$v0
    4e08:	02003021 	move	$a2,$s0
    4e0c:	8fbf0020 	lw	$ra,32($sp)
    4e10:	8fb1001c 	lw	$s1,28($sp)
    4e14:	8fb00018 	lw	$s0,24($sp)
    4e18:	03e00008 	jr	$ra
    4e1c:	27bd0028 	addiu	$sp,$sp,40

0000000000004e20 <si3210_get_power_alarm_status>:
    4e20:	27bdffe8 	addiu	$sp,$sp,-24
    4e24:	afbf0014 	sw	$ra,20($sp)
    4e28:	afb00010 	sw	$s0,16($sp)
    4e2c:	00a08021 	move	$s0,$a1
    4e30:	3085ffff 	andi	$a1,$a0,0xffff
    4e34:	3c020000 	lui	$v0,0x0
    4e38:	24420000 	addiu	$v0,$v0,0
    4e3c:	0040f809 	jalr	$v0
    4e40:	24040093 	li	$a0,147
    4e44:	304200fc 	andi	$v0,$v0,0xfc
    4e48:	a2020000 	sb	$v0,0($s0)
    4e4c:	8fbf0014 	lw	$ra,20($sp)
    4e50:	8fb00010 	lw	$s0,16($sp)
    4e54:	03e00008 	jr	$ra
    4e58:	27bd0018 	addiu	$sp,$sp,24

0000000000004e5c <si3210_clear_power_alarm_status>:
    4e5c:	27bdffe8 	addiu	$sp,$sp,-24
    4e60:	afbf0014 	sw	$ra,20($sp)
    4e64:	afb00010 	sw	$s0,16($sp)
    4e68:	3090ffff 	andi	$s0,$a0,0xffff
    4e6c:	24040013 	li	$a0,19
    4e70:	240500fc 	li	$a1,252
    4e74:	3c020000 	lui	$v0,0x0
    4e78:	24420000 	addiu	$v0,$v0,0
    4e7c:	0040f809 	jalr	$v0
    4e80:	02003021 	move	$a2,$s0
    4e84:	24040093 	li	$a0,147
    4e88:	3c020000 	lui	$v0,0x0
    4e8c:	24420000 	addiu	$v0,$v0,0
    4e90:	0040f809 	jalr	$v0
    4e94:	02002821 	move	$a1,$s0
    4e98:	3c040000 	lui	$a0,0x0
    4e9c:	248412d0 	addiu	$a0,$a0,4816
    4ea0:	304600fc 	andi	$a2,$v0,0xfc
    4ea4:	3c020000 	lui	$v0,0x0
    4ea8:	24420000 	addiu	$v0,$v0,0
    4eac:	0040f809 	jalr	$v0
    4eb0:	02002821 	move	$a1,$s0
    4eb4:	8fbf0014 	lw	$ra,20($sp)
    4eb8:	8fb00010 	lw	$s0,16($sp)
    4ebc:	03e00008 	jr	$ra
    4ec0:	27bd0018 	addiu	$sp,$sp,24

0000000000004ec4 <si3210_get_power_q>:
    4ec4:	27bdffd8 	addiu	$sp,$sp,-40
    4ec8:	afbf0024 	sw	$ra,36($sp)
    4ecc:	afb40020 	sw	$s4,32($sp)
    4ed0:	afb3001c 	sw	$s3,28($sp)
    4ed4:	afb20018 	sw	$s2,24($sp)
    4ed8:	afb10014 	sw	$s1,20($sp)
    4edc:	afb00010 	sw	$s0,16($sp)
    4ee0:	00008021 	move	$s0,$zero
    4ee4:	00c0a021 	move	$s4,$a2
    4ee8:	30b100ff 	andi	$s1,$a1,0xff
    4eec:	2e220006 	sltiu	$v0,$s1,6
    4ef0:	1040002c 	beqz	$v0,4fa4 <si3210_get_power_q+0xe0>
    4ef4:	3092ffff 	andi	$s2,$a0,0xffff
    4ef8:	2404004c 	li	$a0,76
    4efc:	02202821 	move	$a1,$s1
    4f00:	3c020000 	lui	$v0,0x0
    4f04:	24420000 	addiu	$v0,$v0,0
    4f08:	0040f809 	jalr	$v0
    4f0c:	02403021 	move	$a2,$s2
    4f10:	240400cc 	li	$a0,204
    4f14:	3c100000 	lui	$s0,0x0
    4f18:	26100000 	addiu	$s0,$s0,0
    4f1c:	0200f809 	jalr	$s0
    4f20:	02402821 	move	$a1,$s2
    4f24:	240400cd 	li	$a0,205
    4f28:	02402821 	move	$a1,$s2
    4f2c:	0200f809 	jalr	$s0
    4f30:	00409821 	move	$s3,$v0
    4f34:	00403821 	move	$a3,$v0
    4f38:	2622fffe 	addiu	$v0,$s1,-2
    4f3c:	2c420002 	sltiu	$v0,$v0,2
    4f40:	14400009 	bnez	$v0,4f68 <si3210_get_power_q+0xa4>
    4f44:	000710c0 	sll	$v0,$a3,0x3
    4f48:	00071100 	sll	$v0,$a3,0x4
    4f4c:	00471023 	subu	$v0,$v0,$a3
    4f50:	000210c0 	sll	$v0,$v0,0x3
    4f54:	00471023 	subu	$v0,$v0,$a3
    4f58:	00021080 	sll	$v0,$v0,0x2
    4f5c:	00471023 	subu	$v0,$v0,$a3
    4f60:	080013e0 	j	4f80 <si3210_get_power_q+0xbc>
    4f64:	00028180 	sll	$s0,$v0,0x6
    4f68:	00471023 	subu	$v0,$v0,$a3
    4f6c:	00021100 	sll	$v0,$v0,0x4
    4f70:	00471021 	addu	$v0,$v0,$a3
    4f74:	000210c0 	sll	$v0,$v0,0x3
    4f78:	00471021 	addu	$v0,$v0,$a3
    4f7c:	00028080 	sll	$s0,$v0,0x2
    4f80:	3c040000 	lui	$a0,0x0
    4f84:	2484131c 	addiu	$a0,$a0,4892
    4f88:	02402821 	move	$a1,$s2
    4f8c:	3c020000 	lui	$v0,0x0
    4f90:	24420000 	addiu	$v0,$v0,0
    4f94:	0040f809 	jalr	$v0
    4f98:	26660001 	addiu	$a2,$s3,1
    4f9c:	080013f1 	j	4fc4 <si3210_get_power_q+0x100>
    4fa0:	ae900000 	sw	$s0,0($s4)
    4fa4:	3c040000 	lui	$a0,0x0
    4fa8:	24841354 	addiu	$a0,$a0,4948
    4fac:	02402821 	move	$a1,$s2
    4fb0:	3c020000 	lui	$v0,0x0
    4fb4:	24420000 	addiu	$v0,$v0,0
    4fb8:	0040f809 	jalr	$v0
    4fbc:	02203021 	move	$a2,$s1
    4fc0:	ae900000 	sw	$s0,0($s4)
    4fc4:	8fbf0024 	lw	$ra,36($sp)
    4fc8:	8fb40020 	lw	$s4,32($sp)
    4fcc:	8fb3001c 	lw	$s3,28($sp)
    4fd0:	8fb20018 	lw	$s2,24($sp)
    4fd4:	8fb10014 	lw	$s1,20($sp)
    4fd8:	8fb00010 	lw	$s0,16($sp)
    4fdc:	03e00008 	jr	$ra
    4fe0:	27bd0028 	addiu	$sp,$sp,40

0000000000004fe4 <si3210_set_lpf_pole>:
    4fe4:	27bdffe0 	addiu	$sp,$sp,-32
    4fe8:	afbf001c 	sw	$ra,28($sp)
    4fec:	afb20018 	sw	$s2,24($sp)
    4ff0:	afb10014 	sw	$s1,20($sp)
    4ff4:	afb00010 	sw	$s0,16($sp)
    4ff8:	3092ffff 	andi	$s2,$a0,0xffff
    4ffc:	30b000ff 	andi	$s0,$a1,0xff
    5000:	2602ffff 	addiu	$v0,$s0,-1
    5004:	2c420003 	sltiu	$v0,$v0,3
    5008:	10400043 	beqz	$v0,5118 <si3210_set_lpf_pole+0x134>
    500c:	30c6ffff 	andi	$a2,$a2,0xffff
    5010:	3c010000 	lui	$at,0x0
    5014:	d4201418 	0xd4201418
    5018:	44861000 	mtc1	$a2,$f2
    501c:	00000000 	nop
    5020:	468010a1 	cvt.d.w	$f2,$f2
    5024:	46201082 	mul.d	$f2,$f2,$f0
    5028:	3c0140b0 	lui	$at,0x40b0
    502c:	44810800 	mtc1	$at,$f1
    5030:	44800000 	mtc1	$zero,$f0
    5034:	00000000 	nop
    5038:	46220003 	div.d	$f0,$f0,$f2
    503c:	3c0141e0 	lui	$at,0x41e0
    5040:	44811800 	mtc1	$at,$f3
    5044:	44801000 	mtc1	$zero,$f2
    5048:	00000000 	nop
    504c:	4620103e 	c.le.d	$f2,$f0
    5050:	00000000 	nop
    5054:	45030005 	0x45030005
    5058:	46220001 	sub.d	$f0,$f0,$f2
    505c:	4620008d 	c1	0x20008d
    5060:	44061000 	mfc1	$a2,$f2
    5064:	08001420 	j	5080 <si3210_set_lpf_pole+0x9c>
    5068:	000610c0 	sll	$v0,$a2,0x3
    506c:	4620008d 	c1	0x20008d
    5070:	44061000 	mfc1	$a2,$f2
    5074:	3c028000 	lui	$v0,0x8000
    5078:	00c23025 	or	$a2,$a2,$v0
    507c:	000610c0 	sll	$v0,$a2,0x3
    5080:	3051fff8 	andi	$s1,$v0,0xfff8
    5084:	24020002 	li	$v0,2
    5088:	1202000c 	beq	$s0,$v0,50bc <si3210_set_lpf_pole+0xd8>
    508c:	2a020003 	slti	$v0,$s0,3
    5090:	10400005 	beqz	$v0,50a8 <si3210_set_lpf_pole+0xc4>
    5094:	24020001 	li	$v0,1
    5098:	12020009 	beq	$s0,$v0,50c0 <si3210_set_lpf_pole+0xdc>
    509c:	24040025 	li	$a0,37
    50a0:	0800143c 	j	50f0 <si3210_set_lpf_pole+0x10c>
    50a4:	00000000 	nop
    50a8:	24020003 	li	$v0,3
    50ac:	1202000b 	beq	$s0,$v0,50dc <si3210_set_lpf_pole+0xf8>
    50b0:	24040027 	li	$a0,39
    50b4:	0800143c 	j	50f0 <si3210_set_lpf_pole+0x10c>
    50b8:	00000000 	nop
    50bc:	24040026 	li	$a0,38
    50c0:	02202821 	move	$a1,$s1
    50c4:	3c020000 	lui	$v0,0x0
    50c8:	24424190 	addiu	$v0,$v0,16784
    50cc:	0040f809 	jalr	$v0
    50d0:	02403021 	move	$a2,$s2
    50d4:	0800143c 	j	50f0 <si3210_set_lpf_pole+0x10c>
    50d8:	00000000 	nop
    50dc:	02202821 	move	$a1,$s1
    50e0:	3c020000 	lui	$v0,0x0
    50e4:	24424190 	addiu	$v0,$v0,16784
    50e8:	0040f809 	jalr	$v0
    50ec:	02403021 	move	$a2,$s2
    50f0:	3c040000 	lui	$a0,0x0
    50f4:	24841394 	addiu	$a0,$a0,5012
    50f8:	02402821 	move	$a1,$s2
    50fc:	02003021 	move	$a2,$s0
    5100:	3c020000 	lui	$v0,0x0
    5104:	24420000 	addiu	$v0,$v0,0
    5108:	0040f809 	jalr	$v0
    510c:	02203821 	move	$a3,$s1
    5110:	0800144e 	j	5138 <si3210_set_lpf_pole+0x154>
    5114:	8fbf001c 	lw	$ra,28($sp)
    5118:	3c040000 	lui	$a0,0x0
    511c:	248413d4 	addiu	$a0,$a0,5076
    5120:	02402821 	move	$a1,$s2
    5124:	3c020000 	lui	$v0,0x0
    5128:	24420000 	addiu	$v0,$v0,0
    512c:	0040f809 	jalr	$v0
    5130:	02003021 	move	$a2,$s0
    5134:	8fbf001c 	lw	$ra,28($sp)
    5138:	8fb20018 	lw	$s2,24($sp)
    513c:	8fb10014 	lw	$s1,20($sp)
    5140:	8fb00010 	lw	$s0,16($sp)
    5144:	03e00008 	jr	$ra
    5148:	27bd0020 	addiu	$sp,$sp,32

000000000000514c <si3210_get_lpf_pole>:
    514c:	27bdffd8 	addiu	$sp,$sp,-40
    5150:	afbf0020 	sw	$ra,32($sp)
    5154:	afb3001c 	sw	$s3,28($sp)
    5158:	afb20018 	sw	$s2,24($sp)
    515c:	afb10014 	sw	$s1,20($sp)
    5160:	afb00010 	sw	$s0,16($sp)
    5164:	00008821 	move	$s1,$zero
    5168:	00c09821 	move	$s3,$a2
    516c:	30b000ff 	andi	$s0,$a1,0xff
    5170:	2602ffff 	addiu	$v0,$s0,-1
    5174:	2c420003 	sltiu	$v0,$v0,3
    5178:	1040003c 	beqz	$v0,526c <si3210_get_lpf_pole+0x120>
    517c:	3092ffff 	andi	$s2,$a0,0xffff
    5180:	24020002 	li	$v0,2
    5184:	1202000c 	beq	$s0,$v0,51b8 <si3210_get_lpf_pole+0x6c>
    5188:	2a020003 	slti	$v0,$s0,3
    518c:	10400005 	beqz	$v0,51a4 <si3210_get_lpf_pole+0x58>
    5190:	24020001 	li	$v0,1
    5194:	12020009 	beq	$s0,$v0,51bc <si3210_get_lpf_pole+0x70>
    5198:	24040025 	li	$a0,37
    519c:	08001474 	j	51d0 <si3210_get_lpf_pole+0x84>
    51a0:	00000000 	nop
    51a4:	24020003 	li	$v0,3
    51a8:	12020004 	beq	$s0,$v0,51bc <si3210_get_lpf_pole+0x70>
    51ac:	24040027 	li	$a0,39
    51b0:	08001474 	j	51d0 <si3210_get_lpf_pole+0x84>
    51b4:	00000000 	nop
    51b8:	24040026 	li	$a0,38
    51bc:	3c020000 	lui	$v0,0x0
    51c0:	24424218 	addiu	$v0,$v0,16920
    51c4:	0040f809 	jalr	$v0
    51c8:	02402821 	move	$a1,$s2
    51cc:	00408821 	move	$s1,$v0
    51d0:	3c040000 	lui	$a0,0x0
    51d4:	24841420 	addiu	$a0,$a0,5152
    51d8:	02402821 	move	$a1,$s2
    51dc:	02003021 	move	$a2,$s0
    51e0:	3c020000 	lui	$v0,0x0
    51e4:	24420000 	addiu	$v0,$v0,0
    51e8:	0040f809 	jalr	$v0
    51ec:	02203821 	move	$a3,$s1
    51f0:	001110c2 	srl	$v0,$s1,0x3
    51f4:	3051ffff 	andi	$s1,$v0,0xffff
    51f8:	3c010000 	lui	$at,0x0
    51fc:	d4221468 	0xd4221468
    5200:	44910000 	mtc1	$s1,$f0
    5204:	00000000 	nop
    5208:	46800021 	cvt.d.w	$f0,$f0
    520c:	46220002 	mul.d	$f0,$f0,$f2
    5210:	3c0140b0 	lui	$at,0x40b0
    5214:	44811800 	mtc1	$at,$f3
    5218:	44801000 	mtc1	$zero,$f2
    521c:	00000000 	nop
    5220:	46201003 	div.d	$f0,$f2,$f0
    5224:	3c0141e0 	lui	$at,0x41e0
    5228:	44811800 	mtc1	$at,$f3
    522c:	44801000 	mtc1	$zero,$f2
    5230:	00000000 	nop
    5234:	4620103e 	c.le.d	$f2,$f0
    5238:	00000000 	nop
    523c:	45030005 	0x45030005
    5240:	46220001 	sub.d	$f0,$f0,$f2
    5244:	4620008d 	c1	0x20008d
    5248:	44071000 	mfc1	$a3,$f2
    524c:	080014a2 	j	5288 <si3210_get_lpf_pole+0x13c>
    5250:	a6670000 	sh	$a3,0($s3)
    5254:	4620008d 	c1	0x20008d
    5258:	44071000 	mfc1	$a3,$f2
    525c:	3c028000 	lui	$v0,0x8000
    5260:	00e23825 	or	$a3,$a3,$v0
    5264:	080014a2 	j	5288 <si3210_get_lpf_pole+0x13c>
    5268:	a6670000 	sh	$a3,0($s3)
    526c:	3c040000 	lui	$a0,0x0
    5270:	248413d4 	addiu	$a0,$a0,5076
    5274:	02402821 	move	$a1,$s2
    5278:	3c020000 	lui	$v0,0x0
    527c:	24420000 	addiu	$v0,$v0,0
    5280:	0040f809 	jalr	$v0
    5284:	02003021 	move	$a2,$s0
    5288:	8fbf0020 	lw	$ra,32($sp)
    528c:	8fb3001c 	lw	$s3,28($sp)
    5290:	8fb20018 	lw	$s2,24($sp)
    5294:	8fb10014 	lw	$s1,20($sp)
    5298:	8fb00010 	lw	$s0,16($sp)
    529c:	03e00008 	jr	$ra
    52a0:	27bd0028 	addiu	$sp,$sp,40

00000000000052a4 <si3210_set_power_thres>:
    52a4:	27bdffd0 	addiu	$sp,$sp,-48
    52a8:	afbf002c 	sw	$ra,44($sp)
    52ac:	afb40028 	sw	$s4,40($sp)
    52b0:	afb30024 	sw	$s3,36($sp)
    52b4:	afb20020 	sw	$s2,32($sp)
    52b8:	afb1001c 	sw	$s1,28($sp)
    52bc:	afb00018 	sw	$s0,24($sp)
    52c0:	30d1ffff 	andi	$s1,$a2,0xffff
    52c4:	00008021 	move	$s0,$zero
    52c8:	30b300ff 	andi	$s3,$a1,0xff
    52cc:	2662ffff 	addiu	$v0,$s3,-1
    52d0:	2c420003 	sltiu	$v0,$v0,3
    52d4:	104000a8 	beqz	$v0,5578 <si3210_set_power_thres+0x2d4>
    52d8:	3092ffff 	andi	$s2,$a0,0xffff
    52dc:	24140001 	li	$s4,1
    52e0:	12740003 	beq	$s3,$s4,52f0 <si3210_set_power_thres+0x4c>
    52e4:	24020003 	li	$v0,3
    52e8:	1662004e 	bne	$s3,$v0,5424 <si3210_set_power_thres+0x180>
    52ec:	2e220385 	sltiu	$v0,$s1,901
    52f0:	2e221e79 	sltiu	$v0,$s1,7801
    52f4:	10400025 	beqz	$v0,538c <si3210_set_power_thres+0xe8>
    52f8:	02402821 	move	$a1,$s2
    52fc:	3c010000 	lui	$at,0x0
    5300:	d4221548 	0xd4221548
    5304:	44910000 	mtc1	$s1,$f0
    5308:	00000000 	nop
    530c:	46800021 	cvt.d.w	$f0,$f0
    5310:	46220003 	div.d	$f0,$f0,$f2
    5314:	3c0141e0 	lui	$at,0x41e0
    5318:	44811800 	mtc1	$at,$f3
    531c:	44801000 	mtc1	$zero,$f2
    5320:	00000000 	nop
    5324:	4620103e 	c.le.d	$f2,$f0
    5328:	00000000 	nop
    532c:	45030005 	0x45030005
    5330:	46220001 	sub.d	$f0,$f0,$f2
    5334:	4620008d 	c1	0x20008d
    5338:	44031000 	mfc1	$v1,$f2
    533c:	080014d6 	j	5358 <si3210_set_power_thres+0xb4>
    5340:	000311c0 	sll	$v0,$v1,0x7
    5344:	4620008d 	c1	0x20008d
    5348:	44031000 	mfc1	$v1,$f2
    534c:	3c028000 	lui	$v0,0x8000
    5350:	00621825 	or	$v1,$v1,$v0
    5354:	000311c0 	sll	$v0,$v1,0x7
    5358:	3050ff80 	andi	$s0,$v0,0xff80
    535c:	24020001 	li	$v0,1
    5360:	12620005 	beq	$s3,$v0,5378 <si3210_set_power_thres+0xd4>
    5364:	24020003 	li	$v0,3
    5368:	12620006 	beq	$s3,$v0,5384 <si3210_set_power_thres+0xe0>
    536c:	24040022 	li	$a0,34
    5370:	0800154e 	j	5538 <si3210_set_power_thres+0x294>
    5374:	00000000 	nop
    5378:	24040020 	li	$a0,32
    537c:	08001524 	j	5490 <si3210_set_power_thres+0x1ec>
    5380:	02002821 	move	$a1,$s0
    5384:	08001524 	j	5490 <si3210_set_power_thres+0x1ec>
    5388:	02002821 	move	$a1,$s0
    538c:	3c040000 	lui	$a0,0x0
    5390:	24841470 	addiu	$a0,$a0,5232
    5394:	3c020000 	lui	$v0,0x0
    5398:	24420000 	addiu	$v0,$v0,0
    539c:	0040f809 	jalr	$v0
    53a0:	02203021 	move	$a2,$s1
    53a4:	12740005 	beq	$s3,$s4,53bc <si3210_set_power_thres+0x118>
    53a8:	24020003 	li	$v0,3
    53ac:	12620005 	beq	$s3,$v0,53c4 <si3210_set_power_thres+0x120>
    53b0:	001011c2 	srl	$v0,$s0,0x7
    53b4:	080014f9 	j	53e4 <si3210_set_power_thres+0x140>
    53b8:	3050ffff 	andi	$s0,$v0,0xffff
    53bc:	080014f2 	j	53c8 <si3210_set_power_thres+0x124>
    53c0:	24040020 	li	$a0,32
    53c4:	24040022 	li	$a0,34
    53c8:	3c020000 	lui	$v0,0x0
    53cc:	24424218 	addiu	$v0,$v0,16920
    53d0:	0040f809 	jalr	$v0
    53d4:	02402821 	move	$a1,$s2
    53d8:	00408021 	move	$s0,$v0
    53dc:	001011c2 	srl	$v0,$s0,0x7
    53e0:	3050ffff 	andi	$s0,$v0,0xffff
    53e4:	3c010000 	lui	$at,0x0
    53e8:	d4221550 	0xd4221550
    53ec:	44900000 	mtc1	$s0,$f0
    53f0:	00000000 	nop
    53f4:	46800021 	cvt.d.w	$f0,$f0
    53f8:	46220002 	mul.d	$f0,$f0,$f2
    53fc:	3c0141e0 	lui	$at,0x41e0
    5400:	44811800 	mtc1	$at,$f3
    5404:	44801000 	mtc1	$zero,$f2
    5408:	00000000 	nop
    540c:	4620103e 	c.le.d	$f2,$f0
    5410:	00000000 	nop
    5414:	4500003f 	bc1f	5514 <si3210_set_power_thres+0x270>
    5418:	00000000 	nop
    541c:	08001549 	j	5524 <si3210_set_power_thres+0x280>
    5420:	46220001 	sub.d	$f0,$f0,$f2
    5424:	10400020 	beqz	$v0,54a8 <si3210_set_power_thres+0x204>
    5428:	02402821 	move	$a1,$s2
    542c:	3c010000 	lui	$at,0x0
    5430:	d4221558 	0xd4221558
    5434:	44910000 	mtc1	$s1,$f0
    5438:	00000000 	nop
    543c:	46800021 	cvt.d.w	$f0,$f0
    5440:	46220003 	div.d	$f0,$f0,$f2
    5444:	3c0141e0 	lui	$at,0x41e0
    5448:	44811800 	mtc1	$at,$f3
    544c:	44801000 	mtc1	$zero,$f2
    5450:	00000000 	nop
    5454:	4620103e 	c.le.d	$f2,$f0
    5458:	00000000 	nop
    545c:	45030005 	0x45030005
    5460:	46220001 	sub.d	$f0,$f0,$f2
    5464:	4620008d 	c1	0x20008d
    5468:	44051000 	mfc1	$a1,$f2
    546c:	08001522 	j	5488 <si3210_set_power_thres+0x1e4>
    5470:	24040021 	li	$a0,33
    5474:	4620008d 	c1	0x20008d
    5478:	44051000 	mfc1	$a1,$f2
    547c:	3c028000 	lui	$v0,0x8000
    5480:	00a22825 	or	$a1,$a1,$v0
    5484:	24040021 	li	$a0,33
    5488:	000529c0 	sll	$a1,$a1,0x7
    548c:	30a5ff80 	andi	$a1,$a1,0xff80
    5490:	3c020000 	lui	$v0,0x0
    5494:	24424190 	addiu	$v0,$v0,16784
    5498:	0040f809 	jalr	$v0
    549c:	02403021 	move	$a2,$s2
    54a0:	0800154e 	j	5538 <si3210_set_power_thres+0x294>
    54a4:	00000000 	nop
    54a8:	3c040000 	lui	$a0,0x0
    54ac:	248414bc 	addiu	$a0,$a0,5308
    54b0:	3c020000 	lui	$v0,0x0
    54b4:	24420000 	addiu	$v0,$v0,0
    54b8:	0040f809 	jalr	$v0
    54bc:	02203021 	move	$a2,$s1
    54c0:	24040022 	li	$a0,34
    54c4:	3c020000 	lui	$v0,0x0
    54c8:	24424218 	addiu	$v0,$v0,16920
    54cc:	0040f809 	jalr	$v0
    54d0:	02402821 	move	$a1,$s2
    54d4:	000211c2 	srl	$v0,$v0,0x7
    54d8:	3050ffff 	andi	$s0,$v0,0xffff
    54dc:	3c010000 	lui	$at,0x0
    54e0:	d4221560 	0xd4221560
    54e4:	44900000 	mtc1	$s0,$f0
    54e8:	00000000 	nop
    54ec:	46800021 	cvt.d.w	$f0,$f0
    54f0:	46220002 	mul.d	$f0,$f0,$f2
    54f4:	3c0141e0 	lui	$at,0x41e0
    54f8:	44811800 	mtc1	$at,$f3
    54fc:	44801000 	mtc1	$zero,$f2
    5500:	00000000 	nop
    5504:	4620103e 	c.le.d	$f2,$f0
    5508:	00000000 	nop
    550c:	45030005 	0x45030005
    5510:	46220001 	sub.d	$f0,$f0,$f2
    5514:	4620008d 	c1	0x20008d
    5518:	44051000 	mfc1	$a1,$f2
    551c:	0800154e 	j	5538 <si3210_set_power_thres+0x294>
    5520:	30b1ffff 	andi	$s1,$a1,0xffff
    5524:	4620008d 	c1	0x20008d
    5528:	44051000 	mfc1	$a1,$f2
    552c:	3c028000 	lui	$v0,0x8000
    5530:	00a22825 	or	$a1,$a1,$v0
    5534:	30b1ffff 	andi	$s1,$a1,0xffff
    5538:	3c010000 	lui	$at,0x0
    553c:	d4201568 	0xd4201568
    5540:	44911000 	mtc1	$s1,$f2
    5544:	00000000 	nop
    5548:	468010a1 	cvt.d.w	$f2,$f2
    554c:	46201083 	div.d	$f2,$f2,$f0
    5550:	3c040000 	lui	$a0,0x0
    5554:	24841508 	addiu	$a0,$a0,5384
    5558:	02402821 	move	$a1,$s2
    555c:	3c020000 	lui	$v0,0x0
    5560:	24420000 	addiu	$v0,$v0,0
    5564:	f7a20010 	0xf7a20010
    5568:	0040f809 	jalr	$v0
    556c:	02603021 	move	$a2,$s3
    5570:	08001566 	j	5598 <si3210_set_power_thres+0x2f4>
    5574:	8fbf002c 	lw	$ra,44($sp)
    5578:	3c040000 	lui	$a0,0x0
    557c:	248413d4 	addiu	$a0,$a0,5076
    5580:	02402821 	move	$a1,$s2
    5584:	3c020000 	lui	$v0,0x0
    5588:	24420000 	addiu	$v0,$v0,0
    558c:	0040f809 	jalr	$v0
    5590:	02603021 	move	$a2,$s3
    5594:	8fbf002c 	lw	$ra,44($sp)
    5598:	8fb40028 	lw	$s4,40($sp)
    559c:	8fb30024 	lw	$s3,36($sp)
    55a0:	8fb20020 	lw	$s2,32($sp)
    55a4:	8fb1001c 	lw	$s1,28($sp)
    55a8:	8fb00018 	lw	$s0,24($sp)
    55ac:	03e00008 	jr	$ra
    55b0:	27bd0030 	addiu	$sp,$sp,48

00000000000055b4 <si3210_get_power_thres>:
    55b4:	27bdffd8 	addiu	$sp,$sp,-40
    55b8:	afbf0020 	sw	$ra,32($sp)
    55bc:	afb3001c 	sw	$s3,28($sp)
    55c0:	afb20018 	sw	$s2,24($sp)
    55c4:	afb10014 	sw	$s1,20($sp)
    55c8:	afb00010 	sw	$s0,16($sp)
    55cc:	00003821 	move	$a3,$zero
    55d0:	00008021 	move	$s0,$zero
    55d4:	00c09821 	move	$s3,$a2
    55d8:	30b100ff 	andi	$s1,$a1,0xff
    55dc:	2622ffff 	addiu	$v0,$s1,-1
    55e0:	2c420003 	sltiu	$v0,$v0,3
    55e4:	10400058 	beqz	$v0,5748 <si3210_get_power_thres+0x194>
    55e8:	3092ffff 	andi	$s2,$a0,0xffff
    55ec:	24020002 	li	$v0,2
    55f0:	12220023 	beq	$s1,$v0,5680 <si3210_get_power_thres+0xcc>
    55f4:	2a220003 	slti	$v0,$s1,3
    55f8:	10400005 	beqz	$v0,5610 <si3210_get_power_thres+0x5c>
    55fc:	24020001 	li	$v0,1
    5600:	12220008 	beq	$s1,$v0,5624 <si3210_get_power_thres+0x70>
    5604:	24040020 	li	$a0,32
    5608:	080015c9 	j	5724 <si3210_get_power_thres+0x170>
    560c:	00000000 	nop
    5610:	24020003 	li	$v0,3
    5614:	12220025 	beq	$s1,$v0,56ac <si3210_get_power_thres+0xf8>
    5618:	24040022 	li	$a0,34
    561c:	080015c9 	j	5724 <si3210_get_power_thres+0x170>
    5620:	00000000 	nop
    5624:	3c020000 	lui	$v0,0x0
    5628:	24424218 	addiu	$v0,$v0,16920
    562c:	0040f809 	jalr	$v0
    5630:	02402821 	move	$a1,$s2
    5634:	00403821 	move	$a3,$v0
    5638:	000711c2 	srl	$v0,$a3,0x7
    563c:	3050ffff 	andi	$s0,$v0,0xffff
    5640:	3c010000 	lui	$at,0x0
    5644:	d42215b8 	0xd42215b8
    5648:	44900000 	mtc1	$s0,$f0
    564c:	00000000 	nop
    5650:	46800021 	cvt.d.w	$f0,$f0
    5654:	46220002 	mul.d	$f0,$f0,$f2
    5658:	3c0141e0 	lui	$at,0x41e0
    565c:	44811800 	mtc1	$at,$f3
    5660:	44801000 	mtc1	$zero,$f2
    5664:	00000000 	nop
    5668:	4620103e 	c.le.d	$f2,$f0
    566c:	00000000 	nop
    5670:	45000023 	bc1f	5700 <si3210_get_power_thres+0x14c>
    5674:	00000000 	nop
    5678:	080015c4 	j	5710 <si3210_get_power_thres+0x15c>
    567c:	46220001 	sub.d	$f0,$f0,$f2
    5680:	24040021 	li	$a0,33
    5684:	3c020000 	lui	$v0,0x0
    5688:	24424218 	addiu	$v0,$v0,16920
    568c:	0040f809 	jalr	$v0
    5690:	02402821 	move	$a1,$s2
    5694:	00403821 	move	$a3,$v0
    5698:	000711c2 	srl	$v0,$a3,0x7
    569c:	3c010000 	lui	$at,0x0
    56a0:	d42215c0 	0xd42215c0
    56a4:	08001592 	j	5648 <si3210_get_power_thres+0x94>
    56a8:	3050ffff 	andi	$s0,$v0,0xffff
    56ac:	3c020000 	lui	$v0,0x0
    56b0:	24424218 	addiu	$v0,$v0,16920
    56b4:	0040f809 	jalr	$v0
    56b8:	02402821 	move	$a1,$s2
    56bc:	00403821 	move	$a3,$v0
    56c0:	000711c2 	srl	$v0,$a3,0x7
    56c4:	3050ffff 	andi	$s0,$v0,0xffff
    56c8:	3c010000 	lui	$at,0x0
    56cc:	d42215c8 	0xd42215c8
    56d0:	44900000 	mtc1	$s0,$f0
    56d4:	00000000 	nop
    56d8:	46800021 	cvt.d.w	$f0,$f0
    56dc:	46220002 	mul.d	$f0,$f0,$f2
    56e0:	3c0141e0 	lui	$at,0x41e0
    56e4:	44811800 	mtc1	$at,$f3
    56e8:	44801000 	mtc1	$zero,$f2
    56ec:	00000000 	nop
    56f0:	4620103e 	c.le.d	$f2,$f0
    56f4:	00000000 	nop
    56f8:	45030005 	0x45030005
    56fc:	46220001 	sub.d	$f0,$f0,$f2
    5700:	4620008d 	c1	0x20008d
    5704:	44031000 	mfc1	$v1,$f2
    5708:	080015c9 	j	5724 <si3210_get_power_thres+0x170>
    570c:	3070ffff 	andi	$s0,$v1,0xffff
    5710:	4620008d 	c1	0x20008d
    5714:	44031000 	mfc1	$v1,$f2
    5718:	3c028000 	lui	$v0,0x8000
    571c:	00621825 	or	$v1,$v1,$v0
    5720:	3070ffff 	andi	$s0,$v1,0xffff
    5724:	3c040000 	lui	$a0,0x0
    5728:	24841570 	addiu	$a0,$a0,5488
    572c:	02402821 	move	$a1,$s2
    5730:	3c020000 	lui	$v0,0x0
    5734:	24420000 	addiu	$v0,$v0,0
    5738:	0040f809 	jalr	$v0
    573c:	02203021 	move	$a2,$s1
    5740:	080015da 	j	5768 <si3210_get_power_thres+0x1b4>
    5744:	a6700000 	sh	$s0,0($s3)
    5748:	3c040000 	lui	$a0,0x0
    574c:	248413d4 	addiu	$a0,$a0,5076
    5750:	02402821 	move	$a1,$s2
    5754:	3c020000 	lui	$v0,0x0
    5758:	24420000 	addiu	$v0,$v0,0
    575c:	0040f809 	jalr	$v0
    5760:	02203021 	move	$a2,$s1
    5764:	a6700000 	sh	$s0,0($s3)
    5768:	8fbf0020 	lw	$ra,32($sp)
    576c:	8fb3001c 	lw	$s3,28($sp)
    5770:	8fb20018 	lw	$s2,24($sp)
    5774:	8fb10014 	lw	$s1,20($sp)
    5778:	8fb00010 	lw	$s0,16($sp)
    577c:	03e00008 	jr	$ra
    5780:	27bd0028 	addiu	$sp,$sp,40

0000000000005784 <si3210_set_power_alarm_auto>:
    5784:	27bdffe0 	addiu	$sp,$sp,-32
    5788:	afbf001c 	sw	$ra,28($sp)
    578c:	afb20018 	sw	$s2,24($sp)
    5790:	afb10014 	sw	$s1,20($sp)
    5794:	afb00010 	sw	$s0,16($sp)
    5798:	3091ffff 	andi	$s1,$a0,0xffff
    579c:	240400c3 	li	$a0,195
    57a0:	00a09021 	move	$s2,$a1
    57a4:	3c020000 	lui	$v0,0x0
    57a8:	24420000 	addiu	$v0,$v0,0
    57ac:	0040f809 	jalr	$v0
    57b0:	02202821 	move	$a1,$s1
    57b4:	12400004 	beqz	$s2,57c8 <si3210_set_power_alarm_auto+0x44>
    57b8:	00408021 	move	$s0,$v0
    57bc:	3602001f 	ori	$v0,$s0,0x1f
    57c0:	080015f3 	j	57cc <si3210_set_power_alarm_auto+0x48>
    57c4:	305000ff 	andi	$s0,$v0,0xff
    57c8:	321000f7 	andi	$s0,$s0,0xf7
    57cc:	24040043 	li	$a0,67
    57d0:	02002821 	move	$a1,$s0
    57d4:	3c020000 	lui	$v0,0x0
    57d8:	24420000 	addiu	$v0,$v0,0
    57dc:	0040f809 	jalr	$v0
    57e0:	02203021 	move	$a2,$s1
    57e4:	3c060000 	lui	$a2,0x0
    57e8:	12400003 	beqz	$s2,57f8 <si3210_set_power_alarm_auto+0x74>
    57ec:	24c61618 	addiu	$a2,$a2,5656
    57f0:	3c060000 	lui	$a2,0x0
    57f4:	24c61610 	addiu	$a2,$a2,5648
    57f8:	3c040000 	lui	$a0,0x0
    57fc:	248415d0 	addiu	$a0,$a0,5584
    5800:	02202821 	move	$a1,$s1
    5804:	3c020000 	lui	$v0,0x0
    5808:	24420000 	addiu	$v0,$v0,0
    580c:	0040f809 	jalr	$v0
    5810:	02003821 	move	$a3,$s0
    5814:	8fbf001c 	lw	$ra,28($sp)
    5818:	8fb20018 	lw	$s2,24($sp)
    581c:	8fb10014 	lw	$s1,20($sp)
    5820:	8fb00010 	lw	$s0,16($sp)
    5824:	03e00008 	jr	$ra
    5828:	27bd0020 	addiu	$sp,$sp,32
    582c:	27bdffe0 	addiu	$sp,$sp,-32
    5830:	afbf001c 	sw	$ra,28($sp)
    5834:	afb20018 	sw	$s2,24($sp)
    5838:	afb10014 	sw	$s1,20($sp)
    583c:	afb00010 	sw	$s0,16($sp)
    5840:	00809021 	move	$s2,$a0
    5844:	24a5d8ef 	addiu	$a1,$a1,-10001
    5848:	2ca2000d 	sltiu	$v0,$a1,13
    584c:	10400054 	beqz	$v0,59a0 <si3210_set_power_alarm_auto+0x21c>
    5850:	00c08821 	move	$s1,$a2
    5854:	00051080 	sll	$v0,$a1,0x2
    5858:	3c010000 	lui	$at,0x0
    585c:	00220821 	addu	$at,$at,$v0
    5860:	8c221678 	lw	$v0,5752($at)
    5864:	00400008 	jr	$v0
    5868:	00000000 	nop
    586c:	02402021 	move	$a0,$s2
    5870:	08001620 	j	5880 <si3210_set_power_alarm_auto+0xfc>
    5874:	24050001 	li	$a1,1
    5878:	02402021 	move	$a0,$s2
    587c:	00002821 	move	$a1,$zero
    5880:	3c020000 	lui	$v0,0x0
    5884:	24424d68 	addiu	$v0,$v0,19816
    5888:	0040f809 	jalr	$v0
    588c:	00000000 	nop
    5890:	08001669 	j	59a4 <si3210_set_power_alarm_auto+0x220>
    5894:	8fbf001c 	lw	$ra,28($sp)
    5898:	3c020000 	lui	$v0,0x0
    589c:	24420000 	addiu	$v0,$v0,0
    58a0:	92240000 	lbu	$a0,0($s1)
    58a4:	96450000 	lhu	$a1,0($s2)
    58a8:	0040f809 	jalr	$v0
    58ac:	34840080 	ori	$a0,$a0,0x80
    58b0:	08001668 	j	59a0 <si3210_set_power_alarm_auto+0x21c>
    58b4:	a2220002 	sb	$v0,2($s1)
    58b8:	92240000 	lbu	$a0,0($s1)
    58bc:	92250002 	lbu	$a1,2($s1)
    58c0:	3c020000 	lui	$v0,0x0
    58c4:	24420000 	addiu	$v0,$v0,0
    58c8:	0040f809 	jalr	$v0
    58cc:	96460000 	lhu	$a2,0($s2)
    58d0:	08001669 	j	59a4 <si3210_set_power_alarm_auto+0x220>
    58d4:	8fbf001c 	lw	$ra,28($sp)
    58d8:	3c040000 	lui	$a0,0x0
    58dc:	24841620 	addiu	$a0,$a0,5664
    58e0:	92250000 	lbu	$a1,0($s1)
    58e4:	96470000 	lhu	$a3,0($s2)
    58e8:	92220002 	lbu	$v0,2($s1)
    58ec:	92300003 	lbu	$s0,3($s1)
    58f0:	00021200 	sll	$v0,$v0,0x8
    58f4:	02028025 	or	$s0,$s0,$v0
    58f8:	3c020000 	lui	$v0,0x0
    58fc:	24420000 	addiu	$v0,$v0,0
    5900:	0040f809 	jalr	$v0
    5904:	02003021 	move	$a2,$s0
    5908:	02002821 	move	$a1,$s0
    590c:	92240000 	lbu	$a0,0($s1)
    5910:	3c020000 	lui	$v0,0x0
    5914:	24424190 	addiu	$v0,$v0,16784
    5918:	0040f809 	jalr	$v0
    591c:	96460000 	lhu	$a2,0($s2)
    5920:	08001669 	j	59a4 <si3210_set_power_alarm_auto+0x220>
    5924:	8fbf001c 	lw	$ra,28($sp)
    5928:	92240000 	lbu	$a0,0($s1)
    592c:	3c020000 	lui	$v0,0x0
    5930:	24424218 	addiu	$v0,$v0,16920
    5934:	0040f809 	jalr	$v0
    5938:	96450000 	lhu	$a1,0($s2)
    593c:	3c040000 	lui	$a0,0x0
    5940:	24841648 	addiu	$a0,$a0,5704
    5944:	00408021 	move	$s0,$v0
    5948:	02003821 	move	$a3,$s0
    594c:	96450000 	lhu	$a1,0($s2)
    5950:	3c020000 	lui	$v0,0x0
    5954:	24420000 	addiu	$v0,$v0,0
    5958:	0040f809 	jalr	$v0
    595c:	92260000 	lbu	$a2,0($s1)
    5960:	a2300003 	sb	$s0,3($s1)
    5964:	00108202 	srl	$s0,$s0,0x8
    5968:	08001668 	j	59a0 <si3210_set_power_alarm_auto+0x21c>
    596c:	a2300002 	sb	$s0,2($s1)
    5970:	02402021 	move	$a0,$s2
    5974:	3c020000 	lui	$v0,0x0
    5978:	24424348 	addiu	$v0,$v0,17224
    597c:	0040f809 	jalr	$v0
    5980:	02202821 	move	$a1,$s1
    5984:	08001669 	j	59a4 <si3210_set_power_alarm_auto+0x220>
    5988:	8fbf001c 	lw	$ra,28($sp)
    598c:	02402021 	move	$a0,$s2
    5990:	3c020000 	lui	$v0,0x0
    5994:	24424ad8 	addiu	$v0,$v0,19160
    5998:	0040f809 	jalr	$v0
    599c:	02202821 	move	$a1,$s1
    59a0:	8fbf001c 	lw	$ra,28($sp)
    59a4:	8fb20018 	lw	$s2,24($sp)
    59a8:	8fb10014 	lw	$s1,20($sp)
    59ac:	8fb00010 	lw	$s0,16($sp)
    59b0:	03e00008 	jr	$ra
    59b4:	27bd0020 	addiu	$sp,$sp,32
    59b8:	27bdffe8 	addiu	$sp,$sp,-24
    59bc:	afbf0014 	sw	$ra,20($sp)
    59c0:	afb00010 	sw	$s0,16($sp)
    59c4:	00808021 	move	$s0,$a0
    59c8:	8e020020 	lw	$v0,32($s0)
    59cc:	54400009 	0x54400009
    59d0:	02002021 	move	$a0,$s0
    59d4:	3c020000 	lui	$v0,0x0
    59d8:	24423070 	addiu	$v0,$v0,12400
    59dc:	0040f809 	jalr	$v0
    59e0:	00000000 	nop
    59e4:	14400003 	bnez	$v0,59f4 <si3210_set_power_alarm_auto+0x270>
    59e8:	02002021 	move	$a0,$s0
    59ec:	08001683 	j	5a0c <si3210_set_power_alarm_auto+0x288>
    59f0:	00001021 	move	$v0,$zero
    59f4:	3c020000 	lui	$v0,0x0
    59f8:	24423538 	addiu	$v0,$v0,13624
    59fc:	0040f809 	jalr	$v0
    5a00:	00002821 	move	$a1,$zero
    5a04:	24020001 	li	$v0,1
    5a08:	ae020020 	sw	$v0,32($s0)
    5a0c:	8fbf0014 	lw	$ra,20($sp)
    5a10:	8fb00010 	lw	$s0,16($sp)
    5a14:	03e00008 	jr	$ra
    5a18:	27bd0018 	addiu	$sp,$sp,24
    5a1c:	27bdffe8 	addiu	$sp,$sp,-24
    5a20:	afbf0014 	sw	$ra,20($sp)
    5a24:	afb00010 	sw	$s0,16($sp)
    5a28:	00808021 	move	$s0,$a0
    5a2c:	8e020020 	lw	$v0,32($s0)
    5a30:	54400009 	0x54400009
    5a34:	02002021 	move	$a0,$s0
    5a38:	3c020000 	lui	$v0,0x0
    5a3c:	24423070 	addiu	$v0,$v0,12400
    5a40:	0040f809 	jalr	$v0
    5a44:	00000000 	nop
    5a48:	14400003 	bnez	$v0,5a58 <si3210_set_power_alarm_auto+0x2d4>
    5a4c:	02002021 	move	$a0,$s0
    5a50:	0800169b 	j	5a6c <si3210_set_power_alarm_auto+0x2e8>
    5a54:	00001021 	move	$v0,$zero
    5a58:	3c020000 	lui	$v0,0x0
    5a5c:	244234e8 	addiu	$v0,$v0,13544
    5a60:	0040f809 	jalr	$v0
    5a64:	00002821 	move	$a1,$zero
    5a68:	24020001 	li	$v0,1
    5a6c:	8fbf0014 	lw	$ra,20($sp)
    5a70:	8fb00010 	lw	$s0,16($sp)
    5a74:	03e00008 	jr	$ra
    5a78:	27bd0018 	addiu	$sp,$sp,24
    5a7c:	27bdffd8 	addiu	$sp,$sp,-40
    5a80:	afbf0020 	sw	$ra,32($sp)
    5a84:	afb1001c 	sw	$s1,28($sp)
    5a88:	afb00018 	sw	$s0,24($sp)
    5a8c:	3c110000 	lui	$s1,0x0
    5a90:	26313538 	addiu	$s1,$s1,13624
    5a94:	0220f809 	jalr	$s1
    5a98:	00808021 	move	$s0,$a0
    5a9c:	02002021 	move	$a0,$s0
    5aa0:	3c020000 	lui	$v0,0x0
    5aa4:	244233f0 	addiu	$v0,$v0,13296
    5aa8:	0040f809 	jalr	$v0
    5aac:	27a50010 	addiu	$a1,$sp,16
    5ab0:	8fa20010 	lw	$v0,16($sp)
    5ab4:	10400003 	beqz	$v0,5ac4 <si3210_set_power_alarm_auto+0x340>
    5ab8:	02002021 	move	$a0,$s0
    5abc:	0220f809 	jalr	$s1
    5ac0:	00002821 	move	$a1,$zero
    5ac4:	8fbf0020 	lw	$ra,32($sp)
    5ac8:	8fb1001c 	lw	$s1,28($sp)
    5acc:	8fb00018 	lw	$s0,24($sp)
    5ad0:	03e00008 	jr	$ra
    5ad4:	27bd0028 	addiu	$sp,$sp,40
    5ad8:	03e00008 	jr	$ra
    5adc:	00001021 	move	$v0,$zero
    5ae0:	27bdffe8 	addiu	$sp,$sp,-24
    5ae4:	afbf0010 	sw	$ra,16($sp)
    5ae8:	94850000 	lhu	$a1,0($a0)
    5aec:	3c020000 	lui	$v0,0x0
    5af0:	24420000 	addiu	$v0,$v0,0
    5af4:	0040f809 	jalr	$v0
    5af8:	24040080 	li	$a0,128
    5afc:	00403821 	move	$a3,$v0
    5b00:	30e20030 	andi	$v0,$a3,0x30
    5b04:	1440000c 	bnez	$v0,5b38 <si3210_set_power_alarm_auto+0x3b4>
    5b08:	00073102 	srl	$a2,$a3,0x4
    5b0c:	3c040000 	lui	$a0,0x0
    5b10:	24840060 	addiu	$a0,$a0,96
    5b14:	3c050000 	lui	$a1,0x0
    5b18:	24a516ac 	addiu	$a1,$a1,5804
    5b1c:	30c60003 	andi	$a2,$a2,0x3
    5b20:	3c020000 	lui	$v0,0x0
    5b24:	24420000 	addiu	$v0,$v0,0
    5b28:	0040f809 	jalr	$v0
    5b2c:	30e7000f 	andi	$a3,$a3,0xf
    5b30:	080016d7 	j	5b5c <si3210_set_power_alarm_auto+0x3d8>
    5b34:	8fbf0010 	lw	$ra,16($sp)
    5b38:	3c040000 	lui	$a0,0x0
    5b3c:	24840060 	addiu	$a0,$a0,96
    5b40:	3c050000 	lui	$a1,0x0
    5b44:	24a516cc 	addiu	$a1,$a1,5836
    5b48:	3c020000 	lui	$v0,0x0
    5b4c:	24420000 	addiu	$v0,$v0,0
    5b50:	0040f809 	jalr	$v0
    5b54:	00e03021 	move	$a2,$a3
    5b58:	8fbf0010 	lw	$ra,16($sp)
    5b5c:	3c020000 	lui	$v0,0x0
    5b60:	24420060 	addiu	$v0,$v0,96
    5b64:	03e00008 	jr	$ra
    5b68:	27bd0018 	addiu	$sp,$sp,24

0000000000005b6c <si3210_api_init>:
    5b6c:	27bdffe8 	addiu	$sp,$sp,-24
    5b70:	afbf0010 	sw	$ra,16($sp)
    5b74:	3c030000 	lui	$v1,0x0
    5b78:	8c630470 	lw	$v1,1136($v1)
    5b7c:	24020001 	li	$v0,1
    5b80:	1462000a 	bne	$v1,$v0,5bac <si3210_api_init+0x40>
    5b84:	8fbf0010 	lw	$ra,16($sp)
    5b88:	3c010000 	lui	$at,0x0
    5b8c:	ac200470 	sw	$zero,1136($at)
    5b90:	3c040000 	lui	$a0,0x0
    5b94:	24840474 	addiu	$a0,$a0,1140
    5b98:	3c020000 	lui	$v0,0x0
    5b9c:	24420000 	addiu	$v0,$v0,0
    5ba0:	0040f809 	jalr	$v0
    5ba4:	00000000 	nop
    5ba8:	8fbf0010 	lw	$ra,16($sp)
    5bac:	03e00008 	jr	$ra
    5bb0:	27bd0018 	addiu	$sp,$sp,24

0000000000005bb4 <tiuhw_set_daisy_chain>:
    5bb4:	27bdffd8 	addiu	$sp,$sp,-40
    5bb8:	afbf0024 	sw	$ra,36($sp)
    5bbc:	afb20020 	sw	$s2,32($sp)
    5bc0:	afb1001c 	sw	$s1,28($sp)
    5bc4:	afb00018 	sw	$s0,24($sp)
    5bc8:	a3a50010 	sb	$a1,16($sp)
    5bcc:	30d0ffff 	andi	$s0,$a2,0xffff
    5bd0:	001010c0 	sll	$v0,$s0,0x3
    5bd4:	00501021 	addu	$v0,$v0,$s0
    5bd8:	00021080 	sll	$v0,$v0,0x2
    5bdc:	00501023 	subu	$v0,$v0,$s0
    5be0:	3c030000 	lui	$v1,0x0
    5be4:	8c630004 	lw	$v1,4($v1)
    5be8:	00029080 	sll	$s2,$v0,0x2
    5bec:	02431821 	addu	$v1,$s2,$v1
    5bf0:	8c620040 	lw	$v0,64($v1)
    5bf4:	1440000f 	bnez	$v0,5c34 <tiuhw_set_daisy_chain+0x80>
    5bf8:	309100ff 	andi	$s1,$a0,0xff
    5bfc:	3c040000 	lui	$a0,0x0
    5c00:	248416e0 	addiu	$a0,$a0,5856
    5c04:	3c020000 	lui	$v0,0x0
    5c08:	24420000 	addiu	$v0,$v0,0
    5c0c:	0040f809 	jalr	$v0
    5c10:	02002821 	move	$a1,$s0
    5c14:	02002021 	move	$a0,$s0
    5c18:	02202821 	move	$a1,$s1
    5c1c:	3c020000 	lui	$v0,0x0
    5c20:	8c420000 	lw	$v0,0($v0)
    5c24:	27a60010 	addiu	$a2,$sp,16
    5c28:	8c420008 	lw	$v0,8($v0)
    5c2c:	0040f809 	jalr	$v0
    5c30:	24070001 	li	$a3,1
    5c34:	24020003 	li	$v0,3
    5c38:	a3a20011 	sb	$v0,17($sp)
    5c3c:	02002021 	move	$a0,$s0
    5c40:	24050002 	li	$a1,2
    5c44:	3c020000 	lui	$v0,0x0
    5c48:	8c420000 	lw	$v0,0($v0)
    5c4c:	27a60011 	addiu	$a2,$sp,17
    5c50:	8c42002c 	lw	$v0,44($v0)
    5c54:	0040f809 	jalr	$v0
    5c58:	00003821 	move	$a3,$zero
    5c5c:	24030001 	li	$v1,1
    5c60:	3c020000 	lui	$v0,0x0
    5c64:	8c420004 	lw	$v0,4($v0)
    5c68:	8fbf0024 	lw	$ra,36($sp)
    5c6c:	8fb1001c 	lw	$s1,28($sp)
    5c70:	8fb00018 	lw	$s0,24($sp)
    5c74:	02421021 	addu	$v0,$s2,$v0
    5c78:	8fb20020 	lw	$s2,32($sp)
    5c7c:	ac430040 	sw	$v1,64($v0)
    5c80:	03e00008 	jr	$ra
    5c84:	27bd0028 	addiu	$sp,$sp,40
	...

0000000000005c90 <VP880SetTimeSlot>:
    5c90:	27bdffd8 	addiu	$sp,$sp,-40
    5c94:	afbf0020 	sw	$ra,32($sp)
    5c98:	afb1001c 	sw	$s1,28($sp)
    5c9c:	afb00018 	sw	$s0,24($sp)
    5ca0:	a3a50010 	sb	$a1,16($sp)
    5ca4:	a3a60011 	sb	$a2,17($sp)
    5ca8:	00801021 	move	$v0,$a0
    5cac:	8c43000c 	lw	$v1,12($v0)
    5cb0:	3c020000 	lui	$v0,0x0
    5cb4:	2442605c 	addiu	$v0,$v0,24668
    5cb8:	8c70000c 	lw	$s0,12($v1)
    5cbc:	0040f809 	jalr	$v0
    5cc0:	90710008 	lbu	$s1,8($v1)
    5cc4:	9603002e 	lhu	$v1,46($s0)
    5cc8:	93a20010 	lbu	$v0,16($sp)
    5ccc:	00031982 	srl	$v1,$v1,0x6
    5cd0:	0043102b 	sltu	$v0,$v0,$v1
    5cd4:	10400011 	beqz	$v0,5d1c <VP880SetTimeSlot+0x8c>
    5cd8:	93a20011 	lbu	$v0,17($sp)
    5cdc:	0043102b 	sltu	$v0,$v0,$v1
    5ce0:	1040000e 	beqz	$v0,5d1c <VP880SetTimeSlot+0x8c>
    5ce4:	02202021 	move	$a0,$s1
    5ce8:	24050040 	li	$a1,64
    5cec:	24060001 	li	$a2,1
    5cf0:	3c100000 	lui	$s0,0x0
    5cf4:	26100000 	addiu	$s0,$s0,0
    5cf8:	0200f809 	jalr	$s0
    5cfc:	27a70010 	addiu	$a3,$sp,16
    5d00:	02202021 	move	$a0,$s1
    5d04:	24050042 	li	$a1,66
    5d08:	24060001 	li	$a2,1
    5d0c:	0200f809 	jalr	$s0
    5d10:	27a70011 	addiu	$a3,$sp,17
    5d14:	08001748 	j	5d20 <VP880SetTimeSlot+0x90>
    5d18:	00001021 	move	$v0,$zero
    5d1c:	24020001 	li	$v0,1
    5d20:	8fbf0020 	lw	$ra,32($sp)
    5d24:	8fb1001c 	lw	$s1,28($sp)
    5d28:	8fb00018 	lw	$s0,24($sp)
    5d2c:	03e00008 	jr	$ra
    5d30:	27bd0028 	addiu	$sp,$sp,40

0000000000005d34 <VP880SetCodec>:
    5d34:	27bdffd0 	addiu	$sp,$sp,-48
    5d38:	afbf0028 	sw	$ra,40($sp)
    5d3c:	afb30024 	sw	$s3,36($sp)
    5d40:	afb20020 	sw	$s2,32($sp)
    5d44:	afb1001c 	sw	$s1,28($sp)
    5d48:	afb00018 	sw	$s0,24($sp)
    5d4c:	00801821 	move	$v1,$a0
    5d50:	00a08021 	move	$s0,$a1
    5d54:	3c020000 	lui	$v0,0x0
    5d58:	2442605c 	addiu	$v0,$v0,24668
    5d5c:	8c65000c 	lw	$a1,12($v1)
    5d60:	8c730008 	lw	$s3,8($v1)
    5d64:	0040f809 	jalr	$v0
    5d68:	90b10008 	lbu	$s1,8($a1)
    5d6c:	24050061 	li	$a1,97
    5d70:	24060001 	li	$a2,1
    5d74:	27a70010 	addiu	$a3,$sp,16
    5d78:	3c120000 	lui	$s2,0x0
    5d7c:	26520000 	addiu	$s2,$s2,0
    5d80:	0240f809 	jalr	$s2
    5d84:	02202021 	move	$a0,$s1
    5d88:	93a20010 	lbu	$v0,16($sp)
    5d8c:	3043003f 	andi	$v1,$v0,0x3f
    5d90:	24020001 	li	$v0,1
    5d94:	12020008 	beq	$s0,$v0,5db8 <VP880SetCodec+0x84>
    5d98:	a3a30010 	sb	$v1,16($sp)
    5d9c:	12000008 	beqz	$s0,5dc0 <VP880SetCodec+0x8c>
    5da0:	24020002 	li	$v0,2
    5da4:	1602000d 	bne	$s0,$v0,5ddc <VP880SetCodec+0xa8>
    5da8:	24020001 	li	$v0,1
    5dac:	34620080 	ori	$v0,$v1,0x80
    5db0:	08001770 	j	5dc0 <VP880SetCodec+0x8c>
    5db4:	a3a20010 	sb	$v0,16($sp)
    5db8:	34620040 	ori	$v0,$v1,0x40
    5dbc:	a3a20010 	sb	$v0,16($sp)
    5dc0:	02202021 	move	$a0,$s1
    5dc4:	24050060 	li	$a1,96
    5dc8:	24060001 	li	$a2,1
    5dcc:	0240f809 	jalr	$s2
    5dd0:	27a70010 	addiu	$a3,$sp,16
    5dd4:	ae700000 	sw	$s0,0($s3)
    5dd8:	00001021 	move	$v0,$zero
    5ddc:	8fbf0028 	lw	$ra,40($sp)
    5de0:	8fb30024 	lw	$s3,36($sp)
    5de4:	8fb20020 	lw	$s2,32($sp)
    5de8:	8fb1001c 	lw	$s1,28($sp)
    5dec:	8fb00018 	lw	$s0,24($sp)
    5df0:	03e00008 	jr	$ra
    5df4:	27bd0030 	addiu	$sp,$sp,48

0000000000005df8 <VP880SetLineState>:
    5df8:	27bdffc8 	addiu	$sp,$sp,-56
    5dfc:	afbf0034 	sw	$ra,52($sp)
    5e00:	afb40030 	sw	$s4,48($sp)
    5e04:	afb3002c 	sw	$s3,44($sp)
    5e08:	afb20028 	sw	$s2,40($sp)
    5e0c:	afb10024 	sw	$s1,36($sp)
    5e10:	afb00020 	sw	$s0,32($sp)
    5e14:	3c020000 	lui	$v0,0x0
    5e18:	804217d8 	lb	$v0,6104($v0)
    5e1c:	3c030000 	lui	$v1,0x0
    5e20:	806317d9 	lb	$v1,6105($v1)
    5e24:	a3a20010 	sb	$v0,16($sp)
    5e28:	a3a30011 	sb	$v1,17($sp)
    5e2c:	00801821 	move	$v1,$a0
    5e30:	00a09821 	move	$s3,$a1
    5e34:	3c020000 	lui	$v0,0x0
    5e38:	2442605c 	addiu	$v0,$v0,24668
    5e3c:	8c65000c 	lw	$a1,12($v1)
    5e40:	8c700008 	lw	$s0,8($v1)
    5e44:	90b20008 	lbu	$s2,8($a1)
    5e48:	0040f809 	jalr	$v0
    5e4c:	8e140014 	lw	$s4,20($s0)
    5e50:	1440007b 	bnez	$v0,6040 <VP880SetLineState+0x248>
    5e54:	8fbf0034 	lw	$ra,52($sp)
    5e58:	8e030070 	lw	$v1,112($s0)
    5e5c:	24020001 	li	$v0,1
    5e60:	1462002f 	bne	$v1,$v0,5f20 <VP880SetLineState+0x128>
    5e64:	2e620008 	sltiu	$v0,$s3,8
    5e68:	8e02000c 	lw	$v0,12($s0)
    5e6c:	38420001 	xori	$v0,$v0,0x1
    5e70:	0002102b 	sltu	$v0,$zero,$v0
    5e74:	00028880 	sll	$s1,$v0,0x2
    5e78:	24020009 	li	$v0,9
    5e7c:	1262000c 	beq	$s3,$v0,5eb0 <VP880SetLineState+0xb8>
    5e80:	2e62000a 	sltiu	$v0,$s3,10
    5e84:	10400005 	beqz	$v0,5e9c <VP880SetLineState+0xa4>
    5e88:	24020003 	li	$v0,3
    5e8c:	12620008 	beq	$s3,$v0,5eb0 <VP880SetLineState+0xb8>
    5e90:	24020007 	li	$v0,7
    5e94:	08001811 	j	6044 <VP880SetLineState+0x24c>
    5e98:	8fb40030 	lw	$s4,48($sp)
    5e9c:	2402000a 	li	$v0,10
    5ea0:	12620005 	beq	$s3,$v0,5eb8 <VP880SetLineState+0xc0>
    5ea4:	24020007 	li	$v0,7
    5ea8:	08001810 	j	6040 <VP880SetLineState+0x248>
    5eac:	8fbf0034 	lw	$ra,52($sp)
    5eb0:	080017b5 	j	5ed4 <VP880SetLineState+0xdc>
    5eb4:	36220003 	ori	$v0,$s1,0x3
    5eb8:	3c040000 	lui	$a0,0x0
    5ebc:	248417dc 	addiu	$a0,$a0,6108
    5ec0:	3c020000 	lui	$v0,0x0
    5ec4:	24420000 	addiu	$v0,$v0,0
    5ec8:	0040f809 	jalr	$v0
    5ecc:	00000000 	nop
    5ed0:	36220002 	ori	$v0,$s1,0x2
    5ed4:	a3a20018 	sb	$v0,24($sp)
    5ed8:	02402021 	move	$a0,$s2
    5edc:	24050052 	li	$a1,82
    5ee0:	24060001 	li	$a2,1
    5ee4:	3c020000 	lui	$v0,0x0
    5ee8:	24420000 	addiu	$v0,$v0,0
    5eec:	0040f809 	jalr	$v0
    5ef0:	27a70018 	addiu	$a3,$sp,24
    5ef4:	24020029 	li	$v0,41
    5ef8:	a3a20018 	sb	$v0,24($sp)
    5efc:	02402021 	move	$a0,$s2
    5f00:	24050056 	li	$a1,86
    5f04:	24060001 	li	$a2,1
    5f08:	3c020000 	lui	$v0,0x0
    5f0c:	24420000 	addiu	$v0,$v0,0
    5f10:	0040f809 	jalr	$v0
    5f14:	27a70018 	addiu	$a3,$sp,24
    5f18:	0800180d 	j	6034 <VP880SetLineState+0x23c>
    5f1c:	ae140018 	sw	$s4,24($s0)
    5f20:	10400026 	beqz	$v0,5fbc <VP880SetLineState+0x1c4>
    5f24:	00131080 	sll	$v0,$s3,0x2
    5f28:	3c010000 	lui	$at,0x0
    5f2c:	00220821 	addu	$at,$at,$v0
    5f30:	8c221838 	lw	$v0,6200($at)
    5f34:	00400008 	jr	$v0
    5f38:	00000000 	nop
    5f3c:	24020001 	li	$v0,1
    5f40:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5f44:	a3a20018 	sb	$v0,24($sp)
    5f48:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5f4c:	a3a00018 	sb	$zero,24($sp)
    5f50:	24020023 	li	$v0,35
    5f54:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5f58:	a3a20018 	sb	$v0,24($sp)
    5f5c:	2402002b 	li	$v0,43
    5f60:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5f64:	a3a20018 	sb	$v0,24($sp)
    5f68:	92020033 	lbu	$v0,51($s0)
    5f6c:	1040000a 	beqz	$v0,5f98 <VP880SetLineState+0x1a0>
    5f70:	2402002b 	li	$v0,43
    5f74:	a3a20018 	sb	$v0,24($sp)
    5f78:	3c040000 	lui	$a0,0x0
    5f7c:	248417fc 	addiu	$a0,$a0,6140
    5f80:	3c020000 	lui	$v0,0x0
    5f84:	24420000 	addiu	$v0,$v0,0
    5f88:	0040f809 	jalr	$v0
    5f8c:	02402821 	move	$a1,$s2
    5f90:	080017f2 	j	5fc8 <VP880SetLineState+0x1d0>
    5f94:	8203007f 	lb	$v1,127($s0)
    5f98:	24020027 	li	$v0,39
    5f9c:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5fa0:	a3a20018 	sb	$v0,24($sp)
    5fa4:	2402003b 	li	$v0,59
    5fa8:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5fac:	a3a20018 	sb	$v0,24($sp)
    5fb0:	24020033 	li	$v0,51
    5fb4:	080017f1 	j	5fc4 <VP880SetLineState+0x1cc>
    5fb8:	a3a20018 	sb	$v0,24($sp)
    5fbc:	0800180f 	j	603c <VP880SetLineState+0x244>
    5fc0:	24020008 	li	$v0,8
    5fc4:	8203007f 	lb	$v1,127($s0)
    5fc8:	2402ffff 	li	$v0,-1
    5fcc:	10620011 	beq	$v1,$v0,6014 <VP880SetLineState+0x21c>
    5fd0:	93a30018 	lbu	$v1,24($sp)
    5fd4:	2402002b 	li	$v0,43
    5fd8:	10620016 	beq	$v1,$v0,6034 <VP880SetLineState+0x23c>
    5fdc:	2862002c 	slti	$v0,$v1,44
    5fe0:	14400002 	bnez	$v0,5fec <VP880SetLineState+0x1f4>
    5fe4:	24020023 	li	$v0,35
    5fe8:	24020033 	li	$v0,51
    5fec:	10620011 	beq	$v1,$v0,6034 <VP880SetLineState+0x23c>
    5ff0:	2402ffff 	li	$v0,-1
    5ff4:	a202007f 	sb	$v0,127($s0)
    5ff8:	02402021 	move	$a0,$s2
    5ffc:	240500fc 	li	$a1,252
    6000:	24060002 	li	$a2,2
    6004:	3c020000 	lui	$v0,0x0
    6008:	24420000 	addiu	$v0,$v0,0
    600c:	0040f809 	jalr	$v0
    6010:	27a70010 	addiu	$a3,$sp,16
    6014:	02402021 	move	$a0,$s2
    6018:	24050056 	li	$a1,86
    601c:	24060001 	li	$a2,1
    6020:	3c020000 	lui	$v0,0x0
    6024:	24420000 	addiu	$v0,$v0,0
    6028:	0040f809 	jalr	$v0
    602c:	27a70018 	addiu	$a3,$sp,24
    6030:	ae140018 	sw	$s4,24($s0)
    6034:	ae130014 	sw	$s3,20($s0)
    6038:	00001021 	move	$v0,$zero
    603c:	8fbf0034 	lw	$ra,52($sp)
    6040:	8fb40030 	lw	$s4,48($sp)
    6044:	8fb3002c 	lw	$s3,44($sp)
    6048:	8fb20028 	lw	$s2,40($sp)
    604c:	8fb10024 	lw	$s1,36($sp)
    6050:	8fb00020 	lw	$s0,32($sp)
    6054:	03e00008 	jr	$ra
    6058:	27bd0038 	addiu	$sp,$sp,56

000000000000605c <VP880SetChannel>:
    605c:	27bdffe0 	addiu	$sp,$sp,-32
    6060:	afbf001c 	sw	$ra,28($sp)
    6064:	afb00018 	sw	$s0,24($sp)
    6068:	8c82000c 	lw	$v0,12($a0)
    606c:	90830000 	lbu	$v1,0($a0)
    6070:	8c50000c 	lw	$s0,12($v0)
    6074:	10600006 	beqz	$v1,6090 <VP880SetChannel+0x34>
    6078:	90440008 	lbu	$a0,8($v0)
    607c:	24020001 	li	$v0,1
    6080:	50620006 	0x50620006
    6084:	24020002 	li	$v0,2
    6088:	08001832 	j	60c8 <VP880SetChannel+0x6c>
    608c:	8fbf001c 	lw	$ra,28($sp)
    6090:	24020001 	li	$v0,1
    6094:	08001828 	j	60a0 <VP880SetChannel+0x44>
    6098:	a3a20010 	sb	$v0,16($sp)
    609c:	a3a20010 	sb	$v0,16($sp)
    60a0:	2405004a 	li	$a1,74
    60a4:	24060001 	li	$a2,1
    60a8:	3c020000 	lui	$v0,0x0
    60ac:	24420000 	addiu	$v0,$v0,0
    60b0:	0040f809 	jalr	$v0
    60b4:	27a70010 	addiu	$a3,$sp,16
    60b8:	93a30010 	lbu	$v1,16($sp)
    60bc:	00001021 	move	$v0,$zero
    60c0:	a2030022 	sb	$v1,34($s0)
    60c4:	8fbf001c 	lw	$ra,28($sp)
    60c8:	8fb00018 	lw	$s0,24($sp)
    60cc:	03e00008 	jr	$ra
    60d0:	27bd0020 	addiu	$sp,$sp,32

00000000000060d4 <VP880ApiTick>:
    60d4:	27bdffc0 	addiu	$sp,$sp,-64
    60d8:	afbf0038 	sw	$ra,56($sp)
    60dc:	afb50034 	sw	$s5,52($sp)
    60e0:	afb40030 	sw	$s4,48($sp)
    60e4:	afb3002c 	sw	$s3,44($sp)
    60e8:	afb20028 	sw	$s2,40($sp)
    60ec:	afb10024 	sw	$s1,36($sp)
    60f0:	afb00020 	sw	$s0,32($sp)
    60f4:	8c93000c 	lw	$s3,12($a0)
    60f8:	27b50010 	addiu	$s5,$sp,16
    60fc:	8e74000c 	lw	$s4,12($s3)
    6100:	92720008 	lbu	$s2,8($s3)
    6104:	3c020000 	lui	$v0,0x0
    6108:	804217d8 	lb	$v0,6104($v0)
    610c:	3c030000 	lui	$v1,0x0
    6110:	806317d9 	lb	$v1,6105($v1)
    6114:	a3a20010 	sb	$v0,16($sp)
    6118:	a3a30011 	sb	$v1,17($sp)
    611c:	8e820018 	lw	$v0,24($s4)
    6120:	14400003 	bnez	$v0,6130 <VP880ApiTick+0x5c>
    6124:	8c910008 	lw	$s1,8($a0)
    6128:	08001877 	j	61dc <VP880ApiTick+0x108>
    612c:	24020003 	li	$v0,3
    6130:	8223007f 	lb	$v1,127($s1)
    6134:	2402ffff 	li	$v0,-1
    6138:	5062001d 	0x5062001d
    613c:	02402021 	move	$a0,$s2
    6140:	14600007 	bnez	$v1,6160 <VP880ApiTick+0x8c>
    6144:	24020009 	li	$v0,9
    6148:	3c020000 	lui	$v0,0x0
    614c:	24425df8 	addiu	$v0,$v0,24056
    6150:	0040f809 	jalr	$v0
    6154:	8e250014 	lw	$a1,20($s1)
    6158:	8223007f 	lb	$v1,127($s1)
    615c:	24020009 	li	$v0,9
    6160:	54620010 	0x54620010
    6164:	9222007f 	lbu	$v0,127($s1)
    6168:	24020024 	li	$v0,36
    616c:	a3a20018 	sb	$v0,24($sp)
    6170:	02402021 	move	$a0,$s2
    6174:	24050056 	li	$a1,86
    6178:	24060001 	li	$a2,1
    617c:	3c100000 	lui	$s0,0x0
    6180:	26100000 	addiu	$s0,$s0,0
    6184:	0200f809 	jalr	$s0
    6188:	27a70018 	addiu	$a3,$sp,24
    618c:	02402021 	move	$a0,$s2
    6190:	240500fc 	li	$a1,252
    6194:	24060002 	li	$a2,2
    6198:	0200f809 	jalr	$s0
    619c:	02a03821 	move	$a3,$s5
    61a0:	9222007f 	lbu	$v0,127($s1)
    61a4:	2442ffff 	addiu	$v0,$v0,-1
    61a8:	a222007f 	sb	$v0,127($s1)
    61ac:	02402021 	move	$a0,$s2
    61b0:	2405004f 	li	$a1,79
    61b4:	24060002 	li	$a2,2
    61b8:	3c020000 	lui	$v0,0x0
    61bc:	24420000 	addiu	$v0,$v0,0
    61c0:	0040f809 	jalr	$v0
    61c4:	26870044 	addiu	$a3,$s4,68
    61c8:	3c020000 	lui	$v0,0x0
    61cc:	24426200 	addiu	$v0,$v0,25088
    61d0:	0040f809 	jalr	$v0
    61d4:	02602021 	move	$a0,$s3
    61d8:	00001021 	move	$v0,$zero
    61dc:	8fbf0038 	lw	$ra,56($sp)
    61e0:	8fb50034 	lw	$s5,52($sp)
    61e4:	8fb40030 	lw	$s4,48($sp)
    61e8:	8fb3002c 	lw	$s3,44($sp)
    61ec:	8fb20028 	lw	$s2,40($sp)
    61f0:	8fb10024 	lw	$s1,36($sp)
    61f4:	8fb00020 	lw	$s0,32($sp)
    61f8:	03e00008 	jr	$ra
    61fc:	27bd0040 	addiu	$sp,$sp,64

0000000000006200 <VP880_ServiceInterrupts>:
    6200:	27bdffc0 	addiu	$sp,$sp,-64
    6204:	afbf003c 	sw	$ra,60($sp)
    6208:	afb60038 	sw	$s6,56($sp)
    620c:	afb50034 	sw	$s5,52($sp)
    6210:	afb40030 	sw	$s4,48($sp)
    6214:	afb3002c 	sw	$s3,44($sp)
    6218:	afb20028 	sw	$s2,40($sp)
    621c:	afb10024 	sw	$s1,36($sp)
    6220:	afb00020 	sw	$s0,32($sp)
    6224:	3c020000 	lui	$v0,0x0
    6228:	80421858 	lb	$v0,6232($v0)
    622c:	3c030000 	lui	$v1,0x0
    6230:	80631859 	lb	$v1,6233($v1)
    6234:	a3a20010 	sb	$v0,16($sp)
    6238:	a3a30011 	sb	$v1,17($sp)
    623c:	0000a821 	move	$s5,$zero
    6240:	8c93000c 	lw	$s3,12($a0)
    6244:	00009021 	move	$s2,$zero
    6248:	92620009 	lbu	$v0,9($s3)
    624c:	1040006e 	beqz	$v0,6408 <VP880_ServiceInterrupts+0x208>
    6250:	90960008 	lbu	$s6,8($a0)
    6254:	00151080 	sll	$v0,$s5,0x2
    6258:	02621021 	addu	$v0,$s3,$v0
    625c:	8c44003c 	lw	$a0,60($v0)
    6260:	1080005d 	beqz	$a0,63d8 <VP880_ServiceInterrupts+0x1d8>
    6264:	00000000 	nop
    6268:	8c900008 	lw	$s0,8($a0)
    626c:	8e020070 	lw	$v0,112($s0)
    6270:	1440002a 	bnez	$v0,631c <VP880_ServiceInterrupts+0x11c>
    6274:	02751021 	addu	$v0,$s3,$s5
    6278:	90420044 	lbu	$v0,68($v0)
    627c:	92030033 	lbu	$v1,51($s0)
    6280:	30450001 	andi	$a1,$v0,0x1
    6284:	10a3001f 	beq	$a1,$v1,6304 <VP880_ServiceInterrupts+0x104>
    6288:	30540002 	andi	$s4,$v0,0x2
    628c:	14a0001a 	bnez	$a1,62f8 <VP880_ServiceInterrupts+0xf8>
    6290:	a2050033 	sb	$a1,51($s0)
    6294:	3c020000 	lui	$v0,0x0
    6298:	2442605c 	addiu	$v0,$v0,24668
    629c:	0040f809 	jalr	$v0
    62a0:	00000000 	nop
    62a4:	02c02021 	move	$a0,$s6
    62a8:	24050057 	li	$a1,87
    62ac:	24060001 	li	$a2,1
    62b0:	3c110000 	lui	$s1,0x0
    62b4:	26310000 	addiu	$s1,$s1,0
    62b8:	0220f809 	jalr	$s1
    62bc:	27a70018 	addiu	$a3,$sp,24
    62c0:	93a20018 	lbu	$v0,24($sp)
    62c4:	3043000f 	andi	$v1,$v0,0xf
    62c8:	28620002 	slti	$v0,$v1,2
    62cc:	10400004 	beqz	$v0,62e0 <VP880_ServiceInterrupts+0xe0>
    62d0:	02c02021 	move	$a0,$s6
    62d4:	0461000b 	bgez	$v1,6304 <VP880_ServiceInterrupts+0x104>
    62d8:	24120001 	li	$s2,1
    62dc:	02c02021 	move	$a0,$s6
    62e0:	240500fc 	li	$a1,252
    62e4:	24060002 	li	$a2,2
    62e8:	0220f809 	jalr	$s1
    62ec:	27a70010 	addiu	$a3,$sp,16
    62f0:	080018bf 	j	62fc <VP880_ServiceInterrupts+0xfc>
    62f4:	2402000a 	li	$v0,10
    62f8:	2402ffff 	li	$v0,-1
    62fc:	a202007f 	sb	$v0,127($s0)
    6300:	24120001 	li	$s2,1
    6304:	92020034 	lbu	$v0,52($s0)
    6308:	1282003a 	beq	$s4,$v0,63f4 <VP880_ServiceInterrupts+0x1f4>
    630c:	26a30001 	addiu	$v1,$s5,1
    6310:	a2140034 	sb	$s4,52($s0)
    6314:	080018fd 	j	63f4 <VP880_ServiceInterrupts+0x1f4>
    6318:	24120001 	li	$s2,1
    631c:	90450044 	lbu	$a1,68($v0)
    6320:	24020001 	li	$v0,1
    6324:	8e03000c 	lw	$v1,12($s0)
    6328:	14620003 	bne	$v1,$v0,6338 <VP880_ServiceInterrupts+0x138>
    632c:	30b10002 	andi	$s1,$a1,0x2
    6330:	080018cf 	j	633c <VP880_ServiceInterrupts+0x13c>
    6334:	30a50004 	andi	$a1,$a1,0x4
    6338:	30a50001 	andi	$a1,$a1,0x1
    633c:	92020064 	lbu	$v0,100($s0)
    6340:	50a20015 	0x50a20015
    6344:	92020065 	lbu	$v0,101($s0)
    6348:	10a00009 	beqz	$a1,6370 <VP880_ServiceInterrupts+0x170>
    634c:	a2050064 	sb	$a1,100($s0)
    6350:	3c040000 	lui	$a0,0x0
    6354:	2484185c 	addiu	$a0,$a0,6236
    6358:	3c020000 	lui	$v0,0x0
    635c:	24420000 	addiu	$v0,$v0,0
    6360:	0040f809 	jalr	$v0
    6364:	00000000 	nop
    6368:	080018e3 	j	638c <VP880_ServiceInterrupts+0x18c>
    636c:	24020001 	li	$v0,1
    6370:	3c040000 	lui	$a0,0x0
    6374:	24841878 	addiu	$a0,$a0,6264
    6378:	3c020000 	lui	$v0,0x0
    637c:	24420000 	addiu	$v0,$v0,0
    6380:	0040f809 	jalr	$v0
    6384:	00002821 	move	$a1,$zero
    6388:	24020005 	li	$v0,5
    638c:	ae020060 	sw	$v0,96($s0)
    6390:	24120001 	li	$s2,1
    6394:	92020065 	lbu	$v0,101($s0)
    6398:	12220016 	beq	$s1,$v0,63f4 <VP880_ServiceInterrupts+0x1f4>
    639c:	26a30001 	addiu	$v1,$s5,1
    63a0:	12200009 	beqz	$s1,63c8 <VP880_ServiceInterrupts+0x1c8>
    63a4:	a2110065 	sb	$s1,101($s0)
    63a8:	3c040000 	lui	$a0,0x0
    63ac:	24841894 	addiu	$a0,$a0,6292
    63b0:	3c020000 	lui	$v0,0x0
    63b4:	24420000 	addiu	$v0,$v0,0
    63b8:	0040f809 	jalr	$v0
    63bc:	00000000 	nop
    63c0:	080018f3 	j	63cc <VP880_ServiceInterrupts+0x1cc>
    63c4:	24020003 	li	$v0,3
    63c8:	24020004 	li	$v0,4
    63cc:	ae020060 	sw	$v0,96($s0)
    63d0:	080018fc 	j	63f0 <VP880_ServiceInterrupts+0x1f0>
    63d4:	24120001 	li	$s2,1
    63d8:	3c040000 	lui	$a0,0x0
    63dc:	248418a4 	addiu	$a0,$a0,6308
    63e0:	3c020000 	lui	$v0,0x0
    63e4:	24420000 	addiu	$v0,$v0,0
    63e8:	0040f809 	jalr	$v0
    63ec:	00000000 	nop
    63f0:	26a30001 	addiu	$v1,$s5,1
    63f4:	92620009 	lbu	$v0,9($s3)
    63f8:	307500ff 	andi	$s5,$v1,0xff
    63fc:	02a2102b 	sltu	$v0,$s5,$v0
    6400:	1440ff95 	bnez	$v0,6258 <VP880_ServiceInterrupts+0x58>
    6404:	00151080 	sll	$v0,$s5,0x2
    6408:	02401021 	move	$v0,$s2
    640c:	8fbf003c 	lw	$ra,60($sp)
    6410:	8fb60038 	lw	$s6,56($sp)
    6414:	8fb50034 	lw	$s5,52($sp)
    6418:	8fb40030 	lw	$s4,48($sp)
    641c:	8fb3002c 	lw	$s3,44($sp)
    6420:	8fb20028 	lw	$s2,40($sp)
    6424:	8fb10024 	lw	$s1,36($sp)
    6428:	8fb00020 	lw	$s0,32($sp)
    642c:	03e00008 	jr	$ra
    6430:	27bd0040 	addiu	$sp,$sp,64

0000000000006434 <VP880SetRelGain>:
    6434:	27bdffc8 	addiu	$sp,$sp,-56
    6438:	afbf0030 	sw	$ra,48($sp)
    643c:	afb3002c 	sw	$s3,44($sp)
    6440:	afb20028 	sw	$s2,40($sp)
    6444:	afb10024 	sw	$s1,36($sp)
    6448:	afb00020 	sw	$s0,32($sp)
    644c:	00a08021 	move	$s0,$a1
    6450:	00c08821 	move	$s1,$a2
    6454:	00809821 	move	$s3,$a0
    6458:	3c040000 	lui	$a0,0x0
    645c:	248418c4 	addiu	$a0,$a0,6340
    6460:	8e63000c 	lw	$v1,12($s3)
    6464:	3c020000 	lui	$v0,0x0
    6468:	24420000 	addiu	$v0,$v0,0
    646c:	0040f809 	jalr	$v0
    6470:	90720008 	lbu	$s2,8($v1)
    6474:	2a02fffa 	slti	$v0,$s0,-6
    6478:	54400001 	0x54400001
    647c:	2410fffa 	li	$s0,-6
    6480:	2a22fff4 	slti	$v0,$s1,-12
    6484:	54400001 	0x54400001
    6488:	2411fff4 	li	$s1,-12
    648c:	2a020007 	slti	$v0,$s0,7
    6490:	50400001 	0x50400001
    6494:	24100006 	li	$s0,6
    6498:	5e200001 	0x5e200001
    649c:	00008821 	move	$s1,$zero
    64a0:	24020006 	li	$v0,6
    64a4:	00501023 	subu	$v0,$v0,$s0
    64a8:	00028040 	sll	$s0,$v0,0x1
    64ac:	00111023 	negu	$v0,$s1
    64b0:	00028840 	sll	$s1,$v0,0x1
    64b4:	26020001 	addiu	$v0,$s0,1
    64b8:	3c030000 	lui	$v1,0x0
    64bc:	00701821 	addu	$v1,$v1,$s0
    64c0:	906304f0 	lbu	$v1,1264($v1)
    64c4:	3c010000 	lui	$at,0x0
    64c8:	00220821 	addu	$at,$at,$v0
    64cc:	902204f0 	lbu	$v0,1264($at)
    64d0:	a3a30010 	sb	$v1,16($sp)
    64d4:	a3a20011 	sb	$v0,17($sp)
    64d8:	26220001 	addiu	$v0,$s1,1
    64dc:	3c030000 	lui	$v1,0x0
    64e0:	00711821 	addu	$v1,$v1,$s1
    64e4:	9063050c 	lbu	$v1,1292($v1)
    64e8:	3c010000 	lui	$at,0x0
    64ec:	00220821 	addu	$at,$at,$v0
    64f0:	9022050c 	lbu	$v0,1292($at)
    64f4:	a3a30018 	sb	$v1,24($sp)
    64f8:	a3a20019 	sb	$v0,25($sp)
    64fc:	3c020000 	lui	$v0,0x0
    6500:	2442605c 	addiu	$v0,$v0,24668
    6504:	0040f809 	jalr	$v0
    6508:	02602021 	move	$a0,$s3
    650c:	02402021 	move	$a0,$s2
    6510:	24050080 	li	$a1,128
    6514:	24060002 	li	$a2,2
    6518:	3c100000 	lui	$s0,0x0
    651c:	26100000 	addiu	$s0,$s0,0
    6520:	0200f809 	jalr	$s0
    6524:	27a70010 	addiu	$a3,$sp,16
    6528:	02402021 	move	$a0,$s2
    652c:	24050082 	li	$a1,130
    6530:	24060002 	li	$a2,2
    6534:	0200f809 	jalr	$s0
    6538:	27a70018 	addiu	$a3,$sp,24
    653c:	8fbf0030 	lw	$ra,48($sp)
    6540:	8fb3002c 	lw	$s3,44($sp)
    6544:	8fb20028 	lw	$s2,40($sp)
    6548:	8fb10024 	lw	$s1,36($sp)
    654c:	8fb00020 	lw	$s0,32($sp)
    6550:	00001021 	move	$v0,$zero
    6554:	03e00008 	jr	$ra
    6558:	27bd0038 	addiu	$sp,$sp,56
    655c:	00000000 	nop

0000000000006560 <MakeVP880LineObject>:
    6560:	8cc2000c 	lw	$v0,12($a2)
    6564:	30a500ff 	andi	$a1,$a1,0xff
    6568:	8c42000c 	lw	$v0,12($v0)
    656c:	00051880 	sll	$v1,$a1,0x2
    6570:	00431021 	addu	$v0,$v0,$v1
    6574:	8cc30008 	lw	$v1,8($a2)
    6578:	ac46003c 	sw	$a2,60($v0)
    657c:	a0c50000 	sb	$a1,0($a2)
    6580:	2c820002 	sltiu	$v0,$a0,2
    6584:	14400006 	bnez	$v0,65a0 <MakeVP880LineObject+0x40>
    6588:	ac64000c 	sw	$a0,12($v1)
    658c:	24020005 	li	$v0,5
    6590:	10820006 	beq	$a0,$v0,65ac <MakeVP880LineObject+0x4c>
    6594:	24020006 	li	$v0,6
    6598:	0800196d 	j	65b4 <MakeVP880LineObject+0x54>
    659c:	00000000 	nop
    65a0:	24020001 	li	$v0,1
    65a4:	0800196c 	j	65b0 <MakeVP880LineObject+0x50>
    65a8:	ac620070 	sw	$v0,112($v1)
    65ac:	ac600070 	sw	$zero,112($v1)
    65b0:	00001021 	move	$v0,$zero
    65b4:	03e00008 	jr	$ra
    65b8:	00000000 	nop

00000000000065bc <VP880Init>:
    65bc:	27bdffa8 	addiu	$sp,$sp,-88
    65c0:	afbf0054 	sw	$ra,84($sp)
    65c4:	afb60050 	sw	$s6,80($sp)
    65c8:	afb5004c 	sw	$s5,76($sp)
    65cc:	afb40048 	sw	$s4,72($sp)
    65d0:	afb30044 	sw	$s3,68($sp)
    65d4:	afb20040 	sw	$s2,64($sp)
    65d8:	afb1003c 	sw	$s1,60($sp)
    65dc:	afb00038 	sw	$s0,56($sp)
    65e0:	3c020000 	lui	$v0,0x0
    65e4:	804219b8 	lb	$v0,6584($v0)
    65e8:	3c030000 	lui	$v1,0x0
    65ec:	806319b9 	lb	$v1,6585($v1)
    65f0:	a3a20010 	sb	$v0,16($sp)
    65f4:	a3a30011 	sb	$v1,17($sp)
    65f8:	3c020000 	lui	$v0,0x0
    65fc:	804219bc 	lb	$v0,6588($v0)
    6600:	3c030000 	lui	$v1,0x0
    6604:	806319bd 	lb	$v1,6589($v1)
    6608:	a3a20018 	sb	$v0,24($sp)
    660c:	a3a30019 	sb	$v1,25($sp)
    6610:	3c060000 	lui	$a2,0x0
    6614:	24c619c0 	addiu	$a2,$a2,6592
    6618:	88c20003 	lwl	$v0,3($a2)
    661c:	98c20000 	lwr	$v0,0($a2)
    6620:	80c30004 	lb	$v1,4($a2)
    6624:	80c50005 	lb	$a1,5($a2)
    6628:	aba20023 	swl	$v0,35($sp)
    662c:	bba20020 	swr	$v0,32($sp)
    6630:	a3a30024 	sb	$v1,36($sp)
    6634:	a3a50025 	sb	$a1,37($sp)
    6638:	3c060000 	lui	$a2,0x0
    663c:	24c619c8 	addiu	$a2,$a2,6600
    6640:	80c20000 	lb	$v0,0($a2)
    6644:	80c30001 	lb	$v1,1($a2)
    6648:	80c50002 	lb	$a1,2($a2)
    664c:	a3a20028 	sb	$v0,40($sp)
    6650:	a3a30029 	sb	$v1,41($sp)
    6654:	a3a5002a 	sb	$a1,42($sp)
    6658:	00801821 	move	$v1,$a0
    665c:	3c050000 	lui	$a1,0x0
    6660:	24a50000 	addiu	$a1,$a1,0
    6664:	27b00010 	addiu	$s0,$sp,16
    6668:	3c020000 	lui	$v0,0x0
    666c:	244268ec 	addiu	$v0,$v0,26860
    6670:	8c72000c 	lw	$s2,12($v1)
    6674:	0040f809 	jalr	$v0
    6678:	90710008 	lbu	$s1,8($v1)
    667c:	24050004 	li	$a1,4
    6680:	00003021 	move	$a2,$zero
    6684:	27b60030 	addiu	$s6,$sp,48
    6688:	02c03821 	move	$a3,$s6
    668c:	3c130000 	lui	$s3,0x0
    6690:	26730000 	addiu	$s3,$s3,0
    6694:	ae400018 	sw	$zero,24($s2)
    6698:	ae40001c 	sw	$zero,28($s2)
    669c:	a2400020 	sb	$zero,32($s2)
    66a0:	a2400022 	sb	$zero,34($s2)
    66a4:	ae400024 	sw	$zero,36($s2)
    66a8:	a2400014 	sb	$zero,20($s2)
    66ac:	0260f809 	jalr	$s3
    66b0:	02202021 	move	$a0,$s1
    66b4:	3c02afff 	lui	$v0,0xafff
    66b8:	34427a68 	ori	$v0,$v0,0x7a68
    66bc:	3c030000 	lui	$v1,0x0
    66c0:	8c630000 	lw	$v1,0($v1)
    66c4:	00430019 	multu	$v0,$v1
    66c8:	00001010 	mfhi	$v0
    66cc:	1440ffff 	bnez	$v0,66cc <VP880Init+0x110>
    66d0:	2442ffff 	addiu	$v0,$v0,-1
    66d4:	02202021 	move	$a0,$s1
    66d8:	24050046 	li	$a1,70
    66dc:	24060001 	li	$a2,1
    66e0:	02c03821 	move	$a3,$s6
    66e4:	92420034 	lbu	$v0,52($s2)
    66e8:	24140009 	li	$s4,9
    66ec:	0260f809 	jalr	$s3
    66f0:	a3a20030 	sb	$v0,48($sp)
    66f4:	02202021 	move	$a0,$s1
    66f8:	2405006c 	li	$a1,108
    66fc:	24060002 	li	$a2,2
    6700:	0260f809 	jalr	$s3
    6704:	02003821 	move	$a3,$s0
    6708:	3c029ffe 	lui	$v0,0x9ffe
    670c:	344242b0 	ori	$v0,$v0,0x42b0
    6710:	3c030000 	lui	$v1,0x0
    6714:	8c630000 	lw	$v1,0($v1)
    6718:	00430019 	multu	$v0,$v1
    671c:	00001010 	mfhi	$v0
    6720:	1440ffff 	bnez	$v0,6720 <VP880Init+0x164>
    6724:	2442ffff 	addiu	$v0,$v0,-1
    6728:	02202021 	move	$a0,$s1
    672c:	2405004f 	li	$a1,79
    6730:	24060002 	li	$a2,2
    6734:	0260f809 	jalr	$s3
    6738:	26470044 	addiu	$a3,$s2,68
    673c:	92420044 	lbu	$v0,68($s2)
    6740:	30420080 	andi	$v0,$v0,0x80
    6744:	10400006 	beqz	$v0,6760 <VP880Init+0x1a4>
    6748:	2682ffff 	addiu	$v0,$s4,-1
    674c:	305400ff 	andi	$s4,$v0,0xff
    6750:	1680ffee 	bnez	$s4,670c <VP880Init+0x150>
    6754:	3c029ffe 	lui	$v0,0x9ffe
    6758:	08001a31 	j	68c4 <VP880Init+0x308>
    675c:	24020005 	li	$v0,5
    6760:	240200ff 	li	$v0,255
    6764:	a3a20010 	sb	$v0,16($sp)
    6768:	02202021 	move	$a0,$s1
    676c:	2405006c 	li	$a1,108
    6770:	24060002 	li	$a2,2
    6774:	0260f809 	jalr	$s3
    6778:	27a70010 	addiu	$a3,$sp,16
    677c:	02202021 	move	$a0,$s1
    6780:	24050044 	li	$a1,68
    6784:	24060001 	li	$a2,1
    6788:	02c03821 	move	$a3,$s6
    678c:	92420035 	lbu	$v0,53($s2)
    6790:	00008021 	move	$s0,$zero
    6794:	0260f809 	jalr	$s3
    6798:	a3a20030 	sb	$v0,48($sp)
    679c:	92550009 	lbu	$s5,9($s2)
    67a0:	12a00011 	beqz	$s5,67e8 <VP880Init+0x22c>
    67a4:	a3a00030 	sb	$zero,48($sp)
    67a8:	02202021 	move	$a0,$s1
    67ac:	2405004a 	li	$a1,74
    67b0:	24060001 	li	$a2,1
    67b4:	27a70018 	addiu	$a3,$sp,24
    67b8:	0260f809 	jalr	$s3
    67bc:	00f03821 	addu	$a3,$a3,$s0
    67c0:	02202021 	move	$a0,$s1
    67c4:	24050056 	li	$a1,86
    67c8:	24060001 	li	$a2,1
    67cc:	0260f809 	jalr	$s3
    67d0:	02c03821 	move	$a3,$s6
    67d4:	26020001 	addiu	$v0,$s0,1
    67d8:	305000ff 	andi	$s0,$v0,0xff
    67dc:	0215102b 	sltu	$v0,$s0,$s5
    67e0:	1440fff2 	bnez	$v0,67ac <VP880Init+0x1f0>
    67e4:	02202021 	move	$a0,$s1
    67e8:	a3a00030 	sb	$zero,48($sp)
    67ec:	02202021 	move	$a0,$s1
    67f0:	240500e6 	li	$a1,230
    67f4:	24060001 	li	$a2,1
    67f8:	27b30030 	addiu	$s3,$sp,48
    67fc:	3c140000 	lui	$s4,0x0
    6800:	26940000 	addiu	$s4,$s4,0
    6804:	0280f809 	jalr	$s4
    6808:	02603821 	move	$a3,$s3
    680c:	02202021 	move	$a0,$s1
    6810:	240500e4 	li	$a1,228
    6814:	24060003 	li	$a2,3
    6818:	0280f809 	jalr	$s4
    681c:	27a70028 	addiu	$a3,$sp,40
    6820:	24020007 	li	$v0,7
    6824:	a3a20030 	sb	$v0,48($sp)
    6828:	02202021 	move	$a0,$s1
    682c:	240500e6 	li	$a1,230
    6830:	24060001 	li	$a2,1
    6834:	0280f809 	jalr	$s4
    6838:	02603821 	move	$a3,$s3
    683c:	2402002b 	li	$v0,43
    6840:	a3a20030 	sb	$v0,48($sp)
    6844:	12a00011 	beqz	$s5,688c <VP880Init+0x2d0>
    6848:	00008021 	move	$s0,$zero
    684c:	02202021 	move	$a0,$s1
    6850:	2405004a 	li	$a1,74
    6854:	24060001 	li	$a2,1
    6858:	27a70018 	addiu	$a3,$sp,24
    685c:	0280f809 	jalr	$s4
    6860:	00f03821 	addu	$a3,$a3,$s0
    6864:	02202021 	move	$a0,$s1
    6868:	24050056 	li	$a1,86
    686c:	24060001 	li	$a2,1
    6870:	0280f809 	jalr	$s4
    6874:	02603821 	move	$a3,$s3
    6878:	26020001 	addiu	$v0,$s0,1
    687c:	305000ff 	andi	$s0,$v0,0xff
    6880:	0215102b 	sltu	$v0,$s0,$s5
    6884:	1440fff2 	bnez	$v0,6850 <VP880Init+0x294>
    6888:	02202021 	move	$a0,$s1
    688c:	24020001 	li	$v0,1
    6890:	a3a20030 	sb	$v0,48($sp)
    6894:	02202021 	move	$a0,$s1
    6898:	2405004a 	li	$a1,74
    689c:	24060001 	li	$a2,1
    68a0:	3c020000 	lui	$v0,0x0
    68a4:	24420000 	addiu	$v0,$v0,0
    68a8:	0040f809 	jalr	$v0
    68ac:	27a70030 	addiu	$a3,$sp,48
    68b0:	93a30030 	lbu	$v1,48($sp)
    68b4:	24020001 	li	$v0,1
    68b8:	ae420018 	sw	$v0,24($s2)
    68bc:	00001021 	move	$v0,$zero
    68c0:	a2430022 	sb	$v1,34($s2)
    68c4:	8fbf0054 	lw	$ra,84($sp)
    68c8:	8fb60050 	lw	$s6,80($sp)
    68cc:	8fb5004c 	lw	$s5,76($sp)
    68d0:	8fb40048 	lw	$s4,72($sp)
    68d4:	8fb30044 	lw	$s3,68($sp)
    68d8:	8fb20040 	lw	$s2,64($sp)
    68dc:	8fb1003c 	lw	$s1,60($sp)
    68e0:	8fb00038 	lw	$s0,56($sp)
    68e4:	03e00008 	jr	$ra
    68e8:	27bd0058 	addiu	$sp,$sp,88

00000000000068ec <VP880InitDevice>:
    68ec:	90a30006 	lbu	$v1,6($a1)
    68f0:	90a20007 	lbu	$v0,7($a1)
    68f4:	8c84000c 	lw	$a0,12($a0)
    68f8:	00031a00 	sll	$v1,$v1,0x8
    68fc:	00431025 	or	$v0,$v0,$v1
    6900:	a482002e 	sh	$v0,46($a0)
    6904:	90a20008 	lbu	$v0,8($a1)
    6908:	a0820034 	sb	$v0,52($a0)
    690c:	90a20009 	lbu	$v0,9($a1)
    6910:	a0820031 	sb	$v0,49($a0)
    6914:	90a2000a 	lbu	$v0,10($a1)
    6918:	a0820035 	sb	$v0,53($a0)
    691c:	90a2000b 	lbu	$v0,11($a1)
    6920:	a082002c 	sb	$v0,44($a0)
    6924:	90a2000c 	lbu	$v0,12($a1)
    6928:	90a3000d 	lbu	$v1,13($a1)
    692c:	00021200 	sll	$v0,$v0,0x8
    6930:	00621825 	or	$v1,$v1,$v0
    6934:	a4830032 	sh	$v1,50($a0)
    6938:	03e00008 	jr	$ra
    693c:	00001021 	move	$v0,$zero

0000000000006940 <VP880InitLine>:
    6940:	27bdff60 	addiu	$sp,$sp,-160
    6944:	afbf009c 	sw	$ra,156($sp)
    6948:	afbe0098 	sw	$s8,152($sp)
    694c:	afb70094 	sw	$s7,148($sp)
    6950:	afb60090 	sw	$s6,144($sp)
    6954:	afb5008c 	sw	$s5,140($sp)
    6958:	afb40088 	sw	$s4,136($sp)
    695c:	afb30084 	sw	$s3,132($sp)
    6960:	afb20080 	sw	$s2,128($sp)
    6964:	afb1007c 	sw	$s1,124($sp)
    6968:	afb00078 	sw	$s0,120($sp)
    696c:	0080a821 	move	$s5,$a0
    6970:	00a0b821 	move	$s7,$a1
    6974:	8ea2000c 	lw	$v0,12($s5)
    6978:	00e0b021 	move	$s6,$a3
    697c:	90500008 	lbu	$s0,8($v0)
    6980:	3c030000 	lui	$v1,0x0
    6984:	806319cc 	lb	$v1,6604($v1)
    6988:	3c080000 	lui	$t0,0x0
    698c:	810819cd 	lb	$t0,6605($t0)
    6990:	a3a30010 	sb	$v1,16($sp)
    6994:	a3a80011 	sb	$t0,17($sp)
    6998:	3c0a0000 	lui	$t2,0x0
    699c:	254a19d0 	addiu	$t2,$t2,6608
    69a0:	89430003 	lwl	$v1,3($t2)
    69a4:	99430000 	lwr	$v1,0($t2)
    69a8:	aba3001b 	swl	$v1,27($sp)
    69ac:	bba30018 	swr	$v1,24($sp)
    69b0:	3c030000 	lui	$v1,0x0
    69b4:	806319d4 	lb	$v1,6612($v1)
    69b8:	3c080000 	lui	$t0,0x0
    69bc:	810819d5 	lb	$t0,6613($t0)
    69c0:	a3a30020 	sb	$v1,32($sp)
    69c4:	a3a80021 	sb	$t0,33($sp)
    69c8:	3c0a0000 	lui	$t2,0x0
    69cc:	254a19d8 	addiu	$t2,$t2,6616
    69d0:	89430003 	lwl	$v1,3($t2)
    69d4:	99430000 	lwr	$v1,0($t2)
    69d8:	aba3002b 	swl	$v1,43($sp)
    69dc:	bba30028 	swr	$v1,40($sp)
    69e0:	3c0a0000 	lui	$t2,0x0
    69e4:	254a19dc 	addiu	$t2,$t2,6620
    69e8:	89430003 	lwl	$v1,3($t2)
    69ec:	99430000 	lwr	$v1,0($t2)
    69f0:	89480007 	lwl	$t0,7($t2)
    69f4:	99480004 	lwr	$t0,4($t2)
    69f8:	81490008 	lb	$t1,8($t2)
    69fc:	aba30033 	swl	$v1,51($sp)
    6a00:	bba30030 	swr	$v1,48($sp)
    6a04:	aba80037 	swl	$t0,55($sp)
    6a08:	bba80034 	swr	$t0,52($sp)
    6a0c:	a3a90038 	sb	$t1,56($sp)
    6a10:	81430009 	lb	$v1,9($t2)
    6a14:	8148000a 	lb	$t0,10($t2)
    6a18:	a3a30039 	sb	$v1,57($sp)
    6a1c:	a3a8003a 	sb	$t0,58($sp)
    6a20:	3c0a0000 	lui	$t2,0x0
    6a24:	254a19e8 	addiu	$t2,$t2,6632
    6a28:	89430003 	lwl	$v1,3($t2)
    6a2c:	99430000 	lwr	$v1,0($t2)
    6a30:	aba30043 	swl	$v1,67($sp)
    6a34:	bba30040 	swr	$v1,64($sp)
    6a38:	3c030000 	lui	$v1,0x0
    6a3c:	806319ec 	lb	$v1,6636($v1)
    6a40:	3c080000 	lui	$t0,0x0
    6a44:	810819ed 	lb	$t0,6637($t0)
    6a48:	a3a30048 	sb	$v1,72($sp)
    6a4c:	a3a80049 	sb	$t0,73($sp)
    6a50:	3c030000 	lui	$v1,0x0
    6a54:	806319f0 	lb	$v1,6640($v1)
    6a58:	3c080000 	lui	$t0,0x0
    6a5c:	810819f1 	lb	$t0,6641($t0)
    6a60:	a3a30050 	sb	$v1,80($sp)
    6a64:	a3a80051 	sb	$t0,81($sp)
    6a68:	3c0a0000 	lui	$t2,0x0
    6a6c:	254a19f4 	addiu	$t2,$t2,6644
    6a70:	89430003 	lwl	$v1,3($t2)
    6a74:	99430000 	lwr	$v1,0($t2)
    6a78:	aba3005b 	swl	$v1,91($sp)
    6a7c:	bba30058 	swr	$v1,88($sp)
    6a80:	3c0a0000 	lui	$t2,0x0
    6a84:	254a19f8 	addiu	$t2,$t2,6648
    6a88:	89430003 	lwl	$v1,3($t2)
    6a8c:	99430000 	lwr	$v1,0($t2)
    6a90:	aba30063 	swl	$v1,99($sp)
    6a94:	bba30060 	swr	$v1,96($sp)
    6a98:	8c52000c 	lw	$s2,12($v0)
    6a9c:	3c090000 	lui	$t1,0x0
    6aa0:	252919fc 	addiu	$t1,$t1,6652
    6aa4:	89220003 	lwl	$v0,3($t1)
    6aa8:	99220000 	lwr	$v0,0($t1)
    6aac:	aba2006b 	swl	$v0,107($sp)
    6ab0:	bba20068 	swr	$v0,104($sp)
    6ab4:	afa600a8 	sw	$a2,168($sp)
    6ab8:	3c020000 	lui	$v0,0x0
    6abc:	24420000 	addiu	$v0,$v0,0
    6ac0:	8fbe00b0 	lw	$s8,176($sp)
    6ac4:	0040f809 	jalr	$v0
    6ac8:	8eb10008 	lw	$s1,8($s5)
    6acc:	144000f4 	bnez	$v0,6ea0 <VP880InitLine+0x560>
    6ad0:	afa20074 	sw	$v0,116($sp)
    6ad4:	8e420018 	lw	$v0,24($s2)
    6ad8:	54400003 	0x54400003
    6adc:	8e230070 	lw	$v1,112($s1)
    6ae0:	08001ba8 	j	6ea0 <VP880InitLine+0x560>
    6ae4:	24020005 	li	$v0,5
    6ae8:	24020001 	li	$v0,1
    6aec:	54620005 	0x54620005
    6af0:	ae200014 	sw	$zero,20($s1)
    6af4:	24020009 	li	$v0,9
    6af8:	ae220014 	sw	$v0,20($s1)
    6afc:	08001ac2 	j	6b08 <VP880InitLine+0x1c8>
    6b00:	ae220018 	sw	$v0,24($s1)
    6b04:	ae200018 	sw	$zero,24($s1)
    6b08:	ae200038 	sw	$zero,56($s1)
    6b0c:	02002021 	move	$a0,$s0
    6b10:	24050002 	li	$a1,2
    6b14:	00003021 	move	$a2,$zero
    6b18:	27b40070 	addiu	$s4,$sp,112
    6b1c:	3c120000 	lui	$s2,0x0
    6b20:	26520000 	addiu	$s2,$s2,0
    6b24:	0240f809 	jalr	$s2
    6b28:	02803821 	move	$a3,$s4
    6b2c:	3c02afff 	lui	$v0,0xafff
    6b30:	34427a68 	ori	$v0,$v0,0x7a68
    6b34:	3c030000 	lui	$v1,0x0
    6b38:	8c630000 	lw	$v1,0($v1)
    6b3c:	00430019 	multu	$v0,$v1
    6b40:	00001010 	mfhi	$v0
    6b44:	1440ffff 	bnez	$v0,6b44 <VP880InitLine+0x204>
    6b48:	2442ffff 	addiu	$v0,$v0,-1
    6b4c:	8e220070 	lw	$v0,112($s1)
    6b50:	24130001 	li	$s3,1
    6b54:	1453004f 	bne	$v0,$s3,6c94 <VP880InitLine+0x354>
    6b58:	2402ffff 	li	$v0,-1
    6b5c:	8e22000c 	lw	$v0,12($s1)
    6b60:	14530015 	bne	$v0,$s3,6bb8 <VP880InitLine+0x278>
    6b64:	240200ed 	li	$v0,237
    6b68:	a3b30070 	sb	$s3,112($sp)
    6b6c:	02002021 	move	$a0,$s0
    6b70:	24050054 	li	$a1,84
    6b74:	24060001 	li	$a2,1
    6b78:	0240f809 	jalr	$s2
    6b7c:	02803821 	move	$a3,$s4
    6b80:	24020003 	li	$v0,3
    6b84:	a3a20070 	sb	$v0,112($sp)
    6b88:	02002021 	move	$a0,$s0
    6b8c:	24050052 	li	$a1,82
    6b90:	24060001 	li	$a2,1
    6b94:	0240f809 	jalr	$s2
    6b98:	02803821 	move	$a3,$s4
    6b9c:	02002021 	move	$a0,$s0
    6ba0:	240500c2 	li	$a1,194
    6ba4:	24060004 	li	$a2,4
    6ba8:	0240f809 	jalr	$s2
    6bac:	27a70060 	addiu	$a3,$sp,96
    6bb0:	08001b06 	j	6c18 <VP880InitLine+0x2d8>
    6bb4:	24020002 	li	$v0,2
    6bb8:	a3a20070 	sb	$v0,112($sp)
    6bbc:	02002021 	move	$a0,$s0
    6bc0:	24050054 	li	$a1,84
    6bc4:	24060001 	li	$a2,1
    6bc8:	0240f809 	jalr	$s2
    6bcc:	02803821 	move	$a3,$s4
    6bd0:	24020007 	li	$v0,7
    6bd4:	a3a20070 	sb	$v0,112($sp)
    6bd8:	02002021 	move	$a0,$s0
    6bdc:	24050052 	li	$a1,82
    6be0:	24060001 	li	$a2,1
    6be4:	0240f809 	jalr	$s2
    6be8:	02803821 	move	$a3,$s4
    6bec:	02002021 	move	$a0,$s0
    6bf0:	240500f4 	li	$a1,244
    6bf4:	24060004 	li	$a2,4
    6bf8:	0240f809 	jalr	$s2
    6bfc:	27a70068 	addiu	$a3,$sp,104
    6c00:	02002021 	move	$a0,$s0
    6c04:	240500c2 	li	$a1,194
    6c08:	24060004 	li	$a2,4
    6c0c:	0240f809 	jalr	$s2
    6c10:	27a70058 	addiu	$a3,$sp,88
    6c14:	24020002 	li	$v0,2
    6c18:	a3a20070 	sb	$v0,112($sp)
    6c1c:	02002021 	move	$a0,$s0
    6c20:	24050068 	li	$a1,104
    6c24:	24060001 	li	$a2,1
    6c28:	0240f809 	jalr	$s2
    6c2c:	02803821 	move	$a3,$s4
    6c30:	02a02021 	move	$a0,$s5
    6c34:	3c020000 	lui	$v0,0x0
    6c38:	24420000 	addiu	$v0,$v0,0
    6c3c:	0040f809 	jalr	$v0
    6c40:	24050009 	li	$a1,9
    6c44:	02002021 	move	$a0,$s0
    6c48:	24050006 	li	$a1,6
    6c4c:	92e60005 	lbu	$a2,5($s7)
    6c50:	0240f809 	jalr	$s2
    6c54:	26e70006 	addiu	$a3,$s7,6
    6c58:	a3a00070 	sb	$zero,112($sp)
    6c5c:	02002021 	move	$a0,$s0
    6c60:	24050070 	li	$a1,112
    6c64:	24060001 	li	$a2,1
    6c68:	0240f809 	jalr	$s2
    6c6c:	02803821 	move	$a3,$s4
    6c70:	2402003f 	li	$v0,63
    6c74:	a3a20070 	sb	$v0,112($sp)
    6c78:	02002021 	move	$a0,$s0
    6c7c:	24050060 	li	$a1,96
    6c80:	24060001 	li	$a2,1
    6c84:	0240f809 	jalr	$s2
    6c88:	02803821 	move	$a3,$s4
    6c8c:	08001ba1 	j	6e84 <VP880InitLine+0x544>
    6c90:	02002021 	move	$a0,$s0
    6c94:	a222007f 	sb	$v0,127($s1)
    6c98:	02002021 	move	$a0,$s0
    6c9c:	240500fc 	li	$a1,252
    6ca0:	24060002 	li	$a2,2
    6ca4:	0240f809 	jalr	$s2
    6ca8:	27a70010 	addiu	$a3,$sp,16
    6cac:	02a02021 	move	$a0,$s5
    6cb0:	3c020000 	lui	$v0,0x0
    6cb4:	24420000 	addiu	$v0,$v0,0
    6cb8:	0040f809 	jalr	$v0
    6cbc:	00002821 	move	$a1,$zero
    6cc0:	24020002 	li	$v0,2
    6cc4:	a3a20070 	sb	$v0,112($sp)
    6cc8:	02002021 	move	$a0,$s0
    6ccc:	24050054 	li	$a1,84
    6cd0:	24060001 	li	$a2,1
    6cd4:	0240f809 	jalr	$s2
    6cd8:	02803821 	move	$a3,$s4
    6cdc:	02002021 	move	$a0,$s0
    6ce0:	24050006 	li	$a1,6
    6ce4:	92e60005 	lbu	$a2,5($s7)
    6ce8:	0240f809 	jalr	$s2
    6cec:	26e70006 	addiu	$a3,$s7,6
    6cf0:	a3a00070 	sb	$zero,112($sp)
    6cf4:	02002021 	move	$a0,$s0
    6cf8:	24050070 	li	$a1,112
    6cfc:	24060001 	li	$a2,1
    6d00:	0240f809 	jalr	$s2
    6d04:	02803821 	move	$a3,$s4
    6d08:	2402003f 	li	$v0,63
    6d0c:	a3a20070 	sb	$v0,112($sp)
    6d10:	02002021 	move	$a0,$s0
    6d14:	24050060 	li	$a1,96
    6d18:	24060001 	li	$a2,1
    6d1c:	0240f809 	jalr	$s2
    6d20:	02803821 	move	$a3,$s4
    6d24:	a3b30070 	sb	$s3,112($sp)
    6d28:	02002021 	move	$a0,$s0
    6d2c:	24050052 	li	$a1,82
    6d30:	24060001 	li	$a2,1
    6d34:	0240f809 	jalr	$s2
    6d38:	02803821 	move	$a3,$s4
    6d3c:	02002021 	move	$a0,$s0
    6d40:	2405006c 	li	$a1,108
    6d44:	24060002 	li	$a2,2
    6d48:	0240f809 	jalr	$s2
    6d4c:	27a70050 	addiu	$a3,$sp,80
    6d50:	16c00011 	bnez	$s6,6d98 <VP880InitLine+0x458>
    6d54:	02002021 	move	$a0,$s0
    6d58:	240500d2 	li	$a1,210
    6d5c:	2406000b 	li	$a2,11
    6d60:	27b30030 	addiu	$s3,$sp,48
    6d64:	0240f809 	jalr	$s2
    6d68:	02603821 	move	$a3,$s3
    6d6c:	00002821 	move	$a1,$zero
    6d70:	02252021 	addu	$a0,$s1,$a1
    6d74:	02651021 	addu	$v0,$s3,$a1
    6d78:	90430000 	lbu	$v1,0($v0)
    6d7c:	24a20001 	addiu	$v0,$a1,1
    6d80:	304500ff 	andi	$a1,$v0,0xff
    6d84:	2ca2000b 	sltiu	$v0,$a1,11
    6d88:	1440fff9 	bnez	$v0,6d70 <VP880InitLine+0x430>
    6d8c:	a0830074 	sb	$v1,116($a0)
    6d90:	08001b73 	j	6dcc <VP880InitLine+0x48c>
    6d94:	00000000 	nop
    6d98:	24050006 	li	$a1,6
    6d9c:	92c60005 	lbu	$a2,5($s6)
    6da0:	0240f809 	jalr	$s2
    6da4:	26c70006 	addiu	$a3,$s6,6
    6da8:	00002821 	move	$a1,$zero
    6dac:	02252021 	addu	$a0,$s1,$a1
    6db0:	00b61021 	addu	$v0,$a1,$s6
    6db4:	90430006 	lbu	$v1,6($v0)
    6db8:	24a20001 	addiu	$v0,$a1,1
    6dbc:	304500ff 	andi	$a1,$v0,0xff
    6dc0:	2ca2000b 	sltiu	$v0,$a1,11
    6dc4:	1440fff9 	bnez	$v0,6dac <VP880InitLine+0x46c>
    6dc8:	a0830074 	sb	$v1,116($a0)
    6dcc:	17c00007 	bnez	$s8,6dec <VP880InitLine+0x4ac>
    6dd0:	02002021 	move	$a0,$s0
    6dd4:	240500ee 	li	$a1,238
    6dd8:	24060004 	li	$a2,4
    6ddc:	0240f809 	jalr	$s2
    6de0:	27a70018 	addiu	$a3,$sp,24
    6de4:	08001b80 	j	6e00 <VP880InitLine+0x4c0>
    6de8:	02002021 	move	$a0,$s0
    6dec:	24050006 	li	$a1,6
    6df0:	93c60005 	lbu	$a2,5($s8)
    6df4:	0240f809 	jalr	$s2
    6df8:	27c70006 	addiu	$a3,$s8,6
    6dfc:	02002021 	move	$a0,$s0
    6e00:	240500fa 	li	$a1,250
    6e04:	24060002 	li	$a2,2
    6e08:	3c110000 	lui	$s1,0x0
    6e0c:	26310000 	addiu	$s1,$s1,0
    6e10:	0220f809 	jalr	$s1
    6e14:	27a70020 	addiu	$a3,$sp,32
    6e18:	8fa200a8 	lw	$v0,168($sp)
    6e1c:	1440000c 	bnez	$v0,6e50 <VP880InitLine+0x510>
    6e20:	02002021 	move	$a0,$s0
    6e24:	240500c2 	li	$a1,194
    6e28:	24060004 	li	$a2,4
    6e2c:	0220f809 	jalr	$s1
    6e30:	27a70040 	addiu	$a3,$sp,64
    6e34:	02002021 	move	$a0,$s0
    6e38:	240500c6 	li	$a1,198
    6e3c:	24060002 	li	$a2,2
    6e40:	0220f809 	jalr	$s1
    6e44:	27a70048 	addiu	$a3,$sp,72
    6e48:	08001b9a 	j	6e68 <VP880InitLine+0x528>
    6e4c:	24020034 	li	$v0,52
    6e50:	8fa300a8 	lw	$v1,168($sp)
    6e54:	24050006 	li	$a1,6
    6e58:	90660005 	lbu	$a2,5($v1)
    6e5c:	0220f809 	jalr	$s1
    6e60:	24670006 	addiu	$a3,$v1,6
    6e64:	24020034 	li	$v0,52
    6e68:	a3a20070 	sb	$v0,112($sp)
    6e6c:	02002021 	move	$a0,$s0
    6e70:	24050068 	li	$a1,104
    6e74:	24060001 	li	$a2,1
    6e78:	0220f809 	jalr	$s1
    6e7c:	02803821 	move	$a3,$s4
    6e80:	02002021 	move	$a0,$s0
    6e84:	240500e0 	li	$a1,224
    6e88:	24060004 	li	$a2,4
    6e8c:	3c020000 	lui	$v0,0x0
    6e90:	24420000 	addiu	$v0,$v0,0
    6e94:	0040f809 	jalr	$v0
    6e98:	27a70028 	addiu	$a3,$sp,40
    6e9c:	8fa20074 	lw	$v0,116($sp)
    6ea0:	8fbf009c 	lw	$ra,156($sp)
    6ea4:	8fbe0098 	lw	$s8,152($sp)
    6ea8:	8fb70094 	lw	$s7,148($sp)
    6eac:	8fb60090 	lw	$s6,144($sp)
    6eb0:	8fb5008c 	lw	$s5,140($sp)
    6eb4:	8fb40088 	lw	$s4,136($sp)
    6eb8:	8fb30084 	lw	$s3,132($sp)
    6ebc:	8fb20080 	lw	$s2,128($sp)
    6ec0:	8fb1007c 	lw	$s1,124($sp)
    6ec4:	8fb00078 	lw	$s0,120($sp)
    6ec8:	03e00008 	jr	$ra
    6ecc:	27bd00a0 	addiu	$sp,$sp,160

0000000000006ed0 <MakeVP880DeviceObject>:
    6ed0:	a0860008 	sb	$a2,8($a0)
    6ed4:	24020002 	li	$v0,2
    6ed8:	aca20004 	sw	$v0,4($a1)
    6edc:	ac820004 	sw	$v0,4($a0)
    6ee0:	a0a20009 	sb	$v0,9($a1)
    6ee4:	00001821 	move	$v1,$zero
    6ee8:	00031080 	sll	$v0,$v1,0x2
    6eec:	00a21021 	addu	$v0,$a1,$v0
    6ef0:	ac40003c 	sw	$zero,60($v0)
    6ef4:	24630001 	addiu	$v1,$v1,1
    6ef8:	90a20009 	lbu	$v0,9($a1)
    6efc:	306300ff 	andi	$v1,$v1,0xff
    6f00:	0062102b 	sltu	$v0,$v1,$v0
    6f04:	5440fff9 	0x5440fff9
    6f08:	00031080 	sll	$v0,$v1,0x2
    6f0c:	ac85000c 	sw	$a1,12($a0)
    6f10:	3c020000 	lui	$v0,0x0
    6f14:	24420000 	addiu	$v0,$v0,0
    6f18:	ac820010 	sw	$v0,16($a0)
    6f1c:	3c020000 	lui	$v0,0x0
    6f20:	24420000 	addiu	$v0,$v0,0
    6f24:	ac820014 	sw	$v0,20($a0)
    6f28:	3c020000 	lui	$v0,0x0
    6f2c:	24420000 	addiu	$v0,$v0,0
    6f30:	ac820018 	sw	$v0,24($a0)
    6f34:	3c020000 	lui	$v0,0x0
    6f38:	24420000 	addiu	$v0,$v0,0
    6f3c:	ac82001c 	sw	$v0,28($a0)
    6f40:	3c020000 	lui	$v0,0x0
    6f44:	24420000 	addiu	$v0,$v0,0
    6f48:	ac820020 	sw	$v0,32($a0)
    6f4c:	3c020000 	lui	$v0,0x0
    6f50:	24420000 	addiu	$v0,$v0,0
    6f54:	ac820024 	sw	$v0,36($a0)
    6f58:	3c020000 	lui	$v0,0x0
    6f5c:	24420000 	addiu	$v0,$v0,0
    6f60:	ac82002c 	sw	$v0,44($a0)
    6f64:	03e00008 	jr	$ra
    6f68:	00001021 	move	$v0,$zero

0000000000006f6c <VpMakeLineObject>:
    6f6c:	27bdffe0 	addiu	$sp,$sp,-32
    6f70:	afbf0018 	sw	$ra,24($sp)
    6f74:	00804021 	move	$t0,$a0
    6f78:	ad06000c 	sw	$a2,12($t0)
    6f7c:	00a04821 	move	$t1,$a1
    6f80:	ad090008 	sw	$t1,8($t0)
    6f84:	8cc20004 	lw	$v0,4($a2)
    6f88:	24030002 	li	$v1,2
    6f8c:	10430003 	beq	$v0,$v1,6f9c <VpMakeLineObject+0x30>
    6f90:	30e500ff 	andi	$a1,$a3,0xff
    6f94:	08001bee 	j	6fb8 <VpMakeLineObject+0x4c>
    6f98:	24020006 	li	$v0,6
    6f9c:	afa60010 	sw	$a2,16($sp)
    6fa0:	8fa40030 	lw	$a0,48($sp)
    6fa4:	01003021 	move	$a2,$t0
    6fa8:	3c020000 	lui	$v0,0x0
    6fac:	24420000 	addiu	$v0,$v0,0
    6fb0:	0040f809 	jalr	$v0
    6fb4:	01203821 	move	$a3,$t1
    6fb8:	8fbf0018 	lw	$ra,24($sp)
    6fbc:	03e00008 	jr	$ra
    6fc0:	27bd0020 	addiu	$sp,$sp,32
*/

void
VpMpiCmd(
		VpDeviceIdType deviceId,    /**< Chip select, connector and 3 or 4 wire interface for command */
		uint8 ecVal,        	    /**< Value to write to the EC register */
		uint8 cmd,                  /**< Command number */
		uint8 cmdLen,               /**< Number of bytes used by command (cmd) */
		uint8 *dataPtr)             /**< Pointer to the data location */
{
	uint8 byteCnt;
	uint8 isRead = (cmd & READ_COMMAND); 


/*
0000000000006fc4 <VpMpiCmd>:
    6fc4:	27bdffd0 	addiu	$sp,$sp,-48
    6fc8:	afbf0028 	sw	$ra,40($sp)
    6fcc:	afb50024 	sw	$s5,36($sp)
    6fd0:	afb40020 	sw	$s4,32($sp)
    6fd4:	afb3001c 	sw	$s3,28($sp)
    6fd8:	afb20018 	sw	$s2,24($sp)
    6fdc:	afb10014 	sw	$s1,20($sp)
    6fe0:	afb00010 	sw	$s0,16($sp)
    6fe4:	00e0a821 	move	$s5,$a3
    6fe8:	309200ff 	andi	$s2,$a0,0xff
    6fec:	30b100ff 	andi	$s1,$a1,0xff
    6ff0:	30d400ff 	andi	$s4,$a2,0xff
    6ff4:	32330001 	andi	$s3,$s1,0x1
*/
	deviceId &= 0xff;
	ecVal &= 0xff;
	cmd &= 0xff;

#define CSLAC_EC_REG_RD    0x4B   /* Same for all CSLAC devices */
#define CSLAC_EC_REG_WRT   0x4A   /* Same for all CSLAC devices */
#define CSLAC_EC_REG_LEN   0x01   /* Same for all CSLAC devices */


/*
    6ff8:	24020001 	li	$v0,1
    6ffc:	3c100000 	lui	$s0,0x0
    7000:	26100004 	addiu	$s0,$s0,4
    7004:	c2030000 	lwc0	$3,0($s0)
    7008:	00622023 	subu	$a0,$v1,$v0
    700c:	e2040000 	swc0	$4,0($s0)
    7010:	1080fffc 	beqz	$a0,7004 <VpMpiCmd+0x40>
    7014:	00622023 	subu	$a0,$v1,$v0
    7018:	00801021 	move	$v0,$a0
    701c:	04410005 	bgez	$v0,7034 <VpMpiCmd+0x70>
    7020:	00000000 	nop
    7024:	3c020000 	lui	$v0,0x0
    7028:	24420000 	addiu	$v0,$v0,0
    702c:	0040f809 	jalr	$v0
    7030:	2604fffc 	addiu	$a0,$s0,-4
*/
/* Configure glue logic as necessary to talk to the device */
/* Start critical section for MPI access */
VpSysEnterCritical(deviceId, VP_MPI_CRITICAL_SEC);

/*
    7034:	1260000a 	beqz	$s3,7060 <VpMpiCmd+0x9c>
    7038:	02402021 	move	$a0,$s2		// tid
*/
	if(isRead) {
/* read command
    703c:	02202821 	move	$a1,$s1
    7040:	3c020000 	lui	$v0,0x0
    7044:	8c420000 	lw	$v0,0($v0)
    7048:	02a03021 	move	$a2,$s5
    704c:	8c420004 	lw	$v0,4($v0)
    7050:	0040f809 	jalr	$v0
    7054:	02803821 	move	$a3,$s4
    7058:	08001c20 	j	7080 <VpMpiCmd+0xbc>
    705c:	24020001 	li	$v0,1
*/
		// api->read(deviceId, ecVal, cmdLen, cmd, dataPtr);
		
	} else {
/* write command
    7060:	02202821 	move	$a1,$s1
    7064:	3c020000 	lui	$v0,0x0
    7068:	8c420000 	lw	$v0,0($v0)
    706c:	02a03021 	move	$a2,$s5
    7070:	8c420008 	lw	$v0,8($v0)
    7074:	0040f809 	jalr	$v0
    7078:	02803821 	move	$a3,$s4
*/
		// api->read(deviceId, ecVal, 
	}
/*
    707c:	24020001 	li	$v0,1
    7080:	c2030000 	lwc0	$3,0($s0)
    7084:	00622021 	addu	$a0,$v1,$v0
    7088:	e2040000 	swc0	$4,0($s0)
    708c:	1080fffc 	beqz	$a0,7080 <VpMpiCmd+0xbc>
    7090:	00622021 	addu	$a0,$v1,$v0
    7094:	00801021 	move	$v0,$a0
    7098:	1c400006 	bgtz	$v0,70b4 <VpMpiCmd+0xf0>
    709c:	8fbf0028 	lw	$ra,40($sp)
    70a0:	3c020000 	lui	$v0,0x0
    70a4:	24420000 	addiu	$v0,$v0,0
    70a8:	0040f809 	jalr	$v0
    70ac:	2604fffc 	addiu	$a0,$s0,-4
*/
	// mutex_unlock
/*
    70b0:	8fbf0028 	lw	$ra,40($sp)
    70b4:	8fb50024 	lw	$s5,36($sp)
    70b8:	8fb40020 	lw	$s4,32($sp)
    70bc:	8fb3001c 	lw	$s3,28($sp)
    70c0:	8fb20018 	lw	$s2,24($sp)
    70c4:	8fb10014 	lw	$s1,20($sp)
    70c8:	8fb00010 	lw	$s0,16($sp)
    70cc:	03e00008 	jr	$ra
    70d0:	27bd0030 	addiu	$sp,$sp,48
*/
	VpSysExitCritical(deviceId, VP_MPI_CRITICAL_SEC);
}

/*

00000000000070d4 <VpSetLineState>:
    70d4:	27bdffe8 	addiu	$sp,$sp,-24
    70d8:	afbf0010 	sw	$ra,16($sp)
    70dc:	8c82000c 	lw	$v0,12($a0)
    70e0:	8c420024 	lw	$v0,36($v0)
    70e4:	50400005 	0x50400005
    70e8:	24020006 	li	$v0,6
    70ec:	0040f809 	jalr	$v0
    70f0:	00000000 	nop
    70f4:	08001c40 	j	7100 <VpSetLineState+0x2c>
    70f8:	8fbf0010 	lw	$ra,16($sp)
    70fc:	8fbf0010 	lw	$ra,16($sp)
    7100:	03e00008 	jr	$ra
    7104:	27bd0018 	addiu	$sp,$sp,24

0000000000007108 <VpSetChannel>:
    7108:	27bdffe8 	addiu	$sp,$sp,-24
    710c:	afbf0010 	sw	$ra,16($sp)
    7110:	8c82000c 	lw	$v0,12($a0)
    7114:	8c42002c 	lw	$v0,44($v0)
    7118:	50400005 	0x50400005
    711c:	24020006 	li	$v0,6
    7120:	0040f809 	jalr	$v0
    7124:	00000000 	nop
    7128:	08001c4d 	j	7134 <VpSetChannel+0x2c>
    712c:	8fbf0010 	lw	$ra,16($sp)
    7130:	8fbf0010 	lw	$ra,16($sp)
    7134:	03e00008 	jr	$ra
    7138:	27bd0018 	addiu	$sp,$sp,24
    713c:	27bdffe0 	addiu	$sp,$sp,-32
    7140:	afbf001c 	sw	$ra,28($sp)
    7144:	afb00018 	sw	$s0,24($sp)
    7148:	24050073 	li	$a1,115
    714c:	24060002 	li	$a2,2
    7150:	27a70010 	addiu	$a3,$sp,16
    7154:	00808021 	move	$s0,$a0
    7158:	3c020000 	lui	$v0,0x0
    715c:	24426fc4 	addiu	$v0,$v0,28612
    7160:	0040f809 	jalr	$v0
    7164:	92040000 	lbu	$a0,0($s0)
    7168:	93a30010 	lbu	$v1,16($sp)
    716c:	24020001 	li	$v0,1
    7170:	14620006 	bne	$v1,$v0,718c <VpSetChannel+0x84>
    7174:	93a30011 	lbu	$v1,17($sp)
    7178:	240200b2 	li	$v0,178
    717c:	1062000d 	beq	$v1,$v0,71b4 <VpSetChannel+0xac>
    7180:	240200b6 	li	$v0,182
    7184:	5062000c 	0x5062000c
    7188:	8e020044 	lw	$v0,68($s0)
    718c:	3c040000 	lui	$a0,0x0
    7190:	24841d18 	addiu	$a0,$a0,7448
    7194:	96050000 	lhu	$a1,0($s0)
    7198:	93a60010 	lbu	$a2,16($sp)
    719c:	3c020000 	lui	$v0,0x0
    71a0:	24420000 	addiu	$v0,$v0,0
    71a4:	0040f809 	jalr	$v0
    71a8:	93a70011 	lbu	$a3,17($sp)
    71ac:	08001c73 	j	71cc <VpSetChannel+0xc4>
    71b0:	00001021 	move	$v0,$zero
    71b4:	8e020044 	lw	$v0,68($s0)
    71b8:	8c44000c 	lw	$a0,12($v0)
    71bc:	8c820010 	lw	$v0,16($a0)
    71c0:	0040f809 	jalr	$v0
    71c4:	00000000 	nop
    71c8:	24020001 	li	$v0,1
    71cc:	8fbf001c 	lw	$ra,28($sp)
    71d0:	8fb00018 	lw	$s0,24($sp)
    71d4:	03e00008 	jr	$ra
    71d8:	27bd0020 	addiu	$sp,$sp,32
    71dc:	27bdffe8 	addiu	$sp,$sp,-24
    71e0:	afbf0010 	sw	$ra,16($sp)
    71e4:	8c840044 	lw	$a0,68($a0)
    71e8:	8c82000c 	lw	$v0,12($a0)
    71ec:	8c420018 	lw	$v0,24($v0)
    71f0:	0040f809 	jalr	$v0
    71f4:	00000000 	nop
    71f8:	8fbf0010 	lw	$ra,16($sp)
    71fc:	03e00008 	jr	$ra
    7200:	27bd0018 	addiu	$sp,$sp,24
    7204:	27bdffa8 	addiu	$sp,$sp,-88
    7208:	afbf0050 	sw	$ra,80($sp)
    720c:	3c070000 	lui	$a3,0x0
    7210:	24e71d4c 	addiu	$a3,$a3,7500
    7214:	88e20003 	lwl	$v0,3($a3)
    7218:	98e20000 	lwr	$v0,0($a3)
    721c:	88e30007 	lwl	$v1,7($a3)
    7220:	98e30004 	lwr	$v1,4($a3)
    7224:	88e6000b 	lwl	$a2,11($a3)
    7228:	98e60008 	lwr	$a2,8($a3)
    722c:	aba2001b 	swl	$v0,27($sp)
    7230:	bba20018 	swr	$v0,24($sp)
    7234:	aba3001f 	swl	$v1,31($sp)
    7238:	bba3001c 	swr	$v1,28($sp)
    723c:	aba60023 	swl	$a2,35($sp)
    7240:	bba60020 	swr	$a2,32($sp)
    7244:	80e2000c 	lb	$v0,12($a3)
    7248:	80e3000d 	lb	$v1,13($a3)
    724c:	a3a20024 	sb	$v0,36($sp)
    7250:	a3a30025 	sb	$v1,37($sp)
    7254:	3c070000 	lui	$a3,0x0
    7258:	24e71d5c 	addiu	$a3,$a3,7516
    725c:	88e20003 	lwl	$v0,3($a3)
    7260:	98e20000 	lwr	$v0,0($a3)
    7264:	88e30007 	lwl	$v1,7($a3)
    7268:	98e30004 	lwr	$v1,4($a3)
    726c:	88e6000b 	lwl	$a2,11($a3)
    7270:	98e60008 	lwr	$a2,8($a3)
    7274:	aba2002b 	swl	$v0,43($sp)
    7278:	bba20028 	swr	$v0,40($sp)
    727c:	aba3002f 	swl	$v1,47($sp)
    7280:	bba3002c 	swr	$v1,44($sp)
    7284:	aba60033 	swl	$a2,51($sp)
    7288:	bba60030 	swr	$a2,48($sp)
    728c:	88e2000f 	lwl	$v0,15($a3)
    7290:	98e2000c 	lwr	$v0,12($a3)
    7294:	88e30013 	lwl	$v1,19($a3)
    7298:	98e30010 	lwr	$v1,16($a3)
    729c:	aba20037 	swl	$v0,55($sp)
    72a0:	bba20034 	swr	$v0,52($sp)
    72a4:	aba3003b 	swl	$v1,59($sp)
    72a8:	bba30038 	swr	$v1,56($sp)
    72ac:	3c070000 	lui	$a3,0x0
    72b0:	24e71d70 	addiu	$a3,$a3,7536
    72b4:	88e20003 	lwl	$v0,3($a3)
    72b8:	98e20000 	lwr	$v0,0($a3)
    72bc:	88e30007 	lwl	$v1,7($a3)
    72c0:	98e30004 	lwr	$v1,4($a3)
    72c4:	88e6000b 	lwl	$a2,11($a3)
    72c8:	98e60008 	lwr	$a2,8($a3)
    72cc:	aba20043 	swl	$v0,67($sp)
    72d0:	bba20040 	swr	$v0,64($sp)
    72d4:	aba30047 	swl	$v1,71($sp)
    72d8:	bba30044 	swr	$v1,68($sp)
    72dc:	aba6004b 	swl	$a2,75($sp)
    72e0:	bba60048 	swr	$a2,72($sp)
    72e4:	88e2000f 	lwl	$v0,15($a3)
    72e8:	98e2000c 	lwr	$v0,12($a3)
    72ec:	aba2004f 	swl	$v0,79($sp)
    72f0:	bba2004c 	swr	$v0,76($sp)
    72f4:	00003821 	move	$a3,$zero
    72f8:	00803021 	move	$a2,$a0
    72fc:	90a30010 	lbu	$v1,16($a1)
    7300:	2462ffdc 	addiu	$v0,$v1,-36
    7304:	2c420016 	sltiu	$v0,$v0,22
    7308:	10400002 	beqz	$v0,7314 <VpSetChannel+0x20c>
    730c:	27a40040 	addiu	$a0,$sp,64
    7310:	a0c30068 	sb	$v1,104($a2)
    7314:	90c20068 	lbu	$v0,104($a2)
    7318:	2c42002e 	sltiu	$v0,$v0,46
    731c:	50400001 	0x50400001
    7320:	00803821 	move	$a3,$a0
    7324:	3c025555 	lui	$v0,0x5555
    7328:	90c30068 	lbu	$v1,104($a2)
    732c:	34425556 	ori	$v0,$v0,0x5556
    7330:	2463ffdc 	addiu	$v1,$v1,-36
    7334:	00620018 	mult	$v1,$v0
    7338:	00001010 	mfhi	$v0
    733c:	00031fc3 	sra	$v1,$v1,0x1f
    7340:	00431023 	subu	$v0,$v0,$v1
    7344:	00021080 	sll	$v0,$v0,0x2
    7348:	34420022 	ori	$v0,$v0,0x22
    734c:	94a30012 	lhu	$v1,18($a1)
    7350:	a3a20024 	sb	$v0,36($sp)
    7354:	2462ffee 	addiu	$v0,$v1,-18
    7358:	2c420020 	sltiu	$v0,$v0,32
    735c:	54400001 	0x54400001
    7360:	a4c3006a 	sh	$v1,106($a2)
    7364:	90a2000e 	lbu	$v0,14($a1)
    7368:	3c010000 	lui	$at,0x0
    736c:	d4241d80 	0xd4241d80
    7370:	44820000 	mtc1	$v0,$f0
    7374:	00000000 	nop
    7378:	46800021 	cvt.d.w	$f0,$f0
    737c:	46240002 	mul.d	$f0,$f0,$f4
    7380:	3c010000 	lui	$at,0x0
    7384:	d4221d88 	0xd4221d88
    7388:	46220183 	div.d	$f6,$f0,$f2
    738c:	46223082 	mul.d	$f2,$f6,$f2
    7390:	4620300d 	c1	0x20300d
    7394:	44030000 	mfc1	$v1,$f0
    7398:	46241183 	div.d	$f6,$f2,$f4
    739c:	30637fff 	andi	$v1,$v1,0x7fff
    73a0:	90c2006a 	lbu	$v0,106($a2)
    73a4:	a3a30031 	sb	$v1,49($sp)
    73a8:	3c0141e0 	lui	$at,0x41e0
    73ac:	44810800 	mtc1	$at,$f1
    73b0:	44800000 	mtc1	$zero,$f0
    73b4:	2442ffee 	addiu	$v0,$v0,-18
    73b8:	a3a20025 	sb	$v0,37($sp)
    73bc:	4626003e 	c.le.d	$f0,$f6
    73c0:	00031203 	sra	$v0,$v1,0x8
    73c4:	45010005 	bc1t	73dc <VpSetChannel+0x2d4>
    73c8:	a3a20030 	sb	$v0,48($sp)
    73cc:	4620300d 	c1	0x20300d
    73d0:	44030000 	mfc1	$v1,$f0
    73d4:	08001cfd 	j	73f4 <VpSetChannel+0x2ec>
    73d8:	a0c30066 	sb	$v1,102($a2)
    73dc:	46203001 	sub.d	$f0,$f6,$f0
    73e0:	4620008d 	c1	0x20008d
    73e4:	44031000 	mfc1	$v1,$f2
    73e8:	3c028000 	lui	$v0,0x8000
    73ec:	00621825 	or	$v1,$v1,$v0
    73f0:	a0c30066 	sb	$v1,102($a2)
    73f4:	8ca30008 	lw	$v1,8($a1)
    73f8:	24020001 	li	$v0,1
    73fc:	50620007 	0x50620007
    7400:	90a2000d 	lbu	$v0,13($a1)
    7404:	1060007b 	beqz	$v1,75f4 <VpSetChannel+0x4ec>
    7408:	2c620005 	sltiu	$v0,$v1,5
    740c:	5040007a 	0x5040007a
    7410:	8ca20008 	lw	$v0,8($a1)
    7414:	08001d3a 	j	74e8 <VpSetChannel+0x3e0>
    7418:	90a4000d 	lbu	$a0,13($a1)
    741c:	2c420051 	sltiu	$v0,$v0,81
    7420:	14400003 	bnez	$v0,7430 <VpSetChannel+0x328>
    7424:	a3a0002f 	sb	$zero,47($sp)
    7428:	24020050 	li	$v0,80
    742c:	a0a2000d 	sb	$v0,13($a1)
    7430:	90a2000d 	lbu	$v0,13($a1)
    7434:	3c010000 	lui	$at,0x0
    7438:	d4221d90 	0xd4221d90
    743c:	44820000 	mtc1	$v0,$f0
    7440:	00000000 	nop
    7444:	46800021 	cvt.d.w	$f0,$f0
    7448:	46220183 	div.d	$f6,$f0,$f2
    744c:	3c0141e0 	lui	$at,0x41e0
    7450:	44810800 	mtc1	$at,$f1
    7454:	44800000 	mtc1	$zero,$f0
    7458:	00000000 	nop
    745c:	4626003e 	c.le.d	$f0,$f6
    7460:	00000000 	nop
    7464:	45030005 	0x45030005
    7468:	46203001 	sub.d	$f0,$f6,$f0
    746c:	4620300d 	c1	0x20300d
    7470:	44030000 	mfc1	$v1,$f0
    7474:	08001d23 	j	748c <VpSetChannel+0x384>
    7478:	00000000 	nop
    747c:	4620008d 	c1	0x20008d
    7480:	44031000 	mfc1	$v1,$f2
    7484:	3c028000 	lui	$v0,0x8000
    7488:	00621825 	or	$v1,$v1,$v0
    748c:	3c010000 	lui	$at,0x0
    7490:	d4201d98 	0xd4201d98
    7494:	46203182 	mul.d	$f6,$f6,$f0
    7498:	3063ffff 	andi	$v1,$v1,0xffff
    749c:	3c0141e0 	lui	$at,0x41e0
    74a0:	44810800 	mtc1	$at,$f1
    74a4:	44800000 	mtc1	$zero,$f0
    74a8:	00031202 	srl	$v0,$v1,0x8
    74ac:	4626003e 	c.le.d	$f0,$f6
    74b0:	a3a20032 	sb	$v0,50($sp)
    74b4:	45010005 	bc1t	74cc <VpSetChannel+0x3c4>
    74b8:	a3a30033 	sb	$v1,51($sp)
    74bc:	4620300d 	c1	0x20300d
    74c0:	44030000 	mfc1	$v1,$f0
    74c4:	08001d7d 	j	75f4 <VpSetChannel+0x4ec>
    74c8:	a0c30065 	sb	$v1,101($a2)
    74cc:	46203001 	sub.d	$f0,$f6,$f0
    74d0:	4620008d 	c1	0x20008d
    74d4:	44031000 	mfc1	$v1,$f2
    74d8:	3c028000 	lui	$v0,0x8000
    74dc:	00621825 	or	$v1,$v1,$v0
    74e0:	08001d7d 	j	75f4 <VpSetChannel+0x4ec>
    74e4:	a0c30065 	sb	$v1,101($a2)
    74e8:	24021f40 	li	$v0,8000
    74ec:	0044001a 	div	$zero,$v0,$a0
    74f0:	24030001 	li	$v1,1
    74f4:	00001012 	mflo	$v0
    74f8:	a3a3002f 	sb	$v1,47($sp)
    74fc:	50800001 	0x50800001
    7500:	0007000d 	break	0x7
    7504:	3043ffff 	andi	$v1,$v0,0xffff
    7508:	00031202 	srl	$v0,$v1,0x8
    750c:	a3a20036 	sb	$v0,54($sp)
    7510:	a3a30037 	sb	$v1,55($sp)
    7514:	8ca30008 	lw	$v1,8($a1)
    7518:	24020003 	li	$v0,3
    751c:	10620010 	beq	$v1,$v0,7560 <VpSetChannel+0x458>
    7520:	2c620004 	sltiu	$v0,$v1,4
    7524:	10400005 	beqz	$v0,753c <VpSetChannel+0x434>
    7528:	24020002 	li	$v0,2
    752c:	10620008 	beq	$v1,$v0,7550 <VpSetChannel+0x448>
    7530:	00000000 	nop
    7534:	08001d5f 	j	757c <VpSetChannel+0x474>
    7538:	00000000 	nop
    753c:	24020004 	li	$v0,4
    7540:	50620008 	0x50620008
    7544:	90a2000d 	lbu	$v0,13($a1)
    7548:	08001d5f 	j	757c <VpSetChannel+0x474>
    754c:	00000000 	nop
    7550:	3c010000 	lui	$at,0x0
    7554:	d4221da0 	0xd4221da0
    7558:	08001d5b 	j	756c <VpSetChannel+0x464>
    755c:	90a2000d 	lbu	$v0,13($a1)
    7560:	90a2000d 	lbu	$v0,13($a1)
    7564:	3c010000 	lui	$at,0x0
    7568:	d4221da8 	0xd4221da8
    756c:	44820000 	mtc1	$v0,$f0
    7570:	00000000 	nop
    7574:	46800021 	cvt.d.w	$f0,$f0
    7578:	46220183 	div.d	$f6,$f0,$f2
    757c:	3c010000 	lui	$at,0x0
    7580:	d4201db0 	0xd4201db0
    7584:	46203002 	mul.d	$f0,$f6,$f0
    7588:	3c013ff0 	lui	$at,0x3ff0
    758c:	44811800 	mtc1	$at,$f3
    7590:	44801000 	mtc1	$zero,$f2
    7594:	00000000 	nop
    7598:	46201003 	div.d	$f0,$f2,$f0
    759c:	3c0141e0 	lui	$at,0x41e0
    75a0:	44811800 	mtc1	$at,$f3
    75a4:	44801000 	mtc1	$zero,$f2
    75a8:	00000000 	nop
    75ac:	4620103e 	c.le.d	$f2,$f0
    75b0:	00000000 	nop
    75b4:	45030005 	0x45030005
    75b8:	46220001 	sub.d	$f0,$f0,$f2
    75bc:	4620008d 	c1	0x20008d
    75c0:	44031000 	mfc1	$v1,$f2
    75c4:	08001d78 	j	75e0 <VpSetChannel+0x4d8>
    75c8:	3063ffff 	andi	$v1,$v1,0xffff
    75cc:	4620008d 	c1	0x20008d
    75d0:	44031000 	mfc1	$v1,$f2
    75d4:	3c028000 	lui	$v0,0x8000
    75d8:	00621825 	or	$v1,$v1,$v0
    75dc:	3063ffff 	andi	$v1,$v1,0xffff
    75e0:	00031202 	srl	$v0,$v1,0x8
    75e4:	a3a20032 	sb	$v0,50($sp)
    75e8:	90a2000d 	lbu	$v0,13($a1)
    75ec:	a3a30033 	sb	$v1,51($sp)
    75f0:	a0c20065 	sb	$v0,101($a2)
    75f4:	8ca20008 	lw	$v0,8($a1)
    75f8:	acc20060 	sw	$v0,96($a2)
    75fc:	90a2000c 	lbu	$v0,12($a1)
    7600:	2c42009c 	sltiu	$v0,$v0,156
    7604:	54400004 	0x54400004
    7608:	90a2000c 	lbu	$v0,12($a1)
    760c:	2402009b 	li	$v0,155
    7610:	a0a2000c 	sb	$v0,12($a1)
    7614:	90a2000c 	lbu	$v0,12($a1)
    7618:	3c010000 	lui	$at,0x0
    761c:	d4221db8 	0xd4221db8
    7620:	44820000 	mtc1	$v0,$f0
    7624:	00000000 	nop
    7628:	46800021 	cvt.d.w	$f0,$f0
    762c:	46220002 	mul.d	$f0,$f0,$f2
    7630:	3c010000 	lui	$at,0x0
    7634:	d4221dc0 	0xd4221dc0
    7638:	46220183 	div.d	$f6,$f0,$f2
    763c:	3c0141e0 	lui	$at,0x41e0
    7640:	44810800 	mtc1	$at,$f1
    7644:	44800000 	mtc1	$zero,$f0
    7648:	00000000 	nop
    764c:	4626003e 	c.le.d	$f0,$f6
    7650:	00000000 	nop
    7654:	45030005 	0x45030005
    7658:	46203001 	sub.d	$f0,$f6,$f0
    765c:	4620300d 	c1	0x20300d
    7660:	44030000 	mfc1	$v1,$f0
    7664:	08001d9f 	j	767c <VpSetChannel+0x574>
    7668:	00000000 	nop
    766c:	4620008d 	c1	0x20008d
    7670:	44031000 	mfc1	$v1,$f2
    7674:	3c028000 	lui	$v0,0x8000
    7678:	00621825 	or	$v1,$v1,$v0
    767c:	3c010000 	lui	$at,0x0
    7680:	d4201dc8 	0xd4201dc8
    7684:	46203002 	mul.d	$f0,$f6,$f0
    7688:	3c010000 	lui	$at,0x0
    768c:	d4221dd0 	0xd4221dd0
    7690:	46220183 	div.d	$f6,$f0,$f2
    7694:	3063ffff 	andi	$v1,$v1,0xffff
    7698:	3c0141e0 	lui	$at,0x41e0
    769c:	44810800 	mtc1	$at,$f1
    76a0:	44800000 	mtc1	$zero,$f0
    76a4:	00031202 	srl	$v0,$v1,0x8
    76a8:	4626003e 	c.le.d	$f0,$f6
    76ac:	a3a20034 	sb	$v0,52($sp)
    76b0:	45010005 	bc1t	76c8 <VpSetChannel+0x5c0>
    76b4:	a3a30035 	sb	$v1,53($sp)
    76b8:	4620300d 	c1	0x20300d
    76bc:	44030000 	mfc1	$v1,$f0
    76c0:	08001db8 	j	76e0 <VpSetChannel+0x5d8>
    76c4:	a0c30064 	sb	$v1,100($a2)
    76c8:	46203001 	sub.d	$f0,$f6,$f0
    76cc:	4620008d 	c1	0x20008d
    76d0:	44031000 	mfc1	$v1,$f2
    76d4:	3c028000 	lui	$v0,0x8000
    76d8:	00621825 	or	$v1,$v1,$v0
    76dc:	a0c30064 	sb	$v1,100($a2)
    76e0:	3c030000 	lui	$v1,0x0
    76e4:	24630000 	addiu	$v1,$v1,0
    76e8:	8ca50004 	lw	$a1,4($a1)
    76ec:	24020001 	li	$v0,1
    76f0:	10a20005 	beq	$a1,$v0,7708 <VpSetChannel+0x600>
    76f4:	24020002 	li	$v0,2
    76f8:	54a20004 	0x54a20004
    76fc:	acc5005c 	sw	$a1,92($a2)
    7700:	3c030000 	lui	$v1,0x0
    7704:	24630000 	addiu	$v1,$v1,0
    7708:	acc5005c 	sw	$a1,92($a2)
    770c:	00602821 	move	$a1,$v1
    7710:	8cc40044 	lw	$a0,68($a2)
    7714:	27a60018 	addiu	$a2,$sp,24
    7718:	8c82000c 	lw	$v0,12($a0)
    771c:	afa70010 	sw	$a3,16($sp)
    7720:	8c420014 	lw	$v0,20($v0)
    7724:	0040f809 	jalr	$v0
    7728:	27a70028 	addiu	$a3,$sp,40
    772c:	8fbf0050 	lw	$ra,80($sp)
    7730:	03e00008 	jr	$ra
    7734:	27bd0058 	addiu	$sp,$sp,88
    7738:	27bdffe0 	addiu	$sp,$sp,-32
    773c:	afbf0018 	sw	$ra,24($sp)
    7740:	afb10014 	sw	$s1,20($sp)
    7744:	afb00010 	sw	$s0,16($sp)
    7748:	00808821 	move	$s1,$a0
    774c:	24a5d8ef 	addiu	$a1,$a1,-10001
    7750:	2ca2000d 	sltiu	$v0,$a1,13
    7754:	1040004d 	beqz	$v0,788c <VpSetChannel+0x784>
    7758:	00c08021 	move	$s0,$a2
    775c:	00051080 	sll	$v0,$a1,0x2
    7760:	3c010000 	lui	$at,0x0
    7764:	00220821 	addu	$at,$at,$v0
    7768:	8c221dd8 	lw	$v0,7640($at)
    776c:	00400008 	jr	$v0
    7770:	00000000 	nop
    7774:	8e220044 	lw	$v0,68($s1)
    7778:	24050003 	li	$a1,3
    777c:	8c430008 	lw	$v1,8($v0)
    7780:	24020001 	li	$v0,1
    7784:	08001de7 	j	779c <VpSetChannel+0x694>
    7788:	ac62002c 	sw	$v0,44($v1)
    778c:	8e220044 	lw	$v0,68($s1)
    7790:	8c420008 	lw	$v0,8($v0)
    7794:	00002821 	move	$a1,$zero
    7798:	ac40002c 	sw	$zero,44($v0)
    779c:	3c020000 	lui	$v0,0x0
    77a0:	244270d4 	addiu	$v0,$v0,28884
    77a4:	0040f809 	jalr	$v0
    77a8:	8e240044 	lw	$a0,68($s1)
    77ac:	08001e24 	j	7890 <VpSetChannel+0x788>
    77b0:	8fbf0018 	lw	$ra,24($sp)
    77b4:	26070002 	addiu	$a3,$s0,2
    77b8:	92240000 	lbu	$a0,0($s1)
    77bc:	92050000 	lbu	$a1,0($s0)
    77c0:	3c020000 	lui	$v0,0x0
    77c4:	24426fc4 	addiu	$v0,$v0,28612
    77c8:	0040f809 	jalr	$v0
    77cc:	92060001 	lbu	$a2,1($s0)
    77d0:	08001e24 	j	7890 <VpSetChannel+0x788>
    77d4:	8fbf0018 	lw	$ra,24($sp)
    77d8:	8e02000c 	lw	$v0,12($s0)
    77dc:	1040002c 	beqz	$v0,7890 <VpSetChannel+0x788>
    77e0:	8fbf0018 	lw	$ra,24($sp)
    77e4:	8e020008 	lw	$v0,8($s0)
    77e8:	5040002a 	0x5040002a
    77ec:	8fb10014 	lw	$s1,20($sp)
    77f0:	8e020004 	lw	$v0,4($s0)
    77f4:	2842fff4 	slti	$v0,$v0,-12
    77f8:	10400002 	beqz	$v0,7804 <VpSetChannel+0x6fc>
    77fc:	2402fff4 	li	$v0,-12
    7800:	ae020004 	sw	$v0,4($s0)
    7804:	8e020000 	lw	$v0,0($s0)
    7808:	2842fffa 	slti	$v0,$v0,-6
    780c:	10400002 	beqz	$v0,7818 <VpSetChannel+0x710>
    7810:	2402fffa 	li	$v0,-6
    7814:	ae020000 	sw	$v0,0($s0)
    7818:	8e020004 	lw	$v0,4($s0)
    781c:	5c400001 	0x5c400001
    7820:	ae000004 	sw	$zero,4($s0)
    7824:	8e020000 	lw	$v0,0($s0)
    7828:	28420007 	slti	$v0,$v0,7
    782c:	54400004 	0x54400004
    7830:	8e240044 	lw	$a0,68($s1)
    7834:	24020006 	li	$v0,6
    7838:	ae020000 	sw	$v0,0($s0)
    783c:	8e240044 	lw	$a0,68($s1)
    7840:	8e050000 	lw	$a1,0($s0)
    7844:	3c020000 	lui	$v0,0x0
    7848:	24420000 	addiu	$v0,$v0,0
    784c:	0040f809 	jalr	$v0
    7850:	8e060004 	lw	$a2,4($s0)
    7854:	8e020000 	lw	$v0,0($s0)
    7858:	ae22006c 	sw	$v0,108($s1)
    785c:	8e020004 	lw	$v0,4($s0)
    7860:	ae220070 	sw	$v0,112($s1)
    7864:	8e020000 	lw	$v0,0($s0)
    7868:	ae220074 	sw	$v0,116($s1)
    786c:	8e020004 	lw	$v0,4($s0)
    7870:	08001e23 	j	788c <VpSetChannel+0x784>
    7874:	ae220078 	sw	$v0,120($s1)
    7878:	02202021 	move	$a0,$s1
    787c:	3c020000 	lui	$v0,0x0
    7880:	24427204 	addiu	$v0,$v0,29188
    7884:	0040f809 	jalr	$v0
    7888:	02002821 	move	$a1,$s0
    788c:	8fbf0018 	lw	$ra,24($sp)
    7890:	8fb10014 	lw	$s1,20($sp)
    7894:	8fb00010 	lw	$s0,16($sp)
    7898:	03e00008 	jr	$ra
    789c:	27bd0020 	addiu	$sp,$sp,32
    78a0:	27bdffd0 	addiu	$sp,$sp,-48
    78a4:	afbf002c 	sw	$ra,44($sp)
    78a8:	afb40028 	sw	$s4,40($sp)
    78ac:	afb30024 	sw	$s3,36($sp)
    78b0:	afb20020 	sw	$s2,32($sp)
    78b4:	afb1001c 	sw	$s1,28($sp)
    78b8:	afb00018 	sw	$s0,24($sp)
    78bc:	00808021 	move	$s0,$a0
    78c0:	9602002e 	lhu	$v0,46($s0)
    78c4:	3c030000 	lui	$v1,0x0
    78c8:	94630010 	lhu	$v1,16($v1)
    78cc:	2442ffff 	addiu	$v0,$v0,-1
    78d0:	3053ffff 	andi	$s3,$v0,0xffff
    78d4:	24020001 	li	$v0,1
    78d8:	10620009 	beq	$v1,$v0,7900 <VpSetChannel+0x7f8>
    78dc:	0000a021 	move	$s4,$zero
    78e0:	28620002 	slti	$v0,$v1,2
    78e4:	5440000b 	0x5440000b
    78e8:	92020002 	lbu	$v0,2($s0)
    78ec:	24020002 	li	$v0,2
    78f0:	50620005 	0x50620005
    78f4:	24140002 	li	$s4,2
    78f8:	08001e45 	j	7914 <VpSetChannel+0x80c>
    78fc:	92020002 	lbu	$v0,2($s0)
    7900:	08001e44 	j	7910 <VpSetChannel+0x808>
    7904:	24140001 	li	$s4,1
    7908:	00131040 	sll	$v0,$s3,0x1
    790c:	3053ffff 	andi	$s3,$v0,0xffff
    7910:	92020002 	lbu	$v0,2($s0)
    7914:	30420001 	andi	$v0,$v0,0x1
    7918:	10400021 	beqz	$v0,79a0 <VpSetChannel+0x898>
    791c:	02002021 	move	$a0,$s0
    7920:	26110058 	addiu	$s1,$s0,88
    7924:	3c120000 	lui	$s2,0x0
    7928:	26527204 	addiu	$s2,$s2,29188
    792c:	0240f809 	jalr	$s2
    7930:	02202821 	move	$a1,$s1
    7934:	24030005 	li	$v1,5
    7938:	5443000b 	0x5443000b
    793c:	8e040044 	lw	$a0,68($s0)
    7940:	8e020044 	lw	$v0,68($s0)
    7944:	8c44000c 	lw	$a0,12($v0)
    7948:	3c020000 	lui	$v0,0x0
    794c:	24420000 	addiu	$v0,$v0,0
    7950:	0040f809 	jalr	$v0
    7954:	00000000 	nop
    7958:	02002021 	move	$a0,$s0
    795c:	0240f809 	jalr	$s2
    7960:	02202821 	move	$a1,$s1
    7964:	8e040044 	lw	$a0,68($s0)
    7968:	8e050074 	lw	$a1,116($s0)
    796c:	3c020000 	lui	$v0,0x0
    7970:	24420000 	addiu	$v0,$v0,0
    7974:	0040f809 	jalr	$v0
    7978:	8e060078 	lw	$a2,120($s0)
    797c:	ae000080 	sw	$zero,128($s0)
    7980:	ae000088 	sw	$zero,136($s0)
    7984:	ae00007c 	sw	$zero,124($s0)
    7988:	ae000084 	sw	$zero,132($s0)
    798c:	8e020078 	lw	$v0,120($s0)
    7990:	8e030074 	lw	$v1,116($s0)
    7994:	ae020070 	sw	$v0,112($s0)
    7998:	08001e87 	j	7a1c <VpSetChannel+0x914>
    799c:	ae03006c 	sw	$v1,108($s0)
    79a0:	3c110000 	lui	$s1,0x0
    79a4:	26310000 	addiu	$s1,$s1,0
    79a8:	8e040044 	lw	$a0,68($s0)
    79ac:	3c060000 	lui	$a2,0x0
    79b0:	8cc60530 	lw	$a2,1328($a2)
    79b4:	3c070000 	lui	$a3,0x0
    79b8:	8ce70534 	lw	$a3,1332($a3)
    79bc:	8c82000c 	lw	$v0,12($a0)
    79c0:	afa00010 	sw	$zero,16($sp)
    79c4:	8c420014 	lw	$v0,20($v0)
    79c8:	0040f809 	jalr	$v0
    79cc:	02202821 	move	$a1,$s1
    79d0:	24030005 	li	$v1,5
    79d4:	54430008 	0x54430008
    79d8:	8e040044 	lw	$a0,68($s0)
    79dc:	8e020044 	lw	$v0,68($s0)
    79e0:	8c44000c 	lw	$a0,12($v0)
    79e4:	3c020000 	lui	$v0,0x0
    79e8:	24420000 	addiu	$v0,$v0,0
    79ec:	0040f809 	jalr	$v0
    79f0:	00000000 	nop
    79f4:	8e040044 	lw	$a0,68($s0)
    79f8:	3c060000 	lui	$a2,0x0
    79fc:	8cc60530 	lw	$a2,1328($a2)
    7a00:	3c070000 	lui	$a3,0x0
    7a04:	8ce70534 	lw	$a3,1332($a3)
    7a08:	8c82000c 	lw	$v0,12($a0)
    7a0c:	afa00010 	sw	$zero,16($sp)
    7a10:	8c420014 	lw	$v0,20($v0)
    7a14:	0040f809 	jalr	$v0
    7a18:	02202821 	move	$a1,$s1
    7a1c:	8e040044 	lw	$a0,68($s0)
    7a20:	8c82000c 	lw	$v0,12($a0)
    7a24:	326500ff 	andi	$a1,$s3,0xff
    7a28:	8c42001c 	lw	$v0,28($v0)
    7a2c:	0040f809 	jalr	$v0
    7a30:	00a03021 	move	$a2,$a1
    7a34:	8e040044 	lw	$a0,68($s0)
    7a38:	8c82000c 	lw	$v0,12($a0)
    7a3c:	8c420020 	lw	$v0,32($v0)
    7a40:	0040f809 	jalr	$v0
    7a44:	02802821 	move	$a1,$s4
    7a48:	00002821 	move	$a1,$zero
    7a4c:	3c020000 	lui	$v0,0x0
    7a50:	244270d4 	addiu	$v0,$v0,28884
    7a54:	0040f809 	jalr	$v0
    7a58:	8e040044 	lw	$a0,68($s0)
    7a5c:	8fbf002c 	lw	$ra,44($sp)
    7a60:	8fb40028 	lw	$s4,40($sp)
    7a64:	8fb30024 	lw	$s3,36($sp)
    7a68:	8fb20020 	lw	$s2,32($sp)
    7a6c:	8fb1001c 	lw	$s1,28($sp)
    7a70:	8fb00018 	lw	$s0,24($sp)
    7a74:	24020001 	li	$v0,1
    7a78:	03e00008 	jr	$ra
    7a7c:	27bd0030 	addiu	$sp,$sp,48
    7a80:	27bdffe8 	addiu	$sp,$sp,-24
    7a84:	10a00006 	beqz	$a1,7aa0 <VpSetChannel+0x998>
    7a88:	afbf0010 	sw	$ra,16($sp)
    7a8c:	8c840044 	lw	$a0,68($a0)
    7a90:	3c020000 	lui	$v0,0x0
    7a94:	244270d4 	addiu	$v0,$v0,28884
    7a98:	0040f809 	jalr	$v0
    7a9c:	24050004 	li	$a1,4
    7aa0:	8fbf0010 	lw	$ra,16($sp)
    7aa4:	24020001 	li	$v0,1
    7aa8:	03e00008 	jr	$ra
    7aac:	27bd0018 	addiu	$sp,$sp,24
    7ab0:	27bdffe8 	addiu	$sp,$sp,-24
    7ab4:	10a00007 	beqz	$a1,7ad4 <VpSetChannel+0x9cc>
    7ab8:	afbf0010 	sw	$ra,16($sp)
    7abc:	3c020000 	lui	$v0,0x0
    7ac0:	244278a0 	addiu	$v0,$v0,30880
    7ac4:	0040f809 	jalr	$v0
    7ac8:	90850002 	lbu	$a1,2($a0)
    7acc:	08001eba 	j	7ae8 <VpSetChannel+0x9e0>
    7ad0:	8fbf0010 	lw	$ra,16($sp)
    7ad4:	8c820044 	lw	$v0,68($a0)
    7ad8:	8c42000c 	lw	$v0,12($v0)
    7adc:	8c42000c 	lw	$v0,12($v0)
    7ae0:	ac400018 	sw	$zero,24($v0)
    7ae4:	8fbf0010 	lw	$ra,16($sp)
    7ae8:	24020001 	li	$v0,1
    7aec:	03e00008 	jr	$ra
    7af0:	27bd0018 	addiu	$sp,$sp,24
    7af4:	8c820044 	lw	$v0,68($a0)
    7af8:	8c420008 	lw	$v0,8($v0)
    7afc:	8c430060 	lw	$v1,96($v0)
    7b00:	24020006 	li	$v0,6
    7b04:	14620003 	bne	$v1,$v0,7b14 <VpSetChannel+0xa0c>
    7b08:	24020001 	li	$v0,1
    7b0c:	03e00008 	jr	$ra
    7b10:	aca20000 	sw	$v0,0($a1)
    7b14:	03e00008 	jr	$ra
    7b18:	aca00000 	sw	$zero,0($a1)
    7b1c:	8c820044 	lw	$v0,68($a0)
    7b20:	8c420008 	lw	$v0,8($v0)
    7b24:	8c430060 	lw	$v1,96($v0)
    7b28:	24020003 	li	$v0,3
    7b2c:	14620003 	bne	$v1,$v0,7b3c <VpSetChannel+0xa34>
    7b30:	24020001 	li	$v0,1
    7b34:	03e00008 	jr	$ra
    7b38:	aca20000 	sw	$v0,0($a1)
    7b3c:	03e00008 	jr	$ra
    7b40:	aca00000 	sw	$zero,0($a1)
    7b44:	27bdffe8 	addiu	$sp,$sp,-24
    7b48:	24030001 	li	$v1,1
    7b4c:	14a30004 	bne	$a1,$v1,7b60 <VpSetChannel+0xa58>
    7b50:	afbf0010 	sw	$ra,16($sp)
    7b54:	8c840044 	lw	$a0,68($a0)
    7b58:	08001edd 	j	7b74 <VpSetChannel+0xa6c>
    7b5c:	2405000a 	li	$a1,10
    7b60:	8c840044 	lw	$a0,68($a0)
    7b64:	8c820008 	lw	$v0,8($a0)
    7b68:	8c42002c 	lw	$v0,44($v0)
    7b6c:	14430007 	bne	$v0,$v1,7b8c <VpSetChannel+0xa84>
    7b70:	00002821 	move	$a1,$zero
    7b74:	3c020000 	lui	$v0,0x0
    7b78:	244270d4 	addiu	$v0,$v0,28884
    7b7c:	0040f809 	jalr	$v0
    7b80:	00000000 	nop
    7b84:	08001ee8 	j	7ba0 <VpSetChannel+0xa98>
    7b88:	8fbf0010 	lw	$ra,16($sp)
    7b8c:	3c020000 	lui	$v0,0x0
    7b90:	244270d4 	addiu	$v0,$v0,28884
    7b94:	0040f809 	jalr	$v0
    7b98:	24050009 	li	$a1,9
    7b9c:	8fbf0010 	lw	$ra,16($sp)
    7ba0:	03e00008 	jr	$ra
    7ba4:	27bd0018 	addiu	$sp,$sp,24
    7ba8:	8c820044 	lw	$v0,68($a0)
    7bac:	8c420008 	lw	$v0,8($v0)
    7bb0:	90420033 	lbu	$v0,51($v0)
    7bb4:	10400003 	beqz	$v0,7bc4 <VpSetChannel+0xabc>
    7bb8:	24020001 	li	$v0,1
    7bbc:	03e00008 	jr	$ra
    7bc0:	aca20000 	sw	$v0,0($a1)
    7bc4:	03e00008 	jr	$ra
    7bc8:	aca00000 	sw	$zero,0($a1)
    7bcc:	27bdffe8 	addiu	$sp,$sp,-24
    7bd0:	afbf0010 	sw	$ra,16($sp)
    7bd4:	2ca50001 	sltiu	$a1,$a1,1
    7bd8:	00052840 	sll	$a1,$a1,0x1
    7bdc:	3c020000 	lui	$v0,0x0
    7be0:	244270d4 	addiu	$v0,$v0,28884
    7be4:	0040f809 	jalr	$v0
    7be8:	8c840044 	lw	$a0,68($a0)
    7bec:	8fbf0010 	lw	$ra,16($sp)
    7bf0:	03e00008 	jr	$ra
    7bf4:	27bd0018 	addiu	$sp,$sp,24
    7bf8:	27bdffe8 	addiu	$sp,$sp,-24
    7bfc:	24020001 	li	$v0,1
    7c00:	14a20006 	bne	$a1,$v0,7c1c <VpSetChannel+0xb14>
    7c04:	afbf0010 	sw	$ra,16($sp)
    7c08:	8c820004 	lw	$v0,4($a0)
    7c0c:	10400006 	beqz	$v0,7c28 <VpSetChannel+0xb20>
    7c10:	24050006 	li	$a1,6
    7c14:	08001f0a 	j	7c28 <VpSetChannel+0xb20>
    7c18:	24050007 	li	$a1,7
    7c1c:	8c820004 	lw	$v0,4($a0)
    7c20:	0002102b 	sltu	$v0,$zero,$v0
    7c24:	00022840 	sll	$a1,$v0,0x1
    7c28:	3c020000 	lui	$v0,0x0
    7c2c:	244270d4 	addiu	$v0,$v0,28884
    7c30:	0040f809 	jalr	$v0
    7c34:	8c840044 	lw	$a0,68($a0)
    7c38:	8fbf0010 	lw	$ra,16($sp)
    7c3c:	03e00008 	jr	$ra
    7c40:	27bd0018 	addiu	$sp,$sp,24
    7c44:	27bdffe8 	addiu	$sp,$sp,-24
    7c48:	afbf0010 	sw	$ra,16($sp)
    7c4c:	10a00002 	beqz	$a1,7c58 <VpSetChannel+0xb50>
    7c50:	00001021 	move	$v0,$zero
    7c54:	24020005 	li	$v0,5
    7c58:	00402821 	move	$a1,$v0
    7c5c:	3c020000 	lui	$v0,0x0
    7c60:	244270d4 	addiu	$v0,$v0,28884
    7c64:	0040f809 	jalr	$v0
    7c68:	8c840044 	lw	$a0,68($a0)
    7c6c:	8fbf0010 	lw	$ra,16($sp)
    7c70:	03e00008 	jr	$ra
    7c74:	27bd0018 	addiu	$sp,$sp,24
    7c78:	90830002 	lbu	$v1,2($a0)
    7c7c:	30630001 	andi	$v1,$v1,0x1
    7c80:	14600002 	bnez	$v1,7c8c <VpSetChannel+0xb84>
    7c84:	2402000c 	li	$v0,12
    7c88:	2402000d 	li	$v0,13
    7c8c:	03e00008 	jr	$ra
    7c90:	00000000 	nop
    7c94:	27bdffe0 	addiu	$sp,$sp,-32
    7c98:	afbf0018 	sw	$ra,24($sp)
    7c9c:	24050073 	li	$a1,115
    7ca0:	24060002 	li	$a2,2
    7ca4:	27a70010 	addiu	$a3,$sp,16
    7ca8:	3c020000 	lui	$v0,0x0
    7cac:	24426fc4 	addiu	$v0,$v0,28612
    7cb0:	0040f809 	jalr	$v0
    7cb4:	90840000 	lbu	$a0,0($a0)
    7cb8:	93a30010 	lbu	$v1,16($sp)
    7cbc:	24020001 	li	$v0,1
    7cc0:	14620015 	bne	$v1,$v0,7d18 <VpSetChannel+0xc10>
    7cc4:	93a30011 	lbu	$v1,17($sp)
    7cc8:	240200b6 	li	$v0,182
    7ccc:	14620006 	bne	$v1,$v0,7ce8 <VpSetChannel+0xbe0>
    7cd0:	240200b2 	li	$v0,178
    7cd4:	3c040000 	lui	$a0,0x0
    7cd8:	24840090 	addiu	$a0,$a0,144
    7cdc:	3c050000 	lui	$a1,0x0
    7ce0:	08001f40 	j	7d00 <VpSetChannel+0xbf8>
    7ce4:	24a51e0c 	addiu	$a1,$a1,7692
    7ce8:	1462000b 	bne	$v1,$v0,7d18 <VpSetChannel+0xc10>
    7cec:	00000000 	nop
    7cf0:	3c040000 	lui	$a0,0x0
    7cf4:	24840090 	addiu	$a0,$a0,144
    7cf8:	3c050000 	lui	$a1,0x0
    7cfc:	24a51e20 	addiu	$a1,$a1,7712
    7d00:	3c020000 	lui	$v0,0x0
    7d04:	24420000 	addiu	$v0,$v0,0
    7d08:	0040f809 	jalr	$v0
    7d0c:	93a60010 	lbu	$a2,16($sp)
    7d10:	08001f50 	j	7d40 <VpSetChannel+0xc38>
    7d14:	8fbf0018 	lw	$ra,24($sp)
    7d18:	3c040000 	lui	$a0,0x0
    7d1c:	24840090 	addiu	$a0,$a0,144
    7d20:	3c050000 	lui	$a1,0x0
    7d24:	24a51e34 	addiu	$a1,$a1,7732
    7d28:	93a60010 	lbu	$a2,16($sp)
    7d2c:	3c020000 	lui	$v0,0x0
    7d30:	24420000 	addiu	$v0,$v0,0
    7d34:	0040f809 	jalr	$v0
    7d38:	93a70011 	lbu	$a3,17($sp)
    7d3c:	8fbf0018 	lw	$ra,24($sp)
    7d40:	3c020000 	lui	$v0,0x0
    7d44:	24420090 	addiu	$v0,$v0,144
    7d48:	03e00008 	jr	$ra
    7d4c:	27bd0020 	addiu	$sp,$sp,32

0000000000007d50 <VpMakeDeviceObject>:
    7d50:	27bdffe8 	addiu	$sp,$sp,-24
    7d54:	afbf0010 	sw	$ra,16($sp)
    7d58:	00c04021 	move	$t0,$a2
    7d5c:	30e600ff 	andi	$a2,$a3,0xff
    7d60:	00001821 	move	$v1,$zero
    7d64:	24620001 	addiu	$v0,$v1,1
    7d68:	304300ff 	andi	$v1,$v0,0xff
    7d6c:	2c620020 	sltiu	$v0,$v1,32
    7d70:	1440fffd 	bnez	$v0,7d68 <VpMakeDeviceObject+0x18>
    7d74:	24620001 	addiu	$v0,$v1,1
    7d78:	24020002 	li	$v0,2
    7d7c:	11020003 	beq	$t0,$v0,7d8c <VpMakeDeviceObject+0x3c>
    7d80:	ac880004 	sw	$t0,4($a0)
    7d84:	08001f67 	j	7d9c <VpMakeDeviceObject+0x4c>
    7d88:	24020009 	li	$v0,9
    7d8c:	3c020000 	lui	$v0,0x0
    7d90:	24426ed0 	addiu	$v0,$v0,28368
    7d94:	0040f809 	jalr	$v0
    7d98:	00000000 	nop
    7d9c:	8fbf0010 	lw	$ra,16($sp)
    7da0:	03e00008 	jr	$ra
    7da4:	27bd0018 	addiu	$sp,$sp,24
*/

int vp880_abs_api_init(void)
{
	void *apipointer = NULL;
/* prologue
0000000000007da8 <vp880_abs_api_init>:
    7da8:	27bdffe8 	addiu	$sp,$sp,-24
    7dac:	afbf0010 	sw	$ra,16($sp)
*/
/*
    7db0:	3c030000 	lui	$v1,0x0
    7db4:	8c630538 	lw	$v1,1336($v1)
    7db8:	24020001 	li	$v0,1
    7dbc:	1462000a 	bne	$v1,$v0,7de8 <vp880_abs_api_init+0x40>
    7dc0:	8fbf0010 	lw	$ra,16($sp)
    7dc4:	3c010000 	lui	$at,0x0
    7dc8:	ac200538 	sw	$zero,1336($at)
    7dcc:	3c040000 	lui	$a0,0x0
    7dd0:	2484053c 	addiu	$a0,$a0,1340
    7dd4:	3c020000 	lui	$v0,0x0
    7dd8:	24420000 	addiu	$v0,$v0,0
    7ddc:	0040f809 	jalr	$v0
    7de0:	00000000 	nop
*/
	apipointer = globalApiPointer;
	tiuo_fill_drv_api(apipointer);
/*
    7de4:	8fbf0010 	lw	$ra,16($sp)
    7de8:	03e00008 	jr	$ra
    7dec:	27bd0018 	addiu	$sp,$sp,24
    7df0:	27bdffd8 	addiu	$sp,$sp,-40
    7df4:	afbf0024 	sw	$ra,36($sp)
    7df8:	afb20020 	sw	$s2,32($sp)
    7dfc:	afb1001c 	sw	$s1,28($sp)
    7e00:	afb00018 	sw	$s0,24($sp)
    7e04:	00c09021 	move	$s2,$a2
    7e08:	3c020000 	lui	$v0,0x0
    7e0c:	8c4205b0 	lw	$v0,1456($v0)
    7e10:	1440000e 	bnez	$v0,7e4c <vp880_abs_api_init+0xa4>
    7e14:	30b1ffff 	andi	$s1,$a1,0xffff
    7e18:	24020001 	li	$v0,1
    7e1c:	3c010000 	lui	$at,0x0
    7e20:	ac2205b0 	sw	$v0,1456($at)
    7e24:	3c030000 	lui	$v1,0x0
    7e28:	246305c8 	addiu	$v1,$v1,1480
    7e2c:	ac620004 	sw	$v0,4($v1)
    7e30:	ac600000 	sw	$zero,0($v1)
    7e34:	3c010000 	lui	$at,0x0
    7e38:	ac2005d0 	sw	$zero,1488($at)
    7e3c:	24620008 	addiu	$v0,$v1,8
    7e40:	2463000c 	addiu	$v1,$v1,12
    7e44:	ac430004 	sw	$v1,4($v0)
    7e48:	ac430008 	sw	$v1,8($v0)
    7e4c:	3c020000 	lui	$v0,0x0
    7e50:	24420000 	addiu	$v0,$v0,0
    7e54:	0040f809 	jalr	$v0
    7e58:	00000000 	nop
    7e5c:	24040198 	li	$a0,408
    7e60:	240501f2 	li	$a1,498
    7e64:	3c020000 	lui	$v0,0x0
    7e68:	24420000 	addiu	$v0,$v0,0
    7e6c:	0040f809 	jalr	$v0
    7e70:	2406079f 	li	$a2,1951
    7e74:	00408021 	move	$s0,$v0
    7e78:	12000029 	beqz	$s0,7f20 <vp880_abs_api_init+0x178>
    7e7c:	ae500000 	sw	$s0,0($s2)
    7e80:	02002021 	move	$a0,$s0
    7e84:	26050030 	addiu	$a1,$s0,48
    7e88:	24060002 	li	$a2,2
    7e8c:	3c020000 	lui	$v0,0x0
    7e90:	24420000 	addiu	$v0,$v0,0
    7e94:	0040f809 	jalr	$v0
    7e98:	322700ff 	andi	$a3,$s1,0xff
    7e9c:	10400004 	beqz	$v0,7eb0 <vp880_abs_api_init+0x108>
    7ea0:	00402821 	move	$a1,$v0
    7ea4:	3c040000 	lui	$a0,0x0
    7ea8:	08001fc4 	j	7f10 <vp880_abs_api_init+0x168>
    7eac:	24841f18 	addiu	$a0,$a0,7960
    7eb0:	24120005 	li	$s2,5
    7eb4:	afb20010 	sw	$s2,16($sp)
    7eb8:	26040078 	addiu	$a0,$s0,120
    7ebc:	26050098 	addiu	$a1,$s0,152
    7ec0:	02003021 	move	$a2,$s0
    7ec4:	3c110000 	lui	$s1,0x0
    7ec8:	26310000 	addiu	$s1,$s1,0
    7ecc:	0220f809 	jalr	$s1
    7ed0:	00003821 	move	$a3,$zero
    7ed4:	10400004 	beqz	$v0,7ee8 <vp880_abs_api_init+0x140>
    7ed8:	00402821 	move	$a1,$v0
    7edc:	3c040000 	lui	$a0,0x0
    7ee0:	08001fc4 	j	7f10 <vp880_abs_api_init+0x168>
    7ee4:	24841f58 	addiu	$a0,$a0,8024
    7ee8:	afb20010 	sw	$s2,16($sp)
    7eec:	26040088 	addiu	$a0,$s0,136
    7ef0:	26050118 	addiu	$a1,$s0,280
    7ef4:	02003021 	move	$a2,$s0
    7ef8:	0220f809 	jalr	$s1
    7efc:	24070001 	li	$a3,1
    7f00:	10400009 	beqz	$v0,7f28 <vp880_abs_api_init+0x180>
    7f04:	00402821 	move	$a1,$v0
    7f08:	3c040000 	lui	$a0,0x0
    7f0c:	24841f98 	addiu	$a0,$a0,8088
    7f10:	3c020000 	lui	$v0,0x0
    7f14:	24420000 	addiu	$v0,$v0,0
    7f18:	0040f809 	jalr	$v0
    7f1c:	00000000 	nop
    7f20:	08001fcb 	j	7f2c <vp880_abs_api_init+0x184>
    7f24:	00001021 	move	$v0,$zero
    7f28:	24020001 	li	$v0,1
    7f2c:	8fbf0024 	lw	$ra,36($sp)
    7f30:	8fb20020 	lw	$s2,32($sp)
    7f34:	8fb1001c 	lw	$s1,28($sp)
    7f38:	8fb00018 	lw	$s0,24($sp)
    7f3c:	03e00008 	jr	$ra
    7f40:	27bd0028 	addiu	$sp,$sp,40
    7f44:	27bdffe8 	addiu	$sp,$sp,-24
    7f48:	afbf0014 	sw	$ra,20($sp)
    7f4c:	afb00010 	sw	$s0,16($sp)
    7f50:	00a08021 	move	$s0,$a1
    7f54:	8e040000 	lw	$a0,0($s0)
    7f58:	10800007 	beqz	$a0,7f78 <vp880_abs_api_init+0x1d0>
    7f5c:	8fbf0014 	lw	$ra,20($sp)
    7f60:	3c020000 	lui	$v0,0x0
    7f64:	24420000 	addiu	$v0,$v0,0
    7f68:	0040f809 	jalr	$v0
    7f6c:	00000000 	nop
    7f70:	ae000000 	sw	$zero,0($s0)
    7f74:	8fbf0014 	lw	$ra,20($sp)
    7f78:	8fb00010 	lw	$s0,16($sp)
    7f7c:	03e00008 	jr	$ra
    7f80:	27bd0018 	addiu	$sp,$sp,24
    7f84:	03e00008 	jr	$ra
    7f88:	24020002 	li	$v0,2
    7f8c:	03e00008 	jr	$ra
    7f90:	a0c00000 	sb	$zero,0($a2)
	...
    7fa0:	27bdffe8 	addiu	$sp,$sp,-24
    7fa4:	afbf0010 	sw	$ra,16($sp)
    7fa8:	3c020000 	lui	$v0,0x0
    7fac:	24420000 	addiu	$v0,$v0,0
    7fb0:	0040f809 	jalr	$v0
    7fb4:	00000000 	nop
    7fb8:	8fbf0010 	lw	$ra,16($sp)
    7fbc:	24020001 	li	$v0,1
    7fc0:	03e00008 	jr	$ra
    7fc4:	27bd0018 	addiu	$sp,$sp,24
*/
}


/*
0000000000007fc8 <tid_teardown>:
    7fc8:	03e00008 	jr	$ra
    7fcc:	aca00000 	sw	$zero,0($a1)
    7fd0:	03e00008 	jr	$ra
    7fd4:	24020002 	li	$v0,2
    7fd8:	27bdffe8 	addiu	$sp,$sp,-24
    7fdc:	afbf0010 	sw	$ra,16($sp)
    7fe0:	24021001 	li	$v0,4097
    7fe4:	14a20007 	bne	$a1,$v0,8004 <tid_teardown+0x3c>
    7fe8:	00801821 	move	$v1,$a0
    7fec:	00002021 	move	$a0,$zero
    7ff0:	24050080 	li	$a1,128
    7ff4:	3c020000 	lui	$v0,0x0
    7ff8:	24420000 	addiu	$v0,$v0,0
    7ffc:	0040f809 	jalr	$v0
    8000:	94660000 	lhu	$a2,0($v1)
    8004:	8fbf0010 	lw	$ra,16($sp)
    8008:	03e00008 	jr	$ra
    800c:	27bd0018 	addiu	$sp,$sp,24
    8010:	27bdffe8 	addiu	$sp,$sp,-24
    8014:	afbf0010 	sw	$ra,16($sp)
    8018:	3c020000 	lui	$v0,0x0
    801c:	24420000 	addiu	$v0,$v0,0
    8020:	0040f809 	jalr	$v0
    8024:	00000000 	nop
    8028:	8fbf0010 	lw	$ra,16($sp)
    802c:	24020001 	li	$v0,1
    8030:	03e00008 	jr	$ra
    8034:	27bd0018 	addiu	$sp,$sp,24
    8038:	03e00008 	jr	$ra
    803c:	aca00000 	sw	$zero,0($a1)
    8040:	03e00008 	jr	$ra
    8044:	24020002 	li	$v0,2
    8048:	03e00008 	jr	$ra
    804c:	00000000 	nop
    8050:	27bdffe0 	addiu	$sp,$sp,-32
    8054:	afbf0018 	sw	$ra,24($sp)
    8058:	afb10014 	sw	$s1,20($sp)
    805c:	afb00010 	sw	$s0,16($sp)
    8060:	00c08021 	move	$s0,$a2
    8064:	3c020000 	lui	$v0,0x0
    8068:	24420000 	addiu	$v0,$v0,0
    806c:	0040f809 	jalr	$v0
    8070:	30b1ffff 	andi	$s1,$a1,0xffff
    8074:	24040004 	li	$a0,4
    8078:	240501f2 	li	$a1,498
    807c:	3c020000 	lui	$v0,0x0
    8080:	24420000 	addiu	$v0,$v0,0
    8084:	0040f809 	jalr	$v0
    8088:	2406079f 	li	$a2,1951
    808c:	ae020000 	sw	$v0,0($s0)
    8090:	ac510000 	sw	$s1,0($v0)
    8094:	8fbf0018 	lw	$ra,24($sp)
    8098:	8fb10014 	lw	$s1,20($sp)
    809c:	8fb00010 	lw	$s0,16($sp)
    80a0:	24020001 	li	$v0,1
    80a4:	03e00008 	jr	$ra
    80a8:	27bd0020 	addiu	$sp,$sp,32
    80ac:	27bdffe8 	addiu	$sp,$sp,-24
    80b0:	afbf0014 	sw	$ra,20($sp)
    80b4:	afb00010 	sw	$s0,16($sp)
    80b8:	00a08021 	move	$s0,$a1
    80bc:	1200000a 	beqz	$s0,80e8 <tid_teardown+0x120>
    80c0:	8fbf0014 	lw	$ra,20($sp)
    80c4:	8e040000 	lw	$a0,0($s0)
    80c8:	50800008 	0x50800008
    80cc:	8fb00010 	lw	$s0,16($sp)
    80d0:	3c020000 	lui	$v0,0x0
    80d4:	24420000 	addiu	$v0,$v0,0
    80d8:	0040f809 	jalr	$v0
    80dc:	00000000 	nop
    80e0:	ae000000 	sw	$zero,0($s0)
    80e4:	8fbf0014 	lw	$ra,20($sp)
    80e8:	8fb00010 	lw	$s0,16($sp)
    80ec:	03e00008 	jr	$ra
    80f0:	27bd0018 	addiu	$sp,$sp,24
    80f4:	03e00008 	jr	$ra
    80f8:	24020002 	li	$v0,2
    80fc:	27bdffe8 	addiu	$sp,$sp,-24
    8100:	24021001 	li	$v0,4097
    8104:	14a20006 	bne	$a1,$v0,8120 <tid_teardown+0x158>
    8108:	afbf0010 	sw	$ra,16($sp)
    810c:	94840000 	lhu	$a0,0($a0)
    8110:	3c020000 	lui	$v0,0x0
    8114:	24420000 	addiu	$v0,$v0,0
    8118:	0040f809 	jalr	$v0
    811c:	24050001 	li	$a1,1
    8120:	8fbf0010 	lw	$ra,16($sp)
    8124:	03e00008 	jr	$ra
    8128:	27bd0018 	addiu	$sp,$sp,24
    812c:	00000000 	nop
    8130:	03e00008 	jr	$ra
    8134:	24020001 	li	$v0,1
    8138:	03e00008 	jr	$ra
    813c:	00000000 	nop
    8140:	54c00001 	0x54c00001
    8144:	a0c00000 	sb	$zero,0($a2)
    8148:	03e00008 	jr	$ra
    814c:	00000000 	nop
    8150:	03e00008 	jr	$ra
    8154:	24020001 	li	$v0,1
    8158:	03e00008 	jr	$ra
    815c:	24020001 	li	$v0,1
    8160:	03e00008 	jr	$ra
    8164:	24020001 	li	$v0,1
    8168:	03e00008 	jr	$ra
    816c:	24020001 	li	$v0,1
    8170:	54a00001 	0x54a00001
    8174:	aca00000 	sw	$zero,0($a1)
    8178:	03e00008 	jr	$ra
    817c:	00000000 	nop
    8180:	54a00001 	0x54a00001
    8184:	aca00000 	sw	$zero,0($a1)
    8188:	03e00008 	jr	$ra
    818c:	00000000 	nop
    8190:	54a00001 	0x54a00001
    8194:	aca00000 	sw	$zero,0($a1)
    8198:	03e00008 	jr	$ra
    819c:	00000000 	nop
    81a0:	54a00001 	0x54a00001
    81a4:	aca00000 	sw	$zero,0($a1)
    81a8:	03e00008 	jr	$ra
    81ac:	00000000 	nop
    81b0:	03e00008 	jr	$ra
    81b4:	00000000 	nop
    81b8:	03e00008 	jr	$ra
    81bc:	00000000 	nop
    81c0:	03e00008 	jr	$ra
    81c4:	00000000 	nop
    81c8:	54a00001 	0x54a00001
    81cc:	aca00000 	sw	$zero,0($a1)
    81d0:	03e00008 	jr	$ra
    81d4:	00000000 	nop
    81d8:	03e00008 	jr	$ra
    81dc:	00000000 	nop
    81e0:	03e00008 	jr	$ra
    81e4:	00000000 	nop
    81e8:	03e00008 	jr	$ra
    81ec:	00000000 	nop
    81f0:	03e00008 	jr	$ra
    81f4:	00000000 	nop
    81f8:	03e00008 	jr	$ra
    81fc:	24020001 	li	$v0,1
    8200:	03e00008 	jr	$ra
    8204:	24020001 	li	$v0,1
    8208:	03e00008 	jr	$ra
    820c:	24020001 	li	$v0,1
    8210:	03e00008 	jr	$ra
    8214:	24020001 	li	$v0,1
    8218:	03e00008 	jr	$ra
    821c:	00000000 	nop
    8220:	03e00008 	jr	$ra
    8224:	00000000 	nop
    8228:	03e00008 	jr	$ra
    822c:	00000000 	nop
    8230:	3c020000 	lui	$v0,0x0
    8234:	03e00008 	jr	$ra
    8238:	90420610 	lbu	$v0,1552($v0)
    823c:	3c020000 	lui	$v0,0x0
    8240:	03e00008 	jr	$ra
    8244:	24422318 	addiu	$v0,$v0,8984
    8248:	acc00000 	sw	$zero,0($a2)
    824c:	03e00008 	jr	$ra
    8250:	24020001 	li	$v0,1
    8254:	54a00001 	0x54a00001
    8258:	aca00000 	sw	$zero,0($a1)
    825c:	03e00008 	jr	$ra
    8260:	00000000 	nop
    8264:	03e00008 	jr	$ra
    8268:	24020002 	li	$v0,2
    826c:	54c00001 	0x54c00001
    8270:	a0c00000 	sb	$zero,0($a2)
    8274:	03e00008 	jr	$ra
    8278:	00000000 	nop
*/

int tiuo_fill_drv_api(void *apipointer)
{
/* prologue
000000000000827c <tiuo_fill_drv_api>:
    827c:	8c820000 	lw	$v0,0($a0)
    8280:	54400005 	0x54400005
    8284:	8c820004 	lw	$v0,4($a0)
    8288:	3c020000 	lui	$v0,0x0
    828c:	8c420614 	lw	$v0,1556($v0)
    8290:	ac820000 	sw	$v0,0($a0)
    8294:	8c820004 	lw	$v0,4($a0)
    8298:	54400005 	0x54400005
    829c:	8c820008 	lw	$v0,8($a0)
    82a0:	3c020000 	lui	$v0,0x0
    82a4:	8c420618 	lw	$v0,1560($v0)
    82a8:	ac820004 	sw	$v0,4($a0)
    82ac:	8c820008 	lw	$v0,8($a0)
    82b0:	54400005 	0x54400005
    82b4:	8c82000c 	lw	$v0,12($a0)
    82b8:	3c020000 	lui	$v0,0x0
    82bc:	8c42061c 	lw	$v0,1564($v0)
    82c0:	ac820008 	sw	$v0,8($a0)
    82c4:	8c82000c 	lw	$v0,12($a0)
    82c8:	54400005 	0x54400005
    82cc:	8c820010 	lw	$v0,16($a0)
    82d0:	3c020000 	lui	$v0,0x0
    82d4:	8c420620 	lw	$v0,1568($v0)
    82d8:	ac82000c 	sw	$v0,12($a0)
    82dc:	8c820010 	lw	$v0,16($a0)
    82e0:	54400005 	0x54400005
    82e4:	8c820014 	lw	$v0,20($a0)
    82e8:	3c020000 	lui	$v0,0x0
    82ec:	8c420624 	lw	$v0,1572($v0)
    82f0:	ac820010 	sw	$v0,16($a0)
    82f4:	8c820014 	lw	$v0,20($a0)
    82f8:	54400005 	0x54400005
    82fc:	8c820018 	lw	$v0,24($a0)
    8300:	3c020000 	lui	$v0,0x0
    8304:	8c420628 	lw	$v0,1576($v0)
    8308:	ac820014 	sw	$v0,20($a0)
    830c:	8c820018 	lw	$v0,24($a0)
    8310:	54400005 	0x54400005
    8314:	8c82001c 	lw	$v0,28($a0)
    8318:	3c020000 	lui	$v0,0x0
    831c:	8c42062c 	lw	$v0,1580($v0)
    8320:	ac820018 	sw	$v0,24($a0)
    8324:	8c82001c 	lw	$v0,28($a0)
    8328:	54400005 	0x54400005
    832c:	8c820020 	lw	$v0,32($a0)
    8330:	3c020000 	lui	$v0,0x0
    8334:	8c420630 	lw	$v0,1584($v0)
    8338:	ac82001c 	sw	$v0,28($a0)
    833c:	8c820020 	lw	$v0,32($a0)
    8340:	54400005 	0x54400005
    8344:	8c820024 	lw	$v0,36($a0)
    8348:	3c020000 	lui	$v0,0x0
    834c:	8c420634 	lw	$v0,1588($v0)
    8350:	ac820020 	sw	$v0,32($a0)
    8354:	8c820024 	lw	$v0,36($a0)
    8358:	54400005 	0x54400005
    835c:	8c820028 	lw	$v0,40($a0)
    8360:	3c020000 	lui	$v0,0x0
    8364:	8c420638 	lw	$v0,1592($v0)
    8368:	ac820024 	sw	$v0,36($a0)
    836c:	8c820028 	lw	$v0,40($a0)
    8370:	54400005 	0x54400005
    8374:	8c82002c 	lw	$v0,44($a0)
    8378:	3c020000 	lui	$v0,0x0
    837c:	8c42063c 	lw	$v0,1596($v0)
    8380:	ac820028 	sw	$v0,40($a0)
    8384:	8c82002c 	lw	$v0,44($a0)
    8388:	54400005 	0x54400005
    838c:	8c820030 	lw	$v0,48($a0)
    8390:	3c020000 	lui	$v0,0x0
    8394:	8c420640 	lw	$v0,1600($v0)
    8398:	ac82002c 	sw	$v0,44($a0)
    839c:	8c820030 	lw	$v0,48($a0)
    83a0:	54400005 	0x54400005
    83a4:	8c820034 	lw	$v0,52($a0)
    83a8:	3c020000 	lui	$v0,0x0
    83ac:	8c420644 	lw	$v0,1604($v0)
    83b0:	ac820030 	sw	$v0,48($a0)
    83b4:	8c820034 	lw	$v0,52($a0)
    83b8:	54400005 	0x54400005
    83bc:	8c820038 	lw	$v0,56($a0)
    83c0:	3c020000 	lui	$v0,0x0
    83c4:	8c420648 	lw	$v0,1608($v0)
    83c8:	ac820034 	sw	$v0,52($a0)
    83cc:	8c820038 	lw	$v0,56($a0)
    83d0:	54400005 	0x54400005
    83d4:	8c82003c 	lw	$v0,60($a0)
    83d8:	3c020000 	lui	$v0,0x0
    83dc:	8c42064c 	lw	$v0,1612($v0)
    83e0:	ac820038 	sw	$v0,56($a0)
    83e4:	8c82003c 	lw	$v0,60($a0)
    83e8:	54400005 	0x54400005
    83ec:	8c820040 	lw	$v0,64($a0)
    83f0:	3c020000 	lui	$v0,0x0
    83f4:	8c420650 	lw	$v0,1616($v0)
    83f8:	ac82003c 	sw	$v0,60($a0)
    83fc:	8c820040 	lw	$v0,64($a0)
    8400:	54400005 	0x54400005
    8404:	8c820044 	lw	$v0,68($a0)
    8408:	3c020000 	lui	$v0,0x0
    840c:	8c420654 	lw	$v0,1620($v0)
    8410:	ac820040 	sw	$v0,64($a0)
    8414:	8c820044 	lw	$v0,68($a0)
    8418:	54400005 	0x54400005
    841c:	8c820048 	lw	$v0,72($a0)
    8420:	3c020000 	lui	$v0,0x0
    8424:	8c420658 	lw	$v0,1624($v0)
    8428:	ac820044 	sw	$v0,68($a0)
    842c:	8c820048 	lw	$v0,72($a0)
    8430:	54400005 	0x54400005
    8434:	8c82004c 	lw	$v0,76($a0)
    8438:	3c020000 	lui	$v0,0x0
    843c:	8c42065c 	lw	$v0,1628($v0)
    8440:	ac820048 	sw	$v0,72($a0)
    8444:	8c82004c 	lw	$v0,76($a0)
    8448:	54400005 	0x54400005
    844c:	8c820050 	lw	$v0,80($a0)
    8450:	3c020000 	lui	$v0,0x0
    8454:	8c420660 	lw	$v0,1632($v0)
    8458:	ac82004c 	sw	$v0,76($a0)
    845c:	8c820050 	lw	$v0,80($a0)
    8460:	54400005 	0x54400005
    8464:	8c820054 	lw	$v0,84($a0)
    8468:	3c020000 	lui	$v0,0x0
    846c:	8c420664 	lw	$v0,1636($v0)
    8470:	ac820050 	sw	$v0,80($a0)
    8474:	8c820054 	lw	$v0,84($a0)
    8478:	54400005 	0x54400005
    847c:	8c820058 	lw	$v0,88($a0)
    8480:	3c020000 	lui	$v0,0x0
    8484:	8c420668 	lw	$v0,1640($v0)
    8488:	ac820054 	sw	$v0,84($a0)
    848c:	8c820058 	lw	$v0,88($a0)
    8490:	54400005 	0x54400005
    8494:	8c82005c 	lw	$v0,92($a0)
    8498:	3c020000 	lui	$v0,0x0
    849c:	8c42066c 	lw	$v0,1644($v0)
    84a0:	ac820058 	sw	$v0,88($a0)
    84a4:	8c82005c 	lw	$v0,92($a0)
    84a8:	54400005 	0x54400005
    84ac:	8c820064 	lw	$v0,100($a0)
    84b0:	3c020000 	lui	$v0,0x0
    84b4:	8c420670 	lw	$v0,1648($v0)
    84b8:	ac82005c 	sw	$v0,92($a0)
    84bc:	8c820064 	lw	$v0,100($a0)
    84c0:	54400005 	0x54400005
    84c4:	8c820068 	lw	$v0,104($a0)
    84c8:	3c020000 	lui	$v0,0x0
    84cc:	8c420678 	lw	$v0,1656($v0)
    84d0:	ac820064 	sw	$v0,100($a0)
    84d4:	8c820068 	lw	$v0,104($a0)
    84d8:	54400005 	0x54400005
    84dc:	8c82006c 	lw	$v0,108($a0)
    84e0:	3c020000 	lui	$v0,0x0
    84e4:	8c42067c 	lw	$v0,1660($v0)
    84e8:	ac820068 	sw	$v0,104($a0)
    84ec:	8c82006c 	lw	$v0,108($a0)
    84f0:	14400004 	bnez	$v0,8504 <tiuo_fill_drv_api+0x288>
    84f4:	00000000 	nop
    84f8:	3c020000 	lui	$v0,0x0
    84fc:	8c420680 	lw	$v0,1664($v0)
    8500:	ac82006c 	sw	$v0,108($a0)
    8504:	03e00008 	jr	$ra
	...
*/

}

module_init(tiuhw_an_init_module);
module_exit(tiuhw_an_cleanup_module);

MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Wlodzimierz Kalawski <wlk at poczta.fm>");
MODULE_LICENSE("GPL v2");


