/*************************************************************************
	> File Name: format_temp.h
	> Author: 
	> Mail: 
	> Created Time: Tue 04 Sep 2018 10:43:09 AM CST
 ************************************************************************/

#ifndef _FORMAT_TEMP_H
#define _FORMAT_TEMP_H

#define	TIFFTAG_SUBFILETYPE			0xFE	/* subfile data descriptor */
#define	TIFFTAG_IMAGEWIDTH			0x100	/* image width in pixels */
#define	TIFFTAG_IMAGELENGTH			0x101	/* image height in pixels */
#define	TIFFTAG_BITSPERSAMPLE		0x102	/* bits per channel (sample) */
#define	TIFFTAG_COMPRESSION			0x103	/* data compression technique */
#define	TIFFTAG_PHOTOMETRIC			0x106	/* photometric interpretation */
#define	TIFFTAG_IMAGEDESCRIPTION	0x10E	/* info about image */
#define	TIFFTAG_MAKER				0x10F	/* scanner manufacturer name */
#define	TIFFTAG_MODEL				0x110	/* scanner model name/number */
#define	TIFFTAG_STRIPOFFSETS		0x111	/* offsets to data strips */
#define	TIFFTAG_ORIENTATION			0x112	/* +image orientation */
#define	TIFFTAG_SAMPLESPERPIXEL		0x115	/* samples per pixel */
#define	TIFFTAG_ROWSPERSTRIP		0x116	/* rows per strip of data */
#define	TIFFTAG_STRIPBYTECOUNTS		0x117	/* bytes counts for strips */
#define	TIFFTAG_XRESOLUTION			0x11A	/* pixels/resolution in x */
#define	TIFFTAG_YRESOLUTION			0x11B	/* pixels/resolution in y */
#define	TIFFTAG_RESOLUTIONUNIT		0x128	/* units of resolutions */
#define	TIFFTAG_SOFTWARE			0x131	/* name & release */
#define	TIFFTAG_DATETIME			0x132	/* creation date and time */
#define TIFFTAG_EXTERN_FRAMENUMBER		0x8001
#define TIFFTAG_EXTERN_TEMPERATURE		0x8002		
#define TIFFTAG_EXTERN_REALFREQUENCY	0x8003
#define TIFFTAG_EXTERN_EXPOSUREDELAY	0x8004
#define TIFFTAG_EXTERN_ACQUIRETIME		0x8005
#define TIFFTAG_EXTERN_KV				0x8006
#define TIFFTAG_EXTERN_MA 				0x8007
#define TIFFTAG_EXTERN_MS 				0x8008
#define TIFFTAG_EXTERN_SID				0x8009
#define TIFFTAG_EXTERN_DOSERATE			0x800A
#define TIFFTAG_EXTERN_EXPOSURELINE		0x800B
#define TIFFTAG_EXTERN_CORRECTIONFLAG	0x800C
#define TIFFTAG_EXTERN_BINNINGMODE		0x800D
#define TIFFTAG_EXTERN_XRAYWINDOW		0x800E
#define TIFFTAG_EXTERN_FRAMERATE		0x800F
#define TIFFTAG_EXTERN_BODYPARTSECTION	0x8010
#define TIFFTAG_EXTERN_BODYPART 		0x8011

//USER DATA ID
#define USERDATATAG_ANNOTATION			0xA001
#define USERDATATAG_FIRMWAREVERSION		0xA002
#define USERDATATAG_SOFTWAREVERSION		0xA003
#define USERDATATAG_FPDSERIALNO			0xA004
#define USERDATATAG_FILTER				0xA005
#define USERDATATAG_CHECKID				0xA006
#define USERDATATAG_PATIENTID			0xA007
#define USERDATATAG_PATIENTNAME			0xA008
#define USERDATATAG_PATIENTGENDER		0xA009
#define USERDATATAG_PATIENTAGE			0xA00A
#define USERDATATAG_PATIENTWEIGHT		0xA00B
#define USERDATATAG_PATIENTHEIGHT		0xA00C
#define USERDATATAG_PATIENTSIZE			0xA00D

typedef	enum _TIFF_DataType
{
	TIFF_BYTE = 1,
	TIFF_ASCII = 2,
	TIFF_SHORT = 3,
	TIFF_LONG = 4,
	TIFF_RATIONAL =5, // 分数,前4Bytes是分子
	TIFF_FLOAT	= 11,	/* !32-bit IEEE floating point */
	TIFF_DOUBLE	= 12,	/* !64-bit IEEE floating point */
}TIFFDataType;

#endif
