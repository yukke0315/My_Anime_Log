#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#define MAX_LINE 512

typedef struct Anime {
    char title[100];
    char genre[50];
    int favorite;   // 1:お気に入り, 0:他
    char memo[200];
    char row[10]; // n行
    struct Anime* next;
} Anime;

// 末尾の改行や空白を取り除く関数
void trim_newline(char* str) {
    char* p = str;
    while (*p) {
        if (*p == '\n' || *p == '\r') {
            *p = '\0';
            break;
        }
        p++;
    }
}

// CSVからリストに追加
Anime* load_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("失敗");
        return NULL;
    }

    Anime* head = NULL;
    Anime* tail = NULL;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {
        Anime* new_anime = malloc(sizeof(Anime));
        if (!new_anime) {
            perror("メモリ確保失敗");
            fclose(file);
            return head;
        }

        char fav[10];
        sscanf(line, "%99[^,],%49[^,],%9[^,],%199[^,],%9[^\n]", new_anime->title, new_anime->genre, fav, new_anime->memo, new_anime->row);
        trim_newline(new_anime->row);
        new_anime->favorite = (strcmp(fav, "1") == 0);
        new_anime->next = NULL;

        if (!head) {
            head = tail = new_anime;
        } else {
            tail->next = new_anime;
            tail = new_anime;
        }
    }
    fclose(file);
    return head;
}

// 一覧表示
void display_anime_list(Anime* head) {
    printf("\n--- アニメ一覧 ---\n");
    for (Anime* cur = head; cur != NULL; cur = cur->next) {
        printf("タイトル: %s\nジャンル: %s\nお気に入り: %s\nメモ: %s\n\n",
            cur->title, cur->genre, cur->favorite ? "★" : "-", cur->memo);
    }
}

// メモリ解放
void free_anime_list(Anime* head) {
    while (head) {
        Anime* temp = head;
        head = head->next;
        free(temp);
    }
}
// n行判定
// int is_in_row(const char* title, const wchar_t* row_chars, size_t row_len) {
//     wchar_t wtitle[100];
//     mbstowcs(wtitle, title, 100);

//     for (size_t i = 0; i < row_len; i++) {
//         if (wtitle[0] == row_chars[i]) return 1;
//     }
//     return 0;
// }

// メニュー
void display_menu() {
    printf("\n===== メニュー =====\n");
    printf("1. 全て表示\n");
    printf("2. あ行のみ表示\n");
    printf("3. か行のみ表示\n");
    printf("4. さ行のみ表示\n");
    printf("5. た行のみ表示\n");
    printf("6. な行のみ表示\n");
    printf("7. は行のみ表示\n");
    printf("8. ま行のみ表示\n");
    printf("9. や行のみ表示\n");
    printf("10. ら行のみ表示\n");
    printf("11. わ行のみ表示\n");
    printf("12. 終了\n");
    printf("選択してください: ");
}

// 行表示
// void display_row(Anime* list, const wchar_t* row, size_t row_len, const char* row_name) {
//     printf("\n--- %s行 ---\n", row_name);
//     for (Anime* cur = list; cur != NULL; cur = cur->next) {
//         if (is_in_row(cur->title, row, row_len)) {
//             printf("タイトル: %s\nジャンル: %s\nお気に入り: %s\nメモ: %s\n\n",
//                 cur->title, cur->genre, cur->favorite ? "★" : "-", cur->memo);
//         }
//     }
// }
void display_row(Anime* head, const char* target_row) {
    printf("\n--- %s行 ---\n", target_row);
    int found = 0;
    for (Anime* cur = head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->row, target_row) == 0) {
            printf("タイトル: %s\nジャンル: %s\nお気に入り: %s\nメモ: %s\n\n",
                   cur->title, cur->genre, cur->favorite ? "★" : "-", cur->memo);
            found = 1;
        }
    }
    if (!found) {
        printf("該当するアニメが見つかりませんでした。\n");
    }
}

int main() {
    setlocale(LC_ALL, ""); // 日本語

    const char* filename = "animelist.csv";
    Anime* anime_list = load_csv(filename);

    if (!anime_list) {
        printf("データの読み込みに失敗\n");
        return 1;
    }

    // 日本語行の文字配列
    // wchar_t A[] = {L'あ',L'い',L'う',L'え',L'お',L'ア',L'イ',L'ウ',L'エ',L'オ'};
    // wchar_t K[] = {L'か',L'き',L'く',L'け',L'こ',L'カ',L'キ',L'ク',L'ケ',L'コ'};
    // wchar_t S[] = {L'さ',L'し',L'す',L'せ',L'そ',L'サ',L'シ',L'ス',L'セ',L'ソ'};
    // wchar_t T[] = {L'た',L'ち',L'つ',L'て',L'と',L'タ',L'チ',L'ツ',L'テ',L'ト'};
    // wchar_t N[] = {L'な',L'に',L'ぬ',L'ね',L'の',L'ナ',L'ニ',L'ヌ',L'ネ',L'ノ'};
    // wchar_t H[] = {L'は',L'ひ',L'ふ',L'へ',L'ほ',L'ハ',L'ヒ',L'フ',L'ヘ',L'ホ'};
    // wchar_t M[] = {L'ま',L'み',L'む',L'め',L'も',L'マ',L'ミ',L'ム',L'メ',L'モ'};
    // wchar_t Y[] = {L'や',L'ゆ',L'よ',L'ヤ',L'ユ',L'ヨ'};
    // wchar_t R[] = {L'ら',L'り',L'る',L'れ',L'ろ',L'ラ',L'リ',L'ル',L'レ',L'ロ'};
    // wchar_t W[] = {L'わ',L'を',L'ん',L'ワ',L'ヲ',L'ン'};


    int choice;

    do {
        display_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                display_anime_list(anime_list);
                break;
            case 2:
                display_row(anime_list, "あ");
                break;
            case 3:
                display_row(anime_list, "か"); 
                break;
            case 4:
                display_row(anime_list, "さ"); 
                break;
            case 5:
                display_row(anime_list, "た"); 
                break;
            case 6:
                display_row(anime_list, "な"); 
                break;
            case 7:
                display_row(anime_list, "は"); 
                break;
            case 8:
                display_row(anime_list, "ま"); 
                break;
            case 9: 
                display_row(anime_list, "や"); 
                break;
            case 10: 
                display_row(anime_list, "ら"); 
                break;
            case 11: 
                display_row(anime_list, "わ"); 
                break;
            case 12: 
                printf("終了\n"); 
                break;
            default: 
                printf("無効な選択肢\n"); 
                break;
        }
    } while (choice != 12);

    return 0;
}
