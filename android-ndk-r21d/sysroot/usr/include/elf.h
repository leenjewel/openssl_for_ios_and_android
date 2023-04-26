/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _ELF_H
#define _ELF_H

#include <sys/cdefs.h>

#include <bits/auxvec.h>
#include <bits/elf_arm.h>
#include <bits/elf_arm64.h>
#include <bits/elf_mips.h>
#include <bits/elf_x86.h>
#include <bits/elf_x86_64.h>
#include <linux/elf.h>
#include <linux/elf-em.h>

/* http://www.sco.com/developers/gabi/latest/ch4.intro.html */
typedef __u64 Elf32_Xword;
typedef __s64 Elf32_Sxword;

typedef struct {
  __u32 a_type;
  union {
    __u32 a_val;
  } a_un;
} Elf32_auxv_t;

typedef struct {
  __u64 a_type;
  union {
    __u64 a_val;
  } a_un;
} Elf64_auxv_t;

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
typedef struct {
  Elf32_Word ch_type;
  Elf32_Word ch_size;
  Elf32_Word ch_addralign;
} Elf32_Chdr;
typedef struct {
  Elf64_Word ch_type;
  Elf64_Word ch_reserved;
  Elf64_Xword ch_size;
  Elf64_Xword ch_addralign;
} Elf64_Chdr;

typedef struct {
  Elf32_Word l_name;
  Elf32_Word l_time_stamp;
  Elf32_Word l_checksum;
  Elf32_Word l_version;
  Elf32_Word l_flags;
} Elf32_Lib;
typedef struct {
  Elf64_Word l_name;
  Elf64_Word l_time_stamp;
  Elf64_Word l_checksum;
  Elf64_Word l_version;
  Elf64_Word l_flags;
} Elf64_Lib;
/* ElfW(Lib)::l_flags values. */
#define LL_NONE 0x0
#define LL_EXACT_MATCH 0x1
#define LL_IGNORE_INT_VER 0x2
#define LL_REQUIRE_MINOR 0x4
#define LL_EXPORTS 0x8
#define LL_DELAY_LOAD 0x10
#define LL_DELTA 0x20

typedef struct {
  Elf32_Xword m_value;
  Elf32_Word m_info;
  Elf32_Word m_poffset;
  Elf32_Half m_repeat;
  Elf32_Half m_stride;
} Elf32_Move;
typedef struct {
  Elf64_Xword m_value;
  Elf64_Xword m_info;
  Elf64_Xword m_poffset;
  Elf64_Half m_repeat;
  Elf64_Half m_stride;
} Elf64_Move;

typedef __u16 Elf32_Section;
typedef __u16 Elf64_Section;

typedef struct {
  Elf32_Half si_boundto;
  Elf32_Half si_flags;
} Elf32_Syminfo;
typedef struct {
  Elf64_Half si_boundto;
  Elf64_Half si_flags;
} Elf64_Syminfo;
/* ElfW(Syminfo)::si_boundto values. */
#define SYMINFO_BT_SELF 0xffff
#define SYMINFO_BT_PARENT 0xfffe
/* ElfW(Syminfo)::si_flags values. */
#define SYMINFO_FLG_DIRECT 0x1
#define SYMINFO_FLG_PASSTHRU 0x2
#define SYMINFO_FLG_COPY 0x4
#define SYMINFO_FLG_LAZYLOAD 0x8

typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

typedef struct {
  Elf32_Half vd_version;
  Elf32_Half vd_flags;
  Elf32_Half vd_ndx;
  Elf32_Half vd_cnt;
  Elf32_Word vd_hash;
  Elf32_Word vd_aux;
  Elf32_Word vd_next;
} Elf32_Verdef;

typedef struct {
  Elf32_Word vda_name;
  Elf32_Word vda_next;
} Elf32_Verdaux;

typedef struct {
  Elf64_Half vd_version;
  Elf64_Half vd_flags;
  Elf64_Half vd_ndx;
  Elf64_Half vd_cnt;
  Elf64_Word vd_hash;
  Elf64_Word vd_aux;
  Elf64_Word vd_next;
} Elf64_Verdef;

typedef struct {
  Elf64_Word vda_name;
  Elf64_Word vda_next;
} Elf64_Verdaux;

typedef struct {
  Elf32_Half vn_version;
  Elf32_Half vn_cnt;
  Elf32_Word vn_file;
  Elf32_Word vn_aux;
  Elf32_Word vn_next;
} Elf32_Verneed;

