#!/usr/local/bin/bash

sudo cp nginx.conf /usr/local/etc/nginx/
sudo cp sahw4.sh /usr/local/etc/rc.d/hw4

sudo service nginx restart
