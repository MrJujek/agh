use bevy::prelude::*;
use spice;

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum MissionPreset {
    CuriosityLanding,
    CuriosityTraverse,
    // CinematicOrbit,
}

impl MissionPreset {
    pub const ALL: [MissionPreset; 2] = [
        MissionPreset::CuriosityLanding,
        MissionPreset::CuriosityTraverse,
        // MissionPreset::CinematicOrbit,
    ];

    pub fn title(self) -> &'static str {
        match self {
            MissionPreset::CuriosityLanding => "Curiosity: landing approach",
            MissionPreset::CuriosityTraverse => "Curiosity: traverse path",
            // MissionPreset::CinematicOrbit => "Cinematic orbit",
        }
    }

    pub fn description(self) -> &'static str {
        match self {
            MissionPreset::CuriosityLanding => {
                "Start blisko lądowania i trasa w pierwszych dniach misji."
            }
            MissionPreset::CuriosityTraverse => {
                "Dłuższy fragment marsjańskiej tras"
            }
            // MissionPreset::CinematicOrbit => {
            //     "Szerszy, bardziej filmowy przebieg wokół Marsa i łazika."
            // }
        }
    }

    pub fn start_time(self) -> &'static str {
        match self {
            MissionPreset::CuriosityLanding => "2012-08-06 06:00:00 UTC",
            MissionPreset::CuriosityTraverse => "2013-02-01 06:00:00 UTC",
            // MissionPreset::CinematicOrbit => "2012-09-15 06:00:00 UTC",
        }
    }

    pub fn step_size_seconds(self) -> f64 {
        match self {
            MissionPreset::CuriosityLanding => 86400.0 * 10.0,
            MissionPreset::CuriosityTraverse => 86400.0 * 7.0,
            // MissionPreset::CinematicOrbit => 86400.0 * 10.0,
        }
    }

    pub fn sample_count(self) -> usize {
        match self {
            MissionPreset::CuriosityLanding => 200,
            MissionPreset::CuriosityTraverse => 200,
            // MissionPreset::CinematicOrbit => 120,
        }
    }
}

pub fn generate_spice_path(preset: MissionPreset) -> Vec<Vec3> {
    let mut path = Vec::new();

    spice::furnsh("kernels/naif0012.tls");
    spice::furnsh("kernels/de440.bsp");
    spice::furnsh("kernels/pck00010.tpc");
    spice::furnsh("kernels/msl_surf_rover_loc_0000_2003_v1.bsp");

    let et_start = spice::str2et(preset.start_time());
    let steps = preset.sample_count();
    let step_size_seconds = preset.step_size_seconds();
    let scale_factor = 3.0 / 3396.2;

    let mut start_pos_raw = None;

    for i in 0..steps {
        let et = et_start + (i as f64) * step_size_seconds;
        let (pos, _lt) = spice::spkpos("-76", et, "IAU_MARS", "NONE", "MARS");

        if start_pos_raw.is_none() {
            start_pos_raw = Some(pos);
        }
        let origin = start_pos_raw.unwrap();

        let fake_multiplier = 150.0; 
        
        let fake_pos_x = origin[0] + (pos[0] - origin[0]) * fake_multiplier;
        let fake_pos_y = origin[1] + (pos[1] - origin[1]) * fake_multiplier;
        let fake_pos_z = origin[2] + (pos[2] - origin[2]) * fake_multiplier;

        let vec = Vec3::new(
            (fake_pos_x * scale_factor) as f32,
            (fake_pos_z * scale_factor) as f32, 
            -(fake_pos_y * scale_factor) as f32 
        ).normalize() * 3.0;

        path.push(vec);
    }

    spice::kclear();
    path
}