#include "hscpoint.h"
#include "hscpoints.h"
#include "log.h"
#include "utils.h"
#include <fstream>
#include <hscpointparam.h>

std::map<int,std::string> subtype_names = {
    {   0, "UNK"},
    {   5, "IO"},
    {  20, "C300"},
    {  21, "CEEC300"},
    {  22, "IOLINK"},
    {  27, "SERIES_C_IO"},
    {  37, "PGM2"},
    {  38, "PBLINK"},
    {  39, "GENDSBPB"},
    {  40, "PIOMB"},
    {SYS_MODEL, "MODEL"},
    {SYS_SERVER, "SERVER"},
    {SYS_CONTROLLERS, "CONTROLLERS"},
    {SYS_CHANNEL, "CHANNEL"},
    {SYS_CONTROLLER, "CONTROLLER"},
    {SYS_PRINTERS, "PRINTERS"},
    {SYS_PRINTER, "PRINTER"},
    {SYS_SYSINTERFACES, "SYSINTERFACES"},
    {SYS_SYSINTERFACE, "SYSINTERFACE"},
    {SYS_STATIONS, "STATIONS"},
    {SYS_FLEXSTATIONS, "FLEXSTATIONS"},
    {SYS_CONSSTATIONS, "CONSSTATIONS"},
    {SYS_FLEXSTATION, "FLEXSTATION"},
    {SYS_CONSSTATION, "CONSSTATION"},
    {SYS_CONSOLES, "CONSOLES"},
    {SYS_CONSOLE, "CONSOLE"},
    {SYS_CSTNCLIENT, "CSTNCLIENT"},
    {SYS_UNASSIGNEDITEMS, "UNASSIGNEDITEMS"},
    {SYS_COMPUTERS, "COMPUTERS"},
    {SYS_COMPUTER, "COMPUTER"},
    {SYS_DOMAIN, "DOMAIN"},
    {SYS_ORGUNIT, "ORGUNIT"},
    {SYS_NETEQUIP, "NETEQUIP"},
    {SYS_SWITCH, "SWITCH"},
    {SYS_FTE, "FTE"},
    {SYS_FTECOMMUNITY, "FTECOMMUNITY"},
    {SYS_FTEDEVICE, "FTEDEVICE"},
    {SYS_9PSWITCH, "9PSWITCH"},
    {SYS_UNKNOWNCONTROLLERS, "UNKNOWNCONTROLLERS"},
    {SYS_PNTSRV_CONTROLLER, "PNTSRV_CONTROLLER"},
    {SYS_PNTSRV_CHANNEL, "PNTSRV_CHANNEL"},
    {SYS_PNTSRV_NETWORK, "PNTSRV_NETWORK"},
    {SYS_ORPHANACTS, "ORPHANACTS"}
};

HSCPoint::HSCPoint(int32_t point_number)
    : m_number (point_number)
    , m_index (0)
    , m_name ("")
    , m_data (nullptr)
{
    //
}

HSCPoint::HSCPoint(std::string name)
    : m_index (0)
    , m_name (name)
    , m_data (nullptr)
{
    m_number = static_cast<int32_t>(hsc_point_number(name.c_str()));
}

bool HSCPoint::get_data()
{
    if (m_data != nullptr)
        return true;

    Log::message(Log::LOG_VERBOSE, "HSCPoint::get_data():\n");

    if (!HSCPoints::check_point_index(m_number))
        return false;

    m_index = HSCPoints::get_idx(m_number);
    if ((m_number < 1) || (m_number > HSCPoints::get_count()) || (m_index == 0))
    {
        Log::message(Log::LOG_ERROR, "Invalid point index! (#%i<1) || (%i>%i) || (%i==0)\n", m_number,
                  m_number, HSCPoints::get_count(), m_index);
        return false;
    }

    m_data = reinterpret_cast<STApoint*>(shitol(POINTSID, m_index));
    if (m_data == nullptr)
    {
        Log::message(Log::LOG_ERROR, "Unable to get point data for point #%i!\n", m_number);
        return false;
    }

    int size = get_data_size();
    if (SHMSZ(1, m_data) < size)
    {
        SHMSZ(1, m_data);
        HSCPoints::set_idx(m_number, 0);
        Log::message(Log::LOG_ERROR, "Invalid data size(%i)!\n", size);
        return false;
    }

    if (m_data->size != size)
    {
        HSCPoints::set_idx(m_number, 0);
        Log::message(Log::LOG_ERROR, "Invalid data size(%i!=%i)!\n", m_data->size, size);
        return false;
    }

    return true;
}

void HSCPoint::recursive(int level, std::function<void(int,HSCPoint*)> on_point)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::recursive():\n");

    if (!get_data())
        return;

    on_point(level, this);

    if (get_childrens())
        for (int32_t child : m_childs)
            HSCPoints::get_point(child)->recursive(level + 1, on_point);
}

void HSCPoint::params_proc(std::function<void(HSCPointParam*)> on_param)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::params_proc():\n");

    if (!get_data())
        return;

    if (!get_params())
        return;

    for (HSCPointParam* param : m_params)
        on_param(param);
}

void HSCPoint::dump_to_file(std::string fn)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::dump_to_file(%s):\n", fn.c_str());

    if (get_data())
    {
        std::ofstream file(fn, std::ios::out | std::ios::binary | std::ios::app);
        file.write(reinterpret_cast<char*>(m_data), get_data_size());
        file.close();
    }
}

std::string HSCPoint::get_subtype_name()
{
    if (!get_data())
        return "";

    return subtype_names[m_data->subtype];
}

