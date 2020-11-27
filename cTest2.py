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
SMALL_LINE = "--------------------"
BIG_LINE = SMALL_LINE * 3

def runSingleTest(testFilePath):
    test_file = open(testFilePath, 'r')
    test_JSON = test_file.read()
    test_file.close()
    test = json.loads(test_JSON)
    
    print(BIG_LINE)
    print(testFilePath, test['testIdentifier'])
    print(BIG_LINE)
    try:
        test_input_file = open(test['testInput'])
        build_opts = ['g++', '-O3', '-o', 'testCompiled'] + test['compilationFiles']        
        compilation_process = subprocess.run(build_opts, stdin=test_input_file, capture_output=True)
        print(compilation_process.stderr.decode('utf-8'))
    except Exception as e:
        print(f'There was a problem running this test. The reason given was: {e}')

def runTestsDir(directory):
    for testDescriptor in os.listdir(directory):
        fullPath = directory + testDescriptor
        if os.path.isdir(fullPath):
            print(f"Recursively looking for tests in {fullPath}")
            runTestsDir(fullPath)
        elif testDescriptor.endswith(".json"):
            runSingleTest(fullPath)
        #  else ignore non-test files
            

runTestsDir("./tests/")

