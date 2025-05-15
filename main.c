#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"
#include "joueur.h"

// Fixed window dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Structure to hold assigned keys for each action
typedef struct {
    SDLKey moveRightKey;
    SDLKey moveLeftKey;
    SDLKey jumpKey;
    SDLKey crouchKey;
    SDLKey attackKey;
} KeyBindings;

// Function prototypes
void characterSelectionScreen(SDL_Surface *ecran, char *selectedSpriteSheet);
void keybindingScreen(SDL_Surface *ecran, KeyBindings *bindings);
void gameLoop(SDL_Surface *ecran, KeyBindings *bindings, const char *spriteSheetPath);
void handleJump(Perso *p, Uint32 dt);
void handleCrouch(Perso *p);
void handleAttack(Perso *p, Uint32 dt);

int main() {
    SDL_Surface *ecran = NULL;
    Mix_Music *musique = NULL;
    char selectedSpriteSheet[256] = ""; // Store the selected sprite sheet path

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
        return 1;
    }

    // Création de la fenêtre avec des dimensions fixes
    ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        printf("Erreur de création de fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    // State machine: 0 = character selection, 1 = keybinding, 2 = gameplay
    int gameState = 0;
    KeyBindings bindings = { SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN, SDLK_SPACE }; // Default keys

    while (gameState >= 0) {
        if (gameState == 0) {
            // Character selection screen
            characterSelectionScreen(ecran, selectedSpriteSheet);
            if (strlen(selectedSpriteSheet) > 0) {
                gameState = 1; // Proceed to keybinding screen
            } else {
                gameState = -1; // Quit if user exits
            }
        } else if (gameState == 1) {
            // Keybinding screen
            keybindingScreen(ecran, &bindings);
            gameState = 2; // Proceed to gameplay
        } else if (gameState == 2) {
            // Gameplay
            gameLoop(ecran, &bindings, selectedSpriteSheet);
            gameState = -1; // Exit after gameplay
        }
    }

    // Libération des ressources
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    SDL_Quit();
    TTF_Quit();

    return 0;
}

// Character selection screen
void characterSelectionScreen(SDL_Surface *ecran, char *selectedSpriteSheet) {
    SDL_Event event;
    int quitter = 1;

    // Charger l'image de fond (sans redimensionnement)
    Image img = initialiserImage("backround.jpg");
    if (!img.surface) {
        SDL_Quit();
        return;
    }

    // Initialisation des textes
    SDL_Color couleurTexte = {0, 0, 0, 255};
    Texte txt = initialiserTexte("Select", "Maguntia.ttf", 25, couleurTexte, 340, 380);
    Texte txt2 = initialiserTexte("Select", "Maguntia.ttf", 25, couleurTexte, 555, 380);
    Texte txt5 = initialiserTexte("quit", "Maguntia.ttf", 35, couleurTexte, 665, 435);

    // Initialisation des boutons avec sons
    Bouton btn1 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 350);
    Bouton btn2 = initialiserBouton("button22.png", "button333.png", "sb.wav", 500, 350);
    Bouton marco = initialiserBouton("whitepolo.png", "button333.png", "sb.wav", 125, -100);
    Bouton marco2 = initialiserBouton("blackpolo.png", "button333.png", "sb.wav", 525, -100);
    Bouton btn5 = initialiserBouton("button22.png", "button333.png", "sb.wav", 600, 400);

    // Boucle principale du menu
    while (quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    strcpy(selectedSpriteSheet, ""); // Indicate quit
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quitter = 0;
                        strcpy(selectedSpriteSheet, "");
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.x >= btn1.position.x && event.motion.x <= btn1.position.x + btn1.images[0]->w &&
                        event.motion.y >= btn1.position.y && event.motion.y <= btn1.position.y + btn1.images[0]->h) {
                        btn1.etat = 1;
                    } else {
                        btn1.etat = 0;
                    }
                    if (event.motion.x >= btn2.position.x && event.motion.x <= btn2.position.x + btn2.images[0]->w &&
                        event.motion.y >= btn2.position.y && event.motion.y <= btn2.position.y + btn2.images[0]->h) {
                        btn2.etat = 1;
                    } else {
                        btn2.etat = 0;
                    }
                    if (event.motion.x >= btn5.position.x && event.motion.x <= btn5.position.x + btn5.images[0]->w &&
                        event.motion.y >= btn5.position.y && event.motion.y <= btn5.position.y + btn5.images[0]->h) {
                        btn5.etat = 1;
                    } else {
                        btn5.etat = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.x >= btn1.position.x && event.button.x <= btn1.position.x + btn1.images[0]->w &&
                        event.button.y >= btn1.position.y && event.button.y <= btn1.position.y + btn1.images[0]->h) {
                        btn1.etat = 1;
                        Mix_PlayChannel(-1, btn1.son, 0);
                        strcpy(selectedSpriteSheet, "testtth.png");
                        quitter = 0;
                    }
                    if (event.button.x >= btn2.position.x && event.button.x <= btn2.position.x + btn2.images[0]->w &&
                        event.button.y >= btn2.position.y && event.button.y <= btn2.position.y + btn2.images[0]->h) {
                        btn2.etat = 1;
                        Mix_PlayChannel(-1, btn2.son, 0);
                        strcpy(selectedSpriteSheet, "vegeta mvmnt.png");
                        quitter = 0;
                    }
                    if (event.button.x >= btn5.position.x && event.button.x <= btn5.position.x + btn5.images[0]->w &&
                        event.button.y >= btn5.position.y && event.button.y <= btn5.position.y + btn5.images[0]->h) {
                        btn5.etat = 1;
                        Mix_PlayChannel(-1, btn5.son, 0);
                        quitter = 0;
                        strcpy(selectedSpriteSheet, "");
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    btn1.etat = 0;
                    btn2.etat = 0;
                    btn5.etat = 0;
                    break;
            }
        }

        // Afficher l'image de fond et tous les éléments du menu
        afficherImage(ecran, img);
        afficherBouton(ecran, btn1);
        afficherBouton(ecran, btn2);
        afficherBouton(ecran, btn5);
        afficherBouton(ecran, marco);
        afficherBouton(ecran, marco2);
        afficherTexte(ecran, txt);
        afficherTexte(ecran, txt2);
        afficherTexte(ecran, txt5);
        SDL_Flip(ecran);
    }

    // Libération des ressources
    libererImage(&img);
    libererBouton(&btn1);
    libererBouton(&btn2);
    libererBouton(&btn5);
    libererBouton(&marco);
    libererBouton(&marco2);
    libererTexte(&txt);
    libererTexte(&txt2);
    libererTexte(&txt5);
}

