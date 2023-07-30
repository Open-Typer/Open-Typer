#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad
from datetime import date

### This Python script prints currently supported Ubuntu versions
launchpad = Launchpad.login_anonymously('get-supported-releases', 'production') # login
for series in launchpad.projects['ubuntu'].series:
	# Support Ubuntu 22.04 or later
	if series.supported and series.datereleased.year >= 2022:
		print(series.name)
