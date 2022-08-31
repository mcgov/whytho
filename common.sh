#! /bin/bash

install_dependencies() {
    for pkg in "yum" "pacman" "emerge" "zypp" "apt-get" "apk"
    do
        if ! [[ -z `which $pkg` ]]; then
            PACKAGE_MANAGER=$pkg
            break
        fi
    done
    for dependency in $@
    do
        if [ -z `which $dependency` ];
        then
            if [ -z `which $PACKAGE_MANAGER` ];
            then
                echo "$PACKAGE_MANAGER and $dependency are missing, exiting..."
                #exit -1
            else
                # total hack to make centos->ubuntu conversion
                echo "$dependency" >> ./packages_added.log
                if [ `echo $dependency | grep devel` ] && [ "$PACKAGE_MANAGER" == "apt-get" ];
                then
                    sudo $PACKAGE_MANAGER install -y `echo $dependency | sed s/devel/dev/` > /dev/null
                else
                    sudo $PACKAGE_MANAGER install -y $dependency > /dev/null
                fi
            fi
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