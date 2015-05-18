#include <sys/param.h>
#include <sys/event.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/syscall.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>
#include <sys/types.h>
#include <sys/systm.h>

extern struct filterops machport_filtops;

static struct syscall_helper_data osx_syscalls[] = {
	SYSCALL_INIT_HELPER(__proc_info),
	SYSCALL_INIT_HELPER(__iopolicysys),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_vm_allocate_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_vm_deallocate_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_vm_protect_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_vm_map_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_allocate_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_destroy_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_deallocate_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_mod_refs_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_move_member_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_insert_right_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_insert_member_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_extract_member_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_construct_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_destruct_trap),
	SYSCALL_INIT_HELPER(mach_reply_port),
	SYSCALL_INIT_HELPER(thread_self_trap),
	SYSCALL_INIT_HELPER(task_self_trap),
	SYSCALL_INIT_HELPER(host_self_trap),
	SYSCALL_INIT_HELPER(mach_msg_trap),
	SYSCALL_INIT_HELPER(mach_msg_overwrite_trap),
	SYSCALL_INIT_HELPER(semaphore_signal_trap),
	SYSCALL_INIT_HELPER(semaphore_signal_all_trap),
	SYSCALL_INIT_HELPER(semaphore_signal_thread_trap),
	SYSCALL_INIT_HELPER(semaphore_wait_trap),
	SYSCALL_INIT_HELPER(semaphore_wait_signal_trap),
	SYSCALL_INIT_HELPER(semaphore_timedwait_signal_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_guard_trap),
	SYSCALL_INIT_HELPER(_kernelrpc_mach_port_unguard_trap),
	SYSCALL_INIT_HELPER(task_for_pid),
	SYSCALL_INIT_HELPER(pid_for_task),
	SYSCALL_INIT_HELPER(macx_swapon),
	SYSCALL_INIT_HELPER(macx_swapoff),
	SYSCALL_INIT_HELPER(macx_triggers),
	SYSCALL_INIT_HELPER(macx_backing_store_suspend),
	SYSCALL_INIT_HELPER(macx_backing_store_recovery),
	SYSCALL_INIT_HELPER(swtch_pri),
	SYSCALL_INIT_HELPER(swtch),
	SYSCALL_INIT_HELPER(thread_switch),
	SYSCALL_INIT_HELPER(clock_sleep_trap),
	SYSCALL_INIT_HELPER(mach_timebase_info),
	SYSCALL_INIT_HELPER(mach_wait_until),
	SYSCALL_INIT_HELPER(mk_timer_create),
	SYSCALL_INIT_HELPER(mk_timer_destroy),
	SYSCALL_INIT_HELPER(mk_timer_arm),
	SYSCALL_INIT_HELPER(mk_timer_cancel),
	SYSCALL_INIT_LAST
};

static int
mach_mod_init(void)
{
	int err;

	if (!cold) {
		printf("mach services can only be loaded at boot time\n");
		return (EINVAL);
	}

	if ((err = syscall_helper_register(osx_syscalls, SY_THR_STATIC_KLD))) {
		printf("failed to register osx calls: %d\n", err);
		return (EINVAL);
	}
	if (kqueue_add_filteropts(EVFILT_MACHPORT, &machport_filtops)) {
		printf("failed to register machport_filtops\n");
		return (EINVAL);
	}
	return (0);
}

static int
mach_module_event_handler(module_t mod, int what, void *arg)
{
	int err;

	switch (what) {
	case MOD_LOAD:
		if ((err = mach_mod_init()) != 0) {
			printf("mach services failed to load - mach system calls will not be available\n");
			return (err);
		}
		break;
	case MOD_UNLOAD:
		return (EBUSY);
	default:
		return (EOPNOTSUPP);
	}
	printf("mach services loaded - mach system calls available\n");
	return (0);
}

static moduledata_t mach_moduledata = {
	"mach",
	mach_module_event_handler,
	NULL
};

DECLARE_MODULE(mach, mach_moduledata, SI_SUB_KLD, SI_ORDER_ANY);



