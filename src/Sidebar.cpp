/*
 *  Panorama -  A simple system monitor for Linux, written using IMGui.
 *  Copyright (C) 2018 Ronen Lapushner
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Sidebar.h"

panorama::Sidebar::Sidebar(float fWidth)
        : m_fWidth{fWidth},
          m_eCurrentlyVisiblePane{PaneType::PANETYPE_CPU} { }
panorama::Sidebar::~Sidebar() { }

void panorama::Sidebar::renderUI() {
    // Determine scaling
    const char *cstrScaling = std::getenv("PANORAMA_SCALING");
    if (cstrScaling != nullptr && std::stof(cstrScaling) > 1.0)
        m_fWidth = ImGui::CalcTextSize("PROCESSES").x + 30;

    const ImVec2 v2ButtonSize = ImVec2(m_fWidth, m_fWidth / 2);
    static bool bIsAboutOpen = true;

    // CPU Pane button
    if (ImGui::Selectable(ICON_FA_MICROCHIP " CPU",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_CPU), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_CPU;

    // Processes Pane button
    if (ImGui::Selectable(ICON_FA_WINDOW_RESTORE " PROCESSES",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_PROCESSES), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_PROCESSES;

    // Resources Pane button
    if (ImGui::Selectable(ICON_FA_MEMORY " MEMORY",
                          (m_eCurrentlyVisiblePane == PaneType::PANETYPE_MEMORY), 0, v2ButtonSize))
        m_eCurrentlyVisiblePane = PaneType::PANETYPE_MEMORY;

    // Separator
    const float fAvailSpace = ImGui::GetContentRegionAvail().y - 3 * ImGui::GetItemsLineHeightWithSpacing();
    ImGui::InvisibleButton("##siderbar_sep", ImVec2(m_fWidth, fAvailSpace));

    // Theme submenu
    if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Light (default)",
                            nullptr,
                            panorama::theme() == panorama::Theme::PANORAMA_THEME_LIGHT)) {
            panorama::setTheme(panorama::Theme::PANORAMA_THEME_LIGHT);
        }

        if (ImGui::MenuItem("Dark",
                            nullptr,
                            panorama::theme() == panorama::Theme::PANORAMA_THEME_DARK)) {
            panorama::setTheme(panorama::Theme::PANORAMA_THEME_DARK);
        }

        if (ImGui::MenuItem("Classic",
                            nullptr,
                            panorama::theme() == panorama::Theme::PANORAMA_THEME_CLASSIC)) {
            panorama::setTheme(panorama::Theme::PANORAMA_THEME_CLASSIC);
        }

        ImGui::EndMenu();
    }

    // Spacing
    ImGui::Spacing();

    // About
    if (ImGui::Selectable("About...")) {
        bIsAboutOpen = true;
        ImGui::OpenPopup("About Panorama...");
    }

    // Exit
    if (ImGui::Selectable("Exit")) {
        SDL_Event event;
        event.type = SDL_QUIT;

        SDL_PushEvent(&event);
    }

    if (ImGui::BeginPopupModal("About Panorama...", &bIsAboutOpen, ImGuiWindowFlags_NoResize)) {
        AboutDialog::renderUI();

        ImGui::EndPopup();
    }
}
