//
// Lower.h
//

#import <stdio.h>
#import <stdlib.h>
#import "../middleware/Method.h"
#import "../middleware/CallStack.h"

using namespace std;

class Lower : public Method {

    private:

    public:

        CallStack execute(CallStack inputs);

};
