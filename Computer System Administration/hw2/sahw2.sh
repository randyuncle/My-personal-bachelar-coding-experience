#!/usr/local/bin/bash

# the function for output help message
usage() { 
    echo -n -e "\nUsage: sahw2.sh {--sha256 hashes ... | --md5 hashes ...} -i files ...\n\n--sha256: SHA256 hashes to validate input files.\n--md5: MD5 hashes to validate input files.\n-i: Input files.\n" 
}

# arrays that save the input argument
FILE=()
FILE_HASH=()
SHA=()
MD=()
# count the additional arguments of each argument
num_f=0
num_hash=0
# the pattern that used to create users
USERS=()

# parsing the arguements
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h)
            usage
            exit 0
            ;;
        -i)
            shift
            while [[ "$1" != "--md5" && "$1" != "--sha256" && $# -gt 0 ]]; do
                FILE+=("$1")
                num_f=$(( $num_f + 1 ))
                shift
            done
            ;;
        --sha256)
            shift
            while [[ "$1" != "--md5" && "$1" != "-i" && $# -gt 0 ]]; do
                SHA+=("$1")
                num_hash=$(( $num_hash + 1 ))
                shift
            done
            ;;
        --md5)
            shift
            while [[ "$1" != "--sha256" && "$1" != "-i" && $# -gt 0 ]]; do
                MD+=("$1")
                num_hash=$(( $num_hash + 1 ))
                shift
            done
            ;;
        *)
            echo "Error: Invalid arguments." >&2
            usage
            exit 1
            ;;
    esac
done

# Error handler : input contains two types of hash functions
if [[ ! -z $SHA ]] && [[ ! -z $MD ]] ; then
    echo "Error: Only one type of hash function is allowed." >&2
    exit 1
fi

# read and parse the content(s) in the file(s)
if [ "$num_hash" -eq "$num_f" ] ; then
    count=0
    if [[ ! -z $SHA ]] ; then
        for arg in "${FILE[@]}"; do
            file_hash=`sha256 $arg | tail -c 65`
            if [ "$file_hash" != "${SHA[$count]}" ] ; then
                # check the file type
                # Error handler : hash checksum invalid
                echo "Error: Invalid checksum." >&2
                exit 1
            fi
            count+=1
        done
    elif [[ ! -z $MD ]] ; then
        for arg in "${FILE[@]}"; do
            file_hash=$(md5 $arg | tail -c 33)
            if [ "$file_hash" != "${MD[$count]}" ] ; then
                # Error handler : hash checksum invalid
                echo "Error: Invalid checksum." >&2
                exit 1
            fi
            count+=1
        done
    fi

    for arg in "${FILE[@]}"; do
        filetype=$(file -e ascii $arg)
        if [ "$filetype" = "$arg: JSON data" ] ; then
            USERS+=($(jq ".[] | .username" "$arg" | awk '{gsub(/"/, ""); print}'))
        elif [ "$filetype" = "$arg: CSV text" ] ; then
            USERS+=($(awk -F ',' 'NR>1 {print $1}' $arg))
        else
            # Error handler : not json and csv file
            echo "Error: Invalid file format." >&2
            exit 1
        fi
    done

else
    # Error handler : number of hash strings does't match that of files provided
    echo "Error: Invalid values." >&2
    exit 1
fi

# check if user wants to create user(s) in file(s)
if [ "${#USERS[@]}" != "0" ] ; then
    printf "This script will create the following user(s): ${USERS[*]} Do you want to continue? [y/n]:"
    read answer # read stdin
    if [ "$answer" = "n" ] || [ "$answer" = "\n" ] ; then
        exit 0
    fi

    USERS=()
    for arg in "${FILE[@]}"; do
        filetype=$(file -e ascii $arg)
        # check the file type
        if [ "$filetype" = "$arg: JSON data" ] ; then
            USERS=($(jq ".[] | .username" "$arg" | awk '{gsub(/"/, ""); print}'))
            count=0 # destination variable
            for u in "${USERS[@]}"; do
                # check if the user exists
                if id -u "$u" >/dev/null 2>&1; then
                    echo "Warning: user $u already exist"
                else
                    # create an user
                    PASS_WORD=$(jq ".[$count] | .password" "$arg"| awk '{gsub(/"/, ""); print}')
                    USER_SHELL=$(jq ".[$count] | .shell" "$arg" | awk '{gsub(/"/, ""); print}')
                    if [ "${#USER_SHELL}" = "0" ] ; then
                        USER_SHELL="bin/bash"
                    fi

                    if [ "${#PASS_WORD}" = "0" ] ; then
                        pw useradd -n "$u" -w random -s "$SHELL" -m -h 0
                    else
                        echo "$PASS_WORD" | pw useradd -n "$u" -s "$SHELL" -m -h 0
                    fi
                    
                    # check if the user needs to add to group(s)
                    if jq ".[$count] | .groups | length == 0" "$arg" | grep -q false; then
                        user_group=($(jq ".[$count] | .groups[]" "$arg" | awk '{gsub(/"/, ""); print}'))
                        for g in "${user_group[@]}"; do
                            if getent group "$g" >/dev/null 2>&1; then
                                pw usermod "$u" -G "$g"
                            else
                                pw groupadd "$g"
                                pw usermod "$u" -G "$g"
                            fi
                        done
                    fi
                fi
                count+=1
            done
        elif [ "$filetype" = "$arg: CSV text" ] ; then
            # parsing csv by awk
            USERS=($(awk -F ',' 'NR>1 {print $1}' $arg))
            count=2 # destination variable
            for u in "${USERS[@]}"; do
                # check if the user exists
                if id -u "$u" >/dev/null 2>&1; then
                    echo "Warning: user $u already exists."
                else
                    # adding the user info to array if the user doesn't exist
                    PASS_WORD=$(awk -F ',' -v ln="$count" 'NR==ln {print $2}' $arg)                  
                    USER_SHELL=$(awk -F ',' -v ln="$count" 'NR==ln {print $3}' $arg)
                    if [ "${#USER_SHELL}" = "0" ] ; then
                        USER_SHELL="bin/bash"
                    fi

                    if [ "${#PASS_WORD}" = "0" ] ; then
                        pw useradd -n "$u" -w random -s "$SHELL" -m -h 0
                    else
                        echo "$PASS_WORD" | pw useradd -n "$u" -s "$SHELL" -m -h 0
                    fi

                    # check if the user needs to add to group(s)
                    user_group=($(awk -F ',' -v ln="$count" 'NR==ln && $1!="" {for(i=4;i<=NF;i++){printf("%s", $i)}}' $arg))                    
                    if [ "${#user_group[@]}" = "0" ]; then
                        for g in "${user_group[@]}"; do
                            if getent group "$g" >/dev/null 2>&1; then
                                pw usermod "$u" -G "$g"
                            else    
                                pw groupadd "$g"
                                pw usermod "$u" -G "$g"
                            fi
                        done
                    fi
                fi
                count+=1
            done
        fi
    done
fi