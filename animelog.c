#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE 512

typedef struct Anime {
    char title[100];
    char genre[50];
    int favorite;   // 1:お気に入り, 0:他
    char memo[200];
    struct Anime* next;
} Anime;

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
        sscanf(line, "%99[^,],%49[^,],%9[^,],%199[^\n]", new_anime->title, new_anime->genre, fav, new_anime->memo);
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

int main() {
    setlocale(LC_ALL, ""); // 日本語

    const char* filename = "animelist.csv";
    Anime* anime_list = load_csv(filename);

    if (anime_list) {
        display_anime_list(anime_list);
        free_anime_list(anime_list);
    } else {
        printf("データの読み込みに失敗\n");
    }

    return 0;
}
