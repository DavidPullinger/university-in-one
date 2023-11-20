# do test for each file size
for fileSize in {100,1000,10000,100000}; do
    # for a small filterwidth, mid filterwidth and large filterwidth
    for width in {7,15,21}; do
        # try threads ranging from 2 to 16
        for threads in {2,4,8,12,16}; do
            for i in {1..30}; do
                make run-par in=sampleInput$fileSize width=$width cutoff=$(($fileSize / $threads)) out=$fileSize-$width-$threads-out.txt
            done
        done
    done
done
