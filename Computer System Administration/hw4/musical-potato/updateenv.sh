#!/usr/local/bin/bash

rm -r /var/raid
mkdir /var/raid

cat env-sample | sed '/UPLOAD_PATH/d' | sed '/FOLDER_PREFIX/d' | sed '/NUM_DISK/d' | sed '/MAX_SIZE/d' > tmp0
cat /etc/rc.conf | grep UPLOAD_PATH > tmp1
cat tmp0 tmp1 > tmp2
cat /etc/rc.conf | grep FOLDER_PREFIX > tmp1
cat tmp2 tmp1 > tmp0
cat /etc/rc.conf | grep NUM_DISK > tmp1
cat tmp0 tmp1 > tmp2
cat /etc/rc.conf | grep MAX_SIZE > tmp1
cat tmp2 tmp1 > tmp0

cp tmp0 .env

rm tmp*

echo "ENV UPDATE"
