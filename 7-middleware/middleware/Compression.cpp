//
// Compression.cpp
//

#import "Compression.h"

#define DEBUG 0

#pragma mark -
#pragma mark - Private

unsigned long file_size(const char filename[]) {

    FILE *pFile = fopen(filename, "rb");
    fseek (pFile, 0, SEEK_END);
    unsigned long size = ftell(pFile);
    fclose (pFile);
    return size;

}

int decompress_one_file(const char infilename[], const char outfilename[]) {

    gzFile infile = gzopen(infilename, "rb");
    FILE *outfile = fopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char buffer[128];
    int num_read = 0;

    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {

        fwrite(buffer, 1, num_read, outfile);

    }

    gzclose(infile);
    fclose(outfile);

    return 0;

}

int compress_one_file(const char infilename[], const char outfilename[]) {

    FILE *infile = fopen(infilename, "rb");
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;

    unsigned long total_read = 0, total_wrote = 0;

    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {

        total_read += num_read;

        gzwrite(outfile, inbuffer, num_read);

    }

    fclose(infile);
    gzclose(outfile);

    #if DEBUG
    printf("[Compression] Before: %ld bytes, After: %ld bytes, Factor %4.2f%%.\n\n", total_read, file_size(outfilename), (1.0-file_size(outfilename)*1.0/total_read)*100.0);
    #endif

    return 0;

}

#pragma mark -
#pragma mark - Public

char * compress_data(const char data[], int length, int * final_length) {

    srand((unsigned) time(NULL));

    char fa[] = "tmp/XXXXXXXXXX.compression.in";
    char fb[] = "tmp/XXXXXXXXXX.compression.out";

    fa[4] = fb[4] = (rand() % 10) + '0';
    fa[5] = fb[5] = (rand() % 10) + '0';
    fa[6] = fb[6] = (rand() % 10) + '0';
    fa[7] = fb[7] = (rand() % 10) + '0';
    fa[8] = fb[8] = (rand() % 10) + '0';
    fa[9] = fb[9] = (rand() % 10) + '0';
    fa[10] = fb[10] = (rand() % 10) + '0';
    fa[11] = fb[11] = (rand() % 10) + '0';
    fa[12] = fb[12] = (rand() % 10) + '0';
    fa[13] = fb[13] = (rand() % 10) + '0';

    FILE * ftmp = fopen(fa, "wb+");
    fwrite(data, sizeof(char), length, ftmp);
    fclose(ftmp);

    compress_one_file(fa, fb);

    FILE * otmp = fopen(fb, "rb+");
    int flen = file_size(fb) + 1;
    char * out = (char *) malloc(sizeof(char)*flen);
    out[flen] = '\0';
    fread(out, sizeof(char), (flen-1), otmp);
    fclose(otmp);

    *final_length = flen-1;

    return out;

}

char * compress_data(char * data, int length, int * final_length) {

    srand((unsigned) time(NULL));

    char fa[] = "tmp/XXXXXXXXXX.compression.in";
    char fb[] = "tmp/XXXXXXXXXX.compression.out";

    fa[4] = fb[4] = (rand() % 10) + '0';
    fa[5] = fb[5] = (rand() % 10) + '0';
    fa[6] = fb[6] = (rand() % 10) + '0';
    fa[7] = fb[7] = (rand() % 10) + '0';
    fa[8] = fb[8] = (rand() % 10) + '0';
    fa[9] = fb[9] = (rand() % 10) + '0';
    fa[10] = fb[10] = (rand() % 10) + '0';
    fa[11] = fb[11] = (rand() % 10) + '0';
    fa[12] = fb[12] = (rand() % 10) + '0';
    fa[13] = fb[13] = (rand() % 10) + '0';

    FILE * ftmp = fopen(fa, "wb+");
    fwrite(data, sizeof(char), length, ftmp);
    fclose(ftmp);

    compress_one_file(fa, fb);

    FILE * otmp = fopen(fb, "rb+");
    int flen = file_size(fb) + 1;
    char * out = (char *) malloc(sizeof(char)*flen);
    out[flen] = '\0';
    fread(out, sizeof(char), (flen-1), otmp);
    fclose(otmp);

    *final_length = flen-1;

    return out;

}

char * decompress_data(char * data, int length, int * final_length) {

    srand((unsigned) time(NULL));

    char fa[] = "tmp/XXXXXXXXXX.compression.in";
    char fb[] = "tmp/XXXXXXXXXX.compression.out";

    fa[4] = fb[4] = (rand() % 10) + '0';
    fa[5] = fb[5] = (rand() % 10) + '0';
    fa[6] = fb[6] = (rand() % 10) + '0';
    fa[7] = fb[7] = (rand() % 10) + '0';
    fa[8] = fb[8] = (rand() % 10) + '0';
    fa[9] = fb[9] = (rand() % 10) + '0';
    fa[10] = fb[10] = (rand() % 10) + '0';
    fa[11] = fb[11] = (rand() % 10) + '0';
    fa[12] = fb[12] = (rand() % 10) + '0';
    fa[13] = fb[13] = (rand() % 10) + '0';

    FILE * ftmp = fopen(fa, "wb+");
    fwrite(data, sizeof(char), length, ftmp);
    fclose(ftmp);

    decompress_one_file(fa, fb);

    FILE * otmp = fopen(fb, "rb+");
    int flen = file_size(fb);
    char * out = (char *) malloc(sizeof(char)*(flen+1));
    out[flen] = '\0';
    fread(out, sizeof(char), flen, otmp);
    fclose(otmp);

    *final_length = flen;

    return out;

}
