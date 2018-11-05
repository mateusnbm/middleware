//
// Call.cpp
//

#import "Call.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

Call::Call(char * reps) {

    int i = 0;
    int k = 0;
    int n = 0;

    vector <int> lengths;
    vector <string> values;

    for (i = 1; i > 0; i++) {

        if (reps[i] == '>') {

            k = i+1;
            i = -1;
            lengths.push_back(n);

        } else if (reps[i] == ',') {

            lengths.push_back(n);
            n = 0;

        } else {

            n = (n*10)+(reps[i]-'0');

        }

    }

    for (i = 0; i < lengths.size(); i++) {

        unsigned int len = lengths.at(i);
        char * buffer = (char *) malloc((len+1) * sizeof(char));

        memcpy(buffer, (reps+k), len);
        buffer[len] = '\0';
        k += len;

        if (i == 0) {

            this->method = string(buffer);

        } else {

            this->callStack = CallStack(buffer);

        }

        free(buffer);

    }

}

Call::Call(const char method[], CallStack stack) {

    this->method = string(method);
    this->callStack = stack;

}

Call::~Call() {

    // ...

}

void Call::serialize(char ** buf, int * len) {

    string sizes = "";
    string data = "";
    string str_stack = string(this->callStack.serialize());

    sizes = "<";
    sizes = sizes + to_string(this->method.length());
    sizes = sizes + ",";
    sizes = sizes + to_string(str_stack.length());
    sizes = sizes + ">";

    data = sizes + this->method + str_stack;

    unsigned int length = data.length()+1;
    const char * tmp = data.c_str();
    char * buffer = (char *) malloc(length*sizeof(char));

    strcpy(buffer, tmp);
    buffer[length-1] = '\0';

    *buf = buffer;
    *len = length;

}

char * Call::serialize() {

    char * buffer;
    int length;

    this->serialize(&buffer, &length);

    return buffer;

}

char * Call::serialize(int * len) {

    char * buffer;

    this->serialize(&buffer, len);

    return buffer;

}

char * Call::getMethod() {

    string item = this->method;

    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));

    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    return buffer;

}

CallStack Call::getStack() {

    return this->callStack;

}
