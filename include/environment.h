/* environment.h */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

void *environment_init(void *parent);
void environment_add(void *env, void *key, void *val);
int environment_exists(void *env, void *key);
int environment_get(void *env, void *key, void **val);
int environment_modify(void *env, void *key, void *val);
int environment_remove(void *env, void *key);
int environment_get_recurse(void *env, void *key, void **val);
int environment_exists_recurse(void *env, void *key);
int environment_modify_recurse(void *env, void *key, void *val);

#endif

