#ifndef CVTABLE_VTABLE_FOR_C
#define CVTABLE_VTABLE_FOR_C

#include <assert.h>

#define force_cast(type, ptr) ((type) (ptr))

#define DEF_STRUCT(name)                                                                   \
	typedef struct name name;                                                              \
	struct name

#define DECL_VTABLE(name, decls)                                                           \
	typedef struct VTABLE_##name VTABLE_##name;                                            \
	struct VTABLE_##name {                                                                 \
		decls;                                                                             \
		virtual(__deinit);                                                                 \
	}
#define VTABLE_TYPE(name) VTABLE_##name
#define SET_VTABLE_MEMBER(name) VTABLE_TYPE(name) * vtable

#define DECL_BASE(name, members)                                                           \
	typedef struct name name;                                                              \
	struct name {                                                                          \
		SET_VTABLE_MEMBER(name);                                                           \
		members;                                                                           \
	};
#define DECL_BASE_CONSTANT(name, members)                                                  \
	typedef struct name name;                                                              \
	struct name {                                                                          \
		const SET_VTABLE_MEMBER(name);                                                     \
		members;                                                                           \
	};

#define virtual(f) void (*f)(void*)
#define virtual_args(f, ...) void (*f)(void*, __VA_ARGS__)

#define PURE_VIRTUAL_F___deinit // in order to ban __deinit as a pure virtual function
#define PURE_VIRTUAL_F___deinit_dru

#define PURE_VIRTUAL(f)                                                                    \
	void PURE_VIRTUAL_F_##f(void* this) {}                                                 \
	void PURE_VIRTUAL_F_##f##_dru(void*)
#define PURE_VIRTUAL_ARGS(f, ...)                                                          \
	void PURE_VIRTUAL_F_##f(void* this, __VA_ARGS__) {}                                    \
	void PURE_VIRTUAL_F_##f##_dru(void* this, ...)
#define PURE_VIRTUAL_FUNCTION(name) PURE_VIRTUAL_F_##name

#define INSTANT_VTABLE(type, name, ...)                                                    \
	static const type VTABLE_INSTANCE_##name = { __VA_ARGS__ }
#define VTABLE_OF(name) VTABLE_INSTANCE_##name

#define DERIVE(name, base_t, remain)                                                       \
	typedef struct name name;                                                              \
	struct name {                                                                          \
		base_t base;                                                                       \
		remain;                                                                            \
	}

#define IMPL_METHOD(type, name, self_arg) void METHOD_##type##_IMPL_##name(void* self_arg)
#define IMPL_METHOD_ARGS(type, name, self_arg, ...)                                        \
	void METHOD_##type##_IMPL_##name(void* self_arg, __VA_ARGS__)
#define METHOD(type, name) METHOD_##type##_IMPL_##name

#define IMPL_DEINIT(type) void METHOD_##type##_IMPL___deinit(void* this)
#define DEINIT_OF(type) METHOD_##type##_IMPL___deinit
#define SET_DEINIT(type) .__deinit = DEINIT_OF(type)

#define INVOKE(type, name)                                                                 \
	(assert(type && type->vtable && type->vtable->name), type->vtable->name(type))
#define INVOKE_ARGS(type, name, ...) type->vtable->name(type, __VA_ARGS__)
#define DROP(ptr)                                                                          \
	if (ptr && ptr->vtable->__deinit) {                                                    \
		ptr->vtable->__deinit(ptr);                                                        \
	}

#define IS_DERIVED_AS(instance, type)                                                      \
	(force_cast(void*, instance->vtable) == force_cast(void*, &VTABLE_OF(type)))
#define AS_DERIVE_TYPE(instance, type)                                                     \
	(IS_DERIVED_AS(instance, type) ? force_cast(type*, instance) : NULL)

#endif
