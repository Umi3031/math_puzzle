#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h> // Windows-д зориулсан kbhit, getch
#include <windows.h>

#define MAX_LEVEL 100
#define FILENAME "record.txt"

int lives = 3;
int score = 0;
int record = 0;
void print_banner() {
    printf("\033[95m");
    printf(" ______     ______     ______     ______\n");
    printf("/\\  == \\   /\\  ___\\   /\\  == \\   /\\  ___\\\n");
    printf("\\ \\  __<   \\ \\  __\\   \\ \\  __<   \\ \\___  \\\n");
    printf(" \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\\n");
    printf("  \\/_/ /_/   \\/_____/   \\/_/ /_/   \\/_____/\n");
    printf("\033[0m\n");
}
void game_over_banner() {
    printf("\033[91m");
    printf("  _____                         ____                 \n");
    printf(" / ____|                       / __ \\                \n");
    printf("| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ \n");
    printf("| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|\n");
    printf("| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   \n");
    printf(" \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   \n");
    printf("\033[0m\n");
}
void play_correct_sound() {
    Beep(1000, 150); // Зөв хариулт
}
void play_wrong_sound() {
    Beep(300, 400); // Буруу хариулт
}
void play_sound() {
    Beep(500, 500);
}
void load_record() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp != NULL) {
        fscanf(fp, "%d", &record);
        fclose(fp);
    }
}

void save_record() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp != NULL) {
        fprintf(fp, "%d", score);
        fclose(fp);
    }
}

void show_rules() {
    printf("\033[33m_________________________________________________________________________________________________________________________________________________________\n");
    printf("\033[93m Togloomiin durem:\n");
    printf("\033[33m_________________________________________________________________________________________________________________________________________________________\n");
    printf("\033[96m 1. Niit 100 tuvshintei, 3 amitai.\n");
    printf("\033[96m 2. 0-eer tugsukh tuvshind (10, 20 ...) 3 bodlogo ugnu.\n");
    printf("\033[96m 3. Khyalbar bodlogo: +1 onoo, Khund bodlogo: +3 onoo\n");
    printf("\033[96m 4. !, %% bolon butarkhai orson bol onoog 2 dakhin usgunu\n");
    printf("\033[96m 5. Khereb ta record evdvel: +50 bonus onoo ugnu\n");
    printf("\033[96m 6. 'h' gej bichbel tuslamj ugnu\n");
    printf("\033[96m 7. 30 sekundiin dotor kharyult uguugui bol 'Khugtsaa dussan' gej tootsno.\n");
    printf("\033[33m_________________________________________________________________________________________________________________________________________________________\n");
    printf("\033[97m---------------------------------------------\n\n");
}

void show_help() {
    printf("\033[34m_________________________________________________________________________________________________________________________________________________________\n");
    printf("\n \033[93m Tuslamj:\n");
    printf("\033[34m_________________________________________________________________________________________________________________________________________________________\n");
    printf("-\033[93m Ta zubkhun toon kharyu bichne.\n");
    printf("-\033[93m Khereb buruu kharyulbal 1 ami khasagdana.\n");
    printf("-\033[93m Ami 0 bolbol togloom duusna.\n");
    printf("-\033[93m Level buriin ekhend tailbar garna.\n\n");
    printf("\033[34m_________________________________________________________________________________________________________________________________________________________\n");
}

// --------------- Таймтай оролт авах функц ---------------
int timed_input(char *input, int max_len, int timeout_sec) {
    int len = 0;
    time_t start = time(NULL);

    printf("\033[94m Kharyu: ");
    fflush(stdout);

    while (1) {
        if (_kbhit()) {
            char ch = _getch();

            if (ch == '\r' || ch == '\n') {
                input[len] = '\0';
                printf("\n");
                return 1;
            }

            if (len < max_len - 1 && ch >= 32 && ch <= 126) {
                input[len++] = ch;
                putchar(ch);
            }
        }

        if (difftime(time(NULL), start) > timeout_sec) {
            printf("\n \033[91m Khugatsaa duussan!\n");
            play_wrong_sound();
            return 0;
        }
    }
}
// ---------------------------------------------------------

