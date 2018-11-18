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
    this->data = (char *) malloc(sizeof(char)*(len+1));
    this->data[len] = '\0';

    for (int i = 0; i < len; i++) {

        this->data[i] = data[i];

    }

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
        this->data = buffer;

    }

}

Request::~Request() {

    // ...

}

void Request::serialize(char ** buf, int * len) {

    string sizes = "<" + to_string(this->len) + ">";
    int new_len = this->len + sizes.size();
    char * buffer = (char *) malloc(sizeof(char)*(new_len+1));

    buffer[new_len] = '\0';

    for (int i = 0; i < sizes.size(); i++) {

        buffer[i] = sizes.c_str()[i];

    }

    for (int i = sizes.size(); i < new_len; i++) {

        buffer[i] = this->data[i-sizes.size()];

    }

    *buf = buffer;
    *len = new_len;

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

    char * buffer = (char *) malloc(sizeof(char)*(this->len+1));
    buffer[this->len] = '\0';

    for (int i = 0; i < this->len; i++) {

        buffer[i] = this->data[i];

    }

    return buffer;

}