typedef struct {
  Elf32_Word vna_hash;
  Elf32_Half vna_flags;
  Elf32_Half vna_other;
  Elf32_Word vna_name;
  Elf32_Word vna_next;
} Elf32_Vernaux;

typedef struct {
  Elf64_Half vn_version;
  Elf64_Half vn_cnt;
  Elf64_Word vn_file;
  Elf64_Word vn_aux;
  Elf64_Word vn_next;
} Elf64_Verneed;

typedef struct {
  Elf64_Word vna_hash;
  Elf64_Half vna_flags;
  Elf64_Half vna_other;
  Elf64_Word vna_name;
  Elf64_Word vna_next;
} Elf64_Vernaux;

/* Relocation table entry for relative (in section of type SHT_RELR). */
typedef Elf32_Word Elf32_Relr;
typedef Elf64_Xword Elf64_Relr;

/* http://www.sco.com/developers/gabi/latest/ch5.dynamic.html */
#define DF_ORIGIN     0x00000001
#define DF_SYMBOLIC   0x00000002
#define DF_TEXTREL    0x00000004
#define DF_BIND_NOW   0x00000008
#define DF_STATIC_TLS 0x00000010

#define DF_1_NOW        0x00000001 /* Perform complete relocation processing. */
#define DF_1_GLOBAL     0x00000002 /* implies RTLD_GLOBAL */
#define DF_1_GROUP      0x00000004
#define DF_1_NODELETE   0x00000008 /* implies RTLD_NODELETE */
#define DF_1_LOADFLTR   0x00000010
#define DF_1_INITFIRST  0x00000020
#define DF_1_NOOPEN     0x00000040 /* Object can not be used with dlopen(3) */
#define DF_1_ORIGIN     0x00000080
#define DF_1_DIRECT     0x00000100
#define DF_1_TRANS      0x00000200
#define DF_1_INTERPOSE  0x00000400
#define DF_1_NODEFLIB   0x00000800
#define DF_1_NODUMP     0x00001000 /* Object cannot be dumped with dldump(3) */
#define DF_1_CONFALT    0x00002000
#define DF_1_ENDFILTEE  0x00004000
#define DF_1_DISPRELDNE 0x00008000
#define DF_1_DISPRELPND 0x00010000
#define DF_1_NODIRECT   0x00020000
#define DF_1_IGNMULDEF  0x00040000 /* Internal use */
#define DF_1_NOKSYMS    0x00080000 /* Internal use */
#define DF_1_NOHDR      0x00100000 /* Internal use */
#define DF_1_EDITED     0x00200000
#define DF_1_NORELOC    0x00400000 /* Internal use */
#define DF_1_SYMINTPOSE 0x00800000
#define DF_1_GLOBAUDIT  0x01000000
#define DF_1_SINGLETON  0x02000000
#define DF_1_STUB       0x04000000
#define DF_1_PIE        0x08000000

/* http://www.sco.com/developers/gabi/latest/ch5.dynamic.html */
#define DT_BIND_NOW 24
#define DT_INIT_ARRAY 25
#define DT_FINI_ARRAY 26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_RUNPATH 29
#define DT_FLAGS 30
/* glibc and BSD disagree for DT_ENCODING; glibc looks wrong. */
#define DT_PREINIT_ARRAY 32
#define DT_PREINIT_ARRAYSZ 33

/* Experimental support for SHT_RELR sections. For details, see proposal
   at https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg */
#define DT_RELR 0x6fffe000
#define DT_RELRSZ 0x6fffe001
#define DT_RELRENT 0x6fffe003
#define DT_RELRCOUNT 0x6fffe005

/* Android compressed rel/rela sections */
#define DT_ANDROID_REL (DT_LOOS + 2)
#define DT_ANDROID_RELSZ (DT_LOOS + 3)
#define DT_ANDROID_RELA (DT_LOOS + 4)
#define DT_ANDROID_RELASZ (DT_LOOS + 5)

#define DT_GNU_HASH 0x6ffffef5
#define DT_TLSDESC_PLT 0x6ffffef6
#define DT_TLSDESC_GOT 0x6ffffef7

/* http://www.sco.com/developers/gabi/latest/ch4.eheader.html */
#define EI_ABIVERSION 8
#undef EI_PAD
#define EI_PAD 9

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
#define ELFCOMPRESS_ZLIB 1
#define ELFCOMPRESS_LOOS 0x60000000
#define ELFCOMPRESS_HIOS 0x6fffffff
#define ELFCOMPRESS_LOPROC 0x70000000
#define ELFCOMPRESS_HIPROC 0x7fffffff

