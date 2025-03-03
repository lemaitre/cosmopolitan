#-*-mode:makefile-gmake;indent-tabs-mode:t;tab-width:8;coding:utf-8-*-┐
#───vi: set et ft=make ts=8 tw=8 fenc=utf-8 :vi───────────────────────┘

PKGS += THIRD_PARTY_ARGON2

THIRD_PARTY_ARGON2_ARTIFACTS += THIRD_PARTY_ARGON2_A
THIRD_PARTY_ARGON2 = $(THIRD_PARTY_ARGON2_A_DEPS) $(THIRD_PARTY_ARGON2_A)
THIRD_PARTY_ARGON2_A = o/$(MODE)/third_party/argon2/argon2.a
THIRD_PARTY_ARGON2_A_FILES := $(wildcard third_party/argon2/*)
THIRD_PARTY_ARGON2_A_HDRS = $(filter %.h,$(THIRD_PARTY_ARGON2_A_FILES))
THIRD_PARTY_ARGON2_A_SRCS = $(filter %.c,$(THIRD_PARTY_ARGON2_A_FILES))
THIRD_PARTY_ARGON2_A_OBJS = $(THIRD_PARTY_ARGON2_A_SRCS:%.c=o/$(MODE)/%.o)

THIRD_PARTY_ARGON2_A_CHECKS =					\
	$(THIRD_PARTY_ARGON2_A).pkg				\
	$(THIRD_PARTY_ARGON2_A_HDRS:%=o/$(MODE)/%.ok)

THIRD_PARTY_ARGON2_A_DIRECTDEPS =				\
	LIBC_CALLS						\
	LIBC_FMT						\
	LIBC_INTRIN						\
	LIBC_BITS						\
	LIBC_NEXGEN32E						\
	LIBC_MEM						\
	LIBC_SYSV						\
	LIBC_STDIO						\
	LIBC_RUNTIME						\
	LIBC_SYSV_CALLS						\
	LIBC_STR						\
	LIBC_UNICODE						\
	LIBC_STUBS

THIRD_PARTY_ARGON2_A_DEPS :=					\
	$(call uniq,$(foreach x,$(THIRD_PARTY_ARGON2_A_DIRECTDEPS),$($(x))))

$(THIRD_PARTY_ARGON2_A):					\
		third_party/argon2/				\
		$(THIRD_PARTY_ARGON2_A).pkg			\
		$(THIRD_PARTY_ARGON2_A_OBJS)

$(THIRD_PARTY_ARGON2_A).pkg:					\
		$(THIRD_PARTY_ARGON2_A_OBJS)			\
		$(foreach x,$(THIRD_PARTY_ARGON2_A_DIRECTDEPS),$($(x)_A).pkg)

THIRD_PARTY_ARGON2_LIBS = $(foreach x,$(THIRD_PARTY_ARGON2_ARTIFACTS),$($(x)))
THIRD_PARTY_ARGON2_SRCS = $(foreach x,$(THIRD_PARTY_ARGON2_ARTIFACTS),$($(x)_SRCS))
THIRD_PARTY_ARGON2_HDRS = $(foreach x,$(THIRD_PARTY_ARGON2_ARTIFACTS),$($(x)_HDRS))
THIRD_PARTY_ARGON2_CHECKS = $(foreach x,$(THIRD_PARTY_ARGON2_ARTIFACTS),$($(x)_CHECKS))
THIRD_PARTY_ARGON2_OBJS = $(foreach x,$(THIRD_PARTY_ARGON2_ARTIFACTS),$($(x)_OBJS))
$(THIRD_PARTY_ARGON2_OBJS): third_party/argon2/argon2.mk

.PHONY: o/$(MODE)/third_party/argon2
o/$(MODE)/third_party/argon2: $(THIRD_PARTY_ARGON2_CHECKS)
