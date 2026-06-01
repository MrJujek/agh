use bevy::prelude::*;
use bevy_panorbit_camera::PanOrbitCamera;

use crate::state::LoadedMission;

#[derive(Component)]
pub struct MissionEntity;

#[derive(Component)]
pub struct Rover {
    pub path: Vec<Vec3>,
    pub current_target: usize,
}

pub fn spawn_mission_scene(
    mut commands: Commands,
    loaded_mission: Res<LoadedMission>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    let _preset = loaded_mission.preset;
    let start_position = loaded_mission.path.first().copied().unwrap_or(Vec3::ZERO);

    commands.spawn((
        Camera3dBundle {
            transform: Transform::from_xyz(0.0, 3.5, 4.0)
                .looking_at(Vec3::new(0.0, 3.0, 0.0), Vec3::Y),
            ..default()
        },
        PanOrbitCamera::default(),
        MissionEntity,
    ));

    commands.spawn((
        DirectionalLightBundle {
            directional_light: DirectionalLight {
                illuminance: 12_000.0,
                shadows_enabled: true,
                ..default()
            },
            transform: Transform::from_xyz(-5.0, -10.0, 2.0).looking_at(Vec3::ZERO, Vec3::Y),
            ..default()
        },
        MissionEntity,
    ));

    let planet_material = materials.add(StandardMaterial {
        base_color_texture: Some(loaded_mission.mars_texture.clone()),
        perceptual_roughness: 1.0,
        metallic: 0.0,
        ..default()
    });

    commands.spawn((
        PbrBundle {
            mesh: meshes.add(Sphere::new(3.0)),
            material: planet_material.clone(),
            ..default()
        },
        MissionEntity,
    ));

    commands.spawn((
        SceneBundle {
            scene: loaded_mission.rover_scene.clone(),
            transform: Transform::from_translation(start_position).with_scale(Vec3::splat(0.015)),
            ..default()
        },
        Rover {
            path: loaded_mission.path.clone(),
            current_target: 1,
        },
        MissionEntity,
    ));
}

pub fn animate_rover(
    time: Res<Time>,
    mut query: Query<(&mut Transform, &mut Rover)>,
    mut gizmos: Gizmos,
) {
    for (mut transform, mut rover) in &mut query {
        if rover.path.is_empty() {
            continue;
        }

        let target = rover.path[rover.current_target];
        let speed = 0.01;

        transform.translation = transform
            .translation
            .move_towards(target, speed * time.delta_seconds());

        if transform.translation.distance(target) > 0.0001 {
            let up_vector = transform.translation.normalize();
            transform.look_at(target, up_vector);
        }

        if transform.translation.distance(target) < 0.0001 {
            rover.current_target = (rover.current_target + 1) % rover.path.len();
        }

        let last_idx = if rover.current_target == 0 {
            rover.path.len() - 1
        } else {
            rover.current_target - 1
        };

        for i in 0..last_idx {
            gizmos.line(rover.path[i], rover.path[i + 1], Color::srgb(1.0, 1.0, 0.0));
        }
        gizmos.line(
            rover.path[last_idx],
            transform.translation,
            Color::srgb(1.0, 1.0, 0.0),
        );
    }
}

pub fn cleanup_mission_scene(
    mut commands: Commands,
    entities: Query<Entity, With<MissionEntity>>,
) {
    for entity in &entities {
        commands.entity(entity).despawn_recursive();
    }
}
