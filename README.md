# dban-2.1.0-disknukem

This is an enhanced version of DBAN's dwipe suite that allows users to destroy data in any physical mass storage attached.
Since development of DBAN/dwipe has been stopped for a while, this version will try to resolve bugs and add some additional features.

# Development

The development is active, so the possibility you download non-working code is higher than normal.
To start development, check out the source code

### Check out source code
    git clone git://github.com/piannelli/dban-2.1.0-disknukem.git

### Libraries needed

To build correctly you need to have installed libncurses, libpthread, libmicrohttpd 0.9.9 and libxml2.

### Trick to do testing

dwipe expects to have the label of each drive written in /dev so it can display it.
After compiling it, to ensure it will start correctly, you need to manually create a ".label" file for each device udev recognized
in your system. For instance, if you have /dev/sda and /dev/sda1, you must create two files :

  echo -n /dev/sda  > /dev/sda.label
  echo -n /dev/sda1 > /dev/sda1.label

# What will be new?

This version of dwipe will add functionality to expose data through the web in XML and JSON.
It will also have features like fast wipe (although unsecure) and, alltogether with Disk Nukem (see other project), it will provide
a suite to automate data destruction, for instance, in a web farm.

### New command line options :

  --logfile    # specify the path where to log
  --web        # enable the webserver
  --web-port   # identify the port where to listen
  --web-listen # identify the address where to listen
  --web-user   # the user for HTTP Basic Auth
  --web-pass   # the pass for the HTTP Basic Auth user

