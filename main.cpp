#include <iostream>
#include <raylib.h>
#include <vector>

// Constantes pour le jeu
const int FRAME_SPEED = 10; // Vitesse d'animation des sprites
const float PLAYER_SPEED = 1.3f;

struct Player {
    float x, y, w, h, v;
    Texture2D texture;
    Rectangle hitbox, sprite;
    int frameCounter;
};

// Création d'un joueur
void createPlayer(Player& player, float x, float y, const char* file_name) {
    player.x = x;
    player.y = y;
    player.w = 41;
    player.h = 69;
    player.v = PLAYER_SPEED;
    player.texture = LoadTexture(file_name);
    player.hitbox = { x + 4.0f, y + 64.0f, 33.0f, 7.0f };
    player.sprite = { 0.0f, 0.0f, player.w, player.h };
    player.frameCounter = 0;
}

// Mise à jour de l'animation du sprite
void updateSpriteAnimation(Player& player) {
    player.frameCounter++;
    int frame = (player.frameCounter / FRAME_SPEED) % 4;
    player.sprite.x = frame * player.w;
    if (player.frameCounter >= 40) player.frameCounter = 0;
}

void moveSCP(Player& scp_049, Player& guard1, Player& guard2) {
    if (GetTime() < 20.0) {
        scp_049.hitbox.x += scp_049.v;
        scp_049.x += scp_049.v;
        scp_049.sprite.y = 0;
    } else {
        scp_049.hitbox.x -= scp_049.v;
        scp_049.x -= scp_049.v;
        scp_049.sprite.y = scp_049.h;
        guard1.hitbox.x -= guard1.v;
        guard1.x -= guard1.v;
        guard1.sprite.y = guard1.h;
        guard2.hitbox.x -= guard2.v;
        guard2.x -= guard2.v;
        guard2.sprite.y = guard2.h;
    }
    updateSpriteAnimation(scp_049);
    updateSpriteAnimation(guard1);
    updateSpriteAnimation(guard2);
}

void handleMovement(Player& player,
                    std::vector<Rectangle>& collisions) {

    bool isMoving = false;

    // Variables pour détecter les collisions
    bool ableToMove = true;

    // Gestion du mouvement horizontal
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.hitbox.x += player.v;
        for (const auto& rect : collisions) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMove = false;
                break;
            }
        }
        if (ableToMove) {
            player.x += player.v;
            player.sprite.y = 0;
            isMoving = true;
        } else {
            player.hitbox.x -= player.v;
        }
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.hitbox.x -= player.v;
        for (const auto& rect : collisions) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMove = false;
                break;
            }
        }
        if (ableToMove) {
            player.x -= player.v;
            player.sprite.y = player.h;
            isMoving = true;
        } else {
            player.hitbox.x += player.v;
        }
    }

    // Gestion du mouvement vertical
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.hitbox.y -= player.v;
        for (const auto& rect : collisions) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMove = false;
                break;
            }
        }
        if (ableToMove) {
            player.y -= player.v;
            player.sprite.y = player.h * 2;
            isMoving = true;
        } else {
            player.hitbox.y += player.v;
        }
    } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.hitbox.y += player.v;
        for (const auto& rect : collisions) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMove = false;
                break;
            }
        }
        if (ableToMove) {
            player.y += player.v;
            player.sprite.y = player.h * 3;
            isMoving = true;
        } else {
            player.hitbox.y -= player.v;
        }
    }

    // Animation du sprite
    if (isMoving) {
        updateSpriteAnimation(player);
    } else {
        player.frameCounter = 0; // Réinitialise l'animation si immobile
        updateSpriteAnimation(player);
    }
}

void trackMouse(std::vector<Rectangle> buttons, bool& shouldClose) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
        if (CheckCollisionPointRec(GetMousePosition(), buttons[0])) {
            std::cout << buttons[0].y << std::endl;
        } else if (CheckCollisionPointRec(GetMousePosition(), buttons[1])) {
            std::cout << buttons[1].y << std::endl;
        } else if (CheckCollisionPointRec(GetMousePosition(), buttons[2])) {
            shouldClose = true;
        }
    }
}


// Fonction principale
int main() {
    InitWindow(500, 500, "SCP : Secure Contain Protect");
    const int SCREEN_WIDTH = GetScreenWidth();
    const int SCREEN_HEIGHT = GetScreenHeight();
    // Initialisation du joueur

    InitAudioDevice();
    Music main_music = LoadMusicStream("musics/main.mp3");
    PlayMusicStream(main_music);

    Player player;
    createPlayer(player, 250.0f, 150.0f, "./assets/SCP_GuardNoHelmet_Walking.png");

    Player scp_049;
    createPlayer(scp_049, -500.0f, 75.0f, "./assets/SCP_049.png");
    Player guard1;
    createPlayer(guard1, 1500.0f, 65.0f, "./assets/SCP_Guard_Walking.png");
    Player guard2;
    createPlayer(guard2, 1490.0f, 85.0f, "./assets/SCP_Guard_Walking.png");

    std::vector<Rectangle> collisions;
    Texture2D fond = LoadTexture("assets/SCP_Menu_Background.png");
    Rectangle top = {0.0f, 178.0f, 300.0f, 17.0f};
    Rectangle bottom = {0.0f, 231.0f, 300.0f, 17.0f};
    Rectangle left = {-17.0f, 178.0f, 17.0f, 54.0f};
    Rectangle right = {300.0f, 178.0f, 17.0f, 54.0f};
    collisions.push_back(top);
    collisions.push_back(bottom);
    collisions.push_back(left);
    collisions.push_back(right);

    std::vector<Rectangle> buttons;
    Texture2D buttons_texture = LoadTexture("assets/SCP_Menu_Buttons.png");
    Rectangle play = {13.0f, 134.0f, 84.0f, 23.0f};
    Rectangle option = {13.0f, 167.0f, 84.0f, 23.0f};
    Rectangle quit = {13.0f, 200.0f, 84.0f, 23.0f};
    buttons.push_back(play);
    buttons.push_back(option);
    buttons.push_back(quit);

    SetTargetFPS(60);

    bool shouldClose = false;
    while (!shouldClose && !WindowShouldClose()) {
        UpdateMusicStream(main_music);
        // Gestion des mouvements
        trackMouse(buttons, shouldClose);
        handleMovement(player, collisions);
        moveSCP(scp_049, guard1, guard2);
        
        // Dessin
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextureRec(fond, {0.0f, 0.0f, 300.0f, 173.0f}, {0.0f, 0.0f}, WHITE);
        DrawTextureRec(scp_049.texture, scp_049.sprite, {scp_049.x, scp_049.y}, WHITE);
        DrawTextureRec(guard1.texture, guard1.sprite, {guard1.x, guard1.y}, WHITE);
        DrawTextureRec(guard2.texture, guard2.sprite, {guard2.x, guard2.y}, WHITE);
        DrawTextureRec(fond, {0.0f, 174.0f, 300.0f, 133.0f}, {0.0f, 100.0f}, WHITE);        
        DrawTextureRec(player.texture, player.sprite, {player.x, player.y}, WHITE);
        DrawTexture(buttons_texture, 13.0f, 134.0f, WHITE);
        EndDrawing();
    }

    // Libération des ressources
    UnloadTexture(fond);
    UnloadTexture(buttons_texture);
    UnloadTexture(player.texture);
    UnloadTexture(guard1.texture);
    UnloadTexture(guard2.texture);
    UnloadTexture(scp_049.texture);

    UnloadMusicStream(main_music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