// Keybinding screen
void keybindingScreen(SDL_Surface *ecran, KeyBindings *bindings) {
    SDL_Event event;
    int quitter = 1;
    int configuring = 0; // 0 = not configuring, 1 = move right, 2 = move left, etc.

    // Charger l'image de fond (sans redimensionnement)
    Image img = initialiserImage("backround.jpg");
    if (!img.surface) {
        return;
    }

    // Initialisation des textes
    SDL_Color couleurTexte = {0, 0, 0, 255};
    Texte txt = initialiserTexte("move right", "Maguntia.ttf", 25, couleurTexte, 170, 130);
    Texte txt2 = initialiserTexte("move left", "Maguntia.ttf", 25, couleurTexte, 170, 270);
    Texte txt3 = initialiserTexte("jump", "Maguntia.ttf", 25, couleurTexte, 170, 400);
    Texte txt4 = initialiserTexte("crouch", "Maguntia.ttf", 25, couleurTexte, 170, 530);
    Texte txt6 = initialiserTexte("attack", "Maguntia.ttf", 25, couleurTexte, 170, 660);
    Texte txt5 = initialiserTexte("next", "Maguntia.ttf", 35, couleurTexte, 665, 435);

    // Initialisation des boutons avec sons
    Bouton btn1 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 90);
    Bouton btn2 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 230);
    Bouton btn3 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 360);
    Bouton btn4 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 490);
    Bouton btn6 = initialiserBouton("button22.png", "button333.png", "sb.wav", 285, 620);
    Bouton btn5 = initialiserBouton("button22.png", "button333.png", "sb.wav", 600, 400);

    // Boucle principale du menu
    while (quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quitter = 0;
                    }
                    if (configuring > 0) {
                        switch (configuring) {
                            case 1: bindings->moveRightKey = event.key.keysym.sym; break;
                            case 2: bindings->moveLeftKey = event.key.keysym.sym; break;
                            case 3: bindings->jumpKey = event.key.keysym.sym; break;
                            case 4: bindings->crouchKey = event.key.keysym.sym; break;
                            case 5: bindings->attackKey = event.key.keysym.sym; break;
                        }
                        configuring = 0;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.x >= btn1.position.x && event.motion.x <= btn1.position.x + btn1.images[0]->w &&
                        event.motion.y >= btn1.position.y && event.motion.y <= btn1.position.y + btn1.images[0]->h) {
                        btn1.etat = 1;
                    } else {
                        btn1.etat = 0;
                    }
                    if (event.motion.x >= btn2.position.x && event.motion.x <= btn2.position.x + btn2.images[0]->w &&
                        event.motion.y >= btn2.position.y && event.motion.y <= btn2.position.y + btn2.images[0]->h) {
                        btn2.etat = 1;
                    } else {
                        btn2.etat = 0;
                    }
                    if (event.motion.x >= btn3.position.x && event.motion.x <= btn3.position.x + btn3.images[0]->w &&
                        event.motion.y >= btn3.position.y && event.motion.y <= btn3.position.y + btn3.images[0]->h) {
                        btn3.etat = 1;
                    } else {
                        btn3.etat = 0;
                    }
                    if (event.motion.x >= btn4.position.x && event.motion.x <= btn4.position.x + btn4.images[0]->w &&
                        event.motion.y >= btn4.position.y && event.motion.y <= btn4.position.y + btn4.images[0]->h) {
                        btn4.etat = 1;
                    } else {
                        btn4.etat = 0;
                    }
                    if (event.motion.x >= btn6.position.x && event.motion.x <= btn6.position.x + btn6.images[0]->w &&
                        event.motion.y >= btn6.position.y && event.motion.y <= btn6.position.y + btn6.images[0]->h) {
                        btn6.etat = 1;
                    } else {
                        btn6.etat = 0;
                    }
                    if (event.motion.x >= btn5.position.x && event.motion.x <= btn5.position.x + btn5.images[0]->w &&
                        event.motion.y >= btn5.position.y && event.motion.y <= btn5.position.y + btn5.images[0]->h) {
                        btn5.etat = 1;
                    } else {
                        btn5.etat = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.x >= btn1.position.x && event.button.x <= btn1.position.x + btn1.images[0]->w &&
                        event.button.y >= btn1.position.y && event.button.y <= btn1.position.y + btn1.images[0]->h) {
                        btn1.etat = 1;
                        Mix_PlayChannel(-1, btn1.son, 0);
                        configuring = 1;
                    }
                    if (event.button.x >= btn2.position.x && event.button.x <= btn2.position.x + btn2.images[0]->w &&
                        event.button.y >= btn2.position.y && event.button.y <= btn2.position.y + btn2.images[0]->h) {
                        btn2.etat = 1;
                        Mix_PlayChannel(-1, btn2.son, 0);
                        configuring = 2;
                    }
                    if (event.button.x >= btn3.position.x && event.button.x <= btn3.position.x + btn3.images[0]->w &&
                        event.button.y >= btn3.position.y && event.button.y <= btn3.position.y + btn3.images[0]->h) {
                        btn3.etat = 1;
                        Mix_PlayChannel(-1, btn3.son, 0);
                        configuring = 3;
                    }
                    if (event.button.x >= btn4.position.x && event.button.x <= btn4.position.x + btn4.images[0]->w &&
                        event.button.y >= btn4.position.y && event.button.y <= btn4.position.y + btn4.images[0]->h) {
                        btn4.etat = 1;
                        Mix_PlayChannel(-1, btn4.son, 0);
                        configuring = 4;
                    }
                    if (event.button.x >= btn6.position.x && event.button.x <= btn6.position.x + btn6.images[0]->w &&
                        event.button.y >= btn6.position.y && event.button.y <= btn6.position.y + btn6.images[0]->h) {
                        btn6.etat = 1;
                        Mix_PlayChannel(-1, btn6.son, 0);
                        configuring = 5;
                    }
                    if (event.button.x >= btn5.position.x && event.button.x <= btn5.position.x + btn5.images[0]->w &&
                        event.button.y >= btn5.position.y && event.button.y <= btn5.position.y + btn5.images[0]->h) {
                        btn5.etat = 1;
                        Mix_PlayChannel(-1, btn5.son, 0);
                        quitter = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    btn1.etat = 0;
                    btn2.etat = 0;
                    btn3.etat = 0;
                    btn4.etat = 0;
                    btn6.etat = 0;
                    btn5.etat = 0;
                    break;
            }
        }

        // Afficher l'image de fond et tous les éléments du menu
        afficherImage(ecran, img);
        afficherBouton(ecran, btn1);
        afficherBouton(ecran, btn2);
        afficherBouton(ecran, btn3);
        afficherBouton(ecran, btn4);
        afficherBouton(ecran, btn6);
        afficherBouton(ecran, btn5);
        afficherTexte(ecran, txt);
        afficherTexte(ecran, txt2);
        afficherTexte(ecran, txt3);
        afficherTexte(ecran, txt4);
        afficherTexte(ecran, txt6);
        afficherTexte(ecran, txt5);
        SDL_Flip(ecran);
    }

    // Libération des ressources
    libererImage(&img);
    libererBouton(&btn1);
    libererBouton(&btn2);
    libererBouton(&btn3);
    libererBouton(&btn4);
    libererBouton(&btn6);
    libererBouton(&btn5);
    libererTexte(&txt);
    libererTexte(&txt2);
    libererTexte(&txt3);
    libererTexte(&txt4);
    libererTexte(&txt6);
    libererTexte(&txt5);
}

