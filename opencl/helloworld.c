#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <time.h> 
#include <CL/cl.h> 
 
#define FSLCL_ERROR -1 
#define FSLCL_SUCCESS CL_SUCCESS 

struct fsl_kernel_src_str 
{ 
  char *src; 
  long size; 
}; 

typedef struct fsl_kernel_src_str fsl_kernel_src; 
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

/* An error check macro for OpenCL.
 *
 * Usage:
 * CHECK_CL_ERROR(status_code_from_a_cl_operation, "function_name")
 *
 * It will abort with a message if an error occurred.
 */

#define CHECK_CL_ERROR(STATUS_CODE, WHAT) \
  if ((STATUS_CODE) != CL_SUCCESS) \
  { \
    fprintf(stderr, \
        "*** '%s' in '%s' on line %d failed with error '%s'.\n", \
        WHAT, __FILE__, __LINE__, \
        cl_error_to_str(STATUS_CODE)); \
    abort(); \
  }

/* A more automated error check macro for OpenCL, for use with clXxxx
 * functions that return status codes. (Not all of them do, notably 
 * clCreateXxx do not.)
 *
 * Usage:
 * CALL_CL_GUARDED(clFunction, (arg1, arg2));
 *
 * Note the slightly strange comma between the function name and the
 * argument list.
 */

