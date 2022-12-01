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

_func_f1:
  addi $sp, $sp, -12
  sw $ra, 8($sp)
  sw $fp, 4($sp)
  addi $fp, $sp, 12
  lw $t1, 0($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

_func_f:
  addi $sp, $sp, -16
  sw $ra, 12($sp)
  sw $fp, 8($sp)
  addi $fp, $sp, 16
  addi $sp,$sp,-4
  lw $s0, 0($fp)
  sw $s0, 0($sp)
  jal _func_f1
  sw $v0, -12($fp)
  lw $t1, -12($fp)
  li $t2, 3
  add $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

main:
  addi $sp, $sp, -16
  sw $ra, 12($sp)
  sw $fp, 8($sp)
  addi $fp, $sp, 16
  lw $t0, -12($fp)
  li $t1, 1
  move $t0, $t1
  sw $t0, -12($fp)
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_f
  sw $v0, -16($fp)
  lw $t0, -12($fp)
  lw $t1, -16($fp)
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  move $a0, $t0
  jal _func_write
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
