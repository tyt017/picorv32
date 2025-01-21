#include "firmware.h"

int clz32(uint32_t x) {
    int n = 0;
    
    // Check if the upper 16 bits are 0
    int mask = ((x & 0xFFFF0000) == 0);
    n += mask << 4;          // mask * 16
    x <<= mask << 4;         // x <<= (mask * 16)
    
    // Check if the upper 8 bits are 0
    mask = ((x & 0xFF000000) == 0);
    n += mask << 3;          // mask * 8
    x <<= mask << 3;         // x <<= (mask * 8)
    
    // Check if the upper 4 bits are 0
    mask = ((x & 0xF0000000) == 0);
    n += mask << 2;          // mask * 4
    x <<= mask << 2;         // x <<= (mask * 4)
    
    // Check if the upper 2 bits are 0
    mask = ((x & 0xC0000000) == 0);
    n += mask << 1;          // mask * 2
    x <<= mask << 1;         // x <<= (mask * 2)
    
    // Check if the highest bit is 0
    mask = ((x & 0x80000000) == 0);
    n += mask;               // mask * 1

    // If x is 0, return 32, otherwise return n
    return (x == 0) ? 32 : n;
}

void clz_software_test(uint32_t input) {
    unsigned int start_cycles, end_cycles;

    __asm__ volatile ("rdcycle %0" : "=r"(start_cycles));

    uint32_t result = clz32(input);

    __asm__ volatile ("rdcycle %0" : "=r"(end_cycles));

    print_str("RV32i implemented CLZ: ");
    print_dec(result);
    print_str("\t");
    print_str("Cycles: ");
    print_dec(end_cycles - start_cycles);
    print_str("\n");
}

void clz_hardware_test(uint32_t input) {
    unsigned int start_cycles, end_cycles;

    __asm__ volatile ("rdcycle %0" : "=r"(start_cycles));

    uint32_t result = hard_clz(input);

    __asm__ volatile ("rdcycle %0" : "=r"(end_cycles));

    print_str("RV32i_zbb CLZ: ");
    print_dec(result);
    print_str("\t\t");
    print_str("Cycles: ");
    print_dec(end_cycles - start_cycles);
    print_str("\n");
}

void clztest(void) {
    uint32_t test_data[5] = {0xFFFFFFFF, 0x00000000, 0x0EAB1234, 0x000ABCDE, 0x000000AA};

    for (int i = 0; i < 5; i++) {
        print_str("Input: ");
        print_hex(test_data[i], 8);
        print_str("\n");

        clz_software_test(test_data[i]);

        clz_hardware_test(test_data[i]);

        print_str("\n");
    }
}