#!/usr/bin/python3

import sys
from launchpadlib.launchpad import Launchpad
from datetime import date

### This Python script prints currently supported Ubuntu versions
### Note: it isn't 100 % accurate, especially on LTS releases
launchpad = Launchpad.login_anonymously('get-supported-releases', 'production') # login
currentYear = date.today().year
for series in launchpad.projects['ubuntu'].series:
	# Launchpad seems to support LTS releases after their EOL, so at least compare the years
	if series.supported and currentYear - series.datereleased.year <= 5:
		print(series.name)
