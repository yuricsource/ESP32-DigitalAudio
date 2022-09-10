#include "ConfigurationMgr.h"
#include "ApplicationMgr.h"

namespace Configuration
{

ConfigurationMgr *ConfigurationMgr::_configurations;

ConfigurationMgr::ConfigurationMgr()
{
    if (_configurations == nullptr)
    {
        _configurations = this;
    }
    else
    {
        printf("Error!\n");
    }
}

ConfigurationMgr::~ConfigurationMgr()
{
}

void ConfigurationMgr::UseDefaultConfiguration()
{
    _boardConfiguration.DefaultConfiguration();
    // Applications::ApplicationMgr::Instance()->GetWifi().ConfigurationUpdated();
}

void ConfigurationMgr::DefaulAllConfigurations()
{
    _boardConfiguration.DefaultConfiguration();
}

} // namespace Configuration