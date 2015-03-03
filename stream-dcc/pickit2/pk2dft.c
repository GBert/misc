/*
 * pk2dft.c
 *
 * pickit2 device file tool
 *
 * features:
 * - dump device files
 * - compile device files
 *
 * compile:
 *  gcc pk2dft.c -lconfuse -o pk2dft
 *
 * written by amx
 *
 * PK2DeviceFile.dat nconsistencies:
 *  - multiple scripts with same name (probably okay, since they are
 *    identified by the number/id)
 *  - size of script comment sometimes exceeds the limit of 128 characters
 *    as specified in (DeviceFile.h)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include <confuse.h>

typedef struct device_ent {
    char part_name[28];
    uint16_t family;
    uint32_t device_id;
    uint32_t program_mem;
    uint16_t ee_mem;
    uint32_t ee_addr;
    uint8_t config_words;
    uint32_t config_addr;
    uint8_t user_id_words;
    uint32_t user_id_addr;
    uint32_t bandgap_mask;
    uint16_t config_masks[8];
    uint16_t config_blank[8];
    uint16_t cp_mask;
    uint8_t cp_config;
    bool osscal_save;
    uint32_t ignore_address;
    float vdd_min;
    float vdd_max;
    float vdd_erase;
    uint8_t calibration_words;
    uint16_t chip_erase_script;
    uint16_t progmem_addr_set_script;
    uint8_t progmem_addr_bytes;
    uint16_t progmem_rd_script;
    uint16_t progmem_rd_words;
    uint16_t eerd_prep_script;
    uint16_t eerd_script;
    uint16_t eerd_locations;
    uint16_t user_id_rd_prep_script;
    uint16_t user_id_rd_script;
    uint16_t config_rd_prep_script;
    uint16_t config_rd_script;
    uint16_t progmem_wr_prep_script;
    uint16_t progmem_wr_script;
    uint16_t progmem_wr_words;
    uint8_t progmem_panel_bufs;
    uint32_t progmem_panel_offset;
    uint16_t eewr_prep_script;
    uint16_t eewr_script;
    uint16_t eewr_locations;
    uint16_t user_id_wr_prep_script;
    uint16_t user_id_wr_script;
    uint16_t config_wr_prep_script;
    uint16_t config_wr_script;
    uint16_t osccal_rd_script;
    uint16_t osccal_wr_script;
    uint16_t dp_mask;
    bool write_cfg_on_erase;
    bool blank_check_skip_usr_ids;
    uint16_t ignore_bytes;
    uint16_t chip_erase_prep_script;
    uint32_t boot_flash;
    uint16_t config9_mask;
    uint16_t config9_blank;
    uint16_t progmem_erase_script;
    uint16_t eemem_erase_script;
    uint16_t configmem_erase_script;
    uint16_t reserved1_erase_script;
    uint16_t reserved2_erase_script;
    uint16_t test_memory_rd_script;
    uint16_t test_memory_rd_words;
    uint16_t eerow_erase_script;
    uint16_t eerow_erase_words;
    bool export_to_mplab;
    uint16_t debug_halt_script;
    uint16_t debug_run_script;
    uint16_t debug_status_script;
    uint16_t debug_read_exec_ver_script;
    uint16_t debug_single_step_script;
    uint16_t debug_bulk_wr_data_script;
    uint16_t debug_bulk_rd_data_script;
    uint16_t debug_write_vector_script;
    uint16_t debug_read_vector_script;
    uint16_t debug_row_erase_script;
    uint16_t debug_row_erase_size;
    uint16_t debug_reserved5_script;
    uint16_t debug_reserved6_script;
    uint16_t debug_reserved7_script;
    uint16_t debug_reserved8_script;
    uint16_t lvp_script;
} __attribute__ ((__packed__)) device_ent_t;

typedef struct family_ent {
    uint16_t fam_id;
    uint16_t fam_type;
    uint16_t search_prio;
    char name[24];
    uint16_t prg_entry_script;
    uint16_t prg_exit_script;
    uint16_t rd_devid_script;
    uint32_t devid_mask;
    uint32_t blank_value;
    uint8_t bytes_per_loc;
    uint8_t addr_inc;
    bool b_detect;
    uint16_t prg_entry_vpp_script;
    uint16_t unused1;
    uint8_t ee_bytes_per_word;
    uint8_t ee_addr_inc;
    uint8_t user_id_hex_bytes;
    uint8_t user_id_bytes;
    uint8_t prog_mem_hex_bytes;
    uint8_t ee_mem_hex_bytes;
    uint8_t prg_mem_shift;
    uint32_t test_memory_start;
    uint16_t test_memory_length;
    float vpp;
} __attribute__ ((__packed__)) family_ent_t;

typedef struct script_ent {
    char org_name[64];
    uint16_t script_num;
    char name[32];
    uint16_t version;
    uint32_t unused1;
    uint16_t script_length;
    uint8_t script[128];
    char comment[512];
} __attribute__ ((__packed__)) script_ent_t;

typedef struct dat_hdr {
    int version[3];
    char notes[512];
    int num_families;
    int num_devices;
    int num_scripts;
    uint8_t compat;
    uint8_t unused1a;
    uint16_t unused1b;
    uint32_t unused2;
} dat_hdr_t;

cfg_opt_t config_opts[] = {
    CFG_INT("ver_major", 0, CFGF_NONE),
    CFG_INT("ver_minor", 0, CFGF_NONE),
    CFG_INT("ver_dot", 0, CFGF_NONE),
    CFG_INT("compat", 0, CFGF_NONE),
    CFG_INT("unused1a", 0, CFGF_NONE),
    CFG_INT("unused1b", 0, CFGF_NONE),
    CFG_INT("unused2", 0, CFGF_NONE),
    CFG_STR("notes", "", CFGF_NONE),
    CFG_STR_LIST("families", "", CFGF_NONE),
    CFG_STR_LIST("devices", "", CFGF_NONE),
    CFG_STR_LIST("scripts", "", CFGF_NONE),
    CFG_END()
};

cfg_opt_t script_opts[] = {
    CFG_STR("org_name", "", CFGF_NONE),
    CFG_STR("name", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("id", 0, CFGF_NONE),
    CFG_INT("version", 0, CFGF_NONE),
    CFG_INT("unused1", 0, CFGF_NONE),
    CFG_INT_LIST("script", 0, CFGF_NONE),
    CFG_STR("comment", "", CFGF_NONE)
};

cfg_opt_t family_opts[] = {
    CFG_STR("name", "NO_FAM", CFGF_NONE),
    CFG_INT("fam_id", 0, CFGF_NONE),
    CFG_INT("fam_type", 0, CFGF_NONE),
    CFG_INT("search_prio", 0, CFGF_NONE),
    CFG_STR("prg_entry_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("prg_exit_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("rd_devid_script", "NO_SCRIPT", CFGF_NONE),
/*	CFG_INT("devid_mask"          ,           0, CFGF_NONE), */
/*	CFG_INT("blank_value"         ,           0, CFGF_NONE), */
    CFG_STR("devid_mask", "0", CFGF_NONE),	/* because libconfuse does not support 0xffffffff via */
    CFG_STR("blank_value", "0", CFGF_NONE),	/* the integer option */
    CFG_INT("bytes_per_loc", 0, CFGF_NONE),
    CFG_INT("addr_inc", 0, CFGF_NONE),
    CFG_INT("b_detect", 0, CFGF_NONE),
    CFG_STR("prg_entry_vpp_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("ee_bytes_per_word", 0, CFGF_NONE),
    CFG_INT("ee_addr_inc", 0, CFGF_NONE),
    CFG_INT("user_id_hex_bytes", 0, CFGF_NONE),
    CFG_INT("user_id_bytes", 0, CFGF_NONE),
    CFG_INT("prog_mem_hex_bytes", 0, CFGF_NONE),
    CFG_INT("ee_mem_hex_bytes", 0, CFGF_NONE),
    CFG_INT("prg_mem_shift", 0, CFGF_NONE),
    CFG_INT("test_memory_start", 0, CFGF_NONE),
    CFG_INT("test_memory_length", 0, CFGF_NONE),
    CFG_FLOAT("vpp", 0.0, CFGF_NONE)
};

