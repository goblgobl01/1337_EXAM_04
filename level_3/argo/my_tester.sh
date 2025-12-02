#!/bin/bash

set -e

GREEN='\033[1;32m'
RED='\033[1;31m'
CYAN='\033[1;36m'
NC='\033[0m'

BIN=./argo

if [ ! -x "$BIN" ]; then
    echo -e "${RED}Error:${NC} $BIN not found or not executable"
    exit 1
fi

run_test() {
    local input="$1"
    local expected="$2"
    local result
    result=$(/bin/echo -n "$input" | $BIN /dev/stdin | cat -e)

    if [ "$result" == "$expected" ]; then
        echo -e "${GREEN}[OK]${NC} Input: $CYAN$input${NC}"
    else
        echo -e "${RED}[FAIL]${NC} Input: $CYAN$input${NC}"
        echo " Expected: $expected"
        echo " Got:      $result"
    fi
}

echo -e "${CYAN}=== Running argo tests ===${NC}"

# Valid JSON
run_test '1' '1$'
run_test '"hello"' '"hello"$'
run_test '"hello world"' '"hello world"$'
run_test '"escape the \" char"' '"escape the \" char"$'
run_test '{"tomatoes":42,"potatoes":234}' '{"tomatoes":42,"potatoes":234}$'
run_test '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}$'

# Invalid JSON
run_test '"unfinished string' 'Unexpected end of input$'
run_test '"unfinished string 2\"' 'Unexpected end of input$'
run_test '{"no value?":}' "Unexpected token '}'$"
run_test '{ "spaces" : "between_tokens" }' "Unexpected token ' '$"

echo -e "\n${CYAN}=== Tests finished ===${NC}"
