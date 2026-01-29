#include <gui/screen3_screen/Screen3View.hpp>

extern "C" {
    extern uint8_t screenNumber;
}

Screen3View::Screen3View()
{

}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();

    screenNumber = 3;

    // Hide "My Resume" label on the menu screen.
    textArea2.setVisible(false);
    textArea2.invalidate();
}

void Screen3View::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    if (evt.getType() != touchgfx::ClickEvent::RELEASED &&
        evt.getType() != touchgfx::ClickEvent::PRESSED) {
        return;
    }

    const int16_t x = evt.getX();
    const int16_t y = evt.getY();
    const int16_t w = 240;
    const int16_t h = 320;

    // Start game when tapping the Flappy Bird icon area.
    const int16_t bx = selectedProgramBox.getX();
    const int16_t by = selectedProgramBox.getY();
    const int16_t bw = selectedProgramBox.getWidth();
    const int16_t bh = selectedProgramBox.getHeight();

    auto inBox = [&](int16_t px, int16_t py) {
        return px >= bx && px < (bx + bw) && py >= by && py < (by + bh);
    };

    // Accept common touch rotations/mirroring to avoid calibration issues.
    if (inBox(x, y) ||
        inBox(y, x) ||
        inBox(w - 1 - x, y) ||
        inBox(x, h - 1 - y) ||
        inBox(w - 1 - y, x) ||
        inBox(y, h - 1 - x)) {
        application().gotoFlappyScreenScreenNoTransition();
    }
}

void Screen3View::handleTickEvent()
{
    // Override base behavior to prevent auto-transition to Screen4.
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}
