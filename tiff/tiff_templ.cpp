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
    union{
    int32 valueOffset;
    int32 value;
    };
};

class tiff_templ
{
  public:
    tiff_templ();
    ~tiff_templ();
    int32 create(const char* templ_file);
    int32 parse(const char* templ_file);
    int32 add_de(uint16 tagid, short val);
    int32 add_de(uint16 tagid, long val);
    int32 add_de(uint16 tagid, char* val, int32 len);
    int32 set_de(uint16 tagid, short val);
    int32 set_de(uint16 tagid, long val);
    int32 set_de(uint16 tagid, char* val, int32 len);
    int32 get_de(uint16 tagid, short& val);
    int32 get_de(uint16 tagid, long& val);
    int32 get_de(uint16 tagid, char* val, int32 len);
    int32 write_ifd();
    int32 write_raw_data(uint8* pData, int32 len);
    
  private:
    int32 m_ifd_offset;
    FILE* m_file;
    TIFH m_ifh;
    std::map<uint16, TDE> m_demap;
};

tiff_templ::tiff_templ() : m_ifd_offset(0), m_file(NULL) {
    
}

tiff_templ::~tiff_templ() {
    if (m_file != NULL)
        fclose(m_file);
}

int32 tiff_templ::create(const char* templ_file) {
    m_demap.clear();
    m_file = fopen(templ_file, "wb");
    m_ifh.byteOrder = 0x4949;
    m_ifh.version = 0x002a;
    m_ifh.offsetToFirstIFD = 0x00;
    fseek(m_file, m_ifd_offset, SEEK_SET);
    fwrite(&m_ifh, 1, sizeof(TIFH), m_file);
    m_ifd_offset += sizeof(m_ifh);
}

