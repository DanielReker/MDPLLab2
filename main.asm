	; Program is x86 16 bit
	BITS 16

	org 0x100 ; DOS loads .com programs at 0x100 offset

	; examples from NASM docs
	db    0x55                ; just the byte 0x55 
	db    0x55,0x56,0x57      ; three bytes in succession 
testLabel1: ; some test labels
	db    'a',0x55            ; character constants are OK 
	db    'hello',13,10,'$'   ; so are string constants 
	dw    0x1234              ; 0x34 0x12 
_Label_2:
	dw    'a'                 ; 0x61 0x00 (it's just a number) 
	dw    'ab'                ; 0x61 0x62 (character constant) 
	dw    'abc'               ; 0x61 0x62 0x63 0x00 (string) 
	dd    0x12345678          ; 0x78 0x56 0x34 0x12 
		
		
		
	testLabel:
		jmp testLabel	
		
		jmp testLabel
		
	moreLabels: ; If ...
		jg  $+1929
		jl  $+0

	otherLabel1: ; Else ???
		jmp $-2928
	; end of logic

	label_name: ; exit program
		jl  $+4
		jmp $-123
		jg  $-1 ;end
		
		jmp moreLabels