#!/usr/bin/python

import xml.etree.ElementTree as ET
root = ET.parse('ixia.xml').getroot()

for type_tag in root.findall('mObjectXML/CteDataStore/mObjects/CteDataItem'):
    for child in type_tag:
        print(child.tag, child.text)

