// Auto-generated file. Do not edit!
//   Template: src/f32-sigmoid/avx512f-lut32-p2-perm2-scalef.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vunary.h>


void xnn_f32_sigmoid_ukernel__avx512f_rr2_lut32_p2_perm2_scalef_div_x32(
    size_t n,
    const float* x,
    float* y,
    const void* params)
{
  assert(n % sizeof(float) == 0);

  const __m512i vsign_mask = _mm512_set1_epi32(0x80000000);
  const __m512 vmagic_bias = _mm512_set1_ps(0x1.800000p18f);
  const __m512 vlog2e = _mm512_set1_ps(0x1.715476p0f);
  const __m512 vtable_hi = _mm512_set_ps(
    0x1.F50766p+0f, 0x1.EA4AFAp+0f, 0x1.DFC974p+0f, 0x1.D5818Ep+0f,
    0x1.CB720Ep+0f, 0x1.C199BEp+0f, 0x1.B7F770p+0f, 0x1.AE89FAp+0f,
    0x1.A5503Cp+0f, 0x1.9C4918p+0f, 0x1.93737Cp+0f, 0x1.8ACE54p+0f,
    0x1.82589Ap+0f, 0x1.7A1148p+0f, 0x1.71F75Ep+0f, 0x1.6A09E6p+0f);
  const __m512 vtable_lo = _mm512_set_ps(
    0x1.6247ECp+0f, 0x1.5AB07Ep+0f, 0x1.5342B6p+0f, 0x1.4BFDAEp+0f,
    0x1.44E086p+0f, 0x1.3DEA64p+0f, 0x1.371A74p+0f, 0x1.306FE0p+0f,
    0x1.29E9E0p+0f, 0x1.2387A6p+0f, 0x1.1D4874p+0f, 0x1.172B84p+0f,
    0x1.11301Ep+0f, 0x1.0B5586p+0f, 0x1.059B0Ep+0f, 0x1.000000p+0f);
  const __m512 vminus_ln2_hi = _mm512_set1_ps(-0x1.62E43p-1f);
  const __m512 vminus_ln2_lo = _mm512_set1_ps(0x1.05C61p-29f);
  const __m512 vc2 = _mm512_set1_ps(0x1.000000p-1f);
  const __m512 vc1 = _mm512_set1_ps(0x1.0000F6p-0f);
  const __m512 vone = _mm512_set1_ps(1.0f);

  for (; n >= 32 * sizeof(float); n -= 32 * sizeof(float)) {
    const __m512 vx0 = _mm512_loadu_ps(x);
    const __m512 vx1 = _mm512_loadu_ps(x + 16);
    x += 32;

    const __m512 vz0 = _mm512_castsi512_ps(_mm512_or_epi32(_mm512_castps_si512(vx0), vsign_mask));
    const __m512 vz1 = _mm512_castsi512_ps(_mm512_or_epi32(_mm512_castps_si512(vx1), vsign_mask));

    __m512 vn0 = _mm512_fmadd_ps(vz0, vlog2e, vmagic_bias);
    __m512 vn1 = _mm512_fmadd_ps(vz1, vlog2e, vmagic_bias);

    const __m512 vl0 = _mm512_permutex2var_ps(vtable_lo, _mm512_castps_si512(vn0), vtable_hi);
    const __m512 vl1 = _mm512_permutex2var_ps(vtable_lo, _mm512_castps_si512(vn1), vtable_hi);

    vn0 = _mm512_sub_ps(vn0, vmagic_bias);
    vn1 = _mm512_sub_ps(vn1, vmagic_bias);

    __m512 vt0 = _mm512_fmadd_ps(vn0, vminus_ln2_hi, vz0);
    __m512 vt1 = _mm512_fmadd_ps(vn1, vminus_ln2_hi, vz1);

    vt0 = _mm512_fmadd_ps(vn0, vminus_ln2_lo, vt0);
    vt1 = _mm512_fmadd_ps(vn1, vminus_ln2_lo, vt1);

    __m512 vp0 = _mm512_fmadd_ps(vt0, vc2, vc1);
    __m512 vp1 = _mm512_fmadd_ps(vt1, vc2, vc1);

    vt0 = _mm512_mul_ps(vt0, vl0);
    vt1 = _mm512_mul_ps(vt1, vl1);

    vp0 = _mm512_fmadd_ps(vt0, vp0, vl0);
    vp1 = _mm512_fmadd_ps(vt1, vp1, vl1);

    const __m512 ve0 = _mm512_scalef_ps(vp0, vn0);
    const __m512 ve1 = _mm512_scalef_ps(vp1, vn1);

    const __m512 vd0 = _mm512_add_ps(ve0, vone);
    const __m512 vd1 = _mm512_add_ps(ve1, vone);

    __m512 vf0 = _mm512_div_ps(ve0, vd0);
    __m512 vf1 = _mm512_div_ps(ve1, vd1);

    vf0 = _mm512_mask_sub_ps(vf0, _mm512_movepi32_mask(_mm512_castps_si512(vx0)), vone, vf0);
    vf1 = _mm512_mask_sub_ps(vf1, _mm512_movepi32_mask(_mm512_castps_si512(vx1)), vone, vf1);

    _mm512_storeu_ps(y, vf0);
    _mm512_storeu_ps(y + 16, vf1);
    y += 32;
  }
  for (; n >= 16 * sizeof(float); n -= 16 * sizeof(float)) {
    const __m512 vx = _mm512_loadu_ps(x);
    x += 16;

    const __m512 vz = _mm512_castsi512_ps(_mm512_or_epi32(_mm512_castps_si512(vx), vsign_mask));

    __m512 vn = _mm512_fmadd_ps(vz, vlog2e, vmagic_bias);
    const __m512 vl = _mm512_permutex2var_ps(vtable_lo, _mm512_castps_si512(vn), vtable_hi);
    vn = _mm512_sub_ps(vn, vmagic_bias);

    __m512 vt = _mm512_fmadd_ps(vn, vminus_ln2_hi, vz);
    vt = _mm512_fmadd_ps(vn, vminus_ln2_lo, vt);

    __m512 vp = _mm512_fmadd_ps(vt, vc2, vc1);
    vt = _mm512_mul_ps(vt, vl);
    vp = _mm512_fmadd_ps(vt, vp, vl);

    const __m512 ve = _mm512_scalef_ps(vp, vn);
    const __m512 vd = _mm512_add_ps(ve, vone);

    __m512 vf = _mm512_div_ps(ve, vd);

    vf = _mm512_mask_sub_ps(vf, _mm512_movepi32_mask(_mm512_castps_si512(vx)), vone, vf);

    _mm512_storeu_ps(y, vf);
    y += 16;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 15 * sizeof(float));

    // Prepare mask for valid 32-bit elements (depends on n).
    n >>= 2 /* log2(sizeof(float)) */;
    const __mmask16 vmask = _cvtu32_mask16((uint16_t) ((uint32_t) (UINT32_C(1) << n) - UINT32_C(1)));

    const __m512 vx = _mm512_maskz_loadu_ps(vmask, x);
    const __m512 vz = _mm512_castsi512_ps(_mm512_or_epi32(_mm512_castps_si512(vx), vsign_mask));

    __m512 vn = _mm512_fmadd_ps(vz, vlog2e, vmagic_bias);
    const __m512 vl = _mm512_permutex2var_ps(vtable_lo, _mm512_castps_si512(vn), vtable_hi);
    vn = _mm512_sub_ps(vn, vmagic_bias);

    __m512 vt = _mm512_fmadd_ps(vn, vminus_ln2_hi, vz);
    vt = _mm512_fmadd_ps(vn, vminus_ln2_lo, vt);

    __m512 vp = _mm512_fmadd_ps(vt, vc2, vc1);
    vt = _mm512_mul_ps(vt, vl);
    vp = _mm512_fmadd_ps(vt, vp, vl);

    const __m512 ve = _mm512_scalef_ps(vp, vn);
    const __m512 vd = _mm512_add_ps(ve, vone);

    __m512 vf = _mm512_div_ps(ve, vd);

    vf = _mm512_mask_sub_ps(vf, _mm512_movepi32_mask(_mm512_castps_si512(vx)), vone, vf);

    _mm512_mask_storeu_ps(y, vmask, vf);
  }
}
