#!/bin/bash

# check for command line arguments
if [ $# -lt 3 ]; then
    echo "Usage $0 <U> <I> <# s> [<min permutation> <max permutation>]"
    exit
elif [ $# -gt 5 ]; then
    echo "Usage $0 <U> <I> <# s> [<min permutation> <max permutation>]"
    exit
fi

EXEC=./exhaustive
BM=../benchmarks/mwd

FP_ITER=100
SAMPLES=10000

MIN=1
MAX=11664
if [ $# -eq 5 ]; then
    MIN=$4
    MAX=$5
fi

U=$1
I=$2
CFG=$3
NP=9
NM=3

if [ $CFG -eq 3 ]; then
    PARAM="-h 59.75 -a 1 -f $FP_ITER -s $SAMPLES"
    #PARAM="-a 1 -f 1000 -s 1"
elif [ $CFG -eq 4 ]; then
    PARAM="-h 57.25 -a 1 -f $FP_ITER -s $SAMPLES"
    #PARAM="-a 1 -f 1000 -s 1"
else 
    echo "Invalid configuration specified"
    exit
fi

BLND="M3 ARM9 ARM11"
HS="M3 ARM9 ARM11"
HVS="M3 ARM9 ARM11"
IN="M3 ARM9 ARM11"
JUG1="ARM11"
JUG2="ARM9 ARM11"
NR="ARM11"
SE="M3 ARM9 ARM11"
VS="M3 ARM9 ARM11"

MEM1="MEM1MB MEM2MB"
MEM2="MEM1MB MEM2MB"
MEM3="MEM1MB MEM2MB"

COUNT=1
for mem3 in $MEM3; do
for mem2 in $MEM2; do
for mem1 in $MEM1; do
for vs in $VS; do
for se in $SE; do
for nr in $NR; do
for jug2 in $JUG2; do
for jug1 in $JUG1; do
for in in $IN; do
for hvs in $HVS; do
for hs in $HS; do
for blnd in $BLND; do

if [ $COUNT -ge $MIN ]; then
    if [ $COUNT -le $MAX ]; then
	$EXEC -u $U -i $I $PARAM \
	    -p $NP $blnd $hs $hvs $in $jug1 $jug2 $nr $se $vs \
	    -m $NM $mem1 $mem2 $mem3 \
	    -c $BM/config/$CFG-s.cfg \
	    -n $BM/fp/$CFG-s.nets \
	    -t $BM/mwd.tg	
    fi
fi

let COUNT=COUNT+1
done
done
done
done
done
done
done
done
done
done
done
done
