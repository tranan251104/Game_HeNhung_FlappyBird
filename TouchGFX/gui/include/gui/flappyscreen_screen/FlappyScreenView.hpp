#ifndef FLAPPYSCREENVIEW_HPP
#define FLAPPYSCREENVIEW_HPP

#include <gui_generated/flappyscreen_screen/FlappyScreenViewBase.hpp>
#include <gui/flappyscreen_screen/FlappyScreenPresenter.hpp>

class FlappyScreenView : public FlappyScreenViewBase
{
public:
    FlappyScreenView();
    virtual ~FlappyScreenView() {}
    virtual void setupScreen();
    virtual void handleKeyEvent(uint8_t key);
    virtual void handleTickEvent();
    virtual void tearDownScreen();
protected:
    void updateScoreText();
    void endGame();

    touchgfx::ScalableImage* wallUp[4];
    touchgfx::ScalableImage* wallDown[4];
    int16_t pipeX[4];
    bool pipePassed[4];

    int32_t birdY_fp;
    int32_t birdVel_fp;
    bool gameRunning;
    uint16_t gameScore;
};

#endif // FLAPPYSCREENVIEW_HPP
