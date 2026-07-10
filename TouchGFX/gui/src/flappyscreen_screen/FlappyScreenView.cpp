#include <gui/flappyscreen_screen/FlappyScreenView.hpp>
#include <touchgfx/Unicode.hpp>
#include <texts/TextKeysAndLanguages.hpp>

// --- LIÊN KẾT VỚI BIẾN TOÀN CỤC CỦA HỆ THỐNG (MAIN.C) ---
extern "C" {
    extern uint8_t screenNumber;
    extern uint16_t score;
    extern uint16_t topScore;
    // Hàm lưu vào Flash (bạn cần tự viết trong main.c theo hướng dẫn trước)
    // Nếu chưa có thì comment dòng này lại để tránh lỗi biên dịch
    void SaveTopScoreToFlash(uint16_t newScore); 
}

// --- CÁC HÀM TIỆN ÍCH STATIC (Random & Kẹp giá trị) ---
static uint32_t lcgNext(uint32_t state)
{
    // Thuật toán Linear Congruential Generator đơn giản
    return state * 1103515245u + 12345u;
}

static int16_t clamp16(int16_t v, int16_t lo, int16_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

FlappyScreenView::FlappyScreenView()
{
}

void FlappyScreenView::setupScreen()
{
    FlappyScreenViewBase::setupScreen();

    // 1. Khởi tạo biến game
    screenNumber = 1;
    gameScore = 0;
    gameRunning = false; // Đợi chạm màn hình mới chạy
    isDying = false;     // Chưa chết
    
    // Đảm bảo kích thước chim đúng (nếu dùng ScalableImage)
    bird.setWidth(24);
    bird.setHeight(24); 
    updateScoreText();

    // 2. Gán địa chỉ các widget vào mảng để dễ quản lý vòng lặp
    wallUp[0] = &wallUp1;
    wallUp[1] = &wallUp2;
    wallUp[2] = &wallUp3;
    wallUp[3] = &wallUp4;

    wallDown[0] = &wallDown1;
    wallDown[1] = &wallDown2;
    wallDown[2] = &wallDown3;
    wallDown[3] = &wallDown4;

    // 3. Cấu hình vị trí xuất phát các ống
    const int16_t screenH = 320;
    const int16_t gapSize = 90;
    const int16_t minGapTop = 30;
    const int16_t maxGapTop = screenH - gapSize - 30;
    const int16_t spacing = 90;  // Khoảng cách giữa các ống
    const int16_t startX = 300;  // Vị trí xuất phát ngoài màn hình bên phải

    // Khởi tạo hạt giống Random
    rngState = 0xA5A5A5A5u; 

    // Rải đều các ống ra
    for (int i = 0; i < 4; ++i) {
        pipeX[i] = startX + (i * spacing); // SỬA LỖI DÍNH CHÙM: Cộng dồn vị trí
        pipePassed[i] = false;

        if (i == 0) {
            // Ống đầu tiên nằm giữa
            pipeGapTopY[i] = (minGapTop + maxGapTop) / 2;
        } else {
            // Các ống sau random lệch đi một chút so với ống trước
            pipeGapTopY[i] = nextGapTopFrom(pipeGapTopY[i - 1]);
        }
        
        // Vẽ ống lần đầu
        positionPipe(i, pipeGapTopY[i]);
    }

    // 4. Khởi tạo vật lý chim
    birdY_fp = bird.getY() * 256;
    birdVel_fp = 0;
}

void FlappyScreenView::handleKeyEvent(uint8_t key)
{
    (void)key;
}

void FlappyScreenView::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    if (evt.getType() == touchgfx::ClickEvent::PRESSED) {
        flap();
    }
}

void FlappyScreenView::flap()
{
    if (isDying) {
        return;
    }

    if (!gameRunning) {
        gameRunning = true;
    }

    birdVel_fp = -800;
}

