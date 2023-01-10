IFDEF X86
	.686p
	.model flat
	.code

	extrn _enter_func:PROC
	extrn _exit_func:PROC

	__penter proc
		pushad
		mov  eax, esp
		add  eax, 32
		mov  eax, dword ptr[eax]
		sub  eax, 5
		push eax
		call _enter_func
		pop eax
		popad
		ret
	__penter endp

	__pexit proc
		pushad
		mov  eax, esp
		add  eax, 28
		mov  eax, dword ptr[eax]
		sub  eax, 5
		push eax
		call _exit_func
		pop eax
		popad
		ret
	__pexit endp

ELSEIFDEF X64
	.code

	extrn enter_func:PROC
	extrn exit_func:PROC

	_penter proc
		push r11
		push r10
		push r9
		push r8
		push rax
		push rdx
		push rcx
		sub  rsp,20h 
		mov  rcx,rsp
		mov  rcx,qword ptr[rcx+58h]
		sub  rcx,5
		call enter_func
		add  rsp,20h 
		pop rcx
		pop rdx
		pop rax
		pop r8
		pop r9
		pop r10
		pop r11
		ret
	_penter endp

	_pexit proc
		push r11
		push r10
		push r9
		push r8
		push rax
		push rdx
		push rcx
		sub  rsp,20h 
		mov  rcx,rsp
		mov  rcx,qword ptr[rcx+58h]
		call exit_func
		add rsp,20h
		pop rcx
		pop rdx
		pop rax
		pop r8
		pop r9
		pop r10
		pop r11
		ret
	_pexit endp
ENDIF
end
