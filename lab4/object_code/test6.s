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

_func_fab:
  addi $sp, $sp, -28
  sw $ra, 24($sp)
  sw $fp, 20($sp)
  addi $fp, $sp, 28
  lw $t0, 0($fp)
  li $t1, 1
  beq $t0, $t1, label0
  j label3
label3:
  lw $t0, 0($fp)
  li $t1, 2
  beq $t0, $t1, label0
  j label1
label0:
  li $t0, 1
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
label2:

main:
  addi $sp, $sp, -12
  sw $ra, 8($sp)
  sw $fp, 4($sp)
  addi $fp, $sp, 12
  addi $sp,$sp,-4
  li $s0, 1
  sw $s0, 0($sp)
  jal _func_fab
  sw $v0, -12($fp)
  lw $t0, -12($fp)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
