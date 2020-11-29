'''
Copyright 2020 DecafMegalodon

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
          "testOutput": "filePath or value",
          "testLevel": 0 (lower is a higher priority)
        },
        {
          "testIdentifier": "testName",
          "testInput": "filePath",
          "testOutputType": "simple or file",
          "testOutput": "filePath or value",
          "testLevel": 3
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

#  Runs a single test file. Returns a tuple with (tests_passed, test_failed)
def run_test_file(test_file_path):
    tests_passed = 0
    tests_failed = 0
    test_file = open(test_file_path, 'r')
    test_JSON = test_file.read()
    test_file.close()
    test = json.loads(test_JSON)
    
    try:
        build_opts = ['g++', '-O3', '-o', 'testCompiled'] + test['compilationFiles']        
        compilation_process = subprocess.run(build_opts, capture_output=True)      
    except Exception as e:
        print(BIG_LINE)
        print(test_file_path, test['testIdentifier'])        
        print(Fore.RED, f'[ERROR] There was a problem with this test configuration. The reason given was: {e}', Style.RESET_ALL)
        print(BIG_LINE)
        return (0, len(test['tests'])) #  If the test itself it malformed, this may crash. TODO

    if compilation_process.returncode != 0: #  compilation failed
        print(BIG_LINE)
        print(test_file_path, test['testIdentifier'])        
        print(Fore.RED, '[FAIL] This test did not successfully compile. The given message was:', Style.RESET_ALL)
        print(compilation_process.stderr.decode('utf-8'))
        print(BIG_LINE)
        return (0, len(test['tests']))
        
    for cur_test in test['tests']:
        #todo catch errors in individual tests
        test_input_type = cur_test['testInputType']
        test_output_type = cur_test['testOutputType']
        test_input, test_expected_output = None, None
        
        if test_input_type == 'simple': #  Read input directly from the test file
            test_input = cur_test['testInput']
        else: #  hopefully 'file'
            test_input_file = open(cur_test['testInput'], 'r')
            test_input = test_input_file.read().encode()
            test_input_file.close()

        if test_output_type == 'simple':
            test_expected_output = cur_test['testOutput']
        else:
            test_output_file = open(cur_test['testOutput'], 'r')
            test_expected_output = test_output_file.read().strip()
            test_output_file.close()
        
        test_execution_process = subprocess.run('./testCompiled', input=test_input, capture_output=True)
        test_output = test_execution_process.stdout.decode('utf-8').strip()
        
        full_test_name = f"{test_file_path}: {cur_test['testIdentifier']}"
        
        if test_output == test_expected_output:
            print(Fore.GREEN, '[PASS]', Style.RESET_ALL, full_test_name)
            tests_passed += 1
        else:
            print(BIG_LINE)
            print(Fore.RED, '[FAIL]', Style.RESET_ALL, full_test_name)        
            print(SMALL_LINE)
            print(Fore.RED, 'The program did not produce the expected output', Style.RESET_ALL)
            print("Expected:\n", test_expected_output)
            print(SMALL_LINE)
            print("Got:\n", test_output)
            tests_failed += 1
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

