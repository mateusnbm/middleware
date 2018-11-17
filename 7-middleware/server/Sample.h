//
// Sample.h
//

#import <stdio.h>
#import <stdlib.h>
#import "../middleware/Method.h"
#import "../middleware/CallStack.h"

using namespace std;

class Sample : public Method {

    private:

    public:

        CallStack execute(CallStack inputs);

};
