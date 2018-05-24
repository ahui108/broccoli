#include <stdio.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


int get_disk_sn(const char* szDevName, char* szSN, size_t nLimit)
{
    struct hd_driveid id;
    int  nRtn = -1;
    int  fd = open(szDevName, O_RDONLY|O_NONBLOCK); 
    while(1)
    {
        if (fd < 0)
        {
            perror(szDevName);
            break;
        }

        if(!ioctl(fd, HDIO_GET_IDENTITY, &id))
        {
             strcpy(szSN, id.serial_no);
             //printf("szSN Number=(%s)\n",szSN);
             //printf("Model Number=%s\n",id.model);
             nRtn = 0;
        }
        break;
    }
    if(fd >= 0)
        close(fd);
    return nRtn;
}



int Get_bord_id(char *bordid)
{
	FILE *fp = 0;
	int retval = 0;
	char buff[64] = {0};
    char *pTemp = buff;
	if ((fp = popen(" sudo dmidecode -t 2 | grep Serial | awk '{print $3}'", "r")) != NULL) {
		if (fgets(buff, sizeof(buff), fp)) {
			//sscanf(buff, "%d", &retval);
			//printf("buff %s\n",buff);
		}
		pclose(fp);
	}

    while(*pTemp != '\0')
    {
        if(isalnum(*pTemp))
        {
            *bordid = *pTemp;
            bordid++;
        }
        pTemp++;
    }

	return (retval);

}


int Get_cpu_id(char *cpuid)  
{
    unsigned   int   s1,s2;
    char   sel;
    asm volatile 
    ( "movl $0x01 , %%eax ; \n\t"  
    "xorl %%edx , %%edx ;\n\t"  
    "cpuid ;\n\t"   
    "movl %%edx ,%0 ;\n\t"  
    "movl %%eax ,%1 ; \n\t"  
    :"=m"(s1),"=m"(s2)
    );
    if (0 == s1 && 0 == s2)  
    {  
        printf("get cpu id fail\n");
        return -1;  
    }

    sprintf(cpuid,"%08X%08X",htonl(s2), htonl(s1));
    //printf("***%s***\n",cpuid);
    return 0;
}  
int main(int   argc,   char*   argv[])     
{     
    char cpuid[16]={0};
    char diskid[64]={0};
    char bordid[64]={0};
    Get_cpu_id(cpuid);
    Get_bord_id(bordid);
    printf("%s\n",cpuid);
    printf("%s\n",bordid);
    //setuid(geteuid());  
    //setgid(0); 
    //get_disk_sn("/dev/sda",diskid,64);
    
    return   0;     
}
