#! /bin/bash

test -d results || mkdir results


if [ "$WHYTHO_DEBUG" == "1" ]; then
    echo "Running with debug options (xargs should fail on nonzero exit code)"
    export WHYTHO_DEBUG_ACTIONS=""
else
    export WHYTHO_DEBUG_ACTIONS="exit 0;"
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
            # total hack to make centos->ubuntu conversion
            echo "$dependency" >> ./packages_added.log
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
        sudo zypper list -I
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