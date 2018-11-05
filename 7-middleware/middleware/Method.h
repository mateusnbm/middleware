//
// Method.h
//

#import <stdio.h>
#import <stdlib.h>
#import "CallStack.h"

using namespace std;

class Method {

    private:

    public:

        virtual CallStack execute(CallStack inputs) = 0;

};
