#include <gui/flappyscreen_screen/FlappyScreenView.hpp>
#include <touchgfx/Unicode.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern "C" {
    extern uint8_t screenNumber;
    extern uint16_t score;
    extern uint16_t topScore;
}

FlappyScreenView::FlappyScreenView()
{

}

void FlappyScreenView::setupScreen()
{
    FlappyScreenViewBase::setupScreen();

    screenNumber = 1;

    wallUp[0] = &wallUp1;
    wallUp[1] = &wallUp2;
    wallUp[2] = &wallUp3;
    wallUp[3] = &wallUp4;

    wallDown[0] = &wallDown1;
    wallDown[1] = &wallDown2;
    wallDown[2] = &wallDown3;
    wallDown[3] = &wallDown4;

    for (int i = 0; i < 4; ++i) {
        pipeX[i] = wallUp[i]->getX();
        pipePassed[i] = false;
    }

    birdY_fp = bird.getY() * 256;
    birdVel_fp = 0;
    gameRunning = false;
    gameScore = 0;
    updateScoreText();
}

void FlappyScreenView::handleKeyEvent(uint8_t key)
{
    if (key == 0) {
        // Start game on first press and apply jump impulse.
        gameRunning = true;
        birdVel_fp = -800;
    }
}

void FlappyScreenView::handleTickEvent()
{
    if (!gameRunning) {
        return;
    }

    const int32_t gravity_fp = 45;
    const int16_t speed = 2;
    const int16_t screenH = 320;
    const int16_t spacing = 70;

    birdVel_fp += gravity_fp;
    birdY_fp += birdVel_fp;

    int16_t newY = (int16_t)(birdY_fp / 256);
    if (newY < 0 || newY + bird.getHeight() > screenH) {
        endGame();
        return;
    }

    bird.moveTo(bird.getX(), newY);

    int16_t maxX = pipeX[0];
    for (int i = 1; i < 4; ++i) {
        if (pipeX[i] > maxX) {
            maxX = pipeX[i];
        }
    }

    for (int i = 0; i < 4; ++i) {
        pipeX[i] -= speed;
        if (pipeX[i] + wallUp[i]->getWidth() < 0) {
            pipeX[i] = maxX + spacing;
            pipePassed[i] = false;
            maxX = pipeX[i];
        }

        wallUp[i]->moveTo(pipeX[i], wallUp[i]->getY());
        wallDown[i]->moveTo(pipeX[i], wallDown[i]->getY());

        // Score when passing a pipe column
        if (!pipePassed[i] && pipeX[i] + wallUp[i]->getWidth() < bird.getX()) {
            pipePassed[i] = true;
            gameScore++;
            updateScoreText();
        }

        // Collision check
        int16_t bx = bird.getX();
        int16_t by = bird.getY();
        int16_t bw = bird.getWidth();
        int16_t bh = bird.getHeight();

        int16_t ux = wallUp[i]->getX();
        int16_t uy = wallUp[i]->getY();
        int16_t uw = wallUp[i]->getWidth();
        int16_t uh = wallUp[i]->getHeight();

        int16_t dx = wallDown[i]->getX();
        int16_t dy = wallDown[i]->getY();
        int16_t dw = wallDown[i]->getWidth();
        int16_t dh = wallDown[i]->getHeight();

        bool hitUp = (bx < ux + uw) && (bx + bw > ux) && (by < uy + uh) && (by + bh > uy);
        bool hitDown = (bx < dx + dw) && (bx + bw > dx) && (by < dy + dh) && (by + bh > dy);

        if (hitUp || hitDown) {
            endGame();
            return;
        }
    }
}

void FlappyScreenView::tearDownScreen()
{
    FlappyScreenViewBase::tearDownScreen();
}

void FlappyScreenView::updateScoreText()
{
    touchgfx::Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, "%u", (unsigned)gameScore);
    scoreText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_KDY8));
    scoreText.setWildcard(scoreTextBuffer);
    scoreText.resizeToCurrentText();
    scoreText.invalidate();
}

void FlappyScreenView::endGame()
{
    score = gameScore;
    if (gameScore > topScore) {
        topScore = gameScore;
    }
    gameRunning = false;
    application().gotoScreen2ScreenNoTransition();
}
