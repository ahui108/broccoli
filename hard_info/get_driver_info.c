/*************************************************************************
	> File Name: get_driver_info.c
	> Author: 
	> Mail: 
	> Created Time: 2018年05月02日 星期三 16时24分33秒
 ************************************************************************/

#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

static int getdiskid (char *id, char *model, size_t max)
{
    int fd;
    struct hd_driveid hid;
    FILE *fp;
    char line[0x100], *disk, *root, *p;

    fp = fopen ("/etc/mtab", "r");
    if (fp == NULL)
    {
        fprintf (stderr, "No /etc/mtab file.\n");
        return -1;
    }

    fd = -1;
    while (fgets(line, sizeof line, fp) != NULL)
    {
        disk = strtok (line, " ");
        if (disk == NULL)
        {
            continue;
        }

        root = strtok(NULL, " ");
        if (root == NULL)
        {
            continue;
        }
    	if (strcmp(root, "/") == 0)
    	{
            for (p = disk + strlen (disk) - 1; isdigit (*p); p --)
            {
                *p = '\0';
            }
            fd = open(disk, O_RDONLY);
            break;
    	}
    }

    fclose(fp);

    if (fd < 0)
    {
        fprintf(stderr, "open hard disk device failed.\n");
        return -1;
    }

    if (ioctl(fd, HDIO_GET_IDENTITY, &hid) < 0)
    {
        fprintf(stderr, "ioctl error.\n");
        return -1;
    }

    close(fd);

    memset(id, max, 0);
    memset(model, max, 0);
    snprintf(id, max, "%s", hid.serial_no);
    snprintf(model, max, "%s", hid.model);
    fprintf(stdout, "get hard disk serial number<%zd>: \'%s\'\n", strlen(id), id);
    fprintf(stdout, "get hard disk model<%zd>: \'%s\'\n", strlen(model), model);
    return 0;
}


int main(int argc, char** argv)
{
    char sn[64] = {0};
    char model[64] = {0};
    getdiskid(sn, model, 64);
}

