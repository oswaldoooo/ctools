#!/bin/bash
setupdir=/opt/codelab
if [[ "$1" != "" ]];then
    setupdir=$1
fi
installdir=$setupdir/lib
if [[ "$2" != "" ]];then
    installdir=$2
fi
includedir=$setupdir/include
if [[ "$3" != "" ]];then
    includedir=$3
fi
echo "install information[if information has problem you have 3s for ctrl Z]:"
echo "codelab paretn dir: $setupdir"
echo "install library dir: $installdir"
echo "install include dir: $includedir"
sleep 3s
echo "start check path"
if [[ ! -d $setupdir ]];then
    mkdir -p $setupdir
    if [[ $? -ne 0 ]];then
        echo "mkdir $setupdir failed"
        exit 1
    fi
fi
if [[ ! -d $installdir ]];then
    mkdir -p $installdir
fi
if [[ ! -d $includedir ]];then
    mkdir -p $includedir
fi
cd ..
echo "move to $(pwd)"
mv ctools $includedir
cd $includedir/ctools
mv makefile_temp makefile
sed -i "s/?/${installdir//\//\\\/}/g" makefile
if [[ -e Makefile ]];then
    rm Makefile
fi
make install_libs
make lib
make ifile
#generate ctools pkg-config
echo "start generate pkg-config file"
for filename in $(ls pkgconfig)
do
    sed -i "s/?/${setupdir//\//\\\/}/g" pkgconfig/$filename
done
ln pkgconfig/*.pc /usr/lib/pkgconfig/