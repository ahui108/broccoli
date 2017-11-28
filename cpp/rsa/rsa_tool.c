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
            info("Key file is needed");
        } else if (!strlen(infile)) {
            info("Input file is needed");
        } else if (!strlen(outfile)) {
            info("Output file is needed");
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
    int action_type = ACTION_TYPE_HELP;
    parse_args(argc, argv, &action_type);
    
    int encrypt_len = 128;
    unsigned char *source = (unsigned char*)malloc(encrypt_len);
    memset(source, 0, encrypt_len);
    if (source == NULL) {
        info("system error!\n");
        return -1;
    }
    
    unsigned char *ptr_en;
    FILE *file_in;
    if (ACTION_TYPE_HELP == action_type) {
        help();
    } else if (ACTION_TYPE_ENCRYPT == action_type || ACTION_TYPE_DECRYPT == action_type) {
        file_in = fopen(infile, "rb");
        if (file_in != NULL) {
            int count = fread(source, 1, encrypt_len, file_in);
            if (count != encrypt_len) {
                info("read input file failed\n");
            } else {
                debug("read input succeed\n");
                if (ACTION_TYPE_ENCRYPT == action_type) {
                    info("encrypting... ");
                    ptr_en = encrypt(source, key_file);
                } else {
                    info("decrypting... ");
                    ptr_en = decrypt(source, key_file);
                }
                //rewind(file);
                
                char cmd[512] = "\0";
                //sprintf(cmd, "cp -f %s %s", infile, outfile);
                //system(cmd);
                fseek(file_in, encrypt_len, SEEK_SET);
                FILE *file_out = fopen(outfile, "wb");
                if (file_out != NULL) {
                    count = fwrite(ptr_en, 1, encrypt_len, file_out);
                    if (count != encrypt_len) {
                        info("write output file failed\n");
                    }
                    
                    if (ptr_en != NULL) {
                        free(ptr_en);
                    }
                    
                    do {
                        encrypt_len = count;
                        count = fread(source, 1, encrypt_len, file_in);
                        if (count > 0) {
                            encrypt_len = count;
                            count = fwrite(source, 1, encrypt_len, file_out);
                            if (count != encrypt_len) {
                                info("failed");
                                break;
                            }
                        }
                    } while(count == encrypt_len);
                }
                info("done\n");
                
                sprintf(msg_verbose, "count=%d, encrypt_len=%d\n", count, encrypt_len);
                debug(msg_verbose);
                
                free(source);
                
                fclose(file_out);
                fclose(file_in);
            }
        }
    }
	
    return 0;
}

unsigned char *encrypt(unsigned char *str, char *path_key){
    debug("enter encrypt\n");
	unsigned char *p_en;
	RSA *p_rsa;
	FILE *file;
	int flen,rsa_len;
    if((file=fopen(path_key,"r")) == NULL){
		info("open key file error\n");
		return NULL;                
    }
	
    if((p_rsa=PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL){
		info("read rsa public key failed\n");
		ERR_print_errors_fp(stdout);
		return NULL;            
    }
	
	flen=strlen(str);
	rsa_len=RSA_size(p_rsa);
    
    sprintf(msg_verbose, "rsa len:%d\n", rsa_len);
    debug(msg_verbose);
	p_en=(unsigned char *)malloc(rsa_len + 1);
	memset(p_en, 0, rsa_len + 1);
    if(RSA_public_encrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_en, p_rsa, RSA_NO_PADDING) < 0){
		info("encrypt failed\n");
		return NULL;
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
		return NULL;            
    }
	
    if((p_rsa=PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL) {
		ERR_print_errors_fp(stdout);
		return NULL;            
    }
	rsa_len=RSA_size(p_rsa);
	p_de=(char *)malloc(rsa_len + 1);
	memset(p_de, 0, rsa_len + 1);
    if(RSA_private_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_de, p_rsa, RSA_NO_PADDING)<0){
	    return NULL;
    }
    
    RSA_free(p_rsa);
    fclose(file);
    debug("leave decrypt\n");
    return p_de;
}
