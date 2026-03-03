use std::char;
use std::io;

const BOARD_SIZE: usize = 3;
const WIN_CONDITION: usize = 3;

fn main() {
    let mut user_input = String::new();
    let mut cmd: char;
    let mut current_player: char = 'X';
    println!("Type your command:");

    let board: [[char; BOARD_SIZE]; BOARD_SIZE] = fill_board();

    let mut game_over = check_for_win(board, 1, 1);
    while !game_over {
        println!(
            "Player {}, enter your move (1-9) or 'q' to quit:",
            current_player
        );
        let _ = io::stdin().read_line(&mut user_input);
        cmd = user_input.chars().nth(0).unwrap();
        if cmd == 'q' {
            println!("Quitting the game...");
            break;
        }
        let pos = cmd.to_digit(10).unwrap() as usize - 1;

        draw_board(board);
    }
}

fn fill_board() -> [[char; BOARD_SIZE]; BOARD_SIZE] {
    return [[' '; BOARD_SIZE]; BOARD_SIZE];
}

fn check_for_win(board: [[char; BOARD_SIZE]; BOARD_SIZE], move_x: usize, move_y: usize) -> bool {
    return false;
}

fn draw_board(board: [[char; BOARD_SIZE]; BOARD_SIZE]) {
    print!(" ");
    for i in 0..BOARD_SIZE {
        print!(" {} ", i + 1);
        print!(" ");
    }
    println!();
    for i in 0..BOARD_SIZE {
        for j in 0..BOARD_SIZE {
            print!("+---");
        }
        print!("+");
        println!();
        for j in 0..BOARD_SIZE {
            print!("| {} ", board[i][j]);
        }
        print!("| {}", i + 1);
        println!();
        if i == BOARD_SIZE - 1 {
            for j in 0..BOARD_SIZE {
                print!("+---");
            }
            print!("+");
            println!();
        }
    }
}
