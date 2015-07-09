
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/param.h>

#include <sys/mach/mach_types.h>
#include <sys/mach/ipc_host.h>
#include <sys/mach/ipc_sync.h>
#include <sys/mach/ipc_tt.h>
#include <sys/mach/ipc/ipc_voucher.h>
#include <sys/mach/task.h>

ipc_port_t
convert_clock_ctrl_to_port(mach_clock_t clock)
{
	return (NULL);
}

ipc_port_t
convert_clock_to_port(mach_clock_t clock)
{
	return (NULL);
}

vm_map_t
convert_port_entry_to_map(ipc_port_t port)
{
	return (NULL);
}


mach_clock_t
convert_port_to_clock(ipc_port_t port)
{
	return (NULL);
}

semaphore_t
convert_port_to_semaphore(ipc_port_t port)
{

	return (NULL);
}

task_name_t
convert_port_to_task_name(ipc_port_t port)
{

	return (NULL);
}

task_suspension_token_t
convert_port_to_task_suspension_token(ipc_port_t port)
{

	return (NULL);
}

ipc_voucher_t
convert_port_to_voucher(ipc_port_t port)
{

	return (NULL);
}

ipc_port_t
convert_semaphore_to_port(semaphore_t sema)
{

	return (NULL);
}

ipc_port_t
convert_task_suspension_token_to_port(task_suspension_token_t token)
{

	return (NULL);
}

ipc_port_t
convert_voucher_attr_control_to_port(ipc_voucher_attr_control_t control)
{

	return (NULL);
}

ipc_port_t
convert_voucher_to_port(ipc_voucher_t voucher)
{

	return (NULL);
}
