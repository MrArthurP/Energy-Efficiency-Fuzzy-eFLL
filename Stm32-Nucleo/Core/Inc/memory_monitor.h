#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t MemoryMonitor_GetHeapUsage(void);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_MONITOR_H */
