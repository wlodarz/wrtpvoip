
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <asm/mach-ar7/ar7.h>


/*
../../../wrtp300-rootfs/lib/modules/2.4.17_mvl21-malta-mips_fp_le/kernel/drivers/mxpmod.o:     file format elf32-tradlittlemips

Disassembly of section .text:

0000000000000000 <tcb_by_name>:
       0:	27bdffe0 	addiu	$sp,$sp,-32
       4:	afbf001c 	sw	$ra,28($sp)
       8:	afb20018 	sw	$s2,24($sp)
       c:	afb10014 	sw	$s1,20($sp)
      10:	afb00010 	sw	$s0,16($sp)
      14:	00809021 	move	$s2,$a0
      18:	24100001 	li	$s0,1
      1c:	24110030 	li	$s1,48
      20:	3c020000 	lui	$v0,0x0
      24:	8c4200c8 	lw	$v0,200($v0)
      28:	02222021 	addu	$a0,$s1,$v0
      2c:	8c820000 	lw	$v0,0($a0)
      30:	10400009 	beqz	$v0,58 <tcb_by_name+0x58>
      34:	2484001c 	addiu	$a0,$a0,28
      38:	3c020000 	lui	$v0,0x0
      3c:	24422a24 	addiu	$v0,$v0,10788
      40:	0040f809 	jalr	$v0
      44:	02402821 	move	$a1,$s2
      48:	54400004 	0x54400004
      4c:	26100001 	addiu	$s0,$s0,1
      50:	0800001b 	j	6c <tcb_by_name+0x6c>
      54:	02001021 	move	$v0,$s0
      58:	26100001 	addiu	$s0,$s0,1
      5c:	2a020080 	slti	$v0,$s0,128
      60:	1440ffef 	bnez	$v0,20 <tcb_by_name+0x20>
      64:	26310030 	addiu	$s1,$s1,48
      68:	2402ffff 	li	$v0,-1
      6c:	8fbf001c 	lw	$ra,28($sp)
      70:	8fb20018 	lw	$s2,24($sp)
      74:	8fb10014 	lw	$s1,20($sp)
      78:	8fb00010 	lw	$s0,16($sp)
      7c:	03e00008 	jr	$ra
      80:	27bd0020 	addiu	$sp,$sp,32

0000000000000084 <mxp_tcb_identify>:
      84:	27bdffe0 	addiu	$sp,$sp,-32
      88:	afbf0018 	sw	$ra,24($sp)
      8c:	afb10014 	sw	$s1,20($sp)
      90:	afb00010 	sw	$s0,16($sp)
      94:	00808021 	move	$s0,$a0
      98:	40116000 	mfc0	$s1,$12
      9c:	00000000 	nop
      a0:	36210001 	ori	$at,$s1,0x1
      a4:	38210001 	xori	$at,$at,0x1
      a8:	40816000 	mtc0	$at,$12
      ac:	00000040 	sll	$zero,$zero,0x1
      b0:	00000040 	sll	$zero,$zero,0x1
      b4:	00000040 	sll	$zero,$zero,0x1
      b8:	3c020000 	lui	$v0,0x0
      bc:	24420000 	addiu	$v0,$v0,0
      c0:	0040f809 	jalr	$v0
      c4:	26040004 	addiu	$a0,$s0,4
      c8:	2403ffff 	li	$v1,-1
      cc:	1043000c 	beq	$v0,$v1,100 <mxp_tcb_identify+0x7c>
      d0:	ae02001c 	sw	$v0,28($s0)
      d4:	40016000 	mfc0	$at,$12
      d8:	32310001 	andi	$s1,$s1,0x1
      dc:	34210001 	ori	$at,$at,0x1
      e0:	38210001 	xori	$at,$at,0x1
      e4:	02218825 	or	$s1,$s1,$at
      e8:	40916000 	mtc0	$s1,$12
	...
      f8:	0800004a 	j	128 <mxp_tcb_identify+0xa4>
      fc:	00001021 	move	$v0,$zero
     100:	40016000 	mfc0	$at,$12
     104:	32310001 	andi	$s1,$s1,0x1
     108:	34210001 	ori	$at,$at,0x1
     10c:	38210001 	xori	$at,$at,0x1
     110:	02218825 	or	$s1,$s1,$at
     114:	40916000 	mtc0	$s1,$12
	...
     124:	24020001 	li	$v0,1
     128:	8fbf0018 	lw	$ra,24($sp)
     12c:	8fb10014 	lw	$s1,20($sp)
     130:	8fb00010 	lw	$s0,16($sp)
     134:	03e00008 	jr	$ra
     138:	27bd0020 	addiu	$sp,$sp,32

000000000000013c <mxp_tcb_alloc>:
     13c:	27bdffe0 	addiu	$sp,$sp,-32
     140:	afbf001c 	sw	$ra,28($sp)
     144:	afb20018 	sw	$s2,24($sp)
     148:	afb10014 	sw	$s1,20($sp)
     14c:	afb00010 	sw	$s0,16($sp)
     150:	00809021 	move	$s2,$a0
     154:	40106000 	mfc0	$s0,$12
     158:	00000000 	nop
     15c:	36010001 	ori	$at,$s0,0x1
     160:	38210001 	xori	$at,$at,0x1
     164:	40816000 	mtc0	$at,$12
     168:	00000040 	sll	$zero,$zero,0x1
     16c:	00000040 	sll	$zero,$zero,0x1
     170:	00000040 	sll	$zero,$zero,0x1
     174:	26510004 	addiu	$s1,$s2,4
     178:	3c020000 	lui	$v0,0x0
     17c:	24420000 	addiu	$v0,$v0,0
     180:	0040f809 	jalr	$v0
     184:	02202021 	move	$a0,$s1
     188:	2403ffff 	li	$v1,-1
     18c:	1043000c 	beq	$v0,$v1,1c0 <mxp_tcb_alloc+0x84>
     190:	24050001 	li	$a1,1
     194:	40016000 	mfc0	$at,$12
     198:	32100001 	andi	$s0,$s0,0x1
     19c:	34210001 	ori	$at,$at,0x1
     1a0:	38210001 	xori	$at,$at,0x1
     1a4:	02018025 	or	$s0,$s0,$at
     1a8:	40906000 	mtc0	$s0,$12
	...
     1b8:	08000097 	j	25c <mxp_tcb_alloc+0x120>
     1bc:	24020003 	li	$v0,3
     1c0:	24030030 	li	$v1,48
     1c4:	3c020000 	lui	$v0,0x0
     1c8:	8c4200c8 	lw	$v0,200($v0)
     1cc:	00622021 	addu	$a0,$v1,$v0
     1d0:	8c820000 	lw	$v0,0($a0)
     1d4:	54400014 	0x54400014
     1d8:	24a50001 	addiu	$a1,$a1,1
     1dc:	24020001 	li	$v0,1
     1e0:	ac820000 	sw	$v0,0($a0)
     1e4:	ae45001c 	sw	$a1,28($s2)
     1e8:	2484001c 	addiu	$a0,$a0,28
     1ec:	3c020000 	lui	$v0,0x0
     1f0:	24422a5c 	addiu	$v0,$v0,10844
     1f4:	0040f809 	jalr	$v0
     1f8:	02202821 	move	$a1,$s1
     1fc:	40016000 	mfc0	$at,$12
     200:	32100001 	andi	$s0,$s0,0x1
     204:	34210001 	ori	$at,$at,0x1
     208:	38210001 	xori	$at,$at,0x1
     20c:	02018025 	or	$s0,$s0,$at
     210:	40906000 	mtc0	$s0,$12
	...
     220:	08000097 	j	25c <mxp_tcb_alloc+0x120>
     224:	00001021 	move	$v0,$zero
     228:	28a20080 	slti	$v0,$a1,128
     22c:	1440ffe5 	bnez	$v0,1c4 <mxp_tcb_alloc+0x88>
     230:	24630030 	addiu	$v1,$v1,48
     234:	40016000 	mfc0	$at,$12
     238:	32100001 	andi	$s0,$s0,0x1
     23c:	34210001 	ori	$at,$at,0x1
     240:	38210001 	xori	$at,$at,0x1
     244:	02018025 	or	$s0,$s0,$at
     248:	40906000 	mtc0	$s0,$12
	...
     258:	24020004 	li	$v0,4
     25c:	8fbf001c 	lw	$ra,28($sp)
     260:	8fb20018 	lw	$s2,24($sp)
     264:	8fb10014 	lw	$s1,20($sp)
     268:	8fb00010 	lw	$s0,16($sp)
     26c:	03e00008 	jr	$ra
     270:	27bd0020 	addiu	$sp,$sp,32

0000000000000274 <mxp_tcb_free>:
     274:	40056000 	mfc0	$a1,$12
     278:	00000000 	nop
     27c:	34a10001 	ori	$at,$a1,0x1
     280:	38210001 	xori	$at,$at,0x1
     284:	40816000 	mtc0	$at,$12
     288:	00000040 	sll	$zero,$zero,0x1
     28c:	00000040 	sll	$zero,$zero,0x1
     290:	00000040 	sll	$zero,$zero,0x1
     294:	8c83001c 	lw	$v1,28($a0)
     298:	2462ffff 	addiu	$v0,$v1,-1
     29c:	2c42007f 	sltiu	$v0,$v0,127
     2a0:	10400009 	beqz	$v0,2c8 <mxp_tcb_free+0x54>
     2a4:	00031040 	sll	$v0,$v1,0x1
     2a8:	00431021 	addu	$v0,$v0,$v1
     2ac:	3c030000 	lui	$v1,0x0
     2b0:	8c6300c8 	lw	$v1,200($v1)
     2b4:	00021100 	sll	$v0,$v0,0x4
     2b8:	00431821 	addu	$v1,$v0,$v1
     2bc:	8c620000 	lw	$v0,0($v1)
     2c0:	5440000c 	0x5440000c
     2c4:	ac600000 	sw	$zero,0($v1)
     2c8:	40016000 	mfc0	$at,$12
     2cc:	30a50001 	andi	$a1,$a1,0x1
     2d0:	34210001 	ori	$at,$at,0x1
     2d4:	38210001 	xori	$at,$at,0x1
     2d8:	00a12825 	or	$a1,$a1,$at
     2dc:	40856000 	mtc0	$a1,$12
	...
     2ec:	03e00008 	jr	$ra
     2f0:	24020005 	li	$v0,5
     2f4:	40016000 	mfc0	$at,$12
     2f8:	30a50001 	andi	$a1,$a1,0x1
     2fc:	34210001 	ori	$at,$at,0x1
     300:	38210001 	xori	$at,$at,0x1
     304:	00a12825 	or	$a1,$a1,$at
     308:	40856000 	mtc0	$a1,$12
	...
     318:	03e00008 	jr	$ra
     31c:	00001021 	move	$v0,$zero

0000000000000320 <mxp_task_wakeup>:
     320:	27bdffe8 	addiu	$sp,$sp,-24
     324:	afbf0010 	sw	$ra,16($sp)
     328:	24050003 	li	$a1,3
     32c:	8c840010 	lw	$a0,16($a0)
     330:	3c020000 	lui	$v0,0x0
     334:	24420000 	addiu	$v0,$v0,0
     338:	00042140 	sll	$a0,$a0,0x5
     33c:	3c010000 	lui	$at,0x0
     340:	242100cc 	addiu	$at,$at,204
     344:	00242021 	addu	$a0,$at,$a0
     348:	0040f809 	jalr	$v0
     34c:	24060001 	li	$a2,1
     350:	8fbf0010 	lw	$ra,16($sp)
     354:	03e00008 	jr	$ra
     358:	27bd0018 	addiu	$sp,$sp,24

000000000000035c <mxp_task_sleep>:
     35c:	27bdffd8 	addiu	$sp,$sp,-40
     360:	afbf0020 	sw	$ra,32($sp)
     364:	afb3001c 	sw	$s3,28($sp)
     368:	afb20018 	sw	$s2,24($sp)
     36c:	afb10014 	sw	$s1,20($sp)
     370:	afb00010 	sw	$s0,16($sp)
     374:	00801021 	move	$v0,$a0
     378:	8c470004 	lw	$a3,4($v0)
     37c:	40136000 	mfc0	$s3,$12
     380:	00000000 	nop
     384:	36610001 	ori	$at,$s3,0x1
     388:	38210001 	xori	$at,$at,0x1
     38c:	40816000 	mtc0	$at,$12
     390:	00000040 	sll	$zero,$zero,0x1
     394:	00000040 	sll	$zero,$zero,0x1
     398:	00000040 	sll	$zero,$zero,0x1
     39c:	00078940 	sll	$s1,$a3,0x5
     3a0:	3c100000 	lui	$s0,0x0
     3a4:	261000d8 	addiu	$s0,$s0,216
     3a8:	02309021 	addu	$s2,$s1,$s0
     3ac:	3c060000 	lui	$a2,0x0
     3b0:	24c60320 	addiu	$a2,$a2,800
     3b4:	8c450008 	lw	$a1,8($v0)
     3b8:	3c020000 	lui	$v0,0x0
     3bc:	24422e70 	addiu	$v0,$v0,11888
     3c0:	0040f809 	jalr	$v0
     3c4:	02402021 	move	$a0,$s2
     3c8:	40016000 	mfc0	$at,$12
     3cc:	32730001 	andi	$s3,$s3,0x1
     3d0:	34210001 	ori	$at,$at,0x1
     3d4:	38210001 	xori	$at,$at,0x1
     3d8:	02619825 	or	$s3,$s3,$at
     3dc:	40936000 	mtc0	$s3,$12
	...
     3ec:	2610fff4 	addiu	$s0,$s0,-12
     3f0:	3c020000 	lui	$v0,0x0
     3f4:	24420000 	addiu	$v0,$v0,0
     3f8:	0040f809 	jalr	$v0
     3fc:	02302021 	addu	$a0,$s1,$s0
     400:	40136000 	mfc0	$s3,$12
     404:	00000000 	nop
     408:	36610001 	ori	$at,$s3,0x1
     40c:	38210001 	xori	$at,$at,0x1
     410:	40816000 	mtc0	$at,$12
     414:	00000040 	sll	$zero,$zero,0x1
     418:	00000040 	sll	$zero,$zero,0x1
     41c:	00000040 	sll	$zero,$zero,0x1
     420:	8e420000 	lw	$v0,0($s2)
     424:	10400005 	beqz	$v0,43c <mxp_task_sleep+0xe0>
     428:	00000000 	nop
     42c:	3c020000 	lui	$v0,0x0
     430:	24422d80 	addiu	$v0,$v0,11648
     434:	0040f809 	jalr	$v0
     438:	02402021 	move	$a0,$s2
     43c:	40016000 	mfc0	$at,$12
     440:	32730001 	andi	$s3,$s3,0x1
     444:	34210001 	ori	$at,$at,0x1
     448:	38210001 	xori	$at,$at,0x1
     44c:	02619825 	or	$s3,$s3,$at
     450:	40936000 	mtc0	$s3,$12
	...
     460:	00001021 	move	$v0,$zero
     464:	8fbf0020 	lw	$ra,32($sp)
     468:	8fb3001c 	lw	$s3,28($sp)
     46c:	8fb20018 	lw	$s2,24($sp)
     470:	8fb10014 	lw	$s1,20($sp)
     474:	8fb00010 	lw	$s0,16($sp)
     478:	03e00008 	jr	$ra
     47c:	27bd0028 	addiu	$sp,$sp,40
     480:	27bdffe8 	addiu	$sp,$sp,-24
     484:	afbf0010 	sw	$ra,16($sp)
     488:	00003021 	move	$a2,$zero
     48c:	8c870004 	lw	$a3,4($a0)
     490:	40096000 	mfc0	$t1,$12
     494:	00000000 	nop
     498:	35210001 	ori	$at,$t1,0x1
     49c:	38210001 	xori	$at,$at,0x1
     4a0:	40816000 	mtc0	$at,$12
     4a4:	00000040 	sll	$zero,$zero,0x1
     4a8:	00000040 	sll	$zero,$zero,0x1
     4ac:	00000040 	sll	$zero,$zero,0x1
     4b0:	24e2ffff 	addiu	$v0,$a3,-1
     4b4:	2c42007f 	sltiu	$v0,$v0,127
     4b8:	10400009 	beqz	$v0,4e0 <mxp_task_sleep+0x184>
     4bc:	00074040 	sll	$t0,$a3,0x1
     4c0:	01071021 	addu	$v0,$t0,$a3
     4c4:	3c030000 	lui	$v1,0x0
     4c8:	8c6300c8 	lw	$v1,200($v1)
     4cc:	00021100 	sll	$v0,$v0,0x4
     4d0:	00432821 	addu	$a1,$v0,$v1
     4d4:	8ca20000 	lw	$v0,0($a1)
     4d8:	5440000c 	0x5440000c
     4dc:	8ca3000c 	lw	$v1,12($a1)
     4e0:	40016000 	mfc0	$at,$12
     4e4:	31290001 	andi	$t1,$t1,0x1
     4e8:	34210001 	ori	$at,$at,0x1
     4ec:	38210001 	xori	$at,$at,0x1
     4f0:	01214825 	or	$t1,$t1,$at
     4f4:	40896000 	mtc0	$t1,$12
	...
     504:	08000175 	j	5d4 <mxp_task_sleep+0x278>
     508:	24020005 	li	$v0,5
     50c:	8c820008 	lw	$v0,8($a0)
     510:	00622025 	or	$a0,$v1,$v0
     514:	aca4000c 	sw	$a0,12($a1)
     518:	8ca2002c 	lw	$v0,44($a1)
     51c:	8ca30018 	lw	$v1,24($a1)
     520:	24420001 	addiu	$v0,$v0,1
     524:	1060000e 	beqz	$v1,560 <mxp_task_sleep+0x204>
     528:	aca2002c 	sw	$v0,44($a1)
     52c:	8ca30014 	lw	$v1,20($a1)
     530:	14600005 	bnez	$v1,548 <mxp_task_sleep+0x1ec>
     534:	24020001 	li	$v0,1
     538:	8ca20010 	lw	$v0,16($a1)
     53c:	00823024 	and	$a2,$a0,$v0
     540:	08000158 	j	560 <mxp_task_sleep+0x204>
     544:	0006302b 	sltu	$a2,$zero,$a2
     548:	14620005 	bne	$v1,$v0,560 <mxp_task_sleep+0x204>
     54c:	00000000 	nop
     550:	8ca20010 	lw	$v0,16($a1)
     554:	00823024 	and	$a2,$a0,$v0
     558:	00c21026 	xor	$v0,$a2,$v0
     55c:	2c460001 	sltiu	$a2,$v0,1
     560:	10c00006 	beqz	$a2,57c <mxp_task_sleep+0x220>
     564:	01071021 	addu	$v0,$t0,$a3
     568:	3c030000 	lui	$v1,0x0
     56c:	8c6300c8 	lw	$v1,200($v1)
     570:	00021100 	sll	$v0,$v0,0x4
     574:	00431021 	addu	$v0,$v0,$v1
     578:	ac400018 	sw	$zero,24($v0)
     57c:	40016000 	mfc0	$at,$12
     580:	31290001 	andi	$t1,$t1,0x1
     584:	34210001 	ori	$at,$at,0x1
     588:	38210001 	xori	$at,$at,0x1
     58c:	01214825 	or	$t1,$t1,$at
     590:	40896000 	mtc0	$t1,$12
	...
     5a0:	10c0000b 	beqz	$a2,5d0 <mxp_task_sleep+0x274>
     5a4:	00072140 	sll	$a0,$a3,0x5
     5a8:	3c010000 	lui	$at,0x0
     5ac:	242100cc 	addiu	$at,$at,204
     5b0:	00242021 	addu	$a0,$at,$a0
     5b4:	24050003 	li	$a1,3
     5b8:	3c020000 	lui	$v0,0x0
     5bc:	24420000 	addiu	$v0,$v0,0
     5c0:	0040f809 	jalr	$v0
     5c4:	24060001 	li	$a2,1
     5c8:	24020001 	li	$v0,1
     5cc:	af820014 	sw	$v0,20($gp)
     5d0:	00001021 	move	$v0,$zero
     5d4:	8fbf0010 	lw	$ra,16($sp)
     5d8:	03e00008 	jr	$ra
     5dc:	27bd0018 	addiu	$sp,$sp,24
     5e0:	27bdffc0 	addiu	$sp,$sp,-64
     5e4:	afbf0038 	sw	$ra,56($sp)
     5e8:	afb50034 	sw	$s5,52($sp)
     5ec:	afb40030 	sw	$s4,48($sp)
     5f0:	afb3002c 	sw	$s3,44($sp)
     5f4:	afb20028 	sw	$s2,40($sp)
     5f8:	afb10024 	sw	$s1,36($sp)
     5fc:	afb00020 	sw	$s0,32($sp)
     600:	00808821 	move	$s1,$a0
     604:	8e300004 	lw	$s0,4($s1)
     608:	40066000 	mfc0	$a2,$12
     60c:	00000000 	nop
     610:	34c10001 	ori	$at,$a2,0x1
     614:	38210001 	xori	$at,$at,0x1
     618:	40816000 	mtc0	$at,$12
     61c:	00000040 	sll	$zero,$zero,0x1
     620:	00000040 	sll	$zero,$zero,0x1
     624:	00000040 	sll	$zero,$zero,0x1
     628:	2602ffff 	addiu	$v0,$s0,-1
     62c:	2c42007f 	sltiu	$v0,$v0,127
     630:	10400009 	beqz	$v0,658 <mxp_task_sleep+0x2fc>
     634:	00109040 	sll	$s2,$s0,0x1
     638:	02501021 	addu	$v0,$s2,$s0
     63c:	3c070000 	lui	$a3,0x0
     640:	8ce700c8 	lw	$a3,200($a3)
     644:	00021100 	sll	$v0,$v0,0x4
     648:	00472021 	addu	$a0,$v0,$a3
     64c:	8c820000 	lw	$v0,0($a0)
     650:	5440000c 	0x5440000c
     654:	8e25000c 	lw	$a1,12($s1)
     658:	40016000 	mfc0	$at,$12
     65c:	30c60001 	andi	$a2,$a2,0x1
     660:	34210001 	ori	$at,$at,0x1
     664:	38210001 	xori	$at,$at,0x1
     668:	00c13025 	or	$a2,$a2,$at
     66c:	40866000 	mtc0	$a2,$12
	...
     67c:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     680:	24020005 	li	$v0,5
     684:	14a00015 	bnez	$a1,6dc <mxp_task_sleep+0x380>
     688:	8e230008 	lw	$v1,8($s1)
     68c:	8c82000c 	lw	$v0,12($a0)
     690:	00431024 	and	$v0,$v0,$v1
     694:	50400034 	0x50400034
     698:	8e240010 	lw	$a0,16($s1)
     69c:	ae220008 	sw	$v0,8($s1)
     6a0:	8c82000c 	lw	$v0,12($a0)
     6a4:	00031827 	nor	$v1,$zero,$v1
     6a8:	00431024 	and	$v0,$v0,$v1
     6ac:	ac82000c 	sw	$v0,12($a0)
     6b0:	40016000 	mfc0	$at,$12
     6b4:	30c60001 	andi	$a2,$a2,0x1
     6b8:	34210001 	ori	$at,$at,0x1
     6bc:	38210001 	xori	$at,$at,0x1
     6c0:	00c13025 	or	$a2,$a2,$at
     6c4:	40866000 	mtc0	$a2,$12
	...
     6d4:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     6d8:	00001021 	move	$v0,$zero
     6dc:	24020001 	li	$v0,1
     6e0:	14a20015 	bne	$a1,$v0,738 <mxp_task_sleep+0x3dc>
     6e4:	00000000 	nop
     6e8:	8c82000c 	lw	$v0,12($a0)
     6ec:	00431024 	and	$v0,$v0,$v1
     6f0:	5443001d 	0x5443001d
     6f4:	8e240010 	lw	$a0,16($s1)
     6f8:	ae230008 	sw	$v1,8($s1)
     6fc:	8c82000c 	lw	$v0,12($a0)
     700:	00031827 	nor	$v1,$zero,$v1
     704:	00431024 	and	$v0,$v0,$v1
     708:	ac82000c 	sw	$v0,12($a0)
     70c:	40016000 	mfc0	$at,$12
     710:	30c60001 	andi	$a2,$a2,0x1
     714:	34210001 	ori	$at,$at,0x1
     718:	38210001 	xori	$at,$at,0x1
     71c:	00c13025 	or	$a2,$a2,$at
     720:	40866000 	mtc0	$a2,$12
	...
     730:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     734:	00001021 	move	$v0,$zero
     738:	40016000 	mfc0	$at,$12
     73c:	30c60001 	andi	$a2,$a2,0x1
     740:	34210001 	ori	$at,$at,0x1
     744:	38210001 	xori	$at,$at,0x1
     748:	00c13025 	or	$a2,$a2,$at
     74c:	40866000 	mtc0	$a2,$12
	...
     75c:	3c040000 	lui	$a0,0x0
     760:	080001f4 	j	7d0 <mxp_task_sleep+0x474>
     764:	24840120 	addiu	$a0,$a0,288
     768:	1480000c 	bnez	$a0,79c <mxp_task_sleep+0x440>
     76c:	2402ffff 	li	$v0,-1
     770:	40016000 	mfc0	$at,$12
     774:	30c60001 	andi	$a2,$a2,0x1
     778:	34210001 	ori	$at,$at,0x1
     77c:	38210001 	xori	$at,$at,0x1
     780:	00c13025 	or	$a2,$a2,$at
     784:	40866000 	mtc0	$a2,$12
	...
     794:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     798:	2402000c 	li	$v0,12
     79c:	10820012 	beq	$a0,$v0,7e8 <mxp_task_sleep+0x48c>
     7a0:	02502021 	addu	$a0,$s2,$s0
     7a4:	40016000 	mfc0	$at,$12
     7a8:	30c60001 	andi	$a2,$a2,0x1
     7ac:	34210001 	ori	$at,$at,0x1
     7b0:	38210001 	xori	$at,$at,0x1
     7b4:	00c13025 	or	$a2,$a2,$at
     7b8:	40866000 	mtc0	$a2,$12
	...
     7c8:	3c040000 	lui	$a0,0x0
     7cc:	24840158 	addiu	$a0,$a0,344
     7d0:	3c020000 	lui	$v0,0x0
     7d4:	24420000 	addiu	$v0,$v0,0
     7d8:	0040f809 	jalr	$v0
     7dc:	02002821 	move	$a1,$s0
     7e0:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     7e4:	24020024 	li	$v0,36
     7e8:	00042100 	sll	$a0,$a0,0x4
     7ec:	00871021 	addu	$v0,$a0,$a3
     7f0:	ac450014 	sw	$a1,20($v0)
     7f4:	ac430010 	sw	$v1,16($v0)
     7f8:	24030001 	li	$v1,1
     7fc:	ac430018 	sw	$v1,24($v0)
     800:	40016000 	mfc0	$at,$12
     804:	30c60001 	andi	$a2,$a2,0x1
     808:	34210001 	ori	$at,$at,0x1
     80c:	38210001 	xori	$at,$at,0x1
     810:	00c13025 	or	$a2,$a2,$at
     814:	40866000 	mtc0	$a2,$12
	...
     824:	3c020000 	lui	$v0,0x0
     828:	8c4200c8 	lw	$v0,200($v0)
     82c:	00822021 	addu	$a0,$a0,$v0
     830:	8c820018 	lw	$v0,24($a0)
     834:	10400027 	beqz	$v0,8d4 <mxp_task_sleep+0x578>
     838:	0000a821 	move	$s5,$zero
     83c:	afa00010 	sw	$zero,16($sp)
     840:	27a50010 	addiu	$a1,$sp,16
     844:	03801021 	move	$v0,$gp
     848:	aca20004 	sw	$v0,4($a1)
     84c:	00109940 	sll	$s3,$s0,0x5
     850:	3c140000 	lui	$s4,0x0
     854:	269400cc 	addiu	$s4,$s4,204
     858:	3c020000 	lui	$v0,0x0
     85c:	24420000 	addiu	$v0,$v0,0
     860:	0040f809 	jalr	$v0
     864:	02742021 	addu	$a0,$s3,$s4
     868:	24020001 	li	$v0,1
     86c:	af820000 	sw	$v0,0($gp)
     870:	8f820000 	lw	$v0,0($gp)
     874:	3c030000 	lui	$v1,0x0
     878:	8c6300c8 	lw	$v1,200($v1)
     87c:	02501021 	addu	$v0,$s2,$s0
     880:	00021100 	sll	$v0,$v0,0x4
     884:	00431021 	addu	$v0,$v0,$v1
     888:	8c420018 	lw	$v0,24($v0)
     88c:	1040000a 	beqz	$v0,8b8 <mxp_task_sleep+0x55c>
     890:	00000000 	nop
     894:	8f820008 	lw	$v0,8($gp)
     898:	54400007 	0x54400007
     89c:	2415fe00 	li	$s5,-512
     8a0:	3c020000 	lui	$v0,0x0
     8a4:	24420000 	addiu	$v0,$v0,0
     8a8:	0040f809 	jalr	$v0
     8ac:	00000000 	nop
     8b0:	0800021b 	j	86c <mxp_task_sleep+0x510>
     8b4:	24020001 	li	$v0,1
     8b8:	af800000 	sw	$zero,0($gp)
     8bc:	8f820000 	lw	$v0,0($gp)
     8c0:	02742021 	addu	$a0,$s3,$s4
     8c4:	3c020000 	lui	$v0,0x0
     8c8:	24420000 	addiu	$v0,$v0,0
     8cc:	0040f809 	jalr	$v0
     8d0:	27a50010 	addiu	$a1,$sp,16
     8d4:	40066000 	mfc0	$a2,$12
     8d8:	00000000 	nop
     8dc:	34c10001 	ori	$at,$a2,0x1
     8e0:	38210001 	xori	$at,$at,0x1
     8e4:	40816000 	mtc0	$at,$12
     8e8:	00000040 	sll	$zero,$zero,0x1
     8ec:	00000040 	sll	$zero,$zero,0x1
     8f0:	00000040 	sll	$zero,$zero,0x1
     8f4:	2402fe00 	li	$v0,-512
     8f8:	16a20018 	bne	$s5,$v0,95c <mxp_task_sleep+0x600>
     8fc:	00101040 	sll	$v0,$s0,0x1
     900:	3c030000 	lui	$v1,0x0
     904:	8c6300c8 	lw	$v1,200($v1)
     908:	00501021 	addu	$v0,$v0,$s0
     90c:	00021100 	sll	$v0,$v0,0x4
     910:	00431021 	addu	$v0,$v0,$v1
     914:	ac400018 	sw	$zero,24($v0)
     918:	40016000 	mfc0	$at,$12
     91c:	30c60001 	andi	$a2,$a2,0x1
     920:	34210001 	ori	$at,$at,0x1
     924:	38210001 	xori	$at,$at,0x1
     928:	00c13025 	or	$a2,$a2,$at
     92c:	40866000 	mtc0	$a2,$12
	...
     93c:	3c040000 	lui	$a0,0x0
     940:	24840190 	addiu	$a0,$a0,400
     944:	3c020000 	lui	$v0,0x0
     948:	24420000 	addiu	$v0,$v0,0
     94c:	0040f809 	jalr	$v0
     950:	02002821 	move	$a1,$s0
     954:	08000271 	j	9c4 <mxp_task_sleep+0x668>
     958:	2402001f 	li	$v0,31
     95c:	00501021 	addu	$v0,$v0,$s0
     960:	3c030000 	lui	$v1,0x0
     964:	8c6300c8 	lw	$v1,200($v1)
     968:	00021100 	sll	$v0,$v0,0x4
     96c:	00432821 	addu	$a1,$v0,$v1
     970:	8ca20014 	lw	$v0,20($a1)
     974:	14400003 	bnez	$v0,984 <mxp_task_sleep+0x628>
     978:	8ca20010 	lw	$v0,16($a1)
     97c:	8ca3000c 	lw	$v1,12($a1)
     980:	00431024 	and	$v0,$v0,$v1
     984:	ae220008 	sw	$v0,8($s1)
     988:	8ca20010 	lw	$v0,16($a1)
     98c:	8ca3000c 	lw	$v1,12($a1)
     990:	00021027 	nor	$v0,$zero,$v0
     994:	00621824 	and	$v1,$v1,$v0
     998:	aca3000c 	sw	$v1,12($a1)
     99c:	40016000 	mfc0	$at,$12
     9a0:	30c60001 	andi	$a2,$a2,0x1
     9a4:	34210001 	ori	$at,$at,0x1
     9a8:	38210001 	xori	$at,$at,0x1
     9ac:	00c13025 	or	$a2,$a2,$at
     9b0:	40866000 	mtc0	$a2,$12
	...
     9c0:	00001021 	move	$v0,$zero
     9c4:	8fbf0038 	lw	$ra,56($sp)
     9c8:	8fb50034 	lw	$s5,52($sp)
     9cc:	8fb40030 	lw	$s4,48($sp)
     9d0:	8fb3002c 	lw	$s3,44($sp)
     9d4:	8fb20028 	lw	$s2,40($sp)
     9d8:	8fb10024 	lw	$s1,36($sp)
     9dc:	8fb00020 	lw	$s0,32($sp)
     9e0:	03e00008 	jr	$ra
     9e4:	27bd0040 	addiu	$sp,$sp,64
     9e8:	8c830004 	lw	$v1,4($a0)
     9ec:	40066000 	mfc0	$a2,$12
     9f0:	00000000 	nop
     9f4:	34c10001 	ori	$at,$a2,0x1
     9f8:	38210001 	xori	$at,$at,0x1
     9fc:	40816000 	mtc0	$at,$12
     a00:	00000040 	sll	$zero,$zero,0x1
     a04:	00000040 	sll	$zero,$zero,0x1
     a08:	00000040 	sll	$zero,$zero,0x1
     a0c:	2462ffff 	addiu	$v0,$v1,-1
     a10:	2c42007f 	sltiu	$v0,$v0,127
     a14:	10400009 	beqz	$v0,a3c <mxp_task_sleep+0x6e0>
     a18:	00031040 	sll	$v0,$v1,0x1
     a1c:	00431021 	addu	$v0,$v0,$v1
     a20:	3c030000 	lui	$v1,0x0
     a24:	8c6300c8 	lw	$v1,200($v1)
     a28:	00021100 	sll	$v0,$v0,0x4
     a2c:	00432821 	addu	$a1,$v0,$v1
     a30:	8ca20000 	lw	$v0,0($a1)
     a34:	5440000c 	0x5440000c
     a38:	8c820008 	lw	$v0,8($a0)
     a3c:	40016000 	mfc0	$at,$12
     a40:	30c60001 	andi	$a2,$a2,0x1
     a44:	34210001 	ori	$at,$at,0x1
     a48:	38210001 	xori	$at,$at,0x1
     a4c:	00c13025 	or	$a2,$a2,$at
     a50:	40866000 	mtc0	$a2,$12
	...
     a60:	03e00008 	jr	$ra
     a64:	24020005 	li	$v0,5
     a68:	8ca3000c 	lw	$v1,12($a1)
     a6c:	00021027 	nor	$v0,$zero,$v0
     a70:	00621824 	and	$v1,$v1,$v0
     a74:	aca3000c 	sw	$v1,12($a1)
     a78:	40016000 	mfc0	$at,$12
     a7c:	30c60001 	andi	$a2,$a2,0x1
     a80:	34210001 	ori	$at,$at,0x1
     a84:	38210001 	xori	$at,$at,0x1
     a88:	00c13025 	or	$a2,$a2,$at
     a8c:	40866000 	mtc0	$a2,$12
	...
     a9c:	03e00008 	jr	$ra
     aa0:	00001021 	move	$v0,$zero
     aa4:	8c830004 	lw	$v1,4($a0)
     aa8:	40056000 	mfc0	$a1,$12
     aac:	00000000 	nop
     ab0:	34a10001 	ori	$at,$a1,0x1
     ab4:	38210001 	xori	$at,$at,0x1
     ab8:	40816000 	mtc0	$at,$12
     abc:	00000040 	sll	$zero,$zero,0x1
     ac0:	00000040 	sll	$zero,$zero,0x1
     ac4:	00000040 	sll	$zero,$zero,0x1
     ac8:	2462ffff 	addiu	$v0,$v1,-1
     acc:	2c42007f 	sltiu	$v0,$v0,127
     ad0:	10400009 	beqz	$v0,af8 <mxp_task_sleep+0x79c>
     ad4:	00031040 	sll	$v0,$v1,0x1
     ad8:	00431021 	addu	$v0,$v0,$v1
     adc:	3c030000 	lui	$v1,0x0
     ae0:	8c6300c8 	lw	$v1,200($v1)
     ae4:	00021100 	sll	$v0,$v0,0x4
     ae8:	00431821 	addu	$v1,$v0,$v1
     aec:	8c620000 	lw	$v0,0($v1)
     af0:	5440000c 	0x5440000c
     af4:	8c62000c 	lw	$v0,12($v1)
     af8:	40016000 	mfc0	$at,$12
     afc:	30a50001 	andi	$a1,$a1,0x1
     b00:	34210001 	ori	$at,$at,0x1
     b04:	38210001 	xori	$at,$at,0x1
     b08:	00a12825 	or	$a1,$a1,$at
     b0c:	40856000 	mtc0	$a1,$12
	...
     b1c:	03e00008 	jr	$ra
     b20:	24020005 	li	$v0,5
     b24:	ac820008 	sw	$v0,8($a0)
     b28:	40016000 	mfc0	$at,$12
     b2c:	30a50001 	andi	$a1,$a1,0x1
     b30:	34210001 	ori	$at,$at,0x1
     b34:	38210001 	xori	$at,$at,0x1
     b38:	00a12825 	or	$a1,$a1,$at
     b3c:	40856000 	mtc0	$a1,$12
	...
     b4c:	03e00008 	jr	$ra
     b50:	00001021 	move	$v0,$zero
     b54:	acc00000 	sw	$zero,0($a2)
     b58:	8c820000 	lw	$v0,0($a0)
     b5c:	54400005 	0x54400005
     b60:	ac860000 	sw	$a2,0($a0)
     b64:	acc00004 	sw	$zero,4($a2)
     b68:	ac860000 	sw	$a2,0($a0)
     b6c:	03e00008 	jr	$ra
     b70:	aca60000 	sw	$a2,0($a1)
     b74:	acc20004 	sw	$v0,4($a2)
     b78:	03e00008 	jr	$ra
     b7c:	ac460000 	sw	$a2,0($v0)
     b80:	acc00004 	sw	$zero,4($a2)
     b84:	8ca20000 	lw	$v0,0($a1)
     b88:	54400005 	0x54400005
     b8c:	aca60000 	sw	$a2,0($a1)
     b90:	acc00000 	sw	$zero,0($a2)
     b94:	ac860000 	sw	$a2,0($a0)
     b98:	03e00008 	jr	$ra
     b9c:	aca60000 	sw	$a2,0($a1)
     ba0:	acc20000 	sw	$v0,0($a2)
     ba4:	03e00008 	jr	$ra
     ba8:	ac460004 	sw	$a2,4($v0)
     bac:	8cc30004 	lw	$v1,4($a2)
     bb0:	10600003 	beqz	$v1,bc0 <mxp_task_sleep+0x864>
     bb4:	8cc20000 	lw	$v0,0($a2)
     bb8:	080002f1 	j	bc4 <mxp_task_sleep+0x868>
     bbc:	ac620000 	sw	$v0,0($v1)
     bc0:	aca20000 	sw	$v0,0($a1)
     bc4:	8cc30000 	lw	$v1,0($a2)
     bc8:	10600003 	beqz	$v1,bd8 <mxp_task_sleep+0x87c>
     bcc:	8cc20004 	lw	$v0,4($a2)
     bd0:	03e00008 	jr	$ra
     bd4:	ac620004 	sw	$v0,4($v1)
     bd8:	03e00008 	jr	$ra
     bdc:	ac820000 	sw	$v0,0($a0)
     be0:	27bdffe0 	addiu	$sp,$sp,-32
     be4:	afbf0018 	sw	$ra,24($sp)
     be8:	afb10014 	sw	$s1,20($sp)
     bec:	afb00010 	sw	$s0,16($sp)
     bf0:	3c010003 	lui	$at,0x3
     bf4:	ac200000 	sw	$zero,0($at)
     bf8:	3c010003 	lui	$at,0x3
     bfc:	ac200004 	sw	$zero,4($at)
     c00:	3c110000 	lui	$s1,0x0
     c04:	26310000 	addiu	$s1,$s1,0
     c08:	24103fff 	li	$s0,16383
     c0c:	3c040003 	lui	$a0,0x3
     c10:	24840000 	addiu	$a0,$a0,0
     c14:	3c050003 	lui	$a1,0x3
     c18:	24a50004 	addiu	$a1,$a1,4
     c1c:	3c020000 	lui	$v0,0x0
     c20:	24420b80 	addiu	$v0,$v0,2944
     c24:	0040f809 	jalr	$v0
     c28:	02203021 	move	$a2,$s1
     c2c:	2610ffff 	addiu	$s0,$s0,-1
     c30:	0601fff6 	bgez	$s0,c0c <mxp_task_sleep+0x8b0>
     c34:	2631000c 	addiu	$s1,$s1,12
     c38:	3c100003 	lui	$s0,0x3
     c3c:	26100008 	addiu	$s0,$s0,8
     c40:	02002021 	move	$a0,$s0
     c44:	00002821 	move	$a1,$zero
     c48:	3c020000 	lui	$v0,0x0
     c4c:	24420000 	addiu	$v0,$v0,0
     c50:	0040f809 	jalr	$v0
     c54:	3406f000 	li	$a2,0xf000
     c58:	24020001 	li	$v0,1
     c5c:	ae020038 	sw	$v0,56($s0)
     c60:	2404003c 	li	$a0,60
     c64:	241003fe 	li	$s0,1022
     c68:	3c020003 	lui	$v0,0x3
     c6c:	24420034 	addiu	$v0,$v0,52
     c70:	00441021 	addu	$v0,$v0,$a0
     c74:	ac400000 	sw	$zero,0($v0)
     c78:	24430004 	addiu	$v1,$v0,4
     c7c:	ac430004 	sw	$v1,4($v0)
     c80:	ac430008 	sw	$v1,8($v0)
     c84:	2610ffff 	addiu	$s0,$s0,-1
     c88:	0601fff7 	bgez	$s0,c68 <mxp_task_sleep+0x90c>
     c8c:	2484003c 	addiu	$a0,$a0,60
     c90:	8fbf0018 	lw	$ra,24($sp)
     c94:	8fb10014 	lw	$s1,20($sp)
     c98:	8fb00010 	lw	$s0,16($sp)
     c9c:	03e00008 	jr	$ra
     ca0:	27bd0020 	addiu	$sp,$sp,32
     ca4:	24050001 	li	$a1,1
     ca8:	2404003c 	li	$a0,60
     cac:	3c030003 	lui	$v1,0x3
     cb0:	24630040 	addiu	$v1,$v1,64
     cb4:	00641821 	addu	$v1,$v1,$a0
     cb8:	8c620000 	lw	$v0,0($v1)
     cbc:	54400005 	0x54400005
     cc0:	24a50001 	addiu	$a1,$a1,1
     cc4:	24020001 	li	$v0,1
     cc8:	ac620000 	sw	$v0,0($v1)
     ccc:	03e00008 	jr	$ra
     cd0:	00a01021 	move	$v0,$a1
     cd4:	28a20400 	slti	$v0,$a1,1024
     cd8:	1440fff4 	bnez	$v0,cac <mxp_task_sleep+0x950>
     cdc:	2484003c 	addiu	$a0,$a0,60
     ce0:	03e00008 	jr	$ra
     ce4:	00001021 	move	$v0,$zero
     ce8:	27bdffe0 	addiu	$sp,$sp,-32
     cec:	afbf001c 	sw	$ra,28($sp)
     cf0:	afb20018 	sw	$s2,24($sp)
     cf4:	afb10014 	sw	$s1,20($sp)
     cf8:	afb00010 	sw	$s0,16($sp)
     cfc:	00809021 	move	$s2,$a0
     d00:	24110001 	li	$s1,1
     d04:	2410003c 	li	$s0,60
     d08:	3c020003 	lui	$v0,0x3
     d0c:	00501021 	addu	$v0,$v0,$s0
     d10:	8c420040 	lw	$v0,64($v0)
     d14:	3c040003 	lui	$a0,0x3
     d18:	24840040 	addiu	$a0,$a0,64
     d1c:	1040000a 	beqz	$v0,d48 <mxp_task_sleep+0x9ec>
     d20:	2484ffdc 	addiu	$a0,$a0,-36
     d24:	02042021 	addu	$a0,$s0,$a0
     d28:	3c020000 	lui	$v0,0x0
     d2c:	24422a24 	addiu	$v0,$v0,10788
     d30:	0040f809 	jalr	$v0
     d34:	02402821 	move	$a1,$s2
     d38:	54400004 	0x54400004
     d3c:	26310001 	addiu	$s1,$s1,1
     d40:	08000357 	j	d5c <mxp_task_sleep+0xa00>
     d44:	02201021 	move	$v0,$s1
     d48:	26310001 	addiu	$s1,$s1,1
     d4c:	2a220400 	slti	$v0,$s1,1024
     d50:	1440ffed 	bnez	$v0,d08 <mxp_task_sleep+0x9ac>
     d54:	2610003c 	addiu	$s0,$s0,60
     d58:	2402ffff 	li	$v0,-1
     d5c:	8fbf001c 	lw	$ra,28($sp)
     d60:	8fb20018 	lw	$s2,24($sp)
     d64:	8fb10014 	lw	$s1,20($sp)
     d68:	8fb00010 	lw	$s0,16($sp)
     d6c:	03e00008 	jr	$ra
     d70:	27bd0020 	addiu	$sp,$sp,32
     d74:	27bdffd0 	addiu	$sp,$sp,-48
     d78:	afbf0028 	sw	$ra,40($sp)
     d7c:	afb50024 	sw	$s5,36($sp)
     d80:	afb40020 	sw	$s4,32($sp)
     d84:	afb3001c 	sw	$s3,28($sp)
     d88:	afb20018 	sw	$s2,24($sp)
     d8c:	afb10014 	sw	$s1,20($sp)
     d90:	afb00010 	sw	$s0,16($sp)
     d94:	0080a821 	move	$s5,$a0
     d98:	40136000 	mfc0	$s3,$12
     d9c:	00000000 	nop
     da0:	36610001 	ori	$at,$s3,0x1
     da4:	38210001 	xori	$at,$at,0x1
     da8:	40816000 	mtc0	$at,$12
     dac:	00000040 	sll	$zero,$zero,0x1
     db0:	00000040 	sll	$zero,$zero,0x1
     db4:	00000040 	sll	$zero,$zero,0x1
     db8:	26b4001c 	addiu	$s4,$s5,28
     dbc:	3c020000 	lui	$v0,0x0
     dc0:	24420ce8 	addiu	$v0,$v0,3304
     dc4:	0040f809 	jalr	$v0
     dc8:	02802021 	move	$a0,$s4
     dcc:	1840000c 	blez	$v0,e00 <mxp_task_sleep+0xaa4>
     dd0:	00000000 	nop
     dd4:	40016000 	mfc0	$at,$12
     dd8:	32730001 	andi	$s3,$s3,0x1
     ddc:	34210001 	ori	$at,$at,0x1
     de0:	38210001 	xori	$at,$at,0x1
     de4:	02619825 	or	$s3,$s3,$at
     de8:	40936000 	mtc0	$s3,$12
	...
     df8:	080003b2 	j	ec8 <mxp_task_sleep+0xb6c>
     dfc:	24020003 	li	$v0,3
     e00:	3c020000 	lui	$v0,0x0
     e04:	24420ca4 	addiu	$v0,$v0,3236
     e08:	0040f809 	jalr	$v0
     e0c:	00000000 	nop
     e10:	00409021 	move	$s2,$v0
     e14:	12400022 	beqz	$s2,ea0 <mxp_task_sleep+0xb44>
     e18:	00128100 	sll	$s0,$s2,0x4
     e1c:	02128023 	subu	$s0,$s0,$s2
     e20:	00108080 	sll	$s0,$s0,0x2
     e24:	3c110003 	lui	$s1,0x3
     e28:	2631001c 	addiu	$s1,$s1,28
     e2c:	02112021 	addu	$a0,$s0,$s1
     e30:	3c020000 	lui	$v0,0x0
     e34:	24422a5c 	addiu	$v0,$v0,10844
     e38:	0040f809 	jalr	$v0
     e3c:	02802821 	move	$a1,$s4
     e40:	02301021 	addu	$v0,$s1,$s0
     e44:	ac40fff4 	sw	$zero,-12($v0)
     e48:	8ea30010 	lw	$v1,16($s5)
     e4c:	ac43fff8 	sw	$v1,-8($v0)
     e50:	8ea30018 	lw	$v1,24($s5)
     e54:	ac430010 	sw	$v1,16($v0)
     e58:	8ea30014 	lw	$v1,20($s5)
     e5c:	00408821 	move	$s1,$v0
     e60:	ac430014 	sw	$v1,20($v0)
     e64:	ac40ffec 	sw	$zero,-20($v0)
     e68:	ac40fff0 	sw	$zero,-16($v0)
     e6c:	ae20fffc 	sw	$zero,-4($s1)
     e70:	aeb20004 	sw	$s2,4($s5)
     e74:	40016000 	mfc0	$at,$12
     e78:	32730001 	andi	$s3,$s3,0x1
     e7c:	34210001 	ori	$at,$at,0x1
     e80:	38210001 	xori	$at,$at,0x1
     e84:	02619825 	or	$s3,$s3,$at
     e88:	40936000 	mtc0	$s3,$12
	...
     e98:	080003b2 	j	ec8 <mxp_task_sleep+0xb6c>
     e9c:	00001021 	move	$v0,$zero
     ea0:	40016000 	mfc0	$at,$12
     ea4:	32730001 	andi	$s3,$s3,0x1
     ea8:	34210001 	ori	$at,$at,0x1
     eac:	38210001 	xori	$at,$at,0x1
     eb0:	02619825 	or	$s3,$s3,$at
     eb4:	40936000 	mtc0	$s3,$12
	...
     ec4:	24020007 	li	$v0,7
     ec8:	8fbf0028 	lw	$ra,40($sp)
     ecc:	8fb50024 	lw	$s5,36($sp)
     ed0:	8fb40020 	lw	$s4,32($sp)
     ed4:	8fb3001c 	lw	$s3,28($sp)
     ed8:	8fb20018 	lw	$s2,24($sp)
     edc:	8fb10014 	lw	$s1,20($sp)
     ee0:	8fb00010 	lw	$s0,16($sp)
     ee4:	03e00008 	jr	$ra
     ee8:	27bd0030 	addiu	$sp,$sp,48
*/
/*
     eec:	27bdffd0 	addiu	$sp,$sp,-48
     ef0:	afbf0028 	sw	$ra,40($sp)
     ef4:	afb50024 	sw	$s5,36($sp)
     ef8:	afb40020 	sw	$s4,32($sp)
     efc:	afb3001c 	sw	$s3,28($sp)
     f00:	afb20018 	sw	$s2,24($sp)
     f04:	afb10014 	sw	$s1,20($sp)
     f08:	afb00010 	sw	$s0,16($sp)
     f0c:	8c920004 	lw	$s2,4($a0)
     f10:	40156000 	mfc0	$s5,$12
     f14:	00000000 	nop
     f18:	36a10001 	ori	$at,$s5,0x1
     f1c:	38210001 	xori	$at,$at,0x1
     f20:	40816000 	mtc0	$at,$12
     f24:	00000040 	sll	$zero,$zero,0x1
     f28:	00000040 	sll	$zero,$zero,0x1
     f2c:	00000040 	sll	$zero,$zero,0x1
     f30:	2642ffff 	addiu	$v0,$s2,-1
     f34:	2c4203ff 	sltiu	$v0,$v0,1023
     f38:	1040000a 	beqz	$v0,f64 <mxp_task_sleep+0xc08>
     f3c:	00129900 	sll	$s3,$s2,0x4
     f40:	02721023 	subu	$v0,$s3,$s2
     f44:	00021880 	sll	$v1,$v0,0x2
     f48:	3c020003 	lui	$v0,0x3
     f4c:	00431021 	addu	$v0,$v0,$v1
     f50:	8c420040 	lw	$v0,64($v0)
     f54:	3c140003 	lui	$s4,0x3
     f58:	26940040 	addiu	$s4,$s4,64
     f5c:	1440000c 	bnez	$v0,f90 <mxp_task_sleep+0xc34>
     f60:	02831021 	addu	$v0,$s4,$v1
     f64:	40016000 	mfc0	$at,$12
     f68:	32b50001 	andi	$s5,$s5,0x1
     f6c:	34210001 	ori	$at,$at,0x1
     f70:	38210001 	xori	$at,$at,0x1
     f74:	02a1a825 	or	$s5,$s5,$at
     f78:	40956000 	mtc0	$s5,$12
	...
     f88:	0800040e 	j	1038 <mxp_task_sleep+0xcdc>
     f8c:	24020008 	li	$v0,8
     f90:	8c42ffc8 	lw	$v0,-56($v0)
     f94:	1040001a 	beqz	$v0,1000 <mxp_task_sleep+0xca4>
     f98:	00121100 	sll	$v0,$s2,0x4
     f9c:	02721023 	subu	$v0,$s3,$s2
     fa0:	00021080 	sll	$v0,$v0,0x2
     fa4:	3c050003 	lui	$a1,0x3
     fa8:	24a50008 	addiu	$a1,$a1,8
     fac:	00458021 	addu	$s0,$v0,$a1
     fb0:	02002021 	move	$a0,$s0
     fb4:	24a50004 	addiu	$a1,$a1,4
     fb8:	00452821 	addu	$a1,$v0,$a1
     fbc:	8e110000 	lw	$s1,0($s0)
     fc0:	3c020000 	lui	$v0,0x0
     fc4:	24420bac 	addiu	$v0,$v0,2988
     fc8:	0040f809 	jalr	$v0
     fcc:	02203021 	move	$a2,$s1
     fd0:	3c040003 	lui	$a0,0x3
     fd4:	24840000 	addiu	$a0,$a0,0
     fd8:	3c050003 	lui	$a1,0x3
     fdc:	24a50004 	addiu	$a1,$a1,4
     fe0:	3c020000 	lui	$v0,0x0
     fe4:	24420b80 	addiu	$v0,$v0,2944
     fe8:	0040f809 	jalr	$v0
     fec:	02203021 	move	$a2,$s1
     ff0:	8e020000 	lw	$v0,0($s0)
     ff4:	1440ffea 	bnez	$v0,fa0 <mxp_task_sleep+0xc44>
     ff8:	02721023 	subu	$v0,$s3,$s2
     ffc:	00121100 	sll	$v0,$s2,0x4
    1000:	00521023 	subu	$v0,$v0,$s2
    1004:	00021080 	sll	$v0,$v0,0x2
    1008:	00541021 	addu	$v0,$v0,$s4
    100c:	ac400000 	sw	$zero,0($v0)
    1010:	40016000 	mfc0	$at,$12
    1014:	32b50001 	andi	$s5,$s5,0x1
    1018:	34210001 	ori	$at,$at,0x1
    101c:	38210001 	xori	$at,$at,0x1
    1020:	02a1a825 	or	$s5,$s5,$at
    1024:	40956000 	mtc0	$s5,$12
	...
    1034:	00001021 	move	$v0,$zero
    1038:	8fbf0028 	lw	$ra,40($sp)
    103c:	8fb50024 	lw	$s5,36($sp)
    1040:	8fb40020 	lw	$s4,32($sp)
    1044:	8fb3001c 	lw	$s3,28($sp)
    1048:	8fb20018 	lw	$s2,24($sp)
    104c:	8fb10014 	lw	$s1,20($sp)
    1050:	8fb00010 	lw	$s0,16($sp)
    1054:	03e00008 	jr	$ra
    1058:	27bd0030 	addiu	$sp,$sp,48
*/
/*
    105c:	27bdffb0 	addiu	$sp,$sp,-80
    1060:	afbf0048 	sw	$ra,72($sp)
    1064:	afb10044 	sw	$s1,68($sp)
    1068:	afb00040 	sw	$s0,64($sp)
    106c:	24100001 	li	$s0,1
    1070:	3c110003 	lui	$s1,0x3
    1074:	2631007c 	addiu	$s1,$s1,124
    1078:	8e220000 	lw	$v0,0($s1)
    107c:	18400006 	blez	$v0,1098 <mxp_task_sleep+0xd3c>
    1080:	2631003c 	addiu	$s1,$s1,60
    1084:	afb00014 	sw	$s0,20($sp)
    1088:	3c020000 	lui	$v0,0x0
    108c:	24420eec 	addiu	$v0,$v0,3820
    1090:	0040f809 	jalr	$v0
    1094:	27a40010 	addiu	$a0,$sp,16
    1098:	26100001 	addiu	$s0,$s0,1
    109c:	2a020400 	slti	$v0,$s0,1024
    10a0:	5440fff6 	0x5440fff6
    10a4:	8e220000 	lw	$v0,0($s1)
    10a8:	8fbf0048 	lw	$ra,72($sp)
    10ac:	8fb10044 	lw	$s1,68($sp)
    10b0:	8fb00040 	lw	$s0,64($sp)
    10b4:	00001021 	move	$v0,$zero
    10b8:	03e00008 	jr	$ra
    10bc:	27bd0050 	addiu	$sp,$sp,80
    10c0:	27bdff98 	addiu	$sp,$sp,-104
    10c4:	afbf0060 	sw	$ra,96($sp)
    10c8:	afb7005c 	sw	$s7,92($sp)
    10cc:	afb60058 	sw	$s6,88($sp)
    10d0:	afb50054 	sw	$s5,84($sp)
    10d4:	afb40050 	sw	$s4,80($sp)
    10d8:	afb3004c 	sw	$s3,76($sp)
    10dc:	afb20048 	sw	$s2,72($sp)
    10e0:	afb10044 	sw	$s1,68($sp)
    10e4:	afb00040 	sw	$s0,64($sp)
    10e8:	0000b821 	move	$s7,$zero
    10ec:	0000a821 	move	$s5,$zero
    10f0:	0080b021 	move	$s6,$a0
    10f4:	8ec30004 	lw	$v1,4($s6)
    10f8:	40146000 	mfc0	$s4,$12
    10fc:	00000000 	nop
    1100:	36810001 	ori	$at,$s4,0x1
    1104:	38210001 	xori	$at,$at,0x1
    1108:	40816000 	mtc0	$at,$12
    110c:	00000040 	sll	$zero,$zero,0x1
    1110:	00000040 	sll	$zero,$zero,0x1
    1114:	00000040 	sll	$zero,$zero,0x1
    1118:	2462ffff 	addiu	$v0,$v1,-1
    111c:	2c4203ff 	sltiu	$v0,$v0,1023
    1120:	1040000a 	beqz	$v0,114c <mxp_task_sleep+0xdf0>
    1124:	00031100 	sll	$v0,$v1,0x4
    1128:	00431023 	subu	$v0,$v0,$v1
    112c:	00028880 	sll	$s1,$v0,0x2
    1130:	3c020003 	lui	$v0,0x3
    1134:	00511021 	addu	$v0,$v0,$s1
    1138:	8c420040 	lw	$v0,64($v0)
    113c:	3c120003 	lui	$s2,0x3
    1140:	26520040 	addiu	$s2,$s2,64
    1144:	1440000c 	bnez	$v0,1178 <mxp_task_sleep+0xe1c>
    1148:	2642ffd0 	addiu	$v0,$s2,-48
    114c:	40016000 	mfc0	$at,$12
    1150:	32940001 	andi	$s4,$s4,0x1
    1154:	34210001 	ori	$at,$at,0x1
    1158:	38210001 	xori	$at,$at,0x1
    115c:	0281a025 	or	$s4,$s4,$at
    1160:	40946000 	mtc0	$s4,$12
	...
    1170:	080004af 	j	12bc <mxp_task_sleep+0xf60>
    1174:	24020008 	li	$v0,8
    1178:	02229821 	addu	$s3,$s1,$v0
    117c:	02511821 	addu	$v1,$s2,$s1
    1180:	8e620000 	lw	$v0,0($s3)
    1184:	8c63ffd4 	lw	$v1,-44($v1)
    1188:	0043102a 	slt	$v0,$v0,$v1
    118c:	1440000c 	bnez	$v0,11c0 <mxp_task_sleep+0xe64>
    1190:	00000000 	nop
    1194:	40016000 	mfc0	$at,$12
    1198:	32940001 	andi	$s4,$s4,0x1
    119c:	34210001 	ori	$at,$at,0x1
    11a0:	38210001 	xori	$at,$at,0x1
    11a4:	0281a025 	or	$s4,$s4,$at
    11a8:	40946000 	mtc0	$s4,$12
	...
    11b8:	080004af 	j	12bc <mxp_task_sleep+0xf60>
    11bc:	24020009 	li	$v0,9
    11c0:	3c040003 	lui	$a0,0x3
    11c4:	24840000 	addiu	$a0,$a0,0
    11c8:	3c050003 	lui	$a1,0x3
    11cc:	24a50004 	addiu	$a1,$a1,4
    11d0:	3c100003 	lui	$s0,0x3
    11d4:	8e100000 	lw	$s0,0($s0)
    11d8:	3c020000 	lui	$v0,0x0
    11dc:	24420bac 	addiu	$v0,$v0,2988
    11e0:	0040f809 	jalr	$v0
    11e4:	02003021 	move	$a2,$s0
    11e8:	2644ffc8 	addiu	$a0,$s2,-56
    11ec:	02242021 	addu	$a0,$s1,$a0
    11f0:	2645ffcc 	addiu	$a1,$s2,-52
    11f4:	02252821 	addu	$a1,$s1,$a1
    11f8:	8ec2000c 	lw	$v0,12($s6)
    11fc:	ae020008 	sw	$v0,8($s0)
    1200:	3c020000 	lui	$v0,0x0
    1204:	24420b80 	addiu	$v0,$v0,2944
    1208:	0040f809 	jalr	$v0
    120c:	02003021 	move	$a2,$s0
    1210:	8e620000 	lw	$v0,0($s3)
    1214:	24420001 	addiu	$v0,$v0,1
    1218:	ae620000 	sw	$v0,0($s3)
    121c:	2642ffd8 	addiu	$v0,$s2,-40
    1220:	02221821 	addu	$v1,$s1,$v0
    1224:	8c620000 	lw	$v0,0($v1)
    1228:	18400003 	blez	$v0,1238 <mxp_task_sleep+0xedc>
    122c:	02511021 	addu	$v0,$s2,$s1
    1230:	ac600000 	sw	$zero,0($v1)
    1234:	24170001 	li	$s7,1
    1238:	8c42ffec 	lw	$v0,-20($v0)
    123c:	10400006 	beqz	$v0,1258 <mxp_task_sleep+0xefc>
    1240:	00000000 	nop
    1244:	0040a821 	move	$s5,$v0
    1248:	02511021 	addu	$v0,$s2,$s1
    124c:	8c42fff0 	lw	$v0,-16($v0)
    1250:	afb50014 	sw	$s5,20($sp)
    1254:	afa20018 	sw	$v0,24($sp)
    1258:	40016000 	mfc0	$at,$12
    125c:	32940001 	andi	$s4,$s4,0x1
    1260:	34210001 	ori	$at,$at,0x1
    1264:	38210001 	xori	$at,$at,0x1
    1268:	0281a025 	or	$s4,$s4,$at
    126c:	40946000 	mtc0	$s4,$12
	...
    127c:	12e00009 	beqz	$s7,12a4 <mxp_task_sleep+0xf48>
    1280:	24020001 	li	$v0,1
    1284:	af820014 	sw	$v0,20($gp)
    1288:	2644fff4 	addiu	$a0,$s2,-12
    128c:	02242021 	addu	$a0,$s1,$a0
    1290:	24050003 	li	$a1,3
    1294:	3c020000 	lui	$v0,0x0
    1298:	24420000 	addiu	$v0,$v0,0
    129c:	0040f809 	jalr	$v0
    12a0:	24060001 	li	$a2,1
    12a4:	12a00005 	beqz	$s5,12bc <mxp_task_sleep+0xf60>
    12a8:	00001021 	move	$v0,$zero
    12ac:	3c020000 	lui	$v0,0x0
    12b0:	24420480 	addiu	$v0,$v0,1152
    12b4:	0040f809 	jalr	$v0
    12b8:	27a40010 	addiu	$a0,$sp,16
    12bc:	8fbf0060 	lw	$ra,96($sp)
    12c0:	8fb7005c 	lw	$s7,92($sp)
    12c4:	8fb60058 	lw	$s6,88($sp)
    12c8:	8fb50054 	lw	$s5,84($sp)
    12cc:	8fb40050 	lw	$s4,80($sp)
    12d0:	8fb3004c 	lw	$s3,76($sp)
    12d4:	8fb20048 	lw	$s2,72($sp)
    12d8:	8fb10044 	lw	$s1,68($sp)
    12dc:	8fb00040 	lw	$s0,64($sp)
    12e0:	03e00008 	jr	$ra
    12e4:	27bd0068 	addiu	$sp,$sp,104
    12e8:	27bdffc0 	addiu	$sp,$sp,-64
    12ec:	afbf0038 	sw	$ra,56($sp)
    12f0:	afb50034 	sw	$s5,52($sp)
    12f4:	afb40030 	sw	$s4,48($sp)
    12f8:	afb3002c 	sw	$s3,44($sp)
    12fc:	afb20028 	sw	$s2,40($sp)
    1300:	afb10024 	sw	$s1,36($sp)
    1304:	afb00020 	sw	$s0,32($sp)
    1308:	0080a021 	move	$s4,$a0
    130c:	8e910004 	lw	$s1,4($s4)
    1310:	40136000 	mfc0	$s3,$12
    1314:	00000000 	nop
    1318:	36610001 	ori	$at,$s3,0x1
    131c:	38210001 	xori	$at,$at,0x1
    1320:	40816000 	mtc0	$at,$12
    1324:	00000040 	sll	$zero,$zero,0x1
    1328:	00000040 	sll	$zero,$zero,0x1
    132c:	00000040 	sll	$zero,$zero,0x1
    1330:	2622ffff 	addiu	$v0,$s1,-1
    1334:	2c4203ff 	sltiu	$v0,$v0,1023
    1338:	1040000a 	beqz	$v0,1364 <mxp_task_sleep+0x1008>
    133c:	00118100 	sll	$s0,$s1,0x4
    1340:	02111023 	subu	$v0,$s0,$s1
    1344:	00023080 	sll	$a2,$v0,0x2
    1348:	3c020003 	lui	$v0,0x3
    134c:	00461021 	addu	$v0,$v0,$a2
    1350:	8c420040 	lw	$v0,64($v0)
    1354:	3c070003 	lui	$a3,0x3
    1358:	24e70040 	addiu	$a3,$a3,64
    135c:	1440000c 	bnez	$v0,1390 <mxp_task_sleep+0x1034>
    1360:	00e61021 	addu	$v0,$a3,$a2
    1364:	40016000 	mfc0	$at,$12
    1368:	32730001 	andi	$s3,$s3,0x1
    136c:	34210001 	ori	$at,$at,0x1
    1370:	38210001 	xori	$at,$at,0x1
    1374:	02619825 	or	$s3,$s3,$at
    1378:	40936000 	mtc0	$s3,$12
	...
    1388:	08000585 	j	1614 <mxp_task_sleep+0x12b8>
    138c:	24020008 	li	$v0,8
    1390:	8c42ffd0 	lw	$v0,-48($v0)
    1394:	50400029 	0x50400029
    1398:	8e820008 	lw	$v0,8($s4)
    139c:	00118100 	sll	$s0,$s1,0x4
    13a0:	02118023 	subu	$s0,$s0,$s1
    13a4:	00108080 	sll	$s0,$s0,0x2
    13a8:	3c120003 	lui	$s2,0x3
    13ac:	26520008 	addiu	$s2,$s2,8
    13b0:	02121021 	addu	$v0,$s0,$s2
    13b4:	00402021 	move	$a0,$v0
    13b8:	26450004 	addiu	$a1,$s2,4
    13bc:	02052821 	addu	$a1,$s0,$a1
    13c0:	8c510000 	lw	$s1,0($v0)
    13c4:	3c020000 	lui	$v0,0x0
    13c8:	24420bac 	addiu	$v0,$v0,2988
    13cc:	0040f809 	jalr	$v0
    13d0:	02203021 	move	$a2,$s1
    13d4:	3c040003 	lui	$a0,0x3
    13d8:	24840000 	addiu	$a0,$a0,0
    13dc:	3c050003 	lui	$a1,0x3
    13e0:	24a50004 	addiu	$a1,$a1,4
    13e4:	3c020000 	lui	$v0,0x0
    13e8:	24420b80 	addiu	$v0,$v0,2944
    13ec:	0040f809 	jalr	$v0
    13f0:	02203021 	move	$a2,$s1
    13f4:	26520008 	addiu	$s2,$s2,8
    13f8:	8e220008 	lw	$v0,8($s1)
    13fc:	02128021 	addu	$s0,$s0,$s2
    1400:	ae82000c 	sw	$v0,12($s4)
    1404:	8e020000 	lw	$v0,0($s0)
    1408:	2442ffff 	addiu	$v0,$v0,-1
    140c:	ae020000 	sw	$v0,0($s0)
    1410:	40016000 	mfc0	$at,$12
    1414:	32730001 	andi	$s3,$s3,0x1
    1418:	34210001 	ori	$at,$at,0x1
    141c:	38210001 	xori	$at,$at,0x1
    1420:	02619825 	or	$s3,$s3,$at
    1424:	40936000 	mtc0	$s3,$12
	...
    1434:	08000585 	j	1614 <mxp_task_sleep+0x12b8>
    1438:	00001021 	move	$v0,$zero
    143c:	1440000c 	bnez	$v0,1470 <mxp_task_sleep+0x1114>
    1440:	24e4ffd8 	addiu	$a0,$a3,-40
    1444:	40016000 	mfc0	$at,$12
    1448:	32730001 	andi	$s3,$s3,0x1
    144c:	34210001 	ori	$at,$at,0x1
    1450:	38210001 	xori	$at,$at,0x1
    1454:	02619825 	or	$s3,$s3,$at
    1458:	40936000 	mtc0	$s3,$12
	...
    1468:	08000585 	j	1614 <mxp_task_sleep+0x12b8>
    146c:	2402000a 	li	$v0,10
    1470:	00c41821 	addu	$v1,$a2,$a0
    1474:	24020001 	li	$v0,1
    1478:	ac620000 	sw	$v0,0($v1)
    147c:	40016000 	mfc0	$at,$12
    1480:	32730001 	andi	$s3,$s3,0x1
    1484:	34210001 	ori	$at,$at,0x1
    1488:	38210001 	xori	$at,$at,0x1
    148c:	02619825 	or	$s3,$s3,$at
    1490:	40936000 	mtc0	$s3,$12
	...
    14a0:	0000a821 	move	$s5,$zero
    14a4:	8c620000 	lw	$v0,0($v1)
    14a8:	10400027 	beqz	$v0,1548 <mxp_task_sleep+0x11ec>
    14ac:	00809021 	move	$s2,$a0
    14b0:	afa00010 	sw	$zero,16($sp)
    14b4:	27a50010 	addiu	$a1,$sp,16
    14b8:	03801021 	move	$v0,$gp
    14bc:	aca20004 	sw	$v0,4($a1)
    14c0:	24e4fff4 	addiu	$a0,$a3,-12
    14c4:	3c020000 	lui	$v0,0x0
    14c8:	24420000 	addiu	$v0,$v0,0
    14cc:	0040f809 	jalr	$v0
    14d0:	00c42021 	addu	$a0,$a2,$a0
    14d4:	24020001 	li	$v0,1
    14d8:	af820000 	sw	$v0,0($gp)
    14dc:	8f820000 	lw	$v0,0($gp)
    14e0:	02111023 	subu	$v0,$s0,$s1
    14e4:	00021080 	sll	$v0,$v0,0x2
    14e8:	00521021 	addu	$v0,$v0,$s2
    14ec:	8c420000 	lw	$v0,0($v0)
    14f0:	1040000a 	beqz	$v0,151c <mxp_task_sleep+0x11c0>
    14f4:	00000000 	nop
    14f8:	8f820008 	lw	$v0,8($gp)
    14fc:	54400007 	0x54400007
    1500:	2415fe00 	li	$s5,-512
    1504:	3c020000 	lui	$v0,0x0
    1508:	24420000 	addiu	$v0,$v0,0
    150c:	0040f809 	jalr	$v0
    1510:	00000000 	nop
    1514:	08000536 	j	14d8 <mxp_task_sleep+0x117c>
    1518:	24020001 	li	$v0,1
    151c:	af800000 	sw	$zero,0($gp)
    1520:	8f820000 	lw	$v0,0($gp)
    1524:	02112023 	subu	$a0,$s0,$s1
    1528:	00042080 	sll	$a0,$a0,0x2
    152c:	3c010003 	lui	$at,0x3
    1530:	24210034 	addiu	$at,$at,52
    1534:	00242021 	addu	$a0,$at,$a0
    1538:	3c020000 	lui	$v0,0x0
    153c:	24420000 	addiu	$v0,$v0,0
    1540:	0040f809 	jalr	$v0
    1544:	27a50010 	addiu	$a1,$sp,16
    1548:	40136000 	mfc0	$s3,$12
    154c:	00000000 	nop
    1550:	36610001 	ori	$at,$s3,0x1
    1554:	38210001 	xori	$at,$at,0x1
    1558:	40816000 	mtc0	$at,$12
    155c:	00000040 	sll	$zero,$zero,0x1
    1560:	00000040 	sll	$zero,$zero,0x1
    1564:	00000040 	sll	$zero,$zero,0x1
    1568:	2402fe00 	li	$v0,-512
    156c:	12a20014 	beq	$s5,$v0,15c0 <mxp_task_sleep+0x1264>
    1570:	00111100 	sll	$v0,$s1,0x4
    1574:	00511023 	subu	$v0,$v0,$s1
    1578:	00021080 	sll	$v0,$v0,0x2
    157c:	3c010003 	lui	$at,0x3
    1580:	00220821 	addu	$at,$at,$v0
    1584:	8c220010 	lw	$v0,16($at)
    1588:	1440ff85 	bnez	$v0,13a0 <mxp_task_sleep+0x1044>
    158c:	00118100 	sll	$s0,$s1,0x4
    1590:	40016000 	mfc0	$at,$12
    1594:	32730001 	andi	$s3,$s3,0x1
    1598:	34210001 	ori	$at,$at,0x1
    159c:	38210001 	xori	$at,$at,0x1
    15a0:	02619825 	or	$s3,$s3,$at
    15a4:	40936000 	mtc0	$s3,$12
	...
    15b4:	3c040000 	lui	$a0,0x0
    15b8:	08000580 	j	1600 <mxp_task_sleep+0x12a4>
    15bc:	24840204 	addiu	$a0,$a0,516
    15c0:	00511023 	subu	$v0,$v0,$s1
    15c4:	00021080 	sll	$v0,$v0,0x2
    15c8:	3c010003 	lui	$at,0x3
    15cc:	00220821 	addu	$at,$at,$v0
    15d0:	ac200018 	sw	$zero,24($at)
    15d4:	40016000 	mfc0	$at,$12
    15d8:	32730001 	andi	$s3,$s3,0x1
    15dc:	34210001 	ori	$at,$at,0x1
    15e0:	38210001 	xori	$at,$at,0x1
    15e4:	02619825 	or	$s3,$s3,$at
    15e8:	40936000 	mtc0	$s3,$12
	...
    15f8:	3c040000 	lui	$a0,0x0
    15fc:	248401c8 	addiu	$a0,$a0,456
    1600:	3c020000 	lui	$v0,0x0
    1604:	24420000 	addiu	$v0,$v0,0
    1608:	0040f809 	jalr	$v0
    160c:	02202821 	move	$a1,$s1
    1610:	2402001f 	li	$v0,31
    1614:	8fbf0038 	lw	$ra,56($sp)
    1618:	8fb50034 	lw	$s5,52($sp)
    161c:	8fb40030 	lw	$s4,48($sp)
    1620:	8fb3002c 	lw	$s3,44($sp)
    1624:	8fb20028 	lw	$s2,40($sp)
    1628:	8fb10024 	lw	$s1,36($sp)
    162c:	8fb00020 	lw	$s0,32($sp)
    1630:	03e00008 	jr	$ra
    1634:	27bd0040 	addiu	$sp,$sp,64
    1638:	27bdffe0 	addiu	$sp,$sp,-32
    163c:	afbf0018 	sw	$ra,24($sp)
    1640:	afb10014 	sw	$s1,20($sp)
    1644:	afb00010 	sw	$s0,16($sp)
    1648:	00808021 	move	$s0,$a0
    164c:	40116000 	mfc0	$s1,$12
    1650:	00000000 	nop
    1654:	36210001 	ori	$at,$s1,0x1
    1658:	38210001 	xori	$at,$at,0x1
    165c:	40816000 	mtc0	$at,$12
    1660:	00000040 	sll	$zero,$zero,0x1
    1664:	00000040 	sll	$zero,$zero,0x1
    1668:	00000040 	sll	$zero,$zero,0x1
    166c:	3c020000 	lui	$v0,0x0
    1670:	24420ce8 	addiu	$v0,$v0,3304
    1674:	0040f809 	jalr	$v0
    1678:	2604001c 	addiu	$a0,$s0,28
    167c:	ae020004 	sw	$v0,4($s0)
    1680:	2c420001 	sltiu	$v0,$v0,1
    1684:	40016000 	mfc0	$at,$12
    1688:	32310001 	andi	$s1,$s1,0x1
    168c:	34210001 	ori	$at,$at,0x1
    1690:	38210001 	xori	$at,$at,0x1
    1694:	02218825 	or	$s1,$s1,$at
    1698:	40916000 	mtc0	$s1,$12
	...
    16a8:	8fbf0018 	lw	$ra,24($sp)
    16ac:	8fb10014 	lw	$s1,20($sp)
    16b0:	8fb00010 	lw	$s0,16($sp)
    16b4:	03e00008 	jr	$ra
    16b8:	27bd0020 	addiu	$sp,$sp,32
    16bc:	8c830004 	lw	$v1,4($a0)
    16c0:	40066000 	mfc0	$a2,$12
    16c4:	00000000 	nop
    16c8:	34c10001 	ori	$at,$a2,0x1
    16cc:	38210001 	xori	$at,$at,0x1
    16d0:	40816000 	mtc0	$at,$12
    16d4:	00000040 	sll	$zero,$zero,0x1
    16d8:	00000040 	sll	$zero,$zero,0x1
    16dc:	00000040 	sll	$zero,$zero,0x1
    16e0:	2462ffff 	addiu	$v0,$v1,-1
    16e4:	2c4203ff 	sltiu	$v0,$v0,1023
    16e8:	1040000a 	beqz	$v0,1714 <mxp_task_sleep+0x13b8>
    16ec:	00031100 	sll	$v0,$v1,0x4
    16f0:	00431023 	subu	$v0,$v0,$v1
    16f4:	00022880 	sll	$a1,$v0,0x2
    16f8:	3c020003 	lui	$v0,0x3
    16fc:	00451021 	addu	$v0,$v0,$a1
    1700:	8c420040 	lw	$v0,64($v0)
    1704:	3c030003 	lui	$v1,0x3
    1708:	24630040 	addiu	$v1,$v1,64
    170c:	1440000c 	bnez	$v0,1740 <mxp_task_sleep+0x13e4>
    1710:	00651021 	addu	$v0,$v1,$a1
    1714:	40016000 	mfc0	$at,$12
    1718:	30c60001 	andi	$a2,$a2,0x1
    171c:	34210001 	ori	$at,$at,0x1
    1720:	38210001 	xori	$at,$at,0x1
    1724:	00c13025 	or	$a2,$a2,$at
    1728:	40866000 	mtc0	$a2,$12
	...
    1738:	03e00008 	jr	$ra
    173c:	24020008 	li	$v0,8
    1740:	8c42ffd0 	lw	$v0,-48($v0)
    1744:	ac820010 	sw	$v0,16($a0)
    1748:	40016000 	mfc0	$at,$12
    174c:	30c60001 	andi	$a2,$a2,0x1
    1750:	34210001 	ori	$at,$at,0x1
    1754:	38210001 	xori	$at,$at,0x1
    1758:	00c13025 	or	$a2,$a2,$at
    175c:	40866000 	mtc0	$a2,$12
	...
    176c:	03e00008 	jr	$ra
    1770:	00001021 	move	$v0,$zero
    1774:	27bdffc8 	addiu	$sp,$sp,-56
    1778:	afbf0034 	sw	$ra,52($sp)
    177c:	afb40030 	sw	$s4,48($sp)
    1780:	afb3002c 	sw	$s3,44($sp)
    1784:	afb20028 	sw	$s2,40($sp)
    1788:	afb10024 	sw	$s1,36($sp)
    178c:	afb00020 	sw	$s0,32($sp)
    1790:	3c050000 	lui	$a1,0x0
    1794:	24a50234 	addiu	$a1,$a1,564
    1798:	3c130000 	lui	$s3,0x0
    179c:	26730000 	addiu	$s3,$s3,0
    17a0:	0260f809 	jalr	$s3
    17a4:	0080a021 	move	$s4,$a0
    17a8:	00408821 	move	$s1,$v0
    17ac:	24120001 	li	$s2,1
    17b0:	2410003c 	li	$s0,60
    17b4:	3c020003 	lui	$v0,0x3
    17b8:	00501021 	addu	$v0,$v0,$s0
    17bc:	8c420040 	lw	$v0,64($v0)
    17c0:	3c060003 	lui	$a2,0x3
    17c4:	24c60040 	addiu	$a2,$a2,64
    17c8:	10400011 	beqz	$v0,1810 <mxp_task_sleep+0x14b4>
    17cc:	24c2ffdc 	addiu	$v0,$a2,-36
    17d0:	02021021 	addu	$v0,$s0,$v0
    17d4:	afa20018 	sw	$v0,24($sp)
    17d8:	02912021 	addu	$a0,$s4,$s1
    17dc:	3c050000 	lui	$a1,0x0
    17e0:	24a50248 	addiu	$a1,$a1,584
    17e4:	00d01021 	addu	$v0,$a2,$s0
    17e8:	00401821 	move	$v1,$v0
    17ec:	8c42ffd4 	lw	$v0,-44($v0)
    17f0:	8c63ffd0 	lw	$v1,-48($v1)
    17f4:	afa20010 	sw	$v0,16($sp)
    17f8:	afa30014 	sw	$v1,20($sp)
    17fc:	00d01021 	addu	$v0,$a2,$s0
    1800:	8c47ffec 	lw	$a3,-20($v0)
    1804:	0260f809 	jalr	$s3
    1808:	02403021 	move	$a2,$s2
    180c:	02228821 	addu	$s1,$s1,$v0
    1810:	26520001 	addiu	$s2,$s2,1
    1814:	2a420400 	slti	$v0,$s2,1024
    1818:	1440ffe6 	bnez	$v0,17b4 <mxp_task_sleep+0x1458>
    181c:	2610003c 	addiu	$s0,$s0,60
    1820:	24030001 	li	$v1,1
    1824:	8fa20048 	lw	$v0,72($sp)
    1828:	8fbf0034 	lw	$ra,52($sp)
    182c:	8fb40030 	lw	$s4,48($sp)
    1830:	8fb3002c 	lw	$s3,44($sp)
    1834:	8fb20028 	lw	$s2,40($sp)
    1838:	8fb00020 	lw	$s0,32($sp)
    183c:	ac430000 	sw	$v1,0($v0)
    1840:	02201021 	move	$v0,$s1
    1844:	8fb10024 	lw	$s1,36($sp)
    1848:	03e00008 	jr	$ra
    184c:	27bd0038 	addiu	$sp,$sp,56

0000000000001850 <mxl_tmr_init>:
    1850:	27bdffe8 	addiu	$sp,$sp,-24
    1854:	afbf0010 	sw	$ra,16($sp)
    1858:	3c040004 	lui	$a0,0x4
    185c:	2484f008 	addiu	$a0,$a0,-4088
    1860:	00002821 	move	$a1,$zero
    1864:	3c020000 	lui	$v0,0x0
    1868:	24420000 	addiu	$v0,$v0,0
    186c:	0040f809 	jalr	$v0
    1870:	24066fb8 	li	$a2,28600
    1874:	8fbf0010 	lw	$ra,16($sp)
    1878:	03e00008 	jr	$ra
    187c:	27bd0018 	addiu	$sp,$sp,24
    1880:	00002021 	move	$a0,$zero
    1884:	00001821 	move	$v1,$zero
    1888:	3c020004 	lui	$v0,0x4
    188c:	00431021 	addu	$v0,$v0,$v1
    1890:	8c42f008 	lw	$v0,-4088($v0)
    1894:	54400003 	0x54400003
    1898:	24840001 	addiu	$a0,$a0,1
    189c:	03e00008 	jr	$ra
    18a0:	00801021 	move	$v0,$a0
    18a4:	2882028a 	slti	$v0,$a0,650
    18a8:	1440fff7 	bnez	$v0,1888 <mxl_tmr_init+0x38>
    18ac:	2463002c 	addiu	$v1,$v1,44
    18b0:	3c020001 	lui	$v0,0x1
    18b4:	03e00008 	jr	$ra
    18b8:	3442869f 	ori	$v0,$v0,0x869f
    18bc:	27bdffe0 	addiu	$sp,$sp,-32
    18c0:	afbf0018 	sw	$ra,24($sp)
    18c4:	afb10014 	sw	$s1,20($sp)
    18c8:	afb00010 	sw	$s0,16($sp)
    18cc:	00808021 	move	$s0,$a0
    18d0:	40116000 	mfc0	$s1,$12
    18d4:	00000000 	nop
    18d8:	36210001 	ori	$at,$s1,0x1
    18dc:	38210001 	xori	$at,$at,0x1
    18e0:	40816000 	mtc0	$at,$12
    18e4:	00000040 	sll	$zero,$zero,0x1
    18e8:	00000040 	sll	$zero,$zero,0x1
    18ec:	00000040 	sll	$zero,$zero,0x1
    18f0:	3c020000 	lui	$v0,0x0
    18f4:	24421880 	addiu	$v0,$v0,6272
    18f8:	0040f809 	jalr	$v0
    18fc:	00000000 	nop
    1900:	3c030001 	lui	$v1,0x1
    1904:	3463869f 	ori	$v1,$v1,0x869f
    1908:	00402021 	move	$a0,$v0
    190c:	10830025 	beq	$a0,$v1,19a4 <mxl_tmr_init+0x154>
    1910:	00041040 	sll	$v0,$a0,0x1
    1914:	00441021 	addu	$v0,$v0,$a0
    1918:	00021080 	sll	$v0,$v0,0x2
    191c:	00441023 	subu	$v0,$v0,$a0
    1920:	00021080 	sll	$v0,$v0,0x2
    1924:	24030001 	li	$v1,1
    1928:	3c010004 	lui	$at,0x4
    192c:	00220821 	addu	$at,$at,$v0
    1930:	ac23f008 	sw	$v1,-4088($at)
    1934:	8e03000c 	lw	$v1,12($s0)
    1938:	3c010004 	lui	$at,0x4
    193c:	00220821 	addu	$at,$at,$v0
    1940:	ac23f018 	sw	$v1,-4072($at)
    1944:	8e030010 	lw	$v1,16($s0)
    1948:	3c010004 	lui	$at,0x4
    194c:	00220821 	addu	$at,$at,$v0
    1950:	ac23f01c 	sw	$v1,-4068($at)
    1954:	8e030014 	lw	$v1,20($s0)
    1958:	3c010004 	lui	$at,0x4
    195c:	00220821 	addu	$at,$at,$v0
    1960:	ac23f010 	sw	$v1,-4080($at)
    1964:	8e030008 	lw	$v1,8($s0)
    1968:	3c010004 	lui	$at,0x4
    196c:	00220821 	addu	$at,$at,$v0
    1970:	ac23f014 	sw	$v1,-4076($at)
    1974:	ae040004 	sw	$a0,4($s0)
    1978:	40016000 	mfc0	$at,$12
    197c:	32310001 	andi	$s1,$s1,0x1
    1980:	34210001 	ori	$at,$at,0x1
    1984:	38210001 	xori	$at,$at,0x1
    1988:	02218825 	or	$s1,$s1,$at
    198c:	40916000 	mtc0	$s1,$12
	...
    199c:	08000673 	j	19cc <mxl_tmr_init+0x17c>
    19a0:	00001021 	move	$v0,$zero
    19a4:	40016000 	mfc0	$at,$12
    19a8:	32310001 	andi	$s1,$s1,0x1
    19ac:	34210001 	ori	$at,$at,0x1
    19b0:	38210001 	xori	$at,$at,0x1
    19b4:	02218825 	or	$s1,$s1,$at
    19b8:	40916000 	mtc0	$s1,$12
	...
    19c8:	24020019 	li	$v0,25
    19cc:	8fbf0018 	lw	$ra,24($sp)
    19d0:	8fb10014 	lw	$s1,20($sp)
    19d4:	8fb00010 	lw	$s0,16($sp)
    19d8:	03e00008 	jr	$ra
    19dc:	27bd0020 	addiu	$sp,$sp,32
    19e0:	27bdffb0 	addiu	$sp,$sp,-80
    19e4:	afbf0048 	sw	$ra,72($sp)
    19e8:	afb10044 	sw	$s1,68($sp)
    19ec:	afb00040 	sw	$s0,64($sp)
    19f0:	8c900010 	lw	$s0,16($a0)
    19f4:	40116000 	mfc0	$s1,$12
    19f8:	00000000 	nop
    19fc:	36210001 	ori	$at,$s1,0x1
    1a00:	38210001 	xori	$at,$at,0x1
    1a04:	40816000 	mtc0	$at,$12
    1a08:	00000040 	sll	$zero,$zero,0x1
    1a0c:	00000040 	sll	$zero,$zero,0x1
    1a10:	00000040 	sll	$zero,$zero,0x1
    1a14:	8e050004 	lw	$a1,4($s0)
    1a18:	2402ffff 	li	$v0,-1
    1a1c:	10a2000b 	beq	$a1,$v0,1a4c <mxl_tmr_init+0x1fc>
    1a20:	24020002 	li	$v0,2
    1a24:	10a00009 	beqz	$a1,1a4c <mxl_tmr_init+0x1fc>
    1a28:	26040018 	addiu	$a0,$s0,24
    1a2c:	3c060000 	lui	$a2,0x0
    1a30:	24c619e0 	addiu	$a2,$a2,6624
    1a34:	3c020000 	lui	$v0,0x0
    1a38:	24422e70 	addiu	$v0,$v0,11888
    1a3c:	0040f809 	jalr	$v0
    1a40:	02003821 	move	$a3,$s0
    1a44:	08000695 	j	1a54 <mxl_tmr_init+0x204>
    1a48:	8e02000c 	lw	$v0,12($s0)
    1a4c:	ae020000 	sw	$v0,0($s0)
    1a50:	8e02000c 	lw	$v0,12($s0)
    1a54:	50400014 	0x50400014
    1a58:	8e020010 	lw	$v0,16($s0)
    1a5c:	8e020008 	lw	$v0,8($s0)
    1a60:	afa20014 	sw	$v0,20($sp)
    1a64:	8e02000c 	lw	$v0,12($s0)
    1a68:	afa20018 	sw	$v0,24($sp)
    1a6c:	40016000 	mfc0	$at,$12
    1a70:	32310001 	andi	$s1,$s1,0x1
    1a74:	34210001 	ori	$at,$at,0x1
    1a78:	38210001 	xori	$at,$at,0x1
    1a7c:	02218825 	or	$s1,$s1,$at
    1a80:	40916000 	mtc0	$s1,$12
	...
    1a90:	3c020000 	lui	$v0,0x0
    1a94:	24420480 	addiu	$v0,$v0,1152
    1a98:	0040f809 	jalr	$v0
    1a9c:	27a40010 	addiu	$a0,$sp,16
    1aa0:	080006bb 	j	1aec <mxl_tmr_init+0x29c>
    1aa4:	8fbf0048 	lw	$ra,72($sp)
    1aa8:	afa20014 	sw	$v0,20($sp)
    1aac:	8e020014 	lw	$v0,20($s0)
    1ab0:	afa2001c 	sw	$v0,28($sp)
    1ab4:	40016000 	mfc0	$at,$12
    1ab8:	32310001 	andi	$s1,$s1,0x1
    1abc:	34210001 	ori	$at,$at,0x1
    1ac0:	38210001 	xori	$at,$at,0x1
    1ac4:	02218825 	or	$s1,$s1,$at
    1ac8:	40916000 	mtc0	$s1,$12
	...
    1ad8:	3c020000 	lui	$v0,0x0
    1adc:	244210c0 	addiu	$v0,$v0,4288
    1ae0:	0040f809 	jalr	$v0
    1ae4:	27a40010 	addiu	$a0,$sp,16
    1ae8:	8fbf0048 	lw	$ra,72($sp)
    1aec:	8fb10044 	lw	$s1,68($sp)
    1af0:	8fb00040 	lw	$s0,64($sp)
    1af4:	03e00008 	jr	$ra
    1af8:	27bd0050 	addiu	$sp,$sp,80

0000000000001afc <mxp_tmrStart>:
    1afc:	27bdffe8 	addiu	$sp,$sp,-24
    1b00:	afbf0014 	sw	$ra,20($sp)
    1b04:	afb00010 	sw	$s0,16($sp)
    1b08:	00804021 	move	$t0,$a0
    1b0c:	40106000 	mfc0	$s0,$12
    1b10:	00000000 	nop
    1b14:	36010001 	ori	$at,$s0,0x1
    1b18:	38210001 	xori	$at,$at,0x1
    1b1c:	40816000 	mtc0	$at,$12
    1b20:	00000040 	sll	$zero,$zero,0x1
    1b24:	00000040 	sll	$zero,$zero,0x1
    1b28:	00000040 	sll	$zero,$zero,0x1
    1b2c:	8d030004 	lw	$v1,4($t0)
    1b30:	2862028a 	slti	$v0,$v1,650
    1b34:	1040000b 	beqz	$v0,1b64 <mxp_tmrStart+0x68>
    1b38:	00031040 	sll	$v0,$v1,0x1
    1b3c:	00431021 	addu	$v0,$v0,$v1
    1b40:	00021080 	sll	$v0,$v0,0x2
    1b44:	00431023 	subu	$v0,$v0,$v1
    1b48:	00022880 	sll	$a1,$v0,0x2
    1b4c:	3c040004 	lui	$a0,0x4
    1b50:	2484f008 	addiu	$a0,$a0,-4088
    1b54:	00a43821 	addu	$a3,$a1,$a0
    1b58:	8ce20000 	lw	$v0,0($a3)
    1b5c:	1440000c 	bnez	$v0,1b90 <mxp_tmrStart+0x94>
    1b60:	00851021 	addu	$v0,$a0,$a1
    1b64:	40016000 	mfc0	$at,$12
    1b68:	32100001 	andi	$s0,$s0,0x1
    1b6c:	34210001 	ori	$at,$at,0x1
    1b70:	38210001 	xori	$at,$at,0x1
    1b74:	02018025 	or	$s0,$s0,$at
    1b78:	40906000 	mtc0	$s0,$12
	...
    1b88:	080006fa 	j	1be8 <mxp_tmrStart+0xec>
    1b8c:	2402001a 	li	$v0,26
    1b90:	24840018 	addiu	$a0,$a0,24
    1b94:	00a42021 	addu	$a0,$a1,$a0
    1b98:	8d03001c 	lw	$v1,28($t0)
    1b9c:	3c060000 	lui	$a2,0x0
    1ba0:	24c619e0 	addiu	$a2,$a2,6624
    1ba4:	ac430004 	sw	$v1,4($v0)
    1ba8:	24020003 	li	$v0,3
    1bac:	ace20000 	sw	$v0,0($a3)
    1bb0:	3c020000 	lui	$v0,0x0
    1bb4:	24422e70 	addiu	$v0,$v0,11888
    1bb8:	0040f809 	jalr	$v0
    1bbc:	8d050018 	lw	$a1,24($t0)
    1bc0:	40016000 	mfc0	$at,$12
    1bc4:	32100001 	andi	$s0,$s0,0x1
    1bc8:	34210001 	ori	$at,$at,0x1
    1bcc:	38210001 	xori	$at,$at,0x1
    1bd0:	02018025 	or	$s0,$s0,$at
    1bd4:	40906000 	mtc0	$s0,$12
	...
    1be4:	00001021 	move	$v0,$zero
    1be8:	8fbf0014 	lw	$ra,20($sp)
    1bec:	8fb00010 	lw	$s0,16($sp)
    1bf0:	03e00008 	jr	$ra
    1bf4:	27bd0018 	addiu	$sp,$sp,24

0000000000001bf8 <mxp_tmrAbort>:
    1bf8:	27bdffd0 	addiu	$sp,$sp,-48
    1bfc:	afbf0028 	sw	$ra,40($sp)
    1c00:	afb50024 	sw	$s5,36($sp)
    1c04:	afb40020 	sw	$s4,32($sp)
    1c08:	afb3001c 	sw	$s3,28($sp)
    1c0c:	afb20018 	sw	$s2,24($sp)
    1c10:	afb10014 	sw	$s1,20($sp)
    1c14:	afb00010 	sw	$s0,16($sp)
    1c18:	0000a021 	move	$s4,$zero
    1c1c:	40156000 	mfc0	$s5,$12
    1c20:	00000000 	nop
    1c24:	36a10001 	ori	$at,$s5,0x1
    1c28:	38210001 	xori	$at,$at,0x1
    1c2c:	40816000 	mtc0	$at,$12
    1c30:	00000040 	sll	$zero,$zero,0x1
    1c34:	00000040 	sll	$zero,$zero,0x1
    1c38:	00000040 	sll	$zero,$zero,0x1
    1c3c:	8c900004 	lw	$s0,4($a0)
    1c40:	2a02028a 	slti	$v0,$s0,650
    1c44:	1040000b 	beqz	$v0,1c74 <mxp_tmrAbort+0x7c>
    1c48:	00109840 	sll	$s3,$s0,0x1
    1c4c:	02701021 	addu	$v0,$s3,$s0
    1c50:	00021080 	sll	$v0,$v0,0x2
    1c54:	00501023 	subu	$v0,$v0,$s0
    1c58:	00022880 	sll	$a1,$v0,0x2
    1c5c:	3c120004 	lui	$s2,0x4
    1c60:	2652f008 	addiu	$s2,$s2,-4088
    1c64:	00b28821 	addu	$s1,$a1,$s2
    1c68:	8e230000 	lw	$v1,0($s1)
    1c6c:	1460000c 	bnez	$v1,1ca0 <mxp_tmrAbort+0xa8>
    1c70:	24020003 	li	$v0,3
    1c74:	40016000 	mfc0	$at,$12
    1c78:	32b50001 	andi	$s5,$s5,0x1
    1c7c:	34210001 	ori	$at,$at,0x1
    1c80:	38210001 	xori	$at,$at,0x1
    1c84:	02a1a825 	or	$s5,$s5,$at
    1c88:	40956000 	mtc0	$s5,$12
	...
    1c98:	08000748 	j	1d20 <mxp_tmrAbort+0x128>
    1c9c:	2402001a 	li	$v0,26
    1ca0:	54620007 	0x54620007
    1ca4:	8e230000 	lw	$v1,0($s1)
    1ca8:	26440018 	addiu	$a0,$s2,24
    1cac:	3c020000 	lui	$v0,0x0
    1cb0:	24422d80 	addiu	$v0,$v0,11648
    1cb4:	0040f809 	jalr	$v0
    1cb8:	00a42021 	addu	$a0,$a1,$a0
    1cbc:	8e230000 	lw	$v1,0($s1)
    1cc0:	24020002 	li	$v0,2
    1cc4:	14620003 	bne	$v1,$v0,1cd4 <mxp_tmrAbort+0xdc>
    1cc8:	24020001 	li	$v0,1
    1ccc:	08000737 	j	1cdc <mxp_tmrAbort+0xe4>
    1cd0:	2414001b 	li	$s4,27
    1cd4:	50620001 	0x50620001
    1cd8:	2414001c 	li	$s4,28
    1cdc:	02701021 	addu	$v0,$s3,$s0
    1ce0:	00021080 	sll	$v0,$v0,0x2
    1ce4:	00501023 	subu	$v0,$v0,$s0
    1ce8:	00021080 	sll	$v0,$v0,0x2
    1cec:	00521021 	addu	$v0,$v0,$s2
    1cf0:	24030001 	li	$v1,1
    1cf4:	ac430000 	sw	$v1,0($v0)
    1cf8:	40016000 	mfc0	$at,$12
    1cfc:	32b50001 	andi	$s5,$s5,0x1
    1d00:	34210001 	ori	$at,$at,0x1
    1d04:	38210001 	xori	$at,$at,0x1
    1d08:	02a1a825 	or	$s5,$s5,$at
    1d0c:	40956000 	mtc0	$s5,$12
	...
    1d1c:	02801021 	move	$v0,$s4
    1d20:	8fbf0028 	lw	$ra,40($sp)
    1d24:	8fb50024 	lw	$s5,36($sp)
    1d28:	8fb40020 	lw	$s4,32($sp)
    1d2c:	8fb3001c 	lw	$s3,28($sp)
    1d30:	8fb20018 	lw	$s2,24($sp)
    1d34:	8fb10014 	lw	$s1,20($sp)
    1d38:	8fb00010 	lw	$s0,16($sp)
    1d3c:	03e00008 	jr	$ra
    1d40:	27bd0030 	addiu	$sp,$sp,48

0000000000001d44 <mxp_tmrDelete>:
    1d44:	27bdffe0 	addiu	$sp,$sp,-32
    1d48:	afbf0018 	sw	$ra,24($sp)
    1d4c:	afb10014 	sw	$s1,20($sp)
    1d50:	afb00010 	sw	$s0,16($sp)
    1d54:	40116000 	mfc0	$s1,$12
    1d58:	00000000 	nop
    1d5c:	36210001 	ori	$at,$s1,0x1
    1d60:	38210001 	xori	$at,$at,0x1
    1d64:	40816000 	mtc0	$at,$12
    1d68:	00000040 	sll	$zero,$zero,0x1
    1d6c:	00000040 	sll	$zero,$zero,0x1
    1d70:	00000040 	sll	$zero,$zero,0x1
    1d74:	8c830004 	lw	$v1,4($a0)
    1d78:	2862028a 	slti	$v0,$v1,650
    1d7c:	1040000b 	beqz	$v0,1dac <mxp_tmrDelete+0x68>
    1d80:	00031040 	sll	$v0,$v1,0x1
    1d84:	00431021 	addu	$v0,$v0,$v1
    1d88:	00021080 	sll	$v0,$v0,0x2
    1d8c:	00431023 	subu	$v0,$v0,$v1
    1d90:	00022880 	sll	$a1,$v0,0x2
    1d94:	3c040004 	lui	$a0,0x4
    1d98:	2484f008 	addiu	$a0,$a0,-4088
    1d9c:	00a48021 	addu	$s0,$a1,$a0
    1da0:	8e030000 	lw	$v1,0($s0)
    1da4:	1460000c 	bnez	$v1,1dd8 <mxp_tmrDelete+0x94>
    1da8:	24020003 	li	$v0,3
    1dac:	40016000 	mfc0	$at,$12
    1db0:	32310001 	andi	$s1,$s1,0x1
    1db4:	34210001 	ori	$at,$at,0x1
    1db8:	38210001 	xori	$at,$at,0x1
    1dbc:	02218825 	or	$s1,$s1,$at
    1dc0:	40916000 	mtc0	$s1,$12
	...
    1dd0:	08000788 	j	1e20 <mxp_tmrDelete+0xdc>
    1dd4:	2402001a 	li	$v0,26
    1dd8:	54620007 	0x54620007
    1ddc:	ae000000 	sw	$zero,0($s0)
    1de0:	24840018 	addiu	$a0,$a0,24
    1de4:	3c020000 	lui	$v0,0x0
    1de8:	24422d80 	addiu	$v0,$v0,11648
    1dec:	0040f809 	jalr	$v0
    1df0:	00a42021 	addu	$a0,$a1,$a0
    1df4:	ae000000 	sw	$zero,0($s0)
    1df8:	40016000 	mfc0	$at,$12
    1dfc:	32310001 	andi	$s1,$s1,0x1
    1e00:	34210001 	ori	$at,$at,0x1
    1e04:	38210001 	xori	$at,$at,0x1
    1e08:	02218825 	or	$s1,$s1,$at
    1e0c:	40916000 	mtc0	$s1,$12
	...
    1e1c:	00001021 	move	$v0,$zero
    1e20:	8fbf0018 	lw	$ra,24($sp)
    1e24:	8fb10014 	lw	$s1,20($sp)
    1e28:	8fb00010 	lw	$s0,16($sp)
    1e2c:	03e00008 	jr	$ra
    1e30:	27bd0020 	addiu	$sp,$sp,32
    1e34:	3c020000 	lui	$v0,0x0
    1e38:	8c42003c 	lw	$v0,60($v0)
    1e3c:	ac820018 	sw	$v0,24($a0)
    1e40:	03e00008 	jr	$ra
    1e44:	00001021 	move	$v0,$zero

0000000000001e48 <mxp_syscall>:
    1e48:	27bdffe8 	addiu	$sp,$sp,-24
    1e4c:	afbf0010 	sw	$ra,16($sp)
    1e50:	8c830000 	lw	$v1,0($a0)
    1e54:	24020402 	li	$v0,1026
    1e58:	1062006c 	beq	$v1,$v0,200c <mxp_syscall+0x1c4>
    1e5c:	28620403 	slti	$v0,$v1,1027
    1e60:	1040001f 	beqz	$v0,1ee0 <mxp_syscall+0x98>
    1e64:	24020301 	li	$v0,769
    1e68:	1062004a 	beq	$v1,$v0,1f94 <mxp_syscall+0x14c>
    1e6c:	28620302 	slti	$v0,$v1,770
    1e70:	10400010 	beqz	$v0,1eb4 <mxp_syscall+0x6c>
    1e74:	24020201 	li	$v0,513
    1e78:	106200a6 	beq	$v1,$v0,2114 <mxp_syscall+0x2cc>
    1e7c:	28620202 	slti	$v0,$v1,514
    1e80:	10400005 	beqz	$v0,1e98 <mxp_syscall+0x50>
    1e84:	24020200 	li	$v0,512
    1e88:	10620096 	beq	$v1,$v0,20e4 <mxp_syscall+0x29c>
    1e8c:	2402fffe 	li	$v0,-2
    1e90:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1e94:	8fbf0010 	lw	$ra,16($sp)
    1e98:	24020202 	li	$v0,514
    1e9c:	106200a3 	beq	$v1,$v0,212c <mxp_syscall+0x2e4>
    1ea0:	24020203 	li	$v0,515
    1ea4:	10620095 	beq	$v1,$v0,20fc <mxp_syscall+0x2b4>
    1ea8:	2402fffe 	li	$v0,-2
    1eac:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1eb0:	8fbf0010 	lw	$ra,16($sp)
    1eb4:	24020303 	li	$v0,771
    1eb8:	10620042 	beq	$v1,$v0,1fc4 <mxp_syscall+0x17c>
    1ebc:	28620303 	slti	$v0,$v1,771
    1ec0:	1440003a 	bnez	$v0,1fac <mxp_syscall+0x164>
    1ec4:	24020304 	li	$v0,772
    1ec8:	10620044 	beq	$v1,$v0,1fdc <mxp_syscall+0x194>
    1ecc:	24020401 	li	$v0,1025
    1ed0:	10620048 	beq	$v1,$v0,1ff4 <mxp_syscall+0x1ac>
    1ed4:	2402fffe 	li	$v0,-2
    1ed8:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1edc:	8fbf0010 	lw	$ra,16($sp)
    1ee0:	24020407 	li	$v0,1031
    1ee4:	1062005b 	beq	$v1,$v0,2054 <mxp_syscall+0x20c>
    1ee8:	28620408 	slti	$v0,$v1,1032
    1eec:	1040000b 	beqz	$v0,1f1c <mxp_syscall+0xd4>
    1ef0:	24020404 	li	$v0,1028
    1ef4:	10620021 	beq	$v1,$v0,1f7c <mxp_syscall+0x134>
    1ef8:	28620404 	slti	$v0,$v1,1028
    1efc:	14400019 	bnez	$v0,1f64 <mxp_syscall+0x11c>
    1f00:	24020405 	li	$v0,1029
    1f04:	10620047 	beq	$v1,$v0,2024 <mxp_syscall+0x1dc>
    1f08:	24020406 	li	$v0,1030
    1f0c:	1062004b 	beq	$v1,$v0,203c <mxp_syscall+0x1f4>
    1f10:	2402fffe 	li	$v0,-2
    1f14:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1f18:	8fbf0010 	lw	$ra,16($sp)
    1f1c:	24020a02 	li	$v0,2562
    1f20:	1062005e 	beq	$v1,$v0,209c <mxp_syscall+0x254>
    1f24:	28620a03 	slti	$v0,$v1,2563
    1f28:	10400007 	beqz	$v0,1f48 <mxp_syscall+0x100>
    1f2c:	24020a00 	li	$v0,2560
    1f30:	1062004e 	beq	$v1,$v0,206c <mxp_syscall+0x224>
    1f34:	24020a01 	li	$v0,2561
    1f38:	10620052 	beq	$v1,$v0,2084 <mxp_syscall+0x23c>
    1f3c:	2402fffe 	li	$v0,-2
    1f40:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1f44:	8fbf0010 	lw	$ra,16($sp)
    1f48:	24020a03 	li	$v0,2563
    1f4c:	10620059 	beq	$v1,$v0,20b4 <mxp_syscall+0x26c>
    1f50:	24020a04 	li	$v0,2564
    1f54:	1062005d 	beq	$v1,$v0,20cc <mxp_syscall+0x284>
    1f58:	2402fffe 	li	$v0,-2
    1f5c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1f60:	8fbf0010 	lw	$ra,16($sp)
    1f64:	3c020000 	lui	$v0,0x0
    1f68:	244210c0 	addiu	$v0,$v0,4288
    1f6c:	0040f809 	jalr	$v0
    1f70:	00000000 	nop
    1f74:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1f78:	8fbf0010 	lw	$ra,16($sp)
    1f7c:	3c020000 	lui	$v0,0x0
    1f80:	244212e8 	addiu	$v0,$v0,4840
    1f84:	0040f809 	jalr	$v0
    1f88:	00000000 	nop
    1f8c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1f90:	8fbf0010 	lw	$ra,16($sp)
    1f94:	3c020000 	lui	$v0,0x0
    1f98:	24420480 	addiu	$v0,$v0,1152
    1f9c:	0040f809 	jalr	$v0
    1fa0:	00000000 	nop
    1fa4:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1fa8:	8fbf0010 	lw	$ra,16($sp)
    1fac:	3c020000 	lui	$v0,0x0
    1fb0:	244205e0 	addiu	$v0,$v0,1504
    1fb4:	0040f809 	jalr	$v0
    1fb8:	00000000 	nop
    1fbc:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1fc0:	8fbf0010 	lw	$ra,16($sp)
    1fc4:	3c020000 	lui	$v0,0x0
    1fc8:	244209e8 	addiu	$v0,$v0,2536
    1fcc:	0040f809 	jalr	$v0
    1fd0:	00000000 	nop
    1fd4:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1fd8:	8fbf0010 	lw	$ra,16($sp)
    1fdc:	3c020000 	lui	$v0,0x0
    1fe0:	24420aa4 	addiu	$v0,$v0,2724
    1fe4:	0040f809 	jalr	$v0
    1fe8:	00000000 	nop
    1fec:	08000850 	j	2140 <mxp_syscall+0x2f8>
    1ff0:	8fbf0010 	lw	$ra,16($sp)
    1ff4:	3c020000 	lui	$v0,0x0
    1ff8:	24420d74 	addiu	$v0,$v0,3444
    1ffc:	0040f809 	jalr	$v0
    2000:	00000000 	nop
    2004:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2008:	8fbf0010 	lw	$ra,16($sp)
    200c:	3c020000 	lui	$v0,0x0
    2010:	24420eec 	addiu	$v0,$v0,3820
    2014:	0040f809 	jalr	$v0
    2018:	00000000 	nop
    201c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2020:	8fbf0010 	lw	$ra,16($sp)
    2024:	3c020000 	lui	$v0,0x0
    2028:	24421638 	addiu	$v0,$v0,5688
    202c:	0040f809 	jalr	$v0
    2030:	00000000 	nop
    2034:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2038:	8fbf0010 	lw	$ra,16($sp)
    203c:	3c020000 	lui	$v0,0x0
    2040:	244216bc 	addiu	$v0,$v0,5820
    2044:	0040f809 	jalr	$v0
    2048:	00000000 	nop
    204c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2050:	8fbf0010 	lw	$ra,16($sp)
    2054:	3c020000 	lui	$v0,0x0
    2058:	2442105c 	addiu	$v0,$v0,4188
    205c:	0040f809 	jalr	$v0
    2060:	00000000 	nop
    2064:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2068:	8fbf0010 	lw	$ra,16($sp)
    206c:	3c020000 	lui	$v0,0x0
    2070:	244218bc 	addiu	$v0,$v0,6332
    2074:	0040f809 	jalr	$v0
    2078:	00000000 	nop
    207c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2080:	8fbf0010 	lw	$ra,16($sp)
    2084:	3c020000 	lui	$v0,0x0
    2088:	24421afc 	addiu	$v0,$v0,6908
    208c:	0040f809 	jalr	$v0
    2090:	00000000 	nop
    2094:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2098:	8fbf0010 	lw	$ra,16($sp)
    209c:	3c020000 	lui	$v0,0x0
    20a0:	24421bf8 	addiu	$v0,$v0,7160
    20a4:	0040f809 	jalr	$v0
    20a8:	00000000 	nop
    20ac:	08000850 	j	2140 <mxp_syscall+0x2f8>
    20b0:	8fbf0010 	lw	$ra,16($sp)
    20b4:	3c020000 	lui	$v0,0x0
    20b8:	24421d44 	addiu	$v0,$v0,7492
    20bc:	0040f809 	jalr	$v0
    20c0:	00000000 	nop
    20c4:	08000850 	j	2140 <mxp_syscall+0x2f8>
    20c8:	8fbf0010 	lw	$ra,16($sp)
    20cc:	3c020000 	lui	$v0,0x0
    20d0:	24421e34 	addiu	$v0,$v0,7732
    20d4:	0040f809 	jalr	$v0
    20d8:	00000000 	nop
    20dc:	08000850 	j	2140 <mxp_syscall+0x2f8>
    20e0:	8fbf0010 	lw	$ra,16($sp)
    20e4:	3c020000 	lui	$v0,0x0
    20e8:	2442013c 	addiu	$v0,$v0,316
    20ec:	0040f809 	jalr	$v0
    20f0:	00000000 	nop
    20f4:	08000850 	j	2140 <mxp_syscall+0x2f8>
    20f8:	8fbf0010 	lw	$ra,16($sp)
    20fc:	3c020000 	lui	$v0,0x0
    2100:	24420084 	addiu	$v0,$v0,132
    2104:	0040f809 	jalr	$v0
    2108:	00000000 	nop
    210c:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2110:	8fbf0010 	lw	$ra,16($sp)
    2114:	3c020000 	lui	$v0,0x0
    2118:	24420274 	addiu	$v0,$v0,628
    211c:	0040f809 	jalr	$v0
    2120:	00000000 	nop
    2124:	08000850 	j	2140 <mxp_syscall+0x2f8>
    2128:	8fbf0010 	lw	$ra,16($sp)
    212c:	3c020000 	lui	$v0,0x0
    2130:	2442035c 	addiu	$v0,$v0,860
    2134:	0040f809 	jalr	$v0
    2138:	00000000 	nop
    213c:	8fbf0010 	lw	$ra,16($sp)
    2140:	03e00008 	jr	$ra
    2144:	27bd0018 	addiu	$sp,$sp,24
*/

