__kernel void helloworld (__global ushort *input, __global ushort *output, int width, int height) 
{
    int y = get_global_id(0);
    int x = get_global_id(1);
    int id = (y * width) + x;
    output[id] = input[id] - input[id] + 400;
}