/*
 * Copyright (C) 2014  absurdworlds
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Deadalus_Typedefs
#define Deadalus_Typedefs
#include <cstddef>
#include <cstdint>
#include <memory>
#include <experimental/optional>

namespace daedalus {
// Shorthands for integer types
typedef uint8_t        u8;
typedef  int8_t        i8;

typedef uint16_t       u16;
typedef  int16_t       i16;

typedef uint32_t       u32;
typedef  int32_t       i32;

typedef uint64_t       u64;
typedef  int64_t       i64;

typedef ptrdiff_t      diff_t;

//        ... for floating point types
typedef float          f32;
typedef double         f64;

//! Shorthand for std::unique_ptr template
template <typename T>
using uptr = std::unique_ptr<T>;

//! Shorthand for std::experimental::optional template
template <typename T>
using opt = std::experimental::optional<T>;

using nullopt_t = std::experimental::nullopt_t;

constexpr nullopt_t nullopt = std::experimental::nullopt;
} // namespace daedalus
#endif //Deadalus_Typedefs
