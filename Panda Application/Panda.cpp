#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "config_sdl.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <SDL_image.h>

using namespace std;

SDL_Rect bam1;
SDL_Rect bam2;
SDL_Rect title;
SDL_Surface* image;
SDL_Texture* pTextureImage;
SDL_Point points[100];
SDL_Point pointsmax[100];
SDL_Point pointsmin[100];


const int LARGEUR = 1000; //largeur fenetre
const int LARGEUR_TOTALE = 1250;//largeur fenetre
const int HAUTEUR = 700;  //hauteur fenetre
int nb_bambous = 8;

bool Menu = true;
bool Pause = false;
bool Config = false;
bool Apli = false;

bool batterieOff = false;

int jours = 0;
int nbCoupe = 0;
int maxi = 0;
int maxiBambou = 0;
int fastBambou = 0;
int maxifastBambou = 0;
int choixUser = 1;
int choix = choixUser;
bool boucle = false;
int xpanda;
int batterie = 100;
int x = 300;
int y;
int indGraph = 0;
bool charging = false;
bool mute = true;



struct bambous
{
    int taille = 0;
    int croissance = 1;
    int cpt = 0;
};


bambous tab[8];
int coPanda[9];
int tabymoy[100];
int tabymax[100];
int tabymin[100];

void logo(SDL_Window* win) {

    SDL_Surface* icon = IMG_Load("logo.png");
    if (!icon)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());

    }
    SDL_SetWindowIcon(win, icon);
}

void init_point() {
    for (int i = 0; i < 100; i++) {
        points[i].x = LARGEUR_TOTALE + 1;
        points[i].y = HAUTEUR - 175;

        pointsmin[i].x = LARGEUR_TOTALE + 1;
        pointsmin[i].y = HAUTEUR - 75;

        pointsmax[i].x = LARGEUR_TOTALE + 1;
        pointsmax[i].y = HAUTEUR - 275;
    }
}

