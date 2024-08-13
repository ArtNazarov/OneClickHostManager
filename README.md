# OneClickHostManager
Downloads hosts by list to one file

Sources of lists: https://firebog.net ; https://raw.githubusercontent.com/StevenBlack/hosts/master/alternates/porn-only/hosts

How to use:

1. Wait while hosts file will be created
2. Click button under list to save 'hosts' file to app directory
3. Open terminal in app directory and run
```
sudo cp hosts /etc/hosts
```

Screenshot

![Screenshot](https://dl.dropbox.com/scl/fi/hzluigj87dxooub7ymlmj/OneClickHostManager.png?rlkey=d4j70dtdjij2h46qw4ynjw1ye&st=oavyyo9b)

Installing hosts blocklist list on Android

1. Install TWRP and open Terminal
2. Find hosts file using ```find / -iname hosts```
3. Replace it using finded location: ```cp hosts /data/adb/modules/hosts/system/etc/hosts``` for example
