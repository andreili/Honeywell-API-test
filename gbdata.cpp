#include "gbdata.h"
#include "log.h"
#include <windows.h>
#include "hsc_include.h"

//#define GBLD
#include "src/GBcrttbl.h"
#include "src/GBpbttbl.h"
#include "src/GBgdatbl.h"
#include "src/GBtrbtbl.h"
#include "src/GBaastbl.h"
#include "src/GBsysflg.h"
#include "src/GBdiotbl.h"
#include "src/GBwdttbl.h"
//#include "src/GBtimvec.h"
#include "src/GBlogtbl.h"
#include "src/GBperlog.h"
#include "src/GBscnpri.h"
#include "src/GBscntbl.h"
#include "src/GBlfntbl.h"
#include "src/GBprtque.h"
#include "src/GBscnfmt.h"
#include "src/GBchntbl.h"
#include "src/GBrtutbl.h"
//#include "src/GBhatfst.h" -- in GBhatslw.h
#include "src/GBhatslw.h"
//#include "src/GBhatext.h" -- in GBhatslw.h
//#include "src/GBhatexc.h" -- in GBhatslw.h
#include "src/GBseclvl.h"
#include "src/GBsoekys.h"
#include "src/GBevtkys.h"
//#include "src/GBatttbl.h"
//#include "src/GBcattbl.h"
//#include "src/GBdsntbl.h"
//#include "src/GBdsrtbl.h"
#include "src/GBwndtbl.h"
#include "src/GBsyssts.h"
#include "src/GBschedl.h"
#include "src/GBdevtbl.h"
#include "src/GBscript.h"
#include "src/GBcontbl.h"
#include "src/GBalmshv.h"

#include "src/GBsys90.h"
#include "src/GBbase.h"

extern short**	FGBcmn;

bool GBData::load()
{
    if (!load_nt(0))
        return false;

   /* GBsys90 = c_dll_GBsys90();
    GBbase = c_dll_GBbase();
    //TODO

    GBdirtry = reinterpret_cast<DIRTRY>(GBbase[0]);
    _imp__GBC000 = reinterpret_cast<short*>(GBbase[0]);

    #include "gbattach_nt1.h"
    for (int db_idx=1, dir_idx=1, base_idx=4 ; dir_idx<NUM_FILES ; dir_idx++, base_idx++)
    {;
        if ((GBdirtry[dir_idx].dflag & DMRES) && (db_size[db_idx]))
            FGBcmn[db_idx++] = reinterpret_cast<short*>(GBbase[base_idx]);
    }

    #include "gbattach_nt.in.c"*/

    return true;
}

bool GBData::load_nt(int par)
{
    Log::message(Log::LOG_VERBOSE, "GBData::load_nt\n");
    if ((par == 0) && (!is_dbservice_running()))
        return false;
    return true;
}

bool GBData::is_dbservice_running()
{
    Log::message(Log::LOG_VERBOSE, "CALL __hsc_is_dbservice_running\n");
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
        bool ret = ((status.dwCurrentState == SERVICE_START_PENDING) || (status.dwCurrentState == SERVICE_RUNNING));
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
        return ret;
    }
    else
    {
        Log::message(Log::LOG_ERROR, "Unable to query database service status!\n");
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
        return false;
    }
}
