# SH stores a halfword at any 2-byte aligned address, not only 4-byte aligned ones.
# Assemble and link with `make test-bin` in PSEMU/. Expected results are in the Makefile (target `test`).

        .set    noreorder
        .set    noat
        .text
        .globl  start
start:
        lui     $s0, 0x8002
        ori     $t0, $zero, 0x1234
        ori     $t1, $zero, 0xbeef
        sh      $t0, 0x100($s0)         # 4-byte aligned: low half of the word
        sh      $t1, 0x102($s0)         # 2-byte aligned only: high half of the word
        lw      $s1, 0x100($s0)         # s1 = 0xbeef1234
        nop                             # load delay slot
