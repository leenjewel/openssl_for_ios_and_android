/*-
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/lib/msun/arm/fenv.h,v 1.5 2005/03/16 19:03:45 das Exp $
 */

#pragma once

#include <sys/types.h>

__BEGIN_DECLS

/*
 * The ARM Cortex-A75 registers are described here:
 *
 * AArch64:
 *  FPCR: http://infocenter.arm.com/help/topic/com.arm.doc.100403_0200_00_en/lau1442502503726.html
 *  FPSR: http://infocenter.arm.com/help/topic/com.arm.doc.100403_0200_00_en/lau1442502526288.html
 * AArch32:
 *  FPSCR: http://infocenter.arm.com/help/topic/com.arm.doc.100403_0200_00_en/lau1442504290459.html
 */

#if defined(__LP64__)
typedef struct {
  /* FPCR, Floating-point Control Register. */
  __uint32_t __control;
  /* FPSR, Floating-point Status Register. */
  __uint32_t __status;
} fenv_t;

#else
typedef __uint32_t fenv_t;
#endif

typedef __uint32_t fexcept_t;

/* Exception flags. */
#define FE_INVALID    0x01
#define FE_DIVBYZERO  0x02
#define FE_OVERFLOW   0x04
#define FE_UNDERFLOW  0x08
#define FE_INEXACT    0x10
#define FE_DENORMAL   0x80
#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW | FE_DENORMAL)

/* Rounding modes. */
#define FE_TONEAREST  0x0
#define FE_UPWARD     0x1
#define FE_DOWNWARD   0x2
#define FE_TOWARDZERO 0x3

__END_DECLS
