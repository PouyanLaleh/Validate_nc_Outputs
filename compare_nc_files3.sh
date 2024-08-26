#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <path1> <path2>"
  exit 1
fi

# GET Ref and Modified PATHs
PATH1=$1
PATH2=$2

# Constant values
COMPARE_BINARY="./compare_nc"
PATTERN_PREFIX="ORCA_10d_20200120_20200129_grid_U"
NUMBER_OF_FILES=32
VARIABLES="depthu nav_lat nav_lon sdzocrtx time_counter time_counter_bnds vozocrtx"

# Red color
RED="\033[31m"
# Reset color
RESET="\033[0m"

        # echo -e "${RED}'    ._      ._      ._      ._             ._    '"
        # echo -e "${RED}      _.-._)\_\.-._)\_\.-._)\_\.-._)\_\.-._)\_\_ ${RESET}"
	# echo -e "${RED}'                                                '${RESET}"
        # echo -e "${RED}'           o         _,           _,            '${RESET}"
        # echo -e "${RED}'            o      .' (        .-' /            '${RESET}"
        # echo -e "${RED}'           o     _/..._'.    .'   /             '${RESET}"
        # echo -e "${RED}      (    o .-'`      ` '-./  _.'              ${RESET}"
	# echo -e "${RED}'       )    ( o)           ;= <_         (      '${RESET}"
        # echo -e "${RED}'      (      '-.,\\__ __.-;`\   '.        )     '${RESET}"
	# echo -e "${RED}'       )  ) jgs                     `    (   (  '${RESET}"
        # echo -e "${RED}'     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ '${RESET}"
        # echo -e "${RED}'         src : NEMO/src/OCE/nemogem            '${RESET}"


print_box() {
  local msg=$1
  local length=$(( ${#msg} + 4 ))

  printf "%${length}s\n" | tr ' ' '*'

  printf "* %s *\n" "$msg"

  printf "%${length}s\n" | tr ' ' '*'
}

# Track FLAG for comparison status
ALL_IDENTICAL=true

# Iterate over the all nc files (e.g., 0000 to 0031)
for i in $(seq -f "%04g" 0 $((NUMBER_OF_FILES-1))); do
  FILENAME="${PATTERN_PREFIX}_${i}.nc"
  FILE1="$PATH1/$FILENAME"
  FILE2="$PATH2/$FILENAME"

  # Check if both files exist
  if [ -f "$FILE1" ] && [ -f "$FILE2" ]; then
    # Run the comparison binary with the files and variables
    $COMPARE_BINARY "$FILE1" "$FILE2" $VARIABLES
    
    # Check the exit status of the comparison
    if [ $? -ne 1 ]; then
      echo "Comparison failed for $FILENAME"
      ALL_IDENTICAL=false
    fi
  else
    echo "File $FILE1 or $FILE2 does not exist."
    ALL_IDENTICAL=false
  fi
done

# Print the result based on the comparison status
if $ALL_IDENTICAL; then

  print_box "Pass"
else
  print_box "Fail"
fi

