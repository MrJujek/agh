fn main() {
    let mut board = [
        [5, 3, 0, 0, 7, 0, 0, 0, 0],
        [6, 0, 0, 1, 9, 5, 0, 0, 0],
        [0, 9, 8, 0, 0, 0, 0, 6, 0],
        [8, 0, 0, 0, 6, 0, 0, 0, 3],
        [4, 0, 0, 8, 0, 3, 0, 0, 1],
        [7, 0, 0, 0, 2, 0, 0, 0, 6],
        [0, 6, 0, 0, 0, 0, 2, 8, 0],
        [0, 0, 0, 4, 1, 9, 0, 0, 5],
        [0, 0, 0, 0, 8, 0, 0, 7, 9]
    ];

    println!("{}", check_sudoku_board(board));

    board = [
        [5, 3, 0, 0, 7, 0, 0, 5, 0],
        [6, 0, 0, 1, 9, 5, 0, 0, 0],
        [0, 9, 8, 0, 0, 0, 0, 6, 0],
        [8, 0, 0, 0, 6, 0, 0, 0, 3],
        [4, 0, 0, 8, 0, 3, 0, 0, 1],
        [7, 0, 0, 0, 2, 0, 0, 0, 6],
        [0, 6, 0, 0, 0, 0, 2, 8, 0],
        [0, 0, 0, 4, 1, 9, 0, 0, 5],
        [0, 0, 0, 0, 8, 0, 0, 7, 9]
    ];

    println!("{}", check_sudoku_board(board));
}

fn check_sudoku_board(board: [[u8; 9]; 9]) -> bool {
    let mut rows = [[false; 10]; 9];
    let mut cols = [[false; 10]; 9];
    let mut squares = [[false; 10]; 9];

    for row in 0..9 {
        for col in 0..9 {
            let value = board[row][col];

            if value == 0 {
                continue;
            }

            if value < 0 || value > 9 {
                return false;
            }

            let u_value = value as usize;
            let square_index = (row / 3) * 3 + (col / 3);

            if rows[row][u_value] || cols[row][u_value] || squares[square_index][u_value] {
                return false;
            }

            rows[row][u_value] = true;
            cols[row][u_value] = true;
            squares[square_index][u_value] = true;
        }
    }

    return true;
}