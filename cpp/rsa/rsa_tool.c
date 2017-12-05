/*************************************************************************
	> File Name: rsatest.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月22日 星期三 16时05分09秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>  
#include<unistd.h>
#include<sys/stat.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

#define BUFF_LEN 1024

enum action_type{
    ACTION_TYPE_NO,
    ACTION_TYPE_ENCRYPT, 
    ACTION_TYPE_DECRYPT,
    ACTION_TYPE_HELP,
};

char key_file[1024] = "\0";
char infile[1024] = "\0";
char outfile[1024] = "\0";
int show_verbose = 0;
char msg_verbose[1024] = "\0";

unsigned char* encrypt(unsigned char *str,char *path_key);//加密
unsigned char* decrypt(unsigned char *str,char *path_key);//解密

void help() {
    char help[] = {
"Usage: logutil.bin [OPTION]\n\
Encrypt or decrypt file with rsa key pair.\n\
\n\
Valid options are:\n\
-help,--help    Display this summary\n\
-encrypt        Encrypt with key file\n\
-decrypt        Decrypt with key file\n\
-inkey keyfile  Input key file\n\
-in infile      Input file\n\
-out outfile    Output file\n" 
    };
    
    printf("%s", help);
}

void debug(const char * msg) {
    if (!show_verbose) { 
        return;
    } else {
        if (msg != NULL) {
            printf(msg);
        }
    }
}

void info(const char * msg) {
    if (msg != NULL) {
        printf(msg);
    }
}

void parse_args(int argc, char **argv, int *action)
{
    debug("enter parse_args\n");
    const char *optstring="h:e:d:k:i:o:v";
    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"encrypt", no_argument, NULL, 'e'},
        {"decrypt", no_argument, NULL, 'd'},
        {"inkey", required_argument, NULL, 'k'},
        {"in", required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {"v", no_argument, NULL, 'v'},
        {0,0,0,0},
    };
    
    int opt;
    int option_index = 0;
    int type = ACTION_TYPE_NO;
    
    while((opt = getopt_long_only(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        if (opt == 'h') {
            type = ACTION_TYPE_HELP;
        } else if (opt == 'e') {
            type = ACTION_TYPE_ENCRYPT;
        } else if (opt == 'd') {
            type = ACTION_TYPE_DECRYPT;
        } else if (opt == 'k') {
            strcpy(key_file, optarg);
        } else if (opt == 'i') {
            strcpy(infile, optarg);
        } else if (opt == 'o') {
            strcpy(outfile, optarg);
        } else if (opt == 'v') {
            show_verbose = 1;
        } else {
            type = ACTION_TYPE_HELP;
        }
    }
    
    debug("after parse\n");
    int valid_args = 0;
    if (ACTION_TYPE_ENCRYPT == type || ACTION_TYPE_DECRYPT == type) {
        if (!strlen(key_file)) {
            info("Key file is required.\n");
        } else if (!strlen(infile)) {
            info("Input file is required.\n");
        } else if (!strlen(outfile)) {
            info("Output file is required.\n");
        } else {
            valid_args = 1;
        }
    }
    
    if (!valid_args) {
        type = ACTION_TYPE_HELP;
    }

    *action = type;
    debug("leave parse_args\n");
}

int main(int argc, char **argv) {
    debug("enter main\n");
    time_t start = time(NULL);
    int action_type = ACTION_TYPE_HELP;
    parse_args(argc, argv, &action_type);
    
    unsigned char *ptr_crypt;
    unsigned int len_crypt_buffer = 1024;
    unsigned int len_crypting = 0;
    unsigned int len_crypted = 0;
    FILE *file_in;
    if (ACTION_TYPE_HELP == action_type) {
        help();
    } else if (ACTION_TYPE_ENCRYPT == action_type || ACTION_TYPE_DECRYPT == action_type) {
        if (ACTION_TYPE_ENCRYPT == action_type) {
            len_crypting = 117;
            len_crypted = 128;
        } else {
            len_crypting = 128;
            len_crypted = 117;
        }

        unsigned char *source = (unsigned char*)malloc(len_crypt_buffer);
        if (source == NULL) {
            info("system error!\n");
            return -1;
        }
        memset(source, 0, len_crypt_buffer);
        
        file_in = fopen(infile, "rb");
        if (file_in != NULL) {
            int count = fread(source, len_crypting, 1, file_in);
            if (count != 1) {
                info("read input file failed.\n");
            } else {
                debug("read input succeed.\n");
                if (ACTION_TYPE_ENCRYPT == action_type) {
                    info("encrypting...");
                    ptr_crypt = encrypt(source, key_file);
                } else {
                    info("decrypting...");
                    ptr_crypt = decrypt(source, key_file);
                }
                
                if (NULL == ptr_crypt) {
                    return -2;
                }
                char cmd[512] = "\0";
                /*
                sprintf(cmd, "cp -f %s %s", infile, outfile);
                system(cmd);
                */
                FILE *file_out = fopen(outfile, "wb");
                /*
                FILE *file_hd = NULL;
                if (ACTION_TYPE_ENCRYPT == action_type) {
                    file_hd = fopen("hd.en", "wb");
                } else {
                    file_hd = fopen("hd.de", "wb");
                }
                */

                if (file_out != NULL) {
                    debug("before write output\n");
                    /*
                    fwrite(ptr_crypt, 1, len_crypted, file_hd);
                    fclose(file_hd);
                    */
                    count = fwrite(ptr_crypt, len_crypted, 1, file_out);
                    if (count != 1) {
                        info("write output file failed.\n");
                    }
                    
                    if (ptr_crypt != NULL) {
                        free(ptr_crypt);
                    }
                    
                    int insize = 0;
                    struct stat instat;
                    if (0 != stat(infile, &instat)) {
                        debug("get input file size failed.");
                    } else {
                        insize = instat.st_size;
                        info("output appending\n");
                        int blkcnt = 0;
                        int blkleft = 0;
                        if (insize > len_crypting) {
                            insize -= len_crypting;
                            blkcnt = insize / len_crypt_buffer;
                            blkleft = insize % len_crypt_buffer;
                        }
                        int i = 0;
                        for (; i <= blkcnt; i++) {
                            if (i == blkcnt) {
                                if (blkleft > 0) {
                                    len_crypting = blkleft;
                                } else {
                                    break;
                                }
                            } else {
                                len_crypting = len_crypt_buffer;
                            }
                            count = fread(source, len_crypting, 1, file_in);
                            if (count > 0) {
                                count = fwrite(source, len_crypting, 1, file_out);
                                if (count != 1) {
                                    info("failed!");
                                    break;
                                }
                            }
                        }//end for
                    }
                }
                info("done\n");
                
                sprintf(msg_verbose, "count=%d, encrypt_len=%d\n", count, len_crypting);
                debug(msg_verbose);
                
                free(source);
                fclose(file_out);
                fclose(file_in);
            }
        }
    }
	
    time_t end = time(NULL);
    sprintf(msg_verbose, "exit - %ld\n", end - start);
    debug(msg_verbose);
    return 0;
}