// Game loop using the assigned keybindings
void gameLoop(SDL_Surface *ecran, KeyBindings *bindings, const char *spriteSheetPath) {
    SDL_Event event;
    int gameRunning = 1;

    // Load the game background
    Image gameBackground = initialiserImage("kaiplanet.png");
    if (!gameBackground.surface) {
        printf("Erreur de chargement du fond de jeu : %s\n", SDL_GetError());
        return;
    }

    // Initialize the character with the selected sprite sheet
    Perso perso;
    initialiserPerso(&perso, spriteSheetPath);
    SDL_Color couleurTexte = {0, 0, 0, 255};
    Bouton coeur1 = initialiserBouton("coeur.png", "button333.png", "sb.wav", 600, 30);
    Bouton coeur2 = initialiserBouton("coeur.png", "button333.png", "sb.wav", 650, 30);
    Bouton coeur3 = initialiserBouton("coeur.png", "button333.png", "sb.wav", 700, 30);
    Texte score = initialiserTexte("score", "Maguntia.ttf", 45, couleurTexte, 0, 0);

    Uint32 t_prev = SDL_GetTicks(), dt;

    while (gameRunning) {
        dt = SDL_GetTicks() - t_prev;
        t_prev = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        gameRunning = 0;
                    }
                    if (event.key.keysym.sym == bindings->moveRightKey) {
                        perso.moving_right = 1;
                    }
                    if (event.key.keysym.sym == bindings->moveLeftKey) {
                        perso.moving_left = 1;
                    }
                    if (event.key.keysym.sym == bindings->jumpKey && !perso.jumping) {
                        perso.jumping = 1;
                        perso.jump_velocity = 200;
                    }
                    if (event.key.keysym.sym == bindings->crouchKey) {
                        perso.crouching = 1;
                    }
                    if (event.key.keysym.sym == bindings->attackKey && !perso.attacking) {
                        perso.attacking = 1;
                        perso.attack_frame = 0;
                        perso.attack_duration = SDL_GetTicks();
                    }
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == bindings->moveRightKey) {
                        perso.moving_right = 0;
                    }
                    if (event.key.keysym.sym == bindings->moveLeftKey) {
                        perso.moving_left = 0;
                    }
                    if (event.key.keysym.sym == bindings->crouchKey) {
                        perso.crouching = 0;
                    }
                    break;
            }
        }

        // Update character position and state
        movePerso(&perso, dt);
        handleJump(&perso, dt);
        handleCrouch(&perso);
        //handleAttack(&perso, dt);

        // Render the game
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        afficherImage(ecran, gameBackground);
        afficherBouton(ecran, coeur1);
        afficherBouton(ecran, coeur2);
        afficherBouton(ecran, coeur3);
        afficherTexte(ecran, score);
        renderPerso(ecran, &perso);
        SDL_Flip(ecran);

        SDL_Delay(16); // Approximately 60 FPS
    }

    // Free game resources
    libererImage(&gameBackground);
    libererBouton(&coeur1);
    libererBouton(&coeur2);
    libererBouton(&coeur3);
    libererTexte(&score);
    SDL_FreeSurface(perso.spriteSheet);
}

