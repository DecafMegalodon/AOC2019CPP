'''
cTest2 is intended to be a tool to automate testing workflows for groups of
simple programs that use I/O from standard in and out respectively.


in /tests/:
	Will attempt to compile and run tests in .json files.
    File paths should be relative to PWD
    Files should be in JSON format with the following fields.:
		{
		  "testIdentifier": "test name"
		  "compilationFiles": [
			"file1.cpp",
			"file2.cpp" ...
		  ],
		  "testInput": "filePath",
		  "testOutput": "filePath"
		}
'''

import os
import json
import subprocess
from colorama import Fore, Back, Style
SMALL_LINE = "--------------------"
BIG_LINE = SMALL_LINE * 3

#  Runs a single test. -1 for failures, 0 for successes
def runSingleTest(testFilePath):
    test_file = open(testFilePath, 'r')
    test_JSON = test_file.read()
    test_file.close()
    test = json.loads(test_JSON)
    
    
    try:
        test_input_file = open(test['testInput'])
        build_opts = ['g++', '-O3', '-o', 'testCompiled'] + test['compilationFiles']        
        compilation_process = subprocess.run(build_opts, capture_output=True)
        
        test_output_file = open(test['testOutput'])
        expected_output = test_output_file.read().strip()
        test_output_file.close()
        
    except Exception as e:
        print(BIG_LINE)
        print(testFilePath, test['testIdentifier'])        
        print(Fore.RED, f'There was a problem with this test configuration. The reason given was: {e}', Style.RESET_ALL)
        print(BIG_LINE)
        return -1
        
    if compilation_process.returncode != 0: #  compilation failed
        print(BIG_LINE)
        print(testFilePath, test['testIdentifier'])        
        print(Fore.RED, 'This test did not successfully compile. The given message was:', Style.RESET_ALL)
        print(compilation_process.stderr.decode('utf-8'))
        print(BIG_LINE)
        return -1
        
    test_execution_process = subprocess.run('./testCompiled', stdin=test_input_file, capture_output=True)
    test_input_file.close()
    
    test_output = test_execution_process.stdout.decode('utf-8').strip()
    
    if test_output == expected_output:
        print(testFilePath, test['testIdentifier'], Fore.GREEN, '[PASS]', Style.RESET_ALL)
        return 0
    else:
        print(BIG_LINE)
        print(testFilePath, test['testIdentifier'])        
        print(SMALL_LINE)
        print(Fore.RED, 'The program did not produce the expected output', Style.RESET_ALL)
        print("Expected:\n", expected_output)
        print(SMALL_LINE)
        print("Got:\n", test_output)
        print(BIG_LINE, '\n')
    

def runTestsDir(directory):
    for testDescriptor in os.listdir(directory):
        fullPath = directory + testDescriptor
        if os.path.isdir(fullPath):
            #  print(f"Recursively looking for tests in {fullPath}")
            runTestsDir(fullPath+'/')
        elif testDescriptor.endswith(".json"):
            runSingleTest(fullPath)
        #  else ignore non-test files
            

runTestsDir("./tests/")

