/* Copyright 2017 Julian Ingram
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _FUNCTION_SECTIONS_

/* MIPS single vector mode simple ISR call from vector */

#include <mips/regdef.h>
#include <mips/asm.h>
#include <mips/cpu.h>

LEAF(__isr_vec)
	.set 	push
	.set 	noat
AENT(__isr_vec_single)
	j 	_mips_interrupt
	.set 	pop
END(__isr_vec)

.text
.weak	_mips_interrupt
_mips_interrupt:
	j 	_mips_interrupt /* loop */
