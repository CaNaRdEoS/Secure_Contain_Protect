#include <iostream>
#include <raylib.h>
#include <vector>
#include <memory>

// Constantes pour le jeu
constexpr int SCREEN_WIDTH = 600;
constexpr int SCREEN_HEIGHT = 400;
constexpr int FRAME_SPEED = 10; // Vitesse d'animation des sprites
constexpr float PLAYER_SPEED = 3.0f;

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

// Gestion du mouvement du joueur
void handleMovement(Player& player, int& frameCounter,
                    std::vector<Rectangle>& collisionHorizontal,
                    std::vector<Rectangle>& collisionVertical,
                    std::vector<DoorHorizontal>& collisionDoorHorizontal) {

    bool isMoving = false;
    bool ableToMove = 1;
    int i = 0;
    int pi = 0;

    // Déplacement à droite
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.hitbox.x += player.v;
        while(i < collisionHorizontal.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionHorizontal[i])) {
                ableToMove = 0;
            }
            i++;
        }
        while(pi < collisionDoorHorizontal.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionDoorHorizontal[pi].hitboxDoor) && !collisionDoorHorizontal[pi].open) {
                ableToMove = 0;
            }
            pi++;
        }
        if (ableToMove) {
            player.x += player.v;
            player.sprite.y = 0;
            isMoving = true;
        } else {
            player.hitbox.x -= player.v;
        }
        int i = 0;
        int pi = 0;
    }
    // Déplacement à gauche
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.hitbox.x -= player.v;
        while(i < collisionHorizontal.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionHorizontal[i])) {
                ableToMove = 0;
            }
            i++;
        }
        while(pi < collisionDoorHorizontal.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionDoorHorizontal[pi].hitboxDoor) && !collisionDoorHorizontal[pi].open) {
                ableToMove = 0;
            }
            pi++;
        }
        if (ableToMove) {
            player.x -= player.v;
            player.sprite.y = player.h;
            isMoving = true;
        } else {
            player.hitbox.x += player.v;
        }
        int i = 0;
        int pi = 0;
    }
    // Déplacement vers le haut
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.hitbox.y -= player.v;
        while(i < collisionVertical.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionVertical[i])) {
                ableToMove = 0;
            }
            i++;
        }
        if (ableToMove) {
            player.y -= player.v;
            player.sprite.y = player.h * 2;
            isMoving = true;
        } else {
            player.hitbox.y += player.v;
        }
        int i = 0;
        int pi = 0;
    }
    // Déplacement vers le bas
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.hitbox.y += player.v;
        while(i < collisionVertical.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionVertical[i])) {
                ableToMove = 0;
            }
            i++;
        }
        if (ableToMove) {
            player.y += player.v;
            player.sprite.y = player.h * 3;
            isMoving = true;
        } else {
            player.hitbox.y -= player.v;
        }
        int i = 0;
        int pi = 0;
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
    InitWindow(600, 400, "SCP : Secure Contain Protect");

    // Initialisation du joueur
    Player guard;
    createPlayer(guard, 150.0f, 100.0f, "./assets/SCP_Default_Walking.png");

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

    int frameCounter = 0;
    SetTargetFPS(60);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Gestion des mouvements
        handleMovement(guard, frameCounter, collisionHorizontal, collisionVertical, collisionDoorHorizontal);
        if (IsKeyPressed(KEY_E)) {
            for (DoorHorizontal& door : collisionDoorHorizontal)
                if (CheckCollisionRecs(guard.hitbox, door.hitboxButton))
                    interractDoor(door);
        }
        
        // Dessin
        BeginDrawing();
        ClearBackground(BLACK);
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
