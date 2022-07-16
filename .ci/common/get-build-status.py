#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad

### This Python script checks if the given version has been successfully built on Launchpad
launchpad = Launchpad.login_anonymously('fetch-build-status', 'production') # login
archive = launchpad.load('/~adazem009/+archive/ubuntu/open-typer') # get PPA archive
found = False
records = archive.getBuildRecords(source_name='open-typer')
for build in records:
	if sys.argv[1] in build.source_package_version:
		if "Success" in build.buildstate:
			found = True
		else:
			sys.exit(1)
if found:
	sys.exit(0)
else:
	sys.exit(2)
