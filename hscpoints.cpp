#include "hscpoints.h"
#include "hsc_include.h"
#include "hscpoint.h"
#include "log.h"
#include <fstream>

HSCPoints* HSCPoints::m_inst = nullptr;

HSCPoints::HSCPoints()
    : m_points_indexes (nullptr)
{
    m_inst = this;
}

bool HSCPoints::init(bool from_file)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoints::init():\n");

    if (from_file)
    {
        std::ifstream file("point_idxs.dat", std::ios::in | std::ios::binary | std::ios::app);
        int32_t size;
        file.read((char*)&size, sizeof(size));
        m_inst->m_addr = new char[size];
        file.read(m_inst->m_addr, size);
        file.close();
        m_inst->m_points_indexes = reinterpret_cast<int32_t*>(m_inst->m_addr);

        return true;
    }

    m_inst->m_addr = shgetfst(POINTSID);
    Log::message(Log::LOG_VERBOSE, "\t shgetfst(POINTSID)=0x%p\n", static_cast<void*>(m_inst->m_addr));

    if (m_inst->m_addr != nullptr)
    {
        m_inst->m_points_indexes = reinterpret_cast<int32_t*>(m_inst->m_addr);
        Log::message(Log::LOG_VERBOSE, "\t m_points_indexes=0x%p\n", static_cast<void*>(m_inst->m_points_indexes));
    }
    else
    {
        shTransBegin(POINTSID, m_inst->m_addr, m_inst->m_addr);
        Log::message(Log::LOG_VERBOSE, "\t shTransBegin=0x%p\n", static_cast<void*>(m_inst->m_addr));
        int32_t size = 4 * GBdirtry[ALGBLK_F].maxrec + 12;
        Log::message(Log::LOG_VERBOSE, "\t size=%i\n", size);
        m_inst->m_addr = shmalloc(POINTSID, size);
        Log::message(Log::LOG_VERBOSE, "\t shmalloc=0x%p\n", static_cast<void*>(m_inst->m_addr));
        if (m_inst->m_addr == nullptr)
        {
            Log::message(Log::LOG_ERROR, "Unable to malloc point index table!\n");
            shTransAbort(POINTSID, nullptr, nullptr);
            return false;
        }

        m_inst->m_points_indexes = reinterpret_cast<int32_t*>(m_inst->m_addr);
        Log::message(Log::LOG_VERBOSE, "\t pnt_no_ptr=0x%p\n", static_cast<void*>(m_inst->m_points_indexes));
        m_inst->m_points_indexes[0] = 1;
        m_inst->m_points_indexes[1] = 0x10000;
        m_inst->m_points_indexes[2] = (size >> 2) - 3;

        shTransSaveNew(POINTSID, m_inst->m_addr, size);
        shTransEnd(POINTSID, nullptr, nullptr);
    }

    /*int32_t size = SHMSZ(POINTSID, m_inst->m_addr);
    std::ofstream file("point_idxs.dat", std::ios::out | std::ios::binary | std::ios::app);
    file.write((char*)&size, sizeof(size));
    file.write(m_inst->m_addr, size);
    file.close();*/

    return true;
}

bool HSCPoints::check_point_index(int32_t point)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoints::check_point_index(%i):\n", point);

    if (point > m_inst->m_points_indexes[POINTS_FIRST_IDX])
    {
        Log::message(Log::LOG_VERBOSE, "\t point > pnt_no_ptr[0] (%i)\n", m_inst->m_points_indexes[POINTS_FIRST_IDX]);
        int val = GBdirtry[ALGBLK_F].maxrec;
        if (val > m_inst->m_points_indexes[POINTS_FIRST_IDX])
        {
            if (shTransBegin(POINTSID, nullptr, nullptr))
                return false;
            int32_t size = 4 * val + 12;
            m_inst->m_addr = shrealloc(POINTSID, m_inst->m_addr, size);
            if (m_inst->m_addr == nullptr)
            {
                Log::message(Log::LOG_ERROR, "Unable to realloc point index table!\n");
                shTransAbort(POINTSID, nullptr, nullptr);
                return false;
            }

            m_inst->m_points_indexes = reinterpret_cast<int32_t*>(m_inst->m_addr);
            m_inst->m_points_indexes[POINTS_FIRST_IDX] = (size >> 2) - 3;

            shTransSaveNew(POINTSID, m_inst->m_addr, size);
            shTransEnd(POINTSID, nullptr, nullptr);
        }
    }

    return true;
}

int HSCPoints::scan_all_points()
{
    for (int32_t i=1 ; i<1000*1000 ; ++i)
    {
        HSCPoint* pnt = new HSCPoint(i);
        if (pnt != nullptr)
        {
            if (pnt->get_data())
                m_inst->m_points.push_back(pnt);
            else
                delete pnt;
        }
    }
    return get_points_count();
}

HSCPoint* HSCPoints::get_point(int32_t number)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoints::get_point(%i):\n", number);
    for (HSCPoint* pnt : m_inst->m_points)
        if (pnt->get_number() == number)
            return pnt;
    HSCPoint* pnt = new HSCPoint(number);
    if (pnt != nullptr)
        m_inst->m_points.push_back(pnt);
    return pnt;
}

HSCPoint* HSCPoints::get_point(std::string name)
{
    Log::message(Log::LOG_VERBOSE, "HSCPoints::get_point(%s):\n", name.c_str());
    for (HSCPoint* pnt : m_inst->m_points)
        if (pnt->get_name().compare(name) == 0)
            return pnt;
    HSCPoint* pnt = new HSCPoint(name);
    if (pnt != nullptr)
        m_inst->m_points.push_back(pnt);
    return pnt;
}
