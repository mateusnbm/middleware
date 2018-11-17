//
// Sleeper.h
//

#import <cstdlib>
#import <string>
#import <iostream>
#import <vector>
#include <unistd.h>
#import <stdio.h>
#import <stdlib.h>
#import "../middleware/Method.h"
#import "../middleware/CallStack.h"

using namespace std;

class Sleeper : public Method {

    private:

    public:

        CallStack execute(CallStack inputs);

};
