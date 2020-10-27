mod game_controller;
fn main() {
    let mut game = game_controller::GameMain::new();
    game.run();
}