unsigned char *encrypt(unsigned char *str, char *path_key){
    debug("enter encrypt\n");
    
	unsigned char *p_en;
	RSA *p_rsa;
	FILE *file;
	int flen;
    int rsa_len;
    if((file=fopen(path_key,"r")) == NULL){
		info("open key file error\n");
    } else {	
        if((p_rsa=PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL){
            info("read rsa public key failed\n");
            ERR_print_errors_fp(stdout);
        } else {        
            rsa_len=RSA_size(p_rsa);
            flen = rsa_len - 11;
            
            sprintf(msg_verbose, "rsa size:%d\n", rsa_len);
            debug(msg_verbose);
            p_en=(unsigned char *)malloc(rsa_len + 1);
            memset(p_en, 0, rsa_len + 1);
            if(RSA_public_encrypt(flen, (unsigned char *)str, (unsigned char*)p_en, p_rsa, RSA_PKCS1_PADDING) < 0){
                ERR_print_errors_fp(stdout);
                free(p_en);
                p_en = NULL;
                info("encrypt failed\n");
            }
        }
    }
	
	RSA_free(p_rsa);
	fclose(file);
    
    debug("leave encrypt\n");
	return p_en;
}

unsigned char *decrypt(unsigned char *str, char *path_key){
    debug("enter decrypt\n");
	
    unsigned char *p_de;
	RSA *p_rsa;
	FILE *file;
	int rsa_len;
    if((file=fopen(path_key,"r"))==NULL){
		info("open key file error\n");
    } else {	
        if((p_rsa=PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL) {
            ERR_print_errors_fp(stdout);
            debug("read key failed\n");
        } else {
            rsa_len=RSA_size(p_rsa);
            p_de=(char *)malloc(rsa_len + 1);
            memset(p_de, 0, rsa_len + 1);
            if(RSA_private_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_de, p_rsa, RSA_PKCS1_PADDING) < 0) {
                ERR_print_errors_fp(stdout);
                free(p_de);
                p_de = NULL;
                info("decrypt failed\n");
            }
        }
    }
    
    fclose(file);
    RSA_free(p_rsa);
    
    debug("leave decrypt\n");
    return p_de;
}