void menu(SDL_Renderer* rendu) {
    SDL_Surface* menus = IMG_Load("menu.png");

    SDL_Texture* pTextureImagemenu = SDL_CreateTextureFromSurface(rendu, menus);
    SDL_FreeSurface(menus);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    SDL_QueryTexture(pTextureImagemenu, nullptr, nullptr, &src1.w, &src1.h);
    SDL_RenderCopy(rendu, pTextureImagemenu, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void config(SDL_Renderer* rendu) {
    SDL_Surface* config = IMG_Load("config.png");

    SDL_Texture* pTextureImageconfig = SDL_CreateTextureFromSurface(rendu, config);
    SDL_FreeSurface(config);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImageconfig, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImageconfig, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void pause(SDL_Renderer* rendu) {
    SDL_Surface* pause = IMG_Load("pause.png");

    SDL_Texture* pTextureImagepause = SDL_CreateTextureFromSurface(rendu, pause);
    SDL_FreeSurface(pause);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagepause, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagepause, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void coordonéesPanda() {
    for (int i = 1; i < nb_bambous + 2; i++) {
        if (i < 9) {
            coPanda[i - 1] = 80 * i + 10;
        }
        else {
            coPanda[i - 1] = LARGEUR - 95;
        }
    }
}

void textjours(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_Color blanche = { 0,0,0 }; //on définit une couleur de texte

    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille
    positionTexte.x = LARGEUR_TOTALE - 430;
    positionTexte.y = 20;
    //on place le texte au point (100,100)
    char score_text[500];

    ofstream sortie("option.txt", ios::trunc);
    sortie << "Jours : " << jours << endl;
    sortie.close();

    ifstream entrer("option.txt", ios::in);
    entrer.getline(score_text, 500);

    entrer.close();

    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text, blanche);
    SDL_Texture* text = SDL_CreateTextureFromSurface(rendu, textSurface);
    //on maj le rectangle couvrant cette texture

    SDL_QueryTexture(text, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 1.5;
    positionTexte.h *= 1.5;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, text, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(text);
}

void textcoupe(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_Color blanche = { 0,0,255 }; //on définit une couleur de texte

    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille
    positionTexte.x = LARGEUR_TOTALE - 185;
    positionTexte.y = 300;
    //on place le texte au point (100,100)
    char score_text[500];

    ofstream sortie("option.txt2", ios::trunc);
    sortie << "Nb Coupe : " << nbCoupe << endl;
    sortie.close();

    ifstream entrer("option.txt2", ios::in);
    entrer.getline(score_text, 500);

    entrer.close();

    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text, blanche);
    SDL_Texture* text = SDL_CreateTextureFromSurface(rendu, textSurface);
    //on maj le rectangle couvrant cette texture

    SDL_QueryTexture(text, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 1;
    positionTexte.h *= 1;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, text, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(text);
}

void texteLégende(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_Color blanche = { 255,255,255 }; //on définit une couleur de texte

    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 185;
    positionTexte.y = 50;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Légende :", blanche, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 1.5;
    positionTexte.h *= 1.5;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMax(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color vert = { 85, 255, 51 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 150;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut max", vert, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMoy(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color violet = { 127,0,255 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 200;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut moy", violet, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMin(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 250;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut min", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void affichage_panda(SDL_Renderer* rendu, int x) {
    // on importe une image
    image = IMG_Load("panda.png");
    pTextureImage = SDL_CreateTextureFromSurface(rendu, image);
    SDL_FreeSurface(image);

    SDL_Rect src{ 0, 0, 0, 0 };
    SDL_Rect dst{ x, HAUTEUR - 50, 50, 50 };


    SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src.w, &src.h);
    SDL_RenderCopy(rendu, pTextureImage, &src, &dst);
    SDL_DestroyTexture(pTextureImage);
    SDL_RenderPresent(rendu);
}

void init_croissance() {

    tab[0].croissance = 1;
    tab[1].croissance = 9;
    tab[2].croissance = 3;
    tab[3].croissance = 10;
    tab[4].croissance = 5;
    tab[5].croissance = 6;
    tab[6].croissance = 7;
    tab[7].croissance = 8;

}

void init_ligne_max(SDL_Renderer* rendu, int taille, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 0, HAUTEUR - taille, LARGEUR - 11, HAUTEUR - taille);

    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR - 28 - 11;
    positionTexte.y = HAUTEUR - taille - 15;

    SDL_Texture* texture = loadText(rendu, "Max", rouge, font);

    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

    positionTexte.w *= 0.6;
    positionTexte.h *= 0.6;

    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);


    SDL_DestroyTexture(texture);

}

void init_ligne_coupe(SDL_Renderer* rendu, int taille, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 156, 100, 55, 255);
    SDL_RenderDrawLine(rendu, 0, HAUTEUR - taille, LARGEUR - 11, HAUTEUR - taille);

    SDL_Color rouge = { 156, 100, 55 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR - 32 - 20;
    positionTexte.y = HAUTEUR - taille - 15;

    SDL_Texture* texture = loadText(rendu, "Coupe", rouge, font);

    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

    positionTexte.w *= 0.6;
    positionTexte.h *= 0.6;

    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);


    SDL_DestroyTexture(texture);

}

void bambou(SDL_Renderer* rendu, TTF_Font* font) {

    for (int i = 1; i < nb_bambous + 1; i++) {

        bam1.x = 80 * i;
        bam1.w = 10;
        bam1.h = -(tab[i - 1].taille);
        bam1.y = HAUTEUR - 10;

        if (tab[i - 1].taille > maxi) {
            maxi = tab[i - 1].taille;
        }

        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &bam1);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &bam1);
        SDL_RenderPresent(rendu);

        for (int j = 1; j < (tab[i - 1].cpt) + 1; j++) {

            bam2.x = (80 * i) - 4;
            bam2.w = 18;
            bam2.h = 3;
            bam2.y = HAUTEUR - ((((tab[i - 1].croissance * 6) + 5) * j) + 11);

            SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
            SDL_RenderFillRect(rendu, &bam2);
            SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
            SDL_RenderDrawRect(rendu, &bam2);

        }
    }
    if (maxi != 0) {
        init_ligne_max(rendu, maxi + 11, font);
    }
}

void batteriePleine(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect2);

    SDL_Rect rect3;
    rect3.x = 898;
    rect3.w = 10;
    rect3.h = 20;
    rect3.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect3);

    SDL_Rect rect4;
    rect4.x = 910;
    rect4.w = 10;
    rect4.h = 20;
    rect4.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect4);

    SDL_RenderPresent(rendu);
}

