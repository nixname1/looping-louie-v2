#!/bin/bash

CMAKE_DIR=cmake-build-debug
MAKE_DIR=Debug
TEMP_DIR="tmp"


function create_clean_tmp_dir()
{
    TEMP_DIR=$(realpath $TEMP_DIR)
    mkdir -p $TEMP_DIR
    rm -rf $TEMP_DIR/*
}

function clean_and_make()
{
    pushd $1 > /dev/null
    make clean > /dev/null; make all -j8 | grep arm-none-eabi-g | tr -s " " > $TEMP_DIR/$2_build_raw
    popd > /dev/null
}

function get_last_parameter()
{
    echo ${!#}
}

function check_for_compile_call()
{
    ELF_CALL=$(echo $input | grep '\.elf')
    if [[ $ELF_CALL == "" ]]
    then
        return 1
    else
        return 0
    fi
}
function get_all_compile_calls_for_compiler()
{
    cat $1_build_raw | grep arm-none-eabi-$2 | while read input
    do
        check_for_compile_call $input
        IS_COMPILE_CALL=$?
        if [ $IS_COMPILE_CALL -gt 0 ]
        then
            echo $input
        fi
    done
}

function get_all_compile_calls()
{
    echo "$(get_all_compile_calls_for_compiler $1 gcc)"
    echo "$(get_all_compile_calls_for_compiler $1 g++)"    
}

function get_all_compiled_filenames()
{
    cat $1_compile_calls | while read input
    do
    LAST_PARAM=$(get_last_parameter $input)
    if [ "$(echo $LAST_PARAM | grep ^-)" == "" ]
    then
        echo $(echo $(basename $LAST_PARAM) | cut -d "\"" -f 1)
    fi
    done
}

function get_compile_options_for_file()
{
    grep $2 $1_compile_calls | sed 's/ -/\n-/g' | sort | while read input
    do
        echo $input
    done
}

function get_link_call()
{
    cat $1_build_raw | grep arm-none-eabi-g++ | while read input
    do
        check_for_compile_call $input
        IS_COMPILE_CALL=$?
        if [ $IS_COMPILE_CALL -eq 0 ]
        then
            echo $input
        fi
    done
}

function get_link_options()
{
    cat $1_link_call | sed 's/ /\n/g' | sort | while read input
    do
        echo $input
    done
}

function create_files()
{
    pushd $TEMP_DIR > /dev/null

    echo "$(get_all_compile_calls $1)" > $1_compile_calls
    echo "$(get_all_compiled_filenames $1 | sort)" > $1_compiled_filenames
    cat $1_compiled_filenames | while read input
    do
        echo "$(get_compile_options_for_file $1 $input)" > $1_compile_$input
    done

    echo "$(get_link_call $1)" > $1_link_call
    echo "$(get_link_options $1)" > $1_link_options
    popd > /dev/null
}

function evaluate()
{
    pushd $TEMP_DIR > /dev/null
    clear
    echo
    echo

    echo -n "Checking files that are getting compiled: "
    FILES_DIFF=$(diff cmake_compiled_filenames make_compiled_filenames)
    if [[ $FILES_DIFF == "" ]]
    then
        echo "No difference found"
    else
        echo "difference found!"
        echo "$FILES_DIFF"
        exit 1
    fi

    echo
    echo "Checking for compile differences"
    FOUND=0
    cat cmake_compiled_filenames | while read input
    do
        printf "% 25s: " "$input"
        TMP=$(diff cmake_compile_$input make_compile_$input)
        if [[ $TMP == "" ]]
        then
            echo "no difference found"
        else
            echo "difference found!"
            echo "$TMP" > ${input}_diff
            FOUND=1
        fi
    done
    

    popd > /dev/null
}

create_clean_tmp_dir

clean_and_make $CMAKE_DIR "cmake"
clean_and_make $MAKE_DIR "make"

create_files "cmake"
create_files "make"

#evaluate