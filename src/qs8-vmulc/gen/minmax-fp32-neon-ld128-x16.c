// Auto-generated file. Do not edit!
//   Template: src/qs8-vmulc/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/vmul.h>


void xnn_qs8_vmulc_minmax_fp32_ukernel__neon_ld128_x16(
    size_t n,
    const int8_t* input_a,
    const int8_t* input_b,
    int8_t* output,
    const union xnn_qs8_mul_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN XNN_DISABLE_MSAN
{
  #if XNN_ARCH_ARM64
    const int8x16_t va_zero_point = vld1q_dup_s8(params->fp32_neon.a_zero_point);
  #else
    const int8x8_t va_zero_point = vld1_dup_s8(params->fp32_neon.a_zero_point);
  #endif
  const float32x4_t vscale = vld1q_dup_f32(&params->fp32_neon.scale);
  const float32x4_t voutput_min_less_zero_point = vld1q_dup_f32(&params->fp32_neon.output_min_less_zero_point);
  const float32x4_t voutput_max_less_zero_point = vld1q_dup_f32(&params->fp32_neon.output_max_less_zero_point);
  const float32x4_t vmagic_bias = vld1q_dup_f32(&params->fp32_neon.magic_bias);
  const int32x4_t vmagic_bias_less_zero_point = vld1q_dup_s32(&params->fp32_neon.magic_bias_less_zero_point);

  const int8x8_t vb = vld1_dup_s8(input_b);
  const int8x8_t vb_zero_point = vld1_dup_s8(params->fp32_neon.b_zero_point);
  const int16x8_t vxb = vsubl_s8(vb, vb_zero_point);
  for (; n >= 16 * sizeof(int8_t); n -= 16 * sizeof(int8_t)) {
    const int8x16_t va0123456789ABCDEF = vld1q_s8(input_a); input_a += 16;

    #if XNN_ARCH_ARM64
      const int16x8_t vxa01234567 = vsubl_s8(vget_low_s8(va0123456789ABCDEF), vget_low_s8(va_zero_point));
      const int16x8_t vxa89ABCDEF = vsubl_high_s8(va0123456789ABCDEF, va_zero_point);
    #else  // !XNN_ARCH_ARM64
      const int16x8_t vxa01234567 = vsubl_s8(vget_low_s8(va0123456789ABCDEF), va_zero_point);
      const int16x8_t vxa89ABCDEF = vsubl_s8(vget_high_s8(va0123456789ABCDEF), va_zero_point);
    #endif  // XNN_ARCH_ARM64

    int32x4_t vacc0123 = vmull_s16(vget_low_s16(vxa01234567), vget_low_s16(vxb));
    int32x4_t vacc4567 = vmull_s16(vget_high_s16(vxa01234567), vget_high_s16(vxb));
    int32x4_t vacc89AB = vmull_s16(vget_low_s16(vxa89ABCDEF), vget_low_s16(vxb));
    int32x4_t vaccCDEF = vmull_s16(vget_high_s16(vxa89ABCDEF), vget_high_s16(vxb));

    float32x4_t vfpacc0123 = vcvtq_f32_s32(vacc0123);
    float32x4_t vfpacc4567 = vcvtq_f32_s32(vacc4567);
    float32x4_t vfpacc89AB = vcvtq_f32_s32(vacc89AB);
    float32x4_t vfpaccCDEF = vcvtq_f32_s32(vaccCDEF);

    vfpacc0123 = vmulq_f32(vfpacc0123, vscale);
    vfpacc4567 = vmulq_f32(vfpacc4567, vscale);
    vfpacc89AB = vmulq_f32(vfpacc89AB, vscale);
    vfpaccCDEF = vmulq_f32(vfpaccCDEF, vscale);

    vfpacc0123 = vmaxq_f32(vfpacc0123, voutput_min_less_zero_point);
    vfpacc4567 = vmaxq_f32(vfpacc4567, voutput_min_less_zero_point);
    vfpacc89AB = vmaxq_f32(vfpacc89AB, voutput_min_less_zero_point);
    vfpaccCDEF = vmaxq_f32(vfpaccCDEF, voutput_min_less_zero_point);

    vfpacc0123 = vminq_f32(vfpacc0123, voutput_max_less_zero_point);
    vfpacc4567 = vminq_f32(vfpacc4567, voutput_max_less_zero_point);
    vfpacc89AB = vminq_f32(vfpacc89AB, voutput_max_less_zero_point);
    vfpaccCDEF = vminq_f32(vfpaccCDEF, voutput_max_less_zero_point);

    vacc0123 = vreinterpretq_s32_f32(vaddq_f32(vfpacc0123, vmagic_bias));
    vacc4567 = vreinterpretq_s32_f32(vaddq_f32(vfpacc4567, vmagic_bias));
    vacc89AB = vreinterpretq_s32_f32(vaddq_f32(vfpacc89AB, vmagic_bias));
    vaccCDEF = vreinterpretq_s32_f32(vaddq_f32(vfpaccCDEF, vmagic_bias));

    vacc0123 = vsubq_s32(vacc0123, vmagic_bias_less_zero_point);
    vacc4567 = vsubq_s32(vacc4567, vmagic_bias_less_zero_point);
    vacc89AB = vsubq_s32(vacc89AB, vmagic_bias_less_zero_point);
    vaccCDEF = vsubq_s32(vaccCDEF, vmagic_bias_less_zero_point);

#if XNN_ARCH_ARM64
      const int16x8_t vacc01234567 = vuzp1q_s16(vreinterpretq_s16_s32(vacc0123), vreinterpretq_s16_s32(vacc4567));
      const int16x8_t vacc89ABCDEF = vuzp1q_s16(vreinterpretq_s16_s32(vacc89AB), vreinterpretq_s16_s32(vaccCDEF));

      int8x16_t vout0123456789ABCDEF = vuzp1q_s8(vreinterpretq_s8_s16(vacc01234567), vreinterpretq_s8_s16(vacc89ABCDEF));
#else
      const int16x8_t vacc01234567 = vcombine_s16(vmovn_s32(vacc0123), vmovn_s32(vacc4567));
      const int16x8_t vacc89ABCDEF = vcombine_s16(vmovn_s32(vacc89AB), vmovn_s32(vaccCDEF));

      int8x16_t vout0123456789ABCDEF = vcombine_s8(vmovn_s16(vacc01234567), vmovn_s16(vacc89ABCDEF));
#endif

    vst1q_s8(output, vout0123456789ABCDEF); output += 16;
  }
  if XNN_UNLIKELY(n != 0) {
    do {
      const int8x8_t va01234567 = vld1_s8(input_a); input_a += 8;

      #if XNN_ARCH_ARM64
        const int16x8_t vxa01234567 = vsubl_s8(va01234567, vget_low_s8(va_zero_point));
      #else  // !XNN_ARCH_ARM64
        const int16x8_t vxa01234567 = vsubl_s8(va01234567, va_zero_point);
      #endif  // XNN_ARCH_ARM64

      int32x4_t vacc0123 = vmull_s16(vget_low_s16(vxa01234567), vget_low_s16(vxb));
      int32x4_t vacc4567 = vmull_s16(vget_high_s16(vxa01234567), vget_high_s16(vxb));

      float32x4_t vfpacc0123 = vcvtq_f32_s32(vacc0123);
      float32x4_t vfpacc4567 = vcvtq_f32_s32(vacc4567);

      vfpacc0123 = vmulq_f32(vfpacc0123, vscale);
      vfpacc4567 = vmulq_f32(vfpacc4567, vscale);

      vfpacc0123 = vmaxq_f32(vfpacc0123, voutput_min_less_zero_point);
      vfpacc4567 = vmaxq_f32(vfpacc4567, voutput_min_less_zero_point);

      vfpacc0123 = vminq_f32(vfpacc0123, voutput_max_less_zero_point);
      vfpacc4567 = vminq_f32(vfpacc4567, voutput_max_less_zero_point);

      vacc0123 = vreinterpretq_s32_f32(vaddq_f32(vfpacc0123, vmagic_bias));
      vacc4567 = vreinterpretq_s32_f32(vaddq_f32(vfpacc4567, vmagic_bias));

      vacc0123 = vsubq_s32(vacc0123, vmagic_bias_less_zero_point);
      vacc4567 = vsubq_s32(vacc4567, vmagic_bias_less_zero_point);

#if XNN_ARCH_ARM64
      const int16x8_t vacc01234567 = vuzp1q_s16(vreinterpretq_s16_s32(vacc0123), vreinterpretq_s16_s32(vacc4567));
      int8x8_t vout01234567 = vmovn_s16(vacc01234567);
#else
      const int16x8_t vacc01234567 = vcombine_s16(vmovn_s32(vacc0123), vmovn_s32(vacc4567));
      int8x8_t vout01234567 = vmovn_s16(vacc01234567);
#endif

      if XNN_LIKELY(n >= (8 * sizeof(int8_t))) {
        vst1_s8(output, vout01234567); output += 8;
        n -= 8 * sizeof(int8_t);
      } else {
        if (n & (4 * sizeof(int8_t))) {
          vst1_lane_u32(__builtin_assume_aligned(output, 1), vreinterpret_u32_s8(vout01234567), 0); output += 4;
          vout01234567 = vext_s8(vout01234567, vout01234567, 4);
        }
        if (n & (2 * sizeof(int8_t))) {
          vst1_lane_u16(__builtin_assume_aligned(output, 1), vreinterpret_u16_s8(vout01234567), 0); output += 2;
          vout01234567 = vext_s8(vout01234567, vout01234567, 2);
        }
        if (n & (1 * sizeof(int8_t))) {
          vst1_lane_s8(output, vout01234567, 0);
        }
        n = 0;
      }
    } while (n != 0);
  }
}
