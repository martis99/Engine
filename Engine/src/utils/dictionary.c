#include "pch.h"

struct dic_entry {
	char* key;
	void* value;
	struct dic_entry* next;
};

static uint hash(Dictionary* dic, const char* key) {
	unsigned long int value = 0;

	for (uint i = 0; i < strlen(key); i++) {
		value = value * 37 + key[i];
	}

	value = value % dic->entry_count;
	return value;
}

Dictionary* dic_create(size_t entry_count, size_t entry_size) {
	Dictionary* dic = m_malloc(sizeof(Dictionary));
	dic->entry_count = entry_count;
	dic->entries = m_calloc(entry_count, sizeof(dic_entry*));
	dic->entry_size = entry_size;
	return dic;
}

void dic_delete(Dictionary* dic, void(*func)(void*)) {
	for (size_t i = 0; i < dic->entry_count; i++) {
		dic_entry* entry = dic->entries[i];

		if (entry == NULL) {
			continue;
		}

		while (entry != NULL) {
			dic_entry* next = entry->next;

			func(entry->value);

			m_free(entry->key, strlen(entry->key) + 1);
			m_free(entry->value, dic->entry_size);
			m_free(entry, sizeof(dic_entry));
			entry = next;
		}
	}

	m_free(dic->entries, dic->entry_count * sizeof(dic_entry*));
	m_free(dic, sizeof(Dictionary));
}

void dic_delete_arg(Dictionary* dic, void(*func)(void*,void*), void* arg) {
	for (size_t i = 0; i < dic->entry_count; i++) {
		dic_entry* entry = dic->entries[i];

		if (entry == NULL) {
			continue;
		}

		while (entry != NULL) {
			dic_entry* next = entry->next;

			func(entry->value, arg);

			m_free(entry->key, strlen(entry->key) + 1);
			m_free(entry->value, dic->entry_size);
			m_free(entry, sizeof(dic_entry));
			entry = next;
		}
	}

	m_free(dic->entries, dic->entry_count * sizeof(dic_entry*));
	m_free(dic, sizeof(Dictionary));
}

dic_entry* dic_pair(const char* key, size_t size) {
	dic_entry* entry = m_malloc(sizeof(dic_entry));
	entry->key = m_malloc(strlen(key) + 1);
	memcpy(entry->key, key, strlen(key) + 1);
	entry->value = m_malloc(size);
	entry->next = NULL;
	return entry;
}

void* dic_add(Dictionary* dic, const char* key) {
	uint slot = hash(dic, key);
	dic_entry* entry = dic->entries[slot];

	if (entry == NULL) {
		dic->entries[slot] = dic_pair(key, dic->entry_size);
		return dic->entries[slot]->value;
	}

	while (entry->next != NULL) {
		if (strcmp(entry->key, key) == 0) {
			printf("Key already exists\n");
			return NULL;
		}
		entry = entry->next;
	}
	entry->next = dic_pair(key, dic->entry_size);
	return entry->next->value;
}

void* dic_get(Dictionary* dic, const char* key) {
	uint slot = hash(dic, key);
	dic_entry* entry = dic->entries[slot];

	if (entry == NULL) {
		return NULL;
	}

	while (entry != NULL) {
		if (strcmp(entry->key, key) == 0) {
			return entry->value;
		}
		entry = entry->next;
	}
	return NULL;
}

void dic_exec(Dictionary* dic, void(*func)(void*)) {
	for (size_t i = 0; i < dic->entry_count; i++) {
		dic_entry* entry = dic->entries[i];

		if (entry == NULL) {
			continue;
		}

		while (entry != NULL) {
			dic_entry* next = entry->next;
			func(entry->value);
			entry = next;
		}
	}
}