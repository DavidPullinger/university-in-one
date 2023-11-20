# do test for each file size
for fileSize in {100,1000,10000,100000}; do
    # for a small filterwidth, mid filterwidth and large filterwidth
    for width in {7,15,21}; do
        # run program 30 times
        for i in {1..30}; do
            make run-ser in=sampleInput$fileSize width=$width out=$fileSize-$width-out.txt
        done
    done
done
