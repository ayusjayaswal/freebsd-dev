/*
 * following is an simple implementation of uname using libxo
 *
 * compile with: gcc -o main main.c -lxo
 *
 * authored by ayush jayaswal... ;-)
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <libxo/xo.h>
#include <stdbool.h>

void
usage(const char *prog)
{
    fprintf(stderr, "usage: %s same as uname -{ahmnorsv} do man uname for more\n", prog);
    fprintf(stderr, "  --libxo=style   select output style (xml, json, text, html, etc.)\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    struct utsname uts;
    int opt;
    bool all = false;
    bool print_sysname = false;
    bool print_nodename = false;
    bool print_release = false;
    bool print_version = false;
    bool print_machine = false;
    bool print_platform = false;
    bool print_arch = false;

    argc = xo_parse_args(argc, argv);
    if (argc < 0)
        exit(1);

    xo_set_program(argv[0]);
    xo_set_version("1.0");

    //since switch-case is faster than ifs...
    while ((opt = getopt(argc, argv, "ahmnorsv")) != -1) {
        switch (opt) {
        case 'a':
            all = true;
            break;
        case 'h':
            usage(argv[0]);
            break;
        case 'm':
            print_machine = true;
            break;
        case 'n':
            print_nodename = true;
            break;
        case 'o':
        case 's':
            print_sysname = true;
            break;
        case 'r':
            print_release = true;
            break;
        case 'v':
            print_version = true;
            break;
        default:
            usage(argv[0]);
        }
    }

    if (!all && !print_sysname && !print_nodename && !print_release &&
        !print_version && !print_machine && !print_platform && !print_arch) {
        print_sysname = true;
    }

    if (uname(&uts) < 0) {
        xo_err(1, "uname");
    }

    xo_open_container("uname");
    xo_open_list("system_info");

    if (all || print_sysname) {
        xo_open_instance("system_info");
        xo_emit("{:type/%s}{:value/%s}", "sysname", uts.sysname);
        xo_close_instance("system_info");
    }

    if (all || print_nodename) {
        xo_open_instance("system_info");
        xo_emit("{:type/%s}{:value/%s}", "nodename", uts.nodename);
        xo_close_instance("system_info");
    }

    if (all || print_release) {
        xo_open_instance("system_info");
        xo_emit("{:type/%s}{:value/%s}", "release", uts.release);
        xo_close_instance("system_info");
    }

    if (all || print_version) {
        xo_open_instance("system_info");
        xo_emit("{:type/%s}{:value/%s}", "version", uts.version);
        xo_close_instance("system_info");
    }

    if (all || print_machine) {
        xo_open_instance("system_info");
        xo_emit("{:type/%s}{:value/%s}", "machine", uts.machine);
        xo_close_instance("system_info");
    }


    xo_close_list("system_info");
    xo_close_container("uname");
    xo_finish();

    return 0;

    //thanx!! :-)
}
