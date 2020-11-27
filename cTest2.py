'''
For cTest2:

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

def runSingleTest(testFilePath):
    test_file = open(testFilePath, 'r')
    test_JSON = test_file.read()
    test_file.close()
    test = json.loads(test_JSON)
    
    print(test)

def runTestsDir(directory):
    for testDescriptor in os.listdir(directory):
        fullPath = directory + testDescriptor
        if os.path.isdir(fullPath):
            print(f"Recursively looking for tests in {fullPath}")
            runTestsDir(fullPath)
        elif testDescriptor.endswith(".json"):
            runSingleTest(fullPath)
            print("test")
        #  else ignore non-test files
            

runTestsDir("./tests/")

