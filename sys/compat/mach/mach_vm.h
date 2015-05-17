/*	$FreeBSD$ */

/*-
 * Copyright (c) 2002-2003 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Emmanuel Dreyfus
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	_MACH_VM_H_
#define	_MACH_VM_H_


#include <sys/types.h>
#include <sys/param.h>

#include <sys/mach/mach_types.h>
#include <compat/mach/mach_message.h>


int copyin_proc(struct proc *p, const void *uaddr, void *kaddr, size_t len);

int copyout_proc(struct proc *p, const void *kaddr, void *uaddr, size_t len);
	
#define	MACH_ALTERNATE_LOAD_SITE	1
#define MACH_NEW_LOCAL_SHARED_REGIONS	2
#define MACH_QUERY_IS_SYSTEM_REGION	4
#define MACH_SF_PREV_LOADED		1
#define MACH_SYSTEM_REGION_BACKED	2

#define MACH_VM_PROT_COW	0x8
#define MACH_VM_PROT_ZF		0x10

/* mach_vm_behavior_t values */
#define MACH_VM_BEHAVIOR_DEFAULT 0
#define MACH_VM_BEHAVIOR_RANDOM 1
#define MACH_VM_BEHAVIOR_SEQUENTIAL 2
#define MACH_VM_BEHAVIOR_RSEQNTL 3
#define MACH_VM_BEHAVIOR_WILLNEED 4
#define MACH_VM_BEHAVIOR_DONTNEED 5

/* vm_map */
#define MACH_VM_INHERIT_SHARE 0
#define MACH_VM_INHERIT_COPY 1
#define MACH_VM_INHERIT_NONE 2
#define MACH_VM_INHERIT_DONATE_COPY 3

/* vm_allocate */
#define MACH_VM_FLAGS_ANYWHERE 1

/* vm_region */

#define MACH_VM_REGION_BASIC_INFO 10

/* vm_msync */
#define MACH_VM_SYNC_ASYNCHRONOUS 0x01
#define MACH_VM_SYNC_SYNCHRONOUS 0x02
#define MACH_VM_SYNC_INVALIDATE 0x04
#define MACH_VM_SYNC_KILLPAGES 0x08
#define MACH_VM_SYNC_DEACTIVATE 0x10

/* vm_machine_attribute */

#define MACH_MATTR_CACHE		1
#define MACH_MATTR_MIGRATE		2
#define MACH_MATTR_REPLICATE		4

#define MACH_MATTR_VAL_OFF		0
#define MACH_MATTR_VAL_ON		1
#define MACH_MATTR_VAL_GET		2
#define MACH_MATTR_VAL_CACHE_FLUSH	6
#define MACH_MATTR_VAL_DCACHE_FLUSH	7
#define MACH_MATTR_VAL_ICACHE_FLUSH	8
#define MACH_MATTR_VAL_CACHE_SYNC	9
#define MACH_MATTR_VAL_GET_INFO		10

#include <sys/mach_debug/page_info.h>


int mach_vm_wire_32(host_priv_t host_priv, vm_map_t task, vm_address_t address, vm_size_t size, vm_prot_t desired_access);

int mach_vm_map_page_query(vm_map_t target_map, vm_offset_t offset, integer_t *disposition, integer_t *ref_count);
int mach_vm_mapped_pages_info(vm_map_t task, page_address_array_t *pages, mach_msg_type_number_t *pagesCnt);

int mach_vm_map(vm_map_t map, mach_vm_address_t *address, mach_vm_size_t _size,
			mach_vm_offset_t _mask __unused, int _flags, mem_entry_name_port_t object __unused,
			memory_object_offset_t offset __unused, boolean_t copy __unused,
			vm_prot_t cur_protection, vm_prot_t max_protection, vm_inherit_t inh);


int mach_vm_deallocate(vm_map_t map,  vm_offset_t addr, size_t len);
#if 0
int mach_vm_protect(struct thread *td, vm_offset_t addr, size_t len, vm_prot_t prot);
#endif

/* These are machine dependent functions */

int cpu_mach_vm_machine_attribute(vm_map_t, vm_offset_t, size_t, int *);

#endif /* _MACH_VM_H_ */
