import matplotlib.pyplot as plt
import numpy as np

# path to our collected data and folder to save figures
pathToParallelData = "testing/data/parallel/"
pathToSerialData = "testing/data/serial/"
pathToFigs = "testing/figures/serial-vs-parallel/"

# process text files, plot them, and calculate mean
def process():
    # serial data
    S = []
    with open(pathToSerialData+fileNameSer) as f:
        lines = f.readlines()
    for line in lines:
        S.append(float(line))
    plt.plot(S,linewidth=2,label="Serial: {:.3e}".format(np.mean(S)))#,c="#E26D5C"

    # parallel data
    P = []
    with open(pathToParallelData+fileNamePar) as f:
        lines = f.readlines()
    for line in lines:
        P.append(float(line))
    plt.plot(P,linewidth=2,label="Parallel: {:.3e}".format(np.mean(P)))#,c="#4D7EA8"
    


for fileSize in [100,1000,10000,100000,1000000,10000000]:
    for width in [7,15,21]:
        fileNameSer = "{}/{}-{}-out.txt".format(fileSize,fileSize,width)
        # use 2 thread parallel data (100 - 100 000)
        if (fileSize <= 100000):
            fileNamePar = "{}/{}-{}-{}-out.txt".format(fileSize,fileSize,width,2) 
        # use 8 thread parallel data (1 000 000)
        elif(fileSize == 1000000):
            fileNamePar = "{}/{}-{}-{}-out.txt".format(fileSize,fileSize,width,8) 
        # use 16 thread parallel data (10 000 000)
        elif(fileSize == 10000000):
            fileNamePar = "{}/{}-{}-{}-out.txt".format(fileSize,fileSize,width,16) 
        process()
        # clean up plot
        plt.legend(loc="best")
        plt.ylabel("Time (ns)")
        plt.xlabel("Test number")
        plt.title("Performance of parallel vs serial (Data size: {}, Filter width: {})".format(fileSize,width), pad=16)
        plt.savefig(pathToFigs+"{}-{}-out.jpg".format(fileSize,width),dpi=300)
        plt.clf()