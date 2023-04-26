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
#ifndef _UAPI_LINUX_VSOC_SHM_H
#define _UAPI_LINUX_VSOC_SHM_H
#include <linux/types.h>
struct fd_scoped_permission {
  __u32 begin_offset;
  __u32 end_offset;
  __u32 owner_offset;
  __u32 owned_value;
};
#define VSOC_REGION_FREE ((__u32) 0)
struct fd_scoped_permission_arg {
  struct fd_scoped_permission perm;
  __s32 managed_region_fd;
};
#define VSOC_NODE_FREE ((__u32) 0)
struct vsoc_signal_table_layout {
  __u32 num_nodes_lg2;
  __u32 futex_uaddr_table_offset;
  __u32 interrupt_signalled_offset;
};
#define VSOC_REGION_WHOLE ((__s32) 0)
#define VSOC_DEVICE_NAME_SZ 16
struct vsoc_device_region {
  __u16 current_version;
  __u16 min_compatible_version;
  __u32 region_begin_offset;
  __u32 region_end_offset;
  __u32 offset_of_region_data;
  struct vsoc_signal_table_layout guest_to_host_signal_table;
  struct vsoc_signal_table_layout host_to_guest_signal_table;
  char device_name[VSOC_DEVICE_NAME_SZ];
  __u32 managed_by;
};
struct vsoc_shm_layout_descriptor {
  __u16 major_version;
  __u16 minor_version;
  __u32 size;
  __u32 region_count;
  __u32 vsoc_region_desc_offset;
};
#define CURRENT_VSOC_LAYOUT_MAJOR_VERSION 2
#define CURRENT_VSOC_LAYOUT_MINOR_VERSION 0
#define VSOC_CREATE_FD_SCOPED_PERMISSION _IOW(0xF5, 0, struct fd_scoped_permission)
#define VSOC_GET_FD_SCOPED_PERMISSION _IOR(0xF5, 1, struct fd_scoped_permission)
#define VSOC_MAYBE_SEND_INTERRUPT_TO_HOST _IO(0xF5, 2)
#define VSOC_WAIT_FOR_INCOMING_INTERRUPT _IO(0xF5, 3)
#define VSOC_DESCRIBE_REGION _IOR(0xF5, 4, struct vsoc_device_region)
#define VSOC_SELF_INTERRUPT _IO(0xF5, 5)
#define VSOC_SEND_INTERRUPT_TO_HOST _IO(0xF5, 6)
enum wait_types {
  VSOC_WAIT_UNDEFINED = 0,
  VSOC_WAIT_IF_EQUAL = 1,
  VSOC_WAIT_IF_EQUAL_TIMEOUT = 2
};
struct vsoc_cond_wait {
  __u32 offset;
  __u32 value;
  __u64 wake_time_sec;
  __u32 wake_time_nsec;
  __u32 wait_type;
  __u32 wakes;
  __u32 reserved_1;
};
#define VSOC_COND_WAIT _IOWR(0xF5, 7, struct vsoc_cond_wait)
#define VSOC_COND_WAKE _IO(0xF5, 8)
#endif
