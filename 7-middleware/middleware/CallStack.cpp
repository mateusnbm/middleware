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

void CallStack::addFloat(float value) {

    this->stack.push_back(to_string(value));

}

void CallStack::addBoolean(bool value) {

    this->stack.push_back(string(value == true ? "1" : "0"));

}

void CallStack::addVector(vector <int> & value) {

    string arr = string("");

    for (int i = 0; i < value.size(); i++) {

        arr = arr + to_string(value[i]) + "-";

    }

    this->stack.push_back(arr);

}

void CallStack::addVector(vector <float> & value) {

    string arr = string("");

    for (int i = 0; i < value.size(); i++) {

        arr = arr + to_string(value[i]) + "-";

    }

    this->stack.push_back(arr);

}

void CallStack::addFile(const char path[]) {

    FILE * fp = fopen(path, "rb");
    fseek(fp, 0L, SEEK_END);
    int fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char * data = (unsigned char *) malloc(fsize*sizeof(unsigned char));
    fread(data, sizeof(unsigned char), fsize, fp);

    this->stack.push_back(base64_encode(data, fsize));

    fclose(fp);
    free(data);

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

char * CallStack::getCharsAtIndex(int index) {

    return this->getItemAtIndex(index);

}

string CallStack::getStringAtIndex(int index) {

    char * item = this->getItemAtIndex(index);
    string value = string(item);

    free(item);

    return value;

}

int CallStack::getIntAtIndex(int index) {

    char * item = this->getItemAtIndex(index);
    int value = atoi(item);

    free(item);

    return value;

}

float CallStack::getFloatAtIndex(int index) {

    char * item = this->getItemAtIndex(index);
    float value = atof(item);

    free(item);

    return value;

}

bool CallStack::getBooleanAtIndex(int index) {

    char * item = this->getItemAtIndex(index);
    bool value = item[0] == '0' ? false : true;

    free(item);

    return value;

}

vector <int> CallStack::getIntsVectorAtIndex(int index) {

    string buffer = string("");
    vector <int> value;
    char * item = this->getItemAtIndex(index);

    for (int i = 0; item[i] != '\0'; i++) {

        if (item[i] == '-') {

            value.push_back(stoi(buffer));
            buffer = string("");

        } else {

            buffer = buffer + item[i];

        }

    }

    return value;

}

vector <float> CallStack::getFloatsVectorAtIndex(int index) {

    string buffer = string("");
    vector <float> value;
    char * item = this->getItemAtIndex(index);

    for (int i = 0; item[i] != '\0'; i++) {

        if (item[i] == '-') {

            value.push_back(stof(buffer));
            buffer = string("");

        } else {

            buffer = buffer + item[i];

        }

    }

    return value;

}

char * CallStack::getFileAtIndex(int index, int * file_length) {

    string item = this->stack.at(index);
    vector <unsigned char> bytes = base64_decode(item);
    char * data = (char *) malloc(sizeof(char)*(bytes.size()+1));

    for (int i = 0; i < bytes.size(); i++) { data[i] = bytes[i]; }

    data[bytes.size()] = '\0';

    *file_length = bytes.size();

    return data;

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