// irqreturn_t mxp_timer_irq_handler(int i, void *data)
/*
0000000000002148 <mxp_timer_irq_handle>:
    2148:	27bdffe8 	addiu	$sp,$sp,-24
    214c:	3c020000 	lui	$v0,0x0
    2150:	8c420048 	lw	$v0,72($v0)
    2154:	afbf0010 	sw	$ra,16($sp)
    2158:	24420001 	addiu	$v0,$v0,1
    215c:	3c010000 	lui	$at,0x0
    2160:	ac220048 	sw	$v0,72($at)
    2164:	40054800 	mfc0	$a1,$9
    2168:	3c020000 	lui	$v0,0x0
    216c:	8c420000 	lw	$v0,0($v0)
    2170:	2406ffff 	li	$a2,-1
    2174:	10460035 	beq	$v0,$a2,224c <mxp_timer_irq_handle+0x104>
    2178:	3c020002 	lui	$v0,0x2
    217c:	3c030000 	lui	$v1,0x0
    2180:	8c630000 	lw	$v1,0($v1)
    2184:	34427ac3 	ori	$v0,$v0,0x7ac3
    2188:	00a31823 	subu	$v1,$a1,$v1
    218c:	0043102b 	sltu	$v0,$v0,$v1
    2190:	14400007 	bnez	$v0,21b0 <mxp_timer_irq_handle+0x68>
    2194:	3c020003 	lui	$v0,0x3
    2198:	3c020000 	lui	$v0,0x0
    219c:	8c420030 	lw	$v0,48($v0)
    21a0:	24420001 	addiu	$v0,$v0,1
    21a4:	3c010000 	lui	$at,0x0
    21a8:	ac220030 	sw	$v0,48($at)
    21ac:	3c020003 	lui	$v0,0x3
    21b0:	34421974 	ori	$v0,$v0,0x1974
    21b4:	0043102b 	sltu	$v0,$v0,$v1
    21b8:	14400006 	bnez	$v0,21d4 <mxp_timer_irq_handle+0x8c>
    21bc:	00000000 	nop
    21c0:	3c020000 	lui	$v0,0x0
    21c4:	8c420034 	lw	$v0,52($v0)
    21c8:	24420001 	addiu	$v0,$v0,1
    21cc:	3c010000 	lui	$at,0x0
    21d0:	ac220034 	sw	$v0,52($at)
    21d4:	3c040000 	lui	$a0,0x0
    21d8:	8c84000c 	lw	$a0,12($a0)
    21dc:	14860008 	bne	$a0,$a2,2200 <mxp_timer_irq_handle+0xb8>
    21e0:	00000000 	nop
    21e4:	3c010000 	lui	$at,0x0
    21e8:	ac230008 	sw	$v1,8($at)
    21ec:	3c010000 	lui	$at,0x0
    21f0:	ac230004 	sw	$v1,4($at)
    21f4:	3c010000 	lui	$at,0x0
    21f8:	08000893 	j	224c <mxp_timer_irq_handle+0x104>
    21fc:	ac23000c 	sw	$v1,12($at)
    2200:	3c020000 	lui	$v0,0x0
    2204:	8c420008 	lw	$v0,8($v0)
    2208:	0043102b 	sltu	$v0,$v0,$v1
    220c:	10400003 	beqz	$v0,221c <mxp_timer_irq_handle+0xd4>
    2210:	00000000 	nop
    2214:	3c010000 	lui	$at,0x0
    2218:	ac230008 	sw	$v1,8($at)
    221c:	3c020000 	lui	$v0,0x0
    2220:	8c420004 	lw	$v0,4($v0)
    2224:	0062102b 	sltu	$v0,$v1,$v0
    2228:	10400003 	beqz	$v0,2238 <mxp_timer_irq_handle+0xf0>
    222c:	000410c0 	sll	$v0,$a0,0x3
    2230:	3c010000 	lui	$at,0x0
    2234:	ac230004 	sw	$v1,4($at)
    2238:	00441023 	subu	$v0,$v0,$a0
    223c:	00431021 	addu	$v0,$v0,$v1
    2240:	000210c2 	srl	$v0,$v0,0x3
    2244:	3c010000 	lui	$at,0x0
    2248:	ac22000c 	sw	$v0,12($at)
    224c:	3c010000 	lui	$at,0x0
    2250:	ac250010 	sw	$a1,16($at)
    2254:	3c020000 	lui	$v0,0x0
    2258:	8c420010 	lw	$v0,16($v0)
    225c:	3c040000 	lui	$a0,0x0
    2260:	24840058 	addiu	$a0,$a0,88
    2264:	3c010000 	lui	$at,0x0
    2268:	ac220000 	sw	$v0,0($at)
    226c:	24020001 	li	$v0,1
    2270:	c0830000 	lwc0	$3,0($a0)
    2274:	00622825 	or	$a1,$v1,$v0
    2278:	e0850000 	swc0	$5,0($a0)
    227c:	10a0fffc 	beqz	$a1,2270 <mxp_timer_irq_handle+0x128>
    2280:	00622824 	and	$a1,$v1,$v0
    2284:	00a01021 	move	$v0,$a1
    2288:	14400006 	bnez	$v0,22a4 <mxp_timer_irq_handle+0x15c>
    228c:	8fbf0010 	lw	$ra,16($sp)
    2290:	3c020000 	lui	$v0,0x0
    2294:	24420000 	addiu	$v0,$v0,0
    2298:	0040f809 	jalr	$v0
    229c:	2484fffc 	addiu	$a0,$a0,-4 // tasklet schedule
    22a0:	8fbf0010 	lw	$ra,16($sp)
    22a4:	03e00008 	jr	$ra
    22a8:	27bd0018 	addiu	$sp,$sp,24
*/
/*
    22ac:	27bdffc0 	addiu	$sp,$sp,-64
    22b0:	afbf003c 	sw	$ra,60($sp)
    22b4:	afb40038 	sw	$s4,56($sp)
    22b8:	afb30034 	sw	$s3,52($sp)
    22bc:	afb20030 	sw	$s2,48($sp)
    22c0:	afb1002c 	sw	$s1,44($sp)
    22c4:	afb00028 	sw	$s0,40($sp)
    22c8:	00809821 	move	$s3,$a0
    22cc:	3c050000 	lui	$a1,0x0
    22d0:	24a5025c 	addiu	$a1,$a1,604
    22d4:	24100001 	li	$s0,1
    22d8:	24120030 	li	$s2,48
    22dc:	3c140000 	lui	$s4,0x0
    22e0:	26940000 	addiu	$s4,$s4,0
    22e4:	3c020000 	lui	$v0,0x0
    22e8:	8c420038 	lw	$v0,56($v0)
    22ec:	3c060000 	lui	$a2,0x0
    22f0:	8cc60048 	lw	$a2,72($a2)
    22f4:	3c070000 	lui	$a3,0x0
    22f8:	8ce7003c 	lw	$a3,60($a3)
    22fc:	0280f809 	jalr	$s4
    2300:	afa20010 	sw	$v0,16($sp)
    2304:	00408821 	move	$s1,$v0
    2308:	3c020000 	lui	$v0,0x0
    230c:	8c4200c8 	lw	$v0,200($v0)
    2310:	02423021 	addu	$a2,$s2,$v0
    2314:	8cc20000 	lw	$v0,0($a2)
    2318:	10400011 	beqz	$v0,2360 <mxp_timer_irq_handle+0x218>
    231c:	24c2001c 	addiu	$v0,$a2,28
    2320:	8cc30008 	lw	$v1,8($a2)
    2324:	afa20014 	sw	$v0,20($sp)
    2328:	afa30010 	sw	$v1,16($sp)
    232c:	8cc20018 	lw	$v0,24($a2)
    2330:	afa20018 	sw	$v0,24($sp)
    2334:	8cc2000c 	lw	$v0,12($a2)
    2338:	02712021 	addu	$a0,$s3,$s1
    233c:	afa2001c 	sw	$v0,28($sp)
    2340:	8cc2002c 	lw	$v0,44($a2)
    2344:	3c050000 	lui	$a1,0x0
    2348:	24a50280 	addiu	$a1,$a1,640
    234c:	afa20020 	sw	$v0,32($sp)
    2350:	8cc70004 	lw	$a3,4($a2)
    2354:	0280f809 	jalr	$s4
    2358:	02003021 	move	$a2,$s0
    235c:	02228821 	addu	$s1,$s1,$v0
    2360:	26100001 	addiu	$s0,$s0,1
    2364:	2a020080 	slti	$v0,$s0,128
    2368:	1440ffe7 	bnez	$v0,2308 <mxp_timer_irq_handle+0x1c0>
    236c:	26520030 	addiu	$s2,$s2,48
    2370:	02712021 	addu	$a0,$s3,$s1
    2374:	3c050000 	lui	$a1,0x0
    2378:	24a502a0 	addiu	$a1,$a1,672
    237c:	3c100000 	lui	$s0,0x0
    2380:	26100000 	addiu	$s0,$s0,0
    2384:	3c020000 	lui	$v0,0x0
    2388:	8c420008 	lw	$v0,8($v0)
    238c:	3c060000 	lui	$a2,0x0
    2390:	8cc60004 	lw	$a2,4($a2)
    2394:	3c070000 	lui	$a3,0x0
    2398:	8ce7000c 	lw	$a3,12($a3)
    239c:	0200f809 	jalr	$s0
    23a0:	afa20010 	sw	$v0,16($sp)
    23a4:	02228821 	addu	$s1,$s1,$v0
    23a8:	02712021 	addu	$a0,$s3,$s1
    23ac:	3c050000 	lui	$a1,0x0
    23b0:	24a502c4 	addiu	$a1,$a1,708
    23b4:	3c020000 	lui	$v0,0x0
    23b8:	8c420018 	lw	$v0,24($v0)
    23bc:	3c060000 	lui	$a2,0x0
    23c0:	8cc60014 	lw	$a2,20($a2)
    23c4:	3c070000 	lui	$a3,0x0
    23c8:	8ce7001c 	lw	$a3,28($a3)
    23cc:	0200f809 	jalr	$s0
    23d0:	afa20010 	sw	$v0,16($sp)
    23d4:	02228821 	addu	$s1,$s1,$v0
    23d8:	02712021 	addu	$a0,$s3,$s1
    23dc:	3c050000 	lui	$a1,0x0
    23e0:	24a502ec 	addiu	$a1,$a1,748
    23e4:	3c020000 	lui	$v0,0x0
    23e8:	8c420028 	lw	$v0,40($v0)
    23ec:	3c060000 	lui	$a2,0x0
    23f0:	8cc60024 	lw	$a2,36($a2)
    23f4:	3c070000 	lui	$a3,0x0
    23f8:	8ce7002c 	lw	$a3,44($a3)
    23fc:	0200f809 	jalr	$s0
    2400:	afa20010 	sw	$v0,16($sp)
    2404:	02228821 	addu	$s1,$s1,$v0
    2408:	02712021 	addu	$a0,$s3,$s1
    240c:	3c060000 	lui	$a2,0x0
    2410:	8cc60030 	lw	$a2,48($a2)
    2414:	3c070000 	lui	$a3,0x0
    2418:	8ce70034 	lw	$a3,52($a3)
    241c:	3c050000 	lui	$a1,0x0
    2420:	0200f809 	jalr	$s0
    2424:	24a50310 	addiu	$a1,$a1,784
    2428:	24040001 	li	$a0,1
    242c:	02221021 	addu	$v0,$s1,$v0
    2430:	8fa30050 	lw	$v1,80($sp)
    2434:	8fbf003c 	lw	$ra,60($sp)
    2438:	8fb40038 	lw	$s4,56($sp)
    243c:	8fb30034 	lw	$s3,52($sp)
    2440:	8fb20030 	lw	$s2,48($sp)
    2444:	8fb1002c 	lw	$s1,44($sp)
    2448:	8fb00028 	lw	$s0,40($sp)
    244c:	ac640000 	sw	$a0,0($v1)
    2450:	03e00008 	jr	$ra
    2454:	27bd0040 	addiu	$sp,$sp,64
    2458:	24020001 	li	$v0,1
    245c:	3c030000 	lui	$v1,0x0
    2460:	24630010 	addiu	$v1,$v1,16
    2464:	c0640000 	lwc0	$4,0($v1)
    2468:	00822021 	addu	$a0,$a0,$v0
    246c:	e0640000 	swc0	$4,0($v1)
    2470:	1080fffc 	beqz	$a0,2464 <mxp_timer_irq_handle+0x31c>
    2474:	00000000 	nop
    2478:	8c620004 	lw	$v0,4($v1)
    247c:	34420018 	ori	$v0,$v0,0x18
    2480:	03e00008 	jr	$ra
    2484:	ac620004 	sw	$v0,4($v1)
    2488:	24020001 	li	$v0,1
    248c:	3c030000 	lui	$v1,0x0
    2490:	24630010 	addiu	$v1,$v1,16
    2494:	c0640000 	lwc0	$4,0($v1)
    2498:	00822023 	subu	$a0,$a0,$v0
    249c:	e0640000 	swc0	$4,0($v1)
    24a0:	1080fffc 	beqz	$a0,2494 <mxp_timer_irq_handle+0x34c>
    24a4:	00000000 	nop
    24a8:	8c620004 	lw	$v0,4($v1)
    24ac:	34420008 	ori	$v0,$v0,0x8
    24b0:	03e00008 	jr	$ra
    24b4:	ac620004 	sw	$v0,4($v1)
    24b8:	3c040000 	lui	$a0,0x0
    24bc:	8c8400c8 	lw	$a0,200($a0)
    24c0:	3c030000 	lui	$v1,0x0
    24c4:	8c63000c 	lw	$v1,12($v1)
    24c8:	00041582 	srl	$v0,$a0,0x16
    24cc:	00021080 	sll	$v0,$v0,0x2
    24d0:	00621821 	addu	$v1,$v1,$v0
    24d4:	00042282 	srl	$a0,$a0,0xa
    24d8:	8c620000 	lw	$v0,0($v1)
    24dc:	30840ffc 	andi	$a0,$a0,0xffc
    24e0:	00441021 	addu	$v0,$v0,$a0
    24e4:	8c430000 	lw	$v1,0($v0)
    24e8:	3c020000 	lui	$v0,0x0
    24ec:	8c420000 	lw	$v0,0($v0)
    24f0:	3c01ec00 	lui	$at,0xec00
    24f4:	00231821 	addu	$v1,$at,$v1
    24f8:	00031b02 	srl	$v1,$v1,0xc
    24fc:	00031980 	sll	$v1,$v1,0x6
    2500:	00431021 	addu	$v0,$v0,$v1
    2504:	24030001 	li	$v1,1
    2508:	c0440014 	lwc0	$4,20($v0)
    250c:	00832021 	addu	$a0,$a0,$v1
    2510:	e0440014 	swc0	$4,20($v0)
    2514:	1080fffc 	beqz	$a0,2508 <mxp_timer_irq_handle+0x3c0>
    2518:	00000000 	nop
    251c:	03e00008 	jr	$ra
    2520:	00000000 	nop
    2524:	27bdffe8 	addiu	$sp,$sp,-24
    2528:	afbf0010 	sw	$ra,16($sp)
    252c:	00803021 	move	$a2,$a0
    2530:	00a02021 	move	$a0,$a1
    2534:	8c830034 	lw	$v1,52($a0)
    2538:	3c020000 	lui	$v0,0x0
    253c:	8c420000 	lw	$v0,0($v0)
    2540:	00031b00 	sll	$v1,$v1,0xc
    2544:	3c018000 	lui	$at,0x8000
    2548:	00221021 	addu	$v0,$at,$v0
    254c:	0062182b 	sltu	$v1,$v1,$v0
    2550:	50600006 	0x50600006
    2554:	8c820014 	lw	$v0,20($a0)
    2558:	8cc20018 	lw	$v0,24($a2)
    255c:	30420010 	andi	$v0,$v0,0x10
    2560:	10400004 	beqz	$v0,2574 <mxp_timer_irq_handle+0x42c>
    2564:	00000000 	nop
    2568:	8c820014 	lw	$v0,20($a0)
    256c:	34424000 	ori	$v0,$v0,0x4000
    2570:	ac820014 	sw	$v0,20($a0)
    2574:	3c020000 	lui	$v0,0x0
    2578:	24420068 	addiu	$v0,$v0,104
    257c:	ac820030 	sw	$v0,48($a0)
    2580:	8c820014 	lw	$v0,20($a0)
    2584:	3c030008 	lui	$v1,0x8
    2588:	00431025 	or	$v0,$v0,$v1
    258c:	ac820014 	sw	$v0,20($a0)
    2590:	3c020000 	lui	$v0,0x0
    2594:	24422458 	addiu	$v0,$v0,9304
    2598:	0040f809 	jalr	$v0
    259c:	00000000 	nop
    25a0:	8fbf0010 	lw	$ra,16($sp)
    25a4:	00001021 	move	$v0,$zero
    25a8:	03e00008 	jr	$ra
    25ac:	27bd0018 	addiu	$sp,$sp,24
    25b0:	24020001 	li	$v0,1
    25b4:	3c030000 	lui	$v1,0x0
    25b8:	24630010 	addiu	$v1,$v1,16
    25bc:	c0640000 	lwc0	$4,0($v1)
    25c0:	00822021 	addu	$a0,$a0,$v0
    25c4:	e0640000 	swc0	$4,0($v1)
    25c8:	1080fffc 	beqz	$a0,25bc <mxp_timer_irq_handle+0x474>
    25cc:	00000000 	nop
    25d0:	8c620004 	lw	$v0,4($v1)
    25d4:	34420018 	ori	$v0,$v0,0x18
    25d8:	ac620004 	sw	$v0,4($v1)
    25dc:	03e00008 	jr	$ra
    25e0:	00001021 	move	$v0,$zero
    25e4:	24020001 	li	$v0,1
    25e8:	3c030000 	lui	$v1,0x0
    25ec:	24630010 	addiu	$v1,$v1,16
    25f0:	c0640000 	lwc0	$4,0($v1)
    25f4:	00822023 	subu	$a0,$a0,$v0
    25f8:	e0640000 	swc0	$4,0($v1)
    25fc:	1080fffc 	beqz	$a0,25f0 <mxp_timer_irq_handle+0x4a8>
    2600:	00000000 	nop
    2604:	8c620004 	lw	$v0,4($v1)
    2608:	34420008 	ori	$v0,$v0,0x8
    260c:	ac620004 	sw	$v0,4($v1)
    2610:	03e00008 	jr	$ra
    2614:	00001021 	move	$v0,$zero
*/

