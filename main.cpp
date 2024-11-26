#include <iostream>
#include <raylib.h>
#include <vector>
#include <memory>

// Constantes pour le jeu
const int SCREEN_WIDTH = GetScreenWidth();
const int SCREEN_HEIGHT = GetScreenHeight();
const int FRAME_SPEED = 10; // Vitesse d'animation des sprites
const float PLAYER_SPEED = 2.0f;

// Structures pour les éléments du jeu
struct CorridorHorizontal {
    float x, y;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxSouth;
};

struct DoorHorizontal {
    int id;
    float x, y, w, h;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxDoor, hitboxSouth, hitboxButton, spriteBack, spriteFront;
    bool open;
};

struct Player {
    float x, y, w, h, v;
    Texture2D texture;
    Rectangle hitbox, sprite;
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
}

// Création d'un corridor
void createHorizontalCorridor(CorridorHorizontal& corridor, float x, float y, std::vector<Rectangle>& collisionVertical) {
    corridor.x = x;
    corridor.y = y;
    corridor.texture = LoadTexture("./assets/SCP_Corridor.png");
    corridor.hitboxNorth = { x, y + 104.0f, 500.0f, 10.0f };
    corridor.hitboxSouth = { x, y + 172.0f, 500.0f, 10.0f };

    collisionVertical.push_back(corridor.hitboxNorth);
    collisionVertical.push_back(corridor.hitboxSouth);
}

// Création d'une porte
void createHorizontalDoor(DoorHorizontal& door,int id, float x, float y, std::vector<Rectangle>& collisionHorizontal, std::vector<DoorHorizontal>& collisionDoorHorizontal) {
    door.id = id;
    door.x = x;
    door.y = y;
    door.w = 51;
    door.h = 159;
    door.texture = LoadTexture("./assets/SCP_Door.png");
    door.hitboxNorth = { x + 17.0f, y + 95.0f, 17.0f, 14.0f };
    door.hitboxSouth = { x + 17.0f, y + 146.0f, 17.0f, 14.0f };
    door.hitboxDoor = { x + 17.0f, y + 109.0f, 17.0f, 37.0f };
    door.hitboxButton = { x-10, y + 96.0f, 71.0f, 20.0f };
    door.spriteBack = { 0.0f, 0.0f, door.w, door.h - 96 };
    door.spriteFront = { 0.0f, 63.0f, door.w, 96 };
    door.open = 0;

    collisionHorizontal.push_back(door.hitboxNorth);
    collisionHorizontal.push_back(door.hitboxSouth);
    collisionDoorHorizontal.push_back(door);
}

void updateSpriteDoor(DoorHorizontal& door) {
    door.spriteBack.x = door.open ? door.w : 0;
}

void interractDoor(DoorHorizontal& door) {
    door.open = !door.open;
    if (door.open)
        std::cout << "LOG OPEN" << std::endl;
    if (!door.open)
        std::cout << "LOG CLOSE" << std::endl;
    updateSpriteDoor(door);
}

// Mise à jour de l'animation du sprite
void updateSpriteAnimation(Player& player, int& frameCounter) {
    frameCounter++;
    int frame = (frameCounter / FRAME_SPEED) % 4; // Cycle entre 0 et 3
    player.sprite.x = frame * player.w; // Change la colonne
    if (frameCounter >= 40) frameCounter = 0; // Réinitialise après un cycle
}

