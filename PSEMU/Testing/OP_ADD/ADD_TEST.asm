# To Compile use spim -file ADD_TEST.asm

.data

.text
.globl main

main:
    li $t0, 10
    li $t1, 20

    add $t2, $t0, $t1
