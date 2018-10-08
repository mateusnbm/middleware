//
// Invoker.h
//

#import <stdio.h>
#import <stdlib.h>
#import <map>
#import <string>
#import "Method.h"
#import "CallStack.h"

using namespace std;

class Invoker {

    private:

        map <string, Method *> methods;

    public:

        Invoker();
        ~Invoker();

        void registerMethod(const char name[], Method * method);
        CallStack invoke(char * name, CallStack inputs);

};
