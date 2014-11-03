"""
To use this script make sure that the cpsl compiler has been built.
Once built simply run the following from the OutputVerificationTests directory.
python verifyTestFilesOutput PATH_TO_MARS

The script currently does not support files that request input from the console.

Features:
    - Verifies that the output of a .cpsl file matches the expected output defined in a file with the name cpslFileNameResults.txt
    - Provides a print out of the first line which does not match
    - Gives a print out at the end indicating which files have passed/failed
    - If either the cpsl file or the output file contain more lines than the other, then the script will output those lines which
      are missing from the appropriate file.

"""
import argparse
import os
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("marspath", help="The path to a Mars.jar file")
args = parser.parse_args()


def printMissingContents(msg, lOut, sOut):
    print msg
    for ln, ele in enumerate(lOut, start=1):
        if ele not in sOut:
            print "\t" + str(ln) + ": " + ele
        else:
            sOut.remove(ele)


def doMatching(sourceOutput, correctOutput, correctOutputFName, path):
    lenOfSource = len(sourceOutput)
    lenOfCorrect = len(correctOutput)
    isMatch = True
    if lenOfSource != lenOfCorrect:
        isMatch = False
        print "The length of the program's output is " + str(lenOfSource) + " but expected a length of " + str(lenOfCorrect)
        if lenOfSource > lenOfCorrect:
            printMissingContents("Contents of " + path + " missing from correct output: " + correctOutputFName, sourceOutput, correctOutput)
        else:
            printMissingContents("Contents of " + correctOutputFName + " missing from program's output: " + path, correctOutput, sourceOutput)
    else:
        print "The length of the program's output is as expected"
        for i, e in enumerate(correctOutput):
            if e != sourceOutput[i]:
                isMatch = False
                print ("Invalid output at line " + str(i + 1) + " when comparing program output " + sourceOutput[i]
                       + " with correct output " + e)
                break
            else:
                print "\tThe line " + str(i + 1) + ": " + sourceOutput[i] + " is as expected"

    return isMatch


def generateCorrectOutputList(correctOutputFName):
    correctOutput = []
    try:
        with open(correctOutputFName) as f:
            for line in f:
                correctOutput.append(line.rstrip("\n"))
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror) + " " + correctOutputFName
    return correctOutput


def start():
    passLst = []
    failLst = []
    for subdir, dirs, files in os.walk('CpslFiles'):
        for f in files:
            path = os.path.join(subdir, f)
            print "---------------------------------------------------------------------------"
            print "Attempting to compile " + path
            compilerOutput = subprocess.check_output(["../.././cpsl", path])
            if not compilerOutput:
                print "Successfully compiled " + path + "\n"
                env = dict(os.environ)
                env['JAVA_OPTS'] = 'cpslmars'
                print "Attempting to execute asm file in mars"
                s = subprocess.check_output(["java", "-jar", args.marspath, "out.asm"], env=env)
                print "Finished executing asm file in mars\n"

                correctOutputFName = "TestFilesCorrectOutput/" + f[0:-5] + "Results.txt"

                sourceOutput = s.split("\n")[2:-1]
                correctOutput = generateCorrectOutputList(correctOutputFName)

                print "Beginning comparison of " + path + " with " + correctOutputFName
                if doMatching(sourceOutput, correctOutput, correctOutputFName, path):
                    print "The source program's output is as expected"
                    passLst.append(f)
                else:
                    print "The source program's output does not match the expected output"
                    failLst.append(f)

                print "Finished comparison of " + path + "\n"
            else:
                print "Failed to compile " + path + " with error " + compilerOutput + "\n"
            print "---------------------------------------------------------------------------\n"

    if passLst:
        print "Files that passed"
        for ele in passLst:
            print "\t" +  ele

    if failLst:
        print "Files that did not pass"
        for ele in failLst:
            print "\t" + ele

if __name__ == "__main__":
    start()