void batterie3barres(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect2);

    SDL_Rect rect3;
    rect3.x = 898;
    rect3.w = 10;
    rect3.h = 20;
    rect3.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect3);


    SDL_RenderPresent(rendu);
}

void batterie2barres(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 127, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 127, 0, 0);
    SDL_RenderFillRect(rendu, &rect2);


    SDL_RenderPresent(rendu);
}

void batterie1barre(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);

    SDL_RenderPresent(rendu);
}

void batterieAuto(SDL_Renderer* rendu) {

    if (batterie > 75) {
        batteriePleine(rendu);
    }
    if (batterie <= 75 && batterie > 50) {
        batterie3barres(rendu);
    }
    if (batterie <= 50 && batterie > 25) {
        batterie2barres(rendu);
    }
    if (batterie <= 25 && batterie >= 1) {
        batterie1barre(rendu);
    }
}





void moyenneCourbe(SDL_Renderer* rendu) {

    SDL_SetRenderDrawColor(rendu, 127, 0, 255, 0);
    SDL_RenderDrawPoints(rendu, points, 100);
    SDL_SetRenderDrawColor(rendu, 127, 0, 255, 0);
    SDL_RenderDrawLines(rendu, points, 100);

}

void maxCourbe(SDL_Renderer* rendu) {

    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderDrawPoints(rendu, pointsmax, 100);
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderDrawLines(rendu, pointsmax, 100);

}

void minCourbe(SDL_Renderer* rendu) {

    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderDrawPoints(rendu, pointsmin, 100);
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderDrawLines(rendu, pointsmin, 100);

}

void save() {
    ofstream sortie("save.txt", ios::out);
    sortie << nb_bambous << ";";
    for (int i = 0; i < 8; i++) {
        sortie << tab[i].croissance << ";";
    }
    sortie << choixUser << ";";
    sortie.close();
}

void charger() {
    ifstream entrer("save.txt", ios::in);
    char lignes[10];
    if (!entrer.eof()) {
        entrer.getline(lignes, 10, ';');
        nb_bambous = atoi(lignes);
        for (int i = 0; i < 8; i++) {
            entrer.getline(lignes, 10, ';');
            tab[i].croissance = atoi(lignes);
        }
        entrer.getline(lignes, 10, ';');
        choixUser = atoi(lignes);
    }
    entrer.close();
}

void son(SDL_Renderer* rendu) {
    // on importe une image de son
    SDL_Surface* son = IMG_Load("son.png");
    SDL_Texture* pTextureImageson = SDL_CreateTextureFromSurface(rendu, son);
    SDL_FreeSurface(son);
    SDL_Rect src3{ 0, 0, 0, 0 };
    SDL_Rect dst3{ 874,  120,50 , 50 };
    SDL_QueryTexture(pTextureImageson, nullptr, nullptr, &src3.w, &src3.h);
    SDL_RenderCopy(rendu, pTextureImageson, &src3, &dst3);
    SDL_RenderPresent(rendu);

}
void sonOff(SDL_Renderer* rendu) {
    // on importe une image de son
    SDL_Surface* son = IMG_Load("sonOff.png");
    SDL_Texture* pTextureImageson = SDL_CreateTextureFromSurface(rendu, son);
    SDL_FreeSurface(son);
    SDL_Rect src3{ 0, 0, 0, 0 };
    SDL_Rect dst3{ 874,  120,80 , 50 };
    SDL_QueryTexture(pTextureImageson, nullptr, nullptr, &src3.w, &src3.h);
    SDL_RenderCopy(rendu, pTextureImageson, &src3, &dst3);
    SDL_RenderPresent(rendu);

}


