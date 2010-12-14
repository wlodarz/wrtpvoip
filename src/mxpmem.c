
char devname = "mem";
/*
m/lib/modules/2.4.17_mvl21-malta-mips_fp_le/kernel/drivers/mxpmem.o:     file format elf32-tradlittlemips

Disassembly of section .text:

0000000000000000 <mxp_segGetName-0x334>:
   0:	00002021 	move	$a0,$zero
   4:	00002821 	move	$a1,$zero
   8:	3c030000 	lui	$v1,0x0
   c:	00651821 	addu	$v1,$v1,$a1
  10:	8c630000 	lw	$v1,0($v1)
  14:	2402ffff 	li	$v0,-1
  18:	54620003 	0x54620003
  1c:	24840001 	addiu	$a0,$a0,1
  20:	03e00008 	jr	$ra
  24:	00801021 	move	$v0,$a0
  28:	28820008 	slti	$v0,$a0,8
  2c:	1440fff6 	bnez	$v0,8 <mxp_segGetName-0x32c>
  30:	24a50024 	addiu	$a1,$a1,36
  34:	03e00008 	jr	$ra
  38:	2402ffff 	li	$v0,-1
  3c:	27bdffe0 	addiu	$sp,$sp,-32
  40:	afbf001c 	sw	$ra,28($sp)
  44:	afb20018 	sw	$s2,24($sp)
  48:	afb10014 	sw	$s1,20($sp)
  4c:	afb00010 	sw	$s0,16($sp)
  50:	00809021 	move	$s2,$a0
  54:	00008821 	move	$s1,$zero
  58:	00008021 	move	$s0,$zero
  5c:	3c040000 	lui	$a0,0x0
  60:	24840000 	addiu	$a0,$a0,0
  64:	3c030000 	lui	$v1,0x0
  68:	00701821 	addu	$v1,$v1,$s0
  6c:	8c630000 	lw	$v1,0($v1)
  70:	2402ffff 	li	$v0,-1
  74:	1062000a 	beq	$v1,$v0,a0 <mxp_segGetName-0x294>
  78:	24840004 	addiu	$a0,$a0,4
  7c:	02042021 	addu	$a0,$s0,$a0
  80:	3c020000 	lui	$v0,0x0
  84:	24420ec0 	addiu	$v0,$v0,3776
  88:	0040f809 	jalr	$v0
  8c:	02402821 	move	$a1,$s2
  90:	54400004 	0x54400004
  94:	26310001 	addiu	$s1,$s1,1
  98:	0800002d 	j	b4 <mxp_segGetName-0x280>
  9c:	02201021 	move	$v0,$s1
  a0:	26310001 	addiu	$s1,$s1,1
  a4:	2a220008 	slti	$v0,$s1,8
  a8:	1440ffec 	bnez	$v0,5c <mxp_segGetName-0x2d8>
  ac:	26100024 	addiu	$s0,$s0,36
  b0:	2402ffff 	li	$v0,-1
  b4:	8fbf001c 	lw	$ra,28($sp)
  b8:	8fb20018 	lw	$s2,24($sp)
  bc:	8fb10014 	lw	$s1,20($sp)
  c0:	8fb00010 	lw	$s0,16($sp)
  c4:	03e00008 	jr	$ra
  c8:	27bd0020 	addiu	$sp,$sp,32
*/
void func1()
{
/*
  cc:	27bdffe8 	addiu	$sp,$sp,-24
  d0:	afbf0014 	sw	$ra,20($sp)
  d4:	afb00010 	sw	$s0,16($sp)
  d8:	3c100000 	lui	$s0,0x0
  dc:	26100000 	addiu	$s0,$s0,0
  e0:	02002021 	move	$a0,$s0
  e4:	00002821 	move	$a1,$zero
  e8:	3c020000 	lui	$v0,0x0
  ec:	24420000 	addiu	$v0,$v0,0
  f0:	0040f809 	jalr	$v0
  f4:	24060120 	li	$a2,288
  f8:	00002821 	move	$a1,$zero
  fc:	24040007 	li	$a0,7
 100:	00b01821 	addu	$v1,$a1,$s0
 104:	2402ffff 	li	$v0,-1
 108:	ac620000 	sw	$v0,0($v1)
 10c:	2484ffff 	addiu	$a0,$a0,-1
 110:	0481fffb 	bgez	$a0,100 <mxp_segGetName-0x234>
 114:	24a50024 	addiu	$a1,$a1,36
 118:	8fbf0014 	lw	$ra,20($sp)
 11c:	8fb00010 	lw	$s0,16($sp)
 120:	03e00008 	jr	$ra
 124:	27bd0018 	addiu	$sp,$sp,24
*/
}
void func2()
{
/*
 128:	27bdffd8 	addiu	$sp,$sp,-40
 12c:	afbf0024 	sw	$ra,36($sp)
 130:	afb40020 	sw	$s4,32($sp)
 134:	afb3001c 	sw	$s3,28($sp)
 138:	afb20018 	sw	$s2,24($sp)
 13c:	afb10014 	sw	$s1,20($sp)
 140:	afb00010 	sw	$s0,16($sp)
 144:	0080a021 	move	$s4,$a0
 148:	40136000 	mfc0	$s3,$12
 14c:	00000000 	nop
 150:	36610001 	ori	$at,$s3,0x1
 154:	38210001 	xori	$at,$at,0x1
 158:	40816000 	mtc0	$at,$12
 15c:	00000040 	sll	$zero,$zero,0x1
 160:	00000040 	sll	$zero,$zero,0x1
 164:	00000040 	sll	$zero,$zero,0x1
 168:	3c020000 	lui	$v0,0x0
 16c:	24420000 	addiu	$v0,$v0,0
 170:	0040f809 	jalr	$v0
 174:	2410ffff 	li	$s0,-1
 178:	00408821 	move	$s1,$v0
 17c:	1630000c 	bne	$s1,$s0,1b0 <mxp_segGetName-0x184>
 180:	26920018 	addiu	$s2,$s4,24
 184:	40016000 	mfc0	$at,$12
 188:	32730001 	andi	$s3,$s3,0x1
 18c:	34210001 	ori	$at,$at,0x1
 190:	38210001 	xori	$at,$at,0x1
 194:	02619825 	or	$s3,$s3,$at
 198:	40936000 	mtc0	$s3,$12
*/
}
/*
	...
 1a8:	080000a3 	j	28c <mxp_segGetName-0xa8>
 1ac:	24020014 	li	$v0,20
 1b0:	3c020000 	lui	$v0,0x0
 1b4:	2442003c 	addiu	$v0,$v0,60
 1b8:	0040f809 	jalr	$v0
 1bc:	02402021 	move	$a0,$s2
 1c0:	14500028 	bne	$v0,$s0,264 <mxp_segGetName-0xd0>
 1c4:	001180c0 	sll	$s0,$s1,0x3
 1c8:	02118021 	addu	$s0,$s0,$s1
 1cc:	00108080 	sll	$s0,$s0,0x2
 1d0:	3c010000 	lui	$at,0x0
 1d4:	00300821 	addu	$at,$at,$s0
 1d8:	ac310000 	sw	$s1,0($at)
 1dc:	3c040000 	lui	$a0,0x0
 1e0:	24840004 	addiu	$a0,$a0,4
 1e4:	00902021 	addu	$a0,$a0,$s0
 1e8:	3c020000 	lui	$v0,0x0
 1ec:	24420ef8 	addiu	$v0,$v0,3832
 1f0:	0040f809 	jalr	$v0
 1f4:	02402821 	move	$a1,$s2
 1f8:	8e820014 	lw	$v0,20($s4)
 1fc:	3c010000 	lui	$at,0x0
 200:	00300821 	addu	$at,$at,$s0
 204:	ac220014 	sw	$v0,20($at)
 208:	8e82000c 	lw	$v0,12($s4)
 20c:	3c010000 	lui	$at,0x0
 210:	00300821 	addu	$at,$at,$s0
 214:	ac20001c 	sw	$zero,28($at)
 218:	3c010000 	lui	$at,0x0
 21c:	00300821 	addu	$at,$at,$s0
 220:	ac220018 	sw	$v0,24($at)
 224:	8e82000c 	lw	$v0,12($s4)
 228:	3c010000 	lui	$at,0x0
 22c:	00300821 	addu	$at,$at,$s0
 230:	ac220020 	sw	$v0,32($at)
 234:	ae910004 	sw	$s1,4($s4)
 238:	40016000 	mfc0	$at,$12
 23c:	32730001 	andi	$s3,$s3,0x1
 240:	34210001 	ori	$at,$at,0x1
 244:	38210001 	xori	$at,$at,0x1
 248:	02619825 	or	$s3,$s3,$at
 24c:	40936000 	mtc0	$s3,$12
	...
 25c:	080000a3 	j	28c <mxp_segGetName-0xa8>
 260:	00001021 	move	$v0,$zero
 264:	40016000 	mfc0	$at,$12
 268:	32730001 	andi	$s3,$s3,0x1
 26c:	34210001 	ori	$at,$at,0x1
 270:	38210001 	xori	$at,$at,0x1
 274:	02619825 	or	$s3,$s3,$at
 278:	40936000 	mtc0	$s3,$12
	...
 288:	24020003 	li	$v0,3
 28c:	8fbf0024 	lw	$ra,36($sp)
 290:	8fb40020 	lw	$s4,32($sp)
 294:	8fb3001c 	lw	$s3,28($sp)
 298:	8fb20018 	lw	$s2,24($sp)
 29c:	8fb10014 	lw	$s1,20($sp)
 2a0:	8fb00010 	lw	$s0,16($sp)
 2a4:	03e00008 	jr	$ra
 2a8:	27bd0028 	addiu	$sp,$sp,40
 2ac:	27bdffe0 	addiu	$sp,$sp,-32
 2b0:	afbf0018 	sw	$ra,24($sp)
 2b4:	afb10014 	sw	$s1,20($sp)
 2b8:	afb00010 	sw	$s0,16($sp)
 2bc:	00808021 	move	$s0,$a0
 2c0:	40116000 	mfc0	$s1,$12
 2c4:	00000000 	nop
 2c8:	36210001 	ori	$at,$s1,0x1
 2cc:	38210001 	xori	$at,$at,0x1
 2d0:	40816000 	mtc0	$at,$12
 2d4:	00000040 	sll	$zero,$zero,0x1
 2d8:	00000040 	sll	$zero,$zero,0x1
 2dc:	00000040 	sll	$zero,$zero,0x1
 2e0:	3c020000 	lui	$v0,0x0
 2e4:	2442003c 	addiu	$v0,$v0,60
 2e8:	0040f809 	jalr	$v0
 2ec:	26040018 	addiu	$a0,$s0,24
 2f0:	ae020004 	sw	$v0,4($s0)
 2f4:	00021027 	nor	$v0,$zero,$v0
 2f8:	2c420001 	sltiu	$v0,$v0,1
 2fc:	40016000 	mfc0	$at,$12
 300:	32310001 	andi	$s1,$s1,0x1
 304:	34210001 	ori	$at,$at,0x1
 308:	38210001 	xori	$at,$at,0x1
 30c:	02218825 	or	$s1,$s1,$at
 310:	40916000 	mtc0	$s1,$12
	...
 320:	8fbf0018 	lw	$ra,24($sp)
 324:	8fb10014 	lw	$s1,20($sp)
 328:	8fb00010 	lw	$s0,16($sp)
 32c:	03e00008 	jr	$ra
 330:	27bd0020 	addiu	$sp,$sp,32

0000000000000334 <mxp_segGetName>:
 334:	27bdffe8 	addiu	$sp,$sp,-24
 338:	afbf0014 	sw	$ra,20($sp)
 33c:	afb00010 	sw	$s0,16($sp)
 340:	40106000 	mfc0	$s0,$12
 344:	00000000 	nop
 348:	36010001 	ori	$at,$s0,0x1
 34c:	38210001 	xori	$at,$at,0x1
 350:	40816000 	mtc0	$at,$12
 354:	00000040 	sll	$zero,$zero,0x1
 358:	00000040 	sll	$zero,$zero,0x1
 35c:	00000040 	sll	$zero,$zero,0x1
 360:	8c830004 	lw	$v1,4($a0)
 364:	2c620008 	sltiu	$v0,$v1,8
 368:	1040000b 	beqz	$v0,398 <mxp_segGetName+0x64>
 36c:	000310c0 	sll	$v0,$v1,0x3
 370:	00431021 	addu	$v0,$v0,$v1
 374:	00023080 	sll	$a2,$v0,0x2
 378:	3c050000 	lui	$a1,0x0
 37c:	24a50000 	addiu	$a1,$a1,0
 380:	3c030000 	lui	$v1,0x0
 384:	00661821 	addu	$v1,$v1,$a2
 388:	8c630000 	lw	$v1,0($v1)
 38c:	2402ffff 	li	$v0,-1
 390:	5462000c 	0x5462000c
 394:	24a50004 	addiu	$a1,$a1,4
 398:	40016000 	mfc0	$at,$12
 39c:	32100001 	andi	$s0,$s0,0x1
 3a0:	34210001 	ori	$at,$at,0x1
 3a4:	38210001 	xori	$at,$at,0x1
 3a8:	02018025 	or	$s0,$s0,$at
 3ac:	40906000 	mtc0	$s0,$12
	...
 3bc:	08000100 	j	400 <mxp_segGetName+0xcc>
 3c0:	24020015 	li	$v0,21
 3c4:	24840018 	addiu	$a0,$a0,24
 3c8:	3c020000 	lui	$v0,0x0
 3cc:	24420ef8 	addiu	$v0,$v0,3832
 3d0:	0040f809 	jalr	$v0
 3d4:	00c52821 	addu	$a1,$a2,$a1
 3d8:	40016000 	mfc0	$at,$12
 3dc:	32100001 	andi	$s0,$s0,0x1
 3e0:	34210001 	ori	$at,$at,0x1
 3e4:	38210001 	xori	$at,$at,0x1
 3e8:	02018025 	or	$s0,$s0,$at
 3ec:	40906000 	mtc0	$s0,$12
	...
 3fc:	00001021 	move	$v0,$zero
 400:	8fbf0014 	lw	$ra,20($sp)
 404:	8fb00010 	lw	$s0,16($sp)
 408:	03e00008 	jr	$ra
 40c:	27bd0018 	addiu	$sp,$sp,24
 410:	27bdffc8 	addiu	$sp,$sp,-56
 414:	afbf0034 	sw	$ra,52($sp)
 418:	afbe0030 	sw	$s8,48($sp)
 41c:	afb7002c 	sw	$s7,44($sp)
 420:	afb60028 	sw	$s6,40($sp)
 424:	afb50024 	sw	$s5,36($sp)
 428:	afb40020 	sw	$s4,32($sp)
 42c:	afb3001c 	sw	$s3,28($sp)
 430:	afb20018 	sw	$s2,24($sp)
 434:	afb10014 	sw	$s1,20($sp)
 438:	afb00010 	sw	$s0,16($sp)
 43c:	00809021 	move	$s2,$a0
 440:	8e42000c 	lw	$v0,12($s2)
 444:	2403fffc 	li	$v1,-4
 448:	24420003 	addiu	$v0,$v0,3
 44c:	00431024 	and	$v0,$v0,$v1
 450:	24540004 	addiu	$s4,$v0,4
 454:	40156000 	mfc0	$s5,$12
 458:	00000000 	nop
 45c:	36a10001 	ori	$at,$s5,0x1
 460:	38210001 	xori	$at,$at,0x1
 464:	40816000 	mtc0	$at,$12
 468:	00000040 	sll	$zero,$zero,0x1
 46c:	00000040 	sll	$zero,$zero,0x1
 470:	00000040 	sll	$zero,$zero,0x1
 474:	8e430004 	lw	$v1,4($s2)
 478:	2c620008 	sltiu	$v0,$v1,8
 47c:	1040000b 	beqz	$v0,4ac <mxp_segGetName+0x178>
 480:	000310c0 	sll	$v0,$v1,0x3
 484:	00431021 	addu	$v0,$v0,$v1
 488:	00022880 	sll	$a1,$v0,0x2
 48c:	3c110000 	lui	$s1,0x0
 490:	26310000 	addiu	$s1,$s1,0
 494:	3c030000 	lui	$v1,0x0
 498:	00651821 	addu	$v1,$v1,$a1
 49c:	8c630000 	lw	$v1,0($v1)
 4a0:	2402ffff 	li	$v0,-1
 4a4:	5462000c 	0x5462000c
 4a8:	8e44000c 	lw	$a0,12($s2)
 4ac:	40016000 	mfc0	$at,$12
 4b0:	32b50001 	andi	$s5,$s5,0x1
 4b4:	34210001 	ori	$at,$at,0x1
 4b8:	38210001 	xori	$at,$at,0x1
 4bc:	02a1a825 	or	$s5,$s5,$at
 4c0:	40956000 	mtc0	$s5,$12
	...
 4d0:	0800019a 	j	668 <mxp_segGetName+0x334>
 4d4:	24020015 	li	$v0,21
 4d8:	8e420010 	lw	$v0,16($s2)
 4dc:	00820018 	mult	$a0,$v0
 4e0:	26360020 	addiu	$s6,$s1,32
 4e4:	2622001c 	addiu	$v0,$s1,28
 4e8:	00a23021 	addu	$a2,$a1,$v0
 4ec:	00b61021 	addu	$v0,$a1,$s6
 4f0:	8cd30000 	lw	$s3,0($a2)
 4f4:	8c420000 	lw	$v0,0($v0)
 4f8:	0013f0c0 	sll	$s8,$s3,0x3
 4fc:	03d31821 	addu	$v1,$s8,$s3
 500:	00038080 	sll	$s0,$v1,0x2
 504:	00002012 	mflo	$a0
 508:	00501023 	subu	$v0,$v0,$s0
 50c:	24840024 	addiu	$a0,$a0,36
 510:	0044102a 	slt	$v0,$v0,$a0
 514:	1040000c 	beqz	$v0,548 <mxp_segGetName+0x214>
 518:	26620001 	addiu	$v0,$s3,1
 51c:	40016000 	mfc0	$at,$12
 520:	32b50001 	andi	$s5,$s5,0x1
 524:	34210001 	ori	$at,$at,0x1
 528:	38210001 	xori	$at,$at,0x1
 52c:	02a1a825 	or	$s5,$s5,$at
 530:	40956000 	mtc0	$s5,$12
	...
 540:	0800019a 	j	668 <mxp_segGetName+0x334>
 544:	24020012 	li	$v0,18
 548:	acc20000 	sw	$v0,0($a2)
 54c:	26310014 	addiu	$s1,$s1,20
 550:	8e430004 	lw	$v1,4($s2)
 554:	26450018 	addiu	$a1,$s2,24
 558:	000310c0 	sll	$v0,$v1,0x3
 55c:	00431021 	addu	$v0,$v0,$v1
 560:	00021080 	sll	$v0,$v0,0x2
 564:	00511021 	addu	$v0,$v0,$s1
 568:	00031c00 	sll	$v1,$v1,0x10
 56c:	00731821 	addu	$v1,$v1,$s3
 570:	8c570000 	lw	$s7,0($v0)
 574:	3c020000 	lui	$v0,0x0
 578:	24420ef8 	addiu	$v0,$v0,3832
 57c:	02178021 	addu	$s0,$s0,$s7
 580:	ae030000 	sw	$v1,0($s0)
 584:	0040f809 	jalr	$v0
 588:	26040004 	addiu	$a0,$s0,4
 58c:	ae140014 	sw	$s4,20($s0)
 590:	8e420010 	lw	$v0,16($s2)
 594:	ae020018 	sw	$v0,24($s0)
 598:	8e420010 	lw	$v0,16($s2)
 59c:	ae02001c 	sw	$v0,28($s0)
 5a0:	8e440010 	lw	$a0,16($s2)
 5a4:	02840018 	mult	$s4,$a0
 5a8:	8e430004 	lw	$v1,4($s2)
 5ac:	000310c0 	sll	$v0,$v1,0x3
 5b0:	00431021 	addu	$v0,$v0,$v1
 5b4:	00021080 	sll	$v0,$v0,0x2
 5b8:	00561021 	addu	$v0,$v0,$s6
 5bc:	8c430000 	lw	$v1,0($v0)
 5c0:	00002012 	mflo	$a0
 5c4:	00641823 	subu	$v1,$v1,$a0
 5c8:	ac430000 	sw	$v1,0($v0)
 5cc:	8e430004 	lw	$v1,4($s2)
 5d0:	000310c0 	sll	$v0,$v1,0x3
 5d4:	00431021 	addu	$v0,$v0,$v1
 5d8:	00021080 	sll	$v0,$v0,0x2
 5dc:	00518821 	addu	$s1,$v0,$s1
 5e0:	00561021 	addu	$v0,$v0,$s6
 5e4:	8e230000 	lw	$v1,0($s1)
 5e8:	8c420000 	lw	$v0,0($v0)
 5ec:	00621821 	addu	$v1,$v1,$v0
 5f0:	ae030020 	sw	$v1,32($s0)
 5f4:	8e440010 	lw	$a0,16($s2)
 5f8:	1880000c 	blez	$a0,62c <mxp_segGetName+0x2f8>
 5fc:	00002821 	move	$a1,$zero
 600:	2482ffff 	addiu	$v0,$a0,-1
 604:	00a2102a 	slt	$v0,$a1,$v0
 608:	10400002 	beqz	$v0,614 <mxp_segGetName+0x2e0>
 60c:	00003021 	move	$a2,$zero
 610:	00743021 	addu	$a2,$v1,$s4
 614:	ac660000 	sw	$a2,0($v1)
 618:	8e440010 	lw	$a0,16($s2)
 61c:	24a50001 	addiu	$a1,$a1,1
 620:	00a4102a 	slt	$v0,$a1,$a0
 624:	1440fff6 	bnez	$v0,600 <mxp_segGetName+0x2cc>
 628:	00741821 	addu	$v1,$v1,$s4
 62c:	03d31021 	addu	$v0,$s8,$s3
 630:	00021080 	sll	$v0,$v0,0x2
 634:	00571021 	addu	$v0,$v0,$s7
 638:	8c420000 	lw	$v0,0($v0)
 63c:	ae420008 	sw	$v0,8($s2)
 640:	40016000 	mfc0	$at,$12
 644:	32b50001 	andi	$s5,$s5,0x1
 648:	34210001 	ori	$at,$at,0x1
 64c:	38210001 	xori	$at,$at,0x1
 650:	02a1a825 	or	$s5,$s5,$at
 654:	40956000 	mtc0	$s5,$12
	...
 664:	00001021 	move	$v0,$zero
 668:	8fbf0034 	lw	$ra,52($sp)
 66c:	8fbe0030 	lw	$s8,48($sp)
 670:	8fb7002c 	lw	$s7,44($sp)
 674:	8fb60028 	lw	$s6,40($sp)
 678:	8fb50024 	lw	$s5,36($sp)
 67c:	8fb40020 	lw	$s4,32($sp)
 680:	8fb3001c 	lw	$s3,28($sp)
 684:	8fb20018 	lw	$s2,24($sp)
 688:	8fb10014 	lw	$s1,20($sp)
 68c:	8fb00010 	lw	$s0,16($sp)
 690:	03e00008 	jr	$ra
 694:	27bd0038 	addiu	$sp,$sp,56
 698:	27bdffd8 	addiu	$sp,$sp,-40
 69c:	afbf0024 	sw	$ra,36($sp)
 6a0:	afb40020 	sw	$s4,32($sp)
 6a4:	afb3001c 	sw	$s3,28($sp)
 6a8:	afb20018 	sw	$s2,24($sp)
 6ac:	afb10014 	sw	$s1,20($sp)
 6b0:	afb00010 	sw	$s0,16($sp)
 6b4:	0080a021 	move	$s4,$a0
 6b8:	40136000 	mfc0	$s3,$12
 6bc:	00000000 	nop
 6c0:	36610001 	ori	$at,$s3,0x1
 6c4:	38210001 	xori	$at,$at,0x1
 6c8:	40816000 	mtc0	$at,$12
 6cc:	00000040 	sll	$zero,$zero,0x1
 6d0:	00000040 	sll	$zero,$zero,0x1
 6d4:	00000040 	sll	$zero,$zero,0x1
 6d8:	8e830004 	lw	$v1,4($s4)
 6dc:	2c620008 	sltiu	$v0,$v1,8
 6e0:	1040000b 	beqz	$v0,710 <mxp_segGetName+0x3dc>
 6e4:	000310c0 	sll	$v0,$v1,0x3
 6e8:	00431021 	addu	$v0,$v0,$v1
 6ec:	00022080 	sll	$a0,$v0,0x2
 6f0:	3c050000 	lui	$a1,0x0
 6f4:	24a50000 	addiu	$a1,$a1,0
 6f8:	3c030000 	lui	$v1,0x0
 6fc:	00641821 	addu	$v1,$v1,$a0
 700:	8c630000 	lw	$v1,0($v1)
 704:	2402ffff 	li	$v0,-1
 708:	1462000c 	bne	$v1,$v0,73c <mxp_segGetName+0x408>
 70c:	00a41821 	addu	$v1,$a1,$a0
 710:	40016000 	mfc0	$at,$12
 714:	32730001 	andi	$s3,$s3,0x1
 718:	34210001 	ori	$at,$at,0x1
 71c:	38210001 	xori	$at,$at,0x1
 720:	02619825 	or	$s3,$s3,$at
 724:	40936000 	mtc0	$s3,$12
	...
 734:	08000200 	j	800 <mxp_segGetName+0x4cc>
 738:	24020015 	li	$v0,21
 73c:	00601021 	move	$v0,$v1
 740:	8c42001c 	lw	$v0,28($v0)
 744:	10400024 	beqz	$v0,7d8 <mxp_segGetName+0x4a4>
 748:	8c630014 	lw	$v1,20($v1)
 74c:	10400022 	beqz	$v0,7d8 <mxp_segGetName+0x4a4>
 750:	00009021 	move	$s2,$zero
 754:	24700004 	addiu	$s0,$v1,4
 758:	00608821 	move	$s1,$v1
 75c:	02002021 	move	$a0,$s0
 760:	3c020000 	lui	$v0,0x0
 764:	24420ec0 	addiu	$v0,$v0,3776
 768:	0040f809 	jalr	$v0
 76c:	26850018 	addiu	$a1,$s4,24
 770:	1440000e 	bnez	$v0,7ac <mxp_segGetName+0x478>
 774:	26100024 	addiu	$s0,$s0,36
 778:	8e220000 	lw	$v0,0($s1)
 77c:	ae820008 	sw	$v0,8($s4)
 780:	40016000 	mfc0	$at,$12
 784:	32730001 	andi	$s3,$s3,0x1
 788:	34210001 	ori	$at,$at,0x1
 78c:	38210001 	xori	$at,$at,0x1
 790:	02619825 	or	$s3,$s3,$at
 794:	40936000 	mtc0	$s3,$12
	...
 7a4:	08000200 	j	800 <mxp_segGetName+0x4cc>
 7a8:	00001021 	move	$v0,$zero
 7ac:	8e820004 	lw	$v0,4($s4)
 7b0:	000218c0 	sll	$v1,$v0,0x3
 7b4:	00621821 	addu	$v1,$v1,$v0
 7b8:	00031880 	sll	$v1,$v1,0x2
 7bc:	3c020000 	lui	$v0,0x0
 7c0:	00431021 	addu	$v0,$v0,$v1
 7c4:	8c42001c 	lw	$v0,28($v0)
 7c8:	26520001 	addiu	$s2,$s2,1
 7cc:	0242102b 	sltu	$v0,$s2,$v0
 7d0:	1440ffe2 	bnez	$v0,75c <mxp_segGetName+0x428>
 7d4:	26310024 	addiu	$s1,$s1,36
 7d8:	40016000 	mfc0	$at,$12
 7dc:	32730001 	andi	$s3,$s3,0x1
 7e0:	34210001 	ori	$at,$at,0x1
 7e4:	38210001 	xori	$at,$at,0x1
 7e8:	02619825 	or	$s3,$s3,$at
 7ec:	40936000 	mtc0	$s3,$12
	...
 7fc:	24020001 	li	$v0,1
 800:	8fbf0024 	lw	$ra,36($sp)
 804:	8fb40020 	lw	$s4,32($sp)
 808:	8fb3001c 	lw	$s3,28($sp)
 80c:	8fb20018 	lw	$s2,24($sp)
 810:	8fb10014 	lw	$s1,20($sp)
 814:	8fb00010 	lw	$s0,16($sp)
 818:	03e00008 	jr	$ra
 81c:	27bd0028 	addiu	$sp,$sp,40
 820:	8c820008 	lw	$v0,8($a0)
 824:	00021c02 	srl	$v1,$v0,0x10
 828:	3046ffff 	andi	$a2,$v0,0xffff
 82c:	40086000 	mfc0	$t0,$12
 830:	00000000 	nop
 834:	35010001 	ori	$at,$t0,0x1
 838:	38210001 	xori	$at,$at,0x1
 83c:	40816000 	mtc0	$at,$12
 840:	00000040 	sll	$zero,$zero,0x1
 844:	00000040 	sll	$zero,$zero,0x1
 848:	00000040 	sll	$zero,$zero,0x1
 84c:	2c620008 	sltiu	$v0,$v1,8
 850:	1040000b 	beqz	$v0,880 <mxp_segGetName+0x54c>
 854:	000310c0 	sll	$v0,$v1,0x3
 858:	00431021 	addu	$v0,$v0,$v1
 85c:	00022880 	sll	$a1,$v0,0x2
 860:	3c070000 	lui	$a3,0x0
 864:	24e70000 	addiu	$a3,$a3,0
 868:	3c030000 	lui	$v1,0x0
 86c:	00651821 	addu	$v1,$v1,$a1
 870:	8c630000 	lw	$v1,0($v1)
 874:	2402ffff 	li	$v0,-1
 878:	1462000c 	bne	$v1,$v0,8ac <mxp_segGetName+0x578>
 87c:	00e51021 	addu	$v0,$a3,$a1
 880:	40016000 	mfc0	$at,$12
 884:	31080001 	andi	$t0,$t0,0x1
 888:	34210001 	ori	$at,$at,0x1
 88c:	38210001 	xori	$at,$at,0x1
 890:	01014025 	or	$t0,$t0,$at
 894:	40886000 	mtc0	$t0,$12
	...
 8a4:	03e00008 	jr	$ra
 8a8:	24020018 	li	$v0,24
 8ac:	8c42001c 	lw	$v0,28($v0)
 8b0:	00c2102b 	sltu	$v0,$a2,$v0
 8b4:	10400013 	beqz	$v0,904 <mxp_segGetName+0x5d0>
 8b8:	00e51821 	addu	$v1,$a3,$a1
 8bc:	000610c0 	sll	$v0,$a2,0x3
 8c0:	00461021 	addu	$v0,$v0,$a2
 8c4:	8c630014 	lw	$v1,20($v1)
 8c8:	00021080 	sll	$v0,$v0,0x2
 8cc:	00431021 	addu	$v0,$v0,$v1
 8d0:	8c42001c 	lw	$v0,28($v0)
 8d4:	ac820010 	sw	$v0,16($a0)
 8d8:	40016000 	mfc0	$at,$12
 8dc:	31080001 	andi	$t0,$t0,0x1
 8e0:	34210001 	ori	$at,$at,0x1
 8e4:	38210001 	xori	$at,$at,0x1
 8e8:	01014025 	or	$t0,$t0,$at
 8ec:	40886000 	mtc0	$t0,$12
	...
 8fc:	03e00008 	jr	$ra
 900:	00001021 	move	$v0,$zero
 904:	40016000 	mfc0	$at,$12
 908:	31080001 	andi	$t0,$t0,0x1
 90c:	34210001 	ori	$at,$at,0x1
 910:	38210001 	xori	$at,$at,0x1
 914:	01014025 	or	$t0,$t0,$at
 918:	40886000 	mtc0	$t0,$12
	...
 928:	03e00008 	jr	$ra
 92c:	24020018 	li	$v0,24
 930:	8c820008 	lw	$v0,8($a0)
 934:	00021c02 	srl	$v1,$v0,0x10
 938:	3045ffff 	andi	$a1,$v0,0xffff
 93c:	40076000 	mfc0	$a3,$12
 940:	00000000 	nop
 944:	34e10001 	ori	$at,$a3,0x1
 948:	38210001 	xori	$at,$at,0x1
 94c:	40816000 	mtc0	$at,$12
 950:	00000040 	sll	$zero,$zero,0x1
 954:	00000040 	sll	$zero,$zero,0x1
 958:	00000040 	sll	$zero,$zero,0x1
 95c:	2c620008 	sltiu	$v0,$v1,8
 960:	1040000b 	beqz	$v0,990 <mxp_segGetName+0x65c>
 964:	000310c0 	sll	$v0,$v1,0x3
 968:	00431021 	addu	$v0,$v0,$v1
 96c:	00021880 	sll	$v1,$v0,0x2
 970:	3c060000 	lui	$a2,0x0
 974:	24c60000 	addiu	$a2,$a2,0
 978:	3c020000 	lui	$v0,0x0
 97c:	00431021 	addu	$v0,$v0,$v1
 980:	8c420000 	lw	$v0,0($v0)
 984:	2408ffff 	li	$t0,-1
 988:	1448000c 	bne	$v0,$t0,9bc <mxp_segGetName+0x688>
 98c:	00c31021 	addu	$v0,$a2,$v1
 990:	40016000 	mfc0	$at,$12
 994:	30e70001 	andi	$a3,$a3,0x1
 998:	34210001 	ori	$at,$at,0x1
 99c:	38210001 	xori	$at,$at,0x1
 9a0:	00e13825 	or	$a3,$a3,$at
 9a4:	40876000 	mtc0	$a3,$12
	...
 9b4:	03e00008 	jr	$ra
 9b8:	24020018 	li	$v0,24
 9bc:	8c42001c 	lw	$v0,28($v0)
 9c0:	00a2102b 	sltu	$v0,$a1,$v0
 9c4:	1440000c 	bnez	$v0,9f8 <mxp_segGetName+0x6c4>
 9c8:	00c31821 	addu	$v1,$a2,$v1
 9cc:	40016000 	mfc0	$at,$12
 9d0:	30e70001 	andi	$a3,$a3,0x1
 9d4:	34210001 	ori	$at,$at,0x1
 9d8:	38210001 	xori	$at,$at,0x1
 9dc:	00e13825 	or	$a3,$a3,$at
 9e0:	40876000 	mtc0	$a3,$12
	...
 9f0:	03e00008 	jr	$ra
 9f4:	24020018 	li	$v0,24
 9f8:	000510c0 	sll	$v0,$a1,0x3
 9fc:	00451021 	addu	$v0,$v0,$a1
 a00:	8c630014 	lw	$v1,20($v1)
 a04:	00021080 	sll	$v0,$v0,0x2
 a08:	00432821 	addu	$a1,$v0,$v1
 a0c:	8ca2001c 	lw	$v0,28($a1)
 a10:	10400015 	beqz	$v0,a68 <mxp_segGetName+0x734>
 a14:	00000000 	nop
 a18:	8ca30020 	lw	$v1,32($a1)
 a1c:	8c620000 	lw	$v0,0($v1)
 a20:	aca20020 	sw	$v0,32($a1)
 a24:	24620004 	addiu	$v0,$v1,4
 a28:	ac820014 	sw	$v0,20($a0)
 a2c:	ac680000 	sw	$t0,0($v1)
 a30:	8ca2001c 	lw	$v0,28($a1)
 a34:	2442ffff 	addiu	$v0,$v0,-1
 a38:	aca2001c 	sw	$v0,28($a1)
 a3c:	40016000 	mfc0	$at,$12
 a40:	30e70001 	andi	$a3,$a3,0x1
 a44:	34210001 	ori	$at,$at,0x1
 a48:	38210001 	xori	$at,$at,0x1
 a4c:	00e13825 	or	$a3,$a3,$at
 a50:	40876000 	mtc0	$a3,$12
	...
 a60:	03e00008 	jr	$ra
 a64:	00001021 	move	$v0,$zero
 a68:	40016000 	mfc0	$at,$12
 a6c:	30e70001 	andi	$a3,$a3,0x1
 a70:	34210001 	ori	$at,$at,0x1
 a74:	38210001 	xori	$at,$at,0x1
 a78:	00e13825 	or	$a3,$a3,$at
 a7c:	40876000 	mtc0	$a3,$12
	...
 a8c:	03e00008 	jr	$ra
 a90:	24020012 	li	$v0,18
 a94:	8c820008 	lw	$v0,8($a0)
 a98:	00021c02 	srl	$v1,$v0,0x10
 a9c:	3045ffff 	andi	$a1,$v0,0xffff
 aa0:	40076000 	mfc0	$a3,$12
 aa4:	00000000 	nop
 aa8:	34e10001 	ori	$at,$a3,0x1
 aac:	38210001 	xori	$at,$at,0x1
 ab0:	40816000 	mtc0	$at,$12
 ab4:	00000040 	sll	$zero,$zero,0x1
 ab8:	00000040 	sll	$zero,$zero,0x1
 abc:	00000040 	sll	$zero,$zero,0x1
 ac0:	2c620008 	sltiu	$v0,$v1,8
 ac4:	1040000b 	beqz	$v0,af4 <mxp_segGetName+0x7c0>
 ac8:	000310c0 	sll	$v0,$v1,0x3
 acc:	00431021 	addu	$v0,$v0,$v1
 ad0:	00021880 	sll	$v1,$v0,0x2
 ad4:	3c060000 	lui	$a2,0x0
 ad8:	24c60000 	addiu	$a2,$a2,0
 adc:	3c020000 	lui	$v0,0x0
 ae0:	00431021 	addu	$v0,$v0,$v1
 ae4:	8c420000 	lw	$v0,0($v0)
 ae8:	2408ffff 	li	$t0,-1
 aec:	1448000c 	bne	$v0,$t0,b20 <mxp_segGetName+0x7ec>
 af0:	00c31021 	addu	$v0,$a2,$v1
 af4:	40016000 	mfc0	$at,$12
 af8:	30e70001 	andi	$a3,$a3,0x1
 afc:	34210001 	ori	$at,$at,0x1
 b00:	38210001 	xori	$at,$at,0x1
 b04:	00e13825 	or	$a3,$a3,$at
 b08:	40876000 	mtc0	$a3,$12
	...
 b18:	03e00008 	jr	$ra
 b1c:	24020018 	li	$v0,24
 b20:	8c42001c 	lw	$v0,28($v0)
 b24:	00a2102b 	sltu	$v0,$a1,$v0
 b28:	1440000c 	bnez	$v0,b5c <mxp_segGetName+0x828>
 b2c:	00c31821 	addu	$v1,$a2,$v1
 b30:	40016000 	mfc0	$at,$12
 b34:	30e70001 	andi	$a3,$a3,0x1
 b38:	34210001 	ori	$at,$at,0x1
 b3c:	38210001 	xori	$at,$at,0x1
 b40:	00e13825 	or	$a3,$a3,$at
 b44:	40876000 	mtc0	$a3,$12
	...
 b54:	03e00008 	jr	$ra
 b58:	24020018 	li	$v0,24
 b5c:	8c840014 	lw	$a0,20($a0)
 b60:	8c630014 	lw	$v1,20($v1)
 b64:	8c82fffc 	lw	$v0,-4($a0)
 b68:	14480016 	bne	$v0,$t0,bc4 <mxp_segGetName+0x890>
 b6c:	2486fffc 	addiu	$a2,$a0,-4
 b70:	000510c0 	sll	$v0,$a1,0x3
 b74:	00451021 	addu	$v0,$v0,$a1
 b78:	00021080 	sll	$v0,$v0,0x2
 b7c:	00431021 	addu	$v0,$v0,$v1
 b80:	8c430020 	lw	$v1,32($v0)
 b84:	ac83fffc 	sw	$v1,-4($a0)
 b88:	8c43001c 	lw	$v1,28($v0)
 b8c:	ac460020 	sw	$a2,32($v0)
 b90:	24630001 	addiu	$v1,$v1,1
 b94:	ac43001c 	sw	$v1,28($v0)
 b98:	40016000 	mfc0	$at,$12
 b9c:	30e70001 	andi	$a3,$a3,0x1
 ba0:	34210001 	ori	$at,$at,0x1
 ba4:	38210001 	xori	$at,$at,0x1
 ba8:	00e13825 	or	$a3,$a3,$at
 bac:	40876000 	mtc0	$a3,$12
	...
 bbc:	03e00008 	jr	$ra
 bc0:	00001021 	move	$v0,$zero
 bc4:	40016000 	mfc0	$at,$12
 bc8:	30e70001 	andi	$a3,$a3,0x1
 bcc:	34210001 	ori	$at,$at,0x1
 bd0:	38210001 	xori	$at,$at,0x1
 bd4:	00e13825 	or	$a3,$a3,$at
 bd8:	40876000 	mtc0	$a3,$12
	...
 be8:	03e00008 	jr	$ra
 bec:	24020016 	li	$v0,22
*/

