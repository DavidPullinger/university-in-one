import numpy as np

# path to our collected data
pathToSerialData = "out/data/serial/"
pathToInitialData = "data/"

# process median filtered text files 
def process(fileName, width):
    # data
    temp = []
    if (width == 0):
        with open(pathToInitialData+fileName) as f:
            next(f) #ignore first line
            lines = f.readlines()
        for line in lines:
            temp.append(float(line.split()[1].replace(',','.')))
        print("INITIAL: {}".format(np.var(temp)))
    else:
        with open(pathToSerialData+fileName) as f:
            next(f) #ignore first line
            lines = f.readlines()
        for line in lines:
            temp.append(float(line.split()[1].replace(',','.')))
        print("WIDTH {}: {}".format(width,np.var(temp)))


for fileSize in [1000000,10000000]:
    print(fileSize)
    preFileName = "sampleInput{}.txt".format(fileSize)
    process(preFileName, 0)
    for width in [7,15,21]:
        postFileName = "{}-{}-out.txt".format(fileSize,width)
        process(postFileName,width)
        

