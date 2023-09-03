#include "./include/paging.h"
#include "./include/mem_m.h"

/*  Construct Page-Directory Table  */
void _mem_m_construct_pdt(Page_Directory * pdt, bool write, bool user, ptr_t pt_referneced){
    pdt->present = 1;
    pdt->wirte = write;
    pdt->user = user;
    pdt->pt_referenced = pt_referneced;
}
/*  Construct Page Table  */
void _mem_m_construct_pt(Page_Table * pt, bool write, bool user, ptr_t pt_referneced){
    pt->present = 1;
    pt->wirte = write;
    pt->user = user;
    pt->pt_referenced = pt_referneced;
}