void mxp_mem_syscall()
{
/*
0000000000000bf0 <mxp_mem_syscall>:
 bf0:	27bdffe8 	addiu	$sp,$sp,-24
 bf4:	8c820000 	lw	$v0,0($a0)
 bf8:	2443f700 	addiu	$v1,$v0,-2304
 bfc:	2c620009 	sltiu	$v0,$v1,9
 c00:	1040003d 	beqz	$v0,cf8 <mxp_mem_syscall+0x108>
 c04:	afbf0010 	sw	$ra,16($sp)
 c08:	00031080 	sll	$v0,$v1,0x2
 c0c:	3c010000 	lui	$at,0x0
 c10:	00220821 	addu	$at,$at,$v0
 c14:	8c2200c8 	lw	$v0,200($at)
 c18:	00400008 	jr	$v0
 c1c:	00000000 	nop
 c20:	3c020000 	lui	$v0,0x0
 c24:	24420930 	addiu	$v0,$v0,2352
 c28:	0040f809 	jalr	$v0
 c2c:	00000000 	nop
 c30:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 c34:	8fbf0010 	lw	$ra,16($sp)
 c38:	3c020000 	lui	$v0,0x0
 c3c:	24420a94 	addiu	$v0,$v0,2708
 c40:	0040f809 	jalr	$v0
 c44:	00000000 	nop
 c48:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 c4c:	8fbf0010 	lw	$ra,16($sp)
 c50:	3c020000 	lui	$v0,0x0
 c54:	24420820 	addiu	$v0,$v0,2080
 c58:	0040f809 	jalr	$v0
 c5c:	00000000 	nop
 c60:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 c64:	8fbf0010 	lw	$ra,16($sp)
 c68:	3c020000 	lui	$v0,0x0
 c6c:	24420698 	addiu	$v0,$v0,1688
 c70:	0040f809 	jalr	$v0
 c74:	00000000 	nop
 c78:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 c7c:	8fbf0010 	lw	$ra,16($sp)
 c80:	3c020000 	lui	$v0,0x0
 c84:	24420410 	addiu	$v0,$v0,1040
 c88:	0040f809 	jalr	$v0
 c8c:	00000000 	nop
 c90:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 c94:	8fbf0010 	lw	$ra,16($sp)
 c98:	3c020000 	lui	$v0,0x0
 c9c:	24420128 	addiu	$v0,$v0,296
 ca0:	0040f809 	jalr	$v0
 ca4:	00000000 	nop
 ca8:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 cac:	8fbf0010 	lw	$ra,16($sp)
 cb0:	3c020000 	lui	$v0,0x0
 cb4:	244202ac 	addiu	$v0,$v0,684
 cb8:	0040f809 	jalr	$v0
 cbc:	00000000 	nop
 cc0:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 cc4:	8fbf0010 	lw	$ra,16($sp)
 cc8:	3c020000 	lui	$v0,0x0
 ccc:	24420334 	addiu	$v0,$v0,820
 cd0:	0040f809 	jalr	$v0
 cd4:	00000000 	nop
 cd8:	08000340 	j	d00 <mxp_mem_syscall+0x110>
 cdc:	8fbf0010 	lw	$ra,16($sp)
 ce0:	3c020000 	lui	$v0,0x0
 ce4:	244200cc 	addiu	$v0,$v0,204
 ce8:	0040f809 	jalr	$v0
 cec:	00000000 	nop
 cf0:	0800033f 	j	cfc <mxp_mem_syscall+0x10c>
 cf4:	00001021 	move	$v0,$zero
 cf8:	2402fffe 	li	$v0,-2
 cfc:	8fbf0010 	lw	$ra,16($sp)
 d00:	03e00008 	jr	$ra
 d04:	27bd0018 	addiu	$sp,$sp,24
 d08:	27bdffc8 	addiu	$sp,$sp,-56
 d0c:	afbf0034 	sw	$ra,52($sp)
 d10:	afb40030 	sw	$s4,48($sp)
 d14:	afb3002c 	sw	$s3,44($sp)
 d18:	afb20028 	sw	$s2,40($sp)
 d1c:	afb10024 	sw	$s1,36($sp)
 d20:	afb00020 	sw	$s0,32($sp)
 d24:	3c050000 	lui	$a1,0x0
 d28:	24a500ec 	addiu	$a1,$a1,236
 d2c:	3c130000 	lui	$s3,0x0
 d30:	26730000 	addiu	$s3,$s3,0
 d34:	0260f809 	jalr	$s3
 d38:	0080a021 	move	$s4,$a0
 d3c:	00408821 	move	$s1,$v0
 d40:	00008021 	move	$s0,$zero
 d44:	24120007 	li	$s2,7
 d48:	02912021 	addu	$a0,$s4,$s1
 d4c:	3c050000 	lui	$a1,0x0
 d50:	24a50108 	addiu	$a1,$a1,264
 d54:	3c020000 	lui	$v0,0x0
 d58:	24420000 	addiu	$v0,$v0,0
 d5c:	02024821 	addu	$t1,$s0,$v0
 d60:	24470004 	addiu	$a3,$v0,4
 d64:	00501821 	addu	$v1,$v0,$s0
 d68:	00603021 	move	$a2,$v1
 d6c:	00604021 	move	$t0,$v1
 d70:	00601021 	move	$v0,$v1
 d74:	02073821 	addu	$a3,$s0,$a3
 d78:	26100024 	addiu	$s0,$s0,36
 d7c:	8c630014 	lw	$v1,20($v1)
 d80:	2652ffff 	addiu	$s2,$s2,-1
 d84:	afa30010 	sw	$v1,16($sp)
 d88:	8cc30018 	lw	$v1,24($a2)
 d8c:	8d06001c 	lw	$a2,28($t0)
 d90:	afa30014 	sw	$v1,20($sp)
 d94:	afa60018 	sw	$a2,24($sp)
 d98:	8c420020 	lw	$v0,32($v0)
 d9c:	8d260000 	lw	$a2,0($t1)
 da0:	0260f809 	jalr	$s3
 da4:	afa2001c 	sw	$v0,28($sp)
 da8:	0641ffe7 	bgez	$s2,d48 <mxp_mem_syscall+0x158>
 dac:	02228821 	addu	$s1,$s1,$v0
 db0:	24030001 	li	$v1,1
 db4:	8fa20048 	lw	$v0,72($sp)
 db8:	8fbf0034 	lw	$ra,52($sp)
 dbc:	8fb40030 	lw	$s4,48($sp)
 dc0:	8fb3002c 	lw	$s3,44($sp)
 dc4:	8fb20028 	lw	$s2,40($sp)
 dc8:	8fb00020 	lw	$s0,32($sp)
 dcc:	ac430000 	sw	$v1,0($v0)
 dd0:	02201021 	move	$v0,$s1
 dd4:	8fb10024 	lw	$s1,36($sp)
 dd8:	03e00008 	jr	$ra
 ddc:	27bd0038 	addiu	$sp,$sp,56
*/
}

