#include <stdlib.h>
#include "value.h"

#ifndef _PRIMITIVES
#define _PRIMITIVES


Value *primitivePlus(Value *args);

Value *primitiveNull(Value *args);

Value *primitiveCar(Value *args);

Value *primitiveCdr(Value *args);

Value *primitiveCons(Value *args);

Value *primitiveMult(Value *args);

Value *primitiveMinus(Value *args);

Value *primitiveDivide(Value *args);

Value *primitiveEqual(Value *args);

Value *primitiveModulo(Value *args);

Value *primitiveLess(Value *args);

Value *primitiveGreater(Value *args);

#endif
