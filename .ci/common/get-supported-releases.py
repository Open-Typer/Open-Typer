#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad
from datetime import date

### This Python script prints currently supported Ubuntu versions
launchpad = Launchpad.login_anonymously('get-supported-releases', 'production') # login
for series in launchpad.projects['ubuntu'].series:
	# We support LTS releases with extended support, but only since Ubuntu 18.04 LTS
	if series.supported and series.datereleased.year >= 2018:
		print(series.name)
