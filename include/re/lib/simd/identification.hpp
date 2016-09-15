// Copyright (c) 2016 Roman Beránek. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#if defined(__x86_64__) || defined(__amd64__) \
    || defined(_M_X64) || defined(_M_AMD64)
#define RE_ARCH_X64 1
#endif

#if defined(__i386__) || defined(_M_IX86)
#define RE_ARCH_X32 1
#endif

#if defined(RE_ARCH_X32) || defined(RE_ARCH_X64)
#define RE_ARCH_X86 1
#endif

#if defined(RE_ARCH_X86)
#ifdef __SSE3__
#define RE_ARCH_SSE3 1
#endif
#ifdef __SSSE3__
#define RE_ARCH_SSSE3 1
#endif
#ifdef __SSE4_1__
#define RE_ARCH_SSE4_1 1
#endif
#ifdef __SSE4_2__
#define RE_ARCH_SSE4_2 1
#endif
#ifdef __SSE4A__
#defined RE_ARCH_SSE4A
#endif
#ifdef __AVX__
#define RE_ARCH_AVX 1
#endif
#ifdef __AVX512F__
#define RE_ARCH_AVX512 1
#endif
#endif

#if defined(__arm__) || defined(__thumb__) || defined(_M_ARM)
#defined RE_ARCH_ARM32 1
#endif
#if defined(__arm64__)  || defined(__aarch64__)
#define RE_ARCH_ARM64
#endif
#if defined(RE_ARCH_ARM32) || defined(RE_ARCH_ARM64)
#define RE_ARCH_ARM 1
#endif
#ifdef RE_ARCH_ARM
#ifdef __ARM_NEON__
#define RE_ARCH_NEON 1
#endif
#endif
