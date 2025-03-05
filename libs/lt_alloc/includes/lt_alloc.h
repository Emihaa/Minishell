/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:59:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/05 00:17:13 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LT_ALLOC
# define LT_ALLOC

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#ifndef LT_INT_TYPES
#define LT_INT_TYPES

typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t		t_u8;

typedef int64_t		t_s64;
typedef int32_t		t_s32;
typedef int16_t		t_s16;
typedef int8_t		t_s8;

#endif

#define DEFAULT_ARENA_CAPACITY (1024 * 256)

typedef struct s_arena
{
	t_u64	capacity;
	t_u64	size;
	t_u8	*data;
} t_arena;

typedef struct s_arena_temp
{
	t_arena *arena;
	t_u64	pos;
} t_arena_temp;

t_arena	arena_new(t_u64 cap);
void	arena_delete(t_arena *a);

void	*arena_alloc(t_arena *a, t_u64 size);
int		arena_unalloc(t_arena *a, t_u64 size);
void	arena_realloc(t_arena *a, t_u64 size); // maybe rename arena_add or something similar?
void	arena_reset(t_arena *a);

t_arena_temp arena_temp_begin(t_arena *a);
void arena_temp_end(t_arena_temp *a);

#endif


