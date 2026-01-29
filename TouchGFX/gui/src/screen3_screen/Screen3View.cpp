#include <gui/screen3_screen/Screen3View.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

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

    // Reposition selection - centered and enlarged (240x320 screen)
    const int16_t screenW = 240;
    const int16_t boxW = 150;
    const int16_t boxH = 100;
    const int16_t boxX = (screenW - boxW) / 2;
    const int16_t boxY = 50;

    selectedProgramBox.setPosition(boxX, boxY, boxW, boxH);

    // Center icon in box (64x55)
    const int16_t iconW = 64;
    const int16_t iconH = 55;
    flappyBirdIcon.setPosition(boxX + (boxW - iconW) / 2, boxY + 5, iconW, iconH);

    // Center "Flappy Bird" text below icon
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_5GHL));
    textArea1.resizeToCurrentText();
    textArea1.setXY((screenW - textArea1.getWidth()) / 2, boxY + boxH - 20);

    // Hide option 2 (BACK) - keep only "Chơi game" / Play game option.
    textArea2.setVisible(false);
    textArea2.invalidate();
    cvIcon.setVisible(false);
    cvIcon.invalidate();

    // Play button - centered below the selection
    const int16_t btnW = 120;
    const int16_t btnH = 50;
    const int16_t btnX = (screenW - btnW) / 2;
    const int16_t btnY = 210;

    playButton.setPosition(btnX, btnY, btnW, btnH);
    playButton.setColor(touchgfx::Color::getColorFromRGB(34, 139, 34));  // Green
    add(playButton);

    const int16_t playIconW = 26;
    const int16_t playIconH = 26;
    const int16_t iconX = btnX + (btnW - playIconW) / 2;
    const int16_t iconY = btnY + (btnH - playIconH) / 2;

    playIcon.setPosition(iconX, iconY, playIconW, playIconH);
    playIcon.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    add(playIcon);
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

    // Play button bounds - tap here to start game
    const int16_t bx = playButton.getX();
    const int16_t by = playButton.getY();
    const int16_t bw = playButton.getWidth();
    const int16_t bh = playButton.getHeight();

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
