//
// Request.cpp
//

#import "Request.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

Request::Request(char * data, int len) {

    this->len = len;
    this->data = string(data);

}

Request::Request(char * descriptor) {

    int i = 0;
    int k = 0;
    int n = 0;

    vector <int> lengths;
    vector <string> values;

    for (i = 1; i > 0; i++) {

        if (descriptor[i] == '>') {

            k = i+1;
            i = -1;
            lengths.push_back(n);

        } else if (descriptor[i] == ',') {

            lengths.push_back(n);
            n = 0;

        } else {

            n = (n*10)+(descriptor[i]-'0');

        }

    }

    for (i = 0; i < lengths.size(); i++) {

        unsigned int len = lengths.at(i)+1;
        char * buffer = (char *) malloc(len * sizeof(char));

        memcpy(buffer, (descriptor+k), len);
        buffer[len-1] = '\0';
        k += len;

        this->len = len;
        this->data = string(buffer);

        free(buffer);

    }

}

Request::~Request() {

    // ...

}

void Request::serialize(char ** buf, int * len) {

    string sizes = "<" + to_string(this->len) + ">";
    string data = sizes + this->data;

    unsigned int length = data.length()+1;
    const char * tmp = data.c_str();
    char * buffer = (char *) malloc(length*sizeof(char));

    strcpy(buffer, tmp);
    buffer[length-1] = '\0';

    *buf = buffer;
    *len = length;

}

char * Request::serialize() {

    char * buffer;
    int length;

    this->serialize(&buffer, &length);

    return buffer;

}

char * Request::serialize(int * len) {

    char * buffer;

    this->serialize(&buffer, len);

    return buffer;

}

char * Request::getData() {

    string item = this->data;

    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));

    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    return buffer;

}
