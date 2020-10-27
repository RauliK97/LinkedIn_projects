use crate::game_controller::game_map::GameMap;

mod game_map;

pub struct GameMain
{
    map : game_map::GameMap
}
impl GameMain
{
    pub fn new() -> GameMain
    {
        return Self { map : game_map::GameMap::new() };
    }
    pub fn run(&mut self)
    {
        let mut current_player = 'X';
        self.map.draw_map();
        'main_loop: loop{
            GameMap::print_text("Index: ");
            let mut user_input_string = String::new();
            let _ = std::io::stdin().read_line(&mut user_input_string);
            match user_input_string.trim().parse::<usize>()
            {
                Ok(user_input) => {
                    let user_input_char = user_input as u8 as char;
                    if user_input >= 1 && user_input <= 9
                    {
                        if !self.map.set_mark(user_input, current_player)
                        {
                            GameMap::print_message("This option is not available.\n", 2);
                            continue
                        }
                    }
                    else if user_input_char == 10 as char || user_input_char == 13 as char
                    {}
                    else
                    {
                        GameMap::print_message("Please choose a digit from  to 9!\n", 2);
                        continue
                    }
                }
                _ => {
                    GameMap::print_message("This is not a number!\n", 2);
                    continue;
                }
            }
            user_input_string.clear();
            if self.map.redraw_map(current_player)
            {
                loop {
                    GameMap::print_text("Play again ? (y/n): ");
                    let _ = std::io::stdin().read_line(&mut user_input_string);
                    match user_input_string.bytes().next()
                    {
                        Some(user_input) => {
                            let user_input_char = user_input as char;
                            if user_input_char == 'y' || user_input_char == 'Y'
                            {
                                current_player = 'X';
                                self.map.reset();
                                continue 'main_loop
                            }
                            else if user_input_char == 'n' || user_input_char == 'N'
                            {
                                GameMap::print_message("Goodbye!\n\n", 1);
                                break 'main_loop;
                            } else { GameMap::print_message("Input is incorrect!\n", 2); }
                        }
                        _ => {
                            GameMap::print_message("Something went wrong!\n", 2);
                            //break 'main_loop;
                        }
                    }
                    user_input_string.clear();
                }
            }
            current_player = if current_player == 'X' { 'O' } else { 'X' };
        }
    }
}