/* http://www.sco.com/developers/gabi/latest/ch4.eheader.html */
#define ELFOSABI_SYSV 0 /* Synonym for ELFOSABI_NONE used by valgrind. */
#define ELFOSABI_HPUX 1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_GNU 3 /* Synonym for ELFOSABI_LINUX. */
#define ELFOSABI_SOLARIS 6
#define ELFOSABI_AIX 7
#define ELFOSABI_IRIX 8
#define ELFOSABI_FREEBSD 9
#define ELFOSABI_TRU64 10
#define ELFOSABI_MODESTO 11
#define ELFOSABI_OPENBSD 12
#define ELFOSABI_OPENVMS 13
#define ELFOSABI_NSK 14
#define ELFOSABI_AROS 15
#define ELFOSABI_FENIXOS 16
#define ELFOSABI_CLOUDABI 17
#define ELFOSABI_OPENVOS 18
#define ELFOSABI_ARM_AEABI 64

/* http://www.sco.com/developers/gabi/latest/ch4.reloc.html */
#define ELF32_R_INFO(sym, type) ((((Elf32_Word)sym) << 8) | ((type) & 0xff))
#define ELF64_R_INFO(sym, type) ((((Elf64_Xword)sym) << 32) | ((type) & 0xffffffff))

/* http://www.sco.com/developers/gabi/latest/ch4.symtab.html */
#undef ELF_ST_TYPE
#define ELF_ST_TYPE(x) ((x) & 0xf)
#define ELF_ST_INFO(b,t) (((b) << 4) + ((t) & 0xf))
#define ELF32_ST_INFO(b,t) ELF_ST_INFO(b,t)
#define ELF64_ST_INFO(b,t) ELF_ST_INFO(b,t)

/* http://www.sco.com/developers/gabi/latest/ch4.eheader.html */
#define EM_S370 9
#define EM_VPP500 17
#define EM_960 19
#define EM_V800 36
#define EM_FR20 37
#define EM_RH32 38
#define EM_RCE 39
#define EM_FAKE_ALPHA 41
#define EM_TRICORE 44
#define EM_ARC 45
#define EM_H8_300H 47
#define EM_H8S 48
#define EM_H8_500 49
#define EM_MIPS_X 51
#define EM_COLDFIRE 52
#define EM_68HC12 53
#define EM_MMA 54
#define EM_PCP 55
#define EM_NCPU 56
#define EM_NDR1 57
#define EM_STARCORE 58
#define EM_ME16 59
#define EM_ST100 60
#define EM_TINYJ 61
#define EM_PDSP 63
#define EM_PDP10 64
#define EM_PDP11 65
#define EM_FX66 66
#define EM_ST9PLUS 67
#define EM_ST7 68
#define EM_68HC16 69
#define EM_68HC11 70
#define EM_68HC08 71
#define EM_68HC05 72
#define EM_SVX 73
#define EM_ST19 74
#define EM_VAX 75
#define EM_JAVELIN 77
#define EM_FIREPATH 78
#define EM_ZSP 79
#define EM_MMIX 80
#define EM_HUANY 81
#define EM_PRISM 82
#define EM_AVR 83
#define EM_FR30 84
#define EM_D10V 85
#define EM_D30V 86
#define EM_V850 87
#define EM_MN10200 90
#define EM_PJ 91
#define EM_ARC_COMPACT 93
#define EM_XTENSA 94
#define EM_VIDEOCORE 95
#define EM_TMM_GPP 96
#define EM_NS32K 97
#define EM_TPC 98
#define EM_SNP1K 99
#define EM_ST200 100
#define EM_IP2K 101
#define EM_MAX 102
#define EM_CR 103
#define EM_F2MC16 104
#define EM_MSP430 105
#define EM_SE_C33 107
#define EM_SEP 108
#define EM_ARCA 109
#define EM_UNICORE 110
#define EM_EXCESS 111
#define EM_DXP 112
#define EM_CRX 114
#define EM_XGATE 115
#define EM_C166 116
#define EM_M16C 117
#define EM_DSPIC30F 118
#define EM_CE 119
#define EM_M32C 120
#define EM_TSK3000 131
#define EM_RS08 132
#define EM_SHARC 133
#define EM_ECOG2 134
#define EM_SCORE7 135
#define EM_DSP24 136
#define EM_VIDEOCORE3 137
#define EM_LATTICEMICO32 138
#define EM_SE_C17 139
#define EM_TI_C2000 141
#define EM_TI_C5500 142
#define EM_MMDSP_PLUS 160
#define EM_CYPRESS_M8C 161
#define EM_R32C 162
#define EM_TRIMEDIA 163
#define EM_QDSP6 164
#define EM_8051 165
#define EM_STXP7X 166
#define EM_NDS32 167
#define EM_ECOG1 168
#define EM_ECOG1X 168
#define EM_MAXQ30 169
#define EM_XIMO16 170
#define EM_MANIK 171
#define EM_CRAYNV2 172
#define EM_RX 173
#define EM_METAG 174
#define EM_MCST_ELBRUS 175
#define EM_ECOG16 176
#define EM_CR16 177
#define EM_ETPU 178
#define EM_SLE9X 179
#define EM_L10M 180
#define EM_K10M 181
#define EM_AVR32 185
#define EM_STM8 186
#define EM_TILE64 187
#define EM_CUDA 190
#define EM_CLOUDSHIELD 192
#define EM_COREA_1ST 193
#define EM_COREA_2ND 194
#define EM_ARC_COMPACT2 195
#define EM_OPEN8 196
#define EM_RL78 197
#define EM_VIDEOCORE5 198
#define EM_78KOR 199
#define EM_56800EX 200
#define EM_BA1 201
#define EM_BA2 202
#define EM_XCORE 203
#define EM_MCHP_PIC 204
#define EM_INTEL205 205
#define EM_INTEL206 206
#define EM_INTEL207 207
#define EM_INTEL208 208
#define EM_INTEL209 209
#define EM_KM32 210
#define EM_KMX32 211
#define EM_KMX16 212
#define EM_KMX8 213
#define EM_KVARC 214
#define EM_CDP 215
#define EM_COGE 216
#define EM_COOL 217
#define EM_NORC 218
#define EM_CSR_KALIMBA 219
#define EM_Z80 220
#define EM_VISIUM 221
#define EM_FT32 222
#define EM_MOXIE 223
#define EM_AMDGPU 224
#define EM_RISCV 243

