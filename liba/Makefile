SFLAGS += -Iliba/include

liba/src/external/sqlite/mem5.o: CFLAGS += -w

objs += $(addprefix liba/src/, \
  armv7m/setjmp.o \
  armv7m/longjmp.o \
  assert.o \
  bzero.o \
  ctype.o \
  errno.o \
  fpclassify.o \
  fpclassifyf.o \
  ieee754.o \
  malloc.o \
  memcmp.o \
  memcpy.o \
  memmove.o \
  memset.o \
  nearbyintf.o \
  strcmp.o \
  strchr.o \
  strlcpy.o \
  strlen.o \
  external/sqlite/mem5.o \
)

objs += $(addprefix liba/src/external/openbsd/, \
  e_acosf.o \
  e_acoshf.o \
  e_asinf.o \
  e_atanhf.o \
  e_atan2f.o \
  e_coshf.o \
  e_expf.o \
  e_fmodf.o \
  e_lgammaf_r.o \
  e_log10f.o \
  e_logf.o \
  e_powf.o \
  e_rem_pio2f.o \
  e_sinhf.o \
  e_sqrtf.o \
  k_cosf.o \
  k_rem_pio2f.o \
  k_sinf.o \
  k_tanf.o \
  s_asinhf.o\
  s_atanf.o \
  s_ceilf.o \
  s_copysignf.o \
  s_cosf.o \
  s_expm1f.o\
  s_fabsf.o \
  s_floorf.o \
  s_log1pf.o \
  s_roundf.o \
  s_scalbnf.o \
  s_signgam.o \
  s_sinf.o \
  s_tanf.o \
  s_tanhf.o \
  w_lgammaf.o \
)

objs += $(addprefix liba/src/external/openbsd/, \
  e_acos.o \
  e_acosh.o \
  e_asin.o \
  e_atanh.o \
  e_cosh.o \
  e_exp.o \
  e_lgamma_r.o \
  e_log.o \
  e_log10.o \
  e_pow.o \
  e_rem_pio2.o \
  e_sinh.o \
  e_sqrt.o \
  k_cos.o \
  k_rem_pio2.o \
  k_sin.o \
  k_tan.o \
  s_asinh.o \
  s_atan.o \
  s_ceil.o \
  s_copysign.o \
  s_cos.o \
  s_expm1.o \
  s_fabs.o \
  s_floor.o \
  s_log1p.o \
  s_nan.o \
  s_round.o \
  s_scalbn.o \
  s_sin.o \
  s_tan.o \
  s_tanh.o \
  w_lgamma.o \
)

liba/src/external/openbsd/%.o: SFLAGS := -Iliba/src/external/openbsd/include $(SFLAGS)
liba/src/external/openbsd/e_lgammaf_r.o: CFLAGS += -w
liba/src/external/openbsd/s_log1pf.o: CFLAGS += -w
liba/src/external/openbsd/s_scalbnf.o: CFLAGS += -w
liba/src/external/openbsd/e_acosh.o: CFLAGS += -w
liba/src/external/openbsd/e_atanh.o: CFLAGS += -w
liba/src/external/openbsd/e_cosh.o: CFLAGS += -w
liba/src/external/openbsd/e_exp.o: CFLAGS += -w
liba/src/external/openbsd/e_log.o: CFLAGS += -w
liba/src/external/openbsd/e_lgamma_r.o: CFLAGS += -w
liba/src/external/openbsd/e_pow.o: CFLAGS += -w
liba/src/external/openbsd/e_rem_pio2.o: CFLAGS += -w
liba/src/external/openbsd/e_sinh.o: CFLAGS += -w
liba/src/external/openbsd/k_rem_pio2.o: CFLAGS += -w
liba/src/external/openbsd/k_rem_pio2f.o: CFLAGS += -w
liba/src/external/openbsd/s_asinh.o: CFLAGS += -w
liba/src/external/openbsd/s_log1p.o: CFLAGS += -w
liba/src/external/openbsd/s_scalbn.o: CFLAGS += -w
liba/src/external/openbsd/s_tanh.o: CFLAGS += -w
liba/src/external/openbsd/w_lgamma.o: CFLAGS += -w
# some openbsd classes are throwing implicit declaration warnings
ifeq ($(DEBUG),1)
# OpenBSD uses double constants ("0.5" instead of "0.5f") in single-precision
# code. That's annoying because Clang rightfully decides to emit double-to-float
# aeabi conversions when building in -O0 mode, and we really don't want to code
# such functions. A simple workaround is to always build those files -Os.
liba/src/external/openbsd/e_expf.o: CFLAGS += -Os
liba/src/external/openbsd/s_expm1f.o: CFLAGS += -Os
liba/src/external/openbsd/s_log1pf.o: CFLAGS += -Os
liba/src/external/openbsd/s_roundf.o: CFLAGS += -Os
endif

tests += $(addprefix liba/test/, \
  aeabi.c \
  double.c \
  ieee754.c \
  long.c \
  math.c \
  setjmp.c \
  stddef.c \
  stdint.c \
  strlcpy.c \
)

# The use of aeabi-rt could be made conditional to an AEABI target.
# In practice we're always using liba on such a target.
objs += $(addprefix liba/src/aeabi-rt/, \
  atexit.o \
  double.o \
  llsl.o \
  llsr.o \
  long.o \
  memclr.o \
  memcpy.o \
)

liba/src/external/softfloat/src/%.o: CFLAGS += -Iliba/src/external/softfloat/include -Iliba/src/external/softfloat/src/8086 -Iliba/src/external/softfloat/port
liba/src/external/softfloat/src/s_roundPackToF64.o: CFLAGS += -w
liba/src/external/softfloat/src/s_roundPackToF32.o: CFLAGS += -w
# s_roundPackToF64 and s_roundPackToF32 are throwing warnings

objs += $(addprefix liba/src/external/softfloat/src/, \
  8086/s_commonNaNToF32UI.o \
  8086/s_commonNaNToF64UI.o \
  8086/s_f32UIToCommonNaN.o \
  8086/s_f64UIToCommonNaN.o \
  8086/s_propagateNaNF64UI.o \
  8086/softfloat_raiseFlags.o \
  f32_to_f64.o \
  f32_to_i64_r_minMag.o\
  f64_add.o \
  f64_div.o \
  f64_eq.o \
  f64_le.o \
  f64_lt.o \
  f64_mul.o \
  f64_sub.o \
  f64_to_i32_r_minMag.o \
  f64_to_i64_r_minMag.o \
  f64_to_f32.o \
  i32_to_f64.o \
  i64_to_f32.o \
  i64_to_f64.o \
  s_addMagsF64.o \
  s_approxRecip32_1.o \
  s_approxRecip_1Ks.o \
  s_countLeadingZeros32.o \
  s_countLeadingZeros64.o \
  s_countLeadingZeros8.o \
  s_mul64To128M.o \
  s_normRoundPackToF64.o \
  s_normSubnormalF32Sig.o \
  s_normSubnormalF64Sig.o \
  s_roundPackToF32.o \
  s_roundPackToF64.o \
  s_shiftRightJam32.o \
  s_shiftRightJam64.o \
  s_shortShiftRightJam64.o \
  s_subMagsF64.o \
  softfloat_state.o \
)