int ask_question(int level) {
    int a, b, answer, user_answer, correct = 0, score_add = 0;
    char input[20];
    float fresult;
    int bonus_multiplier = 1;

    int max_type = 2;
    for (int i = 10; i <= level && i <= 60; i += 10)
        max_type++;

    // Special 3 challenge
    if (level % 10 == 0) {
        printf("\033[33m_________________________________________________________________________________________________________________________________________________________\n");
        printf("\033[93m >> Special Challenge! 3 daalgavar <<\n");
        for (int i = 0; i < 3; i++) {
            a = rand() % (level + 10) + 1;
            b = rand() % (level + 5) + 1;
            printf("\033[93m [Challenge %d] %d + ? + ? - (%d -?)= %d\n", i + 1, a, a,  a + b + b - (a-b));
            printf("\a");

            if (!timed_input(input, 20, 30)) {
                lives--;
                if (lives <= 0) return correct;
                continue;
            }

            user_answer = atoi(input);
            if (user_answer == b) {
                printf("\033[92m Zuv!\n");
                play_correct_sound();
                score += 1;
                correct++;
            } else {
                printf("\033[91m Buruu! Zuv kharyu: %d\n", b);
                play_wrong_sound();
                lives--;
                if (lives <= 0) return correct;
            }
        }
        return correct;
    }

    // Normal questions
    a = rand() % (level + 10) + 1;
    b = rand() % (level + 5) + 1;

    int type = rand() % max_type;
    switch (type) {
        case 0:
            printf("\033[94m %d + ? + ? = %d\n", a, a + b + b);
            play_sound();
            answer = b;
            score_add = 1;
            break;
        case 1:
            printf("\033[94m %d - ? - ? = %d\n", a + b + b, a);
            play_sound();
            answer = b;
            score_add = 1;
            break;
        case 2:
            printf("\033[94m %d * ? + ? = %d\n", a, a * b + b);
            play_sound();
            answer = b;
            score_add = 3;
            break;
        case 3:
            printf("\033[94m %d / ? - ? = %d\n", a * b + b, a);
            play_sound();
            answer = b;
            score_add = 3;
            break;
        case 4:
            printf("\033[94m %d %% ? = %d\n", a * b + b, b);
            play_sound();
            answer = b;
            score_add = 3;
            bonus_multiplier = 2;
            break;
        case 5:
            fresult = (float)(a * b) / 100.0;
            printf("\033[94m (%d * %d) / 100 = ? (2 oron)\n", a, b);
            play_sound();
            printf("\033[93m Kharyu (ex: 2.35): ");

            if (!timed_input(input, 20, 30)) {
                lives--;
                return 0;
            }

            float user_float = atof(input);
            if (fabs(user_float - fresult) < 0.01) {
                printf("\033[92m Zuv!\n");
                play_correct_sound();
                return 3 * 2;
            } else {
                printf("\033[91m Buruu! Zuv kharyu: %.2f\n", fresult);
                play_wrong_sound();
                lives--;
                return 0;
            }
    }

    if (!timed_input(input, 20, 30)) {
        lives--;
        return 0;
    }

    if (strcmp(input, "h") == 0) {
        show_help();
        return ask_question(level); // дахин асууна
    }

    user_answer = atoi(input);
    if (user_answer == answer) {
        printf("\033[92m Zuv!\n");
        play_correct_sound();
        return score_add * bonus_multiplier;
    } else {
        printf("\033[91m Buruu! Zuv kharyu: %d\n", answer);
        play_wrong_sound();
        lives--;
        return 0;
    }
}

int main() {
    srand(time(NULL));
    int level = 1;
    char input[20];
    play_sound();
    print_banner();
    printf("\033[33m__________________________________________________________________________________________________________________________________________________________\n");
    printf("\033[93m 30 sek-d oroltiin test:\n");
    printf("\a");
    if (timed_input(input, 20, 30)) {
        printf("\a");
        printf("\033[92m Ta oruulsan utga: %s\n", input);
        play_sound();
    } else {
        play_sound();
        printf("\033[91m Ta hugatsaand amjsangui.\n");
        play_sound();
    }
    printf("\033[33m_________________________________________________________________________________________________________________________________________________________\n");

    load_record();
    show_rules();

    while (level <= MAX_LEVEL && lives > 0) {
        printf("\a");
        printf("\033[34m_________________________________________________________________________________________________________________________________________________________\n");
        printf("\033[93m \nTuvshin %d | %d ami | Onoo: %d | Record: %d\n", level, lives, score, record);
        printf("\033[91m 30 secound d kharyu oruulakhgui bol togloom zogson \n");
        printf("\033[34m_________________________________________________________________________________________________________________________________________________________\n");
        printf("\a");
        int add = ask_question(level);
        score += add;
        level++;
    }

    printf("\n\033[93m Togloom duuslaa! Tanii onoo: %d\n", score);
    printf("\a");
    if (score > record) {
        printf("\033[92m Ta shine RECORD togtoolo! +50 bonus onoo!\n");
        printf("\a");
        score += 50;
        save_record();
        printf("\033[93m Shine onoo: %d\n", score);
        printf("\a");
    }

    printf("\033[94m Bayartai!\n");
    game_over_banner();
    play_correct_sound();
    return 0;
}
