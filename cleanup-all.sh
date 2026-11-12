#!/bin/bash
set -e
set -o pipefail
cd "$(dirname "$0")"

echo "### Step 1/2: cleanup kernel"
./uClinux/cleanup.sh

echo "### Step 2/2: cleanup user programs"
./user/cleanup.sh

