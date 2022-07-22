#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad

### This Python script gets the URLs of packages of the given version
# ./get-package-urls.py <launchpad name> <ppa name> <version>
launchpad = Launchpad.login_anonymously('fetch-build-status', 'production') # login
archive = launchpad.load('/~' + sys.argv[1] + '/+archive/ubuntu/' + sys.argv[2]) # get PPA archive
sources = archive.getPublishedSources()
for source in sources:
	if sys.argv[3] in source.source_package_version:
		urls = source.binaryFileUrls()
		for url in urls:
			print(url)
