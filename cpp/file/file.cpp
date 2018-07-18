#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>
#include <map>

std::string TrimStr(std::string& str)
{
    size_t pos1, pos2;
    for (pos1 = pos2 = 0; pos2 < str.size(); pos2++)
    {
        if (str[pos2] == ' ' || str[pos2] == '  ' || str[pos2] == '\r' || str[pos2] == '\n' || str[pos2] == '\0')
        {
            continue;
        }
        else
        {
            break;
        }
    }
    str.erase(pos1, pos2 - pos1);
    if (str.empty())
    {
        return str;
    }

    for (pos1 = pos2 = str.size() - 1; pos1 >= 0; pos1--)
    {
        if (str[pos1] == ' ' || str[pos1] == '  ' || str[pos1] == '\r' || str[pos1] == '\n' || str[pos1] == '\0')
        {
            continue;
        }
        else
        {
            break;
        }
    }
    str.erase(pos1 + 1, pos2 - pos1);
    return str;
}

void read_and_parse_file(const char* filePath)
{
    int ret = -1;
    FILE* pFile = NULL;
    pFile = fopen("Bind.txt", "r");
    if (NULL == pFile)
    {
        std::cout << "Open Bind.txt failed, " << strerror(errno) << std::endl;
    }
    else
    {
        int index = 0;
        char* pch = NULL;
        char szLine[1024] = "\0";
        std::map<int, std::string> mapDetector;
        while(NULL != fgets(szLine, 1024, pFile))
        {
            std::cout << index << " - '" << szLine << "'" << std::endl;
            printf("addr0:%02hhX,addr1:%02hhX,addr2:%02hhX,addr3:%c,addr4:%c,addr5:%c,addr6:%c\n", szLine[0], szLine[1], szLine[2],szLine[3], szLine[4],szLine[5], szLine[6]);
            //printf("addr0:%c,addr1:%c,addr2:%c,addr3:%c,addr4:%c,addr5:%c,addr6:%c\n", szLine[0], szLine[1], szLine[2],szLine[3], szLine[4],szLine[5], szLine[6]);
            int col = 0;
            pch = strtok(szLine, ",");
            //if (pch != NULL)
            //{
            printf("addr0:%02hhX,addr1:%02hhX,addr2:%02hhX,addr3:%c,addr4:%c,addr5:%c,addr6:%c\n", szLine[0], szLine[1], szLine[2],szLine[3], szLine[4],szLine[5], szLine[6]);
            printf("line addr:%p\n", szLine);
            std::cout << index << " - '" << szLine << "'" << std::endl; 
            while(pch != NULL)
            {
                printf("addr:%p\n", pch);
                std::cout << col << " - '" << pch << "'" << std::endl;
                switch (col)
                {
                case 0:
                    //console(1) << index << " - " << pch << ", ";
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    {
                    //console(1) << pch << std::endl;
                    std::string strDet(pch);
                    TrimStr(strDet);
                    std::cout << strDet << std::endl;
                    mapDetector.insert(std::pair<int, std::string>(index, strDet));
                    }
                    break;
                default:
                    break;
                }
                
                col++;   
                pch = strtok(NULL, ",");
            }
            //}
            pch = NULL;
            //memset(szLine, 0, 1024);
            index++;
        }
    }
}

int main(int argc, char** argv)
{
    read_and_parse_file("Bind.txt");
    return 0;
}
