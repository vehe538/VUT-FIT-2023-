#!/bin/bash

# IFJ25 Compiler - Master Test Runner
# Runs all test suites and provides comprehensive summary

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Test counters
TOTAL_PASSED=0
TOTAL_FAILED=0
LEXER_PASSED=0
LEXER_FAILED=0
FUNC_PASSED=0
FUNC_FAILED=0
SYNTAX_PASSED=0
SYNTAX_FAILED=0
TYPE_PASSED=0
TYPE_FAILED=0

# Verbose mode
VERBOSE=0
if [ "$1" = "-v" ] || [ "$1" = "--verbose" ]; then
    VERBOSE=1
fi

# Get project root
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT" || exit 1

# Paths
COMPILER="$PROJECT_ROOT/compiler"
LEXER_DIR="$PROJECT_ROOT/tests/lexer/errors"
FUNC_DIR="$PROJECT_ROOT/tests/function_calls"
SYNTAX_DIR="$PROJECT_ROOT/tests/syntax_errors"
TYPE_VALID_DIR="$PROJECT_ROOT/tests/type/valid"
TYPE_ERROR_DIR="$PROJECT_ROOT/tests/type/errors"

# Header
echo ""
echo "=========================================="
echo "   IFJ25 Compiler - Master Test Suite"
echo "=========================================="
echo ""

# Build compiler if needed
echo -e "${BOLD}Checking compiler build...${NC}"
if [ ! -f "$COMPILER" ]; then
    echo "Compiler not found. Building..."
    make clean > /dev/null 2>&1 && make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error: Failed to build compiler${NC}"
        exit 1
    fi
    echo -e "${GREEN}Compiler built successfully${NC}"
else
    echo -e "${GREEN}Compiler found${NC}"
fi
echo ""

# ============================================
# LEXER ERROR TESTS
# ============================================
echo "=========================================="
echo "   Suite 1: Lexer Error Tests"
echo "=========================================="
echo ""

EXPECTED_LEXER_EXIT=1

for test_file in "$LEXER_DIR"/test_*.wren; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .wren)
        echo -n "Testing: $test_name ... "
        
        timeout 5 "$COMPILER" < "$test_file" > ./test_output.txt 2>&1
        EXIT_CODE=$?
        
        if [ $EXIT_CODE -eq $EXPECTED_LEXER_EXIT ]; then
            echo -e "${GREEN}PASS${NC}"
            ((LEXER_PASSED++))
            
            if [ $VERBOSE -eq 1 ]; then
                echo -e "  ${CYAN}Output:${NC}"
                grep "Error" ./test_output.txt | head -3 | sed 's/^/    /'
                echo ""
            fi
        else
            echo -e "${RED}FAIL${NC} (exit code: $EXIT_CODE, expected: $EXPECTED_LEXER_EXIT)"
            ((LEXER_FAILED++))
            
            echo -e "  ${YELLOW}Error:${NC}"
            tail -5 ./test_output.txt | sed 's/^/    /'
            echo ""
        fi
    fi
done

echo ""
echo -e "${BOLD}Lexer Error Tests:${NC} ${GREEN}$LEXER_PASSED passed${NC}, $LEXER_FAILED failed"
echo ""

# ============================================
# FUNCTION CALL TESTS
# ============================================
echo "=========================================="
echo "   Suite 2: Function Call Tests"
echo "=========================================="
echo ""

for test_file in "$FUNC_DIR"/test_*.wren; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .wren)
        echo -n "Testing: $test_name ... "
        
        timeout 5 "$COMPILER" < "$test_file" > ./test_output.txt 2>&1
        EXIT_CODE=$?
        
        if [ $EXIT_CODE -eq 0 ]; then
            echo -e "${GREEN}PASS${NC}"
            ((FUNC_PASSED++))
            
            if [ $VERBOSE -eq 1 ]; then
                echo -e "  ${CYAN}Output:${NC}"
                head -30 ./test_output.txt | sed 's/^/    /'
                echo ""
            fi
        else
            echo -e "${RED}FAIL${NC} (exit code: $EXIT_CODE)"
            ((FUNC_FAILED++))
            
            echo -e "  ${YELLOW}Error:${NC}"
            tail -10 ./test_output.txt | sed 's/^/    /'
            echo ""
        fi
    fi
done

echo ""
echo -e "${BOLD}Function Call Tests:${NC} ${GREEN}$FUNC_PASSED passed${NC}, $FUNC_FAILED failed"
echo ""

# ============================================
# SYNTAX ERROR TESTS
# ============================================
echo "=========================================="
echo "   Suite 3: Syntax Error Tests"
echo "=========================================="
echo ""

EXPECTED_SYNTAX_EXIT=2

for test_file in "$SYNTAX_DIR"/test_*.wren; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .wren)
        echo -n "Testing: $test_name ... "
        
        timeout 5 "$COMPILER" < "$test_file" > ./test_output.txt 2>&1
        EXIT_CODE=$?
        
        if [ $EXIT_CODE -eq $EXPECTED_SYNTAX_EXIT ]; then
            echo -e "${GREEN}PASS${NC}"
            ((SYNTAX_PASSED++))
            
            if [ $VERBOSE -eq 1 ]; then
                echo -e "  ${CYAN}Output:${NC}"
                grep "Error" ./test_output.txt | head -3 | sed 's/^/    /'
                echo ""
            fi
        else
            echo -e "${RED}FAIL${NC} (exit code: $EXIT_CODE, expected: $EXPECTED_SYNTAX_EXIT)"
            ((SYNTAX_FAILED++))
            
            echo -e "  ${YELLOW}Error:${NC}"
            tail -5 ./test_output.txt | sed 's/^/    /'
            echo ""
        fi
    fi
