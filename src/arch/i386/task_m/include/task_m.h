#ifndef _TASK_M_H
#define _TASK_M_H 1

#include "../../include/types.h"
#include "../../mem_m/include/paging.h"

typedef struct {
    /*  POPA */
    reg_t edi;
    reg_t esi;
    reg_t ebp;
    reg_t ebx;
    reg_t edx;
    reg_t ecx;
    reg_t eax;
    
    reg_t eip;
    reg_t cs;
    /* POPF */
    reg_t eflags;

    reg_t gs;
    reg_t fs;
    reg_t es;
    reg_t ds;

} task_frame_t ;

struct mmap{
    void * vm_end;
    void * vm_start;
    reg_t prot;
    reg_t vm_flags;
    reg_t reserved;
    struct mmap * vm_next;
};

struct mm_struct{
    /*  pgd points to the base of the level 1 table */  
    Page_Directory * pgd;
    /*  mmap points to a list of vm_area_structs */
    struct mmap * mmap;
};

typedef struct {
    task_frame_t * frame;
    pid_t pid;
    struct mm_struct * mm;

} task_t;


#endif
