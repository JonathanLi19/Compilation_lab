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

_func_mod:
  addi $sp, $sp, -20
  sw $ra, 16($sp)
  sw $fp, 12($sp)
  addi $fp, $sp, 20
  lw $t1, 0($fp)
  lw $t2, 4($fp)
  div $t1, $t2
  mflo $t0
  sw $t0, -12($fp)
  lw $t1, -12($fp)
  lw $t2, 4($fp)
  mul $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t1, 0($fp)
  lw $t2, -16($fp)
  sub $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

_func_is_prime:
  addi $sp, $sp, -20
  sw $ra, 16($sp)
  sw $fp, 12($sp)
  addi $fp, $sp, 20
  lw $t0, -12($fp)
  li $t1, 2
  move $t0, $t1
  sw $t0, -12($fp)
label0:
  lw $t0, -12($fp)
  lw $t1, 0($fp)
  blt $t0, $t1, label1
  j label2
label1:
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $s0, 0($fp)
  sw $s0, 0($sp)
  jal _func_mod
  sw $v0, -16($fp)
  lw $t0, -16($fp)
  li $t1, 0
  beq $t0, $t1, label3
  j label4
label3:
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label4:
  lw $t1, -12($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -12($fp)
  lw $t1, -20($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  j label0
label2:
  li $t0, 1
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

_func_fab:
  addi $sp, $sp, -28
  sw $ra, 24($sp)
  sw $fp, 20($sp)
  addi $fp, $sp, 28
  lw $t0, 0($fp)
  li $t1, 1
  beq $t0, $t1, label5
  j label8
label8:
  lw $t0, 0($fp)
  li $t1, 2
  beq $t0, $t1, label5
  j label6
label5:
  li $t0, 1
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  j label7
label6:
  lw $t1, 0($fp)
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -12($fp)
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_fab
  sw $v0, -16($fp)
  lw $t1, 0($fp)
  li $t2, 2
  sub $t0, $t1, $t2
  sw $t0, -20($fp)
  addi $sp,$sp,-4
  lw $s0, -20($fp)
  sw $s0, 0($sp)
  jal _func_fab
  sw $v0, -24($fp)
  lw $t1, -16($fp)
  lw $t2, -24($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t0, -28($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label7:

_func_encode:
  addi $sp, $sp, -100
  sw $ra, 96($sp)
  sw $fp, 92($sp)
  addi $fp, $sp, 100
  li $t1, 1
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -12($fp)
  li $t1, 2
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t1, -12($fp)
  lw $t2, -16($fp)
  add $t0, $t1, $t2
  sw $t0, -20($fp)
  li $t1, 4
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t1, -20($fp)
  lw $t2, -24($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  li $t1, 8
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t1, -28($fp)
  lw $t2, -32($fp)
  add $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t0, -40($fp)
  lw $t1, -36($fp)
  move $t0, $t1
  sw $t0, -40($fp)
  lw $t1, 0($fp)
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -44($fp)
  lw $t1, 0($fp)
  lw $t2, -44($fp)
  add $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t1, 0($fp)
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -52($fp)
  lw $t1, -52($fp)
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -56($fp)
  lw $t1, -48($fp)
  lw $t2, -56($fp)
  add $t0, $t1, $t2
  sw $t0, -60($fp)
  lw $t0, -64($fp)
  lw $t1, -60($fp)
  move $t0, $t1
  sw $t0, -64($fp)
  li $t1, 1
  lw $t2, 0($fp)
  add $t0, $t1, $t2
  sw $t0, -68($fp)
  lw $t1, -68($fp)
  lw $t2, 0($fp)
  mul $t0, $t1, $t2
  sw $t0, -72($fp)
  lw $t1, -72($fp)
  li $t2, 2
  div $t1, $t2
  mflo $t0
  sw $t0, -76($fp)
  lw $t0, -80($fp)
  lw $t1, -76($fp)
  move $t0, $t1
  sw $t0, -80($fp)
  lw $t1, -40($fp)
  lw $t2, -64($fp)
  mul $t0, $t1, $t2
  sw $t0, -84($fp)
  lw $t1, -64($fp)
  lw $t2, -80($fp)
  mul $t0, $t1, $t2
  sw $t0, -88($fp)
  lw $t1, -84($fp)
  lw $t2, -88($fp)
  add $t0, $t1, $t2
  sw $t0, -92($fp)
  lw $t1, -40($fp)
  lw $t2, -80($fp)
  mul $t0, $t1, $t2
  sw $t0, -96($fp)
  lw $t1, -92($fp)
  lw $t2, -96($fp)
  add $t0, $t1, $t2
  sw $t0, -100($fp)
  lw $t0, -100($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

main:
  addi $sp, $sp, -44
  sw $ra, 40($sp)
  sw $fp, 36($sp)
  addi $fp, $sp, 44
  lw $t0, -12($fp)
  li $t1, 5
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t0, -16($fp)
  li $t1, 1
  move $t0, $t1
  sw $t0, -16($fp)
  lw $t0, -20($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -20($fp)
label9:
  lw $t0, -16($fp)
  lw $t1, -12($fp)
  blt $t0, $t1, label10
  j label11
label10:
  addi $sp,$sp,-4
  lw $s0, -16($fp)
  sw $s0, 0($sp)
  jal _func_fab
  sw $v0, -24($fp)
  addi $sp,$sp,-4
  lw $s0, -24($fp)
  sw $s0, 0($sp)
  jal _func_is_prime
  sw $v0, -28($fp)
  lw $t0, -28($fp)
  li $t1, 0
  bne $t0, $t1, label12
  j label13
label12:
  addi $sp,$sp,-4
  lw $s0, -16($fp)
  sw $s0, 0($sp)
  jal _func_fab
  sw $v0, -32($fp)
  lw $t1, -20($fp)
  lw $t2, -32($fp)
  add $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t0, -20($fp)
  lw $t1, -36($fp)
  move $t0, $t1
  sw $t0, -20($fp)
label13:
  lw $t1, -16($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  lw $t0, -16($fp)
  lw $t1, -40($fp)
  move $t0, $t1
  sw $t0, -16($fp)
  j label9
label11:
  lw $t0, -20($fp)
  move $a0, $t0
  jal _func_write
  addi $sp,$sp,-4
  lw $s0, -20($fp)
  sw $s0, 0($sp)
  jal _func_encode
  sw $v0, -44($fp)
  lw $t0, -44($fp)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
