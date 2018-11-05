//
// Call.h
//

#import <stdio.h>
#import <stdlib.h>
#import "CallStack.h"

using namespace std;

class Call {

    private:

        string method;
        CallStack callStack;

    public:

        Call(char * reps);
        Call(const char method[], CallStack stack);
        ~Call();

        void serialize(char ** buf, int * len);
        char * serialize();
        char * serialize(int * len);
        char * getMethod();
        CallStack getStack();

};
