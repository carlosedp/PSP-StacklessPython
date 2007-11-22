/*
 * this is the internal transfer function.
 *
 * HISTORY
 * 18-Jun-07 File creation. Ported from Arm Thumb. - Carlos Eduardo de Paula
 * 01-Oct-07 Added registers "ra", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"
 *           that prevented Stackless to shut down appropriately freezing PSP.
 *
 * NOTES
 *
 *
 * POSSIBLE ERRORS
 *
 *
 */

#define STACK_REFPLUS 1

#ifdef SLP_EVAL
#define STACK_MAGIC 0
#define REGS_TO_SAVE "fp", "sp", "ra", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"


static int
slp_switch(void)
{
	register int *stackref, stsizediff;
	__asm__ volatile ("" : : : REGS_TO_SAVE);
	__asm__ ("move %0,$sp" : "=g" (stackref));
	{
		SLP_SAVE_STATE(stackref, stsizediff);
		__asm__ volatile (
		    "add $sp,$sp,%0\n"
		    "add $fp,$fp,%0\n"
		    :
		    : "r" (stsizediff)
		    );
		SLP_RESTORE_STATE();
		return 0;
	}
	__asm__ volatile ("" : : : REGS_TO_SAVE);
}

#endif
