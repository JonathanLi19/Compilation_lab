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
  addi $sp, $sp, -200
  sw $ra, 196($sp)
  sw $fp, 192($sp)
  addi $fp, $sp, 200
  lw $t0, -12($fp)
  li $t1, 14
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t1, -12($fp)
  li $t2, 20
  mul $t0, $t1, $t2
  sw $t0, -16($fp)
  lw $t1, -16($fp)
  li $t2, 8
  add $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -24($fp)
  lw $t1, -20($fp)
  move $t0, $t1
  sw $t0, -24($fp)
  lw $t1, -24($fp)
  lw $t2, -12($fp)
  mul $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t1, -12($fp)
  lw $t2, -28($fp)
  add $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t1, -32($fp)
  li $t2, 100
  sub $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t0, -40($fp)
  lw $t1, -36($fp)
  move $t0, $t1
  sw $t0, -40($fp)
  lw $t1, -12($fp)
  li $t2, 1
  mul $t0, $t1, $t2
  sw $t0, -44($fp)
  lw $t1, -24($fp)
  li $t2, 2
  mul $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t1, -44($fp)
  lw $t2, -48($fp)
  add $t0, $t1, $t2
  sw $t0, -52($fp)
  lw $t1, -40($fp)
  li $t2, 3
  mul $t0, $t1, $t2
  sw $t0, -56($fp)
  lw $t1, -52($fp)
  lw $t2, -56($fp)
  add $t0, $t1, $t2
  sw $t0, -60($fp)
  lw $t1, -12($fp)
  lw $t2, -24($fp)
  mul $t0, $t1, $t2
  sw $t0, -64($fp)
  lw $t1, -64($fp)
  lw $t2, -40($fp)
  div $t1, $t2
  mflo $t0
  sw $t0, -68($fp)
  lw $t1, -60($fp)
  lw $t2, -68($fp)
  add $t0, $t1, $t2
  sw $t0, -72($fp)
  lw $t0, -76($fp)
  lw $t1, -72($fp)
  move $t0, $t1
  sw $t0, -76($fp)
  lw $t1, -24($fp)
  lw $t2, -40($fp)
  add $t0, $t1, $t2
  sw $t0, -80($fp)
  lw $t1, -80($fp)
  lw $t2, -76($fp)
  sub $t0, $t1, $t2
  sw $t0, -84($fp)
  lw $t1, -24($fp)
  lw $t2, -84($fp)
  div $t1, $t2
  mflo $t0
  sw $t0, -88($fp)
  lw $t1, -88($fp)
  lw $t2, -76($fp)
  mul $t0, $t1, $t2
  sw $t0, -92($fp)
  lw $t1, -40($fp)
  li $t2, 2
  div $t1, $t2
  mflo $t0
  sw $t0, -96($fp)
  li $t1, 1
  lw $t2, -96($fp)
  add $t0, $t1, $t2
  sw $t0, -100($fp)
  lw $t1, -92($fp)
  lw $t2, -100($fp)
  mul $t0, $t1, $t2
  sw $t0, -104($fp)
  lw $t1, -12($fp)
  lw $t2, -104($fp)
  add $t0, $t1, $t2
  sw $t0, -108($fp)
  lw $t0, -112($fp)
  lw $t1, -108($fp)
  move $t0, $t1
  sw $t0, -112($fp)
  lw $t0, -116($fp)
  li $t1, 90
  move $t0, $t1
  sw $t0, -116($fp)
  lw $t1, -116($fp)
  lw $t2, -12($fp)
  mul $t0, $t1, $t2
  sw $t0, -120($fp)
  lw $t0, -124($fp)
  lw $t1, -120($fp)
  move $t0, $t1
  sw $t0, -124($fp)
  lw $t1, -124($fp)
  li $t2, 19
  add $t0, $t1, $t2
  sw $t0, -128($fp)
  lw $t0, -132($fp)
  lw $t1, -128($fp)
  move $t0, $t1
  sw $t0, -132($fp)
  lw $t1, -132($fp)
  li $t2, 50
  sub $t0, $t1, $t2
  sw $t0, -136($fp)
  lw $t0, -140($fp)
  lw $t1, -136($fp)
  move $t0, $t1
  sw $t0, -140($fp)
  lw $t1, -140($fp)
  lw $t2, -140($fp)
  add $t0, $t1, $t2
  sw $t0, -144($fp)
  lw $t0, -148($fp)
  lw $t1, -144($fp)
  move $t0, $t1
  sw $t0, -148($fp)
  lw $t0, -152($fp)
  lw $t1, -148($fp)
  move $t0, $t1
  sw $t0, -152($fp)
  lw $t0, -24($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -76($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -116($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -132($fp)
  move $a0, $t0
  jal _func_write
  lw $t1, -148($fp)
  lw $t2, -152($fp)
  mul $t0, $t1, $t2
  sw $t0, -156($fp)
  lw $t1, -24($fp)
  lw $t2, -156($fp)
  add $t0, $t1, $t2
  sw $t0, -160($fp)
  lw $t1, -132($fp)
  lw $t2, -124($fp)
  sub $t0, $t1, $t2
  sw $t0, -164($fp)
  lw $t1, -140($fp)
  lw $t2, -164($fp)
  mul $t0, $t1, $t2
  sw $t0, -168($fp)
  lw $t1, -116($fp)
  lw $t2, -112($fp)
  add $t0, $t1, $t2
  sw $t0, -172($fp)
  lw $t1, -168($fp)
  lw $t2, -172($fp)
  div $t1, $t2
  mflo $t0
  sw $t0, -176($fp)
  lw $t1, -160($fp)
  lw $t2, -176($fp)
  sub $t0, $t1, $t2
  sw $t0, -180($fp)
  lw $t1, -76($fp)
  lw $t2, -40($fp)
  mul $t0, $t1, $t2
  sw $t0, -184($fp)
  lw $t1, -180($fp)
  lw $t2, -184($fp)
  add $t0, $t1, $t2
  sw $t0, -188($fp)
  lw $t0, -152($fp)
  lw $t1, -188($fp)
  move $t0, $t1
  sw $t0, -152($fp)
  lw $t1, -152($fp)
  lw $t2, -148($fp)
  add $t0, $t1, $t2
  sw $t0, -192($fp)
  lw $t1, -192($fp)
  lw $t2, -140($fp)
  add $t0, $t1, $t2
  sw $t0, -196($fp)
  lw $t1, -196($fp)
  lw $t2, -132($fp)
  add $t0, $t1, $t2
  sw $t0, -200($fp)
  lw $t0, -12($fp)
  lw $t1, -200($fp)
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
