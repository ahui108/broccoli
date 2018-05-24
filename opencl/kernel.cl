__kernel void helloworld (__global ushort *input, __global ushort *output, int width, int height) 
{
    int x = get_global_id(0); //index x
    int y = get_global_id(1); //index y
    
    //int lx = get_local_id(0); //work item index-x in work group
    //int ly = get_local_id(1); //work item index-y in work group
    int xsz = get_global_size(0); //width
    //int ysz = get_global_size(1); //height
    
    //int gx = get_group_id(0); //group id on x
    //int gy = get_group_id(1); //group id on y
    
    //int gnx = get_num_groups(0); //group number on x
    //int gny = get_num_groups(1); //group number on y
    
    int id = (y * xsz) + x;
    //output[id] = id;
    //output[id] = x;
    //output[id] = y;
    //output[id] = xsz;
    //output[id] = lx;
    //output[id] = gx;
    //output[id] = gy;
    //output[id] = gnx;
    //output[id] = gny;
    
    //printf("%04d,%04d\n", x, y);
    output[id] = input[id] - input[id] + 400;
    /*
    if (x < width - 1) {
        output[id] = input[id] - input[id] + 400;
    } else {
        output[id] = input[id];
    }
    */
}