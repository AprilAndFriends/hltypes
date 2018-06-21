import os
import shutil
import sys
import time

VERSION = "1.0"

NAMESPACE_MACRO = "__HTEST_LIB_NAME"
TEST_CLASS_MACRO = "HTEST_CLASS"
TEST_FUNCTION_MACRO = "HTEST_FUNCTION"
RUN_CLASS_BEGIN_MACRO = "HTEST_RUN_CLASS_BEGIN"
RUN_CLASS_END_MACRO = "HTEST_RUN_CLASS_END"
RUN_METHOD_MACRO = "HTEST_RUN_METHOD"
GENERATED_PATH = "generated"
UNIT_TEST_CPP_HEADER = "htest.h"
UNIT_TEST_CPP_SOURCE = "htest.cpp"
CPP_SOURCE_EXTENSION = ".cpp"

GENERATED_HEADER_TEMPLATE = "#define __HTEST_LIB_NAME %s\n\
#include \"" + UNIT_TEST_CPP_HEADER + "\"\n\
\n\
"
GENERATED_TEST_CLASS_BEGIN_TEMPLATE = TEST_CLASS_MACRO + "(%s)\n\
{\n\
"
GENERATED_TEST_CLASS_END = "}\n\
\n\
"
GENERATED_TEST_FUNCTION_TEMPLATE = "\t" + TEST_FUNCTION_MACRO + "(%s);\n"
GENERATED_RUN_CLASS_BEGIN_TEMPLATE = RUN_CLASS_BEGIN_MACRO + "(%s)\n"
GENERATED_RUN_CLASS_END = RUN_CLASS_END_MACRO + "\n"
GENERATED_RUN_TEST_METHOD_TEMPLATE = "\t" + RUN_METHOD_MACRO + "(%s, %s)\n"

def _readFile(filename):
	f = open(filename, "r")
	data = f.read().replace("\r", "")
	f.close()
	return data

def _writeFile(filename, data):
	if os.path.exists(filename):
		f = open(filename, "r")
		contents = f.read()
		f.close()
		if contents == data:
			print ("  %s - no changes, skipping" % filename)
			return


	f = open(filename, "w")
	f.write(data)
	f.close()

def process():
	print ("-------------------------------------------------------------------------------")
	print ("| H-Unit-Test Generator " + VERSION)
	print ("-------------------------------------------------------------------------------")
	
	if len(sys.argv) > 3:
		help()
		return
	libName = ""
	if len(sys.argv) > 1:
		libName = sys.argv[1]
	sourcePath = "../../unittest"
	if len(sys.argv) > 2:
		sourcePath = sys.argv[2]
	generatedPath = os.path.join(sourcePath, GENERATED_PATH)

	# not clearing dir so we can detect if files need to be updated or not.
	# if os.path.exists(generatedPath):
	# 	shutil.rmtree(generatedPath)
	# 	time.sleep(1)
	# os.mkdir(generatedPath)

	if not os.path.exists(generatedPath):
		os.mkdir(generatedPath)	

	files = []
	print ("Seaching for files in %s" % sourcePath)
	for entry in os.listdir(sourcePath):
		if os.path.isfile(os.path.join(sourcePath, entry)) and entry.endswith(CPP_SOURCE_EXTENSION) and entry != UNIT_TEST_CPP_SOURCE:
			files.append(entry)
	print ("Found files: %d" % len(files))
	for filename in files:
		print ("- %s" % filename)
	for filename in files:
		# parse file
		data = _readFile(os.path.join(sourcePath, filename))
		if data != "":
			print ("")
			activeLibName = libName
			if activeLibName == "":
				index = data.find(NAMESPACE_MACRO)
				if index >= 0:
					index += len(NAMESPACE_MACRO)
					activeLibName = data[index:data.find("\n", index)].strip()
			print ("  Processing %s" % filename)
			if activeLibName == "":
				raise Exception("No namespace defined for %s!" % filename)
			print ("  Namespace: %s" % activeLibName)
			index = data.find(TEST_CLASS_MACRO)
			if index < 0:
				raise Exception("No %s defined in %s!" % (TEST_CLASS_MACRO, filename))
			index += len(TEST_CLASS_MACRO)
			testClass = data[index:data.find("\n", index)].strip("()")
			functions = []
			while True:
				index = data.find(TEST_FUNCTION_MACRO, index)
				if index < 0:
					break
				index += len(TEST_FUNCTION_MACRO)
				functions.append(data[index:data.find("\n", index)].strip("()"))
			print ("  Found functions: %d" % len(functions))
			# generate
			data = ""
			data += GENERATED_HEADER_TEMPLATE % activeLibName
			data += GENERATED_TEST_CLASS_BEGIN_TEMPLATE % testClass
			for function in functions:
				data += GENERATED_TEST_FUNCTION_TEMPLATE % function
			data += GENERATED_TEST_CLASS_END
			data += GENERATED_RUN_CLASS_BEGIN_TEMPLATE % testClass
			for function in functions:
				data += GENERATED_RUN_TEST_METHOD_TEMPLATE % (testClass, function)
			data += GENERATED_RUN_CLASS_END
			print ("  Saving %s" % filename)
			_writeFile(os.path.join(generatedPath, "_" + filename), data)
	print ("")
		
def help():
	print ("")
	print ("Generates unit test C++ files for H-Unit-Test.")
	print ("")
	print ("usage:   generate_tests.py [LIB_NAME [SOURCE_PATH]]")
	print ("")
	print ("  LIB_NAME     - Name of the library. If not specified, will use")
	print ("                 to whatever " + NAMESPACE_MACRO + " was set in source file.")
	print ("  SOURCES_PATH - Directory path for the unit test sources.")
	print ("                 Default: ../../unittest")
	print ("")
	print ("example: generate_tests.py hltypes ../../unittest")
	print ("")
	os.system("pause")

process()
