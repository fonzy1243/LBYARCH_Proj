section .data

constant dd 255.0

section .text
bits 64
default rel
global imgCvtGrayFloatToInt

imgCvtGrayFloatToInt:

	xor rsi, rsi

l1: 
	
	movss xmm0, [rdx + rsi * 4]
	movss xmm1, [constant]
	vmulss xmm2, xmm0, xmm1
	cvtss2si rbx, xmm2

	mov byte [r8 + rsi], bl

	inc rsi

	loop l1

ret

