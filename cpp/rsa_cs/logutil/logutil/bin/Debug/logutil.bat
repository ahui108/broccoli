logutil.exe -decrypt -inkey log.key -in ARMLog.INFO.tar.bz2 -out ARMLog.tbz
rmdir /Q/S ARMLOG
mkdir ARMLOG
7z.exe e ARMLog.tbz -oARMLOG
logutil.exe -decrypt -inkey logdg.key -in ARMLOG\diags.log -out ARMLOG\diags.tbz
mkdir ARMLOG\diags_log
7z.exe e ARMLOG\diags.tbz -oARMLOG\diags_log
del ARMLog.tbz
del ARMLOG\diags.log
del ARMLOG\diags.tbz