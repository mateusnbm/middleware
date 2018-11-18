//
// Lower.cpp
//

#include "Sample.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

CallStack Sample::execute(CallStack inputs) {

    char * param1 = inputs.getCharsAtIndex(0);
    printf("[SERVER] Sample procedure call, Parameter 1:\n\n%s\n\n", param1);
    free(param1);

    char * param2 = inputs.getCharsAtIndex(1);
    printf("[SERVER] Sample procedure call, Parameter 2:\n\n%s\n\n", param2);
    free(param2);

    string param3 = inputs.getStringAtIndex(2);
    cout << "[SERVER] Sample procedure call, Parameter 3:\n\n" << param3 << "\n\n";

    int param4 = inputs.getIntAtIndex(3);
    printf("[SERVER] Sample procedure call, Parameter 4:\n\n%i\n\n", param4);

    float param5 = inputs.getFloatAtIndex(4);
    printf("[SERVER] Sample procedure call, Parameter 5:\n\n%f\n\n", param5);

    bool param6 = inputs.getBooleanAtIndex(5);
    printf("[SERVER] Sample procedure call, Parameter 6:\n\n%s\n\n", (param6 == true ? "true" : "false"));

    vector <int> ints_ret = inputs.getIntsVectorAtIndex(6);
    printf("[SERVER] Sample procedure call, Parameter 7:\n\n%i %i %i %i %i\n\n", ints_ret[0], ints_ret[1], ints_ret[2], ints_ret[3], ints_ret[4]);

    vector <float> floats_ret = inputs.getFloatsVectorAtIndex(7);
    printf("[SERVER] Sample procedure call, Parameter 8:\n\n%.2f %.2f %.2f %.2f %.2f\n\n", floats_ret[0], floats_ret[1], floats_ret[2], floats_ret[3], floats_ret[4]);

    int image_lenght = 0;
    char * image_data = inputs.getFileAtIndex(8, &image_lenght);
    FILE * image_file = fopen("tmp/apple.jpeg", "wb+");
    fwrite(image_data, sizeof(char), image_lenght, image_file);
    fclose(image_file);
    free(image_data);
    printf("[SERVER] Image saved to file at path \"tmp/apple.jpeg\".\n\n");

    CallStack stack = CallStack();
    stack.addConstChars("OK");

    return stack;

}