void affichage(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderClear(rendu);

    // on importe une image de maison
    SDL_Surface* fond = IMG_Load("fond.png");
    SDL_Texture* pTextureImagefond = SDL_CreateTextureFromSurface(rendu, fond);
    SDL_FreeSurface(fond);

    SDL_Rect src2{ 0, 0, 0, 0 };
    SDL_Rect dst2{ 0,0,LARGEUR_TOTALE,HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagefond, nullptr, nullptr, &src2.w, &src2.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagefond, &src2, &dst2);



    texteLégende(rendu, font);
    texteHautMax(rendu, font);
    texteHautMoy(rendu, font);
    texteHautMin(rendu, font);
    textjours(rendu, font);
    textcoupe(rendu, font);

    SDL_DestroyTexture(pTextureImagefond);



    bambou(rendu, font);
    moyenneCourbe(rendu);
    minCourbe(rendu);
    maxCourbe(rendu);

    if (mute == false) {

        sonOff(rendu);

    }
    else if (mute == true) {

        son(rendu);

    }

    if (choix == 2 || choix == 3) {
        init_ligne_coupe(rendu, x, font);
    }

    if (jours == 0) {
        affichage_panda(rendu, 790);
        xpanda = 8;
    }
    batterieAuto(rendu);
    SDL_RenderPresent(rendu);

}

void choix1() {
    maxiBambou = 0;
    if (jours > 0) {

        for (int i = 0; i < nb_bambous; i++) {
            if (tab[i].taille > tab[maxiBambou].taille) {
                maxiBambou = i;
            }
        }

        tab[maxiBambou].taille = 0;
        tab[maxiBambou].cpt = 0;
        nbCoupe++;
    }
}

void choix2(SDL_Renderer* rendu, TTF_Font* font) {
    fastBambou = 0;
    if (jours > 0) {

        for (int i = 0; i < nb_bambous; i++) {
            if (tab[i].taille >= x && ((tab[i].croissance * 6) + 5) > ((tab[fastBambou].croissance * 6) + 5)) {
                fastBambou = i;
            }
        }
        if (fastBambou == 0) {
            if (tab[0].taille >= x) {
                tab[fastBambou].taille = 0;
                tab[fastBambou].cpt = 0;
                nbCoupe++;
                xpanda = fastBambou;
            }
            else {

                xpanda = 9;
            }
        }
        else {

            tab[fastBambou].taille = 0;
            tab[fastBambou].cpt = 0;
            nbCoupe++;
            xpanda = fastBambou;

        }
    }
}

void choix3(SDL_Renderer* rendu, TTF_Font* font) {
    maxifastBambou = 0;

    if (jours > 0) {

        for (int i = 0; i < nb_bambous; i++) {
            if ((tab[i].taille >= x && ((tab[i].croissance * 6) + 5) > ((tab[maxifastBambou].croissance * 6) + 5)) || tab[i].taille > tab[maxifastBambou].taille) {

                maxifastBambou = i;

            }
        }

        tab[maxifastBambou].taille = 0;
        tab[maxifastBambou].cpt = 0;
        nbCoupe++;
        xpanda = maxifastBambou;


    }
}

void jauge(SDL_Renderer* rendu, int x) {
    SDL_Rect jauge;
    jauge.x = LARGEUR - 9;
    jauge.w = 10;
    jauge.h = 5 * x;
    jauge.y = HAUTEUR - jauge.h;
    SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
    SDL_RenderFillRect(rendu, &jauge);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderDrawRect(rendu, &jauge);
    SDL_RenderPresent(rendu);
}

void choix0() {

    tab[xpanda].taille = 0;
    tab[xpanda].cpt = 0;
    nbCoupe++;

}

void GameOver(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille
    //on place le texte au point (100,100)
    positionTexte.x = 30;
    positionTexte.y = 250;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Game Over", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 8;
    positionTexte.h *= 8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}


