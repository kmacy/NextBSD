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
/* CMU_HIST */
/*
 * Revision 2.1.1.1.2.1  92/03/03  16:21:23  jeffreyh
 * 	Merged up to Trunk
 * 	[92/02/26            jeffreyh]
 * 
 * Revision 2.4  92/02/26  13:10:29  elf
 * 	Added stupid alaises to make i386/fpu.c compile. RVB will fix.
 * 	 
 * 	[92/02/26            elf]
 * 
 * Revision 2.3  92/02/26  12:47:46  elf
 * 	Installed from i386 directory.
 * 	[92/02/26            danner]
 * 
 * 
 * Revision 2.2  92/01/03  20:19:47  dbg
 * 	Move this file to mach/i386.  Add FP_NO..FP_387 codes for
 * 	floating-point processor status.  Error bits in control
 * 	register are masks, not enables.
 * 	[91/10/19            dbg]
 * 
 */
/* CMU_ENDHIST */
/* 
 * Mach Operating System
 * Copyright (c) 1992-1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */

#ifndef	_I386_FP_SAVE_H_
#define	_I386_FP_SAVE_H_
/*
 *	Floating point registers and status, as saved
 *	and restored by FP save/restore instructions.
 */
struct i386_fp_save	{
	unsigned short	fp_control;	/* control */
	unsigned short	fp_unused_1;
	unsigned short	fp_status;	/* status */
	unsigned short	fp_unused_2;
	unsigned short	fp_tag;		/* register tags */
	unsigned short	fp_unused_3;
	unsigned int	fp_eip;		/* eip at failed instruction */
	unsigned short	fp_cs;		/* cs at failed instruction */
	unsigned short	fp_opcode;	/* opcode of failed instruction */
	unsigned int	fp_dp;		/* data address */
	unsigned short	fp_ds;		/* data segment */
	unsigned short	fp_unused_4;
};

struct i386_fp_regs {
	unsigned short	fp_reg_word[5][8];
					/* space for 8 80-bit FP registers */
};

/*
 * Control register
 */
#define	FPC_IE		0x0001		/* enable invalid operation
					   exception */
#define FPC_IM		FPC_IE
#define	FPC_DE		0x0002		/* enable denormalized operation
					   exception */
#define FPC_DM		FPC_DE
#define	FPC_ZE		0x0004		/* enable zero-divide exception */
#define FPC_ZM		FPC_ZE
#define	FPC_OE		0x0008		/* enable overflow exception */
#define FPC_OM		FPC_OE
#define	FPC_UE		0x0010		/* enable underflow exception */
#define	FPC_PE		0x0020		/* enable precision exception */
#define	FPC_PC		0x0300		/* precision control: */
#define	FPC_PC_24	0x0000			/* 24 bits */
#define	FPC_PC_53	0x0200			/* 53 bits */
#define	FPC_PC_64	0x0300			/* 64 bits */
#define	FPC_RC		0x0c00		/* rounding control: */
#define	FPC_RC_RN	0x0000			/* round to nearest or even */
#define	FPC_RC_RD	0x0400			/* round down */
#define	FPC_RC_RU	0x0800			/* round up */
#define	FPC_RC_CHOP	0x0c00			/* chop */
#define	FPC_IC		0x1000		/* infinity control (obsolete) */
#define	FPC_IC_PROJ	0x0000			/* projective infinity */
#define	FPC_IC_AFF	0x1000			/* affine infinity (std) */

/*
 * Status register
 */
#define	FPS_IE		0x0001		/* invalid operation */
#define	FPS_DE		0x0002		/* denormalized operand */
#define	FPS_ZE		0x0004		/* divide by zero */
#define	FPS_OE		0x0008		/* overflow */
#define	FPS_UE		0x0010		/* underflow */
#define	FPS_PE		0x0020		/* precision */
#define	FPS_SF		0x0040		/* stack flag */
#define	FPS_ES		0x0080		/* error summary */
#define	FPS_C0		0x0100		/* condition code bit 0 */
#define	FPS_C1		0x0200		/* condition code bit 1 */
#define	FPS_C2		0x0400		/* condition code bit 2 */
#define	FPS_TOS		0x3800		/* top-of-stack pointer */
#define	FPS_TOS_SHIFT	11
#define	FPS_C3		0x4000		/* condition code bit 3 */
#define	FPS_BUSY	0x8000		/* FPU busy */

/*
 * Kind of floating-point support provided by kernel.
 */
#define	FP_NO		0		/* no floating point */
#define	FP_SOFT		1		/* software FP emulator */
#define	FP_287		2		/* 80287 */
#define	FP_387		3		/* 80387 or 80486 */

#endif	/* _I386_FP_SAVE_H_ */
