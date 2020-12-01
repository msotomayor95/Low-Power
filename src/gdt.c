/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] =
        {
            .limit_15_0 = 0x0000,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_DATA_0] =
        {
            .limit_15_0 = 0xD1FF,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x2,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_DATA_3] =
        {
            .limit_15_0 = 0xD1FF,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x2,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_CODE_0] =
        {
            .limit_15_0 = 0xD1FF,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0xA,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_CODE_3] =
        {
            .limit_15_0 = 0xD1FF,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0xA,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_VIDEO_0] =
        {
            .limit_15_0 = 0x1F3F,
            .base_15_0 = 0x8000,
            .base_23_16 = 0x0B,
            .type = 0x2,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_TSS_INIT] = 
        {
            .limit_15_0 = SIZE_TSS-1,
            .base_15_0 = 0x0,
            .base_23_16 = 0x0,
            .type = 0x9,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_TSS_IDLE] = 
        {
            .limit_15_0 = SIZE_TSS-1,
            .base_15_0 = 0x0,
            .base_23_16 = 0x0,
            .type = 0x9,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
};

// #define GDT_TSS_INIT   15
        // #define GDT_TSS_IDLE      16

gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};
