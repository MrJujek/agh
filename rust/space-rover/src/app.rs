use bevy::prelude::*;
use bevy_egui::{EguiPlugin, EguiSet};
use bevy_panorbit_camera::PanOrbitCameraPlugin;

use crate::loading::{begin_loading, poll_loading};
use crate::scene::{animate_rover, cleanup_mission_scene, spawn_mission_scene};
use crate::state::{AppState, SelectedMission};
use crate::ui::{loading_screen, mission_hud, mission_menu};

pub fn run() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_plugins(EguiPlugin)
        .add_plugins(PanOrbitCameraPlugin)
        .init_state::<AppState>()
        .insert_resource(SelectedMission::default())
        .add_systems(Update, mission_menu.run_if(in_state(AppState::Menu)).after(EguiSet::InitContexts))
        .add_systems(OnEnter(AppState::Loading), begin_loading)
        .add_systems(Update, loading_screen.run_if(in_state(AppState::Loading)).after(EguiSet::InitContexts))
        .add_systems(Update, poll_loading.run_if(in_state(AppState::Loading)))
        .add_systems(OnEnter(AppState::Mission), spawn_mission_scene)
        .add_systems(OnExit(AppState::Mission), cleanup_mission_scene)
        .add_systems(Update, mission_hud.run_if(in_state(AppState::Mission)).after(EguiSet::InitContexts))
        .add_systems(Update, animate_rover.run_if(in_state(AppState::Mission)))
        .run();
}