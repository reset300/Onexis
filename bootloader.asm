[BITS 16]
[ORG 0x7C00]

start:
    cli


    mov si, boot_msg
    call print_string


    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000
    mov ah, 0x02
    mov al, 32
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0
    int 0x13
    jc disk_error

    mov si, loaded_msg
    call print_string


    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax



    jmp 0x08:protected_mode_entry

disk_error:
    mov si, error_msg
    call print_string
    hlt
    jmp $

print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next_char
.done:
    ret

boot_msg db "Bootloader started...", 0
loaded_msg db "Kernel loaded.", 0
error_msg db "Disk error!", 0

[BITS 32]
protected_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000

    call 0x10000

hang:
    jmp hang

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
