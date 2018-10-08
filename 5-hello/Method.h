//
// Invoker.h
//

#import <stdio.h>
#import <stdlib.h>
#import "CallStack.h"

using namespace std;

class Method {

    private:

    public:

        Method();
        ~Method();

        virtual CallStack execute(CallStack inputs) = 0;

};
