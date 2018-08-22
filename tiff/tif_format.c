#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    Ftp_Memory_Err
};

typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;

// CheckID, TIme, Name, Sex
typedef struct _ftp_node {
    uint16 expline;
} ftp_node;

static int32 ftp_tif_format(char* file_path, ftp_node *p_node)
{
    if (!file_path)
        return Ftp_Local_File_Not_Exist;
    
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL)
        return Ftp_Local_File_Open_Failed;
    else {
        long raw_len = 0;
        uint8* raw_data = NULL;
        if (fseek(fp, 0, SEEK_END) != -1) {
            raw_len = ftell(fp);
            rewind(fp);
            raw_data = (uint8*)malloc(raw_len);
            if (raw_data == NULL) {
                fclose(fp);
                return Ftp_Memory_Err;
            } else {
                if (fread(raw_data, 1, raw_len, fp) != raw_len) {
                    fclose(fp);
                    free(raw_data);
                    return Ftp_Local_File_Read_Failed;
                } else {
                    fclose(fp);
                }
            }
        }
        FILE* ptif = fopen("./templ.tif", "r");
        if (ptif == NULL) {
            free(raw_data);
            return Ftp_Local_File_Open_Failed;
        } else if (fseek(ptif, 0, SEEK_END) != -1) {
            long tif_len = ftell(ptif);
            rewind(ptif);
            uint8* tif_data = (uint8 *)malloc(tif_len);
            if (tif_data == NULL) {
                fclose(ptif);
                free(raw_data);
                return Ftp_Memory_Err;
            } else {
                if(fread(tif_data, 1, tif_len, ptif) != tif_len) {
                    fclose(ptif);
                    free(raw_data);
                    free(tif_data);
                    return Ftp_Local_File_Read_Failed;
                } else {
                    fclose(ptif);
                }
            }
            
            uint16 expline_id = 0x800B;
            uint16 expline = p_node->expline;
            if (tif_len > raw_len) {
                memcpy(tif_data + 0x050e, &expline_id, 2); //set expline id
                memcpy(tif_data + 0x0566, raw_data, raw_len);//save image
                memcpy(tif_data + 0x0516, &expline, 2); //set expline
                char tmp_file[MAX_PATH] = {0};
                sprintf(tmp_file, "/tmp/%s.tif", strrchr(file_path, '/') + 1);
                printf("tmp_file:%s\n", tmp_file);
                FILE* ptmp = fopen(tmp_file, "w");
                if (ptmp == NULL) {
                    free(raw_data);
                    free(tif_data);
                    return Ftp_Local_File_Open_Failed;
                } else {
                    printf("open succeed\n");
                    if (fwrite(tif_data, 1, tif_len, ptmp) != tif_len) {
                        fclose(ptmp);
                        free(raw_data);
                        free(tif_data);
                        return Ftp_Local_File_Write_Failed;
                    } else {
                        printf("write succeed\n");
                        fclose(ptmp);
                        free(raw_data);
                        free(tif_data);
                        strcpy(file_path, tmp_file);
                        printf("file path:%s\n", file_path);
                    }   
                }
            } else {
                fclose(ptif);
                free(raw_data);
                free(tif_data);
                return Ftp_Memory_Err;
            }
        }
    }
    return 0;
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