std::string HSCPoint::get_name()
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::get_name():\n");
    if (m_name.length() == 0)
    {
        if (!get_data())
            return "";

        char buf[PNT_ENTITYNAME_SZ], dst[PNT_ENTITYNAME_SZ];
        getch_ex(PRNAM, buf, PNT_ENTITYNAME_SZ);
        ftocstr(dst, PNT_ENTITYNAME_SZ, buf, PNT_ENTITYNAME_SZ);

        m_name = std::string(dst);
    }
    return m_name;
}

int HSCPoint::get_data_size()
{
    switch (m_data->type)
    {
    case STA:
        return sizeof(STApoint);
    case ANA:
        return sizeof(ANApoint);
    case ACC:
        return sizeof(ACCpoint);
    case ACS:
        return sizeof(ACSpoint);
    case ASH:
        return sizeof(ASHpoint);
    default:
        return sizeof(CDApoint);
    }
}

uint2* HSCPoint::decode_prmdef_ex(uint2 prmdef)
{
    int field = (prmdef >> 6) & 0x03;
    int offset = prmdef & 0x003f;
    Log::message(Log::LOG_VERBOSE, "HSCPoint::decode_prmdef_ex(): len=%i, field=%i, offset=%i\n",
                 prmdef >> 8, field, offset);
    int pos = 0;
    uint16_t *start = reinterpret_cast<uint16_t*>(m_data);
    int size;
    switch (m_data->type)
    {
    case STA:
        switch (field)
        {
        case 0:
            pos = 100;
            size = 24;
            break;
        case 1:
            pos = 124;
            size = 4;
            break;
        case 2:
            pos = 128;
            size = 64;
            break;
        case 3:
            pos = 192;
            size = 128;
            break;
        default:
            start = reinterpret_cast<uint16_t*>(prmdef);
            size = prmdef;
            break;
        }
        break;
    case ANA:
        switch (field)
        {
        case 0:
            pos = 100;
            size = 44;
            break;
        case 1:
            pos = (73 * 2);
            size = 48;
            break;
        case 2:
            pos = (96 * 2);
            size = 64;
            break;
        case 3:
            pos = (128 * 2);
            size = 128;
            break;
        default:
            start = reinterpret_cast<uint16_t*>(prmdef);
            size = prmdef;
            break;
        }
        break;
    case ACC:
        switch (field)
        {
        case 0:
            pos = (50 * 2);
            size = 36;
            break;
        case 1:
            pos = (68 * 2);
            size = 32;
            break;
        case 2:
            pos = (84 * 2);
            size = 16;
            break;
        case 3:
            pos = (92 * 2);
            size = 128;
            break;
        default:
            start = reinterpret_cast<uint16_t*>(prmdef);
            size = prmdef;
            break;
        }
        break;
    case ACS:
        switch (field)
        {
        case 0:
            pos = (50 * 2);
            size = 16;
            break;
        case 1:
            pos = (58 * 2);
            size = 64;
            break;
        case 2:
            pos = (90 * 2);
            size = 64;
            break;
        case 3:
            pos = (122 * 2);
            size = 128;
            break;
        default:
            start = reinterpret_cast<uint16_t*>(prmdef);
            size = prmdef;
            break;
        }
        break;
    case ASH:
    case CLH:
    case SYS:
    case ORH:
    case EQU:
    default:
        switch (field)
        {
        case 1:
            pos = (56 * 2);
            size = 4;
            break;
        case 3:
            pos = 0xf0;
            size = 128;
            break;
        case 0:
            pos = (50 * 2);
            size = 12;
            break;
        case 2:
            pos = (58 * 2);
            size = 4;
            break;
        default:
            start = reinterpret_cast<uint16_t*>(prmdef);
            size = prmdef;
            break;
        }
        break;
    }

    if ((offset < 1) || (offset > size))
        return nullptr;
    return &start[(pos / 2) + offset - 1];
}

int HSCPoint::getch_ex(uint2 prmdef, char *value, short sizeofvalue)
{
    uint2 *buf = decode_prmdef_ex(prmdef);
    if (buf == nullptr)
        return -1;
    __intchr(value, sizeofvalue, (char*)buf, prmdef >> 8);
    return 0;
}

bool HSCPoint::get_childrens()
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::get_childrens():\n");

    if (!get_data())
        return false;

    if (m_childs.size() > 0)
        return true;

    char *addr = shitol(GDAID, m_data->children);
    int32_t size = SHMSZ(GDAID, addr);
    if (size < 1)
        return false;

    int32_t *childs = reinterpret_cast<int32_t*>(addr);
    int idx;
    for (idx=0 ; childs[idx] != 0 ; ++idx)
        m_childs.push_back(childs[idx]);
    /*int count = 0;;
    PNTNUM *childs = nullptr;
    if (hsc_point_get_containment_children(m_number, &count, &childs) == 0)
        for (int i=0 ; i<count ; ++i)
            m_childs.push_back(childs[i]);
    hsc_em_FreePointList(childs);*/
    Log::message(Log::LOG_INFO, "Point has %i childrens\n", idx);

    return true;;
}

bool HSCPoint::get_params()
{
    Log::message(Log::LOG_VERBOSE, "HSCPoint::get_params():\n");

    if (m_params.size() > 0)
        return  true;

    enumlist* list;
    int n = hsc_param_list_create(static_cast<PNTNUM>(m_number), &list);
    for (int i=0 ; i<n ; ++i)
        if (HSCPointParam::is_param_acessible(list[i].value))
        {
            HSCPointParam *param = new HSCPointParam(this, list[i].value, std::string(list[i].text));
            m_params.push_back(param);
        }
    hsc_enumlist_destroy(&list);

    return true;
}