void initialiserPerso(Perso *p, const char *spriteSheetPath) {
    p->x = 100; // Start closer to the left side
    p->y = 445; // Feet at y = 500, assuming height ~55
    p->vitesse = 0;
    p->moving_right = 0;
    p->moving_left = 0;
    p->jumping = 0;
    p->jump_velocity = 0;
    p->gravity = 500;
    p->crouching = 0;
    p->attacking = 0;
    p->attack_frame = 0;
    p->attack_duration = 0;
    p->ground_level = 500; // Ground level at y = 500 to match the background
    p->frame = 0;
    p->last_frame_time = SDL_GetTicks();

    strncpy(p->spriteSheetPath, spriteSheetPath, sizeof(p->spriteSheetPath) - 1);
    p->spriteSheetPath[sizeof(p->spriteSheetPath) - 1] = '\0';

    p->spriteSheet = IMG_Load(p->spriteSheetPath);
    if (!p->spriteSheet) {
        printf("Failed to load sprite sheet: %s\n", SDL_GetError());
        exit(1);
    }

    //SDL_SetColorKey(p->spriteSheet, SDL_SRCCOLORKEY, SDL_MapRGB(p->spriteSheet->format, 0, 255, 0));
}

void movePerso(Perso *p, Uint32 dt) {
    if (p->moving_right) {
        p->vitesse = 100;
        p->x += p->vitesse * (dt / 1000.0);

        Uint32 now = SDL_GetTicks();
        if (now - p->last_frame_time > 100) {
            p->frame = (p->frame + 1) % 10;
            p->last_frame_time = now;
        }
    } else if (p->moving_left) {
        p->vitesse = -100;
        p->x += p->vitesse * (dt / 1000.0);

        Uint32 now = SDL_GetTicks();
        if (now - p->last_frame_time > 100) {
            p->frame = (p->frame + 1) % 10;
            p->last_frame_time = now;
        }
    }

    // Keep character within screen bounds
    int frameWidth = 35; // Average frame width
    if (p->x < 0) p->x = 0;
    if (p->x > SCREEN_WIDTH - frameWidth) p->x = SCREEN_WIDTH - frameWidth;

    if (!p->jumping && !p->moving_right && !p->moving_left) {
        p->frame = 0;
    }
}

