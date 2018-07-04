import os
import sys
import subprocess

VSTEST_PATH = "C:\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow\\vstest.console.exe"
binPath = sys.argv[1]
testApps = {}

for root, dirs, files in os.walk(binPath):
	for file in files:
		if "AppPackages" in root and not "Dependencies" in root and file.endswith(".appx"):
			testApps[file] = os.path.join(root, file)

for name, path in testApps.items():
	print "*", name
	certificateName = path.replace(".appx", ".cer")
	if not os.path.exists(certificateName):
		print "CERTIFICATE DOES NOT EXIST, aborting further tests"
		sys.exit(1)
	if subprocess.call(["certutil.exe", "-verify", certificateName]) != 0:
		print "CERTIFICATE INVALID, aborting further tests"
		sys.exit(1)
	if subprocess.call(["certutil.exe", "-addstore", "TrustedPeople", certificateName]) != 0:
		print "CERTIFICATE COULD NOT BE INSTALLED, aborting further tests"
		sys.exit(1)
	if subprocess.call([VSTEST_PATH, "/Platform:x86", "/InIsolation", path]) != 0:
		print "UNIT TEST ERROR, aborting further tests"
		sys.exit(1)
