/*************************************************************************
	> File Name: format_templ.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Sep 2018 10:26:42 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "tiff_templ.h"

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef signed int     int32;

struct TIFH 
{
    uint16 byteOrder;
    uint16 version;
    uint32 offsetToFirstIFD;
};

struct TDE
{
    uint16 tagid;
    uint16 type;
    uint32 length;
    uint32 valueOffset;
};

#define BUFFER_LEN_DEFUALT 2048
#define BUFFER_INCREMENT_DEFAULT 2048
class tiff_templ
{
  public:
    tiff_templ();
    ~tiff_templ();
    int32 add_de(uint16 tagid, short val);
    int32 add_de(uint16 tagid, long val);
    int32 add_de(uint16 tagid, char* val, int32 len);
    int32 set_de(uint16 tagid, short val);
    int32 set_de(uint16 tagid, long val);
    int32 set_de(uint16 tagid, char* val, int32 len);
    int32 write_ifd();
    int32 write_raw_data(uint8* pData, int32 len);
    
  private:
    int32 m_ifd_offset;
    std::map<uint16, TDE> m_demap;
    FILE* m_file;
};

tiff_templ::tiff_templ() : m_ifd_offset(0) {
    m_file = fopen("./templ.tif", "wb");
    TIFH header = {0x4949, 0x002a, 0x00};
    fseek(m_file, m_ifd_offset, SEEK_SET);
    fwrite(&header, 1, sizeof(TIFH), m_file);
    m_ifd_offset += sizeof(header);
}

tiff_templ::~tiff_templ() {
    fclose(m_file);
}

int32 tiff_templ::add_de(uint16 tagid, short val) {
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end()) {
        set_de(tagid, val);
    } else {
        TDE de;
        de.tagid = tagid;
        de.type = TIFF_SHORT;
        de.length = 1;
        de.valueOffset = val;
        m_demap.insert(std::pair<uint16, TDE>(tagid, de));
    }
}

int32 tiff_templ::add_de(uint16 tagid, long val) {
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end()) {
        set_de(tagid, val);
    } else {
        TDE de;
        de.tagid = tagid;
        de.type = TIFF_LONG;
        de.length = 1;
        de.valueOffset = val;
        m_demap.insert(std::pair<uint16, TDE>(tagid, de));
    }
}

int32 tiff_templ::add_de(uint16 tagid, char* val, int32 len) {
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end()) {
        set_de(tagid, val, len);
    } else {
        TDE de;
        de.tagid = tagid;
        de.type = TIFF_ASCII;
        de.length = len;
        de.valueOffset = m_ifd_offset;
        fseek(m_file, m_ifd_offset, SEEK_SET);
        fwrite(val, 1, len, m_file);
        m_ifd_offset += len;
        m_demap.insert(std::pair<uint16, TDE>(tagid, de));
    }
}

int32 tiff_templ::set_de(uint16 tagid, short val) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_SHORT) {
       it->second.valueOffset = val;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::set_de(uint16 tagid, long val) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_LONG) {
       it->second.valueOffset = val;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::set_de(uint16 tagid, char* val, int32 len) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_ASCII && len <= it->second.length) {
       fseek(m_file, it->second.valueOffset, SEEK_SET);
       fwrite(val, 1, len, m_file); 
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::write_ifd() {
    fseek(m_file, 4, SEEK_SET);
    fwrite(&m_ifd_offset, 1, 4, m_file);
    fseek(m_file, m_ifd_offset, SEEK_SET);
    uint16 de_count = m_demap.size();
    fwrite(&de_count, 1, 2, m_file);
    for (std::map<uint16, TDE>::iterator it = m_demap.begin(); it != m_demap.end(); it++) {
        fwrite(&(it->second), 1, sizeof(TDE), m_file);
    }
    uint32 next_ifd_offset = 0x00;
    fwrite(&next_ifd_offset, 1, 4, m_file);
}

int32 tiff_templ::write_raw_data(uint8* pData, int32 len) {
    long imgoffset = m_ifd_offset + sizeof(TDE) * m_demap.size() + 2 + 4;
    set_de(TIFFTAG_STRIPOFFSETS, imgoffset);
    fseek(m_file, imgoffset, SEEK_SET);
    fwrite(pData, 1, len, m_file);
}

void help() {
    printf("Usage: tiff_templ.bin <width> <height> <depth> <expline>\n");
}

int32 main(int32 argc, char** argv) {
    if (argc < 5) {
        help();
        exit(-1);
    }
    int32 width = atoi(argv[1]);
    int32 height = atoi(argv[2]);
    int32 depth = atoi(argv[3]);
    int32 expline = atoi(argv[4]);
    
    /*
    TDE DEInfo[] = {
    {TIFFTAG_IMAGEWIDTH, 		TIFF_LONG, 		1, width}, 
	{TIFFTAG_IMAGELENGTH, 		TIFF_LONG, 		1, height}, 
	{TIFFTAG_BITSPERSAMPLE, 	TIFF_SHORT, 	1, depth * 8}, // TAG_BITSPERSAMPLE 颜色深度，1=单色，2=16色，8=256色，个数>2=真彩
	{TIFFTAG_COMPRESSION, 		TIFF_SHORT, 	1, 1}, // no Compression
	{TIFFTAG_PHOTOMETRIC, 		TIFF_SHORT, 	1, 1}, // 0:WhiteIsZero, 1:BlackIsZero
	//{TIFFTAG_IMAGEDESCRIPTION,TIFF_ASCII, IMAGE_DESP_LEN, 0}, 
	//{TIFFTAG_MAKER, 			TIFF_ASCII, 	MAKER_LEN, 		0}, 
	//{TIFFTAG_MODEL, 			TIFF_ASCII,	 	MODEL_LEN, 		0}, 
	{TIFFTAG_STRIPOFFSETS, 		TIFF_LONG, 		1, 0}, //TAG_STRIPOFFSET 图像起始地址
	{TIFFTAG_ORIENTATION, 		TIFF_SHORT, 	1, 1}, //default:top left 
	{TIFFTAG_SAMPLESPERPIXEL, 	TIFF_SHORT, 	1, 1}, //TAG_SAMPLESPERPIXEL, for REG, number is 3
	//{TIFFTAG_ROWSPERSTRIP, 	TIFF_LONG, 		1, 0},
	{TIFFTAG_STRIPBYTECOUNTS, 	TIFF_LONG, 		1, width * height * depth}, 
	//{TIFFTAG_XRESOLUTION, 	TIFF_RATIONAL, 	1, 0}, //分数
	//{TIFFTAG_YRESOLUTION, 	TIFF_RATIONAL, 	1, 0}, //分数
	//{TIFFTAG_RESOLUTIONUNIT, 	TIFF_SHORT, 	1, 1}, 
	//{TIFFTAG_SOFTWARE, 		TIFF_ASCII, 	SOFTWARE_INFO_LEN, 0}, 
	//{TIFFTAG_DATETIME, 		TIFF_ASCII, 	DATETIME_INFO_LEN, 0}, 
    {TIFFTAG_EXTERN_EXPOSURELINE, TIFF_LONG, 	1, 0},
    };*/
    
    tiff_templ templ;
    templ.add_de(TIFFTAG_IMAGEWIDTH, (long)width);
    templ.add_de(TIFFTAG_IMAGELENGTH, (long)height);
    templ.add_de(TIFFTAG_BITSPERSAMPLE, (short)(depth * 8));
    templ.add_de(TIFFTAG_COMPRESSION, (short)1);
    templ.add_de(TIFFTAG_PHOTOMETRIC, (short)1);
    templ.add_de(TIFFTAG_STRIPOFFSETS, (long)0);
    templ.add_de(TIFFTAG_ORIENTATION, (short)1);
    templ.add_de(TIFFTAG_SAMPLESPERPIXEL, (short)1);
    templ.add_de(TIFFTAG_STRIPBYTECOUNTS, (long)(width * height * depth));
    templ.add_de(TIFFTAG_EXTERN_EXPOSURELINE, (long)(0xFFFFFFFF));
    templ.add_de(TIFFTAG_MAKER, const_cast<char*>("iRay"), 4);
	templ.set_de(TIFFTAG_STRIPOFFSETS, (long)0x00);
    templ.set_de(TIFFTAG_EXTERN_EXPOSURELINE, (long)expline);
	templ.write_ifd();
    int32 len = width * height * depth;
    uint8* pdata = new uint8[len];
    memset(pdata, 0xFF, len);
    templ.write_raw_data(pdata, len);
    delete []pdata;
    printf("TIF template file saved to ./templ.tif\n");
    exit(0);
}


