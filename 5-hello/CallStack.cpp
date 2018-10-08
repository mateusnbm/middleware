//
// CallStack.cpp
//

#import "CallStack.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

CallStack::CallStack() {

    // ...

}

CallStack::CallStack(char * reps) {

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

        this->stack.push_back(string(buffer));

        free(buffer);

    }

}

CallStack::~CallStack() {

    // ...

}

void CallStack::addChars(char * value) {

    this->stack.push_back(string(value));

}

void CallStack::addConstChars(const char value[]) {

    this->stack.push_back(string(value));

}

void CallStack::addString(string value) {

    this->stack.push_back(value);

}

void CallStack::addInteger(int value) {

    this->stack.push_back(to_string(value));

}

char * CallStack::getItemAtIndex(int index) {

    string item = this->stack.at(index);

    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));

    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    return buffer;

}

char * CallStack::serialize() {

    int i = 0;
    string sizes = string("<");
    string values = string("");

    for (i = 0; i < this->stack.size(); i++) {

        string value = this->stack.at(i);

        sizes = sizes + (i == 0 ? "" : ",") + to_string(value.length());
        values = values + value;

    }

    sizes += ">";
    values = sizes + values;

    char * aux = (char *) malloc((values.length()+1) * sizeof(char));
    const char * temp = values.c_str();

    strcpy(aux, temp);
    aux[values.length()] = '\0';

    return aux;

}