cfg_opt_t device_opts[] = {
    CFG_STR("part_name", "NO_DEV", CFGF_NONE),
    CFG_STR("family", "NO_FAM", CFGF_NONE),
/*	CFG_INT("device_id"                 ,           0, CFGF_NONE), */
    CFG_STR("device_id", "0", CFGF_NONE),
    CFG_INT("program_mem", 0, CFGF_NONE),
    CFG_INT("ee_mem", 0, CFGF_NONE),
    CFG_INT("ee_addr", 0, CFGF_NONE),
    CFG_INT("config_words", 0, CFGF_NONE),
    CFG_INT("config_addr", 0, CFGF_NONE),
    CFG_INT("user_id_words", 0, CFGF_NONE),
    CFG_INT("user_id_addr", 0, CFGF_NONE),
    CFG_INT("bandgap_mask", 0, CFGF_NONE),
/*	CFG_INT_LIST("config_masks"         , {0,0,0,0,0,0,0,0,0,0}, CFGF_NONE), */
/*	CFG_INT_LIST("config_blank"         , {0,0,0,0,0,0,0,0,0,0}, CFGF_NONE), */
    CFG_INT_LIST("config_masks", 0, CFGF_NONE),
    CFG_INT_LIST("config_blank", 0, CFGF_NONE),
    CFG_INT("cp_mask", 0, CFGF_NONE),
    CFG_INT("cp_config", 0, CFGF_NONE),
    CFG_INT("osscal_save", 0, CFGF_NONE),
    CFG_INT("ignore_address", 0, CFGF_NONE),
    CFG_FLOAT("vdd_min", 0.0, CFGF_NONE),
    CFG_FLOAT("vdd_max", 0.0, CFGF_NONE),
    CFG_FLOAT("vdd_erase", 0.0, CFGF_NONE),
    CFG_INT("calibration_words", 0, CFGF_NONE),
    CFG_STR("chip_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("progmem_addr_set_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("progmem_addr_bytes", 0, CFGF_NONE),
    CFG_STR("progmem_rd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("progmem_rd_words", 0, CFGF_NONE),
    CFG_STR("eerd_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("eerd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("eerd_locations", 0, CFGF_NONE),
    CFG_STR("user_id_rd_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("user_id_rd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("config_rd_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("config_rd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("progmem_wr_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("progmem_wr_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("progmem_wr_words", 0, CFGF_NONE),
    CFG_INT("progmem_panel_bufs", 0, CFGF_NONE),
    CFG_INT("progmem_panel_offset", 0, CFGF_NONE),
    CFG_STR("eewr_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("eewr_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("eewr_locations", 0, CFGF_NONE),
    CFG_STR("user_id_wr_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("user_id_wr_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("config_wr_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("config_wr_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("osccal_rd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("osccal_wr_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("dp_mask", 0, CFGF_NONE),
    CFG_INT("write_cfg_on_erase", 0, CFGF_NONE),
    CFG_INT("blank_check_skip_usr_ids", 0, CFGF_NONE),
    CFG_INT("ignore_bytes", 0, CFGF_NONE),
    CFG_STR("chip_erase_prep_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("boot_flash", 0, CFGF_NONE),
    CFG_INT("config9_mask", 0, CFGF_NONE),
    CFG_INT("config9_blank", 0, CFGF_NONE),
    CFG_STR("progmem_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("eemem_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("configmem_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("reserved1_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("reserved2_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("test_memory_rd_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("test_memory_rd_words", 0, CFGF_NONE),
    CFG_STR("eerow_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("eerow_erase_words", 0, CFGF_NONE),
    CFG_INT("export_to_mplab", 0, CFGF_NONE),
    CFG_STR("debug_halt_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_run_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_status_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_read_exec_ver_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_single_step_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_bulk_wr_data_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_bulk_rd_data_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_write_vector_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_read_vector_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_row_erase_script", "NO_SCRIPT", CFGF_NONE),
    CFG_INT("debug_row_erase_size", 0, CFGF_NONE),
    CFG_STR("debug_reserved5_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_reserved6_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_reserved7_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("debug_reserved8_script", "NO_SCRIPT", CFGF_NONE),
    CFG_STR("lvp_script", "NO_SCRIPT", CFGF_NONE),
};

/* global variables */
static dat_hdr_t hdr;

static family_ent_t *families = NULL;
static device_ent_t *devices = NULL;
static script_ent_t *scripts = NULL;

static int num_rd_scripts = 0;
static int num_rd_families = 0;
static int num_rd_devices = 0;

static int verbose = 0;

/* forward declarations */
int parse_script(char *filename, script_ent_t * script);
int parse_family(char *filename, family_ent_t * family);
int parse_device(char *filename, device_ent_t * device);

void print_script(FILE * f, script_ent_t * script);
void print_family(FILE * f, family_ent_t * family);
void print_device(FILE * f, device_ent_t * device);

int read_device_file(char *filename);
int dump_device_file(char *dumpdir);
int build_device_file(char *dir, char *outfile);

void usage(char *argv0);

int main(int argc, char **argv) {
    fprintf(stdout, "pickit2 device file tool ver. 0.1b\n (by amx - alemaxx@hotmail.de)\n\n");

    if (argc < 2) {
	usage(argv[0]);
	return 0;
    }

    if (argv[1][0] != '-') {
	usage(argv[0]);
	return 0;
    }

    switch (argv[1][1]) {
    case 'd':
	if (argc == 4) {
	    if (read_device_file(argv[2]) < 0) {
		printf("error: cannot read device file \"%s\"\n", argv[2]);
	    } else {
		if (dump_device_file(argv[3]) < 0) {
		    printf("error: cannot sump device file!\n");
		}
	    }
	}
	break;

    case 'c':
	if (argc != 4)
	    break;
	if (build_device_file(argv[2], argv[3]) < 0) {
	    printf("error: could not create new device file\n");
	} else
	    printf("success!\n");
	break;

    default:
	usage(argv[0]);
	break;
    }

    if (families != NULL)
	free(families);
    if (devices != NULL)
	free(devices);
    if (scripts != NULL)
	free(scripts);

    return 0;
}

void usage(char *argv0) {
    fprintf(stdout, "usage: %s -[dc] ...\n\n", argv0);
    fprintf(stdout, "  d - dump   (-d device_file.dat dest_dir)\n");
    fprintf(stdout, "  c - create (-c src_dir outfile.dat)\n");
    fprintf(stdout, "\n\n");
}

int parse_script(char *filename, script_ent_t * script) {
    cfg_t *pcfg;
    int i;

    memset(script, 0, sizeof(script_ent_t));

    pcfg = cfg_init(script_opts, CFGF_NONE);
    if (cfg_parse(pcfg, filename) == CFG_PARSE_ERROR)
	return -1;

    strcpy(script->name, cfg_getstr(pcfg, "name"));	/* max 28 chars !!! */
    strcpy(script->org_name, cfg_getstr(pcfg, "org_name"));	/* max 64 chars !!! */
    strcpy(script->comment, cfg_getstr(pcfg, "comment"));	/* max 128 chars !!! */

    script->script_num = cfg_getint(pcfg, "id");
    script->version = cfg_getint(pcfg, "version");
    script->unused1 = cfg_getint(pcfg, "unused1");

    script->script_length = cfg_size(pcfg, "script");
    if (script->script_length & 1) {
	printf("WARNING: odd number of bytes in script ... truncating\n");
    }
    script->script_length >>= 1;
    for (i = 0; i < (script->script_length << 1); i++)
	script->script[i] = (uint8_t) cfg_getnint(pcfg, "script", i);

    cfg_free(pcfg);
    return 0;
}

uint16_t get_script_by_name(char *name) {
    uint16_t ret;

    if (strcmp(name, "NO_SCRIPT") == 0)
	return 0;
    for (ret = 0; ret < num_rd_scripts; ret++) {
	if (strcmp(name, scripts[ret].name) == 0)
	    return ret + 1;
    }

    return -1;
}

uint16_t get_family_by_name(char *name) {
    uint16_t ret;

    if (strcmp(name, "NO_FAM") == 0) {
	printf("error: NO_FAM found\n");
	return -1;
    }

    for (ret = 0; ret < num_rd_families; ret++) {
	if (strcmp(name, families[ret].name) == 0)
	    return ret;
    }

    return -1;
}

int parse_family(char *filename, family_ent_t * family) {
    cfg_t *pcfg;

    memset(family, 0, sizeof(family_ent_t));

    pcfg = cfg_init(family_opts, CFGF_NONE);
    if (cfg_parse(pcfg, filename) == CFG_PARSE_ERROR)
	return -1;

    strcpy(family->name, cfg_getstr(pcfg, "name"));
    family->fam_id = cfg_getint(pcfg, "fam_id");
    family->fam_type = cfg_getint(pcfg, "fam_type");
    family->search_prio = cfg_getint(pcfg, "search_prio");
    family->prg_entry_script = get_script_by_name(cfg_getstr(pcfg, "prg_entry_script"));
    family->prg_exit_script = get_script_by_name(cfg_getstr(pcfg, "prg_exit_script"));
    family->rd_devid_script = get_script_by_name(cfg_getstr(pcfg, "rd_devid_script"));

/*	family->devid_mask           = cfg_getint(pcfg, "devid_mask");  */
/*	family->blank_value          = cfg_getint(pcfg, "blank_value"); */

    family->devid_mask = strtoul(cfg_getstr(pcfg, "devid_mask"), NULL, 16);
    family->blank_value = strtoul(cfg_getstr(pcfg, "blank_value"), NULL, 16);

    family->bytes_per_loc = cfg_getint(pcfg, "bytes_per_loc");
    family->addr_inc = cfg_getint(pcfg, "addr_inc");
    family->b_detect = cfg_getint(pcfg, "b_detect");
    family->prg_entry_vpp_script = get_script_by_name(cfg_getstr(pcfg, "prg_entry_vpp_script"));
    family->ee_bytes_per_word = cfg_getint(pcfg, "ee_bytes_per_word");
    family->ee_addr_inc = cfg_getint(pcfg, "ee_addr_inc");
    family->user_id_hex_bytes = cfg_getint(pcfg, "user_id_hex_bytes");
    family->user_id_bytes = cfg_getint(pcfg, "user_id_bytes");
    family->prog_mem_hex_bytes = cfg_getint(pcfg, "prog_mem_hex_bytes");
    family->ee_mem_hex_bytes = cfg_getint(pcfg, "ee_mem_hex_bytes");
    family->prg_mem_shift = cfg_getint(pcfg, "prg_mem_shift");
    family->test_memory_start = cfg_getint(pcfg, "test_memory_start");
    family->test_memory_length = cfg_getint(pcfg, "test_memory_length");
    family->vpp = cfg_getfloat(pcfg, "vpp");

    cfg_free(pcfg);
    return 0;
}

int parse_device(char *filename, device_ent_t * device) {
    cfg_t *pcfg;
    int i;

    memset(device, 0, sizeof(device_ent_t));

    pcfg = cfg_init(device_opts, CFGF_NONE);
    if (cfg_parse(pcfg, filename) == CFG_PARSE_ERROR)
	return -1;

    strcpy(device->part_name, cfg_getstr(pcfg, "part_name"));
    device->family = get_family_by_name(cfg_getstr(pcfg, "family"));
    device->device_id = strtoul(cfg_getstr(pcfg, "device_id"), NULL, 16);
    device->program_mem = cfg_getint(pcfg, "program_mem");
    device->ee_mem = cfg_getint(pcfg, "ee_mem");
    device->ee_addr = cfg_getint(pcfg, "ee_addr");
    device->config_words = cfg_getint(pcfg, "config_words");
    device->config_addr = cfg_getint(pcfg, "config_addr");
    device->user_id_words = cfg_getint(pcfg, "user_id_words");
    device->user_id_addr = cfg_getint(pcfg, "user_id_addr");
    device->bandgap_mask = cfg_getint(pcfg, "bandgap_mask");

    if (cfg_size(pcfg, "config_masks") != 8)
	printf("WARNING : size of \"config_masks\" array is wrong! (%d)\n", cfg_size(pcfg, "config_masks"));
    for (i = 0; i < 8; i++)
	device->config_masks[i] = (uint16_t) cfg_getnint(pcfg, "config_masks", i);

    if (cfg_size(pcfg, "config_blank") != 8)
	printf("WARNING : size of \"config_blank\" array is wrong!\n");
    for (i = 0; i < 8; i++)
	device->config_blank[i] = (uint16_t) cfg_getnint(pcfg, "config_blank", i);

    device->cp_mask = cfg_getint(pcfg, "cp_mask");
    device->cp_config = cfg_getint(pcfg, "cp_config");
    device->osscal_save = cfg_getint(pcfg, "osscal_save");
    device->ignore_address = cfg_getint(pcfg, "ignore_address");
    device->vdd_min = cfg_getfloat(pcfg, "vdd_min");
    device->vdd_max = cfg_getfloat(pcfg, "vdd_max");
    device->vdd_erase = cfg_getfloat(pcfg, "vdd_erase");
    device->calibration_words = cfg_getint(pcfg, "calibration_words");
    device->chip_erase_script = get_script_by_name(cfg_getstr(pcfg, "chip_erase_script"));
    device->progmem_addr_set_script = get_script_by_name(cfg_getstr(pcfg, "progmem_addr_set_script"));
    device->progmem_addr_bytes = cfg_getint(pcfg, "progmem_addr_bytes");
    device->progmem_rd_script = get_script_by_name(cfg_getstr(pcfg, "progmem_rd_script"));
    device->progmem_rd_words = cfg_getint(pcfg, "progmem_rd_words");
    device->eerd_prep_script = get_script_by_name(cfg_getstr(pcfg, "eerd_prep_script"));
    device->eerd_script = get_script_by_name(cfg_getstr(pcfg, "eerd_script"));
    device->eerd_locations = cfg_getint(pcfg, "eerd_locations");
    device->user_id_rd_prep_script = get_script_by_name(cfg_getstr(pcfg, "user_id_rd_prep_script"));
    device->user_id_rd_script = get_script_by_name(cfg_getstr(pcfg, "user_id_rd_script"));
    device->config_rd_prep_script = get_script_by_name(cfg_getstr(pcfg, "config_rd_prep_script"));
    device->config_rd_script = get_script_by_name(cfg_getstr(pcfg, "config_rd_script"));
    device->progmem_wr_prep_script = get_script_by_name(cfg_getstr(pcfg, "progmem_wr_prep_script"));
    device->progmem_wr_script = get_script_by_name(cfg_getstr(pcfg, "progmem_wr_script"));
    device->progmem_wr_words = cfg_getint(pcfg, "progmem_wr_words");
    device->progmem_panel_bufs = cfg_getint(pcfg, "progmem_panel_bufs");
    device->progmem_panel_offset = cfg_getint(pcfg, "progmem_panel_offset");
    device->eewr_prep_script = get_script_by_name(cfg_getstr(pcfg, "eewr_prep_script"));
    device->eewr_script = get_script_by_name(cfg_getstr(pcfg, "eewr_script"));
    device->eewr_locations = cfg_getint(pcfg, "eewr_locations");
    device->user_id_wr_prep_script = get_script_by_name(cfg_getstr(pcfg, "user_id_wr_prep_script"));
    device->user_id_wr_script = get_script_by_name(cfg_getstr(pcfg, "user_id_wr_script"));
    device->config_wr_prep_script = get_script_by_name(cfg_getstr(pcfg, "config_wr_prep_script"));
    device->config_wr_script = get_script_by_name(cfg_getstr(pcfg, "config_wr_script"));
    device->osccal_rd_script = get_script_by_name(cfg_getstr(pcfg, "osccal_rd_script"));
    device->osccal_wr_script = get_script_by_name(cfg_getstr(pcfg, "osccal_wr_script"));
    device->dp_mask = cfg_getint(pcfg, "dp_mask");
    device->write_cfg_on_erase = cfg_getint(pcfg, "write_cfg_on_erase");
    device->blank_check_skip_usr_ids = cfg_getint(pcfg, "blank_check_skip_usr_ids");
    device->ignore_bytes = cfg_getint(pcfg, "ignore_bytes");
    device->chip_erase_prep_script = get_script_by_name(cfg_getstr(pcfg, "chip_erase_prep_script"));
    device->boot_flash = cfg_getint(pcfg, "boot_flash");
    device->config9_mask = cfg_getint(pcfg, "config9_mask");
    device->config9_blank = cfg_getint(pcfg, "config9_blank");
    device->progmem_erase_script = get_script_by_name(cfg_getstr(pcfg, "progmem_erase_script"));
    device->eemem_erase_script = get_script_by_name(cfg_getstr(pcfg, "eemem_erase_script"));
    device->configmem_erase_script = get_script_by_name(cfg_getstr(pcfg, "configmem_erase_script"));
    device->reserved1_erase_script = get_script_by_name(cfg_getstr(pcfg, "reserved1_erase_script"));
    device->reserved2_erase_script = get_script_by_name(cfg_getstr(pcfg, "reserved2_erase_script"));
    device->test_memory_rd_script = get_script_by_name(cfg_getstr(pcfg, "test_memory_rd_script"));
    device->test_memory_rd_words = cfg_getint(pcfg, "test_memory_rd_words");
    device->eerow_erase_script = get_script_by_name(cfg_getstr(pcfg, "eerow_erase_script"));
    device->eerow_erase_words = cfg_getint(pcfg, "eerow_erase_words");
    device->export_to_mplab = cfg_getint(pcfg, "export_to_mplab");
    device->debug_halt_script = get_script_by_name(cfg_getstr(pcfg, "debug_halt_script"));
    device->debug_run_script = get_script_by_name(cfg_getstr(pcfg, "debug_run_script"));
    device->debug_status_script = get_script_by_name(cfg_getstr(pcfg, "debug_status_script"));
    device->debug_read_exec_ver_script = get_script_by_name(cfg_getstr(pcfg, "debug_read_exec_ver_script"));
    device->debug_single_step_script = get_script_by_name(cfg_getstr(pcfg, "debug_single_step_script"));
    device->debug_bulk_wr_data_script = get_script_by_name(cfg_getstr(pcfg, "debug_bulk_wr_data_script"));
    device->debug_bulk_rd_data_script = get_script_by_name(cfg_getstr(pcfg, "debug_bulk_rd_data_script"));
    device->debug_write_vector_script = get_script_by_name(cfg_getstr(pcfg, "debug_write_vector_script"));
    device->debug_read_vector_script = get_script_by_name(cfg_getstr(pcfg, "debug_read_vector_script"));
    device->debug_row_erase_script = get_script_by_name(cfg_getstr(pcfg, "debug_row_erase_script"));
    device->debug_row_erase_size = cfg_getint(pcfg, "debug_row_erase_size");
    device->debug_reserved5_script = get_script_by_name(cfg_getstr(pcfg, "debug_reserved5_script"));
    device->debug_reserved6_script = get_script_by_name(cfg_getstr(pcfg, "debug_reserved6_script"));
    device->debug_reserved7_script = get_script_by_name(cfg_getstr(pcfg, "debug_reserved7_script"));
    device->debug_reserved8_script = get_script_by_name(cfg_getstr(pcfg, "debug_reserved8_script"));
    device->lvp_script = get_script_by_name(cfg_getstr(pcfg, "lvp_script"));

    cfg_free(pcfg);
    return 0;
}

char *valid_string(char *in, char *out) {
    char *pc, *pc2;
    /* mainly replace " with \" */
    pc = in;
    pc2 = out;
    while (*pc != 0) {
	if (*pc == '\"') {
	    *pc2++ = '\\';
	    *pc2++ = '\"';
	} else
	    *pc2++ = *pc;
	pc++;
    }
    *pc2 = 0;

    return out;
}

char g_tmp[128];

char *script_name(uint16_t script_id) {
    if (scripts == NULL)
	return "no scripts loaded";
    if (((script_id - 1) > hdr.num_scripts) || (script_id < 0))
	return "script ID is invalid";
    if (script_id == 0)
	return "NO_SCRIPT";

    return valid_string(scripts[script_id - 1].name, g_tmp);
}

char *family_name(uint16_t family_id) {
    if (families == NULL)
	return "no families loaded";
    if (((family_id - 1) > hdr.num_families) || (family_id < 0))
	return "family ID is invalid";

    return valid_string(families[family_id].name, g_tmp);
}

void print_family(FILE * f, family_ent_t * devfam_par) {
    fprintf(f, "name                = \"%s\"\n", devfam_par->name);
    fprintf(f, "fam_id              = %d\n", devfam_par->fam_id);
    fprintf(f, "fam_type            = %d\n", devfam_par->fam_type);
    fprintf(f, "search_prio         = %d\n", devfam_par->search_prio);
    fprintf(f, "prg_entry_script    = \"%s\"\n", script_name(devfam_par->prg_entry_script));
    fprintf(f, "prg_exit_script     = \"%s\"\n", script_name(devfam_par->prg_exit_script));
    fprintf(f, "rd_devid_script     = \"%s\"\n", script_name(devfam_par->rd_devid_script));
    fprintf(f, "devid_mask          = 0x%08x\n", devfam_par->devid_mask);
    fprintf(f, "blank_value         = 0x%08x\n", devfam_par->blank_value);
    fprintf(f, "bytes_per_loc       = %d\n", devfam_par->bytes_per_loc);
    fprintf(f, "addr_inc            = %d\n", devfam_par->addr_inc);
    fprintf(f, "b_detect            = %d\n", devfam_par->b_detect);
    fprintf(f, "prg_entry_vpp_script= \"%s\"\n", script_name(devfam_par->prg_entry_vpp_script));
    fprintf(f, "ee_bytes_per_word   = %d\n", devfam_par->ee_bytes_per_word);
    fprintf(f, "ee_addr_inc         = %d\n", devfam_par->ee_addr_inc);
    fprintf(f, "user_id_hex_bytes   = %d\n", devfam_par->user_id_hex_bytes);
    fprintf(f, "user_id_bytes       = %d\n", devfam_par->user_id_bytes);
    fprintf(f, "prog_mem_hex_bytes  = %d\n", devfam_par->prog_mem_hex_bytes);
    fprintf(f, "ee_mem_hex_bytes    = %d\n", devfam_par->ee_mem_hex_bytes);
    fprintf(f, "prg_mem_shift       = %d\n", devfam_par->prg_mem_shift);
    fprintf(f, "test_memory_start   = %d\n", devfam_par->test_memory_start);
    fprintf(f, "test_memory_length  = %d\n", devfam_par->test_memory_length);
    fprintf(f, "vpp                 = %f\n", devfam_par->vpp);
}

void print_device(FILE * f, device_ent_t * devpart_par) {
    int j;

    fprintf(f, "part_name                 = \"%s\"\n", devpart_par->part_name);
    fprintf(f, "family                    = \"%s\"\n", family_name(devpart_par->family));
    fprintf(f, "device_id                 = 0x%08x\n", devpart_par->device_id);
    fprintf(f, "program_mem               = 0x%08x\n", devpart_par->program_mem);
    fprintf(f, "ee_mem                    = 0x%04x\n", devpart_par->ee_mem);
    fprintf(f, "ee_addr                   = 0x%08x\n", devpart_par->ee_addr);
    fprintf(f, "config_words              = %d\n", devpart_par->config_words);
    fprintf(f, "config_addr               = 0x%08x\n", devpart_par->config_addr);
    fprintf(f, "user_id_words             = %d\n", devpart_par->user_id_words);
    fprintf(f, "user_id_addr              = 0x%08x\n", devpart_par->user_id_addr);
    fprintf(f, "bandgap_mask              = 0x%08x\n", devpart_par->bandgap_mask);

    fprintf(f, "config_masks              = {");
    for (j = 0; j < 7; j++)
	fprintf(f, "0x%04x, ", devpart_par->config_masks[j]);
    fprintf(f, "0x%04x}\n", devpart_par->config_masks[7]);

    fprintf(f, "config_blank              = {");
    for (j = 0; j < 7; j++)
	fprintf(f, "0x%04x, ", devpart_par->config_blank[j]);
    fprintf(f, "0x%04x}\n", devpart_par->config_blank[7]);

    fprintf(f, "cp_mask                   = 0x%04x\n", devpart_par->cp_mask);
    fprintf(f, "cp_config                 = 0x%04x\n", devpart_par->cp_config);
    fprintf(f, "osscal_save               = %d\n", devpart_par->osscal_save);
    fprintf(f, "ignore_address            = 0x%08x\n", devpart_par->ignore_address);
    fprintf(f, "vdd_min                   = %f\n", devpart_par->vdd_min);
    fprintf(f, "vdd_max                   = %f\n", devpart_par->vdd_max);
    fprintf(f, "vdd_erase                 = %f\n", devpart_par->vdd_erase);
    fprintf(f, "calibration_words         = %d\n", devpart_par->calibration_words);
    fprintf(f, "chip_erase_script         = \"%s\"\n", script_name(devpart_par->chip_erase_script));
    fprintf(f, "progmem_addr_set_script   = \"%s\"\n", script_name(devpart_par->progmem_addr_set_script));
    fprintf(f, "progmem_addr_bytes        = %d\n", devpart_par->progmem_addr_bytes);
    fprintf(f, "progmem_rd_script         = \"%s\"\n", script_name(devpart_par->progmem_rd_script));
    fprintf(f, "progmem_rd_words          = %d\n", devpart_par->progmem_rd_words);
    fprintf(f, "eerd_prep_script          = \"%s\"\n", script_name(devpart_par->eerd_prep_script));
    fprintf(f, "eerd_script               = \"%s\"\n", script_name(devpart_par->eerd_script));
    fprintf(f, "eerd_locations            = %d\n", devpart_par->eerd_locations);
    fprintf(f, "user_id_rd_prep_script    = \"%s\"\n", script_name(devpart_par->user_id_rd_prep_script));
    fprintf(f, "user_id_rd_script         = \"%s\"\n", script_name(devpart_par->user_id_rd_script));
    fprintf(f, "config_rd_prep_script     = \"%s\"\n", script_name(devpart_par->config_rd_prep_script));
    fprintf(f, "config_rd_script          = \"%s\"\n", script_name(devpart_par->config_rd_script));
    fprintf(f, "progmem_wr_prep_script    = \"%s\"\n", script_name(devpart_par->progmem_wr_prep_script));
    fprintf(f, "progmem_wr_script         = \"%s\"\n", script_name(devpart_par->progmem_wr_script));
    fprintf(f, "progmem_wr_words          = %d\n", devpart_par->progmem_wr_words);
    fprintf(f, "progmem_panel_bufs        = %d\n", devpart_par->progmem_panel_bufs);
    fprintf(f, "progmem_panel_offset      = %d\n", devpart_par->progmem_panel_offset);
    fprintf(f, "eewr_prep_script          = \"%s\"\n", script_name(devpart_par->eewr_prep_script));
    fprintf(f, "eewr_script               = \"%s\"\n", script_name(devpart_par->eewr_script));
    fprintf(f, "eewr_locations            = %d\n", devpart_par->eewr_locations);
    fprintf(f, "user_id_wr_prep_script    = \"%s\"\n", script_name(devpart_par->user_id_wr_prep_script));
    fprintf(f, "user_id_wr_script         = \"%s\"\n", script_name(devpart_par->user_id_wr_script));
    fprintf(f, "config_wr_prep_script     = \"%s\"\n", script_name(devpart_par->config_wr_prep_script));
    fprintf(f, "config_wr_script          = \"%s\"\n", script_name(devpart_par->config_wr_script));
    fprintf(f, "osccal_rd_script          = \"%s\"\n", script_name(devpart_par->osccal_rd_script));
    fprintf(f, "osccal_wr_script          = \"%s\"\n", script_name(devpart_par->osccal_wr_script));
    fprintf(f, "dp_mask                   = 0x%04x\n", devpart_par->dp_mask);
    fprintf(f, "write_cfg_on_erase        = %d\n", devpart_par->write_cfg_on_erase);
    fprintf(f, "blank_check_skip_usr_ids  = %d\n", devpart_par->blank_check_skip_usr_ids);
    fprintf(f, "ignore_bytes              = %d\n", devpart_par->ignore_bytes);
    fprintf(f, "chip_erase_prep_script    = \"%s\"\n", script_name(devpart_par->chip_erase_prep_script));
    fprintf(f, "boot_flash                = %d\n", devpart_par->boot_flash);
    fprintf(f, "config9_mask              = 0x%04x\n", devpart_par->config9_mask);
    fprintf(f, "config9_blank             = 0x%04x\n", devpart_par->config9_blank);
    fprintf(f, "progmem_erase_script      = \"%s\"\n", script_name(devpart_par->progmem_erase_script));
    fprintf(f, "eemem_erase_script        = \"%s\"\n", script_name(devpart_par->eemem_erase_script));
    fprintf(f, "configmem_erase_script    = \"%s\"\n", script_name(devpart_par->configmem_erase_script));
    fprintf(f, "reserved1_erase_script    = \"%s\"\n", script_name(devpart_par->reserved1_erase_script));
    fprintf(f, "reserved2_erase_script    = \"%s\"\n", script_name(devpart_par->reserved2_erase_script));
    fprintf(f, "test_memory_rd_script     = \"%s\"\n", script_name(devpart_par->test_memory_rd_script));
    fprintf(f, "test_memory_rd_words      = %d\n", devpart_par->test_memory_rd_words);
    fprintf(f, "eerow_erase_script        = \"%s\"\n", script_name(devpart_par->eerow_erase_script));
    fprintf(f, "eerow_erase_words         = %d\n", devpart_par->eerow_erase_words);
    fprintf(f, "export_to_mplab           = %d\n", devpart_par->export_to_mplab);
    fprintf(f, "debug_halt_script         = \"%s\"\n", script_name(devpart_par->debug_halt_script));
    fprintf(f, "debug_run_script          = \"%s\"\n", script_name(devpart_par->debug_run_script));
    fprintf(f, "debug_status_script       = \"%s\"\n", script_name(devpart_par->debug_status_script));
    fprintf(f, "debug_read_exec_ver_script= \"%s\"\n", script_name(devpart_par->debug_read_exec_ver_script));
    fprintf(f, "debug_single_step_script  = \"%s\"\n", script_name(devpart_par->debug_single_step_script));
    fprintf(f, "debug_bulk_wr_data_script = \"%s\"\n", script_name(devpart_par->debug_bulk_wr_data_script));
    fprintf(f, "debug_bulk_rd_data_script = \"%s\"\n", script_name(devpart_par->debug_bulk_rd_data_script));
    fprintf(f, "debug_write_vector_script = \"%s\"\n", script_name(devpart_par->debug_write_vector_script));
    fprintf(f, "debug_read_vector_script  = \"%s\"\n", script_name(devpart_par->debug_read_vector_script));
    fprintf(f, "debug_row_erase_script    = \"%s\"\n", script_name(devpart_par->debug_row_erase_script));
    fprintf(f, "debug_row_erase_size      = %d\n", devpart_par->debug_row_erase_size);
    fprintf(f, "debug_reserved5_script    = \"%s\"\n", script_name(devpart_par->debug_reserved5_script));
    fprintf(f, "debug_reserved6_script    = \"%s\"\n", script_name(devpart_par->debug_reserved6_script));
    fprintf(f, "debug_reserved7_script    = \"%s\"\n", script_name(devpart_par->debug_reserved7_script));
    fprintf(f, "debug_reserved8_script    = \"%s\"\n", script_name(devpart_par->debug_reserved8_script));
    fprintf(f, "lvp_script                = \"%s\"\n", script_name(devpart_par->lvp_script));
}

void print_script(FILE * f, script_ent_t * script) {
    int i;
/*        uint8_t *ptr; */

    fprintf(f, "name          = \"%s\"\n", valid_string(script->name, g_tmp));
    if (strlen(script->org_name) > 0)
	fprintf(f, "org_name      = \"%s\"\n", valid_string(script->org_name, g_tmp));
    fprintf(f, "id            = %d\n", script->script_num);
    fprintf(f, "version       = %d\n", script->version);
    fprintf(f, "unused1       = %d\n", script->unused1);
/*	fprintf(f, "script_length    = %d\n", script->script_length); */
    fprintf(f, "script        = {");
/*	ptr = (uint8_t*)script->script; */
    for (i = 0; i < (script->script_length << 1); i++) {
	if ((i & 7) == 0)
	    fprintf(f, "\n");
	fprintf(f, " 0x%02x,", script->script[i]);
    }
    fprintf(f, "\n}\n");
    fprintf(f, "comment       = \"%s\"\n", valid_string(script->comment, g_tmp));
}

int rd_string(FILE * f, char *str) {
    uint8_t b;
    int len;

    fread(&b, 1, 1, f);
    len = b & 0x7f;
    if (b & 0x80) {
	fread(&b, 1, 1, f);
	len += b * 0x80;
    }

    fread(str, len, 1, f);
    str[len] = 0;

    return len;
}

int read_device_file(char *filename) {
    FILE *f;
    int i, j, n;

    if ((f = fopen(filename, "rb")) == NULL)
	return -1;

    /* read file header */
    memset(&hdr, 0, sizeof(dat_hdr_t));
    fread(&hdr, sizeof(int), 3, f);
    rd_string(f, &hdr.notes[0]);
    fread(&hdr.num_families, sizeof(dat_hdr_t) - offsetof(dat_hdr_t, num_families), 1, f);

    /* read family entries */
    families = (family_ent_t *) malloc(sizeof(family_ent_t) * hdr.num_families);
    memset(families, 0, sizeof(family_ent_t) * hdr.num_families);
    for (i = 0; i < hdr.num_families; i++) {
	fread(&families[i], offsetof(family_ent_t, name), 1, f);
	rd_string(f, &families[i].name[0]);
	fread(&families[i].prg_entry_script, sizeof(family_ent_t) - offsetof(family_ent_t, prg_entry_script), 1, f);
    }

    /* read device entries */
    devices = (device_ent_t *) malloc(sizeof(device_ent_t) * hdr.num_devices);
    memset(devices, 0, sizeof(device_ent_t) * hdr.num_devices);
    for (i = 0; i < hdr.num_devices; i++) {
	rd_string(f, &devices[i].part_name[0]);
	fread(&devices[i].family, sizeof(device_ent_t) - offsetof(device_ent_t, family), 1, f);
    }

    /* read script entries */
    scripts = (script_ent_t *) malloc(sizeof(script_ent_t) * hdr.num_scripts);
    memset(scripts, 0, sizeof(script_ent_t) * hdr.num_scripts);
    for (i = 0; i < hdr.num_scripts; i++) {
	fread(&scripts[i], sizeof(uint16_t), 1, f);
	rd_string(f, &scripts[i].name[0]);
	fread(&scripts[i].version, offsetof(script_ent_t, script) - offsetof(script_ent_t, version), 1, f);
	fread(&scripts[i].script, scripts[i].script_length, sizeof(uint16_t), f);
	rd_string(f, &scripts[i].comment[0]);

	/* check if name already exists */
	n = 0;
	scripts[i].org_name[0] = 0;
	for (j = 0; j < i; j++)
	    if (strcmp(scripts[j].name, scripts[i].name) == 0)
		n++;
	if (n > 0) {
	    /* if there are more scripts that share the same name
	     * rename them, and store there orginal name in "org_name"
	     * member */
	    strcpy(scripts[i].org_name, scripts[i].name);
	    sprintf(scripts[i].name, "%s_%d", scripts[i].org_name, n);
	    printf("%d scripts with name \"%s\" -> renaming to \"%s\"\n", n, scripts[i].org_name, scripts[i].name);
	}
    }

    fclose(f);

    return 0;
}

void special_cat(char *dest, char *src) {
    char *pc, *pc2;

    pc = dest;
    pc2 = src;
    while (*pc2 != 0) {
	if (*pc2 == '/')
	    *pc++ = '_';
	else
	    *pc++ = *pc2;
	pc2++;
    }
    *pc = 0;
}

int dump_device_file(char *dumpdir) {
    char fname[255], fbase[255], *p;
    FILE *f, *fcfg;
    int i;

    /* create diretory */
    strcpy(fname, "mkdir ");
    strcat(fname, dumpdir);
    system(fname);

    /* write the config file */
    strcpy(fname, dumpdir);
    strcat(fname, "/config.cfg");
    if ((fcfg = fopen(fname, "w+")) == NULL)
	return -1;

    fprintf(fcfg, "# this configiration file was generated automatically\n");
    fprintf(fcfg, "# please modify it according to the rules :-)\n#\n");
    fprintf(fcfg, "# the source file contained:\n");
    fprintf(fcfg, "#   %d families,\n", hdr.num_families);
    fprintf(fcfg, "#   %d devices and\n", hdr.num_devices);
    fprintf(fcfg, "#   %d scripts.\n\n\n", hdr.num_scripts);
    fprintf(fcfg, "# for the device file header\n");
    fprintf(fcfg, "ver_major = %d\n", hdr.version[0]);
    fprintf(fcfg, "ver_minor = %d\n", hdr.version[1]);
    fprintf(fcfg, "ver_dot   = %d\n", hdr.version[2]);
    fprintf(fcfg, "compat    = %d\n", hdr.compat);
    fprintf(fcfg, "unused1a  = %d\n", hdr.unused1a);
    fprintf(fcfg, "unused1b  = %d\n", hdr.unused1b);
    fprintf(fcfg, "unused2   = %d\n", hdr.unused2);
    fprintf(fcfg, "notes     = \n\"%s\"\n\n\n", hdr.notes);

    /* write families */
    strcpy(fname, "mkdir ");
    strcat(fname, dumpdir);
    strcat(fname, "/families");
    system(fname);

    strcpy(fbase, dumpdir);
    strcat(fbase, "/families/");
    p = fbase + strlen(fbase);
    fprintf(fcfg, "# list of device family files\n");
    fprintf(fcfg, "families  = {\n");
    for (i = 0; i < hdr.num_families; i++) {
	special_cat(p, families[i].name);
	strcat(fbase, ".fam");
	printf("filename : \"%s\"\n", fbase);
	fprintf(fcfg, "\t\"%s\",\n", valid_string(fbase, g_tmp));
	if ((f = fopen(fbase, "w+")) == NULL) {
	    printf("error: cannot create file\n");
	    return -1;
	}
	print_family(f, &families[i]);
	fclose(f);
    }
    fprintf(fcfg, "}\n\n");

    /* write devices */
    strcpy(fname, "mkdir ");
    strcat(fname, dumpdir);
    strcat(fname, "/devices");
    system(fname);

    strcpy(fbase, dumpdir);
    strcat(fbase, "/devices/");
    p = fbase + strlen(fbase);
    fprintf(fcfg, "# list of device files\n");
    fprintf(fcfg, "devices   = {\n");
    for (i = 0; i < hdr.num_devices; i++) {
	special_cat(p, devices[i].part_name);
	strcat(fbase, ".dev");
	printf("filename : \"%s\"\n", fbase);
	fprintf(fcfg, "\t\"%s\",\n", valid_string(fbase, g_tmp));
	if ((f = fopen(fbase, "w+")) == NULL) {
	    printf("error: cannot create file\n");
	    return -1;
	}
	print_device(f, &devices[i]);
	fclose(f);
    }
    fprintf(fcfg, "}\n\n");

    /* write scripts */
    strcpy(fname, "mkdir ");
    strcat(fname, dumpdir);
    strcat(fname, "/scripts");
    system(fname);

    strcpy(fbase, dumpdir);
    strcat(fbase, "/scripts/");
    p = fbase + strlen(fbase);
    fprintf(fcfg, "# list of script files\n");
    fprintf(fcfg, "scripts   = {\n");
    for (i = 0; i < hdr.num_scripts; i++) {
	special_cat(p, scripts[i].name);
	strcat(fbase, ".scr");
	printf("filename : \"%s\"\n", fbase);
	fprintf(fcfg, "\t\"%s\",\n", valid_string(fbase, g_tmp));
	if ((f = fopen(fbase, "w+")) == NULL) {
	    printf("error: cannot create file\n");
	    return -1;
	}
	print_script(f, &scripts[i]);
	fclose(f);
    }
    fprintf(fcfg, "}");

    fclose(fcfg);

    return 0;
}

void write_str(FILE * f, char *str) {
    int slen = strlen(str);	/* -1; */

    if (slen > 0x7f) {
	uint8_t tmp;
	tmp = (slen & 0x7F) | 0x80;
	fwrite(&tmp, 1, 1, f);
	tmp = slen >> 7;
	fwrite(&tmp, 1, 1, f);
    } else {
	uint8_t tmp = slen;
	fwrite(&tmp, 1, 1, f);
    }

    fwrite(str, slen, 1, f);
}

int build_device_file(char *dir, char *outfile) {
    FILE *fout;
    cfg_t *pcfg;
    int i;
    char fname[255];
    dat_hdr_t newhdr;
    uint16_t scr_num;

    /* open the config file */
    strcpy(fname, dir);
    strcat(fname, "/config.cfg");
    pcfg = cfg_init(config_opts, CFGF_NONE);
    if (cfg_parse(pcfg, fname) == CFG_PARSE_ERROR)
	return -1;

    newhdr.version[0] = cfg_getint(pcfg, "ver_major");
    newhdr.version[1] = cfg_getint(pcfg, "ver_minor");
    newhdr.version[2] = cfg_getint(pcfg, "ver_dot");
    strcpy(newhdr.notes, cfg_getstr(pcfg, "notes"));
    newhdr.num_families = cfg_size(pcfg, "families");
    newhdr.num_devices = cfg_size(pcfg, "devices");
    newhdr.num_scripts = cfg_size(pcfg, "scripts");
    newhdr.compat = cfg_getint(pcfg, "compat");
    newhdr.unused1a = cfg_getint(pcfg, "unused1a");
    newhdr.unused1b = cfg_getint(pcfg, "unused1b");
    newhdr.unused2 = cfg_getint(pcfg, "unused2");

    num_rd_devices = newhdr.num_devices;
    num_rd_families = newhdr.num_families;
    num_rd_scripts = newhdr.num_scripts;

    printf("reading %d scripts\n", num_rd_scripts);
    printf("reading %d families\n", num_rd_families);
    printf("reading %d devices\n", num_rd_devices);

    /* read all scripts */
    if (scripts != NULL)
	free(scripts);
    scripts = (script_ent_t *) malloc(sizeof(script_ent_t) * num_rd_scripts);
    printf("reading scripts...");
    fflush(stdout);
    if (verbose > 0)
	printf("\n");
    for (i = 0; i < num_rd_scripts; i++) {
	if (verbose > 0)
	    printf("reading script file \"%s\"\n", cfg_getnstr(pcfg, "scripts", i));
	if (parse_script(cfg_getnstr(pcfg, "scripts", i), &scripts[i]) < 0) {
	    printf("PARSE ERROR!!!\n");
	    cfg_free(pcfg);
	    return -1;
	}
    }
    printf(" done\n");

    /* read all families */
    if (families != NULL)
	free(families);
    families = (family_ent_t *) malloc(sizeof(family_ent_t) * num_rd_families);
    printf("reading families...");
    fflush(stdout);
    if (verbose > 0)
	printf("\n");
    for (i = 0; i < num_rd_families; i++) {
	if (verbose > 0)
	    printf("reading family file \"%s\"\n", cfg_getnstr(pcfg, "families", i));
	if (parse_family(cfg_getnstr(pcfg, "families", i), &families[i]) < 0) {
	    printf("PARSE ERROR!!!\n");
	    cfg_free(pcfg);
	    return -1;
	}
    }
    printf(" done\n");

    /* read all devices */
    if (devices != NULL)
	free(devices);
    devices = (device_ent_t *) malloc(sizeof(device_ent_t) * num_rd_devices);
    printf("reading devices...");
    fflush(stdout);
    if (verbose > 0)
	printf("\n");
    for (i = 0; i < num_rd_devices; i++) {
	if (verbose > 0)
	    printf("reading device file \"%s\"\n", cfg_getnstr(pcfg, "devices", i));
	if (parse_device(cfg_getnstr(pcfg, "devices", i), &devices[i]) < 0) {
	    printf("PARSE ERROR!!!\n");
	    cfg_free(pcfg);
	    return -1;
	}
    }
    printf(" done\n");

    cfg_free(pcfg);

    /* write file header */
    if ((fout = fopen(outfile, "wb")) == NULL) {
	printf("error: unable to open output file \"%s\"\n", outfile);
	return -1;
    }
    fwrite(&newhdr, sizeof(int), 3, fout);
    write_str(fout, newhdr.notes);
    fwrite(&newhdr.num_families, sizeof(dat_hdr_t) - offsetof(dat_hdr_t, num_families), 1, fout);

    /* write all families */
    for (i = 0; i < num_rd_families; i++) {
	if (verbose > 0)
	    printf("writing \"%s\" to file\n", families[i].name);
	fwrite(&families[i], offsetof(family_ent_t, name), 1, fout);
	write_str(fout, families[i].name);
	fwrite(&families[i].prg_entry_script, sizeof(family_ent_t) - offsetof(family_ent_t, prg_entry_script), 1, fout);
    }

    /* write all devices */
    for (i = 0; i < num_rd_devices; i++) {
	if (verbose > 0)
	    printf("writing \"%s\" to file\n", devices[i].part_name);
	write_str(fout, devices[i].part_name);
	fwrite(&devices[i].family, sizeof(device_ent_t) - offsetof(device_ent_t, family), 1, fout);
    }

    fprintf(stdout, "start of scripts 0x%lx\n", ftell(fout));

    /* write all scripts */
    for (i = 0; i < num_rd_scripts; i++) {
	if (verbose > 0)
	    printf("writing \"%s\" to file\n", scripts[i].name);

	/* write script_num */
	if (scripts[i].script_num > 0) {
	    /*      printf("%4d - [%s]\n", scripts[i].script_num, scripts[i].name); */
	    fwrite(&scripts[i].script_num, sizeof(uint16_t), 1, fout);
	} else {
	    scr_num = i + 1;
	    /*      printf("scr_num = %d\n", scr_num); */
	    fwrite(&scr_num, sizeof(uint16_t), 1, fout);
	}

	/* write script name */
	if (strlen(scripts[i].org_name) > 0) {
	    printf("replace with original name!\n");
	    write_str(fout, scripts[i].org_name);
	} else
	    write_str(fout, scripts[i].name);

	fwrite(&scripts[i].version, sizeof(uint16_t), 1, fout);
	fwrite(&scripts[i].unused1, sizeof(uint32_t), 1, fout);
	fwrite(&scripts[i].script_length, sizeof(uint16_t), 1, fout);

	fwrite(scripts[i].script, scripts[i].script_length << 1, 1, fout);

	write_str(fout, scripts[i].comment);
    }

    fclose(fout);

    return 0;
}

#if 0
void dumphex(uint8_t * buff, int length) {
    int i, j;

    for (i = 0; i < length; i++) {
	if ((i & 15) == 0)
	    printf("\n%08X : ", i);
	printf("%02X ", buff[i]);
	if ((i & 15) == 15) {
	    printf(" - ");
	    for (j = (i - 15); j < i; j++) {
		if ((buff[j] > 0x20) && (buff[j] < 0x7f))
		    printf("%c", buff[j]);
		else
		    printf(".");
	    }
	}
    }
    printf("\n");
}
#endif
