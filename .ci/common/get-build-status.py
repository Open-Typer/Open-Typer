#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad

### This Python script checks if the given version has been successfully built on Launchpad
# ./get-build-status.py <launchpad name> <ppa name> <package name> <version>
launchpad = Launchpad.login_anonymously('fetch-build-status', 'production') # login
archive = launchpad.load('/~' + sys.argv[1] + '/+archive/ubuntu/' + sys.argv[2]) # get PPA archive
found = False
records = archive.getBuildRecords(source_name=sys.argv[3])
for build in records:
	print(sys.argv[4], build.source_package_version)
	if sys.argv[4]+"ubuntu3" in build.source_package_version:
		if "Success" in build.buildstate:
			found = True
		else:
			sys.exit(1)
if found:
	sys.exit(0)
else:
	sys.exit(2)
