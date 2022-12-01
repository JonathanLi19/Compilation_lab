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

main:
  addi $sp, $sp, -48
  sw $ra, 44($sp)
  sw $fp, 40($sp)
  addi $fp, $sp, 48
  li $t1, 0
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -20($fp)
  la $t1, -16($fp)
  lw $t2, -20($fp)
  add $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t6, -24($fp)
  lw $t0, 0($t6)
  li $t1, 4
  move $t0, $t1
  lw $t6, -24($fp)
  sw $t0, 0($t6)
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -28($fp)
  la $t1, -16($fp)
  lw $t2, -28($fp)
  add $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t6, -32($fp)
  lw $t0, 0($t6)
  li $t1, 9
  move $t0, $t1
  lw $t6, -32($fp)
  sw $t0, 0($t6)
  li $t1, 0
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -36($fp)
  la $t1, -16($fp)
  lw $t2, -36($fp)
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  lw $t6, -40($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -44($fp)
  la $t1, -16($fp)
  lw $t2, -44($fp)
  add $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t6, -48($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
