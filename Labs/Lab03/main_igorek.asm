org 100h

section .data
    lastX: dw 0
    lastY: dw 0
    newX: dw 0
    newY: dw 0

    currCursor db 1

    Cursor1:   dw  1111111010111111b  ;Background
               dw  1111111010111111b
               dw  1110011010110011b
               dw  1110101010101011b
               dw  1111010010010111b
               dw  1111101010101111b
               dw  1100000111000011b
               dw  1011111111111101b
               dw  1100000111000011b
               dw  1111101010101111b
               dw  1111010010010111b
               dw  1110101010101011b
               dw  1110011010110011b
               dw  1111111010111111b
               dw  1111111010111111b
               dw  1111111101111111b

               dw  0000000101000000b  ;start cursor
               dw  0000000101000000b
               dw  0001100101001100b
               dw  0001010101010100b
               dw  0000101101101000b
               dw  0000010101010000b
               dw  0011111000111100b
               dw  0100000000000010b
               dw  0011111000111100b
               dw  0000010101010000b
               dw  0000101101101000b
               dw  0001010101010100b
               dw  0001100101001100b
               dw  0000000101000000b
               dw  0000000101000000b
               dw  0000000010000000b

    Cursor2 times 8 dd 5555aaaah
            times 8 dd 0aaaa5555h

;------------------------------------------------------------------------------
section .text
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
    ; Set display mode
    xor ah, ah
    mov al, 13 ; 320x200
    int 10h

    ; Check mouse
    xor ax, ax
    int 33h

    ; Set cursor shape
    mov ax, 9
    mov bx, 8 ; Horz Hot Spot
    mov cx, 8 ; Vert Hot Spot
    mov dx, Cursor2 ; es:dx points to mask
    int 33h

    ; Show cursor
    mov ax, 1
    int 33h

    ; Get cursor position
    mov ax, 3
    int 33h

    mov [lastX], cx
    mov [newX], cx
    mov [lastY], dx
    mov [newY], dx

the_loop:
    ; Get cursor position
    mov ax, 3
    int 33h

    test bx, 1
    jnz the_end

    test bx, 2
    jz the_skip
    ; Loop until key is released
still_loop:
    mov ax, 3
    int 33h
    test bx, 2
    jnz still_loop

    call switch_cursor


    ; Alter directions here
the_skip:
    ; new := last
    mov ax, [lastX]
    mov [newX], ax
    mov ax, [lastY]
    mov [newY], ax

    ; CX - horizontal
    ; DX - vertical
    mov ax, cx
    sub ax, [lastX]
    mov bx, dx
    sub bx, [lastY]
    ; AX - delta horizontal
    ; BX - delta vertical


    cmp ax, 0
    jg hor_pos
hor_neg:
    sub [newX], ax
    push bx
    xor bx, bx
    sub bx, ax
    shr bx, 1
    xor ax, ax
    sub ax, bx
    pop bx
    add [newY], ax ; axis inverted
    jmp hor_finish
hor_pos:
    sub [newX], ax ; opposite direction
hor_finish:


    cmp bx, 0
    jg ver_pos
ver_neg:
    sub [newX], bx ; inverted axis
    jmp ver_finish
ver_pos:
    add [newY], bx ; inverted axis
ver_finish:
    ; Check bounds
    mov ax, [newY]
    cmp ax, 10
    jg next3
    add ax, 200-20
next3:
    cmp ax, 200-10
    jl next4
    sub ax, 200-20
next4:
    mov [newY], ax
    ; Check bounds
    mov ax, [newX]
    cmp ax, 10
    jg next1
    add ax, 640-20
next1:
    cmp ax, 640-10
    jl next2
    sub ax, 640-20
next2:
    mov [newX], ax



finish:
    call set_cursor

    jmp the_loop
;------------------------------------------------------------------------------
the_end:
    int 20h
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
set_cursor:
    mov ax, 4
    mov cx, [newX]
    mov [lastX], cx
    mov dx, [newY]
    mov [lastY], dx
    int 33h
    ret
;------------------------------------------------------------------------------
switch_cursor:
    mov ax, 9
    mov bx, 8 ; Horz Hot Spot
    mov cx, 8 ; Vert Hot Spot

    cmp byte [currCursor], 1
    jne switch_cursor_l1

    mov dx, Cursor1
    mov byte [currCursor], 2
    jmp switch_cursor_l2

switch_cursor_l1:
    mov dx, Cursor2
    mov byte [currCursor], 1

switch_cursor_l2:
    int 33h

    call set_cursor

    ret
