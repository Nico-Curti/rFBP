#!/bin/bash

red=$(tput setaf 1)
green=$(tput setaf 2)
reset=$(tput sgr0)

echo "${green}Run testing${reset}"

./bin/test_atanherf
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_fprotocol
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_magp
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_magt
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_params
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_pattern
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

./bin/test_spline
if [[ $? != 0 ]]; then
  echo "${red}ERROR${reset}"
  exit 1;
fi

echo "${green}PASSED${reset}"
