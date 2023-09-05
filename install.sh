#!/bin/bash
setupdir=/opt/codelab
if [[ "$1" != "" ]];then
    setupdir=$1
fi
installdir=$1/lib
if [[ "$2" != "" ]];then
    installdir=$2
fi
includedir=$1/include
if [[ "$3" != "" ]];then
    includedir=$3
fi
echo "install information[if information has problem you have 3s for ctrl Z]:"
echo "codelab paretn dir: `$setupdir`"
echo "install library dir: `$installdir`"
sleep 3s
echo "start check path"
if [[ ! -d $setupdir ]];then
    mkdir -p $setupdir
    if [[ $? -ne 0]];then
        echo "mkdir `$setupdir` failed"
        exit 1
    fi
fi
if [[ ! -d $installdir ]];then
    mkdir -p $installdir
fi
if [[ ! -d $3 ]];then
    mkdir -p $3
fi
cd .. && mv ctools $includedir
cd $includedir/ctools
mv makefile_temp makefile
sed -i "s/?/`$installdir`/g" makefile
make lib
make ifile
#generate ctools pkg-config
echo "start generate pkg-config file"
for filename in $(ls pkgconfig)
do
    sed -i "s/?/`$installdir`/g" pkgconfig/$filename
done
ln pkgconfig/*.pc /usr/lib/pkgconfig/