void init_module()
{
/*
0000000000000de0 <init_module>:
 de0:	27bdffe8 	addiu	$sp,$sp,-24
 de4:	afbf0010 	sw	$ra,16($sp)
*/
/*
 de8:	3c020000 	lui	$v0,0x0
 dec:	244200cc 	addiu	$v0,$v0,204
 df0:	0040f809 	jalr	$v0
 df4:	00000000 	nop
*/
	func1();
/*
 df8:	3c040000 	lui	$a0,0x0
 dfc:	24840128 	addiu	$a0,$a0,296
*/
	string = devname;

/*
 e00:	3c060000 	lui	$a2,0x0
 e04:	8cc60000 	lw	$a2,0($a2)
 e08:	3c020000 	lui	$v0,0x0
 e0c:	24420000 	addiu	$v0,$v0,0
 e10:	0040f809 	jalr	$v0
 e14:	00002821 	move	$a1,$zero
*/
	if (create_proc_entry(devname, 0, mxp_proc_dir) != 0) {
	
/*
 e18:	00401821 	move	$v1,$v0
 e1c:	10600005 	beqz	$v1,e34 <init_module+0x54>
 e20:	00000000 	nop
 e24:	3c020000 	lui	$v0,0x0
 e28:	24420d08 	addiu	$v0,$v0,3336
 e2c:	ac62003c 	sw	$v0,60($v1)
 e30:	ac600038 	sw	$zero,56($v1)
*/
	}

/*
 e34:	3c020000 	lui	$v0,0x0 // text
 e38:	24420bf0 	addiu	$v0,$v0,3056
 e3c:	3c010000 	lui	$at,0x0 // ti_syscall
 e40:	ac220010 	sw	$v0,16($at)
*/
	ti_syscalls = 0; 
/*
 e44:	3c040000 	lui	$a0,0x0 // rodata
 e48:	2484012c 	addiu	$a0,$a0,300
 e4c:	3c020000 	lui	$v0,0x0 //printk
 e50:	24420000 	addiu	$v0,$v0,0
 e54:	0040f809 	jalr	$v0
 e58:	00000000 	nop
*/
	printk(KERN_ERR "mem.MXP memory module loaded\n");
/*
 e5c:	8fbf0010 	lw	$ra,16($sp)
 e60:	00001021 	move	$v0,$zero
 e64:	03e00008 	jr	$ra
 e68:	27bd0018 	addiu	$sp,$sp,24
*/
	return;
}