#define CALL_CL_GUARDED(NAME, ARGLIST) \
  { \
    cl_int status_code; \
      status_code = NAME ARGLIST; \
    CHECK_CL_ERROR(status_code, #NAME); \
  }

/* An error check macro for Unix system functions. If "COND" is true, then the
 * last system error ("errno") is printed along with MSG, which is supposed to
 * be a string describing what you were doing.
 *
 * Example:
 * CHECK_SYS_ERROR(dave != 0, "opening hatch");
 */
#define CHECK_SYS_ERROR(COND, MSG) \
  if (COND) \
  { \
    perror(MSG); \
    abort(); \
  }
  
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

const char *cl_error_to_str(cl_int e)
{
  switch (e)
  {
    case CL_SUCCESS: return "success";
    case CL_DEVICE_NOT_FOUND: return "device not found";
    case CL_DEVICE_NOT_AVAILABLE: return "device not available";
#if !(defined(CL_PLATFORM_NVIDIA) && CL_PLATFORM_NVIDIA == 0x3001)
    case CL_COMPILER_NOT_AVAILABLE: return "device compiler not available";
#endif
    case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "mem object allocation failure";
    case CL_OUT_OF_RESOURCES: return "out of resources";
    case CL_OUT_OF_HOST_MEMORY: return "out of host memory";
    case CL_PROFILING_INFO_NOT_AVAILABLE: return "profiling info not available";
    case CL_MEM_COPY_OVERLAP: return "mem copy overlap";
    case CL_IMAGE_FORMAT_MISMATCH: return "image format mismatch";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "image format not supported";
    case CL_BUILD_PROGRAM_FAILURE: return "build program failure";
    case CL_MAP_FAILURE: return "map failure";

    case CL_INVALID_VALUE: return "invalid value";
    case CL_INVALID_DEVICE_TYPE: return "invalid device type";
    case CL_INVALID_PLATFORM: return "invalid platform";
    case CL_INVALID_DEVICE: return "invalid device";
    case CL_INVALID_CONTEXT: return "invalid context";
    case CL_INVALID_QUEUE_PROPERTIES: return "invalid queue properties";
    case CL_INVALID_COMMAND_QUEUE: return "invalid command queue";
    case CL_INVALID_HOST_PTR: return "invalid host ptr";
    case CL_INVALID_MEM_OBJECT: return "invalid mem object";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "invalid image format descriptor";
    case CL_INVALID_IMAGE_SIZE: return "invalid image size";
    case CL_INVALID_SAMPLER: return "invalid sampler";
    case CL_INVALID_BINARY: return "invalid binary";
    case CL_INVALID_BUILD_OPTIONS: return "invalid build options";
    case CL_INVALID_PROGRAM: return "invalid program";
    case CL_INVALID_PROGRAM_EXECUTABLE: return "invalid program executable";
    case CL_INVALID_KERNEL_NAME: return "invalid kernel name";
    case CL_INVALID_KERNEL_DEFINITION: return "invalid kernel definition";
    case CL_INVALID_KERNEL: return "invalid kernel";
    case CL_INVALID_ARG_INDEX: return "invalid arg index";
    case CL_INVALID_ARG_VALUE: return "invalid arg value";
    case CL_INVALID_ARG_SIZE: return "invalid arg size";
    case CL_INVALID_KERNEL_ARGS: return "invalid kernel args";
    case CL_INVALID_WORK_DIMENSION: return "invalid work dimension";
    case CL_INVALID_WORK_GROUP_SIZE: return "invalid work group size";
    case CL_INVALID_WORK_ITEM_SIZE: return "invalid work item size";
    case CL_INVALID_GLOBAL_OFFSET: return "invalid global offset";
    case CL_INVALID_EVENT_WAIT_LIST: return "invalid event wait list";
    case CL_INVALID_EVENT: return "invalid event";
    case CL_INVALID_OPERATION: return "invalid operation";
    case CL_INVALID_GL_OBJECT: return "invalid gl object";
    case CL_INVALID_BUFFER_SIZE: return "invalid buffer size";
    case CL_INVALID_MIP_LEVEL: return "invalid mip level";

#if defined(cl_khr_gl_sharing) && (cl_khr_gl_sharing >= 1)
    case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR: return "invalid gl sharegroup reference number";
#endif

#ifdef CL_VERSION_1_1
    case CL_MISALIGNED_SUB_BUFFER_OFFSET: return "misaligned sub-buffer offset";
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "exec status error for events in wait list";
    case CL_INVALID_GLOBAL_WORK_SIZE: return "invalid global work size";
#endif

    default: return "invalid/unknown error code";
  }
}

void print_platform_info()
{
    // get number of platforms
    cl_uint plat_count;
    CALL_CL_GUARDED(clGetPlatformIDs, (0, NULL, &plat_count));

    // allocate memory, get list of platform handles
    cl_platform_id *platforms = (cl_platform_id *) malloc(plat_count*sizeof(cl_platform_id));
    CHECK_SYS_ERROR(!platforms, "allocating platform array");
    CALL_CL_GUARDED(clGetPlatformIDs, (plat_count, platforms, NULL));
    
    for (cl_uint i = 0; i < plat_count; ++i)
    {
        char buf[MAX_NAME_LEN];
        CALL_CL_GUARDED(clGetPlatformInfo, (platforms[i], CL_PLATFORM_VENDOR, sizeof(buf), buf, NULL));
        printf("[%d] - platform: %s\n", i, buf);

        cl_uint dev_count;
        CALL_CL_GUARDED(clGetDeviceIDs, (platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &dev_count));

        // allocate memory, get list of device handles in platform
        cl_device_id *devices = (cl_device_id *) malloc(dev_count*sizeof(cl_device_id));
        CHECK_SYS_ERROR(!devices, "allocating device array");

        CALL_CL_GUARDED(clGetDeviceIDs, (platforms[i], CL_DEVICE_TYPE_ALL, dev_count, devices, NULL));
            
        for (cl_uint j = 0; j < dev_count; ++j)
        {
          char buf[MAX_NAME_LEN];
          CALL_CL_GUARDED(clGetDeviceInfo, (devices[j], CL_DEVICE_NAME,
                sizeof(buf), buf, NULL));
          printf("\t[%d] - device: %s\n", j, buf);
        }
        
        printf("\n");
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
    cl_mem buffer_input = NULL; 
    cl_mem buffer_output = NULL; 
    cl_kernel buffer_kernel = NULL; 
    //size_t buffer_size = 0; 
    int buffer_width = 3072; 
    int buffer_height = 3072;
    cl_platform_id platform_id[2]; 
    cl_device_id device_id[2]; 
    cl_context context; 
    cl_command_queue cq; 
    cl_program program; 
    cl_kernel kernel; 

    int dimension = 2;
    size_t global[2] = {buffer_width, buffer_height};
    //size_t local[2] = {4, 16};
    int size_2d = buffer_width * buffer_height * sizeof(unsigned short);//suze of buffer
    cl_int ret;
    cl_int platforms;
    int i, j;
        
    debug("allocating...");
    
    unsigned short * data = NULL;
    unsigned short * out = NULL;
    data = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(data != NULL, -1);
    memset(data, 0, buffer_width * buffer_height * sizeof(unsigned short));
    //init_buf(data, size_2d);
    out = (unsigned short *)malloc(buffer_width * buffer_height * sizeof(unsigned short));
    assert_and_return(out != NULL, -1);
    memset(out, 0, buffer_width * buffer_height * sizeof(unsigned short));
    //init_buf(out, size_2d);
    debug("allocate done");
    
    ret = read_image("./input.raw", buffer_width, buffer_height, data);
    assert_and_return(ret == 0, -1);
    
    debug("Print Platform Info");
    print_platform_info();
    
    debug("1. Get Platform IDs");
    ret = clGetPlatformIDs(2, platform_id, &platforms);
    printf("Platform Num: %d\n", platforms);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    cl_int platform_index = 1;
    char buf[MAX_NAME_LEN] = "\0";
    CALL_CL_GUARDED(clGetPlatformInfo, (platform_id[platform_index], CL_PLATFORM_VENDOR, sizeof(buf), buf, NULL));
    printf("Platform Name:%s\n", buf);

    debug("2. Get Device IDs");
    cl_int devices;
    ret = clGetDeviceIDs(platform_id[platform_index], /*CL_DEVICE_TYPE_GPU*/CL_DEVICE_TYPE_ALL, 2, device_id, &devices);
    printf("Device Num: %d\n", devices);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    cl_int device_index = 0;
    memset(buf, 0, MAX_NAME_LEN);
    CALL_CL_GUARDED(clGetDeviceInfo, (device_id[device_index], CL_DEVICE_NAME, sizeof(buf), buf, NULL));
    printf("Device Name:%s\n", buf);

    debug("3. Create Context");
    cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,  (cl_context_properties)platform_id[platform_index], 0};
    context = clCreateContext(properties, 2, device_id, NULL, NULL, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("4. Create Command Queue");
    cq = clCreateCommandQueue(context, device_id[device_index], 0, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("5. Load Kernel Source");
    fsl_kernel_src app_kernel;
    ret = FSLCL_LoadKernelSource ((char *)"kernel.cl", &app_kernel);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("6. Create Program");
    //submit the source code of the kernel to OpenCL 
    program = clCreateProgramWithSource(context, 1, (const char **)&app_kernel.src, 0,&ret);
    assert_and_return(ret == CL_SUCCESS, -2);

    debug("7. Build Program");
    //and compile it (after this we could extract the compiled version) 
    ret = clBuildProgram(program, 2, device_id, NULL, NULL, NULL);
    if (ret < 0) {
        printf ("Failed, Return: %d\n", ret);
        clGetProgramBuildInfo(program, &device_id[device_index], CL_PROGRAM_BUILD_LOG, app_kernel.size, app_kernel.src, NULL);
        printf ("src-%s\n", app_kernel.src);
    }
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("8. Create Kernel");
    //get a handle and map parameters for the kernel 
    kernel = clCreateKernel(program, "helloworld", &ret); 
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("9. Create Buffer");
    buffer_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, size_2d, data, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    buffer_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, size_2d, out, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    
    debug("10. Set Args");
    clSetKernelArg (kernel, 0, sizeof(cl_mem), &buffer_input); 
    clSetKernelArg (kernel, 1, sizeof(cl_mem), &buffer_output); 
    clSetKernelArg (kernel, 2, sizeof(int), &buffer_width); 
    clSetKernelArg (kernel, 3, sizeof(int), &buffer_height); 
    
    #if 0
    debug("CPU Computing");
    for (i = 0; i < buffer_width; i++) {
        for (j = 0; j < buffer_height; j++) {
            out[i * buffer_width + j] = data[i * buffer_width + j] - data[i * buffer_width + j] + 400;
        }
    }
    debug("Done");
    #endif
    
    #if 0
    debug("11. Enqueue Write Buffer");
    ret = clEnqueueWriteBuffer(cq, buffer_input, CL_TRUE, 0, size_2d, data, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    #endif
    
    //#if 0
    debug("12. GPU Computing");
    ret = clEnqueueNDRangeKernel(cq, kernel, dimension, NULL, &global, NULL/*&local*/, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    debug("Done");
    //#endif
    
    #if 0
    debug("13. Enqueue Read Buffer");
    ret = clEnqueueReadBuffer(cq, buffer_output, CL_TRUE, 0, size_2d, out, 0, NULL, NULL);
    assert_and_return(ret == CL_SUCCESS, -2);
    #endif
    
    unsigned short *device_buffer = (unsigned short *)clEnqueueMapBuffer(cq, buffer_output, CL_TRUE, CL_MAP_READ, 0, size_2d, 0, NULL, NULL, &ret);
    assert_and_return(ret == CL_SUCCESS, -2);
    assert_and_return(device_buffer != NULL, -2);
    debug("Map Buffer done");
    
    ret = save_image(device_buffer, buffer_width, buffer_height, "./out.raw");
    if (ret != 0) {
        debug("write image failed");
    }

    debug("14. Release Objects");
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
