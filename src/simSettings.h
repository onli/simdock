#ifndef _SIMSETTINGS_H
#define _SIMSETTINGS_H

struct simSettings {
    int LEFT_BORDER;
    int RIGHT_BORDER;
    int BOTTOM_BORDER;  // space from bottom to the icon list
    int ICONW;  
    int ICONH;  
    int PERCENT;  // 100 Maximum increase
    int RANGE;  // 180 // Where it has no effect
    int SPACER;   // Pixel space between 2 starters
    int BG_HEIGHT;
    int REFLEX_SCALING;
    int REFLEX_ALPHA;
    int MAXSIZE;    // ICONW + ICONW * PERCENT / 100;
    wxString BG_PATH;
    bool SHOW_REFLEXES;
    bool ENABLE_TASKS;
    bool ENABLE_MINIMIZE;
    bool AUTO_POSITION;
};

#endif