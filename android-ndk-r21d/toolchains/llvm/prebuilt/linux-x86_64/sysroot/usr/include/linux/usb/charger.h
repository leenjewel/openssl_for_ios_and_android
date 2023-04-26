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
#ifndef _UAPI__LINUX_USB_CHARGER_H
#define _UAPI__LINUX_USB_CHARGER_H
enum usb_charger_type {
  UNKNOWN_TYPE,
  SDP_TYPE,
  DCP_TYPE,
  CDP_TYPE,
  ACA_TYPE,
};
enum usb_charger_state {
  USB_CHARGER_DEFAULT,
  USB_CHARGER_PRESENT,
  USB_CHARGER_ABSENT,
};
#endif
