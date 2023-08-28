#ifndef _TYPES_H
#define _TYPES_H

#include "../../libk/include/stdint.h"

#define CONCAT(x, y) x##y
#define RESERVED_TOKEN(x, y) CONCAT(x, y)
#define RESERVED RESERVED_TOKEN(reserved, __LINE__)

#define bool _Bool
#define true 1
#define false 0

#define _packed __attribute__((packed))
#define _inline __attribute__((always_inline)) inline

typedef unsigned long size_t;
typedef long ssize_t;
typedef long ssize_t;


typedef uint32_t time_t;

typedef int32_t dev_t;

typedef uint16_t mode_t;

typedef int pid_t;

typedef int fd_t;

enum std_fd_t {
    stdin = 0,
    stdout,
    stderr,
};

#endif