org 100h

section .data
    mes_st: db '----------------------started--------------------', 13, 10, '$'
    mes_fin: db '---------------------finished--------------------', 13, 10, '$'
    mes_nl: db 13, 10, '$'
;------------------------------------------------------------------------------
section .text
start:
    mov ah, 9
    mov dx, mes_st
    int 21h
;------------------------------------------------------------------------------
    xor bl, bl ; for later

    ; prepare seconds
    mov al, 00h
    out 70h, al
    in al, 71h

    add al, 30h ; amount of seconds
    daa
    cmp al, 60h
    jl l_no_sub
l_sub:
    sub al, 60h
    mov bl, 01h
l_no_sub:
    mov dl, al ; save target seconds

    ; prepare minutes
    mov al, 02h
    out 70h, al
    in al, 71h

    add al, 01 ; amount of minutes
    add al, bl
    daa
    mov dh, al ; save target minutes

; dh == mins, dl == secs
    mov al, 02h
    out 70h, al
lp_min:
    in al, 71h
    cmp dh, al
    jne lp_min

    mov al, 00h
    out 70h, al
lp_sec:
    in al, 71h
    cmp dl, al
    jne lp_sec

;------------------------------------------------------------------------------
    mov ah, 9
    mov dx, mes_fin
    int 21h

    int 20h

;------------------------------------------------------------------------------
; print
; input: al
    ;mov bl, al
    ;and bl, 0fh
    ;add bl, 48
    ;mov bh, al
    ;and bh, 0f0h
    ;shr bh, 4
    ;add bh, 48

    ;mov ah, 02h
    ;mov dl, bh
    ;int 21h

    ;mov ah, 02h
    ;mov dl, bl
    ;int 21h
    ;; new line
    ;mov ah, 9
    ;mov dx, mes_nl
    ;int 21h
