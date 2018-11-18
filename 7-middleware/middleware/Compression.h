//
// Compression.h
//

#import <stdio.h>
#import <stdlib.h>
#import <vector>
#import <iostream>
#import <string>
#import "zlib.h"

using namespace std;

char * compress_data(const char data[], int length, int * final_length);
char * compress_data(char * data, int length, int * final_length);

char * decompress_data(char * data, int length, int * final_length);
