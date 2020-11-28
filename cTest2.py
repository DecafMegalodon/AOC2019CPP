'''
cTest2 is intended to be a tool to automate testing workflows for groups of
simple programs that use I/O from standard in and out respectively.


in ./tests/:
	Will attempt to compile and run tests in .json files.
    File paths should be relative to PWD
    Files should be in JSON format with the following structure:
    {
      "testFamilyName": "name. Optional if only one test",
      "compilationFiles": [
        "file1.cpp",
        "file2.cpp",
        ...
      ],
      "tests": [
        {
          "testIdentifier": "testName",
          "testInput": "filePath",
          "testOutputType": "simple or file",
          "testOutput": "filePath or value"
        },
        {
          "testIdentifier": "testName",
          "testInput": "filePath",
          "testOutputType": "simple or file",
          "testOutput": "filePath or value"
        },
        ...
      ]
    }
'''

import os
import json
import subprocess
from colorama import Fore, Back, Style
SMALL_LINE = "--------------------"
BIG_LINE = SMALL_LINE * 3

#  Runs a single test. -1 for failures, 0 for successes
def run_test_file(test_file_path):
    test_file = open(test_file_path, 'r')
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
        print(test_file_path, test['testIdentifier'])        
        print(Fore.RED, f'[ERROR] There was a problem with this test configuration. The reason given was: {e}', Style.RESET_ALL)
        print(BIG_LINE)
        return -1


    if compilation_process.returncode != 0: #  compilation failed
        print(BIG_LINE)
        print(test_file_path, test['testIdentifier'])        
        print(Fore.RED, '[FAIL] This test did not successfully compile. The given message was:', Style.RESET_ALL)
        print(compilation_process.stderr.decode('utf-8'))
        print(BIG_LINE)
        return -1
        
    test_execution_process = subprocess.run('./testCompiled', stdin=test_input_file, capture_output=True)
    test_input_file.close()
    
    test_output = test_execution_process.stdout.decode('utf-8').strip()
    
    if test_output == expected_output:
        print(Fore.GREEN, '[PASS]', Style.RESET_ALL, test_file_path, test['testIdentifier'])
        return 0
    else:
        print(BIG_LINE)
        print(test_file_path, test['testIdentifier'])        
        print(SMALL_LINE)
        print(Fore.RED, '[FAIL] The program did not produce the expected output', Style.RESET_ALL)
        print("Expected:\n", expected_output)
        print(SMALL_LINE)
        print("Got:\n", test_output)
        print(BIG_LINE, '\n')
    

def run_tests_dir(directory):
    for test_descriptor in os.listdir(directory):
        full_path = directory + test_descriptor
        if os.path.isdir(full_path):
            #  print(f"Recursively looking for tests in {full_path}")
            run_tests_dir(full_path+'/')
        elif test_descriptor.endswith(".json"):
            run_test_file(full_path)
        #  else ignore non-test files
            

run_tests_dir("./tests/")

