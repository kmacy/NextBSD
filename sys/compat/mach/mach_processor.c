#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/malloc.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/proc.h>

#include <sys/mach/mach_types.h>
#include <sys/mach/processor.h>


processor_t		master_processor;

void
pset_add_task(processor_set_t	pset, task_t		task)
{
	;
}

void
pset_remove_task(processor_set_t	pset, task_t		task)
{

}

void
pset_deallocate(processor_set_t	pset)
{
	;
}

void
pset_reference(processor_set_t	pset)
{
	;
}

