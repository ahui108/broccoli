#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <time.h> 

#define MAX_NAME_LEN 1000
#define assert_and_return(exp, ret)\
    do {\
        assert(exp);\
        if (!(exp)) {\
            fprintf(stderr, "assert(%s) failed\n", #exp);\
            return (ret);\
        }\
    } while (0)

#define assert_and_log(exp)\
    do {\
        assert(exp);\
        if (!(exp)) {\
            fprintf(stderr, "assert(%s) failed\n", #exp);\
        }\
    } while (0)

void debug(char *msg)
{   
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    /*
    time_t now;
    time(&now);
    struct tm *timenow;
    */
    
    int elapsed = 0;
    struct tm *timenow;
    #if 0
    if (cycle) {
        struct tm * time_elapsed;
        time_elapsed = localtime(&tv.tv_sec);
        elapsed = time_elassed.tv_usec - timenow.tv_usec;
        timenow = time_elapsed;
    } else {
        timenow = localtime(&tv.tv_sec);
    }
    #endif
    
    timenow = localtime(&tv.tv_sec);
    //printf("time_now:%d/%d/%d:%d:%d:%d.%ld\n", /*1900 + timenow->tm_year, 1 + timenow->tm_mon, timenow->tm_mday, */timenow->tm_hour, timenow->tm_min, timenow->tm_sec, tv.tv_usec); 
    printf("%02d:%02d:%02d.%06ld", /*1900 + timenow->tm_year, 1 + timenow->tm_mon, timenow->tm_mday, */timenow->tm_hour, timenow->tm_min, timenow->tm_sec, tv.tv_usec); 
    if (msg != NULL) {
        printf(" - %s\n", msg);
    } else {
        printf("\n");
    }
    if (elapsed > 0) {
        printf("elapsed:%06ld\n", elapsed);
    }
}

int read_image(char *filename, int width, int height, unsigned short *buffer)
{
    debug("enter read image");
    int ret = -1;
    FILE *file = NULL;
    if (filename != NULL && buffer != NULL) {
        file = fopen(filename, "rb");
        assert_and_return(file != NULL, -1);
        int count = fread(buffer, width * height * sizeof(unsigned short), 1, file);
        if (count != 1) {
            printf("read failed!\n");
        } else {
            ret = 0;
        }
        fclose(file);
    }
    debug("read done");
    
    return ret;
}

int save_image(unsigned short *buffer, int width, int height, char *image_path)
{
    debug("enter save image");
    int ret = 0;
    FILE *file = NULL;
    file = fopen(image_path, "wb");
    assert_and_return(file != NULL, -1);
    int count = fwrite(buffer, width * height * sizeof(unsigned short), 1, file);
    if (count != 1) {
        printf("write image failed\n");
    }
    fclose(file);
    debug("save done");
    return ret;
    
}