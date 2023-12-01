#!/bin/sh

# IXIA .ata file is zlib stream compressed
# the inflated file is a zip archive with two files:
#

# % zipinfo Cte
# Archive:  Cte
# Zip file size: 7352780 bytes, number of entries: 2
# -rw----     2.0 fat        1 bl defN 23-Nov-27 09:46 Version
# -rw----     2.0 fat  7351414 bl defN 23-Nov-27 09:46 CteStorage
# 2 files, 7351415 bytes uncompressed, 7352540 bytes compressed:  0.0%
#
# The config file is JSON embedded in XML

zlib-flate -uncompress < $1 > Cte
unzip Cte

# make files easier to read
sed 's/&lt;/</g' CteStorage | sed 's/&gt;/>/g' | sed 's/&amp;quot;/"/g' > ixia.xml
rm -f Version Cte CteStorage
