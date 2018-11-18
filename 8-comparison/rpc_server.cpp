//
// rpc_server.cpp
//

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#include <Magick++.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include "SHA256.h"
#include "Blowfish.h"
#include "Compression.h"

#define ENCRYPT 0
#define COMPRESS 1

using namespace std;
using namespace Magick;

//
// Base64.
//

typedef char BYTE;
string base64_encode(BYTE const * buf, unsigned int bufLen);
vector<BYTE> base64_decode(string const&);

//
// Main program.
//

class sampleUpperMethod : public xmlrpc_c::method {

    public:

        sampleUpperMethod() {

            this->_signature = "s:is";
            this->_help = "This method returns a uppercase string.";

        }

        void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const  retvalP) {

            string data_base64 = paramList.getString(1);

            int file_index = paramList.getInt(0);
            string filepath = "tmp/" + to_string(file_index) + "-original.jpg";

            char * c_filepath = (char *) malloc(sizeof(char)*(filepath.size()+1));
            std::copy(filepath.begin(), filepath.end(), c_filepath);
            c_filepath[filepath.size()] = '\0';
            int c_filepath_len = filepath.size();

            #if COMPRESS

                vector <char> bytes_a = base64_decode(data_base64);
                char * data_a = (char *) malloc(sizeof(char)*(bytes_a.size()+1));
                for (int i = 0; i < bytes_a.size(); i++) { data_a[i] = bytes_a[i]; }
                data_a[bytes_a.size()] = '\0';
                int data_a_len = bytes_a.size();

                int foo_len_a = 0;
                char * foo_a = decompress_data(data_a, data_a_len, &foo_len_a);
                data_base64 = string(foo_a);

                free(data_a);
                free(foo_a);

            #endif

            #if ENCRYPT

                string hash_a = sha256("secret-password");
                BLOWFISH bf_a(hash_a);
                string en_payload = string(data_base64);
                data_base64 = bf_a.Decrypt_CBC(en_payload);

            #endif

            string item = data_base64;
            vector <char> bytes = base64_decode(item);
            char * data = (char *) malloc(sizeof(char)*(bytes.size()+1));
            for (int i = 0; i < bytes.size(); i++) { data[i] = bytes[i]; }
            data[bytes.size()] = '\0';
            int data_len = bytes.size();

            FILE * out = fopen(c_filepath, "wb+");
            fwrite(data, sizeof(char), data_len, out);
            fclose(out);

            // ...

            Magick::Image image(c_filepath);
            image.type(Magick::GrayscaleType);
            Blob blob;
            image.magick("JPEG");
            image.write(&blob);
            string out_image_base64 = blob.base64();

            // ...

            #if ENCRYPT

                string hash_b = sha256("secret-password");
                BLOWFISH bf_b(hash_b);
                string de_payload = string(out_image_base64);
                out_image_base64 = bf_b.Encrypt_CBC(de_payload);

            #endif

            #if COMPRESS

                char * c_out_image_base64 = (char *) malloc(sizeof(char)*(out_image_base64.size()+1));
                std::copy(out_image_base64.begin(), out_image_base64.end(), c_out_image_base64);
                c_out_image_base64[out_image_base64.size()] = '\0';
                int c_out_image_base64_len = out_image_base64.size();

                int foo_len = 0;
                char * foo_b = compress_data(c_out_image_base64, c_out_image_base64_len, &foo_len);
                out_image_base64 = base64_encode(foo_b, foo_len);

                free(c_out_image_base64);
                free(foo_b);

            #endif

            // ...

            string foo = out_image_base64;
            *retvalP = xmlrpc_c::value_string(foo);

            free(c_filepath);

        }

};

int main(int const, const char ** const) {

    char buffer[1024] = {0};

    const char host[] = "127.0.0.1\0";
    unsigned int port = 7300;

    xmlrpc_c::registry registry;
    registry.addMethod("sample.toupper", (new sampleUpperMethod));

    xmlrpc_c::serverAbyss server = xmlrpc_c::serverAbyss(
        xmlrpc_c::serverAbyss::constrOpt()
        .registryP(&registry)
        .portNumber(port)
        .uriPath("/"));

    server.run();

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
