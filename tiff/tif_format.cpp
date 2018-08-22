#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH (256)

enum {
    Ftp_Open_or_Login_Failed = 0x01,
    Ftp_Mkdir_or_Cd_Failed,
    Ftp_Local_File_Open_Failed,
    Ftp_Local_File_Read_Failed,
    Ftp_Local_File_Write_Failed,
    Ftp_Local_File_Not_Exist,
    Ftp_Put_Failed,
    Ftp_Get_Failed,
    Ftp_File_Verify_Failed,
    Ftp_File_Type_Err,
    Ftp_Memory_Err,
};

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed int int32;

// CheckID, TIme, Name, Sex
typedef struct _ftp_node {
    uint16 expline;
} ftp_node;

static int32 ftp_read_file(const char* file_path, uint8** buf, int32& len) {
    printf("enter read\n");
    int32 ret = 0;
    if (!file_path) {
        ret = Ftp_Local_File_Not_Exist;
    } else {
        FILE* fp = fopen(file_path, "r");
        if (fp == NULL)
            ret = Ftp_Local_File_Open_Failed;
        else {
            if (fseek(fp, 0, SEEK_END) == -1) {
                ret = Ftp_File_Verify_Failed;
            } else {
                len = ftell(fp);
                rewind(fp);
                //uint8 *pbuf = new uint8[len];
                void *pbuf = malloc(len);
                if (!pbuf)
                    ret = Ftp_Memory_Err;
                else if (fread(pbuf, 1, len, fp) != len) {
                    printf("read failed\n");
                    ret = Ftp_Local_File_Read_Failed;
                    if(pbuf != NULL) {
                        free(pbuf);
                        pbuf = NULL;
                    }
                }
                *buf = (uint8 *)pbuf;
            }
            fclose(fp);
        }
    }
    printf("buf:%p\n", *buf);
    printf("exit read\n");
    return ret;
}

static int32 ftp_tif_format(const char* file_path, const ftp_node* p_node)
{
    int32 ret = 0;
    if (!file_path)
        return Ftp_Local_File_Not_Exist;
    if (!p_node)
        return Ftp_File_Type_Err;
        
    uint8* p_raw_data = NULL;
    uint8* p_templ_data = NULL;
    int32 raw_len = 0;
    int32 templ_len = 0;
    if (!(ret = ftp_read_file(file_path, &p_raw_data, raw_len))) {
        if (!(ret = ftp_read_file("./templ.tif", &p_templ_data, templ_len))) {
            printf("raw_data:%p\n", p_raw_data);
            printf("templ_data:%p\n", p_templ_data);
            //uint16 De_count_offset = 0x0488;
            //uint16 De_count = 0x13;
            uint32 expline_De_offset = 0x0562;
            uint16 expline_De_tag = 0x800B;
            uint16 expline_De_type = 0x04;
            uint32 expline_De_len = 0x01;
            uint16 expline = p_node->expline;
            uint32 expline_De_val = expline;
            uint32 next_IFD = 0x00;
            if (templ_len > raw_len) {
                printf("templ len:%d, raw_len:%d\n", templ_len, raw_len);
                //de count offset 0x0488, DE Count: 0x13
                //memcpy(tif_data + De_count_offset, &De_count, 2);
                //exp offset 0x0562, Tag=0x800B
                memcpy(p_templ_data + expline_De_offset, &expline_De_tag, 2); 
                printf("1\n");
                memcpy(p_templ_data + expline_De_offset + 2, &expline_De_type, 2);
                printf("2\n");
                memcpy(p_templ_data + expline_De_offset + 4, &expline_De_len, 4);
                printf("3\n");
                memcpy(p_templ_data + expline_De_offset + 8, &expline_De_val, 4);
                printf("4\n");
                memcpy(p_templ_data + expline_De_offset + 12, &next_IFD, 4);
                printf("5\n");
                memcpy(p_templ_data + expline_De_offset + 16, p_raw_data, raw_len);//save image
                char tmp_file[MAX_PATH] = {0};
                sprintf(tmp_file, "/tmp/%s.tif", strrchr(file_path, '/') + 1);
                printf("tmp_file:%s\n", tmp_file);
                FILE* ptmp = fopen(tmp_file, "w");
                if (ptmp == NULL) {
                    ret = Ftp_Local_File_Open_Failed;
                } else {
                    printf("open succeed\n");
                    if (fwrite(p_templ_data, 1, templ_len, ptmp) != templ_len) {
                        printf("write failed\n");
                        ret = Ftp_Local_File_Write_Failed;
                    }
                    fclose(ptmp);
                    //strcpy(file_path, tmp_file);
                    printf("file path:%s\n", file_path);
                       
                }
            }
        }
    }
    
    if (p_raw_data != NULL)
        free(p_raw_data);
    if (p_templ_data != NULL)
        free(p_templ_data);
    return ret;
}

int main(int argc, char** argv)
{
    ftp_node node;
    node.expline = 10;
    char rawFile[MAX_PATH] = "./image.raw";
    int32 ret = ftp_tif_format(rawFile, &node);
    printf("ret:%d\n", ret);
    system("cp -f /tmp/image.raw.tif .");
    return 0;
}
