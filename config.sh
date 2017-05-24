#!/bin/bash

sudo cp lighttpd.conf /etc/lighttpd/
sudo /etc/init.d/lighttpd force-reload
sudo cp -a html/ /var/www/
chmod 777 /var/www/html/uploads/
chmod 777 /var/www/html/files/
sudo echo "www-data ALL = (ALL) NOPASSWD: /sbin/insmod, /sbin/rmmod" > /etc/sudoers.d/lighttpdConf
