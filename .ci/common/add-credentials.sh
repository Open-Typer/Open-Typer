#!/bin/bash

### This scripts adds git credentials to git credential manager
git config --global credential.helper store 'cache --timeout=86400'
rm -f ~/.git-credentials -f
git credential approve <<EOT
url=https://github.com/${1}
username=GitHub Actions Bot
password=${2}
EOT
