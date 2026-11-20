#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entry {
  char *key;
  int value;
  struct Entry *next;
} Entry;

typedef struct HashMap {
  Entry **buckets;
  size_t capacity;
  size_t size;
} HashMap;

// djb2 hash function
static unsigned long hash_str(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + (unsigned char)c;
  }
  return hash;
}

HashMap *hm_create(size_t capacity) {
  HashMap *map = malloc(sizeof(HashMap));
  if (!map)
    return NULL;

  map->capacity = capacity > 0 ? capacity : 16;
  map->size = 0;
  map->buckets = calloc(map->capacity, sizeof(Entry *));
  if (!map->buckets) {
    free(map);
    return NULL;
  }
  return map;
}

int hm_put(HashMap *map, const char *key, int value) {
  size_t idx = hash_str(key) % map->capacity;
  Entry *curr = map->buckets[idx];

  // Update existing key if present
  while (curr) {
    if (strcmp(curr->key, key) == 0) {
      curr->value = value;
      return 1;
    }
    curr = curr->next;
  }

  // Otherwise allocate and prepend a new entry
  Entry *entry = malloc(sizeof(Entry));
  if (!entry)
    return 0;

  entry->key = malloc(strlen(key) + 1);
  if (!entry->key) {
    free(entry);
    return 0;
  }
  strcpy(entry->key, key);
  entry->value = value;
  entry->next = map->buckets[idx];
  map->buckets[idx] = entry;
  map->size++;
  return 1;
}

int *hm_get(const HashMap *map, const char *key) {
  size_t idx = hash_str(key) % map->capacity;
  Entry *curr = map->buckets[idx];

  while (curr) {
    if (strcmp(curr->key, key) == 0) {
      return &curr->value;
    }
    curr = curr->next;
  }
  return NULL;
}

void hm_free(HashMap *map) {
  if (!map)
    return;
  for (size_t i = 0; i < map->capacity; i++) {
    Entry *curr = map->buckets[i];
    while (curr) {
      Entry *next = curr->next;
      free(curr->key);
      free(curr);
      curr = next;
    }
  }
  free(map->buckets);
  free(map);
}

HashMap *nouvelle_table(void) {
  HashMap *table = hm_create(676);
  if (!table) {
    return NULL;
  }
  char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      char cle[3] = {alphabet[i], alphabet[j], '\0'};
      hm_put(table, cle, alphabet[(i + j) % 26]);
    }
  }
  return table;
}

void encode(char *texte_clair, char *cle, int size) {
  if (!texte_clair || !cle || size <= 0) {
    return;
  }

  size_t cle_len = strlen(cle);
  if (cle_len == 0) {
    return;
  }

  HashMap *table = nouvelle_table();
  if (!table) {
    return;
  }

  size_t cle_idx = 0;
  for (int i = 0; i < size; i++) {
    char c = texte_clair[i];
    int is_upper = (c >= 'A' && c <= 'Z');
    char c_lower = is_upper ? (char)(c + ('a' - 'A')) : c;

    if (c_lower >= 'a' && c_lower <= 'z') {
      char k = cle[cle_idx % cle_len];
      if (k >= 'A' && k <= 'Z') {
        k = (char)(k + ('a' - 'A'));
      }

      char pair[3] = {c_lower, k, '\0'};
      int *val = hm_get(table, pair);
      if (val) {
        texte_clair[i] = is_upper ? (char)(*val - ('a' - 'A')) : (char)*val;
      }
      cle_idx++;
    }
  }

  hm_free(table);
}

void decode(char *texte, char *cle, int size) {
  if (!texte || !cle || size <= 0) {
    return;
  }

  size_t cle_len = strlen(cle);
  if (cle_len == 0) {
    return;
  }

  HashMap *table = nouvelle_table();
  if (!table) {
    return;
  }

  size_t cle_idx = 0;
  for (int i = 0; i < size; i++) {
    char c = texte[i];
    int is_upper = (c >= 'A' && c <= 'Z');
    char c_lower = is_upper ? (char)(c + ('a' - 'A')) : c;

    if (c_lower >= 'a' && c_lower <= 'z') {
      char k = cle[cle_idx % cle_len];
      if (k >= 'A' && k <= 'Z') {
        k = (char)(k + ('a' - 'A'));
      }

      for (char p = 'a'; p <= 'z'; p++) {
        char pair[3] = {p, k, '\0'};
        int *val = hm_get(table, pair);
        if (val && *val == c_lower) {
          texte[i] = is_upper ? (char)(p - ('a' - 'A')) : p;
          break;
        }
      }
      cle_idx++;
    }
  }

  hm_free(table);
}

int main(void) {
  char message[] = "Hello, World! Vigenere Cipher avec HashMap.";
  char cle[] = "caca";
  int size = (int)strlen(message);

  printf("Original:  %s\n", message);
  encode(message, cle, size);
  printf("Chiffre:   %s\n", message);
  decode(message, cle, size);
  printf("Dechiffre: %s\n", message);

  return 0;
}
