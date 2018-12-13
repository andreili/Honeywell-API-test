#ifndef GBDATA_H
#define GBDATA_H

#include <windows.h>
#include <inttypes.h>
#include <accctrl.h>

#include "hsc_include.h"

#include "src/GBsys90.h"
#include "src/dirtry_def"
#include "src/gdatbl_def"
#include "src/pbttbl_def"
#include "src/sysflg_def"
#include "src/crttbl_def"
#include "src/aastbl_def"
#include "src/GBtrbtbl.h"
#include "src/diotbl_def"
#include "src/wdttbl_def"
#include "src/logtbl_def"
#include "src/perlog_def"
#include "src/scnpri_def"
#include "src/scntbl_def"
#include "src/lfntbl_def"
#include "src/prtque_def"
#include "src/scnfmt_def"
#include "src/chntbl_def"
#include "src/rtutbl_def"
#include "src/hatslw_def"
#include "src/seclvl_def"
#include "src/evtkys_def"
#include "src/soekys_def"
#include "src/atttbl_def"
#include "src/cattbl_def"
#include "src/dsntbl_def"
#include "src/dsrtbl_def"
#include "src/alguse_def"
#include "src/wndtbl_def"
#include "src/syssts_def"
#include "src/schedl_def"
#include "src/devtbl_def"
#include "src/script_def"
#include "src/contbl_def"
#include "src/almshv_def"
#include "src/acrnym_def"
#include "src/almdir_def"
#include "src/algblk_def"
#include "src/scnlst_def"
#include "src/msgdir_def"
#include "src/tdclib_def"
#include "src/cmdtbl_def"


typedef struct t_WNDTBL t2_WNDTBL[NUM_WNDS];

#define FILES_COUNT 400
#define TABLES_COUNT 51

#define	DIRTRY_T	0
#define	CRTTBL_T	1
#define	PBTTBL_T	2
#define	GDATBL_T	3
#define	TRBTBL_T	4
#define	AASTBL_T	5
#define	SYSFLG_T	6
#define	DIOTBL_T	7
#define	WDTTBL_T	8
#define	TIMVEC_T	9
#define	LOGTBL_T	10
#define	PERLOG_T	11
#define	SCNPRI_T	12
#define	SCNTBL_T	13
#define	LFNTBL_T	14
#define	PRTQUE_T	15
#define	CNTQUE_T	16
#define	DAQQUE_T	17
#define	SCNFMT_T	18
#define	CHNTBL_T	19
#define	RTUTBL_T	20
#define	HATFST_T	21
#define	HATSLW_T	22
#define	HATEXT_T	23
#define	ACRNYM_T	24
#define	ALMDIR_T	25
#define	ALMFIL_T	26
#define	ALGBLK_T	27
#define	HATEXC_T	28
#define	SECLVL_T	29
#define	SOEKYS_T	30
#define	EVTKYS_T	31
#define	ATTTBL_T	32
#define	CATTBL_T	33
#define	DSNTBL_T	34
#define	DSRTBL_T	35
#define	ALGUSE_T	36
#define	WNDTBL_T	37
#define	SYSSTS_T	38
#define	SCHEDL_T	39
#define	SCNLST_T	40
#define	MSGDIR_T	41
#define	MSGFIL_T	42
#define	DEVTBL_T	43
#define	TDCLIB_T	44
#define	CMDTBL_T	45
#define	SCRIPT_T	46
#define	CONTBL_T	47
#define	ALMSHV_T	48
#define	ACRON2_T	49
#define	UTBL00_T	50

union GB_tables_t
{
    void*   ptr[TABLES_COUNT];
    struct
    {
        dirtry      *DIRTRY;
        t_CRTTBL    *CRTTBL;
        t_PBTTBL    *PBTTBL;
        gdatbl      *GDATBL;
        trbtbl      *TRBTBL;
        aastbl      *AASTBL;
        sysflg      *SYSFLG;
        diotbl      *DIOTBL;
        wdttbl      *WDTTBL;
        void        *TIVTBL;    //unknown structure
        logtbl      *LOGTBL;
        perlog      *PERLOG;
        t_SCNPRI    *SCNPRI;
        scntbl      *SCNTBL;
        lfntbl      *LFNTBL;
        prtque      *PRTQUE;
        void        *CNTQUE;    //unknown structure
        void        *DAQQUE;    //unknown structure
        scnfmt      *SCNFMT;
        chntbl      *CHNTBL;
        rtutbl      *RTUTBL;
        t_HATSLW    *HATFST;
        t_HATSLW    *HATSLW;
        t_HATSLW    *HATEXT;
        void        *ACRNUM;    //unknown structure
        almdir      *ALMDIR;
        void        *ALMFIL;    //unknown structure
        void        *ALGBLK;    //unknown structure
        t_HATSLW    *HATEXC;
        seclvl      *SECLVL;
        t_evtkys    *SOEKYS;
        t_evtkys    *EVTKYS;
        atttbl      *ATTTBL;
        cattbl      *CATTBL;
        dsntbl      *DSNTBL;
        dsrtbl      *DSRTBL;
        void        *ALGUSE;    //unknown structure
        t2_WNDTBL   *WNDTBL;
        syssts      *SYSSTS;
        schedl      *SCHEDL;
        void        *SCNLST;    //unknown structure
        msgdir      *MSGDIR;
        void        *MSGFIL;    //unknown structure
        devtbl      *DEVTBL;
        void        *TDCLIB;    //unknown structure
        void        *CMDTBL;    //unknown structure
        scripttbl   *SCRIPT;
        t_CONTBL    *CONTBL;
        almshv      *ALMSHV;
        // ALARM!!!
        void        *ACRON2;    //unknown structure
        void        *UTBL00;    //unknown structure
    } data;
};

class GBData
{
public:
    GBData();
    ~GBData();

    bool load();
    bool is_loaded() { return m_db_loaded; }

    void save_to_file();
    bool load_from_file();

    int4 get_max_algblk() { return m_gbs.data.DIRTRY[ALGBLK_T].maxrec; }
private:
    sys90       *m_sys90;
    int         **m_gb_base;
    GB_tables_t m_gbs;
    bool        m_db_loaded;
    uint32_t    m_api_acs;
    RTL_CRITICAL_SECTION    m_crit_sec;
    bool                m_unk001;
    uint32_t            m_entries_count;
    EXPLICIT_ACCESS_A   *m_entries;
    SID                 *m_SID;

    bool load_nt(bool is_server);
    bool is_dbservice_running();
    bool gbattach_nt(bool is_server);
    bool init_API_access();

    void set_secure(HANDLE handle, uint32_t value);

    void* map_file(const char *name, bool is_server, DWORD size);
};

extern const int32_t db_sizes[];
extern GBData *gb_data;

#endif // GBDATA_H
