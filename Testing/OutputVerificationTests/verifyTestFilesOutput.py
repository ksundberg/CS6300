"""
To use this script make sure that the cpsl compiler has been built.
Once built simply run the following from the OutputVerificationTests directory.
python verifyTestFilesOutput PATH_TO_MARS

The script currently does not support files that request input from the console.

Features:
    - Verifies that the output of a .cpsl file matches the expected output defined in a file with the name cpslFileNameResults.txt
    - Gives a print out at the end indicating which files have passed/failed
    - If either the cpsl file or the output file contain more lines than the other, then the script will output those lines which
      are missing from the appropriate file.

"""
import argparse
import os
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("marspath", help="The path to a Mars.jar file")
parser.add_argument("-pm", "--pmars", help="Writes mars output to the console", action="store_true")
parser.add_argument("-v", "--verbose", help="Prints additional details during matching", action="store_true")
args = parser.parse_args()


def printMissingContaining(lOut, sOut, isMissing):
    for ln, ele in enumerate(lOut, start=1):
        if ele not in sOut:
            if isMissing:
                print "\tMissing:   " + str(ln) + ": " + ele
            else:
                print "\tAddition:  " + str(ln) + ": " + ele
        else:
            print "\tContains:  " + str(ln) + ": " + ele
            sOut.remove(ele)


def doMatchingEqualLen(correctOutput, sourceOutput):
    print "The length of the program's output is as expected"
    for i, e in enumerate(correctOutput):
        if e != sourceOutput[i]:
            print ("Invalid output at line " + str(i + 1) + " when comparing program output " + sourceOutput[i]
                   + " with correct output " + e)
            return False
        elif args.verbose:
            print "\tThe line " + str(i + 1) + ": " + sourceOutput[i] + " is as expected"

    return True


def doMatching(sourceOutput, correctOutput, path):
    lenOfSource = len(sourceOutput)
    lenOfCorrect = len(correctOutput)

    if lenOfSource != lenOfCorrect:
        print "The length of the program's output is " + str(lenOfSource) + " but expected a length of " + str(lenOfCorrect)
        if lenOfSource > lenOfCorrect:
            print "Output of " + path + " is longer than the expected"
            printMissingContaining(sourceOutput, correctOutput, False)
        else:
            print "Output of " + path + " is shorter than expected"
            printMissingContaining(correctOutput, sourceOutput, True)
        return False
    else:
        return doMatchingEqualLen(correctOutput, sourceOutput)


def generateCorrectOutputList(correctOutputFName):
    correctOutput = []
    try:
        with open(correctOutputFName) as f:
            for line in f:
                correctOutput.append(line.rstrip("\n"))
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror) + " " + correctOutputFName
    return correctOutput


def printPassFailLst(passLst, failLst):
    if passLst:
        print "Files that passed"
        for ele in passLst:
            print "\t" + ele

    if failLst:
        print "Files that did not pass"
        for ele in failLst:
            print "\t" + ele


def attemptCompiling(path):
    print "Attempting to compile " + path
    compilerOutput = subprocess.check_output(["../.././cpsl", path])

    if not compilerOutput:
        print "Successfully compiled " + path + "\n"
        return True
    else:
        print "Failed to compile " + path + " with error " + compilerOutput + "\n"
        return False


def executeMars():
    env = dict(os.environ)
    env['JAVA_OPTS'] = 'cpslmars'

    print "Attempting to execute asm file in mars"
    s = subprocess.check_output(["java", "-jar", args.marspath, "out.asm"], env=env)

    if args.pmars:
        print s
    print "Finished executing asm file in mars\n"

    return s


def start():
    passLst = []
    failLst = []
    for subdir, dirs, files in os.walk('CpslFiles'):
        for f in files:
            if f[-4:] != "cpsl":
                continue

            path = os.path.join(subdir, f)
            print "---------------------------------------------------------------------------"

            if attemptCompiling(path):
                correctOutputFName = "TestFilesCorrectOutput/" + f[0:-5] + "Results.txt"

                sourceOutput = executeMars().split("\n")[2:-1]
                correctOutput = generateCorrectOutputList(correctOutputFName)

                if not correctOutput:
                    failLst.append(f)
                    continue

                print "Beginning comparison of " + path + " with " + correctOutputFName

                if doMatching(sourceOutput, correctOutput, path):
                    print "The source program's output is as expected"
                    passLst.append(f)
                else:
                    print "The source program's output does not match the expected output"
                    failLst.append(f)

                print "Finished comparison of " + path + "\n"
            else:
                failLst.append(f)

            print "---------------------------------------------------------------------------\n"

    printPassFailLst(passLst, failLst)

if __name__ == "__main__":
    start()
