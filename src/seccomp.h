/*
 * bdf2sfd 1.1.2
 * Copyright (c) 2019-2020, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-06-09
 *
 * bdf2sfd is released under the BSD 2-Clause license
 * See LICENSE file for details
 */

#ifndef SECCOMP_H
#define SECCOMP_H

#include <stddef.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>

#define BDF2SFD_SYSCALL_ALLOW(syscall) \
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##syscall, 0, 1), \
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

static struct sock_filter filter[] = {
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	BDF2SFD_SYSCALL_ALLOW(brk),
	BDF2SFD_SYSCALL_ALLOW(close),
	BDF2SFD_SYSCALL_ALLOW(exit_group),
	BDF2SFD_SYSCALL_ALLOW(fstat),
	BDF2SFD_SYSCALL_ALLOW(ioctl),
#if defined(SYS_open)
	BDF2SFD_SYSCALL_ALLOW(open),
#endif
	BDF2SFD_SYSCALL_ALLOW(openat),
	BDF2SFD_SYSCALL_ALLOW(read),
	BDF2SFD_SYSCALL_ALLOW(write),
	BDF2SFD_SYSCALL_ALLOW(writev),

	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL)
};

struct sock_fprog bdf2sfd = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};

#endif /* SECCOMP_H */
