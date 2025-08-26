#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m'

TESTS=(
    "m:test_malloc:Malloc Test"
    "f:test_free:Free Test"
    "r:test_realloc:Realloc Test"
)

run_test() {
    local test_file="$1"
    local use_valgrind="$2"
    
    echo -e "${BLUE}🔨 Compiling $test_file...${NC}"
    
    make --no-print-directory > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Failed to compile library${NC}"
        return 1
    fi
    
    gcc -I includes -I libft test/$test_file.c -L. -lft_malloc -o test_runner_tmp
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Failed to compile test${NC}"
        return 1
    fi
    
    echo -e "${GREEN}✓ Compilation successful${NC}"
    echo -e "${CYAN}════════════════════════════════════════════${NC}"
    
    if [ "$use_valgrind" = "true" ]; then
        echo -e "${MAGENTA}🧪 Running with Valgrind...${NC}"
        LD_LIBRARY_PATH=. valgrind --leak-check=full --show-leak-kinds=all ./test_runner_tmp
    else
        echo -e "${MAGENTA}🧪 Running test...${NC}"
        LD_LIBRARY_PATH=. ./test_runner_tmp
    fi
    
    local exit_code=$?
    rm -f test_runner_tmp
    
    echo -e "${CYAN}════════════════════════════════════════════${NC}"
    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✅ Test completed successfully${NC}"
    else
        echo -e "${RED}❌ Test failed with exit code $exit_code${NC}"
    fi
    
    return $exit_code
}

show_menu() {
    printf "${WHITE}╔═══════════════════════════════════════════════════╗${NC}\n"
    printf "${WHITE}║               ${CYAN}🧪 FT_MALLOC TEST RUNNER${NC}            ║${NC}\n"
    printf "${WHITE}╠═══════════════════════════════════════════════════╣${NC}\n"
    printf "${WHITE}║  ${YELLOW}Commands:${NC}                                        ║${NC}\n"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-43s${WHITE}║${NC}\n" "m" "Malloc Test"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-43s${WHITE}║${NC}\n" "f" "Free Test"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-43s${WHITE}║${NC}\n" "r" "Realloc Test"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-43s${WHITE}║${NC}\n" "v" "Run last test with Valgrind"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-43s${WHITE}║${NC}\n" "q" "Quit"
    printf "${WHITE}║${NC}    ${GREEN}%-1s${NC} - %-39s${WHITE}║${NC}\n" "enter" "Show this menu"
    printf "${WHITE}╚═══════════════════════════════════════════════════╝${NC}\n"
    printf "\n"
}

main() {
    local last_test=""
    
    echo -e "${BLUE}🔨 Building ft_malloc library...${NC}"
    make --no-print-directory
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Failed to build library${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓ Library built successfully${NC}"
    echo
    
    show_menu
    
    while true; do
        echo -ne "${CYAN}ft_malloc_test> ${NC}"
        read -r input
        
        input=$(echo "$input" | tr '[:upper:]' '[:lower:]')
        
        case "$input" in
            "q"|"quit"|"exit")
                echo -e "${YELLOW}👋 Goodbye!${NC}"
                rm -f test_runner_tmp
                exit 0
                ;;
            "")
                clear
                show_menu
                ;;
            "v"|"valgrind")
                clear
                if [ -n "$last_test" ]; then
                    echo -e "${MAGENTA}🔄 Re-running last test ($last_test) with Valgrind...${NC}"
                    run_test "$last_test" "true"
                else
                    echo -e "${RED}❌ No previous test to run with Valgrind${NC}"
                fi
                ;;
            *)
                test_found=false
                for test in "${TESTS[@]}"; do
                    IFS=':' read -r key file desc <<< "$test"
                    if [ "$input" = "$key" ]; then
                        clear
                        echo -e "${CYAN}🚀 Running: $desc${NC}"
                        if run_test "$file" "false"; then
                            last_test="$file"
                        fi
                        test_found=true
                        break
                    fi
                done
                
                if [ "$test_found" = false ]; then
                    echo -e "${RED}❌ Unknown command: '$input'${NC}"
                fi
                ;;
        esac
        echo
    done
}

main "$@"
