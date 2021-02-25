/*!The Treasure Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (C) 2009-present, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        memmov.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#ifdef TB_ASSEMBLER_IS_GAS
#   define TB_LIBC_STRING_IMPL_MEMMOV
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
#ifdef TB_ASSEMBLER_IS_GAS
static tb_pointer_t tb_memmov_impl(tb_pointer_t s1, tb_cpointer_t s2, tb_size_t n)
{
    tb_assert_and_check_return_val(s1 && s2, tb_null);

    tb_long_t eax, ecx, esi, edi;
    __tb_asm__ __tb_volatile__
    (
        "   movl    %%eax, %%edi\n"
        "   cmpl    %%esi, %%eax\n"
        "   je      2f\n" /* (optional) s2 == s1 -> NOP */
        "   jb      1f\n" /* s2 > s1 -> simple copy */
        "   leal    -1(%%esi,%%ecx), %%esi\n"
        "   leal    -1(%%eax,%%ecx), %%edi\n"
        "   std\n"
        "1: rep;    movsb\n"
        "   cld\n"
        "2:\n"

        : "=&c" (ecx), "=&S" (esi), "=&a" (eax), "=&D" (edi)
        : "0" (n), "1" (s2), "2" (s1)
        : "memory"
    );
    return (tb_pointer_t)eax;
}
#endif
