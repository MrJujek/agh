use std::sync::{mpsc, Arc, Mutex};
use std::thread;

use bevy::prelude::*;

use crate::mission::{generate_spice_path, MissionPreset};
use crate::state::{AppState, LoadedMission, SelectedMission};

#[derive(Resource)]
pub struct LoadingMission {
    pub preset: MissionPreset,
    pub rover_scene: Handle<Scene>,
    pub mars_texture: Handle<Image>,
    pub path_receiver: Arc<Mutex<mpsc::Receiver<Vec<Vec3>>>>,
    pub path: Option<Vec<Vec3>>,
}

pub fn begin_loading(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    selected_mission: Res<SelectedMission>,
) {
    let preset = selected_mission.preset;
    let rover_scene = asset_server.load("rover.glb#Scene0");
    let mars_texture = asset_server.load("5672_mars_12k_color.jpg");
    let (sender, receiver) = mpsc::channel();

    thread::spawn(move || {
        let path = generate_spice_path(preset);
        let _ = sender.send(path);
    });

    commands.insert_resource(LoadingMission {
        preset,
        rover_scene,
        mars_texture,
        path_receiver: Arc::new(Mutex::new(receiver)),
        path: None,
    });
}

pub fn poll_loading(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    loading: Option<ResMut<LoadingMission>>,
    mut next_state: ResMut<NextState<AppState>>,
) {
    let Some(mut loading) = loading else {
        return;
    };

    if loading.path.is_none() {
        loading.path = loading
            .path_receiver
            .lock()
            .ok()
            .and_then(|receiver| receiver.try_recv().ok());
    }

    let rover_scene_ready = matches!(
        asset_server.get_load_state(&loading.rover_scene),
        Some(bevy::asset::LoadState::Loaded)
    );
    let mars_texture_ready = matches!(
        asset_server.get_load_state(&loading.mars_texture),
        Some(bevy::asset::LoadState::Loaded)
    );

    let Some(path) = loading.path.clone() else {
        return;
    };

    if !rover_scene_ready || !mars_texture_ready {
        return;
    }

    commands.insert_resource(LoadedMission {
        preset: loading.preset,
        path,
        rover_scene: loading.rover_scene.clone(),
        mars_texture: loading.mars_texture.clone(),
    });
    commands.remove_resource::<LoadingMission>();
    next_state.set(AppState::Mission);
}