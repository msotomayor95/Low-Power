; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

extern IDT_DESC
extern idt_init

extern pic_reset
extern pic_enable

extern GDT_DESC
extern mmu_init_kernel_dir
extern imprimir_libretas

%define GDT_IDX_DATA_0  10
%define GDT_IDX_CODE_0  12
%define GDT_IDX_VIDEO_0 14
%define C_BG_BLACK 0
%define C_BG_GREEN 8192
%define KERNEL_PAGE_DIR 0x00025000

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
    
    ; Imprimir mensaje de bienvenida

    ; Inicializar pantalla
    xor edi, edi 
    .filaNegra:
        mov WORD [fs:edi], C_BG_BLACK
        add edi, 2
        cmp edi, 0xA0
        jnz .filaNegra
    .mapaVerde:
        mov WORD [fs:edi], C_BG_GREEN
        add edi, 2
        cmp edi, 0x19A0
        jnz .mapaVerde
    .tableroNegro:
        mov WORD [fs:edi], C_BG_BLACK
        add edi, 2
        cmp edi, 0x1F40
        jnz .tableroNegro
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir

    ; paddr_t phy_start, paddr_t code_start, size_t pages, vaddr_t v_start, uint8_t rw, uint8_t user_supervisor;

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    call imprimir_libretas
    xchg bx, bx

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT

    call idt_init
    
    ; Cargar IDT
 
    lidt [IDT_DESC]
    ; Configurar controlador de interrupciones

    call pic_reset
    call pic_enable

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"