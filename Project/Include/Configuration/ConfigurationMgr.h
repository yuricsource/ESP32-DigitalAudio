#pragma once

#include "Crc32xZlib.h"
#include "lwip/ip_addr.h"
#include "ConfigurationCommon.h"
#include "HalCommon.h"
#include "BoardConfiguration.h"

namespace Configuration
{

class ConfigurationMgr
{
public:

    ConfigurationMgr();

    ~ConfigurationMgr();

    static inline ConfigurationMgr *Instance()
    {
        if (_configurations == nullptr)
        {
            _configurations = new ConfigurationMgr();
        }
        return _configurations;
    }

    BoardConfiguration *GetBoardConfiguration() {return &_boardConfiguration;}

    void DefaulAllConfigurations();
    void UseDefaultConfiguration();

private:

static ConfigurationMgr *_configurations;
BoardConfiguration _boardConfiguration = {};

private:
    /// @brief	Hide Copy constructor.
    ConfigurationMgr(const ConfigurationMgr &) = delete;

    /// @brief	Hide Assignment operator.
    ConfigurationMgr &operator=(const ConfigurationMgr &) = delete;

    /// @brief	Hide Move constructor.
    ConfigurationMgr(ConfigurationMgr &&) = delete;

    /// @brief	Hide Move assignment operator.
    ConfigurationMgr &operator=(ConfigurationMgr &&) = delete;
};

} // namespace Configuration