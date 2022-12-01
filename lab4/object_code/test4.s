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

_func_func_a:
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

_func_func_b:
  addi $sp, $sp, -24
  sw $ra, 20($sp)
  sw $fp, 16($sp)
  addi $fp, $sp, 24
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
  jal _func_func_a
  sw $v0, -16($fp)
  lw $t0, -20($fp)
  lw $t1, -16($fp)
  move $t0, $t1
  sw $t0, -20($fp)
  lw $t0, -20($fp)
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
  sw $t0, -24($fp)
  lw $t0, -12($fp)
  lw $t1, -24($fp)
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

_func_func_c:
  addi $sp, $sp, -28
  sw $ra, 24($sp)
  sw $fp, 20($sp)
  addi $fp, $sp, 28
  lw $t0, -12($fp)
  li $t1, 2
  move $t0, $t1
  sw $t0, -12($fp)
label5:
  lw $t1, -12($fp)
  lw $t2, -12($fp)
  mul $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  lw $t1, 0($fp)
  ble $t0, $t1, label6
  j label7
label6:
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_func_b
  sw $v0, -20($fp)
  lw $t0, -20($fp)
  li $t1, 0
  bne $t0, $t1, label8
  j label9
label8:
  lw $t1, -12($fp)
  lw $t2, -12($fp)
  mul $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t0, -24($fp)
  lw $t1, 0($fp)
  beq $t0, $t1, label10
  j label11
label10:
  li $t0, 1
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label11:
label9:
  lw $t1, -12($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t0, -12($fp)
  lw $t1, -28($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  j label5
label7:
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

main:
  addi $sp, $sp, -72
  sw $ra, 68($sp)
  sw $fp, 64($sp)
  addi $fp, $sp, 72
  lw $t0, -20($fp)
  li $t1, 2
  move $t0, $t1
  sw $t0, -20($fp)
  li $t1, 0
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -24($fp)
  la $t1, -16($fp)
  lw $t2, -24($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  jal _func_read
  move $t0, $v0
  sw $t0, -32($fp)
  lw $t6, -28($fp)
  lw $t0, 0($t6)
  lw $t1, -32($fp)
  move $t0, $t1
  lw $t6, -28($fp)
  sw $t0, 0($t6)
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -36($fp)
  la $t1, -16($fp)
  lw $t2, -36($fp)
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  jal _func_read
  move $t0, $v0
  sw $t0, -44($fp)
  lw $t6, -40($fp)
  lw $t0, 0($t6)
  lw $t1, -44($fp)
  move $t0, $t1
  lw $t6, -40($fp)
  sw $t0, 0($t6)
  lw $t0, -48($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -48($fp)
label12:
  lw $t0, -48($fp)
  lw $t1, -20($fp)
  blt $t0, $t1, label13
  j label14
label13:
  lw $t1, -48($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -52($fp)
  la $t1, -16($fp)
  lw $t2, -52($fp)
  add $t0, $t1, $t2
  sw $t0, -56($fp)
  addi $sp,$sp,-4
  lw $t6, -56($fp)
  lw $s0, 0($t6)
  sw $s0, 0($sp)
  jal _func_func_c
  sw $v0, -60($fp)
  lw $t0, -60($fp)
  li $t1, 0
  bne $t0, $t1, label15
  j label16
label15:
  lw $t1, -48($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -64($fp)
  la $t1, -16($fp)
  lw $t2, -64($fp)
  add $t0, $t1, $t2
  sw $t0, -68($fp)
  lw $t6, -68($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
label16:
  lw $t1, -48($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -72($fp)
  lw $t0, -48($fp)
  lw $t1, -72($fp)
  move $t0, $t1
  sw $t0, -48($fp)
  j label12
label14:
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
