.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
_func_read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

_func_write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

_func_wired_add:
  addi $sp, $sp, -80
  sw $ra, 76($sp)
  sw $fp, 72($sp)
  addi $fp, $sp, 80
  lw $t0, -12($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t1, 0($fp)
  lw $t2, 8($fp)
  add $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t1, -16($fp)
  lw $t2, 16($fp)
  add $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t1, -20($fp)
  lw $t2, 24($fp)
  add $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t1, -24($fp)
  lw $t2, 32($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t1, -28($fp)
  lw $t2, 40($fp)
  sub $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t1, -32($fp)
  lw $t2, 48($fp)
  sub $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t1, -36($fp)
  lw $t2, 56($fp)
  sub $t0, $t1, $t2
  sw $t0, -40($fp)
  lw $t1, -40($fp)
  lw $t2, 64($fp)
  add $t0, $t1, $t2
  sw $t0, -44($fp)
  lw $t0, -12($fp)
  lw $t1, -44($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t1, 4($fp)
  lw $t2, 12($fp)
  mul $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t1, -12($fp)
  lw $t2, -48($fp)
  add $t0, $t1, $t2
  sw $t0, -52($fp)
  lw $t1, 20($fp)
  lw $t2, 28($fp)
  mul $t0, $t1, $t2
  sw $t0, -56($fp)
  lw $t1, -52($fp)
  lw $t2, -56($fp)
  sub $t0, $t1, $t2
  sw $t0, -60($fp)
  lw $t1, 36($fp)
  lw $t2, 44($fp)
  mul $t0, $t1, $t2
  sw $t0, -64($fp)
  lw $t1, -60($fp)
  lw $t2, -64($fp)
  add $t0, $t1, $t2
  sw $t0, -68($fp)
  lw $t1, 52($fp)
  lw $t2, 60($fp)
  mul $t0, $t1, $t2
  sw $t0, -72($fp)
  lw $t1, -68($fp)
  lw $t2, -72($fp)
  add $t0, $t1, $t2
  sw $t0, -76($fp)
  lw $t0, -12($fp)
  lw $t1, -76($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t0, 0($fp)
  li $t1, 10
  bgt $t0, $t1, label0
  j label1
label0:
  lw $t0, -12($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  j label2
label1:
  lw $t1, -12($fp)
  lw $t2, 0($fp)
  add $t0, $t1, $t2
  sw $t0, -80($fp)
  lw $t0, -80($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label2:

_func_wired_sub:
  addi $sp, $sp, -80
  sw $ra, 76($sp)
  sw $fp, 72($sp)
  addi $fp, $sp, 80
  lw $t0, -12($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t1, 0($fp)
  lw $t2, 8($fp)
  sub $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t1, -16($fp)
  lw $t2, 16($fp)
  sub $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t1, -20($fp)
  lw $t2, 24($fp)
  sub $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t1, -24($fp)
  lw $t2, 32($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t1, -28($fp)
  lw $t2, 40($fp)
  add $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t1, -32($fp)
  lw $t2, 48($fp)
  add $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t1, -36($fp)
  lw $t2, 56($fp)
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  lw $t1, -40($fp)
  lw $t2, 64($fp)
  add $t0, $t1, $t2
  sw $t0, -44($fp)
  lw $t0, -12($fp)
  lw $t1, -44($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t1, -12($fp)
  lw $t2, 4($fp)
  add $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t1, -48($fp)
  lw $t2, 12($fp)
  sub $t0, $t1, $t2
  sw $t0, -52($fp)
  lw $t1, 20($fp)
  lw $t2, 28($fp)
  add $t0, $t1, $t2
  sw $t0, -56($fp)
  lw $t1, -56($fp)
  lw $t2, 36($fp)
  add $t0, $t1, $t2
  sw $t0, -60($fp)
  lw $t1, -60($fp)
  lw $t2, 44($fp)
  div $t1, $t2
  mflo $t0
  sw $t0, -64($fp)
  lw $t1, -52($fp)
  lw $t2, -64($fp)
  sub $t0, $t1, $t2
  sw $t0, -68($fp)
  lw $t1, 52($fp)
  lw $t2, 60($fp)
  mul $t0, $t1, $t2
  sw $t0, -72($fp)
  lw $t1, -68($fp)
  lw $t2, -72($fp)
  add $t0, $t1, $t2
  sw $t0, -76($fp)
  lw $t0, -12($fp)
  lw $t1, -76($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t0, 0($fp)
  li $t1, 10
  bgt $t0, $t1, label3
  j label4
label3:
  lw $t0, -12($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  j label5
label4:
  lw $t1, -12($fp)
  lw $t2, 0($fp)
  add $t0, $t1, $t2
  sw $t0, -80($fp)
  lw $t0, -80($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label5:

main:
  addi $sp, $sp, -392
  sw $ra, 388($sp)
  sw $fp, 384($sp)
  addi $fp, $sp, 392
  lw $t0, -80($fp)
  li $t1, 17
  move $t0, $t1
  sw $t0, -80($fp)
  lw $t0, -84($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -84($fp)
label6:
  lw $t0, -84($fp)
  lw $t1, -80($fp)
  blt $t0, $t1, label7
  j label8
label7:
  lw $t1, -84($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -88($fp)
  la $t1, -76($fp)
  lw $t2, -88($fp)
  add $t0, $t1, $t2
  sw $t0, -92($fp)
  lw $t1, -84($fp)
  li $t2, 3
  mul $t0, $t1, $t2
  sw $t0, -96($fp)
  lw $t1, -96($fp)
  li $t2, 2
  add $t0, $t1, $t2
  sw $t0, -100($fp)
  lw $t6, -92($fp)
  lw $t0, 0($t6)
  lw $t1, -100($fp)
  move $t0, $t1
  lw $t6, -92($fp)
  sw $t0, 0($t6)
  lw $t1, -84($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -104($fp)
  lw $t0, -84($fp)
  lw $t1, -104($fp)
  move $t0, $t1
  sw $t0, -84($fp)
  j label6
label8:
  li $t1, 0
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -108($fp)
  la $t1, -76($fp)
  lw $t2, -108($fp)
  add $t0, $t1, $t2
  sw $t0, -112($fp)
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -116($fp)
  la $t1, -76($fp)
  lw $t2, -116($fp)
  add $t0, $t1, $t2
  sw $t0, -120($fp)
  li $t1, 2
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -124($fp)
  la $t1, -76($fp)
  lw $t2, -124($fp)
  add $t0, $t1, $t2
  sw $t0, -128($fp)
  li $t1, 3
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -132($fp)
  la $t1, -76($fp)
  lw $t2, -132($fp)
  add $t0, $t1, $t2
  sw $t0, -136($fp)
  li $t1, 4
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -140($fp)
  la $t1, -76($fp)
  lw $t2, -140($fp)
  add $t0, $t1, $t2
  sw $t0, -144($fp)
  li $t1, 5
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -148($fp)
  la $t1, -76($fp)
  lw $t2, -148($fp)
  add $t0, $t1, $t2
  sw $t0, -152($fp)
  li $t1, 6
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -156($fp)
  la $t1, -76($fp)
  lw $t2, -156($fp)
  add $t0, $t1, $t2
  sw $t0, -160($fp)
  li $t1, 7
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -164($fp)
  la $t1, -76($fp)
  lw $t2, -164($fp)
  add $t0, $t1, $t2
  sw $t0, -168($fp)
  li $t1, 8
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -172($fp)
  la $t1, -76($fp)
  lw $t2, -172($fp)
  add $t0, $t1, $t2
  sw $t0, -176($fp)
  li $t1, 9
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -180($fp)
  la $t1, -76($fp)
  lw $t2, -180($fp)
  add $t0, $t1, $t2
  sw $t0, -184($fp)
  li $t1, 10
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -188($fp)
  la $t1, -76($fp)
  lw $t2, -188($fp)
  add $t0, $t1, $t2
  sw $t0, -192($fp)
  li $t1, 11
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -196($fp)
  la $t1, -76($fp)
  lw $t2, -196($fp)
  add $t0, $t1, $t2
  sw $t0, -200($fp)
  li $t1, 12
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -204($fp)
  la $t1, -76($fp)
  lw $t2, -204($fp)
  add $t0, $t1, $t2
  sw $t0, -208($fp)
  li $t1, 13
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -212($fp)
  la $t1, -76($fp)
  lw $t2, -212($fp)
  add $t0, $t1, $t2
  sw $t0, -216($fp)
  li $t1, 14
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -220($fp)
  la $t1, -76($fp)
  lw $t2, -220($fp)
  add $t0, $t1, $t2
  sw $t0, -224($fp)
  li $t1, 15
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -228($fp)
  la $t1, -76($fp)
  lw $t2, -228($fp)
  add $t0, $t1, $t2
  sw $t0, -232($fp)
  li $t1, 16
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -236($fp)
  la $t1, -76($fp)
  lw $t2, -236($fp)
  add $t0, $t1, $t2
  sw $t0, -240($fp)
  addi $sp,$sp,-4
  lw $t6, -240($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -232($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -224($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -216($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -208($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -200($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -192($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -184($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -176($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -168($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -160($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -152($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -144($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -136($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -128($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -120($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -112($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  jal _func_wired_add
  sw $v0, -244($fp)
  lw $t0, -248($fp)
  lw $t1, -244($fp)
  move $t0, $t1
  sw $t0, -248($fp)
  li $t1, 0
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -252($fp)
  la $t1, -76($fp)
  lw $t2, -252($fp)
  add $t0, $t1, $t2
  sw $t0, -256($fp)
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -260($fp)
  la $t1, -76($fp)
  lw $t2, -260($fp)
  add $t0, $t1, $t2
  sw $t0, -264($fp)
  li $t1, 2
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -268($fp)
  la $t1, -76($fp)
  lw $t2, -268($fp)
  add $t0, $t1, $t2
  sw $t0, -272($fp)
  li $t1, 3
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -276($fp)
  la $t1, -76($fp)
  lw $t2, -276($fp)
  add $t0, $t1, $t2
  sw $t0, -280($fp)
  li $t1, 4
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -284($fp)
  la $t1, -76($fp)
  lw $t2, -284($fp)
  add $t0, $t1, $t2
  sw $t0, -288($fp)
  li $t1, 5
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -292($fp)
  la $t1, -76($fp)
  lw $t2, -292($fp)
  add $t0, $t1, $t2
  sw $t0, -296($fp)
  li $t1, 6
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -300($fp)
  la $t1, -76($fp)
  lw $t2, -300($fp)
  add $t0, $t1, $t2
  sw $t0, -304($fp)
  li $t1, 7
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -308($fp)
  la $t1, -76($fp)
  lw $t2, -308($fp)
  add $t0, $t1, $t2
  sw $t0, -312($fp)
  li $t1, 8
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -316($fp)
  la $t1, -76($fp)
  lw $t2, -316($fp)
  add $t0, $t1, $t2
  sw $t0, -320($fp)
  li $t1, 9
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -324($fp)
  la $t1, -76($fp)
  lw $t2, -324($fp)
  add $t0, $t1, $t2
  sw $t0, -328($fp)
  li $t1, 10
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -332($fp)
  la $t1, -76($fp)
  lw $t2, -332($fp)
  add $t0, $t1, $t2
  sw $t0, -336($fp)
  li $t1, 11
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -340($fp)
  la $t1, -76($fp)
  lw $t2, -340($fp)
  add $t0, $t1, $t2
  sw $t0, -344($fp)
  li $t1, 12
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -348($fp)
  la $t1, -76($fp)
  lw $t2, -348($fp)
  add $t0, $t1, $t2
  sw $t0, -352($fp)
  li $t1, 13
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -356($fp)
  la $t1, -76($fp)
  lw $t2, -356($fp)
  add $t0, $t1, $t2
  sw $t0, -360($fp)
  li $t1, 14
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -364($fp)
  la $t1, -76($fp)
  lw $t2, -364($fp)
  add $t0, $t1, $t2
  sw $t0, -368($fp)
  li $t1, 15
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -372($fp)
  la $t1, -76($fp)
  lw $t2, -372($fp)
  add $t0, $t1, $t2
  sw $t0, -376($fp)
  li $t1, 16
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -380($fp)
  la $t1, -76($fp)
  lw $t2, -380($fp)
  add $t0, $t1, $t2
  sw $t0, -384($fp)
  addi $sp,$sp,-4
  lw $t6, -384($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -376($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -368($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -360($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -352($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -344($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -336($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -328($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -320($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -312($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -304($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -296($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -288($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -280($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -272($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -264($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $t6, -256($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  jal _func_wired_sub
  sw $v0, -388($fp)
  lw $t0, -392($fp)
  lw $t1, -388($fp)
  move $t0, $t1
  sw $t0, -392($fp)
  lw $t0, -248($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -392($fp)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
