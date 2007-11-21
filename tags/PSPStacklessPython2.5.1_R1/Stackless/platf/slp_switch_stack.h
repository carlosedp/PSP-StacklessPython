/* this file is generated by mkswitch_stack.py, don't edit */

#if   defined(MS_WIN32) && !defined(MS_WIN64) && defined(_M_IX86)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm mov stackref, esp;
	/* modify EBX, ESI and EDI in order to get them preserved */
	__asm mov ebx, ebx;
	__asm xchg esi, edi;
	{
	    SLP_STACK_BEGIN(stackref, stsizediff);
	    __asm {
		mov     eax, stsizediff
		add     esp, eax
		add     ebp, eax
	    }
	    SLP_STACK_END();
	    return 0;
	}
}

#elif defined(MS_WIN64) && defined(_M_X64)


#elif defined(__GNUC__) && defined(__i386__)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : "ebx", "esi", "edi");
	__asm__ ("movl %%esp, %0" : "=g" (stackref));
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "addl %0, %%esp\n"
		    "addl %0, %%ebp\n"
		    :
		    : "r" (stsizediff)
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : "ebx", "esi", "edi");
}

#elif defined(__GNUC__) && defined(__amd64__)

static int
slp_switch_stack(void)
{
    register long *stackref, stsizediff;
    __asm__ volatile ("" : : : REGS_TO_SAVE);
    __asm__ ("movq %%rsp, %0" : "=g" (stackref));
    {
        SLP_STACK_BEGIN(stackref, stsizediff);
        __asm__ volatile (
            "addq %0, %%rsp\n"
            "addq %0, %%rbp\n"
            :
            : "r" (stsizediff)
            );
        SLP_STACK_END();
        return 0;
    }
    __asm__ volatile ("" : : : REGS_TO_SAVE);
}

#elif defined(__GNUC__) && defined(__PPC__) && defined(__linux__)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("mr %0, 1" : "=g" (stackref) : );
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "mr 11, %0\n"
		    "add 1, 1, 11\n"
		    "add 30, 30, 11\n"
		    : /* no outputs */
		    : "g" (stsizediff)
		    : "11"
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__)

static int
slp_switch_stack(void)
{
	static int x = 0;
	register intptr_t *stackref;
	register int stsizediff;
	__asm__ volatile (
	    "; asm block 1\n"
	    : /* no outputs */
	    : "r" (x)
	    : REGS_TO_SAVE
	);
	__asm__ ("; asm block 2\n\tmr %0, r1" : "=g" (stackref) : );
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "; asm block 3\n"
		    "\tmr r11, %0\n"
		    "\tadd r1, r1, r11\n"
		    "\tadd r30, r30, r11\n"
		    : /* no outputs */
		    : "g" (stsizediff)
		    : "r11"
		);
		SLP_STACK_END();
		return 0;
	}
}

#elif defined(__GNUC__) && defined(sparc) && defined(sun)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;

	/* Put the stack pointer into stackref */

	/* Sparc special: at first, flush register windows
	 */
	__asm__ volatile (
	    "ta %1\n\t"
	    "mov %%sp, %0"
	    : "=r" (stackref) :  "i" (ST_FLUSH_WINDOWS));

	{   /* You shalt put SLP_STACK_BEGIN into a local block */

		SLP_STACK_BEGIN(stackref, stsizediff);

		/* Increment stack and frame pointer by stsizediff */

		/* Sparc special: at first load new return address.
		   This cannot be done later, because the stack
		   might be overwritten again just after SLP_STACK_END
		   has finished. BTW: All other registers (l0-l7 and i0-i5)
		   might be clobbered too. 
		 */
		__asm__ volatile (
		    "ld [%0+60], %%i7\n\t"
		    "add %1, %%sp, %%sp\n\t"
		    "add %1, %%fp, %%fp"
		    : : "r" (_cst->stack), "r" (stsizediff)
		    : "%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7",
		      "%i0", "%i1", "%i2", "%i3", "%i4", "%i5");

		SLP_STACK_END();

		/* Run far away as fast as possible, don't look back at the sins.
		 * The LORD rained down burning sulfur on Sodom and Gomorra ...
		 */

		/* Sparc special: Must make it *very* clear to the CPU that
		   it shouldn't look back into the register windows
		 */
		__asm__ volatile ( "ta %0" : : "i" (ST_CLEAN_WINDOWS));
		return 0;
	} 
}

#elif defined(__GNUC__) && defined(__s390__) && defined(__linux__)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("lr %0, 15" : "=g" (stackref) : );
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "ar 15, %0"
		    : /* no outputs */
		    : "g" (stsizediff)
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#elif defined(__GNUC__) && defined(__s390x__) && defined(__linux__)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("lr %0, 15" : "=g" (stackref) : );
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "ar 15, %0"
		    : /* no outputs */
		    : "g" (stsizediff)
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)


#elif defined(__GNUC__) && defined(__arm32__)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("mov %0,sp" : "=g" (stackref));
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "add sp,sp,%0\n"
		    "add fp,fp,%0\n"
		    :
		    : "r" (stsizediff)
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#elif defined(PSP)

static int
slp_switch_stack(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("move %0,$sp" : "=g" (stackref));
	{
		SLP_STACK_BEGIN(stackref, stsizediff);
		__asm__ volatile (
		    "add $sp,$sp,%0\n"
		    "add $fp,$fp,%0\n"
		    :
		    : "r" (stsizediff)
		    );
		SLP_STACK_END();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#endif