void FlappyScreenView::handleTickEvent()
{
    // --- TRƯỜNG HỢP 1: CHIM ĐANG RƠI TỰ DO (CHẾT) ---
    if (isDying) {
        const int32_t gravity_dead = 60; // Rơi nhanh hơn chút
        birdVel_fp += gravity_dead;
        birdY_fp += birdVel_fp;
        
        int16_t newY = (int16_t)(birdY_fp / 256);
        bird.moveTo(bird.getX(), newY); // Chỉ di chuyển chim, ống đứng yên

        // Nếu rơi quá màn hình -> Chuyển màn hình Game Over
        if (newY > 320) {
            endGame();
        }
        return; // Dừng hàm, không xử lý phần dưới
    }

    // --- TRƯỜNG HỢP 2: GAME CHƯA CHẠY ---
    if (!gameRunning) {
        return;
    }

    // --- TRƯỜNG HỢP 3: GAME ĐANG CHẠY ---
    const int32_t gravity_fp = 45;
    const int16_t speed = 2;
    const int16_t screenH = 320;
    const int16_t spacing = 70;

    // 1. Vật lý Chim (Trọng lực)
    birdVel_fp += gravity_fp;
    birdY_fp += birdVel_fp;
    int16_t newY = (int16_t)(birdY_fp / 256);

    // Chạm nóc hoặc đáy màn hình -> Chết
    if (newY < 0 || newY + bird.getHeight() > screenH) {
        startDyingSequence();
        return;
    }
    bird.moveTo(bird.getX(), newY);

    // 2. Tìm ống xa nhất (để nối đuôi)
    int16_t maxX = pipeX[0];
    int16_t maxIndex = 0;
    for (int i = 1; i < 4; ++i) {
        if (pipeX[i] > maxX) {
            maxX = pipeX[i];
            maxIndex = i;
        }
    }

    // 3. Xử lý logic từng ống
    for (int i = 0; i < 4; ++i) {
        // Di chuyển sang trái
        pipeX[i] -= speed;

        // Nếu đi hết màn hình trái -> Tái tạo bên phải
        if (pipeX[i] + wallUp[i]->getWidth() < 0) {
            int prevIndex = maxIndex;
            // Tính chiều cao mới dựa trên ống xa nhất hiện tại
            pipeGapTopY[i] = nextGapTopFrom(pipeGapTopY[prevIndex]);
            
            pipeX[i] = maxX + spacing;
            pipePassed[i] = false;
            
            // Cập nhật mốc xa nhất mới
            maxX = pipeX[i];
            maxIndex = i;
        }

        // Vẽ lại vị trí ống (Có Invalidate)
        positionPipe(i, pipeGapTopY[i]);

        // Tính điểm: Nếu chim vượt qua mép phải của ống
        if (!pipePassed[i] && (pipeX[i] + wallUp[i]->getWidth() < bird.getX())) {
            pipePassed[i] = true;
            gameScore++;
            updateScoreText();
        }

        // Kiểm tra va chạm (AABB Collision)
        int16_t bx = bird.getX() + 4; // Thu nhỏ hitbox chim 1 chút cho dễ thở
        int16_t by = bird.getY() + 4;
        int16_t bw = bird.getWidth() - 8;
        int16_t bh = bird.getHeight() - 8;

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
            startDyingSequence(); // Bắt đầu rơi tự do
            return;
        }
    }
}

// Hàm tính toán độ cao cột tiếp theo (Random delta)
int16_t FlappyScreenView::nextGapTopFrom(int16_t prevGapTop)
{
    const int16_t screenH = 320;
    const int16_t gapSize = 90;
    const int16_t minGapTop = 30;
    const int16_t maxGapTop = screenH - gapSize - 30;
    const int16_t deltaMax = 35; // Độ khó: chênh lệch tối đa giữa 2 cột

    rngState = lcgNext(rngState);
    int16_t delta = (int16_t)((rngState >> 16) % (2 * deltaMax + 1)) - deltaMax;
    return clamp16((int16_t)(prevGapTop + delta), minGapTop, maxGapTop);
}

// Hàm vẽ ống (SỬA LỖI ĐỨNG HÌNH)
void FlappyScreenView::positionPipe(int index, int16_t gapTop)
{
    const int16_t screenH = 320;
    const int16_t gapSize = 90;
    int16_t gapBottom = gapTop + gapSize;

    int16_t upH = gapTop;
    int16_t downH = screenH - gapBottom;

    if (upH < 1) upH = 1;
    if (downH < 1) downH = 1;

    // QUAN TRỌNG: invalidate() TRƯỚC khi di chuyển để xóa hình cũ
    wallUp[index]->invalidate();
    wallDown[index]->invalidate();

    // Cập nhật tọa độ và kích thước
    wallUp[index]->setPosition(pipeX[index], 0, wallUp[index]->getWidth(), upH);
    wallDown[index]->setPosition(pipeX[index], gapBottom, wallDown[index]->getWidth(), downH);

    // QUAN TRỌNG: invalidate() SAU khi di chuyển để vẽ hình mới
    wallUp[index]->invalidate();
    wallDown[index]->invalidate();
}

void FlappyScreenView::startDyingSequence()
{
    isDying = true;       // Bật cờ chết
    birdVel_fp = -500;    // Nảy lên một chút (hiệu ứng va chạm)
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
        
        // Gọi hàm lưu Flash (nếu đã cài đặt trong main.c)
        SaveTopScoreToFlash(topScore); 
    }
    gameRunning = false;
    application().gotoScreen2ScreenNoTransition();
}

void FlappyScreenView::tearDownScreen()
{
    FlappyScreenViewBase::tearDownScreen();
}
