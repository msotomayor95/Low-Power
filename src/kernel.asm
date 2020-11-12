; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern GDT_DESC

%define GDT_IDX_DATA_0  10
%define GDT_IDX_CODE_0  12
%define GDT_IDX_VIDEO_0 14

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0


    ; Habilitar A20
    call A20_enable
    
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Saltar a modo protegido
    jmp (GDT_IDX_CODE_0<<3):modoprotegido

BITS 32
modoprotegido:
    ; Establecer selectores de segmentos
    mov ax, GDT_IDX_DATA_0
    shl ax, 3
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ax, GDT_IDX_VIDEO_0
    shl ax, 3
    mov fs, ax

    ; Establecer la base de la pila
    mov esp, 0x25000
    mov ebp, esp
    
    xchg bx, bx
    ; Imprimir mensaje de bienvenida

    ; Inicializar pantalla
    xor edi, edi 
    .filaNegra:
        mov [fs:edi], C_BG_BLACK
        inc edi
        cmp edi, 0x50
        jnz .filaNegra
    .mapaVerde:
        mov [fs:edi], C_BG_GREEN
        inc edi
        cmp edi, 0xCD0
        jnz .mapaVerde:
    .tableroNegro:
        mov [fs:edi], C_BG_BLACK
        inc edi
        cmp edi, 0xFA0
        jnz .tableroNegro
    xchg bx, bx
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    
    ; Cargar IDT
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"