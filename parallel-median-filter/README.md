# parallel-median-filter

## Description

Median filtering is a digital filtering technique which removes noise
(corrupted, or distorted data) from a dataset. My objective was to parallelize this potentially resource intensive function.

## Dealing with folder structure

### Output from median filters (serial and parallel)

The output of the serial and parallel median filters is saved in out/data/parallel and out/data/serial respectively (as per the Makefile). There is also a script directory in this directory which contains 3 scripts: 2 bash scripts which run the serial and parallel median filter applications with various parameters and 1 python script which calculates the variance of the data in the input files and the processed data that been median filtered.

### Testing from median filters (serial and parallel)

When testing the applications, all data was saved in the testing directory. It has 3 directories: data, figures, and scripts. There are 4 scripts in the scripts directory, 2 bash and 2 python. The 2 bash scripts run the serial and parallel median filter applications with various parameters and save the execution times to the data directory. The 2 python scripts process these execution times and produce figures comparing the execution times firstly between numbers of threads (parallel-threads.py) and secondly between the serial and parallel versions of the median filter (serial-vs-parallel.py)