/* http://www.sco.com/developers/gabi/latest/ch4.eheader.html */
#define ET_LOOS 0xfe00
#define ET_HIOS 0xfeff

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
#define GRP_COMDAT 0x1
#define GRP_MASKOS   0x0ff00000
#define GRP_MASKPROC 0xf0000000

/* http://www.sco.com/developers/gabi/latest/ch5.pheader.html */
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4
#define PF_MASKOS   0x0ff00000
#define PF_MASKPROC 0xf0000000

#define PT_GNU_RELRO 0x6474e552

#define STB_LOOS 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400
#define SHF_COMPRESSED 0x800
#define SHF_MASKOS 0x0ff00000
#define SHF_MASKPROC 0xf0000000

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
#define SHN_LOOS 0xff20
#define SHN_HIOS 0xff3f
#define SHN_XINDEX 0xffff

/* http://www.sco.com/developers/gabi/latest/ch4.sheader.html */
#define SHT_INIT_ARRAY 14
#define SHT_FINI_ARRAY 15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP 17
#define SHT_SYMTAB_SHNDX 18
#undef SHT_NUM
#define SHT_NUM 19
#define SHT_LOOS 0x60000000
#define SHT_HIOS 0x6fffffff

/* Experimental support for SHT_RELR sections. For details, see proposal
   at https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg */
#define SHT_RELR 0x6fffff00

/* http://www.sco.com/developers/gabi/latest/ch4.symtab.html */
#define STN_UNDEF 0

/* http://www.sco.com/developers/gabi/latest/ch4.symtab.html */
#define STT_GNU_IFUNC 10
#define STT_LOOS 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_HIPROC 15

/* http://www.sco.com/developers/gabi/latest/ch4.symtab.html */
#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

/* The kernel uses NT_PRFPREG but glibc also offers NT_FPREGSET */
#define NT_FPREGSET NT_PRFPREG

#define ELF_NOTE_GNU "GNU"

#define NT_GNU_BUILD_ID 3

#define VER_FLG_BASE 0x1
#define VER_FLG_WEAK 0x2

#define VER_NDX_LOCAL 0
#define VER_NDX_GLOBAL 1

#endif /* _ELF_H */
