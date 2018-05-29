org 100h

section .data
    lastX: dw 0
    lastY: dw 0
    newX: dw 0
    newY: dw 0

    Cursor times 8 dd 5555aaaah
            times 8 dd 0aaaa5555h

;------------------------------------------------------------------------------
section .text
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
    ; Set display mode
    xor ah, ah
    mov al, 13 ; 640x200
    int 10h

    ; Check mouse
    xor ax, ax
    int 33h

    ; Set cursor shape
    mov ax, 9
    mov bx, 8 ; Horz Hot Spot
    mov cx, 8 ; Vert Hot Spot
    mov dx, Cursor
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
    call center_cursor
    jmp the_loop


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
    sub [newX], ax ; scale
    sub [newY], ax
    jmp hor_finish
hor_pos:
    add [newX], ax
    add [newX], ax ; scale
    sub [newY], ax
hor_finish:


    cmp bx, 0
    jg ver_pos
ver_neg:
    add [newX], bx
    add [newX], bx ; scale
    sub [newY], bx
    jmp ver_finish
ver_pos:
    sub [newX], bx
    sub [newX], bx ; scale
    sub [newY], bx
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
center_cursor:
    mov word [newX], 320
    mov word [newY], 100
    call set_cursor
    ret
