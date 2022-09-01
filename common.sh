#! /bin/bash


install_dependencies() {
    PACKAGE_MANAGER=""
    for pkg in "dnf" "yum" "pacman" "emerge" "zypp" "zypper" "apt-get" "apt" "apk"
    do
        if ! [[ -z `which $pkg` ]]; then
            PACKAGE_MANAGER=$pkg
            break
        fi
    done
    if [ -z "$PACKAGE_MANAGER" ];
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
            if [ `echo $dependency | grep devel` ] && [ "$PACKAGE_MANAGER" == "apt-get" ];
            then
                sudo $PACKAGE_MANAGER install -y `echo $dependency | sed s/devel/dev/`
            else
                sudo $PACKAGE_MANAGER install -y $dependency 
            fi
            return $?
        fi
        
    done
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