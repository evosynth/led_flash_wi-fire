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

#include <string.h>

extern char _rom_data_copy __attribute__((section(".data")));
extern char _fdata __attribute__((section(".data")));
extern char _edata __attribute__((section(".data")));

void __attribute__((section(".boot"))) software_init_hook(void)
{
	/* copy initialised data from its LMA to VMA */
	memcpy(&_fdata, &_rom_data_copy, (unsigned long int)&_edata -
        (unsigned long int)&_fdata);
}
