#!/bin/sh
#awk '{print} END{print "/etc/sg_dma_autoload.sh"}' ./rc.local > ./rc-temp.local
#awk '{print} BEGIN{print "/etc/sg_dma_autoload.sh"}' ./rc.local
#awk '{if($1=="exit" && $2=="0") print "/etc/sg_dma_autoload.sh \nexit 0"; else print}' ./rc.local
#awk 'BEGIN{print "#!/bin/sh \n/etc/sg_dma_autoload.sh"} {print}' ./rc.local
#awk '{if($1~/#!\/bin\/sh/) print "#!/bin/sh \n/etc/sg_dma_autoload.sh"; else print}' ./rc.local
#awk '{if($1~/#!\/bin\/sh/) {print;print "/etc/sg_dma_autoload.sh"} else {print}}' ./rc.local
awk '{if($1~/#!\/bin/) {print;print "/etc/sg_dma_autoload.sh"} else {print}}' ./rc.local

