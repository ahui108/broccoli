#include "helloworld.h"

int main(int argc, char **argv) 
{
    int ret = 0;
    int buffer_width = 3072; 
    int buffer_height = 3072;
    unsigned short * data = NULL;
    unsigned short * out = NULL;
    
    debug("allocating...");
    data = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(data != NULL, -1);
    memset(data, 0, buffer_width * buffer_height * sizeof(unsigned short));
    out = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(out != NULL, -1);
    memset(out, 0, buffer_width * buffer_height * sizeof(unsigned short));
    debug("allocate done");
    
    debug("Read Image");
    ret = read_image("./input.raw", buffer_width, buffer_height, data);
    assert_and_return(ret == 0, -1);
    debug("Done");
    
    debug("CPU Computing");
    for (int i = 0; i < buffer_width; i++) {
        for (int j = 0; j < buffer_height; j++) {
            out[i * buffer_width + j] = data[i * buffer_width + j] - data[i * buffer_width + j] + 400;
        }
    }
    debug("Done");
    
    debug("Save Image");
    ret = save_image(out, buffer_width, buffer_height, "./out.raw");
    if (ret != 0) {
        debug("write image failed");
    }
    debug("Done");
    
    free(data);
    free(out);
}