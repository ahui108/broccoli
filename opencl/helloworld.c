#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <time.h> 
#include <CL/cl.h> 
 
#define FSLCL_ERROR -1 
#define FSLCL_SUCCESS CL_SUCCESS 

cl_mem buffer_input = NULL; 
cl_mem buffer_output = NULL; 
cl_kernel buffer_kernel = NULL; 
size_t buffer_size = 0; 
int buffer_width = 3072; 
int buffer_height = 3072;
cl_platform_id   platform_id; 
cl_device_id device_id; 
cl_context context; 
cl_command_queue cq; 
cl_program program; 
cl_kernel kernel; 

struct fsl_kernel_src_str 
{ 
  char *src; 
  long size; 
}; 

typedef struct fsl_kernel_src_str fsl_kernel_src; 

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
    
cl_int FSLCL_LoadKernelSource(char *filename, fsl_kernel_src *kernel) 
{ 
    FILE *fp = NULL; 
    fp = fopen (filename, "rb");
    

    if (fp == NULL) {
        printf ("\nFailed to open: %s\n", filename); 
        return FSLCL_ERROR; 
    } 

    fseek (fp, 0, SEEK_END); 
    kernel->size = ftell (fp); 
    rewind (fp); 

    kernel->src =  (char *) malloc (sizeof (char) * kernel->size); 
    if (! kernel->src) { 
        printf ("\nError Allocating memory to load CL program"); 
        return FSLCL_ERROR; 
    } 

    fread (kernel->src, 1, kernel->size, fp); 
    kernel->src[kernel->size] = '\0'; 
    fclose (fp); 

    return FSLCL_SUCCESS; 
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
        
int main(int argc, char **argv) 
{
    debug("enter main");
    int dimension = 2;
    size_t global[2] = {buffer_width, buffer_height};
    //size_t local[2] = {4, 16};
    int size_2d = buffer_width * buffer_height * sizeof(unsigned short);//suze of buffer
    cl_int ret;
    cl_int platforms;
    int i, j;
    
    #if 0
    unsigned short **data;
    unsigned short **data2;
    data = (unsigned short **)malloc(buffer_width * sizeof(unsigned short *));
    data2 = (unsigned short **)malloc(buffer_width * sizeof(unsigned short *));

    for (i = 0; i < buffer_width; i++) {
        data[i] = (unsigned short *)malloc(buffer_height * sizeof(unsigned short));
        data2[i] = (unsigned short *)malloc(buffer_height * sizeof(unsigned short));
    }
     
    for (i = 0; i < buffer_width; i++) {
        for (j = 0; j < buffer_height; j++) {
            data[i][j] = 0;
            data2[i][j] = 0;
        }
    }
    #endif
    
    debug("allocating...");
    unsigned short * data = NULL;
    unsigned short * out = NULL;
    data = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(data != NULL, -1);
    memset(data, 0, buffer_width * buffer_height * sizeof(unsigned short));
    out = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(out != NULL, -1);
    memset(out, 0, buffer_width * buffer_height * sizeof(unsigned short));
    debug("allocate done");
    
    debug("get platform ids");
    ret = clGetPlatformIDs(1, &platform_id, &platforms);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("get device ids");
    cl_int devices;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &devices);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("create context");
    cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,  (cl_context_properties)platform_id, 0};
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("creat command queue");
    cq = clCreateCommandQueue(context, device_id, 0, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("load kernel source");
    fsl_kernel_src app_kernel;
    ret = FSLCL_LoadKernelSource ((char *)"kernel.cl", &app_kernel);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("create program with kernel source");
    //submit the source code of the kernel to OpenCL 
    program = clCreateProgramWithSource(context, 1, (const char **)&app_kernel.src, 0,&ret);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("build program");
    //and compile it (after this we could extract the compiled version) 
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (ret < 0) {
        printf ("device_id:%d, Failed\n", device_id);
        printf ("Return: %d\n", ret);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, app_kernel.size, app_kernel.src, NULL);
        printf ("src-%s\n", app_kernel.src);
    }
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("create buffer");
    buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY, size_2d, NULL, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    buffer_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size_2d, NULL, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("create kernel");
    //get a handle and map parameters for the kernel 
    kernel = clCreateKernel(program, "helloworld", &ret); 
    assert_and_return(ret == CL_SUCCESS, -2);
    
    clSetKernelArg (kernel, 0, sizeof(cl_mem), &buffer_input); 
    clSetKernelArg (kernel, 1, sizeof(cl_mem), &buffer_output); 
    clSetKernelArg (kernel, 2, sizeof(int), &buffer_width); 
    clSetKernelArg (kernel, 3, sizeof(int), &buffer_height); 
    
    ret = read_image("./input.raw", buffer_width, buffer_height, data);
    assert_and_return(ret == 0, -1);
    
    debug("enqueue write buffer");
    ret = clEnqueueWriteBuffer(cq, buffer_input, CL_TRUE, 0, size_2d, data, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    debug("enqueue write buffer done");

    #if 0
    debug("before gpu computing");
    ret = clEnqueueNDRangeKernel(cq, kernel, dimension, NULL, &global, NULL/*&local*/, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    debug("end gpu computing");
    #endif
    
    debug("enqueue read buffer");
    ret = clEnqueueReadBuffer(cq, buffer_output, CL_TRUE, 0, size_2d, out, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    debug("enqueue read buffer done");
    
    //#if 0
    debug("before cpu computing");
    for (i = 0; i < buffer_width; i++) {
        for (j = 0; j < buffer_height; j++) {
            out[i * buffer_width + j] = data[i * buffer_width + j] - data[i * buffer_width + j] + 400;
        }
    }
    debug("end cpu computing");
    //#endif
    
    ret = save_image(out, buffer_width, buffer_height, "./out.raw");
    if (ret != 0) {
        debug("write image failed");
    }

    clFlush( cq);
    clFinish(cq);

    clReleaseMemObject(buffer_input);
    clReleaseMemObject(buffer_output);

    clReleaseContext(context);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cq);

    debug("exit main");
    return 0; 
} 
