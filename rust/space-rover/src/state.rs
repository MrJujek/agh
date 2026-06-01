use bevy::prelude::*;

use crate::mission::MissionPreset;

#[derive(States, Debug, Clone, Copy, Default, Eq, PartialEq, Hash)]
pub enum AppState {
    #[default]
    Menu,
    Loading,
    Mission,
}

#[derive(Resource, Clone, Copy)]
pub struct SelectedMission {
    pub preset: MissionPreset,
}

impl Default for SelectedMission {
    fn default() -> Self {
        Self {
            preset: MissionPreset::CuriosityLanding,
        }
    }
}

#[derive(Resource)]
pub struct LoadedMission {
    pub preset: MissionPreset,
    pub path: Vec<Vec3>,
    pub rover_scene: Handle<Scene>,
    pub mars_texture: Handle<Image>,
}