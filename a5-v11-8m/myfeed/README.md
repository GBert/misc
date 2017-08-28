my own feed example
===================

```
# use absolut path
echo "src-link myfeed /home/me/lede/myfeed" >> feeds.conf.default
scripts/feeds update myfeed
scripts/feeds install hello
# 'make menuconfig' shows 'hello' under Utilities -> mark 
make package/hello/{clean,compile} V=s 
```
