//
// Created by meisj on 19/08/2024.
//

#ifndef RAYLIBANDROID_GAME_HPP
#define RAYLIBANDROID_GAME_HPP

#include<android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "tag", __VA_ARGS__)

#include "config.hpp"
#include "player.hpp"
#include "platform.hpp"

#include <android/log.h>
#include <array>
#include <sstream>
#include <memory>

class Game {
private:
    Camera2D camera;
    Player player;
    Platform groundPlatform;
    std::shared_ptr<std::array<Platform, 20>> platformsA;
    std::shared_ptr<std::array<Platform, 20>> platformsB;
    float lastPlatformEnd;
    int highestPoint;
    int lastHighPoint;

    void readInput() {
        player.readInput();
    }

    void update() {
        player.update();

        if (player.getRect().y > camera.target.y + (GetScreenHeight()/camera.zoom)) {
            resetGame();
        }

        checkPlatformCollisionsAndBounce();

        if (player.getRect().y < ((GetScreenHeight()/3.f)/camera.zoom) + camera.target.y)
            camera.target.y = player.getRect().y - ((GetScreenHeight()/3.f)/camera.zoom);

        if (platformsA->back().getRect().y > camera.target.y + (GetScreenHeight()/camera.zoom))
            ranPlacePlatforms(platformsA);
        if (platformsB->back().getRect().y > camera.target.y + (GetScreenHeight()/camera.zoom))
            ranPlacePlatforms(platformsB);

        if ((int)std::fabs((GetScreenHeight() - START_GROUND) - (player.getRect().y + player.getRect().height)) > highestPoint)
            highestPoint = ((int)std::fabs((GetScreenHeight() - START_GROUND) - (player.getRect().y + player.getRect().height)));
    }

    void render() const {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        DrawLineEx({-100, (float)lastHighPoint}, {PLAY_SIZE_WIDTH + 100, (float)lastHighPoint}, 10, GRAY);

        player.draw();
        groundPlatform.draw();

        for (auto& platform : *platformsA) {
            platform.draw();
        }
        for (auto& platform : *platformsB) {
            platform.draw();
        }

        EndMode2D();

        std::stringstream ss;
        ss << "Highest point: " << (highestPoint/100);
        DrawText(ss.str().c_str(), 0, 0, 50, BLACK);

        EndDrawing();
    }

    void ranPlacePlatforms(std::shared_ptr<std::array<Platform, 20>> platforms) {
        float lastY {lastPlatformEnd};
        for (auto it {platforms->begin()}; it != platforms->end(); it++) {
            float posX = (float) GetRandomValue(0, PLAY_SIZE_WIDTH - PLATFORM_WIDTH);

            float posY = lastY - ((float) GetRandomValue(MIN_PLATFORM_DISTANCE, MAX_PLATFORM_DISTANCE));
            it->setPosition({posX, posY});

            lastY = posY;

            it->setMode(PlatformMode::NORMAL);

            float ranChance = ((float) GetRandomValue(0, 100)/100.f);
            if (ranChance < 0.05f) {
                it->setMode(PlatformMode::SUPER_JUMP);
                it->setColor(RED);
            }
            else {
                it->setMode(PlatformMode::NORMAL);
                it->setColor(PURPLE);
            };
        }
        lastPlatformEnd = lastY;
        //LOGV("%f", lastPlatformEnd);
    }

    void resetGame() {
        //LOGV("Player Died");
        lastPlatformEnd = (GetScreenHeight()/camera.zoom) - START_GROUND;
        ranPlacePlatforms(platformsA);
        ranPlacePlatforms(platformsB);

        camera.target.y = 0.f;


        lastHighPoint = -highestPoint + GetScreenHeight() - START_GROUND;
        player.setPosition(
                {
                        (PLAY_SIZE_WIDTH - PLAYER_SIZE) / 2.f,
                        ((float)GetScreenHeight()/camera.zoom) - START_HEIGHT - PLAYER_SIZE
                }
        );
        player.resetHitBoxes();
        player.setGravityVel(0.f);
    }