void handleMovement(Player& player, int& frameCounter,
                    std::vector<Rectangle>& collisionHorizontal,
                    std::vector<Rectangle>& collisionVertical,
                    std::vector<DoorHorizontal>& collisionDoorHorizontal) {

    bool isMoving = false;

    // Variables pour détecter les collisions
    bool ableToMoveHorizontal = true;
    bool ableToMoveVertical = true;

    // Gestion du mouvement horizontal
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.hitbox.x += player.v;
        for (const auto& rect : collisionHorizontal) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMoveHorizontal = false;
                break;
            }
        }
        for (const auto& door : collisionDoorHorizontal) {
            if (CheckCollisionRecs(player.hitbox, door.hitboxDoor) && !door.open) {
                ableToMoveHorizontal = false;
                break;
            }
        }
        if (ableToMoveHorizontal) {
            player.x += player.v;
            player.sprite.y = 0;
            isMoving = true;
        } else {
            player.hitbox.x -= player.v;
        }
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.hitbox.x -= player.v;
        for (const auto& rect : collisionHorizontal) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMoveHorizontal = false;
                break;
            }
        }
        for (const auto& door : collisionDoorHorizontal) {
            if (CheckCollisionRecs(player.hitbox, door.hitboxDoor) && !door.open) {
                ableToMoveHorizontal = false;
                break;
            }
        }
        if (ableToMoveHorizontal) {
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
        for (const auto& rect : collisionVertical) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMoveVertical = false;
                break;
            }
        }
        if (ableToMoveVertical) {
            player.y -= player.v;
            player.sprite.y = player.h * 2;
            isMoving = true;
        } else {
            player.hitbox.y += player.v;
        }
    } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.hitbox.y += player.v;
        for (const auto& rect : collisionVertical) {
            if (CheckCollisionRecs(player.hitbox, rect)) {
                ableToMoveVertical = false;
                break;
            }
        }
        if (ableToMoveVertical) {
            player.y += player.v;
            player.sprite.y = player.h * 3;
            isMoving = true;
        } else {
            player.hitbox.y -= player.v;
        }
    }

    // Animation du sprite
    if (isMoving) {
        updateSpriteAnimation(player, frameCounter);
    } else {
        frameCounter = 0; // Réinitialise l'animation si immobile
        updateSpriteAnimation(player, frameCounter);
    }
}


// Fonction principale
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SCP : Secure Contain Protect");
    const int SCREEN_WIDTH = GetScreenWidth();
    const int SCREEN_HEIGHT = GetScreenHeight();
    std::cout << "KAKA" << SCREEN_WIDTH << std::endl;
    // Initialisation du joueur
    Player guard;
    createPlayer(guard, 150.0f, 100.0f, "./assets/SCP_049.png");

    //Vectors of collision
    std::vector<Rectangle> collisionVertical;
    std::vector<Rectangle> collisionHorizontal;
    std::vector<DoorHorizontal> collisionDoorHorizontal;


    // Initialisation du corridor et de la porte
    CorridorHorizontal corridor1;
    createHorizontalCorridor(corridor1, 0.0f, 10.0f, collisionVertical);
    CorridorHorizontal corridor2;
    createHorizontalCorridor(corridor2, 498.0f, 10.0f, collisionVertical);
    DoorHorizontal door1;
    createHorizontalDoor(door1, 1, 225.0f, 26.0f, collisionHorizontal, collisionDoorHorizontal);
    createHorizontalDoor(door1, 2, 22.0f, 26.0f, collisionHorizontal, collisionDoorHorizontal);

    std::cout << "KAKA" << SCREEN_WIDTH << " "<< SCREEN_HEIGHT<<std::endl;

    Camera2D camera = { 0 };
    camera.target = { guard.x + guard.w / 2, guard.y + guard.h / 2 }; // Le joueur est au centre
    camera.offset = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };    // Décalage pour centrer
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    int frameCounter = 0;
    SetTargetFPS(60);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Gestion des mouvements
        handleMovement(guard, frameCounter, collisionHorizontal, collisionVertical, collisionDoorHorizontal);
        camera.target = { guard.x + guard.w / 2, guard.y + guard.h / 2 };
        if (IsKeyPressed(KEY_E)) {
            for (DoorHorizontal& door : collisionDoorHorizontal)
                if (CheckCollisionRecs(guard.hitbox, door.hitboxButton))
                    interractDoor(door);
        }
        // Dessin
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawTexture(corridor1.texture, corridor1.x, corridor1.y, WHITE);
        DrawTexture(corridor2.texture, corridor2.x, corridor2.y, WHITE);
        for (DoorHorizontal door : collisionDoorHorizontal) {
            DrawTextureRec(door.texture, door.spriteBack, {door.x, door.y}, WHITE);
        }
        DrawTextureRec(guard.texture, guard.sprite, {guard.x, guard.y}, WHITE);
        for (DoorHorizontal door : collisionDoorHorizontal) {
            DrawTextureRec(door.texture, door.spriteFront, {door.x, door.y+63}, WHITE);
        }

        //Debug : Hitboxs
        //DrawRectangleLinesEx(collisionDoorHorizontal[0].hitboxButton, 2.0f, GREEN);
        EndMode2D();
        EndDrawing();
    }

    // Libération des ressources
    UnloadTexture(guard.texture);
    UnloadTexture(corridor1.texture);
    UnloadTexture(corridor2.texture);
    UnloadTexture(door1.texture);
    for (DoorHorizontal door: collisionDoorHorizontal) {
        UnloadTexture(door.texture);
    }
    CloseWindow();

    return 0;
}
