# An exception taken while $at is 10 is an ordinary exception. Assemble and link with
# `make test-bin` in PSEMU/. Expected: the run ends with exception code 8 (syscall).

        .set    noreorder
        .set    noat
        .text
        .globl  start
start:
        ori     $at, $zero, 10
        syscall
        nop
