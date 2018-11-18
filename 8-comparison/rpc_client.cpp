//
// rpc_client.cpp
//

#include <cstdlib>
#include <string>
#include <iostream>

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#include "SHA256.h"
#include "Blowfish.h"
#include "Compression.h"

#define ENCRYPT 0
#define COMPRESS 1

using namespace std;

//
// Base64.
//

typedef char BYTE;
string base64_encode(BYTE const * buf, unsigned int bufLen);
vector<BYTE> base64_decode(string const&);

//
// Main program.
//

int main(int argc, char **) {

    const int repeat_count = 10000;
    const char times_filename[] = "times/compression.txt";

    const char host[] = "127.0.0.1:7300/\0";
    unsigned int port = 7300;
    xmlrpc_c::clientSimple client;

    int count = 0;
    int file_index = 0;
    char buffer[128] = {0};
    long stime, etime, delta;
    struct timespec time_spec;
    clockid_t clk_id = CLOCK_MONOTONIC;

    FILE * times_file = fopen(times_filename, "w+");

    for (int i = 0; i < repeat_count; i++) {

        FILE * image_file = fopen(((i % 2 == 0) ? "apple.jpg" : "google.jpg"), "rb");
        fseek(image_file, 0, SEEK_END);
        unsigned long image_data_len = ftell(image_file);
        fseek(image_file, 0, SEEK_SET);
        char * image_data = (char *) malloc(sizeof(char)*(image_data_len+1));
        image_data[image_data_len] = '\0';
        fread(image_data, sizeof(char), image_data_len, image_file);
        fclose(image_file);

        string image_base64 = base64_encode(image_data, image_data_len);

        free(image_data);

        #if ENCRYPT

            string hash_a = sha256("secret-password");
            BLOWFISH bf_a(hash_a);
            string de_payload = image_base64;
            image_base64 = bf_a.Encrypt_CBC(de_payload);

        #endif

        #if COMPRESS

            char * c_image_base64 = (char *) malloc(sizeof(char)*(image_base64.size()+1));
            std::copy(image_base64.begin(), image_base64.end(), c_image_base64);
            c_image_base64[image_base64.size()] = '\0';
            int c_image_base64_len = image_base64.size();

            int foo_len_a = 0;
            char * foo_a = compress_data(c_image_base64, c_image_base64_len, &foo_len_a);
            image_base64 = base64_encode(foo_a, foo_len_a);

            free(c_image_base64);
            free(foo_a);

        #endif

        xmlrpc_c::value result2;
        string foo = image_base64;

        char * str_call = (char *) malloc(sizeof(char)*(foo.size()+1));
        std::copy(foo.begin(), foo.end(), str_call);
        str_call[foo.size()] = '\0';
        int str_call_len = foo.size();

        // ...

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        stime = time_spec.tv_nsec;

        client.call(host, "sample.toupper", "is", &result2, (file_index++), str_call);

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        etime = time_spec.tv_nsec;
        delta = (etime - stime) / 1000;

        count = sprintf(buffer, "%ld,", delta);
        count = fwrite(buffer, sizeof(char), count, times_file);

        // ...

        string received_data_base64 = xmlrpc_c::value_string(result2);

        #if COMPRESS

            vector <char> bytes = base64_decode(received_data_base64);
            char * data = (char *) malloc(sizeof(char)*(bytes.size()+1));
            for (int i = 0; i < bytes.size(); i++) { data[i] = bytes[i]; }
            data[bytes.size()] = '\0';
            int data_len = bytes.size();

            int foo_len = 0;
            char * foo_b = decompress_data(data, data_len, &foo_len);
            received_data_base64 = string(foo_b);

            free(data);
            free(foo_b);

        #endif

        #if ENCRYPT

            string hash_b = sha256("secret-password");
            BLOWFISH bf_b(hash_b);
            string en_payload = string(received_data_base64);
            received_data_base64 = bf_b.Decrypt_CBC(en_payload);

        #endif

        // ...

        string filepath = "tmp/" + to_string(file_index) + "-grayscale.jpg";

        char * c_filepath = (char *) malloc(sizeof(char)*(filepath.size()+1));
        std::copy(filepath.begin(), filepath.end(), c_filepath);
        c_filepath[filepath.size()] = '\0';
        int c_filepath_len = filepath.size();

        vector <char> received_bytes = base64_decode(received_data_base64);
        char * received_data = (char *) malloc(sizeof(char)*(received_bytes.size()+1));
        for (int i = 0; i < received_bytes.size(); i++) { received_data[i] = received_bytes[i]; }
        received_data[received_bytes.size()] = '\0';
        int received_data_len = received_bytes.size();

        FILE * out = fopen(c_filepath, "wb+");
        fwrite(received_data, sizeof(char), received_data_len, out);
        fclose(out);

        // ...

        free(str_call);

    }

    fclose(times_file);

    return 0;

}

//
// Base64.
//

static const string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(BYTE const* buf, unsigned int bufLen) {
  string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}

vector<BYTE> base64_decode(string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  BYTE char_array_4[4], char_array_3[3];
  vector<BYTE> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}
