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
#include "libc/bits/weaken.h"
#include "libc/calls/calls.h"
#include "libc/calls/internal.h"
#include "libc/calls/struct/iovec.h"
#include "libc/dce.h"
#include "libc/intrin/asan.internal.h"
#include "libc/sock/internal.h"
#include "libc/sock/sock.h"
#include "libc/sysv/errfuns.h"
#include "libc/zipos/zipos.internal.h"

/**
 * Reads data from file descriptor.
 *
 * @param fd is something open()'d earlier
 * @param buf is copied into, cf. copy_file_range(), sendfile(), etc.
 * @param size in range [1..0x7ffff000] is reasonable
 * @return [1..size] bytes on success, 0 on EOF, or -1 w/ errno; with
 *     exception of size==0, in which case return zero means no error
 * @see write(), pread(), readv()
 * @asyncsignalsafe
 */
ssize_t read(int fd, void *buf, size_t size) {
  if (fd >= 0) {
    if (IsAsan() && !__asan_is_valid(buf, size)) return efault();
    if (fd < g_fds.n && g_fds.p[fd].kind == kFdZip) {
      return weaken(__zipos_read)(
          (struct ZiposHandle *)(intptr_t)g_fds.p[fd].handle,
          &(struct iovec){buf, size}, 1, -1);
    } else if (!IsWindows() && !IsMetal()) {
      return sys_read(fd, buf, size);
    } else if (fd >= g_fds.n) {
      return ebadf();
    } else if (IsMetal()) {
      return sys_readv_metal(g_fds.p + fd, &(struct iovec){buf, size}, 1);
    } else {
      return sys_readv_nt(g_fds.p + fd, &(struct iovec){buf, size}, 1);
    }
  } else {
    return einval();
  }
}
