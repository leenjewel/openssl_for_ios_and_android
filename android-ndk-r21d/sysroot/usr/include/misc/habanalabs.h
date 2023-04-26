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
#ifndef HABANALABS_H_
#define HABANALABS_H_
#include <linux/types.h>
#include <linux/ioctl.h>
#define GOYA_KMD_SRAM_RESERVED_SIZE_FROM_START 0x8000
enum goya_queue_id {
  GOYA_QUEUE_ID_DMA_0 = 0,
  GOYA_QUEUE_ID_DMA_1,
  GOYA_QUEUE_ID_DMA_2,
  GOYA_QUEUE_ID_DMA_3,
  GOYA_QUEUE_ID_DMA_4,
  GOYA_QUEUE_ID_CPU_PQ,
  GOYA_QUEUE_ID_MME,
  GOYA_QUEUE_ID_TPC0,
  GOYA_QUEUE_ID_TPC1,
  GOYA_QUEUE_ID_TPC2,
  GOYA_QUEUE_ID_TPC3,
  GOYA_QUEUE_ID_TPC4,
  GOYA_QUEUE_ID_TPC5,
  GOYA_QUEUE_ID_TPC6,
  GOYA_QUEUE_ID_TPC7,
  GOYA_QUEUE_ID_SIZE
};
enum goya_engine_id {
  GOYA_ENGINE_ID_DMA_0 = 0,
  GOYA_ENGINE_ID_DMA_1,
  GOYA_ENGINE_ID_DMA_2,
  GOYA_ENGINE_ID_DMA_3,
  GOYA_ENGINE_ID_DMA_4,
  GOYA_ENGINE_ID_MME_0,
  GOYA_ENGINE_ID_TPC_0,
  GOYA_ENGINE_ID_TPC_1,
  GOYA_ENGINE_ID_TPC_2,
  GOYA_ENGINE_ID_TPC_3,
  GOYA_ENGINE_ID_TPC_4,
  GOYA_ENGINE_ID_TPC_5,
  GOYA_ENGINE_ID_TPC_6,
  GOYA_ENGINE_ID_TPC_7,
  GOYA_ENGINE_ID_SIZE
};
enum hl_device_status {
  HL_DEVICE_STATUS_OPERATIONAL,
  HL_DEVICE_STATUS_IN_RESET,
  HL_DEVICE_STATUS_MALFUNCTION
};
#define HL_INFO_HW_IP_INFO 0
#define HL_INFO_HW_EVENTS 1
#define HL_INFO_DRAM_USAGE 2
#define HL_INFO_HW_IDLE 3
#define HL_INFO_DEVICE_STATUS 4
#define HL_INFO_VERSION_MAX_LEN 128
struct hl_info_hw_ip_info {
  __u64 sram_base_address;
  __u64 dram_base_address;
  __u64 dram_size;
  __u32 sram_size;
  __u32 num_of_events;
  __u32 device_id;
  __u32 reserved[3];
  __u32 armcp_cpld_version;
  __u32 psoc_pci_pll_nr;
  __u32 psoc_pci_pll_nf;
  __u32 psoc_pci_pll_od;
  __u32 psoc_pci_pll_div_factor;
  __u8 tpc_enabled_mask;
  __u8 dram_enabled;
  __u8 pad[2];
  __u8 armcp_version[HL_INFO_VERSION_MAX_LEN];
};
struct hl_info_dram_usage {
  __u64 dram_free_mem;
  __u64 ctx_dram_mem;
};
struct hl_info_hw_idle {
  __u32 is_idle;
  __u32 busy_engines_mask;
};
struct hl_info_device_status {
  __u32 status;
  __u32 pad;
};
struct hl_info_args {
  __u64 return_pointer;
  __u32 return_size;
  __u32 op;
  __u32 ctx_id;
  __u32 pad;
};
#define HL_CB_OP_CREATE 0
#define HL_CB_OP_DESTROY 1
struct hl_cb_in {
  __u64 cb_handle;
  __u32 op;
  __u32 cb_size;
  __u32 ctx_id;
  __u32 pad;
};
struct hl_cb_out {
  __u64 cb_handle;
};
union hl_cb_args {
  struct hl_cb_in in;
  struct hl_cb_out out;
};
struct hl_cs_chunk {
  __u64 cb_handle;
  __u32 queue_index;
  __u32 cb_size;
  __u32 cs_chunk_flags;
  __u32 pad[11];
};
#define HL_CS_FLAGS_FORCE_RESTORE 0x1
#define HL_CS_STATUS_SUCCESS 0
struct hl_cs_in {
  __u64 chunks_restore;
  __u64 chunks_execute;
  __u64 chunks_store;
  __u32 num_chunks_restore;
  __u32 num_chunks_execute;
  __u32 num_chunks_store;
  __u32 cs_flags;
  __u32 ctx_id;
};
struct hl_cs_out {
  __u64 seq;
  __u32 status;
  __u32 pad;
};
union hl_cs_args {
  struct hl_cs_in in;
  struct hl_cs_out out;
};
struct hl_wait_cs_in {
  __u64 seq;
  __u64 timeout_us;
  __u32 ctx_id;
  __u32 pad;
};
#define HL_WAIT_CS_STATUS_COMPLETED 0
#define HL_WAIT_CS_STATUS_BUSY 1
#define HL_WAIT_CS_STATUS_TIMEDOUT 2
#define HL_WAIT_CS_STATUS_ABORTED 3
#define HL_WAIT_CS_STATUS_INTERRUPTED 4
struct hl_wait_cs_out {
  __u32 status;
  __u32 pad;
};
union hl_wait_cs_args {
  struct hl_wait_cs_in in;
  struct hl_wait_cs_out out;
};
#define HL_MEM_OP_ALLOC 0
#define HL_MEM_OP_FREE 1
#define HL_MEM_OP_MAP 2
#define HL_MEM_OP_UNMAP 3
#define HL_MEM_CONTIGUOUS 0x1
#define HL_MEM_SHARED 0x2
#define HL_MEM_USERPTR 0x4
struct hl_mem_in {
  union {
    struct {
      __u64 mem_size;
    } alloc;
    struct {
      __u64 handle;
    } free;
    struct {
      __u64 hint_addr;
      __u64 handle;
    } map_device;
    struct {
      __u64 host_virt_addr;
      __u64 hint_addr;
      __u64 mem_size;
    } map_host;
    struct {
      __u64 device_virt_addr;
    } unmap;
  };
  __u32 op;
  __u32 flags;
  __u32 ctx_id;
  __u32 pad;
};
struct hl_mem_out {
  union {
    __u64 device_virt_addr;
    __u64 handle;
  };
};
union hl_mem_args {
  struct hl_mem_in in;
  struct hl_mem_out out;
};
#define HL_DEBUG_MAX_AUX_VALUES 10
struct hl_debug_params_etr {
  __u64 buffer_address;
  __u64 buffer_size;
  __u32 sink_mode;
  __u32 pad;
};
struct hl_debug_params_etf {
  __u64 buffer_address;
  __u64 buffer_size;
  __u32 sink_mode;
  __u32 pad;
};
struct hl_debug_params_stm {
  __u64 he_mask;
  __u64 sp_mask;
  __u32 id;
  __u32 frequency;
};
struct hl_debug_params_bmon {
  __u64 start_addr0;
  __u64 addr_mask0;
  __u64 start_addr1;
  __u64 addr_mask1;
  __u32 bw_win;
  __u32 win_capture;
  __u32 id;
  __u32 pad;
};
struct hl_debug_params_spmu {
  __u64 event_types[HL_DEBUG_MAX_AUX_VALUES];
  __u32 event_types_num;
  __u32 pad;
};
#define HL_DEBUG_OP_ETR 0
#define HL_DEBUG_OP_ETF 1
#define HL_DEBUG_OP_STM 2
#define HL_DEBUG_OP_FUNNEL 3
#define HL_DEBUG_OP_BMON 4
#define HL_DEBUG_OP_SPMU 5
#define HL_DEBUG_OP_TIMESTAMP 6
#define HL_DEBUG_OP_SET_MODE 7
struct hl_debug_args {
  __u64 input_ptr;
  __u64 output_ptr;
  __u32 input_size;
  __u32 output_size;
  __u32 op;
  __u32 reg_idx;
  __u32 enable;
  __u32 ctx_id;
};
#define HL_IOCTL_INFO _IOWR('H', 0x01, struct hl_info_args)
#define HL_IOCTL_CB _IOWR('H', 0x02, union hl_cb_args)
#define HL_IOCTL_CS _IOWR('H', 0x03, union hl_cs_args)
#define HL_IOCTL_WAIT_CS _IOWR('H', 0x04, union hl_wait_cs_args)
#define HL_IOCTL_MEMORY _IOWR('H', 0x05, union hl_mem_args)
#define HL_IOCTL_DEBUG _IOWR('H', 0x06, struct hl_debug_args)
#define HL_COMMAND_START 0x01
#define HL_COMMAND_END 0x07
#endif
