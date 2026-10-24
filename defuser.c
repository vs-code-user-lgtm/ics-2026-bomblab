#include "defuser.h"
#include <inttypes.h>
#include <stdio.h>
#define MAX_FIELDS 8
/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。objdump -d --no-show-raw-insn --disassemble=phase_01 ./bomb > phase_01.S
    uint32_t H = (uint32_t)(challenge >> 32);
    uint32_t M = (uint32_t)(challenge >> 16);
    uint32_t L = (uint32_t)challenge;
    uint32_t S0 = (H + 0xa40bu) ^ L;
    uint32_t S1 = (H + 8u * M) ^ 0xd258u;
    uint32_t S2 = (3u * S1) ^ (L + 0x9019u);
    uint32_t pass = S0 ^ S2;
    values[count++] = pass;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    int i;
    values[count++] = 3u * (uint32_t)((challenge >> 18) & 0x3f);
    int cl = 6;
    for (i = 1; i < 6; i++) {
        uint32_t prev = values[count-1];
        values[count++] = 5u * (prev + (uint32_t)i)
                        - (uint32_t)((challenge >> cl) & 0x1f);
        cl += 5;
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;    
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t sel = (uint32_t)challenge & 0x7u;
    uint32_t a   = ((uint32_t)(challenge >> 3))
                 ^ (uint32_t)(challenge >> 32);
    uint32_t n2;
    switch(sel){
        case 0: n2 = (a ^ 0xb75eu) + (a>>2); break;
        case 1: n2 = 5u*a + 0x6cd6u;  break;
        case 2: n2 = 5u*a + 0x785au;  break;
        case 3: n2 = (4u*a) ^ 0x64f1u; break;
        case 4: n2 = (4u*a) ^ 0x7089u; break;
        case 5: n2 = 5u*a + 0x3f3f9u; break;
        case 6: n2 = 5u*a + 0xd746u;  break;
        default: n2 = (a ^ 0x7fdfu) + (a>>1);
    }
    values[count++] = sel;
    values[count++] = n2;
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t a = (uint32_t)challenge & 0x7fu;
    uint32_t k = ((uint32_t)(challenge >> 7)) ^ (uint32_t)(challenge >> 32);
    uint32_t lo = 0u, hi = 0x7fu;
    uint32_t mid[8];
    char     dir[8];
    int  depth = 0;
    for (;;) {
        uint32_t m = (hi - lo)/2 + lo;
        if (m == a) {
            mid[depth] = m; dir[depth] = 1;
            depth++;
            break;
        }
        mid[depth] = m;
        if (a < m) { dir[depth] = 1; hi = m - 1; }
        else       { dir[depth] = 0; lo = m + 1; }
        depth++;
    }
    uint32_t res = (0x37u + mid[depth-1]) ^ k;
    for (int i = depth-2; i >= 0; i--) {
        res = (3u*res) ^ (mid[i] + k) ^ (dir[i] ? 0x1u : 0x8u);
    }
    values[count++] = res;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    static const uint64_t ADDR[12] = {0x405ba0,0x405bb0,0x405bc0,0x405bd0,0x405be0,0x405bf0,
                                      0x405c00,0x405c10,0x405c20,0x405c30,0x405c40,0x405c50};
    static const uint64_t NEXT[12] = {0x0,      0x405c20,0x405be0,0x405c10,0x405bd0,0x405c00,
                                      0x405c40,0x405ba0,0x405bf0,0x405bb0,0x405bc0,0x405c30};
    static const uint32_t F8[12]   = {0x142,0x715,0x018,0xb5c,0x80f,0x216,0x57f,0x31b,0xa77,0x439,0x62d,0x96b};
    static const uint32_t FC[12]   = {0xcf62,0xa227,0xdc27,0x1721,0x2a3a,0x8d4b,0xdcd5,0x1776,0x8b2f,0x4972,0xef91,0x85a1};
    static const uint64_t TABLE[8] = {0x405c50,0x405c30,0x405bb0,0x405c20,0x405bf0,0x405c00,0x405c40,0x405bc0};
    uint64_t cur = TABLE[challenge & 7];
    int idx = -1;
    for (int k = 0; k < 12; k++) if (ADDR[k] == cur) idx = k;
    uint32_t v = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32) ^ 0x16cf;
    v = 3u * v + FC[idx];
    v ^= F8[idx];
    uint32_t outv = F8[idx];
    int steps = (int)((challenge >> 3) & 3);
    for (int s = 0; s < steps; s++) {
        uint64_t nx = NEXT[idx];
        idx = -1;
        for (int k = 0; k < 12; k++) if (ADDR[k] == nx) idx = k;
        v = 3u * v + FC[idx];
        v ^= F8[idx];
        outv = F8[idx];
    }
    values[count++] = v;
    values[count++] = outv;
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    static const int TBL[8] = {5, 1, 5, 0, 2, 2, 3, 4};
    uint32_t x = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);
    int pick[2] = { TBL[challenge & 7], TBL[(challenge >> 3) & 7] };
    for (int r = 0; r < 2; r++) {
        if (r == 1) x ^= 0x44f2u;
        switch (pick[r]) {
            case 0: x = (x >> 5) | (x << 27); break;
            case 1: x = (x << 1) | (x >> 31); x ^= 0x4ed3u; break;
            case 2: x = (x ^ 0x9571u) + (x << 7); break;
            case 3: x = (0x5c72u + 4u * x) ^ (3u * x); break;
            case 4: x = (x + 0xc008u) ^ (x >> 5); break;
            case 5: x = (x ^ (x >> 2)) + 0x7812u; break;
        }
    }
    values[count++] = x + 0x6eccu;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint64_t X = challenge, Y = context;
    uint32_t px = (uint32_t)(X >> 32) ^ (uint32_t)X;
    uint32_t py = (uint32_t)(Y >> 32) ^ (uint32_t)Y;
    uint32_t z  = px ^ py;
    z= (z << 1) | (z >> 31);
    uint32_t m  = (z + 0x7a0du) ^ 0x8736u;
    uint32_t a0 = (uint32_t)Y & 3u;
    uint32_t a1 = m ^ (a0 * 0x7a0du) ^ (uint32_t)(Y >> 16);
    uint32_t a2 = a1 ^ (0x8736u + py + a0 + (px >> 1));
    int n = snprintf(out, cap, "%32sOPEN %u %u %u", "", a0, a1, a2);
    if (n < 0 || (size_t)n >= cap) return 1;
    return 0;
}
