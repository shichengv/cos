#include "../../../libk/include/stdint.h"
#include "../include/types.h"
#include "./include/intr_m.h"

/* 设置中断允许, 但并不影响异常和不可屏蔽中断 */
/* 不可屏蔽由INT产生的中断 */
void _intr_m_interrupt_enable(bool status){
    if (status)
        __asm__ volatile("sti\n"); // 响应可屏蔽硬件中断
    else
        __asm__ volatile("cli\n"); // 禁止响应可屏蔽硬件中断
}