void renderPerso(SDL_Surface *screen, Perso *p) {
    int frameWidth = 35;
    int frameHeight = 63;
    int row = 0;
    int frame = p->frame;
    int y_offset = 0;

    if (p->attacking) {
        row = 4;
        frame = p->attack_frame;
        frameWidth = 35;
        frameHeight = 63;
        y_offset = 0;
    } else if (p->crouching) {
        row = 3;
        frame = 0;
        frameWidth = 45;  // Updated to match new sprite sheet
        frameHeight = 58;
        y_offset = 5;     // 63 - 58 = 5
    } else if (p->jumping) {
        row = 2;
        frame = 0;
        frameWidth = 35;  // Updated to match new sprite sheet
        frameHeight = 70;
        y_offset = -7;    // 70 - 63 = 7
    } else if (!p->moving_right && !p->moving_left) {
        row = 1;
        frame = 0;
        frameWidth = 35;
        frameHeight = 63;
        y_offset = 0;
    } else {
        row = 0;
        frameWidth = 35;
        frameHeight = 63;
        y_offset = 0;
    }

    int src_x = frame * frameWidth;
    int src_y;
    if (row == 0) src_y = 0;
    else if (row == 1) src_y = 63;
    else if (row == 2) src_y = 126;
    else if (row == 3) src_y = 196;
    else src_y = 254;

    SDL_Rect src_rect = { src_x, src_y, frameWidth, frameHeight };
    SDL_Rect dest_rect = { (int)p->x, (int)p->y + y_offset, frameWidth, frameHeight };

    SDL_BlitSurface(p->spriteSheet, &src_rect, screen, &dest_rect);
}

void handleJump(Perso *p, Uint32 dt) {
    if (p->jumping) {
        p->y -= p->jump_velocity * (dt / 1000.0);
        p->jump_velocity -= p->gravity * (dt / 1000.0);
        if (p->y >= p->ground_level) {
            p->y = p->ground_level;
            p->jumping = 0;
            p->jump_velocity = 0;
        }
    }
}

void handleCrouch(Perso *p) {
    if (p->crouching) {
        p->y = p->ground_level;
        p->jumping = 0;
        p->jump_velocity = 0;
    }
}

/*void handleAttack(Perso *p, Uint32 dt) {
    if (p->attacking) {
        Uint32 now = SDL_GetTicks();
        if (now - p->attack_duration >= 200) { // 200 ms per frame
            p->attack_frame++;
            p->attack_duration = now;
            if (p->attack_frame >= 2) { // Only 2 frames in the new sprite sheet
                p->attacking = 0;
                p->attack_frame = 0;
            }
        }
    }
}*/

