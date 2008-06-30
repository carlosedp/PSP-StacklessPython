# NOTE: This is not yet safe to use.  Checked in for the same of reference.
#
# (int) slp_switch (void);

       .text
       .type   slp_switch, @function
       # This next line is required to the C code can find and
       # link against this function.
       .global slp_switch
slp_switch:
       pushq   %rbp
       pushq   %r15
       pushq   %r14
       pushq   %r13
       pushq   %r12
       pushq   %rbx

       # Disabled for now, which should ideally give us identical
       # behaviour to the inline C version.  Can add this when we
       # are ready for it.
       #subq    $8, %rsp
       #stmxcsr (%rsp)

       movq    %rsp, %rdi

       call slp_save_state             # diff = slp_save_state([?]stackref)

       cmp     $-1, %rax               # if (diff == -1)
       je      .exit                   #     return -1;

       cmp     $1, %rax                # if (diff ==  1)
       je      .no_restore             #     return 0;

.restore:
       add    %rax, %rsp               # Adjust the stack pointer for the state we are restoring.

       call slp_restore_state          # slp_restore_state()

.no_restore:
       xor     %rax, %rax              # Switch successful (whether we restored or not).

.exit:
       #ldmxcsr (%rsp)
       #addq    $8, %rsp

       popq    %rbx
       popq    %r12
       popq    %r13
       popq    %r14
       popq    %r15
       # rbp gets popped by the leave statement

       leave
       ret
.LFE11:
       .size   slp_switch, .-slp_switch
