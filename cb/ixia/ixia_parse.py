#!/usr/bin/python3

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

import sys, getopt
import zipfile, zlib
import json, xml.etree.ElementTree as ET

ixia_data = {}
ixia_data_port = {}
ixia_data_filter = {}

def deflate_config(inputfile):
    # first zlib inflate
    ixia_file = open(inputfile,'rb').read()
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
    # save the clean XML file
    ixia_xml_file = open('ixia.xml', 'w')
    ixia_xml_file.write(ixia_xml)
    ixia_xml_file.close()

def parse_xml_json():
    # read the XML file with included JSON objects
    xmlroot = ET.parse('ixia.xml').getroot()

    # use dictionary (aka associative array)
    for xml_tag in xmlroot.findall('mObjectXML/CteDataStore/mObjects/CteDataItem'):
        for child in xml_tag:
            if (child.tag == 'mType'):
                ixia_data_type = child.text
                # print(child.text)
            if (child.tag == 'mData'):
                json_object = json.loads(child.text)
                json_formatted_str = json.dumps(json_object, indent=2)
                # print(json_formatted_str)
                if ixia_data_type == 'CTE_PORT':
                    ixia_data_port[json_object['uuid']] = child.text
                if ixia_data_type == 'CTE_FILTER':
                    ixia_data_filter[json_object['uuid']] = child.text

def print_ports():
    for key in ixia_data_port:
        data = json.loads(ixia_data_port[key])
        print(data['default_name'],",",data['name'], ",", end="")
        for filter in data['source_filter_uuid_list']:
            filter_data = json.loads(ixia_data_filter[filter])
            print(" ", filter_data['default_name'], end="")
            print(",",end ="")
        for filter in data['dest_filter_uuid_list']:
            filter_data = json.loads(ixia_data_filter[filter])
            print(" ", filter_data['default_name'], end="")
        print("")

def print_filters():
    for key in ixia_data_filter:
        data = json.loads(ixia_data_filter[key])
        print(data['default_name'], ",", end="")
        # get the uuids for the source ports
        for interface in data['source_port_uuid_list']:
            port_data = json.loads(ixia_data_port[interface])
            print(" ", port_data['default_name'], end="")
        print("")

def main(argv):
    inputfile =''
    pports = 0
    pfilter = 0
    opts, args = getopt.getopt(argv,"hi:pf")
    for opt, arg in opts:
        if opt == '-h':
            print ('ixia_parse.py -i <inputfile> -p -f')
            sys.exit()
        elif opt in ("-i"):
            inputfile = arg
        elif opt in ("-p"):
            pports = 1
        elif opt in ("-f"):
            pfilter = 1
    if inputfile == '':
        sys.exit()
    deflate_config(inputfile)
    parse_xml_json()
    if pports == 1:
        print_ports()
    if pfilter == 1:
        print_filters()

if __name__ == "__main__":
    main(sys.argv[1:])

