#include "gbdata.h"
#include "log.h"
#include "utils.h"
#include "hscserver.h"
#include <iostream>
#include <fstream>

#define DB_KEY 0x90000
#define DB_PREFIX "Global\\"
#define DB_SYS90 "GBsys90"
#define DB_DIRTRY "GBdirtry"

#define FMAP_ACCES (STANDARD_RIGHTS_REQUIRED | SECTION_QUERY | SECTION_MAP_WRITE | SECTION_MAP_READ)

const int32_t db_sizes[] = {
             16000,
             279480,
             612000,
             600600,
             16000,
             96096,
             4096,
             9624,
             2000,
             2500,
             4000,
             15984,
             92,
             139880,
             5600,
             6803400,
             142856,
             282716,
             10240,
             12672,
             127872,
             1000000,
             1000000,
             1000000,
             19406,
             2,
             2,
             2400000,
             1000000,
             32000,
             393204,
             393204,
             5120,
             3200,
             41984,
             83968,
             50000,
             3133440,
             4096,
             10020,
             499500,
             2,
             2,
             42624,
             57600,
             160,
             1280,
             41984,
             1040,
             64000,
             262144,
             0};

GBData::GBData()
{
    m_db_loaded = false;
    m_unk001 = false;
}

GBData::~GBData()
{
    delete m_gb_base;
}

extern void **FGBcmn;

bool GBData::load()
{
    Log::message(Log::LOG_VERBOSE, "GBData::load\n");

    #ifdef DATA_DIR
    if (!load_nt(0))
        return false;
    #else
    if (c_gbload() != 0)
        return false;

    for (int i=0 ; i<TABLES_COUNT ; ++i)
        m_gbs.ptr[i] = FGBcmn[i];

    #endif

    return true;
}

void GBData::save_to_file()
{
    Log::message(Log::LOG_VERBOSE, "CALL GBData::save_to_file\n");

    std::ofstream file("gbdata.dat", std::ios::out | std::ios::binary | std::ios::app);
    file.write(reinterpret_cast<char*>(m_sys90), 0x804);
    file.write(reinterpret_cast<char*>(m_gb_base), sizeof(int) * FILES_COUNT);
    for (int i=0 ; i<TABLES_COUNT ; ++i)
        file.write(reinterpret_cast<char*>(m_gbs.ptr[i]), db_sizes[i] * 2);
    file.close();
}

bool GBData::load_from_file()
{
    Log::message(Log::LOG_VERBOSE, "CALL GBData::load_from_file %x\n", sizeof(sys90));

    std::ifstream file("gbdata.dat", std::ios::in | std::ios::binary | std::ios::app);

    m_sys90 = (sys90*)malloc(0x804);
    file.read(reinterpret_cast<char*>(m_sys90), 0x804);

    m_gb_base = new int*[FILES_COUNT];
    file.read(reinterpret_cast<char*>(m_gb_base), sizeof(int) * FILES_COUNT);

    for (int i=0 ; i<TABLES_COUNT ; ++i)
    {
        m_gbs.ptr[i] = malloc(db_sizes[i] * 2);
        file.read(reinterpret_cast<char*>(m_gbs.ptr[i]), db_sizes[i] * 2);
    }

    file.close();

    Log::message(Log::LOG_VERBOSE, "\tif (shInitialise(m_sys90, m_gbs.ptr))\n");
    if (shInitialise(m_sys90, m_gbs.ptr))
        return false;

    Log::message(Log::LOG_VERBOSE, "\tOK\n");
    return true;
}

bool GBData::load_nt(bool is_server)
{
    Log::message(Log::LOG_VERBOSE, "GBData::load_nt\n");

    if ((!is_server) && (!is_dbservice_running()))
        return false;

    if (!gbattach_nt(is_server))
        return false;

    if (shInitialise(m_sys90, m_gbs.ptr))
        return false;

    //SetUnhandledExceptionFilter() TODO

    //set_secure(GetCurrentProcess(), 6);
    //set_secure(GetCurrentThread(), 6);

    //if (m_sys90)

    return true;
}

bool GBData::is_dbservice_running()
{
    Log::message(Log::LOG_VERBOSE, "CALL GBData::is_dbservice_running\n");
    SC_HANDLE manager = OpenSCManagerA(nullptr, nullptr, 0x80000000);
    if (manager == nullptr)
    {
        Log::message(Log::LOG_ERROR, "Can't connect to Service Control Manager!\n");
        return false;
    }
    Log::message(Log::LOG_VERBOSE, "\t OpenServiceA\n");
    SC_HANDLE service = OpenServiceA(manager, "HSCSERVER_Database", SERVICE_QUERY_STATUS);
    if (service == nullptr)
    {
        Log::message(Log::LOG_ERROR, "Unable to query database service!\n");
        CloseServiceHandle(manager);
        return false;
    }
    SERVICE_STATUS status;
    Log::message(Log::LOG_VERBOSE, "\t QueryServiceStatus (%p)\n", service);
    if (QueryServiceStatus(service, &status))
    {
        Log::message(Log::LOG_VERBOSE, "\t running\n");
        bool ret = ((status.dwCurrentState == SERVICE_START_PENDING) || (status.dwCurrentState == SERVICE_RUNNING));
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
        return ret;
    }
    else
    {
        Log::message(Log::LOG_VERBOSE, "\t not running\n");
        Log::message(Log::LOG_ERROR, "Unable to query database service status!\n");
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
        return false;
    }
}

