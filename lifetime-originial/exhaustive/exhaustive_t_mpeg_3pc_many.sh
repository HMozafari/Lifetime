#!/bin/bash

# check for command line arguments
if [ $# -lt 3 ]; then
    echo "Usage $0 <u> <i> <# s> [<min permutation> <max permutation>]"
    exit
elif [ $# -gt 5 ]; then
    echo "Usage $0 <u> <i> <# s> [<min permutation> <max permutation>]"
    exit
fi

EXEC=./exhaustive
BM=../benchmarks/mpeg_3p_constrained

FP_ITER=100
SAMPLES=10000

MIN=1
MAX=142884
if [ $# -eq 5 ]; then
    MIN=$4
    MAX=$5
fi

U=$1
I=$2
CFG=$3
NP=7
NM=4

PARAM="-u $U -i $I -a 1 -f $FP_ITER -s $SAMPLES"

if [ $CFG -eq 4 ]; then
    PARAM="$PARAM -h 64.9395"
elif [ $CFG -eq 5 ]; then
    PARAM="$PARAM -h 61.95"
else
    echo "Invalid configuration specified"
    exit
fi

#PARAM="-u $U -i $I -a 1 -f 100"
#PARAM="-h 38.4253 -u $U -i $I -a 1 -f 100"
#PARAM="-u $U -i $I -a 0 -w 0.4 0.4 -f 100"
#PARAM="-h 38.488 -u $U -i $I -a 0 -w 0.4 0.4 -f 100"
#PARAM="-u $U -i $I -a 0 -w 0.0 1.0 -f 100"
#PARAM="-h 38.5257 -u $U -i $I -a 0 -w 0.0 1.0 -f 100"

BSP="ARM9 ARM11"
DBLK="M3 ARM9 ARM11"
DRNG="ARM11"
PAD1="M3 ARM9 ARM11"
PAD2="M3 ARM9 ARM11"
RCNS="M3 ARM9 ARM11"
SMTXTR="ARM11"

VBV="MEM64KB MEM96KB MEM128KB MEM192KB MEM256KB MEM384KB MEM512KB"
VCV2="MEM64KB MEM96KB MEM128KB MEM192KB MEM256KB MEM384KB MEM512KB"
VCV3="MEM96KB MEM128KB MEM192KB MEM256KB MEM384KB MEM512KB"
VMV="MEM256KB MEM384KB MEM512KB"

COUNT=1

for vmv in $VMV; do
for vcv3 in $VCV3; do
for vcv2 in $VCV2; do
for vbv in $VBV; do
for smtxtr in $SMTXTR; do
for rcns in $RCNS; do
for pad2 in $PAD2; do
for pad1 in $PAD1; do
for drng in $DRNG; do
for dblk in $DBLK; do
for bsp in $BSP; do
 
if [ $COUNT -ge $MIN ]; then
    if [ $COUNT -le $MAX ]; then
	#echo $EXEC $PARAM \
	#    -p $NP $bsp $dblk $drng $pad1 $pad2 $rcns $smtxtr \
	#    -m $NM $vbv $vcv2 $vcv3 $vmv \
	#    -c $BM/config/$CFG-s.cfg \
	#    -n $BM/fp/$CFG-s.nets \
	#    -t $BM/mpeg_3p_constrained.tg
	$EXEC $PARAM \
	    -p $NP $bsp $dblk $drng $pad1 $pad2 $rcns $smtxtr \
	    -m $NM $vbv $vcv2 $vcv3 $vmv \
	    -c $BM/config/$CFG-s.cfg \
	    -n $BM/fp/$CFG-s.nets \
	    -t $BM/mpeg_3p_constrained.tg
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

#echo $COUNT