# rsp - stack pointer
# rbp - base pointer, stack pointer when execution began

#
# positive offsets from rsp : stack-passed args
# negative offsets from rsp : local variables
#

.intel_syntax noprefix

# prologue
push rbp										# push the existing value of rbp onto the stack to save for caller
mov  rbp, rsp 							# copy the value of rsp to rbp, thus setting up the local base pointer

# body
#mov	DWORD PTR [rbp-4], edi
#mov	DWORD PTR [rbp-8], esi
#mov	eax, DWORD PTR [rbp-8]
#mov	edx, DWORD PTR [rbp-4]
#add	eax, edx

#mov eax, DWORD PTR [rbp-8]	# move parameter 1 into eax
#add eax, DWORD PTR [rbp-4]	# place the return value in eax

#mov eax, edi
#add eax, esi
lea eax, [edi + esi]
add eax, 0x3e								# 8-bit  add
add eax, 0x0102							# 16-bit add
add eax, 0x01020304					# 32-bit add
mov rax, 0x0102030405060708
mov rcx, 0x0102030405060708
mov rdx, 0x0102030405060708
mov rbx, 0x0102030405060708
mov rsi, 0x0102030405060708
mov rdi, 0x0102030405060708
mov rbp, 0x0102030405060708
mov rsp, 0x0102030405060708
mov r8, 0x0102030405060708
mov r9, 0x0102030405060708
mov r10, 0x0102030405060708
mov r11, 0x0102030405060708
mov r12, 0x0102030405060708
mov r13, 0x0102030405060708
mov r14, 0x0102030405060708
mov r15, 0x0102030405060708
add rax, rbx
#add eax, [edi]
#add eax, edi
#add eax, esi

# epilogue
pop rbp											# restore base pointer
ret
