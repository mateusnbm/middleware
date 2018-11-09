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
        void addFloat(float value);
        void addBoolean(bool value);

        char * getItemAtIndex(int index);
        char * getCharsAtIndex(int index);
        string getStringAtIndex(int index);
        int getIntAtIndex(int index);
        float getFloatAtIndex(int index);
        bool getBooleanAtIndex(int index);

        char * serialize();

};