done

echo ""
echo -e "${BOLD}Syntax Error Tests:${NC} ${GREEN}$SYNTAX_PASSED passed${NC}, $SYNTAX_FAILED failed"
echo ""

# ============================================
# TYPE SYSTEM TESTS
# ============================================
echo "=========================================="
echo "   Suite 4: Type System Tests"
echo "=========================================="
echo ""

echo -e "${YELLOW}Testing valid type operations...${NC}"
echo ""

# Valid type tests
for test_file in "$TYPE_VALID_DIR"/test_*.wren; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .wren)
        echo -n "Testing: $test_name ... "
        
        timeout 5 "$COMPILER" < "$test_file" > ./test_output.txt 2>&1
        EXIT_CODE=$?
        
        if [ $EXIT_CODE -eq 0 ]; then
            echo -e "${GREEN}PASS${NC}"
            ((TYPE_PASSED++))
            
            if [ $VERBOSE -eq 1 ]; then
                echo -e "  ${CYAN}Output:${NC}"
                head -20 ./test_output.txt | sed 's/^/    /'
                echo ""
            fi
        else
            echo -e "${RED}FAIL${NC} (exit code: $EXIT_CODE)"
            ((TYPE_FAILED++))
            
            echo -e "  ${YELLOW}Error:${NC}"
            tail -10 ./test_output.txt | sed 's/^/    /'
            echo ""
        fi
    fi
done

echo ""
echo -e "${YELLOW}Testing type error cases...${NC}"
echo ""

# Type error tests
for test_file in "$TYPE_ERROR_DIR"/test_*.wren; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .wren)
        echo -n "Testing: $test_name ... "
        
        timeout 5 "$COMPILER" < "$test_file" > ./test_output.txt 2>&1
        EXIT_CODE=$?
        
        if [ $EXIT_CODE -eq 6 ] || [ $EXIT_CODE -eq 7 ]; then
            echo -e "${GREEN}PASS${NC}"
            ((TYPE_PASSED++))
            
            if [ $VERBOSE -eq 1 ]; then
                echo -e "  ${CYAN}Output:${NC}"
                grep "Error" ./test_output.txt | head -3 | sed 's/^/    /'
                echo ""
            fi
        else
            echo -e "${RED}FAIL${NC} (exit code: $EXIT_CODE, expected: 6 or 7)"
            ((TYPE_FAILED++))
            
            echo -e "  ${YELLOW}Error:${NC}"
            tail -5 ./test_output.txt | sed 's/^/    /'
            echo ""
        fi
    fi
done

echo ""
echo -e "${BOLD}Type System Tests:${NC} ${GREEN}$TYPE_PASSED passed${NC}, $TYPE_FAILED failed"
echo ""

# ============================================
# FINAL SUMMARY
# ============================================
TOTAL_PASSED=$((LEXER_PASSED + FUNC_PASSED + SYNTAX_PASSED + TYPE_PASSED))
TOTAL_FAILED=$((LEXER_FAILED + FUNC_FAILED + SYNTAX_FAILED + TYPE_FAILED))
TOTAL_TESTS=$((TOTAL_PASSED + TOTAL_FAILED))

echo ""
echo "=========================================="
echo "   Final Test Summary"
echo "=========================================="
echo ""

echo -e "${BOLD}Results by Suite:${NC}"
echo ""
echo -e "  Lexer Error Tests:       $LEXER_PASSED passed, $LEXER_FAILED failed"
echo -e "  Function Call Tests:     $FUNC_PASSED passed, $FUNC_FAILED failed"
echo -e "  Syntax Error Tests:      $SYNTAX_PASSED passed, $SYNTAX_FAILED failed"
echo -e "  Type System Tests:       $TYPE_PASSED passed, $TYPE_FAILED failed"
echo ""
echo -e "${BOLD}Overall Statistics:${NC}"
echo ""
echo -e "  Total Tests:             $TOTAL_TESTS"
echo -e "  ${GREEN}Total Passed:${NC}            $TOTAL_PASSED"
if [ $TOTAL_FAILED -gt 0 ]; then
    echo -e "  ${RED}Total Failed:${NC}            $TOTAL_FAILED"
else
    echo -e "  Total Failed:            $TOTAL_FAILED"
fi
echo ""

# Tips
if [ $VERBOSE -eq 0 ] && [ $TOTAL_FAILED -gt 0 ]; then
    echo -e "${YELLOW}Tip: Run with -v or --verbose for detailed output${NC}"
    echo ""
fi

# Final verdict
if [ $TOTAL_FAILED -eq 0 ]; then
    echo -e "${GREEN}Result: All tests passed successfully.${NC}"
    echo ""
    exit 0
else
    echo -e "${RED}Result: Some tests failed.${NC}"
    echo ""
    exit 1
fi