# do test for each file size
for fileSize in {100,1000,10000,100000}; do
    # for a small filterwidth, mid filterwidth and large filterwidth
    for width in {7,15,21}; do
        make run-par in=sampleInput$fileSize width=$width cutoff=$(($fileSize / 2)) out=$fileSize-$width-out.txt
    done
done
