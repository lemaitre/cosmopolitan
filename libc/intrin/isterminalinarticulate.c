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
#include "libc/bits/safemacros.internal.h"
#include "libc/log/internal.h"
#include "libc/log/libfatal.internal.h"
#include "libc/log/log.h"
#include "libc/nt/enum/version.h"
#include "libc/runtime/runtime.h"
#include "libc/str/str.h"

bool g_isterminalinarticulate;

bool IsTerminalInarticulate(void) {
  return g_isterminalinarticulate;
}

textstartup noasan void g_isterminalinarticulate_init(int argc, char **argv,
                                                      char **envp,
                                                      intptr_t *auxv) {
  char *s;
  if (IsWindows() && NtGetVersion() < kNtVersionWindows10) {
    g_isterminalinarticulate = true;
  } else if ((s = __getenv(envp, "TERM"))) {
    g_isterminalinarticulate = !__strcmp(s, "dumb");
  }
}

const void *const g_isterminalinarticulate_ctor[] initarray = {
    g_isterminalinarticulate_init,
};
