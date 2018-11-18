//
// Grayscale.cpp
//

#include "Grayscale.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

CallStack Grayscale::execute(CallStack inputs) {

    string filename = inputs.getStringAtIndex(0);
    string filepath = "tmp/" + filename + "-original.jpg";

    char * c_filepath = (char *) malloc(sizeof(char)*(filepath.size()+1));
    std::copy(filepath.begin(), filepath.end(), c_filepath);
    c_filepath[filepath.size()] = '\0';
    int c_filepath_len = filepath.size();

    int image_lenght = 0;
    char * image_data = inputs.getFileAtIndex(1, &image_lenght);
    FILE * image_file = fopen(c_filepath, "wb+");
    fwrite(image_data, sizeof(char), image_lenght, image_file);

    free(image_data);
    fclose(image_file);

    Magick::Image image(c_filepath);
    image.type(Magick::GrayscaleType);
    Blob blob;
    image.magick("JPEG");
    image.write(&blob);

    CallStack stack = CallStack();
    stack.addString(blob.base64());

    free(c_filepath);

    return stack;

}
