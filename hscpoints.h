#ifndef HSCPOINTS_H
#define HSCPOINTS_H

#include <inttypes.h>
#include <string>
#include <deque>

#define POINTS_FIRST_IDX 2

class HSCPoint;

class HSCPoints
{
public:
    HSCPoints();

    static bool init(bool from_file = false);
    static bool check_point_index(int32_t point);
    static int32_t get_count() { return m_inst->m_points_indexes[POINTS_FIRST_IDX]; }
    static int32_t get_idx(int32_t point) { return m_inst->m_points_indexes[POINTS_FIRST_IDX + point]; }
    static void set_idx(int32_t point, int32_t val) { m_inst->m_points_indexes[POINTS_FIRST_IDX + point] = val; }

    static int scan_all_points();

    static HSCPoint* get_point(int32_t number);
    static HSCPoint* get_point(std::string name);

    static int get_points_count() { return m_inst->m_points.size(); }
    static HSCPoint* get_point_by_idx(int32_t idx) { if (idx < m_inst->m_points.size()) return m_inst->m_points[idx]; else return nullptr; }
private:
    char                    *m_addr;
    int32_t                 *m_points_indexes;
    std::deque<HSCPoint*>   m_points;

    static HSCPoints *m_inst;
};

#endif // HSCPOINTS_H
