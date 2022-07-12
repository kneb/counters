#include "global.h"
#include <filesystem>
#include <unistd.h>
#include <limits.h>

namespace global{
    std::string currentPath;
    const char *monthName_v[] = {
        "Января", "Февраля", "Марта", "Апреля", "Мая", "Июня",
        "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря"
    };
    const char *monthName_i[] = {
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };

    int getDayMonth(int month, int year){
        // month 0..11
        // 31 28/29 31 30 31 30 31 31 30 31 30 31
        int count = 31;
        if (month == 3 || month == 5 || month == 8 || month == 10){
            count = 30;
        } else if (month == 1){
            ((year%4 == 0 && year%100!=0) || year%400 == 0) ? count = 29 : count = 28;
        }
        return count;
    }

    time_t decMonth(struct tm *date, int count){
    for (int i=0; i<count; i++)
        if (date->tm_mon > 0){
        date->tm_mon --;
        } else {
        date->tm_mon = 11;
        date->tm_year --;
        }
    return mktime(date);
    }

    std::string getCurPath(){
        char buf[PATH_MAX];
        readlink("/proc/self/exe", buf, sizeof(buf)-1);
        std::filesystem::path p(buf);
        return p.parent_path();
    }
}



