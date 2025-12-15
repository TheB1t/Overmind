#!/bin/bash

TEMP_DIR="tmp"
OUTPUT_DIR="output"

#
#   COLORS
#
NOCOLOR='\033[0m';    RED='\033[0;31m';    GREEN='\033[0;32m'
ORANGE='\033[0;33m';  BLUE='\033[0;34m';    PURPLE='\033[0;35m'
CYAN='\033[0;36m';    YELLOW='\033[1;33m';  WHITE='\033[1;37m'
GRAY='\033[0;90m'

#
#   LOG LEVELS
#
LOG_INFO="${GREEN}[INFO]"
LOG_WARN="${YELLOW}[WARN]"
LOG_ERROR="${RED}[ERROR]"
LOG_DIFF="${CYAN}[DIFF]"

#   Function for pretty logs
function log() {
    declare -n level=$1; local message=$2
    echo -e "${level} ${ORANGE}${message}${NOCOLOR}"
}

function pack_addon() {
    local ADDON_PATH=$1
    local NAME=$2

    ADDON_ROOT_FOLDER="@${NAME}"

    log LOG_INFO "Packing addon ${ADDON_ROOT_FOLDER} from ${ADDON_PATH}"

    mkdir ${TEMP_DIR}

    # Addon packing
    rm -rf ${OUTPUT_DIR}/${ADDON_ROOT_FOLDER}
    mkdir ${OUTPUT_DIR}/${ADDON_ROOT_FOLDER}
    mkdir ${OUTPUT_DIR}/${ADDON_ROOT_FOLDER}/addons

    cd ${TEMP_DIR}

    mkdir ${NAME}
    cp -r ../${ADDON_PATH}/sqf_src/* ${NAME}
    pbo_unpacker pack ${NAME} 1>/dev/null
    rename 'y/A-Z/a-z/' ${NAME}.pbo

    cd -

    mv ${TEMP_DIR}/*.pbo ${OUTPUT_DIR}/${ADDON_ROOT_FOLDER}/addons
    cp ${OUTPUT_DIR}/${NAME}*.{so,dll} ${OUTPUT_DIR}/${ADDON_ROOT_FOLDER}

    rm -rf ${TEMP_DIR}
}

function main() {
    local action=$1; shift

    case "$action" in
        "pack")
                log LOG_INFO "Packing..."
                pack_addon "extension_server" "overmind_server"
                pack_addon "extension_client" "overmind_client"

                log LOG_INFO "DONE!"
                ;;
            *)
                [[ -n "$action" ]] && echo "Not implemented action $1" || echo "Action not passed"
                ;;
    esac

}

main $@