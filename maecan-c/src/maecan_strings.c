/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <ixam97@ixam97.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * Maximilian Goldschmidt
 * ----------------------------------------------------------------------------
 * https://github.com/Ixam97
 * ----------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "maecan_common.h"

#define BASE_LIST_LENGTH	    2
#define BASE_DEVICE_LENGTH	    111
#define BASE_READINGS_LENGTH	181
#define BASE_DROPDOWN_LENGTH	85
#define BASE_SLIDER_LENGTH	    115

/* Return Digits of int. */

int digits(int value) {
    if (value == 0) {
        return 1;
    } else {
        if (value > 0)
            return floor(log10(abs(value))) + 1;
        else
            return floor(log10(abs(value))) + 2;
    }
}

/***********************************************************************************************/
/*  Generate JSON string from "devices" struct to transfer to remote clients or save as file.  */
/***********************************************************************************************/

void generateDevicesJson(char **buffer, struct device_t * devices, uint8_t n_devices) {
    size_t len = BASE_LIST_LENGTH + 1;

    char *temp_list_buffer;

    temp_list_buffer = malloc(len);
    memset(temp_list_buffer, 0, len);
    strcat(temp_list_buffer, "[");

    for (uint8_t i = 0; i < n_devices; i++) {

        size_t i_len = BASE_DEVICE_LENGTH;
        i_len += digits(devices[i].uid);	/* UID digits */
        i_len += strlen(devices[i].name);	/* Name */
        i_len += digits(devices[i].v_low) + digits(devices[i].v_high) + 1;	/* Digits of high and low version number and point */
        i_len += digits(devices[i].n_reads);	/* digits of n_reads */
        for (uint8_t j = 0; j < devices[i].n_reads; j++) {

            if (j == 0)
                i_len += strlen(",\"status_chanels_info\":[]");

            /* Caclulating for each readings channel */
            i_len += BASE_READINGS_LENGTH;
            i_len += digits(devices[i].readings[j].index);
            i_len += digits(devices[i].readings[j].power);
            i_len += digits(devices[i].readings[j].origin);
            for (uint8_t k = 0; k < 4; k++) {
                i_len += strlen(devices[i].readings[j].colors[k]);
                i_len += digits(devices[i].readings[j].ranges[k]);
            }
            i_len += strlen(devices[i].readings[j].name);
            i_len += strlen(devices[i].readings[j].start);
            i_len += strlen(devices[i].readings[j].end);
            i_len += strlen(devices[i].readings[j].unit);

            if (j < (devices[i].n_reads - 1))
            /* Comma seperator */
                i_len++;
        }
        i_len += digits(devices[i].n_configs);
        /* digits of n_configs */
        for (uint8_t j = 0; j < devices[i].n_configs; j++) {

            if (j == 0)
                i_len += strlen(",\"config_chanels_info\":[]");
            if (j < (devices[i].n_configs - 1))
                i_len++;

            if (devices[i].config_types[j] == 1) {
                i_len += BASE_DROPDOWN_LENGTH;
                i_len += digits(devices[i].configs[j].dropdown.index);
                i_len += digits(devices[i].configs[j].dropdown.n_options);
                i_len += digits(devices[i].configs[j].dropdown.def_option);
                i_len += strlen(devices[i].configs[j].dropdown.description);

                for (uint8_t k = 0; k < devices[i].configs[j].dropdown.n_options; k++) {
                    i_len += strlen(devices[i].configs[j].dropdown.options[k]) + 2;
                    if (k < (devices[i].configs[j].dropdown.n_options - 1))
                        i_len++;
                }
            } else if (devices[i].config_types[j] == 2) {
                i_len += BASE_SLIDER_LENGTH;
                i_len += digits(devices[i].configs[j].slider.index);
                i_len += digits(devices[i].configs[j].slider.min);
                i_len += digits(devices[i].configs[j].slider.max);
                i_len += digits(devices[i].configs[j].slider.def_value);
                i_len += strlen(devices[i].configs[j].slider.description);
                i_len += strlen(devices[i].configs[j].slider.start);
                i_len += strlen(devices[i].configs[j].slider.end);
                i_len += strlen(devices[i].configs[j].slider.unit);

            }
        }
        i_len += digits(devices[i].serial_nbr);		/* digits of serial number */
        i_len += strlen(devices[i].product_nbr);	/* Product number */
        if (i < (n_devices - 1))
            /* Comma seperator */
            i_len++;

        char *temp_device_buffer = malloc(i_len + 1);	/* create temporary buffer to hold device */
        memset(temp_device_buffer, 0, i_len + 1);

        /* construct basic device info: */
        sprintf(temp_device_buffer, "{\"uid\": %ld,\"name\": \"%s\",\"version\": \"%d.%d\",\"status_chanels\": %d,\"config_chanels\": %d,\"serial_number\": %d,\"product_number\": \"%s\"", devices[i].uid, devices[i].name, devices[i].v_low, devices[i].v_high, devices[i].n_reads, devices[i].n_configs, devices[i].serial_nbr, devices[i].product_nbr);

        /* construct readings: */
        if (devices[i].n_reads > 0) {
            strcat(temp_device_buffer, ",\"status_chanels_info\":[");
            for (uint8_t j = 0; j < devices[i].n_reads; j++) {
                sprintf(&temp_device_buffer[strlen(temp_device_buffer)],
                    "{\"chanel\": %d,\"power\": %d,\"color_1\": \"%s\",\"color_2\": \"%s\",\"color_3\": \"%s\",\"color_4\": \"%s\",\"origin\": %d,\"range_1\": %d,\"range_2\": %d,\"range_3\": %d,\"range_4\": %d,\"name\": \"%s\",\"start\": \"%s\",\"end\": \"%s\",\"unit\": \"%s\"}",
                    devices[i].readings[j].index, devices[i].readings[j].power, devices[i].readings[j].colors[0],
                    devices[i].readings[j].colors[1], devices[i].readings[j].colors[2],
                    devices[i].readings[j].colors[3], devices[i].readings[j].origin,
                    devices[i].readings[j].ranges[0], devices[i].readings[j].ranges[1],
                    devices[i].readings[j].ranges[2], devices[i].readings[j].ranges[3], devices[i].readings[j].name,
                    devices[i].readings[j].start, devices[i].readings[j].end, devices[i].readings[j].unit);
                if (j < (devices[i].n_reads - 1))
                    strcat(temp_device_buffer, ",");
            }
            strcat(temp_device_buffer, "]");
        }
        /* construct configs */
        if (devices[i].n_configs > 0) {
            strcat(temp_device_buffer, ",\"config_chanels_info\":[");
            for (uint8_t j = 0; j < devices[i].n_configs; j++) {

                if (devices[i].config_types[j] == 1) {
                    /* Dropdown */
                    sprintf(&temp_device_buffer[strlen(temp_device_buffer)],
                        "{\"chanel\": %d,\"type\": %d,\"num_options\": %d,\"def_option\": %d,\"description\": \"%s\",\"options\":[",
                        devices[i].configs[j].dropdown.index, devices[i].config_types[j],
                        devices[i].configs[j].dropdown.n_options, devices[i].configs[j].dropdown.def_option,
                        devices[i].configs[j].dropdown.description);
                    for (uint8_t k = 0; k < devices[i].configs[j].dropdown.n_options; k++) {
                        sprintf(&temp_device_buffer[strlen(temp_device_buffer)], "\"%s\"",
                            devices[i].configs[j].dropdown.options[k]);
                        if (k < (devices[i].configs[j].dropdown.n_options - 1))
                            strcat(temp_device_buffer, ",");
                    }
                    strcat(temp_device_buffer, "]}");
                } else {
                    /* Slider */
                    sprintf(&temp_device_buffer[strlen(temp_device_buffer)],
                        "{\"chanel\": %d,\"type\": %d,\"min_value\": %d,\"max_value\": %d,\"def_value\": %d,\"description\": \"%s\",\"start\": \"%s\",\"end\": \"%s\",\"unit\": \"%s\"}",
                        devices[i].configs[j].slider.index, devices[i].config_types[j],
                        devices[i].configs[j].slider.min, devices[i].configs[j].slider.max,
                        devices[i].configs[j].slider.def_value, devices[i].configs[j].slider.description,
                        devices[i].configs[j].slider.start, devices[i].configs[j].slider.end,
                        devices[i].configs[j].slider.unit);
                }

                if (j < (devices[i].n_configs - 1))
                    strcat(temp_device_buffer, ",");
            }
            strcat(temp_device_buffer, "]");
        }

        strcat(temp_device_buffer, "}");

        if (i < (n_devices - 1))
            strcat(temp_device_buffer, ",");
        len += strlen(temp_device_buffer);
        temp_list_buffer = realloc(temp_list_buffer, len + 1);
        strcat(temp_list_buffer, temp_device_buffer);
        free(temp_device_buffer);
    }
    strcat(temp_list_buffer, "]");

    /* copy complete device list string to buffer */
    *buffer = realloc(*buffer, len);
    memset(*buffer, 0, len);
    memcpy(*buffer, temp_list_buffer, len - 1);

    free(temp_list_buffer);
}

