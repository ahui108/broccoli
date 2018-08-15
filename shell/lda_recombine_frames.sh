#!/bin/bash

file_path=$1
frame_count=$2

help()
{
    echo "USAGE: lda_recombine_frame <file_path> <frame_count> [file1] [file2]"
    exit 0
}

if [ -z ${file_path} ]; then
    echo "Error: <file_path> is not given"
    help
elif [ ! -f ${file_path} ]; then
    echo "Error: file '${file_path}' does not exist"
    help
fi

if [ -z ${frame_count} ]; then
    echo "Error: <frame_count> is not given"
    help
elif [ ${frame_count} -le 0 ]; then
    echo "Error: invalid <frame_count>"
    help
fi

img_high="lda_h.raw"
img_low="lda_l.raw"
if [ -n "$3" ]; then
    img_high=$3
fi
if [ -n "$4" ]; then
    img_low=$4
fi

rm lda-* 2>/dev/null
rm ${img_high} ${img_low} 2>/dev/null

#split-1
echo "Splitting phase-1..."
split -a 2 -n $frame_count -d $file_path lda-

#split-2
echo "Splitting phase-2..."
for((i=0; i<${frame_count}; i++))
do
    img_idx=`printf %02d $i`
    split -a 1 -n 2 -d lda-${img_idx} lda-${img_idx}-
done

#combine
echo "Recombing to new frames..."
for((i=0; i<${frame_count}; i++))
do
    img_idx=`printf %02d $i`
    cat ./lda-${img_idx}-0 >> ${img_high}
    cat ./lda-${img_idx}-1 >> ${img_low}
done

echo "New frames are created: "${img_high}, ${img_low}
rm lda-* -f
    
