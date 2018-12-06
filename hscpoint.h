#ifndef HSCPOINT_H
#define HSCPOINT_H

#include "hsc_include.h"
#include <string>
#include <inttypes.h>
#include <functional>
#include <deque>
#include <map>

class HSCPointParam;

// types
#define	PNT_STA	1
#define	PNT_ANA	2
#define	PNT_ACC	3
#define	PNT_ACS	4
#define	PNT_CON	5
#define	PNT_ASH	6
#define	PNT_CLH	7
#define	PNT_SYS	8
#define	PNT_ORH	9
#define	PNT_EQU	10
#define	PNT_CDA	101
#define	PNT_RDA	102
#define	PNT_PSA	103
#define	PNT_ACT	104

// subtypes
#define	SYS_UNK   0
#define	SYS_IO   5
#define	SYS_C300  20
#define	SYS_CEEC300  21
#define	SYS_IOLINK  22
#define	SYS_SERIES_C_IO  27
#define	SYS_PGM2  37
#define	SYS_PBLINK  38
#define	SYS_GENDSBPB  39
#define	SYS_PIOMB  40
#define	SYS_MODEL	1001
#define	SYS_SERVER	1002
#define	SYS_CONTROLLERS	1003
#define	SYS_CHANNEL	1004
#define	SYS_CONTROLLER	1005
#define	SYS_PRINTERS	1006
#define	SYS_PRINTER	1007
#define	SYS_SYSINTERFACES	1008
#define	SYS_SYSINTERFACE	1009
#define	SYS_STATIONS	1010
#define	SYS_FLEXSTATIONS	1011
#define	SYS_CONSSTATIONS	1012
#define	SYS_FLEXSTATION	1013
#define	SYS_CONSSTATION	1014
#define	SYS_CONSOLES	1015
#define	SYS_CONSOLE	1016
#define	SYS_CSTNCLIENT	1017
#define	SYS_UNASSIGNEDITEMS	1018
#define	SYS_COMPUTERS	1019
#define	SYS_COMPUTER	1020
#define	SYS_DOMAIN	1021
#define	SYS_ORGUNIT	1022
#define	SYS_NETEQUIP	1023
#define	SYS_SWITCH	1024
#define	SYS_FTE	1025
#define	SYS_FTECOMMUNITY	1026
#define	SYS_FTEDEVICE	1027
#define	SYS_9PSWITCH	1028
#define	SYS_UNKNOWNCONTROLLERS	1029
#define	SYS_PNTSRV_SYSTEM_HIDDEN	1001
#define	SYS_PNTSRV_CONTROLLER	1030
#define	SYS_PNTSRV_CHANNEL	1031
#define	SYS_PNTSRV_NETWORK	1032
#define	SYS_ORPHANACTS	1033

class HSCPoint
{
public:
    HSCPoint(int32_t point_number);
    HSCPoint(std::string name);

    void find_parent();

    bool get_data();

    void recursive(int level, std::function<void(int,HSCPoint*)> on_point);
    void params_proc(std::function<void(HSCPointParam*)> on_param);

    void dump_to_file(std::string fn);

    STApoint* get_data_ptr() { return m_data; }
    int32_t get_number() { return m_number; }
    uint16_t get_type() { return m_data->type; }

    uint16_t get_subtype() { return m_data->subtype; }
    std::string get_subtype_name();

    GINDX get_parameters() { return m_data->parameters; }

    std::string get_name();

    int get_data_size();
private:
    int32_t             m_number;
    int32_t             m_index;
    std::string         m_name;
    STApoint            *m_data;
    std::deque<int32_t> m_childs;
    std::deque<HSCPointParam*>  m_params;

    uint2* decode_prmdef_ex(uint2 prmdef);
    int getch_ex(uint2 prmdef, char *value, short sizeofvalue);
    bool get_childrens();
    bool get_params();
};

#endif // HSCPOINT_H
