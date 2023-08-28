#include "../include/stdint.h"
#include "../include/string.h"
#include "../../include/kernel/types.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
