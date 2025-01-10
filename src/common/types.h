#pragma once
typedef unsigned long long u64;
typedef long long s64;
typedef unsigned long ul32;
typedef long sl32;
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef double f64;
typedef float f32;
struct data { u64 first, second; };
template <typename T>
using fptr = T(*);
template <typename R, typename T, typename ...A>
using mfptr = R(T::*)(A...);