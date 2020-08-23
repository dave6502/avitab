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
#ifndef SRC_MAPS_OVERLAYED_AIRPORT_H_
#define SRC_MAPS_OVERLAYED_AIRPORT_H_

#include "OverlayedNode.h"
#include "src/libxdata/world/models/airport/Airport.h"

namespace maps {

class OverlayedAirport : public OverlayedNode {

public:
    static std::shared_ptr<OverlayedAirport> getInstanceIfVisible(const xdata::Airport *airport);

    OverlayedAirport(const xdata::Airport *airport);

    void drawGraphics();
    void drawText(bool detailed);

private:

    enum class AerodromeType {
        AIRPORT,
        AIRSTRIP,
        SEAPORT,
        HELIPORT
    };

    const xdata::Airport *airport;
    AerodromeType type;
    uint32_t color;
    int px;
    int py;

    static bool isVisible(const xdata::Airport *airport);
    static bool isEnabled(const xdata::Airport *airport);
    static AerodromeType getAerodromeType(const xdata::Airport *airport);
    static uint32_t getAirportColor(const xdata::Airport *airport);

    static void drawAirportBlob(int x, int y, uint32_t color);
    static void drawAirportICAOCircleAndRwyPattern(const xdata::Airport *airport, int x, int y, uint32_t color);
    static void drawAirportICAORing(const xdata::Airport *airport, int x, int y, uint32_t color);
    static void drawAirportICAOGeographicRunways(const xdata::Airport *airport, uint32_t color);
    static void drawAirportGeographicRunways(const xdata::Airport *airport);
    static void getRunwaysCentre(const xdata::Airport *airport, int zoomLevel, int & xCentre, int & yCentre);
    static int getMaxRunwayDistanceFromCentre(const xdata::Airport *airport, int zoomLevel, int xCentre, int yCentre);
    static void drawRunwayRectangles(const xdata::Airport *airport, float size, uint32_t color);

    static const int ICAO_CIRCLE_RADIUS = 15;
    static const int SHOW_DETAILED_INFO_AT_MAPWIDTHNM = 40;
    static const int DRAW_BLOB_RUNWAYS_AT_MAPWIDTHNM = 200;
    static const int MAX_BLOB_SIZE = 12;
    static const int BLOB_SIZE_DIVIDEND = DRAW_BLOB_RUNWAYS_AT_MAPWIDTHNM * MAX_BLOB_SIZE;
    static const int DRAW_GEOGRAPHIC_RUNWAYS_AT_MAPWIDTHNM = 5;
    static const int ICAO_RING_RADIUS = 12;
};

} /* namespace maps */

#endif /* SRC_MAPS_OVERLAYED_AIRPORT_H_ */