int32 tiff_templ::parse(const char* templ_file) {
    m_demap.clear();
    m_file = fopen(templ_file, "rb");
    TIFH ifh;
    fread(&ifh, 1, sizeof(TIFH), m_file);
    uint32 offsetFirstIFD = ifh.offsetToFirstIFD;
    fseek(m_file, offsetFirstIFD, SEEK_SET);
    uint16 de_count = 0;
    fread(&de_count, 1, 2, m_file);
    TDE de;
    for (int i = 0; i < de_count; i++) {
        fread(&de, 1, sizeof(TDE), m_file);
        m_demap.insert(std::pair<uint32, TDE>(de.tagid, de));
    }
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
        de.value = val;
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
        de.value = val;
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
       it->second.value = val;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::set_de(uint16 tagid, long val) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_LONG) {
       it->second.value = val;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::set_de(uint16 tagid, char* val, int32 len) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_ASCII) {
       fseek(m_file, it->second.valueOffset, SEEK_SET);
       int32 len_to_set = len < it->second.length ? len : it->second.length;
       fwrite(val, 1, len_to_set, m_file); 
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::get_de(uint16 tagid, short& val) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_SHORT) {
        val = it->second.value;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::get_de(uint16 tagid, long& val) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_LONG) {
        val = it->second.value;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::get_de(uint16 tagid, char* val, int32 len) {
    int32 ret = 0;
    std::map<uint16, TDE>::iterator it = m_demap.find(tagid);
    if (it != m_demap.end() && it->second.type == TIFF_ASCII) {
        int32 offset = it->second.valueOffset;
        uint32 val_len = it->second.length;
        char* pval = new char[val_len];
        memset(pval, 0, val_len);
        fseek(m_file, offset, SEEK_SET);
        fread(pval, 1, val_len, m_file);
        uint32 len_to_copy = val_len < len ? val_len : len;
        memcpy(val, pval, len_to_copy);
        delete []pval;
    } else {
        ret = -1;
    }
    return ret;
}

int32 tiff_templ::write_ifd() {
    fseek(m_file, 0, SEEK_SET);
    m_ifh.offsetToFirstIFD = m_ifd_offset;
    fwrite(&m_ifh, 1, sizeof(TIFH), m_file);
    
    const int DECountLen = 2;
    const int NextIFDOffsetLen = 4;
    long img_offset = m_ifd_offset + sizeof(TDE) * m_demap.size() + DECountLen + NextIFDOffsetLen;
    set_de(TIFFTAG_STRIPOFFSETS, img_offset);
    printf("Raw data offset:0x%02X\n", (uint32)img_offset);
    
    fseek(m_file, m_ifd_offset, SEEK_SET);
    uint16 de_count = m_demap.size();
    fwrite(&de_count, 1, 2, m_file);
    for (std::map<uint16, TDE>::iterator it = m_demap.begin(); it != m_demap.end(); it++) {
        if (it->second.tagid == TIFFTAG_EXTERN_EXPOSURELINE) {
            long pos = ftell(m_file);
            printf("Expline offset:0x%02X, cur value:0x%02X\n", (uint32)(pos + 2 + 2 + 4), it->second.value);
        }
        fwrite(&(it->second), 1, sizeof(TDE), m_file);
    }
    uint32 next_ifd_offset = 0x00;
    fwrite(&next_ifd_offset, 1, 4, m_file);
}

int32 tiff_templ::write_raw_data(uint8* pData, int32 len) {
    fseek(m_file, m_demap[TIFFTAG_STRIPOFFSETS].valueOffset, SEEK_SET);
    fwrite(pData, 1, len, m_file);
    fflush(m_file);
}

void help() {
    printf("Usage: tiff_templ.bin <create | parse> <templ_file> [width] [height] [depth] [expline]\n");
}

int32 main(int32 argc, char** argv) {
    if (argc < 3) {
        help();
        exit(-1);
    }
    bool b_create_templ = false;
    char templ_file[512] = "./templ.tif";
    int32 width = 0;
    int32 height = 0;
    int32 depth = 0;
    int32 expline = 0;
    if (0 == strcmp(argv[1], "create")) {
        if (argc < 7) {
            help();
            exit(-1);
        }
        strcpy(templ_file, argv[2]);
        width = atoi(argv[3]);
        height = atoi(argv[4]);
        depth = atoi(argv[5]);
        expline = atoi(argv[6]);
        b_create_templ = true;
    } else if (0 == strcmp(argv[1], "parse")) {
        strcpy(templ_file, argv[2]);
    } else {
        help();
        exit(-1);
    }
    
    /*
    TDE DEInfo[] = {
    {TIFFTAG_IMAGEWIDTH, 		TIFF_LONG, 		1, width}, 
	{TIFFTAG_IMAGELENGTH, 		TIFF_LONG, 		1, height}, 
	{TIFFTAG_BITSPERSAMPLE, 	TIFF_SHORT, 	1, depth * 8}, // TAG_BITSPERSAMPLE 颜色深度，1=单色，2=16色，8=256色，个数>2=真彩
	{TIFFTAG_COMPRESSION, 		TIFF_SHORT, 	1, 1}, // no Compression
	{TIFFTAG_PHOTOMETRIC, 		TIFF_SHORT, 	1, 1}, // 0:WhiteIsZero, 1:BlackIsZero
	{TIFFTAG_IMAGEDESCRIPTION,TIFF_ASCII, IMAGE_DESP_LEN, 0}, 
	{TIFFTAG_MAKER, 			TIFF_ASCII, 	MAKER_LEN, 		0}, 
	{TIFFTAG_MODEL, 			TIFF_ASCII,	 	MODEL_LEN, 		0}, 
	{TIFFTAG_STRIPOFFSETS, 		TIFF_LONG, 		1, 0}, //TAG_STRIPOFFSET 图像起始地址
	{TIFFTAG_ORIENTATION, 		TIFF_SHORT, 	1, 1}, //default:top left 
	{TIFFTAG_SAMPLESPERPIXEL, 	TIFF_SHORT, 	1, 1}, //TAG_SAMPLESPERPIXEL, for REG, number is 3
	{TIFFTAG_ROWSPERSTRIP, 	TIFF_LONG, 		1, 0},
	{TIFFTAG_STRIPBYTECOUNTS, 	TIFF_LONG, 		1, width * height * depth}, 
	{TIFFTAG_XRESOLUTION, 	TIFF_RATIONAL, 	1, 0}, //分数
	{TIFFTAG_YRESOLUTION, 	TIFF_RATIONAL, 	1, 0}, //分数
	{TIFFTAG_RESOLUTIONUNIT, 	TIFF_SHORT, 	1, 1}, 
	{TIFFTAG_SOFTWARE, 		TIFF_ASCII, 	SOFTWARE_INFO_LEN, 0}, 
	{TIFFTAG_DATETIME, 		TIFF_ASCII, 	DATETIME_INFO_LEN, 0}, 
    {TIFFTAG_EXTERN_EXPOSURELINE, TIFF_LONG, 	1, 0},
    };*/
    int ret = 0;
    if (b_create_templ) {
        tiff_templ* templ = new tiff_templ();
        printf("Creating templ '%s'...\n", templ_file);
        templ->create(templ_file);
        templ->add_de(TIFFTAG_IMAGEWIDTH, (long)width);
        templ->add_de(TIFFTAG_IMAGELENGTH, (long)height);
        templ->add_de(TIFFTAG_BITSPERSAMPLE, (short)(depth * 8));
        templ->add_de(TIFFTAG_COMPRESSION, (short)1);
        templ->add_de(TIFFTAG_PHOTOMETRIC, (short)1);
        templ->add_de(TIFFTAG_STRIPOFFSETS, (long)0);
        templ->add_de(TIFFTAG_ORIENTATION, (short)1);
        templ->add_de(TIFFTAG_SAMPLESPERPIXEL, (short)1);
        templ->add_de(TIFFTAG_STRIPBYTECOUNTS, (long)(width * height * depth));
        templ->add_de(TIFFTAG_EXTERN_EXPOSURELINE, (long)(0xFFFFFFFF));
        templ->add_de(TIFFTAG_MAKER, const_cast<char*>("iRay"), 4);
        templ->set_de(TIFFTAG_EXTERN_EXPOSURELINE, (long)expline);
        templ->write_ifd();
        int32 len = width * height * depth;
        uint8* pdata = new uint8[len];
        memset(pdata, 0xFF, len);
        templ->write_raw_data(pdata, len);
        delete []pdata;
        printf("TIF template file saved to ./templ.tif\n");
        delete templ;
    } else {
        tiff_templ* templ = new tiff_templ();
        printf("Parsing templ '%s'...\n", templ_file);
        templ->parse(templ_file);
        long exposure_line = 0;
        templ->get_de(TIFFTAG_EXTERN_EXPOSURELINE, exposure_line);
        printf("Expline:0x%02X\n", (uint32)exposure_line);
        long raw_data_offset = 0;
        ret = templ->get_de(TIFFTAG_STRIPOFFSETS, raw_data_offset);
        printf("Raw data offset:0x%02X\n", (uint32)raw_data_offset);
        char maker[512] = "\0";
        templ->get_de(TIFFTAG_MAKER, maker, 512);
        printf("Maker:%s\n", maker);
        delete templ;
    }
    
    exit(0);
}


