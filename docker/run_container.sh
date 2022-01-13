#!/bin/bash

OS_ARCH=$(arch)

case "$OS_ARCH" in
	"x86_64")
		DOCKER_ARCH=amd64
		;;
	*)
		echo "Invalid architecture \"$OS_ARCH\" supported architectures are: x86_64"
		exit
		;;
esac

GID=$(id -g)
umask 0002
mkdir -p ${HOME}/.gradle
docker run -it --rm --net=host \
	-e USER=$USER \
	--user $UID:$GID \
	--volume="/etc/group:/etc/group:ro" \
	--volume="/etc/gshadow:/etc/gshadow:ro" \
    	--volume="/etc/passwd:/etc/passwd:ro" \
    	--volume="/etc/shadow:/etc/shadow:ro" \
	--volume="${HOME}/.gradle:${HOME}/.gradle:rw" \
	--volume="${HOME}:${HOME}:ro" \
	--volume="$(realpath $(dirname ${0})/..):/build/CKRIOPassthru" \
	 guitar24t/ck-roborio-buildenv:${DOCKER_ARCH} \
	/bin/bash -c '\
	cd CKRIOPassthru \
	&& bash	\
	'
