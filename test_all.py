#  Automated test for AOC 2017 based on my personal puzzle inputs
import fileinput
import subprocess
import time
import os

testsPassed = 0
testsFailed = 0
testsSlow = 0
results = [line.strip() for line in fileinput.input(files=("inputs/results.txt"))]


for day in range(1,3+1):
    paddedDay = ('0' if day < 10 
                else '') + str(day)
    dayInput = open("inputs/day" + paddedDay + ".txt")
    for part in ('a', 'b'):
        if day == 25 and part == 'b': #  No part b on on 25th
            break
        progName = ''.join(['day', paddedDay, part, '.cpp'])
        
        #compile
        compProcess = subprocess.run(args=["g++", progName, '-O3', '-o',  'testCompiled'],
                                stdin=dayInput, capture_output=True)
                                
        compilationSuccessful = (compProcess.returncode == 0)
        
        if compilationSuccessful:
            startTime = time.time()
            subProc = subprocess.run(args=["./testCompiled"],
                                    stdin=dayInput, capture_output=True)
            stopTime = time.time()
            timeTaken = round(stopTime - startTime, 4)
            
            output = str(subProc.stdout.decode('utf-8')).strip()
            expected = results[(day*2) + (-2 if part == 'a' else -1)]
            testCorrect = (output == expected)
            
            result = "OK" if testCorrect else "FAIL"
            if testCorrect and timeTaken > 15:
                result = "SLOW"
            print("D" + paddedDay + part, result, timeTaken, "seconds")
            
            if testCorrect:
                testsPassed += 1
            else:
                testsFailed += 1
                print("----------------------------------------")
                print("Expected: %s, got:" % expected)
                print(output)
                print("----------------------------------------")

            if timeTaken >= 15:
                testsSlow += 1

        else: #  Compilation not successful
            testsFailed += 1
            print("D" + paddedDay + part + " Compilation failed")
            print("----------------------------------------")
            print(compProcess.stderr.decode('utf-8'))
            print("----------------------------------------")
        


        dayInput.seek(0,0)

print("Test results: %d passed, %d failed, %d slow" % (testsPassed, testsFailed, testsSlow))