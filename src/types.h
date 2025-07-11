/**
 * @file types.h
 * @author Enzo Solis (elenzosistemas@gmail.com)
 * @brief Common type definitions and inline utility functions.
 * @version 0.1
 * @date 2025-07-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef void *PVOID;
typedef uint64_t U64, UINT64;
typedef uint32_t U32, UINT;
typedef uint16_t U16, UINT16;
typedef uint8_t  U8, BYTE, *PBYTE;
typedef int8_t   I8, INT8;
typedef int16_t I16, INT16;
typedef int32_t I32, INT;
typedef int64_t I64, INT64;
typedef float  FLOAT;
typedef double DOUBLE;

#define STRINGIFY(x) #x

#define BS_FAILED -1
#define BS_OK 0

#ifndef BS_INLINE
#define BS_INLINE static inline
#endif

#define BS_MEMCPY(dest, src, size)  memcpy(dest, src, size)
#define BS_MEMSET(ptr, value, size) memset(ptr, value, size)
#define BS_ZEROMEMORY(ptr, size) BS_MEMSET(ptr, 0, size)

#if defined (__clang__)
#define MAYBE_UNUSED __attribute__((unused))
#elif defined (__GNUC__)
#define MAYBE_UNUSED __attribute__((unused))
#elif define (_MSC_VER)
#define MAYBE_UNUSED 
#endif

/**
 * @def BS_SWAP_BITS
 * @brief Intercambia valores entre dos variables.
 * 
 * @param a primera variable.
 * @param b segunda variable.
 * 
 * @warning las variables deben ser del mismo tipo y no pueden ser expresiones.
 */
#define BS_SWAP_BITS(a, b) a^=b;b^=a;a^=b

#define CAST_TO_THIS(type_object, ptr) type_object this = (type_object)ptr

/**
 * @fn bs_memswap
 * @brief Intercambio de bits entre dos arreglos de bytes de N bytes.
 * 
 * @param ptr1 puntero a una secuencia de bytes.
 * @param ptr2 puntero a otra secuencia de bytes.
 * @param bytes longitud de las secuencias ptr1 y ptr2.
 * @return BS_INLINE 
 */
BS_INLINE void bs_memswap(U8* ptr1, U8* ptr2, size_t bytes) {
	for (size_t i = 0; i < bytes; i++) {
		BS_SWAP_BITS(ptr1[i], ptr2[i]);
	}
}

/**
 * @fn bs_next_power2.
 * @brief Obtiene el siguiente número potencia de 2.
 *
 * @see https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
 * @param num
 * @return 
 */
BS_INLINE U32 bs_next_power2(U32 num) {
  if (num == 0)
    return 1;

  --num;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;
  return ++num;
}

#endif//__TYPES_H__