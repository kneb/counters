#include "list.h"
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <limits.h>

namespace global{
    std::string currentPath;
}

const char *mon_name[] = {
    "Января", "Февраля", "Март", "Апреля", "Мая", "Июня",
    "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря"
};

std::string getCurPath(){
  char buf[PATH_MAX];
  int i = readlink("/proc/self/exe", buf, sizeof(buf)-1);
  std::filesystem::path p(buf);

  //std::string str(buf);
  return p.parent_path();
}
