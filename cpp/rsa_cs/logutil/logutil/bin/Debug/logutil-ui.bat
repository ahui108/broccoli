logutil.exe -decrypt -inkey log.key -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
rmdir /Q/S ARMLOG
mkdir ARMLOG
7z.exe e -tgzip ARMLog.tbz -oARMLOG
7z.exe e -ttar ARMLOG\ARMLog -oARMLOG
del ARMLog.tbz
del ARMLOG\ARMLog