void croissance(SDL_Renderer* rendu, TTF_Font* font) {

    int minimum = 0;
    int maximum = 0;
    int moyenne = 0;

    for (int i = 1; i <= nb_bambous; i++) {
        if (tab[i - 1].taille + ((tab[i - 1].croissance * 6) + 5) < HAUTEUR - (65 * 2)) {
            tab[i - 1].taille = (tab[i - 1].taille) + ((tab[i - 1].croissance * 6) + 5);
            tab[i - 1].cpt++;
        }
    }

    jours++;

    minimum = tab[0].taille;

    for (int i = 0; i < nb_bambous; i++) {

        if (tab[i].taille > maximum) {
            maximum = tab[i].taille;
        }
        if (tab[i].taille < minimum && tab[i].taille != 0) {
            minimum = tab[i].taille;
        }
        moyenne = tab[i].taille + moyenne;
    }
    moyenne = moyenne / nb_bambous;

    if (indGraph == 60) {
        indGraph = indGraph - 2;


        for (int i = 2; i < 60; i++) {
            points[i - 2].x = (i - 2) * 3 + 1005;
            points[i - 2].y = points[i].y;

            pointsmin[i - 2].x = (i - 2) * 3 + 1005;
            pointsmin[i - 2].y = pointsmin[i].y;

            pointsmax[i - 2].x = (i - 2) * 3 + 1005;
            pointsmax[i - 2].y = pointsmax[i].y;
        }
    }

    points[indGraph].x = indGraph * 3 + 1005;
    points[indGraph].y = HAUTEUR - ((moyenne * 100) / 500) - 90;


    pointsmax[indGraph].x = indGraph * 3 + 1005;
    pointsmax[indGraph].y = HAUTEUR - ((maximum * 100) / 500) - 190;

    pointsmin[indGraph].x = indGraph * 3 + 1005;
    pointsmin[indGraph].y = HAUTEUR - ((minimum * 100)/ 500) - 10;



    indGraph++;





    if (choix == 1) {
        if (batterie < 6) {
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[8]);
            jauge(rendu, nbCoupe);
            charging = true;
            while (batterie < 100) {
                SDL_Delay(150);
                batterie = batterie + 10;
                batterieAuto(rendu);
            }
            batterie = 100;

        }
        else {
            batterie = batterie - 5;
            choix1();
            charging = false;
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[maxiBambou]);
            jauge(rendu, nbCoupe);
            xpanda = maxiBambou;
        }

    }
    else if (choix == 2) {
        if (batterie < 6) {
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[8]);
            jauge(rendu, nbCoupe);
            charging = true;
            while (batterie < 100) {
                SDL_Delay(150);
                batterie = batterie + 10;
                batterieAuto(rendu);
            }
            batterie = 100;

        }
        else {

            choix2(rendu, font);
            affichage(rendu, font);
            jauge(rendu, nbCoupe);
            charging = false;
            if (xpanda == 9) {
                affichage_panda(rendu, 790);

                xpanda = 8;
            }
            else {
                batterie = batterie - 5;
                affichage_panda(rendu, coPanda[fastBambou]);
                xpanda = fastBambou;

            }
        }

    }
    else if (choix == 3) {
        if (batterie < 6) {
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[8]);
            jauge(rendu, nbCoupe);
            charging = true;
            while (batterie < 100) {
                SDL_Delay(150);
                batterie = batterie + 10;
                batterieAuto(rendu);
            }
            batterie = 100;

        }
        else {

            choix3(rendu, font);
            affichage(rendu, font);
            charging = false;
            jauge(rendu, nbCoupe);
            if (xpanda == 9) {
                affichage_panda(rendu, 790);

                xpanda = 8;
            }
            else {
                batterie = batterie - 5;
                affichage_panda(rendu, coPanda[maxifastBambou]);
                xpanda = maxifastBambou;


            }
        }

    }
    else if (choix == 0) {
        if (batterie == 0) {
            batterieOff = true;
            charging = true;
        }
        else {
            batterie = batterie - 5;
            choix0();
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[xpanda]);
            jauge(rendu, nbCoupe);

        }
    }
    else {
        affichage(rendu, font);
        affichage_panda(rendu, coPanda[8]);
        jauge(rendu, nbCoupe);

    }

    ofstream sortie("laptest.txt", ios::app);
    sortie << "Jours : " << jours << endl;
    sortie << "Hauteur Maximum : " << maximum << endl;
    sortie << "Hauteur Moyenne : " << moyenne << endl;
    sortie << "Hauteur Minimum : " << minimum << endl;
    sortie << "En charge : " << (charging == true ? "Oui" : "Non") << endl;
    sortie << "------------------------------" << endl;

    sortie.close();

}

