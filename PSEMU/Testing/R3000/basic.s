# Basic R3000 integer test. Assemble and link with `make test-bin` in PSEMU/.
# The program runs from 0x80010000 and ends by falling off its last instruction.
# Expected register and memory results are listed in the Makefile (target `test`).

        .set    noreorder
        .set    noat
        .text
        .globl  start
start:
        # ADD / ADDU / ADDI / ADDIU / SUBU
        addiu   $t0, $zero, 10
        addiu   $t1, $zero, 20
        add     $t2, $t0, $t1           # t2 = 30
        lui     $t3, 0x7fff
        ori     $t3, $t3, 0xffff        # t3 = 0x7fffffff
        addu    $t4, $t3, $t0           # t4 = 0x80000009, ADDU never traps on overflow
        addi    $t5, $t0, -3            # t5 = 7
        addiu   $t6, $t0, -11           # t6 = 0xffffffff
        subu    $t7, $t1, $t0           # t7 = 10

        # Stores and loads through RAM at 0x80020000
        lui     $s0, 0x8002
        sw      $t2, 0($s0)             # mem[0x80020000] = 30
        sw      $t4, 4($s0)             # mem[0x80020004] = 0x80000009
        lw      $s1, 0($s0)             # s1 = 30
        nop                             # load delay slot
        lw      $s2, 4($s0)             # s2 = 0x80000009
        nop
        addiu   $t8, $zero, 0xab
        sb      $t8, 8($s0)
        lbu     $s3, 8($s0)             # s3 = 0xab
        nop
        lb      $s6, 8($s0)             # s6 = 0xffffffab (sign extended)
        nop

        # Backward branch: s4 = 5 + 4 + 3 + 2 + 1
        move    $s4, $zero
        addiu   $s5, $zero, 5
loop:
        addu    $s4, $s4, $s5
        addiu   $s5, $s5, -1
        bne     $s5, $zero, loop
        nop                             # delay slot

        # A taken branch still runs its delay slot, and skips the next instruction
        beq     $zero, $zero, taken
        addiu   $v0, $zero, 1           # delay slot: v0 = 1
        addiu   $v1, $zero, 99          # skipped, v1 stays 0
taken:
        # An untaken branch runs its delay slot and falls through
        bne     $t0, $t0, nowhere
        addiu   $a0, $zero, 5           # delay slot: a0 = 5
        addiu   $a1, $zero, 7           # a1 = 7
nowhere:
        nop
