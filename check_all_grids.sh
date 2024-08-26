#!/bin/bash

module load prgenv/intel intel/2021.4.0 hpcx-openmpi/2.9.0 hdf5-parallel/1.12.2 netcdf4-parallel/4.9.1

export LD_LIBRARY_PATH=/usr/local/apps/netcdf4-parallel/4.9.1/INTEL/2021.4/HPCX/2.9/lib:$LD_LIBRARY_PATH

# Check if user enters two paths
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <var1> <var2>"
  exit 1
fi

var1=$1
var2=$2

# Run the four scripts for all grids
./compare_nc_files.sh "$var1" "$var2"
./compare_nc_files2.sh "$var1" "$var2" 
./compare_nc_files3.sh "$var1" "$var2"
./compare_nc_files4.sh "$var1" "$var2"

echo "All scripts executed successfully."
