//
// Upper.cpp
//

#include "Upper.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

CallStack Upper::execute(CallStack inputs) {

    char * v = inputs.getItemAtIndex(0);
    string v_str = string(v);
    transform(v_str.begin(), v_str.end(), v_str.begin(), ::toupper);

    CallStack stack = CallStack();
    stack.addString(v_str);

    free(v);

    return stack;

}
