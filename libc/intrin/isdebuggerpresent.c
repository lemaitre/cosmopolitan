/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/dce.h"
#include "libc/log/libfatal.internal.h"
#include "libc/log/log.h"
#include "libc/nexgen32e/vendor.internal.h"
#include "libc/nt/struct/teb.h"
#include "libc/sysv/consts/o.h"

#define kBufSize 1024
#define kPid     "TracerPid:\t"

/**
 * Determines if gdb, strace, windbg, etc. is controlling process.
 * @return non-zero if attached, otherwise 0
 */
noasan noubsan int IsDebuggerPresent(bool force) {
  /* asan runtime depends on this function */
  int fd, res;
  ssize_t got;
  char *p, buf[1024];
  if (!force) {
    if (IsGenuineCosmo()) return 0;
    if (__getenv(__envp, "HEISENDEBUG")) return 0;
  }
  if (IsWindows()) {
    return NtGetPeb()->BeingDebugged; /* needs noasan */
  } else {
    res = 0;
    if ((fd = __sysv_open("/proc/self/status", O_RDONLY, 0)) >= 0) {
      if ((got = __sysv_read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[got] = '\0';
        if ((p = __strstr(buf, kPid))) {
          p += sizeof(kPid) - 1;
          res = __atoul(p);
        }
      }
      __sysv_close(fd);
    }
    return res;
  }
}