void affiche1(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "1", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 1;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche2(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "2", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 2;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche3(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "3", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 3;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche4(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "4", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 4;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche5(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "5", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 5;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche6(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "6", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 6;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche7(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "7", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 7;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche8(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "8", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 8;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}
void affiche9(SDL_Renderer* rendu, int x, TTF_Font* font, int y) {
    SDL_Color black = { 0,0,0 };
    SDL_Rect positionTexte;
    positionTexte.x = x;
    positionTexte.y = 310;
    SDL_Texture* texture = loadText(rendu, "9", black, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 1.8;
    positionTexte.h *= 1.8;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
    tab[y - 1].croissance = 9;
    tab[y - 1].taille = 0;
    tab[y - 1].cpt = 0;
}

int main(int argn, char* argv[]) {

    //ouverture de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Echec à l?ouverture de SDL";
        return 1;
    }


    //on crée la fenêtre
    SDL_Window* win = SDL_CreateWindow("Panda",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_TOTALE,
        HAUTEUR,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* rendu = SDL_CreateRenderer(
        win,  //nom de la fenêtre
        -1, //par défaut
        SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);


    //la musique

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;
    // Initialisation de SDL_Mixer
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    Mix_Music* music = Mix_LoadMUS("musique1.mp3"); // Charge notre musique

    if (music == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    Mix_PlayMusic(music, -1); // Joue notre musique 


    logo(win);
    init_croissance();
    init_point();
    coordonéesPanda();

    menu(rendu);

    ofstream sortie("laptest.txt", ios::trunc);
    sortie << "------------------------------" << endl;
    sortie.close();


    int cpt = 0;
    bool continuer = true;


    SDL_Event event;
    SDL_Event events;

    while (continuer) {

        SDL_WaitEvent(&event);

        switch (event.type) {

        case SDL_QUIT:
            continuer = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_RETURN && batterieOff == false) {
                if (Apli == true) {
                    choix = choixUser;
                    croissance(rendu, font);
                }
            }
            if (event.key.keysym.sym == SDLK_r) {
                if (Apli == true) {
                    choix = choixUser;

                    for (int i = 0; i < nb_bambous; i++) {
                        tab[i].taille = 0;
                        jours = 0;
                        nbCoupe = 0;
                        tab[i].cpt = 0;
                        batterie = 100;
                        batterieOff = false;
                        indGraph = 0;
                        init_point();
                    }
                    maxiBambou = 0;
                    maxi = 0;
                    fastBambou = 0;
                    affichage(rendu, font);

                }
            }
            if (event.key.keysym.sym == SDLK_RSHIFT && batterieOff == false) {
                if (Apli == true) {
                    choix = choixUser;
                    boucle = true;
                    while (boucle == true) {
                        SDL_PollEvent(&events);
                        SDL_Delay(600);
                        if (events.key.keysym.sym == SDLK_s) {
                            boucle = false;
                        }
                        else {
                            croissance(rendu, font);
                        }
                    }
                }


            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                if (batterie == 0) {
                    batterieOff = true;
                    GameOver(rendu, font);
                }
                if (Apli == true && batterieOff == false) {
                    bool diffJour = false;
                    charging = false;
                    choix = 0;
                    if (jours == 0) {
                        jours = 1;
                        diffJour = true;
                    }
                    affichage(rendu, font);
                    if (xpanda == 0) {

                        affichage_panda(rendu, coPanda[7]);
                        xpanda = 7;

                    }
                    else {
                        affichage_panda(rendu, coPanda[xpanda - 1]);
                        xpanda = xpanda - 1;
                    }
                    if (diffJour == true) {
                        jours--;
                        diffJour = false;
                    }
                    jauge(rendu, nbCoupe);
                    SDL_Delay(200);
                }
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                charging = false;
                bool diffJour = false;
                if (batterie == 0) {
                    batterieOff = true;
                    GameOver(rendu, font);
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;
                    if (jours == 0) {
                        jours = 1;
                        diffJour = true;
                    }
                    affichage(rendu, font);
                    if (xpanda == 7 || xpanda == 8) {

                        affichage_panda(rendu, coPanda[0]);
                        xpanda = 0;
                    }
                    else {
                        affichage_panda(rendu, coPanda[xpanda + 1]);
                        xpanda = xpanda + 1;
                    }
                    if (diffJour == true) {
                        jours--;
                        diffJour = false;
                    }
                    jauge(rendu, nbCoupe);
                    SDL_Delay(100);
                }



            }

            if (event.key.keysym.sym == SDLK_DOWN) {
                charging = true;
                if (batterie == 0) {
                    batterieOff = true;
                    GameOver(rendu, font);
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;

                    croissance(rendu, font);

                    affichage(rendu, font);
                    affichage_panda(rendu, coPanda[8]);
                    xpanda = 8;

                    SDL_Delay(300);

                    while (batterie < 100) {
                        SDL_Delay(150);
                        batterie = batterie + 10;
                        batterieAuto(rendu);
                    }
                    batterie = 100;
                    jauge(rendu, nbCoupe);
                }
            }

            if (event.key.keysym.sym == SDLK_c) {
                if (batterie == 0) {
                    batterieOff = true;
                    GameOver(rendu, font);
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;
                    croissance(rendu, font);
                }
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (Apli == true) {
                    pause(rendu);
                    Pause = true;
                    Apli = false;
                }
            }

            break;

        case SDL_MOUSEBUTTONUP://appui souris
            if (event.button.button == SDL_BUTTON_LEFT) {
                //Bouton Play menu
                if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>380 && event.button.y < 380 + 82) {
                    if (Menu == true) {
                        Menu = false;
                        Apli = true;
                        affichage(rendu, font);
                        SDL_RenderPresent(rendu);

                    }
                }
                //Bouton exit menu
                if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>537 && event.button.y < 537 + 82) {
                    if (Menu == true) {
                        Menu = false;
                        continuer = false;
                    }
                }
                //Save Button
                if (event.button.x > 725 && event.button.x < 1209 && event.button.y>630 && event.button.y < 675) {
                    if (Config == true) {

                        save();
                    }
                }
                if (event.button.x > 357 && event.button.x < 671 && event.button.y>630 && event.button.y < 675) {
                    if (Config == true) {
                        charger();
                    }
                }
                //Bouton config menu
                if (event.button.x > 1054 && event.button.x < 1054 + 136 && event.button.y>167 && event.button.y < 167 + 48) {
                    if (Menu == true) {
                        config(rendu);
                        Config = true;
                        Menu = false;
                    }
                }
                if (event.button.x > 418 && event.button.x < 418 + 154 && event.button.y>48 && event.button.y < 93) {
                    if (Config == true) {
                        Menu = true;
                        Config = false;
                        menu(rendu);
                    }
                }

                if (event.button.x > 674 && event.button.x < 674 + 154 && event.button.y>48 && event.button.y < 93) {
                    if (Config == true) {
                        Config = false;
                        Apli = true;
                        affichage(rendu, font);
                        SDL_RenderPresent(rendu);
                    }
                }

                //Config de bambou

                if (event.button.x > 550 && event.button.x < 570 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 550;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 1;
                    }
                }
                if (event.button.x > 620 && event.button.x < 650 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline1;
                        underline1.x = 625;
                        underline1.w = 19;
                        underline1.h = 5;
                        underline1.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline1);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 2;
                    }
                }
                if (event.button.x > 700 && event.button.x < 725 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 704;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 3;
                    }
                }
                if (event.button.x > 780 && event.button.x < 803 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 783;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 4;
                    }
                }
                if (event.button.x > 860 && event.button.x < 883 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 862;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 5;
                    }
                }
                if (event.button.x > 940 && event.button.x < 963 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 943;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 6;
                    }
                }
                if (event.button.x > 1020 && event.button.x < 1043 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1021;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 7;
                    }
                }
                if (event.button.x > 1100 && event.button.x < 1123 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1101;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 8;
                    }
                }

                //Menu Pause

                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>232 && event.button.y < 232 + 50) {
                    if (Pause == true) {
                        affichage(rendu, font);
                        Pause = false;
                        Apli = true;
                        if (jours == 0) {
                            affichage_panda(rendu, 790);
                        }
                        else {
                            affichage_panda(rendu, coPanda[xpanda]);
                        }
                    }
                }

                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>414 && event.button.y < 414 + 50) {
                    if (Pause == true) {
                        continuer = false;
                    }
                }
                //Config ModeDeJeu

                if (event.button.x > 528 && event.button.x < 668 && event.button.y>453 && event.button.y < 476) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 530;
                        underline.w = 137;
                        underline.h = 5;
                        underline.y = 485;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        choixUser = 1;
                        choix = choixUser;
                    }
                }

                if (event.button.x > 727 && event.button.x < 905 && event.button.y>453 && event.button.y < 476) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 727;
                        underline.w = 905 - 725;
                        underline.h = 5;
                        underline.y = 485;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        choixUser = 2;
                        choix = choixUser;
                    }
                }

                if (event.button.x > 964 && event.button.x < 1061 && event.button.y>453 && event.button.y < 476) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 964;
                        underline.w = 1061 - 962;
                        underline.h = 5;
                        underline.y = 485;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        choixUser = 3;
                        choix = choixUser;
                    }
                }

                //ConfigCroissance

                if (event.button.x > 550 && event.button.x < 623 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 574;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp1 = true;
                        while (temp1 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 1);
                                temp1 = false;
                            }
                        }

                    }
                }
                if (event.button.x > 635 && event.button.x < 707 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 659;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp2 = true;
                        while (temp2 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 2);
                                temp2 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 724 && event.button.x < 797 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 745;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 3);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 808 && event.button.x < 881 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 829;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 4);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 892 && event.button.x < 965 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 913;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 5);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 975 && event.button.x < 1048 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 997;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 6);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 1061 && event.button.x < 1133 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1082;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 7);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 1145 && event.button.x < 1219 && event.button.y>290 && event.button.y < 367) {

                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1166;
                        underline.w = 30;
                        underline.h = 5;
                        underline.y = 359;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);

                        bool temp3 = true;
                        while (temp3 == true) {
                            SDL_PollEvent(&events);
                            if (events.key.keysym.sym == SDLK_1) {
                                affiche1(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_2) {
                                affiche2(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_3) {
                                affiche3(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_4) {
                                affiche4(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_5) {
                                affiche5(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_6) {
                                affiche6(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_7) {
                                affiche7(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_8) {
                                affiche8(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                            if (events.key.keysym.sym == SDLK_9) {
                                affiche9(rendu, underline.x + 3, font, 8);
                                temp3 = false;
                            }
                        }
                    }
                }
                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>323 && event.button.y < 323 + 50) {
                    if (Pause == true) {
                        Pause = false;
                        config(rendu);
                        SDL_Delay(500);
                        Config = true;

                    }
                }
                if (event.button.x > 874 && event.button.x < 920 && event.button.y>130 && event.button.y < 165) {
                    if (mute == true && Apli == true) {
                        Mix_PauseMusic();
                        mute = false;
                        affichage(rendu, font);
                    }
                    else if (mute == false && Apli == true) {

                        Mix_ResumeMusic();
                        mute = true;
                        affichage(rendu, font);
                    }
                }
            }
            break;

        }
    }

    TTF_CloseFont(font); //on ferme la font
    TTF_Quit(); //on quitte la TTF

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(win);

    //fermeture
    SDL_Quit();

    return 0;
}