static int __init mxpmod_init_module(void) {
/*
0000000000002618 <init_module>:
    2618:	27bdffc8 	addiu	$sp,$sp,-56
    261c:	afbf0030 	sw	$ra,48($sp)
    2620:	afb3002c 	sw	$s3,44($sp)
    2624:	afb20028 	sw	$s2,40($sp)
    2628:	afb10024 	sw	$s1,36($sp)
*/
/*
    262c:	3c020000 	lui	$v0,0x0
    2630:	8c420050 	lw	$v0,80($v0)
    2634:	afb00020 	sw	$s0,32($sp)
    2638:	8c420000 	lw	$v0,0($v0)
    263c:	30420008 	andi	$v0,$v0,0x8
    2640:	10400008 	beqz	$v0,2664 <init_module+0x4c>
    2644:	00002021 	move	$a0,$zero
    2648:	3c040000 	lui	$a0,0x0
    264c:	24840328 	addiu	$a0,$a0,808
    2650:	3c020000 	lui	$v0,0x0
    2654:	24420000 	addiu	$v0,$v0,0
    2658:	0040f809 	jalr	$v0
    265c:	00000000 	nop
*/
/*
    2660:	00002021 	move	$a0,$zero
    2664:	3c120000 	lui	$s2,0x0
    2668:	26520350 	addiu	$s2,$s2,848
    266c:	02402821 	move	$a1,$s2
    2670:	3c060000 	lui	$a2,0x0
    2674:	8cc60050 	lw	$a2,80($a2)
    2678:	2403fff7 	li	$v1,-9
    267c:	8cc20000 	lw	$v0,0($a2)
    2680:	240700f1 	li	$a3,241
    2684:	00431024 	and	$v0,$v0,$v1
    2688:	acc20000 	sw	$v0,0($a2)
    268c:	3c030000 	lui	$v1,0x0
    2690:	8c630050 	lw	$v1,80($v1)
    2694:	3c130000 	lui	$s3,0x0
    2698:	26730074 	addiu	$s3,$s3,116
    269c:	8c620000 	lw	$v0,0($v1)
    26a0:	00003021 	move	$a2,$zero
    26a4:	34420008 	ori	$v0,$v0,0x8
    26a8:	ac620000 	sw	$v0,0($v1)
    26ac:	3c030000 	lui	$v1,0x0
    26b0:	8c63004c 	lw	$v1,76($v1)
    26b4:	24021e84 	li	$v0,7812
    26b8:	a4620004 	sh	$v0,4($v1)
    26bc:	3c030000 	lui	$v1,0x0
    26c0:	8c63004c 	lw	$v1,76($v1)
    26c4:	3402800f 	li	$v0,0x800f
    26c8:	a4620000 	sh	$v0,0($v1)
    26cc:	afa00010 	sw	$zero,16($sp)
    26d0:	240221b6 	li	$v0,8630
    26d4:	afa20014 	sw	$v0,20($sp)
    26d8:	afb30018 	sw	$s3,24($sp)
    26dc:	3c020000 	lui	$v0,0x0
    26e0:	24420000 	addiu	$v0,$v0,0
    26e4:	0040f809 	jalr	$v0
    26e8:	afa0001c 	sw	$zero,28($sp)
*/
/*
    26ec:	3c010000 	lui	$at,0x0
    26f0:	ac2200bc 	sw	$v0,188($at)
    26f4:	3c020000 	lui	$v0,0x0
    26f8:	24422ff8 	addiu	$v0,$v0,12280
    26fc:	0040f809 	jalr	$v0
    2700:	00000000 	nop
*/
/*
    2704:	3c020000 	lui	$v0,0x0
    2708:	24421850 	addiu	$v0,$v0,6224
    270c:	0040f809 	jalr	$v0
    2710:	00000000 	nop
*/
/*
    2714:	3c020000 	lui	$v0,0x0
    2718:	24420be0 	addiu	$v0,$v0,3040
    271c:	0040f809 	jalr	$v0
    2720:	00000000 	nop
*/
/*
    2724:	afa00010 	sw	$zero,16($sp)
    2728:	2404000d 	li	$a0,13
    272c:	3c050000 	lui	$a1,0x0
    2730:	24a52148 	addiu	$a1,$a1,8520 
    2734:	3c070000 	lui	$a3,0x0
    2738:	24e70354 	addiu	$a3,$a3,852
    273c:	3c020000 	lui	$v0,0x0
    2740:	24420000 	addiu	$v0,$v0,0
    2744:	0040f809 	jalr	$v0          // requst_irq
    2748:	00003021 	move	$a2,$zero
*/
/*
    274c:	3c040000 	lui	$a0,0x0
    2750:	24840360 	addiu	$a0,$a0,864
    2754:	14400032 	bnez	$v0,2820 <init_module+0x208>
    2758:	240501f2 	li	$a1,498
*/
/*
    275c:	24041800 	li	$a0,6144
    2760:	3c020000 	lui	$v0,0x0
    2764:	24420000 	addiu	$v0,$v0,0
    2768:	0040f809 	jalr	$v0
    276c:	2406079f 	li	$a2,1951
*/
/*
    2770:	3c010000 	lui	$at,0x0
    2774:	ac2200c8 	sw	$v0,200($at)
    2778:	3c040000 	lui	$a0,0x0
    277c:	24840388 	addiu	$a0,$a0,904
    2780:	10400027 	beqz	$v0,2820 <init_module+0x208>
    2784:	00002821 	move	$a1,$zero
    2788:	00402021 	move	$a0,$v0
    278c:	3c100000 	lui	$s0,0x0
    2790:	26100000 	addiu	$s0,$s0,0
    2794:	0200f809 	jalr	$s0
    2798:	24061800 	li	$a2,6144
*/
/*
    279c:	3c110000 	lui	$s1,0x0
    27a0:	263100cc 	addiu	$s1,$s1,204
    27a4:	02202021 	move	$a0,$s1
    27a8:	00002821 	move	$a1,$zero
    27ac:	0200f809 	jalr	$s0
    27b0:	24061000 	li	$a2,4096
*/
/*
    27b4:	24040001 	li	$a0,1
    27b8:	00041140 	sll	$v0,$a0,0x5
    27bc:	00511021 	addu	$v0,$v0,$s1
    27c0:	ac400000 	sw	$zero,0($v0)
    27c4:	24430004 	addiu	$v1,$v0,4
    27c8:	ac430004 	sw	$v1,4($v0)
    27cc:	ac430008 	sw	$v1,8($v0)
    27d0:	24840001 	addiu	$a0,$a0,1
    27d4:	28820080 	slti	$v0,$a0,128
    27d8:	1440fff8 	bnez	$v0,27bc <init_module+0x1a4>
    27dc:	00041140 	sll	$v0,$a0,0x5
    27e0:	240400f1 	li	$a0,241
    27e4:	02402821 	move	$a1,$s2
    27e8:	3c020000 	lui	$v0,0x0
    27ec:	24420000 	addiu	$v0,$v0,0
    27f0:	0040f809 	jalr	$v0
    27f4:	02603021 	move	$a2,$s3
*/
/*
    27f8:	0441000f 	bgez	$v0,2838 <init_module+0x220>
    27fc:	02402021 	move	$a0,$s2
    2800:	3c040000 	lui	$a0,0x0
    2804:	8c8400c8 	lw	$a0,200($a0)
    2808:	3c020000 	lui	$v0,0x0
    280c:	24420000 	addiu	$v0,$v0,0
    2810:	0040f809 	jalr	$v0
    2814:	00000000 	nop
*/
/*
    2818:	3c040000 	lui	$a0,0x0
    281c:	248403b4 	addiu	$a0,$a0,948
    2820:	3c020000 	lui	$v0,0x0
    2824:	24420000 	addiu	$v0,$v0,0
    2828:	0040f809 	jalr	$v0
    282c:	00000000 	nop
*/
/*
    2830:	08000a3d 	j	28f4 <init_module+0x2dc>
    2834:	24020001 	li	$v0,1
    2838:	3c020000 	lui	$v0,0x0
    283c:	24420000 	addiu	$v0,$v0,0
    2840:	0040f809 	jalr	$v0
    2844:	00002821 	move	$a1,$zero
*/
/*
    2848:	3c010000 	lui	$at,0x0
    284c:	ac2200c0 	sw	$v0,192($at)
    2850:	3c030000 	lui	$v1,0x0
    2854:	24630000 	addiu	$v1,$v1,0
    2858:	ac430028 	sw	$v1,40($v0)
    285c:	3c040000 	lui	$a0,0x0
    2860:	248403d0 	addiu	$a0,$a0,976
    2864:	00002821 	move	$a1,$zero
    2868:	3c100000 	lui	$s0,0x0
    286c:	26100000 	addiu	$s0,$s0,0
    2870:	0200f809 	jalr	$s0
    2874:	00403021 	move	$a2,$v0
    2878:	00401821 	move	$v1,$v0
    287c:	10600005 	beqz	$v1,2894 <init_module+0x27c>
    2880:	00000000 	nop
    2884:	3c020000 	lui	$v0,0x0
    2888:	244222ac 	addiu	$v0,$v0,8876
    288c:	ac62003c 	sw	$v0,60($v1)
    2890:	ac600038 	sw	$zero,56($v1)
    2894:	3c040000 	lui	$a0,0x0
    2898:	248403d8 	addiu	$a0,$a0,984
    289c:	3c060000 	lui	$a2,0x0
    28a0:	8cc600c0 	lw	$a2,192($a2)
    28a4:	0200f809 	jalr	$s0
    28a8:	00002821 	move	$a1,$zero
*/
/*
    28ac:	00401821 	move	$v1,$v0
    28b0:	10600005 	beqz	$v1,28c8 <init_module+0x2b0>
    28b4:	00000000 	nop
    28b8:	3c020000 	lui	$v0,0x0
    28bc:	24421774 	addiu	$v0,$v0,6004
    28c0:	ac62003c 	sw	$v0,60($v1)
    28c4:	ac600038 	sw	$zero,56($v1)
    28c8:	3c020000 	lui	$v0,0x0
    28cc:	24421e48 	addiu	$v0,$v0,7752
    28d0:	3c010000 	lui	$at,0x0
    28d4:	ac22000c 	sw	$v0,12($at)
    28d8:	3c040000 	lui	$a0,0x0
    28dc:	248403e0 	addiu	$a0,$a0,992
    28e0:	3c020000 	lui	$v0,0x0
    28e4:	24420000 	addiu	$v0,$v0,0
    28e8:	0040f809 	jalr	$v0
    28ec:	00000000 	nop
*/
/*
    28f0:	00001021 	move	$v0,$zero
    28f4:	8fbf0030 	lw	$ra,48($sp)
    28f8:	8fb3002c 	lw	$s3,44($sp)
    28fc:	8fb20028 	lw	$s2,40($sp)
    2900:	8fb10024 	lw	$s1,36($sp)
    2904:	8fb00020 	lw	$s0,32($sp)
    2908:	03e00008 	jr	$ra
    290c:	27bd0038 	addiu	$sp,$sp,56
*/
	return 0;
}

