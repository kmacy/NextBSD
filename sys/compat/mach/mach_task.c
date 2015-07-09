

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/param.h>
#include <sys/exec.h>
#include <sys/systm.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/mutex.h>
#include <sys/priv.h>
#include <sys/proc.h>
#include <sys/resourcevar.h>
#include <sys/sysproto.h>
#include <sys/uio.h>
#include <sys/ktrace.h>


#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_extern.h>

#include <sys/mach/mach_types.h>

#include <sys/mach/task_server.h>

int
task_get_mach_voucher(
	task_t task,
	mach_voucher_selector_t which,
	ipc_voucher_t *voucher
)
UNSUPPORTED;

int
task_get_state(
	task_t task,
	thread_state_flavor_t flavor,
	thread_state_t old_state,
	mach_msg_type_number_t *old_stateCnt
)
UNSUPPORTED;

int
task_policy_get(
	task_t task,
	task_policy_flavor_t flavor,
	task_policy_t policy_info,
	mach_msg_type_number_t *policy_infoCnt,
	boolean_t *get_default
)
UNSUPPORTED;

int
task_policy_set(
	task_t task,
	task_policy_flavor_t flavor,
	task_policy_t policy_info,
	mach_msg_type_number_t policy_infoCnt
)
UNSUPPORTED;

int
task_purgable_info(
	task_t task,
	task_purgable_info_t *stats
)
UNSUPPORTED;

int
task_resume2(
	task_suspension_token_t suspend_token
)
UNSUPPORTED;

int
task_set_mach_voucher(
	task_t task,
	ipc_voucher_t voucher
	)
UNSUPPORTED;

int
task_set_phys_footprint_limit(
	task_t task,
	int new_limit,
	int *old_limit
)
UNSUPPORTED;

int
task_set_state(
	task_t task,
	thread_state_flavor_t flavor,
	thread_state_t new_state,
	mach_msg_type_number_t new_stateCnt
)
UNSUPPORTED;

int
task_suspend2(
	task_t target_task,
	task_suspension_token_t *suspend_token
)
UNSUPPORTED;

int
task_swap_mach_voucher(
	task_t task,
	ipc_voucher_t new_voucher,
	ipc_voucher_t *old_voucher
)
UNSUPPORTED;

int
task_zone_info(
	task_t target_task,
	mach_zone_name_array_t *names,
	mach_msg_type_number_t *namesCnt,
	task_zone_info_array_t *info,
	mach_msg_type_number_t *infoCnt
)
UNSUPPORTED;

int
thread_create_from_user(
	task_t parent_task,
	thread_act_t *child_act
)
UNSUPPORTED;

int
thread_create_running_from_user(
	task_t parent_task,
	thread_state_flavor_t flavor,
	thread_state_t new_state,
	mach_msg_type_number_t new_stateCnt,
	thread_act_t *child_act
)
UNSUPPORTED;

