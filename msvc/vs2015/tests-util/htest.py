import os
import sys
import subprocess

VSTEST_PATH = "C:\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow\\vstest.console.exe"
binPath = sys.argv[1]
testDlls = {}

for root, dirs, files in os.walk(binPath):
	for file in files:
		if file.endswith("_tests.dll"):
			testDlls[file] = os.path.join(root, file)

for name, path in testDlls.items():
	print "*", name
	if subprocess.call([VSTEST_PATH, "/Platform:x86", path]) != 0:
		print "UNIT TEST ERROR, aborting further tests"
		sys.exit(1)
