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

CPSLDIR=../ #where cpsl compiler binary lives
BINARY=cpsl #binary name
ASM=asm/ #tmp directory for asm files for mars to run

MARSDIR=./
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

    echo -n "Executing: ${file}..."
    java -Djava.awt.headless=true -jar ${MARSDIR}${MARSJAR} me ic nc 1000000 ${ASM}${file} 2> stderr.txt > ${RESULTS}${file}
    if [ $? -ne 0 ]; then
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
        echo $diff
        echo
    fi
done
rm -f stderr.txt
exit $ret
