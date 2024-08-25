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

class Game {
private:
    Camera2D camera;
    Player player;
    Platform groundPlatform;
    std::array<Platform, 20> platforms;
    float randomizePlatformTarget;
    float lastPlatformEnd;

    void readInput() {
        player.readInput();
    }

    void update() {
        player.update();

        if (player.getRect().y + player.getRect().height > camera.target.y + GetScreenHeight()) {
            //LOGV("Player Died");
            float ranDist = (float)GetRandomValue(MIN_PLATFORM_DISTANCE, MAX_PLATFORM_DISTANCE);
            float ranEnd = (float) GetRandomValue((MIN_PLATFORM_DISTANCE + PLATFORM_HEIGHT) * platforms.size(), (MAX_PLATFORM_DISTANCE + PLATFORM_HEIGHT) * platforms.size());
            ranPlacePlatformsWithinRange(GetScreenHeight() - START_GROUND - ranDist, - ranEnd);

            camera.target.y = 0.f;

            player.setPosition(
            {
                (PLAY_SIZE_WIDTH - PLAYER_SIZE) / 2.f,
                ((float)GetScreenHeight()/camera.zoom) - START_HEIGHT - PLAYER_SIZE
                }
            );
            player.resetHitBoxes();
            player.setGravityVel(0.f);
        }

        bool hitGround = player.checkDynamicHitBoxRectCollision(groundPlatform.getRect());

        if (hitGround) {
            player.setPosition({player.getRect().x, groundPlatform.getRect().y - player.getRect().height});
            player.setGravityVel(JUMP_STRENGTH);
        }

        if (player.getGravityVel() > 0) {
            bool hitPlatform {false};
            for (auto& platform : platforms) {
                hitPlatform = player.checkDynamicHitBoxRectCollision(platform.getRect());
                if (hitPlatform) {
                    player.setPosition({player.getRect().x, platform.getRect().y - player.getRect().height});
                    player.setGravityVel(JUMP_STRENGTH);
                }
            }
        }

        if (player.getRect().y < ((GetScreenHeight()/3.f)/camera.zoom) + camera.target.y)
            camera.target.y = player.getRect().y - ((GetScreenHeight()/3.f)/camera.zoom);
    }

    void render() const {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        player.draw();
        groundPlatform.draw();

        for (auto& platform : platforms) {
            platform.draw();
        }

        EndMode2D();
        EndDrawing();
    }

    void ranPlacePlatformsWithinRange(float start, float end) {
        float lastDistanceOffset {0};
        for (size_t i {0}; i < platforms.size(); i++) {
            float posX = (float)GetRandomValue(0, PLAY_SIZE_WIDTH - PLATFORM_WIDTH);
            if (i == 0) {
                platforms.at(i).setPosition({posX, start - PLATFORM_HEIGHT});
            }
            else if (i == platforms.size() - 1) {
                platforms.at(i).setPosition({posX, end});
            }
            else {
                float ranDist = (float)GetRandomValue(MIN_PLATFORM_DISTANCE, MAX_PLATFORM_DISTANCE);
                float posY = start - ranDist - lastDistanceOffset;
                platforms.at(i).setPosition({posX, posY});
                lastDistanceOffset += ranDist;
            }
            lastDistanceOffset += PLATFORM_HEIGHT;
        }
        lastPlatformEnd = end;
    }
public:
    Game() {
        InitWindow(0,0,"Test game");
        const int width = GetScreenWidth();
        const int height = GetScreenHeight();
        SetWindowSize(width,height);
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        SetTargetFPS(60);

        camera.target = {0, 0.f};
        camera.offset = {0.f, 0.f};
        camera.rotation = 0.f;
        camera.zoom = (float)width/(float)PLAY_SIZE_WIDTH;

        player = {
            (PLAY_SIZE_WIDTH - PLAYER_SIZE)/2.f,
            ((float)height/camera.zoom) - START_HEIGHT - PLAYER_SIZE,
            PLAYER_SIZE,
            PLAYER_SIZE,
            BLUE
        };
        groundPlatform = {
            -100.f,
            ((float)height/camera.zoom) - START_GROUND,
            PLAY_SIZE_WIDTH + 200.f,
            START_GROUND + 20.f,
            RED
        };

        for (size_t i {0}; i < 20; i++) {
            platforms.at(i) = {
                0,
                0,
                PLATFORM_WIDTH,
                PLATFORM_HEIGHT,
                PURPLE
            };
            platforms.at(i).setColor(RED);
        }
        float ranDist = (float)GetRandomValue(MIN_PLATFORM_DISTANCE, MAX_PLATFORM_DISTANCE);
        float ranEnd = (float) GetRandomValue((MIN_PLATFORM_DISTANCE + PLATFORM_HEIGHT) * platforms.size(), (MAX_PLATFORM_DISTANCE + PLATFORM_HEIGHT) * platforms.size());
        ranPlacePlatformsWithinRange(height - START_GROUND - ranDist, - ranEnd);
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
