#ifndef T1000_RESOURCE_MANAGER_H
#define T1000_RESOURCE_MANAGER_H

#include <Config.h>

#include <string>
#include <vector>

#include <utils/XDataBase.h>

namespace t1000 {
  class ResourceManager {
  public:
    xF::XDataBase* Load(const std::string	& filename);
    void Release();
    std::vector<xF::XDataBase*> m_resources;
  };
}

#endif