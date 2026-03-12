use std::char;
use std::io;
use std::io::Write;

const BOARD_SIZE: usize = 3;

fn main() {
    let mut user_input = String::new();
    let mut cmd: char;
    let mut current_player: char = 'X';
    println!("Type your command:");

    let mut board: [[char; BOARD_SIZE]; BOARD_SIZE] = fill_board();

    draw_board(board);

    loop {
        println!(
            "Player {}, enter your move (1-9) or 'q' to quit:",
            current_player
        );
        print!("> ");
        io::stdout().flush().unwrap();

        user_input.clear();
        let _ = io::stdin().read_line(&mut user_input);
        cmd = user_input.chars().nth(0).unwrap();

        if cmd == 'q' {
            println!("Quitting the game...");
            break;
        }

        let pos = cmd.to_digit(10).unwrap() as usize - 1;

        let row = pos / BOARD_SIZE;
        let col = pos % BOARD_SIZE;

        if board[row][col] == 'X' || board[row][col] == 'O' {
            println!("That position is already taken. Chose another one.");
            continue;
        }

        board[row][col] = current_player;
        draw_board(board);

        if check_for_win(board, current_player) {
            println!("Player {} wins!", current_player);
            break;
        }
        
        if check_for_draw(board) {
            println!("It's a draw!");
            break;
        }

        current_player = if current_player == 'X' { 'O' } else { 'X' };
    }
}

fn fill_board() -> [[char; BOARD_SIZE]; BOARD_SIZE] {
    let mut board = [[' '; BOARD_SIZE]; BOARD_SIZE];
    let mut count = 1;

    for i in 0..BOARD_SIZE {
        for j in 0..BOARD_SIZE {
            board[i][j] = std::char::from_digit(count, 10).unwrap();
            count += 1;
        }
    }

    board
}

fn check_for_win(board: [[char; BOARD_SIZE]; BOARD_SIZE], player: char) -> bool {
    for i in 0..BOARD_SIZE {
        if board[i][0] == player && board[i][1] == player && board[i][2] == player {
            return true;
        } 
    }

    for j in 0..BOARD_SIZE {
        if board[0][j] == player && board[1][j] == player && board[2][j] == player {
            return true;
        } 
    }

    if board[0][0] == player && board[1][1] == player && board[2][2] == player {
        return true;
    }

    if board[0][2] == player && board[1][1] == player && board[2][0] == player {
        return true;
    }
    
    return false;
}

fn check_for_draw(board: [[char; BOARD_SIZE]; BOARD_SIZE]) -> bool {
    for i in 0..BOARD_SIZE {
        for j in 0..BOARD_SIZE {
            if board[i][j] != 'X' || board[i][j] != 'O' {
                return false;
            }
        }
    }
    return true;
}

fn draw_board(board: [[char; BOARD_SIZE]; BOARD_SIZE]) {
    println!();
    for i in 0..BOARD_SIZE {
        for _ in 0..BOARD_SIZE {
            print!("+---");
        }
        println!("+");
        for j in 0..BOARD_SIZE {
            print!("| {} ", board[i][j]);
        }
        println!("|");
        if i == BOARD_SIZE - 1 {
            for _ in 0..BOARD_SIZE {
                print!("+---");
            }
            println!("+");
        }
    }
}

// fn draw_board_with_axis(board: [[char; BOARD_SIZE]; BOARD_SIZE]) {
//     print!(" ");
//     for i in 0..BOARD_SIZE {
//         print!(" {} ", i + 1);
//         print!(" ");
//     }
//     println!();
//     for i in 0..BOARD_SIZE {
//         for _ in 0..BOARD_SIZE {
//             print!("+---");
//         }
//         println!("+");
//         for j in 0..BOARD_SIZE {
//             print!("| {} ", board[i][j]);
//         }
//         println!("| {}", i + 1);
//         if i == BOARD_SIZE - 1 {
//             for _ in 0..BOARD_SIZE {
//                 print!("+---");
//             }
//             println!("+");
//         }
//     }
// }