static void __exit mxpmod_cleanup_module(void)
{

/*
0000000000002910 <cleanup_module>:
    2910:	27bdffe0 	addiu	$sp,$sp,-32
    2914:	afbf0018 	sw	$ra,24($sp)
    2918:	afb10014 	sw	$s1,20($sp)
    291c:	afb00010 	sw	$s0,16($sp)
    2920:	2404000d 	li	$a0,13
    2924:	3c020000 	lui	$v0,0x0
    2928:	24420000 	addiu	$v0,$v0,0
    292c:	0040f809 	jalr	$v0
    2930:	00002821 	move	$a1,$zero
    2934:	3c050000 	lui	$a1,0x0
    2938:	8ca50050 	lw	$a1,80($a1)
    293c:	3c040000 	lui	$a0,0x0
    2940:	24840054 	addiu	$a0,$a0,84
    2944:	8ca20000 	lw	$v0,0($a1)
    2948:	2403fff7 	li	$v1,-9
    294c:	00431024 	and	$v0,$v0,$v1
    2950:	aca20000 	sw	$v0,0($a1)
    2954:	3c020000 	lui	$v0,0x0
    2958:	24420000 	addiu	$v0,$v0,0
    295c:	0040f809 	jalr	$v0
    2960:	00000000 	nop
    2964:	3c040000 	lui	$a0,0x0
    2968:	8c8400bc 	lw	$a0,188($a0)
    296c:	3c020000 	lui	$v0,0x0
    2970:	24420000 	addiu	$v0,$v0,0
    2974:	0040f809 	jalr	$v0
    2978:	00000000 	nop
    297c:	3c040000 	lui	$a0,0x0
    2980:	248403d0 	addiu	$a0,$a0,976
    2984:	3c050000 	lui	$a1,0x0
    2988:	8ca500c0 	lw	$a1,192($a1)
    298c:	3c100000 	lui	$s0,0x0
    2990:	26100000 	addiu	$s0,$s0,0
    2994:	0200f809 	jalr	$s0
    2998:	00000000 	nop
    299c:	3c050000 	lui	$a1,0x0
    29a0:	8ca500c0 	lw	$a1,192($a1)
    29a4:	3c040000 	lui	$a0,0x0
    29a8:	0200f809 	jalr	$s0
    29ac:	248403d8 	addiu	$a0,$a0,984
    29b0:	3c110000 	lui	$s1,0x0
    29b4:	26310350 	addiu	$s1,$s1,848
    29b8:	02202021 	move	$a0,$s1
    29bc:	0200f809 	jalr	$s0
    29c0:	00002821 	move	$a1,$zero
    29c4:	3c010000 	lui	$at,0x0
    29c8:	ac20000c 	sw	$zero,12($at)
    29cc:	240400f1 	li	$a0,241
    29d0:	3c020000 	lui	$v0,0x0
    29d4:	24420000 	addiu	$v0,$v0,0
    29d8:	0040f809 	jalr	$v0
    29dc:	02202821 	move	$a1,$s1
    29e0:	3c040000 	lui	$a0,0x0
    29e4:	8c8400c8 	lw	$a0,200($a0)
    29e8:	3c020000 	lui	$v0,0x0
    29ec:	24420000 	addiu	$v0,$v0,0
    29f0:	0040f809 	jalr	$v0
    29f4:	00000000 	nop
    29f8:	3c040000 	lui	$a0,0x0
    29fc:	248403f4 	addiu	$a0,$a0,1012
    2a00:	3c020000 	lui	$v0,0x0
    2a04:	24420000 	addiu	$v0,$v0,0
    2a08:	0040f809 	jalr	$v0
    2a0c:	00000000 	nop
    2a10:	8fbf0018 	lw	$ra,24($sp)
    2a14:	8fb10014 	lw	$s1,20($sp)
    2a18:	8fb00010 	lw	$s0,16($sp)
    2a1c:	03e00008 	jr	$ra
    2a20:	27bd0020 	addiu	$sp,$sp,32
    2a24:	08000a8d 	j	2a34 <cleanup_module+0x124>
    2a28:	80830000 	lb	$v1,0($a0)
    2a2c:	24a50001 	addiu	$a1,$a1,1
    2a30:	80830000 	lb	$v1,0($a0)
    2a34:	80a20000 	lb	$v0,0($a1)
    2a38:	54620004 	0x54620004
    2a3c:	80820000 	lb	$v0,0($a0)
    2a40:	5460fffa 	0x5460fffa
    2a44:	24840001 	addiu	$a0,$a0,1
    2a48:	80820000 	lb	$v0,0($a0)
    2a4c:	80a30000 	lb	$v1,0($a1)
    2a50:	00431026 	xor	$v0,$v0,$v1
    2a54:	03e00008 	jr	$ra
    2a58:	0002102b 	sltu	$v0,$zero,$v0
    2a5c:	08000a9d 	j	2a74 <cleanup_module+0x164>
    2a60:	80a20000 	lb	$v0,0($a1)
    2a64:	a0830000 	sb	$v1,0($a0)
    2a68:	24a50001 	addiu	$a1,$a1,1
    2a6c:	24840001 	addiu	$a0,$a0,1
    2a70:	80a20000 	lb	$v0,0($a1)
    2a74:	1440fffb 	bnez	$v0,2a64 <cleanup_module+0x154>
    2a78:	90a30000 	lbu	$v1,0($a1)
    2a7c:	03e00008 	jr	$ra
    2a80:	a0800000 	sb	$zero,0($a0)
*/
}

