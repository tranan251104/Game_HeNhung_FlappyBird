#ifndef FLAPPYSCREENVIEW_HPP
#define FLAPPYSCREENVIEW_HPP

#include <gui_generated/flappyscreen_screen/FlappyScreenViewBase.hpp>
#include <gui/flappyscreen_screen/FlappyScreenPresenter.hpp>
#include <touchgfx/events/ClickEvent.hpp>

class FlappyScreenView : public FlappyScreenViewBase
{
public:
    FlappyScreenView();
    virtual ~FlappyScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleKeyEvent(uint8_t key);
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleTickEvent();

protected:
    // --- CÁC MẢNG QUẢN LÝ ỐNG ---
    touchgfx::ScalableImage* wallUp[4];
    touchgfx::ScalableImage* wallDown[4];
    int16_t pipeX[4];           // Tọa độ X của từng cặp ống
    int16_t pipeGapTopY[4];     // Tọa độ Y của mép trên khe hở
    bool pipePassed[4];         // Trạng thái đã đi qua ống chưa

    // --- BIẾN VẬT LÝ & GAME ---
    int32_t birdY_fp;           // Vị trí Y (Fixed Point)
    int32_t birdVel_fp;         // Vận tốc Y (Fixed Point)
    
    bool gameRunning;           // Game đang chạy hay dừng
    bool isDying;               // Trạng thái "đang rơi tự do" (sau khi chết)
    uint16_t gameScore;         // Điểm số tạm thời
    uint32_t rngState;          // Biến lưu trạng thái Random

    // --- CÁC HÀM HỖ TRỢ ---
    void updateScoreText();     // Cập nhật điểm lên màn hình
    void endGame();             // Kết thúc game, chuyển màn hình
    void startDyingSequence();  // Bắt đầu hiệu ứng rơi tự do
    
    // Hàm tính vị trí ống và vẽ lại (quan trọng để không bị đứng hình)
    void positionPipe(int index, int16_t gapTop);
    void flap();
    
    // Hàm tính toán độ cao cột mới dựa trên cột cũ (Random thông minh)
    int16_t nextGapTopFrom(int16_t prevGapTop);
};

#endif // FLAPPYSCREENVIEW_HPP