/****************************************************/
/*  Generate loco names in .cs2 format for MS2      */
/****************************************************/

void generateLokNamenResponse(char **buffer, struct loco_info_t *locolist, uint8_t locolist_len, uint8_t start_index,
                  uint8_t num_names) {
    char *temp_buffer;
    size_t len = 0;
    int index = 0;
    temp_buffer = malloc(0);
    for (uint8_t i = 0; i < num_names; i++) {
        if (i < locolist_len) {
            index = len;
            len += (13 + strlen(locolist[i].name));
            temp_buffer = realloc(temp_buffer, len + 1);
            sprintf(&temp_buffer[index], "[lok]\n.name=%s\n", locolist[i].name);
        } else {
            index = len;
            len += 13;
            temp_buffer = realloc(temp_buffer, len + 1);
            sprintf(&temp_buffer[index], "[lok]\n.name=\n");
        }
        locolist[i].vmin = 0;
    }
    index = len;
    len += (17 + digits(locolist_len));
    temp_buffer = realloc(temp_buffer, len + 1);
    sprintf(&temp_buffer[index], "[numloks]\n.wert=%d\n", locolist_len);

    *buffer = realloc(*buffer, len + 1);
    memset(*buffer, 0, len + 1);
    memcpy(*buffer, temp_buffer, len);

    free(temp_buffer);
}

/****************************************************/
/*  Generate loco info in .cs2 format for one loco  */
/****************************************************/

void generateLocoInfoCs2(struct loco_info_t *loco) {

    char s_type[9];
    memset(s_type, 0, 9);

    switch (loco->type) {
        case 0:
            strcpy(s_type, "mm2_prg");
            break;
        case 1:
            strcpy(s_type, "mm2_dil8");
            break;
        case 2:
            strcpy(s_type, "dcc");
            break;
        case 3:
            strcpy(s_type, "mfx");
            break;
        case 4:
            strcpy(s_type, "sx1");
            break;
        default:
            break;
    }

    printf("[lokomotive]\nlok\n.uid=0x%04x\n.name=%s\n.adresse=0x%04x\n.typ=%s\n.sid=0x%08x\n.mfxuid=0x%08x\n.icon=%s\n.symbol=%d\n.av=%d\n.bv=%d\n.volume=%d\n.tachomax=%d\n.vmax=%d\n.vmin=%d\n",
     loco->uid, loco->name, loco->address, s_type, loco->sid, loco->mfxuid, loco->icon, loco->symbol, loco->av,
     loco->bv, loco->volume, loco->tachomax, loco->vmax, loco->vmin);

}