/*
0000000000002a84 <tmrobj_clock>:
    2a84:	27bdffe8 	addiu	$sp,$sp,-24
    2a88:	afbf0014 	sw	$ra,20($sp)
    2a8c:	afb00010 	sw	$s0,16($sp)
    2a90:	40024800 	mfc0	$v0,$9
    2a94:	3c010000 	lui	$at,0x0
    2a98:	ac220020 	sw	$v0,32($at)
    2a9c:	3c030000 	lui	$v1,0x0
    2aa0:	8c630020 	lw	$v1,32($v1)
    2aa4:	3c020000 	lui	$v0,0x0
    2aa8:	8c420010 	lw	$v0,16($v0)
    2aac:	2405ffff 	li	$a1,-1
    2ab0:	10450021 	beq	$v0,$a1,2b38 <tmrobj_clock+0xb4>
    2ab4:	00000000 	nop
    2ab8:	3c020000 	lui	$v0,0x0
    2abc:	8c420010 	lw	$v0,16($v0)
    2ac0:	3c040000 	lui	$a0,0x0
    2ac4:	8c84001c 	lw	$a0,28($a0)
    2ac8:	14850008 	bne	$a0,$a1,2aec <tmrobj_clock+0x68>
    2acc:	00621823 	subu	$v1,$v1,$v0
    2ad0:	3c010000 	lui	$at,0x0
    2ad4:	ac230018 	sw	$v1,24($at)
    2ad8:	3c010000 	lui	$at,0x0
    2adc:	ac230014 	sw	$v1,20($at)
    2ae0:	3c010000 	lui	$at,0x0
    2ae4:	08000ace 	j	2b38 <tmrobj_clock+0xb4>
    2ae8:	ac23001c 	sw	$v1,28($at)
    2aec:	3c020000 	lui	$v0,0x0
    2af0:	8c420018 	lw	$v0,24($v0)
    2af4:	0043102b 	sltu	$v0,$v0,$v1
    2af8:	10400003 	beqz	$v0,2b08 <tmrobj_clock+0x84>
    2afc:	00000000 	nop
    2b00:	3c010000 	lui	$at,0x0
    2b04:	ac230018 	sw	$v1,24($at)
    2b08:	3c020000 	lui	$v0,0x0
    2b0c:	8c420014 	lw	$v0,20($v0)
    2b10:	0062102b 	sltu	$v0,$v1,$v0
    2b14:	10400003 	beqz	$v0,2b24 <tmrobj_clock+0xa0>
    2b18:	000410c0 	sll	$v0,$a0,0x3
    2b1c:	3c010000 	lui	$at,0x0
    2b20:	ac230014 	sw	$v1,20($at)
    2b24:	00441023 	subu	$v0,$v0,$a0
    2b28:	00431021 	addu	$v0,$v0,$v1
    2b2c:	000210c2 	srl	$v0,$v0,0x3
    2b30:	3c010000 	lui	$at,0x0
    2b34:	ac22001c 	sw	$v0,28($at)
    2b38:	40106000 	mfc0	$s0,$12
    2b3c:	00000000 	nop
    2b40:	36010001 	ori	$at,$s0,0x1
    2b44:	38210001 	xori	$at,$at,0x1
    2b48:	40816000 	mtc0	$at,$12
    2b4c:	00000040 	sll	$zero,$zero,0x1
    2b50:	00000040 	sll	$zero,$zero,0x1
    2b54:	00000040 	sll	$zero,$zero,0x1
    2b58:	3c030000 	lui	$v1,0x0
    2b5c:	8c630048 	lw	$v1,72($v1)
    2b60:	3c020000 	lui	$v0,0x0
    2b64:	8c42003c 	lw	$v0,60($v0)
    2b68:	00622823 	subu	$a1,$v1,$v0
    2b6c:	14a0000c 	bnez	$a1,2ba0 <tmrobj_clock+0x11c>
    2b70:	00000000 	nop
    2b74:	40016000 	mfc0	$at,$12
    2b78:	32100001 	andi	$s0,$s0,0x1
    2b7c:	34210001 	ori	$at,$at,0x1
    2b80:	38210001 	xori	$at,$at,0x1
    2b84:	02018025 	or	$s0,$s0,$at
    2b88:	40906000 	mtc0	$s0,$12
	...
    2b98:	08000b51 	j	2d44 <tmrobj_clock+0x2c0>
    2b9c:	8fbf0014 	lw	$ra,20($sp)
    2ba0:	3c020000 	lui	$v0,0x0
    2ba4:	8c42003c 	lw	$v0,60($v0)
    2ba8:	3c040000 	lui	$a0,0x0
    2bac:	8c840040 	lw	$a0,64($a0)
    2bb0:	3c030000 	lui	$v1,0x0
    2bb4:	8c630038 	lw	$v1,56($v1)
    2bb8:	00451021 	addu	$v0,$v0,$a1
    2bbc:	3c010000 	lui	$at,0x0
    2bc0:	ac22003c 	sw	$v0,60($at)
    2bc4:	00651821 	addu	$v1,$v1,$a1
    2bc8:	3c010000 	lui	$at,0x0
    2bcc:	ac230038 	sw	$v1,56($at)
    2bd0:	1080001c 	beqz	$a0,2c44 <tmrobj_clock+0x1c0>
    2bd4:	3c027fff 	lui	$v0,0x7fff
    2bd8:	8c820008 	lw	$v0,8($a0)
    2bdc:	0062102b 	sltu	$v0,$v1,$v0
    2be0:	14400018 	bnez	$v0,2c44 <tmrobj_clock+0x1c0>
    2be4:	3c027fff 	lui	$v0,0x7fff
    2be8:	8c820000 	lw	$v0,0($a0)
    2bec:	3c010000 	lui	$at,0x0
    2bf0:	ac220040 	sw	$v0,64($at)
    2bf4:	ac800004 	sw	$zero,4($a0)
    2bf8:	10400002 	beqz	$v0,2c04 <tmrobj_clock+0x180>
    2bfc:	ac800000 	sw	$zero,0($a0)
    2c00:	ac400004 	sw	$zero,4($v0)
    2c04:	8c82000c 	lw	$v0,12($a0)
    2c08:	10400003 	beqz	$v0,2c18 <tmrobj_clock+0x194>
    2c0c:	00000000 	nop
    2c10:	0040f809 	jalr	$v0
    2c14:	00000000 	nop
    2c18:	3c040000 	lui	$a0,0x0
    2c1c:	8c840040 	lw	$a0,64($a0)
    2c20:	10800008 	beqz	$a0,2c44 <tmrobj_clock+0x1c0>
    2c24:	3c027fff 	lui	$v0,0x7fff
    2c28:	8c830008 	lw	$v1,8($a0)
    2c2c:	3c020000 	lui	$v0,0x0
    2c30:	8c420038 	lw	$v0,56($v0)
    2c34:	0043102b 	sltu	$v0,$v0,$v1
    2c38:	5040ffec 	0x5040ffec
    2c3c:	8c820000 	lw	$v0,0($a0)
    2c40:	3c027fff 	lui	$v0,0x7fff
    2c44:	3c040000 	lui	$a0,0x0
    2c48:	8c840038 	lw	$a0,56($a0)
    2c4c:	3442fffe 	ori	$v0,$v0,0xfffe
    2c50:	0044102b 	sltu	$v0,$v0,$a0
    2c54:	1040000d 	beqz	$v0,2c8c <tmrobj_clock+0x208>
    2c58:	00000000 	nop
    2c5c:	3c030000 	lui	$v1,0x0
    2c60:	8c630040 	lw	$v1,64($v1)
    2c64:	10600007 	beqz	$v1,2c84 <tmrobj_clock+0x200>
    2c68:	00000000 	nop
    2c6c:	8c620008 	lw	$v0,8($v1)
    2c70:	00441023 	subu	$v0,$v0,$a0
    2c74:	ac620008 	sw	$v0,8($v1)
    2c78:	8c630000 	lw	$v1,0($v1)
    2c7c:	5460fffc 	0x5460fffc
    2c80:	8c620008 	lw	$v0,8($v1)
    2c84:	3c010000 	lui	$at,0x0
    2c88:	ac200038 	sw	$zero,56($at)
    2c8c:	40016000 	mfc0	$at,$12
    2c90:	32100001 	andi	$s0,$s0,0x1
    2c94:	34210001 	ori	$at,$at,0x1
    2c98:	38210001 	xori	$at,$at,0x1
    2c9c:	02018025 	or	$s0,$s0,$at
    2ca0:	40906000 	mtc0	$s0,$12
	...
    2cb0:	40034800 	mfc0	$v1,$9
    2cb4:	3c020000 	lui	$v0,0x0
    2cb8:	8c420020 	lw	$v0,32($v0)
    2cbc:	3c040000 	lui	$a0,0x0
    2cc0:	8c84002c 	lw	$a0,44($a0)
    2cc4:	00621823 	subu	$v1,$v1,$v0
    2cc8:	2402ffff 	li	$v0,-1
    2ccc:	14820009 	bne	$a0,$v0,2cf4 <tmrobj_clock+0x270>
    2cd0:	8fbf0014 	lw	$ra,20($sp)
    2cd4:	3c010000 	lui	$at,0x0
    2cd8:	ac230028 	sw	$v1,40($at)
    2cdc:	3c010000 	lui	$at,0x0
    2ce0:	ac230024 	sw	$v1,36($at)
    2ce4:	3c010000 	lui	$at,0x0
    2ce8:	ac23002c 	sw	$v1,44($at)
    2cec:	08000b52 	j	2d48 <tmrobj_clock+0x2c4>
    2cf0:	8fb00010 	lw	$s0,16($sp)
    2cf4:	3c020000 	lui	$v0,0x0
    2cf8:	8c420028 	lw	$v0,40($v0)
    2cfc:	0043102b 	sltu	$v0,$v0,$v1
    2d00:	10400003 	beqz	$v0,2d10 <tmrobj_clock+0x28c>
    2d04:	00000000 	nop
    2d08:	3c010000 	lui	$at,0x0
    2d0c:	ac230028 	sw	$v1,40($at)
    2d10:	3c020000 	lui	$v0,0x0
    2d14:	8c420024 	lw	$v0,36($v0)
    2d18:	0062102b 	sltu	$v0,$v1,$v0
    2d1c:	10400003 	beqz	$v0,2d2c <tmrobj_clock+0x2a8>
    2d20:	000410c0 	sll	$v0,$a0,0x3
    2d24:	3c010000 	lui	$at,0x0
    2d28:	ac230024 	sw	$v1,36($at)
    2d2c:	00441023 	subu	$v0,$v0,$a0
    2d30:	00431021 	addu	$v0,$v0,$v1
    2d34:	000210c2 	srl	$v0,$v0,0x3
    2d38:	3c010000 	lui	$at,0x0
    2d3c:	ac22002c 	sw	$v0,44($at)
    2d40:	8fbf0014 	lw	$ra,20($sp)
    2d44:	8fb00010 	lw	$s0,16($sp)
    2d48:	03e00008 	jr	$ra
    2d4c:	27bd0018 	addiu	$sp,$sp,24

0000000000002d50 <tmrobj_Check>:
    2d50:	3c020000 	lui	$v0,0x0
    2d54:	8c420040 	lw	$v0,64($v0)
    2d58:	10400007 	beqz	$v0,2d78 <tmrobj_Check+0x28>
    2d5c:	00000000 	nop
    2d60:	54820003 	0x54820003
    2d64:	8c420000 	lw	$v0,0($v0)
    2d68:	03e00008 	jr	$ra
    2d6c:	24020001 	li	$v0,1
    2d70:	1440fffb 	bnez	$v0,2d60 <tmrobj_Check+0x10>
    2d74:	00000000 	nop
    2d78:	03e00008 	jr	$ra
    2d7c:	00001021 	move	$v0,$zero

0000000000002d80 <tmrobj_Delete>:
    2d80:	27bdffe0 	addiu	$sp,$sp,-32
    2d84:	afbf0018 	sw	$ra,24($sp)
    2d88:	afb10014 	sw	$s1,20($sp)
    2d8c:	afb00010 	sw	$s0,16($sp)
    2d90:	00808021 	move	$s0,$a0
    2d94:	40116000 	mfc0	$s1,$12
    2d98:	00000000 	nop
    2d9c:	36210001 	ori	$at,$s1,0x1
    2da0:	38210001 	xori	$at,$at,0x1
    2da4:	40816000 	mtc0	$at,$12
    2da8:	00000040 	sll	$zero,$zero,0x1
    2dac:	00000040 	sll	$zero,$zero,0x1
    2db0:	00000040 	sll	$zero,$zero,0x1
    2db4:	3c020000 	lui	$v0,0x0
    2db8:	24422d50 	addiu	$v0,$v0,11600
    2dbc:	0040f809 	jalr	$v0
    2dc0:	02002021 	move	$a0,$s0
    2dc4:	5440000c 	0x5440000c
    2dc8:	8e030000 	lw	$v1,0($s0)
    2dcc:	40016000 	mfc0	$at,$12
    2dd0:	32310001 	andi	$s1,$s1,0x1
    2dd4:	34210001 	ori	$at,$at,0x1
    2dd8:	38210001 	xori	$at,$at,0x1
    2ddc:	02218825 	or	$s1,$s1,$at
    2de0:	40916000 	mtc0	$s1,$12
	...
    2df0:	08000b98 	j	2e60 <tmrobj_Delete+0xe0>
    2df4:	8fbf0018 	lw	$ra,24($sp)
    2df8:	50600004 	0x50600004
    2dfc:	8e030004 	lw	$v1,4($s0)
    2e00:	8e020004 	lw	$v0,4($s0)
    2e04:	ac620004 	sw	$v0,4($v1)
    2e08:	8e030004 	lw	$v1,4($s0)
    2e0c:	10600003 	beqz	$v1,2e1c <tmrobj_Delete+0x9c>
    2e10:	00000000 	nop
    2e14:	8e020000 	lw	$v0,0($s0)
    2e18:	ac620000 	sw	$v0,0($v1)
    2e1c:	3c020000 	lui	$v0,0x0
    2e20:	8c420040 	lw	$v0,64($v0)
    2e24:	14500004 	bne	$v0,$s0,2e38 <tmrobj_Delete+0xb8>
    2e28:	00000000 	nop
    2e2c:	8e020000 	lw	$v0,0($s0)
    2e30:	3c010000 	lui	$at,0x0
    2e34:	ac220040 	sw	$v0,64($at)
    2e38:	40016000 	mfc0	$at,$12
    2e3c:	32310001 	andi	$s1,$s1,0x1
    2e40:	34210001 	ori	$at,$at,0x1
    2e44:	38210001 	xori	$at,$at,0x1
    2e48:	02218825 	or	$s1,$s1,$at
    2e4c:	40916000 	mtc0	$s1,$12
	...
    2e5c:	8fbf0018 	lw	$ra,24($sp)
    2e60:	8fb10014 	lw	$s1,20($sp)
    2e64:	8fb00010 	lw	$s0,16($sp)
    2e68:	03e00008 	jr	$ra
    2e6c:	27bd0020 	addiu	$sp,$sp,32

0000000000002e70 <tmrobj_Start>:
    2e70:	27bdffd0 	addiu	$sp,$sp,-48
    2e74:	afbf0028 	sw	$ra,40($sp)
    2e78:	afb50024 	sw	$s5,36($sp)
    2e7c:	afb40020 	sw	$s4,32($sp)
    2e80:	afb3001c 	sw	$s3,28($sp)
    2e84:	afb20018 	sw	$s2,24($sp)
    2e88:	afb10014 	sw	$s1,20($sp)
    2e8c:	afb00010 	sw	$s0,16($sp)
    2e90:	00a08021 	move	$s0,$a1
    2e94:	00009021 	move	$s2,$zero
    2e98:	00808821 	move	$s1,$a0
    2e9c:	00c09821 	move	$s3,$a2
    2ea0:	00e0a021 	move	$s4,$a3
    2ea4:	40156000 	mfc0	$s5,$12
    2ea8:	00000000 	nop
    2eac:	36a10001 	ori	$at,$s5,0x1
    2eb0:	38210001 	xori	$at,$at,0x1
    2eb4:	40816000 	mtc0	$at,$12
    2eb8:	00000040 	sll	$zero,$zero,0x1
    2ebc:	00000040 	sll	$zero,$zero,0x1
    2ec0:	00000040 	sll	$zero,$zero,0x1
    2ec4:	3c020000 	lui	$v0,0x0
    2ec8:	24422d50 	addiu	$v0,$v0,11600
    2ecc:	0040f809 	jalr	$v0
    2ed0:	02202021 	move	$a0,$s1
    2ed4:	10400006 	beqz	$v0,2ef0 <tmrobj_Start+0x80>
    2ed8:	3c027fff 	lui	$v0,0x7fff
    2edc:	3c020000 	lui	$v0,0x0
    2ee0:	24422d80 	addiu	$v0,$v0,11648
    2ee4:	0040f809 	jalr	$v0
    2ee8:	02202021 	move	$a0,$s1
    2eec:	3c027fff 	lui	$v0,0x7fff
    2ef0:	3442fffe 	ori	$v0,$v0,0xfffe
    2ef4:	0050102b 	sltu	$v0,$v0,$s0
    2ef8:	10400004 	beqz	$v0,2f0c <tmrobj_Start+0x9c>
    2efc:	00000000 	nop
    2f00:	3c107fff 	lui	$s0,0x7fff
    2f04:	08000bc5 	j	2f14 <tmrobj_Start+0xa4>
    2f08:	3610fffe 	ori	$s0,$s0,0xfffe
    2f0c:	52000001 	0x52000001
    2f10:	24100001 	li	$s0,1
    2f14:	ae33000c 	sw	$s3,12($s1)
    2f18:	ae340010 	sw	$s4,16($s1)
    2f1c:	3c020000 	lui	$v0,0x0
    2f20:	8c420038 	lw	$v0,56($v0)
    2f24:	3c040000 	lui	$a0,0x0
    2f28:	8c840040 	lw	$a0,64($a0)
    2f2c:	00501021 	addu	$v0,$v0,$s0
    2f30:	08000bcf 	j	2f3c <tmrobj_Start+0xcc>
    2f34:	ae220008 	sw	$v0,8($s1)
    2f38:	8e440000 	lw	$a0,0($s2)
    2f3c:	10800013 	beqz	$a0,2f8c <tmrobj_Start+0x11c>
    2f40:	00000000 	nop
    2f44:	8c830008 	lw	$v1,8($a0)
    2f48:	8e220008 	lw	$v0,8($s1)
    2f4c:	0043102b 	sltu	$v0,$v0,$v1
    2f50:	5040fff9 	0x5040fff9
    2f54:	00809021 	move	$s2,$a0
    2f58:	ae240000 	sw	$a0,0($s1)
    2f5c:	8c820004 	lw	$v0,4($a0)
    2f60:	ae220004 	sw	$v0,4($s1)
    2f64:	ac910004 	sw	$s1,4($a0)
    2f68:	8e220004 	lw	$v0,4($s1)
    2f6c:	54400001 	0x54400001
    2f70:	ac510000 	sw	$s1,0($v0)
    2f74:	3c020000 	lui	$v0,0x0
    2f78:	8c420040 	lw	$v0,64($v0)
    2f7c:	1482000c 	bne	$a0,$v0,2fb0 <tmrobj_Start+0x140>
    2f80:	00000000 	nop
    2f84:	08000bea 	j	2fa8 <tmrobj_Start+0x138>
    2f88:	00000000 	nop
    2f8c:	52400005 	0x52400005
    2f90:	ae200004 	sw	$zero,4($s1)
    2f94:	ae320004 	sw	$s2,4($s1)
    2f98:	ae510000 	sw	$s1,0($s2)
    2f9c:	08000bec 	j	2fb0 <tmrobj_Start+0x140>
    2fa0:	ae200000 	sw	$zero,0($s1)
    2fa4:	ae200000 	sw	$zero,0($s1)
    2fa8:	3c010000 	lui	$at,0x0
    2fac:	ac310040 	sw	$s1,64($at)
    2fb0:	40016000 	mfc0	$at,$12
    2fb4:	32b50001 	andi	$s5,$s5,0x1
    2fb8:	34210001 	ori	$at,$at,0x1
    2fbc:	38210001 	xori	$at,$at,0x1
    2fc0:	02a1a825 	or	$s5,$s5,$at
    2fc4:	40956000 	mtc0	$s5,$12
	...
    2fd4:	8fbf0028 	lw	$ra,40($sp)
    2fd8:	8fb50024 	lw	$s5,36($sp)
    2fdc:	8fb40020 	lw	$s4,32($sp)
    2fe0:	8fb3001c 	lw	$s3,28($sp)
    2fe4:	8fb20018 	lw	$s2,24($sp)
    2fe8:	8fb10014 	lw	$s1,20($sp)
    2fec:	8fb00010 	lw	$s0,16($sp)
    2ff0:	03e00008 	jr	$ra
    2ff4:	27bd0030 	addiu	$sp,$sp,48

0000000000002ff8 <tmrobj_init>:
    2ff8:	40026000 	mfc0	$v0,$12
    2ffc:	00000000 	nop
    3000:	34410001 	ori	$at,$v0,0x1
    3004:	38210001 	xori	$at,$at,0x1
    3008:	40816000 	mtc0	$at,$12
    300c:	00000040 	sll	$zero,$zero,0x1
    3010:	00000040 	sll	$zero,$zero,0x1
    3014:	00000040 	sll	$zero,$zero,0x1
    3018:	40016000 	mfc0	$at,$12
    301c:	30420001 	andi	$v0,$v0,0x1
    3020:	34210001 	ori	$at,$at,0x1
    3024:	38210001 	xori	$at,$at,0x1
    3028:	00411025 	or	$v0,$v0,$at
    302c:	40826000 	mtc0	$v0,$12
	...
    303c:	3c010000 	lui	$at,0x0
    3040:	ac200040 	sw	$zero,64($at)
    3044:	03e00008 	jr	$ra
    3048:	00001021 	move	$v0,$zero
    304c:	00000000 	nop
*/


module_init(mxpmod_init_module);
module_exit(mxpmod_cleanup_module);

