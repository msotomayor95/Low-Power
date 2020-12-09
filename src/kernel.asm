; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start

; INTERRUPCIONES
extern IDT_DESC
extern idt_init

; PIC
extern pic_reset
extern pic_enable

; GDT
extern GDT_DESC

; Administrador de memoria
extern mmu_init
extern mmu_init_task_dir
extern mmu_init_kernel_dir

; inicializadores 
extern tss_init
extern sched_init
extern screen_init

; funciones dummy
extern imprimir_libretas

; GDT Defines
%define GDT_IDX_DATA_0  10
%define GDT_IDX_CODE_0  12
%define GDT_IDX_VIDEO_0 14
%define GDT_IDX_TSS_INIT 15
%define GDT_IDX_TSS_IDLE 16

; Defines varios
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

offset: dd 0
selector: dw 0

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
    ; xor edi, edi 
    ; .filaNegra:
    ;     mov WORD [fs:edi], C_BG_BLACK
    ;     add edi, 2
    ;     cmp edi, 0xA0
    ;     jnz .filaNegra
    ; .mapaVerde:
    ;     mov WORD [fs:edi], C_BG_GREEN
    ;     add edi, 2
    ;     cmp edi, 0x19A0
    ;     jnz .mapaVerde
    ; .tableroNegro:
    ;     mov WORD [fs:edi], C_BG_BLACK
    ;     add edi, 2
    ;     cmp edi, 0x1F40
    ;     jnz .tableroNegro
    call screen_init
    
    ; Inicializar el manejador de memoria
    ;mov eax, 1
    ;push eax
    ;push eax
    call mmu_init
 
    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir

    ; .inicializadoDeUnaTarea:
    ; xchg bx, bx
    ; call init_morty               ; esto devuelve un cr3 de una tarea rick

    ; Cargar directorio de paginas
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ;call imprimir_libretas

    ; Inicializar tss de la tarea Idle e inicial
    .activaTSS:
    call tss_init

    ; Inicializar el scheduler
    call sched_init

    ; Inicializar la IDT
    call idt_init
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable

    ; Cargar tarea inicial
    mov ax, GDT_IDX_TSS_INIT<<3
    ltr ax

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle
    xchg bx, bx
    .antesDelJumPFar:
    mov ax, GDT_IDX_TSS_IDLE<<3
    mov [selector], ax
    jmp far [offset]

    ; Ciclar infinitamente (por si algo sale mal...)
    ; mov eax, 0xFFFF
    ; mov ebx, 0xFFFF
    ; mov ecx, 0xFFFF
    ; mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"