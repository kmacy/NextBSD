#include <sys/types.h>
#include <machine/atomic.h>
typedef int OSSpinLock;

#define OSAtomicIncrement32Barrier(v) atomic_fetchadd_int((volatile int *)v, 1)
#define OSAtomicDecrement32Barrier(v) atomic_fetchadd_int((volatile int *)v, -1)
#define OSAtomicTestAndSetBarrier(i, v) atomic_testandset_int((volatile int *)v, i)
#define OSAtomicAdd64(i, v) atomic_add_64((volatile int *)v, i)
#define OSAtomicIncrement32(v) atomic_fetchadd_32((volatile int *)v, 1)
#define OSAtomicDecrement32(v) atomic_fetchadd_32((volatile int *)v, -1)
#define OSAtomicCompareAndSwapLongBarrier(o, i, v) atomic_cmpset_long(v, o, i)
#define OSAtomicCompareAndSwap32Barrier(o, i, v) atomic_cmpset_32(v, o, i)
#define OSSpinLockLock(l) while (atomic_testandset_int((volatile int *)l, 1) == 1)
#define OSSpinLockUnlock(l) atomic_clear_int((volatile int *)l, 1)
