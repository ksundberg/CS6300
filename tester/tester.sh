#!/bin/bash

on_die()
{
  echo
  exit 1;
}

cd `dirname "$0"`

TESTDIR=TestFiles/ #test files directory (where all test.cpsl files are)
RESULTS=Result/ #results folder (where to store cpsl run results for comparison)
BASE=Base/ #base folder name (contains results to compare against)

CPSLDIR=../build/ #where cpsl compiler binary lives
BINARY=cpsl #binary name
ASM=asm/ #tmp directory for asm files for mars to run

MARSDIR=~/src/mars/
MARSJAR=Mars4_4.jar

if [ -z $1 ]; then
  pushd . >> /dev/null
  cd ${TESTDIR}
  files=`ls *.cpsl`
  popd >>/dev/null
else
  files=$1
fi

#create these directories if they don't exist already
mkdir -p $ASM $RESULTS

trap on_die SIGINT
trap on_die TERM

ret=0

for file in $files; do

    if [[ ! -f ${TESTDIR}${file} ]]; then
        echo "File '${file}' not found"
        ret=1
        continue
    fi

    ${CPSLDIR}${BINARY} -o ${ASM}${file} ${TESTDIR}${file} 

    if [ $? -ne 0 ]; then
        echo "Error running: ${CPSLDIR}${BINARY} -o ${ASM}${file} ${TESTDIR}${file}"
        ret=1
        continue
    fi

    echo -n "Executing: ${file}"
    java -Djava.awt.headless=true -jar ${MARSDIR}${MARSJAR} nc 1000000 ${ASM}${file} > ${RESULTS}${file}

    if [ $? -ne 0 ]; then
        echo "Error running: java -jar nc 1000000 ${MARSDIR}${MARSJAR} ${ASM}${file} > ${RESULTS}${file}"
        ret=1
        continue
    fi
    echo "...finished"

    cmp ${RESULTS}${file} ${BASE}${file}
done
exit $ret
