# Greenpea IoT 'the Things Network' webserver

![GitHub Logo](../assets/greenpea_logo_small.png)
> [greenpea project in 2 sentences]

## RaspberryPi IoT Web Server Setup
### Hardware
1. Raspberry Pi 2B or 3
2. Power Supply for Raspberry Pi
3. MicroSD card (8GB min.)
4. Modem and Router
5. Ethernet Cord

### Raspbian OS Setup
1. Install the 'Raspbian' operating system by following [these steps](https://www.raspberrypi.org/help/noobs-setup/)
2. Once the operating system is installed, from the command line update the software to the latest
		$ sudo apt-get update; sudo apt-get upgrade
3. (Optional) add extra user for [additional security #1](http://arroyocode.com/raspberry-pi-nodejs-web-server-with-pm2/)
4. (Optional) change the default `pi` user password to something more secure
		$ sudo raspi-config
5. Navigate to `2 Change User Password` and follow the steps

### (Optional) SSH setup
Follow these steps to be able to log into your RaspberryPi remotely (so no need for a screen and keyboard etc.)
1. For settings see:[ssh setup](https://www.raspberrypi.org/documentation/remote-access/ssh/)
2. For passwordless access see:  [passwordless ssh access](https://www.raspberrypi.org/documentation/remote-access/ssh/passwordless.md/)
3. Log into your RaspberryPi remotely:
    $ ssh into RaspberryPi: ssh <USER>@<IP-ADDRESS>

### (Optional) extra safety features
For extra security see: [additional security #2](https://superuser.com/questions/1065898/open-ssh-port-a-bad-idea-on-a-raspberry-pi/)


### Install latest NodeJS
1. Install [NodeJS](http://nodejs.org/):
	$ wget http://node-arm.herokuapp.com/node_latest_armhf.deb
	$ sudo dpkg -i node_latest_armhf.deb
	$ node -v

### Install and configure PM2
1. Install [PM2](http://pm2.keymetrics.io/): 
    $ npm install pm2@latest -g

2. Make pm2 start on (re)boot RaspberryPi:
    $ sudo pm2 startup systemd -u <username>

3. Install pm2 logrotate:   

### Install Git and fork this repo
1. First install git:
	$ sudo apt-get install git

2. Then clone [this repo](https://github.com/sebastiaanstuij/greenpea) to custom location on your Pi:
	$ mkdir <name custom directory>
    $ cd ~/<name custom directory>
	$ git clone <https://github.com/sebastiaanstuij/greenpea>

### Register new 'The Thingsnetwork' app 
1. First go to the Thingsnetwork website and register (for free) a new user and a new app: [the things network](<https://www.thethingsnetwork.org/)
2. Then go to your [developer console](https://console.thethingsnetwork.org/applications/)
3. Select your app and note the ApplicationID and access key
4. Open the app.js file and modify the 'var appId' and 'var accessKey' variables to reflect your settings:

    $ nano app.js

### Starting the app with PM2
1. First navigate to the right folder:
    $ cd ~/<name custom directory>/greenpea/webserver-iot

2. Then install the app's dependencies:
    $ npm install 

3. To start the app with PM2:
    $ pm2 start app.js --watch --name webserver-iot

4. To save the current PM2 state (so that it will run automatically on reboot or crash):
    $ pm2 save

5. To view the status of the process:
    $ pm2 list

6. To view the logs of the process:
    $ pm2 logs webserver-iot --lines 200

7. (optional) Install [logrotate](https://github.com/pm2-hive/pm2-logrotate) in order to handle logging more efficiently
    $ pm2 install pm2-logrotate    

### (Optional) Configure webbased logging:
In order to analyze and access the PM2 logging from outside your RaspberryPi or local network:

1. Go to the [Logly website](https://www.loggly.com) and register for a new account (free).
2. Go to your new page and copy the 'Live tail token': https://<userdefined>.loggly.com/live_tail/tokens
3. Install the pm2-plugin on your RaspberryPi: [pm2-loggly](https://github.com/dfrankland/pm2-loggly):
    $ pm2 install pm2-loggly
4. Then configure pm2-loggly:
    $ pm2 set pm2-loggly.logglyClient.token <my-extra-long-token-from-loggly>
    $ pm2 set pm2-loggly.logglyClient.subdomain <mylogglysubdomain>
    $ pm2 set pm2-loggly.logglyClient.tags webserver-iot, <etc.>
    $ pm2 set pm2-loggly.pm2Apps webserver-iot, <etc.>
5. Go to your new loggly page and see incoming log messages

### (Optional) get latest repo updates
1. To get the latest updates after first install:
    $ cd ~/<name custom directory>/greenpea
	$ git pull
2. Git will ask you to enter username and password each time you want to pull
3. (Optional) In order to setup passwordless ssh access with github, 
    see [this turorial](https://help.github.com/articles/changing-a-remote-s-url/) to change remote URL from HTTPS to SSH and [this tutorial](http://garywoodfine.com/setting-up-ssh-keys-for-github-access/) for the remaining steps


### (Optional) app deployment with PM2
work in progress

### (Optional) Unit testing with [MochaJS](https://mochajs.org/)
work in progress


License
----

--