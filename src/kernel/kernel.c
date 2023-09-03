#include "../libk/include/stdint.h"
#include "../libk/include/stdio.h"

void _show_system_logo(){
    printk("  ____                      _    ___  ____  \n");
    printk(" / ___|__ _ ___ _   _  __ _| |  / _ \\/ ___| \n");
    printk("| |   / _` / __| | | |/ _` | | | | | \\___ \\ \n");
    printk("| |__| (_| \\__ \\ |_| | (_| | | | |_| |___) |\n");
    printk(" \\____\\__,_|___/\\__,_|\\__,_|_|  \\___/|____/ \n");
    printk("                                            \n");
}

void _show_skadi(){
printk(" ____  _             _ _    ___  ____  \n");
printk("/ ___|| | ____ _  __| (_)  / _ \\/ ___| \n");
printk("\\___ \\| |/ / _` |/ _` | | | | | \\___ \\ \n");
printk(" ___) |   < (_| | (_| | | | |_| |___) |\n");
printk("|____/|_|\\_\\__,_|\\__,_|_|  \\___/|____/ \n");
printk("                                       \n");

}
void kernel_main(){
    _show_system_logo();
    // _show_skadi();
    while (1)
    {
        ;
    }
    
}