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

_func_fact:
  addi $sp, $sp, -20
  sw $ra, 16($sp)
  sw $fp, 12($sp)
  addi $fp, $sp, 20
  lw $t0, 0($fp)
  li $t1, 1
  beq $t0, $t1, label0
  j label1
label0:
  lw $t0, 0($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  j label2
label1:
  lw $t1, 0($fp)
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -12($fp)
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_fact
  sw $v0, -16($fp)
  lw $t1, 0($fp)
  lw $t2, -16($fp)
  mul $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label2:

main:
  addi $sp, $sp, -24
  sw $ra, 20($sp)
  sw $fp, 16($sp)
  addi $fp, $sp, 24
  jal _func_read
  move $t0, $v0
  sw $t0, -12($fp)
  lw $t0, -16($fp)
  lw $t1, -12($fp)
  move $t0, $t1
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  li $t1, 1
  bgt $t0, $t1, label3
  j label4
label3:
  addi $sp,$sp,-4
  lw $s0, -16($fp)
  sw $s0, 0($sp)
  jal _func_fact
  sw $v0, -20($fp)
  lw $t0, -24($fp)
  lw $t1, -20($fp)
  move $t0, $t1
  sw $t0, -24($fp)
  j label5
label4:
  lw $t0, -24($fp)
  li $t1, 1
  move $t0, $t1
  sw $t0, -24($fp)
label5:
  lw $t0, -24($fp)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
