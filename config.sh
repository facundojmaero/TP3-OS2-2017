#!/bin/bash

sudo cp lighttpd.conf /etc/lighttpd/
chmod 777 /var/www/html/uploads/
chmod 777 /var/www/html/files/

sudo cp -a html/ /var/www/
sudo /etc/init.d/lighttpd force-reload

sudo echo "www-data ALL = (ALL) NOPASSWD: /sbin/insmod, /sbin/rmmod" > /etc/sudoers.d/lighttpdConf
