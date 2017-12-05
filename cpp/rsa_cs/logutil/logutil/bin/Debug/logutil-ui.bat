logutil.exe -decrypt -inkey log.key -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
rmdir /Q/S ARMLOG
mkdir ARMLOG
7z.exe e ARMLog.tbz -oARMLOG
del ARMLog.tbz