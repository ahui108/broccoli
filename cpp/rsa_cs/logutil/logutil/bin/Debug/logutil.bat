logutil.exe -decrypt -inkey log.key -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
rmdir /Q/S ARMLOG
mkdir ARMLOG
7z.exe e -tgzip ARMLog.tbz -oARMLOG
7z.exe e -ttar ARMLOG/ARMLog -oARMLOG
logutil.exe -decrypt -inkey logdg.key -in ARMLOG\diags.log -out ARMLOG\diags.tbz
mkdir ARMLOG\diags_log
7z.exe e -tgzip ARMLOG\diags.tbz -oARMLOG\diags_log
7z.exe e -ttar ARMLOG\diags_log\diags -oARMLOG\diags_log

del ARMLog.tbz
del ARMLOG\ARMLog
del ARMLOG\diags.log
del ARMLOG\diags.tbz
del ARMLOG\diags_log\diags