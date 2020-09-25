
.text
.globl main
main:
	la $t1,array
	li $s1,10
L1: beq $s1,$s2,L2
li $v0,5
syscall
sw $v0,0($t1)
addi $t1,$t1,4
addi $s2,$s2,1
j L1
li $s1,40
li $s2,0
li $s3,4  


J 
