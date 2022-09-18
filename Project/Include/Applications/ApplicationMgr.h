#pragma once    
#include "Hardware.h"
#include "thread.hpp"
#include "MenuService.h"
#include "AIService.h"
#include "InputScanService.h"
#include "AudioAnalyzer.h"
#include "AudioPlayer.h"

namespace Applications
{

class ApplicationMgr
{
public:
    ApplicationMgr();

    static inline ApplicationMgr *Instance()
    {
        if (_applications == nullptr)
        {
            _applications = new ApplicationMgr();
        }
        return _applications;
    }

    MenuService& GetMenu() {return *_menu;}
    AIService& GetAI() {return *_ai;}
    InputScanService& GetInputScan() {return *_inputScan;}
    AudioAnalyzer& GetAudioAnalyzer() {return *_audioAnalyzer;}
    AudioPlayer& GetAudioPlayer() {return *_audioPlayer;}

private:
    static ApplicationMgr *_applications;
    MenuService *_menu;
    AIService *_ai;
    InputScanService *_inputScan;
    AudioAnalyzer *_audioAnalyzer;
    AudioPlayer *_audioPlayer;
    
    // Allocate all the tasks
    void initialize();

private:
    /// @brief	Hide Copy constructor.
    ApplicationMgr(const ApplicationMgr &) = delete;

    /// @brief	Hide Assignment operator.
    ApplicationMgr &operator=(const ApplicationMgr &) = delete;

    /// @brief	Hide Move constructor.
    ApplicationMgr(ApplicationMgr &&) = delete;

    /// @brief	Hide Move assignment operator.
    ApplicationMgr &operator=(ApplicationMgr &&) = delete;
};

} // namespace Applications
