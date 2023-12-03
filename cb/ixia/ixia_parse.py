#!/usr/bin/python

# IXIA global config .ata files are zlib stream compressed
# the inflated file (named here 'Cte') contains a zip archive with two files:
#
# % zipinfo Cte
# Archive:  Cte
# Zip file size: 7352780 bytes, number of entries: 2
# -rw----     2.0 fat        1 bl defN 23-Nov-27 09:46 Version
# -rw----     2.0 fat  7351414 bl defN 23-Nov-27 09:46 CteStorage
# 2 files, 7351415 bytes uncompressed, 7352540 bytes compressed:  0.0%
#
# The config file is JSON embedded in XML
# uuid seems to be the uniq key for the obejcts

import zlib
import zipfile
import json
import xml.etree.ElementTree as ET

# first zlib inflate
ixia_file = open('ixia.ata','rb').read()
ixia_pkzip = zlib.decompress(ixia_file)
pkzip = open('Cte','wb')
pkzip.write(ixia_pkzip)
# now get the files from pkzip archive
with zipfile.ZipFile('Cte', 'r') as zip_ref:
    zip_ref.extractall('.')

# IXIA config is in 'CteStorage'
# Version file is ignored - it seems to be always '1'

# substitue some strings
# &lt; ->  <
# &gt; ->  >
# &amp;quot; -> "

ixia_xml_file = open('CteStorage','r')
ixia_xml = ixia_xml_file.read()
ixia_xml = ixia_xml.replace('&lt;','<')
ixia_xml = ixia_xml.replace('&gt;','>')
ixia_xml = ixia_xml.replace('&amp;quot;','"')
ixia_xml_file.close()
# print(ixia_xml)
ixia_xml_file = open('ixia.xml', 'w')
ixia_xml_file.write(ixia_xml)
ixia_xml_file.close()

# now we read the XML file
xmlroot = ET.parse('ixia.xml').getroot()

for xml_tag in xmlroot.findall('mObjectXML/CteDataStore/mObjects/CteDataItem'):
    for child in xml_tag:
        if (child.tag == 'mType'):
            print(child.text)
        if (child.tag == 'mData'):
            json_object = json.loads(child.text)
            json_formatted_str = json.dumps(json_object, indent=2)
            print(json_formatted_str)