void cleanup_module()
{
/*
0000000000000e6c <cleanup_module>:
 e6c:	27bdffe8 	addiu	$sp,$sp,-24
 e70:	afbf0010 	sw	$ra,16($sp)
 e74:	3c040000 	lui	$a0,0x0
 e78:	24840128 	addiu	$a0,$a0,296
 e7c:	3c050000 	lui	$a1,0x0
 e80:	8ca50000 	lw	$a1,0($a1)
 e84:	3c020000 	lui	$v0,0x0
 e88:	24420000 	addiu	$v0,$v0,0
 e8c:	0040f809 	jalr	$v0
 e90:	00000000 	nop
 e94:	3c010000 	lui	$at,0x0
 e98:	ac200010 	sw	$zero,16($at)
 e9c:	3c040000 	lui	$a0,0x0
 ea0:	24840148 	addiu	$a0,$a0,328
 ea4:	3c020000 	lui	$v0,0x0
 ea8:	24420000 	addiu	$v0,$v0,0
 eac:	0040f809 	jalr	$v0
 eb0:	00000000 	nop
 eb4:	8fbf0010 	lw	$ra,16($sp)
 eb8:	03e00008 	jr	$ra
 ebc:	27bd0018 	addiu	$sp,$sp,24
 ec0:	080003b4 	j	ed0 <cleanup_module+0x64>
 ec4:	80830000 	lb	$v1,0($a0)
 ec8:	24a50001 	addiu	$a1,$a1,1
 ecc:	80830000 	lb	$v1,0($a0)
 ed0:	80a20000 	lb	$v0,0($a1)
 ed4:	54620004 	0x54620004
 ed8:	80820000 	lb	$v0,0($a0)
 edc:	5460fffa 	0x5460fffa
 ee0:	24840001 	addiu	$a0,$a0,1
 ee4:	80820000 	lb	$v0,0($a0)
 ee8:	80a30000 	lb	$v1,0($a1)
 eec:	00431026 	xor	$v0,$v0,$v1
 ef0:	03e00008 	jr	$ra
 ef4:	0002102b 	sltu	$v0,$zero,$v0
 ef8:	080003c4 	j	f10 <cleanup_module+0xa4>
 efc:	80a20000 	lb	$v0,0($a1)
 f00:	a0830000 	sb	$v1,0($a0)
 f04:	24a50001 	addiu	$a1,$a1,1
 f08:	24840001 	addiu	$a0,$a0,1
 f0c:	80a20000 	lb	$v0,0($a1)
 f10:	1440fffb 	bnez	$v0,f00 <cleanup_module+0x94>
 f14:	90a30000 	lbu	$v1,0($a1)
 f18:	03e00008 	jr	$ra
 f1c:	a0800000 	sb	$zero,0($a0)
*/
}
