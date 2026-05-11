use bevy::prelude::*;
use bevy_panorbit_camera::{PanOrbitCamera, PanOrbitCameraPlugin};
use spice;

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_plugins(PanOrbitCameraPlugin)
        .add_systems(Startup, setup_scene)
        .add_systems(Update, animate_rover)
        .run();
}

#[derive(Component)]
struct Rover {
    path: Vec<Vec3>,
    current_target: usize,
}

fn generate_spice_path() -> Vec<Vec3> {
    let mut path = Vec::new();

    spice::furnsh("kernels/naif0012.tls"); 
    spice::furnsh("kernels/de440.bsp");    
    spice::furnsh("kernels/pck00010.tpc"); 
    spice::furnsh("kernels/msl_surf_rover_loc_0000_2003_v1.bsp"); 

    let start_time_str = "2012-08-06 06:00:00 UTC";
    let et_start = spice::str2et(start_time_str);

    let steps = 200;
    let step_size_seconds = 86400.0 * 10.0; // Odstęp: 10 dni
    let scale_factor = 3.0 / 3396.2; // Skala Marsa

    for i in 0..steps {
        let et = et_start + (i as f64) * step_size_seconds;

        let (pos, _lt) = spice::spkpos(
            "-76",       
            et, 
            "IAU_MARS",  
            "NONE", 
            "MARS"      
        );

        let vec = Vec3::new(
            (pos[0] * scale_factor) as f32,
            (pos[2] * scale_factor) as f32, 
            -(pos[1] * scale_factor) as f32 
        );

        path.push(vec);
    }

    spice::kclear();
    path
}

fn setup_scene(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    commands.spawn((
        Camera3dBundle {
            transform: Transform::from_xyz(0.0, 3.5, 4.0).looking_at(Vec3::new(0.0, 3.0, 0.0), Vec3::Y),
            ..default()
        },
        PanOrbitCamera::default(),
    ));

    commands.spawn(DirectionalLightBundle {
        directional_light: DirectionalLight {
            illuminance: 12_000.0,
            shadows_enabled: true,
            ..default()
        },
        transform: Transform::from_xyz(5.0, 10.0, 2.0).looking_at(Vec3::ZERO, Vec3::Y),
        ..default()
    });

    commands.spawn(PbrBundle {
        mesh: meshes.add(Sphere::new(3.0)),
        material: materials.add(Color::srgb(0.8, 0.4, 0.2)),
        ..default()
    });

    let path = generate_spice_path();

    if !path.is_empty() {
        commands.spawn((
            SceneBundle {
                scene: asset_server.load("rover.glb#Scene0"),
                transform: Transform::from_translation(path[0]).with_scale(Vec3::splat(0.015)),
                ..default()
            },
            Rover { path, current_target: 1 },
        ));
    }
}

fn animate_rover(
    time: Res<Time>, 
    mut query: Query<(&mut Transform, &mut Rover)>, 
    mut gizmos: Gizmos
) {
    for (mut transform, mut rover) in &mut query {
        if rover.path.is_empty() { continue; }

        let target = rover.path[rover.current_target];
        
        let speed = 0.01;
        
        transform.translation = transform.translation.move_towards(target, speed * time.delta_seconds());
        
        // Zabezpieczenie obrotu przed wektorami o zerowej długości
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
            gizmos.line(rover.path[i], rover.path[i+1], Color::srgb(1.0, 1.0, 0.0));
        }
        gizmos.line(rover.path[last_idx], transform.translation, Color::srgb(1.0, 1.0, 0.0));
    }
}