    void checkPlatformCollisionsAndBounce() {
        bool hitGround = player.checkDynamicHitBoxRectCollision(groundPlatform.getRect());

        if (hitGround) {
            player.setPosition({player.getRect().x, groundPlatform.getRect().y - player.getRect().height});
            player.setGravityVel(JUMP_STRENGTH);
        }

        if (player.getGravityVel() > 0) {
            bool hitPlatform {false};
            for (auto& platform : *platformsA) {
                hitPlatform = player.checkDynamicHitBoxRectCollision(platform.getRect());
                if (hitPlatform) {
                    player.setPosition({player.getRect().x, platform.getRect().y - player.getRect().height});
                    PlatformMode mode = platform.getMode();
                    if (mode == SUPER_JUMP)
                        player.setGravityVel(SUPER_JUMP_STRENGTH);
                    else
                        player.setGravityVel(JUMP_STRENGTH);
                    break;
                }
            }
            for (auto& platform : *platformsB) {
                if (hitPlatform) break;
                hitPlatform = player.checkDynamicHitBoxRectCollision(platform.getRect());
                if (hitPlatform) {
                    player.setPosition({player.getRect().x, platform.getRect().y - player.getRect().height});
                    PlatformMode mode = platform.getMode();
                    if (mode == SUPER_JUMP)
                        player.setGravityVel(SUPER_JUMP_STRENGTH);
                    else
                        player.setGravityVel(JUMP_STRENGTH);
                    break;
                }
            }
        }
    }

    void initWindow() {
        InitWindow(0,0,"Test game");
        const int width = GetScreenWidth();
        const int height = GetScreenHeight();
        SetWindowSize(width,height);
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        SetTargetFPS(60);
    }

    void initCamera() {
        camera.target = {0, 0.f};
        camera.offset = {0.f, 0.f};
        camera.rotation = 0.f;
        camera.zoom = (float)GetScreenWidth()/(float)PLAY_SIZE_WIDTH;
    }

    void initPlayer() {
        player = {
                (PLAY_SIZE_WIDTH - PLAYER_SIZE)/2.f,
                ((float)GetScreenHeight()/camera.zoom) - START_HEIGHT - PLAYER_SIZE,
                PLAYER_SIZE,
                PLAYER_SIZE,
                BLUE
        };
    }

    void initPlatforms() {
        groundPlatform = {
                -100.f,
                ((float)GetScreenHeight()/camera.zoom) - START_GROUND,
                PLAY_SIZE_WIDTH + 200.f,
                START_GROUND + 20.f,
                RED
        };

        platformsA = std::make_shared<std::array<Platform, 20>>();
        platformsB = std::make_shared<std::array<Platform, 20>>();
        for (size_t i {0}; i < platformsA->size(); i++) {
            platformsA->at(i) = {
                    0,
                    0,
                    PLATFORM_WIDTH,
                    PLATFORM_HEIGHT,
                    PURPLE
            };
        }
        for (size_t i {0}; i < platformsB->size(); i++) {
            platformsB->at(i) = {
                    0,
                    0,
                    PLATFORM_WIDTH,
                    PLATFORM_HEIGHT,
                    PURPLE
            };
        }
        lastPlatformEnd = (GetScreenHeight()/camera.zoom) - START_GROUND;
        ranPlacePlatforms(platformsA);
        ranPlacePlatforms(platformsB);
    }

    void initHeightScore() {
        highestPoint = (int)std::fabs((GetScreenHeight() - START_GROUND) - (player.getRect().y + player.getRect().height));
        lastHighPoint = GetScreenHeight()*2;
    }
public:
    Game() {
        initWindow();
        initCamera();
        initPlayer();
        initPlatforms();
        initHeightScore();
    }

    ~Game() = default;

    void run() {
        while (!WindowShouldClose()) {
            readInput();
            update();
            render();
        }
        CloseWindow();
    }
};

#endif //RAYLIBANDROID_GAME_HPP
