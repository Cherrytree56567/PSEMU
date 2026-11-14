# SH at an odd address must still raise a store address error (exception code 5)
# and leave memory untouched. Assemble and link with `make test-bin` in PSEMU/.

        .set    noreorder
        .set    noat
        .text
        .globl  start
start:
        lui     $s0, 0x8002
        ori     $t0, $zero, 0x1234
        sh      $t0, 0x101($s0)         # odd address: raises the exception
        nop
