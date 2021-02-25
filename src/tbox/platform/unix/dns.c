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
 * @file        dns.c
 * @ingroup     platform
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../file.h"
#include "../impl/dns.h"
#include "../../libc/libc.h"
#include "../../stream/stream.h"
#include "../../network/network.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tb_bool_t tb_dns_init_env()
{
    // done
    tb_size_t count = 0;
    if (tb_file_info("/etc/resolv.conf", tb_null))
    {
        /* try get list from "/etc/resolv.conf"
         *
         * # Generated by NetworkManager
         * nameserver 10.1.20.10
         * nameserver 8.8.8.8
         *
         */
        tb_stream_ref_t stream = tb_stream_init_from_url("/etc/resolv.conf");
        if (stream)
        {
            // open
            if (tb_stream_open(stream))
            {
                // read
                tb_long_t size = 0;
                tb_char_t line[8192];
                while ((size = tb_stream_bread_line(stream, line, 8192)) >= 0)
                {
                    if (size && !tb_strnicmp(line, "nameserver", 10))
                    {
                        // seek to server
                        tb_char_t const* p = line + 10;
                        while (*p && !tb_isdigit(*p)) p++;
                        tb_check_continue(*p);

                        // add server
                        tb_dns_server_add(p);

                        // count++
                        count++;
                    }
                }
            }

            // exit
            tb_stream_exit(stream);
        }
    }

    // ok
    return tb_true;
}
tb_void_t tb_dns_exit_env()
{
}

