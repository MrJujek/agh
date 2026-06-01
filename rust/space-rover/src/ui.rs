use bevy::prelude::*;
use bevy_egui::{egui, EguiContexts};

use crate::loading::LoadingMission;
use crate::mission::MissionPreset;
use crate::state::{AppState, SelectedMission};

pub fn mission_menu(
    mut contexts: EguiContexts,
    mut selected_mission: ResMut<SelectedMission>,
    mut next_state: ResMut<NextState<AppState>>,
) {
    let Some(ctx) = contexts.try_ctx_mut() else {
        return;
    };

    egui::Area::new("mission_menu".into())
        .anchor(egui::Align2::CENTER_CENTER, [0.0, 0.0])
        .show(ctx, |ui| {
            egui::Frame::dark_canvas(ui.style())
                .rounding(egui::Rounding::same(18.0))
                .inner_margin(egui::Margin::symmetric(28.0, 24.0))
                .show(ui, |ui| {
                    ui.set_min_width(420.0);
                    ui.heading("Space Rover");
                    ui.add_space(8.0);
                    ui.label("Choose a mission.");
                    ui.add_space(18.0);

                    for preset in MissionPreset::ALL {
                        let is_selected = selected_mission.preset == preset;
                        let fill = if is_selected {
                            egui::Color32::from_rgb(210, 120, 60)
                        } else {
                            egui::Color32::from_rgb(45, 45, 48)
                        };

                        if ui
                            .add_sized(
                                [340.0, 56.0],
                                egui::Button::new(preset.title()).fill(fill),
                            )
                            .clicked()
                        {
                            selected_mission.preset = preset;
                        }

                        ui.add_space(6.0);
                        ui.label(preset.description());
                        ui.add_space(12.0);
                    }

                    if ui
                        .add_sized(
                            [340.0, 44.0],
                            egui::Button::new("Start mission").fill(egui::Color32::from_rgb(180, 72, 44)),
                        )
                        .clicked()
                    {
                        next_state.set(AppState::Loading);
                    }
                });
        });
}

pub fn loading_screen(mut contexts: EguiContexts, loading: Option<Res<LoadingMission>>) {
    let Some(loading) = loading else {
        return;
    };

    let Some(ctx) = contexts.try_ctx_mut() else {
        return;
    };

    egui::Area::new("loading_screen".into())
        .anchor(egui::Align2::CENTER_CENTER, [0.0, 0.0])
        .show(ctx, |ui| {
            egui::Frame::dark_canvas(ui.style())
                .rounding(egui::Rounding::same(18.0))
                .inner_margin(egui::Margin::symmetric(28.0, 24.0))
                .show(ui, |ui| {
                    ui.set_min_width(380.0);
                    ui.heading("Loading mission...");
                    ui.add_space(10.0);
                    ui.label(loading.preset.title());
                    ui.add_space(8.0);
                    ui.label("Loading SPICE kernels, textures and the rover model...");
                    ui.add_space(16.0);
                    ui.add(egui::Spinner::new());
                });
        });
}

pub fn mission_hud(
    mut contexts: EguiContexts,
    mut next_state: ResMut<NextState<AppState>>,
) {
    let Some(ctx) = contexts.try_ctx_mut() else {
        return;
    };

    egui::Area::new("mission_hud".into())
        .anchor(egui::Align2::LEFT_TOP, [16.0, 16.0])
        .show(ctx, |ui| {
            egui::Frame::dark_canvas(ui.style())
                .rounding(egui::Rounding::same(12.0))
                .inner_margin(egui::Margin::symmetric(14.0, 12.0))
                .show(ui, |ui| {
                    ui.horizontal(|ui| {
                        if ui.button("Menu").clicked() {
                            next_state.set(AppState::Menu);
                        }

                        ui.label("Drag to move, scroll to zoom.");
                    });
                });
        });
}