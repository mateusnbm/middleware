//
// Sleeper.cpp
//

#include "Sleeper.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

CallStack Sleeper::execute(CallStack inputs) {

    sleep(inputs.getIntAtIndex(0));

    CallStack stack = CallStack();
    stack.addConstChars("OK");

    return stack;

}
