# LH and LHU read from the full 32-bit address. Assemble and link with `make test-bin`
# in PSEMU/. Expected results are in the Makefile (target `test`).
#
# RAM offset 0 holds 0x11223344, so a load16 that only sees the low 8 bits of the
# address reads the halfword at offset 0 or 2 of RAM instead of the requested one.

        .set    noreorder
        .set    noat
        .text
        .globl  start
start:
        lui     $s0, 0x8002
        lui     $t0, 0xbeef
        ori     $t0, $t0, 0x1234
        sw      $t0, 0x100($s0)         # halfwords: 0x1234 at +0x100, 0xbeef at +0x102
        lui     $s2, 0x8000
        lui     $t1, 0x1122
        ori     $t1, $t1, 0x3344
        sw      $t1, 0($s2)             # halfwords: 0x3344 at +0, 0x1122 at +2
        lhu     $s3, 0x102($s0)         # s3 = 0x0000beef
        nop                             # load delay slot
        lh      $s4, 0x102($s0)         # s4 = 0xffffbeef (sign extended)
        nop
        lhu     $s5, 0x100($s0)         # s5 = 0x00001234
        nop
