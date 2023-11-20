import matplotlib.pyplot as plt
import numpy as np

# path to our collected data and folder to save figures
pathToParallelData = "testing/data/parallel/"
pathToFigs = "testing/figures/parallel-threads/"

# process text file, plot it, and calculate mean
def process(thread, fileSize):
    P = []
    with open(pathToParallelData+fileName) as f:
        lines = f.readlines()
    for line in lines:
        P.append(float(line))
    plt.plot(P,linewidth=2,label="{} threads: {:.3e} ns".format(thread,np.mean(P),3))
    
# loop through text files
for fileSize in [100,1000,10000,100000,1000000,10000000]:
    for width in [7,15,21]:
        for thread in [2,4,8,12,16]:
            fileName = "{}/{}-{}-{}-out.txt".format(fileSize,fileSize,width,thread)
            process(thread, fileSize)
        # clean up plot
        plt.legend(loc="best")
        plt.ylabel("Time (ns)")
        plt.xlabel("Test number")
        plt.title("Performance of different threads (Data size: {}, Filter width: {})".format(fileSize,width), pad=16)
        plt.savefig(pathToFigs+"{}-{}-out.jpg".format(fileSize,width),dpi=300)
        plt.clf()