bool GBData::gbattach_nt(bool is_server)
{
    Log::message(Log::LOG_VERBOSE, "CALL GBData::gbattach_nt\n");

    m_sys90 = static_cast<sys90*>(map_file(DB_SYS90, is_server, 0x804));
    if (m_sys90 == nullptr)
        return false;

    m_gb_base = new int*[FILES_COUNT];
    if (m_gb_base == nullptr)
    {
        Log::message(Log::LOG_ERROR, "Not enough memory for GBbase!\n");
        return false;
    }
    memset(m_gb_base, 0, sizeof(int*) * FILES_COUNT);

    m_gbs.data.DIRTRY = static_cast<dirtry*>(map_file(DB_DIRTRY, is_server, db_sizes[0] * 2));
    if (m_gbs.data.DIRTRY == nullptr)
        return false;

    if (is_server)
    {
        Log::message(Log::LOG_ERROR, "Server functions not implemented!\n");
        return false;
    }

    if ((m_gbs.data.DIRTRY->dflag & DMRES) != DMRES)
    {
        Log::message(Log::LOG_ERROR, "Directory not in memory!\n");
        return false;
    }

    for (int dir_idx=1, db_idx=1 ; dir_idx<NUM_FILES ; ++dir_idx)
    {
        if ((m_gbs.data.DIRTRY[dir_idx].dflag & DMRES) != DMRES)
            continue;

        int32_t map_size = db_sizes[db_idx] * 2;
        if (map_size == 0)
            break;

        char buf[20], b1[20];
        __intchr(b1, 20, m_gbs.data.DIRTRY[dir_idx].filnam, DIR_FILNAMLN);
        ftocstr(buf, 20, b1, 20);
        void* val = map_file(buf, is_server, map_size);
        if (val == nullptr)
            return false;
        m_gbs.ptr[db_idx++] = val;
    }

    m_db_loaded = true;
    init_API_access();

    return true;
}

bool GBData::init_API_access()
{
    Log::message(Log::LOG_VERBOSE, "CALL GBData::init_API_access\n");
    if ((m_api_acs == 0) && (!InterlockedCompareExchange(&m_api_acs, 1, 0)))
    {
        InitializeCriticalSection(&m_crit_sec);
        InterlockedExchange(&m_api_acs, 2);
    }
    while (m_api_acs == 1)
        Sleep(1);
    if (m_api_acs == 2)
        return true;
    return false;
}

void GBData::set_secure(HANDLE handle, uint32_t value)
{
    (void)(handle);
    (void)(value);

    if (!m_unk001)
    {
        std::string groups = HSCServer::get_reg_entry_silent("Group");
        if (groups.size() > 0)
        {
            Log::message(Log::LOG_VERBOSE, "CALL GBData::set_secure %s\n", groups.c_str());

            std::deque<std::string> entries;
            m_entries_count = 1;
            size_t start = 0;
            for (size_t i=0 ; i<groups.size() ; ++i)
                if (groups[i] == ';')
                {
                    entries.push_back(groups.substr(start, i - start - 1));
                    start = i + 1;
                    ++m_entries_count;
                }
            entries.push_back(groups.substr(start));

            m_entries = new EXPLICIT_ACCESS_A[m_entries_count];
            m_SID = new SID[m_entries_count];

            for (uint32_t i=0 ; i<m_entries_count ; ++i)
            {
                DWORD cbSid;
                char ref_domain[255];
                DWORD cch_domain;
                SID_NAME_USE sid_use;
                if (LookupAccountNameA(nullptr, entries[i].c_str(), &m_SID[i], &cbSid, ref_domain, &cch_domain, &sid_use))
                    break;

               /* m_entries[i].grfAccessMode = 2;
                m_entries[i].Trustee.TrusteeType = 2;
                m_entries[i].grfAccessPermissions = 0x1fffff;
                m_entries[i].grfInheritance = 0;
                m_entries[i].Trustee.pMultipleTrustee = nullptr;
                m_entries[i].Trustee.MultipleTrusteeOperation = 0;
                m_entries[i].Trustee.TrusteeForm = 0;
                m_entries[i].Trustee.ptstrName = ;*/
                //TODO
            }
        }
    }
}

void* GBData::map_file(const char *name, bool is_server, DWORD size)
{
    char fn[255];
    sprintf(fn, "%s%s_%x", DB_PREFIX, name, DB_KEY);
    Log::message(Log::LOG_VERBOSE, "CALL GBData::map_file(%s, 0x%x)\n", fn, size);

    HANDLE handle;
    if (is_server)
    {
        handle = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size, fn);
        if (handle == nullptr)
        {
            Log::message(Log::LOG_ERROR, "%s CreateFileMapping error!\n", name);
            return nullptr;
        }
        set_secure(handle, 0x80000006);
    }
    else
    {
        handle = OpenFileMappingA(FMAP_ACCES, false, fn);
        if (handle == nullptr)
        {
            switch (errno)
            {
            case ENOENT:
                Log::message(Log::LOG_ERROR, "Server database is not loaded!\n");
                return nullptr;
            case EIO:
                Log::message(Log::LOG_ERROR, "Insufficent priveledges to access Server database!\n");
                return nullptr;
            default:
                Log::message(Log::LOG_ERROR, "Server database cannot be accessed!\n");
                return nullptr;
            }
        }
    }

    void* ret = MapViewOfFile(handle, FILE_MAP_WRITE, 0, 0, size);
    if (ret == nullptr)
    {
        Log::message(Log::LOG_ERROR, "%s MapViewOfFile error (%u)!\n", name, GetLastError());
        return nullptr;
    }

    return ret;
}
