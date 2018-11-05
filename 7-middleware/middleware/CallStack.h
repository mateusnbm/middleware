//
// CallStack.h
//

#import <stdio.h>
#import <stdlib.h>
#import <vector>
#import <iostream>

using namespace std;

class CallStack {

    private:

        vector <string> stack;

    public:

        CallStack();
        CallStack(char * reps);
        ~CallStack();

        void addChars(char * value);
        void addConstChars(const char value[]);
        void addString(string value);
        void addInteger(int value);
        char * getItemAtIndex(int index);
        char * serialize();

};
