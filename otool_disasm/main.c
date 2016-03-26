//
//
//  some must have defines from the original main.c
//
//
//
//

/*
 * Copyright © 2009 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1.  Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @APPLE_LICENSE_HEADER_END@
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stuff/bool.h"
#include "stuff/allocate.h"
#include "stuff/symbol.h"
#include "stuff/bytesex.h"
#include "stuff/guess_short_name.h"
#include "dyld_bind_info.h"

/* Name of this program for error messages (argv[0]) */
char *progname = NULL;

/*
 * The flags to indicate the actions to perform.
 */
enum bool gflag = FALSE; /* group the disassembly */
enum bool Xflag = FALSE; /* don't print leading address in disassembly */
enum bool Bflag = FALSE; /* force Thumb disassembly (ARM objects only) */
enum bool qflag = TRUE; /* use 'C' Public llvm-mc disassembler */
enum bool jflag = FALSE; /* print opcode bytes */
enum bool no_show_raw_insn = FALSE; /* no raw inst, for llvm-objdump testing
                                     with 32-bit arm */
char *mcpu = "";	/* the arg of the -mcpu=arg flag */

/*
 * Function for qsort for comparing symbols.
 */
int
sym_compare(
struct symbol *sym1,
struct symbol *sym2)
{
	if(sym1->n_value == sym2->n_value)
	    return(0);
	if(sym1->n_value < sym2->n_value)
	    return(-1);
	else
	    return(1);
}

/*
 * Function for qsort for comparing relocation entries.
 */
int
rel_compare(
struct relocation_info *rel1,
struct relocation_info *rel2)
{
    struct scattered_relocation_info *srel;
    uint32_t r_address1, r_address2;

	if((rel1->r_address & R_SCATTERED) != 0){
	    srel = (struct scattered_relocation_info *)rel1;
	    r_address1 = srel->r_address;
	}
	else
	    r_address1 = rel1->r_address;
	if((rel2->r_address & R_SCATTERED) != 0){
	    srel = (struct scattered_relocation_info *)rel2;
	    r_address2 = srel->r_address;
	}
	else
	    r_address2 = rel2->r_address;

	if(r_address1 == r_address2)
	    return(0);
	if(r_address1 < r_address2)
	    return(-1);
	else
	    return(1);
}

