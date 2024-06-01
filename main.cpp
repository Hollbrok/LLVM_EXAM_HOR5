//  CPU INSTRUCTION FORMATS:
//  Registers: 16 registers (32 bits) R0-R15
//
//  FF FF FF FF
//        ^^ ^^ - R3/IMM - third register or immediate
//      ^       - R2     - second register
//     ^        - R1     - first register
//  ^^          - opcode - operation code

//  INSTRUCTION SET ARCHITECTURE:
//  [OPC] INSTR ARGS        (OPERATION)
//  0x53  WRITE R1          (WRITE R1)
//  0x54  MOV   R1, R2      (R1 = R2)
//  0x55  MOVli R1, IMM     (R1 = IMM)
//  0x56  MOVhi R1, IMM     (R1 = IMM << 16)
//  0x57  ADD   R1, R2, R3  (R1 = R2 + R3)
//  0x58  ADDi  R1, R2, IMM (R1 = R2 + IMM)
//  0x59  SUB   R1, R2, R3  (R1 = R2 - R3)
//  0x60  SUBi  R1, R2, IMM (R1 = R2 - IMM)
//  0x61  MUL   R1, R2, R3  (R1 = R2 * R3)
//  0x62  MULi  R1, R2, IMM (R1 = R2 * IMM)
//  0x63  DIV   R1, R2, R3  (R1 = R2 / R3)
//  0x64  DIVi  R1, R2, IMM (R1 = R2 / IMM)


#include <stdio.h>
#include <stdint.h>

#define OPCODE_MASK 0xFF000000
#define R1_MASK     0x00F00000
#define R2_MASK     0x000F0000
#define R3IMM_MASK  0x0000FFFF

#define GET_OPCODE(instr) ((instr & OPCODE_MASK) >> 24)
#define GET_R1(instr)     ((instr & R1_MASK) >> 20)
#define GET_R2(instr)     ((instr & R2_MASK) >> 16)
#define GET_R3IMM(instr)  (instr & R3IMM_MASK)

//

void decodeInstruction(uint32_t instr) {
    uint8_t opcode = GET_OPCODE(instr);
    uint8_t r1 = GET_R1(instr);
    uint8_t r2 = GET_R2(instr);
    uint16_t r3_imm = GET_R3IMM(instr);

    switch (opcode) {
        case 0x53:
            printf("WRITE R%d\n", r1);
            break;
        case 0x54:
            printf("MOV R%d, R%d\n", r1, r2);
            break;
        case 0x55:
            printf("MOVli R%d, %d\n", r1, r3_imm);
            break;
        case 0x56:
            printf("MOVhi R%d, %d\n", r1, r3_imm);
            break;
        case 0x57:
            printf("ADD R%d, R%d, R%d\n", r1, r2, r3_imm);
            break;
        case 0x58:
            printf("ADDi R%d, R%d, %d\n", r1, r2, r3_imm);
            break;
        case 0x59:
            printf("SUB R%d, R%d, R%d\n", r1, r2, r3_imm);
            break;
        case 0x60:
            printf("SUBi R%d, R%d, %d\n", r1, r2, r3_imm);
            break;
        case 0x61:
            printf("MUL R%d, R%d, R%d\n", r1, r2, r3_imm);
            break;
        case 0x62:
            printf("MULi R%d, R%d, %d\n", r1, r2, r3_imm);
            break;
        case 0x63:
            printf("DIV R%d, R%d, R%d\n", r1, r2, r3_imm);
            break;
        case 0x64:
            printf("DIVi R%d, R%d, %d\n", r1, r2, r3_imm);
            break;
        default:
            printf("UNKNOWN INSTRUCTION\n");
            break;
    }
}

void executeInstruction(uint32_t instr, uint32_t *registers) {
    uint8_t opcode = GET_OPCODE(instr);
    uint8_t r1 = GET_R1(instr);
    uint8_t r2 = GET_R2(instr);
    uint16_t r3_imm = GET_R3IMM(instr);

    switch (opcode) {
        case 0x53:
            printf("%d\n", registers[r1]);
            break;
        case 0x54:
            registers[r1] = registers[r2];
            break;
        case 0x55:
            registers[r1] = r3_imm;
            break;
        case 0x56:
            registers[r1] = r3_imm << 16;
            break;
        case 0x57:
            registers[r1] = registers[r2] + registers[r3_imm];
            break;
        case 0x58:
            registers[r1] = registers[r2] + r3_imm;
            break;
        case 0x59:
            registers[r1] = registers[r2] - registers[r3_imm];
            break;
        case 0x60:
            registers[r1] = registers[r2] - r3_imm;
            break;
        case 0x61:
            registers[r1] = registers[r2] * registers[r3_imm];
            break;
        case 0x62:
            registers[r1] = registers[r2] * r3_imm;
            break;
        case 0x63:
            registers[r1] = registers[r2] / registers[r3_imm];
            break;
        case 0x64:
            registers[r1] = registers[r2] / r3_imm;
            break;
        default:
            printf("UNKNOWN INSTRUCTION\n");
            break;
    }
}

//

void printAsm(uint32_t * bin, uint32_t binSize) {
    printf("INSTRUCTIONS:\n");
    for (int i = 0; i < binSize; ++i) {
        decodeInstruction(bin[i]);
    }
    printf("\n");
}

void execute(uint32_t * bin, uint32_t binSize) {
    printf("EXECUTION:\n");
    uint32_t registers[16] = {0};
    for (int i = 0; i < binSize; ++i) {
        executeInstruction(bin[i], registers);
    }
    printf("\n");
}

int main() {
    uint32_t BIN[] = {0x56000001, 0x53000000, 0x64100100, 0x53100000,\
                      0x58210010, 0x53200000, 0x60310010, 0x53300000,\
                      0x57320003, 0x53300000, 0x59230002, 0x53200000,\
                      0x59330002, 0x53300000, 0x57120003, 0x53100000,\
                      0x62110010, 0x53100000, 0x63000001, 0x53000000};
    printAsm(BIN, sizeof(BIN)/sizeof(uint32_t));
    execute(BIN, sizeof(BIN)/sizeof(uint32_t));
    return 0;
}

// EXPECTED OUTPUT:
//
// INSTRUCTIONS:
// MOVhi R0, 1
// WRITE R0
// DIVi R1, R0, 256
// WRITE R1
// ADDi R2, R1, 16
// WRITE R2
// SUBi R3, R1, 16
// WRITE R3
// ADD R3, R2, R3
// WRITE R3
// SUB R2, R3, R2
// WRITE R2
// SUB R3, R3, R2
// WRITE R3
// ADD R1, R2, R3
// WRITE R1
// MULi R1, R1, 16
// WRITE R1
// DIV R0, R0, R1
// WRITE R0
// 
// EXECUTION:
// 65536
// 256
// 272
// 240
// 512
// 240
// 272
// 512
// 8192
// 8