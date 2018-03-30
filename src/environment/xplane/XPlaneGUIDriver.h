/*
 *   AviTab - Aviator's Virtual Tablet
 *   Copyright (C) 2018 Folke Will <folko@solhost.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef SRC_ENVIRONMENT_XPLANE_XPLANEGUIDRIVER_H_
#define SRC_ENVIRONMENT_XPLANE_XPLANEGUIDRIVER_H_

#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMDisplay.h>
#include "src/environment/GUIDriver.h"
#include "DataRef.h"

namespace avitab {

class XPlaneGUIDriver: public GUIDriver {
public:
    XPlaneGUIDriver();

    void init(int width, int height) override;
    void createWindow(const std::string &title) override;
    void readPointerState(int &x, int &y, bool &pressed) override;
private:
    DataRef<bool> isVrEnabled;
    int textureId = -1;
    XPLMWindowID window = nullptr;
    int mouseX = 0, mouseY = 0;
    bool mousePressed = false;

    void onDraw();
    bool onClick(int x, int y, XPLMMouseStatus status);
    bool onRightClick(int x, int y, XPLMMouseStatus status);
    void onKey(char key, XPLMKeyFlags flags, char virtualKey, bool losingFocus);
    XPLMCursorStatus getCursor(int x, int y);
    bool onMouseWheel(int x, int y, int wheel, int clicks);
};

} /* namespace avitab */

#endif /* SRC_ENVIRONMENT_XPLANE_XPLANEGUIDRIVER_H_ */
