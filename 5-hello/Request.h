//
// Request.h
//

#import <stdio.h>
#import <stdlib.h>
#import "CallStack.h"

using namespace std;

class Request {

    private:

        int len;
        string data;

    public:

        Request(char * data, int len);
        Request(char * descriptor);
        ~Request();

        void serialize(char ** buf, int * len);
        char * serialize();
        char * serialize(int * len);
        char * getData();

};
