/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef LINUX_IO_URING_H
#define LINUX_IO_URING_H
#include <linux/fs.h>
#include <linux/types.h>
struct io_uring_sqe {
  __u8 opcode;
  __u8 flags;
  __u16 ioprio;
  __s32 fd;
  __u64 off;
  __u64 addr;
  __u32 len;
  union {
    __kernel_rwf_t rw_flags;
    __u32 fsync_flags;
    __u16 poll_events;
    __u32 sync_range_flags;
    __u32 msg_flags;
  };
  __u64 user_data;
  union {
    __u16 buf_index;
    __u64 __pad2[3];
  };
};
#define IOSQE_FIXED_FILE (1U << 0)
#define IOSQE_IO_DRAIN (1U << 1)
#define IOSQE_IO_LINK (1U << 2)
#define IORING_SETUP_IOPOLL (1U << 0)
#define IORING_SETUP_SQPOLL (1U << 1)
#define IORING_SETUP_SQ_AFF (1U << 2)
#define IORING_OP_NOP 0
#define IORING_OP_READV 1
#define IORING_OP_WRITEV 2
#define IORING_OP_FSYNC 3
#define IORING_OP_READ_FIXED 4
#define IORING_OP_WRITE_FIXED 5
#define IORING_OP_POLL_ADD 6
#define IORING_OP_POLL_REMOVE 7
#define IORING_OP_SYNC_FILE_RANGE 8
#define IORING_OP_SENDMSG 9
#define IORING_OP_RECVMSG 10
#define IORING_FSYNC_DATASYNC (1U << 0)
struct io_uring_cqe {
  __u64 user_data;
  __s32 res;
  __u32 flags;
};
#define IORING_OFF_SQ_RING 0ULL
#define IORING_OFF_CQ_RING 0x8000000ULL
#define IORING_OFF_SQES 0x10000000ULL
struct io_sqring_offsets {
  __u32 head;
  __u32 tail;
  __u32 ring_mask;
  __u32 ring_entries;
  __u32 flags;
  __u32 dropped;
  __u32 array;
  __u32 resv1;
  __u64 resv2;
};
#define IORING_SQ_NEED_WAKEUP (1U << 0)
struct io_cqring_offsets {
  __u32 head;
  __u32 tail;
  __u32 ring_mask;
  __u32 ring_entries;
  __u32 overflow;
  __u32 cqes;
  __u64 resv[2];
};
#define IORING_ENTER_GETEVENTS (1U << 0)
#define IORING_ENTER_SQ_WAKEUP (1U << 1)
struct io_uring_params {
  __u32 sq_entries;
  __u32 cq_entries;
  __u32 flags;
  __u32 sq_thread_cpu;
  __u32 sq_thread_idle;
  __u32 resv[5];
  struct io_sqring_offsets sq_off;
  struct io_cqring_offsets cq_off;
};
#define IORING_REGISTER_BUFFERS 0
#define IORING_UNREGISTER_BUFFERS 1
#define IORING_REGISTER_FILES 2
#define IORING_UNREGISTER_FILES 3
#define IORING_REGISTER_EVENTFD 4
#define IORING_UNREGISTER_EVENTFD 5
#endif
