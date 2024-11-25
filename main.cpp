#include <iostream>
#include <raylib.h>
#include <vector>

// Structures pour les éléments du jeu
struct CorridorHorizontal {
    float x, y;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxSouth;
};

struct DoorHorizontal {
    float x, y, w, h;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxDoor, hitboxSouth, sprite;
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
    player.v = 3;
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
void createHorizontalDoor(DoorHorizontal& door, float x, float y, std::vector<Rectangle>& collisionHorizontal) {
    door.x = x;
    door.y = y;
    door.w = 51;
    door.h = 159;
    door.texture = LoadTexture("./assets/SCP_Door.png");
    door.hitboxNorth = { x + 17.0f, y + 95.0f, 17.0f, 14.0f };
    door.hitboxSouth = { x + 17.0f, y + 146.0f, 17.0f, 14.0f };
    door.hitboxDoor = { x + 17.0f, y + 109.0f, 17.0f, 37.0f };
    door.sprite = { 0.0f, 0.0f, door.w, door.h };
    door.open = 1;

    collisionHorizontal.push_back(door.hitboxNorth);
    collisionHorizontal.push_back(door.hitboxSouth);
}

void updateSpriteDoor(DoorHorizontal& door) {
    if (door.open) {
        door.sprite.x = door.w;
    } else {
        door.sprite.x = 0;
    }
}

void interractDoor(DoorHorizontal& door) {
    if (door.open) {
        door.open = 0;
    } else {
        door.open = 1;
    }
    updateSpriteDoor(door);
}

// Mise à jour de l'animation du sprite
void updateSpriteAnimation(Player& player, int& frameCounter) {
    frameCounter++;
    int frame = (frameCounter / 10) % 4; // Cycle entre 0 et 3
    player.sprite.x = frame * player.w; // Change la colonne
    if (frameCounter >= 40) frameCounter = 0; // Réinitialise après un cycle
}

// Gestion du mouvement du joueur
void handleMovement(Player& player, int& frameCounter, std::vector<Rectangle>& collisionHorizontal, std::vector<Rectangle>& collisionVertical) {
    bool isMoving = false;
    bool ableToMove = 1;
    int i = 0;

    // Déplacement à droite
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.hitbox.x += player.v;
        while(i < collisionHorizontal.size() && ableToMove) {
            if(CheckCollisionRecs(player.hitbox, collisionHorizontal[i])) {
                ableToMove = 0;
            }
            i++;
        }
        if (ableToMove) {
            player.x += player.v;
            player.sprite.y = 0; // Ligne pour "droite"
            isMoving = true;
        } else {
            player.hitbox.x -= player.v;
        }
        
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
        if (ableToMove) {
            player.x -= player.v;
            player.sprite.y = player.h; // Ligne pour "gauche"
            isMoving = true;
        } else {
            player.hitbox.x += player.v;
        }
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
            player.sprite.y = player.h * 2; // Ligne pour "haut"
            isMoving = true;
        } else {
            player.hitbox.y += player.v;
        }
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
            player.sprite.y = player.h * 3; // Ligne pour "bas"
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
    InitWindow(600, 400, "SCP : Secure Contain Protect");

    // Initialisation du joueur
    Player guard;
    createPlayer(guard, 150.0f, 100.0f, "./assets/SCP_Default_Walking.png");

    //Vectors of collision
    std::vector<Rectangle> collisionVertical;
    std::vector<Rectangle> collisionHorizontal;

    // Initialisation du corridor et de la porte
    CorridorHorizontal corridor1;
    createHorizontalCorridor(corridor1, 0.0f, 10.0f, collisionVertical);
    CorridorHorizontal corridor2;
    createHorizontalCorridor(corridor2, 498.0f, 10.0f, collisionVertical);
    DoorHorizontal door1;
    createHorizontalDoor(door1, 225.0f, 26.0f, collisionHorizontal);

    int frameCounter = 0;
    SetTargetFPS(60);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Gestion des mouvements
        handleMovement(guard, frameCounter, collisionHorizontal, collisionVertical);
        if (IsKeyPressed(KEY_E)) {
            interractDoor(door1);
        }
        

        // Dessin
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(corridor1.texture, corridor1.x, corridor1.y, WHITE);
        DrawTexture(corridor2.texture, corridor2.x, corridor2.y, WHITE);
        DrawTextureRec(door1.texture, door1.sprite, {door1.x, door1.y}, WHITE);
        DrawTextureRec(guard.texture, guard.sprite, { static_cast<float>(guard.x), static_cast<float>(guard.y) }, WHITE);

        // Debug : Affiche les hitboxes
        DrawRectangleLinesEx(corridor1.hitboxNorth, 2.0f, RED);
        DrawRectangleLinesEx(corridor1.hitboxSouth, 2.0f, RED);
        DrawRectangleLinesEx(guard.hitbox, 2.0f, GREEN);
        DrawRectangleLinesEx(door1.hitboxNorth, 2.0f, RED);
        DrawRectangleLinesEx(door1.hitboxDoor, 2.0f, PURPLE);
        DrawRectangleLinesEx(door1.hitboxSouth, 2.0f, RED);

        EndDrawing();
    }

    // Libération des ressources
    UnloadTexture(guard.texture);
    UnloadTexture(corridor1.texture);
    UnloadTexture(corridor2.texture);
    UnloadTexture(door1.texture);
    CloseWindow();

    return 0;
}
