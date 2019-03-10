#!/bin/bash

on_die()
{
  echo
  exit 1;
}

cd `dirname "${0}"`

TESTDIR=TestFiles/ #test files directory (where all test.cpsl files are)
RESULTS=Result/ #results folder (where to store cpsl run results for comparison)
BASE=Base/ #base folder name (contains results to compare against)

CPSLDIR=${CPSLDIR:-../} #where cpsl compiler binary lives
BINARY=cpsl #binary name
ASM=asm/ #tmp directory for asm files for mars to run

MARSDIR=${MARSDIR:-./}
MARSJAR=Mars4_4.jar

files=`ls ${TESTDIR}${1}*.cpsl`

#create these directories if they don't exist already
mkdir -p ${ASM} ${RESULTS}

trap on_die SIGINT
trap on_die TERM

ret=0

baseTotal=0
outTotal=0
totalPerc=0
count=0
best=900
worst=0

for file in ${files}; do

    file=$(basename ${file})

    if [[ ! -f ${TESTDIR}${file} ]]; then
        echo "File '${file}' not found"
        ret=1
        continue
    fi

    ${CPSLDIR}${BINARY} -c -o ${ASM}${file} -i ${TESTDIR}${file}

    if [ $? -ne 0 ]; then
        echo "Error running: ${CPSLDIR}${BINARY} -o ${ASM}${file} ${TESTDIR}${file}"
        ret=1
        continue
    fi

    echo -n "Executing: ${file}..."
    java -Djava.awt.headless=true -jar ${MARSDIR}${MARSJAR} me ic nc 1000000 ${ASM}${file} 2> stderr.txt > ${RESULTS}${file}
    if [ ! -s ${RESULTS}${file} ]; then
        echo "  Error running: java -jar nc 1000000 ${MARSDIR}${MARSJAR} ${ASM}${file} > ${RESULTS}${file}"
        cat stderr.txt
        ret=1
        continue
    fi

    le=$(cat stderr.txt | grep -oE "[[:digit:]]+")
    be=$(cat ${BASE}count_${file} 2>/dev/null)

    if [ -z "${le}" ]; then
        le="?"
    fi

    if [ -z "${be}" ]; then
        be="?"
    fi

    div=$(awk "BEGIN {printf \"%d\",${le}*100/${be}}" 2>/dev/null)

    if [ $? -ne 0 ]; then
        div="?"
    fi

    echo "finished  ${le}/${be} ${div}%"
    echo ${le} > ${RESULTS}count_${file}

    diff=$(diff ${RESULTS}${file} ${BASE}${file})

    if [ $? -ne 0 ]; then
        echo "  diff ${RESULTS}${file} ${BASE}${file}"
        echo ${diff}
        echo
        ret=1
    fi

    baseTotal=$((baseTotal + be))
    outTotal=$((outTotal + le))
    totalPerc=$((totalPerc + div))
    count=$((count + 1))
    worst=$((worst<div?div:worst))
    best=$((best>div?div:best))

done
rm -f stderr.txt

if [ $ret -ne 1 ]; then
    echo
    echo "No Errors Found"
    echo
    totalDiv=$(((outTotal*100)/baseTotal))
    echo "Total Instructions: ${outTotal}/${baseTotal} ${totalDiv}%"
    avgPerc=$((totalPerc/count))
    echo "Average of Percentages: ${totalPerc}%/${count} ${avgPerc}%"
    echo "Best Improvement Percentage: ${best}%"
    echo "Worst Improvement Percentage: ${worst}%"
else
    echo
    echo "Errors Found"
fi

exit ${ret}
