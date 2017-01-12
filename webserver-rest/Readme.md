## Greenpea REST API webserver
[![Logo](img-url)](ref-url)
> [greenpea project in 2 sentences]

## RaspberryPi Web Server Setup
### Hardware
1. Raspberry Pi 2B or 3
2. Power Supply for Raspberry Pi
3. MicroSD card (8GB min.)
4. Modem and Router
5. Ethernet Cord

### Raspbian OS Setup
1. Install the 'Raspbian' operating system by following [these steps]:[raspbian-noobs]
2. Once the operating system is installed, from the command line update the software to the latest
		$ sudo apt-get update; sudo apt-get upgrade
3. (Optional) add extra user (for added security), see: [pi-security]
4. (Optional) change the default `pi` user password to something more secure
		$ sudo raspi-config
5. Navigate to `2 Change User Password` and follow the steps

### (optional) SSH setup
Follow these steps to be able to log into your RaspberryPi remotely (so no need for a screen and keyboard etc.)
1. For settings see: [raspberrypi-ssh-setup]
2. For passwordless access see:  [raspberrypi-ssh-passwordless]
3. Log into your RaspberryPi remotely:
    $ ssh into RaspberryPi: ssh <USER>@<IP-ADDRESS>

### (optional) extra safety features
For added security see: [pi-security2]

### Install latest NodeJS
1. Install NodeJS ([node.js]):
	$ wget http://node-arm.herokuapp.com/node_latest_armhf.deb
	$ sudo dpkg -i node_latest_armhf.deb
	$ node -v

### Install and configure PM2
1. Install PM2 ([pm2]): 
    $ npm install pm2@latest -g

2. Make pm2 start on (re)boot RaspberryPi:
    $ sudo pm2 startup systemd -u <username>

### Install Git and fork this repo
1. First install git:
	$ sudo apt-get install git

2. Then clone [this](https://github.com/sebastiaanstuij/greenpea) to custom location on your Pi:
	$ mkdir <name custom directory>
    $ cd ~/<name custom directory>
	$ git clone <https://github.com/sebastiaanstuij/greenpea>

### Starting the app with PM2
1. First navigate to the right folder:
    $ cd ~/<name custom directory>/greenpea/webserver-rest

2. Then install the app's dependencies:
    $ npm install 

3. To start the app with PM2:
    $ pm2 start app.js --watch --name webserver-rest

4. To save the current PM2 state (so that it will run automatically on reboot or crash):
    $ pm2 save

5. To view the status of the process:
    $ pm2 list

6. To view the logs of the process:
    pm2 logs webserver-rest  --lines 200


### (Optional) Open server to the outside world
1. Dynamic DNS:
In order for your people to access your home web server, you'll need your own domain name that points to your router's dynamic IP provided by your local ISP. 
You can use the free-version of [noip.com](https://noip.com/) for this. After creating an account with noip, follow these steps.
    $ cd ~/
    $ wget http://www.no-ip.com/client/linux/noip-duc-linux.tar.gz
    $ tar vzxf noip-duc-linux.tar.gz
    $ cd noip-2.1.9-1
    $ sudo make
    $ sudo make install

Follow the commands (entering in your username and password from noip) and then start the process.

$ sudo /usr/local/bin/noip2
Now we want this to start whenever your raspberry pi boots, so follow [this]http://www.stuffaboutcode.com/2012/06/raspberry-pi-run-program-at-start-up.html tutorial to create a startup script if you want it.

2. Static Raspberry Pi IP Address:
Whenever your Raspberry Pi reboots, it'll get a new dynamic IP from your router. We'd rather not have this happen, so let's make it a static IP instead. I followed [this](http://www.modmypi.com/blog/tutorial-how-to-give-your-raspberry-pi-a-static-ip-address) tutorial to accomplish this.

3. Open your router to the world:
You need to enable single port forwarding on your local router. Our webserver is running on port <see log>, so I followed [this](http://www.noip.com/support/knowledgebase/port-forwarding-on-a-linksys-wrt610n-router/) tutorial to open up that port on a certain router.


### (Optional) app deployment with PM2
work in progress

### (Optional) Setup log server
work in progress


License
----

--


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)
   [node.js]: <http://nodejs.org/>
   [pm2]: <http://pm2.keymetrics.io/>
   [raspbian-noobs]: <https://www.raspberrypi.org/help/noobs-setup/>
   [pi-security]: <http://arroyocode.com/raspberry-pi-nodejs-web-server-with-pm2/>
   [raspberrypi-ssh-setup]: <https://www.raspberrypi.org/documentation/remote-access/ssh/>
   [raspberrypi-ssh-passwordless]: <https://www.raspberrypi.org/documentation/remote-access/ssh/passwordless.md/>
   [pi-security2]: <https://superuser.com/questions/1065898/open-ssh-port-a-bad-idea-on-a-raspberry-pi/>
   [the-things-network]: <https://www.thethingsnetwork.org/>
   [the-things-network-console]: <https://console.thethingsnetwork.org/applications/>