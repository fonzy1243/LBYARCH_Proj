%include "io64.inc"

section .data

var dq 0.45
var1 dq 255.0

section .text
global main
main:
    
    movsd xmm0, [var]
    movsd xmm1, [var1]
    vmulsd xmm3, xmm0, xmm1
    cvtsd2si rax, xmm3
    PRINT_DEC 1, AL
    
    xor rax, rax
    ret