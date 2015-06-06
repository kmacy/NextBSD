/*
 * Copyright 1991-1998 by Open Software Foundation, Inc. 
 *              All Rights Reserved 
 *  
 * Permission to use, copy, modify, and distribute this software and 
 * its documentation for any purpose and without fee is hereby granted, 
 * provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation. 
 *  
 * OSF DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE. 
 *  
 * IN NO EVENT SHALL OSF BE LIABLE FOR ANY SPECIAL, INDIRECT, OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT, 
 * NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION 
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
/*
 * MkLinux
 */
#ifndef _MACH_I386_RPC_H_
#define _MACH_I386_RPC_H_

/*
 * Definition of RPC "glue code" operations vector -- entry
 * points needed to accomplish short-circuiting
 */
typedef struct rpc_glue_vector {
	kern_return_t	(*rpc_simple)(int, int, void *);
	boolean_t	(*copyin)(mach_port_t, void *);
	boolean_t	(*copyout)(mach_port_t, void *);
	boolean_t	(*copyinstr)(mach_port_t, void *);
	kern_return_t	(*thread_switch)(void *);
	kern_return_t	(*thread_depress_abort)(mach_port_t);
} *rpc_glue_vector_t;

/*
 * Macros used to dereference glue code ops vector -- note
 * hard-wired references to global defined below.  Also note
 * that most of the macros assume their caller has stacked
 * the target args somewhere, so that they can pass just the
 * address of the first arg to the short-circuited
 * implementation.
 */
#define CAN_SHCIRCUIT(name)	(_rpc_glue_vector->name != 0)
#define RPC_SIMPLE(port, rtn_num, argc, argv) \
	((*(_rpc_glue_vector->rpc_simple))(rtn_num, argc, (void *)(&(port))))
#define COPYIN(map, from, to, count) \
	((*(_rpc_glue_vector->copyin))(map, (void *)(&(from))))
#define COPYOUT(map, from, to, count) \
	((*(_rpc_glue_vector->copyout))(map, (void *)(&(from))))
#define COPYINSTR(map, from, to, max, actual) \
	((*(_rpc_glue_vector->copyinstr))(map, (void *)(&(from))))
#define THREAD_SWITCH(thread, option, option_time) \
	((*(_rpc_glue_vector->thread_switch))((void *)(&(thread))))
#define	THREAD_DEPRESS_ABORT(act)	\
	((*(_rpc_glue_vector->thread_depress_abort))(act))

/*
 * User machine dependent macros for mach rpc
 */
#define MACH_RPC(sig_ptr, sig_size, id, dest, arg_list)			   \
	mach_rpc_trap(dest, id, (mach_rpc_signature_t) sig_ptr, sig_size)

/*
 * Kernel machine dependent macros for mach rpc
 *
 * User args (argv) begin two words above the frame pointer (past saved ebp 
 * and return address) on the user stack. Return code is stored in register
 * ecx, by convention (must be a caller-saves register, to survive return
 * from server work function). The user space instruction pointer is eip,
 * and the user stack pointer is uesp.
 */
#define MACH_RPC_ARGV(act)	( (char *)(USER_REGS(act)->ebp + 8) )
#define MACH_RPC_RET(act)	( USER_REGS(act)->ecx )
#define MACH_RPC_UIP(act)	( USER_REGS(act)->eip )
#define MACH_RPC_USP(act)	( USER_REGS(act)->uesp )


extern boolean_t	klcopyin(
				mach_port_t,
				void *);

extern boolean_t	klcopyout(
				mach_port_t,
				void *);

extern boolean_t	klcopyinstr(
				mach_port_t,
				void *);

extern kern_return_t	klthread_switch(
				void *);

#endif	/* _MACH_I386_RPC_H_ */
