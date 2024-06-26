/*
 *   AviTab - Aviator's Virtual Tablet
 *   Copyright (C) 2018-2024 Folke Will <folko@solhost.org>
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

#include <algorithm>
#include <sstream>
#include "XApproach.h"
#include "src/Logger.h"

namespace xdata {

XApproach::XApproach(const std::string &id)
:    world::Approach(id), ProcedureOptions(id)
{
}

world::NavNodeList XApproach::getWaypoints(std::shared_ptr<world::Runway> arrivalRwy, std::string appTransName) const
{
    if ((transitions.size() + approach.size()) == 0) {
        logger::warn("Approach %s has no waypoints", getID().c_str());
        return world::NavNodeList();
    }

    logger::info("Approach %s from fix '%s'\n%s",
        getID().c_str(), appTransName.c_str(), toDebugString().c_str());

    world::NavNodeList waypoints;
    if (transitions.empty() || appTransName.empty()) {
        waypoints = approach;
    } else {
        // Populate waypoints from requested transition then the approach waypoints
        try {
            waypoints = transitions.at(appTransName);
        } catch (std::out_of_range &e) {
            logger::warn("Couldn't find '%s' in approach transitions", appTransName.c_str());
        }
        waypoints.insert(waypoints.end(), approach.begin(), approach.end());

        auto newEnd = std::unique(waypoints.begin(), waypoints.end());
        waypoints.erase(newEnd, waypoints.end());
    }

    // Remove all waypoints after any runway as these are missed approach
    for (auto it = waypoints.begin(); it != waypoints.end(); it++) {
        if ((*it)->isRunway()) {
            waypoints.erase(it + 1, waypoints.end());
        }
    }

    std::stringstream ss;
    for (auto w: waypoints) {
        ss << w->getID() << " ";
    }
    logger::info("Approach waypoints: %s", ss.str().c_str());

    return waypoints;
}

std::string XApproach::toDebugString() const
{
    std::stringstream res;

    for (auto &it: transitions) {
        res << "  Transition " << it.first << " connects ";
        for (auto &node: it.second) {
            res << node->getID() << ", ";
        }
        res << "\n";
    }

    res << "  Actual approach: ";
    for (auto &node: approach) {
        res << node->getID() << ", ";
    }
    res << "\n";

    return res.str();
}

void XApproach::addTransition(const std::string& id, const world::NavNodeList& nodes)
{
    transitions.insert(std::make_pair(id, nodes));
}

void XApproach::addApproach(const world::NavNodeList &nodes)
{
    approach = nodes;
}

const std::shared_ptr<world::Fix> XApproach::getStartFix() const
{
    if (approach.empty()) {
        return nullptr;
    }
    return std::dynamic_pointer_cast<world::Fix>(approach.front());
}

const std::shared_ptr<world::Runway> XApproach::getRunway() const
{
    for (auto &node: approach) {
        if (node->isRunway()) {
            return std::dynamic_pointer_cast<world::Runway>(node);
        }
    }
    return nullptr;
}

void XApproach::iterateTransitions(std::function<void(const std::string&, std::shared_ptr<world::Fix>, std::shared_ptr<world::Runway>)> f)
{
    for (auto &it: transitions) {
        if (it.second.empty()) {
            continue;
        }
        auto &startFix = it.second.front();
        if (startFix->isGlobalFix()) {
            f(it.first, std::dynamic_pointer_cast<world::Fix>(startFix), getRunway());
        }
    }
}

} /* namespace xdata */
