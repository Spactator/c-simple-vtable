# c-simple-vtable
A very simple vtable implement for c99

Example
```c
#include <stdio.h>
#include <stdlib.h>

#include "../cvtable.h"

// declare the vtable
DECL_VTABLE(Animal, virtual(speak));

// declare the base, name should as the same as the vtable's
DECL_BASE(Animal, int age; char* name);

PURE_VIRTUAL(speak);
// __deinit method has to be implemented explicitly
IMPL_DEINIT(Animal) {
	// "this" as an argument was declared by the macro
	free(this);
}
// vtable should be instant globlely
INSTANT_VTABLE(VTABLE_TYPE(Animal), Animal, PURE_VIRTUAL_FUNCTION(speak),
			   DEINIT_OF(Animal));

// this will use an Animal object as the first member (Animal base;)
DERIVE(Dog, Animal, int bone_count);

IMPL_METHOD(Dog, speak, base_this) {
	// the third argument is "base_this" in this excample
	// it is void*, so you have to cast it to the real type
	Dog* this = (Dog*) base_this;
	printf("Dog %s says woof! I have %d bones, and it is %d years old!\n",
		   this->base.name,
		   this->bone_count,
		   this->base.age);
}

IMPL_DEINIT(Dog) {
	free(this);
}

// you can instant the vtable by using member's names too
INSTANT_VTABLE(VTABLE_TYPE(Animal), Dog, .speak = METHOD(Dog, speak),
			   .__deinit = DEINIT_OF(Dog));
// .__deinit = DEINIT_OF(Dog) can also be written as SET_DEINIT(Dog)
#if 0
INSTANT_VTABLE(VTABLE_TYPE(Animal), Dog, .speak = METHOD(Dog, speak), SET_DEINIT(Dog));
#endif

// base class constructor
Animal* new_animal(char* name, int age) {
	Animal* animal = (Animal*) malloc(sizeof(Animal));
	animal->vtable = &VTABLE_OF(Animal);
	animal->name = name;
	animal->age = age;
	return animal;
}

// constructor needed to be created manually
Dog* new_Dog(char* name, int age, int bone_count) {
	Dog* this = (Dog*) malloc(sizeof(Dog));
	this->base.vtable = &VTABLE_OF(Dog);
	this->base.name = name;
	this->base.age = age;
	this->bone_count = bone_count;
	return this;
}

int main() {
	Animal* animals[3];
	animals[0] = (Animal*) new_Dog("Fatty", 5, 10);
	animals[1] = (Animal*) new_Dog("Sphynny", 3, 7);
	animals[2] = (Animal*) new_animal("Cat", 6);

	INVOKE(animals[0], speak);
	INVOKE(animals[1], speak);
	INVOKE(animals[2], speak); // this won't work because it is a pure virtual function

	// these will call the __deinit function in each struct
	DROP(animals[0]);
	DROP(animals[1]);
	DROP(animals[2]);
}
```
