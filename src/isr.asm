; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

idle_offset:     dd 0xFFFFFFFF
idle_selector:   dw 0xFFFF

extern print_hex

;; PIC
extern pic_finish1

;; Sched
extern meeseek_llamo_crear_meeseek
extern matar_meeseek
extern valores_validos
extern puedo_crear_meeseek
extern crear_meeseek
extern sentenciar_ganador
extern sched_next_task

%define GDT_IDX_TSS_IDLE 16
%define GDT_IDX_TSS_RICK 17
%define GDT_IDX_TSS_MORTY 18

;;
global _isrClock
global _isrKey

global _isr88
global _isr89
global _isr100
global _isr123

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
    pushad
    mov eax, %1
    str dx
    cmp dx, GDT_IDX_TSS_RICK <<3
    je .ganoMorty
    cmp dx, GDT_IDX_TSS_MORTY <<3
    je .ganoRick
    call sched_next_task
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]
    .ganoMorty:
        jmp $
    .ganoRick:
        jmp $    
    popad

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

_isrClock:
    pushad
    call pic_finish1
    call next_clock
    call sched_next_task
    str dx
    cmp ax, dx
    je .fin
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]
    .fin:
        popad
        iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

_isrKey:
    pushad
    call pic_finish1
    xor eax, eax
    in al, 0x60
    cmp eax, 0x80
    jge .solte 
    push 0x0f       ; text blanco en fondo negro
    push 0
    push 79
    push 1
    push eax
    call print_hex
    add esp, 20
    .solte:
    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

resultado_temporal: dd 0x0

_isr88:
    ; eax = code_start
    ; ebx = x
    ; ecx = y

    pushad
    push ecx
    push ebx
    push eax
    
    mov [resultado_temporal], eax
    call meeseek_llamo_crear_meeseek
    xchg bx, bx
    cmp al, 0
    je .checkeoValores
    
    ; Si llegue aca es porque un Mr. M llamo a esta syscall, por lo que muere.
    call matar_meeseek
    jmp .saltar_idle

    .checkeoValores:
    call valores_validos
    cmp al, 1
    je .checkeoSiHayEspacioParaUnNuevoMrM
    
    ; Rick o Morty llamaron a la syscall con datos invalidos.
    xor eax, eax
    push eax
    call sentenciar_ganador
    jmp $
    
    .checkeoSiHayEspacioParaUnNuevoMrM:
    call puedo_crear_meeseek
    cmp al, 0
    je .todosLosMrMActivos
    
    ; Existe espacio para crear un Mr. M
    call crear_meeseek
    mov [resultado_temporal], eax
    jmp .saltar_idle

    .todosLosMrMActivos:
    mov DWORD [resultado_temporal], 0

    .saltar_idle:
    mov dx, GDT_IDX_TSS_IDLE<<3
    mov [idle_selector], dx
    jmp far [idle_offset]

    pop eax
    pop ebx
    pop ecx
    popad
    mov eax, [resultado_temporal]
iret

_isr89:
    mov eax, 0x59
    mov dx, GDT_IDX_TSS_IDLE<<3
    mov [idle_selector], dx
    jmp far [idle_offset]
    iret

_isr100:
    mov eax, 0x64
    mov dx, GDT_IDX_TSS_IDLE<<3
    mov [idle_selector], dx
    jmp far [idle_offset]
    iret

_isr123:
    mov eax, 0x7b
    mov dx, GDT_IDX_TSS_IDLE<<3
    mov [idle_selector], dx
    jmp far [idle_offset]
    iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret