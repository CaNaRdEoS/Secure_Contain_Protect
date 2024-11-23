#include <iostream>
#include <raylib.h>

// Structures pour les éléments du jeu
struct CorridorHorizontal {
    int x, y;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxSouth;
};

struct DoorHorizontal {
    int x, y;
    Texture2D texture;
    Rectangle hitboxNorth, hitboxDoor, hitboxSouth;
    bool open;
};

struct Player {
    int x, y, w, h, v;
    Texture2D texture;
    Rectangle hitbox;
};

// Création d'un joueur
void createPlayer(Player& player, float x, float y, const char* file_name) {
    player.x = x;
    player.y = y;
    player.w = 41;
    player.h = 69;
    player.v = 3;
    player.texture = LoadTexture(file_name);
    player.hitbox = { x + 4.0f, y + 64.0f, 33.0f, 7.0f }; // Bas de la texture pour hitbox
}

// Création d'un corridor
void createCorridor(CorridorHorizontal& corridor, float x, float y) {
    corridor.x = x;
    corridor.y = y;
    corridor.texture = LoadTexture("./assets/SCP_Corridor.png");
    corridor.hitboxNorth = { x, y + 104.0f, 500.0f, 10.0f };
    corridor.hitboxSouth = { x, y + 172.0f, 500.0f, 10.0f };
}

// Création d'une porte
void createDoor(DoorHorizontal& door, float x, float y) {
    door.x = x;
    door.y = y;
    door.texture = LoadTexture("./assets/SCP_Door.png");
    door.hitboxNorth = { x + 17.0f, y + 95.0f, 17.0f, 14.0f };
    door.hitboxSouth = { x + 17.0f, y + 146.0f, 17.0f, 14.0f };
    door.hitboxDoor = { x + 17.0f, y + 109.0f, 17.0f, 37.0f };
    door.open = false;
}

// Mise à jour de l'animation du sprite
void updateSpriteAnimation(Player& player, Rectangle& sprite, int& frameCounter) {
    frameCounter++;
    int frame = (frameCounter / 10) % 4; // Cycle entre 0 et 3
    sprite.x = frame * player.w; // Change la colonne
    if (frameCounter >= 40) frameCounter = 0; // Réinitialise après un cycle
}

// Gestion du mouvement du joueur
void handleMovement(Player& player, Rectangle& sprite, int& frameCounter) {
    bool isMoving = false;

    // Déplacement à droite
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player.x += player.v;
        player.hitbox.x += player.v;
        sprite.y = 0; // Ligne pour "droite"
        isMoving = true;
    }
    // Déplacement à gauche
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player.x -= player.v;
        player.hitbox.x -= player.v;
        sprite.y = player.h; // Ligne pour "gauche"
        isMoving = true;
    }
    // Déplacement vers le haut
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        player.y -= player.v;
        player.hitbox.y -= player.v;
        sprite.y = player.h * 2; // Ligne pour "haut"
        isMoving = true;
    }
    // Déplacement vers le bas
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        player.y += player.v;
        player.hitbox.y += player.v;
        sprite.y = player.h * 3; // Ligne pour "bas"
        isMoving = true;
    }

    // Animation du sprite
    if (isMoving) {
        updateSpriteAnimation(player, sprite, frameCounter);
    } else {
        frameCounter = 0; // Réinitialise l'animation si immobile
    }
}

// Fonction principale
int main() {
    InitWindow(500, 200, "SCP : Secure Contain Protect");

    // Initialisation du joueur
    Player guard;
    createPlayer(guard, 150.0f, 100.0f, "./assets/SCP_Guard_Walking.png");
    Rectangle guardSprite = { 0.0f, 0.0f, static_cast<float>(guard.w), static_cast<float>(guard.h) };

    // Initialisation du corridor et de la porte
    CorridorHorizontal corridor1;
    createCorridor(corridor1, 0.0f, 10.0f);
    DoorHorizontal door1;
    createDoor(door1, 225.0f, 26.0f);

    int frameCounter = 0;
    SetTargetFPS(60);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Gestion des mouvements
        handleMovement(guard, guardSprite, frameCounter);

        // Dessin
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(corridor1.texture, corridor1.x, corridor1.y, WHITE);
        DrawTexture(door1.texture, door1.x, door1.y, WHITE);
        DrawTextureRec(guard.texture, guardSprite, { static_cast<float>(guard.x), static_cast<float>(guard.y) }, WHITE);

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
    UnloadTexture(door1.texture);
    CloseWindow();

    return 0;
}
