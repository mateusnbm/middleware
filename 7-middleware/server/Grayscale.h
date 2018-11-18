//
// Grayscale.h
//

#import <stdio.h>
#import <stdlib.h>
#import "../middleware/Method.h"
#import "../middleware/CallStack.h"
#include <Magick++.h>

using namespace std;
using namespace Magick;

class Grayscale : public Method {

    private:

    public:

        CallStack execute(CallStack inputs);

};
