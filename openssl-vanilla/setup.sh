#!/bin/bash

#Clean previous allocations:
rm -rf datax_low.csv datax_medium.csv datax_high.csv

#For LOW
cat datax.csv >> datax_low.csv
cat datax.csv >> datax_low.csv

#For MEDIUM
cat datax_low.csv >> datax_medium.csv
split -b 13000k datax.csv segment
cat segmentaa >> datax_medium.csv
rm -rf segment*

#For HIGH
cat datax_low.csv >> datax_high.csv
cat datax_low.csv >> datax_high.csv
