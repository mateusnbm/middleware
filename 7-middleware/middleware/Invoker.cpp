//
// Invoker.cpp
//

#include "Invoker.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

Invoker::Invoker() {

    // ...

}

Invoker::~Invoker() {

    // ...

}

void Invoker::registerMethod(const char name[], Method * method) {

    this->methods[name] = method;

}

CallStack Invoker::invoke(char * name, CallStack inputs) {

    Method * method = this->methods[name];

    return method->execute(inputs);

}
