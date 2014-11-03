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
    passFailLst = []
    for subdir, dirs, files in os.walk('CpslFiles'):
        for f in files:
            path = os.path.join(subdir, f)
            print "---------------------------------------------------------------------------"
            print "Attempting to compile " + path
            compilerOutput = subprocess.check_output(["../.././cpsl", path])
            if not compilerOutput:
                print "Successfully compiled " + path
                env = dict(os.environ)
                env['JAVA_OPTS'] = 'cpslmars'
                s = subprocess.check_output(["java", "-jar", args.marspath, "out.asm"], env=env)

                correctOutputFName = "TestFilesCorrectOutput/" + f[0:-5] + "Results.txt"

                sourceOutput = s.split("\n")[2:-1]
                correctOutput = generateCorrectOutputList(correctOutputFName)

                print "Beginning comparison of " + path + " with " + correctOutputFName
                if doMatching(sourceOutput, correctOutput, correctOutputFName, path):
                    print "The source program's output is as expected"
                    passFailLst.append("The output of " + f + " is correct")
                else:
                    print "The source program's output does not match the expected output"
                    passFailLst.append("The output of " + f + " is not correct")

                print "Finished comparison of " + path
            else:
                print "Failed to compile " + path + " with error " + compilerOutput
            print "---------------------------------------------------------------------------\n"

    for ele in passFailLst:
        print ele

if __name__ == "__main__":
    start()
