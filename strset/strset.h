// Author: Piotr Borowski
// Important notice: In this project we were not allowed to create own
//					 structers or classes!

#ifndef _STRSET_H_
#define _STRSET_H_

#include <stddef.h>

#ifdef __cplusplus
namespace jnp1 {
extern "C" {
#endif // __cplusplus

unsigned long strset_new();

void strset_delete(unsigned long id);

size_t strset_size(unsigned long id);

void strset_insert(unsigned long id, const char* value);

void strset_remove(unsigned long id, const char* value);

int strset_test(unsigned long id, const char* value);

void strset_clear(unsigned long id);

int strset_comp(unsigned long id1, unsigned long id2);

#ifdef __cplusplus
}
} // namespace jnp1
#endif // __cplusplus

#endif // _STRSET_H_
