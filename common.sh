#! /bin/bash

test -d results || mkdir results


if [ "$WHYTHO_DEBUG" == "1" ]; then
    echo "Running with debug options (xargs should fail on nonzero exit code)"
    export WHYTHO_DEBUG_ACTIONS="|| exit 255"
else
    export WHYTHO_DEBUG_ACTIONS="exit 0"
fi

detect_package_manager() {
    export WHYTHO_PACKAGE_MANAGER=""
    for pkg in "dnf" "yum" "pacman" "emerge" "zypp" "zypper" "apt-get" "apt" "apk"
    do
        if ! [[ -z `which $pkg 2> /dev/null` ]]; then
            export WHYTHO_PACKAGE_MANAGER=$pkg
            break
        fi
    done
}

install_dependencies() {
    detect_package_manager
    if [ -z "$WHYTHO_PACKAGE_MANAGER" ];
    then
        echo "Could not identify package manager for this system!"
        return 0
    fi
    for dependency in $@
    do
        if [ -z `which $dependency` ]; # this doesn't work for libraries but then also sort of does
        then
            if [ -z "`test -f ./packages_added.log && cat ./packages_added.log | grep $dependency`"];
            then
                echo "$dependency" >> ./packages_added.log
            fi

            # total hack to make centos->ubuntu conversion
            if [ `echo $dependency | grep devel` ] && [ "$WHYTHO_PACKAGE_MANAGER" == "apt-get" ];
            then
                sudo $WHYTHO_PACKAGE_MANAGER install -y `echo $dependency | sed s/devel/dev/`
            else
                sudo $WHYTHO_PACKAGE_MANAGER install -y $dependency
            fi
            return $?
        fi

    done
}

show_installed_packages() {
    detect_package_manager
    case $WHYTHO_PACKAGE_MANAGER in
    dnf | yum)
        sudo yum list installed
        ;;
    apt | apt-get)
        sudo dpkg --list --no-pager
        ;;
    zypper | zypp)
        sudo zypper search -i
        ;;
    pacman)
        sudo pacman -Qqe
        ;;
    *)
        echo "get_installed_packages is not implemented on this platform!"
        ;;
    esac
}

get_content_within() {
    sudo find $1 \
    | grep -v .pem \
    | grep -v id_rsa \
    | grep -v .p7m \
    | grep -v .crt  \
    | grep -v bin \
    | grep -v .zip \
    | grep -v xml \
    | xargs -t -I % sh -c "sudo cat %"
}

start_interrupts_log() {
    cat /proc/interrupts >> before.log
}

compare_interrupts_logs() {
    cat /proc/interrupts >> after.log
    echo "difference-----------------------------------------"
    diff before.log after.log
    start_interrupts_log #reset after comparison
}

cleanup_interrupts_logs() {
    test -e before.log && rm before.log
    test -e after.log  && rm after.log
}

install_sysbench() {
    install_dependencies sysbench curl

    # check if it was missing, if yes then use their package install script.
    if [ -z `which sysbench` ];
    then
        curl -s https://packagecloud.io/install/repositories/akopytov/sysbench/script.rpm.sh | sudo bash
    fi;

    install_dependencies sysbench
    if [ -z `which sysbench` ];
    then
        exit 0;
    fi;

}