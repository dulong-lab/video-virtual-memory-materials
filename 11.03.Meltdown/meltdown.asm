.CODE

OutOfOrderExecution PROC
	mov   r8,  qword ptr [r8]
	movzx rax, byte ptr [rcx]
	shl   rax, 12
	mov   al,  byte ptr [rdx + rax]
	ret
OutOfOrderExecution ENDP

END
