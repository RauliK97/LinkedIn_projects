use std::io::Write;

pub struct GameMap
{
    marks: [[char; 3]; 3],
}

impl GameMap
{
    pub fn new() -> GameMap
    {
        GameMap {
            marks : [['7', '8', '9'],
                ['4', '5', '6'],
                ['1', '2', '3'] ]
        }
    }
    pub fn reset(&mut self)
    {
        self.marks = [['7', '8', '9'],
            ['4', '5', '6'],
            ['1', '2', '3']];
        self.redraw_map(' ');
    }

    pub fn draw_map(&self)
    {
        for row in 0..3
        {
            for col in 0..3
            {
                let temp_mark = self.marks[row][col];
                if temp_mark == 'X' || temp_mark == 'O' || temp_mark == ' ' { print!("\x1B[1m"); }
                else { print!("\x1B[2m"); }
                print!(" {} ", self.marks[row][col]);
                print!("\x1B[0m");
                if col == 2
                {
                    break;
                }
                print!("\u{2502}");
            }
            println!();
            if row == 2
            {
                break;
            }
            for i in 0..3
            {
                for _j in 0..3
                {
                    print!("\u{2500}");
                }
                if i == 2
                {
                    break;
                }
                print!("\u{253C}");
            }
            println!();
        }
        println!();
        println!();


    }
    pub fn redraw_map(&self, current_player: char) -> bool
    {
        print!("\x1B[8A\x1B[J");
        self.draw_map();
        if self.win_check()
        {
            GameMap::print_message(&*format!("Congratulations! {} won!\n", current_player), 1);
            return true;
        }
        else if self.draw_check()
        {
            GameMap::print_message("Draw!\n", 1);
            return true;
        }

        return false;
    }
    pub fn print_message( text: &str, lines_to_delete: u8 )
    {
        print!("\x1B[{}A\x1B[J", lines_to_delete);
        GameMap::print_text(text);
    }
    pub fn print_text( text: &str)
    {
        print!("{}", text);
        let _ = std::io::stdout().flush();
    }
    fn win_check(&self) -> bool
    {
        for i in 0..3
        {
            if self.check_line(i)
            {
                return true;
            }
        }
        if self.check_diag()
        {
            return true;
        }
        return false;
    }
    fn draw_check(&self) -> bool
    {
        for i in 0..3
        {
            for j in 0..3
            {
                if self.marks[i][j] >= '1' && self.marks[i][j] <= '9'
                {
                    return false;
                }
            }
        }
        return true;
    }

    fn check_line(&self, i: usize) -> bool
    {
        let mark = [self.marks[i][0], self.marks[0][i]];
        if (mark[0] == self.marks[i][1] && mark[0] == self.marks[i][2]) ||
            (mark[1] == self.marks[1][i] && mark[1] == self.marks[2][i])
        { return true; }
        return false;
    }
    fn check_diag (&self) -> bool
    {
        let mark = [self.marks[0][0], self.marks[0][2]];
        if (mark[0] == self.marks[1][1] && mark[0] == self.marks[2][2]) ||
            (mark[1] == self.marks[1][1] && mark[1] == self.marks[2][0])
        { return true; }
        return false;
    }

    pub fn set_mark(&mut self, mut index: usize, mark: char) -> bool
    {
        index = index - 1;
        let current_mark = &mut self.marks[2 - index / 3][index % 3];
        if *current_mark < '1' || *current_mark > '9'
        {
            return false;
        }
        *current_mark = mark;
        return true;
    }
}